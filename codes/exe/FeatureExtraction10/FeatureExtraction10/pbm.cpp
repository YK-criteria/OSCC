#include "pbm.h"

Pbm::Pbm( )
{
	filename = "" ;
	type = "P1" ;
	comment = "#comment" ;
	height = 0 ;
	width = 0 ;
	data = NULL ;
}

Pbm::Pbm( string filename )
{
	this->filename = filename ;
	type = "P1" ;
	comment = "#comment" ;
	height = 0 ;
	width = 0 ;
	data = NULL ;
}

Pbm::Pbm( int height , int width )
{
	filename = "" ;
	type = "P1" ;
	comment = "#comment" ;
	this->height = height ;
	this->width = width ;
	data = new PbmColor*[ height ]( ) ;
	for( int i = 0 ; i < height ; i++ )
	{
		data[ i ] = new PbmColor[ width ]( ) ;
	}
}

Pbm::Pbm( const Pbm &pbm )
{
	filename = pbm.filename ;
	type = pbm.type ;
	comment = pbm.comment ;
	height = pbm.height ;
	width = pbm.width ;

	//�R�s�[���̔z��ɓ��e���Ȃ�
	if( !pbm.data )
	{
		data = NULL ;
	}
	else
	{
		//���������蓖��
		data = new PbmColor*[ height ]( ) ;
		for( int i = 0 ; i < height ; i++ )
		{
			data[ i ] = new PbmColor[ width ]( ) ;
		}

		//���e���R�s�[
		for( int i = 0 ; i < height ; i++ )
		{
			for( int j = 0 ; j < width ; j++ )
			{
				data[ i ][ j ] = pbm.data[ i ][ j ] ;
			}
		}
	}
}

Pbm::~Pbm( )
{
	if( data )
	{
		for( int i = 0 ; i < height ; i++ )
		{
			if( data[ i ] )
			{
				delete[ ] data[ i ] ;
				data[ i ] = NULL ;
			}
		}
		delete[ ] data ;
		data = NULL ;
	}
}

Pbm& Pbm::operator=( const Pbm& pbm )
{
	//��f�f�[�^���Ȃ�
	if( !pbm.data )
	{
		filename = pbm.filename ;
		height = pbm.height ;
		width = pbm.width ;
		type = pbm.type ;
		comment = pbm.comment ;
		data = NULL ;
	}
	else
	{
		if( data )
		{
			//��U�R�s�[��̔z���j��
			for( int i = 0 ; i < height ; i++ )
			{
				delete[ ] data[ i ] ;
				data[ i ] = NULL ;
			}
			delete[ ] data ;
			data = NULL ;
		}

		//�ϐ��̒l���R�s�[
		filename = pbm.filename ;
		height = pbm.height ;
		width = pbm.width ;
		type = pbm.type ;
		comment = pbm.comment ;

		//�z��̎�蒼��
		data = new PbmColor*[ height ]( ) ;
		for( int i = 0 ; i < height ; i++ )
		{
			data[ i ] = new PbmColor[ width ]( ) ;
		}

		//�z��̓��e���R�s�[
		for( int i = 0 ; i < height ; i++ )
		{
			for( int j = 0 ; j < width ; j++ )
			{
				data[ i ][ j ] = pbm.data[ i ][ j ] ;
			}
		}
	}

	return *this ;
}

void Pbm::Open( )
{
	ifstream ifs( filename ) ;	//�t�@�C���X�g���[��

	//�t�@�C���I�[�v���G���[
	if( !ifs )
	{
		cout << filename << " can't open." << endl ;
		exit( EXIT_FAILURE ) ;
	}

	//�t�@�C���^�C�v�擾
	getline( ifs , type ) ;

	//�t�@�C���^�C�v��P1�̃��m�N���e�L�X�g�^�C�v�łȂ��Ƃ��͋����I��
	if( type != "P1" )
	{
		cout << filename << " is not P1 Pbm." << endl ;
		exit( EXIT_FAILURE ) ;
	}

	//�R�����g�擾
	getline( ifs , comment ) ;

	//�T�C�Y�擾
	int new_height , new_width ;
	ifs >> new_width ;
	ifs >> new_height ;

	//��U���݂̃f�[�^��j��
	for( int i = 0 ; i < height ; i++ )
	{
		delete[ ] data[ i ] ;
		data[ i ] = NULL ;
	}
	delete[ ] data ;
	data = NULL ;

	//�V�����T�C�Y�ɍX�V���ă������̊��蓖��
	height = new_height ;
	width = new_width ;
	data = new PbmColor*[ height ]( ) ;
	for( int i = 0 ; i < height ; i++ )
	{
		data[ i ] = new PbmColor[ width ]( ) ;
	}
	
	//�t�@�C������f�[�^�R�s�[
	for( int i = 0 ; i < height ; i++ )
	{
		for( int j = 0 ; j < width ; j++ )
		{
			string s = "" ;
			ifs >> s ;
			data[ i ][ j ] = s == "1" ? BLACK : WHITE ;
		}
	}
}

void Pbm::Open( string filename )
{
	//�V�����t�@�C�����ɍX�V���Ă���t�@�C���I�[�v��
	this->filename = filename ;
	Open( ) ;
}

void Pbm::Save( )
{
	ofstream ofs( filename ) ;	//�t�@�C���X�g���[��

	//�t�@�C���I�[�v���G���[
	if( ofs.fail( ) )
	{
		cout << filename << " can't open." << endl ;
		exit( EXIT_FAILURE ) ;
	}

	//�t�@�C���^�C�v �R�����g �T�C�Y�o��
	ofs << type << endl ;
	ofs << comment << endl ;
	ofs << width << " " << height << endl ;

	//��f�f�[�^�o��
	for( int i = 0 ; i < height ; i++ )
	{
		for( int  j = 0 ; j < width ; j++ )
		{
			string out = data[ i ][ j ] == BLACK ? "1 " : "0 " ;
			ofs << out  ;
		}
		ofs << endl ;
	}
	ofs << endl ;
}

void Pbm::Save( string filename )
{
	//�V�����t�@�C�����ɍX�V���Ă���t�@�C���Z�[�u
	this->filename = filename ;
	Save( ) ;
}

void Pbm::Print( )
{
	for( int i = 0 ; i < height ; i++ )
	{
		for( int j = 0 ; j < width ; j++ )
		{
			cout << data[ i ][ j ] << " " ;
		}
		cout << endl ;
	}
}
