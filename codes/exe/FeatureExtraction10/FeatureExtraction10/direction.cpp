#include "direction.h"

Direction::Direction( ) : dim( 1 ) , increase( 10 ) , threshold( 5.f )
{
	//親クラスの変数設定
	dimension = dim ;
	feature = new float[ dimension ]( ) ;
}

Direction::Direction( Pbm image ) : dim( 1 ) , increase( 10 ) , threshold( 5.f )
{
	//親クラスの変数設定
	dimension = dim ;
	feature = new float[ dimension ]( ) ;
	
	//imageからファイル名だけ変更
	original = image ;
	original.filename = "original.pbm" ;
}

Direction::Direction( const Direction &direction ) : dim( 1 ) , increase( 10 ) , threshold( 5.f )
{
	//親クラスで書いたほうがいいかも？
	dimension = direction.dimension ;
	feature = new float[ dimension ]( ) ;
	for( int i = 0 ; i < dimension ; i++ )
	{
		feature[ i ] = direction.feature[ i ] ;
	}

	//イメージコピー
	original = direction.original ;
}

Direction::~Direction( )
{
	
}

Direction& Direction::operator=( const Direction& direction )
{
	//親クラスで書いたほうがいいかも？
	dimension = direction.dimension ;
	original = direction.original ;
	delete feature ;
	feature = new float[ dimension ] ;
	for( int i = 0 ; i < dimension ; i++ )
	{
		feature[ i ] = direction.feature[ i ] ;
	}

	return *this ;
}

void Direction::Extract( )
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
	//	向き計算
	//////////////////////////////////////////////////////////////
	Calc( ) ;

	//////////////////////////////////////////////////////////////
	//	出力
	//////////////////////////////////////////////////////////////
	Print( ) ;
}

void Direction::SetIsBorder( )
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

void Direction::Labeling( )
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
	

	//枠内にある境界線はラベリングしない

	/*for( int i = 0 ; i < original.height ; i++ )
	{
		for( int j = 0 ; j < original.width ; j++ )
		{
			printf( "%5d " , index[ i ][ j ] ) ;
		}
		printf( "\n" ) ;
	}*/
}

void Direction::Calc( )
{
	//境界線を直線近時
	for( int i = 0 ; i < borderlist.num ; i++ )
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

	for( int i = 0 ; i < borderlist.num ; i++ )
	{
		//一番最初の分割点を設定
		Point first = borderlist.border[ i ].point[ 0 ] ;
		Point second ;
		Point third ;

		float rad ;	//成す角
		Point point[ 256 ] ;	//谷点格納用

		//二番目の点を探索
		int j = 0 ;
		for( ; j < borderlist.border[ i ].length ; j++ )
		{
			//分割点を発見
			if( borderlist.border[ i ].point[ j ].devide )
			{
				second = borderlist.border[ i ].point[ j ] ;
				break ;
			}
		}

		//三番目の点を探索
		for( ; j < borderlist.border[ i ].length ; j++ )
		{
			//分割点を発見
			if( borderlist.border[ i ].point[ j ].devide )
			{
				third = borderlist.border[ i ].point[ j ] ;

				//角度を計算
				rad = Radian( first , second , third ) ;
				break ;
			}
		}

		//4番目からの分割点を探索
		for( ; j < borderlist.border[ i ].length ; j++ )
		{
			//分割点を発見
			if( borderlist.border[ i ].point[ j ].devide )
			{
				third = borderlist.border[ i ].point[ j ] ;

				//角度を計算
				rad = Radian( first , second , third ) ;
				
			}
		}
	}

	const int DIRECTION_NUM = 16 ;
	float *direction = new float[ DIRECTION_NUM ]( ) ;

	for( int i = 0 ; i < borderlist.num ; i++ )
	{
		Point start = borderlist.border[ i ].point[ 0 ] ;
		Point end ;

		for( int j = 0 ; j < borderlist.border[ i ].length ; j++ )
		{
			//分割点を発見
			if( borderlist.border[ i ].point[ j ].devide )
			{
				end = borderlist.border[ i ].point[ j ] ;

				//向きを計算
				float dx = end.x - start.x ;
				float dy = end.y - start.y ;
				
				int min_index = 0 ;
				int min_radian = 3.14 ;

				//候補に一番近いベクトルを検索
				for( int k = 0 ; k < DIRECTION_NUM ; k++ )
				{
					//候補ベクトル
					float vx = cos( 3.14 / DIRECTION_NUM * k ) ;
					float vy = sin( 3.14 / DIRECTION_NUM * k ) ;

					//成す角
					float radian = acos( ( dx * vx + dy* vy ) / ( sqrt( pow( dx , 2.f ) + pow( dy , 2.f ) ) * sqrt( pow( vx , 2.f ) + pow( vy , 2.f ) ) ) ) ;

					//今までの最小角より小さい
					if( radian < min_radian )
					{
						min_index = k ;
						min_radian = radian ;
					}
				}

				//分割点間の距離を格納
				direction[ min_index ] += sqrt( pow( end.x - start.x , 2.f ) + pow( end.y - start.y , 2.f ) ) ;
			}
		}
	}

	//総和
	float sum_direction = 0.f ;
	for( int i = 0 ; i < DIRECTION_NUM ; i++ )
	{
		sum_direction += direction[ i ] ;
	}
	//平均
	float average = sum_direction / DIRECTION_NUM ;
	//分散
	float variance = 0.f ;
	for( int i = 0 ; i < DIRECTION_NUM ; i++ )
	{
		variance += pow( direction[ i ] - average , 2.f ) ;
	}
	variance /= DIRECTION_NUM ;

	/*for( int i = 0 ; i < DIRECTION_NUM ; i++ )
	{
		printf( "direction[ %d ] %f\n" , i , direction[ i ] ) ;
	}*/
	//長さで正規化
	float sum_length = 0.f ;
	for( int i = 0 ; i < borderlist.num ; i++ )
	{
		sum_length += borderlist.border[ i ].length - 1 ;
	}
	//printf( "variance : %f\n" , variance / sum_length ) ;

	feature[ 0 ] = sum_length != 0 ? variance / sum_length : 0.f ;
}

void Direction::Devide( int num , int start , int end , int cnt )
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

float Direction::Distance( Point p1 , Point p2 , Point p3 )
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

float Direction::Radian( Point first , Point second , Point third )
{
	float sf_x = first.x - second.x ;
	float sf_y = first.y - second.y ;
	float st_x = third.x - second.x ;
	float st_y = third.y - second.y ;

	return acos( ( sf_x * st_x + sf_y * st_y ) / ( sqrt( pow( sf_x , 2.f ) + pow( sf_y , 2.f ) ) * sqrt( pow( st_x , 2.f ) + pow( st_y , 2.f ) ) ) ) ;
}

void Direction::Print( )
{
	Ppm ppm( original.height , original.width ) ;
	ppm.filename = "direction.ppm" ;

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