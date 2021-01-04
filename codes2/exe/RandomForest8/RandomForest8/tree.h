#pragma once

#include <iostream>
#include <string>

using namespace std ;

#include "datapoint.h"
#include "subset.h"
#include "node.h"

class Tree
{
public :
	Config config ;
	Node root ;
	
	Tree( ) ;
	Tree( Config config ) ;
	
	map< string , int > Train( Subset subset ) ;
	string Test( DataPoint datapoint ) ;
} ;
