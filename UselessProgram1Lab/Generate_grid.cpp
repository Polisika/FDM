#include "Generator.h"

void generate_grid(double step, double kx, double ky, double x0, double y0,
											double lambda, double gamma,
											int width, int height, grid& out)
{
	out.lambda = lambda;
	out.gamma = gamma;
	out.step = step;
	out.num_nodes = width * height;
	out.x.resize(out.num_nodes);
	out.y.resize(out.num_nodes);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			out.x[j] = x0 + step * j * kx;
			out.y[j] = y0 + step * i * ky;
		}
	}
}