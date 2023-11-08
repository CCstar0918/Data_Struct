// 10727127 邱佳駿 10727152 黃景泓 
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<time.h>
#include<math.h>
#include<queue>
#include<stack> 
#define COLUMNS	6
#define MAX_LEN 10
#define BIG_INT 255 
using namespace std ;
fstream binfile ;
fstream adjfile ;
fstream cntfile ;
struct studentPair {
	char sid1[MAX_LEN] ; 	// sender
	char sid2[MAX_LEN] ; 	// receiver
	float weight ;			// pair weight
};

struct node {
	string sid2 ;
	float weight ;
};

struct list {
	string sid1 ;
	vector<node> receiver ;	
};

struct message {
	string sid1 ;
	vector<string> visit ;
};

struct hashdata{
	string id ;
	int loc ;
	bool hasvisit ;
	bool hasdata ;
};

void PrintCommand() {
	cout <<  endl << "************  Hash Table  ************" << endl <<
					 "***                               ***" << endl <<
			         "* 0. 離開                           *" << endl <<
    		         "* 1. Build adjacency lists          *" << endl <<
			         "* 2. Compute connection counts      *" << endl << 
			         "* 3. Estimate influence value       *" << endl <<
			         "*************************************" << endl << 
                     "請輸入指令( 0, 1, 2, 3 ): " ; 
} // PrintCommand

class Graph {
	vector<studentPair> student_list ;
	vector<list> adjL ;
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
	
	void SortForID( vector<string> &temp, int first, int last ) {
		int pivotInt ;
		if ( first < last) {
			partitionForID( temp, first, last, pivotInt ) ;
			SortForID( temp, first, pivotInt - 1 ) ;
			SortForID( temp, pivotInt+1, last) ;
		} // if	
	} // SortForID()
	
	void SortForCount( vector<message> &temp, int first, int last ) {
		int pivotInt ;
		if ( first < last) {
			partitionForCount( temp, first, last, pivotInt ) ;
			SortForCount( temp, first, pivotInt - 1 ) ;
			SortForCount( temp, pivotInt+1, last) ;
		} // if	
	} // SortForCount()
	
	void Quick_sort( vector<node> &temp, int first, int last ) {
		int pivotInt ;
		if ( first < last) {
			partition( temp, first, last, pivotInt ) ;
			Quick_sort( temp, first, pivotInt - 1 ) ;
			Quick_sort( temp, pivotInt+1, last) ;
		} // if
	} // Quick_sort()
	
	void partitionForID( vector<string> &temp, int first, int last, int & pivotInt ) {
		int lastS1 = first ;
		int unknow = first + 1 ;
		while ( unknow <= last ) {
			if ( atoi( temp[unknow].c_str() ) < atoi( temp[first].c_str() ) ) {
				lastS1++ ;
				swap( temp[lastS1], temp[unknow]) ;
			} // if
			
				unknow ++ ;
		} // while
		
		swap( temp[first], temp[lastS1] ) ;
		pivotInt = lastS1 ;
	} // partition()
	
	void partitionForCount( vector<message> &temp, int first, int last, int & pivotInt) {
		int lastS1 = first ;
		int unknow = first + 1 ;
		while ( unknow <= last ) {
			if ( temp[unknow].visit.size() > temp[first].visit.size() ) {
				lastS1++ ;
				swap( temp[lastS1], temp[unknow]) ;
			} // if
			
			else if ( temp[unknow].visit.size() == temp[first].visit.size() ) {
				if ( temp[unknow].sid1 < temp[first].sid1 ) {
					lastS1++ ;
					swap( temp[lastS1], temp[unknow]) ;
				}
			} // if
				unknow ++ ;
		} // while
		
		swap( temp[first], temp[lastS1] ) ;
		pivotInt = lastS1 ;
	} // partitionForCount()
	
	void partition( vector<node> &temp, int first, int last, int & pivotInt ) {
		int lastS1 = first ;
		int unknow = first + 1 ;
		while ( unknow <= last ) {
			if ( atoi( temp[unknow].sid2.c_str() ) < atoi( temp[first].sid2.c_str() ) ) {
				lastS1++ ;
				swap( temp[lastS1], temp[unknow]) ;
			} // if
			
				unknow ++ ;
		} // while
		
		swap( temp[first], temp[lastS1] ) ;
		pivotInt = lastS1 ;
	} // partition()
	
