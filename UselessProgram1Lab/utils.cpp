#include "utils.h"

double norm(const std::vector<double>& x)
{
    double res = 0;

    for (const auto& el : x)
        res += el * el;

    return sqrt(res);
}

double relative_residual(Matrix& A, const std::vector<double>& b, const std::vector<double>& xk)
{
    double sum = 0;

    for (int i = 0; i < A.get_size(); i++)
    {
        // Главная диагональ
        double elem = A.values[2][i] * xk[i];

        // Нижний треугольник
        if (i > A.ig[0] - 1)
            elem += A.values[1][i] * xk[i - A.ig[0]];

        if (i > A.ig[1] - 1)
            elem += A.values[0][i] * xk[i - A.ig[1]];

        // Верхний треугольник
        if (i < A.get_size() - A.ig[0])
            elem += A.values[3][i] * xk[i + A.ig[0]];

        if (i < A.get_size() - A.ig[1])
            elem += A.values[4][i] * xk[i + A.ig[1]];

        elem = b[i] - elem;
        sum += elem * elem;
    }
    
    return sqrt(sum) / norm(b);
}
