#include "dir.h"

vector<string> Dir::ReadFile( string folder )
{
	// �錾
	vector<string> fileList ;
	HANDLE hFind ;
	WIN32_FIND_DATA fd ;

	// �t�@�C���������̂��߂Ƀ��C���h�J�[�h�ǉ�
	stringstream ss ;
	ss << folder ;
	string::iterator itr = folder.end( ) ;
	itr-- ;
	if( *itr != '\\' ) ss << '\\' ;
	ss << "*.*" ;

	// �t�@�C���T��
	hFind = FindFirstFile( ss.str( ).c_str( ) , &fd ) ;

	// �������s
	if( hFind == INVALID_HANDLE_VALUE )
	{
		std::cout << "�t�@�C���ꗗ���擾�ł��܂���ł���" << std::endl;
		exit( 1 ) ; // �G���[�I��
	}

	// �t�@�C���������X�g�Ɋi�[���邽�߂̃��[�v
	do{
		// �t�H���_�͏���
		if( !( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) &&
			!( fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ) )
		{
			//�t�@�C���������X�g�Ɋi�[
			char *file = fd.cFileName ;
			string str = file ;
			fileList.push_back( str ) ;
		}
	}
	while( FindNextFile( hFind , &fd ) ) ; //���̃t�@�C����T��
	
	// hFind�̃N���[�Y
	FindClose( hFind ) ; 
	return fileList ;
}

vector<string> Dir::ReadFolder( string folder )
{
		// �錾
	vector<string> fileList ;
	HANDLE hFind ;
	WIN32_FIND_DATA fd ;

	// �t�@�C���������̂��߂Ƀ��C���h�J�[�h�ǉ�
	stringstream ss ;
	ss << folder ;
	string::iterator itr = folder.end( ) ;
	itr-- ;
	if( *itr != '\\' ) ss << '\\' ;
	ss << "*.*" ;

	// �t�@�C���T��
	hFind = FindFirstFile( ss.str( ).c_str( ) , &fd ) ;

	// �������s
	if( hFind == INVALID_HANDLE_VALUE )
	{
		std::cout << "�t�@�C���ꗗ���擾�ł��܂���ł���" << std::endl;
		exit( 1 ) ; // �G���[�I��
	}

	// �t�@�C���������X�g�Ɋi�[���邽�߂̃��[�v
	do{
		// �t�H���_�͏���
		if( ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) &&
			!( fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ) )
		{
			//�t�@�C���������X�g�Ɋi�[
			char *file = fd.cFileName ;
			string str = file ;
			fileList.push_back( str ) ;
		}
	}
	while( FindNextFile( hFind , &fd ) ) ; //���̃t�@�C����T��
	
	// hFind�̃N���[�Y
	FindClose( hFind ) ; 
	return fileList ;
}