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
#include<stack>
#define LINESZ 1024
#define PTR_NUM 3
#define KEY_NUM PTR_NUM - 1 
using namespace std ;
fstream infile ;


struct slotT{
	vector<int> rSet;
	vector<string> print_line;
	string key;		
};

struct slotA{
	vector<int> rSet;
	vector<string> print_line;
	int key;		
};
	
struct nodeT{
	slotT data[KEY_NUM];
	nodeT *link[PTR_NUM];
	nodeT *parent;
};	
	
struct pointT{
	nodeT *pNode;
	int pidx;
};
	
struct blockT{
	slotT slot;
	nodeT *link;
};

struct branch {
	slotA data;
	branch *left;
	branch *right;
};

class TTtree {
	

		
	int serial_num ;
	fstream infile ;
	vector<int> Set;
	vector<int> graduate_num ;
	vector<string> line_data;
	nodeT *root;
	
	bool Load( string fileName ) {  // load file        
		fileName = "input" + fileName + ".txt" ; 
		infile.open( fileName.c_str(), ios::in ) ;  
		if ( infile == NULL ) {
			cout << endl << fileName << " does not exist !! " << endl ;
			return false ;
		} // if

		return true ;
	} // Load() 
	
	
	nodeT *CreateNode( nodeT *left,nodeT *right, nodeT *pNode, slotT newS ) {
		
		nodeT *newNode = NULL;
		
		try {
			newNode = new nodeT;
			newNode->data[0].rSet = newS.rSet;    
			newNode->data[1].rSet.clear();
			newNode->data[0].print_line = newS.print_line;
			newNode->data[1].print_line.clear();
			newNode->data[0].key = newS.key;
			newNode->data[1].key = "";
			newNode->parent = pNode;
			newNode->link[0] = left;
			newNode->link[1] = right;
			newNode->link[2] = NULL;			
		} // try
		catch ( std::bad_alloc& ba) {
			std::cerr << endl << "bad_ alloc caught:" << ba.what() << endl;
		} // catch
		
		return newNode;
		
	} // CreateNode()
	
	void InsertLeaf( slotT newS, pointT aLeaf ) {
		
		for( int i = KEY_NUM-1; i>= aLeaf.pidx; i-- ) {
			if( i > aLeaf.pidx ) {
				aLeaf.pNode->data[i].rSet = aLeaf.pNode->data[i-1].rSet;
				aLeaf.pNode->data[i].key = aLeaf.pNode->data[i-1].key;
				aLeaf.pNode->data[i].print_line = aLeaf.pNode->data[i-1].print_line;
			} // if
			else if( i == aLeaf.pidx ) {
				aLeaf.pNode->data[i].rSet = newS.rSet;
				aLeaf.pNode->data[i].key = newS.key;
				aLeaf.pNode->data[i].print_line = newS.print_line;
			} // else if
			else 
				break;
		} // for	
	} // InsertLeaf()
	
	void SplitLeaf( slotT newS, pointT aLeaf, blockT &aBlock ) {
		slotT buf[PTR_NUM];
		int idx = 0;
		for( int i = 0; i < PTR_NUM; i++ ) {
			buf[i].rSet = ( i == aLeaf.pidx )? newS.rSet : aLeaf.pNode->data[idx].rSet;
			buf[i].print_line = ( i == aLeaf.pidx )? newS.print_line : aLeaf.pNode->data[idx].print_line;
			buf[i].key = (i == aLeaf.pidx )? newS.key : aLeaf.pNode->data[idx++].key;
		} // for
		
		aLeaf.pNode->data[0].rSet = buf[0].rSet;
		aLeaf.pNode->data[0].print_line = buf[0].print_line;
		aLeaf.pNode->data[0].key = buf[0].key;
		for( int i = 1; i < KEY_NUM; i++ ) {
			aLeaf.pNode->data[i].rSet.clear();
			aLeaf.pNode->data[i].print_line.clear();
		} // for
		
		aBlock.link = CreateNode( NULL, NULL, aLeaf.pNode->parent, buf[2] );

		aBlock.slot.rSet = buf[1].rSet;
		aBlock.slot.key = buf[1].key;
		aBlock.slot.print_line = buf[1].print_line;
		
		
	} // SplitLeaf()
	
