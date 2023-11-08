// 10727127 邱佳駿 10727157 毛琨鈜
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
FILE *infile = NULL ;
struct school {
	char school_id[10] ;
	char school_name[100] ;
	char department_id[10] ;
	char department_name[100] ;
	char day[10] ;
	char day2[10] ;
	char leveal[10] ;
	char leveal2[10] ;
	char student[10] ;
	char teacher[10] ;
	int graduate ;
	char city[10] ;
	char city2[10] ;
	char type[10] ;
	char type2[10] ;
}; // school

vector<school> school_list ;
vector<school> temp ;
int Time ;
void PrintCommand() {
	cout <<  "* 0. 離開" << endl <<
    "* 1. Selection sort VS. Bubble sort" << endl << "* 2. Merge sort VS. Quick sort " << endl  << "* 3. Radix Sort " << endl <<
    "請輸入指令( 0, 1, 2, 3 ):" ; 
} // PrintCommand

bool IsDigit( string input ) {
	for ( int i = 0 ; i < input.size() ; i++ )
    	if ( !isdigit( input[i] ) )
   			return false ;
	return true ;
    // 輸入的東西只有純數字的狀況才接受 
} // IsDigit() 

bool Load( string fileName) {  // load file        
	fileName = "input" + fileName + ".txt" ; 
	infile = fopen( fileName.c_str(), "r" ) ;   
	if ( infile == NULL) {
		cout << endl << fileName << " does not exist" << endl ;
		return false ;
	} // if

	return true ;
} // Load() 

void StoreFile() {
	char ch ;
	// 把多餘的東西讀掉 
	for ( int i = 0 ; i < 3 ; i ++) {
		fscanf( infile, "%c", &ch ) ; 
		while ( ch != '\n') 
			fscanf( infile, "%c", &ch ) ;
	} // for
	school t ;
	while ( fscanf( infile, "%s%s%s%s%s%s%s%s%s%s%d%s%s%s%s", &t.school_id, &t.school_name, &t.department_id, &t.department_name, &t.day,
	                                                          &t.day2, &t.leveal, &t.leveal2, &t.student, &t.teacher,
															  &t.graduate, &t.city, &t.city2, &t.type, &t.type2  ) != EOF ) {
		// cout << t.school_id << t.school_name << t.department_id  << t.department_name<< endl ;
		school_list.push_back(t) ;
		temp.push_back(t) ;
	} // while
	
	// 讀完關檔 	
	fclose(infile) ;
} // StoreFile()

void Sellect_sort( string fileName ) {
	int time = clock() ;
	for ( int i = 0 ; i < school_list.size() - 1 ; i ++ ) {
		int max = i;
		for ( int j = i + 1 ; j < school_list.size() ; j++ ) {
			if ( school_list[j].graduate > school_list[max].graduate ) 
				max = j ;
		} // for
		
		swap( school_list[max], school_list[i] ) ; // 把最大的值往前丟 
	} // for
	
	time = clock() - time ;
	cout << "Sellect sort : " << time << " ms" << endl ; 
	
	fileName  = "SelectSorted_" + fileName  + ".txt" ; 
	infile = fopen( fileName.c_str(), "w+b" ) ;
	for ( int i = 0 ; i < school_list.size() ; i ++ ) {
		fprintf( infile, "%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%d%c%s%c%s%c%s%c%s%c", school_list[i].school_id, '\t', school_list[i].school_name, '\t',
										school_list[i].department_id, '\t',school_list[i].department_name,'\t', school_list[i].day, ' ', school_list[i].day2, '\t', 
										school_list[i].leveal, ' ', school_list[i].leveal2, '\t', school_list[i].student, '\t',school_list[i].teacher, '\t', 
										school_list[i].graduate, '\t', school_list[i].city, ' ',school_list[i].city2, '\t', school_list[i].type, ' ', school_list[i].type2, '\n')  ;
	} // for

	fclose( infile ) ;
	school_list.clear() ;
} // Sellect_sort

void Bubble_sort( string fileName ) {
	int time = clock() ;
	bool sorted = false ;
	for ( int i = 0 ; i < temp.size() && !sorted ; i ++ ) {
		sorted = true ;
		for ( int j = 0 ; j < temp.size() - i ; j++ ) {
			if ( temp[j].graduate < temp[j + 1].graduate ) {
				swap( temp[j], temp[j+1] ) ;
				sorted = false ;
			} // if
		} // for
	} // for
	
	time = clock() - time ;
	cout << "Bubble sort : " << time << " ms" << endl ; 
	
	fileName  = "BubbleSorted_" + fileName  + ".txt" ; 
	infile = fopen( fileName.c_str(), "w+b" ) ;
	for ( int i = 0 ; i < temp.size() ; i ++ ) {
		fprintf( infile, "%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%d%c%s%c%s%c%s%c%s%c", temp[i].school_id, '\t', temp[i].school_name, '\t',
										temp[i].department_id, '\t',temp[i].department_name,'\t', temp[i].day, ' ', temp[i].day2, '\t', 
										temp[i].leveal, ' ', temp[i].leveal2, '\t', temp[i].student, '\t',temp[i].teacher, '\t', 
										temp[i].graduate, '\t', temp[i].city, ' ',temp[i].city2, '\t', temp[i].type, ' ', temp[i].type2, '\n')  ;
	} // for

	fclose( infile ) ;
	temp.clear() ;
} // Bubble_sort()

