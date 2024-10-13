#include <iostream>
#include <array>
#include <cmath>
#include <functional>
using namespace std;

// Возвращает tk (шаг минимизирующий функцию вдоль антиградиента)
double golden_section_search(double a0, double b0, std::function<double(double)> func)
{
    // Константа phi
    const double phi = (3 - sqrt(5)) / 2;

    // Точность
    const double e = 0.01;

    // Переменные для итераций
    double a_k = a0;
    double b_k = b0;
    double y_k = a_k + phi * (b_k - a_k);
    double z_k = a_k + b_k - y_k;
    double f_y_k = func(y_k);
    double f_z_k = func(z_k);
    int iteration = 2;

    // Основной цикл метода золотого сечения
    while (fabs(b_k - a_k) > e)
    {
        if (f_y_k <= f_z_k)
        {
            b_k = z_k;
            z_k = y_k;
            y_k = a_k + b_k - y_k;
            f_z_k = f_y_k;
            f_y_k = func(y_k);
        }
        else
        {
            a_k = y_k;
            y_k = z_k;
            z_k = a_k + b_k - z_k;
            f_y_k = f_z_k;
            f_z_k = func(z_k);
        }
        iteration++;
    }

    //  Минимальная точка и минимальное значение функции
    double x_min = (a_k + b_k) / 2;
    double f_min = func(x_min);

    return x_min;
}
//  Оптимизируемая функция
double func(const std::array<double, 2>& x)
{
	return 2 * x[0] * x[0] + x[0] * x[1] + x[1] * x[1];
}

// Первая производная по х1
double func_x1(const std::array<double, 2>& x)
{
	return 4 * x[0] + x[1];
}

// Первая производная по x2
double func_x2(const std::array<double, 2>& x)
{
	return x[0] + 2 * x[1];
}

// Функция для вычисления нормы градиента
double func_grad_norm(const std::array<double, 2>& x)
{
	double grad_x1 = func_x1(x);
	double grad_x2 = func_x2(x);

	double result = sqrt(pow(grad_x1, 2) + pow(grad_x2, 2));
	return result;
}

// Возвращает значение функции в новой точке new_x
double fi(double t, const std::array<double, 2>& x, const std::array<double, 2>& grad)
{
    std::array<double, 2> new_x = { x[0] - t * grad[0], x[1] - t * grad[1] };
    return func(new_x);
}
int main()
{
    setlocale(LC_ALL, "Russian"); // Для чтения из консоли

    // Начальная точка (или же вектор)
    std::array<double, 2> x0 = { 0.5, 1.0 };

    // Точности для остановки алгоритма
    double eps_1{ 0.1 }, eps_2{ 0.15 };

    // Предельное число итераций
    int M{ 1 };

    // Счетчик итераций
    int k{ 0 };

    do
    {
        // Начальное значение функции
        double f_k = func(x0);

        // Вычисляем норму градиента (Шаг 3)
        double grad_norm = func_grad_norm(x0);

        // Проверка условия окончания по норме градиента (ШАГ 4)
        if (grad_norm < eps_1)
        {
            cout << "Условие окончания выполнено: Точка = (" << x0[0] << ", " << x0[1] << "), Значение функции = " << f_k << endl;
            break;
        }

        // Находим градиент в текущей точке
        std::array<double, 2> grad = { func_x1(x0), func_x2(x0) };

        // Используем метод золотого сечения для нахождения оптимального шага t_k
        double t_k = golden_section_search(0, 1, [x0, grad](double t) {
            return fi(t, x0, grad);
            });

        // Обновляем точку x (Шаг 7)
        std::array<double, 2> x_new = { x0[0] - t_k * grad[0], x0[1] - t_k * grad[1] };

        // Вычисляем новое значение функции 
        double f_new = func(x_new);

        // Проверка условия окончания по изменению функции (Шаг 8)
        if (fabs(f_new - f_k) < eps_2)
        {
            cout << "Условие окончания выполнено: Итерация " << k << ": Точка = (" << x0[0] << ", " << x0[1] << "), Значение функции в точке = " << f_k << endl;;
            break;
        }

        // Обновление точки и значения функции
        x0 = x_new;
        f_k = f_new;

        k++;

        if (k >= M)
        {
            cout << "Выполнено неравенство K >= M. Итерация " << k << ": Точка = (" << x0[0] << ", " << x0[1] << "), Значение функции в точке  = " << f_k << endl;
        }

        // Для проверки в каждой итерации.
        // cout << "Итерация " << k << ": Точка = (" << x0[0] << ", " << x0[1] << "), Значение функции в точке  = " << f_k << endl;
    } while (k < M);

    return 0;
}