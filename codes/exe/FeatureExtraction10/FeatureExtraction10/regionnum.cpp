#include "regionnum.h"

RegionNum::RegionNum( ) : dim ( 1 ) , exp_steps( 3 )
{
	//�e�N���X�̕ϐ��ݒ�
	dimension = dim ;
	feature = new float[ dimension ]( ) ;
	
	//�f�[�^�Ȃ�
	index = NULL ;
}

RegionNum::RegionNum( Pbm image ) : dim ( 1 ) , exp_steps( 3 )
{
	//�e�N���X�̕ϐ��ݒ�
	dimension = dim ;
	feature = new float[ dimension ]( ) ;
	
	//image����t�@�C���������ύX
	original = image ;
	original.filename = "original.pbm" ;	

	//�傫���ƃt�@�C�������w��
	Pbm tmp( original.height , original.width ) ;
	expansion = tmp ;
	expansion.filename = "expansion.pbm" ;

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

RegionNum::RegionNum( const RegionNum &regionnum ) : dim ( 1 ) , exp_steps( 3 )
{
	//�e�N���X�ŏ������ق������������H
	dimension = regionnum.dimension ;
	feature = new float[ dimension ]( ) ;
	for( int i = 0 ; i < dimension ; i++ )
	{
		feature[ i ] = regionnum.feature[ i ] ;
	}

	//�C���[�W�R�s�[
	original = regionnum.original ;
	expansion = regionnum.expansion ;

	//index�z��̓��e�R�s�[
	if( !regionnum.index )
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
				index[ i ][ j ] = regionnum.index[ i ][ j ] ;
			}
		}
	}
}

RegionNum::~RegionNum( )
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

RegionNum& RegionNum::operator=( const RegionNum& regionnum )
{
	//�e�N���X�ŏ������ق������������H
	dimension = regionnum.dimension ;
	delete feature ;
	feature = new float[ dimension ] ;
	for( int i = 0 ; i < dimension ; i++ )
	{
		feature[ i ] = regionnum.feature[ i ] ;
	}

	//�C���f�b�N�X�z�񂪂Ȃ�
	if( !regionnum.index )
	{
		original = regionnum.original ;
		expansion = regionnum.expansion ;
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
		original = regionnum.original ;
		expansion = regionnum.expansion ;

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
				index[ i ][ j ] = regionnum.index[ i ][ j ] ;
			}
		}
	}

	return *this ;
}

void RegionNum::Extract( )
{
	//////////////////////////////////////////////////////////////
	//	����f�c��
	//////////////////////////////////////////////////////////////	
	for( int i = 0 ; i < exp_steps ; i++ )
	{
		Expand( ) ;
	}
	//�c���摜�o��
	expansion.Save( ) ;

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
	//	���̈搔�J�E���g
	//////////////////////////////////////////////////////////////
	int count = Count( ) ;

	//////////////////////////////////////////////////////////////
	//	�����ʌ���
	//////////////////////////////////////////////////////////////
	feature[ 0 ] = count ;
}

void RegionNum::Expand( )
{
	for( int i = 0 ; i < original.height ; i++ )
	{
		for( int j = 0 ; j < original.width ; j++ )
		{
			//��U���̉�f�l�Ɠ������̂���
			expansion.data[ i ][ j ] = original.data[ i ][ j ] ;

			//��ɃA�N�Z�X�ł���
			if( 0 <= i - 1 )
			{
				//�オ����f
				if( original.data[ i - 1 ][ j ] == BLACK )
				{
					expansion.data[ i ][ j ] = BLACK ;
				}
			}

			//���ɃA�N�Z�X�ł���
			if( i + 1 < original.height )
			{
				//��������f
				if( original.data[ i + 1 ][ j ] == BLACK )
				{
					expansion.data[ i ][ j ] = BLACK ;
				}
			}

			//���ɃA�N�Z�X�ł���
			if( 0 <= j - 1 )
			{
				//��������f
				if( original.data[ i ][ j - 1 ] == BLACK )
				{
					expansion.data[ i ][ j ] = BLACK ;
				}
			}

			//�E�ɃA�N�Z�X�ł���
			if( j + 1 < original.width )
			{
				if( original.data[ i ][ j + 1 ] == BLACK )
				{
					expansion.data[ i ][ j ] = BLACK ;
				}
			}
		}
	}
}

void RegionNum::Labeling( )
{
	for( bool changed = true ; changed ; )
	{
		//��U�X�V�Ȃ��ɂ���
		changed = false ;

		for( int i = 0 ; i < expansion.height ; i++ )
		{
			for( int j = 0 ; j < expansion.width ; j++ )
			{
				//��ɃA�N�Z�X�ł���
				if( 0 <= i - 1 )
				{
					//���ڂ��Ă����f��index�����index�̂ق����������������g������f
					if( index[ i - 1 ][ j ] < index[ i ][ j ] && expansion.data[ i ][ j ] == BLACK )
					{
						index[ i ][ j ] = index[ i - 1 ][ j ] ;
						changed = true ;
					}
				}

				//���ɃA�N�Z�X�ł���
				if( i + 1 < expansion.height )
				{
					//���ڂ��Ă����f��index��艺��index�̂ق����������������g������f
					if( index[ i + 1 ][ j ] < index[ i ][ j ] && expansion.data[ i ][ j ] == BLACK )
					{
						index[ i ][ j ] = index[ i + 1 ][ j ] ;
						changed = true ;
					}
				}

				//���ɃA�N�Z�X�ł���
				if( 0 <= j - 1 )
				{
					//���ڂ��Ă����f��index��荶��index�̂ق����������������g������f
					if( index[ i ][ j - 1 ] < index[ i ][ j ] && expansion.data[ i ][ j ] == BLACK )
					{
						index[ i ][ j ] = index[ i ][ j - 1 ] ;
						changed = true ;
					}
				}

				//�E�ɃA�N�Z�X�ł���
				if( j + 1 < expansion.width )
				{
					//���ڂ��Ă����f��index���E��index�̂ق����������������g������f
					if( index[ i ][ j + 1 ] < index[ i ][ j ] && expansion.data[ i ][ j ] == BLACK )
					{
						index[ i ][ j ] = index[ i ][ j + 1 ] ;
						changed = true ;
					}
				}
			}
		}
	}
}

