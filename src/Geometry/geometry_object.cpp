#include "Chimes/Geometry/geometry_object.h"

namespace Chimes
{
    namespace geometry
    {
        //The default initialization sets the id to 0.
        GeometryObject::GeometryObject() : guid_(0)
        {
            
        }
        //Set the (int)id for the object. Default is 0.
        void GeometryObject::set_guid(int id)
        {
            guid_ = id;
        }
        //Get the id of the object.
        int GeometryObject::guid() const
        {
            return guid_;
        }
    } // namespace Geometry
} // namespace Chimes