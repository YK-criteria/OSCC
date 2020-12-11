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
	int class_num ;	//�N���X��
	int dimension_num ;	//������
	int select_dimension_num ;	//�I��������
	int tree_num ;	//�؂̖{��
	int max_depth ;	//�ő�[�x
	int normal_num ;	//�@���x�N�g���̌�␔
	int threshold_num ;	//臒l�̌�␔
	int train_num ;	//�P���f�[�^��
	int test_num ;	//�e�X�g�f�[�^��
	int subset_size ;	//�����T�u�Z�b�g�̃T�C�Y
	int min_subset_size ;	//�ŏ��T�u�Z�b�g�̃T�C�Y
	float min_informationgain ;	//�ŏ���񗘓�
	SplitType split_type ;	//�������@
	SelectType select_type ;	//�T�u�Z�b�g�쐬���̃f�[�^�_�I����@
	string train_filename ;	//�P���f�[�^�t�@�C���̃p�X
	string test_filename ;	//�e�X�g�f�[�^�t�@�C���̃p�X
	string result_filename ;	//�e�X�g���ʏo�̓t�@�C���̃p�X

	Config( ) ;	//�f�t�H���g�R���X�g���N�^
	//�x�N�g���̑������������R�s�[�R���X�g���N�^�Ƒ�����Z�q�I�[�o�[���[�h�͍��Ȃ��Ă��悢
} ;
