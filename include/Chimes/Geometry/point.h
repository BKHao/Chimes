#pragma once
#include <Chimes/Geometry/geometry_object.h>
#include <Chimes/Core/template_concept.h>
#include <vector>

namespace Chimes
{
    namespace geometry
    {
        template<typename Real>
        class Point_2;
        template<typename Real>
        class Point_3;

        //The base class of point, which requires a template of real number class. Contains four real numbers (x, y, z, w), set the visibility according to the actual type.
        template <typename Real>
        requires concept_real<Real>
        class Point : public GeometryObject
        {
        public:
            //Corresponding class for real number.
            using R = Real;
            //Default initialization called by subclasses. Initialize homogeneous coordinates to 0.
            Point() : _coordinates(4, Real())
            {
                _coordinates[3] = 1;
            }
            //Get a reference to an array of homogeneous coordinates.
            std::vector<Real>& data()
            {
                return _coordinates;
            }
            //Overloads [] to obtain homogeneous coordinates, and is overloaded by subclasses to set visibility.
            //Pure virtual function.
            virtual Real operator[](size_t i) const = 0;

            virtual Real& operator()(size_t i) = 0;
            //Get the length.
            //Pure virtual function.
            virtual Real norm() const = 0;
            //Get the squared distance.
            //Pure virtual function.
            virtual Real squared_norm() const = 0;
            //Get a string describing the type of the object.
            //Pure virtual function.
            virtual std::string info() const = 0;
            //Normalize the vector.
            //Pure virtual function.
            virtual void normalize() = 0;
            //Get the dimension.
            //Pure virtual function.
            virtual int dimension() const = 0;
        protected:
            //Homogeneous coordinates (x, y, z, w).
            std::vector<Real> _coordinates;
        };
        //2D point, inherited from Point. z and w are not visible in homogeneous coordinates.
        template <typename Real>
        class Point_2 :public Point<Real>
        {
        private:
            using Base = Point<Real>;
        public:
            //The coordinates are initialized to (0, 0, 0, 1) by default.
            Point_2()
            {

            }
            //The coordinates are initialized to (a, b, 0, 1) by default.
            Point_2(const Real& a, const Real& b)
            {
                Base::_coordinates[0] = a;
                Base::_coordinates[1] = b;
            }
            //Initialize with the coordinates of another 2D point p.
            template <typename R2>
            Point_2(const Point_2<R2>& p)
            {
                Base::_coordinates[0] = p.x();
                Base::_coordinates[1] = p.y();
            }
            //Get the coordinate components, i from 0 to 1.
            virtual Real operator[](size_t i) const
            {
                if (i > 1)
                {
                    return Real(0);
                }
                return Base::_coordinates[i];
            }
            virtual Real& operator()(size_t i)
            {
                if (i > 1)
                {
                    return Base::_coordinates[3];
                }
                return Base::_coordinates[i];
            }
            //Get the dimension, return 2.
            virtual int dimension() const
            {
                return 2;
            }
            //Get the x component.
            Real x() const
            {
                return Base::_coordinates[0];
            }
            //Get the x component.
            Real y() const
            {
                return Base::_coordinates[1];
            }
            //Return string "Point2".
            virtual std::string info() const
            {
                return "Point2";
            }
            //Get the inner product with p. Return a real number.
            template <typename R2>
            Real operator*(const Point_2<R2>& p) const
            {
                return Base::_coordinates[0] * p.x() + Base::_coordinates[1] * p.y();
            }
            //Return the point multiplied by t.
            Point_2 operator*(const Real& t) const
            {
                return Point_2(Base::_coordinates[0] * t, Base::_coordinates[1] * t);
            }
            //Return the point divided by t.
            Point_2 operator/(const Real& t) const
            {
                return Point_2(Base::_coordinates[0] / t, Base::_coordinates[1] / t);
            }
            //Get the vector from p to the point. Return a 2D point.
            template <typename R2>
            Point_2 operator-(const Point_2<R2>& p) const
            {
                return Point_2(Base::_coordinates[0] - p.x(), Base::_coordinates[1] - p.y());
            }
            //Returns a 2D point whose coordinates are the addition of this point and p.
            template <typename R2>
            Point_2 operator+(const Point_2<R2>& p)
            {
                return Point_2(Base::_coordinates[0] + p.x(), Base::_coordinates[1] + p.y());
            }
            //Get the squared distance.
            virtual Real squared_norm() const
            {
                return Base::_coordinates[0] * Base::_coordinates[0] + Base::_coordinates[1] * Base::_coordinates[1];
            }
            //Get the length.
            virtual Real norm() const
            {
                return Numerical::sqrt(squared_norm());
            }
            //Normalize the vector.
            virtual void normalize()
            {
                Real len = norm();
                Base::_coordinates[0] /= len;
                Base::_coordinates[1] /= len;
            }
            //Return the unitization of the vector (Point_2), the vector itself is unchanged.
            Point_2 normalized() const
            {
                Real len = norm();
                return Point_2(Base::_coordinates[0] / len, Base::_coordinates[1] / len);
            }
            Point_2 abs() const
            {
                return Point_2(
                    Numerical::abs(Base::_coordinates[0]),
                    Numerical::abs(Base::_coordinates[1]));
            }
            //Returns the outer product with a 2D point p. The result is a 3D point Point_3.
            Point_3<Real> operator^(const Point_2& p) const
            {
                return Point_3(0, 0, Base::_coordinates[0] * p.y() - Base::_coordinates[1] * p.x());
            }
            friend std::ostream& operator<< <Real>(std::ostream& os, const Point_2& p);
            friend std::istream& operator>> <Real>(std::istream& is, Point_2& p);
        };

