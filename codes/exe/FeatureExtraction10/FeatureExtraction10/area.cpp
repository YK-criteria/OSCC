#include "area.h"

Area::Area( ) : dim( 1 ) , increase( 10 ) , threshold( 0.001 )
{
	//親クラスの変数設定
	dimension = dim ;
	feature = new float[ dimension ]( ) ;
	
	//データなし
	index = NULL ;
}

Area::Area( Pbm image ) : dim( 1 ) , increase( 10 ) , threshold( 0.001 )
{
	//親クラスの変数設定
	dimension = dim ;
	feature = new float[ dimension ]( ) ;
	
	//imageからファイル名だけ変更
	original = image ;
	original.filename = "original.pbm" ;	

	index = new int*[ image.height ]( ) ;
	//画像からindexを決定
	for( int i = 0 ; i < image.height ; i++ )
	{
		index[ i ] = new int[ image.width ]( ) ;

		for( int j = 0 ; j < image.width ; j++ )
		{
			//画素値コピー
			original.data[ i ][ j ] = image.data[ i ][ j ] ;

			//黒画素
			if( original.data[ i ][ j ] == BLACK )
			{
				//通し番号で初期化
				index[ i ][ j ] = i * image.width + j ;
			}
			//白画素
			else
			{
				//最大番号で初期化　ラベル伝播に関係ないならどんな数でもよい
				index[ i ][ j ] = image.height * image.width ;
			}
		}
	}
}

Area::Area( const Area &area ) : dim( 1 ) , increase( 10 ) , threshold( 0.001 )
{
	//親クラスで書いたほうがいいかも？
	dimension = area.dimension ;
	feature = new float[ dimension ]( ) ;
	for( int i = 0 ; i < dimension ; i++ )
	{
		feature[ i ] = area.feature[ i ] ;
	}

	//イメージコピー
	original = area.original ;

	//index配列の内容コピー
	if( !area.index )
	{
		index = NULL ;
	}
	else
	{
		//メモリ割り当て
		index = new int*[ original.height ]( ) ;
		for( int i = 0 ; i < original.height ; i++ )
		{
			index[ i ] = new int[ original.width ]( ) ;
		}

		//内容をコピー
		for( int i = 0 ; i < original.height ; i++ )
		{
			for( int j = 0 ; i < original.width ; j++ )
			{
				index[ i ][ j ] = area.index[ i ][ j ] ;
			}
		}
	}
}

Area::~Area( )
{
	if( index )
	{
		for( int i = 0 ; i < original.height ; i++ )
		{
			if( index[ i ] )
			{
				delete[ ] index[ i ] ;
				index[ i ] = NULL ;
			}
		}
			delete[ ] index ;
		index = NULL ;
	}
}

Area& Area::operator=( const Area& area )
{
	//親クラスで書いたほうがいいかも？
	dimension = area.dimension ;
	delete feature ;
	feature = new float[ dimension ] ;
	for( int i = 0 ; i < dimension ; i++ )
	{
		feature[ i ] = area.feature[ i ] ;
	}

	//インデックス配列がない
	if( !area.index )
	{
		original = area.original ;
		index = NULL ;
	}
	else
	{
		//一旦現在の配列を破棄
		for( int i = 0 ; i < original.height ; i++ )
		{
			delete[ ] index[ i ] ;
		}
		delete[ ] index ;

		//変数の値をコピー
		original = area.original ;

		//配列の取り直し
		index = new int*[ original.height ]( ) ;
		for( int i = 0 ; i < original.height ; i++ )
		{
			index[ i ] = new int[ original.width ]( ) ;
		}

		//配列の内容をコピー
		for( int i = 0 ; i < original.height ; i++ )
		{
			for( int j = 0 ; j < original.width ; j++ )
			{
				index[ i ][ j ] = area.index[ i ][ j ] ;
			}
		}
	}

	return *this ;
}

void Area::Extract( )
{
	//////////////////////////////////////////////////////////////
	//	ラベリング
	//////////////////////////////////////////////////////////////	
	Labeling( ) ;
	/*for( int i = 0 ; i < expansion.height ; i++ )
	{
		for( int j = 0 ; j < expansion.width ; j++ )
		{
			printf( "%5d " , index[ i ][ j ] ) ;
		}
		cout << endl ;
	}
	cout << endl ;*/

	//////////////////////////////////////////////////////////////
	//	黒画素領域数カウント
	//////////////////////////////////////////////////////////////
	Count( ) ;

	//////////////////////////////////////////////////////////////
	//	黒画素領域の面積をリスト化
	//////////////////////////////////////////////////////////////
	MakeList( ) ;

	//////////////////////////////////////////////////////////////
	//　閾値を満たす黒画素領域をカウント
	//////////////////////////////////////////////////////////////
	int countsmall = CountSmall( ) ;

	//////////////////////////////////////////////////////////////
	//	特徴量決定
	//////////////////////////////////////////////////////////////
	feature[ 0 ] = countsmall ;
}

