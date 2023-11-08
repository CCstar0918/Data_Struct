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
#define PTR_NUM 3
#define KEY_NUM PTR_NUM - 1 
using namespace std ;
fstream infile ;
int count = 0 ;
struct school {
	string graduate ;
	string school_name ;
	string department_name ;
	string day ;
	string leveal ;
	int serial_number ;
	int numOfGraduate ;
}; // school

vector<school> school_list ;

void PrintCommand() {
	cout <<  endl << "********  Search Tree  ********" << endl <<
					 "***                               ***" << endl <<
			         "* 0. 離開                           *" << endl <<
    		         "* 1. Build 2-3 tree                 *" << endl <<
			         "* 2. Build AVL tree                 *" << endl << 
			         "* 3. Top-K search on AVL tree       *" << endl <<
			         "*************************************" << endl << 
                     "請輸入指令( 0, 1, 2, 3 ): " ; 
} // PrintCommand


class TTtree{
	struct slotType{				// a slot in a tree node
		vector<int> rSet ;			// a set of record identifiers with the same key
		string key ;				// a key of comparisons
	}; // slotType
	
	struct nodeType{				// a tree node of a 2-3tree
		slotType data[KEY_NUM] ;	// a list of record sorted by keys
		nodeType *link[PTR_NUM] ;	// a list of pointer
		nodeType *parent ;			// a pointer to the parent node
	}; // nodeType
	
	struct pointType{				// a point on the search path
		nodeType *parentNode ;		// point to a parent node
		int pidx ;					// entrance index on the parent node
	}; // pointType
	
	struct blockType{				// a data block received from a split
		slotType slot ;				// a pair of( record id, key)
		nodeType *link ;			// a pointer to child on the right
	}; // blockType
	
	// --------------------------------------struct-----------------------------------------------
	
	nodeType *root ;
	
	bool Load( string fileName ) {  // load file        
		fileName = "input" + fileName + ".txt" ; 
		infile.open( fileName.c_str(), ios::in ) ;  
		if ( infile == NULL ) {
			cout << endl << fileName << " does not exist !! " << endl ;
			return false ;
		} // if
	
		return true ;
	} // Load() 
	
	nodeType *CreateNode( nodeType *left, nodeType *right, nodeType *pNode, slotType newSlot ) {
		nodeType *newNode = NULL ;
		
		try {
			newNode = new nodeType ;
			newNode -> data[0].rSet = newSlot.rSet ;
			newNode -> data[1].rSet.clear() ;
			newNode -> data[0].key = newSlot.key ;
			newNode -> data[1].key = "" ;
			newNode -> parent = pNode ;
			newNode -> link[0] = left ;
			newNode -> link[1] = right ;
			newNode -> link[2] = NULL ;
		} // try
		
		catch ( std::bad_alloc& ba) {
			cerr << endl << "bad_ alloc caught:" << ba.what() << endl;
		} // catch
			
		return newNode ;
	} // CreateNode()
	
	void SearchPath( string name, stack<pointType> &path ) {
		pointType onePoint ;
		int pos;
		nodeType *cur = root ;
		while ( cur != NULL ) {
			onePoint.parentNode = cur ;
			for ( pos = 0 ; pos < KEY_NUM ; pos++ ) {
				if ( ( !cur -> data[pos].rSet.size() ) || ( name.compare( cur->data[pos].key )  < 0 ) ) // unused slot, name > key or name < key
					break ;
				else if ( name.compare( cur->data[pos].key ) == 0 ) {									// name = key
					onePoint.pidx = pos ;
					path.push( onePoint ) ;
					return ;
				} // else if
			} // for
			
			onePoint.pidx = pos;
			path.push( onePoint );
			cur = cur->link[pos];
			
		} // while
	} // SearchPath()
	
	void InsertLeaf( slotType newSlot, pointType aLeaf ) {
		for ( int i = KEY_NUM - 1 ; i >= aLeaf.pidx ; i-- ) { // scan from right to left
			if ( i > aLeaf.pidx ) { // shift an existing record to the right
				aLeaf.parentNode -> data[i].rSet = aLeaf.parentNode -> data[i - 1].rSet ;
				aLeaf.parentNode -> data[i].key = aLeaf.parentNode -> data[i - 1].key ;
			} // if
			
			else if ( i == aLeaf.pidx ) { 
				aLeaf.parentNode -> data[i].rSet = newSlot.rSet ; // save new record in a new slot
				aLeaf.parentNode -> data[i].key = newSlot.key ;
			} // else if
			
			else 
				break ;
		} // for
	} // InsertLeaf() 
	