	nodeT *CreateRoot( nodeT *left, nodeT *right, slotT oneSlot ) {
		
		nodeT *newRoot = CreateNode( left,right,NULL, oneSlot );
		
		left->parent = newRoot;
		
		right->parent = newRoot;

		return newRoot;
		
	} // CreateRoot()
	
	void InsertNonLeaf( blockT &oneB, pointT goal ) {
		
		if( goal.pidx == 0 ) {  // 插入非葉節點時 是插入第一個位置，必須得把原來的資料往右移 
			goal.pNode->data[1].rSet = goal.pNode->data[0].rSet;
			goal.pNode->data[1].print_line = goal.pNode->data[0].print_line;
			goal.pNode->data[1].key = goal.pNode->data[0].key; 
			goal.pNode->data[0].rSet = oneB.slot.rSet;
			goal.pNode->data[0].print_line = oneB.slot.print_line;
			goal.pNode->data[0].key = oneB.slot.key;
			goal.pNode->link[2] = goal.pNode->link[1];
			goal.pNode->link[1] = oneB.link;
		} // if
		else {      // 是插入第二個位置 
			goal.pNode->data[1].rSet = oneB.slot.rSet;
			goal.pNode->data[1].print_line = oneB.slot.print_line;
			goal.pNode->data[1].key = oneB.slot.key;
			goal.pNode->link[2] = oneB.link;
		} // else
		
		
	} // InsertNonLeaf()
	
	void SplitNonLeaf( pointT goal, blockT &oneB ) {
		
		slotT buf[PTR_NUM];
		nodeT *ptr[PTR_NUM+1];
		int idx = 0;

		for( int i = 0; i < PTR_NUM; i++ ) {
			buf[i].rSet = ( i == goal.pidx )? oneB.slot.rSet : goal.pNode->data[idx].rSet;
			buf[i].print_line = ( i == goal.pidx )? oneB.slot.print_line : goal.pNode->data[idx].print_line;
			buf[i].key = (i == goal.pidx )? oneB.slot.key : goal.pNode->data[idx++].key;
		} // for	
		
		if( goal.pidx == 0 ) {
			ptr[0] = goal.pNode->link[0];
			ptr[1] = oneB.link;
			ptr[2] = goal.pNode->link[1];
			ptr[3] = goal.pNode->link[2];
		} // if
		else if( goal.pidx == 1 ) {
			ptr[0] = goal.pNode->link[0];
			ptr[1] = goal.pNode->link[1];
			ptr[2] = oneB.link;
			ptr[3] = goal.pNode->link[2];
		} // else if
		else if( goal.pidx == 2 ) {
			ptr[0] = goal.pNode->link[0];
			ptr[1] = goal.pNode->link[1];
			ptr[2] = goal.pNode->link[2];
			ptr[3] = oneB.link;
		} // else if
		
		goal.pNode->data[0].rSet = buf[0].rSet; // 2
		goal.pNode->data[0].print_line = buf[0].print_line;
		goal.pNode->data[0].key = buf[0].key;
		for( int i = 1; i < KEY_NUM; i++ ) {
			goal.pNode->data[i].rSet.clear();
		} // for		
		
		goal.pNode->link[0] = ptr[0];
		goal.pNode->link[1] = ptr[1];
		goal.pNode->link[2] = NULL;
		
		
		nodeT *newSibling = CreateNode( ptr[2],ptr[3],goal.pNode->parent, buf[2]);
		oneB.link = newSibling;
		oneB.slot.rSet = buf[1].rSet;
		oneB.slot.print_line = buf[1].print_line;	
		oneB.slot.key = buf[1].key;
	} // SplitNonLeaf()
	
	
	void Insert23Tree( int newRid, string newKey,char need[] ) {		
	
		slotT newSlot;            // 新東西進來 將它存在一個slot裡面 
		newSlot.rSet.push_back( newRid );
		newSlot.print_line.push_back( need );
		newSlot.key = newKey;
 
		
		if( root == NULL ) {    // 如果是空樹的話 
			
			root = CreateNode( NULL, NULL, NULL, newSlot );  
		} // if
		else {                // 不是空樹 
			
			stack<pointT> aPath;  // 用來記錄訪問節點時走過的路 
			pointT curP;		  // 紀錄最後訪物的節點
			blockT blockUp;		  
			
			SearchPath( newKey, aPath );   
			if( !aPath.empty() ) {
				curP = aPath.top();     // 找到最後訪問的節點 
				if( (curP.pNode->data[curP.pidx].rSet.size()) && ( !newKey.compare(curP.pNode->data[curP.pidx].key) ) )  {  // 節點裡走就有這個名稱了 
					curP.pNode->data[curP.pidx].rSet.push_back( newRid );   // 將序號&行資訊加入即可 
					curP.pNode->data[curP.pidx].print_line.push_back( need );   
				} // if
				else if( !curP.pNode->data[KEY_NUM-1].rSet.size() ) {      // 節點裡沒這個名字且節點只有1個key 
					InsertLeaf( newSlot, curP );                           // 直接加入此節點成為第二個key 
				} // else if
				else {                                                     // 節點裡沒這個名字且有兩個key，那就得分裂了
					SplitLeaf( newSlot, curP, blockUp );
					if( curP.pNode->parent == NULL ) {   // 分裂的是root
						
						root = CreateRoot( curP.pNode, blockUp.link, blockUp.slot ); 
						
					} // if
					else {
						do {
							aPath.pop();
							curP = aPath.top();
							if( !curP.pNode->data[KEY_NUM-1].rSet.size() ) {    // 如果加進去KEY只有兩個 那就停止分裂 
								InsertNonLeaf( blockUp, curP );
								break;
							} // if
							else {                                              // 加進去KEY有3個 
								SplitNonLeaf( curP, blockUp ); 
								if( curP.pNode->parent == NULL ) {   // 分裂的是root
									root = CreateRoot( curP.pNode, blockUp.link, blockUp.slot ); 
									break;
								} // if
							} // else
						} while ( true );
					}
						 
				} // else
				
			} // if
			 
		} // else
			
	} // Insert23Tree
		
