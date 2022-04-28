#pragma once

#include <Chimes/Optimization/line_search.h>
#include <time.h>
#include <iostream>

namespace Chimes
{
    template <class Fun, class Scalar = double>
    class SteepestDescent : public LineSearchMethod<Fun, Scalar>
    {
    private:
        using Base = LineSearchMethod<Fun, Scalar>;
    public:
        SteepestDescent(Fun& fun, const Vector& init_x) : Base(fun, init_x)
        {
            
        }
        ~SteepestDescent()
        {

        }
        virtual void solve()
        {
            const clock_t start_t = clock();
            const int n = init_x_.size();
            Vector gradient(n);
            gradient.setZero();
            Vector iter_x = init_x_;
            Scalar fval = fun_(iter_x, gradient);
            if (parameter_.is_show_)
            {
                std::cout << 0 << "\t" << 0 << "\t" << (clock() - start_t) * 1.0 / CLOCKS_PER_SEC << "\t" << gradient.norm() << "\t"
                    << fval << std::endl;
            }
            Vector direction = -gradient;
            int k = 0;
            int l = 0;
            Scalar step = Scalar(1.0) / direction.norm();
            while (1)
            {
                if (gradient.norm() < parameter_.epsilon_)
                {
                    if (parameter_.is_show_)
                    {
                        std::cout << "[info][SteepestDescent]reach the gradient tolerance" << std::endl;
                    }
                    break;
                }
                if (parameter_.max_time_ > 0 && parameter_.max_time_ < (clock() - start_t) * 1000.0 / CLOCKS_PER_SEC)
                {
                    if (parameter_.is_show_)
                    {
                        std::cout << "[info][SteepestDescent]reach the max time" << std::endl;
                    }
                    break;
                }
                if (parameter_.max_iteration_ != 0 && parameter_.max_iteration_ == k)
                {
                    if (parameter_.is_show_)
                    {
                        std::cout << "[info][SteepestDescent]reach the max itertion time" << std::endl;
                    }
                    break;
                }
                int num_step_search = Base::stepSearch(fval, iter_x, gradient, step, direction);
                if (num_step_search == parameter_.max_stepsearch_)
                {
                    if (parameter_.is_show_)
                    {
                        std::cout << "[info][SteepestDescent]reach the max stepsearch time" << std::endl;
                    }
                    break;
                }
                if (num_step_search < 0)
                {
                    if (parameter_.is_show_)
                    {
                        std::cout << "[info][SteepestDescent]can't fine a right step" << std::endl;
                    }
                    break;
                }
                l += num_step_search;
                k++;
                if (parameter_.is_show_)
                {
                    std::cout << k << "\t" << l << "\t" << (clock() - start_t) * 1.0 / CLOCKS_PER_SEC << "\t" << gradient.norm()
                        << "\t" << fval << std::endl;
                }
                direction = -gradient;
                step = Scalar(1.0) / direction.norm();
            }
            result_.fval = fval;
            result_.res_x = iter_x;
            result_.res_gradient = gradient;
            result_.iter_time = k;
            result_.stepsearch_time = l;
        }
    };
} // namespace Chimes
