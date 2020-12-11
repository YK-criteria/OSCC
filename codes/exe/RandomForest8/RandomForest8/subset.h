#pragma once

#include <vector>

using namespace std ;

#include "datapoint.h"

class Subset
{
public :
	vector< DataPoint > datapoint ;	//データ点

	Subset( ) ;	//デフォルトコンストラクタ
	Subset( vector< DataPoint > datapoint ) ;	//コンストラクタ
	Subset( const Subset &subset ) ;	//コピーコンストラクタ
	Subset &operator=( const Subset &subset ) ;	//代入演算子オーバーロード
} ;