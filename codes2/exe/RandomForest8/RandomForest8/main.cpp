#include "main.h"

int main( int argc , char *argv[ ] )
{
	if( argc != 2 )
	{
		cout << "input 'execute file' 'initilize file'" << endl ;
		exit( EXIT_FAILURE ) ;
	}

	RandomForest rf( argv[ 1 ] ) ;

	rf.Train( ) ;

	rf.Test( ) ;
 }

