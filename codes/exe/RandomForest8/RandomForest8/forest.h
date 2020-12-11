#pragma once

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
using namespace std ;

#include "config.h"
#include "datapoint.h"
#include "tree.h"
#include "subset.h"

class Forest
{
public :
	Config config ;	//フォレスト構築パラメータ
	vector< Tree > tree ;	//ツリー

	Forest( ) ;	//デフォルトコンストラクタ
	Forest( Config config ) ;	//コンストラクタ
	
	map< string , int > Train( vector< DataPoint > datapoint ) ;	//訓練
	Subset MakeSubset( vector< DataPoint > datapoint ) ;	//サブセット作成
	string Test( DataPoint datapoint ) ;	//テスト
} ;