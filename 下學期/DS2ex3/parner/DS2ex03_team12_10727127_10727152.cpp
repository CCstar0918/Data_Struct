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
#include<iomanip>
#define COLUMNS 6
#define	MAX_LEN 10
#define BIG_INT 255
using namespace std ;


struct sT {           
	char sid[MAX_LEN];         		// 存學號 
	char sname[MAX_LEN];			// 存名字 
	unsigned char score[COLUMNS];	// 存六個分數 
	float mean;						// 存平均分數 
};

struct hT {
	bool open;               		// 用於確認櫃子是否存放東西 
	char sid[MAX_LEN];				
	char sname[MAX_LEN];
	float mean;
	int hValue;                  	// 存放雜湊值 
};

void PrintCommand() {
	cout <<  endl << "********  Hash Table         ********" << endl <<
					 "***                               ***" << endl <<
			         "* 0. 離開                           *" << endl <<
    		         "* 1. quadratic probing              *" << endl <<
			         "* 2. Double hashing                 *" << endl << 
			         "*************************************" << endl << 
                     "請輸入指令( 0, 1, 2 ): " ; 
} // PrintCommand

class Hash {


	void FindPrimeWithNum( int &num ) {	// 傳入一個數字，找尋比他大的質數 
		int i = num-1;
		while( i > 0 ) {				
			if( num%i == 0 )
				break;
			
			i = i - 1;
		
		} // while
	
		if( i != 1 ) {
			num = num + 1;
			FindPrimeWithNum( num );
		} // if
	} // FindPrimeWithNum()

	int FindKey( char sid[], int size ) {	// 利用傳入的學號 算出雜湊值，亦即應該所在的櫃子號碼 
	
		int i = 0;
		unsigned long long int key = 1;
		while ( i < strlen( sid ) ) {
		
			key = key * sid[i];
			i++;
				
		} // while 
	
		key = key%size;
		return key;	
	} // FindKey()
	
	int Step( char sid[], int dataNum ) {	// 步階函數，當發生碰撞時，利用此函數算出步階 
		int i = 0;
		int num = ( dataNum / 3) + 1;
		FindPrimeWithNum( num ); // num為大於資料筆數三分之一的質數
		unsigned long long int sum = 1;
		while ( i < strlen( sid ) ) {
		
			sum = sum * sid[i];
			i++;
				
		} // while 
		
		return ( num - ( sum%num ) );
	} // Step

	void DoQuadraticHash( sT oneSt, hT hashArray[], int size ) {	 // 進行雜湊，當發生碰撞時，利用平方法找尋新位置	
		int key = FindKey( oneSt.sid, size );
	
		if( hashArray[key].open == false ) {    // 表示櫃子還沒人開過，直接放進櫃子裡 			
			strcpy( hashArray[key].sid, oneSt.sid );
			strcpy( hashArray[key].sname, oneSt.sname );
			hashArray[key].mean = oneSt.mean;
			hashArray[key].hValue = key;
			hashArray[key].open = true;
		} // if
		else {   // 有人使用櫃子利用平方法 
			
			int i = 1;
			unsigned long long int newKey; // 記錄新的位置 
			while(true) {
			
				unsigned long long int square = i * i; 
				newKey = key + square;     // 新的位置是原來在的位置去加平方值 
				newKey = newKey % size;    // 如果超過櫃子數大小，就用取餘數方式 
				if ( hashArray[newKey].open == false  ) {   // 找到沒開過的放進去 
					strcpy( hashArray[newKey].sid, oneSt.sid );
					strcpy( hashArray[newKey].sname, oneSt.sname );
					hashArray[newKey].mean = oneSt.mean;
					hashArray[newKey].hValue = key;
					hashArray[newKey].open = true;	
					break;			
				} // if
			
				i++;     // 平方的數字加1 
			
			} // while
			
		} // else 
	
	} // doHash
	
