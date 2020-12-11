#include "node.h"

Node::Node( )
{

}

Node::Node( Config config )
{
	this->config = config ;
}

map< string , int > Node::Train( Subset subset , int depth )
{
	//����ȏ�[���؂𐬒������Ȃ�����I��
	if( config.max_depth < depth )
	{
		//�T�u�Z�b�g����ł������N���X�����̃m�[�h�̃N���X�ɐݒ肷��
		classname = GetClassName( subset ) ;

		//�I�������ݒ�
		map< string , int > mp ;
		mp[ "max_depth" ]++ ;

		return mp ;
	}

	//�T�u�Z�b�g�����������邩��I��
	if( ( signed )subset.datapoint.size( ) <= config.min_subset_size )
	{
		//�T�u�Z�b�g����ł������N���X�����̃m�[�h�̃N���X�ɐݒ肷��
		classname = GetClassName( subset ) ;

		//�I�������ݒ�
		map< string , int > mp ;
		mp[ "min_subset_size" ]++ ;

		return mp ;
	}

	//�p�����[�^�͈̔͂ōŗǂ̕����֐���T��
	Split best ;
	float max_informationgain = -1.f ;
	for( int i = 0 ; i < config.normal_num ; i++ )
	{
		Split candidate ;	//�������

		//�@���x�N�g���������_���ɐ���
		candidate.normal = MakeNormal( subset ) ;

		for( int j = 0 ; j < config.threshold_num ; j++ )
		{
			//��������臒l
			candidate.threshold = MakeThreshold( subset , candidate.normal ) ;

			float informationgain = CalcInformationGain( subset , candidate ) ;
			//���܂łōł���񗘓�������
			if( max_informationgain < informationgain )
			{
				//�ŗǂ̕����֐����X�V
				best = candidate ;

				//�ő��񗘓����X�V
				max_informationgain = informationgain ;
			}
		}
	}
	
	//�ŏ���񗘓���������Ă��邩�番�����Ȃ�
	if( max_informationgain < config.min_informationgain )
	{
		//�T�u�Z�b�g����ł������N���X�����̃m�[�h�̃N���X�ɐݒ肷��
		classname = GetClassName( subset ) ;

		//�I�������ݒ�
		map< string , int > mp ;
		mp[ "min_informationgain" ]++ ;

		return mp ;
	}

	//���̍ŗǂ̕��������̃m�[�h�̕����֐��ɐݒ�
	split = best ;

	//�T�u�Z�b�g�𕪊�
	Subset left_subset , right_subset ;
	left_subset = SeparateSubset( subset , true ) ;
	right_subset = SeparateSubset( subset , false ) ;

	//�q�m�[�h���C���X�^���X��
	Node left_node( config ) ;
	Node right_node( config ) ;
	
	//�q�m�[�h�P��
	map< string , int > left_mp = left_node.Train( left_subset, depth + 1 ) ;
	map< string , int > right_mp = right_node.Train( right_subset, depth + 1 ) ;

	child.push_back( left_node ) ;
	child.push_back( right_node ) ;

	//map�̓���
	map< string , int > mp ;
	//���̎q���瓾��ꂽmap
	for( auto itr = left_mp.begin( ) ; itr != left_mp.end( ) ; itr++ )
	{
		mp[ itr->first ] += itr->second ;
	}
	//�E�̎q���瓾��ꂽmap
	for( auto itr = right_mp.begin( ) ; itr != right_mp.end( ) ; itr++ )
	{
		mp[ itr->first ] += itr->second ;
	}

	return mp ;
}

string Node::GetClassName( Subset subset )
{
	map< string , int > mp ;	//�N���X���Əo���񐔂̘A�z�z��

	//�N���X���̏o���񐔂𓝍�
	for( unsigned int i = 0 ; i < subset.datapoint.size( ) ; i++ )
	{
		mp[ subset.datapoint[ i ].classname ]++ ;
	}

	string max_classname ;	//�ŕp�o�̃N���X��
	int max_cnt = 0 ;	//�ŕp�o�̃N���X�̏o����
	for( auto itr = mp.begin( ) ; itr != mp.end( ) ; itr++ )
	{
		//���܂łōł��ŕp�o�ȃN���X�𔭌�
		if( max_cnt < itr->second )
		{
			//�ŕp�o�N���X�̍X�V
			max_classname = itr->first ;
			max_cnt = itr->second ;
		}
	}

	return max_classname ;
}

vector< float > Node::MakeNormal( Subset subset )
{
	vector< float > v ;

	//���؂�
	if( config.split_type == SPLITTYPE_AXIS )
	{
		int r = rand( ) % config.dimension_num ;

		//�O����0���l�߂�
		for( int i = 0 ; i < r ; i++ )
		{
			v.push_back( 0.f ) ;
		}

		//-1�`+1�̐��l�����蓖��
		float f = RandFloat( -1.f , 1.f ) ;
		v.push_back( f ) ;

		//�㔼��0���l�߂�
		for( int i = r + 1 ; i < config.dimension_num ; i++ )
		{
			v.push_back( 0.f ) ;
		}
	}
	else if( config.split_type == SPLITTYPE_LINEAR )
	{
		cout << "linear split is nt inplemented." << endl ;
		exit( EXIT_FAILURE ) ;
	}

	return v ;
}

float Node::RandFloat( float min , float max  )
{
	return min + ( float )rand( ) / ( float )RAND_MAX * ( max - min ) ; 
}

