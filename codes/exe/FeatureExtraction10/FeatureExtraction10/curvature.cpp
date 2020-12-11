#include "curvature.h"

Curvature::Curvature( ) : dim( 1 ) , increase( 10 ) , threshold( 3.f )
{
	//�e�N���X�̕ϐ��ݒ�
	dimension = dim ;
	feature = new float[ dimension ]( ) ;
}

Curvature::Curvature( Pbm image ) : dim( 1 ) , increase( 10 ) , threshold( 3.f )
{
	//�e�N���X�̕ϐ��ݒ�
	dimension = dim ;
	feature = new float[ dimension ]( ) ;
	
	//image����t�@�C���������ύX
	original = image ;
	original.filename = "original.pbm" ;
}

Curvature::Curvature( const Curvature &curvature ) : dim( 3 ) , increase( 10 ) , threshold( 3.f )
{
	//�e�N���X�ŏ������ق������������H
	dimension = curvature.dimension ;
	feature = new float[ dimension ]( ) ;
	for( int i = 0 ; i < dimension ; i++ )
	{
		feature[ i ] = curvature.feature[ i ] ;
	}

	//�C���[�W�R�s�[
	original = curvature.original ;
}

Curvature::~Curvature( )
{
	
}

Curvature& Curvature::operator=( const Curvature& curvature )
{
	//�e�N���X�ŏ������ق������������H
	dimension = curvature.dimension ;
	original = curvature.original ;
	delete feature ;
	feature = new float[ dimension ] ;
	for( int i = 0 ; i < dimension ; i++ )
	{
		feature[ i ] = curvature.feature[ i ] ;
	}

	return *this ;
}

void Curvature::Extract( )
{
	//////////////////////////////////////////////////////////////
	//	���E����f�𔻕�
	//////////////////////////////////////////////////////////////
	SetIsBorder( ) ;

	//////////////////////////////////////////////////////////////
	//	���E���Ƀ��x�����O
	//////////////////////////////////////////////////////////////
	Labeling( ) ;

	//////////////////////////////////////////////////////////////
	//	�ȗ��v�Z
	//////////////////////////////////////////////////////////////
	Calc( ) ;

	//////////////////////////////////////////////////////////////
	//	�o��
	//////////////////////////////////////////////////////////////
	Print( ) ;
}

void Curvature::SetIsBorder( )
{
	//�̈�m��
	is_border = new bool*[ original.height ]( ) ;
	for( int i = 0 ; i < original.height ; i++ )
	{
		is_border[ i ] = new bool[ original.width ]( ) ;
		for( int j = 0 ; j < original.width ; j++ )
		{
			is_border[ i ][ j ] = false ;
		}
	}

	//���E����f�̔���
	for( int i = 0 ; i < original.height ; i++ )
	{
		for( int j = 0 ; j < original.width ; j++ )
		{
			//�������g������f
			if( original.data[ i ][ j ] == BLACK )
			{
				//��ɃA�N�Z�X�\
				if( 0 <= i - 1 )
				{
					//�オ����f
					if( original.data[ i - 1 ][ j ] == WHITE )
					{
						is_border[ i ][ j ] = true ;
					}
				}
				////�E��ɃA�N�Z�X�\
				//if( 0 <= i - 1 && j + 1 < original.width )
				//{
				//	//�E�オ����f
				//	if( original.data[ i - 1 ][ j + 1 ] == WHITE )
				//	{
				//		is_border[ i ][ j ] = true ;
				//	}
				//}
				//�E�ɃA�N�Z�X�\
				if( j + 1 < original.width )
				{
					//�E������f
					if( original.data[ i ][ j + 1 ] == WHITE )
					{
						is_border[ i ][ j ] = true ;
					}
				}
				////�E���ɃA�N�Z�X�\
				//if( i + 1 < original.height && j + 1 < original.width )
				//{
				//	//�E��������f
				//	if( original.data[ i + 1 ][ j + 1 ] == WHITE )
				//	{
				//		is_border[ i ][ j ] = true ;
				//	}
				//}
				//���ɃA�N�Z�X�\
				if( i + 1 < original.height )
				{
					//��������f
					if( original.data[ i + 1 ][ j ] == WHITE )
					{
						is_border[ i ][ j ] = true ;
					}
				}
				////�����ɃA�N�Z�X�\
				//if( i + 1 < original.height && 0 <= j - 1 )
				//{
				//	if( original.data[ i + 1 ][ j - 1 ] == WHITE )
				//	{
				//		is_border[ i ][ j ] = true ;
				//	}
				//}
				//���ɃA�N�Z�X�\
				if( 0 <= j - 1 )
				{
					if( original.data[ i ][ j - 1 ] == WHITE )
					{
						is_border[ i ][ j ] = true ;
					}
				}
				////����ɃA�N�Z�X�\
				//if( 0 <= i - 1 && 0 <= j - 1 )
				//{
				//	if( original.data[ i - 1 ][ j - 1 ] == WHITE )
				//	{
				//		is_border[ i ][ j ] = true ;
				//	}
				//}
			}
		}
	}

	//�m�F�p
	/*for( int i = 0 ; i < original.height ; i++ )
	{
		for( int j = 0 ; j < original.width ; j++ )
		{
			cout << ( is_border[ i ][ j ] == true ? "t" : " " ) << " " ;
		}
	
		cout << endl ;
	}*/
}

