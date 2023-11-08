// 10727127 邱佳駿 10727152 黃景泓 
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>
#include<fstream>
#include<sstream>
#include<iostream>
#include<time.h>
#include<math.h>
#include<stack> 
#define COLUMNS	6
#define MAX_LEN 10
#define BIG_INT 255 
using namespace std ;
fstream binfile ;
fstream txtfile ;

struct student {
	char sid[10] ;
	char sname[10] ;
	unsigned char score[6] ;
	float mean ;		
};
struct hashData{
	int key ;
	int hvalue ; 
	int listvalue;
	bool hasdata ;
};
void PrintCommand() {
	cout <<  endl << "************  Hash Table  ************" << endl <<
					 "***                               ***" << endl <<
			         "* 0. 離開                           *" << endl <<
    		         "* 1. Quadratic probing              *" << endl <<
			         "* 2. Double hashing                 *" << endl << 
			         "*************************************" << endl << 
                     "請輸入指令( 0, 1, 2 ): " ; 
} // PrintCommand

class Hash{
	int hashsize ;
	vector<hashData> hashtable ;
	vector<hashData> overflow ;
	vector<student> student_list ;
	bool LoadBin( string fileName ) {  // load file        
		fileName = "input" + fileName + ".bin" ; 
		binfile.open(fileName.c_str(), fstream::in | fstream::binary) ;  
		if ( binfile == NULL ) {
			cout << endl << "### " << fileName << " does not exist !! " << " ###"<< endl ;
			binfile.close() ;
			return false ;
		} // if
	
		else {
			binfile.close() ;
			return true ;
		} // else
		
	} // LoadBin() 
	
	bool LoadTxt( string fileName ) {  // load file        
		fileName = "input" + fileName + ".txt" ; 
		txtfile.open( fileName.c_str(), ios::in ) ;  
		if ( txtfile == NULL ) {
			cout << endl << "### " << fileName << " does not exist !! " << " ###" << endl ;
			txtfile.close() ;
			return false ;
		} // if
		
		else {
			return true ;
		} // else
	} // LoadTxt() 
	
	void TxtToBin(string fileName) {
		fileName = "input" + fileName + ".bin" ;
		int stNo = 0 ;
		binfile.open(fileName.c_str(), fstream::out | fstream::binary) ;
		if (binfile.is_open()) {
			char rBuf[BIG_INT] ;
			while ( txtfile.getline(rBuf, BIG_INT, '\n') ) {
				string temp ;
				student oneSt ;
				int cNo = 0, pre = 0, pos = 0 ;
				stNo++ ;
				temp.assign(rBuf) ;
				pos = temp.find_first_of('\t', pre) ;
				while ( pos!= string::npos ) {
					switch (++cNo) {
						case 1 : strcpy(oneSt.sid, temp.substr(pre, pos - pre).c_str() ) ;
							break ;
						case 2 : strcpy(oneSt.sname, temp.substr(pre, pos - pre).c_str() ) ;
							break ;
						default : oneSt.score[cNo - 3] = atoi(temp.substr(pre, pos - pre).c_str() ) ;
					} // switch
					
					pre = ++pos ;
					pos = temp.find_first_of('\t', pre ) ;
				} // while
				
				pos = temp.find_last_of('\t') ;
				oneSt.mean = atof( temp.substr(pos + 1 ).c_str() ) ;
				binfile.write((char*)&oneSt, sizeof(oneSt)) ;
			} // while
			
		} // if
		
		txtfile.close() ;
		binfile.close() ;
	} // TxttoBin()
	
	void Prime(int &num) { // 找值數 
		int i = num-1;
		while( i > 0 ) {				
			if( num%i == 0 )
				break;
			i = i - 1;
		} // while
	
		if( i != 1 ) {
			num = num + 1;
			Prime( num );
		} // if
	} // Prime()
	
	void LoadInVector(string fileName ) {
		fileName = "input" + fileName + ".bin" ; 
		student oneSt ;
		int stNo = 0 ;
		binfile.open(fileName.c_str(), fstream::in | fstream::binary) ;
		if ( binfile.is_open() ) {
			binfile.seekg(0, binfile.end ) ;
			stNo = binfile.tellg() / sizeof( oneSt ) ;
			int num =  stNo * 1.2 + 1 ; // 不包含自己的質數 
			Prime(num) ; // set hashSize
			hashsize = num ;
			binfile.seekg(0, binfile.beg) ;
			for ( int i = 0 ; i < stNo ; i++ ) { // 把所有學生資料存在vector 
				binfile.read((char*)&oneSt,sizeof(oneSt) ) ;
				student_list.push_back(oneSt) ;
			} // for
		} // if
		
		binfile.close() ;
	} // LoadInVector()
	
