#pragma once

#include "border.h"

class BorderList
{
public :
	BorderList( ) ;
	void Add( ) ;
	bool Exist( int x , int y ) ;

	int num ;
	int max_num ;
	Border *border ;

	const int increase ;
} ;
