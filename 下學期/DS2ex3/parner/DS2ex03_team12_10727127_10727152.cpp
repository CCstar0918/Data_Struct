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
#include<stack>
#include<iomanip>
#define COLUMNS 6
#define	MAX_LEN 10
#define BIG_INT 255
using namespace std ;


struct sT {           
	char sid[MAX_LEN];         		// �s�Ǹ� 
	char sname[MAX_LEN];			// �s�W�r 
	unsigned char score[COLUMNS];	// �s���Ӥ��� 
	float mean;						// �s�������� 
};

struct hT {
	bool open;               		// �Ω�T�{�d�l�O�_�s��F�� 
	char sid[MAX_LEN];				
	char sname[MAX_LEN];
	float mean;
	int hValue;                  	// �s������� 
};

void PrintCommand() {
	cout <<  endl << "********  Hash Table         ********" << endl <<
					 "***                               ***" << endl <<
			         "* 0. ���}                           *" << endl <<
    		         "* 1. quadratic probing              *" << endl <<
			         "* 2. Double hashing                 *" << endl << 
			         "*************************************" << endl << 
                     "�п�J���O( 0, 1, 2 ): " ; 
} // PrintCommand

class Hash {


	void FindPrimeWithNum( int &num ) {	// �ǤJ�@�ӼƦr�A��M��L�j����� 
		int i = num-1;
		while( i > 0 ) {				
			if( num%i == 0 )
				break;
			
			i = i - 1;
		
		} // while
	
		if( i != 1 ) {
			num = num + 1;
			FindPrimeWithNum( num );
		} // if
	} // FindPrimeWithNum()

	int FindKey( char sid[], int size ) {	// �Q�ζǤJ���Ǹ� ��X����ȡA��Y���өҦb���d�l���X 
	
		int i = 0;
		unsigned long long int key = 1;
		while ( i < strlen( sid ) ) {
		
			key = key * sid[i];
			i++;
				
		} // while 
	
		key = key%size;
		return key;	
	} // FindKey()
	
	int Step( char sid[], int dataNum ) {	// �B����ơA��o�͸I���ɡA�Q�Φ���ƺ�X�B�� 
		int i = 0;
		int num = ( dataNum / 3) + 1;
		FindPrimeWithNum( num ); // num���j���Ƶ��ƤT�����@�����
		unsigned long long int sum = 1;
		while ( i < strlen( sid ) ) {
		
			sum = sum * sid[i];
			i++;
				
		} // while 
		
		return ( num - ( sum%num ) );
	} // Step

	void DoQuadraticHash( sT oneSt, hT hashArray[], int size ) {	 // �i������A��o�͸I���ɡA�Q�Υ���k��M�s��m	
		int key = FindKey( oneSt.sid, size );
	
		if( hashArray[key].open == false ) {    // ����d�l�٨S�H�}�L�A������i�d�l�� 			
			strcpy( hashArray[key].sid, oneSt.sid );
			strcpy( hashArray[key].sname, oneSt.sname );
			hashArray[key].mean = oneSt.mean;
			hashArray[key].hValue = key;
			hashArray[key].open = true;
		} // if
		else {   // ���H�ϥ��d�l�Q�Υ���k 
			
			int i = 1;
			unsigned long long int newKey; // �O���s����m 
			while(true) {
			
				unsigned long long int square = i * i; 
				newKey = key + square;     // �s����m�O��Ӧb����m�h�[����� 
				newKey = newKey % size;    // �p�G�W�L�d�l�Ƥj�p�A�N�Ψ��l�Ƥ覡 
				if ( hashArray[newKey].open == false  ) {   // ���S�}�L����i�h 
					strcpy( hashArray[newKey].sid, oneSt.sid );
					strcpy( hashArray[newKey].sname, oneSt.sname );
					hashArray[newKey].mean = oneSt.mean;
					hashArray[newKey].hValue = key;
					hashArray[newKey].open = true;	
					break;			
				} // if
			
				i++;     // ���誺�Ʀr�[1 
			
			} // while
			
		} // else 
	
	} // doHash
	