	void SetHashtable() {
		vector<hashData> temp(hashsize) ;
		hashtable = temp ;
		for ( int i = 0 ; i < hashtable.size() ; i++ ) {
			hashtable[i].hvalue = 0 ;
			hashtable[i].listvalue = -1 ;
			hashtable[i].hasdata = false ;
		} // for
	} // SetStudentlistSize()
	
	void  QuadraticCollision(int key, hashData &temp, int i) { // 碰撞 
		temp.hvalue++ ;
		if (hashtable[( key + i * i ) % hashsize].hasdata) { 
			i++ ;
			QuadraticCollision(key, temp, i) ;
		} // if
		else 
			hashtable[( key + i * i ) % hashsize] = temp ;
	} //  QuadraticCollision()
	
	void  DoubleHashCollision(int key, hashData &temp, int stepsize) { // 碰撞 
		temp.hvalue++ ;
		if (hashtable[( key + stepsize) % hashsize].hasdata) { 
			DoubleHashCollision(key + stepsize, temp, stepsize) ;
		} // if
		else 
			hashtable[( key + stepsize ) % hashsize] = temp ;
	} //  DoubleHashCollision()
	
	void QuadraticHashtable() {
		for ( int i = 0 ; i < student_list.size() ; i++ ) {
			int key = 1 ;
			hashData temp ;
			temp.hvalue = 1 ;
			temp.listvalue = i ;
			temp.hasdata = true ;
			for ( int j = 0 ; student_list[i].sid[j] != '\0' ; j ++) { // search oneSt key
				key *= student_list[i].sid[j] ; 
				key = key % hashsize ; 
			} // for
			
			temp.key = key ;
			if ( hashtable[key].hasdata) // have data in this location ==> Collision
				 QuadraticCollision( key, temp, 1) ;
			else // 直接放入 
				hashtable[key] = temp ;
				
		} // for
	} // QuadraticHashtable()
	void PrintUnsuccess() {
		float total = 0 ;
		float num ;
		for ( int i = 0 ; i < hashtable.size() ; i++) {
			if ( hashtable[i].hasdata) 
				for ( int j = 0 ; hashtable[(i + j * j) % hashsize].hasdata ; j++ )  // 加上偏移量去尋找 
					total++ ;
		} // for
		
		num = total / hashsize ;
		cout << "unsuccessful search: " ;
		printf("%.4f", num) ; 
		 cout << " comparisons on average" << endl ;
	} // PrintUnsuccess()
	
	void PrintSuccess() {
		float total = 0 ;
		float num ;
		for ( int i = 0 ; i < hashtable.size() ; i++) 
			if ( hashtable[i].hasdata)
				total += hashtable[i].hvalue  ;
		num = total / student_list.size() ;
		cout << "successful search: "  ;
		printf("%.4f", num) ;
		 cout << " comparisons on average" << endl ;
	} // PrintSuccess()
	
	void PrintQuadratic(string fileName) {
		fileName = "quadratic" + fileName + ".txt" ;
		txtfile.open(fileName.c_str(), ios::out) ;
		txtfile << " --- Hash table created by Quadratic probing ---\n" ;
		for ( int i = 0 ; i < hashtable.size() ; i++ ) {
			txtfile << "[" << i << "]\t" ;
			if ( hashtable[i].hasdata) {
				txtfile << hashtable[i].key << ",\t" << student_list[hashtable[i].listvalue].sid << ",\t" << 
				student_list[hashtable[i].listvalue].sname << ",\t" << student_list[hashtable[i].listvalue].mean ;
			} // if
			
			txtfile << "\n" ;
		} // for
		
		txtfile << " ----------------------------------------------------- " ;
		txtfile.close() ;
		cout<< endl << "Hash table has been successfully created by Quadratic probing" << endl ;
		PrintUnsuccess() ; 
		PrintSuccess() ;
	} // PrintQuadratic()
	
