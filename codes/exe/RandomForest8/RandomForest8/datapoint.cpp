#include "datapoint.h"

DataPoint::DataPoint( )
{

}

DataPoint::DataPoint( string classname , vector< float > feature )
{
	//クラス名コピー
	this->classname = classname ;
	
	//デi ータ点コピー
	for( unsigned int i = 0 ; i < feature.size( ) ; i++ )
	{
		this->feature.push_back( feature[ i ] ) ;
	}
}

DataPoint::DataPoint( const DataPoint &datapoint )
{
	//クラス名コピー
	classname = datapoint.classname ;

	//データ点コピー
	for( unsigned int i = 0 ; i < datapoint.feature.size( ) ; i++ )
	{
		feature.push_back( datapoint.feature[ i ] ) ;
	}
}

DataPoint &DataPoint::operator=( const DataPoint &datapoint )
{
	//クラス名コピー
	classname = datapoint.classname ;

	//データ点コピー
	for( unsigned int i = 0 ; i < datapoint.feature.size( ) ; i++ )
	{
		feature.push_back( datapoint.feature[ i ] ) ;
	}

	return *this ;
}

bool DataPoint::operator==( const DataPoint &datapoint )
{
	//クラス名が異なる
	if( classname != datapoint.classname )
	{
		return false ;
	}

	//特徴量配列の要素数が異なる
	if( feature.size( ) != datapoint.feature.size( ) )
	{
		return false ;
	}

	//特徴量配列の要素が異なる
	for( unsigned int i = 0 ; i < feature.size( ) ; i++ )
	{
		if( feature[ i ] != datapoint.feature[ i ] )
		{
			return false ;
		}
	}

	return true ;
}