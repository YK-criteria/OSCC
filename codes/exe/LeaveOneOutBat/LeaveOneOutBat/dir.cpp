#include "dir.h"

vector<string> Dir::ReadFile( string folder )
{
	// 宣言
	vector<string> fileList ;
	HANDLE hFind ;
	WIN32_FIND_DATA fd ;

	// ファイル名検索のためにワイルドカード追加
	stringstream ss ;
	ss << folder ;
	string::iterator itr = folder.end( ) ;
	itr-- ;
	if( *itr != '\\' ) ss << '\\' ;
	ss << "*.*" ;

	// ファイル探索
	hFind = FindFirstFile( ss.str( ).c_str( ) , &fd ) ;

	// 検索失敗
	if( hFind == INVALID_HANDLE_VALUE )
	{
		std::cout << "ファイル一覧を取得できませんでした" << std::endl;
		exit( 1 ) ; // エラー終了
	}

	// ファイル名をリストに格納するためのループ
	do{
		// フォルダは除く
		if( !( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) &&
			!( fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ) )
		{
			//ファイル名をリストに格納
			char *file = fd.cFileName ;
			string str = file ;
			fileList.push_back( str ) ;
		}
	}
	while( FindNextFile( hFind , &fd ) ) ; //次のファイルを探索
	
	// hFindのクローズ
	FindClose( hFind ) ; 
	return fileList ;
}

vector<string> Dir::ReadFolder( string folder )
{
		// 宣言
	vector<string> fileList ;
	HANDLE hFind ;
	WIN32_FIND_DATA fd ;

	// ファイル名検索のためにワイルドカード追加
	stringstream ss ;
	ss << folder ;
	string::iterator itr = folder.end( ) ;
	itr-- ;
	if( *itr != '\\' ) ss << '\\' ;
	ss << "*.*" ;

	// ファイル探索
	hFind = FindFirstFile( ss.str( ).c_str( ) , &fd ) ;

	// 検索失敗
	if( hFind == INVALID_HANDLE_VALUE )
	{
		std::cout << "ファイル一覧を取得できませんでした" << std::endl;
		exit( 1 ) ; // エラー終了
	}

	// ファイル名をリストに格納するためのループ
	do{
		// フォルダは除く
		if( ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) &&
			!( fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ) )
		{
			//ファイル名をリストに格納
			char *file = fd.cFileName ;
			string str = file ;
			fileList.push_back( str ) ;
		}
	}
	while( FindNextFile( hFind , &fd ) ) ; //次のファイルを探索
	
	// hFindのクローズ
	FindClose( hFind ) ; 
	return fileList ;
}