void Merge( int first, int mid, int last ) {
	vector<school> tempVector ;
	school tmp ;
	int first1 = first, last1 = mid ;
	int first2 = mid + 1, last2 = last ;
	int index = first ;
	for ( ; ( first1 <= last1 ) && ( first2 <= last2 ) ; index++ ) {
		if ( school_list[first1].graduate >= school_list[first2].graduate ) {
			tmp = school_list[first1] ;
			tempVector.push_back(tmp) ;
			first1++ ; 
		} // if
		
		else {
			tmp = school_list[first2] ;
			tempVector.push_back(tmp) ;
			first2 ++ ;
		} // else
	} // for
	
	for ( ; first1 <= last1 ; first1++ ) {
		tmp = school_list[first1] ;
		tempVector.push_back(tmp) ;
	} // for
	for ( ; first2 <= last2 ; first2 ++) {
		tmp = school_list[first2] ;
		tempVector.push_back(tmp) ;
	} // for
	
	int i = 0 ;
	for ( index = first ; index <= last ; index++ ) {
		tmp = tempVector[i] ;
		school_list.at( index ) = tmp ;
		i++ ;
	} // for 
} // Merge()

void Merge_sort( int first, int last ) {
	if ( first < last ) {
		int mid = ( first + last ) / 2 ;
		Merge_sort( first, mid ) ;
		Merge_sort( mid + 1, last ) ;
		Merge( first, mid, last ) ;
	} // if
	
} // Merge_sort()

void Print_merge( string fileName ) {
	fileName  = "MergeSorted_" + fileName  + ".txt" ; 
	infile = fopen( fileName.c_str(), "w+b" ) ;
	for ( int i = 0 ; i < school_list.size() ; i ++ ) {
		fprintf( infile, "%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%d%c%s%c%s%c%s%c%s%c", school_list[i].school_id, '\t', school_list[i].school_name, '\t',
										school_list[i].department_id, '\t',school_list[i].department_name,'\t', school_list[i].day, ' ', school_list[i].day2, '\t', 
										school_list[i].leveal, ' ', school_list[i].leveal2, '\t', school_list[i].student, '\t',school_list[i].teacher, '\t', 
										school_list[i].graduate, '\t', school_list[i].city, ' ',school_list[i].city2, '\t', school_list[i].type, ' ', school_list[i].type2, '\n')  ;
	} // for

	fclose( infile ) ;
	school_list.clear() ;
} // Print_merge()

void partition( int first, int last, int & pivotInt ) {
	int lastS1 = first ;
	int unknow = first + 1 ;
	while ( unknow <= last ) {
		if ( temp[unknow].graduate > temp[first].graduate ) {
			lastS1++ ;
			swap( temp[lastS1], temp[unknow]) ;
		} // if
		
			unknow ++ ;
	} // while
	
	swap( temp[first], temp[lastS1] ) ;
	pivotInt = lastS1 ;
} // partition()

void Quick_sort( int first, int last ) {
	int pivotInt ;
	if ( first < last) {
		partition( first, last, pivotInt ) ;
		Quick_sort( first, pivotInt - 1 ) ;
		Quick_sort( pivotInt+1, last) ;
	} // if
} // Quick_sort()

void Print_quick( string fileName){
	fileName  = "QuickSorted_" + fileName  + ".txt" ; 
	infile = fopen( fileName.c_str(), "w+b" ) ;
	for ( int i = 0 ; i < temp.size() ; i ++ ) {
		fprintf( infile, "%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%d%c%s%c%s%c%s%c%s%c", temp[i].school_id, '\t', temp[i].school_name, '\t',
										temp[i].department_id, '\t',temp[i].department_name,'\t', temp[i].day, ' ', temp[i].day2, '\t', 
										temp[i].leveal, ' ', temp[i].leveal2, '\t', temp[i].student, '\t',temp[i].teacher, '\t', 
										temp[i].graduate, '\t', temp[i].city, ' ',temp[i].city2, '\t', temp[i].type, ' ', temp[i].type2, '\n')  ;
	} // for

	fclose( infile ) ;
	temp.clear() ;
} // Pring_quick

