// 05/05/2022 by BKHao in Chimes.
#pragma once

#include <Chimes/Core/template_concept.h>
#include <Chimes/Core/MemoryPool/memory_alloc.h>
#include <Chimes/Geometry/geometry_object.h>

namespace Chimes
{
	namespace geometry
	{
        //The base class of point, which requires a template of real number class. Contains a number array, set the visibility according to the actual type.
        template <typename Real>
            requires concept_real<Real>
        class Point : public GeometryObject
        {
        public:
            //Corresponding class for real number.
            using R = Real;
            //Default initialization called by subclasses. Initialize coordinates to 0.
            Point() : coordinates_(0)
            {

            }
            //Get a reference to an array of coordinates.
            MemoryPtr<Real>& Data()
            {
                return coordinates_;
            }
            //Overloads [] to obtain homogeneous coordinates, and is overloaded by subclasses to set visibility.
            //Pure virtual function.
            virtual Real operator[](size_t i) const = 0;

            virtual Real& operator()(size_t i) = 0;
            //Get the length.
            //Pure virtual function.
            virtual Real Norm() const = 0;
            //Get the squared distance.
            //Pure virtual function.
            virtual Real SquaredNorm() const = 0;
            //Get a string describing the type of the object.
            //Pure virtual function.
            virtual GeometryType Info() const = 0;
            //Normalize the vector.
            //Pure virtual function.
            virtual void Normalize() = 0;
            //Get the dimension.
            //Pure virtual function.
            virtual int Dimension() const = 0;
        protected:
            Point(size_t n) : coordinates_(n)
            {

            }
        protected:
            //coordinates array.
            MemoryPtr<Real> coordinates_;
        };

