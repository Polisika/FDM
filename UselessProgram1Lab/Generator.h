#pragma once
#include "Structs.h"

// Сгенерировать квадрат шириной width, высотой height
// Коэффициент разрядки действует слева направо и снизу вверх
void generate_grid(double step, double kx, double ky, double x0, double y0,
											double lambda, double gamma,
											int width, int height, grid& out);
