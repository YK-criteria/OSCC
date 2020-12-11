#include "main.h"

int main( int argc , char *argv[ ] )
{
	//引数エラーチェック
	if( argc != 2 )
	{
		printf( "command '～.exe' '～.pbm'\n" ) ;
		exit( EXIT_FAILURE ) ;
	}

	//特徴抽出機の初期化　特徴抽出を行うファイル名を引数に取る
	Extractor extractor( argv[ 1 ] ) ;

	//特徴抽出
	extractor.Extract( ) ;

	//結果出力
	extractor.Output( ) ;

	return 0 ;
}