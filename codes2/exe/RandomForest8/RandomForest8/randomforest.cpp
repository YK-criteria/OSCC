#include "randomforest.h"

RandomForest::RandomForest( )
{
	cnt_init = 0 ;
}

RandomForest::RandomForest( string filename )
{
	cnt_init = 0 ;

	this->filename = filename ;

	Init( ) ;
}

void RandomForest::Init( )
{
	if( cnt_init == 0 )
	{
		srand( ( unsigned )time( NULL ) ) ;
		cnt_init++ ;
	}

	ifstream ifs( filename ) ;
	if( !ifs )
	{
		cout << filename << " can not open" << endl ;
		exit( EXIT_FAILURE ) ;
	}

	string dummy ;	
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
	
	string split , select ;
	ifs >> dummy ;	ifs >> split ;
	ifs >> dummy ;	ifs >> select ;
	transform( split.cbegin( ) , split.cend( ) , split.begin( ) , toupper ) ;
	transform( select.cbegin( ) , select.cend( ) , select.begin( ) , toupper ) ;
	
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

	ifs >> dummy ;	ifs >> config.train_filename ;
	ifs >> dummy ;	ifs >> config.test_filename ;
	ifs >> dummy ;	ifs >> config.result_filename ;

	forest = Forest( config ) ;

	ifs = ifstream ( config.train_filename ) ;
	for( int i = 0 ; i < config.train_num ; i++ )
	{
		DataPoint datapoint ;
		ifs >> datapoint.classname ;
		for( int j = 0 ; j < config.dimension_num ; j++ )
		{
			float feature ;
			ifs >> feature ;
			datapoint.feature.push_back( feature ) ;
		}
		traindata.push_back( datapoint ) ;
	}

	//テストデータ格納
	ifs = ifstream ( config.test_filename ) ;
	for( int i = 0 ; i < config.test_num ; i++ )
	{
		DataPoint datapoint ;
		ifs >> datapoint.classname ;
		for( int j = 0 ; j < config.dimension_num ; j++ )
		{
			float feature ;
			ifs >> feature ;
			datapoint.feature.push_back( feature ) ;
		}
		testdata.push_back( datapoint ) ;
	}
}

void RandomForest::Init( string filename )
{
	this->filename = filename ;

	Init( ) ;
}

void RandomForest::Train( )
{
	map< string , int > exit_conditions = forest.Train( traindata ) ;

}

void RandomForest::Test( )
{
	ofstream ofs( config.result_filename ) ;
	map< string , int > mp_true ;
	map< string , int > mp_search ;
	map< string , int > mp_compatibility;
	
	for( int i = 0 ; i < testdata.size( ) ; i++ )
	{
		mp_true[ testdata[ i ].classname ]++ ;
	}

	for( unsigned int i = 0 ; i < testdata.size( ) ; i++ )
	{
		string test_classname = forest.Test( testdata[ i ] ) ;

		mp_search[ test_classname ]++ ;

		mp_compatibility[ testdata[ i ].classname ] += testdata[ i ].classname == test_classname ? 1 : 0 ;

		ofs << "true classname[ " << testdata[ i ].classname << " ]" << "result classname[ " << test_classname << " ]" << endl ;
	}

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
		
		sprintf( str_num_true , "%6d" , num_true ) ;
		sprintf( str_num_search , "%6d" , num_search ) ;
		sprintf( str_num_compatibility , "%6d" , num_compatibility ) ;
		sprintf( str_precision , "%6f" , precision ) ;
		sprintf( str_recall , "%6f" , recall ) ;
		sprintf( str_f_measure , "%6f" , 2 * precision * recall / ( precision + recall ) ) ;

		ofs << itr->first << " " ;
		ofs << "true[ " << str_num_true << " ]  "  ;
		ofs << "search[ " << str_num_true << " ]  " ;
		ofs << "compatibility[ " << str_num_compatibility << " ]  " ;
		ofs << "precision[ " << str_precision << " ]  " ;
		ofs << "recall[ " << str_recall << " ]  " ;
		ofs << "F-Measure[ " << str_f_measure << " ]  " << endl ;

		average_f_measure += 2 * precision * recall / ( precision + recall ) ;
	}

	ofs << "average_f_measure" << average_f_measure / ( float )cnt << endl << endl ;

	ofs.close( ) ;
}
