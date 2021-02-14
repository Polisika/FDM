#include "Solver.h"
int diags = 5;

// Решить СЛАУ, матрица которой представляет собой пятидиагональную матрицу.
void Solver::Solve5Diag(Matrix& A, vector<double>& b, vector<double>& x)
{
	throw exception("Timur not released this method");
}

// Убрать из матрицы узлы, которых нет.
void Solver::DropNodes(Matrix& A, vector<double>& b, vector<int> nodes)
{
	for (auto& node : nodes)
	{
		for (int i = 0; i < A.values.size(); i++)
			A.values[i][node] = 0;
		b[node] = 0;
	}
}

// Учесть краевые условия.
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

// Собрать матрицу (с учетом фиктивных узлов).
// Функция f - функция правой части.
// b - правая часть уравнения.
void Solver::Make(grid& g, double (*f)(double, double), Matrix& A, vector<double>& b)
{
	int end_node = g.width * (g.height - 1) - 1;
	for (int i = g.width + 1; i < end_node; i++)
	{
		int border_feature = (i + 1) % g.width; // +1 так как нумерация с нуля
		if (border_feature != 1 && border_feature != 0)
		{
			double hxi_1 = g.x[i] - g.x[i - 1],
				hxi = g.x[i + 1] - g.x[i],
				hyj_1 = g.y[i] - g.y[i - 1],
				hyj = g.y[i + 1] - g.x[i];
			// Для i-го элемента необходимо заполнить строку матрицы.
			// Берем сначала нижний (- width).
			A.values[0][i] = 2.0 / (hyj_1 * (hyj + hyj_1));
			// левый (-1),
			A.values[1][i] = 2.0 / (hxi_1 * (hxi + hxi_1));
			// по середине
			A.values[2][i] = -((2.0 / (hxi_1 * hxi)) + (2.0 / (hyj_1 * hyj)));
			// правый (+1), 
			A.values[3][i] = 2.0 / (hxi * (hxi + hxi_1));
			// верхний (+ width), 
			A.values[4][i] = 2.0 / (hyj * (hyj + hyj_1));

			// Заполняем правую часть
		}	b[i] = f(g.x[i], g.y[i]);
	}
}

// Получить решение МКР.
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