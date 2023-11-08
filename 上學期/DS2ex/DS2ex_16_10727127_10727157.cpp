// 10727127 �����@ 10727157 ��b�r
#include<stdio.h>
#include<cstdlib>
#include<cstring>
#include<math.h>
#include<iostream>
#include<fstream> 
#include<vector>


using namespace std ;
char map[50][50] ;
int length, width ;
int goalnum = 0 ;                   // �D�حn�D�ؼмƶq
int truegoalnum = 0 ;               // ��ڨ��L�ؼмƶq 
bool accept = false ;
bool Isdigit( string input ) {
	for ( int i = 0 ; i < input.size() ; i++ )
    	if ( !isdigit( input[i] ) )
   			return false ;
	return true ;
    // ��J���F��u���¼Ʀr�����p�~���� 
} // ISdigit() 

void PrintCommand() {
	cout << "�ѹ����g�c" << endl  <<
    	"*" << " 0. ���}" << endl << "*" <<
    	" 1. One Goal" << endl << "*" << " 2. More Goal" << endl <<
    	"�п�J���O( 0, 1, 2 ):" ; 
} // PrintCommand

class Maze { // Ū���g�c�a�ϸ�� 
	public :
		bool load( string fileName ) { // ���e�P�w�a�ϬO�_Ū��success         
   			FILE * infile = NULL ;     // Ū�ɮe�� 
   			fileName = "input" + fileName + ".txt" ; // �ɮפ��榡 
   			infile = fopen( fileName.c_str(), "r" ) ;  // file open  
   			if ( infile == NULL) {
    			cout << endl << fileName << " does not exist" << endl ;
    			return false ;
   			} // if
   
   			else { // read map
    			char ch ;
    			fscanf(infile,"%d", &length) ;
    			fscanf(infile,"%d", &width) ;
    			fscanf(infile, "%c", &ch ) ; // read '\n'
    			while ( ch != '\n')
    				fscanf(infile, "%c", &ch ) ;
    			for ( int i = 0 ; i < width ; i ++ ) {
     				for ( int j = 0 ; j <  length; j++ ) {
     					fscanf( infile, "%c", &ch ) ;
      					map[i][j] = ch ;
     				} // for
     
    				
    				int eof = 0 ;
    				while ( ch != '\n' && eof != EOF )
    					eof = fscanf(infile, "%c", &ch ) ;
    			} // for
    
    			fclose(infile) ;  // file close
    			return true ;
   			} // else
      
		} // load()
}; // maze

class Mouse { 
	public :
   		int x ;
   		int y ;
   		char Vmaze[50][50] ;                	// �ΨӬ������L���|���a�� 
   		char Rmaze[50][50] ;                	// �ΨӬ����ߤ@���|���a�� 
   		bool end ;								// 	�P�w�O�_�䧹�Ҧ� Goal 
    	void Equalmap( char maze[50][50] ) {	// clone Map
      		for ( int i = 0 ; i < width ; i ++ )
       			for ( int j = 0 ; j < length ; j ++ )    
       				maze[i][j] = map[i][j] ; 
 		} // Equalmap() 
 
		Mouse() { // ��l�� 
    		Equalmap( Vmaze ) ;
			Equalmap( Rmaze ) ; 
			end = false ;
			x = 0 ;
			y = 0 ;
		} // Mouse()
  
