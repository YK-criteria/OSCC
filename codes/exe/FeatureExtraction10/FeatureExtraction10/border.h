#pragma once

#include "point.h"
#include <iostream>

class Border
{
public :
	Border( ) ;	//コンストラクタ
	Border( int length ) ;	//コンストラクタ
	Border( const Border &border ) ;	//コピーコンストラクタ
	~Border( ) ;	//デストラクタ
	
	Border& operator=( const Border& border ) ;	//代入演算子オーバーロード

	void Add( int x , int y ) ;	//座標追加
	bool Exist( int x , int y ) ;	//指定座標があるか

	int length ;	//境界線の長さ
	int max_length ;	//point配列に入る画素の最大数
	Point *point ;	//境界線の画素
	bool island ;	//島フラグ

	//パラメータ
	const int increase ;	//リスト拡張分
} ;