	void Insert( char sid[MAX_LEN] ) {
		list temp ;
		temp.sid1 = sid ;
		if ( adjL.empty() ) // 空的直接放入 
			adjL.push_back( temp ) ;
		else {
			bool hasInsert = false ;
			for ( int i = 0 ; i < adjL.size() ; i++ ) {
				if ( strcmp( adjL[i].sid1.c_str(), sid ) == 0 ) { // value相同 停止迴圈 
					hasInsert = true ;
					break ;
				}
				else if ( strcmp( adjL[i].sid1.c_str(), sid ) > 0 ) { // 找到位置 insert 
					adjL.insert( adjL.begin() + i, temp ) ;
					hasInsert = true ;
					break ;
				} // else if
			} // for
			
			if ( !hasInsert )
				adjL.push_back(temp) ;
		} // else
		
	} // Insert()
	
	void SetHashtable( int hashsize, vector<hashdata> &hashtable) {
		vector<hashdata> temp(hashsize) ;
		hashtable = temp ;
		for ( int i = 0 ; i < hashtable.size() ; i++ ) 
			hashtable[i].hasdata = false ;
	} // SetStudentlistSize()
		
	void  QuadraticCollision( vector<hashdata> &hashtable, int key, hashdata &temp, int i, int size) { // 碰撞 
		if (hashtable[( key + i * i ) % size].hasdata) { 
			i++ ;
			QuadraticCollision( hashtable, key, temp, i, size ) ;
		} // if
		
		else 
			hashtable[( key + i * i ) % size] = temp ;
	} //  QuadraticCollision()
	
	void QuadraticHashtable( vector<hashdata> &hashtable, int size ) {
		for ( int i = 0 ; i < adjL.size() ; i++ ) {
			int key = 1 ;
			hashdata temp ;
			temp.loc = i ;
			temp.id = adjL[i].sid1 ;
			temp.hasvisit = false ;
			temp.hasdata = true ;
			char sid1[MAX_LEN] ;
			strcpy( sid1, adjL[i].sid1.c_str() ) ;
			for ( int j = 0 ; sid1[j] != '\0' ; j ++) { // search oneSt key
				key *= sid1[j] ; 
				key = key % size ; 
			} // for
			
			if ( hashtable[key].hasdata) // have data in this location ==> Collision
				QuadraticCollision( hashtable, key, temp, 1, size) ;
			else  { // 直接放入 
				hashtable[key] = temp ;
			} // else
		} // for
	} // QuadraticHashtable()
	
	bool Search( vector<hashdata> &hashtable, string id, int &loc, int size ) {
		char sid1[MAX_LEN] ;
		strcpy(sid1, id.c_str() ) ;
		int key = 1 ;
		for ( int j = 0 ; sid1[j] != '\0' ; j ++) { // search oneSt key
			key *= sid1[j] ; 
			key = key % size ; 
		} // for
		
		// 一直找 找到自己的ID為止 , 出 while表示找到 
		int i = 0 ;
		while ( hashtable[( key + i * i ) % size].id != id ) {
			i++ ;
		} // while
		
		if ( hashtable[( key + i * i ) % size].hasvisit == false ) {
			loc = hashtable[( key + i * i ) % size].loc ;
			hashtable[( key + i * i ) % size].hasvisit = true ;
			return true ;
		} // if
		
		else {
			return false ;
		}
	} // Search()
	
	
	public :
		void InfluenceCount( string fileName, int num ) {
			
			vector<message> messageCount ;
			vector<hashdata> hashtable ;
			int size =  adjL.size() * 1.2 + 1 ; // 不包含自己的質數 
			Prime(size) ; // set hashSize
			SetHashtable( size, hashtable ) ;
			QuadraticHashtable( hashtable, size ) ;
			for ( int i = 0 ; i < adjL.size() ; i++ ) {
				message temp ;
				temp.sid1 = adjL[i].sid1 ;
				stack<int> aStack ;
				int loc ;
				Search( hashtable, adjL[i].sid1, loc, size ) ;
				aStack.push( i ) ;
				
				// DFS
				while ( !aStack.empty() ) {
					int j = 0 ;
					int pos = aStack.top();
					aStack.pop();
					while ( j < adjL[pos].receiver.size() ) {
						
						if( adjL[pos].receiver[j].weight >= num ) {
							if ( Search( hashtable, adjL[pos].receiver[j].sid2, loc, size ) == true ) {							
								aStack.push( loc ) ;
								temp.visit.push_back( adjL[pos].receiver[j].sid2 ) ;
							} // if
						
						} // if
						
						
						j++;
					} // for
					
				} // while
					
				messageCount.push_back( temp ) ;
				for ( int j = 0 ; j < hashtable.size() ; j++ )
					hashtable[j].hasvisit = false ;
			} // for
			
			SortForCount( messageCount, 0, messageCount.size() - 1 ) ;
			for ( int i = 0 ; i < messageCount.size() ; i++ ) {
				if ( messageCount[i].visit.size() == 0 ) {
					messageCount.erase( messageCount.begin() + i ) ;	
					i-- ;
				}
			} 
			
			for ( int i = 0 ; i < messageCount.size() ; i++ )
				SortForID( messageCount[i].visit, 0, messageCount[i].visit.size() - 1 ) ;
			
			FileInfluence( fileName, messageCount ) ;
			
		} // INfluenceCount()
		
