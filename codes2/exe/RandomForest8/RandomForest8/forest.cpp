#include "forest.h"

Forest::Forest( )
{

}

Forest::Forest( Config config )
{
	this->config = config ;
}

map< string , int > Forest::Train( vector< DataPoint > datapoint )
{
	map< string , int > mp ;

	cout << "training forest now..." << endl ;

	for( int i = 0 ; i < config.tree_num ; i++ )
	{
		Subset subset = MakeSubset( datapoint ) ;

		Tree tree( config ) ;
		
		cout << "[ " << i << " ]" ;

		map< string , int > tmp = tree.Train( subset ) ;
		

		this->tree.push_back( tree ) ;
	}
	cout << endl ;

	return mp ;
}

string Forest::Test( DataPoint datapoint )
{
	map< string , int > mp ;
	
	for( unsigned int i = 0 ; i < tree.size( ) ; i++ )
	{
		mp[ tree[ i ].Test( datapoint ) ]++ ;
		
	}

	string max_classname ;
	int max_cnt = -1 ;

	for( auto itr = mp.begin( ) ; itr != mp.end( ) ; itr++ )
	{
		if( max_cnt < itr->second )
		{
			max_classname = itr->first ;
			max_cnt = itr->second ;
		}
	}

	return max_classname ;
}

Subset Forest::MakeSubset( vector< DataPoint > datapoint )
{
	Subset subset ;

	for( int i = 0 ; i < config.subset_size ; i++ )
	{
		int r  = rand( ) % datapoint.size( ) ;

		if( config.select_type == SELECTTYPE_SINGLE )
		{
			bool is_single = true ;

			for( unsigned int j = 0 ; j < subset.datapoint.size( ) ; j++ )
			{
				if( subset.datapoint[ j ] == datapoint[ r ] )
				{
					is_single = false ;

					break ;
				}
			}

			if( is_single )
			{
				subset.datapoint.push_back( datapoint[ r ] ) ;
			}
		}
		else if( config.select_type == SELECTTYPE_MULTIPLE )
		{
			subset.datapoint.push_back( datapoint[ r ] ) ;
		}
		else	//if( config.select_type == SELECTTYPE_DEFAULT )
		{
			cout << "SELECTTYPE_DEFAULT" << endl ;
			exit( EXIT_FAILURE ) ;
		}
	}

	return subset ;
}