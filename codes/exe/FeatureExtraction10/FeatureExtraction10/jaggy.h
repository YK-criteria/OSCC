#pragma once

#include <cmath>

#include "pbm.h"
#include "basefeature.h"
#include "point.h"
#include "borderlist.h"
#include "enum_direction.h"
#include "ppm.h"

class Jaggy : public BaseFeature
{
public :
	Jaggy( ) ;	//�R���X�g���N�^
	Jaggy( Pbm image ) ;	//�摜�t�@�C���������p�R���X�g���N�^
	Jaggy( const Jaggy &jaggy ) ;	//�R�s�[�R���X�g���N�^
	~Jaggy( ) ;	//�f�X�g���N�^

	Jaggy& operator=( const Jaggy& jaggy ) ;	//������Z�q�I�[�o�[���[�h

	virtual void Extract( ) ;	//���o�֐��@�e�N���X�̏������z�֐��̎���
	void SetIsBorder( ) ;	//���E����f�̐ݒ�
	void Labeling( ) ;	//���E���̃��x�����O����
	void Calc( ) ;	//�M�U�M�U�v�Z
	float Radian( Point start , Point middle , Point end ) ;	//3�_��^�����Ƃ��ɂł���2�x�N�g���������p�x�i���W�A���j���v�Z
	void Devide( int num , int start , int end , int cnt ) ;	//���E������
	float Distance( Point p1 , Point p2 , Point p3 ) ;	//p1p2��ʂ钼���ɑ΂���p3�Ƃ̋���
	bool Tip( int size , Point point ) ;

	void Print( ) ;

	Pbm original ;	//�摜�f�[�^
	bool **is_border ;	//���E����\����f
	BorderList borderlist ;	//���E�����X�g

	int **index ;
	int **label ;


	//�p�����[�^
	const int dim ;
	const int increase ;	//1��̃��X�g������
	const float threshold ;	//

} ;
