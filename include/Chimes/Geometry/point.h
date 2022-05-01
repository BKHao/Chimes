#pragma once
#include <Chimes/Geometry/geometry_object.h>
#include <Chimes/Core/template_concept.h>
#include <vector>

namespace Chimes
{
    namespace geometry
    {
        template<typename Real>
        class Point2;
        template<typename Real>
        class Point3;

        //The base class of point, which requires a template of real number class. Contains a number array, set the visibility according to the actual type.
        template <typename Real>
        requires concept_real<Real>
        class Point : public GeometryObject
        {
        public:
            //Corresponding class for real number.
            using R = Real;
            //Default initialization called by subclasses. Initialize coordinates to 0.
            Point() : coordinates_(nullptr)
            {
                
            }
            //Get a reference to an array of coordinates.
            std::shared_ptr<Real>& Data()
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
            //coordinates array.
            std::shared_ptr<Real> coordinates_;
        };
        //2D point, (x, y), inherited from Point.
        template <typename Real>
        class Point2 :public Point<Real>
        {
        private:
            using Base = Point<Real>;
        public:
            //The coordinates are initialized to (0, 0) by default.
            Point2()
            {
                Real* data = new Real[2];
                Base::coordinates_ = std::shared_ptr<Real>(data, [](Real* p) {delete[] p; });
                data[0] = Real(0);
                data[1] = Real(0);
            }
            //The coordinates are initialized to (a, b).
            Point2(const Real& a, const Real& b)
            {
                Real* data = new Real[2];
                Base::coordinates_ = std::shared_ptr<Real>(data, [](Real* p) {delete[] p; });
                data[0] = a;
                data[1] = b;
            }
            //Initialize with the coordinates of another 2D point p.
            template <typename R2>
            Point2(const Point2<R2>& p)
            {
                Real* data = new Real[2];
                Base::coordinates_ = std::shared_ptr<Real>(data, [](Real* p) {delete[] p; });
                data[0] = p.x();
                data[1] = p.y();
            }
            //Initialize with the coordinates of another 2D point p.
            template <typename R2>
            Point2& operator=(const Point2<R2>& p)
            {
                Real* data = new Real[2];
                Base::coordinates_ = std::shared_ptr<Real>(data, [](Real* p) {delete[] p; });
                data[0] = p.x();
                data[1] = p.y();
                return *this;
            }
            //Initialize with the coordinates of another 2D point p.
            Point2(const Point2& p)
            {
                Real* data = new Real[2];
                Base::coordinates_ = std::shared_ptr<Real>(data, [](Real* p) {delete[] p; });
                data[0] = p.x();
                data[1] = p.y();
            }
            //Initialize with the coordinates of another 2D point p.
            Point2& operator=(const Point2& p)
            {
                Real* data = new Real[2];
                Base::coordinates_ = std::shared_ptr<Real>(data, [](Real* p) {delete[] p; });
                data[0] = p.x();
                data[1] = p.y();
                return *this;
            }
            //Initialize by move constructor, move another point p to this. Please make sure that p will not be used again.
            Point2(Point2<Real>&& p)
            {
                Base::coordinates_ = std::move(p.Data());
            }
            //Get the coordinate components, i from 0 to 1.
            virtual Real operator[](size_t i) const
            {
                if (i > 1)
                {
                    std::runtime_error("The index of a 2D point out of range!");
                }
                return Base::coordinates_.get()[i];
            }
            //Get the coordinate components reference, i from 0 to 1.
            virtual Real& operator()(size_t i)
            {
                if (i > 1)
                {
                    std::runtime_error("The index of a 2D point out of range!");
                }
                return Base::coordinates_.get()[i];
            }
            //Get the dimension, return 2.
            virtual int Dimension() const
            {
                return 2;
            }
            //Get the x component.
            Real x() const
            {
                return Base::coordinates_.get()[0];
            }
            //Get the x component.
            Real y() const
            {
                return Base::coordinates_.get()[1];
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
                return Base::coordinates_.get()[0] * p.x() + Base::coordinates_.get()[1] * p.y();
            }
            //Return the point multiplied by t.
            Point2 operator*(const Real& t) const
            {
                return Point2(Base::coordinates_.get()[0] * t, Base::coordinates_.get()[1] * t);
            }
            //Return the point divided by t.
            Point2 operator/(const Real& t) const
            {
                return Point2(Base::coordinates_.get()[0] / t, Base::coordinates_.get()[1] / t);
            }
            //Get the vector from p to the point. Return a 2D point.
            template <typename R2>
            Point2 operator-(const Point2<R2>& p) const
            {
                return Point2(Base::coordinates_.get()[0] - p.x(), Base::coordinates_.get()[1] - p.y());
            }
            //Returns a 2D point whose coordinates are the addition of this point and p.
            template <typename R2>
            Point2 operator+(const Point2<R2>& p)
            {
                return Point2(Base::coordinates_.get()[0] + p.x(), Base::coordinates_.get()[1] + p.y());
            }
            //this point += p
            template <typename P2>
            requires concept_point_index<P2>
                Point2& operator+=(const P2& p)
            {
                Base::coordinates_.get()[0] += p[0];
                Base::coordinates_.get()[1] += p[1];
                return *this;
            }
            //Get the squared distance.
            virtual Real SquaredNorm() const
            {
                return Base::coordinates_.get()[0] * Base::coordinates_.get()[0] + Base::coordinates_.get()[1] * Base::coordinates_.get()[1];
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
                Base::coordinates_.get()[0] /= len;
                Base::coordinates_.get()[1] /= len;
            }
            //Return the unitization of the vector (Point_2), the vector itself is unchanged.
            Point2 Normalized() const
            {
                Real len = Norm();
                return Point2(Base::coordinates_.get()[0] / len, Base::coordinates_.get()[1] / len);
            }
            Point2 Abs() const
            {
                return Point2(
                    Numerical::abs(Base::coordinates_.get()[0]),
                    Numerical::abs(Base::coordinates_.get()[1]));
            }
            //Returns the outer product with a 2D point p. The result is a 3D point Point_3.
            Point3<Real> operator^(const Point2& p) const
            {
                return Point3(0, 0, Base::coordinates_.get()[0] * p.y() - Base::coordinates_.get()[1] * p.x());
            }
            friend std::ostream& operator<< <Real>(std::ostream& os, const Point2& p);
            friend std::istream& operator>> <Real>(std::istream& is, Point2& p);
        };

