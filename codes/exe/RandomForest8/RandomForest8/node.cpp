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
	//これ以上深く木を成長させないから終了
	if( config.max_depth < depth )
	{
		//サブセットから最も多いクラスをこのノードのクラスに設定する
		classname = GetClassName( subset ) ;

		//終了条件設定
		map< string , int > mp ;
		mp[ "max_depth" ]++ ;

		return mp ;
	}

	//サブセットが小さすぎるから終了
	if( ( signed )subset.datapoint.size( ) <= config.min_subset_size )
	{
		//サブセットから最も多いクラスをこのノードのクラスに設定する
		classname = GetClassName( subset ) ;

		//終了条件設定
		map< string , int > mp ;
		mp[ "min_subset_size" ]++ ;

		return mp ;
	}

	//パラメータの範囲で最良の分割関数を探索
	Split best ;
	float max_informationgain = -1.f ;
	for( int i = 0 ; i < config.normal_num ; i++ )
	{
		Split candidate ;	//分割候補

		//法線ベクトルをランダムに生成
		candidate.normal = MakeNormal( subset ) ;

		for( int j = 0 ; j < config.threshold_num ; j++ )
		{
			//分割候補の閾値
			candidate.threshold = MakeThreshold( subset , candidate.normal ) ;

			float informationgain = CalcInformationGain( subset , candidate ) ;
			//今までで最も情報利得が高い
			if( max_informationgain < informationgain )
			{
				//最良の分割関数を更新
				best = candidate ;

				//最大情報利得を更新
				max_informationgain = informationgain ;
			}
		}
	}
	
	//最小情報利得が下回っているから分割しない
	if( max_informationgain < config.min_informationgain )
	{
		//サブセットから最も多いクラスをこのノードのクラスに設定する
		classname = GetClassName( subset ) ;

		//終了条件設定
		map< string , int > mp ;
		mp[ "min_informationgain" ]++ ;

		return mp ;
	}

	//今の最良の分割をこのノードの分割関数に設定
	split = best ;

	//サブセットを分割
	Subset left_subset , right_subset ;
	left_subset = SeparateSubset( subset , true ) ;
	right_subset = SeparateSubset( subset , false ) ;

	//子ノードをインスタンス化
	Node left_node( config ) ;
	Node right_node( config ) ;
	
	//子ノード訓練
	map< string , int > left_mp = left_node.Train( left_subset, depth + 1 ) ;
	map< string , int > right_mp = right_node.Train( right_subset, depth + 1 ) ;

	child.push_back( left_node ) ;
	child.push_back( right_node ) ;

	//mapの統合
	map< string , int > mp ;
	//左の子から得られたmap
	for( auto itr = left_mp.begin( ) ; itr != left_mp.end( ) ; itr++ )
	{
		mp[ itr->first ] += itr->second ;
	}
	//右の子から得られたmap
	for( auto itr = right_mp.begin( ) ; itr != right_mp.end( ) ; itr++ )
	{
		mp[ itr->first ] += itr->second ;
	}

	return mp ;
}

string Node::GetClassName( Subset subset )
{
	map< string , int > mp ;	//クラス名と出現回数の連想配列

	//クラス名の出現回数を統合
	for( unsigned int i = 0 ; i < subset.datapoint.size( ) ; i++ )
	{
		mp[ subset.datapoint[ i ].classname ]++ ;
	}

	string max_classname ;	//最頻出のクラス名
	int max_cnt = 0 ;	//最頻出のクラスの出現回数
	for( auto itr = mp.begin( ) ; itr != mp.end( ) ; itr++ )
	{
		//今までで最も最頻出なクラスを発見
		if( max_cnt < itr->second )
		{
			//最頻出クラスの更新
			max_classname = itr->first ;
			max_cnt = itr->second ;
		}
	}

	return max_classname ;
}

vector< float > Node::MakeNormal( Subset subset )
{
	vector< float > v ;

	//軸切り
	if( config.split_type == SPLITTYPE_AXIS )
	{
		int r = rand( ) % config.dimension_num ;

		//前半の0を詰める
		for( int i = 0 ; i < r ; i++ )
		{
			v.push_back( 0.f ) ;
		}

		//-1～+1の数値を割り当て
		float f = RandFloat( -1.f , 1.f ) ;
		v.push_back( f ) ;

		//後半の0を詰める
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

	//サブセット内のデータ点と法線ベクトルとの内積を計算
	for( unsigned int i = 0 ; i < subset.datapoint.size( ) ; i++ )
	{
		dot.push_back( Dot( normal , subset.datapoint[ i ] ) ) ;
	}

	//最小値と最大値を探索
	float min = dot[ 0 ] ;
	float max = dot[ 0 ] ;
	for( unsigned int i = 1 ; i < dot.size( ) ; i++ )
	{
		//より小さい内積を発見
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
	//分割前の各クラスの出現回数を集計
	map< string , int > pre_mp ;
	for( unsigned int i = 0 ; i < subset.datapoint.size( ) ; i++ )
	{
		pre_mp[ subset.datapoint[ i ].classname ]++ ;
	}

	//分割前のエントロピー
	float pre_etp = 0.f ;
	//分割前のエントロピー計算
	for( auto itr = pre_mp.begin( ) ; itr != pre_mp.end( ) ; itr++ )
	{
		float p = ( float )itr->second / ( float )subset.datapoint.size( ) ;
		pre_etp += - p * Log2( p ) ;
	}

	//分割後のそれぞれのデータ点
	map< string , int > left_mp , right_mp ;
	int left_cnt = 0 ,  right_cnt = 0 ;
	//分割及び分割後のそれぞれのグループの各クラスの出現回数を集計
	for( unsigned int i = 0 ; i < subset.datapoint.size( ) ; i++ )
	{
		//データ点の内積結果が閾値未満
		if( Dot( split.normal , subset.datapoint[ i ] ) < split.threshold )
		{
			left_mp[ subset.datapoint[ i ].classname ]++ ;
			left_cnt++ ;
		}
		//閾値以上
		else
		{
			right_mp[ subset.datapoint[ i ].classname ]++ ;
			right_cnt++ ;
		}
	}

	//分割後のエントロピー
	float post_etp = 0.f ;
	//分割後の左側のエントロピー
	for( auto itr = left_mp.begin( ) ; itr != left_mp.end( ) ; itr++ )
	{
		float p = left_cnt == 0 ? 0 : ( float )itr->second / ( float )left_cnt ;
		post_etp += - p * Log2( p ) * ( ( float )left_cnt / ( float )subset.datapoint.size( ) );
	}
	//分割後の右側のエントロピー
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
		//小さいフラグかつ内積が小さいなら新しいサブセットに追加 逆も然り
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
	//法線ベクトルの次元数とデータ点の次元数が異なる 　念のため確認しておく
	if( normal.size( ) != datapoint.feature.size( ) )
	{
		cout << "in Dot , dimension is differ.\n" << endl ;
		exit( EXIT_FAILURE ) ;
	}

	//内積計算
	float dot = 0.f ;
	for( unsigned int i = 0 ; i < normal.size( ) ; i++ )
	{
		dot += normal[ i ] * datapoint.feature[ i ] ;
	}

	return dot ;
}

string Node::Test( DataPoint datapoint )
{
	//子ノードがない
	if( child.size( ) == 0 )
	{	
		return classname ;
	}
	else
	{
		//分割関数による分割で閾値をした回る
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