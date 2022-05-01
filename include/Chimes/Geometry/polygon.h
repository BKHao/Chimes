#pragma once
#include <Chimes/geometry/geometry_object.h>
#include <Chimes/core/template_concept.h>
#include <vector>
#include <memory>

namespace Chimes
{
    namespace geometry
    {
        //A simple polygon composed of an array of points, the order of the points indicates the boundary connection. Represent the points through smart pointer.
        template <typename P>
        requires concept_point_operater<P>&& concept_point_normal<P>
        class SimplePolygon : public GeometryObject
        {
        public:
            //Initialize the polygon with no ponit.
            SimplePolygon() :points_(0), state_(0)
            {

            }
            //Initialize the polygon with an array of pointer.
            //The point data is not saved.
            SimplePolygon(const std::vector<std::shared_ptr<P>>& points) :points_(points), state_(1)
            {

            }
            //Initialize the polygon with an array of pointer.
            //The point data is not saved.
            SimplePolygon(const std::vector<std::shared_ptr<P>>&& points) :points_(points), state_(1)
            {

            }
            //Start to insert the construction polygon point by point. The old data will be cleared.
            void Start()
            {
                state_ = S_PROCESSING;
                points_.clear();
            }
            //Insert a point p.
            //The point data is saved.
            bool Insert(const P& p)
            {
                if (state_ & S_PROCESSING)
                {
                    points_.push_back(std::make_shared<P>(p));
                    return true;
                }
                return false;
            }
            //Insert a point p.
            //The point data is not saved.
            bool Insert(const std::shared_ptr<P>& p)
            {
                if (state_ & S_PROCESSING)
                {
                    points_.push_back(p);
                    return true;
                }
                return false;
            }
            //End the construction.
            bool End()
            {
                if (points_.size() > 2)
                {
                    state_ &= (~S_PROCESSING);
                    state_ |= S_COMPLETE;
                    return true;
                }
                else
                {
                    state_ = 0;
                    points_.clear();
                    return false;
                }
            }
            //Get the const pointer of the i-th point.
            std::shared_ptr<P> operator[](size_t i) const
            {
                if (i < points_.size())
                    return points_[i];
                else
                    return nullptr;
            }
            //Get the id of the i-th point.
            int Pid(size_t i) const
            {
                if (i < points_.size())
                    return points_[i]->id();
                else
                    return -1;
            }
            //Return the geometry type.
            virtual GeometryType Info() const
            {
                return GeometryType::SIMPLE_POLYGON;
            }
        protected:
            std::vector<std::shared_ptr<P>> points_;
            static const unsigned int S_COMPLETE = 1;
            static const unsigned int S_PROCESSING = 1 << 1;
            unsigned int state_;
        };
        //Convex polygon, inherited from SimplexPolygon.
        template <typename P>
        class ConvexPolygon : public SimplePolygon<P>
        {
        private:
            using Base = SimplePolygon<P>;
        public:
            //Return the mass center of the convex polygon.
            P MassCenter() const
            {
                static P res;
                static bool calculated = false;
                if (calculated)
                    return res;
                P c = *Base::points_[0];
                for (size_t i = 1; i < Base::points_.size(); ++i)
                {
                    c += *Base::points_[i];
                }
                res = c / P::R(Base::points_.size());
                calculated = true;
                return res;
            }
            //Return the geometry type.
            virtual GeometryType Info() const
            {
                return GeometryType::CONVEX_POLYGON;
            }

        };
    }
}