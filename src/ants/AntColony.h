#ifndef ANT_COLONY_H
#define ANT_COLONY_H
#include <unordered_map>
#include <IPlanner.h>
#include "Utils.h"
#include "GridMap.h"

struct pair_hash
{

    template<class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2> &pair) const
    {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

class Ant
{
  public:
    Ant(Node start = Node(0, 0), int id = 0);
    std::vector<Node> path_;
    bool found_goal_ = false;
    Node current_node_;
    Node previous_node_;
    int steps_ = 0, id_;
};

class AntColony : public IPlanner
{
  public:
    AntColony(std::shared_ptr<util::GridMap<unsigned char>> grid,int n_ants = 1, double alpha = 0.5, double beta = 0.5, double evap_rate = 0.5, int iterations = 1, double Q = 10.0);

    void PrintAntPath(Ant &ant);

    void RemoveLoop(Ant &ant);

    std::vector<util::Point> makePlan(const util::Point &start, const util::Point &goal) override ;
    std::vector<Node> ant_colony(util::GridMap<unsigned char> &grid, Node start, Node goal);

  private:
    std::unordered_map<std::pair<int, int>, double, pair_hash> pheromone_edges_;
    int n_ants_, iterations_, max_steps_, grid_size_;
    double alpha_, beta_, evap_rate_, Q_;
    Node start_, goal_;
    std::vector<Ant> ants_;
    std::vector<Node> motions_;
};

#endif
