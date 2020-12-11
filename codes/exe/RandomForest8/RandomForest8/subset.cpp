#include "subset.h"

Subset::Subset( )
{

}

Subset::Subset( vector< DataPoint > datapoint )
{
	//引数のデータ点配列をコピー
	for( unsigned int i = 0 ; i < datapoint.size( ) ; i++ )
	{
		this->datapoint.push_back( datapoint[ i ] ) ;
	}
}

Subset::Subset( const Subset &subset )
{
	//引数のサブセット内のデータ点をコピー
	for( unsigned int i = 0 ; i < subset.datapoint.size( ) ; i++ )
	{
		this->datapoint.push_back( subset.datapoint[ i ] ) ;
	}
}

Subset &Subset::operator=( const Subset &subset )
{
	//引数のサブセット内のデータ点をコピー
	for( unsigned int i = 0 ; i < subset.datapoint.size( ) ; i++ )
	{
		this->datapoint.push_back( subset.datapoint[ i ] ) ;
	}

	return *this ;
}
