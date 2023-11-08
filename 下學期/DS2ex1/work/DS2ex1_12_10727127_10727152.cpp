// 10727127 �����@ 10727152 �����l 
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
	int numOfGraduate ;
	int serial_number ;
}; // school

vector<school> school_list ;

void PrintCommand() {
	cout <<  endl << "********  Heap Construction  ********" << endl <<
					 "***                               ***" << endl <<
			         "* 0. ���}                           *" << endl <<
    		         "* 1. Build a min heap               *" << endl <<
			         "* 2. Build a min-max heap           *" << endl << 
			         "*************************************" << endl << 
                     "�п�J���O( 0, 1, 2 ): " ; 
} // PrintCommand

bool IsDigit( string input ) {
	for ( int i = 0 ; i < input.size() ; i++ )
    	if ( !isdigit( input[i] ) )
   			return false ;
	return true ;
} // IsDigit() 

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
    cout << "��J�n���檺�ɮ� : " ; 
    string fileName ;
    cin >> fileName ;
    while ( !Load(fileName) ) { // Ū�즳�ɮ׬��� 
		cout << "��J�n���檺�ɮ� : " ;
		cin >> fileName ;
	} // while
} // ReadFile()

void BuildMinHeap( int node ) {
	// �b���}�C���ƭȥ洫�P����ɭn�`�N node - 1 �~�|���o���T�� index 
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
	// �������إߦbbottom�A�h����`�I����� and �洫 
	while ( getline( infile, line, '\t') ) {           // school_id 
		getline ( infile, line , '\t') ;               // school_name
		getline ( infile, line, '\t') ; 			   // dempartment_id
		getline ( infile, line, '\t' ) ;               // department_name
		getline	( infile, line, '\t' ) ;			   // day
		getline ( infile, line, '\t') ;			       // leveal
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
	// �H2^n �h�P�_ leftMost ����m 
	int leftMost = 1 ;
	i = 0 ;
	while ( leftMost <= school_list.size() ) {
		i++ ;
		leftMost *= 2 ;
	} // while
	
	return leftMost / 2 ;
} // FindLeftMost()

void ChangeWithGrandfather(int node, int floor ) {
	// ��Ƽh�U�p���U�W�� �A ���Ƽh�U�j���U�W��
	if ( floor % 2 == 1 ) { // ��� 
		if ( node / 4 != 0 ) {
			if ( school_list[node - 1].numOfGraduate < school_list[node/4 - 1].numOfGraduate ) {
				swap( school_list[node - 1], school_list[node/4 - 1] ) ;
				node = node / 4 ;
				ChangeWithGrandfather( node, floor ) ;
			} // if
		} // if
	} // if
	
	else { // ���� 
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
	// �b���}�C���ƭȥ洫�P����ɭn�`�N node - 1 �~�|���o���T�� index 
	// ����쪺bottom�b�ĴX�h( �� or �� ) --> �ھ� floor �B�z min or max �P���h���洫 --> �ھڥ洫�᪺ floor �P�����h������i��洫
	// �]���n���W��A�ҥH�b��Ƽh�n�� max �A���Ƽh�n�� min 
	if ( floor % 2 == 1 ) { // ��Ƽh max
		if ( node / 2 != 0 ) {
			if ( school_list[node - 1].numOfGraduate > school_list[node/2 - 1].numOfGraduate ) {
				swap( school_list[node - 1], school_list[node/2 - 1] ) ;
				node = node / 2 ;
				floor-- ;
			} // if
			
			ChangeWithGrandfather( node, floor ) ;
		} // if
	} // if
	
	else { // ���Ƽh min
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
	// �������إߦbbottom�A�h����`�I����� and �洫 
	while ( getline( infile, line, '\t') ) {           // school_id 
		getline ( infile, line , '\t') ;               // school_name
		getline ( infile, line, '\t') ; 			   // dempartment_id
		getline ( infile, line, '\t' ) ;               // department_name
		getline	( infile, line, '\t' ) ;			   // day
		getline ( infile, line, '\t') ;			       // leveal
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

void Print() {
	int i = 0 ;
	int leftMost = FindLeftMost(i) ;
	cout << "root : [" << school_list[0].serial_number << "]" << " " << school_list[0].graduate << endl ;
	cout << "bottom : [" << school_list[school_list.size() - 1].serial_number << "]" << " " << school_list[school_list.size() - 1].graduate << endl ;
	cout << "leftmost bottom : [" << school_list[leftMost - 1].serial_number << "]" << " " << school_list[leftMost - 1].graduate << endl ;
} // PrintMission1()

int main() {
	string com = "" ; // �������O 
	bool havetree  = false ; 
 	while ( 1 ) {                            
		PrintCommand() ;
      	cin >> com ;
   		while ( !IsDigit( com ) || atoi( com.c_str() ) < 0 || atoi( com.c_str() ) > 2 ) { // �Ʊ��_�ǼƦr 
    		cout << "���O���s�b,�Э��s��J: " ;
    		cout << endl ; 
       		PrintCommand() ;
       		cin >> com ;
      	} // while
      	
      	if ( com.compare( "0" ) == 0 ) {
      		exit(0) ;
		} // if
        	
        if ( com == "1") { // Min Heap
        	ReadFile() ;
			MinHeap() ;
			cout << "< min heap >" << endl ;
			Print() ;
        } // if
        
        if ( com == "2") { // Min-Max Heap
        	ReadFile() ;
        	MinMaxHeap() ;
        	cout << "< min-max heap >" << endl ;
        	Print() ;
		} // if
		
	} // while 
} // main()
 
