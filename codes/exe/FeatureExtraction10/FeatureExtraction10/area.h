#pragma once

#include "pbm.h"
#include "basefeature.h"

class Area : public BaseFeature
{
public :
	Area( ) ;	//�R���X�g���N�^
	Area( Pbm image ) ;	//�摜�t�@�C���������p�R���X�g���N�^
	Area( const Area &area ) ;	//�R�s�[�R���X�g���N�^
	~Area( ) ;	//�f�X�g���N�^

	Area& operator=( const Area& area ) ;	//������Z�q�I�[�o�[���[�h

	virtual void Extract( ) ;	//���o�֐��@�e�N���X�̏������z�֐��̎���
	void Expand( ) ;	//�c������
	void Labeling( ) ;	//���x�����O����
	void Count( ) ;	//�J�E���g
	void MakeList( ) ;	//����f�̈�̖ʐςƗ̈�ԍ������X�g��
	int CountSmall( ) ;	//臒l�ȉ��ƂȂ�̈�ʐς̂��̂��J�E���g

	Pbm original ;	//�摜�f�[�^
	int **index ;	//���x�����O�p�C���f�b�N�X�z��
	
	int list_length ;	//���X�g�̒���
	int list_count ;	//���X�g�Ɋ܂܂��index�̐�
	int *label_list ;	//�̈�ԍ����X�g
	int *area_list ;	//�̈�ʐσ��X�g
	
	//�p�����[�^
	const int dim ;
	const int increase ;	//1��̃��X�g������
	float threshold ;	//����������f�̈�Ƃ��Đ�����Ƃ��̖ʐς�臒l
} ;
