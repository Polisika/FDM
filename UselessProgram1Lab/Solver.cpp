#include "Solver.h"
int diags = 5;

// ������ ����, ������� ������� ������������ ����� ���������������� �������.
void Solver::Solve5Diag(Matrix& A, vector<double>& b, vector<double>& x)
{
	throw exception("Timur not released this method");
}

// ������ �� ������� ����, ������� ���.
void Solver::DropNodes(Matrix& A, vector<double>& b, vector<int> nodes)
{
	for (auto& node : nodes)
	{
		for (int i = 0; i < A.values.size(); i++)
			A.values[i][node] = 0;
		b[node] = 0;
	}
}

// ������ ������� �������.
void Solver::Conditions(Matrix& A, condition& x, condition& y, borders& border, vector<double>& b, grid& g) 
{
	if (x.num_cond == 1)
	{
		for (auto& border : border.bordx)
		{
			for (auto& node : border)
			{
				for (int i = 0; i < A.values.size(); i++)
					A.values[i][node] = 0;
				A.values[2][node] = 1;
				b[node] = x.getValue(g.x[node], g.y[node]);
			}
		}
	}
	else
		throw exception("Conditions");
}

// ������� ������� (� ������ ��������� �����).
// ������� f - ������� ������ �����.
// b - ������ ����� ���������.
void Solver::Make(grid& g, double (*f)(double, double), Matrix& A, vector<double>& b)
{
	int end_node = g.width * (g.height - 1) - 1;
	for (int i = g.width + 1; i < end_node; i++)
	{
		int border_feature = (i + 1) % g.width; // +1 ��� ��� ��������� � ����
		if (border_feature != 1 && border_feature != 0)
		{
			double hxi_1 = g.x[i] - g.x[i - 1],
				hxi = g.x[i + 1] - g.x[i],
				hyj_1 = g.y[i] - g.y[i - 1],
				hyj = g.y[i + 1] - g.x[i];
			// ��� i-�� �������� ���������� ��������� ������ �������.
			// ����� ������� ������ (- width).
			A.values[0][i] = 2.0 / (hyj_1 * (hyj + hyj_1));
			// ����� (-1),
			A.values[1][i] = 2.0 / (hxi_1 * (hxi + hxi_1));
			// �� ��������
			A.values[2][i] = -((2.0 / (hxi_1 * hxi)) + (2.0 / (hyj_1 * hyj)));
			// ������ (+1), 
			A.values[3][i] = 2.0 / (hxi * (hxi + hxi_1));
			// ������� (+ width), 
			A.values[4][i] = 2.0 / (hyj * (hyj + hyj_1));

			// ��������� ������ �����
		}	b[i] = f(g.x[i], g.y[i]);
	}
}

// �������� ������� ���.
void Solver::GetSolve(grid& g, double (*f)(double, double), condition& x, condition& y,
	borders& border, vector<int>& needDrop, vector<double>& result)
{
	Matrix A(g.height * g.width, diags, g.width);
	vector<double> b(g.height * g.width);
	Make(g, f, A, b);
	DropNodes(A, b, needDrop);
	Conditions(A, x, y, border, b);
	result.resize(b.size());
	Solve5Diag(A, b, result);
}