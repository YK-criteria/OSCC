#pragma once

#include <cmath>

#include "pbm.h"
#include "basefeature.h"
#include "point.h"
#include "borderlist.h"
#include "enum_direction.h"
#include "ppm.h"

class Curvature : public BaseFeature
{
public :
	Curvature( ) ;
	Curvature( Pbm image ) ;
	Curvature( const Curvature &curvature ) ;
	~Curvature( ) ;

	Curvature& operator=( const Curvature& curvature ) ;

	virtual void Extract( ) ;
	void SetIsBorder( ) ;
	void Labeling( ) ;
	void Calc( ) ;
	float Radian( Point start , Point middle , Point end ) ;
	void Devide( int num , int start , int end , int cnt ) ;
	float Distance( Point p1 , Point p2 , Point p3 ) ;

	void Print( ) ;

	Pbm original ;
	bool **is_border ;
	BorderList borderlist ;

	int **index ;
	int **label ;

	const int dim ;
	const int increase ;
	const float threshold ;

} ;
