#pragma once

#include <vector>
#include "Matrix.h"

double norm(const std::vector<double>& x);

double relative_residual(Matrix& A, const std::vector<double>& b, const std::vector<double> &xk);