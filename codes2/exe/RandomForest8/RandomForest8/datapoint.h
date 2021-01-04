#pragma once

#include <string>
#include <vector>

using namespace std ;

class DataPoint
{
public :
	string classname ;
	vector< float > feature ;

	DataPoint( ) ;	
	DataPoint( string classname , vector< float > feature ) ;
	DataPoint( const DataPoint &datapoint ) ;
	DataPoint &operator=( const DataPoint &datapoint ) ;
	bool operator==( const DataPoint &datapoint ) ;
} ;
