// 10727127 邱佳駿 10727157 毛琨鈜 
# include <cstdio>
# include <cstring>           
# include <iostream>
# include <stdlib.h>
# include <math.h>

using namespace std ;
unsigned long long int fibonacci[100] ; // 用來存fibonacci 每一項數值 
unsigned long long int count = 0 ;     
unsigned long long int factor = 0 ;  // 
int recursion_count ;
int truecom = -1 ;                      // 以integer的形式存command(0,1,2)       
int tempCount = 0 ;   

bool Isdigit( string input ) {
  for ( int i = 0 ; i < input.size() ; i++ )
    if ( !isdigit( input[i] ) )
	  return false ;
  return true ;
  
  // 輸入的東西只有純數字的狀況才接受 
} // ISdigit() 

unsigned long long int Fibonacci( int n ) {           
  if ( fibonacci[n] != 0  )
    return fibonacci[n] ;
  if ( n == 1 ) {      
    fibonacci[n] = 1 ;                                                                   
    return fibonacci[n] ;
  } // if
  
  else if ( n == 2 ) {
  	fibonacci[n] = 2 ;
    return fibonacci[n] ;
  }// else if
  
  else {
    fibonacci[n] = Fibonacci( n - 1 ) + Fibonacci( n - 2 ) ;
    return fibonacci[n] ;
  }// else
 	
} // Fiboncci()

unsigned long long int Factor( unsigned long long int num, unsigned long long int & factor ) { 
  count++ ; // 記總遞迴次數 
  tempCount++ ;
  if ( num % factor != 0 ) {
    factor-- ;
    if ( tempCount > 10000 ) {
      tempCount = 0 ;
      return factor ;
	} // if
    
    return Factor( num, factor ) ;
     
  } // if

  return factor ;
  
} // Factor()

void Fibonacci_recursion( int i, int num ) {
  unsigned long long int number = 1 ;   // fibonacci 數值 
  number = Fibonacci( i ) ;
  factor = sqrt( number ) ;
  cout << "[ " << i << " ]" <<                       
  " " << number << " = " ;
  factor = Factor( number, factor ) ;	
  while ( number % factor != 0 )
    factor = Factor( number, factor ) ;	
  
  cout << factor << "*" << number/factor << "         " <<
  "(Inner recursion:   " << count << " times)"  <<
  endl ;
  
  recursion_count = i;
  count = 0 ;      
  if ( i < num )
    Fibonacci_recursion( i+1, num ) ;
} // Fibonacci_recursion()

int main() {
  while ( 1 ) { // 無窮 
    for ( int i = 0 ; i < 100 ; i++ )                      
      fibonacci[i] = 0 ;
    // 將每一項初始化
	 
    string com = "" ; // 接收指令 
	                                    
    cout << "費氏數列生產機" << endl  <<
    "*" << " 0. 離開" << endl << "*" <<
    " 1. 迴圈" << endl << "*" << " 2. 遞迴" << endl <<
    "請輸入指令( 0, 1, 2 ):" ; 
    
    cin >> com ;                            
  
    while ( !Isdigit( com ) || atoi( com.c_str() ) < 0 || atoi( com.c_str() ) > 2 ) { // 排掉奇怪數字 
      cout << "指令不存在,請重新輸入" << endl <<
      "費氏數列生產機" << endl  <<
      "*" << " 0. 離開" << endl << "*" <<
      " 1. 迴圈" << endl << "*" << " 2. 遞迴" << endl <<
      "請輸入指令( 0, 1, 2 ):" ;
      cin >> com ;
    } // while  
                          
    truecom = atoi( com.c_str() ) ;  // 確定指令正常 並將指令改為integer 
    	        
    if ( truecom == 0 ) 
      exit(0) ;
    
    // 以下為1或2的command 狀況
	 
    cout << endl << "請輸入1~92之正整數" << endl ;               
    string number = "" ;                        
     cin >> number ;                                          

    // 確認輸入在範圍內 
    while ( !Isdigit( number ) || atoi( number.c_str() ) <= 0 || atoi( number.c_str() ) > 92 ) {   
      cout << "輸入之數字不符規範,請重新輸入" << endl ;   
      cin >> number ;
    } // while
  
    int num = atoi( number.c_str() ) ;                         // String用c_str轉為c字串 再轉為int 
  
    unsigned long long int pre = 1 ;                      // 費氏數列中前一個數字  
    unsigned long long int cur = 1 ;                      // 費氏數列中後一個數字 
    unsigned long long int sum = 1 ;                      // pre+cur 
  
    if ( truecom == 1 ) {    	
      for ( int i = 1 ; i <= num ; i++ ) {   // i為outer loop count         
        cout << "[ " << i << " ]" ;  
        
        // 輸出費氏數列 
        if ( i == 1 )                                                                
          cout << " "<< cur << " = " ;         
        else {
          cout << " " << pre + cur << " = " ;
          //費氏規則 
          sum = pre + cur ;                 
  	      pre = cur ;
  	      cur = sum ;
        } // else 
    
        for ( unsigned long long int j = sqrt( sum ) ; j > 0 ; j-- ) {  // 開根號開始找因數 
          count++ ;    // innerloop count
          if ( sum % j == 0 ) {
            cout << j << "*" << sum/j << "         " <<
            "(Inner loop:   " << count << " times)"  <<
            endl ;
            break ;
          } // if    
        } // for    
		    
        count = 0 ;
      } // for  
    
      cout << " <Outer loop:     " << num - 1 << " times>" << endl ; 
    } // if
  
    if ( truecom == 2 ) {
      Fibonacci_recursion( 1, num ) ; 
      cout << " <Outer recursion:     " << recursion_count << " times>" << endl ;
    } // if
  } // while
} // main()



