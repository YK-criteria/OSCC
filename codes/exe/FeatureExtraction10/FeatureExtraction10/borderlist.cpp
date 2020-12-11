#include "borderlist.h"

BorderList::BorderList( ) : increase( 10 )
{
	num = 0 ;
	max_num = increase ;
	border = new Border[ max_num ]( ) ;
}

void BorderList::Add( )
{
	//border配列が足りない
	if( num == max_num )
	{
		int new_max_num = max_num + increase ;	//新たに作るpointo配列の大きさ
		Border *new_border = new Border[ new_max_num ]( ) ;	//新たなpoint配列
		//現在のpoint配列の内容をコピー
		for( int i = 0 ; i < num ; i++ )
		{
			new_border[ i ] = border[ i ] ;
		}
		delete[ ] border ;

		//現在の値を更新
		border = new_border ;
		max_num = new_max_num ;
	}

	//境界線を追加
	border[ num ] = Border( ) ;
	
	//境界線画素の数を増やす
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