		void ConnectCount( string fileName ) {
			vector<message> messageCount ;
			vector<hashdata> hashtable ;
			int size =  adjL.size() * 1.2 + 1 ; // 不包含自己的質數 
			Prime(size) ; // set hashSize
			SetHashtable( size, hashtable ) ;
			QuadraticHashtable( hashtable, size ) ;
			for ( int i = 0 ; i < adjL.size() ; i++ ) {
				message temp ;
				temp.sid1 = adjL[i].sid1 ;
				queue<int> q ;
				int loc ;
				Search( hashtable, adjL[i].sid1, loc, size ) ;
				q.push( i ) ;
				// 這個點的周圍visit完，才pop 
				while ( !q.empty() ) {
					for ( int j = 0 ; j < adjL[q.front()].receiver.size() ; j++ ) {
						if ( Search( hashtable, adjL[q.front()].receiver[j].sid2, loc, size ) == true ) {
							q.push( loc ) ;
							temp.visit.push_back( adjL[q.front()].receiver[j].sid2 ) ;
						} // if
					} // for
					
					q.pop() ;
				} // while   visit END
								
				messageCount.push_back( temp ) ;
				for ( int j = 0 ; j < hashtable.size() ; j++ )
					hashtable[j].hasvisit = false ;
			} // for
			
			SortForCount( messageCount, 0, messageCount.size() - 1 ) ;
			for ( int i = 0 ; i < messageCount.size() ; i++ )
				SortForID( messageCount[i].visit, 0, messageCount[i].visit.size() - 1 ) ;
			FileCount(fileName, messageCount) ;
		} // ConnectCount()
		
		void AdjacencyList() {
			for ( int i = 0 ; i < student_list.size() ; i++ ) {
				// 比對主陣列有無此 ID 
				Insert ( student_list[i].sid1 ) ; 
				Insert ( student_list[i].sid2 ) ;
				// 建立 sender 與 receiver link
				node temp ;
				temp.sid2 = student_list[i].sid2 ;
				temp.weight = student_list[i].weight ;
				for ( int j = 0 ; j < adjL.size() ; j++ ) {
					if (  student_list[i].sid1 == adjL[j].sid1 )
						adjL[j].receiver.push_back(temp) ;
				} // for
			} // for
			
		} // AdjacencyList()
		
		bool LoadFile(string fileName) {
			fileName = "pairs" + fileName + ".bin" ; 
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
		} // LoadBinfile()
		
		void LoadInVector(string fileName ) {
			fileName = "pairs" + fileName + ".bin" ; 
			studentPair oneSt ;
			int stNo = 0 ;
			binfile.open(fileName.c_str(), fstream::in | fstream::binary) ;
			if ( binfile.is_open() ) {
				binfile.seekg(0, binfile.end ) ;
				stNo = binfile.tellg() / sizeof( oneSt ) ;
				binfile.seekg(0, binfile.beg) ;
				for ( int i = 0 ; i < stNo ; i++ ) { // 把所有學生資料存在vector 
					binfile.read((char*)&oneSt,sizeof(oneSt) ) ;
					student_list.push_back(oneSt) ;
				} // for
			} // if
			
			binfile.close() ;
		} // LoadInVector()
		
		void FileInfluence( string fileName, vector<message> messageCount ) {
			fileName = "pairs" + fileName + ".inf" ;
			cntfile.open(fileName.c_str(), ios::out) ;
			cout << endl << "<<< There are " << messageCount.size() << " IDs in total. >>>"<< endl ;
			cntfile << "<<< There are " << messageCount.size() << " IDs in total. >>>\n" ;
			for ( int i = 0 ; i < messageCount.size() ; i++ ) {
				cntfile << "[" ;
				if ( i < 9 )
					cntfile << " " ;
				cntfile << i + 1 << "] " << messageCount[i].sid1 << "(" << messageCount[i].visit.size() << "): \n" ;
				for ( int j = 0 ; j < messageCount[i].visit.size() ; j++ ) {
					cntfile << "\t(" ;
					if ( j < 9 )
						cntfile << " " ;
					cntfile << j + 1 << ")" << messageCount[i].visit[j] ;
					if ( ( j + 1) %10 == 0 )
						cntfile << "\n" ;
				} // for
				
				cntfile << "\n" ;
			} // for
			
			cntfile.close() ;
		} // FileCount()
		
