#include "Solver.h"

// Решить СЛАУ, матрица которой представляет собой пятидиагональную матрицу.
void Solver::Solve5Diag(Matrix& A, vector<double>& b, vector<double>& x)
{
	throw exception("Timur not released this method");
}

// Убрать из матрицы узлы, которых нет.
void Solver::DropNodes(Matrix& A, vector<int> nodes)
{
	throw exception("DropNodes");
}

// Учесть краевые условия.
void Solver::Conditions(Matrix& A, condition& x, condition& y, borders& border) 
{
	throw exception("Conditions");
}

// Собрать матрицу (с учетом фиктивных узлов).
void Solver::Make(grid& g, double (*f)(double, double), Matrix& A, vector<double>& b)
{
	
}

// Получить решение МКР.
void Solver::GetSolve(grid& g, double (*f)(double, double), condition& x, condition& y,
	borders& border, vector<int>& needDrop, vector<double>& result)
{
	Matrix A;
	vector<double> b;
	Make(g, f, A, b);
	DropNodes(A, needDrop);
	Conditions(A, x, y, border);
	result.resize(b.size());
	Solve5Diag(A, b, result);
}