	void SplitLeaf( slotType newSlot, pointType aLeaf, blockType &aBlock) {
		slotType buf[PTR_NUM] ;
		int idx = 0 ;
		for ( int i = 0 ; i < PTR_NUM ; i++ ) {
			buf[i].rSet = ( i == aLeaf.pidx )? newSlot.rSet : aLeaf.parentNode -> data[idx].rSet;
			buf[i].key = (i == aLeaf.pidx )? newSlot.key : aLeaf.parentNode -> data[idx++].key;
		} // for
		
		aLeaf.parentNode -> data[0].rSet = buf[0].rSet ;
		aLeaf.parentNode -> data[0].key = buf[0].key ;
		for ( int i = 1 ; i < KEY_NUM ; i++ ) {
			aLeaf.parentNode -> data[i].rSet.clear() ;
		} // for
		
		aBlock.link = CreateNode( NULL, NULL, aLeaf.parentNode -> parent, buf[2] ) ; // create a sibling node on the right
		aBlock.slot.rSet = buf[1].rSet ; // block to move upwards
		aBlock.slot.key = buf[1].key ;
		
	} // SplitLeaf()
	
	nodeType *CreateRoot( nodeType *left, nodeType *right, slotType oneSlot ) {
		nodeType *newRoot = CreateNode( left, right, NULL, oneSlot ) ;
		left -> parent = newRoot ;
		right -> parent = newRoot ;
		return newRoot ; 
	} // CreateRoot()
	
	void InsertNonleaf( blockType &oneB, pointType goal ) {
		if( goal.pidx == 0 ) {  // 插入非葉節點時 是插入第一個位置，必須得把原來的資料往右移 
			goal.parentNode->data[1].rSet = goal.parentNode->data[0].rSet;
			goal.parentNode->data[1].key = goal.parentNode->data[0].key; 
			goal.parentNode->data[0].rSet = oneB.slot.rSet;
			goal.parentNode->data[0].key = oneB.slot.key;
			goal.parentNode->link[2] = goal.parentNode->link[1];
			goal.parentNode->link[1] = oneB.link;
		} // if
		
		else {      // 是插入第二個位置 
			goal.parentNode->data[1].rSet = oneB.slot.rSet;
			goal.parentNode->data[1].key = oneB.slot.key;
			goal.parentNode->link[2] = oneB.link;
		} // else
	} // IndertNonleaf()
	
	
	void SplitNonleaf( pointType goal, blockType &oneB ) {
		slotType buf[PTR_NUM] ;
		nodeType *ptr[PTR_NUM + 1] ;
		int idx = 0 ;
		for ( int i = 0 ; i < PTR_NUM ; i++ ) {
			buf[i].rSet = ( i == goal.pidx )? oneB.slot.rSet : goal.parentNode -> data[idx].rSet;
			buf[i].key = (i == goal.pidx )? oneB.slot.key : goal.parentNode -> data[idx++].key;
		} // for
		
		if( goal.pidx == 0 ) {
			ptr[0] = goal.parentNode->link[0];
			ptr[1] = oneB.link;
			ptr[2] = goal.parentNode->link[1];
			ptr[3] = goal.parentNode->link[2];
		} // if
		
		else if( goal.pidx == 1 ) {
			ptr[0] = goal.parentNode->link[0];
			ptr[1] = goal.parentNode->link[1];
			ptr[2] = oneB.link;
			ptr[3] = goal.parentNode->link[2];
		} // else if
		
		else if( goal.pidx == 2 ) {
			ptr[0] = goal.parentNode->link[0];
			ptr[1] = goal.parentNode->link[1];
			ptr[2] = goal.parentNode->link[2];
			ptr[3] = oneB.link;
		} // else if
		
		goal.parentNode->data[0].rSet = buf[0].rSet;
		goal.parentNode->data[0].key = buf[0].key;
		for( int i = 1; i < KEY_NUM; i++ ) {
			goal.parentNode->data[i].rSet.clear();
		} // for		
		
		goal.parentNode->link[0] = ptr[0];
		goal.parentNode->link[1] = ptr[1];
		goal.parentNode->link[2] = NULL;
		
		nodeType *newSibling = CreateNode( ptr[2], ptr[3], goal.parentNode -> parent, buf[2] );
		oneB.link = newSibling;
		oneB.slot.rSet = buf[1].rSet;	
		oneB.slot.key = buf[1].key;
	} // SplitNonleaf()
	
