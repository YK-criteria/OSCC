#include "tree.h"

Tree::Tree( )
{

}

Tree::Tree( Config config )
{
	this->config = config ;
	root = Node( config ) ;
}

map< string ,  int > Tree::Train( Subset subset )
{
	return root.Train( subset , 0 ) ;
}

string Tree::Test( DataPoint datapoint )
{
	return root.Test( datapoint ) ;
}
