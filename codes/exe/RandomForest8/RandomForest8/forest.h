#pragma once

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
using namespace std ;

#include "config.h"
#include "datapoint.h"
#include "tree.h"
#include "subset.h"

class Forest
{
public :
	Config config ;	//�t�H���X�g�\�z�p�����[�^
	vector< Tree > tree ;	//�c���[

	Forest( ) ;	//�f�t�H���g�R���X�g���N�^
	Forest( Config config ) ;	//�R���X�g���N�^
	
	map< string , int > Train( vector< DataPoint > datapoint ) ;	//�P��
	Subset MakeSubset( vector< DataPoint > datapoint ) ;	//�T�u�Z�b�g�쐬
	string Test( DataPoint datapoint ) ;	//�e�X�g
} ;