#include "ppm.h"

Ppm::Ppm( )
{
	filename = "" ;
	type = "P3" ;
	comment = "#comment" ;
	height = 0 ;
	width = 0 ;
	data = NULL ;
}

Ppm::Ppm( string filename )
{
	this->filename = filename ;
	type = "P3" ;
	comment = "#comment" ;
	height = 0 ;
	width = 0 ;
	data = NULL ;
}

Ppm::Ppm( int height , int width )
{
	filename = "" ;
	type = "P3" ;
	comment = "#comment" ;
	this->height = height ;
	this->width = width ;
	data = new int**[ height ]( ) ;
	for( int i = 0 ; i < height ; i++ )
	{
		data[ i ] = new int*[ width ]( ) ;
		for( int j = 0 ; j < width ; j++ )
		{
			data[ i ][ j ] = new int[ 3 ]( ) ;
		}
	}
}

Ppm::Ppm( const Ppm &ppm )
{
	filename = ppm.filename ;
	type = ppm.type ;
	comment = ppm.comment ;
	height = ppm.height ;
	width = ppm.width ;

	//コピー元の配列に内容がない
	if( !ppm.data )
	{
		data = NULL ;
	}
	else
	{
		//メモリ割り当て
		data = new int**[ height ]( ) ;
		for( int i = 0 ; i < height ; i++ )
		{
			data[ i ] = new int*[ width ]( ) ;
			for( int j = 0 ; j < width ; j++ )
			{
				data[ i ][ j ] = new int[ 3 ]( ) ;
			}
		}

		//内容をコピー
		for( int i = 0 ; i < height ; i++ )
		{
			for( int j = 0 ; j < width ; j++ )
			{
				for( int k = 0 ; k < 3 ; k++ )
				{
					data[ i ][ j ][ k ] = ppm.data[ i ][ j ][ k ] ;
				}
			}
		}
	}
}

Ppm::~Ppm( )
{
	if( data )
	{
		for( int i = 0 ; i < height ; i++ )
		{
			for( int j = 0 ; j < width ; j++ )
			{
				if( data[ i ][ j ] )
				{
					delete[ ] data[ i ][ j ] ;
					data[ i ][ j ] = NULL ;
				}
			}
			if( data[ i ] )
			{
				delete[ ] data[ i ] ;
			}
		}
		delete[ ] data ;
		data = NULL ;
	}
}

Ppm& Ppm::operator=( const Ppm& ppm )
{
	//画素データがない
	if( !ppm.data )
	{
		filename = ppm.filename ;
		height = ppm.height ;
		width = ppm.width ;
		type = ppm.type ;
		comment = ppm.comment ;
		data = NULL ;
	}
	else
	{
		if( data )
		{
			//一旦コピー先の配列を破棄
			for( int i = 0 ; i < height ; i++ )
			{
				for( int j = 0 ; j < width ; j++ )
				{
					delete [ ] data[ i ][ j ] ;
					data[ i ][ j ] = NULL ;
				}
				delete[ ] data[ i ] ;
				data[ i ] = NULL ;
			}
			delete[ ] data ;
			data = NULL ;
		}

		//変数の値をコピー
		filename = ppm.filename ;
		height = ppm.height ;
		width = ppm.width ;
		type = ppm.type ;
		comment = ppm.comment ;

		//配列の取り直し
		data = new int**[ height ]( ) ;
		for( int i = 0 ; i < height ; i++ )
		{
			data[ i ] = new int*[ width ]( ) ;
			for( int j = 0 ; j < width ; j++ )
			{
				data[ i ][ j ] = new int[ 3 ]( ) ;
			}
		}

		//配列の内容をコピー
		for( int i = 0 ; i < height ; i++ )
		{
			for( int j = 0 ; j < width ; j++ )
			{
				for( int k = 0 ; k < 3 ; k++ )
				{
					data[ i ][ j ][ k ] = ppm.data[ i ][ j ][ k ] ;
				}
			}
		}
	}

	return *this ;
}

void Ppm::Open( )
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
	if( type != "P3" )
	{
		cout << filename << " is not P1 Ppm." << endl ;
		exit( EXIT_FAILURE ) ;
	}

	//コメント取得
	getline( ifs , comment ) ;

	//サイズ取得
	int new_height , new_width ;
	ifs >> new_width ;
	ifs >> new_height ;

	//最大輝度取得
	int max_intence ;
	ifs >> max_intence ;

	//一旦現在のデータを破棄
	for( int i = 0 ; i < height ; i++ )
	{
		for( int j = 0 ; j < width ; j++ )
		{
			delete[ ] data[ i ][ j ] ;
		}
		delete[ ] data[ i ] ;
		data[ i ] = NULL ;
	}
	delete[ ] data ;
	data = NULL ;

	//新しいサイズに更新してメモリの割り当て
	height = new_height ;
	width = new_width ;
	data = new int**[ height ]( ) ;
	for( int i = 0 ; i < height ; i++ )
	{
		data[ i ] = new int*[ width ]( ) ;
		for( int j = 0 ; j < width ; j++ )
		{
			data[ i ][ j ] = new int[ 3 ]( ) ;
		}
	}
	
	//ファイルからデータコピー
	for( int i = 0 ; i < height ; i++ )
	{
		for( int j = 0 ; j < width ; j++ )
		{
			int r , g , b ;
			ifs >> r >> g >> b ;
			data[ i ][ j ][ 0 ] = r ;
			data[ i ][ j ][ 1 ] = g ;
			data[ i ][ j ][ 2 ] = b ;
		}
	}
}

void Ppm::Open( string filename )
{
	//新しいファイル名に更新してからファイルオープン
	this->filename = filename ;
	Open( ) ;
}

void Ppm::Save( )
{
	ofstream ofs( filename ) ;	//ファイルストリーム

	//ファイルオープンエラー
	if( ofs.fail( ) )
	{
		cout << filename << " can't open." << endl ;
		exit( EXIT_FAILURE ) ;
	}

	//ファイルタイプ コメント サイズ 最大輝度出力
	ofs << type << endl ;
	ofs << comment << endl ;
	ofs << width << " " << height << endl ;
	ofs << 255 << endl ;

	//画素データ出力
	for( int i = 0 ; i < height ; i++ )
	{
		for( int  j = 0 ; j < width ; j++ )
		{
			ofs << data[ i ][ j ][ 0 ] << " " << data[ i ][ j ][ 1 ] << " " << data[ i ][ j ][ 2 ] << endl ;
		}
	}
}

void Ppm::Save( string filename )
{
	//新しいファイル名に更新してからファイルセーブ
	this->filename = filename ;
	Save( ) ;
}

void Ppm::Print( )
{
	for( int i = 0 ; i < height ; i++ )
	{
		for( int j = 0 ; j < width ; j++ )
		{
			cout << data[ i ][ j ][ 0 ] << " " << data[ i ][ j ][ 1 ] << " " << data[ i ][ j ][ 2 ] << endl ;
		}
		cout << endl ;
	}
}
