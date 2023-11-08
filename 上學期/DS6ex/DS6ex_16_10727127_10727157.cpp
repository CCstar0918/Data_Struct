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
fstream infile ;
int count = 0 ;
struct school {
	string school_name ;
	string department_name ;
	string day ;
	string leveal ;
	string student ;
	string teacher ;
	string graduate ;
}; // school

vector<school> school_list ;
vector<int> delete_item ;
struct node {
	node *left  ;
	node *right ;
	int data ;
};
node *root = NULL ;
node *root2 = NULL ;

void PrintCommand() {
	cout <<  "*************************************" << endl <<
			 "* 0. 離開                           *" << endl <<
    		 "* 1. Build Two Binary Search Tree   *" << endl <<
			 "* 2. Search By Number Of Graduates  *" << endl << 
			 "* 3. Search By School Name          *" << endl <<
			 "* 4. Delete By School Name          *" << endl <<
			 "*************************************" << endl << 
    "請輸入指令( 0, 1, 2, 3, 4 ):" ; 
} // PrintCommand

bool IsDigit( string input ) {
	for ( int i = 0 ; i < input.size() ; i++ )
    	if ( !isdigit( input[i] ) )
   			return false ;
	return true ;
    // 輸入的東西只有純數字的狀況才接受 
} // IsDigit() 

bool Load( string fileName ) {  // load file        
	fileName = "input" + fileName + ".txt" ; 
	infile.open( fileName.c_str(), ios::in ) ;  
	if ( infile == NULL ) {
		cout << endl << fileName << " does not exist" << endl ;
		return false ;
	} // if

	return true ;
} // Load() 

void StoreFile() {
	string line ;
	// 把多餘的東西讀掉 
	for ( int i = 0 ; i < 3 ; i ++) {
		getline( infile, line ) ;
	} // for
	school t ;
	while ( getline( infile, line, '\t') ) { // school_id 
		getline ( infile, t.school_name , '\t') ; // school_name
		getline ( infile, line, '\t') ; 			   // dempartment_id
		getline ( infile, t.department_name, '\t' ) ;	   // department_name
		getline	( infile, t.day, '\t' ) ;				   // day
		getline ( infile, t.leveal, '\t') ;			   // leveal
		getline ( infile, t.student, '\t') ;			   // student
		getline ( infile, t.teacher, '\t') ;			   // teacher
		getline ( infile, t.graduate, '\t') ;			   // graduate
		getline ( infile, line) ;							   // city and type
		school_list.push_back(t) ;
	} // while
	
	// 讀完關檔 	
	infile.close() ;
} // StoreFile()

void BinaryTree( node *&n , int i, int &a ) {
	if ( n == NULL) {
		n = new node ;
		n->data = i ;
		n->left = NULL ;
		n->right = NULL ;
		a++ ;
	} //if
	else if ( atoi( school_list[n->data].graduate.c_str() ) <= atoi( school_list[i].graduate.c_str() ) ) {
		a++ ;
		BinaryTree( n->right, i, a ) ;
	}
	else {
		a++ ;
		BinaryTree( n->left, i, a ) ;
	}
}//BinaryTree()

void Search_graduate( node *n , int num ) {
	if ( n == NULL )
		;
	else if ( atoi( school_list[n->data].graduate.c_str() ) >= num ) { // 大於這個node走兩邊
		if ( count == 0 )
		cout << "Search results :" << endl ; 
		cout << "[" << count + 1 << "]" << '\t'<<
						school_list[n->data].school_name << '\t'<<
						school_list[n->data].department_name << '\t' <<
						school_list[n->data].day << '\t' << 
						school_list[n->data].leveal << '\t' <<
						school_list[n->data].student << '\t' <<
						school_list[n->data].teacher << '\t' <<
						school_list[n->data].graduate << endl ;
		count++ ;
		Search_graduate( n->left, num) ;	
		Search_graduate( n->right, num) ;
	} // else if
	
	else  // 否則小於這個 node 只走右邊 
		Search_graduate( n->right, num) ;
}// Search_graduate

void NameTree( node *&n , int i, int &b) {
	if ( n == NULL) {
		n = new node ;
		n->data = i ;
		n->left = NULL ;
		n->right = NULL ;
		b++ ;
	} //if
	else if ( school_list[n->data].school_name <= school_list[i].school_name ) {
		b++ ;
		NameTree( n->right, i, b) ;
	}
	else {
		b++ ;
		NameTree( n->left, i, b) ;
	}
}//NameTree()

void Search_name( node *n , string name ) {
	if ( n == NULL )
		;
	else if ( school_list[n->data].school_name == name ) { // 等於這個
		if ( count == 0 )
		cout << "Search results :" << endl ; 
		cout << "[" << count + 1 << "]" << '\t'<<
						school_list[n->data].school_name << '\t'<<
						school_list[n->data].department_name << '\t' <<
						school_list[n->data].day << '\t' << 
						school_list[n->data].leveal << '\t' <<
						school_list[n->data].student << '\t' <<
						school_list[n->data].teacher << '\t' <<
						school_list[n->data].graduate << endl ;
		count++ ;
		Search_name( n->right, name) ;
	} // else if
	
	else if ( school_list[n->data].school_name > name ) {
		Search_name( n->left, name) ;
		Search_name( n->right, name) ;
	} // else if
	else  // 否則小於這個 node 只走右邊 
		Search_name( n->right, name) ;
}// Search_name

