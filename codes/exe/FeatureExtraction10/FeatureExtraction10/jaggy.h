#pragma once

#include <cmath>

#include "pbm.h"
#include "basefeature.h"
#include "point.h"
#include "borderlist.h"
#include "enum_direction.h"
#include "ppm.h"

class Jaggy : public BaseFeature
{
public :
	Jaggy( ) ;	//コンストラクタ
	Jaggy( Pbm image ) ;	//画像ファイル初期化用コンストラクタ
	Jaggy( const Jaggy &jaggy ) ;	//コピーコンストラクタ
	~Jaggy( ) ;	//デストラクタ

	Jaggy& operator=( const Jaggy& jaggy ) ;	//代入演算子オーバーロード

	virtual void Extract( ) ;	//抽出関数　親クラスの純粋仮想関数の実装
	void SetIsBorder( ) ;	//境界線画素の設定
	void Labeling( ) ;	//境界線のラベリング処理
	void Calc( ) ;	//ギザギザ計算
	float Radian( Point start , Point middle , Point end ) ;	//3点を与えたときにできる2ベクトルが成す角度（ラジアン）を計算
	void Devide( int num , int start , int end , int cnt ) ;	//境界線分割
	float Distance( Point p1 , Point p2 , Point p3 ) ;	//p1p2を通る直線に対するp3との距離
	bool Tip( int size , Point point ) ;

	void Print( ) ;

	Pbm original ;	//画像データ
	bool **is_border ;	//境界線を表す画素
	BorderList borderlist ;	//境界線リスト

	int **index ;
	int **label ;


	//パラメータ
	const int dim ;
	const int increase ;	//1回のリスト増加数
	const float threshold ;	//

} ;
