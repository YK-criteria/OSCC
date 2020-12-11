#include "randomforest.h"

RandomForest::RandomForest( )
{
	cnt_init = 0 ;
}

RandomForest::RandomForest( string filename )
{
	cnt_init = 0 ;

	//�ݒ�t�@�C������ݒ�
	this->filename = filename ;

	//�ݒ�t�@�C���Ɋ�Â�������
	Init( ) ;
}

void RandomForest::Init( )
{
	//�ŏ���Init���Ă΂ꂽ
	if( cnt_init == 0 )
	{
		//�����V�[�h������	
		srand( ( unsigned )time( NULL ) ) ;
		cnt_init++ ;
	}

	//�ݒ�t�@�C���ǂݍ���
	ifstream ifs( filename ) ;
	if( !ifs )
	{
		cout << filename << " can not open" << endl ;
		exit( EXIT_FAILURE ) ;
	}

	string dummy ;	//��ǂݗp�ϐ�
	//��ǂ݂Ɛ��l�i�[
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
	
	//�����񂩂�i�[����ϐ���ݒ�
	string split , select ;
	ifs >> dummy ;	ifs >> split ;
	ifs >> dummy ;	ifs >> select ;
	//�啶����������ʂȂ�
	transform( split.cbegin( ) , split.cend( ) , split.begin( ) , toupper ) ;
	transform( select.cbegin( ) , select.cend( ) , select.begin( ) , toupper ) ;
	
	//�������@�ݒ�
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
	//�I����@�ݒ�
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

	//������i�[
	ifs >> dummy ;	ifs >> config.train_filename ;
	ifs >> dummy ;	ifs >> config.test_filename ;
	ifs >> dummy ;	ifs >> config.result_filename ;

	//�t�H���X�g��config���w��
	forest = Forest( config ) ;

	//�P���f�[�^�i�[
	ifs = ifstream ( config.train_filename ) ;
	for( int i = 0 ; i < config.train_num ; i++ )
	{
		//�P���f�[�^�_����
		DataPoint datapoint ;
		//�f�[�^�_�̃N���X���ݒ�
		ifs >> datapoint.classname ;
		//�f�[�^�_�̓����ʐݒ�
		for( int j = 0 ; j < config.dimension_num ; j++ )
		{
			float feature ;
			ifs >> feature ;
			datapoint.feature.push_back( feature ) ;
		}
		//�f�[�^�_���P���f�[�^�ɒǉ�
		traindata.push_back( datapoint ) ;
	}

	//�e�X�g�f�[�^�i�[
	ifs = ifstream ( config.test_filename ) ;
	for( int i = 0 ; i < config.test_num ; i++ )
	{
		//�e�X�g�f�[�^�_����
		DataPoint datapoint ;
		//�f�[�^�_�̃N���X���ݒ�
		ifs >> datapoint.classname ;
		//�f�[�^�_�̓����ʐݒ�
		for( int j = 0 ; j < config.dimension_num ; j++ )
		{
			float feature ;
			ifs >> feature ;
			datapoint.feature.push_back( feature ) ;
		}
		//�f�[�^�_���e�X�g�f�[�^�ɒǉ�
		testdata.push_back( datapoint ) ;
	}
}

void RandomForest::Init( string filename )
{
	//�ݒ�t�@�C������ݒ�
	this->filename = filename ;

	//�ݒ�t�@�C���Ɋ�Â�������
	Init( ) ;
}

void RandomForest::Train( )
{
	//�t�H���X�g�̌P���ƃt�H���X�g(�܂��͖�)�̏I������������̊i�[
	map< string , int > exit_conditions = forest.Train( traindata ) ;

	//�t�H���X�g�̌P���ƕ\��
	//cout << "forest exit condition : " << endl ;
	//for()
}

void RandomForest::Test( )
{
	ofstream ofs( config.result_filename ) ;	//�o�̓t�@�C���X�g���[��
	map< string , int > mp_true ;	//����
	map< string , int > mp_search ;	//������
	map< string , int > mp_compatibility;	//�K����
	
	//���𐔂��v�Z
	for( int i = 0 ; i < testdata.size( ) ; i++ )
	{
		mp_true[ testdata[ i ].classname ]++ ;
	}

	//�e�X�g
	for( unsigned int i = 0 ; i < testdata.size( ) ; i++ )
	{
		string test_classname = forest.Test( testdata[ i ] ) ;	//�e�X�g�ƃe�X�g���ʂ��i�[

		//�������ʂ܂Ƃ�
		mp_search[ test_classname ]++ ;

		//�K�����ʂ܂Ƃ�
		mp_compatibility[ testdata[ i ].classname ] += testdata[ i ].classname == test_classname ? 1 : 0 ;

		ofs << "true classname[ " << testdata[ i ].classname << " ]" << "result classname[ " << test_classname << " ]" << endl ;
	}

	//�e�X�g���ʏo��
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
		
		//��������
		sprintf( str_num_true , "%6d" , num_true ) ;
		sprintf( str_num_search , "%6d" , num_search ) ;
		sprintf( str_num_compatibility , "%6d" , num_compatibility ) ;
		sprintf( str_precision , "%6f" , precision ) ;
		sprintf( str_recall , "%6f" , recall ) ;
		sprintf( str_f_measure , "%6f" , 2 * precision * recall / ( precision + recall ) ) ;

		//�o��
		ofs << itr->first << " " ;
		ofs << "true[ " << str_num_true << " ]  "  ;
		ofs << "search[ " << str_num_true << " ]  " ;
		ofs << "compatibility[ " << str_num_compatibility << " ]  " ;
		ofs << "precision[ " << str_precision << " ]  " ;
		ofs << "recall[ " << str_recall << " ]  " ;
		ofs << "F-Measure[ " << str_f_measure << " ]  " << endl ;

		//����F�l
		average_f_measure += 2 * precision * recall / ( precision + recall ) ;
	}

	//����F�l�o��
	ofs << "average_f_measure" << average_f_measure / ( float )cnt << endl << endl ;

	//�t�@�C���X�g���[���I��
	ofs.close( ) ;
}

