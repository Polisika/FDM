#pragma once
#include <vector>
#include <ostream>

using namespace std;

class Matrix
{
public:
	Matrix(int size, int diags, int _m)
	{
		m = _m;
		values.resize(diags);
		for (int i = 0; i < diags; i++)
			values[i].resize(size);
	}
	int m; // ’ранит смещение относительно диагонали.
	vector<vector<double>> values;
	void display(ostream&);
};