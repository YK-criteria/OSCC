#pragma once

class Point
{
public :
	Point( ) ;	//コンストラクタ
	Point( int x , int y ) ;	//座標指定用コンストラクタ

	int x ;	//x座標
	int y ;	//y座標
	bool devide ;	//分割点フラグ
} ;
