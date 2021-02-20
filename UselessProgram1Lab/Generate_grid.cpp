#include "Generator.h"
#include <algorithm>

void generate_grid(double step, double kx, double ky, double x0, double y0,
											double lambda, double gamma,
											int width, int height, grid& out)
{
	out.kx = kx;
	out.ky = ky;
	out.lambda = lambda;
	out.gamma = gamma;
	out.step = step;
	out.num_nodes = width * height;
	out.width = width;
	out.height = height;
	out.x.resize(out.num_nodes);
	out.y.resize(out.num_nodes);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			out.x[i*width + j] = x0 + step * j * kx;
			out.y[i*width + j] = y0 + step * i * ky;
		}
	}
}

/*void generate_border(grid& g, borders& border, vector<int> need_drop)
{
	int cursor = 0;
	for (auto& drop : need_drop)
	{
		if (find(need_drop.begin(), need_drop.end(), drop) != need_drop.end())
		{
			// Найден
			// Отгрузить как конец границы
			// Начать писать в другую границу, инвертированную
		}
		{
			// Не найден
		}
	}
} */