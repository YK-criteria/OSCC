#pragma once

#include <string>
#include <vector>

using namespace std ;

class DataPoint
{
public :
	string classname ;	//クラス名
	vector< float > feature ;	//特徴量

	DataPoint( ) ;	//デフォルトコンストラクタ	
	DataPoint( string classname , vector< float > feature ) ;	//コンストラクタ
	DataPoint( const DataPoint &datapoint ) ;	//コピーコンストラクタ
	DataPoint &operator=( const DataPoint &datapoint ) ;	//代入演算子オーバーロード
	bool operator==( const DataPoint &datapoint ) ;	//比較演算子オーバーロード
} ;
