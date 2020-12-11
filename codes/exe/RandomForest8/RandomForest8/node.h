#pragma once

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std ;

#include "config.h"
#include "split.h"
#include "subset.h"
#include "datapoint.h"

class Node
{
public :
	Config config ;	//フォレスト構築パラメータ
	string classname ;	//ノードにもっとも多いデータのクラス名 葉ノードで設定
	Split split ;	//分割関数
	vector< Node > child ;	//子ノード

	Node( ) ;	//デフォルトコンストラクタ
	Node( Config config ) ;	//コンストラクタ
	
	map< string , int > Train( Subset subset , int depth ) ;	//訓練
	string GetClassName( Subset subset ) ;	//サブセット内の最も多いクラス名を計算
	vector< float > MakeNormal( Subset subset ) ;	//法線ベクトル作成
	float RandFloat( float min , float max ) ;	//最小値以上最大値以下の数値をランダムに返す
	float MakeThreshold( Subset subset ,  vector< float > normal ) ;	//閾値作成
	float CalcInformationGain( Subset subset , Split split ) ;	//情報利得計算	
	float Log2( float x ) ;	//ログ2底
	float Dot( vector< float > normal , DataPoint datapoint ) ;	//内積計算
	Subset SeparateSubset( Subset subset , bool is_lower ) ;	//サブセット分割処理
	string Test( DataPoint datapoint ) ;	//テスト

	void Print( int depth ) ;
} ;
