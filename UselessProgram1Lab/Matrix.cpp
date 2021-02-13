#include "Matrix.h"

void Matrix::display(ostream& out)
{
	for (int j = 0; j < values.size(); j++)
	{
		for (int i = 0; i < values[j].size(); i++)
			out << values[j][i] << " ";
		out << endl;
	}
}