void Delete_name( node *n, string name ) {
	if ( n == NULL )
		;
	else if ( school_list[n->data].school_name == name ) { // 等於這個
		if ( count == 0 )
		cout << "Delete Record :" << endl ; 
		cout << "[" << count + 1 << "]" << '\t'<<
						school_list[n->data].school_name << '\t'<<
						school_list[n->data].department_name << '\t' <<
						school_list[n->data].day << '\t' << 
						school_list[n->data].leveal << '\t' <<
						school_list[n->data].student << '\t' <<
						school_list[n->data].teacher << '\t' <<
						school_list[n->data].graduate << endl ;
		count++ ;
		Delete_name( n->right, name) ;	
		delete_item.push_back(n->data) ;
	} // else if
	
	else if ( school_list[n->data].school_name > name ) {
		Delete_name( n->left, name) ;
		Delete_name( n->right, name) ;
	} // else if
	else  // 否則小於這個 node 只走右邊 
		Delete_name( n->right, name) ;
} // Delete_name()

int main() {
	string com = "" ; // 接收指令 
	bool havetree  = false ; 
 	while ( 1 ) {                            
		PrintCommand() ;
      	cin >> com ;
   		while ( !IsDigit( com ) || atoi( com.c_str() ) < 0 || atoi( com.c_str() ) > 4 ) { // 排掉奇怪數字 
    		cout << "指令不存在,請重新輸入: " ;
    		cout << endl ; 
       		PrintCommand() ;
       		cin >> com ;
      	} // while
      	
      	if ( com.compare( "0" ) == 0 ) {
        	school_list.clear() ;
			root = NULL ;
			root2 = NULL ;
			exit(0) ;
		} // if
        	
        if ( com == "1") {
        	school_list.clear() ;
        	delete root ;
        	delete root2 ;
			root = NULL ;
			root2 = NULL ;
			string s ; // 用來存標題 
	  		cout << "輸入要執行的檔案 :" ; 
	  		string fileName ;
	  		cin >> fileName ;
			while ( !Load(fileName) ) { // 讀到有檔案為止 
				cout << "輸入要執行的檔案 :" ;
				cin >> fileName ;
			} // while
			havetree = true ;
			StoreFile() ;
			int Number_max = 0 ;
			int Name_max = 0 ;
			for ( int i = 0 ; i < school_list.size() ; i++ ) {
				int a = 0 ; 
				int b = 0 ; // 紀錄進入幾層迴圈 
				cout << "[" << i + 1 << "]" << '\t'<<
						school_list[i].school_name << '\t'<<
						school_list[i].department_name << '\t' <<
						school_list[i].day << '\t' << 
						school_list[i].leveal << '\t' <<
						school_list[i].student << '\t' <<
						school_list[i].teacher << '\t' <<
						school_list[i].graduate << endl ;
				BinaryTree( root,i, a ) ;
				if ( Number_max < a)
					Number_max = a ;
				NameTree( root2, i, b) ;
				if (Name_max < b )
					Name_max = b ;
			} // for
			cout << endl << "Tree Highs : " << endl ;
			cout << "{Number Of Graduate} = "<< Number_max << endl ;
			cout << "{School Name} = " << Name_max << endl << endl ;
		} // if
        if ( com == "2") {
        	if ( !havetree )
        		cout << endl << "請先執行指令1" << endl << endl ;
			else {
				int number ;
				cout <<"Input the number of graduate :" ;
				cin >> number ;
				Search_graduate( root, number ) ;
				if ( count == 0)
					cout << "There is no match!" << endl ;
				cout << endl ;
			} // else
			
			count = 0 ;
		} // if
		
		if ( com == "3") {
			if ( !havetree )
        		cout << endl << "請先執行指令1" << endl << endl;
			else {
				string name ;
				cout << "Input a school name :" ;
				cin >> name ;
				Search_name( root2, name ) ;
				if ( count == 0)
					cout << "There is no match!" << endl ;
				cout << endl ; 
			} // else
			
			count = 0 ;
		} // if
		
		if ( com == "4") {
			if ( !havetree )
				cout << endl << "請先執行指令1" << endl << endl;
			else {
				string name ;
				cout << "Input a school name :" ;
				cin >> name ;
				cout << endl ;
				delete_item.clear() ;
				Delete_name( root2, name ) ;
				if ( count != 0) {
					for ( int i = 0 ; i < delete_item.size() ; i++ ) { // delete school_list information 
						school_list.erase( school_list.begin() + delete_item[i] ) ;
						for ( int j = i ; j < delete_item.size() ; j++ ) {
							if ( delete_item[i] < delete_item[j] ) // 如果後面要刪的位置大過現在的位置, -1個位置 
								delete_item[j] -- ;
						} // for
					} // for
					
					delete root ;
		        	delete root2 ;
					root = NULL ;
					root2 = NULL ;
					int Number_max = 0 ;
					int Name_max = 0 ;
					for ( int i = 0 ; i < school_list.size() ; i++ ) {
						int a = 0 ; 
						int b = 0 ; // 紀錄進入幾層迴圈 
						BinaryTree( root,i, a ) ;
						if ( Number_max < a)
							Number_max = a ;
						NameTree( root2, i, b) ;
						if (Name_max < b )
							Name_max = b ;
					} // for
					
					cout << endl << "Tree Highs : " << endl ;
					cout << "{Number Of Graduate} = "<< Number_max << endl ;
					cout << "{School Name} = " << Name_max << endl << endl ;
				} // if
				
				else 
					cout << "There is no match!" << endl ;
				cout << endl ; 
			} // else
			
			count = 0 ;
		} // if
	} // while 
} // main()
 
