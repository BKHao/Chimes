#include <Chimes/Optimization/steepest_descent.h>
#include <Chimes/Core/MemoryPool/memory_alloc.h>
#include <Chimes/Geometry/point.h>
#include <Chimes/Geometry/polygon.h>
#include <Chimes/Geometry/triangle.h>
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

class Test
{
public:
	Test() :id(-1)
	{
		std::cout << "default Test() " << id << std::endl;
	}
	Test(int i) :id(i)
	{
		std::cout << "int Test(int i) " << id << std::endl;
	}
	Test(const Test& test)
	{
		id = test.id;
		std::cout << "copy Test(const Test& test) " << id << std::endl;
	}
	Test(Test&& test)
	{
		id = test.id;
		std::cout << "move Test(Test&& test) " << id << std::endl;
	}
	int id;
};

int main(int argv, char* argc[])
{	
	using p2d = Chimes::geometry::Point2<double>;
	using p3d = Chimes::geometry::Point3<double>;
	using polygon = Chimes::geometry::ConvexPolygon<p3d>;
	using Triangle = Chimes::geometry::Triangle<p3d>;
	p3d point1(1, 1, 1);
	p3d point2;
	point2[0] = 2;
	point2[1] = 2;
	point2[2] = 2;
	p3d point3(3, 3, 3);
	p3d point4(0);
	p3d point5(point4);
	point4 = p3d(4, 4, 4);
	point5 = p3d(5, 5, 5);
	std::vector<p3d> points;
	points.push_back(point1);
	points.push_back(point2);
	points.push_back(point3);
	points.push_back(point4);
	points.push_back(point5);
	for (size_t i = 0; i < 5; ++i)
	{
		std::cout << points[i] << std::endl;
	}
	//std::vector<p3d> share_points(5, 0);
	//for (size_t i = 0; i < 5; ++i)
	//{
	//	share_points[i].share(points[i]);
	//}
	std::vector<p3d> poly_points;
	for (size_t i = 0; i < 5; ++i)
	{
		p3d sp(0);
		sp.share(points[i]);
		poly_points.push_back(std::move(sp));
	}
	polygon poly(std::move(poly_points));
	//poly_points[0][2] = 200;
	points[0][2] = 200;

	
	for (size_t i = 0; i < 5; ++i)
	{
		std::cout << points[i] << std::endl;
		std::cout << poly[i] << std::endl;
	}

	Triangle tri(p3d(0, 0, 0), p3d(1, 2, 3), p3d(0, 5, 0));
	for (size_t i = 0; i < 3; ++i)
	{
		std::cout << tri[i] << std::endl;
	}

	std::cout << "======================" << std::endl;

	//std::vector<Test> tests;
	////tests.reserve(2);

	//Test test;
	//Test test2(1);	
	//std::cout << "====================push back 1" << std::endl;
	//tests.push_back(test);
	//std::cout << "====================push back 2" << std::endl;	
	//tests.push_back(std::move(test2));
		
	std::cout << "Success!" << std::endl;
	return 0;
}
