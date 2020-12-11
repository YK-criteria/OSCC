#include "main.h"

int main( int argc , char *argv[ ] )
{
	//�t�@�C���I�[�v���G���[
	if( argc != 2 )
	{
		cout << "input .exe file and .ini file." << endl ;
		exit( EXIT_FAILURE ) ;
	}
	
	ifstream ifs( "leaveoneoutbat.ini" ) ;	//���̓t�@�C���X�g���[��
	//�t�@�C���I�[�v���G���[
	if( !ifs )
	{
		cout << "leaveoneoutbat.ini can not open." << endl ;
		exit( EXIT_FAILURE ) ;
	}

	//�ݒ�t�@�C���ǂݎ��
	string dummy , exe , feature , pbm , train , test , config , result ;	//������ǂݍ��ݗp
	ifs >> dummy ;	ifs >> exe ;
	ifs >> dummy ;	ifs >> feature ;
	ifs >> dummy ;	ifs >> pbm ;
	ifs >> dummy ;	ifs >> train ;
	ifs >> dummy ;	ifs >> test ;
	ifs >> dummy ;	ifs >> config ;
	ifs >> dummy ;	ifs >> result ;

	//�t�H���_���擾
	Dir dir ;
	vector< string > folderlist = dir.ReadFolder( feature ) ;

	//.��..�̃t�H���_�����폜���Ă���
	for( unsigned int i = 0 ; i < folderlist.size( ) ; )
	{
		if( folderlist[ i ] == "." || folderlist[ i ] == ".." )
		{
			folderlist.erase( folderlist.begin( ) ) ;
			continue ;
		}

		i++ ;
	}

	//�����o���p�t�@�C�����쐬
	vector< string > all_feature ;	//�S�����ʃt�@�C��
	vector< string > all_path ;	//�S�t�@�C���̏����o���p�X
	vector< string > all_folder ;	//�S�t�@�C���̃t�H���_��
	for( unsigned int i = 0 ; i < folderlist.size( ) ; i++ )
	{
		vector< string > filelist = dir.ReadFile( feature + folderlist[ i ] ) ;

		for( unsigned int j = 0 ; j < filelist.size( ) ; j++ )
		{
			all_feature.push_back( feature + folderlist[ i ] + "\\" +  filelist[ j ] ) ;
			all_path.push_back( folderlist[ i ] + "\\" + filelist[ j ] ) ;
			all_folder.push_back( folderlist[ i ] ) ;
		}
	}
	vector< string > all_file ;	//�S�t�@�C��
	for( unsigned int i = 0 ; i < folderlist.size( ) ; i++ )
	{
		vector< string > filelist = dir.ReadFile( pbm + folderlist[ i ] ) ;
		for( unsigned int j = 0 ; j < filelist.size( ) ; j++ )
		{
			all_file.push_back( filelist[ j ] ) ;
		}
	}

	//�o�͗p
	ofstream ofs( "leaveoneoutbat.bat" , ios::trunc ) ;	//�o�̓t�@�C���X�g���[��
	if( !ofs )
	{
		cout << exe + " can not open." << endl ;
		exit( EXIT_FAILURE ) ;
	}

	//��U�O��̃t�@�C�����폜
	for( unsigned int i = 0 ; i < folderlist.size( ) ; i++ )
	{
		vector< string > test_filelist = dir.ReadFile( test + folderlist[ i ] ) ;
		for( unsigned int j = 0 ; j < test_filelist.size( ) ; j++ )
		{
			ofs << "DEL /Q " + test + folderlist[ i ] + "\\" + test_filelist[ j ] << endl ;
		}

		vector< string > train_filelist = dir.ReadFile( train + folderlist[ i ] ) ;
		for( unsigned int j = 0 ; j < train_filelist.size( ) ; j++ )
		{
			ofs << "DEL /Q " + train + folderlist[ i ] + "\\" + train_filelist[ j ] << endl ;
		}
	}

	//�o�b�`�t�@�C���쐬
	for( unsigned int i = 0 ; i < all_feature.size( ) ; i++ )
	{
		int cnt = 0 ;
		for( unsigned int j = 0 ; j < folderlist.size( ) ; j++ )
		{
			vector< string > filelist = dir.ReadFile( feature + folderlist[ j ] ) ;
			for( unsigned int k = 0 ; k < filelist.size( ) ; k++ )
			{
				//�e�X�g�f�[�^
				if( i == cnt )
				{
					//�N���X�����ɏ����o��
					ofs << "SET /P X = " + folderlist[ j ] + " < NUL >> " + test + all_path[ i ] << endl ;
					//�����ʏ����o��
					ofs << "more " + all_feature[ cnt ] + " >> " + test + all_path[ i ] << endl ;
				}
				//�P���f�[�^
				else
				{
					//�N���X�����ɏ����o��
					ofs << "SET /P X = " + folderlist[ j ] + " < NUL >> " + train + all_path[ i ] << endl ;
					//�����ʏ����o��
					ofs << "more " + all_feature[ cnt ] + " >> " + train + all_path[ i ] << endl ;
				}

				//�S���J�E���^
				cnt++ ;
			}
		}

		ofs << "echo class_num           5" << " > " + config + all_folder[ i ] + "\\" + all_file[ i ] + ".ini" << endl ;
		ofs << "echo dimension_num       8" << " >> " + config + all_folder[ i ] + "\\" + all_file[ i ] + ".ini" << endl ;
		ofs << "echo select_dimension    3" << " >> " + config + all_folder[ i ] + "\\" + all_file[ i ] + ".ini" << endl ;
		ofs << "echo tree_num            300" << " >> " + config + all_folder[ i ] + "\\" + all_file[ i ] + ".ini" << endl ;
		ofs << "echo max_depth           10" << " >> " + config + all_folder[ i ] + "\\" + all_file[ i ] + ".ini" << endl ;
		ofs << "echo normal_num          3" << " >> " + config + all_folder[ i ] + "\\" + all_file[ i ] + ".ini" << endl ;
		ofs << "echo threshold_num       50" << " >> " + config + all_folder[ i ] + "\\" + all_file[ i ] + ".ini" << endl ;
		ofs << "echo train_num           100" << " >> " + config + all_folder[ i ] + "\\" + all_file[ i ] + ".ini" << endl ;
		ofs << "echo test_num            1" << " >> " + config + all_folder[ i ] + "\\" + all_file[ i ] + ".ini" << endl ;
		ofs << "echo subset_size         100" << " >> " + config + all_folder[ i ] + "\\" + all_file[ i ] + ".ini" << endl ;
		ofs << "echo min_subset_size     2" << " >> " + config + all_folder[ i ] + "\\" + all_file[ i ] + ".ini" << endl ;
		ofs << "echo min_informationgain 0.01" << " >> " + config + all_folder[ i ] + "\\" + all_file[ i ] + ".ini" << endl ;
		ofs << "echo split_type          axis" << " >> " + config + all_folder[ i ] + "\\" + all_file[ i ] + ".ini" << endl ;
		ofs << "echo select_type         multiple" << " >> " + config + all_folder[ i ] + "\\" + all_file[ i ] + ".ini" << endl ;
		ofs << "echo train_filename      " << train + all_path[ i ] + " >> " + config + all_folder[ i ] + "\\" + all_file[ i ] + ".ini" << endl ;
		ofs << "echo test_filename       " << test + all_path[ i ] + " >> " + config + all_folder[ i ] + "\\" + all_file[ i ] + ".ini" << endl ;
		ofs << "echo result_filename     " << result + all_path[ i ] + " >> " + config + all_folder[ i ] + "\\" + all_file[ i ] + ".ini" << endl ;
	}

	ofs.close( ) ;
	return 0 ;
}
