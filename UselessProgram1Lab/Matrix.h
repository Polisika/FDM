#pragma once
#include <vector>
#include <ostream>

using namespace std;

class Matrix
{
public:
	vector<vector<double>> values;
	void display(ostream&);
};