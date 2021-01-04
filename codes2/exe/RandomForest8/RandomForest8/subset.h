#pragma once

#include <vector>

using namespace std ;

#include "datapoint.h"

class Subset
{
public :
	vector< DataPoint > datapoint ;

	Subset( ) ;
	Subset( vector< DataPoint > datapoint ) ;
	Subset( const Subset &subset ) ;
	Subset &operator=( const Subset &subset ) ;
} ;