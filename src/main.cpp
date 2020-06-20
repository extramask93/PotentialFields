//
// Created by damian on 19.04.2020.
//
#include <PotentialFieldsPlanner.h>
#include <PRM.h>
#include <DepthFirst.h>
#include <AntColony.h>
#include "RrtPlanner.h"
#include "rrtstar.h"
#include "AStar.h"
#include "MapLoader.h"
//#include <matplotlibcpp.h>
int main(int, char **)
{

    double resolution = 1.0;
    auto map = util::MapLoader::loadPGMMap("/home/damian/PathPlanningAlgorithms/resources/maps/wielkosci/map100x100.pgm");
    std::vector<util::Point> testPointsMap{
        util::Point{ 11.5 * resolution, 11.5 * resolution },
        util::Point{ 22.5 * resolution, 11.5 * resolution },
        util::Point{ 33.5 * resolution, 11.5 * resolution },
        util::Point{ 11.5 * resolution, 30.5 * resolution },
        util::Point{ 22.5 * resolution, 30.5 * resolution },
        util::Point{ 33.5 * resolution, 30.5 * resolution },
        util::Point{ 36.5 * resolution, 36.5 * resolution },
    };
    //auto new_ant_colony = astar::AStar(map);
    //auto new_ant_colony = rrt::RrtStar(map);
    //auto new_ant_colony = df::DepthFirst(map);
    //auto new_ant_colony = prm::Prm(map,100);
    //auto new_ant_colony = AntColony(map);
    auto new_ant_colony = pf::PotentialFieldsPlanner(map);
    util::Robot::movementType = util::Robot::MovementType::EUCLIDEAN;
    //new_ant_colony.setHeuristic(astar::AStar::HeuristicType::NO_HEURISTIC);
    auto plan = new_ant_colony.makePlan({ 2.5, 3.5 }, util::Point{ (map->getCellWidth() - 1) * 1.0, (map->getCellHeight() - 1) * 1.0 });
    map->plotPathOnMap(plan);
    return 0;
}