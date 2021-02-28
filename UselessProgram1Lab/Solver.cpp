#include "Solver.h"
#include <cmath>
#include <algorithm>

int diags = 5;

void Solver::step(Matrix& A, const std::vector<double>& xk, const std::vector<double>& xk_1, std::vector<double>& y, const std::vector<double>& b, const double& w)
{
    for (int i = 0; i < A.get_size(); i++)
    {
        double sum1 = 0, sum2 = A.values[2][i] * xk[i];

        // Нижний треугольник
        if (i > A.ig[0] - 1)
            sum1 += A.values[1][i] * xk_1[i - A.ig[0]];

        if (i > A.ig[1] - 1)
            sum1 += A.values[0][i] * xk_1[i - A.ig[1]];

        // Верхний треугольник
        if (i < A.get_size() - A.ig[0])
            sum2 += A.values[3][i] * xk[i + A.ig[0]];

        if (i < A.get_size() - A.ig[1])
            sum2 += A.values[4][i] * xk[i + A.ig[1]];

        y[i] = xk[i] + w / A.values[2][i] * (b[i] - sum1 - sum2);
    }
}

// Решить СЛАУ, матрица которой представляет собой пятидиагональную матрицу.
void Solver::Solve5Diag(Matrix& A, vector<double>& b, vector<double>& x, const double& eps, const double& w,
    const int& max_iter)
{
    // Начальное приближение - нулевое
    std::fill(x.begin(), x.end(), 0);

    int N = A.get_size(), iter = 0;
    double rr = relative_residual(A, b, x);

    while (rr >= eps && iter < max_iter)
    {
        step(A, x, x, x, b, w);
        rr = relative_residual(A, b, x);

        ++iter;
    }
}

// Убрать из матрицы узлы, которых нет.
void Solver::DropNodes(Matrix& A, vector<double>& b, vector<int> nodes)
{
    for (auto& node : nodes)
    {
        for (int i = 0; i < A.values.size(); i++)
            A.values[i][node] = 0;

        A.values[2][node] = 1;
        b[node] = 0;
    }
}

// Учесть краевые условия.
void Solver::Conditions(Matrix& A, condition& cond, borders& border, vector<double>& b, grid& g)
{
    if (cond.xnum_cond == 1)
    {
        for (auto& bord : border.bordx)
        {
            int index = 0;
            // Нашли индекс в краевых условиях
            for (; index < cond.borders.size(); index++)
                if (cond.similar(get<0>(bord), cond.borders[index]))
                    break;

            // Обнуляем строки в матрице по всем найденным узлам
            // Присваиваем значения в соответствии с порядком
            int j_ = 0;
            for (auto& node : get<0>(bord))
            {
                for (int i = 0; i < A.values.size(); i++)
                    A.values[i][node] = 0;
                A.values[2][node] = 1;
                b[node] = cond.values[index][j_];
                j_++;
            }
        }
    }
    else if (cond.xnum_cond == 2)
    {
        for (auto& bord : border.bordx)
        {
            int index = 0;
            // Нашли индекс в краевых условиях
            for (; index < cond.borders.size(); index++)
                if (cond.similar(get<0>(bord), cond.borders[index]))
                    break;

            // Обнуляем строки в матрице по всем найденным узлам
            // Присваиваем значения в соответствии с порядком
            int j_ = 0;
            for (auto& node : get<0>(bord))
            {
                for (int i = 0; i < A.values.size(); i++)
                    A.values[i][node] = 0;

                if (get<1>(bord))
                {
                    // Тогда учитываем минус
                    int right_node = node + 1;
                    double value = g.lambda / (g.y[right_node] - g.y[node]);
                    A.values[2][node] = value;
                    A.values[2][right_node] = -value;
                }
                else
                {
                    int left_node = node - 1;
                    double value = g.lambda / (g.y[left_node] - g.y[node]);
                    A.values[2][node] = -value;
                    A.values[2][left_node] = value;
                }

                b[node] = cond.values[index][j_];
                j_++;
            }
        }
    }

    if (cond.ynum_cond == 1)
    {
        for (auto& bord : border.bordy)
        {
            int index = 0;
            // Нашли индекс в краевых условиях
            for (; index < cond.borders.size(); index++)
                if (cond.similar(get<0>(bord), cond.borders[index]))
                    break;

            // Обнуляем строки в матрице по всем найденным узлам
            // Присваиваем значения в соответствии с порядком
            int j_ = 0;
            for (auto& node : get<0>(bord))
            {
                for (int i = 0; i < A.values.size(); i++)
                    A.values[i][node] = 0;
                A.values[2][node] = 1;
                b[node] = cond.values[index][j_];
                j_++;
            }
        }
    }
    else if (cond.ynum_cond == 2)
    {
        for (auto& bord : border.bordy)
        {
            int index = 0;
            // Нашли индекс в краевых условиях
            for (; index < cond.borders.size(); index++)
                if (cond.similar(get<0>(bord), cond.borders[index]))
                    break;

            // Обнуляем строки в матрице по всем найденным узлам
            // Присваиваем значения в соответствии с порядком
            int j_ = 0;
            for (auto& node : get<0>(bord))
            {
                for (int i = 0; i < A.values.size(); i++)
                    A.values[i][node] = 0;

                if (get<1>(bord))
                {
                    // Тогда учитываем минус
                    int upper_node = node + g.width;
                    double value = g.lambda / (g.y[upper_node] - g.y[node]);
                    A.values[2][node] = value;
                    A.values[2][upper_node] = -value;
                }
                else
                {
                    int lower_node = node - g.width;
                    double value = g.lambda / (g.y[lower_node] - g.y[node]);
                    A.values[2][node] = -value;
                    A.values[2][lower_node] = value;
                }

                b[node] = cond.values[index][j_];
                j_++;
            }
        }
    }
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
            double hxi_1 = g.step * pow(g.kx, i % g.width),
                hxi = hxi_1 * g.kx,
                hyj_1 = g.step * pow(g.ky, i % g.height),
                hyj = hyj_1 * g.ky;
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
            b[i] = f(g.x[i], g.y[i]);
        }	
    }
}

// Получить решение МКР.
void Solver::GetSolve(grid& g, double (*f)(double, double), condition& cond,
    borders& border, vector<int>& needDrop, vector<double>& result, const double& eps, const double& w,
    const int& max_iter)
{
    Matrix A(g.height * g.width, diags, g.width);
    vector<double> b(g.height * g.width);
    Make(g, f, A, b);
    DropNodes(A, b, needDrop);
    Conditions(A, cond, border, b, g);
    result.resize(b.size());
    Solve5Diag(A, b, result, eps, w, max_iter);
}