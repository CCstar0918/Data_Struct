 // 10727127 �����@ 10727157 ��b�r
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<vector>
#include<fstream>
#include<iostream>
#include<time.h>
using namespace std ;
FILE *infile = NULL ;
clock_t start_time, end_time ;
int ReadingTime, SortingTime, WritingTime ;
int CPU_time = 1 ; // �ĴX�Ӯɶ��I 
int JobItem = 0 ;
class Job{
	public :
		int OID = 0 ;
		int Arrival = 0 ;
		int Duration = 0 ;
		int TimeOut = 0 ;
		int CPU_doing_time = 0 ;
}; // Job

struct Abort {
	int OID ;
	int Abort ;
	int Delay ;
}; // Abort

struct Done {
	int OID ;
	int Departure ;
	int Delay ;
}; // Done

Abort Abort_temp ;
Done Done_temp ;
vector<Job> Job_list ;
Job temp ;
class CPU {
	public:
		vector<Abort> Abort_Jobs ;
		vector<Done> Done_Jobs ;
		vector<Job> Q_Jobs ; // �u��s4��Job 
	void Joblist_to_CPU() {
		Q_Jobs.push_back( Job_list[JobItem] ) ;
	} // Joblist_to_CPU()
	
	void Q_fullOut() {
		while ( Q_Jobs.size() != 4 ) { 
			Abort_temp.OID = Q_Jobs[4].OID ;
			Abort_temp.Abort = CPU_time ;
			Abort_temp.Delay = 0 ;
			Abort_Jobs.push_back( Abort_temp ) ;  // �[�JAbort 
			Q_Jobs.erase( Q_Jobs.begin() + 4 ) ; // �R��Q 
		} // while	
	} // Q_fullOut()
	
	void Execute_Abort() {
		Abort_temp.OID = Q_Jobs[0].OID ;
		Abort_temp.Abort = CPU_time ;
		Abort_temp.Delay = Abort_temp.Abort - Q_Jobs[0].Arrival ;
		Abort_Jobs.push_back( Abort_temp ) ;  // �[�JAbort 
		Q_Jobs.erase( Q_Jobs.begin() ) ;
	} // Execute_Abort()
	
	void Done() {
		Done_temp.OID = Q_Jobs[0].OID ;
		Done_temp.Departure = CPU_time ;
		Done_temp.Delay = Q_Jobs[0].CPU_doing_time - Q_Jobs[0].Arrival ;
		Done_Jobs.push_back( Done_temp ) ;
		Q_Jobs.erase( Q_Jobs.begin() ) ;
	} // Done
	
	float Average_Delay() {
		int total = 0 ;
		for ( int i = 0 ;i < Abort_Jobs.size() ; i ++) 
			total +=  Abort_Jobs[i].Delay ;
		for ( int i = 0 ;i < Done_Jobs.size() ; i ++) 
			total +=  Done_Jobs[i].Delay ;
		return  100 * total / ( Abort_Jobs.size() + Done_Jobs.size() )  ;
	} // Average_Delay
	
	float Success_Rate() {
		return Done_Jobs.size() * 10000 / ( Abort_Jobs.size() + Done_Jobs.size() )   ;
	} // Success_Ratr()
	
