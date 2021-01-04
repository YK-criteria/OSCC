#pragma once

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std ;

#include "config.h"
#include "split.h"
#include "subset.h"
#include "datapoint.h"

class Node
{
public :
	Config config ;
	string classname ;
	Split split ;
	vector< Node > child ;

	Node( ) ;
	Node( Config config ) ;
	
	map< string , int > Train( Subset subset , int depth ) ;
	string GetClassName( Subset subset ) ;
	vector< float > MakeNormal( Subset subset ) ;
	float RandFloat( float min , float max ) ;
	float MakeThreshold( Subset subset ,  vector< float > normal ) ;
	float CalcInformationGain( Subset subset , Split split ) ;
	float Log2( float x ) ;
	float Dot( vector< float > normal , DataPoint datapoint ) ;
	Subset SeparateSubset( Subset subset , bool is_lower ) ;
	string Test( DataPoint datapoint ) ;

	void Print( int depth ) ;
} ;
