#pragma once

#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>

using namespace std ;

#include "datapoint.h"
#include "forest.h"

class RandomForest
{
public :
	int cnt_init ;	//Initが呼ばれた回数を記録 srandを1回しか呼ばないようにするため
	Config config ;	//フォレスト構築パラメータ
	string filename ;	//設定ファイル名
	Forest forest ;	//フォレスト
	vector< DataPoint > traindata ;	//訓練データ
	vector< DataPoint > testdata ;	//テストデータ

	RandomForest( ) ;	//デフォルトコンストラクタ
	RandomForest( string filename ) ;	//コンストラクタ
	
	void Init( ) ;	//初期化処理
	void Init( string filename ) ;	//ファイル名指定初期化処理
	void Train( ) ;	//訓練
	void Test( ) ;	//テスト
} ;