#pragma once

#include <vector>

using namespace std ;

#include "datapoint.h"

class Subset
{
public :
	vector< DataPoint > datapoint ;	//�f�[�^�_

	Subset( ) ;	//�f�t�H���g�R���X�g���N�^
	Subset( vector< DataPoint > datapoint ) ;	//�R���X�g���N�^
	Subset( const Subset &subset ) ;	//�R�s�[�R���X�g���N�^
	Subset &operator=( const Subset &subset ) ;	//������Z�q�I�[�o�[���[�h
} ;