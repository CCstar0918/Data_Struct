// 10727127 �����@ 10727157 ��b�r 
# include <cstdio>
# include <cstring>
# include <math.h>
# include <stdlib.h>
# include <iostream>
# include <vector>

using namespace std ;

string trash ;
vector< string > temp ;

class Expression { // �ŧi�@�� pointer 
  public :
	string Str ;
    Expression* next ;
}; // class Expression 

class MyStack {
  private :
    Expression *h ;
  public : 
    MyStack() {
    	h = NULL ;
 	} // MyStack()
 
 	~MyStack() {
   		clear() ;
 	} // ~MyStack
 
	void clear() {
   		while ( !isEmpty() )
     	pop() ;
 	} // clear() 
     
 	void push( string s ) {
	   Expression *temp = new Expression() ;
	   temp->Str = s ;
	   temp->next = h ;
	   h = temp ;
 	} // push()
 
 	string pop() {
		if ( h != NULL ) {
			string temp = h->Str ;
			Expression *Dtemp = h ;
			h = h->next ;
			delete Dtemp ;
			return temp ;
		} // if
  
		else if ( h == NULL )
	    	return "�����D" ;
	      
	} // pop()
 
	string getTop() {
		if ( h != NULL )
			return h->Str ;
	   	else
	  		return "" ;
	} // getTop()
 
	bool isEmpty() {
		return h == NULL ;
	} // isEmpty
 
}; // class MyStack

class Input {
  public : 
    Input () {
      expression = NULL ;
      input = '\0' ;   
    } // Input()
   
 	Expression* expression ;
 	char input ;
  
 	void PrintCommand() {
    	cout << endl << "*" << " 0. ���}" << endl << "*" <<
     	" 1. ���Ǧ��B�⦡�˴�" << endl << "*" << " 2. ���Ǧ����Ǧ�" << endl <<
     	"�п�J���O( 0, 1, 2 ):" ; 
    } // PrintCommand

 	bool isStringDigit( string str ) {
    	for ( int i = 0 ; i < str.length() ; i++ )
     	if ( !isdigit( str[i] ) ) 
    		return false ;
   		return true ;
  	} // isStringDigit()
 
 	string Read() {
    	string str = "" ;
      	char ch = '\0' ;
      	scanf( "%c", &ch ) ;
   		while ( ch != '\n' ) {
	    	str = str + ch ;
	    	scanf( "%c", &ch ) ;  
	    } // while   
	    
      	return str ; 
    } // Read()
 
 	string gettoken() { // ��C��token��J�P�@��string �Ϊťդ��} 

		int i = 0 ;
	   	string read = Read() ;
	   	string str = "" ;
	   	bool num = false ;
    	while ( i < read.length() ){
     		if ( read[i] <= '9' && read[i] >= '0' ) { // �B�z�Ʀr 
       			num = true ;
       			str = str + read[i] ;
  			} // if
      
    		else if ( read[i] == ' ' ) {
  			}  
  			
    		else {
      			if ( num ) {
        			str = str + ' ' ;
        			num = false ;
       			} // if
   
      			str = str + read[i] + ' ' ;
        	} // else
  
  			i++ ;
   		} // while
   		
   		return str ;
   		
 	} // gettoken()
  
    bool IsOperator( string str ) {   
    	if ( str == "+" || str == "-" || str == "*" || str == "/")
        	return true ;
    } // IsOperator()
    
    int inorderTest( string str ) { // �P�_�O�_���X�檺���� 
		int count = 0 ;
		string connect = "" ;
		// �Ntoken�qstring���} ��Jvector�� 
		for ( int i = 0 ; i < str.length() ; i++ ) {
        	if ( str[i] != ' ' ) {
         		connect += str[i] ;
         		if ( i + 1 == str.length() ) // �̫�@��L������J 
           			temp.push_back( connect ) ;      
        	} // if
        
       		else if ( str[i] == ' ' ) {
          		temp.push_back( connect ) ;
         		connect = "" ;
  			} // else if	
   		} // for
   
   		
      	for ( int j = 0 ; j < temp.size() ; j++ ) {
      		
      		// ����~�Ÿ� 
        	if ( !isStringDigit( temp[j] ) && temp[j] != "+" &&
            	temp[j] != "-" && temp[j] != "*" && temp[j] != "/" &&
            	temp[j] != "(" && temp[j] != ")" ) {
          
          		trash = temp[j] ;
         		return 1 ;  // �Ÿ����~ 
        	} // if
        
        	if ( temp[j] == "(" )
         		count ++ ;
       		if ( temp[j] == ")")
         		count -- ;
        	if ( count < 0 )
         		return 21 ;   // �k�A���Ӧh 
       		if ( j+1 < temp.size() && temp[j] == "(" && temp[j+1] == ")" ) // �h�@�լA�� 
         		return 212 ;
       		if ( j > 0 &&  (( temp[j] == "(" && isStringDigit( temp[j-1] ) ) ||
          	( j+1 < temp.size() && temp[j] == ")" && isStringDigit( temp[j+1] ) )   )  )
         		return 32 ;    // �B��ƤӦh
     		if ( j+1 < temp.size() && IsOperator( temp[j]) && IsOperator( temp[j+1]) )
    			return 31 ;   // �B��l�Ӧh 
   		} // for
   
   		if ( count > 0 )
     		return 22 ;   // ���A���Ӧh  
      	if ( temp.size() != 0 && IsOperator(temp[temp.size()-1] ))
     		return 31 ;

    
   		return 0 ; 
 	} // inorderTest()
          
}; // class Input

