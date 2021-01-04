#include "node.h"

Node::Node( )
{

}

Node::Node( Config config )
{
	this->config = config ;
}

map< string , int > Node::Train( Subset subset , int depth )
{
	if( config.max_depth < depth )
	{
		classname = GetClassName( subset ) ;

		map< string , int > mp ;
		mp[ "max_depth" ]++ ;

		return mp ;
	}

	if( ( signed )subset.datapoint.size( ) <= config.min_subset_size )
	{
		classname = GetClassName( subset ) ;

		map< string , int > mp ;
		mp[ "min_subset_size" ]++ ;

		return mp ;
	}

	Split best ;
	float max_informationgain = -1.f ;
	for( int i = 0 ; i < config.normal_num ; i++ )
	{
		Split candidate ;

		candidate.normal = MakeNormal( subset ) ;

		for( int j = 0 ; j < config.threshold_num ; j++ )
		{
			candidate.threshold = MakeThreshold( subset , candidate.normal ) ;

			float informationgain = CalcInformationGain( subset , candidate ) ;
			if( max_informationgain < informationgain )
			{
				best = candidate ;

				max_informationgain = informationgain ;
			}
		}
	}
	
	if( max_informationgain < config.min_informationgain )
	{
		classname = GetClassName( subset ) ;

		map< string , int > mp ;
		mp[ "min_informationgain" ]++ ;

		return mp ;
	}

	split = best ;

	Subset left_subset , right_subset ;
	left_subset = SeparateSubset( subset , true ) ;
	right_subset = SeparateSubset( subset , false ) ;

	Node left_node( config ) ;
	Node right_node( config ) ;
	
	map< string , int > left_mp = left_node.Train( left_subset, depth + 1 ) ;
	map< string , int > right_mp = right_node.Train( right_subset, depth + 1 ) ;

	child.push_back( left_node ) ;
	child.push_back( right_node ) ;

	map< string , int > mp ;
	for( auto itr = left_mp.begin( ) ; itr != left_mp.end( ) ; itr++ )
	{
		mp[ itr->first ] += itr->second ;
	}
	for( auto itr = right_mp.begin( ) ; itr != right_mp.end( ) ; itr++ )
	{
		mp[ itr->first ] += itr->second ;
	}

	return mp ;
}

