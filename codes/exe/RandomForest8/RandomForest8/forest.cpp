#include "forest.h"

Forest::Forest( )
{

}

Forest::Forest( Config config )
{
	this->config = config ;
}

map< string , int > Forest::Train( vector< DataPoint > datapoint )
{
	map< string , int > mp ;	//木の終了条件と終了条件の出現回数の連想配列

	cout << "training forest now..." << endl ;

	//木の本数分まわす
	for( int i = 0 ; i < config.tree_num ; i++ )
	{
		//サブセットをランダムに作成
		Subset subset = MakeSubset( datapoint ) ;

		//一時的にツリー生成
		Tree tree( config ) ;
		
		cout << "[ " << i << " ]" ;

		//ツリー訓練 同時に木の訓練の終了条件を連想配列に格納
		map< string , int > tmp = tree.Train( subset ) ;
		
		//この木から帰ってくる連想配列の内容を追加

		//訓練済みのツリーを格納
		this->tree.push_back( tree ) ;
	}
	cout << endl ;

	return mp ;
}

string Forest::Test( DataPoint datapoint )
{
	map< string , int > mp ;	//クラス名と出力数の連想配列
	
	//テスト結果である木から得られるクラス名の出力数をそれぞれ計算
	for( unsigned int i = 0 ; i < tree.size( ) ; i++ )
	{
		mp[ tree[ i ].Test( datapoint ) ]++ ;
		
	}

	string max_classname ;	//最も出力数の多いクラスのクラス名
	int max_cnt = -1 ;	//最も出力数が多いクラスの出力数

	//連想配列に登録されている要素を全て探索
	for( auto itr = mp.begin( ) ; itr != mp.end( ) ; itr++ )
	{
		//より出力数の多いクラスを発見
		if( max_cnt < itr->second )
		{
			//より出力数の多いクラスを更新
			max_classname = itr->first ;
			max_cnt = itr->second ;
		}
	}

	return max_classname ;
}

Subset Forest::MakeSubset( vector< DataPoint > datapoint )
{
	Subset subset ;	//作成するサブセットのインスタンス

	//作成するサブセットのサイズ分まわす
	for( int i = 0 ; i < config.subset_size ; i++ )
	{
		int r  = rand( ) % datapoint.size( ) ;	//データ点集合のうちランダムに選ばれた点をあらわす添え字

		//重複チェックあり
		if( config.select_type == SELECTTYPE_SINGLE )
		{
			bool is_single = true ;	//重複なしフラグ

			//現在登録されているサブセット内の全ての点を対象として探索
			for( unsigned int j = 0 ; j < subset.datapoint.size( ) ; j++ )
			{
				//サブセット内に同じ点を発見
				if( subset.datapoint[ j ] == datapoint[ r ] )
				{
					//重複ありに設定
					is_single = false ;

					//探索終了
					break ;
				}
			}

			//重複なし
			if( is_single )
			{
				//サブセットに追加
				subset.datapoint.push_back( datapoint[ r ] ) ;
			}
		}
		//重複チェックなし
		else if( config.select_type == SELECTTYPE_MULTIPLE )
		{
			//無条件にサブセットに追加
			subset.datapoint.push_back( datapoint[ r ] ) ;
		}
		else	//if( config.select_type == SELECTTYPE_DEFAULT )
		{
			cout << "SELECTTYPE_DEFAULT" << endl ;
			exit( EXIT_FAILURE ) ;
		}
	}

	return subset ;
}