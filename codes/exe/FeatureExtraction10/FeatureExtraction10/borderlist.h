#pragma once

#include "border.h"

class BorderList
{
public :
	BorderList( ) ;	//�R���X�g���N�^
	void Add( ) ;	//���E���̐V�K�ǉ�
	bool Exist( int x , int y ) ;	//�w����W���܂ދ��E�������邩

	int num ;	//���E����
	int max_num ;	//border�z��ɓ��鋫�E���̍ő吔
	Border *border ;	//���E���z��

	//�p�����[�^
	const int increase ;
} ;