	void Insert23tree( int i ) {
		slotType newSlot ;
		newSlot.rSet.push_back(i) ;
		newSlot.key = school_list[i].school_name ;
		if ( root == NULL )
			root = CreateNode( NULL, NULL, NULL, newSlot ) ;
		else {
			stack<pointType> aPath ;	// search path
			pointType curPath ;		// last-visited node at the top of stack
			blockType blockUp ; 		// a data block received from a split
			SearchPath( school_list[i].school_name, aPath ) ;
			if ( !aPath.empty() ) { // have somthing
				curPath = aPath.top() ;
				if ( ( curPath.parentNode -> data[curPath.pidx].rSet.size() ) && ( !school_list[i].school_name.compare( curPath.parentNode->data[curPath.pidx].key ) ) ) {
					// duplicate key , insert directly
					curPath.parentNode -> data[curPath.pidx].rSet.push_back( i );    
				} // if
				
				else if ( !curPath.parentNode -> data[KEY_NUM-1].rSet.size() ) {
					InsertLeaf( newSlot,curPath ) ;
				} // else if
				
				else {
					SplitLeaf( newSlot, curPath, blockUp ) ; // split a leaf for an insertion
					if ( curPath.parentNode -> parent == NULL ) { // if a root is split, create new root
						root = CreateRoot( curPath.parentNode, blockUp.link, blockUp.slot ) ;
					} // if
					
					else {
						do{
							aPath.pop() ;
							curPath = aPath.top() ;
							if ( !curPath.parentNode -> data[KEY_NUM - 1].rSet.size() ) {
								InsertNonleaf( blockUp, curPath ) ;
								break ;
							} // if
							
							else {
								SplitNonleaf( curPath, blockUp ) ;
								if ( curPath.parentNode -> parent == NULL ) {
									root = CreateRoot( curPath.parentNode, blockUp.link, blockUp.slot ) ;
									break ;
								} // if
							} // else
						} while(true) ;	
					} // else
				} // else
			} // if
		} // else
	} // Insert23tree()
	
	void Free23Tree( nodeType *root ) {		
		if ( root != NULL ) {
			Free23Tree( root->link[0] );
			for( int i = 0; i < KEY_NUM; i++ ) {
				if( !root->data[i].rSet.size() )
					break;
					
				Free23Tree( root->link[i+1] );
			} // for
			
			delete root;
		} // if
	} // Free23Tree()
	
	int TreeHeight( nodeType *root ) {
		if( root -> link[0] != NULL )
			return 1 + TreeHeight( root -> link[0] );
				
		return 1;
	} // TreeHigh()
	
	int NumOfNode( nodeType *root ) {
		if(  root != NULL )
			return 1 + NumOfNode( root -> link[0] ) + NumOfNode( root -> link[1] ) + NumOfNode( root -> link[2] ) ;
		else
			return 0 ;
	} // NumOfNode() 
	
	public:
		
		TTtree() {
			root = NULL;
			school_list.clear() ;
		}
		~TTtree() {
			Free23Tree( root );
		}
		void ReadFile() {
			school_list.clear() ;
		    cout << "輸入要執行的檔案 : " ; 
		    string fileName ;
		    cin >> fileName ;
		    while ( !Load(fileName) ) { // 讀到有檔案為止 
				cout << "輸入要執行的檔案 : " ;
				cin >> fileName ;
			} // while
			
			string line ;
			// read three lines text
			for ( int i = 0 ; i < 3 ; i ++) { 
				getline( infile, line ) ;
			} // for
			
			school t ;
			int count = 1 ;
			while ( getline( infile, line, '\t') ) {           // school_id 
				getline ( infile, t.school_name , '\t') ;      // school_name
				getline ( infile, line, '\t') ; 			   // dempartment_id
				getline ( infile, t.department_name, '\t' ) ;  // department_name
				getline	( infile, t.day, '\t' ) ;			   // day
				getline ( infile, t.leveal, '\t') ;			   // leveal
				getline ( infile, line, '\t') ;		           // student
				getline ( infile, line, '\t') ;		           // teacher
				getline ( infile, t.graduate, '\t') ;		   // graduate
				getline ( infile, line) ;					   // city and type
				t.numOfGraduate = atoi( t.graduate.c_str() ) ; // graduate string to int
				t.serial_number = count ;                      // serial_number
				school_list.push_back(t) ;
				count++ ;
			} // while
			
			infile.close() ;
		} // ReadFile()
		