void Area::Labeling( )
{
	for( bool changed = true ; changed ; )
	{
		//一旦更新なしにする
		changed = false ;

		for( int i = 0 ; i < original.height ; i++ )
		{
			for( int j = 0 ; j < original.width ; j++ )
			{
				//上にアクセスできる
				if( 0 <= i - 1 )
				{
					//注目している画素のindexより上のindexのほうが小さく自分自身が黒画素
					if( index[ i - 1 ][ j ] < index[ i ][ j ] && original.data[ i ][ j ] == BLACK )
					{
						index[ i ][ j ] = index[ i - 1 ][ j ] ;
						changed = true ;
					}
				}

				//下にアクセスできる
				if( i + 1 < original.height )
				{
					//注目している画素のindexより下のindexのほうが小さく自分自身が黒画素
					if( index[ i + 1 ][ j ] < index[ i ][ j ] && original.data[ i ][ j ] == BLACK )
					{
						index[ i ][ j ] = index[ i + 1 ][ j ] ;
						changed = true ;
					}
				}

				//左にアクセスできる
				if( 0 <= j - 1 )
				{
					//注目している画素のindexより左のindexのほうが小さく自分自身が黒画素
					if( index[ i ][ j - 1 ] < index[ i ][ j ] && original.data[ i ][ j ] == BLACK )
					{
						index[ i ][ j ] = index[ i ][ j - 1 ] ;
						changed = true ;
					}
				}

				//右にアクセスできる
				if( j + 1 < original.width )
				{
					//注目している画素のindexより右のindexのほうが小さく自分自身が黒画素
					if( index[ i ][ j + 1 ] < index[ i ][ j ] && original.data[ i ][ j ] == BLACK )
					{
						index[ i ][ j ] = index[ i ][ j + 1 ] ;
						changed = true ;
					}
				}
			}
		}
	}
}

void Area::Count( )
{
	list_length = increase ;	//リストの長さ
	list_count = 0 ;	//リストに含まれるindexの数
	label_list = new int[ increase ]( ) ;	//リスト

	for( int i = 0 ; i < original.height ; i++ )
	{
		for( int j = 0 ; j < original.width ; j++ )
		{
			//ラベルが最大画素番号以外
			if( index[ i ][ j ] != original.height * original.width )
			{
				bool exist = false ;	//リストに一致する番号があるかフラグ

				//リストにこの番号があるか探索
				for( int k = 0 ; k < list_count ; k++ )
				{
					//リストの番号と今注目している番号が一致
					if( label_list[ k ] == index[ i ][ j ] )
					{
						exist = true ;
					}
				}

				//リストにない
				if( !exist )
				{
					//リストの長さと中に入っている数が同じで長さが足りない
					if( list_count == list_length )
					{
						//新しいリスト
						int new_list_length = list_length + increase ;
						int *new_list = new int[ new_list_length ]( ) ;

						//古いリストの中のものをコピー
						for( int k = 0 ; k < list_length ; k++ )
						{
							new_list[ k ] = label_list[ k ] ;
						}

						//古いリストを破棄
						delete[ ] label_list ;

						//新しいものに更新
						label_list = new_list ;
						list_length = new_list_length ;
					}

					//リストに追加
					label_list[ list_count ] = index[ i ][ j ] ;
					//リスト内の数を加算
					list_count++ ;
				}
			}
		}
	}
}

void Area::MakeList( )
{
	//領域面積リスト確保
	area_list = new int[ list_count ]( ) ;

	//領域面積リスト化
	for( int i = 0 ; i < original.height ; i++ )
	{
		for( int j = 0 ; j < original.width  ; j++ )
		{
			bool exist = false ;

			//リストの中に今注目している了領域番号があるか探索
			for( int k = 0 ; k < list_count ; k++ )
			{
				//領域番号を発見
				if( index[ i ][ j ] == label_list[ k ] )
				{
					area_list[ k ]++ ;
					break ;
				}
			}
		}
	}
}

int Area::CountSmall( )
{
	int count = 0 ;	//領域数カウンタ

	for( int i = 0 ; i < list_count ; i++ )
	{
		//閾値を満たすならカウンタ増加
		if( ( double )area_list[ i ] / ( double )( original.height * original.width ) < threshold )
		{
			count++ ;
		}
	}

	return count ;
}