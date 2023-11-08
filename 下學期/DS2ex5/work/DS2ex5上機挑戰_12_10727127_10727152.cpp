// 10727127 邱佳駿 10727152 黃景泓 
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<math.h>
#include<queue>
#include<stack> 
#include<iomanip>
#define COLUMNS	6
#define MAX_LEN 10
#define BIG_INT 255 
using namespace std ;
fstream binfile ;
fstream adjfile ;
fstream cntfile ;
fstream ccfile ;
fstream dsfile ;
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
struct vertex {
	string sid1 ;
	float weight ;
};

void PrintCommand() {
	cout <<  endl << "*************  Hash Table  ************" << endl <<
					 "***                                 ***" << endl <<
			         "* -1. 離開                            *" << endl <<
    		         "*  0. Create adjacency lists          *" << endl <<
			         "*  1. Build connected components      *" << endl << 
			         "*  2. Find shortest paths by Dijkstra *" << endl <<
			         "*  3. Generate minimum spanning trees *" << endl <<
			         "***************************************" << endl << 
                     "請輸入指令( -1, 0, 1, 2, 3): " ; 
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
			if ( temp[unknow].sid2 < temp[first].sid2) {
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
	
	void Quadratic( vector<hashdata> &hashtable, int size, vector<vertex> path ) {
		for ( int i = 0 ; i < path.size() ; i++ ) {
			int key = 1 ;
			hashdata temp ;
			temp.loc = i ;
			temp.id = path[i].sid1 ;
			temp.hasvisit = false ;
			temp.hasdata = true ;
			char sid1[MAX_LEN] ;
			strcpy( sid1, path[i].sid1.c_str() ) ;
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
	} // Quadratic()
	
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
	
	bool InsureSearch( vector<hashdata> &hashtable, string id, int loc, int size ) {
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
		
		if ( hashtable[( key + i * i ) % size].hasvisit == false ) 
			return true ;
		else 
			return false ;
	} // InsureSearch()
	
	int SearchLoc( vector<hashdata> &hashtable, string id, int size ) {
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
		
		return hashtable[( key + i * i ) % size].loc ;
	} // Search()
	
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
	
	bool SearchID( vector<hashdata> &hashtable, string id, int &loc, int size) {
		char sid1[MAX_LEN] ;
		strcpy(sid1, id.c_str() ) ;
		int key = 1 ;
		for ( int j = 0 ; sid1[j] != '\0' ; j ++) { // search oneSt key
			key *= sid1[j] ; 
			key = key % size ; 
		} // for
		
		// 一直找 找到自己的ID為止 , 出 while表示找到 
		int i = 0 ;
		while ( hashtable[( key + i * i ) % size].hasdata == true ) {
			if (hashtable[( key + i * i ) % size].id == id) {
				loc = hashtable[( key + i * i ) % size].loc ;
				hashtable[( key + i * i ) % size].hasvisit = true ;
				return true ;
			} // if
			i++ ;
		} // while
		
		return false ;
		} // SearchID()
	
	void PrintID() {
		cout << endl ;
		int count = 0 ;
		for ( int i = 0 ; i < adjL.size() ; i++ ) {
			cout << adjL[i].sid1 << " " ;
			count++ ;
			if ( count % 8 == 0)
				cout << endl ;
		} // for
		
		cout << endl << "Input a student ID [0:exit]: " ;
	} // PrintID
	
	
	float EdgeWeight( string u, int loc) {
		for ( int i = 0 ; i < adjL[loc].receiver.size() ; i++ ) {
			if ( adjL[loc].receiver[i].sid2 == u )
				return adjL[loc].receiver[i].weight ;
		} // for
		
		return 10000 ;
	} // EdgeWeight()
	
	
	float FindShortest(vector<hashdata> &hashtable, vector<string> v , int size, int loc, string &ID ) {
		float min = 10000 ;
		for ( int i = 0 ; i < v.size() ; i++ ) {
			loc = SearchLoc( hashtable, v[i], size ) ;
			for ( int j = 0 ; j < adjL[loc].receiver.size() ; j++ ) {
				if ( InsureSearch(hashtable, adjL[loc].receiver[j].sid2 , loc, size ) == true ) {
					if ( min > adjL[loc].receiver[j].weight  ) {
						min = adjL[loc].receiver[j].weight ;
						ID = adjL[loc].receiver[j].sid2 ;
					}
				} // if	
			} // for
			
		} // for
		
		if ( min == 10000 )
			return 0 ;
		else
			return min ;
	} // FindShortest()
	
	public :
		float SpanningTree(vector<hashdata> &hashtable, vector<string> visit ) {
			int size =  adjL.size() * 1.2 + 1 ; 
			float total = 0 ;
			Prime(size) ;
			int loc ;
			string ID ;
			vector<string> v ;
			// reset
			for ( int j = 0 ; j < hashtable.size() ; j++ )
				hashtable[j].hasvisit = false ;
			
			v.push_back(visit[0]) ;
			Search( hashtable, visit[0] , loc, size ) ;
			for ( int i = 0 ; i < visit.size() ; i++ ) {
				total += FindShortest( hashtable, v, size, loc, ID ) ;
				v.push_back(ID) ;
				Search( hashtable, ID , loc, size ) ;
			} // for
			
			return total ;
		} // SpanningTree()
		
		void ShortestPath(vector<message> component, vector<hashdata> &hashtable, string fileName, string weight ) {
			int size =  adjL.size() * 1.2 + 1 ; 
			Prime(size) ;
			int loc ;
			PrintID() ;
			string ID ;
			cin >> ID ;
			while ( ID != "0") {
				vector<vertex> answerPath ;
				stack<int> aStack ;
				vector<vertex> path ;
				vertex temp;
				// Reset hash table 
				for ( int j = 0 ; j < hashtable.size() ; j++ )
					hashtable[j].hasvisit = false ;
				if ( SearchID( hashtable, ID, loc, size ) == false )
					cout << "### the student id does not exist! ###" << endl ;
					
				else {
					// 將 ID 與 weight 存於 資料欄內  
					// weight 除了自己為0以外 其他設定為 10000 
					temp.sid1 = ID ;
					temp.weight = 0 ;
					aStack.push( loc ) ;
					path.push_back(temp) ;
					while ( !aStack.empty() ) {
						int j = 0 ;
						int pos = aStack.top();
						aStack.pop();
						while ( j < adjL[pos].receiver.size() ) {
							
							if ( Search( hashtable, adjL[pos].receiver[j].sid2, loc, size ) == true ) {							
								aStack.push( loc ) ;
								temp.sid1 =  adjL[pos].receiver[j].sid2 ;
								temp.weight = 10000 ;
								path.push_back(temp) ;
							} // if
		
							j++;
						} //while		
						
					} // while
					
					// 針對 path 設立一個 hashtable 
					int path_size = path.size() * 1.2 + 1 ; 
					Prime(path_size) ; 
					vector<hashdata> path_hash ; 
					SetHashtable( path_size, path_hash ) ;
					Quadratic( path_hash, path_size, path ) ;
					int path_loc ;
					// reset
					for ( int j = 0 ; j < hashtable.size() ; j++ )
						hashtable[j].hasvisit = false ;
					
					float cheapest = 10000 ;
					string v = ID ;
					vertex temp ;
					temp.sid1 = ID ;
					temp.weight = 0 ;
					do {
						answerPath.push_back(temp) ;
						Search( hashtable, v, loc, size ) ;
						for ( int j = 0 ; j < path.size() ; j++ ) {
							if ( InsureSearch( hashtable, path[j].sid1, loc, size ) == true  ) {
								float min = path[SearchLoc(path_hash, v, path_size) ].weight + EdgeWeight( path[j].sid1, loc ) ;
								
								if (min < path[j].weight )
									path[j].weight = min ;
							} // if
						} // for
						
						cheapest = 10000;
						for ( int j = 0 ; j < path.size() ; j++ ) {
							if ( InsureSearch( hashtable, path[j].sid1, loc, size ) == true  ) {
								if( path[j].weight < cheapest ) {
									v = path[j].sid1;
									temp.sid1 = path[j].sid1 ;
									temp.weight = path[j].weight ;
									cheapest = path[j].weight;
								} // if
							} // if
						} // for
					} while (cheapest < 10000) ;
					
					FileShorest( fileName, weight, answerPath ) ;
				} // else
				
				PrintID() ;	
				cin >> ID ;
			} // while
			
			
		} // ShortestPath()
		
		vector<message> ConnectComponent( string fileName, string weight, vector<hashdata> &hashtable ) {
			vector<message> messageCount ;
			int size =  adjL.size() * 1.2 + 1 ; // 不包含自己的質數 
			Prime(size) ; // set hashSize
			SetHashtable( size, hashtable ) ;
			QuadraticHashtable( hashtable, size ) ;
			for ( int i = 0 ; i < adjL.size() ; i++ ) {
				message temp ;
				temp.sid1 = adjL[i].sid1 ;
				stack<int> aStack ;
				int loc ;
				if ( Search( hashtable, adjL[i].sid1, loc, size ) == true ) {
					aStack.push( i ) ;
					temp.visit.push_back( adjL[i].sid1 ) ;
				}
				
				// DFS
				while ( !aStack.empty() ) {
					int j = 0 ;
					int pos = aStack.top();
					aStack.pop();
					while ( j < adjL[pos].receiver.size() ) {
						if ( Search( hashtable, adjL[pos].receiver[j].sid2, loc, size ) == true ) {							
							aStack.push( loc ) ;
							temp.visit.push_back( adjL[pos].receiver[j].sid2 ) ;
						} // if
	
						j++;
					} // while
					
				} // while
					
				messageCount.push_back( temp ) ;
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
			
			FileConnect( fileName, messageCount, weight ) ;
			return messageCount ;
		} // ConnectComponent()
		
		void AdjacencyList( float realNum) {
			for ( int i = 0 ; i < student_list.size() ; i++ ) {
				// 比對主陣列有無此 ID 
				if ( student_list[i].weight <= realNum ) {
					Insert ( student_list[i].sid1 ) ; 
					Insert ( student_list[i].sid2 ) ;
					// 建立 sender 與 receiver link
					node temp1, temp2 ;
					temp1.sid2 = student_list[i].sid2 ;
					temp1.weight = student_list[i].weight ;
					temp2.sid2 = student_list[i].sid1 ;
					temp2.weight = student_list[i].weight ;
					for ( int j = 0 ; j < adjL.size() ; j++ ) {
						if ( student_list[i].sid1 == adjL[j].sid1 )
							adjL[j].receiver.push_back(temp1) ;
						if ( student_list[i].sid2 == adjL[j].sid1 )
							adjL[j].receiver.push_back(temp2) ;
					} // for
				} // if
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
		
		void FileConnect( string fileName, vector<message> messageCount, string weight ) {
			fileName = "pairs" + fileName + "_" + weight + ".cc" ;
			cntfile.open(fileName.c_str(), ios::out) ;
			cout << endl << "<<< There are " << messageCount.size() << " IDs in total. >>>"<< endl ;
			cntfile << "<<< There are " << messageCount.size() << " IDs in total. >>>\n" ;
			for ( int i = 0 ; i < messageCount.size() ; i++ ) {
				cntfile << "{" ;
				cout << "{" ;
				if ( i < 9 ) {
					cntfile << " " ;
					cout << " " ;
				} // if
				cntfile << i + 1 << "} Connected Component: size = " << messageCount[i].visit.size() << endl ;
				cout << i + 1 << " }" << " Connect Component: size = " << messageCount[i].visit.size() << endl ;
				for ( int j = 0 ; j < messageCount[i].visit.size() ; j++ ) {
					cntfile << "\t(" ;
					if ( j < 9 )
						cntfile << " " ;
					cntfile << j + 1 << ")" << messageCount[i].visit[j] ;
					if ( ( j + 1) %8 == 0 )
						cntfile << "\n" ;
				} // for
				
				cntfile << "\n" ;
			} // for
			
			cntfile.close() ;
		} // FileConnect()
		void FileShorest( string fileName, string weight, vector<vertex> answerPath ) {
			fileName = "paris" + fileName + "_" + weight + ".ds" ;
			dsfile.open(fileName.c_str(), ios::out| ios::app ) ;
			dsfile << "\n" << "origin : " << answerPath[0].sid1 << "\n" ;
			for ( int i = 1 ; i < answerPath.size() ; i++ ) {
				dsfile << setw(5)<< " ( " << setw(2) << i << ") " << setw(8) << answerPath[i].sid1 << ", " << setw(4) << answerPath[i].weight ;
				if ( i % 8 == 0 )
					dsfile << "\n" ;	
			} // for
			
			dsfile.close() ;
		} // FileShorest()
		
		void FileOut( string fileName, string weight ) {
			int total = 0 ;
			for ( int i = 0 ; i < adjL.size() ; i++) {
				Quick_sort( adjL[i].receiver, 0, adjL[i].receiver.size() - 1 ) ;
				total += adjL[i].receiver.size() ;
			} // for
			fileName = "paris" + fileName + "_" + weight + ".adj" ;
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
	bool hasComponent = false ;
	vector<list> temp ;
	string weight ;
	float realNum ;
	vector<message> component ;
	vector<hashdata> hashtable ;
	while (1) {
		PrintCommand() ;
		cin >> com ;
		if ( com == "-1")
			exit(0) ;
		else if ( com == "0" ) {
			cout << "Input a real number in (0,1] : " ;
			cin >> weight ;
			while ( atof(weight.c_str()) > 1 || atof(weight.c_str()) <= 0 ) {
				cout << "### It is NOT in (0,1] ###" << endl ;
				cout << "Input a real number in (0,1] : " ;
				cin >> weight ;
			} // while
			realNum = atof(weight.c_str()) ;
			Graph graph ;
			cout << "Input a file number ([0] Quit) : " ; 
		    cin >> fileName ;
		    if ( fileName != "0" ) {
				if ( graph.LoadFile(fileName) ) {
					graph.LoadInVector(fileName) ;
					graph.AdjacencyList(realNum) ;
					graph.FileOut(fileName, weight ) ;
					temp = graph.GetAdjL() ;
					hasData = true ;
				} // if
			} // if
		} // else if
		
		else if ( com == "1" ) {
			if ( !hasData )
				cout << "### There is no graph and choose 0 first. ###" << endl ;
			else {
				Graph graph ;
				graph.Set( temp ) ;
				component = graph.ConnectComponent( fileName, weight, hashtable) ;
				hasComponent = true ;
			} // else
		} // else if
		
		else if ( com == "2" ) {
			if ( !hasData )
				cout << "### There is no graph and choose 0 first. ###" << endl ;
			else if ( !hasComponent )
				cout << "### Choose 1 to find connected components. ###" << endl ;
			else {
				Graph graph ;
				graph.Set(temp) ;
				graph.ShortestPath(component, hashtable, fileName, weight ) ;
			} // else
		} // else if
		
		else if ( com == "3" ) {
			if ( !hasData )
				cout << "### There is no graph and choose 0 first. ###" << endl ;
			else if ( !hasComponent )
				cout << "### Choose 1 to find connected components. ###" << endl ;
			else {
				Graph graph ;
				graph.Set(temp) ;
				for ( int i = 0 ; i < component.size() ; i++ ) {
					cout << "The MST cost of connected component { "  << setw(2)<< i + 1 << "} = " ;
					printf( "%.4f\n", graph.SpanningTree(hashtable, component[i].visit)) ;
					//cout << graph.SpanningTree(hashtable, component[i].visit) << endl ;
				}
			} // else
		} // elde if
		
		else 
			cout << "指令不存在,請重新輸入" << endl ;
	} // while
} // main()