	void DoDoubleHash( sT oneSt, hT hashArray[], int size, int dataNum  ) {  // �i������A��o�͸I���ɡA�Q�ΨB����Ƨ�M�s��m 
		int key = FindKey( oneSt.sid, size );
		
		if( hashArray[key].open == false ) {    // ����d�l�٨S�H�}�L 			
			strcpy( hashArray[key].sid, oneSt.sid );
			strcpy( hashArray[key].sname, oneSt.sname );
			hashArray[key].mean = oneSt.mean;
			hashArray[key].hValue = key;
			hashArray[key].open = true;
		} // if
		else {   // ���H�ϥ��d�l�Q�Υt�@��hash 
			int h2 =  Step( oneSt.sid, dataNum );     // ��X�B�� 
			int newKey = (key+h2)%size;        		   
			while( hashArray[newKey].open == true ) {  // �Ū��d�l���e �A�ΦP�˪��B���@����  			
				newKey = (newKey+h2)%size;
			} // while
			
			strcpy( hashArray[newKey].sid, oneSt.sid );
			strcpy( hashArray[newKey].sname, oneSt.sname );
			hashArray[newKey].mean = oneSt.mean;
			hashArray[newKey].hValue = key;
			hashArray[newKey].open = true;						
						
		} // else 
			
	} // 
	
	void PrintQuadraticScreen( hT hashArray[], int size ) {     // �L�X����k��ܦb�ù��W����T 
		int i = 0 ; 
		float searchNoValue = 0;
		while (  i < size ) {
			
			int walk = 0;

			unsigned long long int newKey = i ;   // �Ω�����s��m 
			int k = 1;
			while( hashArray[newKey].open == true ) {   // ������Ū��d�l�e ����������� 
				unsigned long long int square = k * k;
				k++;
				newKey = i + square;
				newKey = newKey%size;
				walk++;	      // ���Ȥ������+1			
			} // while
			
			searchNoValue = searchNoValue + walk;      // ��C���d�l�j�M���s�b�Ȭۥ[ 
			i++;
		} // while
		
		cout << "Hash table has been successfully created by Quadratic probing\n" ;
		cout << "unsuccessful search: ";
		printf( "%.4f", (searchNoValue / size) ) ;		// ���H�d�l�� 
		cout << " comparisons on average\n";
		
		i = 0;
		int dataNum = 0;
		while ( i < size ) {						 // ��X�`��Ƶ��ơA�Ω�p��j�M�s�b��
			if( hashArray[i].open == true )
				dataNum++;			
			i++;
		} // while
		
		i = 0;
		float searchValue = 0;
		while (  i < size ) {						// �j�M�o����Ʀb���e�Ҥ������ 
			
			int walk = 0;
			if( hashArray[i].open == true ) {    
				int j = hashArray[i].hValue;
				int k = 1;
				int newJ = j;

				while( strcmp(hashArray[i].sid, hashArray[newJ].sid) != 0 ) {
					int square = k * k;
					k++;
					newJ = j + square;
					newJ = newJ%size;
					walk++;					
				} // while	
				
				walk = walk + 1;			
			} // if
			
			searchValue = searchValue + walk;			// ��C���{�s�ȷj�M�쪺������Ƭۥ[ 
			i++;
		} // while		
 
		cout << "successful search: ";
		printf( "%.4f", ( searchValue / dataNum) ) ;	// ���H��Ƶ���
		cout << " comparisons on average" << endl ;
		
	} // PrintScreen()
	
	void PrintDoubleHashScreen( hT hashArray[], int size, int dataNum ) {	// �L�XDouble hash��ܦb�ù��W����T 
		
		cout << "Hash table has been successfully created by Double hashing\n" ;
		

		
		int i = 0;
		float searchValue = 0;
		while (  i < size ) {
			
			int walk = 0;
			if( hashArray[i].open == true ) {    	// �j�M�o����Ʀb���e�Ҥ������ 
				int j = hashArray[i].hValue;
				int h2 = Step( hashArray[i].sid, dataNum );
				int newJ = j ;

				while( strcmp(hashArray[i].sid, hashArray[newJ].sid) != 0 ) {


					newJ = (newJ+h2)%size ;
					walk++;					
				} // while	
				
				walk = walk + 1;			
			} // if
			
			searchValue = searchValue + walk;
			i++;
		} // while		

 
		cout << "successful search: ";
		printf( "%.4f", (searchValue / dataNum) ) ;
		cout << " comparisons on average" << endl ;
		
	} // PrintScreen()
	
