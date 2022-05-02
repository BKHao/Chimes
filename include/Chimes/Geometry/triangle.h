#pragma once
#include <Chimes/Geometry/polygon.h>

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
            //Use (p0,p1,p2) to initialize the points.
            //The point data is saved.
            Triangle(const P& p0, const P& p1, const P& p2)
            {
                Base::points_.resize(3);
                Base::points_[0] = std::make_shared<P>(p0);
                Base::points_[1] = std::make_shared<P>(p1);
                Base::points_[2] = std::make_shared<P>(p2);
                Base::state_ = Base::S_COMPLETE;
            }
            //Use (p0,p1,p2) to initialize the points.
            //The point data is not saved.
            Triangle(const std::shared_ptr<P>& p0, const std::shared_ptr<P>& p1, const std::shared_ptr<P>& p2)
            {
                Base::points_.resize(3);
                Base::points_[0] = p0;
                Base::points_[1] = p1;
                Base::points_[2] = p2;
                Base::state_ = Base::S_COMPLETE;
            }
            //Return the geometry type.
            virtual GeometryType Info() const
            {
                return GeometryType::TRIANGLE;
            }
        };
        template <typename P>
        inline std::ostream& operator<<(std::ostream& os, const Triangle<P>& t)
        {
            os << t.Pid(0) << " " << t.Pid(1) << " " << t.Pid(2);
            return os;
        }
    }
}