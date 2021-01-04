#include "datapoint.h"

DataPoint::DataPoint( )
{

}

DataPoint::DataPoint( string classname , vector< float > feature )
{
	this->classname = classname ;
	
	for( unsigned int i = 0 ; i < feature.size( ) ; i++ )
	{
		this->feature.push_back( feature[ i ] ) ;
	}
}

DataPoint::DataPoint( const DataPoint &datapoint )
{
	classname = datapoint.classname ;

	for( unsigned int i = 0 ; i < datapoint.feature.size( ) ; i++ )
	{
		feature.push_back( datapoint.feature[ i ] ) ;
	}
}

DataPoint &DataPoint::operator=( const DataPoint &datapoint )
{
	classname = datapoint.classname ;

	for( unsigned int i = 0 ; i < datapoint.feature.size( ) ; i++ )
	{
		feature.push_back( datapoint.feature[ i ] ) ;
	}

	return *this ;
}

bool DataPoint::operator==( const DataPoint &datapoint )
{
	if( classname != datapoint.classname )
	{
		return false ;
	}

	if( feature.size( ) != datapoint.feature.size( ) )
	{
		return false ;
	}

	for( unsigned int i = 0 ; i < feature.size( ) ; i++ )
	{
		if( feature[ i ] != datapoint.feature[ i ] )
		{
			return false ;
		}
	}

	return true ;
}