        ////3D point, inherited from Point. w is not visible in homogeneous coordinates.
        template <typename Real>
        class Point_3 : public Point<Real>
        {
        private:
            using Base = Point<Real>;
        public:
            //The coordinates are initialized to (0, 0, 0, 1) by default.
            Point_3()
            {

            }
            //The coordinates are initialized to (a, b, c, 1) by default.
            Point_3(const Real& a, const Real& b, const Real& c)
            {
                Base::_coordinates[0] = a;
                Base::_coordinates[1] = b;
                Base::_coordinates[2] = c;
            }
            //The coordinates are initialized to (a, b, 0, 1) by default.
            Point_3(const Real& a, const Real& b)
            {
                Base::_coordinates[0] = a;
                Base::_coordinates[1] = b;
            }
            //Initialize with the coordinates of another 3D point p.
            template <typename R2>
            Point_3(const Point_3<R2>& p)
            {
                Base::_coordinates[0] = p.x();
                Base::_coordinates[1] = p.y();
                Base::_coordinates[2] = p.z();
            }
            //Initialize with the coordinates of another 2D point p, and z = 0;
            template <typename R2>
            Point_3(const Point_2<R2>& p)
            {
                Base::_coordinates[0] = p.x();
                Base::_coordinates[1] = p.y();
            }
            //Get the coordinate components, i from 0 to 2.
            virtual Real operator[](size_t i) const
            {
                if (i > 2)
                {
                    return Real(0);
                }
                return Base::_coordinates[i];
            }
            virtual Real& operator()(size_t i)
            {
                if (i > 2)
                {
                    return Base::_coordinates[3];
                }
                return Base::_coordinates[i];
            }
            //Get the dimension, return 3.
            virtual int dimension() const
            {
                return 3;
            }
            //Get the x component.
            Real x() const
            {
                return Base::_coordinates[0];
            }
            //Get the y component.
            Real y() const
            {
                return Base::_coordinates[1];
            }
            //Get the z component.
            Real z() const
            {
                return Base::_coordinates[2];
            }
            //Return string "Point3".
            virtual std::string info() const
            {
                return "Point3";
            }
            //Get the inner product with 2D point p. Return a real number.
            template <typename R2>
            Real operator*(const Point_2<R2>& p) const
            {
                return Base::_coordinates[0] * p.x() + Base::_coordinates[1] * p.y();
            }
            //Return the inner product with 3D point p.
            template <typename R2>
            Real operator*(const Point_3<R2>& p) const
            {
                return Base::_coordinates[0] * p.x() + Base::_coordinates[1] * p.y() + Base::_coordinates[2] * p.z();
            }

