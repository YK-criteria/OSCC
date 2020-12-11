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
	map< string , int > mp ;	//�؂̏I�������ƏI�������̏o���񐔂̘A�z�z��

	cout << "training forest now..." << endl ;

	//�؂̖{�����܂킷
	for( int i = 0 ; i < config.tree_num ; i++ )
	{
		//�T�u�Z�b�g�������_���ɍ쐬
		Subset subset = MakeSubset( datapoint ) ;

		//�ꎞ�I�Ƀc���[����
		Tree tree( config ) ;
		
		cout << "[ " << i << " ]" ;

		//�c���[�P�� �����ɖ؂̌P���̏I��������A�z�z��Ɋi�[
		map< string , int > tmp = tree.Train( subset ) ;
		
		//���̖؂���A���Ă���A�z�z��̓��e��ǉ�

		//�P���ς݂̃c���[���i�[
		this->tree.push_back( tree ) ;
	}
	cout << endl ;

	return mp ;
}

string Forest::Test( DataPoint datapoint )
{
	map< string , int > mp ;	//�N���X���Əo�͐��̘A�z�z��
	
	//�e�X�g���ʂł���؂��瓾����N���X���̏o�͐������ꂼ��v�Z
	for( unsigned int i = 0 ; i < tree.size( ) ; i++ )
	{
		mp[ tree[ i ].Test( datapoint ) ]++ ;
		
	}

	string max_classname ;	//�ł��o�͐��̑����N���X�̃N���X��
	int max_cnt = -1 ;	//�ł��o�͐��������N���X�̏o�͐�

	//�A�z�z��ɓo�^����Ă���v�f��S�ĒT��
	for( auto itr = mp.begin( ) ; itr != mp.end( ) ; itr++ )
	{
		//���o�͐��̑����N���X�𔭌�
		if( max_cnt < itr->second )
		{
			//���o�͐��̑����N���X���X�V
			max_classname = itr->first ;
			max_cnt = itr->second ;
		}
	}

	return max_classname ;
}

Subset Forest::MakeSubset( vector< DataPoint > datapoint )
{
	Subset subset ;	//�쐬����T�u�Z�b�g�̃C���X�^���X

	//�쐬����T�u�Z�b�g�̃T�C�Y���܂킷
	for( int i = 0 ; i < config.subset_size ; i++ )
	{
		int r  = rand( ) % datapoint.size( ) ;	//�f�[�^�_�W���̂��������_���ɑI�΂ꂽ�_������킷�Y����

		//�d���`�F�b�N����
		if( config.select_type == SELECTTYPE_SINGLE )
		{
			bool is_single = true ;	//�d���Ȃ��t���O

			//���ݓo�^����Ă���T�u�Z�b�g���̑S�Ă̓_��ΏۂƂ��ĒT��
			for( unsigned int j = 0 ; j < subset.datapoint.size( ) ; j++ )
			{
				//�T�u�Z�b�g���ɓ����_�𔭌�
				if( subset.datapoint[ j ] == datapoint[ r ] )
				{
					//�d������ɐݒ�
					is_single = false ;

					//�T���I��
					break ;
				}
			}

			//�d���Ȃ�
			if( is_single )
			{
				//�T�u�Z�b�g�ɒǉ�
				subset.datapoint.push_back( datapoint[ r ] ) ;
			}
		}
		//�d���`�F�b�N�Ȃ�
		else if( config.select_type == SELECTTYPE_MULTIPLE )
		{
			//�������ɃT�u�Z�b�g�ɒǉ�
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