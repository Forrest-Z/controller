//
// Created by ljn on 20-4-14.
//

#ifndef PATH_OPTIMIZER_INCLUDE_PATH_OPTIMIZER_REFERENCE_PATH_SMOOTHER_TENSION_SMOOTHER_HPP_
#define PATH_OPTIMIZER_INCLUDE_PATH_OPTIMIZER_REFERENCE_PATH_SMOOTHER_TENSION_SMOOTHER_HPP_
#include <vector>
#include <cppad/cppad.hpp>
#include <cppad/ipopt/solve.hpp>
#include <cfloat>
#include "Eigen/Dense"
#include "Eigen/Sparse"
#include "sample_optimizing_smoother/reference_path_smoother/reference_path_smoother.hpp"

namespace bz_robot
{
namespace PathOptimizationNS {
class Configs;
using CppAD::AD;
class FgEvalReferenceSmoothing {
 public:
    FgEvalReferenceSmoothing(const std::vector<double> &seg_x_list,
                             const std::vector<double> &seg_y_list,
                             const std::vector<double> &seg_s_list,
                             const std::vector<double> &seg_angle_list,
                              std::shared_ptr<Configs> p_configs) :
        seg_s_list_(seg_s_list),
        seg_x_list_(seg_x_list),
        seg_y_list_(seg_y_list),
        seg_angle_list_(seg_angle_list),
         mp_configs(p_configs)
    {

    }
    virtual ~FgEvalReferenceSmoothing() = default;
    typedef CPPAD_TESTVECTOR(AD<double>) ADvector;
    typedef AD<double> ad;
    virtual void operator()(ADvector &fg, const ADvector &vars);
 protected:
    const std::vector<double> &seg_s_list_;
    const std::vector<double> &seg_x_list_;
    const std::vector<double> &seg_y_list_;
    const std::vector<double> &seg_angle_list_;
    std::shared_ptr<Configs> mp_configs;
};

class TensionSmoother : public ReferencePathSmoother {
 public:
    TensionSmoother() = delete;
    TensionSmoother(const std::vector<State> &input_points,
                    const State &start_state,
                    std::shared_ptr<Map> p_grid_map,
                    std::shared_ptr<Configs> p_configs);
    ~TensionSmoother() override = default;

 private:
    bool smooth(PathOptimizationNS::ReferencePath *reference_path,
                std::vector<State> *smoothed_path_display) override;
    virtual bool ipoptSmooth(const std::vector<double> &x_list,
                             const std::vector<double> &y_list,
                             const std::vector<double> &angle_list,
                             const std::vector<double> &k_list,
                             const std::vector<double> &s_list,
                             std::vector<double> *result_x_list,
                             std::vector<double> *result_y_list,
                             std::vector<double> *result_s_list);
    virtual bool osqpSmooth(const std::vector<double> &x_list,
                            const std::vector<double> &y_list,
                            const std::vector<double> &angle_list,
                            const std::vector<double> &k_list,
                            const std::vector<double> &s_list,
                            std::vector<double> *result_x_list,
                            std::vector<double> *result_y_list,
                            std::vector<double> *result_s_list);
    virtual void setHessianMatrix(size_t size, Eigen::SparseMatrix<double> *matrix_h) const;
    virtual void setConstraintMatrix(const std::vector<double> &x_list,
                                     const std::vector<double> &y_list,
                                     const std::vector<double> &angle_list,
                                     const std::vector<double> &k_list,
                                     const std::vector<double> &s_list,
                                     Eigen::SparseMatrix<double> *matrix_constraints,
                                     Eigen::VectorXd *lower_bound,
                                     Eigen::VectorXd *upper_bound) const;
};

}
}
#endif //PATH_OPTIMIZER_INCLUDE_PATH_OPTIMIZER_REFERENCE_PATH_SMOOTHER_TENSION_SMOOTHER_HPP_
