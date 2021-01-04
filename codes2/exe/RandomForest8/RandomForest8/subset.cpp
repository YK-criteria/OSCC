#include "subset.h"

Subset::Subset( )
{

}

Subset::Subset( vector< DataPoint > datapoint )
{
	for( unsigned int i = 0 ; i < datapoint.size( ) ; i++ )
	{
		this->datapoint.push_back( datapoint[ i ] ) ;
	}
}

Subset::Subset( const Subset &subset )
{
	for( unsigned int i = 0 ; i < subset.datapoint.size( ) ; i++ )
	{
		this->datapoint.push_back( subset.datapoint[ i ] ) ;
	}
}

Subset &Subset::operator=( const Subset &subset )
{
	for( unsigned int i = 0 ; i < subset.datapoint.size( ) ; i++ )
	{
		this->datapoint.push_back( subset.datapoint[ i ] ) ;
	}

	return *this ;
}
