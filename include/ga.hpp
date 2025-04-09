#pragma once
#include "./types.hpp"

class ga
{
    // Variables
private:
    int populationSize = 0;
    Creature *population;

    // Functions
public:
    ga(/* args */);

    void InitiliazePopulation(int size);
    void EvaluatePopulation();
    void SelectParents();
    void PerformCrossover();
    void PerformMutation();

    void Run(int iterations);

    ~ga();
};

ga::ga(/* args */)
{
}

ga::~ga()
{
}