	void SearchPath( string name, stack<pointT> &Path ) {
		
		pointT oneP;
		int pos;		
		nodeT *cur = root; // 負責去走節點 
		while( cur != NULL ) {
			oneP.pNode = cur;			
			for( pos=0; pos<KEY_NUM; pos++ ) {
				if( ( !cur->data[pos].rSet.size() ) || ( name.compare( cur->data[pos].key )  < 0 ) ) {
					break;
				} // if
				else if( !name.compare( cur->data[pos].key )) {
					
					oneP.pidx = pos;
					Path.push( oneP );
					return ;
				} // else if
								
			} // for
						
			oneP.pidx = pos;
			Path.push( oneP );
			cur = cur->link[pos];
			
		} // while 
				
	} // SearchPath
	
	void Free23Tree( nodeT *root ) {		
		if ( root != NULL ) {
			Free23Tree( root->link[0] );
			for( int i = 0; i < KEY_NUM; i++ ) {
				if( !root->data[i].rSet.size() )
					break;
					
				Free23Tree( root->link[i+1] );
			} // for
			
			delete root;
		} // if
	} // GoodbyeTree
		
	public:	
	
		TTtree() {
			root = NULL;
			serial_num = 1;

		}
		~TTtree() {
			Set.clear();
			graduate_num.clear();
			line_data.clear();
			Free23Tree( root );
		}

		void ReadFile() {
  		  cout << "輸入要執行的檔案 : " ; 
  		  string fileName ;
  		  cin >> fileName ;
  		  while ( !Load(fileName) ) { // 讀到有檔案為止 
				cout << "輸入要執行的檔案 : " ;
				cin >> fileName ;
			} // while
		} // ReadFile()	

		void Build23Tree() {
			
			string line ;		// read three lines text
			int graduate;
			char need[LINESZ];
			string name ;       // read the school name
			for ( int i = 0 ; i < 3 ; i ++) { 
				getline( infile, line ) ;
			} // for
		
			while ( getline( infile, line, '\t') ) {            // school_id 
				char need[LINESZ] = "";     				    // put the data we want inside
				getline ( infile, name , '\t') ;                // school_name
				strcat( need, name.c_str() );
				strcat( need, ", ")	;				
				getline ( infile, line, '\t') ; 			    // dempartment_id
				getline ( infile, line, '\t' ) ;                // department_name
				strcat( need, line.c_str() );
				strcat( need, ", ")	;
				getline	( infile, line, '\t' ) ;			    // day
				strcat( need, line.c_str() );
				strcat( need, ", ")	;
				getline ( infile, line, '\t') ;			        // level
				strcat( need, line.c_str() );
				strcat( need, ", ")	;
				getline ( infile, line, '\t') ;		            // student
				getline ( infile, line, '\t') ;		            // teacher
				getline ( infile, line, '\t') ;		            // graduate
				strcat( need, line.c_str() );
				graduate = atoi( line.c_str() );
				getline ( infile, line) ;					    // city and type
				Set.push_back( serial_num );
				graduate_num.push_back( graduate );
				line_data.push_back( need );
				Insert23Tree( serial_num, name,need );	
				serial_num++;			
			} // while
		
			infile.close() ;		
		
		} // Build23Tree()
		
