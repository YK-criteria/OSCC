#include "curvature.h"

Curvature::Curvature( ) : dim( 1 ) , increase( 10 ) , threshold( 3.f )
{
	//親クラスの変数設定
	dimension = dim ;
	feature = new float[ dimension ]( ) ;
}

Curvature::Curvature( Pbm image ) : dim( 1 ) , increase( 10 ) , threshold( 3.f )
{
	//親クラスの変数設定
	dimension = dim ;
	feature = new float[ dimension ]( ) ;
	
	//imageからファイル名だけ変更
	original = image ;
	original.filename = "original.pbm" ;
}

Curvature::Curvature( const Curvature &curvature ) : dim( 3 ) , increase( 10 ) , threshold( 3.f )
{
	//親クラスで書いたほうがいいかも？
	dimension = curvature.dimension ;
	feature = new float[ dimension ]( ) ;
	for( int i = 0 ; i < dimension ; i++ )
	{
		feature[ i ] = curvature.feature[ i ] ;
	}

	//イメージコピー
	original = curvature.original ;
}

Curvature::~Curvature( )
{
	
}

Curvature& Curvature::operator=( const Curvature& curvature )
{
	//親クラスで書いたほうがいいかも？
	dimension = curvature.dimension ;
	original = curvature.original ;
	delete feature ;
	feature = new float[ dimension ] ;
	for( int i = 0 ; i < dimension ; i++ )
	{
		feature[ i ] = curvature.feature[ i ] ;
	}

	return *this ;
}

void Curvature::Extract( )
{
	//////////////////////////////////////////////////////////////
	//	境界線画素を判別
	//////////////////////////////////////////////////////////////
	SetIsBorder( ) ;

	//////////////////////////////////////////////////////////////
	//	境界線にラベリング
	//////////////////////////////////////////////////////////////
	Labeling( ) ;

	//////////////////////////////////////////////////////////////
	//	曲率計算
	//////////////////////////////////////////////////////////////
	Calc( ) ;

	//////////////////////////////////////////////////////////////
	//	出力
	//////////////////////////////////////////////////////////////
	Print( ) ;
}

void Curvature::SetIsBorder( )
{
	//領域確保
	is_border = new bool*[ original.height ]( ) ;
	for( int i = 0 ; i < original.height ; i++ )
	{
		is_border[ i ] = new bool[ original.width ]( ) ;
		for( int j = 0 ; j < original.width ; j++ )
		{
			is_border[ i ][ j ] = false ;
		}
	}

	//境界線画素の判別
	for( int i = 0 ; i < original.height ; i++ )
	{
		for( int j = 0 ; j < original.width ; j++ )
		{
			//自分自身が黒画素
			if( original.data[ i ][ j ] == BLACK )
			{
				//上にアクセス可能
				if( 0 <= i - 1 )
				{
					//上が白画素
					if( original.data[ i - 1 ][ j ] == WHITE )
					{
						is_border[ i ][ j ] = true ;
					}
				}
				////右上にアクセス可能
				//if( 0 <= i - 1 && j + 1 < original.width )
				//{
				//	//右上が白画素
				//	if( original.data[ i - 1 ][ j + 1 ] == WHITE )
				//	{
				//		is_border[ i ][ j ] = true ;
				//	}
				//}
				//右にアクセス可能
				if( j + 1 < original.width )
				{
					//右が白画素
					if( original.data[ i ][ j + 1 ] == WHITE )
					{
						is_border[ i ][ j ] = true ;
					}
				}
				////右下にアクセス可能
				//if( i + 1 < original.height && j + 1 < original.width )
				//{
				//	//右下が白画素
				//	if( original.data[ i + 1 ][ j + 1 ] == WHITE )
				//	{
				//		is_border[ i ][ j ] = true ;
				//	}
				//}
				//下にアクセス可能
				if( i + 1 < original.height )
				{
					//下が白画素
					if( original.data[ i + 1 ][ j ] == WHITE )
					{
						is_border[ i ][ j ] = true ;
					}
				}
				////左下にアクセス可能
				//if( i + 1 < original.height && 0 <= j - 1 )
				//{
				//	if( original.data[ i + 1 ][ j - 1 ] == WHITE )
				//	{
				//		is_border[ i ][ j ] = true ;
				//	}
				//}
				//左にアクセス可能
				if( 0 <= j - 1 )
				{
					if( original.data[ i ][ j - 1 ] == WHITE )
					{
						is_border[ i ][ j ] = true ;
					}
				}
				////左上にアクセス可能
				//if( 0 <= i - 1 && 0 <= j - 1 )
				//{
				//	if( original.data[ i - 1 ][ j - 1 ] == WHITE )
				//	{
				//		is_border[ i ][ j ] = true ;
				//	}
				//}
			}
		}
	}

	//確認用
	/*for( int i = 0 ; i < original.height ; i++ )
	{
		for( int j = 0 ; j < original.width ; j++ )
		{
			cout << ( is_border[ i ][ j ] == true ? "t" : " " ) << " " ;
		}
	
		cout << endl ;
	}*/
}

