// 10727127 �����@ 10727157 ��b�r 
# include <cstdio>
# include <cstring>           
# include <iostream>
# include <stdlib.h>
# include <math.h>

using namespace std ;
unsigned long long int fibonacci[100] ; // �ΨӦsfibonacci �C�@���ƭ� 
unsigned long long int count = 0 ;     
unsigned long long int factor = 0 ;  // 
int recursion_count ;
int truecom = -1 ;                      // �Hinteger���Φ��scommand(0,1,2)       
int tempCount = 0 ;   

bool Isdigit( string input ) {
  for ( int i = 0 ; i < input.size() ; i++ )
    if ( !isdigit( input[i] ) )
	  return false ;
  return true ;
  
  // ��J���F��u���¼Ʀr�����p�~���� 
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
  count++ ; // �O�`���j���� 
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
  unsigned long long int number = 1 ;   // fibonacci �ƭ� 
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
  while ( 1 ) { // �L�a 
    for ( int i = 0 ; i < 100 ; i++ )                      
      fibonacci[i] = 0 ;
    // �N�C�@����l��
	 
    string com = "" ; // �������O 
	                                    
    cout << "�O��ƦC�Ͳ���" << endl  <<
    "*" << " 0. ���}" << endl << "*" <<
    " 1. �j��" << endl << "*" << " 2. ���j" << endl <<
    "�п�J���O( 0, 1, 2 ):" ; 
    
    cin >> com ;                            
  
    while ( !Isdigit( com ) || atoi( com.c_str() ) < 0 || atoi( com.c_str() ) > 2 ) { // �Ʊ��_�ǼƦr 
      cout << "���O���s�b,�Э��s��J" << endl <<
      "�O��ƦC�Ͳ���" << endl  <<
      "*" << " 0. ���}" << endl << "*" <<
      " 1. �j��" << endl << "*" << " 2. ���j" << endl <<
      "�п�J���O( 0, 1, 2 ):" ;
      cin >> com ;
    } // while  
                          
    truecom = atoi( com.c_str() ) ;  // �T�w���O���` �ñN���O�אּinteger 
    	        
    if ( truecom == 0 ) 
      exit(0) ;
    
    // �H�U��1��2��command ���p
	 
    cout << endl << "�п�J1~92�������" << endl ;               
    string number = "" ;                        
     cin >> number ;                                          

    // �T�{��J�b�d�� 
    while ( !Isdigit( number ) || atoi( number.c_str() ) <= 0 || atoi( number.c_str() ) > 92 ) {   
      cout << "��J���Ʀr���ųW�d,�Э��s��J" << endl ;   
      cin >> number ;
    } // while
  
    int num = atoi( number.c_str() ) ;                         // String��c_str�ରc�r�� �A�ରint 
  
    unsigned long long int pre = 1 ;                      // �O��ƦC���e�@�ӼƦr  
    unsigned long long int cur = 1 ;                      // �O��ƦC����@�ӼƦr 
    unsigned long long int sum = 1 ;                      // pre+cur 
  
    if ( truecom == 1 ) {    	
      for ( int i = 1 ; i <= num ; i++ ) {   // i��outer loop count         
        cout << "[ " << i << " ]" ;  
        
        // ��X�O��ƦC 
        if ( i == 1 )                                                                
          cout << " "<< cur << " = " ;         
        else {
          cout << " " << pre + cur << " = " ;
          //�O��W�h 
          sum = pre + cur ;                 
  	      pre = cur ;
  	      cur = sum ;
        } // else 
    
        for ( unsigned long long int j = sqrt( sum ) ; j > 0 ; j-- ) {  // �}�ڸ��}�l��]�� 
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



