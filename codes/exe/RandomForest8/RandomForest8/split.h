#pragma once

#include <vector>

using namespace std ;

#include "config.h"

class Split
{
public :
	float threshold ;	//閾値
	vector< float > normal ;	//法線ベクトル
	
	Split( ) ;	//デフォルトコンストラクタ
	Split( vector< float > normal , float threshold ) ;	//コンストラクタ
	Split( const Split &split ) ;	//コピーコンストラクタ
	Split &operator=( const Split &split ) ;	//代入演算子オーバーロード
} ;
