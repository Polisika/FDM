#include "Structs.h"
#include <istream>

using namespace std;

// ���, ���������� ����� (������� ���������), ����. �������� x, y (���� �����
// ����������, �� kx=ky=1), ������ � ����� ��������, ������ � �����
// ���� (x y) �������������, ������� ����� 
// �����, ����� ������� �� �����, ����� ����������� �����, �� �� ������ ����.
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

// ���������� ������, ������������ ������ (������� �� �, ����� �� �): 
//  ���-�� �� ���
//		���������� �����, ������������ �����.
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