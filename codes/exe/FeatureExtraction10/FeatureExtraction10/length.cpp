#include "length.h"

Length::Length( ) : dim( 1 )
{
	//親クラスの変数設定
	dimension = dim ;
	feature = new float[ dimension ]( ) ;
}

Length::Length( Pbm image ) : dim( 1 )
{
	//親クラスの変数設定
	dimension = dim ;
	feature = new float[ dimension ]( ) ;

	//入力画像をコピー
	original = image ;
	original.filename = "original.pbm" ;
}

Length::Length( const Length &length ) : dim( 1 )
{
	//親クラスで書いたほうがいいかも？
	dimension = length.dimension ;
	feature = new float[ dimension ]( ) ;
	for( int i = 0 ; i < dimension ; i++ )
	{
		feature[ i ] = length.feature[ i ] ;
	}

	//イメージコピー
	original = length.original ;
}

Length::~Length( )
{
	//デストラクタは処理なし
}

Length& Length::operator=( const Length& length )
{
	//親クラスで書いたほうがいいかも？
	dimension = length.dimension ;
	delete feature ;
	feature = new float[ dimension ] ;
	for( int i = 0 ; i < dimension ; i++ )
	{
		feature[ i ] = length.feature[ i ] ;
	}

	original = length.original ;

	return *this ;
}

void Length::Extract( )
{
	int count = 0 ;	//境界線画素数

	for( int i = 0 ; i < original.height ; i++ )
	{
		for( int j = 0 ; j < original.width ; j++ )
		{
			//自分自身が黒画素
			if( original.data[ i ][ j ] == BLACK )
			{
				bool border = false ;	//境界線フラグ

				//上にアクセスできる
				if( 0 <= i - 1 )
				{
					//上が白画素
					if( original.data[ i - 1 ][ j ] == WHITE )
					{
						border = true ;
					}
				}

				//下にアクセスできる
				if( i + 1 < original.height )
				{
					//下が白画素
					if( original.data[ i + 1 ][ j ] == WHITE )
					{
						border = true ;
					}
				}

				//左にアクセスできる
				if( 0 <= j - 1 )
				{
					//左が白画素
					if( original.data[ i ][ j - 1 ] == WHITE )
					{
						border = true ;
					}
				}

				//右にアクセスできる
				if( j + 1 < original.width )
				{
					//右が白画素
					if( original.data[ i ][ j + 1 ] == WHITE )
					{
						border = true ;
					}
				}

				if( border )
				{
					count++ ;
				}
			}
		}
	}

	feature[ 0 ] = count ;
}