	void DoDoubleHash( sT oneSt, hT hashArray[], int size, int dataNum  ) {  // 進行雜湊，當發生碰撞時，利用步階函數找尋新位置 
		int key = FindKey( oneSt.sid, size );
		
		if( hashArray[key].open == false ) {    // 表示櫃子還沒人開過 			
			strcpy( hashArray[key].sid, oneSt.sid );
			strcpy( hashArray[key].sname, oneSt.sname );
			hashArray[key].mean = oneSt.mean;
			hashArray[key].hValue = key;
			hashArray[key].open = true;
		} // if
		else {   // 有人使用櫃子利用另一個hash 
			int h2 =  Step( oneSt.sid, dataNum );     // 找出步階 
			int newKey = (key+h2)%size;        		   
			while( hashArray[newKey].open == true ) {  // 空的櫃子找到前 ，用同樣的步階一直走  			
				newKey = (newKey+h2)%size;
			} // while
			
			strcpy( hashArray[newKey].sid, oneSt.sid );
			strcpy( hashArray[newKey].sname, oneSt.sname );
			hashArray[newKey].mean = oneSt.mean;
			hashArray[newKey].hValue = key;
			hashArray[newKey].open = true;						
						
		} // else 
			
	} // 
	
	void PrintQuadraticScreen( hT hashArray[], int size ) {     // 印出平方法顯示在螢幕上的資訊 
		int i = 0 ; 
		float searchNoValue = 0;
		while (  i < size ) {
			
			int walk = 0;

			unsigned long long int newKey = i ;   // 用於紀錄新位置 
			int k = 1;
			while( hashArray[newKey].open == true ) {   // 直到找到空的櫃子前 紀錄比較次數 
				unsigned long long int square = k * k;
				k++;
				newKey = i + square;
				newKey = newKey%size;
				walk++;	      // 有值比較次數+1			
			} // while
			
			searchNoValue = searchNoValue + walk;      // 把每個櫃子搜尋不存在值相加 
			i++;
		} // while
		
		cout << "Hash table has been successfully created by Quadratic probing\n" ;
		cout << "unsuccessful search: ";
		printf( "%.4f", (searchNoValue / size) ) ;		// 除以櫃子數 
		cout << " comparisons on average\n";
		
		i = 0;
		int dataNum = 0;
		while ( i < size ) {						 // 算出總資料筆數，用於計算搜尋存在值
			if( hashArray[i].open == true )
				dataNum++;			
			i++;
		} // while
		
		i = 0;
		float searchValue = 0;
		while (  i < size ) {						// 搜尋這筆資料在找到前所比較次數 
			
			int walk = 0;
			if( hashArray[i].open == true ) {    
				int j = hashArray[i].hValue;
				int k = 1;
				int newJ = j;

				while( strcmp(hashArray[i].sid, hashArray[newJ].sid) != 0 ) {
					int square = k * k;
					k++;
					newJ = j + square;
					newJ = newJ%size;
					walk++;					
				} // while	
				
				walk = walk + 1;			
			} // if
			
			searchValue = searchValue + walk;			// 把每筆現存值搜尋到的比較次數相加 
			i++;
		} // while		
 
		cout << "successful search: ";
		printf( "%.4f", ( searchValue / dataNum) ) ;	// 除以資料筆數
		cout << " comparisons on average" << endl ;
		
	} // PrintScreen()
	
	void PrintDoubleHashScreen( hT hashArray[], int size, int dataNum ) {	// 印出Double hash顯示在螢幕上的資訊 
		
		cout << "Hash table has been successfully created by Double hashing\n" ;
		

		
		int i = 0;
		float searchValue = 0;
		while (  i < size ) {
			
			int walk = 0;
			if( hashArray[i].open == true ) {    	// 搜尋這筆資料在找到前所比較次數 
				int j = hashArray[i].hValue;
				int h2 = Step( hashArray[i].sid, dataNum );
				int newJ = j ;

				while( strcmp(hashArray[i].sid, hashArray[newJ].sid) != 0 ) {


					newJ = (newJ+h2)%size ;
					walk++;					
				} // while	
				
				walk = walk + 1;			
			} // if
			
			searchValue = searchValue + walk;
			i++;
		} // while		

 
		cout << "successful search: ";
		printf( "%.4f", (searchValue / dataNum) ) ;
		cout << " comparisons on average" << endl ;
		
	} // PrintScreen()
	
