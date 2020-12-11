#pragma once

#include <string>
#include <vector>

using namespace std ;

class DataPoint
{
public :
	string classname ;	//�N���X��
	vector< float > feature ;	//������

	DataPoint( ) ;	//�f�t�H���g�R���X�g���N�^	
	DataPoint( string classname , vector< float > feature ) ;	//�R���X�g���N�^
	DataPoint( const DataPoint &datapoint ) ;	//�R�s�[�R���X�g���N�^
	DataPoint &operator=( const DataPoint &datapoint ) ;	//������Z�q�I�[�o�[���[�h
	bool operator==( const DataPoint &datapoint ) ;	//��r���Z�q�I�[�o�[���[�h
} ;
