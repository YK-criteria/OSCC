#include "curvature.h"

Curvature::Curvature( ) : dim( 1 ) , increase( 10 ) , threshold( 3.f )
{
	dimension = dim ;
	feature = new float[ dimension ]( ) ;
}

Curvature::Curvature( Pbm image ) : dim( 1 ) , increase( 10 ) , threshold( 3.f )
{
	dimension = dim ;
	feature = new float[ dimension ]( ) ;
	
	original = image ;
	original.filename = "original.pbm" ;
}

Curvature::Curvature( const Curvature &curvature ) : dim( 3 ) , increase( 10 ) , threshold( 3.f )
{
	dimension = curvature.dimension ;
	feature = new float[ dimension ]( ) ;
	for( int i = 0 ; i < dimension ; i++ )
	{
		feature[ i ] = curvature.feature[ i ] ;
	}

	original = curvature.original ;
}

Curvature::~Curvature( )
{
	
}

Curvature& Curvature::operator=( const Curvature& curvature )
{
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
	SetIsBorder( ) ;

	Labeling( ) ;

	Calc( ) ;

	Print( ) ;
}

void Curvature::SetIsBorder( )
{
	is_border = new bool*[ original.height ]( ) ;
	for( int i = 0 ; i < original.height ; i++ )
	{
		is_border[ i ] = new bool[ original.width ]( ) ;
		for( int j = 0 ; j < original.width ; j++ )
		{
			is_border[ i ][ j ] = false ;
		}
	}

	for( int i = 0 ; i < original.height ; i++ )
	{
		for( int j = 0 ; j < original.width ; j++ )
		{
			if( original.data[ i ][ j ] == BLACK )
			{
				if( 0 <= i - 1 )
				{
					if( original.data[ i - 1 ][ j ] == WHITE )
					{
						is_border[ i ][ j ] = true ;
					}
				}
				if( j + 1 < original.width )
				{
					if( original.data[ i ][ j + 1 ] == WHITE )
					{
						is_border[ i ][ j ] = true ;
					}
				}
				if( i + 1 < original.height )
				{
					if( original.data[ i + 1 ][ j ] == WHITE )
					{
						is_border[ i ][ j ] = true ;
					}
				}
				if( 0 <= j - 1 )
				{
					if( original.data[ i ][ j - 1 ] == WHITE )
					{
						is_border[ i ][ j ] = true ;
					}
				}
			}
		}
	}

}

