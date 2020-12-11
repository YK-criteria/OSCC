#include "config.h"

Config::Config( )
{
	class_num = 0 ;
	dimension_num = 0 ;
	select_dimension_num = 0 ;
	tree_num = 0 ;
	max_depth = 0 ;
	normal_num = 0 ;
	threshold_num = 0 ;
	train_num = 0 ;
	test_num = 0 ;
	subset_size = 0 ;
	min_subset_size = 0 ;
	min_informationgain = 0.f ;
	split_type = SPLITTYPE_DEFAULT ;
	select_type = SELECTTYPE_DEFAULT ;
	train_filename = "" ;
	test_filename = "" ;
	result_filename = "" ;
}
