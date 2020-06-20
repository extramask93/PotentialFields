//
// Created by damian on 07.06.2020.
//
#include <catch2/catch.hpp>
#include <GridMap.h>
#include <AStar.h>
#include <MapLoader.h>
#include <Benchmarker.h>
#include <matplotlibcpp.h>
#include <memory>
#include <utility>
#include <DepthFirst.h>
#include <RrtPlanner.h>
#include <rrtstar.h>
#include <PotentialFieldsPlanner.h>
#include <AntColony.h>
#include <PRM.h>

static std::pair<util::Point, util::Point>
getRandomStartAndGoalLocation(std::shared_ptr<util::GridMap<unsigned char>> map, double minDistance) {
    auto planner = astar::AStar(map);
    util::Point startLocation;
    util::Point goalLocation;
    static std::mt19937 randomGenerator(10);
    using Distribution = std::uniform_real_distribution<double>;
    Distribution xDistribution(map->getOriginX(),
                               map->getOriginX() + map->getWorldWidth());
    Distribution yDistribution(map->getOriginY(),
                               map->getOriginY() + map->getWorldHeight());
    double distance = 0.0;
    do {
        startLocation = util::Point{xDistribution(randomGenerator), yDistribution(randomGenerator)};
        goalLocation = util::Point{xDistribution(randomGenerator), yDistribution(randomGenerator)};
        distance = map->worldDistanceEuclidean(startLocation, goalLocation);
    } while (!planner.isStartAndGoalValid(startLocation, goalLocation) || (distance < minDistance));
    return std::make_pair(startLocation, goalLocation);
}

static std::vector<std::pair<util::Point, util::Point>>
getRandomStartAndGoalLocations(std::shared_ptr<util::GridMap<unsigned char>> map, int nrOfPoints) {
    std::vector<std::pair<util::Point, util::Point>> result;
    auto minDistance = ceil(map->getWorldWidth() * 0.2);
    for (int i = 0; i < nrOfPoints; i++) {
        result.push_back(getRandomStartAndGoalLocation(map, minDistance));
    }
    return result;
}

static std::vector<std::string> Corridormaps = {
        "/home/damian/PathPlanningAlgorithms/resources/maps/korytarze/korytarz1.pgm",
        "/home/damian/PathPlanningAlgorithms/resources/maps/korytarze/korytarz2.pgm",
        "/home/damian/PathPlanningAlgorithms/resources/maps/korytarze/korytarz3.pgm",
};

static std::vector<std::string> Sizemaps = {
        "/home/damian/PathPlanningAlgorithms/resources/maps/wielkosci/map100x100.pgm",
        "/home/damian/PathPlanningAlgorithms/resources/maps/wielkosci/map500x500.pgm",
        "/home/damian/PathPlanningAlgorithms/resources/maps/wielkosci/map1000x1000.pgm",
        "/home/damian/PathPlanningAlgorithms/resources/maps/wielkosci/map2000x2000.pgm",
};

static std::vector<std::pair<util::Point, util::Point>> points1 = {
        {{436.477, 371.604}, {197.589, 192.953}},
        {{362.583, 223.582}, {204.179, 255.2}},
        {{200.799, 333.171}, {304.847, 435.493}},
        {{95.9787, 185.552}, {225.158, 230.27}},
        {{434.83,  282.063}, {280.333, 439.657}},
};

static std::vector<std::pair<util::Point, util::Point>> points2 = {
        {{226.885, 299.122}, {77.5639, 386.195}},
        {{ 103.066, 288.547 },{ 301.477, 443.872 }},
        {{ 149.617, 381.427 },{ 407.528, 263.157 }},
        {{ 93.9751, 321.653 },{ 305.85, 452.636 }},
        {{ 69.9906, 238.158 },{ 239.067, 311.747 }},
};

TEST_CASE("Dijkstra planner corridors", "[Dijkstra]") {
    for (const auto &mapPath : Corridormaps) {
        auto map = util::MapLoader::loadPGMMap(mapPath);
        auto planner = astar::AStar(map);
        util::Robot::movementType = util::Robot::MovementType::EUCLIDEAN;
        planner.setHeuristic(astar::AStar::HeuristicType::NO_HEURISTIC);
        planner.setName("Dijkstra");
        auto benchmarker = util::Benchmarker("corridors-dijkstra.csv");
        auto points = getRandomStartAndGoalLocations(map, 5);
        //auto path = planner.makePlan(points[0].first, points[0].second);
        //map->plotPathOnMap(path);
        benchmarker.evaluatePlanners(planner, map, points, 100);
    }
}

