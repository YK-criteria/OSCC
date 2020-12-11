#pragma once

#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>

using namespace std ;

#include "datapoint.h"
#include "forest.h"

class RandomForest
{
public :
	int cnt_init ;	//Init���Ă΂ꂽ�񐔂��L�^ srand��1�񂵂��Ă΂Ȃ��悤�ɂ��邽��
	Config config ;	//�t�H���X�g�\�z�p�����[�^
	string filename ;	//�ݒ�t�@�C����
	Forest forest ;	//�t�H���X�g
	vector< DataPoint > traindata ;	//�P���f�[�^
	vector< DataPoint > testdata ;	//�e�X�g�f�[�^

	RandomForest( ) ;	//�f�t�H���g�R���X�g���N�^
	RandomForest( string filename ) ;	//�R���X�g���N�^
	
	void Init( ) ;	//����������
	void Init( string filename ) ;	//�t�@�C�����w�菉��������
	void Train( ) ;	//�P��
	void Test( ) ;	//�e�X�g
} ;