float Node::MakeThreshold( Subset subset ,  vector< float > normal )
{
	vector< float > dot ;

	//�T�u�Z�b�g���̃f�[�^�_�Ɩ@���x�N�g���Ƃ̓��ς��v�Z
	for( unsigned int i = 0 ; i < subset.datapoint.size( ) ; i++ )
	{
		dot.push_back( Dot( normal , subset.datapoint[ i ] ) ) ;
	}

	//�ŏ��l�ƍő�l��T��
	float min = dot[ 0 ] ;
	float max = dot[ 0 ] ;
	for( unsigned int i = 1 ; i < dot.size( ) ; i++ )
	{
		//��菬�������ς𔭌�
		if( dot[ i ] < min )
		{
			min = dot[ i ] ;
		}

		if( max < dot[ i ] )
		{
			max = dot[ i ] ;
		}
	}

	return RandFloat( min + 0.0001f , max - 0.0001f ) ;
}

float Node::CalcInformationGain( Subset subset , Split split )
{
	//�����O�̊e�N���X�̏o���񐔂��W�v
	map< string , int > pre_mp ;
	for( unsigned int i = 0 ; i < subset.datapoint.size( ) ; i++ )
	{
		pre_mp[ subset.datapoint[ i ].classname ]++ ;
	}

	//�����O�̃G���g���s�[
	float pre_etp = 0.f ;
	//�����O�̃G���g���s�[�v�Z
	for( auto itr = pre_mp.begin( ) ; itr != pre_mp.end( ) ; itr++ )
	{
		float p = ( float )itr->second / ( float )subset.datapoint.size( ) ;
		pre_etp += - p * Log2( p ) ;
	}

	//������̂��ꂼ��̃f�[�^�_
	map< string , int > left_mp , right_mp ;
	int left_cnt = 0 ,  right_cnt = 0 ;
	//�����y�ѕ�����̂��ꂼ��̃O���[�v�̊e�N���X�̏o���񐔂��W�v
	for( unsigned int i = 0 ; i < subset.datapoint.size( ) ; i++ )
	{
		//�f�[�^�_�̓��ό��ʂ�臒l����
		if( Dot( split.normal , subset.datapoint[ i ] ) < split.threshold )
		{
			left_mp[ subset.datapoint[ i ].classname ]++ ;
			left_cnt++ ;
		}
		//臒l�ȏ�
		else
		{
			right_mp[ subset.datapoint[ i ].classname ]++ ;
			right_cnt++ ;
		}
	}

	//������̃G���g���s�[
	float post_etp = 0.f ;
	//������̍����̃G���g���s�[
	for( auto itr = left_mp.begin( ) ; itr != left_mp.end( ) ; itr++ )
	{
		float p = left_cnt == 0 ? 0 : ( float )itr->second / ( float )left_cnt ;
		post_etp += - p * Log2( p ) * ( ( float )left_cnt / ( float )subset.datapoint.size( ) );
	}
	//������̉E���̃G���g���s�[
	for( auto itr = right_mp.begin( ) ; itr != right_mp.end( ) ; itr++ )
	{
		float p = right_cnt == 0 ? 0 : ( float )itr->second / ( float )right_cnt ;
		post_etp += - p * Log2( p ) * ( ( float )right_cnt / ( float )subset.datapoint.size( ) );
	}

	return pre_etp - post_etp ;
}

float Node::Log2( float x )
{
	return x == 0.f ? 0.f : log( x ) /  log( 2.f ) ;
}

Subset Node::SeparateSubset( Subset subset , bool is_lower )
{
	Subset new_subset ;

	for( unsigned int i = 0 ; i < subset.datapoint.size( ) ; i++ )
	{
		//�������t���O�����ς��������Ȃ�V�����T�u�Z�b�g�ɒǉ� �t���R��
		if( is_lower && Dot( split.normal , subset.datapoint[ i ] ) < split.threshold ||
			!is_lower && split.threshold <= Dot( split.normal , subset.datapoint[ i ] ) )
		{
			new_subset.datapoint.push_back( subset.datapoint[ i ] ) ;
		}
	}

	return new_subset ;
}

float Node::Dot( vector< float > normal , DataPoint datapoint )
{
	//�@���x�N�g���̎������ƃf�[�^�_�̎��������قȂ� �@�O�̂��ߊm�F���Ă���
	if( normal.size( ) != datapoint.feature.size( ) )
	{
		cout << "in Dot , dimension is differ.\n" << endl ;
		exit( EXIT_FAILURE ) ;
	}

	//���όv�Z
	float dot = 0.f ;
	for( unsigned int i = 0 ; i < normal.size( ) ; i++ )
	{
		dot += normal[ i ] * datapoint.feature[ i ] ;
	}

	return dot ;
}

string Node::Test( DataPoint datapoint )
{
	//�q�m�[�h���Ȃ�
	if( child.size( ) == 0 )
	{	
		return classname ;
	}
	else
	{
		//�����֐��ɂ�镪����臒l���������
		if( Dot( split.normal , datapoint ) < split.threshold )
		{
			return child[ 0 ].Test( datapoint )  ;
		}
		else
		{
			return child[ 1 ].Test( datapoint ) ;
		}
	}
}

void Node::Print( int depth )
{
	for( int i = 0 ; i < depth ; i++ )
	{
		cout << " " << endl ;
	}
	cout << "." << " classname is " << classname ;

	if( child.size( ) != 0 )
	{
		child[ 0 ].Print( depth + 1 ) ;
		child[ 1 ].Print( depth + 1 ) ;
	}
}