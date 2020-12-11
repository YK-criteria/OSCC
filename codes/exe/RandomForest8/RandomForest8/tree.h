#pragma once

#include <iostream>
#include <string>

using namespace std ;

#include "datapoint.h"
#include "subset.h"
#include "node.h"

class Tree
{
public :
	Config config ;	//�t�H���X�g�\�z�p�����[�^
	Node root ;	//���m�[�h
	
	Tree( ) ;	//�f�t�H���g�R���X�g���N�^
	Tree( Config config ) ;	//�R���X�g���N�^
	
	map< string , int > Train( Subset subset ) ;	//�P��
	string Test( DataPoint datapoint ) ;	//�e�X�g
} ;
