// 09/05/2022 by BKHao in Chimes.
#pragma once
#include <Chimes/Geometry/polygon.h>
#include <vector>
namespace Chimes
{
	namespace geometry
	{
        //Triganle, inherited from Convex_Polygon, which consists of three points.
        template <typename P>
        class Triangle : public ConvexPolygon<P>
        {
            friend std::ostream& operator<< <P>(std::ostream& os, const Triangle& t);
        private:
            using Base = ConvexPolygon<P>;
        public:
            using Base::Base;
            //Default constructor with doing nothing.
            Triangle() : Base(0)
            {

            }
            //Use (p0,p1,p2) to initialize the points.
            //The point data is saved.
            Triangle(const P& p0, const P& p1, const P& p2) :Base(3)
            {
                std::cout << "copy" << std::endl;
                Base::points_.push_back(p0);
                Base::points_.push_back(p1);
                Base::points_.push_back(p2);
                Base::state_ = Base::S_COMPLETE;
            }
            //Use (p0,p1,p2) to initialize the points.
            //The point data is not saved.
            Triangle(P&& p0, P&& p1, P&& p2) :Base(3)
            {
                std::cout << "move" << std::endl;
                Base::points_.push_back(std::move(p0));
                Base::points_.push_back(std::move(p1));
                Base::points_.push_back(std::move(p2));
                Base::state_ = Base::S_COMPLETE;
            }
            //Initialize with the another triangle.
            Triangle(Triangle&& tri) noexcept
            {
                Base::points_ = std::move(tri.points_);
                Base::pids_ = std::move(tri.pids_);
                Base::cursor_ = tri.cursor_;
                Base::state_ = tri.state_;
            }
            //Return the geometry type.
            virtual GeometryType Info() const
            {
                return GeometryType::TRIANGLE;
            }
        };
        //Send the id of points to os.
        template <typename P>
        inline std::ostream& operator<<(std::ostream& os, const Triangle<P>& t)
        {
            os << t.Pid(0) << " " << t.Pid(1) << " " << t.Pid(2);
            return os;
        }
	} // namespace geometry
} // namespace Chimes
