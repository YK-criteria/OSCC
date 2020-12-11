#pragma once

#include "point.h"
#include <iostream>

class Border
{
public :
	Border( ) ;	//�R���X�g���N�^
	Border( int length ) ;	//�R���X�g���N�^
	Border( const Border &border ) ;	//�R�s�[�R���X�g���N�^
	~Border( ) ;	//�f�X�g���N�^
	
	Border& operator=( const Border& border ) ;	//������Z�q�I�[�o�[���[�h

	void Add( int x , int y ) ;	//���W�ǉ�
	bool Exist( int x , int y ) ;	//�w����W�����邩

	int length ;	//���E���̒���
	int max_length ;	//point�z��ɓ����f�̍ő吔
	Point *point ;	//���E���̉�f
	bool island ;	//���t���O

	//�p�����[�^
	const int increase ;	//���X�g�g����
} ;