    void PrintQuadraticFile( hT hashArray[], int size, string fileName )	 {	//  將平方法的結果印成txt檔 
    	ofstream outFile;
    	fileName = "quadratic" + fileName + ".txt";
    	outFile.open( fileName.c_str(), ios::out );
    	int i = 0;
    	while( i < size ) {
    		if( i == 0 ) 
    			outFile << "--- Hash table created by Quadratic probing ---" << endl;

			if ( hashArray[i].open == true ) {
    			outFile << "[" << i << "] " << '\t' << hashArray[i].hValue << "," << '\t';
				outFile << hashArray[i].sid << "," << '\t'<< hashArray[i].sname << ","<< '\t' << hashArray[i].mean << endl;
			} // if
			else {
				outFile << "[" << i << "] " << endl;
			}

			i++;
		} // while
		outFile << "----------------------------------------------------- ";
		outFile.close();
    	
	} // PrintFile()
	
    void PrintDoubleHashFile( hT hashArray[], int size, string fileName )	{    //  將Double hash的結果印成txt檔  
    	ofstream outFile;
    	fileName = "double" + fileName + ".txt";
    	outFile.open( fileName.c_str(), ios::out );
    	int i = 0;
    	while( i < size ) {
    		if( i == 0 ) 
    			outFile << "--- Hash table created by Double hashing ---" << endl;

			if ( hashArray[i].open == true ) {
    			outFile << "[" << i << "] " << '\t' << hashArray[i].hValue << "," << '\t';
				outFile << hashArray[i].sid << "," << '\t'<< hashArray[i].sname << ","<< '\t' << hashArray[i].mean << endl;
			} // if
			else {
				outFile << "[" << i << "] " << endl;
			}

			i++;
		} // while
		outFile << "----------------------------------------------------- ";
		outFile.close();
    	
	} // PrintFile()	
	
	public:
	
	bool ReadFile( string &fileName, int &dataNum ) {     // 用於讀取檔案及判斷是否成功讀取檔案 
			
		cout << "Input a file number: ";
		cin >> fileName;
		if( !Quadratic( fileName, dataNum ) ) {       // 判斷有無bin檔 ，沒有則讀取TXT檔，並轉成bin檔 
			cout << "input" + fileName + ".bin does not exist!\n" ;
			if( !Text2Binary(fileName) ) {     		// 如果連TXT檔都沒有就是讀取失敗 
				cout << "input" + fileName + ".txt does not exist!\n" ;
				return false;
			} // if
			else 
				Quadratic( fileName, dataNum );			
		} // if
				
		return true;
						
	} // Read()

	bool Text2Binary( string &fileName ) {
	
		fstream inFile, outFile;
		int stNo = 0;	

		inFile.open( ( "input" + fileName + ".txt").c_str(), fstream::in );
		if( inFile.is_open() == false )
			return false;
			
		string binFileName = "input" + fileName + ".bin";
		outFile.open( binFileName.c_str(), fstream::out | fstream::binary );
		if( outFile.is_open() ) {
			char rBuf[BIG_INT];
			while( inFile.getline(rBuf, BIG_INT, '\n') ) {
				string temp;
				sT oneSt;
				int cNo = 0, pre = 0, pos = 0;
			
				stNo++;
				temp.assign(rBuf);
				pos = temp.find_first_of( '\t', pre );
			
				while( pos != string::npos ) {
					switch(++cNo) {
						case 1: strcpy( oneSt.sid, temp.substr(pre,pos-pre).c_str() );
							break;
						case 2: strcpy( oneSt.sname, temp.substr(pre,pos-pre).c_str() );
							break;
						default: oneSt.score[cNo-3] = atoi(temp.substr(pre,pos-pre).c_str()) ;
							break;
					} // switch
				
					pre = ++pos;
					pos = temp.find_first_of( '\t', pre );
				
				} // while
			 
				pos = temp.find_last_of('\t');
				oneSt.mean = atof( temp.substr(pos+1).c_str() );
				outFile.write((char*)&oneSt, sizeof(oneSt));
			
			} // while
		
			outFile.close();
		} // if
	
		inFile.close();
		return true;
	
	} // Text2Binary()

