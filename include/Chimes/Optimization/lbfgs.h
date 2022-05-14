// 14/05/2022 by BKHao in Chimes.
#include <Chimes/Optimization/line_search.h>
#include <time.h>
#include <vector>

namespace Chimes
{
    template <class Fun, class Scalar = double>
    class LBFGS : public LineSearchMethod<Fun, Scalar>
    {
    private:
        using Base = LineSearchMethod<Fun, Scalar>;
    public:
        LBFGS(Fun& fun, const typename Base::Vector& init_x) : Base(fun, init_x)
        {

        }
        virtual void solve()
        {
            const clock_t start_t = clock();
            const size_t n = Base::init_x_.size();
            typename Base::Vector gradient(n);
            typename Base::Vector old_gradient(n);
            gradient.setZero();
            old_gradient.setZero();
            std::vector<typename Base::Vector> s(Base::parameter_.lbfgs_remain_);
            std::vector<typename Base::Vector> y(Base::parameter_.lbfgs_remain_);
            std::vector<Scalar> ys(Base::parameter_.lbfgs_remain_);
            std::vector<Scalar> alpha(Base::parameter_.lbfgs_remain_);
            typename Base::Vector iter_x = Base::init_x_;
            Scalar fval = Base::fun_(iter_x, gradient);
            old_gradient = gradient;
            if (Base::parameter_.is_show_)
            {
                std::cout << 0 << "\t" << 0 << "\t" << (clock() - start_t) * 1.0 / CLOCKS_PER_SEC << "\t" << gradient.norm() << "\t"
                    << fval << std::endl;
            }
            typename Base::Vector direction = -gradient;
            size_t k = 0;
            size_t l = 0;
            size_t cursor = 0;
            Scalar step = Scalar(1.0) / direction.norm();
            while (1)
            {
                if (gradient.norm() < Base::parameter_.epsilon_)
                {
                    if (Base::parameter_.is_show_)
                    {
                        std::cout << "[info][LBFGS]reach the gradient tolerance" << std::endl;
                    }
                    break;
                }
                if (Base::parameter_.max_time_ > 0 && Base::parameter_.max_time_ < (clock() - start_t) * 1000.0 / CLOCKS_PER_SEC)
                {
                    if (Base::parameter_.is_show_)
                    {
                        std::cout << "[info][LBFGS]reach the max time" << std::endl;
                    }
                    break;
                }
                if (Base::parameter_.max_iteration_ != 0 && Base::parameter_.max_iteration_ == k)
                {
                    if (Base::parameter_.is_show_)
                    {
                        std::cout << "[info][LBFGS]reach the max itertion time" << std::endl;
                    }
                    break;
                }
                size_t num_step_search = Base::stepSearch(fval, iter_x, gradient, step, direction);
                if (num_step_search == Base::parameter_.max_stepsearch_)
                {
                    if (Base::parameter_.is_show_)
                    {
                        std::cout << "[info][LBFGS]reach the max stepsearch time" << std::endl;
                    }
                    break;
                }
                if (num_step_search < 0)
                {
                    if (Base::parameter_.is_show_)
                    {
                        std::cout << "[info][LBFGS]can't fine a right step" << std::endl;
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
                s[cursor] = step * direction;
                y[cursor] = gradient - old_gradient;
                old_gradient = gradient;

                Scalar _ys = y[cursor].dot(s[cursor]);
                Scalar _yy = y[cursor].dot(y[cursor]);
                ys[cursor] = _ys;
                cursor = (cursor + 1) % Base::parameter_.lbfgs_remain_;

                size_t incr, bound;
                if (k < Base::parameter_.lbfgs_remain_)
                {
                    incr = 0;
                    bound = k;
                }
                else
                {
                    incr = cursor;
                    bound = Base::parameter_.lbfgs_remain_;
                }
                direction = -gradient;
                size_t j = cursor;
                for (size_t i = 0; i < bound; ++i)
                {
                    j = (j + Base::parameter_.lbfgs_remain_ - 1) % Base::parameter_.lbfgs_remain_;
                    alpha[j] = s[j].dot(direction) / ys[j];
                    direction = direction - alpha[j] * y[j];
                }
                direction = (_ys / _yy) * direction;
                for (size_t i = 0; i < bound; ++i)
                {
                    Scalar beta = y[j].dot(direction) / ys[j];
                    direction = direction + (alpha[j] - beta) * s[j];
                    j = (j + 1) % Base::parameter_.lbfgs_remain_;
                }                
                step = Scalar(1.0);
            }
            Base::result_.fval = fval;
            Base::result_.res_x = iter_x;
            Base::result_.res_gradient = gradient;
            Base::result_.iter_time = k;
            Base::result_.stepsearch_time = l;
        }
    };
} // namespace Chimes