void Curvature::Labeling( )
{
	//インデックス配列初期化
	index = new int*[ original.height ]( ) ;
	for( int i = 0 ; i < original.height ; i++ )
	{
		index[ i ] = new int[ original.width ]( ) ;
		for( int j = 0 ; j < original.width ; j++ )
		{
			index[ i ][ j ] = original.height * original.width ;
		}
	}
	//ラベル配列初期化
	label = new int*[ original.height ]( ) ;
	for( int i = 0 ; i < original.height ; i++ )
	{
		label[ i ] = new int[ original.width ]( ) ;
		for( int j = 0 ; j < original.width ; j++ )
		{
			label[ i ][ j ] = -1 ;
		}
	}

	int count = 0 ;

	//枠から切れている境界線
	for( int i = 0 ; i < original.height ; i++ )
	{
		for( int j = 0 ; j < original.width ; j++ )
		{
			//枠で境界線
			if( ( i == 0 || i == original.height - 1 || j == 0 || j == original.width - 1 ) && is_border[ i ][ j ] && index[ i ][ j ] == original.height * original.width )
			{
				index[ i ][ j ] = 0 ;
				label[ i ][ j ] = count ;

				//ラベリング
				bool changed = true ;
				for( ; changed ; )
				{
					//ループ終了条件に設定　ラベリングが行われないならそのまま抜ける
					changed = false ;

					//ラベリング
					for( int i = 0 ; i < original.height ; i++ )
					{
						for( int j = 0 ; j < original.width ; j++ )
						{
							//自分が境界線画素
							if( is_border[ i ][ j ] )
							{
								int min_index = index[ i ][ j ] ;	//自分のインデックスで初期化

								//左上にアクセスできる
								if( 0 <= i - 1 && 0 <= j - 1 )
								{
									//左上が境界線
									if( is_border[ i - 1 ][ j - 1 ] )
									{
										//最小値より小さいなら更新
										if( index[ i - 1 ][ j - 1 ] < min_index )
										{
											min_index = index[ i - 1 ][ j - 1 ] ;
										}
									}
								}
								//左にアクセスできる
								if( 0 <= j - 1 )
								{
									//左が境界線
									if( is_border[ i ][ j - 1 ] )
									{
										//最小値より小さいなら更新
										if( index[ i ][ j - 1 ] < min_index )
										{
											min_index = index[ i ][ j - 1 ] ;
										}
									}
								}
								//左下にアクセスできる
								if( i + 1 < original.height && 0 <= j - 1 )
								{
									//左下が境界線
									if( is_border[ i + 1 ][ j - 1 ] )
									{
										//最小値より小さいなら更新
										if( index[ i + 1 ][ j - 1 ] < min_index )
										{
											min_index = index[ i + 1 ][ j - 1 ] ;
										}
									}
								}
								//下にアクセスできる
								if( i + 1 < original.height )
								{
									//下が境界線
									if( is_border[ i + 1 ][ j ] )
									{
										//最小値より小さいなら更新
										if( index[ i + 1 ][ j ] < min_index )
										{
											min_index = index[ i + 1 ][ j ] ;
										}
									}
								}
								//右下にアクセスできる
								if( i + 1 < original.height && j + 1 < original.width )
								{
									//右下が境界線
									if( is_border[ i + 1 ][ j + 1 ] )
									{
										//最小値より小さいなら更新
										if( index[ i + 1 ][ j + 1 ] < min_index )
										{
											min_index = index[ i + 1 ][ j + 1 ] ;
										}
									}
								}
								//右にアクセスできる
								if( j + 1 < original.width )
								{
									//右が境界線
									if( is_border[ i ][ j + 1 ] )
									{
										//最小値より小さいなら更新
										if( index[ i ][ j + 1 ] < min_index )
										{
											min_index = index[ i ][ j + 1 ] ;
										}
									}
								}
								//右上にアクセスできる
								if( 0 <= i - 1 && j + 1 < original.width )
								{
									//右上が境界線
									if( is_border[ i - 1 ][ j + 1 ] )
									{
										//最小値より小さいなら更新
										if( index[ i - 1 ][ j + 1 ] < min_index )
										{
											min_index = index[ i - 1 ][ j + 1 ] ;
										}
									}
								}
								//上にアクセスできる
								if( 0 <= i - 1 )
								{
									//上が境界線
									if( is_border[ i - 1 ][ j ] )
									{
										//最小値より小さいなら更新
										if( index[ i - 1 ][ j ] < min_index )
										{
											min_index = index[ i - 1 ][ j ] ;
										}
									}
								}

								//最小値が最大値(境界線の画素ではあるがまだ伝播してきていない)
								//自分自身-最小値が1のとき
								//最小値が自分自身　これらのときは更新しない
								if( min_index != original.height * original.width &&
									index[ i ][ j ] - min_index != 1 && 
									min_index != index[ i ][ j ] )
								{
									index[ i ][ j ] = min_index + 1 ;
									label[ i ][ j ] = count ;
									changed = true ;
								}
							}
						}
					}
				}
				
				//今行ったラベリングの境界線の末尾を検索
				int max = 0 ;
				int max_i = 0 ;
				int max_j = 0 ;
				for( int i = 0 ; i < original.height ; i++ )
				{
					for( int j = 0 ; j < original.width ; j++ )
					{
						if( label[ i ][ j ] == count && max < index[ i ][ j ] )
						{
							max = index[ i ][ j ] ;
							max_i = i ;
							max_j = j ;
						}
					}
				}
				
				//printf( "count %d max %d max_i %d max_j %d\n" , count , max , max_i , max_j ) ;

				//今の境界線を登録
				borderlist.Add( ) ;

				//今の境界線を枠から切れている境界線としてフラグを設定
				borderlist.border[ borderlist.num - 1 ].island = false ;

				int current_label = index[ max_i ][ max_j ] ;
				int current_i = max_i ;
				int current_j = max_j ;
				for( ; ; )
				{
					//今の座標を登録
					borderlist.border[ borderlist.num - 1 ].Add( current_j , current_i ) ;

					//座標が始点に帰ってきたら終了
					if( current_label == 0 )
					{
						break ;
					}

					//次の座標は近傍の1小さいものを選ぶ
					int min_index = index[ current_i ][ current_j ] ;	//自分のインデックスで初期化
					int min_i = current_i ;
					int min_j = current_j ;
					//左上にアクセスできる
					if( 0 <= current_i - 1 && 0 <= current_j - 1 )
					{
						//左上が境界線
						if( is_border[ current_i - 1 ][ current_j - 1 ] )
						{
							//最小値より小さいなら更新
							if( index[ current_i - 1 ][ current_j - 1 ] < min_index )
							{
								min_index = index[ current_i - 1 ][ current_j - 1 ] ;
								min_i = current_i - 1 ;
								min_j = current_j - 1 ;
							}
						}
					}
					//左にアクセスできる
					if( 0 <= current_j - 1 )
					{
						//左が境界線
						if( is_border[ current_i ][ current_j - 1 ] )
						{
							//最小値より小さいなら更新
							if( index[ current_i ][ current_j - 1 ] < min_index )
							{
								min_index = index[ current_i ][ current_j - 1 ] ;
								min_i = current_i ;
								min_j = current_j - 1 ;
							}
						}
					}
					//左下にアクセスできる
					if( current_i + 1 < original.height && 0 <= current_j - 1 )
					{
						//左下が境界線
						if( is_border[ current_i + 1 ][ current_j - 1 ] )
						{
							//最小値より小さいなら更新
							if( index[ current_i + 1 ][ current_j - 1 ] < min_index )
							{
								min_index = index[ current_i + 1 ][ current_j - 1 ] ;
								min_i = current_i + 1 ;
								min_j = current_j - 1 ;
							}
						}
					}
					//下にアクセスできる
					if( current_i + 1 < original.height )
					{
						//下が境界線
						if( is_border[ current_i + 1 ][ current_j ] )
						{
							//最小値より小さいなら更新
							if( index[ current_i + 1 ][ current_j ] < min_index )
							{
								min_index = index[ current_i + 1 ][ current_j ] ;
								min_i = current_i + 1 ;
								min_j = current_j ;
							}
						}
					}
					//右下にアクセスできる
					if( current_i + 1 < original.height && current_j + 1 < original.width )
					{
						//右下が境界線
						if( is_border[ current_i + 1 ][ current_j + 1 ] )
						{
							//最小値より小さいなら更新
							if( index[ current_i + 1 ][ current_j + 1 ] < min_index )
							{
								min_index = index[ current_i + 1 ][ current_j + 1 ] ;
								min_i = current_i + 1 ;
								min_j = current_j + 1 ;
							}
						}
					}
					//右にアクセスできる
					if( current_j + 1 < original.width )
					{
						//右が境界線
						if( is_border[ current_i ][ current_j + 1 ] )
						{
							//最小値より小さいなら更新
							if( index[ current_i ][ current_j + 1 ] < min_index )
							{
								min_index = index[ current_i ][ current_j + 1 ] ;
								min_i = current_i ;
								min_j = current_j + 1 ;
							}
						}
					}
					//右上にアクセスできる
					if( 0 <= current_i - 1 && current_j + 1 < original.width )
					{
						//右上が境界線
						if( is_border[ current_i - 1 ][ current_j + 1 ] )
						{
							//最小値より小さいなら更新
							if( index[ current_i - 1 ][ current_j + 1 ] < min_index )
							{
								min_index = index[ current_i - 1 ][ current_j + 1 ] ;
								min_i = current_i - 1 ;
								min_j = current_j + 1 ;
							}
						}
					}
					//上にアクセスできる
					if( 0 <= current_i - 1 )
					{
						//上が境界線
						if( is_border[ current_i - 1 ][ current_j ] )
						{
							//最小値より小さいなら更新
							if( index[ current_i - 1 ][ current_j ] < min_index )
							{
								min_index = index[ current_i - 1 ][ current_j ] ;
								min_i = current_i - 1 ;
								min_j = current_j ;
							}
						}
					}

					//座標を移動
					current_label = min_index ;
					current_i = min_i ;
					current_j = min_j ;
				}

				//ラベル番号を更新
				count++ ;
			}
		}
	}

	/*for( int i = 0 ; i < borderlist.num ; i++ )
	{
		for( int j = 0 ; j < borderlist.border[ i ].length ; j++ )
		{
			printf( "%d %d\n" , borderlist.border[ i ].point[ j ].x , borderlist.border[ i ].point[ j ].y ) ;
		}
	}*/
	

	//枠内にある境界線
	for( int i = 1 ; i < original.height - 1 ; i++ )
	{
		for( int j = 1 ; j < original.width - 1 ; j++ )
		{
			//境界線画素
			if( is_border[ i ][ j ] )
			{
				//ラベリングが済んでいない
				if( label[ i ][ j ] == -1 )
				{
					//境界線を追跡しながらラベリングと境界線登録
					int sx = j ;	//初期値のx座標
					int sy = i ;	//初期値のy座標
					int tx = j ;	//追跡点のx座標
					int ty = i ;	//追跡点のy座標
					Enum_Direction d = UPLEFT ;	//前回の追跡点

					//境界線を追加
					borderlist.Add( ) ;

					//今の境界線を枠から切れている境界線としてフラグを設定
					borderlist.border[ borderlist.num - 1 ].island = true ;

					//追跡点が初期位置に戻ってくるまで続行
					for( ; ; )
					{
						//下のループを回った結果孤立点なら終了
						if( d == DEFAULT ) 
						{
							break ;
						}

						//ラベリングを進める
						index[ ty ][ tx ] = borderlist.border[ borderlist.num - 1 ].length ;

						//今の座標を登録
						borderlist.border[ borderlist.num - 1 ].Add( tx , ty ) ;

						//lengthが1よりも大きくて初期座標と追跡座標が同じときも終了
						if( 1 < borderlist.border[ borderlist.num - 1 ].length && sx == tx && sy == ty )
						{
							break ;
						}

						

						//ラベルもつける
						label[ ty ][ tx ] = count ;

						//9回目は孤立判定をするループ
						for( int i = 0 ; i < 9 ; i++ )
						{
							//上の検索
							if( ( i + ( int )d + 1 ) % 8 == 0 )
							{
								//printf( "UP serch\n" ) ;
								//上にアクセスできる
								if( 0 < ty - 1 )
								{
									//上が対象画素
									if( original.data[ ty - 1 ][ tx ] == BLACK && is_border[ ty - 1 ][ tx ] )
									{
										//printf( "  UP serched\n" ) ;
										//前回の追跡点を下に設定
										d = DOWN ;
										//追跡点を更新
										ty -= 1 ;

										//ループ終了
										break ;
									}
								}
							}
							//左上の検索
							else if( ( i + ( int )d + 1 ) % 8 == 1 )
							{
								//printf( "UPLEFT serch\n" ) ;
								//左上にアクセスできる
								if( 0 < tx - 1 && 0 < ty - 1 )
								{
									//左上が対象画素
									if( original.data[ ty - 1 ][ tx - 1 ] == BLACK && is_border[ ty - 1 ][ tx - 1 ] )
									{
										//printf( "  UPLEFT serched\n" ) ;
										//前回の追跡点を下に設定
										d = DOWNRIGHT ;
										//追跡点を更新
										ty -= 1 ;
										tx -= 1 ;
				
										//ループ終了
										break ;
									}
								}
							}
							//左の検索
							else if( ( i + ( int )d + 1 ) % 8 == 2 )
							{
								//printf( "LFET serch\n" ) ;
								//左にアクセスできる
								if( 0 < tx - 1 )
								{
									//左が対象画素
									if( original.data[ ty ][ tx - 1 ] == BLACK && is_border[ ty ][ tx - 1 ] )
									{
										//printf( "  LEFT serched\n" ) ;
										//前回の追跡点を下に設定
										d = RIGHT ;
										//追跡点を更新
										tx -= 1 ;

										//ループ終了
										break ;
									}
								}
							}
							//左下の検索
							else if( ( i + ( int )d + 1 ) % 8 == 3 )
							{
								//printf( "DOWNLEFT serch\n" ) ;
								//左下にアクセスできる
								if( 0 < tx - 1 && ty + 1 < original.height )
								{
									//左下が対象画素
									if( original.data[ ty + 1 ][ tx - 1 ] == BLACK && is_border[ ty + 1 ][ tx - 1 ] )
									{
										//printf( "DOWNLEFT serched\n" ) ;
										//前回の追跡点を下に設定
										d = UPRIGHT ;
										//追跡点を更新
										tx -= 1 ;
										ty += 1 ;
	
										//ループ終了
										break ;
									}
								}
							}
							//下の検索
							else if( ( i + ( int )d + 1 ) % 8 == 4 )
							{
								//printf( "DOWN serch\n" ) ;
								//下にアクセスできる
								if( ty + 1 < original.height )
								{
									//下が対象画素
									if( original.data[ ty + 1 ][ tx ] == BLACK && is_border[ ty + 1 ][ tx ] )
									{
										//printf( "  DOWN serched\n" ) ;
										//前回の追跡点を下に設定
										d = UP ;
										//追跡点を更新
										ty += 1 ;
											
										//ループ終了
										break ;
									}
								}
							}
							//右下の検索
							else if( ( i + ( int )d + 1 ) % 8 == 5 )
							{
								//printf( "DOWNRIGHT serch\n" ) ;
								//右下にアクセスできる
								if( tx + 1 < original.width && ty + 1 < original.height )
								{
									//右下が対象画素
									if( original.data[ ty + 1 ][ tx + 1 ] == BLACK && is_border[ ty + 1 ][ tx + 1 ] )
									{
										//printf( "  DOWNRIGHT serched\n" ) ;
										//前回の追跡点を下に設定
										d = UPLEFT ;
										//追跡点を更新
										ty += 1 ;
										tx += 1 ;

										//ループ終了
										break ;
									}
								}
							}
							//右の検索
							else if( ( i + ( int )d + 1 ) % 8 == 6 )
							{
								//printf( "RIGHT serch\n" ) ;
								//右にアクセスできる
								if( tx + 1 < original.width )
								{
									//右が対象画素
									if( original.data[ ty ][ tx + 1 ] == BLACK && is_border[ ty ][ tx + 1 ] )
									{
										//printf( "  RIGHT serched\n" ) ;
										//前回の追跡点を下に設定
										d = LEFT ;
										//追跡点を更新
										tx += 1 ;
	
										//ループ終了
										break ;
									}
								}
							}
							//右上の検索
							else if( ( i + ( int )d + 1 ) % 8 == 7 )
							{
								//printf( "UPRIGHT serch\n" ) ;
								//右上にアクセスできる
								if( tx + 1 < original.width && 0 < ty - 1 )
								{
									//右上が対象画素
									if( original.data[ ty - 1 ][ tx + 1 ] == BLACK && is_border[ ty - 1 ][ tx + 1 ] )
									{
										//printf( "  UPRIGHT serched\n" ) ;
										//前回の追跡点を下に設定
										d = DOWNLEFT ;
										//追跡点を更新
										ty -= 1 ;
										tx += 1 ;

										//ループ終了
										break ;
									}
								}
							}
							//孤立点
							else
							{
								d = DEFAULT ;
								break ;
							}
						}
					}

					//自分自身はラベルが-1だが近傍にラベルが付いたものがあるならそれを伝播
					bool propagate = true ;	//伝播フラグ
					for( ; propagate ; )
					{
						//一旦伝播していない状態として記録
						propagate = false ;
						
						for( int k = 0 ; k < original.height ; k++ )
						{
							for( int l = 0 ; l < original.width ; l++ )
							{
								//境界線でまだラベリングが済んでいない
								if( is_border[ k ][ l ] && label[ k ][ l ] == -1 )
								{
									//左上にアクセスできる
									if( 0 <= k - 1 && 0 <= l - 1 )
									{
										//左上が-1以外のラベル
										if( label[ k - 1 ][ l - 1 ] != -1 )
										{
											label[ k ][ l ] = label[ k - 1 ][ l - 1 ] ;
											propagate = true ;
										}
									}
									//左にアクセスできる
									if( 0 <= l - 1 )
									{
										//左が-1以外のラベル
										if( label[ k ][ l - 1 ] != -1 )
										{
											label[ k ][ l ] = label[ k ][ l - 1 ] ;
											propagate = true ;
										}
									}
									//左下にアクセスできる
									if( k + 1 < original.height && 0 <= l - 1 )
									{
										//左下が-1以外のラベル
										if( label[ k + 1 ][ l - 1 ] != -1 )
										{
											label[ k ][ l ] = label[ k + 1 ][ l - 1 ] ;
											propagate = true ;
										}
									}
									//下にアクセスできる
									if( k + 1 < original.height )
									{
										//下が-1以外のラベル
										if( label[ k + 1 ][ l ] != -1 )
										{
											label[ k ][ l ] = label[ k + 1 ][ l ] ;
											propagate = true ;
										}
									}
									//右下にアクセスできる
									if( k + 1 < original.height && l + 1 < original.width )
									{
										//右下が-1以外のラベル
										if( label[ k + 1 ][ l + 1 ] != -1 )
										{
											label[ k ][ l ] = label[ k + 1 ][ l + 1 ] ;
											propagate = true ;
										}
									}
									//右にアクセスできる
									if( l + 1 < original.width )
									{
										//右が-1以外のラベル
										if( label[ k ][ l + 1 ] != -1 )
										{
											label[ k ][ l ] = label[ k ][ l + 1 ] ;
											propagate = true ;
										}
									}
									//右上にアクセスできる
									if( 0 <= k - 1 && l + 1 < original.width )
									{
										//右上が-1以外のラベル
										if( label[ k - 1 ][ l + 1 ] != -1 )
										{
											label[ k ][ l ] = label[ k - 1 ][ l + 1 ] ;
											propagate = true ;
										}
									}
									//上にアクセスできる
									if( 0 <= k - 1 )
									{
										//上が-1以外のラベル
										if( label[ k - 1 ][ l ] != -1 )
										{
											label[ k ][ l ] = label[ k - 1 ][ l ] ;
											propagate = true ;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	/*for( int i = 0 ; i < original.height ; i++ )
	{
		for( int j = 0 ; j < original.width ; j++ )
		{
			printf( "%5d " , index[ i ][ j ] ) ;
		}
		printf( "\n" ) ;
	}*/
}

void Curvature::Calc( )
{
	//境界線を直線近時
	for( int i = 0 ; i < borderlist.num ; i++ )
	//for( int i = 0 ; i < 1 ; i++ )
	{
		//printf( "length %d\n" , borderlist.border[ i ].length ) ;

		//島の周りにできている境界線なら半分の位置で予め分割
		if( borderlist.border[ i ].island )
		{
			borderlist.border[ i ].point[ 0 ].devide = true ;
			borderlist.border[ i ].point[ ( int )( ( borderlist.border[ i ].length - 1 ) / 2.f ) ].devide = true ;
			Devide( i , 0 , ( int )( ( borderlist.border[ i ].length - 1 ) / 2.f ) , 0 ) ;
			Devide( i , ( int )( ( borderlist.border[ i ].length - 1 ) / 2.f ) , borderlist.border[ i ].length - 1 , 0 ) ;
		}
		else
		{
			borderlist.border[ i ].point[ 0 ].devide = true ;
			borderlist.border[ i ].point[ borderlist.border[ i ].length - 1 ].devide = true ;
			Devide( i , 0 , borderlist.border[ i ].length - 1 , 0 ) ;
		}
	}

	//曲率計算
	int sum_devide = 0 ;
	int sum_length = 0 ;
	for( int i = 0 ; i < borderlist.num ; i++ )
	{
		//分割点を数える
		for( int j = 0 ; j < borderlist.border[ i ].length ; j++ )
		{
			if( borderlist.border[ i ].point[ j ].devide )
			{
				sum_devide++ ;
			}
		}

		//境界線の長さを合計 初期位置と最終位置が同じだから1引く
		sum_length += borderlist.border[ i ].length - 1 ;
	}

	feature[ 0 ] = ( float )sum_devide / ( float )sum_length ;
}

void Curvature::Devide( int num , int start , int end , int cnt )
{
	//3未満しか点が取れない場合は分割できない
	if( end - start < 3 )
	{
		//printf( "few\n" ) ;
		return ;
	}

	float max_distance = 0.f ;	//最長の最短距離
	int max_index = ( int )( ( end - start ) / 2.f ) ;	//最短距離を与えるインデックス

	//startからendを通る直線における最大となる距離を持つ座標を求める
	for( int i = start ; i < end ; i++ )
	{
		float distance = Distance( borderlist.border[ num ].point[ end ] , borderlist.border[ num ].point[ start ] , borderlist.border[ num ].point[ i ] ) ;
		//printf( "(%d %d) " , borderlist.border[ num ].point[ start ].x , borderlist.border[ num ].point[ start ].y ) ;
		//printf( "(%d %d) " , borderlist.border[ num ].point[ end ].x , borderlist.border[ num ].point[ end ].y ) ;
		//printf( "(%d %d) " , borderlist.border[ num ].point[ i ].x , borderlist.border[ num ].point[ i ].y ) ;
		//printf( "%f\n" , distance ) ;

		if( max_distance < distance )
		{
			max_distance = distance ;
			max_index = i ;
		}
	}

	//printf( "num %d\n" , num ) ;
	//printf( "start %d\n" , start ) ;
	//printf( "end %d\n" , end ) ;
	//printf( "max_index %d\n" , max_index ) ;
	//printf( "max_distance %f\n" , max_distance ) ;
	//printf( "\n" ) ;

	//閾値以下の距離なら分割しない
	if( max_distance < threshold )
	{
		//printf( "short\n" ) ;
		return ;
	}

	//分割点として設定
	borderlist.border[ num ].point[ max_index ].devide = true ;

	//さらに分割
	Devide( num , start , max_index , cnt + 1 ) ;
	Devide( num , max_index , end , cnt + 1 ) ;
}

float Curvature::Distance( Point p1 , Point p2 , Point p3 )
{
	//y軸に平行(傾き無限大)
	if( p2.x - p1.x == 0 )
	{
		float d = p3.x - p1.x ;
		return d < 0 ? -d : d ;
	}
	else
	{
		float m = ( float )( p2.y - p1.y ) / ( float )( p2.x - p1.x ) ;
		float n = ( float )-1.f * ( float )( p2.y - p1.y ) / ( float )( p2.x - p1.x ) * p1.x + p1.y ;
		float d = ( p3.y - m * p3.x - n ) / sqrt( 1 + pow( m , 2.f ) ) ;
		return d < 0 ? -d : d ;
	}
}

float Curvature::Radian( Point first , Point second , Point third )
{
	float sf_x = first.x - second.x ;
	float sf_y = first.y - second.y ;
	float st_x = third.x - second.x ;
	float st_y = third.y - second.y ;

	return acos( ( sf_x * st_x + sf_y * st_y ) / ( sqrt( pow( sf_x , 2.f ) + pow( sf_y , 2.f ) ) * sqrt( pow( st_x , 2.f ) + pow( st_y , 2.f ) ) ) ) ;
}

void Curvature::Print( )
{
	Ppm ppm( original.height , original.width ) ;
	ppm.filename = "curvature.ppm" ;

	for( int i = 0 ; i < original.height ; i++ )
	{
		for( int j =0 ; j < original.width ; j++ )
		{
			int count = 0 ;
			bool flg = false ;

			for( int k = 0 ; k < borderlist.num ; k++ )
			{
				for( int l = 0 ; l < borderlist.border[ k ].length ; l++ )
				{
					//今注目している画素が分割点
					if( borderlist.border[ k ].point[ l ].devide &&
						borderlist.border[ k ].point[ l ].x == j &&
						borderlist.border[ k ].point[ l ].y == i )
					{
						flg = true ;
					}
				}
			}

			if( flg )
			{
				//printf( "  *   " ) ;
				ppm.data[ i ][ j ][ 0 ] = 255 ;
				ppm.data[ i ][ j ][ 1 ] = 255 ;
				ppm.data[ i ][ j ][ 2 ] = 0 ;
			}
			else if( is_border[ i ][ j ] )
			{
				//printf( "%5d " , index[ i ][ j ] ) ;
				ppm.data[ i ][ j ][ 0 ] = 128 ;
				ppm.data[ i ][ j ][ 1 ] = 128 ;
				ppm.data[ i ][ j ][ 2 ] = 128 ;
			}
			else if( original.data[ i ][ j ] == BLACK )
			{
				//printf( "%5d " , index[ i ][ j ] ) ;
				ppm.data[ i ][ j ][ 0 ] = 0 ;
				ppm.data[ i ][ j ][ 1 ] = 0 ;
				ppm.data[ i ][ j ][ 2 ] = 0 ;
			}
			else
			{
				//printf( "%5d " , index[ i ][ j ] ) ;
				ppm.data[ i ][ j ][ 0 ] = 0 ;
				ppm.data[ i ][ j ][ 1 ] = 0 ;
				ppm.data[ i ][ j ][ 2 ] = 255 ;
			}
		}

		//printf( "\n" ) ;
	}

	ppm.Save( ) ;
}