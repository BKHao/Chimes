#include <Chimes/Optimization/steepest_descent.h>
#include <Chimes/Core/MemoryPool/memory_alloc.h>
#include <Chimes/Geometry/point.h>
#include <functional>
#include <memory>
#include <iostream>

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

void test_memorypool()
{
	Chimes::MemoryPtr<int> pData;
	*pData = 100;
	Chimes::MemoryPtr<int> pData2;
	*pData2 = 200;
	std::cout << "p1 " << *pData.Get() << "  p2 " << *pData2.Get() << std::endl;
	pData2 = pData;
	Chimes::MemoryPtr<int> pData3;
	std::cout << "p1 " << *pData.Get() << "  p2 " << *pData2.Get() << "  p3 " << *pData3.Get() << std::endl;
	Chimes::MemoryPtr<int> pData4(std::move(pData3));
	std::cout << "p1 " << *pData.Get() << "  p2 " << *pData2.Get() << "  p4 " << *pData4.Get() << std::endl;
	Chimes::MemoryPtr<int> pArray(3);
	pArray[0] = 100;
	pArray[1] = 200;
	pArray[2] = 300;
	std::cout << *pArray << "  " << *(pArray.Get() + 1) << "  " << pArray[2] << std::endl;
}


int main(int argv, char* argc[])
{	
	using p2d = Chimes::geometry::Point2<double>;
	using p3d = Chimes::geometry::Point3<double>;
	p3d point2;
	std::cout << point2 << std::endl;
	point2[0] = 1;
	point2[1] = 2;
	std::cout << point2 << std::endl;
	std::cout << point2[0] << " " << point2.y() << std::endl;
	p3d point3;
	std::cout << point3 << std::endl;
	point3.share(point2);
	point2[0] = 2;
	std::cout << point3 << std::endl;
	std::cin >> point2;
	std::cout << point3 << std::endl;
	p3d point4(0);
	p3d point5(point4);
	point4 = point3;
	std::cout << point4 << std::endl;
	std::vector<p2d> points(5, 0);
	std::cout << "======================" << std::endl;
		
	std::cout << "Success!" << std::endl;
	return 0;
}