            //Return the inner product with 3D point p, which has p[].
            template <typename P3>
            requires Concept_Point_Index<P3>
            Real operator*(const P3& p) const
            {
                return Base::_coordinates[0] * p[0] + Base::_coordinates[1] * p[1] + Base::_coordinates[2] * p[2];
            }

            //Return the result of a matrix3 m*p, m has operator[][].
            template <typename Matrix3>
            requires Concept_Matrix_Index<Matrix3>
            Point_3 transform(const Matrix3& m) const
            {
                return Point_3(
                    Base::_coordinates[0] * m(0, 0) + Base::_coordinates[1] * m(0, 1) + Base::_coordinates[2] * m(0, 2),
                    Base::_coordinates[0] * m(1, 0) + Base::_coordinates[1] * m(1, 1) + Base::_coordinates[2] * m(1, 2),
                    Base::_coordinates[0] * m(2, 0) + Base::_coordinates[1] * m(2, 1) + Base::_coordinates[2] * m(2, 2));
            }

            //Return the result of a matrix3 m^T*p, m has operator[][].
            template <typename Matrix3>
            requires Concept_Matrix_Index<Matrix3>
                Point_3 transformT(const Matrix3& m) const
            {
                return Point_3(
                    Base::_coordinates[0] * m(0, 0) + Base::_coordinates[1] * m(1, 0) + Base::_coordinates[2] * m(2, 0),
                    Base::_coordinates[0] * m(0, 1) + Base::_coordinates[1] * m(1, 1) + Base::_coordinates[2] * m(2, 1),
                    Base::_coordinates[0] * m(0, 2) + Base::_coordinates[1] * m(1, 2) + Base::_coordinates[2] * m(2, 2));
            }

            //Return the point multiplied by t.
            Point_3 operator*(const Real& t) const
            {
                return Point_3(Base::_coordinates[0] * t, Base::_coordinates[1] * t, Base::_coordinates[2] * t);
            }
            //Return the point divided by t.
            Point_3 operator/(const Real& t) const
            {
                return Point_3(Base::_coordinates[0] / t, Base::_coordinates[1] / t, Base::_coordinates[2] / t);
            }

            //Get the vector from p to the point. Return a 3D point.
            template <typename R2>
            Point_3 operator-(const Point_2<R2>& p) const
            {
                return Point_3(Base::_coordinates[0] - p.x(), Base::_coordinates[1] - p.y(), Base::_coordinates[2]);
            }
            
            //Get the vector from p to the point. Return a 3D point.
            template <typename R2>
            Point_3 operator-(const Point_3<R2>& p) const
            {
                return Point_3(Base::_coordinates[0] - p.x(), Base::_coordinates[1] - p.y(), Base::_coordinates[2] - p.z());
            }
            //Get the vector from p to the point. Return a 3D point.
            template <typename P3>
            requires Concept_Point_Index<P3>
            Point_3 operator-(const P3& p) const
            {
                return Point_3(Base::_coordinates[0] - p[0], Base::_coordinates[1] - p[1], Base::_coordinates[2] - p[2]);
            }
            //Return a 3D point whose coordinates are the addition of this point and p.
            template <typename R2>
            Point_3 operator+(const Point_2<R2>& p) const
            {
                return Point_3(Base::_coordinates[0] + p.x(), Base::_coordinates[1] + p.y(), Base::_coordinates[2]);
            }
            //Return a 3D point whose coordinates are the addition of this point and p.
            template <typename R2>
            Point_3 operator+(const Point_3<R2>& p) const
            {
                return Point_3(Base::_coordinates[0] + p.x(), Base::_coordinates[1] + p.y(), Base::_coordinates[2] + p.z());
            }
            //Return a 3D point whose coordinates are the addition of this point and p.
            template <typename P3>
            requires Concept_Point_Index<P3>
            Point_3 operator+(const P3& p) const
            {
                return Point_3(Base::_coordinates[0] + p[0], Base::_coordinates[1] + p[1], Base::_coordinates[2] + p[2]);
            }
            //this point -= p
            template <typename P3>
            requires Concept_Point_Index<P3>
            Point_3& operator-=(const P3& p)
            {
                Base::_coordinates[0] -= p[0];
                Base::_coordinates[1] -= p[1];
                Base::_coordinates[2] -= p[2];
                return *this;
            }
            //this point += p
            template <typename P3>
            requires Concept_Point_Index<P3>
            Point_3& operator+=(const P3& p)
            {
                Base::_coordinates[0] += p[0];
                Base::_coordinates[1] += p[1];
                Base::_coordinates[2] += p[2];
                return *this;
            }
            