        ////3D point, (x, y, z), inherited from Point.
        template <typename Real>
        class Point3 : public Point<Real>
        {
        private:
            using Base = Point<Real>;
        public:
            //The coordinates are initialized to (0, 0, 0) by default.
            Point3()
            {
                Real* data = new Real[3];
                Base::coordinates_ = std::shared_ptr<Real>(data, [](Real* p) {delete[] p; });
                data[0] = Real(0);
                data[1] = Real(0);
                data[2] = Real(0);
            }
            //The coordinates are initialized to (a, b, c) by default.
            Point3(const Real& a, const Real& b, const Real& c)
            {
                Real* data = new Real[3];
                Base::coordinates_ = std::shared_ptr<Real>(data, [](Real* p) {delete[] p; });
                data[0] = a;
                data[1] = b;
                data[2] = c;
            }
            //The coordinates are initialized to (a, b, 0) by default.
            Point3(const Real& a, const Real& b)
            {
                Real* data = new Real[3];
                Base::coordinates_ = std::shared_ptr<Real>(data, [](Real* p) {delete[] p; });
                data[0] = a;
                data[1] = b;
                data[2] = Real(0);
            }
            //Initialize with the coordinates of another 3D point p.
            template <typename R2>
            Point3(const Point3<R2>& p)
            {
                Real* data = new Real[3];
                Base::coordinates_ = std::shared_ptr<Real>(data, [](Real* p) {delete[] p; });
                data[0] = p.x();
                data[1] = p.y();
                data[2] = p.z();
            }
            //Initialize with the coordinates of another 3D point p.
            template <typename R2>
            Point3& operator=(const Point3<R2>& p)
            {
                Real* data = new Real[3];
                Base::coordinates_ = std::shared_ptr<Real>(data, [](Real* p) {delete[] p; });
                data[0] = p.x();
                data[1] = p.y();
                data[2] = p.z();
                return *this;
            }
            //Initialize with the coordinates of another 3D point p.
            Point3(const Point3& p)
            {
                Real* data = new Real[3];
                Base::coordinates_ = std::shared_ptr<Real>(data, [](Real* p) {delete[] p; });
                data[0] = p.x();
                data[1] = p.y();
                data[2] = p.z();
            }
            //Initialize with the coordinates of another 3D point p.
            Point3& operator=(const Point3& p)
            {
                Real* data = new Real[3];
                Base::coordinates_ = std::shared_ptr<Real>(data, [](Real* p) {delete[] p; });
                data[0] = p.x();
                data[1] = p.y();
                data[2] = p.z();
                return *this;
            }
            //Initialize with the coordinates of another 2D point p, and z = 0;
            template <typename R2>
            Point3(const Point2<R2>& p)
            {
                Real* data = new Real[3];
                Base::coordinates_ = std::shared_ptr<Real>(data, [](Real* p) {delete[] p; });
                data[0] = p.x();
                data[1] = p.y();
                data[2] = Real(0);
            }
            //Initialize by move constructor, move another point p to this. Please make sure that p will not be used again.
            Point3(Point3<Real>&& p) noexcept
            {
                Base::coordinates_ = std::move(p.Data());
            }
            //Get the coordinate components, i from 0 to 2.
            virtual Real operator[](size_t i) const
            {
                if (i > 2)
                {
                    std::runtime_error("The index of a 3D point out of range!");
                }
                return Base::coordinates_.get()[i];
            }
            virtual Real& operator()(size_t i)
            {
                if (i > 2)
                {
                    std::runtime_error("The index of a 3D point out of range!");
                }
                return Base::coordinates_.get()[i];
            }
            //Get the dimension, return 3.
            virtual int Dimension() const
            {
                return 3;
            }
            //Get the x component.
            Real x() const
            {
                return Base::coordinates_.get()[0];
            }
            //Get the y component.
            Real y() const
            {
                return Base::coordinates_.get()[1];
            }
            //Get the z component.
            Real z() const
            {
                return Base::coordinates_.get()[2];
            }
            //Return the geometry type.
            virtual GeometryType Info() const
            {
                return GeometryType::POINT3;
            }
            //Get the inner product with 2D point p. Return a real number.
            template <typename R2>
            Real operator*(const Point2<R2>& p) const
            {
                return Base::coordinates_.get()[0] * p.x() + Base::coordinates_.get()[1] * p.y();
            }
            //Return the inner product with 3D point p.
            template <typename R2>
            Real operator*(const Point3<R2>& p) const
            {
                return Base::coordinates_.get()[0] * p.x() + Base::coordinates_.get()[1] * p.y() + Base::coordinates_.get()[2] * p.z();
            }
            //Return the inner product with 3D point p, which has p[].
            template <typename P3>
            requires concept_point_index<P3>
            Real operator*(const P3& p) const
            {
                return Base::coordinates_.get()[0] * p[0] + Base::coordinates_.get()[1] * p[1] + Base::coordinates_.get()[2] * p[2];
            }
            //Return the result of a matrix3 m*p, m has operator[][].
            template <typename Matrix3>
            requires Concept_Matrix_Index<Matrix3>
            Point3 Transform(const Matrix3& m) const
            {
                return Point3(
                    Base::coordinates_.get()[0] * m(0, 0) + Base::coordinates_.get()[1] * m(0, 1) + Base::coordinates_.get()[2] * m(0, 2),
                    Base::coordinates_.get()[0] * m(1, 0) + Base::coordinates_.get()[1] * m(1, 1) + Base::coordinates_.get()[2] * m(1, 2),
                    Base::coordinates_.get()[0] * m(2, 0) + Base::coordinates_.get()[1] * m(2, 1) + Base::coordinates_.get()[2] * m(2, 2));
            }
            //Return the result of a matrix3 m^T*p, m has operator[][].
            template <typename Matrix3>
            requires Concept_Matrix_Index<Matrix3>
                Point3 TransformT(const Matrix3& m) const
            {
                return Point3(
                    Base::coordinates_.get()[0] * m(0, 0) + Base::coordinates_.get()[1] * m(1, 0) + Base::coordinates_.get()[2] * m(2, 0),
                    Base::coordinates_.get()[0] * m(0, 1) + Base::coordinates_.get()[1] * m(1, 1) + Base::coordinates_.get()[2] * m(2, 1),
                    Base::coordinates_.get()[0] * m(0, 2) + Base::coordinates_.get()[1] * m(1, 2) + Base::coordinates_.get()[2] * m(2, 2));
            }
            //Return the point multiplied by t.
            Point3 operator*(const Real& t) const
            {
                return Point3(Base::coordinates_.get()[0] * t, Base::coordinates_.get()[1] * t, Base::coordinates_.get()[2] * t);
            }
            //Return the point divided by t.
            Point3 operator/(const Real& t) const
            {
                return Point3(Base::coordinates_.get()[0] / t, Base::coordinates_.get()[1] / t, Base::coordinates_.get()[2] / t);
            }

