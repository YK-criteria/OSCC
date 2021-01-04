#include "borderlist.h"

BorderList::BorderList( ) : increase( 10 )
{
	num = 0 ;
	max_num = increase ;
	border = new Border[ max_num ]( ) ;
}

void BorderList::Add( )
{
	if( num == max_num )
	{
		int new_max_num = max_num + increase ;
		Border *new_border = new Border[ new_max_num ]( ) ;
		for( int i = 0 ; i < num ; i++ )
		{
			new_border[ i ] = border[ i ] ;
		}
		delete[ ] border ;

		border = new_border ;
		max_num = new_max_num ;
	}

	border[ num ] = Border( ) ;
	
	num++ ;
}

bool BorderList::Exist( int x , int y )
{
	for( int i = 0 ; i < num ; i++ )
	{
		if( border[ i ].Exist( x , y ) )
		{
			return true ;
		}
	}

	return false ;
}