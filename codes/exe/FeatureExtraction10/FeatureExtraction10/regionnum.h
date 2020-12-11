#pragma once

#include "pbm.h"
#include "basefeature.h"
#include <vector>

class RegionNum : public BaseFeature
{
public :
	RegionNum( ) ;	//�R���X�g���N�^
	RegionNum( Pbm image ) ;	//�摜�t�@�C���������p�R���X�g���N�^
	RegionNum( const RegionNum &regionnum ) ;	//�R�s�[�R���X�g���N�^
	~RegionNum( ) ;	//�f�X�g���N�^

	RegionNum& operator=( const RegionNum& regionnum ) ;	//������Z�q�I�[�o�[���[�h

	virtual void Extract( ) ;	//���o�֐��@�e�N���X�̏������z�֐��̎���
	void Expand( ) ;	//�c������
	void Labeling( ) ;	//���x�����O����
	int Count( ) ;	//�J�E���g

	Pbm original ;	//�摜�f�[�^
	Pbm expansion ;	//����f�c���摜
	int **index ;	//���x�����O�p�C���f�b�N�X�z��

	//�p�����[�^
	const int dim ;
	const int exp_steps ;
} ;
