#pragma once

#include "pbm.h"
#include "basefeature.h"

class Area : public BaseFeature
{
public :
	Area( ) ;
	Area( Pbm image ) ;
	Area( const Area &area ) ;
	~Area( ) ;

	Area& operator=( const Area& area ) ;

	virtual void Extract( ) ;
	void Expand( ) ;
	void Labeling( ) ;
	void Count( ) ;
	void MakeList( ) ;
	int CountSmall( ) ;

	Pbm original ;
	int **index ;
	
	int list_length ;
	int list_count ;
	int *label_list ;
	int *area_list ;
	
	const int dim ;
	const int increase ;
	float threshold ;
} ;
