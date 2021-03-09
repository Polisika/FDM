#include <iostream>
#include <fstream>
#include "Generator.h"
#include "Structs.h"
#include "Solver.h"
#include "input.h"
#include <map>
#include <string>
#include <iomanip>

const double EPS = 1e-13, WEIGHT = 1., INIT_STEP = 0.1;
const int MAX_ITER = 1e5;

#pragma region Func
void get_solve()
{
    grid g;
    generate_grid(0.2, 1, 1, 0, 0, 1, 1, 5, 4, g);
    vector<int> need_drop({ 2 });
    g.display(cout);
    cout << endl;

    ifstream inputBorder("input_border.txt");
    borders border;
    inputBord(inputBorder, border);
    inputBorder.close();
    border.display(cout);
    cout << endl;

    ifstream inputConditions("input_condition.txt");
    condition x;
    inputCond(inputConditions, x);
    inputConditions.close();
    x.display(cout);
    cout << endl;

    vector<double> res;
    Solver s;
    s.GetSolve(g, [](double x, double y) { return x * x - 2.; }, x, border, need_drop, res, 1e-13, 1, 100000);
    for (int i = 0; i < res.size(); i++)
        cout << res[i] << " ";
}

// Вернет желанную область
// params заполнять в main
void get_grid(map<string, double> ParamsGrid, grid& g)
{
    generate_grid(ParamsGrid["step"], ParamsGrid["kx"], ParamsGrid["ky"], ParamsGrid["x0"],
        ParamsGrid["y0"], ParamsGrid["lambda"], ParamsGrid["gamma"], ParamsGrid["width"], ParamsGrid["height"], g);
}

// Вернет желанные краевые
void get_cond(int x, int y, double (*u)(double, double), double (*dudx)(double, double), double (*dudy)(double, double),
    grid& g, borders& b, condition& cond)
{
    if (x == y && x == 1)
        generate_cond11(g, b, cond, u);
    else if (x == y && x == 2)
        generate_cond21(g, b, cond, dudx, u);
    else if (x == 1 && y == 2)
        generate_cond12(g, b, cond, u, dudy);
    else if (y == 2 && x == 2)
        generate_cond21(g, b, cond, dudx, dudy);
    else
        throw invalid_argument("Invalid number combination of conditions.");
}
#pragma endregion

class TestCase
{
private:
    map<string, double> params;
    vector<int> needDrop;
    int num_x_cond, num_y_cond;
    double(*u)(double, double);
    double (*dudx)(double, double);
    double (*dudy)(double, double);
    double (*f)(double, double);

public:
    TestCase(map<string, double> params, vector<int> needDrop, double(*u)(double, double),
        double (*dudx)(double, double), double (*dudy)(double, double), double (*f)(double, double),
        int num_x_cond, int num_y_cond)
    {
        this->params = params;
        this->needDrop = needDrop;
        this->u = u;
        this->dudx = dudx;
        this->dudy = dudy;
        this->f = f;
        this->num_x_cond = num_x_cond;
        this->num_y_cond = num_y_cond;
    }

    void run(ofstream& out)
    {
        // Сетка
        grid g;
        get_grid(params, g);

        // Границы
        borders border;
        generate_border(g, border, needDrop);

        // Краевые условия
        condition cond;
        get_cond(num_x_cond, num_y_cond, u, dudx, dudy, g, border, cond);

        // Решение
        vector<double> result;
        Solver s;
        s.GetSolve(g, f, cond, border, needDrop, result, EPS, WEIGHT, MAX_ITER);

        // Печать результатов
        out << "U,U*,|U - U*|" << endl;

        for (int i = 0; i < result.size(); i++)
        {
            out << scientific << setprecision(7) << result[i] << ",";

            double val = u(g.x[i], g.y[i]);

            // Если строка нулевая, то и значение, и погрешность ноль.
            if (find(needDrop.begin(), needDrop.end(), i) != needDrop.end())
                out << scientific << setprecision(7) << 0. << "," << 0. << endl;
            else
                out << scientific << setprecision(7) << val << "," << abs(result[i] - val) << endl;
        }
    }
};

