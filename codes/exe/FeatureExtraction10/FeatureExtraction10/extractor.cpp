#include "extractor.h"

Extractor::Extractor( string filename )
{
	//ファイル名指定
	input.filename = filename ;

	//ファイルオープン
	input.Open( ) ;

	//画像を渡して初期化
	area = Area( input ) ;
	curvature = Curvature( input ) ;
	jaggy = Jaggy( input ) ;
	direction = Direction( input ) ;
	funicular =Funicular( input ) ;
	length = Length( input ) ;
	regionnum = RegionNum( input ) ;
}

Extractor::~Extractor( )
{

}

void Extractor::Extract( )
{
	area.Extract( ) ;
	curvature.Extract( ) ;
	jaggy.Extract( ) ;
	direction.Extract( ) ;
	funicular.Extract( ) ;
	length.Extract( ) ;
	regionnum.Extract( ) ;
}

void Extractor::Output( )
{
	area.Output( ) ;
	curvature.Output( ) ;
	jaggy.Output( ) ;
	direction.Output( ) ;
	funicular.Output( ) ;
	length.Output( ) ;
	regionnum.Output( ) ;
	cout << endl ;
}
