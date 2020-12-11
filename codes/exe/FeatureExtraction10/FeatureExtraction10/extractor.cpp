#include "extractor.h"

Extractor::Extractor( string filename )
{
	//�t�@�C�����w��
	input.filename = filename ;

	//�t�@�C���I�[�v��
	input.Open( ) ;

	//�摜��n���ď�����
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
