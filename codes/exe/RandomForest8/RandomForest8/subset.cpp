#include "subset.h"

Subset::Subset( )
{

}

Subset::Subset( vector< DataPoint > datapoint )
{
	//�����̃f�[�^�_�z����R�s�[
	for( unsigned int i = 0 ; i < datapoint.size( ) ; i++ )
	{
		this->datapoint.push_back( datapoint[ i ] ) ;
	}
}

Subset::Subset( const Subset &subset )
{
	//�����̃T�u�Z�b�g���̃f�[�^�_���R�s�[
	for( unsigned int i = 0 ; i < subset.datapoint.size( ) ; i++ )
	{
		this->datapoint.push_back( subset.datapoint[ i ] ) ;
	}
}

Subset &Subset::operator=( const Subset &subset )
{
	//�����̃T�u�Z�b�g���̃f�[�^�_���R�s�[
	for( unsigned int i = 0 ; i < subset.datapoint.size( ) ; i++ )
	{
		this->datapoint.push_back( subset.datapoint[ i ] ) ;
	}

	return *this ;
}
