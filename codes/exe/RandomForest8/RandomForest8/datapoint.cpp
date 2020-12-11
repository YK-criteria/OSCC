#include "datapoint.h"

DataPoint::DataPoint( )
{

}

DataPoint::DataPoint( string classname , vector< float > feature )
{
	//�N���X���R�s�[
	this->classname = classname ;
	
	//�fi �[�^�_�R�s�[
	for( unsigned int i = 0 ; i < feature.size( ) ; i++ )
	{
		this->feature.push_back( feature[ i ] ) ;
	}
}

DataPoint::DataPoint( const DataPoint &datapoint )
{
	//�N���X���R�s�[
	classname = datapoint.classname ;

	//�f�[�^�_�R�s�[
	for( unsigned int i = 0 ; i < datapoint.feature.size( ) ; i++ )
	{
		feature.push_back( datapoint.feature[ i ] ) ;
	}
}

DataPoint &DataPoint::operator=( const DataPoint &datapoint )
{
	//�N���X���R�s�[
	classname = datapoint.classname ;

	//�f�[�^�_�R�s�[
	for( unsigned int i = 0 ; i < datapoint.feature.size( ) ; i++ )
	{
		feature.push_back( datapoint.feature[ i ] ) ;
	}

	return *this ;
}

bool DataPoint::operator==( const DataPoint &datapoint )
{
	//�N���X�����قȂ�
	if( classname != datapoint.classname )
	{
		return false ;
	}

	//�����ʔz��̗v�f�����قȂ�
	if( feature.size( ) != datapoint.feature.size( ) )
	{
		return false ;
	}

	//�����ʔz��̗v�f���قȂ�
	for( unsigned int i = 0 ; i < feature.size( ) ; i++ )
	{
		if( feature[ i ] != datapoint.feature[ i ] )
		{
			return false ;
		}
	}

	return true ;
}