string Node::GetClassName( Subset subset )
{
	map< string , int > mp ;

	for( unsigned int i = 0 ; i < subset.datapoint.size( ) ; i++ )
	{
		mp[ subset.datapoint[ i ].classname ]++ ;
	}

	string max_classname ;
	int max_cnt = 0 ;
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

vector< float > Node::MakeNormal( Subset subset )
{
	vector< float > v ;

	if( config.split_type == SPLITTYPE_AXIS )
	{
		int r = rand( ) % config.dimension_num ;

		for( int i = 0 ; i < r ; i++ )
		{
			v.push_back( 0.f ) ;
		}

		float f = RandFloat( -1.f , 1.f ) ;
		v.push_back( f ) ;

		for( int i = r + 1 ; i < config.dimension_num ; i++ )
		{
			v.push_back( 0.f ) ;
		}
	}
	else if( config.split_type == SPLITTYPE_LINEAR )
	{
		cout << "linear split is nt inplemented." << endl ;
		exit( EXIT_FAILURE ) ;
	}

	return v ;
}

float Node::RandFloat( float min , float max  )
{
	return min + ( float )rand( ) / ( float )RAND_MAX * ( max - min ) ; 
}

float Node::MakeThreshold( Subset subset ,  vector< float > normal )
{
	vector< float > dot ;

	for( unsigned int i = 0 ; i < subset.datapoint.size( ) ; i++ )
	{
		dot.push_back( Dot( normal , subset.datapoint[ i ] ) ) ;
	}

	float min = dot[ 0 ] ;
	float max = dot[ 0 ] ;
	for( unsigned int i = 1 ; i < dot.size( ) ; i++ )
	{
		if( dot[ i ] < min )
		{
			min = dot[ i ] ;
		}

		if( max < dot[ i ] )
		{
			max = dot[ i ] ;
		}
	}

	return RandFloat( min + 0.0001f , max - 0.0001f ) ;
}

float Node::CalcInformationGain( Subset subset , Split split )
{
	map< string , int > pre_mp ;
	for( unsigned int i = 0 ; i < subset.datapoint.size( ) ; i++ )
	{
		pre_mp[ subset.datapoint[ i ].classname ]++ ;
	}

	float pre_etp = 0.f ;
	for( auto itr = pre_mp.begin( ) ; itr != pre_mp.end( ) ; itr++ )
	{
		float p = ( float )itr->second / ( float )subset.datapoint.size( ) ;
		pre_etp += - p * Log2( p ) ;
	}

	map< string , int > left_mp , right_mp ;
	int left_cnt = 0 ,  right_cnt = 0 ;
	for( unsigned int i = 0 ; i < subset.datapoint.size( ) ; i++ )
	{
		if( Dot( split.normal , subset.datapoint[ i ] ) < split.threshold )
		{
			left_mp[ subset.datapoint[ i ].classname ]++ ;
			left_cnt++ ;
		}
		else
		{
			right_mp[ subset.datapoint[ i ].classname ]++ ;
			right_cnt++ ;
		}
	}

	float post_etp = 0.f ;
	for( auto itr = left_mp.begin( ) ; itr != left_mp.end( ) ; itr++ )
	{
		float p = left_cnt == 0 ? 0 : ( float )itr->second / ( float )left_cnt ;
		post_etp += - p * Log2( p ) * ( ( float )left_cnt / ( float )subset.datapoint.size( ) );
	}
	for( auto itr = right_mp.begin( ) ; itr != right_mp.end( ) ; itr++ )
	{
		float p = right_cnt == 0 ? 0 : ( float )itr->second / ( float )right_cnt ;
		post_etp += - p * Log2( p ) * ( ( float )right_cnt / ( float )subset.datapoint.size( ) );
	}

	return pre_etp - post_etp ;
}

float Node::Log2( float x )
{
	return x == 0.f ? 0.f : log( x ) /  log( 2.f ) ;
}

Subset Node::SeparateSubset( Subset subset , bool is_lower )
{
	Subset new_subset ;

	for( unsigned int i = 0 ; i < subset.datapoint.size( ) ; i++ )
	{
		if( is_lower && Dot( split.normal , subset.datapoint[ i ] ) < split.threshold ||
			!is_lower && split.threshold <= Dot( split.normal , subset.datapoint[ i ] ) )
		{
			new_subset.datapoint.push_back( subset.datapoint[ i ] ) ;
		}
	}

	return new_subset ;
}

float Node::Dot( vector< float > normal , DataPoint datapoint )
{
	if( normal.size( ) != datapoint.feature.size( ) )
	{
		cout << "in Dot , dimension is differ.\n" << endl ;
		exit( EXIT_FAILURE ) ;
	}

	float dot = 0.f ;
	for( unsigned int i = 0 ; i < normal.size( ) ; i++ )
	{
		dot += normal[ i ] * datapoint.feature[ i ] ;
	}

	return dot ;
}

string Node::Test( DataPoint datapoint )
{
	if( child.size( ) == 0 )
	{	
		return classname ;
	}
	else
	{
		if( Dot( split.normal , datapoint ) < split.threshold )
		{
			return child[ 0 ].Test( datapoint )  ;
		}
		else
		{
			return child[ 1 ].Test( datapoint ) ;
		}
	}
}

void Node::Print( int depth )
{
	for( int i = 0 ; i < depth ; i++ )
	{
		cout << " " << endl ;
	}
	cout << "." << " classname is " << classname ;

	if( child.size( ) != 0 )
	{
		child[ 0 ].Print( depth + 1 ) ;
		child[ 1 ].Print( depth + 1 ) ;
	}
}