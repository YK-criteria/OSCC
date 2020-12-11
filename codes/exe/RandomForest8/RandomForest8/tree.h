#pragma once

#include <iostream>
#include <string>

using namespace std ;

#include "datapoint.h"
#include "subset.h"
#include "node.h"

class Tree
{
public :
	Config config ;	//フォレスト構築パラメータ
	Node root ;	//根ノード
	
	Tree( ) ;	//デフォルトコンストラクタ
	Tree( Config config ) ;	//コンストラクタ
	
	map< string , int > Train( Subset subset ) ;	//訓練
	string Test( DataPoint datapoint ) ;	//テスト
} ;
