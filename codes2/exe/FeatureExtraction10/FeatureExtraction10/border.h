#pragma once

#include "point.h"
#include <iostream>

class Border
{
public :
	Border( ) ;
	Border( int length ) ;
	Border( const Border &border ) ;
	~Border( ) ;
	
	Border& operator=( const Border& border ) ;

	void Add( int x , int y ) ;
	bool Exist( int x , int y ) ;

	int length ;
	int max_length ;
	Point *point ;
	bool island ;

	const int increase ;
} ;