void Curvature::Labeling( )
{
	//�C���f�b�N�X�z�񏉊���
	index = new int*[ original.height ]( ) ;
	for( int i = 0 ; i < original.height ; i++ )
	{
		index[ i ] = new int[ original.width ]( ) ;
		for( int j = 0 ; j < original.width ; j++ )
		{
			index[ i ][ j ] = original.height * original.width ;
		}
	}
	//���x���z�񏉊���
	label = new int*[ original.height ]( ) ;
	for( int i = 0 ; i < original.height ; i++ )
	{
		label[ i ] = new int[ original.width ]( ) ;
		for( int j = 0 ; j < original.width ; j++ )
		{
			label[ i ][ j ] = -1 ;
		}
	}

	int count = 0 ;

	//�g����؂�Ă��鋫�E��
	for( int i = 0 ; i < original.height ; i++ )
	{
		for( int j = 0 ; j < original.width ; j++ )
		{
			//�g�ŋ��E��
			if( ( i == 0 || i == original.height - 1 || j == 0 || j == original.width - 1 ) && is_border[ i ][ j ] && index[ i ][ j ] == original.height * original.width )
			{
				index[ i ][ j ] = 0 ;
				label[ i ][ j ] = count ;

				//���x�����O
				bool changed = true ;
				for( ; changed ; )
				{
					//���[�v�I�������ɐݒ�@���x�����O���s���Ȃ��Ȃ炻�̂܂ܔ�����
					changed = false ;

					//���x�����O
					for( int i = 0 ; i < original.height ; i++ )
					{
						for( int j = 0 ; j < original.width ; j++ )
						{
							//���������E����f
							if( is_border[ i ][ j ] )
							{
								int min_index = index[ i ][ j ] ;	//�����̃C���f�b�N�X�ŏ�����

								//����ɃA�N�Z�X�ł���
								if( 0 <= i - 1 && 0 <= j - 1 )
								{
									//���オ���E��
									if( is_border[ i - 1 ][ j - 1 ] )
									{
										//�ŏ��l��菬�����Ȃ�X�V
										if( index[ i - 1 ][ j - 1 ] < min_index )
										{
											min_index = index[ i - 1 ][ j - 1 ] ;
										}
									}
								}
								//���ɃA�N�Z�X�ł���
								if( 0 <= j - 1 )
								{
									//�������E��
									if( is_border[ i ][ j - 1 ] )
									{
										//�ŏ��l��菬�����Ȃ�X�V
										if( index[ i ][ j - 1 ] < min_index )
										{
											min_index = index[ i ][ j - 1 ] ;
										}
									}
								}
								//�����ɃA�N�Z�X�ł���
								if( i + 1 < original.height && 0 <= j - 1 )
								{
									//���������E��
									if( is_border[ i + 1 ][ j - 1 ] )
									{
										//�ŏ��l��菬�����Ȃ�X�V
										if( index[ i + 1 ][ j - 1 ] < min_index )
										{
											min_index = index[ i + 1 ][ j - 1 ] ;
										}
									}
								}
								//���ɃA�N�Z�X�ł���
								if( i + 1 < original.height )
								{
									//�������E��
									if( is_border[ i + 1 ][ j ] )
									{
										//�ŏ��l��菬�����Ȃ�X�V
										if( index[ i + 1 ][ j ] < min_index )
										{
											min_index = index[ i + 1 ][ j ] ;
										}
									}
								}
								//�E���ɃA�N�Z�X�ł���
								if( i + 1 < original.height && j + 1 < original.width )
								{
									//�E�������E��
									if( is_border[ i + 1 ][ j + 1 ] )
									{
										//�ŏ��l��菬�����Ȃ�X�V
										if( index[ i + 1 ][ j + 1 ] < min_index )
										{
											min_index = index[ i + 1 ][ j + 1 ] ;
										}
									}
								}
								//�E�ɃA�N�Z�X�ł���
								if( j + 1 < original.width )
								{
									//�E�����E��
									if( is_border[ i ][ j + 1 ] )
									{
										//�ŏ��l��菬�����Ȃ�X�V
										if( index[ i ][ j + 1 ] < min_index )
										{
											min_index = index[ i ][ j + 1 ] ;
										}
									}
								}
								//�E��ɃA�N�Z�X�ł���
								if( 0 <= i - 1 && j + 1 < original.width )
								{
									//�E�オ���E��
									if( is_border[ i - 1 ][ j + 1 ] )
									{
										//�ŏ��l��菬�����Ȃ�X�V
										if( index[ i - 1 ][ j + 1 ] < min_index )
										{
											min_index = index[ i - 1 ][ j + 1 ] ;
										}
									}
								}
								//��ɃA�N�Z�X�ł���
								if( 0 <= i - 1 )
								{
									//�オ���E��
									if( is_border[ i - 1 ][ j ] )
									{
										//�ŏ��l��菬�����Ȃ�X�V
										if( index[ i - 1 ][ j ] < min_index )
										{
											min_index = index[ i - 1 ][ j ] ;
										}
									}
								}

								//�ŏ��l���ő�l(���E���̉�f�ł͂��邪�܂��`�d���Ă��Ă��Ȃ�)
								//�������g-�ŏ��l��1�̂Ƃ�
								//�ŏ��l���������g�@�����̂Ƃ��͍X�V���Ȃ�
								if( min_index != original.height * original.width &&
									index[ i ][ j ] - min_index != 1 && 
									min_index != index[ i ][ j ] )
								{
									index[ i ][ j ] = min_index + 1 ;
									label[ i ][ j ] = count ;
									changed = true ;
								}
							}
						}
					}
				}
				
				//���s�������x�����O�̋��E���̖���������
				int max = 0 ;
				int max_i = 0 ;
				int max_j = 0 ;
				for( int i = 0 ; i < original.height ; i++ )
				{
					for( int j = 0 ; j < original.width ; j++ )
					{
						if( label[ i ][ j ] == count && max < index[ i ][ j ] )
						{
							max = index[ i ][ j ] ;
							max_i = i ;
							max_j = j ;
						}
					}
				}
				
				//printf( "count %d max %d max_i %d max_j %d\n" , count , max , max_i , max_j ) ;

				//���̋��E����o�^
				borderlist.Add( ) ;

				//���̋��E����g����؂�Ă��鋫�E���Ƃ��ăt���O��ݒ�
				borderlist.border[ borderlist.num - 1 ].island = false ;

				int current_label = index[ max_i ][ max_j ] ;
				int current_i = max_i ;
				int current_j = max_j ;
				for( ; ; )
				{
					//���̍��W��o�^
					borderlist.border[ borderlist.num - 1 ].Add( current_j , current_i ) ;

					//���W���n�_�ɋA���Ă�����I��
					if( current_label == 0 )
					{
						break ;
					}

					//���̍��W�͋ߖT��1���������̂�I��
					int min_index = index[ current_i ][ current_j ] ;	//�����̃C���f�b�N�X�ŏ�����
					int min_i = current_i ;
					int min_j = current_j ;
					//����ɃA�N�Z�X�ł���
					if( 0 <= current_i - 1 && 0 <= current_j - 1 )
					{
						//���オ���E��
						if( is_border[ current_i - 1 ][ current_j - 1 ] )
						{
							//�ŏ��l��菬�����Ȃ�X�V
							if( index[ current_i - 1 ][ current_j - 1 ] < min_index )
							{
								min_index = index[ current_i - 1 ][ current_j - 1 ] ;
								min_i = current_i - 1 ;
								min_j = current_j - 1 ;
							}
						}
					}
					//���ɃA�N�Z�X�ł���
					if( 0 <= current_j - 1 )
					{
						//�������E��
						if( is_border[ current_i ][ current_j - 1 ] )
						{
							//�ŏ��l��菬�����Ȃ�X�V
							if( index[ current_i ][ current_j - 1 ] < min_index )
							{
								min_index = index[ current_i ][ current_j - 1 ] ;
								min_i = current_i ;
								min_j = current_j - 1 ;
							}
						}
					}
					//�����ɃA�N�Z�X�ł���
					if( current_i + 1 < original.height && 0 <= current_j - 1 )
					{
						//���������E��
						if( is_border[ current_i + 1 ][ current_j - 1 ] )
						{
							//�ŏ��l��菬�����Ȃ�X�V
							if( index[ current_i + 1 ][ current_j - 1 ] < min_index )
							{
								min_index = index[ current_i + 1 ][ current_j - 1 ] ;
								min_i = current_i + 1 ;
								min_j = current_j - 1 ;
							}
						}
					}
					//���ɃA�N�Z�X�ł���
					if( current_i + 1 < original.height )
					{
						//�������E��
						if( is_border[ current_i + 1 ][ current_j ] )
						{
							//�ŏ��l��菬�����Ȃ�X�V
							if( index[ current_i + 1 ][ current_j ] < min_index )
							{
								min_index = index[ current_i + 1 ][ current_j ] ;
								min_i = current_i + 1 ;
								min_j = current_j ;
							}
						}
					}
					//�E���ɃA�N�Z�X�ł���
					if( current_i + 1 < original.height && current_j + 1 < original.width )
					{
						//�E�������E��
						if( is_border[ current_i + 1 ][ current_j + 1 ] )
						{
							//�ŏ��l��菬�����Ȃ�X�V
							if( index[ current_i + 1 ][ current_j + 1 ] < min_index )
							{
								min_index = index[ current_i + 1 ][ current_j + 1 ] ;
								min_i = current_i + 1 ;
								min_j = current_j + 1 ;
							}
						}
					}
					//�E�ɃA�N�Z�X�ł���
					if( current_j + 1 < original.width )
					{
						//�E�����E��
						if( is_border[ current_i ][ current_j + 1 ] )
						{
							//�ŏ��l��菬�����Ȃ�X�V
							if( index[ current_i ][ current_j + 1 ] < min_index )
							{
								min_index = index[ current_i ][ current_j + 1 ] ;
								min_i = current_i ;
								min_j = current_j + 1 ;
							}
						}
					}
					//�E��ɃA�N�Z�X�ł���
					if( 0 <= current_i - 1 && current_j + 1 < original.width )
					{
						//�E�オ���E��
						if( is_border[ current_i - 1 ][ current_j + 1 ] )
						{
							//�ŏ��l��菬�����Ȃ�X�V
							if( index[ current_i - 1 ][ current_j + 1 ] < min_index )
							{
								min_index = index[ current_i - 1 ][ current_j + 1 ] ;
								min_i = current_i - 1 ;
								min_j = current_j + 1 ;
							}
						}
					}
					//��ɃA�N�Z�X�ł���
					if( 0 <= current_i - 1 )
					{
						//�オ���E��
						if( is_border[ current_i - 1 ][ current_j ] )
						{
							//�ŏ��l��菬�����Ȃ�X�V
							if( index[ current_i - 1 ][ current_j ] < min_index )
							{
								min_index = index[ current_i - 1 ][ current_j ] ;
								min_i = current_i - 1 ;
								min_j = current_j ;
							}
						}
					}

					//���W���ړ�
					current_label = min_index ;
					current_i = min_i ;
					current_j = min_j ;
				}

				//���x���ԍ����X�V
				count++ ;
			}
		}
	}

	/*for( int i = 0 ; i < borderlist.num ; i++ )
	{
		for( int j = 0 ; j < borderlist.border[ i ].length ; j++ )
		{
			printf( "%d %d\n" , borderlist.border[ i ].point[ j ].x , borderlist.border[ i ].point[ j ].y ) ;
		}
	}*/
	

	//�g���ɂ��鋫�E��
	for( int i = 1 ; i < original.height - 1 ; i++ )
	{
		for( int j = 1 ; j < original.width - 1 ; j++ )
		{
			//���E����f
			if( is_border[ i ][ j ] )
			{
				//���x�����O���ς�ł��Ȃ�
				if( label[ i ][ j ] == -1 )
				{
					//���E����ǐՂ��Ȃ��烉�x�����O�Ƌ��E���o�^
					int sx = j ;	//�����l��x���W
					int sy = i ;	//�����l��y���W
					int tx = j ;	//�ǐՓ_��x���W
					int ty = i ;	//�ǐՓ_��y���W
					Enum_Direction d = UPLEFT ;	//�O��̒ǐՓ_

					//���E����ǉ�
					borderlist.Add( ) ;

					//���̋��E����g����؂�Ă��鋫�E���Ƃ��ăt���O��ݒ�
					borderlist.border[ borderlist.num - 1 ].island = true ;

					//�ǐՓ_�������ʒu�ɖ߂��Ă���܂ő��s
					for( ; ; )
					{
						//���̃��[�v����������ʌǗ��_�Ȃ�I��
						if( d == DEFAULT ) 
						{
							break ;
						}

						//���x�����O��i�߂�
						index[ ty ][ tx ] = borderlist.border[ borderlist.num - 1 ].length ;

						//���̍��W��o�^
						borderlist.border[ borderlist.num - 1 ].Add( tx , ty ) ;

						//length��1�����傫���ď������W�ƒǐՍ��W�������Ƃ����I��
						if( 1 < borderlist.border[ borderlist.num - 1 ].length && sx == tx && sy == ty )
						{
							break ;
						}

						

						//���x��������
						label[ ty ][ tx ] = count ;

						//9��ڂ͌Ǘ���������郋�[�v
						for( int i = 0 ; i < 9 ; i++ )
						{
							//��̌���
							if( ( i + ( int )d + 1 ) % 8 == 0 )
							{
								//printf( "UP serch\n" ) ;
								//��ɃA�N�Z�X�ł���
								if( 0 < ty - 1 )
								{
									//�オ�Ώۉ�f
									if( original.data[ ty - 1 ][ tx ] == BLACK && is_border[ ty - 1 ][ tx ] )
									{
										//printf( "  UP serched\n" ) ;
										//�O��̒ǐՓ_�����ɐݒ�
										d = DOWN ;
										//�ǐՓ_���X�V
										ty -= 1 ;

										//���[�v�I��
										break ;
									}
								}
							}
							//����̌���
							else if( ( i + ( int )d + 1 ) % 8 == 1 )
							{
								//printf( "UPLEFT serch\n" ) ;
								//����ɃA�N�Z�X�ł���
								if( 0 < tx - 1 && 0 < ty - 1 )
								{
									//���オ�Ώۉ�f
									if( original.data[ ty - 1 ][ tx - 1 ] == BLACK && is_border[ ty - 1 ][ tx - 1 ] )
									{
										//printf( "  UPLEFT serched\n" ) ;
										//�O��̒ǐՓ_�����ɐݒ�
										d = DOWNRIGHT ;
										//�ǐՓ_���X�V
										ty -= 1 ;
										tx -= 1 ;
				
										//���[�v�I��
										break ;
									}
								}
							}
							//���̌���
							else if( ( i + ( int )d + 1 ) % 8 == 2 )
							{
								//printf( "LFET serch\n" ) ;
								//���ɃA�N�Z�X�ł���
								if( 0 < tx - 1 )
								{
									//�����Ώۉ�f
									if( original.data[ ty ][ tx - 1 ] == BLACK && is_border[ ty ][ tx - 1 ] )
									{
										//printf( "  LEFT serched\n" ) ;
										//�O��̒ǐՓ_�����ɐݒ�
										d = RIGHT ;
										//�ǐՓ_���X�V
										tx -= 1 ;

										//���[�v�I��
										break ;
									}
								}
							}
							//�����̌���
							else if( ( i + ( int )d + 1 ) % 8 == 3 )
							{
								//printf( "DOWNLEFT serch\n" ) ;
								//�����ɃA�N�Z�X�ł���
								if( 0 < tx - 1 && ty + 1 < original.height )
								{
									//�������Ώۉ�f
									if( original.data[ ty + 1 ][ tx - 1 ] == BLACK && is_border[ ty + 1 ][ tx - 1 ] )
									{
										//printf( "DOWNLEFT serched\n" ) ;
										//�O��̒ǐՓ_�����ɐݒ�
										d = UPRIGHT ;
										//�ǐՓ_���X�V
										tx -= 1 ;
										ty += 1 ;
	
										//���[�v�I��
										break ;
									}
								}
							}
							//���̌���
							else if( ( i + ( int )d + 1 ) % 8 == 4 )
							{
								//printf( "DOWN serch\n" ) ;
								//���ɃA�N�Z�X�ł���
								if( ty + 1 < original.height )
								{
									//�����Ώۉ�f
									if( original.data[ ty + 1 ][ tx ] == BLACK && is_border[ ty + 1 ][ tx ] )
									{
										//printf( "  DOWN serched\n" ) ;
										//�O��̒ǐՓ_�����ɐݒ�
										d = UP ;
										//�ǐՓ_���X�V
										ty += 1 ;
											
										//���[�v�I��
										break ;
									}
								}
							}
							//�E���̌���
							else if( ( i + ( int )d + 1 ) % 8 == 5 )
							{
								//printf( "DOWNRIGHT serch\n" ) ;
								//�E���ɃA�N�Z�X�ł���
								if( tx + 1 < original.width && ty + 1 < original.height )
								{
									//�E�����Ώۉ�f
									if( original.data[ ty + 1 ][ tx + 1 ] == BLACK && is_border[ ty + 1 ][ tx + 1 ] )
									{
										//printf( "  DOWNRIGHT serched\n" ) ;
										//�O��̒ǐՓ_�����ɐݒ�
										d = UPLEFT ;
										//�ǐՓ_���X�V
										ty += 1 ;
										tx += 1 ;

										//���[�v�I��
										break ;
									}
								}
							}
							//�E�̌���
							else if( ( i + ( int )d + 1 ) % 8 == 6 )
							{
								//printf( "RIGHT serch\n" ) ;
								//�E�ɃA�N�Z�X�ł���
								if( tx + 1 < original.width )
								{
									//�E���Ώۉ�f
									if( original.data[ ty ][ tx + 1 ] == BLACK && is_border[ ty ][ tx + 1 ] )
									{
										//printf( "  RIGHT serched\n" ) ;
										//�O��̒ǐՓ_�����ɐݒ�
										d = LEFT ;
										//�ǐՓ_���X�V
										tx += 1 ;
	
										//���[�v�I��
										break ;
									}
								}
							}
							//�E��̌���
							else if( ( i + ( int )d + 1 ) % 8 == 7 )
							{
								//printf( "UPRIGHT serch\n" ) ;
								//�E��ɃA�N�Z�X�ł���
								if( tx + 1 < original.width && 0 < ty - 1 )
								{
									//�E�オ�Ώۉ�f
									if( original.data[ ty - 1 ][ tx + 1 ] == BLACK && is_border[ ty - 1 ][ tx + 1 ] )
									{
										//printf( "  UPRIGHT serched\n" ) ;
										//�O��̒ǐՓ_�����ɐݒ�
										d = DOWNLEFT ;
										//�ǐՓ_���X�V
										ty -= 1 ;
										tx += 1 ;

										//���[�v�I��
										break ;
									}
								}
							}
							//�Ǘ��_
							else
							{
								d = DEFAULT ;
								break ;
							}
						}
					}

					//�������g�̓��x����-1�����ߖT�Ƀ��x�����t�������̂�����Ȃ炻���`�d
					bool propagate = true ;	//�`�d�t���O
					for( ; propagate ; )
					{
						//��U�`�d���Ă��Ȃ���ԂƂ��ċL�^
						propagate = false ;
						
						for( int k = 0 ; k < original.height ; k++ )
						{
							for( int l = 0 ; l < original.width ; l++ )
							{
								//���E���ł܂����x�����O���ς�ł��Ȃ�
								if( is_border[ k ][ l ] && label[ k ][ l ] == -1 )
								{
									//����ɃA�N�Z�X�ł���
									if( 0 <= k - 1 && 0 <= l - 1 )
									{
										//���オ-1�ȊO�̃��x��
										if( label[ k - 1 ][ l - 1 ] != -1 )
										{
											label[ k ][ l ] = label[ k - 1 ][ l - 1 ] ;
											propagate = true ;
										}
									}
									//���ɃA�N�Z�X�ł���
									if( 0 <= l - 1 )
									{
										//����-1�ȊO�̃��x��
										if( label[ k ][ l - 1 ] != -1 )
										{
											label[ k ][ l ] = label[ k ][ l - 1 ] ;
											propagate = true ;
										}
									}
									//�����ɃA�N�Z�X�ł���
									if( k + 1 < original.height && 0 <= l - 1 )
									{
										//������-1�ȊO�̃��x��
										if( label[ k + 1 ][ l - 1 ] != -1 )
										{
											label[ k ][ l ] = label[ k + 1 ][ l - 1 ] ;
											propagate = true ;
										}
									}
									//���ɃA�N�Z�X�ł���
									if( k + 1 < original.height )
									{
										//����-1�ȊO�̃��x��
										if( label[ k + 1 ][ l ] != -1 )
										{
											label[ k ][ l ] = label[ k + 1 ][ l ] ;
											propagate = true ;
										}
									}
									//�E���ɃA�N�Z�X�ł���
									if( k + 1 < original.height && l + 1 < original.width )
									{
										//�E����-1�ȊO�̃��x��
										if( label[ k + 1 ][ l + 1 ] != -1 )
										{
											label[ k ][ l ] = label[ k + 1 ][ l + 1 ] ;
											propagate = true ;
										}
									}
									//�E�ɃA�N�Z�X�ł���
									if( l + 1 < original.width )
									{
										//�E��-1�ȊO�̃��x��
										if( label[ k ][ l + 1 ] != -1 )
										{
											label[ k ][ l ] = label[ k ][ l + 1 ] ;
											propagate = true ;
										}
									}
									//�E��ɃA�N�Z�X�ł���
									if( 0 <= k - 1 && l + 1 < original.width )
									{
										//�E�オ-1�ȊO�̃��x��
										if( label[ k - 1 ][ l + 1 ] != -1 )
										{
											label[ k ][ l ] = label[ k - 1 ][ l + 1 ] ;
											propagate = true ;
										}
									}
									//��ɃA�N�Z�X�ł���
									if( 0 <= k - 1 )
									{
										//�オ-1�ȊO�̃��x��
										if( label[ k - 1 ][ l ] != -1 )
										{
											label[ k ][ l ] = label[ k - 1 ][ l ] ;
											propagate = true ;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	/*for( int i = 0 ; i < original.height ; i++ )
	{
		for( int j = 0 ; j < original.width ; j++ )
		{
			printf( "%5d " , index[ i ][ j ] ) ;
		}
		printf( "\n" ) ;
	}*/
}

void Curvature::Calc( )
{
	//���E���𒼐��ߎ�
	for( int i = 0 ; i < borderlist.num ; i++ )
	//for( int i = 0 ; i < 1 ; i++ )
	{
		//printf( "length %d\n" , borderlist.border[ i ].length ) ;

		//���̎���ɂł��Ă��鋫�E���Ȃ甼���̈ʒu�ŗ\�ߕ���
		if( borderlist.border[ i ].island )
		{
			borderlist.border[ i ].point[ 0 ].devide = true ;
			borderlist.border[ i ].point[ ( int )( ( borderlist.border[ i ].length - 1 ) / 2.f ) ].devide = true ;
			Devide( i , 0 , ( int )( ( borderlist.border[ i ].length - 1 ) / 2.f ) , 0 ) ;
			Devide( i , ( int )( ( borderlist.border[ i ].length - 1 ) / 2.f ) , borderlist.border[ i ].length - 1 , 0 ) ;
		}
		else
		{
			borderlist.border[ i ].point[ 0 ].devide = true ;
			borderlist.border[ i ].point[ borderlist.border[ i ].length - 1 ].devide = true ;
			Devide( i , 0 , borderlist.border[ i ].length - 1 , 0 ) ;
		}
	}

	//�ȗ��v�Z
	int sum_devide = 0 ;
	int sum_length = 0 ;
	for( int i = 0 ; i < borderlist.num ; i++ )
	{
		//�����_�𐔂���
		for( int j = 0 ; j < borderlist.border[ i ].length ; j++ )
		{
			if( borderlist.border[ i ].point[ j ].devide )
			{
				sum_devide++ ;
			}
		}

		//���E���̒��������v �����ʒu�ƍŏI�ʒu������������1����
		sum_length += borderlist.border[ i ].length - 1 ;
	}

	feature[ 0 ] = ( float )sum_devide / ( float )sum_length ;
}

void Curvature::Devide( int num , int start , int end , int cnt )
{
	//3���������_�����Ȃ��ꍇ�͕����ł��Ȃ�
	if( end - start < 3 )
	{
		//printf( "few\n" ) ;
		return ;
	}

	float max_distance = 0.f ;	//�Œ��̍ŒZ����
	int max_index = ( int )( ( end - start ) / 2.f ) ;	//�ŒZ������^����C���f�b�N�X

	//start����end��ʂ钼���ɂ�����ő�ƂȂ鋗���������W�����߂�
	for( int i = start ; i < end ; i++ )
	{
		float distance = Distance( borderlist.border[ num ].point[ end ] , borderlist.border[ num ].point[ start ] , borderlist.border[ num ].point[ i ] ) ;
		//printf( "(%d %d) " , borderlist.border[ num ].point[ start ].x , borderlist.border[ num ].point[ start ].y ) ;
		//printf( "(%d %d) " , borderlist.border[ num ].point[ end ].x , borderlist.border[ num ].point[ end ].y ) ;
		//printf( "(%d %d) " , borderlist.border[ num ].point[ i ].x , borderlist.border[ num ].point[ i ].y ) ;
		//printf( "%f\n" , distance ) ;

		if( max_distance < distance )
		{
			max_distance = distance ;
			max_index = i ;
		}
	}

	//printf( "num %d\n" , num ) ;
	//printf( "start %d\n" , start ) ;
	//printf( "end %d\n" , end ) ;
	//printf( "max_index %d\n" , max_index ) ;
	//printf( "max_distance %f\n" , max_distance ) ;
	//printf( "\n" ) ;

	//臒l�ȉ��̋����Ȃ番�����Ȃ�
	if( max_distance < threshold )
	{
		//printf( "short\n" ) ;
		return ;
	}

	//�����_�Ƃ��Đݒ�
	borderlist.border[ num ].point[ max_index ].devide = true ;

	//����ɕ���
	Devide( num , start , max_index , cnt + 1 ) ;
	Devide( num , max_index , end , cnt + 1 ) ;
}

float Curvature::Distance( Point p1 , Point p2 , Point p3 )
{
	//y���ɕ��s(�X��������)
	if( p2.x - p1.x == 0 )
	{
		float d = p3.x - p1.x ;
		return d < 0 ? -d : d ;
	}
	else
	{
		float m = ( float )( p2.y - p1.y ) / ( float )( p2.x - p1.x ) ;
		float n = ( float )-1.f * ( float )( p2.y - p1.y ) / ( float )( p2.x - p1.x ) * p1.x + p1.y ;
		float d = ( p3.y - m * p3.x - n ) / sqrt( 1 + pow( m , 2.f ) ) ;
		return d < 0 ? -d : d ;
	}
}

float Curvature::Radian( Point first , Point second , Point third )
{
	float sf_x = first.x - second.x ;
	float sf_y = first.y - second.y ;
	float st_x = third.x - second.x ;
	float st_y = third.y - second.y ;

	return acos( ( sf_x * st_x + sf_y * st_y ) / ( sqrt( pow( sf_x , 2.f ) + pow( sf_y , 2.f ) ) * sqrt( pow( st_x , 2.f ) + pow( st_y , 2.f ) ) ) ) ;
}

void Curvature::Print( )
{
	Ppm ppm( original.height , original.width ) ;
	ppm.filename = "curvature.ppm" ;

	for( int i = 0 ; i < original.height ; i++ )
	{
		for( int j =0 ; j < original.width ; j++ )
		{
			int count = 0 ;
			bool flg = false ;

			for( int k = 0 ; k < borderlist.num ; k++ )
			{
				for( int l = 0 ; l < borderlist.border[ k ].length ; l++ )
				{
					//�����ڂ��Ă����f�������_
					if( borderlist.border[ k ].point[ l ].devide &&
						borderlist.border[ k ].point[ l ].x == j &&
						borderlist.border[ k ].point[ l ].y == i )
					{
						flg = true ;
					}
				}
			}

			if( flg )
			{
				//printf( "  *   " ) ;
				ppm.data[ i ][ j ][ 0 ] = 255 ;
				ppm.data[ i ][ j ][ 1 ] = 255 ;
				ppm.data[ i ][ j ][ 2 ] = 0 ;
			}
			else if( is_border[ i ][ j ] )
			{
				//printf( "%5d " , index[ i ][ j ] ) ;
				ppm.data[ i ][ j ][ 0 ] = 128 ;
				ppm.data[ i ][ j ][ 1 ] = 128 ;
				ppm.data[ i ][ j ][ 2 ] = 128 ;
			}
			else if( original.data[ i ][ j ] == BLACK )
			{
				//printf( "%5d " , index[ i ][ j ] ) ;
				ppm.data[ i ][ j ][ 0 ] = 0 ;
				ppm.data[ i ][ j ][ 1 ] = 0 ;
				ppm.data[ i ][ j ][ 2 ] = 0 ;
			}
			else
			{
				//printf( "%5d " , index[ i ][ j ] ) ;
				ppm.data[ i ][ j ][ 0 ] = 0 ;
				ppm.data[ i ][ j ][ 1 ] = 0 ;
				ppm.data[ i ][ j ][ 2 ] = 255 ;
			}
		}

		//printf( "\n" ) ;
	}

	ppm.Save( ) ;
}