	void PrintDoubleHash(string fileName) {
		fileName = "double" + fileName + ".txt" ;
		txtfile.open(fileName.c_str(), ios::out) ;
		txtfile << " --- Hash table created by Double hashing ---\n" ;
		for ( int i = 0 ; i < hashtable.size() ; i++ ) {
			txtfile << "[" << i << "]\t" ;
			if ( hashtable[i].hasdata) {
				txtfile << hashtable[i].key << ",\t" << student_list[hashtable[i].listvalue].sid << ",\t" << 
				student_list[hashtable[i].listvalue].sname << ",\t" << student_list[hashtable[i].listvalue].mean ;
			} // if
			
			txtfile << "\n" ;
		} // for
		
		txtfile << " ----------------------------------------------------- " ;
		txtfile.close() ;
		cout<< endl << "Hash table has been successfully created by Quadratic probing" << endl ;
		PrintSuccess() ;
	} // PrintDoubleHash()
	
	void DoubleHashtable() {
		int num = ( student_list.size() / 3 ) + 1 ; // 最高步階 
		Prime(num) ;
		
		for ( int i = 0 ; i < student_list.size() ; i++ ) {
			int key = 1 ;
			int stepsize = 1 ;
			hashData temp ;
			temp.hvalue = 1 ;
			temp.listvalue = i ;
			temp.hasdata = true ;
			for ( int j = 0 ; student_list[i].sid[j] != '\0' ; j ++) { // search oneSt key
				key *= student_list[i].sid[j] ; 
				key = key % hashsize ; 
				stepsize *= student_list[i].sid[j] ; 
				stepsize = stepsize % num ;
			} // for
			
			stepsize = num - stepsize ;
			temp.key = key ;
			if ( hashtable[key].hasdata) // have data in this location ==> Collision
				 DoubleHashCollision( key, temp, stepsize) ;
			else // 直接放入 
				hashtable[key] = temp ;
				
		} // for
	} // DoubleHashtable()
	
	public :
		bool LoadFile(string fileName) {
		    if ( LoadBin(fileName) )  // .bin file is exist
				return true ;
			else { // .bin is not exist， insure .txt is exist
				if ( LoadTxt(fileName) ) { // .txt is exist ， need transfer to .bin 
					TxtToBin(fileName) ;
					return true ;
				} // if
				
				else // .txt dose not exist
					return false ;
			} // else	
		} // LoadFile()
		
		void Quadratic(string fileName) { 
			LoadInVector(fileName) ;
			SetHashtable() ;
			QuadraticHashtable() ;
			PrintQuadratic(fileName) ;
		} // Quadratic()
		
		void DoubleHash(string fileName) {
			SetHashtable() ;
			DoubleHashtable() ;
			PrintDoubleHash(fileName) ;
		} // DoubleHash()
		
		void Set(int tempsize, vector<hashData> temphash, vector<student> templist ) {
			hashsize = tempsize ;
			hashtable = temphash;
			student_list = templist ;
		} // Set()
		
		int Gethashsize() {
			return hashsize ;
		} // Gethashsize()
		
		vector<hashData> Gethash() {
			return hashtable ;
		} // Gethash()
		
		vector<student> Getlist() {
			return student_list ;
		} // Getlist()
};

int main() {
	string com ="" ;
	int hashsize ;
	vector<hashData> hashtable ;
	vector<student> student_list ;
	bool haveData = false ;
	string fileName ;
	while (1) {
		PrintCommand() ;
		cin >> com ;
		if ( com == "0")
			exit(0) ;
		else if ( com == "1" ) {
			Hash hash ;
			cout << "輸入要執行的檔案 : " ; 
		    cin >> fileName ;
			if ( hash.LoadFile(fileName) ) 
				hash.Quadratic(fileName) ;
			haveData = true ;
			
			hashsize = hash.Gethashsize() ;
			hashtable = hash.Gethash() ;
			student_list = hash.Getlist() ;
		} // else if
		
		else if ( com == "2" ) {
			if ( haveData == false ) 
        		cout << "Please enter command 1 first!\n" ;
        	else {
        		Hash hash ;
        		hash.Set(hashsize, hashtable, student_list) ;
        		hash.DoubleHash(fileName) ;
			} // else
		} // else if
		
		else 
			cout << "指令不存在,請重新輸入" << endl ;
	} // while
} // main()
