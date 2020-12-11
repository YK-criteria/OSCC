#include "main.h"

int main( int argc , char *argv[ ] )
{
	//引数チェック
	if( argc != 2 )
	{
		cout << "input 'execute file' 'initilize file'" << endl ;
		exit( EXIT_FAILURE ) ;
	}

	//ファイルを指定してランダムフォレストクラスを初期化
	RandomForest rf( argv[ 1 ] ) ;

	//初期化に基づいて訓練
	rf.Train( ) ;

	//テストとテスト結果を表示
	rf.Test( ) ;
 }

