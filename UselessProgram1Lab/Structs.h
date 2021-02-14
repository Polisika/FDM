#pragma once
#include <vector>
#include <tuple>

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
};

struct borders
{
	int num_x, num_y;
	vector<tuple<vector<int>, bool>> bordy; // Границы, параллельные y
	vector<tuple<vector<int>, bool>> bordx; // Границы, параллельные х

	bool belongs_bordy(int num_node)
	{
		for (auto& bord : bordy)
			for (auto& node : get<0>(bord))
				if (node == num_node)
					return true;

		return false;
	}

	bool belongs_bordx(int num_node)
	{
		for (auto& bord : bordx)
			for (auto& node : get<0>(bord))
				if (node == num_node)
					return true;

		return false;
	}
};

struct condition
{
	int num_cond;
	double (*getValue)(double, double);
	condition(int cond, double (*func)(double, double)) : getValue(func) 
	{
		if (cond != 1 && cond != 2)
			throw exception("Number condition have to be 1 or 2");
		else
			num_cond = cond;
	}
};
