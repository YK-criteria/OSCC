#include "randomforest.h"

RandomForest::RandomForest( )
{
	cnt_init = 0 ;
}

RandomForest::RandomForest( string filename )
{
	cnt_init = 0 ;

	//設定ファイル名を設定
	this->filename = filename ;

	//設定ファイルに基づき初期化
	Init( ) ;
}

void RandomForest::Init( )
{
	//最初にInitが呼ばれた
	if( cnt_init == 0 )
	{
		//乱数シード初期化	
		srand( ( unsigned )time( NULL ) ) ;
		cnt_init++ ;
	}

	//設定ファイル読み込み
	ifstream ifs( filename ) ;
	if( !ifs )
	{
		cout << filename << " can not open" << endl ;
		exit( EXIT_FAILURE ) ;
	}

	string dummy ;	//空読み用変数
	//空読みと数値格納
	ifs >> dummy ;	ifs >> config.class_num ;
	ifs >> dummy ;	ifs >> config.dimension_num ;
	ifs >> dummy ;	ifs >> config.select_dimension_num ;
	ifs >> dummy ;	ifs >> config.tree_num ;
	ifs >> dummy ;	ifs >> config.max_depth ;
	ifs >> dummy ;	ifs >> config.normal_num ;
	ifs >> dummy ;	ifs >> config.threshold_num ;
	ifs >> dummy ;	ifs >> config.train_num ;
	ifs >> dummy ;	ifs >> config.test_num ;
	ifs >> dummy ;	ifs >> config.subset_size ;
	ifs >> dummy ;	ifs >> config.min_subset_size ;
	ifs >> dummy ;	ifs >> config.min_informationgain ;
	
	//文字列から格納する変数を設定
	string split , select ;
	ifs >> dummy ;	ifs >> split ;
	ifs >> dummy ;	ifs >> select ;
	//大文字小文字区別なし
	transform( split.cbegin( ) , split.cend( ) , split.begin( ) , toupper ) ;
	transform( select.cbegin( ) , select.cend( ) , select.begin( ) , toupper ) ;
	
	//分割方法設定
	if( split == "AXIS" )
	{
		config.split_type = SPLITTYPE_AXIS ;
	}
	else if( split == "LINEAR" )
	{
		config.split_type = SPLITTYPE_LINEAR ;
	}
	else
	{
		config.split_type = SPLITTYPE_DEFAULT ;
	}
	//選択方法設定
	if( select == "SINGLE" )
	{
		config.select_type = SELECTTYPE_SINGLE ;
	}
	else if( select == "MULTIPLE" )
	{
		config.select_type = SELECTTYPE_MULTIPLE ;
	}
	else
	{
		config.select_type = SELECTTYPE_DEFAULT ;
	}

	//文字列格納
	ifs >> dummy ;	ifs >> config.train_filename ;
	ifs >> dummy ;	ifs >> config.test_filename ;
	ifs >> dummy ;	ifs >> config.result_filename ;

	//フォレストのconfigを指定
	forest = Forest( config ) ;

	//訓練データ格納
	ifs = ifstream ( config.train_filename ) ;
	for( int i = 0 ; i < config.train_num ; i++ )
	{
		//訓練データ点生成
		DataPoint datapoint ;
		//データ点のクラス名設定
		ifs >> datapoint.classname ;
		//データ点の特徴量設定
		for( int j = 0 ; j < config.dimension_num ; j++ )
		{
			float feature ;
			ifs >> feature ;
			datapoint.feature.push_back( feature ) ;
		}
		//データ点を訓練データに追加
		traindata.push_back( datapoint ) ;
	}

	//テストデータ格納
	ifs = ifstream ( config.test_filename ) ;
	for( int i = 0 ; i < config.test_num ; i++ )
	{
		//テストデータ点生成
		DataPoint datapoint ;
		//データ点のクラス名設定
		ifs >> datapoint.classname ;
		//データ点の特徴量設定
		for( int j = 0 ; j < config.dimension_num ; j++ )
		{
			float feature ;
			ifs >> feature ;
			datapoint.feature.push_back( feature ) ;
		}
		//データ点をテストデータに追加
		testdata.push_back( datapoint ) ;
	}
}

void RandomForest::Init( string filename )
{
	//設定ファイル名を設定
	this->filename = filename ;

	//設定ファイルに基づき初期化
	Init( ) ;
}

void RandomForest::Train( )
{
	//フォレストの訓練とフォレスト(または木)の終了条件文字列の格納
	map< string , int > exit_conditions = forest.Train( traindata ) ;

	//フォレストの訓練と表示
	//cout << "forest exit condition : " << endl ;
	//for()
}

void RandomForest::Test( )
{
	ofstream ofs( config.result_filename ) ;	//出力ファイルストリーム
	map< string , int > mp_true ;	//正解数
	map< string , int > mp_search ;	//検索数
	map< string , int > mp_compatibility;	//適合数
	
	//正解数を計算
	for( int i = 0 ; i < testdata.size( ) ; i++ )
	{
		mp_true[ testdata[ i ].classname ]++ ;
	}

	//テスト
	for( unsigned int i = 0 ; i < testdata.size( ) ; i++ )
	{
		string test_classname = forest.Test( testdata[ i ] ) ;	//テストとテスト結果を格納

		//検索結果まとめ
		mp_search[ test_classname ]++ ;

		//適合結果まとめ
		mp_compatibility[ testdata[ i ].classname ] += testdata[ i ].classname == test_classname ? 1 : 0 ;

		ofs << "true classname[ " << testdata[ i ].classname << " ]" << "result classname[ " << test_classname << " ]" << endl ;
	}

	//テスト結果出力
	float average_f_measure = 0.f ;
	int cnt = 0 ;
	for( auto itr = mp_true.begin( ) ; itr != mp_true.end( ) ; itr++ , cnt++ )
	{
		int num_true = itr->second ;
		int num_search = mp_search[ itr->first ] ;
		int num_compatibility = mp_compatibility[ itr->first ] ;
		float precision = ( float )num_compatibility / ( float )num_search ;
		float recall = ( float )num_compatibility / ( float )num_true ;
		char str_num_true[ 64 ] ;
		char str_num_search[ 64 ] ;
		char str_num_compatibility[ 64 ] ;
		char str_precision[ 64 ] ;
		char str_recall[ 64 ] ;
		char str_f_measure[ 64 ] ;
		
		//書式揃え
		sprintf( str_num_true , "%6d" , num_true ) ;
		sprintf( str_num_search , "%6d" , num_search ) ;
		sprintf( str_num_compatibility , "%6d" , num_compatibility ) ;
		sprintf( str_precision , "%6f" , precision ) ;
		sprintf( str_recall , "%6f" , recall ) ;
		sprintf( str_f_measure , "%6f" , 2 * precision * recall / ( precision + recall ) ) ;

		//出力
		ofs << itr->first << " " ;
		ofs << "true[ " << str_num_true << " ]  "  ;
		ofs << "search[ " << str_num_true << " ]  " ;
		ofs << "compatibility[ " << str_num_compatibility << " ]  " ;
		ofs << "precision[ " << str_precision << " ]  " ;
		ofs << "recall[ " << str_recall << " ]  " ;
		ofs << "F-Measure[ " << str_f_measure << " ]  " << endl ;

		//平均F値
		average_f_measure += 2 * precision * recall / ( precision + recall ) ;
	}

	//平均F値出力
	ofs << "average_f_measure" << average_f_measure / ( float )cnt << endl << endl ;

	//ファイルストリーム終了
	ofs.close( ) ;
}

