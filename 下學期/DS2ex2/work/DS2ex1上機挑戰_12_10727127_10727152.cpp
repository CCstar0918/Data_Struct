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
using namespace std ;
fstream infile ;
int count = 0 ;
struct school {
	string graduate ;
	string school_name ;
	string department_name ;
	string day ;
	string leveal ;
	int numOfGraduate ;
	int serial_number ;
}; // school

vector<school> school_list ;

void PrintCommand() {
	cout <<  endl << "********  Heap Construction  ********" << endl <<
					 "***                               ***" << endl <<
			         "* 0. 離開                           *" << endl <<
    		         "* 1. Build a min heap               *" << endl <<
			         "* 2. Build a min-max heap           *" << endl << 
			         "* 3. Top-K minimum from min-max heap*" << endl <<
			         "*************************************" << endl << 
                     "請輸入指令( 0, 1, 2, 3 ): " ; 
} // PrintCommand

bool Load( string fileName ) {  // load file        
	fileName = "input" + fileName + ".txt" ; 
	infile.open( fileName.c_str(), ios::in ) ;  
	if ( infile == NULL ) {
		cout << endl << fileName << " does not exist !! " << endl ;
		return false ;
	} // if

	return true ;
} // Load() 

void ReadFile() {
	school_list.clear() ;
    cout << "輸入要執行的檔案 : " ; 
    string fileName ;
    cin >> fileName ;
    while ( !Load(fileName) ) { // 讀到有檔案為止 
		cout << "輸入要執行的檔案 : " ;
		cin >> fileName ;
	} // while
} // ReadFile()

void BuildMinHeap( int node ) {
	// 在做陣列的數值交換與比較時要注意 node - 1 才會取得正確的 index 
	// compare with father and change
	if ( node / 2 != 0 ) {
		if ( school_list[node - 1].numOfGraduate < school_list[node/2 - 1].numOfGraduate ) {
			swap( school_list[node - 1], school_list[node/2 - 1] ) ;
			node = node / 2 ;
			BuildMinHeap( node ) ;
		} // if
	} // if
} // BuildMinHeap()

void MinHeap() {
	string line ;
	// read three lines text
	for ( int i = 0 ; i < 3 ; i ++) { 
		getline( infile, line ) ;
	} // for
	
	school t ;
	int count = 1 ;
	// 先直接建立在bottom再去跟父節點做比較 and 交換 
	while ( getline( infile, line, '\t') ) {           // school_id 
		getline ( infile, t.school_name , '\t') ;               // school_name
		getline ( infile, line, '\t') ; 			   // dempartment_id
		getline ( infile, t.department_name, '\t' ) ;               // department_name
		getline	( infile, t.day, '\t' ) ;			   // day
		getline ( infile, t.leveal, '\t') ;			       // leveal
		getline ( infile, line, '\t') ;		           // student
		getline ( infile, line, '\t') ;		           // teacher
		getline ( infile, t.graduate, '\t') ;		   // graduate
		getline ( infile, line) ;					   // city and type
		t.numOfGraduate = atoi( t.graduate.c_str() ) ; // graduate string to int
		t.serial_number = count ;                      // serial_number
		school_list.push_back(t) ;
		BuildMinHeap( count ) ;
		count++ ;
	} // while
	
	infile.close() ;
} // MinHeap()

int FindLeftMost( int & i ) {
	// 以2^n 去判斷 leftMost 的位置 
	int leftMost = 1 ;
	i = 0 ;
	while ( leftMost <= school_list.size() ) {
		i++ ;
		leftMost *= 2 ;
	} // while
	
	return leftMost / 2 ;
} // FindLeftMost()

void ChangeWithGrandfather(int node, int floor ) {
	// 單數層愈小的愈上面 ， 雙數層愈大的愈上面
	if ( floor % 2 == 1 ) { // 單數 
		if ( node / 4 != 0 ) {
			if ( school_list[node - 1].numOfGraduate < school_list[node/4 - 1].numOfGraduate ) {
				swap( school_list[node - 1], school_list[node/4 - 1] ) ;
				node = node / 4 ;
				ChangeWithGrandfather( node, floor ) ;
			} // if
		} // if
	} // if
	
	else { // 雙數 
		if ( node / 4 != 0 ) {
			if ( school_list[node - 1].numOfGraduate > school_list[node/4 - 1].numOfGraduate ) {
				swap( school_list[node - 1], school_list[node/4 - 1] ) ;
				node = node / 4 ;
				ChangeWithGrandfather( node, floor ) ;
			} // if	
		} // if
	} // else
	
} // ChangeWithGrandfather()

