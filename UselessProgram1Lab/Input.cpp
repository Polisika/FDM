#include "input.h"

using namespace std;

// Шаг, количество узлов (включая фиктивные), коэф. разрядки x, y (если сетка
// равномерна, то kx=ky=1), ширина и длина квадрата, лямбда и гамма
// узлы (x y) перечисняются, начиная слева 
// снизу, шагая направо до упора, потом возвращаясь снова, но на строку выше.
void inputGrid(istream& input, grid& out)
{
	input >> out.step;
	input >> out.num_nodes;
	input >> out.kx >> out.ky;
	input >> out.width >> out.height;
	input >> out.lambda;
	input >> out.gamma;

	out.x.resize(out.num_nodes);
	out.y.resize(out.num_nodes);

	for (int i = 0; i < out.num_nodes; i++)
	{
		input >> out.x[i];
		input >> out.y[i];
	}
}

void inputCond(istream& input, condition& out)
{
	input >> out.xnum_cond >> out.ynum_cond;
	int num;
	// Количество границ
	input >> num; 
	out.borders.resize(num);
	out.values.resize(num);
	for (int i = 0; i < num; i++)
	{
		int node; 
		// Количество узлов
		input >> node;
		for (int j = 0; j < node; j++)
		{
			// Узел
			int n;
			input >> n;
			out.borders[i].push_back(n);
			// Значение в этой точке
			double value;
			input >> value;
			out.values[i].push_back(value);
		}
	}
}

void inputBord(istream& input, borders& out)
{
	// Количество границ по оси х
	input >> out.num_x;
	vector<int> buffx(out.num_x);

	// Количество границ по оси у
	input >> out.num_y;
	vector<int> buffy(out.num_y);

	out.bordx.resize(out.num_x);
	out.bordy.resize(out.num_y);
	// Записываем границы
	for (int i = 0; i < out.num_x; i++)
	{
		// Инвертированность и количество узлов на границе
		bool inverted;
		int count_nodes;
		input >> inverted;
		input >> count_nodes;
		buffx.resize(count_nodes);
		// Записываем узлы в границу
		for (int j = 0; j < count_nodes; j++)
			input >> buffx[j];

		out.bordx[i] = make_tuple(buffx, inverted);
		buffx.clear();
	}

	for (int i = 0; i < out.num_y; i++)
	{
		// Инвертированность и количество узлов на границе
		bool inverted;
		int count_nodes;
		input >> inverted;
		input >> count_nodes;
		buffy.resize(count_nodes);
		// Записываем узлы в границу
		for (int j = 0; j < count_nodes; j++)
			input >> buffy[j];

		out.bordy[i] = make_tuple(buffy, inverted);
		buffy.clear();
	}
}