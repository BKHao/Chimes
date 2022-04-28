#pragma once
#include <Eigen/Core>
namespace Chimes
{
    template <class Scalar>
    class LineSearchMethod
    {
    public:
        template <class Fun>
        LineSearchMethod(Fun& fun, Eigen::Matrix<Scalar, Eigen::Dynamic, 1>& init_x)
        {
                        
        }
        virtual void solve() = 0;
    protected:
        template <class Fun>
        Scalar lineSearch(Fun& fun, Scalar e)
        {
            return Scalar(0);
        }
    };
} // namespace Chimes
