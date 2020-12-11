#pragma once

#include "pbm.h"
#include "basefeature.h"
#include <vector>

class RegionNum : public BaseFeature
{
public :
	RegionNum( ) ;	//コンストラクタ
	RegionNum( Pbm image ) ;	//画像ファイル初期化用コンストラクタ
	RegionNum( const RegionNum &regionnum ) ;	//コピーコンストラクタ
	~RegionNum( ) ;	//デストラクタ

	RegionNum& operator=( const RegionNum& regionnum ) ;	//代入演算子オーバーロード

	virtual void Extract( ) ;	//抽出関数　親クラスの純粋仮想関数の実装
	void Expand( ) ;	//膨張処理
	void Labeling( ) ;	//ラベリング処理
	int Count( ) ;	//カウント

	Pbm original ;	//画像データ
	Pbm expansion ;	//黒画素膨張画像
	int **index ;	//ラベリング用インデックス配列

	//パラメータ
	const int dim ;
	const int exp_steps ;
} ;