		void FileCount( string fileName, vector<message> messageCount ) {
			fileName = "pairs" + fileName + ".cnt" ;
			cntfile.open(fileName.c_str(), ios::out) ;
			cout << endl << "<<< There are " << messageCount.size() << " IDs in total. >>>"<< endl ;
			cntfile << "<<< There are " << messageCount.size() << " IDs in total. >>>\n" ;
			for ( int i = 0 ; i < messageCount.size() ; i++ ) {
				cntfile << "[" ;
				if ( i < 9 )
					cntfile << " " ;
				cntfile << i + 1 << "] " << messageCount[i].sid1 << "(" << messageCount[i].visit.size() << "): \n" ;
				for ( int j = 0 ; j < messageCount[i].visit.size() ; j++ ) {
					cntfile << "\t(" ;
					if ( j < 9 )
						cntfile << " " ;
					cntfile << j + 1 << ")" << messageCount[i].visit[j] ;
					if ( ( j + 1) %10 == 0 )
						cntfile << "\n" ;
				} // for
				
				cntfile << "\n" ;
			} // for
			
			cntfile.close() ;
		} // FileCount()
		
		void FileOut( string fileName ) {
			int total = 0 ;
			for ( int i = 0 ; i < adjL.size() ; i++) {
				Quick_sort( adjL[i].receiver, 0, adjL[i].receiver.size() - 1 ) ;
				total += adjL[i].receiver.size() ;
			} // for
			
			fileName = "paris" + fileName + ".adj" ;
			adjfile.open(fileName.c_str(), ios::out) ;
			cout << endl << "<<< There are " << adjL.size() << " IDs in total. >>>"<< endl ;
			adjfile << "<<< There are " << adjL.size() << " IDs in total. >>>\n" ;
			for ( int i = 0 ; i < adjL.size() ; i++ ) {
				adjfile << "[" ;
				if ( i < 9 )
					adjfile << " " ;
				adjfile << i + 1 << "] " << adjL[i].sid1 << ": \n" ;
				
				for ( int j = 0 ; j < adjL[i].receiver.size() ; j++ ) {
					adjfile << "\t(" ;
					if ( j < 9)
						adjfile << " " ;
					adjfile << j + 1 << ") " << adjL[i].receiver[j].sid2 << ",  " << 
					adjL[i].receiver[j].weight  ;
					if ( ( j + 1 ) % 10 == 0)
						adjfile << "\n" ;
				} // for
				
				adjfile << "\n" ;
			} // for
			
			cout << "<<< There are "<< total << " nodes in total. >>>" << endl ;
			adjfile << "<<< There are "<< total << " nodes in total. >>>" ;
			adjfile.close() ;
		} // FileOut()
		
		vector<list> GetAdjL() {
			return adjL ;
		} // GetAdjL()
		
		void Set( vector<list> temp ) {
			adjL = temp ;
		} // Set()
	
};

int main() {
	string com ="" ;
	string fileName ;
	bool hasData = false ;
	vector<list> temp ;
	while (1) {
		PrintCommand() ;
		cin >> com ;
		if ( com == "0")
			exit(0) ;
		else if ( com == "1" ) {
			Graph graph ;
			cout << "輸入要執行的檔案 : " ; 
		    cin >> fileName ;
			if ( graph.LoadFile(fileName) ) {
				graph.LoadInVector(fileName) ;
				graph.AdjacencyList() ;
				graph.FileOut(fileName) ;
				temp = graph.GetAdjL() ;
				hasData = true ;
			} // if
		} // else if
		
		else if ( com == "2" ) {
			if ( !hasData ) 
				cout << "### There is no graph and choose 1 first. ###" << endl ;
			else {
				Graph graph ;
				graph.Set( temp ) ;
				// 假設 A 走訪過 BCDE，將BCDE存到 hashtable 找一次就可以判斷是否存在過 
				graph.ConnectCount(fileName) ;
			} // else		
		} // else if
		
		else if ( com == "3" ) {
			if ( !hasData )
				cout << "### There is no graph and choose 1 first. ###" << endl ;
			else {
				Graph graph ;
				graph.Set( temp ) ;
				cout << "Input a real number in [0,1]: " ;
				float num ;
				cin >> num ;
				while ( num < 0 || num > 1) {
					cout << "### It is NOT in [0,1] ###" << endl ;
					cin >> num ;
				} // while
				
				graph.InfluenceCount( fileName, num ) ;
				
			} // else
		} // else if
		
		else 
			cout << "指令不存在,請重新輸入" << endl ;
	} // while
} // main()
