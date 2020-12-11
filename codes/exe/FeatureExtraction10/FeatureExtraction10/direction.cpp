#include "direction.h"

Direction::Direction( ) : dim( 1 ) , increase( 10 ) , threshold( 5.f )
{
	//�e�N���X�̕ϐ��ݒ�
	dimension = dim ;
	feature = new float[ dimension ]( ) ;
}

Direction::Direction( Pbm image ) : dim( 1 ) , increase( 10 ) , threshold( 5.f )
{
	//�e�N���X�̕ϐ��ݒ�
	dimension = dim ;
	feature = new float[ dimension ]( ) ;
	
	//image����t�@�C���������ύX
	original = image ;
	original.filename = "original.pbm" ;
}

Direction::Direction( const Direction &direction ) : dim( 1 ) , increase( 10 ) , threshold( 5.f )
{
	//�e�N���X�ŏ������ق������������H
	dimension = direction.dimension ;
	feature = new float[ dimension ]( ) ;
	for( int i = 0 ; i < dimension ; i++ )
	{
		feature[ i ] = direction.feature[ i ] ;
	}

	//�C���[�W�R�s�[
	original = direction.original ;
}

Direction::~Direction( )
{
	
}

Direction& Direction::operator=( const Direction& direction )
{
	//�e�N���X�ŏ������ق������������H
	dimension = direction.dimension ;
	original = direction.original ;
	delete feature ;
	feature = new float[ dimension ] ;
	for( int i = 0 ; i < dimension ; i++ )
	{
		feature[ i ] = direction.feature[ i ] ;
	}

	return *this ;
}

void Direction::Extract( )
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
	//	�����v�Z
	//////////////////////////////////////////////////////////////
	Calc( ) ;

	//////////////////////////////////////////////////////////////
	//	�o��
	//////////////////////////////////////////////////////////////
	Print( ) ;
}

void Direction::SetIsBorder( )
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

void Direction::Labeling( )
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
	

	//�g���ɂ��鋫�E���̓��x�����O���Ȃ�

	/*for( int i = 0 ; i < original.height ; i++ )
	{
		for( int j = 0 ; j < original.width ; j++ )
		{
			printf( "%5d " , index[ i ][ j ] ) ;
		}
		printf( "\n" ) ;
	}*/
}

void Direction::Calc( )
{
	//���E���𒼐��ߎ�
	for( int i = 0 ; i < borderlist.num ; i++ )
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

	for( int i = 0 ; i < borderlist.num ; i++ )
	{
		//��ԍŏ��̕����_��ݒ�
		Point first = borderlist.border[ i ].point[ 0 ] ;
		Point second ;
		Point third ;

		float rad ;	//�����p
		Point point[ 256 ] ;	//�J�_�i�[�p

		//��Ԗڂ̓_��T��
		int j = 0 ;
		for( ; j < borderlist.border[ i ].length ; j++ )
		{
			//�����_�𔭌�
			if( borderlist.border[ i ].point[ j ].devide )
			{
				second = borderlist.border[ i ].point[ j ] ;
				break ;
			}
		}

		//�O�Ԗڂ̓_��T��
		for( ; j < borderlist.border[ i ].length ; j++ )
		{
			//�����_�𔭌�
			if( borderlist.border[ i ].point[ j ].devide )
			{
				third = borderlist.border[ i ].point[ j ] ;

				//�p�x���v�Z
				rad = Radian( first , second , third ) ;
				break ;
			}
		}

		//4�Ԗڂ���̕����_��T��
		for( ; j < borderlist.border[ i ].length ; j++ )
		{
			//�����_�𔭌�
			if( borderlist.border[ i ].point[ j ].devide )
			{
				third = borderlist.border[ i ].point[ j ] ;

				//�p�x���v�Z
				rad = Radian( first , second , third ) ;
				
			}
		}
	}

	const int DIRECTION_NUM = 16 ;
	float *direction = new float[ DIRECTION_NUM ]( ) ;

	for( int i = 0 ; i < borderlist.num ; i++ )
	{
		Point start = borderlist.border[ i ].point[ 0 ] ;
		Point end ;

		for( int j = 0 ; j < borderlist.border[ i ].length ; j++ )
		{
			//�����_�𔭌�
			if( borderlist.border[ i ].point[ j ].devide )
			{
				end = borderlist.border[ i ].point[ j ] ;

				//�������v�Z
				float dx = end.x - start.x ;
				float dy = end.y - start.y ;
				
				int min_index = 0 ;
				int min_radian = 3.14 ;

				//���Ɉ�ԋ߂��x�N�g��������
				for( int k = 0 ; k < DIRECTION_NUM ; k++ )
				{
					//���x�N�g��
					float vx = cos( 3.14 / DIRECTION_NUM * k ) ;
					float vy = sin( 3.14 / DIRECTION_NUM * k ) ;

					//�����p
					float radian = acos( ( dx * vx + dy* vy ) / ( sqrt( pow( dx , 2.f ) + pow( dy , 2.f ) ) * sqrt( pow( vx , 2.f ) + pow( vy , 2.f ) ) ) ) ;

					//���܂ł̍ŏ��p��菬����
					if( radian < min_radian )
					{
						min_index = k ;
						min_radian = radian ;
					}
				}

				//�����_�Ԃ̋������i�[
				direction[ min_index ] += sqrt( pow( end.x - start.x , 2.f ) + pow( end.y - start.y , 2.f ) ) ;
			}
		}
	}

	//���a
	float sum_direction = 0.f ;
	for( int i = 0 ; i < DIRECTION_NUM ; i++ )
	{
		sum_direction += direction[ i ] ;
	}
	//����
	float average = sum_direction / DIRECTION_NUM ;
	//���U
	float variance = 0.f ;
	for( int i = 0 ; i < DIRECTION_NUM ; i++ )
	{
		variance += pow( direction[ i ] - average , 2.f ) ;
	}
	variance /= DIRECTION_NUM ;

	/*for( int i = 0 ; i < DIRECTION_NUM ; i++ )
	{
		printf( "direction[ %d ] %f\n" , i , direction[ i ] ) ;
	}*/
	//�����Ő��K��
	float sum_length = 0.f ;
	for( int i = 0 ; i < borderlist.num ; i++ )
	{
		sum_length += borderlist.border[ i ].length - 1 ;
	}
	//printf( "variance : %f\n" , variance / sum_length ) ;

	feature[ 0 ] = sum_length != 0 ? variance / sum_length : 0.f ;
}

void Direction::Devide( int num , int start , int end , int cnt )
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

float Direction::Distance( Point p1 , Point p2 , Point p3 )
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

float Direction::Radian( Point first , Point second , Point third )
{
	float sf_x = first.x - second.x ;
	float sf_y = first.y - second.y ;
	float st_x = third.x - second.x ;
	float st_y = third.y - second.y ;

	return acos( ( sf_x * st_x + sf_y * st_y ) / ( sqrt( pow( sf_x , 2.f ) + pow( sf_y , 2.f ) ) * sqrt( pow( st_x , 2.f ) + pow( st_y , 2.f ) ) ) ) ;
}

void Direction::Print( )
{
	Ppm ppm( original.height , original.width ) ;
	ppm.filename = "direction.ppm" ;

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