		void Build23tree() {
			// 用序號 i 去撈取存在資料庫裡需要的資訊 
			for ( int i = 0 ; i < school_list.size() ; i++ ) {
				Insert23tree(i) ;
			} // for
		} // Build23tree()
		
		void Print() {
			cout << "Tree height = " << TreeHeight( root ) << endl ;
			cout << "Number of nodes = " << NumOfNode( root ) << endl ;
			int count = 1 ;
			for ( int i = 1 ; i >= 0 ; i--) {
				if ( root -> data[i].rSet.size() != 0 ) {
					for ( int j = 0 ; j < root -> data[i].rSet.size() ; j++ ) {
						cout << count << ": " << "[" << school_list[root -> data[i].rSet[j] ]. serial_number << "] " <<
												    school_list[root -> data[i].rSet[j] ].school_name << ", " <<
												    school_list[root -> data[i].rSet[j] ].department_name << ", " <<
												    school_list[root -> data[i].rSet[j] ].day << ", " <<
												    school_list[root -> data[i].rSet[j] ].leveal << ", " <<
												    school_list[root -> data[i].rSet[j] ].graduate << endl ;
						count++ ;
					} // for
				} // if
			} // for
		} // Print()
		
}; // class TTtree

class AVLtree{
	struct nodeType{
		vector<int> rSet ;
		int key ;
		nodeType *left ;
		nodeType *right ;
	}; // nodeType
	
	nodeType *root ;
	
	void InsertAVLtree( int i, nodeType *&root ) {
		if ( root == NULL ) {
			root = new nodeType ;
			root -> rSet.push_back(i) ;
			root -> key = school_list[i].numOfGraduate ;
			root -> left = NULL ;
			root -> right = NULL ;
		} // if
		
		else {
			if ( school_list[i].numOfGraduate == root -> key ) {
				root -> rSet.push_back(i) ;
			}
			else if( school_list[i].numOfGraduate > root -> key ) {
				InsertAVLtree( i, root -> right ) ;
			}
			else  { // school_list[i].numOfGraduate < root -> key
				InsertAVLtree( i, root -> left ) ;
			}
		} // else
			
		CheckBF( root ) ;
	} // InsertAVLtree()
	
	void CheckBF( nodeType *&walk ) {

		if(  (  BF( walk ) < -1 ) ||  ( 1 < BF( walk )  )  ) {			
			Rotate( walk );     // 這個節點不平衡需旋轉
		} // if
	
	} // CheckBF()
	
	int BF( nodeType *walk ) {
		int balance_factor = 0;  // 紀錄平衡係數 		
		balance_factor = TreeHeight( walk -> left ) - TreeHeight( walk -> right );  // 計算左子樹 跟 右子樹的樹高 		
		return 	balance_factor;
	} // BF()
	
