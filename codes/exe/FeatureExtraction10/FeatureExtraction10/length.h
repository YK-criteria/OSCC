#pragma once

#include "pbm.h"
#include "basefeature.h"

class Length : public BaseFeature
{
public :
	Length( ) ;	//�R���X�g���N�^
	Length( Pbm image ) ;	//�摜�t�@�C���������p�R���X�g���N�^
	Length( const Length &length ) ;	//�R�s�[�R���X�g���N�^
	~Length( ) ;	//�f�X�g���N�^

	Length& operator=( const Length& length ) ;	//������Z�q�I�[�o�[���[�h

	virtual void Extract( ) ;	//���o�֐��@�e�N���X�̏������z�֐��̎���
	
	Pbm original ;	//�摜�f�[�^

	//�p�����[�^
	const int dim ;
} ;
