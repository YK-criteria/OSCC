//pbm.h

#pragma once

#include <cstdlib>

#include <iostream>
#include <string>
#include <fstream>
using namespace std ;

#define MAX_COMMENT_SIZE 1024

class Ppm
{
public :
	Ppm( ) ;	//コンストラクタ
	Ppm( string filename ) ;	//名前指定コンストラクタ
	Ppm( int height , int width ) ;	//大きさ指定コンストラクタ
	Ppm( const Ppm &ppm ) ;	//コピーコンストラクタ
	~Ppm( ) ;	//デストラクタ
	Ppm& operator=( const Ppm& ppm ) ;	//代入演算子オーバーロード

	void Open( ) ;	//ファイルオープン
	void Open( string filename ) ;	//名前指定ファイルオープン
	void Save( ) ;	//ファイルセーブ
	void Save( string filename ) ;	//名前指定ファイルセーブ
	void Print( ) ;	//表示

	string filename ;	//ファイル名
	string type ;	//P1固定だが念のため
	string comment ;	//コメント
	int height ;	//縦幅
	int width ;	//横幅
	int ***data ;	//データ
} ;