void Add( Expression *&head, string s ) {
	if ( head == NULL ){
		head = new Expression() ;
   		head->Str = s ;
   		head->next = NULL ;
  	} // if
    
  	else 
   		Add ( head->next, s ) ;  
} // Add()

void Delete( Expression *&head ) {
	if ( head != NULL )
 		Delete( head->next ) ;
  	delete head ;
  	head = NULL ; 
} // Delete()
  

int main() {
  string str ;
  string com ;
  char ch = '\0' ;
  char stemp[256] ;
  int back = -1 ; // �s�B�⦡�����T�ο��~�T�����O 
  Expression *postfix = NULL ;
  Input v ;
  MyStack stack ;
  while ( 1 ) {
	v.PrintCommand() ;
	scanf( "%s%c", stemp, &ch ) ; // Ū�����O��'\n' 
	com = string(stemp) ;
   	while ( !v.isStringDigit( com ) || atoi( com.c_str() ) < 0 || atoi( com.c_str() ) > 2 ) { // �Ʊ��_�ǼƦr 
    	cout << "���O���s�b,�Э��s��J" << endl ;
		cout << endl ; 
		v.PrintCommand() ;
		scanf( "%s%c", stemp, &ch ) ;
		com = string(stemp) ;
    } // while
    
   	if ( com == "0" )
		exit(0) ;
		
	else if ( com == "1" ) {
   		cout << "Input: " ;
		str = v.gettoken() ;
		back = v.inorderTest( str ) ;
		if ( back == 0 )
       		cout << endl <<  "It is a legitimate infix expression." ;
      	if ( back == 1 )
       		cout << endl << "Error 1: " << trash << " is not a legitimate character." ; 
      	if ( back == 21 )
       		cout << endl << "Error 2: there is one extra close parenthesis." ; 
      	if ( back == 22 )
       		cout << endl << "Error 2: there is one extra open parenthesis." ; 
     	if ( back == 212 ) {
			cout << endl << "Error 2: there is one extra open parenthesis." ; 
			cout << endl << "Error 2: there is one extra close parenthesis." ; 
      	} // if
       
     	if ( back == 31 )
       		cout << endl << "Error 3: there is one extra operator." ; 
      	if ( back == 32 )
       		cout << endl << "Error 3: there is one extra operand." ;
    } // if
    
    else if ( com == "2" ) {
    	cout << "Input: " ;
		str = v.gettoken() ;
		back = v.inorderTest( str ) ;
		bool myFirst = true ;
      	if ( back == 1 )
       		cout << endl << "Error 1: " << trash << " is not a legitimate character." ; 
      	if ( back == 21 )
       		cout << endl << "Error 2: there is one extra close parenthesis." ; 
      	if ( back == 22 )
       		cout << endl << "Error 2: there is one extra open parenthesis." ; 
     	if ( back == 212 ) {
       		cout << endl << "Error 2: there is one extra open parenthesis." ; 
       		cout << endl << "Error 2: there is one extra close parenthesis." ; 
      	} // if
       
     	if ( back == 31 )
       		cout << endl << "Error 3: there is one extra operator." ; 
      	if ( back == 32 )
       		cout << endl << "Error 3: there is one extra operand." ;
       		
       	// �S�����~�N�ഫ����� 
      	else { 
       		cout << "Postfix expression: " ;
       		for ( int i = 0 ; i < temp.size() ; i++ ) {
         		if ( v.isStringDigit( temp[i] ) ) // �Ʀr������Jpostfix 
            		Add( postfix, temp[i] ) ;
         		else if ( temp[i] == "(" ) // ���A����Jstack 
           			stack.push( temp[i] ) ;
         		else if ( v.IsOperator( temp[i] ) ) {
         			//  �B�z�Ÿ����p�G����Ÿ��u���סA�p�Gstack�����u���� >= Ū�i�Ӫ��Ÿ��u���סA�Nstack�̤W�h��Jpostfix�A�b�NŪ�i�Ӫ��Ÿ���Jstack 
           			while ( stack.getTop() == "*" || stack.getTop() == "/" || 
           				( ( stack.getTop() == "+" || stack.getTop() == "-" )
      					&& ( temp[i] == "+" || temp[i] == "-" ) ) )
             			Add( postfix, stack.pop() ) ;
           			stack.push( temp[i] ) ;
          		} // else if
             	
             	// �B�z�A�����p�G�@�J��k�A���N��̪񪺥��A���H�����Ҧ�operator��Jpostfix 
         		else if ( temp[i] == ")" ) {
           			while ( stack.getTop() != "(") 
             			Add( postfix, stack.pop() ) ;
           			stack.pop() ; // �R�����A�� 
          		} // else if
          
        	} // for
        
         	while ( stack.getTop() != "" )
           		Add( postfix, stack.pop() ) ;        
   		} // else
   
   		// ��X��� 
	  	for ( Expression *t = postfix ; t != NULL ; t = t->next ) {
			cout << ( myFirst ? "" : ", " ) << t->Str ;
     		myFirst = false ;
   	  	} // for  
    } // else if  
    
    // �k�٪Ŷ� 
    Delete( postfix ) ;
    stack.clear() ;
   	temp.clear() ;
  } // while  
} // main()

