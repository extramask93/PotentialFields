//
// Created by damian on 01.05.2020.
//

#ifndef PLANNING_RRTPLANNER_H
#define PLANNING_RRTPLANNER_H

#include <Vertex.h>
#include "GridMap.h"
#include "Point.h"
#include <IPlanner.h>
namespace rrt {
class RrtPlanner : IPlanner
{
  public:
    explicit RrtPlanner(const util::GridMap<unsigned char> &obstacleMap);
    std::vector<util::Point> makePlan(const util::Point &start, const util::Point &goal) override;
    void initialize(const util::GridMap<unsigned char> &map, const util::Options &options) override;
    void setMaxExtendDistance(double distance);
    void setMaxNrOfIterations(unsigned iterationNr);

  protected:
    util::Vertex getRandomVertex() const;
    double getRandomExtendDistance() const;
    bool isObstacle(const util::Point &point) const;
    bool reachedGoal(const util::Vertex &vertex) const;
    bool isOnCollisionPath(const util::Vertex &vertex) const;
    util::Vertex steer(const util::Vertex &from, const util::Vertex &to) const;
    util::Vertex findClosestVertex(const util::Vertex &from) const;
    int makeGraph(const util::Point &start, const util::Point &goal);
    std::vector<util::Point> buildPlan(int goalIndex) const;

  private:
    util::GridMap<unsigned char> obstacleMap_;
    std::vector<util::Vertex> vertexList_;
    util::Vertex goalVertex_;
    util::Vertex startVertex_;
    double goalRadius_;
    double goalSamplingRatio_;
    unsigned maxNrOfIterations_;
    double maxExtendDistance_;
    static constexpr unsigned OBSTACLE = 0;
};
}// namespace rrt


#endif//PLANNING_RRTPLANNER_H
