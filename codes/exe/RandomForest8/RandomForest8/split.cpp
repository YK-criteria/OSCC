#include "split.h"

Split::Split( )
{

}

Split::Split( vector< float > normal , float threshold )
{
	//臒l�R�s�[
	this->threshold = threshold ;

	//�@���x�N�g���R�s�[
	for( unsigned int i = 0 ; i < normal.size( ) ; i++ )
	{
		this->normal.push_back( normal[ i ] ) ;
	}
}

Split::Split( const Split &split )
{
	//臒l�R�s�[
	threshold = split.threshold ;

	//�@���x�N�g���R�s�[
	/*for( unsigned int i = 0 ; i < split.normal.size( ) ; i++ )
	{
		normal.push_back( split.normal[ i ] ) ;
	}*/
	normal = split.normal ;
}

Split &Split::operator=( const Split &split )
{
	//臒l�R�s�[
	threshold = split.threshold ;

	//�@���x�N�g���R�s�[
	/*for( unsigned int i = 0 ; i < split.normal.size( ) ; i++ )
	{
		normal.push_back( split.normal[ i ] ) ;
	}*/
	normal = split.normal ;

	return *this ;
}