void Curvature::Labeling( )
{
	index = new int*[ original.height ]( ) ;
	for( int i = 0 ; i < original.height ; i++ )
	{
		index[ i ] = new int[ original.width ]( ) ;
		for( int j = 0 ; j < original.width ; j++ )
		{
			index[ i ][ j ] = original.height * original.width ;
		}
	}
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

	for( int i = 0 ; i < original.height ; i++ )
	{
		for( int j = 0 ; j < original.width ; j++ )
		{
			if( ( i == 0 || i == original.height - 1 || j == 0 || j == original.width - 1 ) && is_border[ i ][ j ] && index[ i ][ j ] == original.height * original.width )
			{
				index[ i ][ j ] = 0 ;
				label[ i ][ j ] = count ;

				bool changed = true ;
				for( ; changed ; )
				{
					changed = false ;

					for( int i = 0 ; i < original.height ; i++ )
					{
						for( int j = 0 ; j < original.width ; j++ )
						{
							if( is_border[ i ][ j ] )
							{
								int min_index = index[ i ][ j ] ;

								if( 0 <= i - 1 && 0 <= j - 1 )
								{
									if( is_border[ i - 1 ][ j - 1 ] )
									{
										if( index[ i - 1 ][ j - 1 ] < min_index )
										{
											min_index = index[ i - 1 ][ j - 1 ] ;
										}
									}
								}
								if( 0 <= j - 1 )
								{
									if( is_border[ i ][ j - 1 ] )
									{
										if( index[ i ][ j - 1 ] < min_index )
										{
											min_index = index[ i ][ j - 1 ] ;
										}
									}
								}
								if( i + 1 < original.height && 0 <= j - 1 )
								{
									if( is_border[ i + 1 ][ j - 1 ] )
									{
										if( index[ i + 1 ][ j - 1 ] < min_index )
										{
											min_index = index[ i + 1 ][ j - 1 ] ;
										}
									}
								}
								if( i + 1 < original.height )
								{
									if( is_border[ i + 1 ][ j ] )
									{
										if( index[ i + 1 ][ j ] < min_index )
										{
											min_index = index[ i + 1 ][ j ] ;
										}
									}
								}
								if( i + 1 < original.height && j + 1 < original.width )
								{
									if( is_border[ i + 1 ][ j + 1 ] )
									{
										if( index[ i + 1 ][ j + 1 ] < min_index )
										{
											min_index = index[ i + 1 ][ j + 1 ] ;
										}
									}
								}
								if( j + 1 < original.width )
								{
									if( is_border[ i ][ j + 1 ] )
									{
										if( index[ i ][ j + 1 ] < min_index )
										{
											min_index = index[ i ][ j + 1 ] ;
										}
									}
								}
								if( 0 <= i - 1 && j + 1 < original.width )
								{
									if( is_border[ i - 1 ][ j + 1 ] )
									{
										if( index[ i - 1 ][ j + 1 ] < min_index )
										{
											min_index = index[ i - 1 ][ j + 1 ] ;
										}
									}
								}
								if( 0 <= i - 1 )
								{
									if( is_border[ i - 1 ][ j ] )
									{
										if( index[ i - 1 ][ j ] < min_index )
										{
											min_index = index[ i - 1 ][ j ] ;
										}
									}
								}

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
				

				borderlist.Add( ) ;

				borderlist.border[ borderlist.num - 1 ].island = false ;

				int current_label = index[ max_i ][ max_j ] ;
				int current_i = max_i ;
				int current_j = max_j ;
				for( ; ; )
				{
					borderlist.border[ borderlist.num - 1 ].Add( current_j , current_i ) ;

					if( current_label == 0 )
					{
						break ;
					}

					int min_index = index[ current_i ][ current_j ] ;
					int min_i = current_i ;
					int min_j = current_j ;
					if( 0 <= current_i - 1 && 0 <= current_j - 1 )
					{
						if( is_border[ current_i - 1 ][ current_j - 1 ] )
						{
							if( index[ current_i - 1 ][ current_j - 1 ] < min_index )
							{
								min_index = index[ current_i - 1 ][ current_j - 1 ] ;
								min_i = current_i - 1 ;
								min_j = current_j - 1 ;
							}
						}
					}
					if( 0 <= current_j - 1 )
					{
						if( is_border[ current_i ][ current_j - 1 ] )
						{
							if( index[ current_i ][ current_j - 1 ] < min_index )
							{
								min_index = index[ current_i ][ current_j - 1 ] ;
								min_i = current_i ;
								min_j = current_j - 1 ;
							}
						}
					}
					if( current_i + 1 < original.height && 0 <= current_j - 1 )
					{
						if( is_border[ current_i + 1 ][ current_j - 1 ] )
						{
							if( index[ current_i + 1 ][ current_j - 1 ] < min_index )
							{
								min_index = index[ current_i + 1 ][ current_j - 1 ] ;
								min_i = current_i + 1 ;
								min_j = current_j - 1 ;
							}
						}
					}
					if( current_i + 1 < original.height )
					{
						if( is_border[ current_i + 1 ][ current_j ] )
						{
							if( index[ current_i + 1 ][ current_j ] < min_index )
							{
								min_index = index[ current_i + 1 ][ current_j ] ;
								min_i = current_i + 1 ;
								min_j = current_j ;
							}
						}
					}
					if( current_i + 1 < original.height && current_j + 1 < original.width )
					{
						if( is_border[ current_i + 1 ][ current_j + 1 ] )
						{
							if( index[ current_i + 1 ][ current_j + 1 ] < min_index )
							{
								min_index = index[ current_i + 1 ][ current_j + 1 ] ;
								min_i = current_i + 1 ;
								min_j = current_j + 1 ;
							}
						}
					}
					if( current_j + 1 < original.width )
					{
						if( is_border[ current_i ][ current_j + 1 ] )
						{
							if( index[ current_i ][ current_j + 1 ] < min_index )
							{
								min_index = index[ current_i ][ current_j + 1 ] ;
								min_i = current_i ;
								min_j = current_j + 1 ;
							}
						}
					}
					if( 0 <= current_i - 1 && current_j + 1 < original.width )
					{
						if( is_border[ current_i - 1 ][ current_j + 1 ] )
						{
							if( index[ current_i - 1 ][ current_j + 1 ] < min_index )
							{
								min_index = index[ current_i - 1 ][ current_j + 1 ] ;
								min_i = current_i - 1 ;
								min_j = current_j + 1 ;
							}
						}
					}
					if( 0 <= current_i - 1 )
					{
						if( is_border[ current_i - 1 ][ current_j ] )
						{
							if( index[ current_i - 1 ][ current_j ] < min_index )
							{
								min_index = index[ current_i - 1 ][ current_j ] ;
								min_i = current_i - 1 ;
								min_j = current_j ;
							}
						}
					}

					current_label = min_index ;
					current_i = min_i ;
					current_j = min_j ;
				}

				count++ ;
			}
		}
	}

	for( int i = 1 ; i < original.height - 1 ; i++ )
	{
		for( int j = 1 ; j < original.width - 1 ; j++ )
		{
			if( is_border[ i ][ j ] )
			{
				if( label[ i ][ j ] == -1 )
				{
					int sx = j ;
					int sy = i ;
					int tx = j ;
					int ty = i ;
					Enum_Direction d = UPLEFT ;

					borderlist.Add( ) ;

					borderlist.border[ borderlist.num - 1 ].island = true ;

					for( ; ; )
					{
						if( d == DEFAULT ) 
						{
							break ;
						}

						index[ ty ][ tx ] = borderlist.border[ borderlist.num - 1 ].length ;

						borderlist.border[ borderlist.num - 1 ].Add( tx , ty ) ;

						if( 1 < borderlist.border[ borderlist.num - 1 ].length && sx == tx && sy == ty )
						{
							break ;
						}

						

						label[ ty ][ tx ] = count ;

						for( int i = 0 ; i < 9 ; i++ )
						{
							if( ( i + ( int )d + 1 ) % 8 == 0 )
							{
								if( 0 < ty - 1 )
								{
									if( original.data[ ty - 1 ][ tx ] == BLACK && is_border[ ty - 1 ][ tx ] )
									{
										d = DOWN ;
										ty -= 1 ;

										break ;
									}
								}
							}
							else if( ( i + ( int )d + 1 ) % 8 == 1 )
							{
								if( 0 < tx - 1 && 0 < ty - 1 )
								{
									if( original.data[ ty - 1 ][ tx - 1 ] == BLACK && is_border[ ty - 1 ][ tx - 1 ] )
									{
										d = DOWNRIGHT ;
										ty -= 1 ;
										tx -= 1 ;
				
										break ;
									}
								}
							}
							else if( ( i + ( int )d + 1 ) % 8 == 2 )
							{
								if( 0 < tx - 1 )
								{
									if( original.data[ ty ][ tx - 1 ] == BLACK && is_border[ ty ][ tx - 1 ] )
									{
										d = RIGHT ;
										tx -= 1 ;

										break ;
									}
								}
							}
							else if( ( i + ( int )d + 1 ) % 8 == 3 )
							{
								if( 0 < tx - 1 && ty + 1 < original.height )
								{
									if( original.data[ ty + 1 ][ tx - 1 ] == BLACK && is_border[ ty + 1 ][ tx - 1 ] )
									{
										d = UPRIGHT ;
										tx -= 1 ;
										ty += 1 ;
	
										break ;
									}
								}
							}
							else if( ( i + ( int )d + 1 ) % 8 == 4 )
							{
								if( ty + 1 < original.height )
								{
									if( original.data[ ty + 1 ][ tx ] == BLACK && is_border[ ty + 1 ][ tx ] )
									{
										d = UP ;
										ty += 1 ;
											
										break ;
									}
								}
							}
							else if( ( i + ( int )d + 1 ) % 8 == 5 )
							{
								if( tx + 1 < original.width && ty + 1 < original.height )
								{
									if( original.data[ ty + 1 ][ tx + 1 ] == BLACK && is_border[ ty + 1 ][ tx + 1 ] )
									{
										d = UPLEFT ;
										ty += 1 ;
										tx += 1 ;

										break ;
									}
								}
							}
							else if( ( i + ( int )d + 1 ) % 8 == 6 )
							{
								if( tx + 1 < original.width )
								{
									if( original.data[ ty ][ tx + 1 ] == BLACK && is_border[ ty ][ tx + 1 ] )
									{
										d = LEFT ;
										tx += 1 ;
	
										break ;
									}
								}
							}
							else if( ( i + ( int )d + 1 ) % 8 == 7 )
							{
								if( tx + 1 < original.width && 0 < ty - 1 )
								{
									if( original.data[ ty - 1 ][ tx + 1 ] == BLACK && is_border[ ty - 1 ][ tx + 1 ] )
									{
										d = DOWNLEFT ;
										ty -= 1 ;
										tx += 1 ;

										break ;
									}
								}
							}
							else
							{
								d = DEFAULT ;
								break ;
							}
						}
					}

					bool propagate = true ;
					for( ; propagate ; )
					{
						propagate = false ;
						
						for( int k = 0 ; k < original.height ; k++ )
						{
							for( int l = 0 ; l < original.width ; l++ )
							{
								if( is_border[ k ][ l ] && label[ k ][ l ] == -1 )
								{
									if( 0 <= k - 1 && 0 <= l - 1 )
									{
										if( label[ k - 1 ][ l - 1 ] != -1 )
										{
											label[ k ][ l ] = label[ k - 1 ][ l - 1 ] ;
											propagate = true ;
										}
									}
									if( 0 <= l - 1 )
									{
										if( label[ k ][ l - 1 ] != -1 )
										{
											label[ k ][ l ] = label[ k ][ l - 1 ] ;
											propagate = true ;
										}
									}
									if( k + 1 < original.height && 0 <= l - 1 )
									{
										if( label[ k + 1 ][ l - 1 ] != -1 )
										{
											label[ k ][ l ] = label[ k + 1 ][ l - 1 ] ;
											propagate = true ;
										}
									}
									if( k + 1 < original.height )
									{
										if( label[ k + 1 ][ l ] != -1 )
										{
											label[ k ][ l ] = label[ k + 1 ][ l ] ;
											propagate = true ;
										}
									}
									if( k + 1 < original.height && l + 1 < original.width )
									{
										if( label[ k + 1 ][ l + 1 ] != -1 )
										{
											label[ k ][ l ] = label[ k + 1 ][ l + 1 ] ;
											propagate = true ;
										}
									}
									if( l + 1 < original.width )
									{
										if( label[ k ][ l + 1 ] != -1 )
										{
											label[ k ][ l ] = label[ k ][ l + 1 ] ;
											propagate = true ;
										}
									}
									if( 0 <= k - 1 && l + 1 < original.width )
									{
										if( label[ k - 1 ][ l + 1 ] != -1 )
										{
											label[ k ][ l ] = label[ k - 1 ][ l + 1 ] ;
											propagate = true ;
										}
									}
									if( 0 <= k - 1 )
									{
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

}

void Curvature::Calc( )
{
	for( int i = 0 ; i < borderlist.num ; i++ )
	{
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

	int sum_devide = 0 ;
	int sum_length = 0 ;
	for( int i = 0 ; i < borderlist.num ; i++ )
	{
		for( int j = 0 ; j < borderlist.border[ i ].length ; j++ )
		{
			if( borderlist.border[ i ].point[ j ].devide )
			{
				sum_devide++ ;
			}
		}

		sum_length += borderlist.border[ i ].length - 1 ;
	}

	feature[ 0 ] = ( float )sum_devide / ( float )sum_length ;
}

void Curvature::Devide( int num , int start , int end , int cnt )
{
	if( end - start < 3 )
	{
		return ;
	}

	float max_distance = 0.f ;
	int max_index = ( int )( ( end - start ) / 2.f ) ;

	for( int i = start ; i < end ; i++ )
	{
		float distance = Distance( borderlist.border[ num ].point[ end ] , borderlist.border[ num ].point[ start ] , borderlist.border[ num ].point[ i ] ) ;

		if( max_distance < distance )
		{
			max_distance = distance ;
			max_index = i ;
		}
	}

	if( max_distance < threshold )
	{
		return ;
	}

	borderlist.border[ num ].point[ max_index ].devide = true ;

	Devide( num , start , max_index , cnt + 1 ) ;
	Devide( num , max_index , end , cnt + 1 ) ;
}

float Curvature::Distance( Point p1 , Point p2 , Point p3 )
{
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