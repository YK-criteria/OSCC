#pragma once

#include <vector>

using namespace std ;

#include "config.h"

class Split
{
public :
	float threshold ;	//臒l
	vector< float > normal ;	//�@���x�N�g��
	
	Split( ) ;	//�f�t�H���g�R���X�g���N�^
	Split( vector< float > normal , float threshold ) ;	//�R���X�g���N�^
	Split( const Split &split ) ;	//�R�s�[�R���X�g���N�^
	Split &operator=( const Split &split ) ;	//������Z�q�I�[�o�[���[�h
} ;
