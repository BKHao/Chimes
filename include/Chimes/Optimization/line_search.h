#pragma once

namespace Chimes
{
    template <class Scalar>
    class LineSearchMethod
    {
    public:
        virtual void solve() = 0;
    protected:
        template <class Fun>
        Scalar lineSearch(Fun& f, Scalar e);
    };
} // namespace Chimes