		int Count_23TreeHeight( nodeT *root ) {
			
			int count = 1;

			if( root->link[0] != NULL )
				count = count + Count_23TreeHeight( root->link[0]);
				
			return count;
			
		} // Count_2-3TreeHeight()
		
		void Count_nodes( nodeT *root, int &nodes ) {
			
			nodeT *walk = root ;
			if(  root != NULL )
				nodes++;
			
			if( root->link[0] != NULL )
				Count_nodes( root->link[0], nodes );
			if( root->link[1] != NULL )	
				Count_nodes( root->link[1], nodes );
			if( root->link[2] != NULL )
				Count_nodes( root->link[2], nodes );
				
				
		} // Count_nodes()
		
		void PrintAnswer() {
			
			int height = 0;
			int num_of_nodes = 0;
			height = Count_23TreeHeight( root );
			Count_nodes( root, num_of_nodes );
			
			
			if( root != NULL ) {
			
				cout << endl << "Tree Height = " << height ;
				cout << endl << "Number of nodes = " << num_of_nodes << endl;
					
				int i = 0;
				if( root->data[1].rSet.size() != 0 ) {
					while( i < root->data[1].rSet.size() ) {
						cout << i+1 << ": ";
						cout << "[" << root->data[1].rSet[i] << "] ";
						cout << root->data[1].print_line[i] << endl;
						i++;
					} // while					
				} // if					
										
				int j = 0;
				while( j < root->data[0].rSet.size() ) {
					cout << i+j+1 << ": ";
					cout << "[" << root->data[0].rSet[j] << "] ";
					cout << root->data[0].print_line[j] << endl;
					j++;
				} // while
				

				
			} // if
		
			
		} // PrintAnswer()
		
		vector<int> GetSet() {
			return Set;
		} // GetSet()
		
		vector<int> GetG() {
			return graduate_num ;
		} // GeTG()
		
		vector<string> GetLine() {
			return line_data;
		} // GetLine()
					
};



class AVLtree {
	
	branch *root;
	vector<int> Set;
	vector<int> graduate_num ;
	vector<string> line_data;
	
	void InsertAVLTree( int i,branch *&root ) {
		
		if( root == NULL ) {   // 找到最後沒找到 所以建一個新節點
			root = new branch;
			root->data.key = graduate_num[i];
			root->data.rSet.push_back( Set[i] );
			root->data.print_line.push_back( line_data[i] );
			root->left = NULL;
			root->right = NULL;
		} // if
		else {
			if( graduate_num[i] > root->data.key ) {
				InsertAVLTree( i, root->right );
			} // if
			else if( graduate_num[i] < root->data.key ) {
				InsertAVLTree( i, root->left );
			} // else if
			else {    // 有相同的畢業生人數，直接加入序號及行資訊 
				root->data.rSet.push_back( Set[i] ); 
				root->data.print_line.push_back( line_data[i] );				  
			} // else
		} // else
				
		CheckBF( root );
			
	} // InsertAVLTree()
	
	int TreeHeight( branch *walk ) {
		int countL = 1;
		int countR = 1;
		if( walk == NULL )
			return 0;
			
		countL = countL + TreeHeight( walk->left );
		countR = countR + TreeHeight( walk->right );
			
		if ( countL > countR )
			return countL;
		else 
			return countR;
			
	} // TreeHeight()
		
	int BF( branch *walk ) {    // 這個節點的平衡係數 
	
		int balance_factor = 0;  // 紀錄平衡係數 		
		balance_factor = TreeHeight( walk->left ) - TreeHeight( walk->right );  // 計算左子樹 跟 右子樹的樹高 		
		return 	balance_factor;
		
	} // IsBlance()
	
	branch *RotateLL( branch *x ) {
		branch *y = x->left; 
		x->left = y->right;
		y->right = x;
		return y;
		
	} // RotateLL()
	
	branch *RotateRR( branch *x ) {
		branch *y = x->right; 
		x->right = y->left;
		y->left = x;
		return y;
		
	} // RotateRR()	