  		bool Goalsmove() {  // �h�ӥؼ� 
  		
  		    // ���L�Ĥ@��ðO�� 
	    	Vmaze[0][0] = 'V' ;  
	   		Rmaze[0][0] = 'R' ;
    		bool findG = false ; // �i�D�W�@�h���j�w���G
    		if ( x+1 < length && ( Vmaze[y][x+1] == 'E' || Vmaze[y][x+1] == 'G' ) ) {  // ���k�����p    
      			if ( Vmaze[y][x+1] == 'G' ) {
       				truegoalnum++ ;
				    Vmaze[y][x+1] = 'V';
				    x++ ; // �y�в��� 
       				if ( truegoalnum < goalnum ) // goal�O�_�䧹 
         				Goalsmove() ;
        			x-- ;
        			Vmaze[y][x+1] = 'A'; // �קK���P�@��G 
					 
        			findG = true ; 
        			if ( Rmaze[y][x] != 'G' )
          				Rmaze[y][x] = 'R' ;
        			if ( truegoalnum == goalnum ) { // ���Ҧ��ؼ� 
          				end = true ;
          				return true ;  
        			} // if
      			} // if
      
      			else if ( Vmaze[y][x+1] == 'E') {
       				Vmaze[y][x+1] ='V' ;
       				x++ ;
        			findG = Goalsmove() || findG ;
        			x-- ;
        
        			if ( findG )
         				if ( Rmaze[y][x] != 'G' )
             				Rmaze[y][x] = 'R' ;
        			if ( end )
          				return true ;
      			} // else if
      
			} // if
    
    		if ( y+1 < width && ( Vmaze[y+1][x] == 'E' || Vmaze[y+1][x] == 'G' ) ) {  // ���U�����p    
      			if ( Vmaze[y+1][x] == 'G' ){
       				truegoalnum++ ;
       				Vmaze[y+1][x] = 'V';
        			y++ ;
        			if ( truegoalnum < goalnum )
         				Goalsmove() ;
        			y-- ;
        			Vmaze[y+1][x] = 'A';
        			findG = true ; 
        			if ( Rmaze[y][x] != 'G' )
          				Rmaze[y][x] = 'R' ;
        			if ( truegoalnum == goalnum ) {
          				end = true ;
          				return true ;
        			} // if
      			} // if
      
      			else if ( Vmaze[y+1][x] == 'E') {
       				Vmaze[y+1][x] ='V' ;
     				y++ ;
        			findG = Goalsmove() || findG ; 
        			y-- ;
        
        		if ( findG )
         			if ( Rmaze[y][x] != 'G' )
            			Rmaze[y][x] = 'R' ;
        		if ( end )
          			return true ;
      			} // else if   
      		
    		} // if
    
    		if ( x-1 >= 0 && x-1 < length && (Vmaze[y][x-1] == 'E' || Vmaze[y][x-1] == 'G') ) { //���������p     
      			if ( Vmaze[y][x-1] == 'G' ){
       				truegoalnum++ ;
       				Vmaze[y][x-1] = 'V';
        			x-- ;
        			if ( truegoalnum < goalnum )
         				Goalsmove() ;
        			x++ ;
        			Vmaze[y][x-1] = 'A' ;
        			findG = true ; 
        			if ( Rmaze[y][x] != 'G' )
          				Rmaze[y][x] = 'R' ;       
        			if ( truegoalnum == goalnum ) {
          				end = true ;
          				return true ;
        			} // if
      			} // if
      
      			else if ( Vmaze[y][x-1] == 'E') {
       				Vmaze[y][x-1] ='V' ;
     				x-- ;
        			findG = Goalsmove() || findG ; 
        			x++ ;     
        			if ( findG )
         				if ( Rmaze[y][x] != 'G' )
             				Rmaze[y][x] = 'R' ;
        			if ( end )
          				return true ; 
      			} // else if
			} // if
      
    		if ( y-1 >= 0 && y-1 < width && ( Vmaze[y-1][x] == 'E' || Vmaze[y-1][x] == 'G' ) ) { // ���W�����p 
      			if ( Vmaze[y-1][x] == 'G' ) {
       				truegoalnum++ ;
       				Vmaze[y-1][x] = 'V' ;
        			y-- ;
        			if ( truegoalnum < goalnum )
         				Goalsmove() ;
        			y++ ;
        			Vmaze[y-1][x] = 'A' ;
        			findG = true ; 
	        		if ( Rmaze[y][x] != 'G' )
	          			Rmaze[y][x] = 'R' ;
	        		if ( truegoalnum == goalnum ) {
	          			end = true ;
	          			return true ;
	        		} // if
      			} // if
      
      			else if ( Vmaze[y-1][x] == 'E') {
       				Vmaze[y-1][x] ='V' ;
     				y-- ;
        			findG = Goalsmove() || findG ;
        			y++ ;
        			if ( findG )
         				if ( Rmaze[y][x] != 'G' )
             				Rmaze[y][x] = 'R' ;
        			if ( end )
          				return true ; 
      			} // else if
      			
			} // if
    
    		return findG ;
     
		} // Goalsmove()
   
		bool Goalmove() {   // ��@�ؼ� 
   			Vmaze[0][0] = 'V' ;
   			Rmaze[0][0] = 'R' ;
    
    		if ( x+1 < length && ( Vmaze[y][x+1] == 'E' || Vmaze[y][x+1] == 'G' ) ) {  // ���k�����p     
      			if ( Vmaze[y][x+1] == 'G' ) {
       				x++ ;  // ����G 
        			end = true ; // find Goal 
        			x-- ;  // ���^�W�@�� 
        			Rmaze[y][x] = 'R' ;
        			return true ;
      			} // if
      
      			else if ( Vmaze[y][x+1] == 'E') {
       				Vmaze[y][x+1] ='V' ;
       				x++ ;
        			Goalmove() ;
        			x-- ;     
        			if ( end ) {
          				Rmaze[y][x] = 'R' ;
          				return true ;
        			} // if
        		} // else if
      		} // if
    
    		if ( y+1 < width && ( Vmaze[y+1][x] == 'E' || Vmaze[y+1][x] == 'G' ) ) { // ���U�����p 
      			if ( Vmaze[y+1][x] == 'G' ){
			        y++ ;
			        end = true ;
			        y-- ;
			        Rmaze[y][x] = 'R' ;
     				return true ;
      			} // if
      
      			else if ( Vmaze[y+1][x] == 'E') {
       				Vmaze[y+1][x] ='V' ;
     				y++ ;
        			Goalmove() ; 
        			y-- ;       
        			if ( end ) {
          				Rmaze[y][x] = 'R' ;
          				return true ;
        			} // if
				} // else if
      
    		} // if
    
    		if ( x-1 >= 0 && x-1 < length && (Vmaze[y][x-1] == 'E' || Vmaze[y][x-1] == 'G') ) {   // ���������p   
				if ( Vmaze[y][x-1] == 'G' ){
		        	x-- ;
		        	end = true ;
		        	x++ ;
		        	Rmaze[y][x] = 'R' ;
     				return true ;
      			} // if
      
      			else if ( Vmaze[y][x-1] == 'E') {
       			Vmaze[y][x-1] ='V' ;
     			x-- ;
        		Goalmove() ; 
        		x++ ;
        		if ( end ) {
          		Rmaze[y][x] = 'R' ;
          		return true ;
        		} // if
        		
        		} // else if
    		} // if
      
    		if ( y-1 >= 0 && y-1 < width && ( Vmaze[y-1][x] == 'E' || Vmaze[y-1][x] == 'G' ) ) {  // ���W�����p 
      			if ( Vmaze[y-1][x] == 'G' ) {
        			y-- ;
        			end = true ;
        			y++ ;
        			Rmaze[y][x] = 'R' ;
     				return true ;
      			} // if
      
      			else if ( Vmaze[y-1][x] == 'E') {
       				Vmaze[y-1][x] = 'V' ;
     				y-- ;
        			Goalmove() ;
        			y++ ;     
        			if ( end ) {
          				Rmaze[y][x] = 'R' ;
          				return true ;
        			} // if
        
      			} // else if
    		} // if
    
    		return false ;
     
  		} // Goalmove
}; // class Mouse

