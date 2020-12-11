#include "length.h"

Length::Length( ) : dim( 1 )
{
	//�e�N���X�̕ϐ��ݒ�
	dimension = dim ;
	feature = new float[ dimension ]( ) ;
}

Length::Length( Pbm image ) : dim( 1 )
{
	//�e�N���X�̕ϐ��ݒ�
	dimension = dim ;
	feature = new float[ dimension ]( ) ;

	//���͉摜���R�s�[
	original = image ;
	original.filename = "original.pbm" ;
}

Length::Length( const Length &length ) : dim( 1 )
{
	//�e�N���X�ŏ������ق������������H
	dimension = length.dimension ;
	feature = new float[ dimension ]( ) ;
	for( int i = 0 ; i < dimension ; i++ )
	{
		feature[ i ] = length.feature[ i ] ;
	}

	//�C���[�W�R�s�[
	original = length.original ;
}

Length::~Length( )
{
	//�f�X�g���N�^�͏����Ȃ�
}

Length& Length::operator=( const Length& length )
{
	//�e�N���X�ŏ������ق������������H
	dimension = length.dimension ;
	delete feature ;
	feature = new float[ dimension ] ;
	for( int i = 0 ; i < dimension ; i++ )
	{
		feature[ i ] = length.feature[ i ] ;
	}

	original = length.original ;

	return *this ;
}

void Length::Extract( )
{
	int count = 0 ;	//���E����f��

	for( int i = 0 ; i < original.height ; i++ )
	{
		for( int j = 0 ; j < original.width ; j++ )
		{
			//�������g������f
			if( original.data[ i ][ j ] == BLACK )
			{
				bool border = false ;	//���E���t���O

				//��ɃA�N�Z�X�ł���
				if( 0 <= i - 1 )
				{
					//�オ����f
					if( original.data[ i - 1 ][ j ] == WHITE )
					{
						border = true ;
					}
				}

				//���ɃA�N�Z�X�ł���
				if( i + 1 < original.height )
				{
					//��������f
					if( original.data[ i + 1 ][ j ] == WHITE )
					{
						border = true ;
					}
				}

				//���ɃA�N�Z�X�ł���
				if( 0 <= j - 1 )
				{
					//��������f
					if( original.data[ i ][ j - 1 ] == WHITE )
					{
						border = true ;
					}
				}

				//�E�ɃA�N�Z�X�ł���
				if( j + 1 < original.width )
				{
					//�E������f
					if( original.data[ i ][ j + 1 ] == WHITE )
					{
						border = true ;
					}
				}

				if( border )
				{
					count++ ;
				}
			}
		}
	}

	feature[ 0 ] = count ;
}