	branch *RotateLR( branch *x ) {
		x->left = RotateRR( x->left );
		x = RotateLL( x );
		
	} // RotateLR()
	
	branch *RotateRL( branch *x ) {
		x->right = RotateLL( x->right );
		x = RotateRR( x );
		
	} // RotateLR()
	
	branch *Rotate( branch *&walk ) {
		

		int bf1, bf2;
		bf1 = BF(walk);

		if( bf1 > 0 ) { // 左邊比較重
			bf2 = BF( walk->left );
			if( bf2 >= 0 ) {    // 他的左子樹也是左邊重 
				walk = RotateLL( walk ); 
			}	 // if
			else {    // 他的左子樹反而是右邊重 
				walk = RotateLR( walk );

			} // else
		} // if
		else {  // 右邊比較重
			bf2 = BF( walk->right ); 
			if( bf2 <= 0 ) {
				walk = RotateRR( walk );
			} // if
			else {  // 他的右子樹反而是左邊重 
				walk = RotateRL( walk );	
			} // else
			
  		} // else
				 

		
		
	} // Rotate()
	
	void CheckBF( branch *&walk ) {


		if(  (  BF( walk ) < -1 ) ||  ( 1 < BF( walk )  )  ) {			
			Rotate( walk );     // 這個節點不平衡需旋轉
		} // if
	
		
	} // CheckBf
	
	void Count_nodes( int &nodes, branch *root ) {
		
		if( root != NULL )
			nodes++;
			
		if( root->left != NULL )
			Count_nodes( nodes, root->left );
		if( root->right != NULL )	
			Count_nodes( nodes, root->right );
		
	} // Count_nodes()
	
	public :
	
	
	
		AVLtree() {
			root = NULL;
		} // AVLtree()
		
		~AVLtree() {
			Set.clear();
			graduate_num.clear();
			line_data.clear();			
		} // AVLtree()
		
		
		void Build_AVLtree() {
			
			int i = 0;
			while( i < Set.size() ) {			
				InsertAVLTree( i, root );	    
				i++;
			} // while
			
			
		} // Build_AVLtree
		
		void PrintAnswer() {
		

			if ( root != NULL ) {
				
				int i = 0;
				int height = TreeHeight( root );
				int nodes;
				Count_nodes( nodes, root );
				cout << "Tree height = " << height << endl;
				cout << "Number of nodes = " << nodes << endl;
				while ( i < root->data.rSet.size() ) {
					cout << i+1 << ": " << "[" <<root->data.rSet[i] << "] " << root->data.print_line[i] << endl;
					i++;
				} // while
				
								
			} // if			

		} // PrintAnswer()
	
		void GetData( vector<int> newSet, vector<int> newG, vector<string> newLine ) {
			Set = newSet;
			graduate_num = newG;
			line_data = newLine;
		} // GetData()
		
		 
	
	
	
};


int main() {
	
 	string command = "";
 	bool hasData = false;
	vector<int> Set;
	vector<int> graduate_num ;
	vector<string> line_data;
 	
 	do{
		printf( "%s\n", "**** University Graduate Information System ****" );
 		printf( "%s\n", "* 0. Quit                                      *" );
 		printf( "%s\n", "* 1. Build 2-3 Tree                            *" );
 		printf( "%s\n", "* 2. Build AVL Tree                            *" );
 		printf( "%s\n", "************************************************" );
 		printf( "%s", "Input a command(0, 1 ,2) : "); 	
 		cin >> command;	
		
 		if ( command == "0" ) {
 			exit(0);
		} // if
 		else if ( command == "1" ) {
 			TTtree schoolList;
			schoolList.ReadFile();
			schoolList.Build23Tree();
			schoolList.PrintAnswer();
			Set = schoolList.GetSet();
			graduate_num = schoolList.GetG();
			line_data = schoolList.GetLine();
			hasData = true;			
		} // else if
		else if ( command == "2" ) {
			if ( hasData == false )
				cout << "### Choose 1 first ###" << endl ;
			else {
				AVLtree tree;
				tree.GetData( Set, graduate_num, line_data );

				tree.Build_AVLtree();

				tree.PrintAnswer();
			} // else			
		} // else if
 		else {
 			printf( "Command does not exist!\n" );	
		} // else
		 
		printf("\n"); 
		 
		 
	} while( 1 );
	

}



