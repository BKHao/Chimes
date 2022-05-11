// 06/05/2022 by BKHao in Chimes.

#pragma once
#include <Chimes/Core/MemoryPool/memory_alloc.h>
#include <Chimes/Geometry/geometry_object.h>
#include <Chimes/Core/template_concept.h>

namespace Chimes
{
	namespace geometry
	{
        //A simple polygon composed of an array of points, the order of the points indicates the boundary connection. Represent the points through smart pointer.
        template <typename P>
            requires concept_point_operater<P>&& concept_point_normal<P>
        class SimplePolygon : public GeometryObject
        {
        protected:
            static const unsigned int S_COMPLETE = 1;
            static const unsigned int S_PROCESSING = 1 << 1;
            static const unsigned int S_PID = 1 << 2;
        public:
            //Initialize the polygon with no ponit.
            SimplePolygon() :points_(0), state_(0), pids_(0)
            {

            }
            //Initialize the polygon with no ponit, but reserve n points capacity.
            SimplePolygon(size_t n) :points_(0), state_(0), pids_(0)
            {
                points_.reserve(n);
            }
            //Initialize the polygon with an array of points.
            //The point data is saved.
            SimplePolygon(const std::vector<P>& points) :points_(points), state_(S_COMPLETE), pids_(0)
            {
                
            }
            //Initialize the polygon with an array of points.
            //The point data is not saved.
            SimplePolygon(std::vector<P>&& points) :points_(std::move(points)), state_(S_COMPLETE), pids_(0)
            {
                
            }
            //Copy construct the polygon with another polygon.
            //The point data is saved but don't set pid.
            SimplePolygon(const SimplePolygon& polygon): points_(polygon.points_), state_(S_COMPLETE), pids_(0), cursor_(0)
            {
                
            }
            //Move construct the polygon with another polygon.
            // The point data is not saved.
            SimplePolygon(SimplePolygon&& polygon) : points_(std::move(polygon.points_)), state_(polygon.state_), pids_(std::move(polygon.pids_)), cursor_(polygon.cursor_)
            {
                
            }
            //Assign the polygon with another polygon.
            //The point data is saved.
            SimplePolygon& operator=(const SimplePolygon& polygon)
            {
                points_ = polygon.points_;
                pids_ = MemoryPtr<size_t>(0);
                cursor_ = 0;
                state_ = S_COMPLETE;
            }
            //Assign the polygon with another polygon.
            //The point data is not saved.
            SimplePolygon& operator=(SimplePolygon&& polygon)
            {
                points_ = std::move(polygon.points_);
                pids_ = std::move(polygon.pids_);
                cursor_ = polygon.cursor_;
                state_ = polygon.state_;
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
                    points_.push_back(p);
                    return true;
                }
                return false;
            }
            //Insert a point p.
            //The point data is not saved.
            bool Insert(P&& p)
            {
                if (state_ & S_PROCESSING)
                {
                    points_.push_back(std::move(p));
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
            const P& operator[](size_t i) const
            {
                if (i < points_.size())
                    return points_[i];
                else
                    throw std::runtime_error("out of the range");
            }
            //Set the id of the points. Believe the data is correct.
            void SetPid(const MemoryPtr<size_t>& pids, size_t cursor)
            {
                pids_ = pids;
                cursor_ = cursor;
                state_ |= S_PID;
            }
            //Get the id of the i-th point.
            size_t Pid(size_t i) const
            {
                if (!(state_ & S_PID))
                    throw std::runtime_error("Pid has not been set.");
                if (i < points_.size())
                    return pids_[cursor_ + i];
                else
                    return -1;
            }
            //Return the geometry type.
            virtual GeometryType Info() const
            {
                return GeometryType::SIMPLE_POLYGON;
            }
        protected:
            std::vector<P> points_;
            unsigned int state_;
            MemoryPtr<size_t> pids_;
            size_t cursor_;
        };

        //Convex polygon, inherited from SimplexPolygon.
        template <typename P>
        class ConvexPolygon : public SimplePolygon<P>
        {
        private:
            using Base = SimplePolygon<P>;
        public:
            using Base::Base;
            //Return the mass center of the convex polygon.
            P MassCenter() const
            {
                P c = Base::points_[0];
                for (size_t i = 1; i < Base::points_.size(); ++i)
                {
                    c += *Base::points_[i];
                }
                return (c / P::R(Base::points_.size()));
            }
            //Return the geometry type.
            virtual GeometryType Info() const
            {
                return GeometryType::CONVEX_POLYGON;
            }

        };
	} // namespace geometry
} // namespace Chimes
