#include "area.h"

Area::Area( ) : dim( 1 ) , increase( 10 ) , threshold( 0.001 )
{
	dimension = dim ;
	feature = new float[ dimension ]( ) ;
	
	index = NULL ;
}

Area::Area( Pbm image ) : dim( 1 ) , increase( 10 ) , threshold( 0.001 )
{
	dimension = dim ;
	feature = new float[ dimension ]( ) ;
	
	original = image ;
	original.filename = "original.pbm" ;	

	index = new int*[ image.height ]( ) ;
	for( int i = 0 ; i < image.height ; i++ )
	{
		index[ i ] = new int[ image.width ]( ) ;

		for( int j = 0 ; j < image.width ; j++ )
		{
			original.data[ i ][ j ] = image.data[ i ][ j ] ;

			if( original.data[ i ][ j ] == BLACK )
			{
				index[ i ][ j ] = i * image.width + j ;
			}
			else
			{
				index[ i ][ j ] = image.height * image.width ;
			}
		}
	}
}

Area::Area( const Area &area ) : dim( 1 ) , increase( 10 ) , threshold( 0.001 )
{
	dimension = area.dimension ;
	feature = new float[ dimension ]( ) ;
	for( int i = 0 ; i < dimension ; i++ )
	{
		feature[ i ] = area.feature[ i ] ;
	}

	original = area.original ;

	if( !area.index )
	{
		index = NULL ;
	}
	else
	{
		index = new int*[ original.height ]( ) ;
		for( int i = 0 ; i < original.height ; i++ )
		{
			index[ i ] = new int[ original.width ]( ) ;
		}

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
	dimension = area.dimension ;
	delete feature ;
	feature = new float[ dimension ] ;
	for( int i = 0 ; i < dimension ; i++ )
	{
		feature[ i ] = area.feature[ i ] ;
	}

	if( !area.index )
	{
		original = area.original ;
		index = NULL ;
	}
	else
	{
		for( int i = 0 ; i < original.height ; i++ )
		{
			delete[ ] index[ i ] ;
		}
		delete[ ] index ;

		original = area.original ;

		index = new int*[ original.height ]( ) ;
		for( int i = 0 ; i < original.height ; i++ )
		{
			index[ i ] = new int[ original.width ]( ) ;
		}

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
	Labeling( ) ;
	Count( ) ;

	MakeList( ) ;

	int countsmall = CountSmall( ) ;

	feature[ 0 ] = countsmall ;
}

void Area::Labeling( )
{
	for( bool changed = true ; changed ; )
	{
		changed = false ;

		for( int i = 0 ; i < original.height ; i++ )
		{
			for( int j = 0 ; j < original.width ; j++ )
			{
				if( 0 <= i - 1 )
				{
					if( index[ i - 1 ][ j ] < index[ i ][ j ] && original.data[ i ][ j ] == BLACK )
					{
						index[ i ][ j ] = index[ i - 1 ][ j ] ;
						changed = true ;
					}
				}

				if( i + 1 < original.height )
				{
					if( index[ i + 1 ][ j ] < index[ i ][ j ] && original.data[ i ][ j ] == BLACK )
					{
						index[ i ][ j ] = index[ i + 1 ][ j ] ;
						changed = true ;
					}
				}

				if( 0 <= j - 1 )
				{
					if( index[ i ][ j - 1 ] < index[ i ][ j ] && original.data[ i ][ j ] == BLACK )
					{
						index[ i ][ j ] = index[ i ][ j - 1 ] ;
						changed = true ;
					}
				}

				if( j + 1 < original.width )
				{
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
	list_length = increase ;
	list_count = 0 ;
	label_list = new int[ increase ]( ) ;

	for( int i = 0 ; i < original.height ; i++ )
	{
		for( int j = 0 ; j < original.width ; j++ )
		{
			if( index[ i ][ j ] != original.height * original.width )
			{
				bool exist = false ;

				for( int k = 0 ; k < list_count ; k++ )
				{
					if( label_list[ k ] == index[ i ][ j ] )
					{
						exist = true ;
					}
				}

				if( !exist )
				{
					if( list_count == list_length )
					{
						int new_list_length = list_length + increase ;
						int *new_list = new int[ new_list_length ]( ) ;

						for( int k = 0 ; k < list_length ; k++ )
						{
							new_list[ k ] = label_list[ k ] ;
						}

						delete[ ] label_list ;

						label_list = new_list ;
						list_length = new_list_length ;
					}

					label_list[ list_count ] = index[ i ][ j ] ;
					list_count++ ;
				}
			}
		}
	}
}

void Area::MakeList( )
{
	area_list = new int[ list_count ]( ) ;

	for( int i = 0 ; i < original.height ; i++ )
	{
		for( int j = 0 ; j < original.width  ; j++ )
		{
			bool exist = false ;

			for( int k = 0 ; k < list_count ; k++ )
			{
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
	int count = 0 ;

	for( int i = 0 ; i < list_count ; i++ )
	{
		if( ( double )area_list[ i ] / ( double )( original.height * original.width ) < threshold )
		{
			count++ ;
		}
	}

	return count ;
}