#pragma once
#include <vector>
#include <tuple>
#include <ostream>
#include <set>

using namespace std;


// Номеру узла соответствуют координаты в x и y массивах. 
struct grid
{
	double lambda, gamma;
	int num_nodes;
	double step;
	// Коэффициент разрядки
	double kx, ky;
	vector<double> x;
	vector<double> y;
	int width, height;

	void display(ostream& out)
	{
		out << width << "x" << height << endl;
		out << "step: " << step << endl;
		out << "x: ";
		for (int i = 0; i < x.size(); i++)
			out << x[i] << " ";
		out << endl;

		out << "y: ";
		for (int i = 0; i < x.size(); i++)
			out << y[i] << " ";
		out << endl;

		out << "lam: " << lambda << endl;
		out << "gamma: " << gamma << endl;
	}
};

struct borders
{
	int num_x, num_y;
	vector<tuple<vector<int>, bool>> bordy; // Границы, параллельные y
	vector<tuple<vector<int>, bool>> bordx; // Границы, параллельные х

	void display(ostream& out)
	{
		for (int i = 0; i < num_x; i++)
		{
			out << i << " border x: ";
			auto b = get<0>(bordx[i]);
			for (int j = 0; j < b.size(); j++)
				out << b[j] << " ";
			out << (get<1>(bordx[i]) ? "inverted" : "not inverted");
			out << endl;
		}

		for (int i = 0; i < num_y; i++)
		{
			out << i << " border y: ";
			auto b = get<0>(bordy[i]);
			for (int j = 0; j < b.size(); j++)
				out << b[j] << " ";
			out << (get<1>(bordy[i]) ? "inverted" : "not inverted");
			out << endl;
		}
	}
};

struct condition
{
	int xnum_cond, ynum_cond;
	vector<vector<double>> values;
	vector<vector<int>> borders;
	void display(ostream& out)
	{
		out << "Conditions x: " << xnum_cond << " y: " << ynum_cond << endl;
		for (int i = 0; i < values.size(); i++)
		{
			out << "Values, under - " << i << " border: " << endl;
			for (int j = 0; j < borders[i].size(); j++)
				out << borders[i][j] << " ";
			out << endl;
			for (int j = 0; j < values[i].size(); j++)
				out << values[i][j] << " ";
			out << endl;
		}
	}

	bool similar(vector<int> a, vector<int> b)
	{
		set<int> first(a.begin(), a.end());
		set<int> second(b.begin(), b.end());
		if (first == second)
			return true;
		else
			return false;
	}
};
