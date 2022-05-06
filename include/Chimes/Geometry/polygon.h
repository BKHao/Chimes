// 06/05/2022 by BKHao in Chimes.

#pragma once
#include <Chimes/Geometry/geometry_object.h>
#include <Chimes/Core/template_concept.h>

namespace Chimes
{
	namespace geometry
	{
        ////A simple polygon composed of an array of points, the order of the points indicates the boundary connection. Represent the points through smart pointer.
        //template <typename P>
        //    requires concept_point_operater<P>&& concept_point_normal<P>
        //class SimplePolygon : public GeometryObject
        //{
        //private:
        //    static const unsigned int S_COMPLETE = 1;
        //    static const unsigned int S_PROCESSING = 1 << 1;
        //    static const unsigned int S_PID = 1 << 2;
        //public:
        //    //Initialize the polygon with no ponit.
        //    SimplePolygon() :points_(0), state_(0), pids_(nullptr)
        //    {

        //    }
        //    //Initialize the polygon with an array of pointer.
        //    //The point data is not saved.
        //    SimplePolygon(const std::vector<P>& points) :points_(points), state_(1)
        //    {

        //    }
        //    //Initialize the polygon with an array of pointer.
        //    //The point data is not saved.
        //    SimplePolygon(const std::vector<std::shared_ptr<P>>&& points) :points_(points), state_(1)
        //    {

        //    }
        //    //Start to insert the construction polygon point by point. The old data will be cleared.
        //    void Start()
        //    {
        //        state_ = S_PROCESSING;
        //        points_.clear();
        //    }
        //    //Insert a point p.
        //    //The point data is saved.
        //    bool Insert(const P& p)
        //    {
        //        if (state_ & S_PROCESSING)
        //        {
        //            points_.push_back(std::make_shared<P>(p));
        //            return true;
        //        }
        //        return false;
        //    }
        //    //Insert a point p.
        //    //The point data is not saved.
        //    bool Insert(const std::shared_ptr<P>& p)
        //    {
        //        if (state_ & S_PROCESSING)
        //        {
        //            points_.push_back(p);
        //            return true;
        //        }
        //        return false;
        //    }
        //    //End the construction.
        //    bool End()
        //    {
        //        if (points_.size() > 2)
        //        {
        //            state_ &= (~S_PROCESSING);
        //            state_ |= S_COMPLETE;
        //            return true;
        //        }
        //        else
        //        {
        //            state_ = 0;
        //            points_.clear();
        //            return false;
        //        }
        //    }
        //    //Get the const pointer of the i-th point.
        //    std::shared_ptr<P> operator[](size_t i) const
        //    {
        //        if (i < points_.size())
        //            return points_[i];
        //        else
        //            return nullptr;
        //    }
        //    //Set the id of the points. Believe the data is correct.
        //    void SetPid(std::shared_ptr<int>& pids)
        //    {
        //        pids_ = pids;
        //        state_ |= S_PID;
        //    }
        //    //Get the id of the i-th point.
        //    int Pid(size_t i) const
        //    {
        //        if (!(state_ & S_PID))
        //            std::runtime_error("Pid has not been set.");
        //        if (i < points_.size())
        //            return pids_.get()[i];
        //        else
        //            return -1;
        //    }
        //    //Return the geometry type.
        //    virtual GeometryType Info() const
        //    {
        //        return GeometryType::SIMPLE_POLYGON;
        //    }
        //protected:
        //    std::vector<P> points_;
        //    unsigned int state_;
        //    std::shared_ptr<int> pids_;
        //};
	} // namespace geometry
} // namespace Chimes
