#pragma once

#include <cstdio>

#include "pbm.h"

class BaseFeature
{
public :
	BaseFeature( ) ;
	BaseFeature( const BaseFeature &src ) ;
	~BaseFeature( ) ;
	
	virtual void Extract( ) = 0 ;
	void Output( ) ;

	int dimension ;
	float *feature ;
} ;