	int TreeHeight( nodeType *walk ) {
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
	
	nodeType *RotateLL( nodeType *x ) {
		nodeType *y = x -> left; 
		x -> left = y -> right;
		y -> right = x;
		return y;
		
	} // RotateLL()
	
	nodeType *RotateRR( nodeType *x ) {
		nodeType *y = x -> right; 
		x -> right = y -> left;
		y -> left = x;
		return y;
		
	} // RotateRR()	

	nodeType *RotateLR( nodeType *x ) {
		x -> left = RotateRR( x -> left );
		x = RotateLL( x );
		
	} // RotateLR()
	
	nodeType *RotateRL( nodeType *x ) {
		x -> right = RotateLL( x -> right );
		x = RotateRR( x );
		
	} // RotateLR()
	
	nodeType *Rotate( nodeType *&walk ) {
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
	
	int NumOfNode( nodeType *root ) {
		if(  root != NULL )
			return 1 + NumOfNode( root -> left ) + NumOfNode( root -> right ) ;
		else
			return 0 ;
	} // NumOfNode() 
	
	void LargeNode(nodeType *root, int &serial, int k ) {
		
		if ( root -> right != NULL ) {
			LargeNode( root -> right, serial, k ) ;
		} // if
		
		if ( serial <= k ) {
			for ( int j = 0 ; j < root -> rSet.size() ; j++ ) {
					cout << serial << ": " << "[" << school_list[root -> rSet[j] ]. serial_number << "] " <<
											   		 school_list[root -> rSet[j] ].school_name << ", " <<
											  		 school_list[root -> rSet[j] ].department_name << ", " <<
											   		 school_list[root -> rSet[j] ].day << ", " <<
											  		 school_list[root -> rSet[j] ].leveal << ", " <<
												     school_list[root -> rSet[j] ].graduate << endl ;
					serial ++ ;
			} // for
		} // if
			
		if ( root -> left != NULL )
			LargeNode( root -> left, serial, k ) ;
			
	} // LargeNode()

	public :
		AVLtree(){
			root = NULL ;
		}
		~AVLtree(){
		}
		
		void BuildAVLtree() {
			for ( int i = 0 ; i < school_list.size() ; i++ ) {
				InsertAVLtree( i, root ) ;
			} // for
		} // BuildAVLtree() 
		
		void TreeSearch( int k ) {
			int serial = 1 ;
			LargeNode( root, serial, k ) ;
		} // TreeSearch()
		
		void Print() {
			if ( root != NULL ) {
				int i = 0;
				cout << "Tree height = " << TreeHeight( root ) << endl;
				cout << "Number of nodes = " << NumOfNode( root )  << endl;
				for ( int j = 0 ; j < root -> rSet.size() ; j++ ) {
					cout << j + 1 << ": " << "[" << school_list[root -> rSet[j] ]. serial_number << "] " <<
											    school_list[root -> rSet[j] ].school_name << ", " <<
											    school_list[root -> rSet[j] ].department_name << ", " <<
											    school_list[root -> rSet[j] ].day << ", " <<
											    school_list[root -> rSet[j] ].leveal << ", " <<
											    school_list[root -> rSet[j] ].graduate << endl ;
				} // for				
			} // if			
		} // Print()
		
		nodeType *GetRoot() {       // 把樹根送出去 
			return root;
		}
		
		void SetRoot( nodeType *newRoot ) {    // 讓樹根等於傳進來的新樹根 
			root = newRoot;
		}
		
		
}; // class AVLtree


int main() {
	string com = "" ; // 接收指令 
    bool haveData = false ;
    bool haveAVLtree = false ;
	AVLtree treeCopy ;
 	while ( 1 ) {                            
		PrintCommand() ;
      	cin >> com ;
      	if ( com.compare( "0" ) == 0 ) {
      		exit(0) ;
		} // if
        	
        else if ( com == "1") { // 2-3 Tree
        	TTtree tree ;
        	tree.ReadFile() ;
        	tree.Build23tree();
        	tree.Print() ;
        	haveData = true ;
        	haveAVLtree = false;
        } // else if
        
        else if ( com == "2" ) { // AVL tree
        	if ( !haveData ) 
        		cout << "### Choose 1 first ###" << endl ;
        	else {
        		AVLtree tree;
        		tree.BuildAVLtree() ;
        		tree.Print() ;
        		treeCopy.SetRoot( tree.GetRoot() );
        		haveAVLtree = true ;
			} // else
		} // else if
		
		else if ( com == "3" ) { // AVL tree search
			if ( !haveData) 
				cout << "### Choose 1 first ###" << endl ;
			else {
				if ( !haveAVLtree )
					cout << "### Choose 2 first ###" << endl ;
				else {
					if ( school_list.size() > 0) {
						int k ;
						cout << "Enter K in [1," << school_list.size() << "] : " ;
						cin >> k ;
						while( k > school_list.size() || k < 0 ) {
							cout << "Input error!!"	<< endl;
							cout << "Enter K in [1," << school_list.size() << "] : " ;
							cin >> k ;
						} // while
						treeCopy.TreeSearch( k ) ;
					}
				} // else
			} // else
		} // else if
		
		else {
			cout << "指令不存在,請重新輸入" << endl;
		}
	} // while 
} // main()