    void PrintQuadraticFile( hT hashArray[], int size, string fileName )	 {	//  �N����k�����G�L��txt�� 
    	ofstream outFile;
    	fileName = "quadratic" + fileName + ".txt";
    	outFile.open( fileName.c_str(), ios::out );
    	int i = 0;
    	while( i < size ) {
    		if( i == 0 ) 
    			outFile << "--- Hash table created by Quadratic probing ---" << endl;

			if ( hashArray[i].open == true ) {
    			outFile << "[" << i << "] " << '\t' << hashArray[i].hValue << "," << '\t';
				outFile << hashArray[i].sid << "," << '\t'<< hashArray[i].sname << ","<< '\t' << hashArray[i].mean << endl;
			} // if
			else {
				outFile << "[" << i << "] " << endl;
			}

			i++;
		} // while
		outFile << "----------------------------------------------------- ";
		outFile.close();
    	
	} // PrintFile()
	
    void PrintDoubleHashFile( hT hashArray[], int size, string fileName )	{    //  �NDouble hash�����G�L��txt��  
    	ofstream outFile;
    	fileName = "double" + fileName + ".txt";
    	outFile.open( fileName.c_str(), ios::out );
    	int i = 0;
    	while( i < size ) {
    		if( i == 0 ) 
    			outFile << "--- Hash table created by Double hashing ---" << endl;

			if ( hashArray[i].open == true ) {
    			outFile << "[" << i << "] " << '\t' << hashArray[i].hValue << "," << '\t';
				outFile << hashArray[i].sid << "," << '\t'<< hashArray[i].sname << ","<< '\t' << hashArray[i].mean << endl;
			} // if
			else {
				outFile << "[" << i << "] " << endl;
			}

			i++;
		} // while
		outFile << "----------------------------------------------------- ";
		outFile.close();
    	
	} // PrintFile()	
	
	public:
	
	bool ReadFile( string &fileName, int &dataNum ) {     // �Ω�Ū���ɮפΧP�_�O�_���\Ū���ɮ� 
			
		cout << "Input a file number: ";
		cin >> fileName;
		if( !Quadratic( fileName, dataNum ) ) {       // �P�_���Lbin�� �A�S���hŪ��TXT�ɡA���নbin�� 
			cout << "input" + fileName + ".bin does not exist!\n" ;
			if( !Text2Binary(fileName) ) {     		// �p�G�sTXT�ɳ��S���N�OŪ������ 
				cout << "input" + fileName + ".txt does not exist!\n" ;
				return false;
			} // if
			else 
				Quadratic( fileName, dataNum );			
		} // if
				
		return true;
						
	} // Read()

	bool Text2Binary( string &fileName ) {
	
		fstream inFile, outFile;
		int stNo = 0;	

		inFile.open( ( "input" + fileName + ".txt").c_str(), fstream::in );
		if( inFile.is_open() == false )
			return false;
			
		string binFileName = "input" + fileName + ".bin";
		outFile.open( binFileName.c_str(), fstream::out | fstream::binary );
		if( outFile.is_open() ) {
			char rBuf[BIG_INT];
			while( inFile.getline(rBuf, BIG_INT, '\n') ) {
				string temp;
				sT oneSt;
				int cNo = 0, pre = 0, pos = 0;
			
				stNo++;
				temp.assign(rBuf);
				pos = temp.find_first_of( '\t', pre );
			
				while( pos != string::npos ) {
					switch(++cNo) {
						case 1: strcpy( oneSt.sid, temp.substr(pre,pos-pre).c_str() );
							break;
						case 2: strcpy( oneSt.sname, temp.substr(pre,pos-pre).c_str() );
							break;
						default: oneSt.score[cNo-3] = atoi(temp.substr(pre,pos-pre).c_str()) ;
							break;
					} // switch
				
					pre = ++pos;
					pos = temp.find_first_of( '\t', pre );
				
				} // while
			 
				pos = temp.find_last_of('\t');
				oneSt.mean = atof( temp.substr(pos+1).c_str() );
				outFile.write((char*)&oneSt, sizeof(oneSt));
			
			} // while
		
			outFile.close();
		} // if
	
		inFile.close();
		return true;
	
	} // Text2Binary()

