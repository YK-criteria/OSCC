#include "area.h"

Area::Area( ) : dim( 1 ) , increase( 10 ) , threshold( 0.001 )
{
	//�e�N���X�̕ϐ��ݒ�
	dimension = dim ;
	feature = new float[ dimension ]( ) ;
	
	//�f�[�^�Ȃ�
	index = NULL ;
}

Area::Area( Pbm image ) : dim( 1 ) , increase( 10 ) , threshold( 0.001 )
{
	//�e�N���X�̕ϐ��ݒ�
	dimension = dim ;
	feature = new float[ dimension ]( ) ;
	
	//image����t�@�C���������ύX
	original = image ;
	original.filename = "original.pbm" ;	

	index = new int*[ image.height ]( ) ;
	//�摜����index������
	for( int i = 0 ; i < image.height ; i++ )
	{
		index[ i ] = new int[ image.width ]( ) ;

		for( int j = 0 ; j < image.width ; j++ )
		{
			//��f�l�R�s�[
			original.data[ i ][ j ] = image.data[ i ][ j ] ;

			//����f
			if( original.data[ i ][ j ] == BLACK )
			{
				//�ʂ��ԍ��ŏ�����
				index[ i ][ j ] = i * image.width + j ;
			}
			//����f
			else
			{
				//�ő�ԍ��ŏ������@���x���`�d�Ɋ֌W�Ȃ��Ȃ�ǂ�Ȑ��ł��悢
				index[ i ][ j ] = image.height * image.width ;
			}
		}
	}
}

Area::Area( const Area &area ) : dim( 1 ) , increase( 10 ) , threshold( 0.001 )
{
	//�e�N���X�ŏ������ق������������H
	dimension = area.dimension ;
	feature = new float[ dimension ]( ) ;
	for( int i = 0 ; i < dimension ; i++ )
	{
		feature[ i ] = area.feature[ i ] ;
	}

	//�C���[�W�R�s�[
	original = area.original ;

	//index�z��̓��e�R�s�[
	if( !area.index )
	{
		index = NULL ;
	}
	else
	{
		//���������蓖��
		index = new int*[ original.height ]( ) ;
		for( int i = 0 ; i < original.height ; i++ )
		{
			index[ i ] = new int[ original.width ]( ) ;
		}

		//���e���R�s�[
		for( int i = 0 ; i < original.height ; i++ )
		{
			for( int j = 0 ; i < original.width ; j++ )
			{
				index[ i ][ j ] = area.index[ i ][ j ] ;
			}
		}
	}
}

Area::~Area( )
{
	if( index )
	{
		for( int i = 0 ; i < original.height ; i++ )
		{
			if( index[ i ] )
			{
				delete[ ] index[ i ] ;
				index[ i ] = NULL ;
			}
		}
			delete[ ] index ;
		index = NULL ;
	}
}

Area& Area::operator=( const Area& area )
{
	//�e�N���X�ŏ������ق������������H
	dimension = area.dimension ;
	delete feature ;
	feature = new float[ dimension ] ;
	for( int i = 0 ; i < dimension ; i++ )
	{
		feature[ i ] = area.feature[ i ] ;
	}

	//�C���f�b�N�X�z�񂪂Ȃ�
	if( !area.index )
	{
		original = area.original ;
		index = NULL ;
	}
	else
	{
		//��U���݂̔z���j��
		for( int i = 0 ; i < original.height ; i++ )
		{
			delete[ ] index[ i ] ;
		}
		delete[ ] index ;

		//�ϐ��̒l���R�s�[
		original = area.original ;

		//�z��̎�蒼��
		index = new int*[ original.height ]( ) ;
		for( int i = 0 ; i < original.height ; i++ )
		{
			index[ i ] = new int[ original.width ]( ) ;
		}

		//�z��̓��e���R�s�[
		for( int i = 0 ; i < original.height ; i++ )
		{
			for( int j = 0 ; j < original.width ; j++ )
			{
				index[ i ][ j ] = area.index[ i ][ j ] ;
			}
		}
	}

	return *this ;
}

void Area::Extract( )
{
	//////////////////////////////////////////////////////////////
	//	���x�����O
	//////////////////////////////////////////////////////////////	
	Labeling( ) ;
	/*for( int i = 0 ; i < expansion.height ; i++ )
	{
		for( int j = 0 ; j < expansion.width ; j++ )
		{
			printf( "%5d " , index[ i ][ j ] ) ;
		}
		cout << endl ;
	}
	cout << endl ;*/

	//////////////////////////////////////////////////////////////
	//	����f�̈搔�J�E���g
	//////////////////////////////////////////////////////////////
	Count( ) ;

	//////////////////////////////////////////////////////////////
	//	����f�̈�̖ʐς����X�g��
	//////////////////////////////////////////////////////////////
	MakeList( ) ;

	//////////////////////////////////////////////////////////////
	//�@臒l�𖞂�������f�̈���J�E���g
	//////////////////////////////////////////////////////////////
	int countsmall = CountSmall( ) ;

	//////////////////////////////////////////////////////////////
	//	�����ʌ���
	//////////////////////////////////////////////////////////////
	feature[ 0 ] = countsmall ;
}

