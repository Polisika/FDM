#pragma once
#include "Structs.h"

// ������������� ������� ������� width, ������� height
// ����������� �������� ��������� ����� ������� � ����� �����
void generate_grid(double step, double kx, double ky, double x0, double y0,
										int width, int height, grid& out);