	bool Quadratic( string fileName, int &dataNum ) {     // 將bin檔讀入 ，並執行平方法的雜湊 
		
		string binFileName = "input" + fileName + ".bin";
		fstream binFile;
		sT oneSt;
		int stNo = 0;
	
		binFile.open( binFileName.c_str(), fstream::in | fstream::binary );
		if( binFile.is_open() ) {
			binFile.seekg(0,binFile.end);
			stNo = binFile.tellg() / sizeof(oneSt);
		
			float num = stNo*1.2;
			int arraySize = 0;
			while ( arraySize < num ) {      
				arraySize++;
			} // while
			
			FindPrimeWithNum( arraySize );   // 比資料總數1.2倍大的質數
			hT hashArray[arraySize];         // 用於雜湊的櫃子 
			
			int i = 0;
			while ( i < arraySize ) {
				hashArray[i].open = false;
				i++;	
			} // while
			binFile.seekg(0,binFile.beg);
			for( int i = 0; i < stNo; i++) {
				binFile.read((char*)&oneSt, sizeof(oneSt));
				DoQuadraticHash( oneSt, hashArray, arraySize );    // 一筆一筆資料讀入進行雜湊 

			} // for
			
			dataNum = stNo;
			if( dataNum != 0 )                                   // 如果讀入的檔案資料筆數是0 就不用印在螢幕上了 
				PrintQuadraticScreen(hashArray, arraySize); 
				
			PrintQuadraticFile(hashArray, arraySize, fileName);
			binFile.close();
			
			return true;
		} // if
		else 
			return false;
	} // Quadratic()
	
	void DoubleHash( string fileName, int dataNum ) {		// 將bin檔讀入 ，並執行Double hash的雜湊 
		string binFileName = "input" + fileName + ".bin";
		fstream binFile;
		sT oneSt;
		int stNo = 0;
	
		binFile.open( binFileName.c_str(), fstream::in | fstream::binary );
		if( binFile.is_open() ) {
			binFile.seekg(0,binFile.end);
			stNo = binFile.tellg() / sizeof(oneSt);
		
			int arraySize = stNo*1.2;
			arraySize++; //這邊加1 因為要大於這個數字的質數 就算他自己是質數也不行 
			FindPrimeWithNum( arraySize );   
			hT hashArray[arraySize];
			
			int i = 0;
			while ( i < arraySize ) {
				hashArray[i].open = false;
				i++;	
			} // while
			binFile.seekg(0,binFile.beg);
			for( int i = 0; i < stNo; i++) {
				binFile.read((char*)&oneSt, sizeof(oneSt));
				DoDoubleHash( oneSt, hashArray, arraySize, dataNum );

			} // for
			dataNum = stNo;
			if ( dataNum != 0 )											// 如果讀入的檔案資料筆數是0 就不用印在螢幕上了 
				PrintDoubleHashScreen(hashArray, arraySize, dataNum); 
				
			PrintDoubleHashFile(hashArray, arraySize, fileName);
			
			binFile.close();
		} // if
	} // DoubleHash()

}; // class Hash



int main() {
	string com = "" ; // 接收指令 
    string fileName;
    int dataNum;
	bool haveData = false;
 	while ( 1 ) {                            
		PrintCommand() ;
      	cin >> com ;
      	if ( com.compare( "0" ) == 0 ) {
      		exit(0) ;
		} // if
        	
        else if ( com == "1") { 
			Hash aHash ;
        	if( aHash.ReadFile( fileName, dataNum ) == true )
        		haveData = true;
        	else 
        		haveData = false;
        } // else if
        
        else if ( com == "2" ) { 
        	if ( haveData == false ) 
        		cout << "Please enter command 1 first!\n" ; 
			else {
				Hash aHash;
				aHash.DoubleHash( fileName, dataNum );
			} // else

		} // else if		
		else {
			cout << "指令不存在,請重新輸入" << endl;
		}
	} // while 
} // main()
