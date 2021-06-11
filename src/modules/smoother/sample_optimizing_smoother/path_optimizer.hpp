//
// Created by ljn on 19-8-16.
//

#ifndef PATH_OPTIMIZER__PATHOPTIMIZER_HPP_
#define PATH_OPTIMIZER__PATHOPTIMIZER_HPP_

#include <string>
#include <vector>
#include <memory>
#include <tuple>
//#include <glog/logging.h>
//#include "grid_map_core/grid_map_core.hpp"
#include "map/map_base.h"
#include "sample_optimizing_smoother/config/planning_flags.hpp"
#include "sample_optimizing_smoother/data_struct/data_struct.hpp"

namespace bz_robot
{
namespace PathOptimizationNS {

class ReferencePath;
class State;
class Map;
class CollisionChecker;
class VehicleState;
class Configs;
class PathOptimizer {
public:
    PathOptimizer() = delete;
    PathOptimizer(const State &start_state,
                  const State &end_state,
                  std::shared_ptr<MapBase> p_map,
                  std::shared_ptr<Configs> p_configs);
    ~PathOptimizer();
    PathOptimizer(const PathOptimizer &optimizer) = delete;
    PathOptimizer &operator=(const PathOptimizer &optimizer) = delete;

    // Call this to get the optimized path.
    bool solve(const std::vector<State> &reference_points, std::vector<State> *final_path);
    bool solveWithoutSmoothing(const std::vector<State> &reference_points, std::vector<State> *final_path);

    // Only for visualization purpose.
    const std::vector<State> &getSmoothedPath() const;
    const std::vector<std::vector<double>> &getSearchResult() const;
    std::vector<std::tuple<State, double, double>> display_abnormal_bounds() const;

private:
    // Core function.
    bool optimizePath(std::vector<State> *final_path);

    // Divide smoothed path into segments.
    bool segmentSmoothedPath();

    //const Map *grid_map_;
    std::shared_ptr<PathOptimizationNS::Map> mp_map;
    std::shared_ptr<Configs> mp_configs;
    CollisionChecker *collision_checker_;
    ReferencePath *reference_path_;
    VehicleState *vehicle_state_;
    size_t size_{};

    // For visualization purpose.
    std::vector<State> smoothed_path_;
    std::vector<std::vector<double>> reference_searching_display_;
};
}
}
#endif //PATH_OPTIMIZER__PATHOPTIMIZER_HPP_
