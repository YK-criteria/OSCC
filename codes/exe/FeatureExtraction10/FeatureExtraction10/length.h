#pragma once

#include "pbm.h"
#include "basefeature.h"

class Length : public BaseFeature
{
public :
	Length( ) ;	//コンストラクタ
	Length( Pbm image ) ;	//画像ファイル初期化用コンストラクタ
	Length( const Length &length ) ;	//コピーコンストラクタ
	~Length( ) ;	//デストラクタ

	Length& operator=( const Length& length ) ;	//代入演算子オーバーロード

	virtual void Extract( ) ;	//抽出関数　親クラスの純粋仮想関数の実装
	
	Pbm original ;	//画像データ

	//パラメータ
	const int dim ;
} ;
