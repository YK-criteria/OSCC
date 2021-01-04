#pragma once

#include <string>

using namespace std ;

enum SplitType
{
	SPLITTYPE_DEFAULT = -1 ,
	SPLITTYPE_AXIS ,
	SPLITTYPE_LINEAR ,
} ;

enum SelectType
{
	SELECTTYPE_DEFAULT = -1 ,
	SELECTTYPE_SINGLE ,
	SELECTTYPE_MULTIPLE ,
} ;

class Config
{
public :
	int class_num ;
	int dimension_num ;
	int select_dimension_num ;
	int tree_num ;
	int max_depth ;
	int normal_num ;
	int threshold_num ;
	int train_num ;
	int test_num ;
	int subset_size ;
	int min_subset_size ;
	float min_informationgain ;
	SplitType split_type ;
	SelectType select_type ;
	string train_filename ;
	string test_filename ;
	string result_filename ;

	Config( ) ;
} ;