	void OutputNewFile( string fileName, FILE *infile ) {
		fileName = "output" + fileName + ".txt" ;
		infile = fopen( fileName.c_str(), "w+b" ) ; 
		string s = "[Abort Jobs]" ;
		cout << '\t' << s << endl  ;
		fprintf(infile, "%c%s%c", '\t', s.c_str(), '\n') ;  // ��C�r��s
		string a =  "OID"    ;
		string b = "Abort"   ;
		string c = "Delay" ;
		cout << '\t'<< a << '\t' << b << '\t' << c << endl ;
		fprintf(infile, "%c%s%c%s%c%s%c",'\t', a.c_str(), '\t',b.c_str(), '\t',c.c_str(), '\n' ) ;
		for ( int i = 0 ; i < Abort_Jobs.size() ; i ++ ) {
			fprintf(infile, "%s%d%c%d%c%d%c%d%c","[", i + 1, '\t', Abort_Jobs[i].OID, '\t', Abort_Jobs[i].Abort, '\t', Abort_Jobs[i].Delay,'\n' ) ;
			cout << "[" << i + 1 << '\t'<< Abort_Jobs[i].OID << '\t'<< Abort_Jobs[i].Abort << '\t'<< Abort_Jobs[i].Delay << endl ;
		} // for
		
		s = "[Jobs Done]" ;
		cout << '\t' << s << endl ;
		fprintf(infile, "%c%s%c", '\t', s.c_str(),'\n' ) ;  // ��C�r��s
		b = "Departure" ;
		cout <<'\t' << a << '\t' << b << '\t' << c << endl ;
		fprintf(infile, "%c%s%c%s%c%s%c",'\t', a.c_str(), '\t',b.c_str(), '\t',c.c_str(), '\n' ) ;
		for ( int i = 0 ; i < Done_Jobs.size() ; i ++ ) {
			fprintf(infile, "%s%d%c%d%c%d%c%d%c","[", i + 1, '\t', Done_Jobs[i].OID, '\t', Done_Jobs[i].Departure, '\t', Done_Jobs[i].Delay,'\n' ) ;
			cout << "[" << i + 1 << '\t'<< Done_Jobs[i].OID << '\t'<< Done_Jobs[i].Departure << '\t'<< Done_Jobs[i].Delay << endl ;
		} // for
		
		s = "[Average Delay]" ;
		float y = Average_Delay() / 100  ;
		cout << s << '\t' << y << "ms" << endl ;
		fprintf(infile, "%s%c%.2f%s%c", s.c_str(), '\t', y, "ms", '\n' ) ;
		s = "[Success Rate]" ;
		float x = Success_Rate() / 100 ;
		cout << s << '\t' << x  << "%" << endl ; 
		fprintf(infile, "%s%c%.2f%s%c", s.c_str(), '\t', x , "%", '\n' ) ;
		fclose(infile) ;
	} // OutPtNewFile()
}; // CPU

void PrintCommand() {
	cout << "*" << " 0. ���}" << endl << "*" <<
    " 1. ���w�覡�t�رƧ���" << endl << "*" << " 2. ��@CPU�ǦC����" << endl <<
    "�п�J���O( 0, 1, 2 ):" ; 
} // PrintCommand

bool IsDigit( string input ) {
	for ( int i = 0 ; i < input.size() ; i++ )
    	if ( !isdigit( input[i] ) )
   			return false ;
	return true ;
    // ��J���F��u���¼Ʀr�����p�~���� 
} // IsDigit() 

bool Load( string fileName) {  // load file        
	fileName = "input" + fileName + ".txt" ; 
	infile = fopen( fileName.c_str(), "r" ) ;   
	if ( infile == NULL) {
		cout << endl << fileName << " does not exist" << endl ;
		return false ;
	} // if

	return true ;
} // Load() 

string StoreFile() {
	start_time = clock() ;
	char ch ;
	string s;
	fscanf( infile, "%c", &ch ) ; 
	while ( ch != '\n') {
		s = s + ch ;
		fscanf( infile, "%c", &ch ) ;
	} // while
	
	s = s + '\n' ;
	while ( fscanf( infile, "%d%d%d%d", &temp.OID, &temp.Arrival, &temp.Duration, &temp.TimeOut ) != EOF ) 
		Job_list.push_back(temp) ;
	// Ū������ 
	
	fclose(infile) ;
	ReadingTime = clock() - start_time ;
	return s ;
} // StoreFile()

void ShellSort( int n ) {
	start_time = clock() ;
	for( int h = n / 2 ; h > 0 ; h = h / 2 )
		for( int unsorted = h ; unsorted < n ; unsorted ++ ) {
			int loc = unsorted ;
			Job nextItem = Job_list[unsorted] ; 
			for( ; (loc>=h ) && (Job_list[loc - h].Arrival > nextItem.Arrival) ; loc = loc - h) 
				Job_list[loc] = Job_list[loc - h] ;
			Job_list[loc] = nextItem ;
			 
		} // for
		
	for( int h = n / 2 ; h > 0 ; h = h / 2 )
		for( int unsorted = h ; unsorted < n ; unsorted ++ ) {
			int loc = unsorted ;
			Job nextItem = Job_list[unsorted] ;
			for ( ; Job_list[loc - h].Arrival == nextItem.Arrival && Job_list[loc - h].OID > nextItem.OID ; loc = loc - h )
				Job_list[loc] = Job_list[loc - h] ;	
			Job_list[loc] = nextItem ;
		} // for
		
	SortingTime = clock() - start_time  ;
	
} // ShellSort 

void SortNewFile( string fileName, string s ) {
	start_time = clock() ;
	fileName = "sorted" + fileName + ".txt" ; 
	infile = fopen( fileName.c_str(), "w+b" ) ; 
	fprintf(infile, "%s", s.c_str()) ;  // ��C�r��s
	cout << s  ;
	
	for ( int i = 0 ; i < Job_list.size() ; i ++ ) {
		cout << Job_list[i].OID << '\t' << Job_list[i].Arrival << '\t' << Job_list[i].Duration << '\t' << Job_list[i].TimeOut<< endl ;
		fprintf(infile, "%d%c%d%c%d%c%d%c", Job_list[i].OID, '\t', Job_list[i].Arrival, '\t', Job_list[i].Duration, '\t', Job_list[i].TimeOut, '\n' ) ;
	} // for
	
	fclose(infile) ;
	WritingTime = clock() - start_time ;
} // WriteNewFile()