            //Get the vector from p to the point. Return a 3D point.
            template <typename R2>
            Point3 operator-(const Point2<R2>& p) const
            {
                return Point3(Base::coordinates_.get()[0] - p.x(), Base::coordinates_.get()[1] - p.y(), Base::coordinates_.get()[2]);
            }
            
            //Get the vector from p to the point. Return a 3D point.
            template <typename R2>
            Point3 operator-(const Point3<R2>& p) const
            {
                return Point3(Base::coordinates_.get()[0] - p.x(), Base::coordinates_.get()[1] - p.y(), Base::coordinates_.get()[2] - p.z());
            }
            //Get the vector from p to the point. Return a 3D point.
            template <typename P3>
            requires concept_point_index<P3>
            Point3 operator-(const P3& p) const
            {
                return Point3(Base::coordinates_.get()[0] - p[0], Base::coordinates_.get()[1] - p[1], Base::coordinates_.get()[2] - p[2]);
            }
            //Return a 3D point whose coordinates are the addition of this point and p.
            template <typename R2>
            Point3 operator+(const Point2<R2>& p) const
            {
                return Point3(Base::coordinates_.get()[0] + p.x(), Base::coordinates_.get()[1] + p.y(), Base::coordinates_.get()[2]);
            }
            //Return a 3D point whose coordinates are the addition of this point and p.
            template <typename R2>
            Point3 operator+(const Point3<R2>& p) const
            {
                return Point_3(Base::coordinates_.get()[0] + p.x(), Base::coordinates_.get()[1] + p.y(), Base::coordinates_.get()[2] + p.z());
            }
            //Return a 3D point whose coordinates are the addition of this point and p.
            template <typename P3>
            requires concept_point_index<P3>
            Point3 operator+(const P3& p) const
            {
                return Point3(Base::coordinates_.get()[0] + p[0], Base::coordinates_.get()[1] + p[1], Base::coordinates_.get()[2] + p[2]);
            }
            //this point -= p
            template <typename P3>
            requires concept_point_index<P3>
            Point3& operator-=(const P3& p)
            {
                Base::coordinates_.get()[0] -= p[0];
                Base::coordinates_.get()[1] -= p[1];
                Base::coordinates_.get()[2] -= p[2];
                return *this;
            }
            //this point += p
            template <typename P3>
            requires concept_point_index<P3>
            Point3& operator+=(const P3& p)
            {
                Base::coordinates_.get()[0] += p[0];
                Base::coordinates_.get()[1] += p[1];
                Base::coordinates_.get()[2] += p[2];
                return *this;
            }
            //Get the squared distance.
            virtual Real SquaredNorm() const
            {
                return Base::coordinates_.get()[0] * Base::coordinates_.get()[0] + Base::coordinates_.get()[1] * Base::coordinates_.get()[1] + Base::coordinates_.get()[2] * Base::coordinates_.get()[2];
            }
            //Get the lenght.
            virtual Real Norm() const
            {
                return sqrt(SquaredNorm());
            }
            //Normalize the vector.
            virtual void Normalize()
            {
                Real len = Norm();
                if (Numerical::instance().sign(len) != Numerical::Sign::ZERO)
                {
                    Base::coordinates_.get()[0] /= len;
                    Base::coordinates_.get()[1] /= len;
                    Base::coordinates_.get()[2] /= len;
                }                
            }
            //Return the unitization of the vector (Point_3), the vector itself is unchanged.
            Point3 Normalized() const
            {
                Real len = Norm();
                if (Numerical::instance().sign(len) != Numerical::Sign::ZERO)
                {
                    return Point3(Base::coordinates_.get()[0] / len, Base::coordinates_.get()[1] / len, Base::coordinates_.get()[2] / len);
                }
                return *this;
            }
            Point3 Abs() const
            {
                return Point3(
                    Numerical::abs(Base::coordinates_.get()[0]),
                    Numerical::abs(Base::coordinates_.get()[1]),
                    Numerical::abs(Base::coordinates_.get()[2]));
            }
            //Returns the outer product with a 2D point p. The result is a 3D point Point_3.
            Point3 operator^(const Point3& p) const
            {
                return Point3(Base::coordinates_.get()[1] * p.z() - Base::coordinates_.get()[2] * p.y(), Base::coordinates_.get()[2] * p.x() - Base::coordinates_.get()[0] * p.z(), Base::coordinates_.get()[0] * p.y() - Base::coordinates_.get()[1] * p.x());
            }
            friend Point3 operator* <Real>(const Real& t, const Point3& p);
            friend std::ostream& operator<< <Real>(std::ostream& os, const Point3& p);
            friend std::istream& operator>> <Real>(std::istream& is, Point3& p);
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
            is >> p.coordinates_.get()[0] >> p.coordinates_.get()[1];
            return is;
        }
        //Get the number multiplied by t. Return a 3D point.
        template <typename Real>
        inline Point3<Real> operator*(const Real& t, const Point3<Real>& p)
        {
            return p * t;
        }
        //Send x, y and z to os.
        template <typename Real>
        inline std::ostream& operator<<(std::ostream& os, const Point3<Real>& p)
        {
            os << p.x() << " " << p.y() << " " << p.z();
            return os;
        }
        //Set x, y and z from is.
        template <typename Real>
        inline std::istream& operator>>(std::istream& is, Point3<Real>& p)
        {
            is >> p.coordinates_.get()[0] >> p.coordinates_.get()[1] >> p.coordinates_.get()[2];
            return is;
        }
    }
}
