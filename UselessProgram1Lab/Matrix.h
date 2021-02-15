#pragma once
#include <vector>
#include <ostream>

using namespace std;

class Matrix
{
private:
	int size;

public:
	Matrix(int _size, int diags, int _m)
	{
		size = _size;
		m = _m;
		values.resize(diags);
		for (int i = 0; i < diags; i++)
			values[i].resize(size);

		ig.resize(2);
		ig[0] = 1;

		for (int i = 1; i < ig.size(); i++)
			ig[i] = m + i + 1;
	}

	int m; // ’ранит смещение относительно диагонали.
	vector<int> ig;
	vector<vector<double>> values;
	void display(ostream&);

	int get_size();
};