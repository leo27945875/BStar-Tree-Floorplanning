#pragma once

#include <iostream>
#include <limits.h>
#include <ctime>
#include <cfloat>
#include "floorplanning.hpp"

#define NEIGHBOR_SEARCH_ITER 5
#define COST_WEIGHT_AREA     0.5
#define COST_WEIGHT_RATIO    0.5

#define ANNEALING_FACTOR     0.85
#define INIT_PROBABILITY     0.999
#define LOCAL_SEARCH_SCALE   100.
#define LOCAL_SEARCH_ITER    20

#define MAX_SA_TIME_SECOND   (clock_t)3580
#define MIN_TEMPERATURE      -DBL_MAX
#define MAX_REJECT_RATE      0.975
#define MAX_ITERATION        INT_MAX

#define IS_FAST_SA           0


class AnnealingSchedule {
public:
    double  P               = INIT_PROBABILITY;
    double  C               = LOCAL_SEARCH_SCALE;
    double  r               = ANNEALING_FACTOR;
    double  T;
    double  initT;

    int     nNeighborSearch = NEIGHBOR_SEARCH_ITER;
    int     nLocal          = LOCAL_SEARCH_ITER;
    int     nIter           = 0;
    int     nReject         = 0;

    clock_t startTime;
    clock_t maxRunningTime  = MAX_SA_TIME_SECOND * 1000000;
    double  minTemperature  = MIN_TEMPERATURE;
    double  maxRejectRate   = MAX_REJECT_RATE;
    int     maxIteration    = MAX_ITERATION;

public:
    AnnealingSchedule() = default;

public:
    void   init          (double avgDelta, int nBlock);
    void   step          (double cumDeltaCost, int nReject);
    double getTemperature();
    double getRejectRate ();
    double getRunningTime();
    bool   isAccept      (double deltaCost);
    bool   isFrozen      ();
};

class SimulatedAnnealing {
public:
    Floorplanning     fp;
    Floorplanning     lastFP;
    Floorplanning     bestFP;
    AnnealingSchedule schedule;


    std::string       inputFilename;
    double            avgArea, avgUphill;
    double            lastCost, bestCost;
    double            lastArea, bestArea;
    double            lastAspectRatio, bestAspectRatio;

public:
    SimulatedAnnealing(std::string inputFilename, int avgTimes = 50, bool isRecover = true);
    SimulatedAnnealing(std::string inputFilename, double avgArea, double avgUphill);

public:
    void cast();
    void save(std::string outputFilename);

public:
    void   getAverageAreaAndUphill(int times, bool isRecover, double& avgArea, double& avgUphill);
    double getCost                (double area, double aspectRatio);
    double getCost                ();

private:
    void updateLast(const Floorplanning& fp, double cost, double area, double aspectRatio);
    void updateBest(const Floorplanning& fp, double cost, double area, double aspectRatio);
};