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

void generate_border(grid& g, borders& border, vector<int> need_drop)
{
	if (need_drop.empty())
		throw invalid_argument("Need drop is empty.");
	

	border.num_x = border.num_y = 4;
	vector<int> border_contains;
	int node = 0;
	// Левое основание
	while(find(need_drop.begin(), need_drop.end(), node + 1) == need_drop.end())
	{
			// Не найден
			border_contains.push_back(node);
			node++; // Просто перемещаемся дальше по узлам
	}
	border_contains.push_back(node);
	vector<tuple<vector<int>, bool>>* current = &(border.bordy);
	
	// Нужно отгрузить границу в current
	// Граница по х, инвертирована
	border_contains.shrink_to_fit();
	bool inverted = true;
	sort(border_contains.begin(), border_contains.end());
	current->push_back(make_tuple(border_contains, inverted));
	border_contains.clear();

	// Так как форма буквы П, то нужно пройтись по букве п
	// Идем по левой внутренней границе
	while (find(need_drop.begin(), need_drop.end(), node + 1) != need_drop.end())
	{
		border_contains.push_back(node);
		node += g.width;
	}
	border_contains.push_back(node);

	// Нужно отгрузить границу в current
	// Граница по y, не инвертирована
	inverted = false;
	current = &(border.bordx);
	border_contains.shrink_to_fit();
	sort(border_contains.begin(), border_contains.end());
	current->push_back(make_tuple(border_contains, inverted));
	border_contains.clear();

	border_contains.push_back(node);
	node++;
	// Левый верхний (внутри фигуры) узел найден, идем вправо
	while (find(need_drop.begin(), need_drop.end(), node - g.width) != need_drop.end())
	{
		border_contains.push_back(node);
		node++;
	}
	border_contains.push_back(node);

	// Найдена верхняя внутренняя граница
	// Граница по x, инвертирована
	inverted = true;
	current = &(border.bordy);
	border_contains.shrink_to_fit();
	sort(border_contains.begin(), border_contains.end());
	current->push_back(make_tuple(border_contains, inverted));
	border_contains.clear();

	border_contains.push_back(node);
	node -= g.width;
	// Вниз по правой внутренней
	while (find(need_drop.begin(), need_drop.end(), node - 1) != need_drop.end())
	{
		border_contains.push_back(node);
		node -= g.width;
	}
	
	// Правая внутренняя граница
	// Граница инвертирована
	// Граница по у отгружается
	node += g.width;
	inverted = true;
	current = &(border.bordx);
	border_contains.shrink_to_fit();
	sort(border_contains.begin(), border_contains.end());
	current->push_back(make_tuple(border_contains, inverted));
	border_contains.clear();

	// Далее идем по классике
	for (; node < g.width; node++)
		border_contains.push_back(node);

	// Правое основание
	inverted = true;
	current = &(border.bordy);
	border_contains.shrink_to_fit();
	sort(border_contains.begin(), border_contains.end());
	current->push_back(make_tuple(border_contains, inverted));
	border_contains.clear();

	node--;
	// Правая сторона
	for (; node <= g.num_nodes - 1; node+= g.width)
		border_contains.push_back(node);

	inverted = false;
	current = &(border.bordx);
	border_contains.shrink_to_fit();
	sort(border_contains.begin(), border_contains.end());
	current->push_back(make_tuple(border_contains, inverted));
	border_contains.clear();

	node = g.num_nodes - 1;
	// Крышка
	for (; node >= g.num_nodes - g.width; node--)
		border_contains.push_back(node);

	inverted = false;
	current = &(border.bordy);
	border_contains.shrink_to_fit();
	sort(border_contains.begin(), border_contains.end());
	current->push_back(make_tuple(border_contains, inverted));
	border_contains.clear();

	node++;
	// Левая сторона
	for (; node >= 0; node-= g.width)
		border_contains.push_back(node);

	inverted = true;
	current = &(border.bordx);
	border_contains.shrink_to_fit();
	sort(border_contains.begin(), border_contains.end());
	current->push_back(make_tuple(border_contains, inverted));
	border_contains.clear();
} 