        //2D point, (x, y), inherited from Point.
        template <typename Real>
        class Point2 :public Point<Real>
        {
        private:
            using Base = Point<Real>;
        public:
            //The coordinates are initialized to (0, 0) by default.
            Point2(): Base(2)
            {
                Base::coordinates_[0] = 0;
                Base::coordinates_[1] = 0;
            }
            //The coordinates are initialized to (a, b).
            Point2(const Real& a, const Real& b) : Base(2)
            {
                Base::coordinates_[0] = a;
                Base::coordinates_[1] = b;
            }
            //Initialize with the coordinates of another 2D point p.
            template <typename R2>
            Point2(const Point2<R2>& p) : Base(2)
            {
                Base::coordinates_[0] = Real(p.x());
                Base::coordinates_[1] = Real(p.y());
            }
            //Initialize with the coordinates of another 2D point p.
            template <typename R2>
            Point2& operator=(const Point2<R2>& p)
            {
                Base::coordinates_ = MemoryPtr<Real>(2);
                Base::coordinates_[0] = Real(p.x());
                Base::coordinates_[1] = Real(p.y());
                return *this;
            }
            //Initialize with the coordinates of another 2D point p.
            Point2(const Point2& p) : Base(2)
            {
                Base::coordinates_[0] = p.x();
                Base::coordinates_[1] = p.y();
            }
            //Initialize with the coordinates of another 2D point p.
            Point2& operator=(const Point2& p)
            {
                Base::coordinates_ = MemoryPtr<Real>(2);
                Base::coordinates_[0] = p.x();
                Base::coordinates_[1] = p.y();
                return *this;
            }
            //Initialize by move constructor, move another point p to this. Please make sure that p will not be used again.
            Point2(Point2<Real>&& p) : Base(2)
            {
                
            }
            //Get the coordinate components, i from 0 to 1.
            virtual Real operator[](size_t i) const
            {
                if (i > 1)
                {
                    std::runtime_error("The index of a 2D point out of range!");
                }
                return Base::coordinates_[i];
            }
            //Get the coordinate components reference, i from 0 to 1.
            virtual Real& operator()(size_t i)
            {
                if (i > 1)
                {
                    std::runtime_error("The index of a 2D point out of range!");
                }
                return Base::coordinates_[i];
            }
            //Get the dimension, return 2.
            virtual int Dimension() const
            {
                return 2;
            }
            //Get the x component.
            Real x() const
            {
                return Base::coordinates_[0];
            }
            //Get the x component.
            Real y() const
            {
                return Base::coordinates_[1];
            }
            //Return the geometry type.
            virtual GeometryType Info() const
            {
                return GeometryType::POINT2;
            }
            //Get the inner product with p. Return a real number.
            template <typename R2>
            Real operator*(const Point2<R2>& p) const
            {
                return Base::coordinates_[0] * p.x() + Base::coordinates_[1] * p.y();
            }
            //Return the point multiplied by t.
            Point2 operator*(const Real& t) const
            {
                return Point2(Base::coordinates_[0] * t, Base::coordinates_[1] * t);
            }
            //Return the point divided by t.
            Point2 operator/(const Real& t) const
            {
                return Point2(Base::coordinates_[0] / t, Base::coordinates_[1] / t);
            }
            //Get the vector from p to the point. Return a 2D point.
            template <typename R2>
            Point2 operator-(const Point2<R2>& p) const
            {
                return Point2(Base::coordinates_[0] - p.x(), Base::coordinates_[1] - p.y());
            }
            //Returns a 2D point whose coordinates are the addition of this point and p.
            template <typename R2>
            Point2 operator+(const Point2<R2>& p)
            {
                return Point2(Base::coordinates_[0] + p.x(), Base::coordinates_[1] + p.y());
            }
            //this point += p
            template <typename P2>
                requires concept_point_index<P2>
            Point2& operator+=(const P2& p)
            {
                Base::coordinates_[0] += p[0];
                Base::coordinates_[1] += p[1];
                return *this;
            }
            //Get the squared distance.
            virtual Real SquaredNorm() const
            {
                return Base::coordinates_[0] * Base::coordinates_[0] + Base::coordinates_[1] * Base::coordinates_[1];
            }
            //Get the length.
            virtual Real Norm() const
            {
                return Numerical::sqrt(SquaredNorm());
            }
            //Normalize the vector.
            virtual void Normalize()
            {
                Real len = Norm();
                Base::coordinates_[0] /= len;
                Base::coordinates_[1] /= len;
            }
            //Return the unitization of the vector (Point_2), the vector itself is unchanged.
            Point2 Normalized() const
            {
                Real len = Norm();
                return Point2(Base::coordinates_[0] / len, Base::coordinates_[1] / len);
            }
            Point2 Abs() const
            {
                return Point2(
                    Numerical::abs(Base::coordinates_[0]),
                    Numerical::abs(Base::coordinates_[1]));
            }
            ////Returns the outer product with a 2D point p. The result is a 3D point Point_3.
            //Point3<Real> operator^(const Point2& p) const
            //{
            //    return Point3(0, 0, Base::coordinates_[0] * p.y() - Base::coordinates_[1] * p.x());
            //}
            //friend std::ostream& operator<< <Real>(std::ostream& os, const Point2& p);
            //friend std::istream& operator>> <Real>(std::istream& is, Point2& p);
        };

        //Send x and y to os.
        template <typename Real>
        inline std::ostream& operator<<(std::ostream& os, const Point2<Real>& p)
        {
            os << p.x() << " " << p.y();
            return os;
        }
        //Set x and y from is.
        template <typename Real>
        inline std::istream& operator>>(std::istream& is, Point2<Real>& p)
        {
            is >> p.coordinates_[0] >> p.coordinates_[1];
            return is;
        }
        ////Get the number multiplied by t. Return a 3D point.
        //template <typename Real>
        //inline Point3<Real> operator*(const Real& t, const Point3<Real>& p)
        //{
        //    return p * t;
        //}
        ////Send x, y and z to os.
        //template <typename Real>
        //inline std::ostream& operator<<(std::ostream& os, const Point3<Real>& p)
        //{
        //    os << p.x() << " " << p.y() << " " << p.z();
        //    return os;
        //}
        ////Set x, y and z from is.
        //template <typename Real>
        //inline std::istream& operator>>(std::istream& is, Point3<Real>& p)
        //{
        //    is >> p.coordinates_.get()[0] >> p.coordinates_.get()[1] >> p.coordinates_.get()[2];
        //    return is;
        //}
	} // namespace geometry
} // namespace Chimes
