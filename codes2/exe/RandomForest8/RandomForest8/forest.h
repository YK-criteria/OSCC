#pragma once

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
using namespace std ;

#include "config.h"
#include "datapoint.h"
#include "tree.h"
#include "subset.h"

class Forest
{
public :
	Config config ;
	vector< Tree > tree ;

	Forest( ) ;
	Forest( Config config ) ;
	
	map< string , int > Train( vector< DataPoint > datapoint ) ;
	Subset MakeSubset( vector< DataPoint > datapoint ) ;
	string Test( DataPoint datapoint ) ;
} ;