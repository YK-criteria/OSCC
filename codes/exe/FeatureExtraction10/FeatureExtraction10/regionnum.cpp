#include "regionnum.h"

RegionNum::RegionNum( ) : dim ( 1 ) , exp_steps( 3 )
{
	//親クラスの変数設定
	dimension = dim ;
	feature = new float[ dimension ]( ) ;
	
	//データなし
	index = NULL ;
}

RegionNum::RegionNum( Pbm image ) : dim ( 1 ) , exp_steps( 3 )
{
	//親クラスの変数設定
	dimension = dim ;
	feature = new float[ dimension ]( ) ;
	
	//imageからファイル名だけ変更
	original = image ;
	original.filename = "original.pbm" ;	

	//大きさとファイル名を指定
	Pbm tmp( original.height , original.width ) ;
	expansion = tmp ;
	expansion.filename = "expansion.pbm" ;

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

RegionNum::RegionNum( const RegionNum &regionnum ) : dim ( 1 ) , exp_steps( 3 )
{
	//親クラスで書いたほうがいいかも？
	dimension = regionnum.dimension ;
	feature = new float[ dimension ]( ) ;
	for( int i = 0 ; i < dimension ; i++ )
	{
		feature[ i ] = regionnum.feature[ i ] ;
	}

	//イメージコピー
	original = regionnum.original ;
	expansion = regionnum.expansion ;

	//index配列の内容コピー
	if( !regionnum.index )
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
				index[ i ][ j ] = regionnum.index[ i ][ j ] ;
			}
		}
	}
}

RegionNum::~RegionNum( )
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

RegionNum& RegionNum::operator=( const RegionNum& regionnum )
{
	//親クラスで書いたほうがいいかも？
	dimension = regionnum.dimension ;
	delete feature ;
	feature = new float[ dimension ] ;
	for( int i = 0 ; i < dimension ; i++ )
	{
		feature[ i ] = regionnum.feature[ i ] ;
	}

	//インデックス配列がない
	if( !regionnum.index )
	{
		original = regionnum.original ;
		expansion = regionnum.expansion ;
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
		original = regionnum.original ;
		expansion = regionnum.expansion ;

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
				index[ i ][ j ] = regionnum.index[ i ][ j ] ;
			}
		}
	}

	return *this ;
}

void RegionNum::Extract( )
{
	//////////////////////////////////////////////////////////////
	//	黒画素膨張
	//////////////////////////////////////////////////////////////	
	for( int i = 0 ; i < exp_steps ; i++ )
	{
		Expand( ) ;
	}
	//膨張画像出力
	expansion.Save( ) ;

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
	//	上皮領域数カウント
	//////////////////////////////////////////////////////////////
	int count = Count( ) ;

	//////////////////////////////////////////////////////////////
	//	特徴量決定
	//////////////////////////////////////////////////////////////
	feature[ 0 ] = count ;
}

void RegionNum::Expand( )
{
	for( int i = 0 ; i < original.height ; i++ )
	{
		for( int j = 0 ; j < original.width ; j++ )
		{
			//一旦元の画素値と同じものを代入
			expansion.data[ i ][ j ] = original.data[ i ][ j ] ;

			//上にアクセスできる
			if( 0 <= i - 1 )
			{
				//上が黒画素
				if( original.data[ i - 1 ][ j ] == BLACK )
				{
					expansion.data[ i ][ j ] = BLACK ;
				}
			}

			//下にアクセスできる
			if( i + 1 < original.height )
			{
				//下が黒画素
				if( original.data[ i + 1 ][ j ] == BLACK )
				{
					expansion.data[ i ][ j ] = BLACK ;
				}
			}

			//左にアクセスできる
			if( 0 <= j - 1 )
			{
				//左が黒画素
				if( original.data[ i ][ j - 1 ] == BLACK )
				{
					expansion.data[ i ][ j ] = BLACK ;
				}
			}

			//右にアクセスできる
			if( j + 1 < original.width )
			{
				if( original.data[ i ][ j + 1 ] == BLACK )
				{
					expansion.data[ i ][ j ] = BLACK ;
				}
			}
		}
	}
}

void RegionNum::Labeling( )
{
	for( bool changed = true ; changed ; )
	{
		//一旦更新なしにする
		changed = false ;

		for( int i = 0 ; i < expansion.height ; i++ )
		{
			for( int j = 0 ; j < expansion.width ; j++ )
			{
				//上にアクセスできる
				if( 0 <= i - 1 )
				{
					//注目している画素のindexより上のindexのほうが小さく自分自身が黒画素
					if( index[ i - 1 ][ j ] < index[ i ][ j ] && expansion.data[ i ][ j ] == BLACK )
					{
						index[ i ][ j ] = index[ i - 1 ][ j ] ;
						changed = true ;
					}
				}

				//下にアクセスできる
				if( i + 1 < expansion.height )
				{
					//注目している画素のindexより下のindexのほうが小さく自分自身が黒画素
					if( index[ i + 1 ][ j ] < index[ i ][ j ] && expansion.data[ i ][ j ] == BLACK )
					{
						index[ i ][ j ] = index[ i + 1 ][ j ] ;
						changed = true ;
					}
				}

				//左にアクセスできる
				if( 0 <= j - 1 )
				{
					//注目している画素のindexより左のindexのほうが小さく自分自身が黒画素
					if( index[ i ][ j - 1 ] < index[ i ][ j ] && expansion.data[ i ][ j ] == BLACK )
					{
						index[ i ][ j ] = index[ i ][ j - 1 ] ;
						changed = true ;
					}
				}

				//右にアクセスできる
				if( j + 1 < expansion.width )
				{
					//注目している画素のindexより右のindexのほうが小さく自分自身が黒画素
					if( index[ i ][ j + 1 ] < index[ i ][ j ] && expansion.data[ i ][ j ] == BLACK )
					{
						index[ i ][ j ] = index[ i ][ j + 1 ] ;
						changed = true ;
					}
				}
			}
		}
	}
}

