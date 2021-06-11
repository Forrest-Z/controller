//
// Created by ljn on 20-4-14.
//

#ifndef PATH_OPTIMIZER_INCLUDE_PATH_OPTIMIZER_REFERENCE_PATH_SMOOTHER_ANGLE_DIFF_SMOOTHER_HPP_
#define PATH_OPTIMIZER_INCLUDE_PATH_OPTIMIZER_REFERENCE_PATH_SMOOTHER_ANGLE_DIFF_SMOOTHER_HPP_
#include <vector>
#include <cppad/cppad.hpp>
#include <cppad/ipopt/solve.hpp>
#include <cfloat>
//#include <tinyspline_ros/tinysplinecpp.h>
#include "sample_optimizing_smoother/data_struct/data_struct.hpp"
#include "sample_optimizing_smoother/reference_path_smoother/reference_path_smoother.hpp"
namespace bz_robot
{
namespace PathOptimizationNS {
class Configs;
using CppAD::AD;
class FgEvalFrenetSmooth {
 public:
    FgEvalFrenetSmooth(const std::vector<double> &seg_x_list,
                       const std::vector<double> &seg_y_list,
                       const std::vector<double> &seg_angle_list,
                       const std::vector<double> &seg_s_list,
                       const int &N,
                       const std::vector<double> &cost_func);
    typedef CPPAD_TESTVECTOR(AD <double>) ADvector;
    void operator()(ADvector &fg, const ADvector &vars);

 private:
    size_t N;
    const std::vector<double> &seg_s_list_;
    const std::vector<double> &seg_x_list_;
    const std::vector<double> &seg_y_list_;
    const std::vector<double> &seg_angle_list_;
    double cost_func_curvature_weight_{};
    double cost_func_curvature_rate_weight_{};
    double cost_func_bound_weight_{};
    double cost_func_s_weight_{};
};

class AngleDiffSmoother final : public ReferencePathSmoother {
 public:
    AngleDiffSmoother() = delete;
    AngleDiffSmoother(const std::vector<State> &input_points,
                      const State &start_state,
                      std::shared_ptr<Map> p_grid_map,
                      std::shared_ptr<Configs> p_configs);
    ~AngleDiffSmoother() override = default;

 private:
    bool smooth(PathOptimizationNS::ReferencePath *reference_path,
                std::vector<State> *smoothed_path_display) override;
};

}
}
#endif //PATH_OPTIMIZER_INCLUDE_PATH_OPTIMIZER_REFERENCE_PATH_SMOOTHER_ANGLE_DIFF_SMOOTHER_HPP_
