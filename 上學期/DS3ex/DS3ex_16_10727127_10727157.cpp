// 10727127 邱佳駿 10727157 毛琨鈜 
# include <cstdio>
# include <cstring>
# include <math.h>
# include <stdlib.h>
# include <iostream>
# include <vector>

using namespace std ;

string trash ;
vector< string > temp ;

class Expression { // 宣告一個 pointer 
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
	    	return "有問題" ;
	      
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
    	cout << endl << "*" << " 0. 離開" << endl << "*" <<
     	" 1. 中序式運算式檢測" << endl << "*" << " 2. 中序式轉後序式" << endl <<
     	"請輸入指令( 0, 1, 2 ):" ; 
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
 
 	string gettoken() { // 把每個token放入同一個string 用空白分開 

		int i = 0 ;
	   	string read = Read() ;
	   	string str = "" ;
	   	bool num = false ;
    	while ( i < read.length() ){
     		if ( read[i] <= '9' && read[i] >= '0' ) { // 處理數字 
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
    
    int inorderTest( string str ) { // 判斷是否為合格的中序 
		int count = 0 ;
		string connect = "" ;
		// 將token從string分開 放入vector裡 
		for ( int i = 0 ; i < str.length() ; i++ ) {
        	if ( str[i] != ' ' ) {
         		connect += str[i] ;
         		if ( i + 1 == str.length() ) // 最後一格無條件推入 
           			temp.push_back( connect ) ;      
        	} // if
        
       		else if ( str[i] == ' ' ) {
          		temp.push_back( connect ) ;
         		connect = "" ;
  			} // else if	
   		} // for
   
   		
      	for ( int j = 0 ; j < temp.size() ; j++ ) {
      		
      		// 找錯誤符號 
        	if ( !isStringDigit( temp[j] ) && temp[j] != "+" &&
            	temp[j] != "-" && temp[j] != "*" && temp[j] != "/" &&
            	temp[j] != "(" && temp[j] != ")" ) {
          
          		trash = temp[j] ;
         		return 1 ;  // 符號錯誤 
        	} // if
        
        	if ( temp[j] == "(" )
         		count ++ ;
       		if ( temp[j] == ")")
         		count -- ;
        	if ( count < 0 )
         		return 21 ;   // 右括號太多 
       		if ( j+1 < temp.size() && temp[j] == "(" && temp[j+1] == ")" ) // 多一組括號 
         		return 212 ;
       		if ( j > 0 &&  (( temp[j] == "(" && isStringDigit( temp[j-1] ) ) ||
          	( j+1 < temp.size() && temp[j] == ")" && isStringDigit( temp[j+1] ) )   )  )
         		return 32 ;    // 運算數太多
     		if ( j+1 < temp.size() && IsOperator( temp[j]) && IsOperator( temp[j+1]) )
    			return 31 ;   // 運算子太多 
   		} // for
   
   		if ( count > 0 )
     		return 22 ;   // 左括號太多  
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
  int back = -1 ; // 存運算式為正確或錯誤訊息指令 
  Expression *postfix = NULL ;
  Input v ;
  MyStack stack ;
  while ( 1 ) {
	v.PrintCommand() ;
	scanf( "%s%c", stemp, &ch ) ; // 讀取指令及'\n' 
	com = string(stemp) ;
   	while ( !v.isStringDigit( com ) || atoi( com.c_str() ) < 0 || atoi( com.c_str() ) > 2 ) { // 排掉奇怪數字 
    	cout << "指令不存在,請重新輸入" << endl ;
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
       		
       	// 沒有錯誤就轉換為後序 
      	else { 
       		cout << "Postfix expression: " ;
       		for ( int i = 0 ; i < temp.size() ; i++ ) {
         		if ( v.isStringDigit( temp[i] ) ) // 數字直接放入postfix 
            		Add( postfix, temp[i] ) ;
         		else if ( temp[i] == "(" ) // 左括號放入stack 
           			stack.push( temp[i] ) ;
         		else if ( v.IsOperator( temp[i] ) ) {
         			//  處理符號狀況：比較符號優先度，如果stack中的優先度 >= 讀進來的符號優先度，將stack最上層放入postfix，在將讀進來的符號放入stack 
           			while ( stack.getTop() == "*" || stack.getTop() == "/" || 
           				( ( stack.getTop() == "+" || stack.getTop() == "-" )
      					&& ( temp[i] == "+" || temp[i] == "-" ) ) )
             			Add( postfix, stack.pop() ) ;
           			stack.push( temp[i] ) ;
          		} // else if
             	
             	// 處理括號狀況：一遇到右括號就把最近的左括號以內的所有operator放入postfix 
         		else if ( temp[i] == ")" ) {
           			while ( stack.getTop() != "(") 
             			Add( postfix, stack.pop() ) ;
           			stack.pop() ; // 刪除左括號 
          		} // else if
          
        	} // for
        
         	while ( stack.getTop() != "" )
           		Add( postfix, stack.pop() ) ;        
   		} // else
   
   		// 輸出後序 
	  	for ( Expression *t = postfix ; t != NULL ; t = t->next ) {
			cout << ( myFirst ? "" : ", " ) << t->Str ;
     		myFirst = false ;
   	  	} // for  
    } // else if  
    
    // 歸還空間 
    Delete( postfix ) ;
    stack.clear() ;
   	temp.clear() ;
  } // while  
} // main()