int RegionNum::Count( )
{
	const int increase = 10 ;	//1回のリスト増加数
	int list_length = increase ;	//リストの長さ
	int list_count = 0 ;	//リストに含まれるindexの数
	int *label_list = new int[ increase ]( ) ;	//リスト
	int all_count = list_count ;	//
	int out_count = 0 ;

	for( int i = 0 ; i < expansion.height ; i++ )
	{
		for( int j = 0 ; j < expansion.width ; j++ )
		{
			//ラベルが最大画素番号以外
			if( index[ i ][ j ] != expansion.height * expansion.width )
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

	//全ての黒画素の集まりの数
	all_count = list_count ;

	//リストの内容をもどす
	list_count = 0 ;

	for( int i = 0 ; i < expansion.height ; i++ )
	{
		for( int j = 0 ; j < expansion.width ; j++ )
		{
			//外枠の黒画素
			if( expansion.data[ i ][ j ] == BLACK && ( i == 0 || i == expansion.height - 1 || j == 0 || j == expansion.width - 1 ) )
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
					//外枠に黒画素は限定的だから全ての黒画素のindexを登録できたlabel_listの
					//リストの長さが足りなくなることは無い					

					//リストに追加
					label_list[ list_count ] = index[ i ][ j ] ;
					//リスト内の数を加算
					list_count++ ;
				}
			}
		}
	}

	//外枠にはみ出ている黒画素の集まりの数
	out_count = list_count ;

	//外枠画素の白画素の連続数をカウント
	vector< int > white_count ;
	int white = 0 ;
	//左上から右上
	for( int i = 0 ; i < original.width ; i++ )
	{
		if( original.data[ 0 ][ i ] == WHITE )
		{
			white++ ;
		}
		else	//if( original.data[ 0 ][ i ] == BLACK )
		{
			//白画素数を保存
			if( 0 < white )
			{
				white_count.push_back( white ) ;
				cout << white << endl ;
			}
			white = 0 ;
			continue ;
		}
	}
	//右上から右下
	for( int i = 0 ; i < original.height ; i++ )
	{
		if( original.data[ i ][ original.width - 1 ] == WHITE )
		{
			white++ ;
		}
		else	//if( original.data[ i ][ original.width - 1 ] == BLACK )
		{
			//白画素数を保存
			if( 0 < white )
			{
				white_count.push_back( white ) ;
			}
			white = 0 ;
			continue ;
		}
	}
	//右下から左下
	for( int i = original.width - 1 ; 0 <= i ; i-- )
	{
		if( original.data[ original.height - 1 ][ i ] == WHITE )
		{
			white++ ;
		}
		else	//if( original.data[ i ][ original.width - 1 ] == BLACK )
		{
			//白画素数を保存
			if( 0 < white )
			{
				white_count.push_back( white ) ;
			}
			white = 0 ;
			continue ;
		}
	}
	//左下から左上 左上の画素は入れない
	for( int i = original.height - 1 ; 0 < i ; i-- )
	{
		if( original.data[ i ][ 0 ] == WHITE )
		{
			white++ ;
		}
		else	//if( original.data[ i ][ original.width - 1 ] == BLACK )
		{
			//白画素数を保存
			if( 0 < white )
			{
				white_count.push_back( white ) ;
			}
			white = 0 ;
			continue ;
		}
	}
	//最後の画素が白で最初の画素も白なら最初の白画素数に最後の白画素数を追加
	if( original.data[ 0 ][ 0 ] == WHITE && original.data[ 1 ][ 0 ] == WHITE )
	{
		white_count[ 0 ] += white ;
	}
	//閾値を満たす白画素数カウント
	int wc = 0;
	for( unsigned int i = 0 ; i < white_count.size( ) ; i++ )
	{
		//閾値以上の白画素なカウント
		if( 0.001f < ( float )white_count[ i ] / ( float )( original.height * original.width ) )
		{
			wc++ ;
		}
	}


	//リスト破棄
	delete[ ] label_list ;

	//cout << all_count << endl ;
	//cout << out_count << endl ;

	//return all_count - out_count + 1 ;
	return all_count - out_count + wc ;
}