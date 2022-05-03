#include <Chimes/Optimization/steepest_descent.h>
#include <functional>
#include <memory>

void test_steepest_descent()
{
	using Fun = std::function<double(const Eigen::VectorXd& x, Eigen::VectorXd& gradient)>;
	Fun fun
		= [](const Eigen::VectorXd& x, Eigen::VectorXd& gradient)
	{
		double fval = (x(0) - 1) * (x(0) - 1) + (x(1) - 1) * (x(1) - 1);
		gradient.setZero();
		gradient(0) = 2 * (x(0) - 1);
		gradient(1) = 2 * (x(1) - 1);
		return fval;
	};
	Eigen::VectorXd init_x(2);
	init_x(0) = 53;
	init_x(1) = -68;
	Chimes::SteepestDescent<Fun, double> steepest_descent = Chimes::SteepestDescent<Fun, double>(fun, init_x);
	steepest_descent.solve();
	Chimes::SteepestDescent<Fun, double>::SolveResult result = steepest_descent.get_result();
	std::cout << "fval: " << result.fval << "  x: " << result.res_x[0] << " " << result.res_x[1] << std::endl;
}

int main(int argv, char* argc[])
{
	test_steepest_descent();
	std::cout << "Success!" << std::endl;
	return 0;
}
