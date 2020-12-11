#pragma once

//�摜�t�@�C���ǂݍ��݃w�b�_�[
#include "pbm.h"

//�ʂ̓������o�@�w�b�_�[
#include "area.h"
#include "curvature.h"
#include "direction.h"
#include "jaggy.h"
#include "funicular.h"
#include "length.h"
#include "regionnum.h"

class Extractor
{
public:
	Extractor( string filename ) ;	//�J���摜�t�@�C���w��p�R���X�g���N�^
	~Extractor( ) ;	//�f�X�g���N�^

	void Extract( ) ;	//�������o
	void Output( ) ;	//�o��

	Pbm input ;	//�摜�t�@�C��

	Area area ;	//���̈�̖ʐςɊւ���������o�@
	Curvature curvature ;	//���̈�̋ȗ��Ɋւ���������o�@
	Direction direction ;	//�ˋN�̕����Ɋւ���������o�@
	Jaggy jaggy ;	//�M�U�M�U
	Funicular funicular ;	//���̈�̍��󂳂Ɋւ���������o�@
	Length length ;	//���E���̒����Ɋւ���������o�@
	RegionNum regionnum ;	//���̈�̐��Ɋւ���������o�@
} ;