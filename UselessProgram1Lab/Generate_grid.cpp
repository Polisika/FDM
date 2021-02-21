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
	// ����� ���������
	while(find(need_drop.begin(), need_drop.end(), node + 1) == need_drop.end())
	{
			// �� ������
			border_contains.push_back(node);
			node++; // ������ ������������ ������ �� �����
	}
	border_contains.push_back(node);
	vector<tuple<vector<int>, bool>>* current = &(border.bordy);
	
	// ����� ��������� ������� � current
	// ������� �� �, �������������
	border_contains.shrink_to_fit();
	bool inverted = true;
	sort(border_contains.begin(), border_contains.end());
	current->push_back(make_tuple(border_contains, inverted));
	border_contains.clear();

	// ��� ��� ����� ����� �, �� ����� �������� �� ����� �
	// ���� �� ����� ���������� �������
	while (find(need_drop.begin(), need_drop.end(), node + 1) != need_drop.end())
	{
		border_contains.push_back(node);
		node += g.width;
	}
	border_contains.push_back(node);

	// ����� ��������� ������� � current
	// ������� �� y, �� �������������
	inverted = false;
	current = &(border.bordx);
	border_contains.shrink_to_fit();
	sort(border_contains.begin(), border_contains.end());
	current->push_back(make_tuple(border_contains, inverted));
	border_contains.clear();

	border_contains.push_back(node);
	node++;
	// ����� ������� (������ ������) ���� ������, ���� ������
	while (find(need_drop.begin(), need_drop.end(), node - g.width) != need_drop.end())
	{
		border_contains.push_back(node);
		node++;
	}
	border_contains.push_back(node);

	// ������� ������� ���������� �������
	// ������� �� x, �������������
	inverted = true;
	current = &(border.bordy);
	border_contains.shrink_to_fit();
	sort(border_contains.begin(), border_contains.end());
	current->push_back(make_tuple(border_contains, inverted));
	border_contains.clear();

	border_contains.push_back(node);
	node -= g.width;
	// ���� �� ������ ����������
	while (find(need_drop.begin(), need_drop.end(), node - 1) != need_drop.end())
	{
		border_contains.push_back(node);
		node -= g.width;
	}
	
	// ������ ���������� �������
	// ������� �������������
	// ������� �� � �����������
	node += g.width;
	inverted = true;
	current = &(border.bordx);
	border_contains.shrink_to_fit();
	sort(border_contains.begin(), border_contains.end());
	current->push_back(make_tuple(border_contains, inverted));
	border_contains.clear();

	// ����� ���� �� ��������
	for (; node < g.width; node++)
		border_contains.push_back(node);

	// ������ ���������
	inverted = true;
	current = &(border.bordy);
	border_contains.shrink_to_fit();
	sort(border_contains.begin(), border_contains.end());
	current->push_back(make_tuple(border_contains, inverted));
	border_contains.clear();

	node--;
	// ������ �������
	for (; node <= g.num_nodes - 1; node+= g.width)
		border_contains.push_back(node);

	inverted = false;
	current = &(border.bordx);
	border_contains.shrink_to_fit();
	sort(border_contains.begin(), border_contains.end());
	current->push_back(make_tuple(border_contains, inverted));
	border_contains.clear();

	node = g.num_nodes - 1;
	// ������
	for (; node >= g.num_nodes - g.width; node--)
		border_contains.push_back(node);

	inverted = false;
	current = &(border.bordy);
	border_contains.shrink_to_fit();
	sort(border_contains.begin(), border_contains.end());
	current->push_back(make_tuple(border_contains, inverted));
	border_contains.clear();

	node++;
	// ����� �������
	for (; node >= 0; node-= g.width)
		border_contains.push_back(node);

	inverted = true;
	current = &(border.bordx);
	border_contains.shrink_to_fit();
	sort(border_contains.begin(), border_contains.end());
	current->push_back(make_tuple(border_contains, inverted));
	border_contains.clear();
} 

// ��������� ������ ������� (�� ���� ��������)
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

// ��������� ������ ������� �� �, ������ �� �����
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

// ��������� ������ ������� �� �, ������ �� �����
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

// ��������� ������ ������� (�� ���� ��������)
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
