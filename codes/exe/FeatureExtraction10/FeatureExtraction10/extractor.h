#pragma once

//画像ファイル読み込みヘッダー
#include "pbm.h"

//個別の特徴抽出機ヘッダー
#include "area.h"
#include "curvature.h"
#include "direction.h"
#include "jaggy.h"
#include "funicular.h"
#include "length.h"
#include "regionnum.h"

class Extractor
{
public:
	Extractor( string filename ) ;	//開く画像ファイル指定用コンストラクタ
	~Extractor( ) ;	//デストラクタ

	void Extract( ) ;	//特徴抽出
	void Output( ) ;	//出力

	Pbm input ;	//画像ファイル

	Area area ;	//上皮領域の面積に関する特徴抽出機
	Curvature curvature ;	//上皮領域の曲率に関する特徴抽出機
	Direction direction ;	//突起の方向に関する特徴抽出機
	Jaggy jaggy ;	//ギザギザ
	Funicular funicular ;	//上皮領域の索状さに関する特徴抽出機
	Length length ;	//境界線の長さに関する特徴抽出機
	RegionNum regionnum ;	//上皮領域の数に関する特徴抽出機
} ;