TEST_CASE("AStar planner corridors", "[AStar]") {
    for (const auto &mapPath : Corridormaps) {
        auto map = util::MapLoader::loadPGMMap(mapPath);
        auto planner = astar::AStar(map);
        util::Robot::movementType = util::Robot::MovementType::EUCLIDEAN;
        planner.setHeuristic(astar::AStar::HeuristicType::EUCLID);
        planner.setName("astar");
        auto benchmarker = util::Benchmarker("corridors-astar.csv");
        auto points = getRandomStartAndGoalLocations(map, 5);
        //auto path = planner.makePlan(points[0].first, points[0].second);
        //map->plotPathOnMap(path);
        benchmarker.evaluatePlanners(planner, map, points, 100);
    }
}

TEST_CASE("DepthFirst planner corridors", "[DepthFirst]") {
    for (const auto &mapPath : Corridormaps) {
        auto map = util::MapLoader::loadPGMMap(mapPath);
        auto planner = df::DepthFirst(map);
        util::Robot::movementType = util::Robot::MovementType::EUCLIDEAN;
        planner.setName("df");
        auto benchmarker = util::Benchmarker("corridors-df.csv");
        auto points = getRandomStartAndGoalLocations(map, 5);
        benchmarker.evaluatePlanners(planner, map, points, 100);
    }
}

TEST_CASE("RRT planner corridors", "[RRT]") {
    //for(const auto &mapPath : Corridormaps) {
    auto map = util::MapLoader::loadPGMMap(Corridormaps[2]);
    auto planner = rrt::RrtPlanner(map);
    planner.setName("rrt");
    auto benchmarker = util::Benchmarker("corridors-rrt.csv");
    auto points = points2;// getRandomStartAndGoalLocations(map,5);
    benchmarker.evaluatePlanners(planner, map, points, 100);
    //}
}

TEST_CASE("RRTStar planner corridors", "[RRTStar]") {
    //for (const auto &mapPath : Corridormaps) {
        auto map = util::MapLoader::loadPGMMap(Corridormaps[2]);
        auto planner = rrt::RrtStar(map);
        planner.setName("rrt-star");
        planner.setRunToMaxIterations(true);
        planner.setGamma(50);
        auto benchmarker = util::Benchmarker("corridors-rrt-star.csv");
        auto points = points2;//getRandomStartAndGoalLocations(map, 5);
        //auto path = planner.makePlan(points[0].first, points[0].second);
        //map->plotPathOnMap(path);
        benchmarker.evaluatePlanners(planner, map, points, 100);
    //}
}

TEST_CASE("RPM planner corridors", "[PRM]") {
    for (const auto &mapPath : Corridormaps) {
        auto map = util::MapLoader::loadPGMMap(mapPath);
        auto planner = prm::Prm(map, 100);
        planner.setName("prm");
        auto benchmarker = util::Benchmarker("corridors-prm-star.csv");
        auto points = getRandomStartAndGoalLocations(map, 5);
        //auto path = planner.makePlan(points[0].first, points[0].second);
        //map->plotPathOnMap(path);
        benchmarker.evaluatePlanners(planner, map, points, 100);
    }
}

TEST_CASE("Potential fields planner corridors", "[PF]") {
    //for(const auto &mapPath : Corridormaps) {
    auto map = util::MapLoader::loadPGMMap(Corridormaps[0]);
    auto planner = pf::PotentialFieldsPlanner(map);
    planner.setName("pf");
    auto benchmarker = util::Benchmarker("corridors-pf.csv");
    auto points = getRandomStartAndGoalLocations(map, 5);
    //auto path = planner.makePlan(points[0].first, points[0].second);
    //map->plotPathOnMap(path);
    benchmarker.evaluatePlanners(planner, map, points, 1);
    //}
}

TEST_CASE("Ant colony planner corridors", "[ANT]") {
    for (const auto &mapPath : Corridormaps) {
        auto map = util::MapLoader::loadPGMMap(mapPath);
        auto planner = AntColony(map);
        planner.setName("ant");
        auto benchmarker = util::Benchmarker("corridors-ant.csv");
        auto points = getRandomStartAndGoalLocations(map, 5);
        //auto path = planner.makePlan(points[0].first, points[0].second);
        //map->plotPathOnMap(path);
        benchmarker.evaluatePlanners(planner, map, points,1);
    }
}