// Генерация первых краевых (на всех границах)
void generate_cond11(grid& g, borders& b, condition& cond, double (*u)(double, double))
{
	cond.xnum_cond = cond.ynum_cond = 1;
	for (tuple<vector<int>, bool>& bx : b.bordx)
	{
		vector<double> cur_val;
		for (auto& node : get<0>(bx))
			cur_val.push_back(u(g.x[node], g.y[node]));

		cond.borders.push_back(get<0>(bx));
		cur_val.shrink_to_fit();
		cond.values.push_back(cur_val);
	}

	for (tuple<vector<int>, bool>& by : b.bordy)
	{
		vector<double> cur_val;
		for (auto& node : get<0>(by))
			cur_val.push_back(u(g.x[node], g.y[node]));

		cond.borders.push_back(get<0>(by));
		cur_val.shrink_to_fit();
		cond.values.push_back(cur_val);
	}
}

// Генерация первых краевых на х, вторых на игрек
void generate_cond12(grid& g, borders& b, condition& cond, double (*u)(double, double), double (*dudy)(double, double))
{
	cond.xnum_cond = 1;
	cond.ynum_cond = 2;
	for (tuple<vector<int>, bool>& bx : b.bordx)
	{
		vector<double> cur_val;
		for (auto& node : get<0>(bx))
			cur_val.push_back(u(g.x[node], g.y[node]));

		cond.borders.push_back(get<0>(bx));
		cur_val.shrink_to_fit();
		cond.values.push_back(cur_val);
	}

	for (tuple<vector<int>, bool>& by : b.bordy)
	{
		vector<double> cur_val;
		for (auto& node : get<0>(by))
			cur_val.push_back(dudy(g.x[node], g.y[node]));

		cond.borders.push_back(get<0>(by));
		cur_val.shrink_to_fit();
		cond.values.push_back(cur_val);
	}
}

// Генерация первых краевых на х, вторых на игрек
void generate_cond21(grid& g, borders& b, condition& cond, double (*dudx)(double, double), double (*u)(double, double))
{
	cond.xnum_cond = 2;
	cond.ynum_cond = 1;
	for (tuple<vector<int>, bool>& bx : b.bordx)
	{
		vector<double> cur_val;
		for (auto& node : get<0>(bx))
			cur_val.push_back(dudx(g.x[node], g.y[node]));

		cond.borders.push_back(get<0>(bx));
		cur_val.shrink_to_fit();
		cond.values.push_back(cur_val);
	}

	for (tuple<vector<int>, bool>& by : b.bordy)
	{
		vector<double> cur_val;
		for (auto& node : get<0>(by))
			cur_val.push_back(u(g.x[node], g.y[node]));

		cond.borders.push_back(get<0>(by));
		cur_val.shrink_to_fit();
		cond.values.push_back(cur_val);
	}
}

// Генерация вторых краевых (на всех границах)
void generate_cond22(grid& g, borders& b, condition& cond, double (*dudx)(double, double), double (*dudy)(double, double))
{
	cond.xnum_cond = cond.ynum_cond = 2;
	for (tuple<vector<int>, bool>& bx : b.bordx)
	{
		vector<double> cur_val;
		for (auto& node : get<0>(bx))
			cur_val.push_back(dudx(g.x[node], g.y[node]));

		cond.borders.push_back(get<0>(bx));
		cur_val.shrink_to_fit();
		cond.values.push_back(cur_val);
	}

	for (tuple<vector<int>, bool>& by : b.bordy)
	{
		vector<double> cur_val;
		for (auto& node : get<0>(by))
			cur_val.push_back(dudy(g.x[node], g.y[node]));

		cond.borders.push_back(get<0>(by));
		cur_val.shrink_to_fit();
		cond.values.push_back(cur_val);
	}
}
