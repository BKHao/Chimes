#pragma once

#include <Chimes/Optimization/line_search.h>
#include <time.h>

namespace Chimes
{
    template <class Fun, class Scalar = double>
    class SteepestDescent : public LineSearchMethod<Fun, Scalar>
    {
    private:
        using Base = LineSearchMethod<Fun, Scalar>;
    public:
        SteepestDescent(Fun& fun, const typename Base::Vector& init_x) : Base(fun, init_x)
        {
            
        }
        ~SteepestDescent()
        {

        }
        virtual void solve()
        {
            const clock_t start_t = clock();
            const int n = Base::init_x_.size();
            typename Base::Vector gradient(n);
            gradient.setZero();
            typename Base::Vector iter_x = Base::init_x_;
            Scalar fval = fun_(iter_x, gradient);
            if (Base::parameter_.is_show_)
            {
                std::cout << 0 << "\t" << 0 << "\t" << (clock() - start_t) * 1.0 / CLOCKS_PER_SEC << "\t" << gradient.norm() << "\t"
                    << fval << std::endl;
            }
            typename Base::Vector direction = -gradient;
            int k = 0;
            int l = 0;
            Scalar step = Scalar(1.0) / direction.norm();
            while (1)
            {
                if (gradient.norm() < Base::parameter_.epsilon_)
                {
                    if (Base::parameter_.is_show_)
                    {
                        std::cout << "[info][SteepestDescent]reach the gradient tolerance" << std::endl;
                    }
                    break;
                }
                if (Base::parameter_.max_time_ > 0 && Base::parameter_.max_time_ < (clock() - start_t) * 1000.0 / CLOCKS_PER_SEC)
                {
                    if (Base::parameter_.is_show_)
                    {
                        std::cout << "[info][SteepestDescent]reach the max time" << std::endl;
                    }
                    break;
                }
                if (Base::parameter_.max_iteration_ != 0 && Base::parameter_.max_iteration_ == k)
                {
                    if (Base::parameter_.is_show_)
                    {
                        std::cout << "[info][SteepestDescent]reach the max itertion time" << std::endl;
                    }
                    break;
                }
                int num_step_search = Base::stepSearch(fval, iter_x, gradient, step, direction);
                if (num_step_search == Base::parameter_.max_stepsearch_)
                {
                    if (Base::parameter_.is_show_)
                    {
                        std::cout << "[info][SteepestDescent]reach the max stepsearch time" << std::endl;
                    }
                    break;
                }
                if (num_step_search < 0)
                {
                    if (Base::parameter_.is_show_)
                    {
                        std::cout << "[info][SteepestDescent]can't fine a right step" << std::endl;
                    }
                    break;
                }
                l += num_step_search;
                k++;
                if (Base::parameter_.is_show_)
                {
                    std::cout << k << "\t" << l << "\t" << (clock() - start_t) * 1.0 / CLOCKS_PER_SEC << "\t" << gradient.norm()
                        << "\t" << fval << std::endl;
                }
                direction = -gradient;
                step = Scalar(1.0) / direction.norm();
            }
            Base::result_.fval = fval;
            Base::result_.res_x = iter_x;
            Base::result_.res_gradient = gradient;
            Base::result_.iter_time = k;
            Base::result_.stepsearch_time = l;
        }
    };
} // namespace Chimes
