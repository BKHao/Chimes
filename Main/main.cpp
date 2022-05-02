
#include <Chimes/Optimization/steepest_descent.h>
#include <Chimes/Geometry/point.h>
#include <Chimes/Geometry/polygon.h>
#include <Chimes/Geometry/triangle.h>
//#include <Chimes/Geometry/surface.h>
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
	using point2 = Chimes::geometry::Point2<double>;
	using point3 = Chimes::geometry::Point3<double>;
	using polygon = Chimes::geometry::ConvexPolygon<point2>;
	using tri = Chimes::geometry::Triangle<point2>;
	point2 p0(0, 0);
	point2 p1(1, 0);
	point2 p2(1, 1);
	point2 p3(0, 1);
	polygon poly;
	poly.Start();
	poly.Insert(p0);
	poly.Insert(p1);
	poly.Insert(p2);
	poly.Insert(p3);
	poly.End();
	std::cout << poly.MassCenter() << std::endl;
	int* ids = new int[3];
	ids[0] = 0;
	ids[1] = 1;
	ids[2] = 2;
	std::shared_ptr<int> sids(ids, [](int* p) {delete[] p; });
	tri tri0(p0, p1, p2);
	tri0.SetPid(sids);
	std::cout << tri0 << std::endl;
	return 0;
}
