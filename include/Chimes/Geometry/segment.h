// 12/06/2022 by BKHao in Chimes.

#pragma once
#include <Chimes/Core/MemoryPool/memory_alloc.h>
#include <Chimes/Geometry/geometry_object.h>
#include <Chimes/Core/template_concept.h>

namespace Chimes
{
	namespace geometry
	{
        //A segment composed of two points.
        template <typename P>
        requires concept_point_operater<P>&& concept_point_normal<P>
            class Segment : public GeometryObject
        {
        public:
            //Initialize the segment with no ponit.
            Segment() :points_(0), pids_(0), cursor_(0)
            {

            }
            //Initialize the segment with an array of points.
            //The point data is saved.
            Segment(const std::vector<P>& points) :points_(points), pids_(0), cursor_(0)
            {
                if (points_.size() != 2)
                    throw std::runtime_error("the point number for segment is not equal to 2.");
            }
            //Initialize the segment with an array of points.
            //The point data is not saved.
            Segment(std::vector<P>&& points) :points_(std::move(points)), pids_(0), cursor_(0)
            {
                if (points_.size() != 2)
                    throw std::runtime_error("the point number for segment is not equal to 2.");
            }
            Segment(const P& point0, const P& point1) :points_(2, 0), pids_(0), cursor_(0)
            {
                points_[0] = point0;
                points_[1] = point1;
            }

            Segment(P&& point0, P&& point1) :points_(2, 0), pids_(0), cursor_(0)
            {
                points_[0].share(point0);
                points_[1].share(point1);
            }

            Segment(size_t i0, size_t i1) :points_(0), pids_(2), cursor_(0)
            {
                pids_[0] = i0;
                pids_[1] = i1;
            }

            //Initialize the segment with an array of index.
            Segment(const MemoryPtr<size_t>& pids, size_t cursor) :points_(0), pids_(pids), cursor_(cursor)
            {
                if (pids_.size() != 2)
                    throw std::runtime_error("the point number for segment is not equal to 2.");
            }
            //Copy construct the segment with another segment.
            //The point data is saved but don't set pid.
            Segment(const Segment& segment) : points_(segment.points_), pids_(0), cursor_(0)
            {

            }
            //Move construct the segment with another segment.
            // The point data is not saved.
            Segment(Segment&& segment) : points_(std::move(segment.points_)), pids_(std::move(segment.pids_)), cursor_(segment.cursor_)
            {

            }
            //Assign the segment with another segment.
            //The point data is saved.
            Segment& operator=(const Segment& segment)
            {
                points_ = segment.points_;
                pids_ = MemoryPtr<size_t>(0);
                cursor_ = 0;
            }
            //Assign the segment with another segment.
            //The point data is not saved.
            Segment& operator=(Segment&& segment)
            {
                points_ = std::move(segment.points_);
                pids_ = std::move(segment.pids_);
                cursor_ = segment.cursor_;
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
            }
            //Get the id of the i-th point.
            size_t Pid(size_t i) const
            {
                if (i < points_.size())
                    return pids_[cursor_ + i];
                else
                    return -1;
            }
            //Return the geometry type.
            virtual GeometryType Info() const
            {
                return GeometryType::SEGMENT;
            }
        protected:
            std::vector<P> points_;
            MemoryPtr<size_t> pids_;
            size_t cursor_;
        };
    } // namespace geometry
} // namespace Chimes