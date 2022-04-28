#pragma once
#include <Eigen/Core>
namespace Chimes
{
    template <class Fun, class Scalar=double>
    class LineSearchMethod
    {
    protected:
        //using Vector = Eigen::MatrixBase<Eigen::Matrix<Scalar, Eigen::Dynamic, 1>>;
        using Vector = Eigen::Matrix<Scalar, Eigen::Dynamic, 1>;
    public:
        enum class StepSearchMethod
        {
            SUFFICIENT_DECREASE,
            WOLFE,
            STRONG_WOLFE
        };
        class Parameter
        {
        public:
            Parameter() :max_iteration_(1000), max_stepsearch_(100), min_step_(1e-7), min_xtol_(1e-7), min_ftol_(1e-7), epsilon_(1e-5), is_show_(true), descent_rate_(1e-4), wolfe_(0.9), max_time_(-1)
            {
                step_search_method_ = (StepSearchMethod::WOLFE);
            }
        public:
            int max_iteration_;
            int max_stepsearch_;
            Scalar min_step_;
            Scalar min_xtol_;
            Scalar min_ftol_;
            Scalar epsilon_;
            bool is_show_;
            Scalar descent_rate_;
            Scalar wolfe_;
            double max_time_;
            StepSearchMethod step_search_method_;
        };

        class SolveResult
        {
        public:
            Scalar fval;
            Vector res_x;
            Vector res_gradient;
            int iter_time;
            int stepsearch_time;
        };

    public:
        LineSearchMethod(Fun& fun, const Vector& init_x): fun_(fun), init_x_(init_x)
        {
            parameter_ = Parameter();
        }
        virtual void solve() = 0;
        const SolveResult& get_result() const
        {
            return result_;
        }
    protected:
        int stepSearch(Scalar& fval, Vector& iter_x, Vector& gradient, Scalar& step, const Vector& direction)
        {
            if (step < Scalar(0))
            {
                std::cout << "[error][lineSearch] init step < 0" << std::endl;
                throw std::runtime_error("[error][lineSearch] init step < 0");
            }
            const Scalar idg = gradient.dot(direction);
            if (idg > Scalar(0))
            {
                std::cout << "[error][lineSearch] direction is not a descent direction." << std::endl;
                throw std::runtime_error("[error][lineSearch] direction is not a descent direction");
            }
            Scalar ndg = idg;
            const Scalar idescent = parameter_.descent_rate_ * idg;
            const Scalar ifval = fval;
            const Vector ix = iter_x;
            Scalar step_l = 0;
            Scalar step_u = std::numeric_limits<Scalar>::infinity();
            int k = 1;
            while (1)
            {
                iter_x = ix + step * direction;
                fval = fun_(iter_x, gradient);
                if (fval > ifval + step * idescent)
                {
                    step_u = step;
                }
                else
                {
                    if (parameter_.step_search_method_ == StepSearchMethod::SUFFICIENT_DECREASE)
                    {
                        break;
                    }
                    ndg = gradient.dot(direction);
                    if (ndg < parameter_.wolfe_ * idg)
                    {
                        step_l = step;
                    }
                    else
                    {
                        if (parameter_.step_search_method_ == StepSearchMethod::WOLFE)
                        {
                            break;
                        }
                        if (ndg > -parameter_.wolfe_ * idg)
                        {
                            step_u = step;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                if (k == parameter_.max_stepsearch_)
                {
                    ++k;
                    iter_x = ix;
                    break;
                }
                if (step < parameter_.min_step_)
                {
                    k = -1;
                    iter_x = ix;
                    break;
                }
                ++k;
                step = isinf(step_u) ? 2 * step : 0.5 * (step_l + step_u);
            }            
            return k;
        }

    public:
        Parameter parameter_;
    protected:
        Fun& fun_;
        Vector init_x_;
        SolveResult result_;
    };
} // namespace Chimes
