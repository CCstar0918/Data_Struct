// 10727127 邱佳駿 10727157 毛琨鈜
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
int goalnum = 0 ;                   // 題目要求目標數量
int truegoalnum = 0 ;               // 實際走過目標數量 
bool accept = false ;
bool Isdigit( string input ) {
	for ( int i = 0 ; i < input.size() ; i++ )
    	if ( !isdigit( input[i] ) )
   			return false ;
	return true ;
    // 輸入的東西只有純數字的狀況才接受 
} // ISdigit() 

void PrintCommand() {
	cout << "老鼠走迷宮" << endl  <<
    	"*" << " 0. 離開" << endl << "*" <<
    	" 1. One Goal" << endl << "*" << " 2. More Goal" << endl <<
    	"請輸入指令( 0, 1, 2 ):" ; 
} // PrintCommand

class Maze { // 讀取迷宮地圖資料 
	public :
		bool load( string fileName ) { // 長寬判定地圖是否讀取success         
   			FILE * infile = NULL ;     // 讀檔容器 
   			fileName = "input" + fileName + ".txt" ; // 檔案文件格式 
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
   		char Vmaze[50][50] ;                	// 用來紀錄走過路徑的地圖 
   		char Rmaze[50][50] ;                	// 用來紀錄唯一路徑的地圖 
   		bool end ;								// 	判定是否找完所有 Goal 
    	void Equalmap( char maze[50][50] ) {	// clone Map
      		for ( int i = 0 ; i < width ; i ++ )
       			for ( int j = 0 ; j < length ; j ++ )    
       				maze[i][j] = map[i][j] ; 
 		} // Equalmap() 
 
		Mouse() { // 初始化 
    		Equalmap( Vmaze ) ;
			Equalmap( Rmaze ) ; 
			end = false ;
			x = 0 ;
			y = 0 ;
		} // Mouse()
  
  		bool Goalsmove() {  // 多個目標 
  		
  		    // 走過第一格並記錄 
	    	Vmaze[0][0] = 'V' ;  
	   		Rmaze[0][0] = 'R' ;
    		bool findG = false ; // 告訴上一層遞迴已找到G
    		if ( x+1 < length && ( Vmaze[y][x+1] == 'E' || Vmaze[y][x+1] == 'G' ) ) {  // 往右的狀況    
      			if ( Vmaze[y][x+1] == 'G' ) {
       				truegoalnum++ ;
				    Vmaze[y][x+1] = 'V';
				    x++ ; // 座標移動 
       				if ( truegoalnum < goalnum ) // goal是否找完 
         				Goalsmove() ;
        			x-- ;
        			Vmaze[y][x+1] = 'A'; // 避免找到同一個G 
					 
        			findG = true ; 
        			if ( Rmaze[y][x] != 'G' )
          				Rmaze[y][x] = 'R' ;
        			if ( truegoalnum == goalnum ) { // 找到所有目標 
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
    
    		if ( y+1 < width && ( Vmaze[y+1][x] == 'E' || Vmaze[y+1][x] == 'G' ) ) {  // 往下的狀況    
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
    
    		if ( x-1 >= 0 && x-1 < length && (Vmaze[y][x-1] == 'E' || Vmaze[y][x-1] == 'G') ) { //往左的壯況     
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
      
    		if ( y-1 >= 0 && y-1 < width && ( Vmaze[y-1][x] == 'E' || Vmaze[y-1][x] == 'G' ) ) { // 往上的狀況 
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
   
		bool Goalmove() {   // 單一目標 
   			Vmaze[0][0] = 'V' ;
   			Rmaze[0][0] = 'R' ;
    
    		if ( x+1 < length && ( Vmaze[y][x+1] == 'E' || Vmaze[y][x+1] == 'G' ) ) {  // 往右的狀況     
      			if ( Vmaze[y][x+1] == 'G' ) {
       				x++ ;  // 走到G 
        			end = true ; // find Goal 
        			x-- ;  // 走回上一格 
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
    
    		if ( y+1 < width && ( Vmaze[y+1][x] == 'E' || Vmaze[y+1][x] == 'G' ) ) { // 往下的狀況 
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
    
    		if ( x-1 >= 0 && x-1 < length && (Vmaze[y][x-1] == 'E' || Vmaze[y][x-1] == 'G') ) {   // 往左的狀況   
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
      
    		if ( y-1 >= 0 && y-1 < width && ( Vmaze[y-1][x] == 'E' || Vmaze[y-1][x] == 'G' ) ) {  // 往上的狀況 
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
	string com = "" ; // 接收指令  
 	while ( 1 ) {                            
		PrintCommand() ;
      	cin >> com ;
      	if ( com.compare( "0" ) == 0 )
        	exit(0) ;
   		while ( !Isdigit( com ) || atoi( com.c_str() ) < 0 || atoi( com.c_str() ) > 2 ) { // 排掉奇怪數字 
    		cout << "指令不存在,請重新輸入: " ;
    		cout << endl ; 
       		PrintCommand() ;
       		cin >> com ;
      	} // while
      
      	if ( com == "1" ) { // Mission 1 
        	cout << "輸入要執行的檔案: " ; 
     		string fileName ;
     		cin >> fileName ;
     		Maze maze ;
     		while ( !maze.load( fileName ) ){ 
       			cout << "輸入要執行的檔案: " ;
    			cin >> fileName ;
     		} // while
     
     		Mouse v ;

     		accept = v.Goalmove() ; 
     		
			//走過的路徑 
			for ( int i = 0 ; i < width ; i ++ ) {
    			for ( int j = 0 ; j < length ; j++ ) 
   					cout << v.Vmaze[i][j] ; 
  				cout << endl ;
     		} // for
 
     		cout << endl ;
     		
     		// 唯一路徑 
     		if ( accept ) {
	     		for ( int i = 0 ; i < width ; i ++ ) {
	       			for ( int j = 0 ; j < length ; j++ ) 
	      				cout << v.Rmaze[i][j] ;
	  				cout << endl ;
	  			} // for
	     
	     		cout << endl ;
       		} // if
       		
       		else 
			   cout << "確定不存在任何路徑" << endl ;  // 找不到任何目標 
       		
       		accept = false ;
      	}  // if
      
      	else if ( com == "2" ) { // Mission 2
			string Ncom ;
			cout << "輸入目標數N(正整數): " ;
			cin >> Ncom ;
			while ( !Isdigit( Ncom ) || atoi( Ncom.c_str()) <= 0 ) { 
         		cout << "輸入之數字不符規範,請重新輸入: " ; 
         		cin >> Ncom ;
       		} // while
       
       		goalnum = atoi( Ncom.c_str() ) ; 
        	cout << "輸入要執行的檔案: " ; 
     		string fileName ;
     		cin >> fileName ;
     		Maze maze ;
     		while ( !maze.load( fileName ) ){
       			cout << "輸入要執行的檔案: " ;
    			cin >> fileName ;
     		} // while
     
     		Mouse v ; 
     		v.Goalsmove() ; 
     		
     		// 走過的路徑 
  			for ( int i = 0 ; i < width ; i ++ ) {
    			for ( int j = 0 ; j < length ; j++ ) {
     				if ( v.Vmaze[i][j] == 'A' ) // 把原本用來避免重複找到'G'的 A 字元改成 'G' 
       					v.Vmaze[i][j] = 'G' ;
   					cout << v.Vmaze[i][j] ;
  				} // for
  
  				cout << endl ;
     		} // for
 
     		cout << endl ; 
     		
     		//走過Goal的最短路徑 
     		if ( truegoalnum == 0 ) 
     		  cout << "確定不存在任何路徑" ; 
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
