#pragma once

#include <cstdio>

#include "pbm.h"

class BaseFeature
{
public :
	BaseFeature( ) ;	//コンストラクタ
	BaseFeature( const BaseFeature &src ) ;	//コピーコンストラクタ
	~BaseFeature( ) ;	//デストラクタ
	
	virtual void Extract( ) = 0 ;	//特徴抽出関数　純粋仮想関数だから派生クラスでは必ず実装すること
	void Output( ) ;	//出力関数

	int dimension ;	//次元
	float *feature ;	//特徴量配列
} ;