#pragma once
#include <string>
namespace Chimes
{
    //This namespace contains geometry-related classes.
    namespace geometry
    {
        enum class GeometryType
        {
            META,
            POINT,
            POINT2,
            POINT3,
            SIMPLE_POLYGON,
            CONVEX_POLYGON,
            TRIANGLE,
            POLYGON_MESH,
            TRIANGLE_MESH,
        };
        //Chimes's geometry object class, all geometry objects are based on it.
        class GeometryObject
        {
        public:
            //The default initialization sets the id to 0.
            GeometryObject();
            //Set the (int)id for the object. Default is 0.
            virtual void set_guid(int id);
            //Get the id of the object.
            virtual int guid() const;
            //Get a string describing the type of the object.
            virtual GeometryType Info() const = 0;
        protected:
            int guid_;
        };
    }

}