int RegionNum::Count( )
{
	const int increase = 10 ;	//1��̃��X�g������
	int list_length = increase ;	//���X�g�̒���
	int list_count = 0 ;	//���X�g�Ɋ܂܂��index�̐�
	int *label_list = new int[ increase ]( ) ;	//���X�g
	int all_count = list_count ;	//
	int out_count = 0 ;

	for( int i = 0 ; i < expansion.height ; i++ )
	{
		for( int j = 0 ; j < expansion.width ; j++ )
		{
			//���x�����ő��f�ԍ��ȊO
			if( index[ i ][ j ] != expansion.height * expansion.width )
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

	//�S�Ă̍���f�̏W�܂�̐�
	all_count = list_count ;

	//���X�g�̓��e�����ǂ�
	list_count = 0 ;

	for( int i = 0 ; i < expansion.height ; i++ )
	{
		for( int j = 0 ; j < expansion.width ; j++ )
		{
			//�O�g�̍���f
			if( expansion.data[ i ][ j ] == BLACK && ( i == 0 || i == expansion.height - 1 || j == 0 || j == expansion.width - 1 ) )
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
					//�O�g�ɍ���f�͌���I������S�Ă̍���f��index��o�^�ł���label_list��
					//���X�g�̒���������Ȃ��Ȃ邱�Ƃ͖���					

					//���X�g�ɒǉ�
					label_list[ list_count ] = index[ i ][ j ] ;
					//���X�g���̐������Z
					list_count++ ;
				}
			}
		}
	}

	//�O�g�ɂ͂ݏo�Ă��鍕��f�̏W�܂�̐�
	out_count = list_count ;

	//�O�g��f�̔���f�̘A�������J�E���g
	vector< int > white_count ;
	int white = 0 ;
	//���ォ��E��
	for( int i = 0 ; i < original.width ; i++ )
	{
		if( original.data[ 0 ][ i ] == WHITE )
		{
			white++ ;
		}
		else	//if( original.data[ 0 ][ i ] == BLACK )
		{
			//����f����ۑ�
			if( 0 < white )
			{
				white_count.push_back( white ) ;
				cout << white << endl ;
			}
			white = 0 ;
			continue ;
		}
	}
	//�E�ォ��E��
	for( int i = 0 ; i < original.height ; i++ )
	{
		if( original.data[ i ][ original.width - 1 ] == WHITE )
		{
			white++ ;
		}
		else	//if( original.data[ i ][ original.width - 1 ] == BLACK )
		{
			//����f����ۑ�
			if( 0 < white )
			{
				white_count.push_back( white ) ;
			}
			white = 0 ;
			continue ;
		}
	}
	//�E�����獶��
	for( int i = original.width - 1 ; 0 <= i ; i-- )
	{
		if( original.data[ original.height - 1 ][ i ] == WHITE )
		{
			white++ ;
		}
		else	//if( original.data[ i ][ original.width - 1 ] == BLACK )
		{
			//����f����ۑ�
			if( 0 < white )
			{
				white_count.push_back( white ) ;
			}
			white = 0 ;
			continue ;
		}
	}
	//�������獶�� ����̉�f�͓���Ȃ�
	for( int i = original.height - 1 ; 0 < i ; i-- )
	{
		if( original.data[ i ][ 0 ] == WHITE )
		{
			white++ ;
		}
		else	//if( original.data[ i ][ original.width - 1 ] == BLACK )
		{
			//����f����ۑ�
			if( 0 < white )
			{
				white_count.push_back( white ) ;
			}
			white = 0 ;
			continue ;
		}
	}
	//�Ō�̉�f�����ōŏ��̉�f�����Ȃ�ŏ��̔���f���ɍŌ�̔���f����ǉ�
	if( original.data[ 0 ][ 0 ] == WHITE && original.data[ 1 ][ 0 ] == WHITE )
	{
		white_count[ 0 ] += white ;
	}
	//臒l�𖞂�������f���J�E���g
	int wc = 0;
	for( unsigned int i = 0 ; i < white_count.size( ) ; i++ )
	{
		//臒l�ȏ�̔���f�ȃJ�E���g
		if( 0.001f < ( float )white_count[ i ] / ( float )( original.height * original.width ) )
		{
			wc++ ;
		}
	}


	//���X�g�j��
	delete[ ] label_list ;

	//cout << all_count << endl ;
	//cout << out_count << endl ;

	//return all_count - out_count + 1 ;
	return all_count - out_count + wc ;
}