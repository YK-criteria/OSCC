#pragma once

#include <cstdio>

#include "pbm.h"

class BaseFeature
{
public :
	BaseFeature( ) ;	//�R���X�g���N�^
	BaseFeature( const BaseFeature &src ) ;	//�R�s�[�R���X�g���N�^
	~BaseFeature( ) ;	//�f�X�g���N�^
	
	virtual void Extract( ) = 0 ;	//�������o�֐��@�������z�֐�������h���N���X�ł͕K���������邱��
	void Output( ) ;	//�o�͊֐�

	int dimension ;	//����
	float *feature ;	//�����ʔz��
} ;