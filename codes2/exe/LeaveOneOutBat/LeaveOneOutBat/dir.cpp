#include "dir.h"

vector<string> Dir::ReadFile( string folder )
{
	vector<string> fileList ;
	HANDLE hFind ;
	WIN32_FIND_DATA fd ;

	stringstream ss ;
	ss << folder ;
	string::iterator itr = folder.end( ) ;
	itr-- ;
	if( *itr != '\\' ) ss << '\\' ;
	ss << "*.*" ;

	hFind = FindFirstFile( ss.str( ).c_str( ) , &fd ) ;

	if( hFind == INVALID_HANDLE_VALUE )
	{
		std::cout << "Could not get a list of file names." << std::endl;
		exit( 1 ) ;
	}

	do{
		if( !( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) &&
			!( fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ) )
		{
			char *file = fd.cFileName ;
			string str = file ;
			fileList.push_back( str ) ;
		}
	}
	while( FindNextFile( hFind , &fd ) ) ;
	
	FindClose( hFind ) ; 
	return fileList ;
}

vector<string> Dir::ReadFolder( string folder )
{
	vector<string> fileList ;
	HANDLE hFind ;
	WIN32_FIND_DATA fd ;

	stringstream ss ;
	ss << folder ;
	string::iterator itr = folder.end( ) ;
	itr-- ;
	if( *itr != '\\' ) ss << '\\' ;
	ss << "*.*" ;

	hFind = FindFirstFile( ss.str( ).c_str( ) , &fd ) ;

	if( hFind == INVALID_HANDLE_VALUE )
	{
		std::cout << "Could not get a list of file names." << std::endl;
		exit( 1 ) ; // ƒGƒ‰[I—¹
	}

	do{
		if( ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) &&
			!( fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ) )
		{
			char *file = fd.cFileName ;
			string str = file ;
			fileList.push_back( str ) ;
		}
	}
	while( FindNextFile( hFind , &fd ) ) ;
	
	FindClose( hFind ) ; 
	return fileList ;
}