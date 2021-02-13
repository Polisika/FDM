#pragma once
#include <vector>

using namespace std;


// ������ ���� ������������� ���������� � x � y ��������. 
struct grid
{
	int num_nodes;
	double step;
	// ����������� ��������
	double kx, ky;
	vector<double> x;
	vector<double> y;
	int width, height;
};

struct borders
{
	int num_x, num_y;
	vector<vector<int>> bordy; // �������, ������������ y
	vector<vector<int>> bordx; // �������, ������������ �

	bool belongs_bordy(int num_node)
	{
		for (auto& bord : bordy)
			for (auto& node : bord)
				if (node == num_node)
					return true;

		return false;
	}

	bool belongs_bordx(int num_node)
	{
		for (auto& bord : bordx)
			for (auto& node : bord)
				if (node == num_node)
					return true;

		return false;
	}
};

struct condition
{
	int num_cond;
	double (*getValue)(double, double);
	condition(int cond, double (*func)(double, double)) : num_cond(cond), getValue(func) {}
};
