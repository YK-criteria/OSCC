#pragma once

#include <vector>

using namespace std ;

#include "config.h"

class Split
{
public :
	float threshold ;
	vector< float > normal ;
	
	Split( ) ;
	Split( vector< float > normal , float threshold ) ;
	Split( const Split &split ) ;
	Split &operator=( const Split &split ) ;
} ;