            //Get the squared distance.
            virtual Real squared_norm() const
            {
                return Base::_coordinates[0] * Base::_coordinates[0] + Base::_coordinates[1] * Base::_coordinates[1] + Base::_coordinates[2] * Base::_coordinates[2];
            }
            //Get the lenght.
            virtual Real norm() const
            {
                return sqrt(squared_norm());
            }
            //Normalize the vector.
            virtual void normalize()
            {
                Real len = norm();
                if (Numerical::instance().sign(len) != Numerical::Sign::ZERO)
                {
                    Base::_coordinates[0] /= len;
                    Base::_coordinates[1] /= len;
                    Base::_coordinates[2] /= len;
                }                
            }
            //Return the unitization of the vector (Point_3), the vector itself is unchanged.
            Point_3 normalized() const
            {
                Real len = norm();
                if (Numerical::instance().sign(len) != Numerical::Sign::ZERO)
                {
                    return Point_3(Base::_coordinates[0] / len, Base::_coordinates[1] / len, Base::_coordinates[2] / len);
                }
                return *this;
            }
            Point_3 abs() const
            {
                return Point_3(
                    Numerical::abs(Base::_coordinates[0]),
                    Numerical::abs(Base::_coordinates[1]),
                    Numerical::abs(Base::_coordinates[2]));
            }
            //Returns the outer product with a 2D point p. The result is a 3D point Point_3.
            Point_3 operator^(const Point_3& p) const
            {
                return Point_3(Base::_coordinates[1] * p.z() - Base::_coordinates[2] * p.y(), Base::_coordinates[2] * p.x() - Base::_coordinates[0] * p.z(), Base::_coordinates[0] * p.y() - Base::_coordinates[1] * p.x());
            }
            friend Point_3 operator* <Real>(const Real& t, const Point_3& p);
            friend std::ostream& operator<< <Real>(std::ostream& os, const Point_3& p);
            friend std::istream& operator>> <Real>(std::istream& is, Point_3& p);
        };
        //Send x and y to os.
        template <typename Real>
        inline std::ostream& operator<<(std::ostream& os, const Point_2<Real>& p)
        {
            os << p.x() << " " << p.y();
            return os;
        }
        //Set x and y from is.
        template <typename Real>
        inline std::istream& operator>>(std::istream& is, Point_2<Real>& p)
        {
            is >> p._coordinates[0] >> p._coordinates[1];
            return is;
        }
        //Get the number multiplied by t. Return a 3D point.
        template <typename Real>
        inline Point_3<Real> operator*(const Real& t, const Point_3<Real>& p)
        {
            return p * t;
        }
        //Send x, y and z to os.
        template <typename Real>
        inline std::ostream& operator<<(std::ostream& os, const Point_3<Real>& p)
        {
            os << p.x() << " " << p.y() << " " << p.z();
            return os;
        }
        //Set x, y and z from is.
        template <typename Real>
        inline std::istream& operator>>(std::istream& is, Point_3<Real>& p)
        {
            is >> p._coordinates[0] >> p._coordinates[1] >> p._coordinates[2];
            return is;
        }

    }
}
