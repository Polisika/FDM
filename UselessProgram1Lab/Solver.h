#pragma once
#include "Matrix.h"
#include "Structs.h"
#include "utils.h"

class Solver
{
private:
    void step(Matrix& A, const std::vector<double>& xk, const std::vector<double>& xk_1, std::vector<double>& y,
        const std::vector<double>& b, const double& w);
public:
    void Solve5Diag(Matrix& A, vector<double>& b, vector<double>& x, const double& eps, const double& w, 
        const int& max_iter);
    void DropNodes(Matrix& A, vector<double>& b, vector<int> nodes);
    void GetSolve(grid& grid, double (*f)(double, double), condition& left, condition& right, borders& border, 
        vector<int>& needDrop, vector<double>& result, const double& eps, const double& w, const int& max_iter);
    void Conditions(Matrix&, condition&, condition&, borders&, vector<double>& b, grid& g);
    void Make(grid&, double (*f)(double, double), Matrix&, vector<double>&);
};