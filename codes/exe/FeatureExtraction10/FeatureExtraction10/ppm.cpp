#include "ppm.h"

Ppm::Ppm( )
{
	filename = "" ;
	type = "P3" ;
	comment = "#comment" ;
	height = 0 ;
	width = 0 ;
	data = NULL ;
}

Ppm::Ppm( string filename )
{
	this->filename = filename ;
	type = "P3" ;
	comment = "#comment" ;
	height = 0 ;
	width = 0 ;
	data = NULL ;
}

Ppm::Ppm( int height , int width )
{
	filename = "" ;
	type = "P3" ;
	comment = "#comment" ;
	this->height = height ;
	this->width = width ;
	data = new int**[ height ]( ) ;
	for( int i = 0 ; i < height ; i++ )
	{
		data[ i ] = new int*[ width ]( ) ;
		for( int j = 0 ; j < width ; j++ )
		{
			data[ i ][ j ] = new int[ 3 ]( ) ;
		}
	}
}

Ppm::Ppm( const Ppm &ppm )
{
	filename = ppm.filename ;
	type = ppm.type ;
	comment = ppm.comment ;
	height = ppm.height ;
	width = ppm.width ;

	//�R�s�[���̔z��ɓ��e���Ȃ�
	if( !ppm.data )
	{
		data = NULL ;
	}
	else
	{
		//���������蓖��
		data = new int**[ height ]( ) ;
		for( int i = 0 ; i < height ; i++ )
		{
			data[ i ] = new int*[ width ]( ) ;
			for( int j = 0 ; j < width ; j++ )
			{
				data[ i ][ j ] = new int[ 3 ]( ) ;
			}
		}

		//���e���R�s�[
		for( int i = 0 ; i < height ; i++ )
		{
			for( int j = 0 ; j < width ; j++ )
			{
				for( int k = 0 ; k < 3 ; k++ )
				{
					data[ i ][ j ][ k ] = ppm.data[ i ][ j ][ k ] ;
				}
			}
		}
	}
}

Ppm::~Ppm( )
{
	if( data )
	{
		for( int i = 0 ; i < height ; i++ )
		{
			for( int j = 0 ; j < width ; j++ )
			{
				if( data[ i ][ j ] )
				{
					delete[ ] data[ i ][ j ] ;
					data[ i ][ j ] = NULL ;
				}
			}
			if( data[ i ] )
			{
				delete[ ] data[ i ] ;
			}
		}
		delete[ ] data ;
		data = NULL ;
	}
}

Ppm& Ppm::operator=( const Ppm& ppm )
{
	//��f�f�[�^���Ȃ�
	if( !ppm.data )
	{
		filename = ppm.filename ;
		height = ppm.height ;
		width = ppm.width ;
		type = ppm.type ;
		comment = ppm.comment ;
		data = NULL ;
	}
	else
	{
		if( data )
		{
			//��U�R�s�[��̔z���j��
			for( int i = 0 ; i < height ; i++ )
			{
				for( int j = 0 ; j < width ; j++ )
				{
					delete [ ] data[ i ][ j ] ;
					data[ i ][ j ] = NULL ;
				}
				delete[ ] data[ i ] ;
				data[ i ] = NULL ;
			}
			delete[ ] data ;
			data = NULL ;
		}

		//�ϐ��̒l���R�s�[
		filename = ppm.filename ;
		height = ppm.height ;
		width = ppm.width ;
		type = ppm.type ;
		comment = ppm.comment ;

		//�z��̎�蒼��
		data = new int**[ height ]( ) ;
		for( int i = 0 ; i < height ; i++ )
		{
			data[ i ] = new int*[ width ]( ) ;
			for( int j = 0 ; j < width ; j++ )
			{
				data[ i ][ j ] = new int[ 3 ]( ) ;
			}
		}

		//�z��̓��e���R�s�[
		for( int i = 0 ; i < height ; i++ )
		{
			for( int j = 0 ; j < width ; j++ )
			{
				for( int k = 0 ; k < 3 ; k++ )
				{
					data[ i ][ j ][ k ] = ppm.data[ i ][ j ][ k ] ;
				}
			}
		}
	}

	return *this ;
}

void Ppm::Open( )
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
	if( type != "P3" )
	{
		cout << filename << " is not P1 Ppm." << endl ;
		exit( EXIT_FAILURE ) ;
	}

	//�R�����g�擾
	getline( ifs , comment ) ;

	//�T�C�Y�擾
	int new_height , new_width ;
	ifs >> new_width ;
	ifs >> new_height ;

	//�ő�P�x�擾
	int max_intence ;
	ifs >> max_intence ;

	//��U���݂̃f�[�^��j��
	for( int i = 0 ; i < height ; i++ )
	{
		for( int j = 0 ; j < width ; j++ )
		{
			delete[ ] data[ i ][ j ] ;
		}
		delete[ ] data[ i ] ;
		data[ i ] = NULL ;
	}
	delete[ ] data ;
	data = NULL ;

	//�V�����T�C�Y�ɍX�V���ă������̊��蓖��
	height = new_height ;
	width = new_width ;
	data = new int**[ height ]( ) ;
	for( int i = 0 ; i < height ; i++ )
	{
		data[ i ] = new int*[ width ]( ) ;
		for( int j = 0 ; j < width ; j++ )
		{
			data[ i ][ j ] = new int[ 3 ]( ) ;
		}
	}
	
	//�t�@�C������f�[�^�R�s�[
	for( int i = 0 ; i < height ; i++ )
	{
		for( int j = 0 ; j < width ; j++ )
		{
			int r , g , b ;
			ifs >> r >> g >> b ;
			data[ i ][ j ][ 0 ] = r ;
			data[ i ][ j ][ 1 ] = g ;
			data[ i ][ j ][ 2 ] = b ;
		}
	}
}

void Ppm::Open( string filename )
{
	//�V�����t�@�C�����ɍX�V���Ă���t�@�C���I�[�v��
	this->filename = filename ;
	Open( ) ;
}

void Ppm::Save( )
{
	ofstream ofs( filename ) ;	//�t�@�C���X�g���[��

	//�t�@�C���I�[�v���G���[
	if( ofs.fail( ) )
	{
		cout << filename << " can't open." << endl ;
		exit( EXIT_FAILURE ) ;
	}

	//�t�@�C���^�C�v �R�����g �T�C�Y �ő�P�x�o��
	ofs << type << endl ;
	ofs << comment << endl ;
	ofs << width << " " << height << endl ;
	ofs << 255 << endl ;

	//��f�f�[�^�o��
	for( int i = 0 ; i < height ; i++ )
	{
		for( int  j = 0 ; j < width ; j++ )
		{
			ofs << data[ i ][ j ][ 0 ] << " " << data[ i ][ j ][ 1 ] << " " << data[ i ][ j ][ 2 ] << endl ;
		}
	}
}

void Ppm::Save( string filename )
{
	//�V�����t�@�C�����ɍX�V���Ă���t�@�C���Z�[�u
	this->filename = filename ;
	Save( ) ;
}

void Ppm::Print( )
{
	for( int i = 0 ; i < height ; i++ )
	{
		for( int j = 0 ; j < width ; j++ )
		{
			cout << data[ i ][ j ][ 0 ] << " " << data[ i ][ j ][ 1 ] << " " << data[ i ][ j ][ 2 ] << endl ;
		}
		cout << endl ;
	}
}
