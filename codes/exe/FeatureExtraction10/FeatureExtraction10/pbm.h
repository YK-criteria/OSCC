//pbm.h

#pragma once

#include <cstdlib>

#include <iostream>
#include <string>
#include <fstream>
using namespace std ;

#define MAX_COMMENT_SIZE 1024

enum PbmColor
{
	WHITE ,
	BLACK ,
} ;

class Pbm
{
public :
	Pbm( ) ;	//�R���X�g���N�^
	Pbm( string filename ) ;	//���O�w��R���X�g���N�^
	Pbm( int height , int width ) ;	//�傫���w��R���X�g���N�^
	Pbm( const Pbm &pbm ) ;	//�R�s�[�R���X�g���N�^
	~Pbm( ) ;	//�f�X�g���N�^
	Pbm& operator=( const Pbm& pbm ) ;	//������Z�q�I�[�o�[���[�h

	void Open( ) ;	//�t�@�C���I�[�v��
	void Open( string filename ) ;	//���O�w��t�@�C���I�[�v��
	void Save( ) ;	//�t�@�C���Z�[�u
	void Save( string filename ) ;	//���O�w��t�@�C���Z�[�u
	void Print( ) ;	//�\��

	string filename ;	//�t�@�C����
	string type ;	//P1�Œ肾���O�̂���
	string comment ;	//�R�����g
	int height ;	//�c��
	int width ;	//����
	PbmColor **data ;	//�f�[�^
} ;
