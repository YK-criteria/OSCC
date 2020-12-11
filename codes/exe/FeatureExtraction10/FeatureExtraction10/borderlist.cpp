#include "borderlist.h"

BorderList::BorderList( ) : increase( 10 )
{
	num = 0 ;
	max_num = increase ;
	border = new Border[ max_num ]( ) ;
}

void BorderList::Add( )
{
	//border�z�񂪑���Ȃ�
	if( num == max_num )
	{
		int new_max_num = max_num + increase ;	//�V���ɍ��pointo�z��̑傫��
		Border *new_border = new Border[ new_max_num ]( ) ;	//�V����point�z��
		//���݂�point�z��̓��e���R�s�[
		for( int i = 0 ; i < num ; i++ )
		{
			new_border[ i ] = border[ i ] ;
		}
		delete[ ] border ;

		//���݂̒l���X�V
		border = new_border ;
		max_num = new_max_num ;
	}

	//���E����ǉ�
	border[ num ] = Border( ) ;
	
	//���E����f�̐��𑝂₷
	num++ ;
}

bool BorderList::Exist( int x , int y )
{
	for( int i = 0 ; i < num ; i++ )
	{
		if( border[ i ].Exist( x , y ) )
		{
			return true ;
		}
	}

	return false ;
}