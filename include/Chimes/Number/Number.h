#pragma once
#include <cmath>
namespace Chimes
{
    //Operations related to numerical comparisons and calculations are accessed through singletons.
    class Number
    {
    public:
        //Numerical comparison result, -, +, 0 and error.
        enum class SIGN
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
        static Number& instance()
        {
            static Number _instance;
            return _instance;
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
        SIGN sign(const double& d) const
        {
            if (d > _precision_double)
                return SIGN::POSITIVE;
            else if (d > -_precision_double)
                return SIGN::ZERO;
            else
                return SIGN::NEGATIVE;
        }
        //Numerical comparison for double type.
        SIGN sign(const double& a, const double& b) const
        {
            return sign(a - b);
        }
        //Numerical comparison for float type.
        SIGN sign(const float& f) const
        {
            if (f > _precision_float)
                return SIGN::POSITIVE;
            else if (f > -_precision_float)
                return SIGN::ZERO;
            else
                return SIGN::NEGATIVE;
        }
        //Numerical comparison for float type.
        SIGN sign(const float& a, const float& b) const
        {
            return sign(a - b);
        }
        
        //Set precision for double. The default is 1e-16.
        void set_precision(const double& d)
        {
            _precision_double = d;
        }
        //Set precision for float. The default is 1e-6.
        void set_precision(const float& f)
        {
            _precision_float = f;
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
        Number()
        {
            _precision_double = 1e-16;
            _precision_float = 1e-6f;
        }
    protected:
        double _precision_double;
        float _precision_float;
    };   
}