int main()
{
    vector<TestCase> tests = {
#pragma region Первичное тестирование
        // 3.1
        TestCase(
            map<string, double>
            {
                {"step", 0.2},
                {"kx", 1},
                {"ky", 1},
                {"x0", 0},
                {"y0", 0},
                {"lambda", 1},
                {"gamma", 1},
                {"width", 5},
                {"height", 4},
            },
            vector<int> {2},
            [](double x, double y) { return 1.; }, // u
            [](double x, double y) { return 0.; }, // dudx
            [](double x, double y) { return 0.; }, // dudy
            [](double x, double y) { return 1.; }, // f
            1, // краевые при фиксированном х
            1 // краевые при фиксированном y
        ),
        // 3.2
        TestCase(
            map<string, double>
            {
                {"step", 0.2},
                {"kx", 1},
                {"ky", 1},
                {"x0", 0},
                {"y0", 0},
                {"lambda", 1},
                {"gamma", 2},
                {"width", 5},
                {"height", 4},
            },
            vector<int> {2},
            [](double x, double y) { return x + y; }, // u
            [](double x, double y) { return 1.; }, // dudx
            [](double x, double y) { return 1.; }, // dudy
            [](double x, double y) { return 2 * (x + y); }, // f
            1, // краевые при фиксированном х
            1 // краевые при фиксированном y
        ),
#pragma endregion

#pragma region Порядок аппроксимации
        // 4.1
        TestCase(
            map<string, double>
            {
                {"step", 0.1},
                {"kx", 1},
                {"ky", 1},
                {"x0", 0},
                {"y0", 0},
                {"lambda", 1},
                {"gamma", 2},
                {"width", 10},
                {"height", 8},
            },
            vector<int> {3, 4, 13, 14},
            [](double x, double y) { return 1.; }, // u
            [](double x, double y) { return 0.; }, // dudx
            [](double x, double y) { return 0.; }, // dudy
            [](double x, double y) { return 2.; }, // f
            1, // краевые при фиксированном х
            1 // краевые при фиксированном y
        ),

        // 4.2
        TestCase(
            map<string, double>
            {
                {"step", 0.1},
                {"kx", 1},
                {"ky", 1},
                {"x0", 0},
                {"y0", 0},
                {"lambda", 1},
                {"gamma", 2},
                {"width", 10},
                {"height", 8},
            },
            vector<int> {3, 4, 13, 14},
            [](double x, double y) { return x + y; }, // u
            [](double x, double y) { return 1.; }, // dudx
            [](double x, double y) { return 1.; }, // dudy
            [](double x, double y) { return 2 * (x + y); }, // f
            1, // краевые при фиксированном х
            1 // краевые при фиксированном y
        ),

        // 4.3
        TestCase(
            map<string, double>
            {
                {"step", 0.1},
                {"kx", 1},
                {"ky", 1},
                {"x0", 0},
                {"y0", 0},
                {"lambda", 1},
                {"gamma", 2},
                {"width", 10},
                {"height", 8},
            },
            vector<int> {3, 4, 13, 14},
            [](double x, double y) { return x * x + y * y; }, // u
            [](double x, double y) { return 2 * x; }, // dudx
            [](double x, double y) { return 2 * y; }, // dudy
            [](double x, double y) { return 2 * (x * x + y * y) - 4; }, // f
            1, // краевые при фиксированном х
            1 // краевые при фиксированном y
        ),

        // 4.4
        TestCase(
            map<string, double>
            {
                {"step", 0.1},
                {"kx", 1},
                {"ky", 1},
                {"x0", 0},
                {"y0", 0},
                {"lambda", 1},
                {"gamma", 2},
                {"width", 10},
                {"height", 8},
            },
            vector<int> {3, 4, 13, 14},
            [](double x, double y) { return x * x * x + y * y * y; }, // u
            [](double x, double y) { return 3 * x * x; }, // dudx
            [](double x, double y) { return 3 * y * y; }, // dudy
            [](double x, double y) { return 2 * (x * x * x + y * y * y - 3 * (x + y)); }, // f
            1, // краевые при фиксированном х
            1 // краевые при фиксированном y
        ),
#pragma endregion

#pragma region Порядок сходимости
        // 5.1
        TestCase(
            map<string, double>
            {
                {"step", INIT_STEP},
                {"kx", 1},
                {"ky", 1},
                {"x0", 0},
                {"y0", 0},
                {"lambda", 1},
                {"gamma", 1},
                {"width", 10},
                {"height", 8},
            },
            vector<int> {3, 4, 13, 14},
            [](double x, double y) { return sin(x + y); }, // u
            [](double x, double y) { return cos(x + y); }, // dudx
            [](double x, double y) { return cos(x + y); }, // dudy
            [](double x, double y) { return 3 * sin(x + y); }, // f
            1, // краевые при фиксированном х
            1 // краевые при фиксированном y
        ),

        // 5.2
        TestCase(
            map<string, double>
            {
                {"step", INIT_STEP / 2},
                { "kx", 1 },
                { "ky", 1 },
                { "x0", 0 },
                { "y0", 0 },
                { "lambda", 1 },
                { "gamma", 1 },
                {"width", 20},
                {"height", 16},
            },
            vector<int> {6, 7, 26, 27},
            [](double x, double y) { return sin(x + y); }, // u
            [](double x, double y) { return cos(x + y); }, // dudx
            [](double x, double y) { return cos(x + y); }, // dudy
            [](double x, double y) { return 3 * sin(x + y); }, // f
            1, // краевые при фиксированном х
            1 // краевые при фиксированном y
        ),

        // 5.3
        TestCase(
            map<string, double>
            {
                {"step", INIT_STEP / 4},
                { "kx", 1 },
                { "ky", 1 },
                { "x0", 0 },
                { "y0", 0 },
                { "lambda", 1 },
                { "gamma", 1 },
                {"width", 40},
                {"height", 32},
            },
            vector<int> {13, 14, 53, 54},
            [](double x, double y) { return sin(x + y); }, // u
            [](double x, double y) { return cos(x + y); }, // dudx
            [](double x, double y) { return cos(x + y); }, // dudy
            [](double x, double y) { return 3 * sin(x + y); }, // f
            1, // краевые при фиксированном х
            1 // краевые при фиксированном y
        ),

        // 5.4
        TestCase(
            map<string, double>
            {
                {"step", INIT_STEP / 8},
                {"kx", 1},
                {"ky", 1},
                {"x0", 0},
                {"y0", 0},
                {"lambda", 1},
                {"gamma", 1},
                {"width", 80},
                {"height", 64},
            },
            vector<int> {13, 14, 93, 94},
                    [](double x, double y) { return sin(x + y); }, // u
                    [](double x, double y) { return cos(x + y); }, // dudx
                    [](double x, double y) { return cos(x + y); }, // dudy
                    [](double x, double y) { return 3 * sin(x + y); }, // f
            1, // краевые при фиксированном х
            1 // краевые при фиксированном y
        ),

        // 5.5
        TestCase(
            map<string, double>
            {
                {"step", INIT_STEP / 16},
                {"kx", 1},
                {"ky", 1},
                {"x0", 0},
                {"y0", 0},
                {"lambda", 1},
                {"gamma", 1},
                {"width", 160},
                {"height", 128},
            },
            vector<int> {13, 14, 173, 174},
                    [](double x, double y) { return sin(x + y); }, // u
                    [](double x, double y) { return cos(x + y); }, // dudx
                    [](double x, double y) { return cos(x + y); }, // dudy
                    [](double x, double y) { return 3 * sin(x + y); }, // f
            1, // краевые при фиксированном х
            1 // краевые при фиксированном y
        ),
#pragma endregion
    };

    for (int i = 0; i < tests.size(); i++)
    {
        ofstream out("test" + to_string(i + 1) + ".csv");
        cout << "Running test #" << i + 1 << endl;

        tests[i].run(out);
    }

    return 0;
}