int main() {
	string com = "" ; // �������O  
 	while ( 1 ) {                            
		PrintCommand() ;
      	cin >> com ;
      	if ( com.compare( "0" ) == 0 )
        	exit(0) ;
   		while ( !Isdigit( com ) || atoi( com.c_str() ) < 0 || atoi( com.c_str() ) > 2 ) { // �Ʊ��_�ǼƦr 
    		cout << "���O���s�b,�Э��s��J: " ;
    		cout << endl ; 
       		PrintCommand() ;
       		cin >> com ;
      	} // while
      
      	if ( com == "1" ) { // Mission 1 
        	cout << "��J�n���檺�ɮ�: " ; 
     		string fileName ;
     		cin >> fileName ;
     		Maze maze ;
     		while ( !maze.load( fileName ) ){ 
       			cout << "��J�n���檺�ɮ�: " ;
    			cin >> fileName ;
     		} // while
     
     		Mouse v ;

     		accept = v.Goalmove() ; 
     		
			//���L�����| 
			for ( int i = 0 ; i < width ; i ++ ) {
    			for ( int j = 0 ; j < length ; j++ ) 
   					cout << v.Vmaze[i][j] ; 
  				cout << endl ;
     		} // for
 
     		cout << endl ;
     		
     		// �ߤ@���| 
     		if ( accept ) {
	     		for ( int i = 0 ; i < width ; i ++ ) {
	       			for ( int j = 0 ; j < length ; j++ ) 
	      				cout << v.Rmaze[i][j] ;
	  				cout << endl ;
	  			} // for
	     
	     		cout << endl ;
       		} // if
       		
       		else 
			   cout << "�T�w���s�b������|" << endl ;  // �䤣�����ؼ� 
       		
       		accept = false ;
      	}  // if
      
      	else if ( com == "2" ) { // Mission 2
			string Ncom ;
			cout << "��J�ؼм�N(�����): " ;
			cin >> Ncom ;
			while ( !Isdigit( Ncom ) || atoi( Ncom.c_str()) <= 0 ) { 
         		cout << "��J���Ʀr���ųW�d,�Э��s��J: " ; 
         		cin >> Ncom ;
       		} // while
       
       		goalnum = atoi( Ncom.c_str() ) ; 
        	cout << "��J�n���檺�ɮ�: " ; 
     		string fileName ;
     		cin >> fileName ;
     		Maze maze ;
     		while ( !maze.load( fileName ) ){
       			cout << "��J�n���檺�ɮ�: " ;
    			cin >> fileName ;
     		} // while
     
     		Mouse v ; 
     		v.Goalsmove() ; 
     		
     		// ���L�����| 
  			for ( int i = 0 ; i < width ; i ++ ) {
    			for ( int j = 0 ; j < length ; j++ ) {
     				if ( v.Vmaze[i][j] == 'A' ) // ��쥻�Ψ��קK���Ƨ��'G'�� A �r���令 'G' 
       					v.Vmaze[i][j] = 'G' ;
   					cout << v.Vmaze[i][j] ;
  				} // for
  
  				cout << endl ;
     		} // for
 
     		cout << endl ; 
     		
     		//���LGoal���̵u���| 
     		if ( truegoalnum == 0 ) 
     		  cout << "�T�w���s�b������|" ; 
     		else if ( truegoalnum == goalnum ) {
       			for ( int i = 0 ; i < width ; i ++ ) {
         			for ( int j = 0 ; j < length ; j++ ) 
        				cout << v.Rmaze[i][j] ;
            		cout << endl ;
       			} // for
       			
       			cout << endl ;  
        	} // else if
       
		} // else if
      
      	truegoalnum = 0 ;
	}  // while
} // main()
