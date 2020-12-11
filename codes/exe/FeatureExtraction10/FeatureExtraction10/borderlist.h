#pragma once

#include "border.h"

class BorderList
{
public :
	BorderList( ) ;	//コンストラクタ
	void Add( ) ;	//境界線の新規追加
	bool Exist( int x , int y ) ;	//指定座標を含む境界線があるか

	int num ;	//境界線数
	int max_num ;	//border配列に入る境界線の最大数
	Border *border ;	//境界線配列

	//パラメータ
	const int increase ;
} ;
