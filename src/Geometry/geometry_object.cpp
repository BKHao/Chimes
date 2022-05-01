#include "Chimes/Geometry/geometry_object.h"

namespace Chimes
{
    namespace geometry
    {
        //The default initialization sets the id to 0.
        GeometryObject::GeometryObject() : id_(0)
        {
            
        }
        //Set the (int)id for the object. Default is 0.
        void GeometryObject::set_id(int id)
        {
            id_ = id;
        }
        //Get the id of the object.
        int GeometryObject::id() const
        {
            return id_;
        }
    } // namespace Geometry
} // namespace Chimes