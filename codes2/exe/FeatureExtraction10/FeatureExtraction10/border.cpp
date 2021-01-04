#include "border.h"

Border::Border( ) : increase( 100 )
{
	length = 0 ;
	max_length = increase ;
	point = new Point[ max_length ]( ) ;
}

Border::Border( int length ) : increase( 100 )
{
	this->length = length ;
	max_length = length ;
	point = new Point[ length ]( ) ;
}

Border::Border( const Border &border ) : increase( 100 )
{
	length = border.length ;
	max_length = border.max_length ;

	delete[ ] point ;
	point = new Point[ max_length ]( ) ;
	for( int i = 0 ; i < length ; i++ )
	{
		point[ i ] = border.point[ i ] ;
	}
}

Border::~Border( )
{
	
}

Border& Border::operator=( const Border& border )
{
	length = border.length ;
	max_length = border.max_length ;

	delete[ ] point ;
	point = new Point[ max_length ]( ) ;
	for( int i = 0 ; i < length ; i++ )
	{
		point[ i ] = border.point[ i ] ;
	}

	return *this ;
}

void Border::Add( int x , int y )
{
	if( length == max_length )
	{
		int new_max_length = max_length + increase ;
		Point *new_point = new Point[ new_max_length ]( ) ;
		
		for( int i = 0 ; i < length ; i++ )
		{
			new_point[ i ] = point[ i ] ;
		}
		delete[ ] point ;

		point = new_point ;
		max_length = new_max_length ;
	}

	point[ length ] = Point( x , y ) ;

	length++ ;
}

bool Border::Exist( int x , int y )
{
	for( int i = 0 ; i < length ; i++ )
	{
		if( point[ i ].x == x && point[ i ].y == y )
		{
			return true ;
		}
	}

	return false ;
}
