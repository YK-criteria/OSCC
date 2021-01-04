#include "split.h"

Split::Split( )
{

}

Split::Split( vector< float > normal , float threshold )
{
	this->threshold = threshold ;

	for( unsigned int i = 0 ; i < normal.size( ) ; i++ )
	{
		this->normal.push_back( normal[ i ] ) ;
	}
}

Split::Split( const Split &split )
{
	threshold = split.threshold ;

	normal = split.normal ;
}

Split &Split::operator=( const Split &split )
{
	threshold = split.threshold ;

	normal = split.normal ;

	return *this ;
}
