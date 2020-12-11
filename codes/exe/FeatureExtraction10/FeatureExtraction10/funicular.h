#pragma once

#include "pbm.h"
#include "basefeature.h"

class Funicular : public BaseFeature
{
public :
	Funicular( ) ;	//コンストラクタ
	Funicular( Pbm image ) ;	//画像ファイル初期化用コンストラクタ
	Funicular( const Funicular &funicular ) ;	//コピーコンストラクタ
	~Funicular( ) ;	//デストラクタ

	Funicular& operator=( const Funicular& funicular ) ;	//代入演算子オーバーロード

	virtual void Extract( ) ;	//抽出関数　親クラスの純粋仮想関数の実装
	void Expand( ) ;	//膨張処理
	void Labeling( ) ;	//ラベリング処理
	void Count( ) ;	//カウント
	void MakeList( ) ;	//黒画素領域の面積と境界線の長さをリスト化
	float Max( ) ;

	Pbm original ;	//画像データ
	int **index ;	//ラベリング用インデックス配列
	
	int list_length ;	//リストの長さ
	int list_count ;	//リストに含まれるindexの数
	int *label_list ;	//領域番号リスト
	int *area_list ;	//領域面積リスト
	int *border_list ;	//境界線の長さリスト
	
	//パラメータ
	const int dim ;
	const int increase ;	//1回のリスト増加数
	float threshold ;	//小さい黒画素領域として数えるときの面積の閾値
} ;
