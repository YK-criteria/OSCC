#include "basefeature.h"

BaseFeature::BaseFeature( )
{

}

BaseFeature::BaseFeature( const BaseFeature &src )
{
	//必要か検討中
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
