#pragma once

#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>

using namespace std ;

#include "datapoint.h"
#include "forest.h"

class RandomForest
{
public :
	int cnt_init ;
	Config config ;
	string filename ;
	Forest forest ;
	vector< DataPoint > traindata ;
	vector< DataPoint > testdata ;

	RandomForest( ) ;
	RandomForest( string filename ) ;
	
	void Init( ) ;
	void Init( string filename ) ;
	void Train( ) ;
	void Test( ) ;
} ;