#pragma region Includes
#include <iostream>
#include <fstream>
#include "Generator.h"
#include "Structs.h"
#include "Solver.h"
#include "input.h"
#include <map>
#include <string>
#include <iomanip>
#pragma endregion

#pragma region Func
void get_solve()
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
	s.GetSolve(g, [](double x, double y) { return x * x - 2.; }, x, border, need_drop, res, 1e-13, 1, 100000);
	for (int i = 0; i < res.size(); i++)
		cout << res[i] << " ";
}

// ������ �������� �������
// params ��������� � main
void get_grid(map<string, double> ParamsGrid, grid& g)
{
	generate_grid(ParamsGrid["step"], ParamsGrid["kx"], ParamsGrid["ky"], ParamsGrid["x0"],
		ParamsGrid["y0"], ParamsGrid["lambda"], ParamsGrid["gamma"], ParamsGrid["width"], ParamsGrid["height"], g);
}

// ������ �������� �������
void get_cond(int x, int y, double (*u)(double, double), double (*dudx)(double, double), double (*dudy)(double, double),
	grid& g, borders& b, condition& cond)
{
	if (x == y && x == 1)
		generate_cond11(g, b, cond, u);
	else if (x == y && x == 2)
		generate_cond21(g, b, cond, dudx, u);
	else if (x == 1 && y == 2)
		generate_cond12(g, b, cond, u, dudy);
	else if (y == 2 && x == 2)
		generate_cond21(g, b, cond, dudx, dudy);
	else
		throw invalid_argument("Invalid number combination of conditions.");
}
#pragma endregion

int main()
{
	#pragma region ������� �����
	// ������� ���������
	map<string, double> params = { 
		{"step", 0.2},
		{"kx", 1},
		{"ky", 1},
		{"x0", 0},
		{"y0", 0},
		{"lambda", 1},
		{"gamma", 1},
		{"width", 5},
		{"height", 4},
	};
	grid g;
	get_grid(params, g);
	#pragma endregion

	#pragma region ������� ������
	borders border;
	vector<int> needDrop({
		// ����� ����� ������� ����, ������� ������ �������� �� �����
		2
		});
	generate_border(g, border, needDrop);
	#pragma endregion

	#pragma region ������� ������� ������� (������������� �������, � �����������)
	double (*u)(double x, double y) =	// u - ������������� �������
		[](double x, double y)
		{ return x * x; };

	double (*dudx)(double x, double y) =	// du - ����������� ������������� ������� �� ���������������� ����������
		[](double x, double y)					// ���� ���������� - ������ � ����������� ��� �� �������
	{ return 2. * x; };							// ����� ������� ������ ������� - ������ ����� ����� ����������

	double (*dudy)(double x, double y) =	// du - ����������� ������������� ������� �� ���������������� ����������
		[](double x, double y)					// ���� ���������� - ������ � ����������� ��� �� �������
	{ return 0.; };								// ����� ������� ������ ������� - ������ ����� ����� ����������

	condition cond;
	int num_x_cond = 1; // ����� ��������, ����� ������������ x
	int num_y_cond = 1; // ����� ��������, ����� ������������ �
	get_cond(num_x_cond, num_y_cond, u, dudx, dudy, g, border, cond);
	#pragma endregion

	#pragma region Some magic...
	vector<double> result;
	Solver s;
	s.GetSolve(g, 
		[](double x, double y)
		{ return x * x - 2.; }, // ������� f ������ �����
		cond, border, needDrop, result, 1e-13, 1, 100000);
	#pragma endregion

	#pragma region ������ �����������
	cout << setprecision(5) << scientific << setw(15);
	cout << "Result \t\tanalytic \terror" << endl;

	for (int i = 0; i < result.size(); i++)
	{
		cout << result[i] << "\t";
		double val = u(g.x[i], g.y[i]);

		// ���� ������ �������, �� � ��������, � ����������� ����.
		if (find(needDrop.begin(), needDrop.end(), i) != needDrop.end())
		{
			cout << 0. << "\t" << 0. << endl;
		}
		else
		{
			cout << val << "\t";
			cout << result[i] - val << endl;
		}
	}
	cout << endl;

	#pragma endregion
}