void BuildMinMaxHeap(int node, int floor) {
	// 在做陣列的數值交換與比較時要注意 node - 1 才會取得正確的 index 
	// 先找到的bottom在第幾層( 單 or 雙 ) --> 根據 floor 處理 min or max 與父層做交換 --> 根據交換後的 floor 與祖父層做比較進行交換
	// 因為要往上找，所以在單數層要找 max ，雙數層要找 min 
	if ( floor % 2 == 1 ) { // 單數層 max
		if ( node / 2 != 0 ) {
			if ( school_list[node - 1].numOfGraduate > school_list[node/2 - 1].numOfGraduate ) {
				swap( school_list[node - 1], school_list[node/2 - 1] ) ;
				node = node / 2 ;
				floor-- ;
			} // if
			
			ChangeWithGrandfather( node, floor ) ;
		} // if
	} // if
	
	else { // 雙數層 min
		if ( node / 2 != 0 ) {
			if ( school_list[node - 1].numOfGraduate < school_list[node/2 - 1].numOfGraduate ) {
				swap( school_list[node - 1], school_list[node/2 - 1] ) ;
				node = node / 2 ;
				floor-- ;
			} // if
			
			ChangeWithGrandfather( node, floor ) ;
		} // if
		
	} // else
} // BuildMinMaxHeap()

void MinMaxHeap() {
	string line ;
	// read three lines text
	for ( int i = 0 ; i < 3 ; i ++) { 
		getline( infile, line ) ;
	} // for
	
	school t ;
	int count = 1 ;
	// 先直接建立在bottom再去跟父節點做比較 and 交換 
	while ( getline( infile, line, '\t') ) {           // school_id 
		getline ( infile, t.school_name , '\t') ;               // school_name
		getline ( infile, line, '\t') ; 			   // dempartment_id
		getline ( infile, t.department_name, '\t' ) ;               // department_name
		getline	( infile, t.day, '\t' ) ;			   // day
		getline ( infile, t.leveal, '\t') ;			       // leveal
		getline ( infile, line, '\t') ;		           // student
		getline ( infile, line, '\t') ;		           // teacher
		getline ( infile, t.graduate, '\t') ;		   // graduate
		getline ( infile, line) ;					   // city and type
		t.numOfGraduate = atoi( t.graduate.c_str() ) ; // graduate string to int
		t.serial_number = count ;                      // serial_number
		school_list.push_back(t) ;
		int floor  ;
		FindLeftMost(floor) ;
		BuildMinMaxHeap( count, floor ) ;
		count++ ;
	} // while
	
	infile.close() ;
} // MinMaxHeap()

void RebuildMinHeap( int node ) { 
	// 分兩種狀況來看 ， 先判斷左子樹是否存在，再進階判斷右子樹是否存在 ( 左子樹不存在 ，右子樹就一定不存在 ) 
	// 左右兩邊比較取小的，在跟 node 比較 
	if ( node * 2 <= school_list.size() ) {
		if ( node * 2 + 1 <= school_list.size() ) {
			// 左右子樹都存在 
			int min = 0 ; // 記比較小的節點位置 
			if ( school_list[node * 2 - 1].numOfGraduate < school_list[node * 2].numOfGraduate)
				min = node * 2 ;
			else 
				min = node * 2 + 1 ;
			
			if ( school_list[min - 1].numOfGraduate < school_list[node - 1].numOfGraduate ) {
				swap( school_list[min - 1], school_list[node - 1] ) ;
				node = min ;
				RebuildMinHeap( node ) ;
			} // if
		} // if
		
		else {
			// 只存在左子樹 
			if ( school_list[node * 2 - 1].numOfGraduate < school_list[node -  1].numOfGraduate ) {
				swap( school_list[node * 2 - 1], school_list[node - 1] ) ;
				node = node * 2 ;
				RebuildMinHeap( node ) ;
			} // if
		} // else 
		
	} // if
} // RebuildMinHeap()

void ChangeWithGrandson( int node ) {	
	// 先確認有沒有孫子
	if ( node * 4 <= school_list.size() ) {
		// 四個孫子取最小的
		int i = node * 4 ; // 記孫子的位置
		int min = i ;
		for ( int j = 1 ; j < 4 && ( ( i + j ) <= school_list.size() ) ; j++ ) {
			if ( school_list[i + j - 1].numOfGraduate < school_list[min - 1].numOfGraduate ) 
				min = i + j ;
		} // for
		
		if ( school_list[min - 1].numOfGraduate < school_list[node - 1].numOfGraduate ) {
			swap( school_list[min - 1], school_list[node - 1] ) ;
			node = min ;
			// function recursion
			ChangeWithGrandson( node ) ;
		} // if
	} // if
	
	else { // 沒有孫子，如果還有兒子多一次判斷
		if ( node * 2 <= school_list.size() ) {
			if ( node * 2 + 1 <= school_list.size() ) { // 兩個兒子的狀況
				int min ;
				if ( school_list[node * 2 - 1].numOfGraduate < school_list[node * 2].numOfGraduate )
					min = node * 2 ;
				else
					min = node * 2 + 1 ;
				
				if ( school_list[min - 1].numOfGraduate < school_list[node - 1].numOfGraduate )
					swap( school_list[min - 1], school_list[node - 1] ) ;
			} // if
			
			else {
				if ( school_list[node * 2 - 1].numOfGraduate < school_list[node - 1].numOfGraduate ) 
					swap(school_list[node * 2 - 1], school_list[node - 1]  ) ;
			} // else
		} // if
		
	} // else
} // ChangeWithGrandson()

