#include "main.h"

int main( int argc , char *argv[ ] )
{
	//�����`�F�b�N
	if( argc != 2 )
	{
		cout << "input 'execute file' 'initilize file'" << endl ;
		exit( EXIT_FAILURE ) ;
	}

	//�t�@�C�����w�肵�ă����_���t�H���X�g�N���X��������
	RandomForest rf( argv[ 1 ] ) ;

	//�������Ɋ�Â��ČP��
	rf.Train( ) ;

	//�e�X�g�ƃe�X�g���ʂ�\��
	rf.Test( ) ;
 }

