#pragma once
#include "Matrix.h"
#include "Structs.h"

class Solver
{
	void Solve5Diag(Matrix& A, vector<double>& b, vector<double>& x);
	void DropNodes(Matrix& A, vector<double>& b, vector<int> nodes);
	void GetSolve(grid& grid, double (*f)(double, double), condition& left, condition& right,
							borders& border, vector<int>& needDrop, vector<double>& result);
	void Conditions(Matrix&, condition&, condition&, borders&, vector<double>& b, grid& g);
	void Make(grid&, double (*f)(double, double), Matrix&, vector<double>&);
};