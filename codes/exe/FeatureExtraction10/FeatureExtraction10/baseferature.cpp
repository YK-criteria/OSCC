#include "basefeature.h"

BaseFeature::BaseFeature( )
{

}

BaseFeature::BaseFeature( const BaseFeature &src )
{
	//�K�v��������
}

BaseFeature::~BaseFeature( )
{
	
}

void BaseFeature::Output( )
{
	for( int i = 0 ; i < dimension ; i++ )
	{
		cout << feature[ i ] << " " ;
	}
}