void Area::Labeling( )
{
	for( bool changed = true ; changed ; )
	{
		//��U�X�V�Ȃ��ɂ���
		changed = false ;

		for( int i = 0 ; i < original.height ; i++ )
		{
			for( int j = 0 ; j < original.width ; j++ )
			{
				//��ɃA�N�Z�X�ł���
				if( 0 <= i - 1 )
				{
					//���ڂ��Ă����f��index�����index�̂ق����������������g������f
					if( index[ i - 1 ][ j ] < index[ i ][ j ] && original.data[ i ][ j ] == BLACK )
					{
						index[ i ][ j ] = index[ i - 1 ][ j ] ;
						changed = true ;
					}
				}

				//���ɃA�N�Z�X�ł���
				if( i + 1 < original.height )
				{
					//���ڂ��Ă����f��index��艺��index�̂ق����������������g������f
					if( index[ i + 1 ][ j ] < index[ i ][ j ] && original.data[ i ][ j ] == BLACK )
					{
						index[ i ][ j ] = index[ i + 1 ][ j ] ;
						changed = true ;
					}
				}

				//���ɃA�N�Z�X�ł���
				if( 0 <= j - 1 )
				{
					//���ڂ��Ă����f��index��荶��index�̂ق����������������g������f
					if( index[ i ][ j - 1 ] < index[ i ][ j ] && original.data[ i ][ j ] == BLACK )
					{
						index[ i ][ j ] = index[ i ][ j - 1 ] ;
						changed = true ;
					}
				}

				//�E�ɃA�N�Z�X�ł���
				if( j + 1 < original.width )
				{
					//���ڂ��Ă����f��index���E��index�̂ق����������������g������f
					if( index[ i ][ j + 1 ] < index[ i ][ j ] && original.data[ i ][ j ] == BLACK )
					{
						index[ i ][ j ] = index[ i ][ j + 1 ] ;
						changed = true ;
					}
				}
			}
		}
	}
}

void Area::Count( )
{
	list_length = increase ;	//���X�g�̒���
	list_count = 0 ;	//���X�g�Ɋ܂܂��index�̐�
	label_list = new int[ increase ]( ) ;	//���X�g

	for( int i = 0 ; i < original.height ; i++ )
	{
		for( int j = 0 ; j < original.width ; j++ )
		{
			//���x�����ő��f�ԍ��ȊO
			if( index[ i ][ j ] != original.height * original.width )
			{
				bool exist = false ;	//���X�g�Ɉ�v����ԍ������邩�t���O

				//���X�g�ɂ��̔ԍ������邩�T��
				for( int k = 0 ; k < list_count ; k++ )
				{
					//���X�g�̔ԍ��ƍ����ڂ��Ă���ԍ�����v
					if( label_list[ k ] == index[ i ][ j ] )
					{
						exist = true ;
					}
				}

				//���X�g�ɂȂ�
				if( !exist )
				{
					//���X�g�̒����ƒ��ɓ����Ă��鐔�������Œ���������Ȃ�
					if( list_count == list_length )
					{
						//�V�������X�g
						int new_list_length = list_length + increase ;
						int *new_list = new int[ new_list_length ]( ) ;

						//�Â����X�g�̒��̂��̂��R�s�[
						for( int k = 0 ; k < list_length ; k++ )
						{
							new_list[ k ] = label_list[ k ] ;
						}

						//�Â����X�g��j��
						delete[ ] label_list ;

						//�V�������̂ɍX�V
						label_list = new_list ;
						list_length = new_list_length ;
					}

					//���X�g�ɒǉ�
					label_list[ list_count ] = index[ i ][ j ] ;
					//���X�g���̐������Z
					list_count++ ;
				}
			}
		}
	}
}

void Area::MakeList( )
{
	//�̈�ʐσ��X�g�m��
	area_list = new int[ list_count ]( ) ;

	//�̈�ʐσ��X�g��
	for( int i = 0 ; i < original.height ; i++ )
	{
		for( int j = 0 ; j < original.width  ; j++ )
		{
			bool exist = false ;

			//���X�g�̒��ɍ����ڂ��Ă��闹�̈�ԍ������邩�T��
			for( int k = 0 ; k < list_count ; k++ )
			{
				//�̈�ԍ��𔭌�
				if( index[ i ][ j ] == label_list[ k ] )
				{
					area_list[ k ]++ ;
					break ;
				}
			}
		}
	}
}

int Area::CountSmall( )
{
	int count = 0 ;	//�̈搔�J�E���^

	for( int i = 0 ; i < list_count ; i++ )
	{
		//臒l�𖞂����Ȃ�J�E���^����
		if( ( double )area_list[ i ] / ( double )( original.height * original.width ) < threshold )
		{
			count++ ;
		}
	}

	return count ;
}