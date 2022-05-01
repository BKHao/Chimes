#pragma once
#include <Chimes/Core/numerical.h>
namespace Chimes
{
    namespace geometry
    {
        //Describe the conditions for geometry that a real number type needs to meet.
        template<typename Real>
        concept concept_real = requires(Real r, const Real & t, Numerical::Sign s)
        {
            r = t;
            r = r + t;
            r = r * t;
            r = r - t;
            r = r / t;
            r = Real();
            r = Real(0);
            r = Real(1);
            r = Real(0.5);
            r = Numerical::sqrt(t);
            s = Numerical::instance().sign(r);
        };
        //Describe the point whose coordinates are obtained by index.
        template <typename P>
        concept concept_point_index = requires(P p, size_t i)
        {
            Numerical::is_real(p[i]);
        };
        //Describe the point whose coordinates are obtained by xyz.
        template <typename P>
        concept concept_point_xyz = requires(P p)
        {
            Numerical::is_real(p.x());
            Numerical::is_real(p.y());
            Numerical::is_real(p.z());
        };
        //Describe the operation of the point.
        template <typename P>
        concept concept_point_operater = requires(P p, P q, P r, typename P::R t)
        {
            r = p - q;
            r = p + q;
            r = p * t;
            t = p * q;
        };
        //Describe the unitization of points.
        template <typename P>
        concept concept_point_normal = requires(P p, P q, typename P::R t)
        {
            t = p.norm();
            t = p.squared_norm();
            q = p.normalized();
            p.normalize();
        };
        //Describe the operation of points and out stream.
        template <typename P>
        concept concept_point_ostream = requires(P p, P q, std::ostream os)
        {
            os << p << q;
        };
        //Describe the operation of points and input stream.
        template <typename P>
        concept concept_point_istream = requires(P p, P q, std::istream is)
        {
            is >> p >> q;
        };

        template <typename Matrix>
        concept concept_matrix_index = requires(Matrix m, size_t i, size_t j)
        {
            Numerical::is_real(m(i, j));
        };
    }
}