	bool Quadratic( string fileName, int &dataNum ) {     // �Nbin��Ū�J �A�ð��業��k������ 
		
		string binFileName = "input" + fileName + ".bin";
		fstream binFile;
		sT oneSt;
		int stNo = 0;
	
		binFile.open( binFileName.c_str(), fstream::in | fstream::binary );
		if( binFile.is_open() ) {
			binFile.seekg(0,binFile.end);
			stNo = binFile.tellg() / sizeof(oneSt);
		
			float num = stNo*1.2;
			int arraySize = 0;
			while ( arraySize < num ) {      
				arraySize++;
			} // while
			
			FindPrimeWithNum( arraySize );   // �����`��1.2���j�����
			hT hashArray[arraySize];         // �Ω����ꪺ�d�l 
			
			int i = 0;
			while ( i < arraySize ) {
				hashArray[i].open = false;
				i++;	
			} // while
			binFile.seekg(0,binFile.beg);
			for( int i = 0; i < stNo; i++) {
				binFile.read((char*)&oneSt, sizeof(oneSt));
				DoQuadraticHash( oneSt, hashArray, arraySize );    // �@���@�����Ū�J�i������ 

			} // for
			
			dataNum = stNo;
			if( dataNum != 0 )                                   // �p�GŪ�J���ɮ׸�Ƶ��ƬO0 �N���ΦL�b�ù��W�F 
				PrintQuadraticScreen(hashArray, arraySize); 
				
			PrintQuadraticFile(hashArray, arraySize, fileName);
			binFile.close();
			
			return true;
		} // if
		else 
			return false;
	} // Quadratic()
	
	void DoubleHash( string fileName, int dataNum ) {		// �Nbin��Ū�J �A�ð���Double hash������ 
		string binFileName = "input" + fileName + ".bin";
		fstream binFile;
		sT oneSt;
		int stNo = 0;
	
		binFile.open( binFileName.c_str(), fstream::in | fstream::binary );
		if( binFile.is_open() ) {
			binFile.seekg(0,binFile.end);
			stNo = binFile.tellg() / sizeof(oneSt);
		
			int arraySize = stNo*1.2;
			arraySize++; //�o��[1 �]���n�j��o�ӼƦr����� �N��L�ۤv�O��Ƥ]���� 
			FindPrimeWithNum( arraySize );   
			hT hashArray[arraySize];
			
			int i = 0;
			while ( i < arraySize ) {
				hashArray[i].open = false;
				i++;	
			} // while
			binFile.seekg(0,binFile.beg);
			for( int i = 0; i < stNo; i++) {
				binFile.read((char*)&oneSt, sizeof(oneSt));
				DoDoubleHash( oneSt, hashArray, arraySize, dataNum );

			} // for
			dataNum = stNo;
			if ( dataNum != 0 )											// �p�GŪ�J���ɮ׸�Ƶ��ƬO0 �N���ΦL�b�ù��W�F 
				PrintDoubleHashScreen(hashArray, arraySize, dataNum); 
				
			PrintDoubleHashFile(hashArray, arraySize, fileName);
			
			binFile.close();
		} // if
	} // DoubleHash()

}; // class Hash



int main() {
	string com = "" ; // �������O 
    string fileName;
    int dataNum;
	bool haveData = false;
 	while ( 1 ) {                            
		PrintCommand() ;
      	cin >> com ;
      	if ( com.compare( "0" ) == 0 ) {
      		exit(0) ;
		} // if
        	
        else if ( com == "1") { 
			Hash aHash ;
        	if( aHash.ReadFile( fileName, dataNum ) == true )
        		haveData = true;
        	else 
        		haveData = false;
        } // else if
        
        else if ( com == "2" ) { 
        	if ( haveData == false ) 
        		cout << "Please enter command 1 first!\n" ; 
			else {
				Hash aHash;
				aHash.DoubleHash( fileName, dataNum );
			} // else

		} // else if		
		else {
			cout << "���O���s�b,�Э��s��J" << endl;
		}
	} // while 
} // main()