void RebuildMinMaxHeap( int node ) {
	// 第一層跟第二層先處理
	if ( node * 2 <= school_list.size() ) {
		if ( node * 2 + 1 <= school_list.size() ) {
			// 左右子樹都存在 
			int min = 0 ; // 記比較小的節點位置 
			if ( school_list[node * 2 - 1].numOfGraduate < school_list[node * 2].numOfGraduate)
				min = node * 2 ;
			else 
				min = node * 2 + 1 ;
			
			if ( school_list[min - 1].numOfGraduate < school_list[node - 1].numOfGraduate ) {
				swap( school_list[min - 1], school_list[node - 1] ) ;
			} // if
		} // if
		
		else {
			// 只存在左子樹 
			if ( school_list[node * 2 - 1].numOfGraduate < school_list[node -  1].numOfGraduate )
				swap( school_list[node * 2 - 1], school_list[node - 1] ) ;
		} // else 	
	} // if
	
	// 下面的要用function 
	ChangeWithGrandson( node ) ;
} // RebuildMinMaxHeap()

void Print() {
	int i = 0 ;
	int leftMost = FindLeftMost(i) ;
	cout << "root : [" << school_list[0].serial_number << "]" << " " << school_list[0].graduate << endl ;
	cout << "bottom : [" << school_list[school_list.size() - 1].serial_number << "]" << " " << school_list[school_list.size() - 1].graduate << endl ;
	cout << "leftmost bottom : [" << school_list[leftMost - 1].serial_number << "]" << " " << school_list[leftMost - 1].graduate << endl ;
} // PrintMission1()

int main() {
	string com = "" ; // 接收指令 
	bool haveMinHeap  = false ; 
	bool haveMinMaxHeap = false ;
 	while ( 1 ) {                            
		PrintCommand() ;
      	cin >> com ;
      	if ( com.compare( "0" ) == 0 ) {
      		exit(0) ;
		} // if
        	
        else if ( com == "1") { // Min Heap
        	ReadFile() ;
			MinHeap() ;
			cout << "< min heap >" << endl ;
			Print() ;
			haveMinHeap = true ;
			haveMinMaxHeap = false ;
        } // else if
        
        else if ( com == "2") { // Min-Max Heap
        	ReadFile() ;
        	MinMaxHeap() ;
        	cout << "< min-max heap >" << endl ;
        	Print() ;
        	haveMinMaxHeap = true ;
        	haveMinHeap = false ;
		} // else if
		
		/*
		else if ( com == "3") { 
			// 對 min heap delete， delete 最小的 
			if ( haveMinHeap ) { 
				// root swap with bottom and rebuildHeap
				if ( school_list.size() != 0) {
					swap( school_list[0], school_list[school_list.size() - 1] ) ;
					school_list.erase( school_list.begin() + school_list.size() - 1) ;
					int node = 1 ;
					RebuildMinHeap( node ) ;
					if ( school_list.size() != 0 )
						Print() ;
					else 
						cout << "Min-Heap is empty !!!" << endl ;
				}
				
				else
					cout << "Min-Heap is empty !!!" << endl ;
			} // if
			 
			else 
				cout << "請先執行任務 1" << endl ;		
		} // else if
		*/
		else if ( com == "3" ) {
			// min max heap delete， delete min
			if ( haveMinMaxHeap ) {
				if ( school_list.size() != 0 ) {
					int k ;
					cout << "Enter the value of K in [1," << school_list.size() << "] :" ;
					cin >> k ;
					if ( 1 <= k && k <= school_list.size() ) {
						for ( int i = 0 ; i < k ; i++ ) {
							cout << "Top " << i + 1 << ": " << "[" << school_list[0].serial_number << "]" <<
																      school_list[0].school_name << school_list[0].department_name << ", " <<
																      school_list[0].day << ", " <<
																      school_list[0].leveal << ", " <<
																      school_list[0].graduate << endl ;
																   
							swap( school_list[0], school_list[school_list.size() - 1] ) ;
							school_list.erase( school_list.begin() + school_list.size() - 1) ;
							int node = 1 ;
							RebuildMinMaxHeap( node ) ;
						} // for
					} // if
				} // if
				
				else
					cout << "MinMax-Heap is empty !!!" << endl ;
			} // if
			
			else
				cout << "請先執行任務2" << endl ; 
		} // else if
		
		else {
			cout << "指令不存在,請重新輸入" << endl;
		}
	} // while 
} // main()
 