void Print_Radix( string fileName ) {
	fileName  = "RadixSorted_" + fileName  + ".txt" ; 
	infile = fopen( fileName.c_str(), "w+b" ) ;
	for ( int i = 0 ; i < school_list.size() ; i ++ ) {
		fprintf( infile, "%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%d%c%s%c%s%c%s%c%s%c", school_list[i].school_id, '\t', school_list[i].school_name, '\t',
										school_list[i].department_id, '\t',school_list[i].department_name,'\t', school_list[i].day, ' ', school_list[i].day2, '\t', 
										school_list[i].leveal, ' ', school_list[i].leveal2, '\t', school_list[i].student, '\t',school_list[i].teacher, '\t', 
										school_list[i].graduate, '\t', school_list[i].city, ' ',school_list[i].city2, '\t', school_list[i].type, ' ', school_list[i].type2, '\n')  ;
	} // for

	fclose( infile ) ;
	school_list.clear() ;
} // Print_Radix
void Radix_sort() {
	vector<school> tempVector[10] ;
	school tmp ;
	int count = 10 ;
	int largest = 0 ;
	for ( int i = 0 ; i < school_list.size() ; i++) 
		if ( largest < school_list[i].graduate )
			largest = school_list[i].graduate ;
			
	for ( int base = 1 ; (largest/base) > 0 ; base*= 10) {
		for ( int i = 0 ; i < school_list.size(); i ++) {
			for ( int j = 0 ; j < 10 ; j ++) {
				if ( ( school_list[i].graduate / base ) % 10 == j ) {
					tmp = school_list[i] ;
					tempVector[j].push_back(tmp) ; 
				} //if	
			} // for
		} // for
	
		school_list.clear() ;
		for ( int i = 9 ; i >= 0 ; i--) {
			for ( int j = 0 ; j < tempVector[i].size() ; j++) {
				tmp = tempVector[i][j] ;
				school_list.push_back(tmp) ;
			} // for
			
			tempVector[i].clear() ;
		} // for
	
	} // for	
} // 
int main() {
	string com = "" ; // 接收指令  
 	while ( 1 ) {                            
		PrintCommand() ;
      	cin >> com ;
   		while ( !IsDigit( com ) || atoi( com.c_str() ) < 0 || atoi( com.c_str() ) > 3 ) { // 排掉奇怪數字 
    		cout << "指令不存在,請重新輸入: " ;
    		cout << endl ; 
       		PrintCommand() ;
       		cin >> com ;
      	} // while
      	
      	if ( com.compare( "0" ) == 0 )
        	exit(0) ;
        	
        if ( com == "1") {
			string s ; // 用來存標題 
	  		cout << "輸入要執行的檔案 :" ; 
	  		string fileName ;
	  		cin >> fileName ;
			while ( !Load(fileName) ) { // 讀到有檔案為止 
				cout << "輸入要執行的檔案 :" ;
				cin >> fileName ;
			} // while
			
			StoreFile() ;
			Sellect_sort( fileName ) ;
			Bubble_sort( fileName ) ;
		} // if
        if ( com == "2") {
        	string s ; // 用來存標題 
	  		cout << "輸入要執行的檔案 :" ; 
	  		string fileName ;
	  		cin >> fileName ;
			while ( !Load(fileName) ) { // 讀到有檔案為止 
				cout << "輸入要執行的檔案 :" ;
				cin >> fileName ;
			} // while
			
			StoreFile() ;
        	int i = 0 ;
        	int time = clock() ;
        	Merge_sort( i, school_list.size() - 1 ) ; 
        	time = clock() - time ;
        	cout << "Merge sort : " << time  << " ms" << endl ; 
        	Print_merge( fileName ) ;
        	i = 0 ; 
        	time = clock() ;
        	Quick_sort( i, temp.size() - 1 ) ;
			time = clock() - time ;
        	cout << "Quick sort : " << time  << " ms" << endl ; 
        	Print_quick( fileName ) ;
        	school_list.clear() ;
		} // if
		
		if (com == "3" ) {
			string s ; // 用來存標題 
	  		cout << "輸入要執行的檔案 :" ; 
	  		string fileName ;
	  		cin >> fileName ;
			while ( !Load(fileName) ) { // 讀到有檔案為止 
				cout << "輸入要執行的檔案 :" ;
				cin >> fileName ;
			} // while
			
			StoreFile() ;
			int time = clock() ;
			Radix_sort() ;
			time = clock() - time ;
			cout << "Radix sort : " << time << " ms" << endl ;
			Print_Radix( fileName) ;
			school_list.clear() ;
		} // if 

		school_list.clear() ;
		temp.clear() ;
	} // while 
} // main()
 
