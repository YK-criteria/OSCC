//pbm.h

#pragma once

#include <cstdlib>

#include <iostream>
#include <string>
#include <fstream>
using namespace std ;

#define MAX_COMMENT_SIZE 1024

class Ppm
{
public :
	Ppm( ) ;	//�R���X�g���N�^
	Ppm( string filename ) ;	//���O�w��R���X�g���N�^
	Ppm( int height , int width ) ;	//�傫���w��R���X�g���N�^
	Ppm( const Ppm &ppm ) ;	//�R�s�[�R���X�g���N�^
	~Ppm( ) ;	//�f�X�g���N�^
	Ppm& operator=( const Ppm& ppm ) ;	//������Z�q�I�[�o�[���[�h

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
	int ***data ;	//�f�[�^
} ;
