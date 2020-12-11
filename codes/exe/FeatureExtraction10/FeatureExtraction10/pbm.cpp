#include "pbm.h"

Pbm::Pbm( )
{
	filename = "" ;
	type = "P1" ;
	comment = "#comment" ;
	height = 0 ;
	width = 0 ;
	data = NULL ;
}

Pbm::Pbm( string filename )
{
	this->filename = filename ;
	type = "P1" ;
	comment = "#comment" ;
	height = 0 ;
	width = 0 ;
	data = NULL ;
}

Pbm::Pbm( int height , int width )
{
	filename = "" ;
	type = "P1" ;
	comment = "#comment" ;
	this->height = height ;
	this->width = width ;
	data = new PbmColor*[ height ]( ) ;
	for( int i = 0 ; i < height ; i++ )
	{
		data[ i ] = new PbmColor[ width ]( ) ;
	}
}

Pbm::Pbm( const Pbm &pbm )
{
	filename = pbm.filename ;
	type = pbm.type ;
	comment = pbm.comment ;
	height = pbm.height ;
	width = pbm.width ;

	//コピー元の配列に内容がない
	if( !pbm.data )
	{
		data = NULL ;
	}
	else
	{
		//メモリ割り当て
		data = new PbmColor*[ height ]( ) ;
		for( int i = 0 ; i < height ; i++ )
		{
			data[ i ] = new PbmColor[ width ]( ) ;
		}

		//内容をコピー
		for( int i = 0 ; i < height ; i++ )
		{
			for( int j = 0 ; j < width ; j++ )
			{
				data[ i ][ j ] = pbm.data[ i ][ j ] ;
			}
		}
	}
}

Pbm::~Pbm( )
{
	if( data )
	{
		for( int i = 0 ; i < height ; i++ )
		{
			if( data[ i ] )
			{
				delete[ ] data[ i ] ;
				data[ i ] = NULL ;
			}
		}
		delete[ ] data ;
		data = NULL ;
	}
}

Pbm& Pbm::operator=( const Pbm& pbm )
{
	//画素データがない
	if( !pbm.data )
	{
		filename = pbm.filename ;
		height = pbm.height ;
		width = pbm.width ;
		type = pbm.type ;
		comment = pbm.comment ;
		data = NULL ;
	}
	else
	{
		if( data )
		{
			//一旦コピー先の配列を破棄
			for( int i = 0 ; i < height ; i++ )
			{
				delete[ ] data[ i ] ;
				data[ i ] = NULL ;
			}
			delete[ ] data ;
			data = NULL ;
		}

		//変数の値をコピー
		filename = pbm.filename ;
		height = pbm.height ;
		width = pbm.width ;
		type = pbm.type ;
		comment = pbm.comment ;

		//配列の取り直し
		data = new PbmColor*[ height ]( ) ;
		for( int i = 0 ; i < height ; i++ )
		{
			data[ i ] = new PbmColor[ width ]( ) ;
		}

		//配列の内容をコピー
		for( int i = 0 ; i < height ; i++ )
		{
			for( int j = 0 ; j < width ; j++ )
			{
				data[ i ][ j ] = pbm.data[ i ][ j ] ;
			}
		}
	}

	return *this ;
}

void Pbm::Open( )
{
	ifstream ifs( filename ) ;	//ファイルストリーム

	//ファイルオープンエラー
	if( !ifs )
	{
		cout << filename << " can't open." << endl ;
		exit( EXIT_FAILURE ) ;
	}

	//ファイルタイプ取得
	getline( ifs , type ) ;

	//ファイルタイプがP1のモノクロテキストタイプでないときは強制終了
	if( type != "P1" )
	{
		cout << filename << " is not P1 Pbm." << endl ;
		exit( EXIT_FAILURE ) ;
	}

	//コメント取得
	getline( ifs , comment ) ;

	//サイズ取得
	int new_height , new_width ;
	ifs >> new_width ;
	ifs >> new_height ;

	//一旦現在のデータを破棄
	for( int i = 0 ; i < height ; i++ )
	{
		delete[ ] data[ i ] ;
		data[ i ] = NULL ;
	}
	delete[ ] data ;
	data = NULL ;

	//新しいサイズに更新してメモリの割り当て
	height = new_height ;
	width = new_width ;
	data = new PbmColor*[ height ]( ) ;
	for( int i = 0 ; i < height ; i++ )
	{
		data[ i ] = new PbmColor[ width ]( ) ;
	}
	
	//ファイルからデータコピー
	for( int i = 0 ; i < height ; i++ )
	{
		for( int j = 0 ; j < width ; j++ )
		{
			string s = "" ;
			ifs >> s ;
			data[ i ][ j ] = s == "1" ? BLACK : WHITE ;
		}
	}
}

void Pbm::Open( string filename )
{
	//新しいファイル名に更新してからファイルオープン
	this->filename = filename ;
	Open( ) ;
}

void Pbm::Save( )
{
	ofstream ofs( filename ) ;	//ファイルストリーム

	//ファイルオープンエラー
	if( ofs.fail( ) )
	{
		cout << filename << " can't open." << endl ;
		exit( EXIT_FAILURE ) ;
	}

	//ファイルタイプ コメント サイズ出力
	ofs << type << endl ;
	ofs << comment << endl ;
	ofs << width << " " << height << endl ;

	//画素データ出力
	for( int i = 0 ; i < height ; i++ )
	{
		for( int  j = 0 ; j < width ; j++ )
		{
			string out = data[ i ][ j ] == BLACK ? "1 " : "0 " ;
			ofs << out  ;
		}
		ofs << endl ;
	}
	ofs << endl ;
}

void Pbm::Save( string filename )
{
	//新しいファイル名に更新してからファイルセーブ
	this->filename = filename ;
	Save( ) ;
}

void Pbm::Print( )
{
	for( int i = 0 ; i < height ; i++ )
	{
		for( int j = 0 ; j < width ; j++ )
		{
			cout << data[ i ][ j ] << " " ;
		}
		cout << endl ;
	}
}
