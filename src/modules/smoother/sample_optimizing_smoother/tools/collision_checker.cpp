//
// Created by yangt on 19-5-8.
//
#include "sample_optimizing_smoother/tools/collosion_checker.hpp"
#include "sample_optimizing_smoother/config/planning_flags.hpp"

namespace bz_robot
{
namespace PathOptimizationNS {

CollisionChecker::CollisionChecker(std::shared_ptr<MapBase> in_gm,
                                   std::shared_ptr<Configs> p_configs)
    : map_(in_gm),
      mp_configs(p_configs),
    car_(mp_configs->FLAGS_car_width,
           mp_configs->FLAGS_car_length / 2.0 - mp_configs->FLAGS_rear_axle_to_center,
           mp_configs->FLAGS_car_length / 2.0 + mp_configs->FLAGS_rear_axle_to_center)
{
}

bool CollisionChecker::isSingleStateCollisionFree(const State &current) {
    // get the footprint circles based on current vehicle state in global frame
    std::vector<Circle> footprint =
        this->car_.getCircles(current);
    // footprint checking
    for (auto &circle_itr : footprint) {
        Eigen::Vector2d pos(circle_itr.x,
                               circle_itr.y);
        // complete collision checking
        if (map_.isInside(pos)) {
            double clearance = this->map_.getObstacleDistance(pos);
            if (clearance < circle_itr.r) {  // collision
                // less than circle radius, collision
                return false;
            }
        } else {
            // beyond boundaries , collision
            return false;
        }
    }
    // all checked, current state is collision-free
    return true;
}

bool CollisionChecker::isSingleStateCollisionFreeImproved(const State &current) {
    // get the bounding circle position in global frame
    Circle bounding_circle = this->car_.getBoundingCircle(current);

    Eigen::Vector2d pos(bounding_circle.x,
                           bounding_circle.y);
    if (map_.isInside(pos)) {
        double clearance = this->map_.getObstacleDistance(pos);
        if (clearance < bounding_circle.r) {
            // the big circle is not collision-free, then do an exact
            // collision checking
            return (this->isSingleStateCollisionFree(current));
        } else { // collision-free
            return true;
        }
    } else {  // beyond the map boundary
        return false;
    }
}

}
}
