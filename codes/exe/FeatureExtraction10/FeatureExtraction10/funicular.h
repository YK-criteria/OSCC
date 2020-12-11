#pragma once

#include "pbm.h"
#include "basefeature.h"

class Funicular : public BaseFeature
{
public :
	Funicular( ) ;	//�R���X�g���N�^
	Funicular( Pbm image ) ;	//�摜�t�@�C���������p�R���X�g���N�^
	Funicular( const Funicular &funicular ) ;	//�R�s�[�R���X�g���N�^
	~Funicular( ) ;	//�f�X�g���N�^

	Funicular& operator=( const Funicular& funicular ) ;	//������Z�q�I�[�o�[���[�h

	virtual void Extract( ) ;	//���o�֐��@�e�N���X�̏������z�֐��̎���
	void Expand( ) ;	//�c������
	void Labeling( ) ;	//���x�����O����
	void Count( ) ;	//�J�E���g
	void MakeList( ) ;	//����f�̈�̖ʐςƋ��E���̒��������X�g��
	float Max( ) ;

	Pbm original ;	//�摜�f�[�^
	int **index ;	//���x�����O�p�C���f�b�N�X�z��
	
	int list_length ;	//���X�g�̒���
	int list_count ;	//���X�g�Ɋ܂܂��index�̐�
	int *label_list ;	//�̈�ԍ����X�g
	int *area_list ;	//�̈�ʐσ��X�g
	int *border_list ;	//���E���̒������X�g
	
	//�p�����[�^
	const int dim ;
	const int increase ;	//1��̃��X�g������
	float threshold ;	//����������f�̈�Ƃ��Đ�����Ƃ��̖ʐς�臒l
} ;