void PrintTime() {
	cout << "Reading data: " << ReadingTime << " ms" << endl ;
	cout << "Sorting data: " << SortingTime << " ms" << endl ;
	cout << "Writing data: " << WritingTime << " ms" << endl ;
} // PrintTime()
int main() {
	string com = "" ; // �������O  
 	while ( 1 ) {                            
		PrintCommand() ;
      	cin >> com ;
   		while ( !IsDigit( com ) || atoi( com.c_str() ) < 0 || atoi( com.c_str() ) > 2 ) { // �Ʊ��_�ǼƦr 
    		cout << "���O���s�b,�Э��s��J: " ;
    		cout << endl ; 
       		PrintCommand() ;
       		cin >> com ;
      	} // while
      	
      	if ( com.compare( "0" ) == 0 )
        	exit(0) ;
      	if ( com == "1" ) {
      		string s ; // �ΨӦs���D 
      		cout << "��J�n���檺�ɮ� :" ; 
      		string fileName ;
      		cin >> fileName ;
			while ( !Load(fileName) ) { // Ū�즳�ɮ׬��� 
				cout << "��J�n���檺�ɮ� :" ;
				cin >> fileName ;
			} // while
			
			s = StoreFile() ;
			ShellSort( Job_list.size() ) ;
			SortNewFile( fileName, s ) ;
			PrintTime() ;
		} // if 
		
		else if ( com == "2" ) {
			cout << "��J�n���檺�ɮ� :" ;
			string fileName ;
			cin >> fileName ;
			while ( !Load(fileName) ) { // Ū�즳�ɮ׬��� 
				cout << "��J�n���檺�ɮ� :" ;
				cin >> fileName ;
			} // while
			
			StoreFile() ;
			ShellSort( Job_list.size() ) ;
			CPU cpu ;
			while ( (JobItem < Job_list.size()  ) || !cpu.Q_Jobs.empty() ) { // JobList�٨S�]���BCPU�٦�Job
				if ( !cpu.Q_Jobs.empty() ) { // cpu���F��  
					if ( cpu.Q_Jobs[0].Duration + cpu.Q_Jobs[0].CPU_doing_time == CPU_time ) // ���e���� 
						cpu.Done() ;
					else if ( cpu.Q_Jobs[0].TimeOut == CPU_time && cpu.Q_Jobs[0].Duration + cpu.Q_Jobs[0].CPU_doing_time > cpu.Q_Jobs[0].TimeOut ) 
						cpu.Execute_Abort() ;
					else if ( cpu.Q_Jobs[0].TimeOut == CPU_time && cpu.Q_Jobs[0].Duration + cpu.Q_Jobs[0].CPU_doing_time == cpu.Q_Jobs[0].TimeOut ) 
						cpu.Done() ; // ��n���� 
					// �P�_��C�n�i�JCPU�ɪ��u�@���A�A�w�g�O�ɩέ�n�O�� 
					while ( !cpu.Q_Jobs.empty() && cpu.Q_Jobs[0].CPU_doing_time == 0 && cpu.Q_Jobs[0].TimeOut <= CPU_time  )
						cpu.Execute_Abort() ;
				} // if 
				
				if ( JobItem < Job_list.size() ) {// list�����ŴN��i�� 
					while ( Job_list[JobItem].Arrival == CPU_time ) {
						cpu.Joblist_to_CPU() ;
						JobItem ++ ;
						if ( cpu.Q_Jobs.size() > 4 ) { //�W�L�ƶ��H��
							cpu.Q_fullOut() ;
						} // if
					} // while
				} // if
				
				if ( !cpu.Q_Jobs.empty() && cpu.Q_Jobs[0].CPU_doing_time == 0 ) { // setting the Job Doing Time  �`�N��C���� 
					cpu.Q_Jobs[0].CPU_doing_time = CPU_time ;
				} // if
				 
				
				CPU_time ++ ;
			} // while 
			
			cpu.OutputNewFile(fileName, infile) ;
				
			cpu.Abort_Jobs.clear() ;
			cpu.Done_Jobs.clear() ;
			cpu.Q_Jobs.clear() ;
			CPU_time = 0 ;
			JobItem = 0 ;
		} // else if
		
		Job_list.clear() ;
	} // while
     
} // main()
