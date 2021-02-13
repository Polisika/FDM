#include "Structs.h"
#include <istream>

using namespace std;

// Ўаг, количество узлов (включа€ фиктивные), коэф. разр€дки x, y (если сетка
// равномерна, то kx=ky=1), ширина и длина квадрата, 
// узлы (x y) перечисн€ютс€, начина€ слева 
// снизу, шага€ направо до упора, потом возвраща€сь снова, но на строку выше.
void inputGrid(istream& input, grid& out)
{
	input >> out.step;
	input >> out.num_nodes;
	input >> out.kx >> out.ky;
	input >> out.width >> out.height;

	out.x.resize(out.num_nodes);
	out.y.resize(out.num_nodes);

	for (int i = 0; i < out.num_nodes; i++)
	{
		input >> out.x[i];
		input >> out.y[i];
	}
}

//  оличество границ, перечисление границ (сначала по х, потом по у): 
//   ол-во по оси
//		 оличество узлов, перечисление узлов.
void inputBorder(istream& input, borders& out)
{
	input >> out.num_x;
	out.bordx.resize(out.num_x);

	for (int i = 0; i < out.num_x; i++)
	{
		int num_nodes;
		input >> num_nodes;
		out.bordx[i].resize(num_nodes);

		for (int j = 0; j < num_nodes; j++)
			input >> out.bordx[i][j];
	}

	input >> out.num_y;
	out.bordy.resize(out.num_y);

	for (int i = 0; i < out.num_y; i++)
	{
		int num_nodes;
		input >> num_nodes;
		out.bordx[i].resize(num_nodes);

		for (int j = 0; j < num_nodes; j++)
			input >> out.bordx[i][j];
	}
}

void inputCoeffs(istream& input, double& lambda, double& gamma)
{
	input >> lambda;
	input >> gamma;
}