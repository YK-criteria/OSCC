#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <Windows.h>

using namespace std;

class Dir
{
public:
	vector< string > ReadFile( string folder ) ;
	vector< string > ReadFolder( string folder ) ;
};