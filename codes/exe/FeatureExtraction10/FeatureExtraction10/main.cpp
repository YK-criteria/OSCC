#include "main.h"

int main( int argc , char *argv[ ] )
{
	//�����G���[�`�F�b�N
	if( argc != 2 )
	{
		printf( "command '�`.exe' '�`.pbm'\n" ) ;
		exit( EXIT_FAILURE ) ;
	}

	//�������o�@�̏������@�������o���s���t�@�C�����������Ɏ��
	Extractor extractor( argv[ 1 ] ) ;

	//�������o
	extractor.Extract( ) ;

	//���ʏo��
	extractor.Output( ) ;

	return 0 ;
}