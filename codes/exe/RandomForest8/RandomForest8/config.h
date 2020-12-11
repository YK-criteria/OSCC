#pragma once

#include <string>

using namespace std ;

enum SplitType
{
	SPLITTYPE_DEFAULT = -1 ,
	SPLITTYPE_AXIS ,
	SPLITTYPE_LINEAR ,
} ;

enum SelectType
{
	SELECTTYPE_DEFAULT = -1 ,
	SELECTTYPE_SINGLE ,
	SELECTTYPE_MULTIPLE ,
} ;

class Config
{
public :
	int class_num ;	//クラス数
	int dimension_num ;	//次元数
	int select_dimension_num ;	//選択次元数
	int tree_num ;	//木の本数
	int max_depth ;	//最大深度
	int normal_num ;	//法線ベクトルの候補数
	int threshold_num ;	//閾値の候補数
	int train_num ;	//訓練データ数
	int test_num ;	//テストデータ数
	int subset_size ;	//初期サブセットのサイズ
	int min_subset_size ;	//最小サブセットのサイズ
	float min_informationgain ;	//最小情報利得
	SplitType split_type ;	//分割方法
	SelectType select_type ;	//サブセット作成時のデータ点選択方法
	string train_filename ;	//訓練データファイルのパス
	string test_filename ;	//テストデータファイルのパス
	string result_filename ;	//テスト結果出力ファイルのパス

	Config( ) ;	//デフォルトコンストラクタ
	//ベクトルの代入が無いからコピーコンストラクタと代入演算子オーバーロードは作らなくてもよい
} ;
