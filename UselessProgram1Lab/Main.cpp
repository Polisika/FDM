#include <iostream>
#include <fstream>
#include "Generator.h"
#include "Structs.h"
#include "Solver.h"
#include "input.h"

void get_solve(double w, double prec, int maxiters)
{
	grid g;
	generate_grid(0.2, 1, 1, 0, 0, 1, 1, 5, 4, g);
	vector<int> need_drop({ 2 });
	g.display(cout);
	cout << endl;

	ifstream inputBorder("input_border.txt");
	borders border;
	inputBord(inputBorder, border);
	inputBorder.close();
	border.display(cout);
	cout << endl;

	ifstream inputConditions("input_condition.txt");
	condition x;
	inputCond(inputConditions, x);
	inputConditions.close();
	x.display(cout);
	cout << endl;

	vector<double> res;
	Solver s;
	s.GetSolve(g, [](double x, double y) { return x * x - 2.; }, x, border, need_drop, res, prec, w, maxiters);
	for (int i = 0; i < res.size(); i++)
		cout << res[i] << " ";
}

int main()
{
	get_solve(1, 1e-13, 1000);
}
