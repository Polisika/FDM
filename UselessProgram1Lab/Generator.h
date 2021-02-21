#pragma once
#include "Structs.h"

// Сгенерировать квадрат шириной width, высотой height
// Коэффициент разрядки действует слева направо и снизу вверх
void generate_grid(double step, double kx, double ky, double x0, double y0,
											double lambda, double gamma,
											int width, int height, grid& out);

void generate_border(grid& g, borders& border, vector<int> need_drop);

void generate_cond11(grid& g, borders& b, condition& cond, double (*u)(double, double));
void generate_cond12(grid& g, borders& b, condition& cond, double (*u)(double, double), double (*du)(double, double));
void generate_cond21(grid& g, borders& b, condition& cond, double (*u)(double, double), double (*du)(double, double));
void generate_cond22(grid& g, borders& b, condition& cond, double (*dudx)(double, double), double (*dudy)(double, double));
