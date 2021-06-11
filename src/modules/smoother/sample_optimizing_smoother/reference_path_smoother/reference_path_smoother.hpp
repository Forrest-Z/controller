//
// Created by ljn on 20-1-31.
//

#ifndef PATH_OPTIMIZER_INCLUDE_REFERENCE_PATH_SMOOTHER_REFERENCE_PATH_SMOOTHER_HPP_
#define PATH_OPTIMIZER_INCLUDE_REFERENCE_PATH_SMOOTHER_REFERENCE_PATH_SMOOTHER_HPP_
#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <string>
#include <queue>
#include <ctime>
#include "sample_optimizing_smoother/tinyspline/tinysplinecpp.h"
#include "sample_optimizing_smoother/tools/spline.h"
#include "sample_optimizing_smoother/data_struct/data_struct.hpp"

namespace bz_robot
{
namespace PathOptimizationNS {
class Configs;
class Map;
class ReferencePath;
// This class uses searching method to improve the quality of the input points (if needed), and
// then uses a smoother to obtain a smoothed reference path.
class ReferencePathSmoother {

 public:
    ReferencePathSmoother() = delete;
    ReferencePathSmoother(const std::vector<State> &input_points,
                          const State &start_state,
                          std::shared_ptr<Map> p_grid_map,
                          std::shared_ptr<Configs> p_configs);
    virtual ~ReferencePathSmoother() = default;

    static std::unique_ptr<ReferencePathSmoother> create(const std::string &type,
                                                         const std::vector<State> &input_points,
                                                         const State &start_state,
                                                         std::shared_ptr<Map> p_grid_map,
                                                         std::shared_ptr<Configs> p_configs);

    bool solve(ReferencePath *reference_path, std::vector<State> *smoothed_path_display = nullptr);
    std::vector<std::vector<double>> display() const;

 protected:
    bool segmentRawReference(std::vector<double> *x_list,
                             std::vector<double> *y_list,
                             std::vector<double> *s_list,
                             std::vector<double> *angle_list,
                             std::vector<double> *k_list) const;
    double getClosestPointOnSpline(const tk::spline &x_s, const tk::spline &y_s, const double max_s) const;
    const State &start_state_;
    //const Map &grid_map_;
    std::shared_ptr<Map> mp_grid_map;
    // Data to be passed into solvers.
    std::vector<double> x_list_, y_list_, s_list_;
    std::shared_ptr<Configs> mp_configs;
 private:
    virtual bool smooth(PathOptimizationNS::ReferencePath *reference_path,
                        std::vector<State> *smoothed_path_display) = 0;
    void bSpline();
    // search.
    bool graphSearch();
    inline bool checkExistenceInClosedSet(const APoint &point) const;
    inline double getG(const APoint &point, const APoint &parent) const;
    inline double getH(const APoint &p) const;
    const std::vector<State> &input_points_;
    // Sampled points in searching process.
    std::vector<std::vector<APoint>> sampled_points_;
    double target_s_{};
    std::priority_queue<APoint *, std::vector<APoint *>, PointComparator> open_set_;
    std::set<const APoint *> closed_set_;
};
}
}
#endif //PATH_OPTIMIZER_INCLUDE_REFERENCE_PATH_SMOOTHER_REFERENCE_PATH_SMOOTHER_HPP_
