#pragma once
#include <cmath>
namespace Chimes
{
    //Operations related to numerical comparisons and calculations are accessed through singletons.
    class Numerical
    {
    public:
        //Numerical comparison result, -, +, 0 and error.
        enum class Sign
        {
            ZERO,
            POSITIVE,
            NEGATIVE,
            ERROR
        };
    public:
        static void is_real(const float& f)
        {
            return;
        }
        static void is_real(const double& d)
        {
            return;
        }
        //Get singleton instance.
        static Numerical& instance()
        {
            static Numerical instance_;
            return instance_;
        }
        //Square root
        static double sqrt(const double& d)
        {
            return std::sqrt(d);
        }
        //Square root
        static float sqrt(const float& f)
        {
            return std::sqrtf(f);
        }
        //If in<min, return min
        //if in>max, return max
        //else return in
        static float clamp(const float& in, const float& min, const float& max)
        {
            if (in < min)
                return min;
            else if (in > max)
                return max;
            else
                return in;
        }
        //If in<min, return min
        //if in>max, return max
        //else return in
        static double clamp(const double& in, const double& min, const double& max)
        {
            if (in < min)
                return min;
            else if (in > max)
                return max;
            else
                return in;
        }
        //Numerical comparison for double type.
        Sign sign(const double& d) const
        {
            if (d > precision_double_)
                return Sign::POSITIVE;
            else if (d > -precision_double_)
                return Sign::ZERO;
            else
                return Sign::NEGATIVE;
        }
        //Numerical comparison for double type.
        Sign sign(const double& a, const double& b) const
        {
            return sign(a - b);
        }
        //Numerical comparison for float type.
        Sign sign(const float& f) const
        {
            if (f > precision_float_)
                return Sign::POSITIVE;
            else if (f > -precision_float_)
                return Sign::ZERO;
            else
                return Sign::NEGATIVE;
        }
        //Numerical comparison for float type.
        Sign sign(const float& a, const float& b) const
        {
            return sign(a - b);
        }
        
        //Set precision for double. The default is 1e-16.
        void set_precision(const double& d)
        {
            precision_double_ = d;
        }
        //Set precision for float. The default is 1e-6.
        void set_precision(const float& f)
        {
            precision_float_ = f;
        }
        static double abs(const double& d)
        {
            return fabs(d);
        }
        static float abs(const float& f)
        {
            return fabs(f);
        }
    protected:
        Numerical()
        {
            precision_double_ = 1e-16;
            precision_float_ = 1e-6f;
        }
    protected:
        double precision_double_;
        float precision_float_;
    };   
}
