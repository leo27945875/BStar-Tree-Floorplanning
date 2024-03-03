#include <iostream>
#include <ctime>
#include "include/utils.hpp"
#include "include/floorplanning.hpp"
#include "include/simulated_annealing.hpp"


void AnnealingSchedule::init(double avgDelta, int nBlock)
{
    initT            = -avgDelta / log(P);
    T                = initT;
    nNeighborSearch *= nBlock;
    startTime        = clock();
}

bool AnnealingSchedule::isFrozen(){
    return (
        nIter >= maxIteration                ||
        clock() - startTime > maxRunningTime ||
        getRejectRate() > maxRejectRate      ||
        T < minTemperature                                
    );
}

bool AnnealingSchedule::isAccept(double deltaCost){
    return RANDOM_0TO1 < exp(-deltaCost / T);
}

double AnnealingSchedule::getRunningTime(){
    return static_cast<double>(clock() - startTime) / 1000000.;
}

double AnnealingSchedule::getTemperature(){
    return T;
}

double AnnealingSchedule::getRejectRate(){
    return static_cast<double>(nReject) / nNeighborSearch;
}

void AnnealingSchedule::step(double cumDeltaCost, int nRejectTime){
    nIter++;
    nReject = nRejectTime;

#if IS_FAST_SA == 1
    double avgDeltaCost = cumDeltaCost / static_cast<double>(nNeighborSearch);
    if (nIter <= nLocal) 
        T = (initT * avgDeltaCost) / (static_cast<double>(nIter) * C);
    else
        T = (initT * avgDeltaCost) / static_cast<double>(nIter);
#else
    T *= r;
#endif

    return;
}


SimulatedAnnealing::SimulatedAnnealing(std::string inputFilename, int avgTimes, bool isRecover)
    : inputFilename(inputFilename)
{
    LOG("Loading data ...");
    fp.inputFromFile(inputFilename);
    LOG("Calculating average area & uphill ...");
    getAverageAreaAndUphill(avgTimes, isRecover, avgArea, avgUphill);
    LOGKV(avgArea);
    LOGKV(avgUphill);
    END_LINE;
}

SimulatedAnnealing::SimulatedAnnealing(std::string inputFilename, double avgArea, double avgUphill)
    : inputFilename(inputFilename), avgArea(avgArea), avgUphill(avgUphill)
{
    LOG("Loading data ...");
    fp.inputFromFile(inputFilename);
    END_LINE;
}

void SimulatedAnnealing::cast(){
    // Initialization
    int    layoutWidth;
    int    layoutHeight;
    int    currArea;
    double currAspectRatio;
    double currCost;

    fp.packLayout(layoutWidth, layoutHeight, currArea, currAspectRatio);
    currCost = getCost(currArea, currAspectRatio);
    updateBest(fp, currCost, currArea, currAspectRatio);

    schedule.init(avgUphill, fp.m_blocks.size());

    DDASH_LINE;
    SA_START(bestCost, bestArea, bestAspectRatio, schedule.getTemperature());
    END_LINE;

    // Annealing Loop
    int    nReject;
    double deltaCost, cumDeltaCost;

    while (!schedule.isFrozen()){

        LOOP_START(schedule.nIter + 1);

        nReject      = 0;
        cumDeltaCost = 0.;

        for (int i = 0; i < schedule.nNeighborSearch; i++){

            updateLast(fp, currCost, currArea, currAspectRatio);

            fp.perturb();
            fp.packLayout(layoutWidth, layoutHeight, currArea, currAspectRatio);

            currCost  = getCost(currArea, currAspectRatio);
            deltaCost = currCost - lastCost;
            cumDeltaCost += deltaCost;

            if (deltaCost > 0. && !schedule.isAccept(deltaCost)){
                fp              = lastFP;
                currCost        = lastCost;
                currArea        = lastArea;
                currAspectRatio = lastAspectRatio;
                nReject++;
            }
            if (currCost < bestCost){
                updateBest(fp, currCost, currArea, currAspectRatio);
                UPDATE_BEST(bestCost);
            }
        }

        schedule.step(cumDeltaCost, nReject);

        SA_LOOP_END(bestCost, schedule.getTemperature(), schedule.getRejectRate(), schedule.getRunningTime());
        END_LINE;
    }
}

void SimulatedAnnealing::save(std::string outputFilename){
    bestFP.outputToFile(outputFilename);
}

void SimulatedAnnealing::getAverageAreaAndUphill(int times, bool isRecover, double& avgArea, double& avgUphill){
    int    cumArea   = 0;
    double cumUphill = 0.;
    double currCost, prevCost, diffCost;

    int    layoutWidth;
    int    layoutHeight;
    int    area;
    double aspectRatio;

    Floorplanning originFP;

    if (isRecover)
        originFP = fp;

    for (int i = 0; i < times; i++){
        fp.perturb();
        fp.packLayout(layoutWidth, layoutHeight, area, aspectRatio);
        cumArea += area;
    }
    avgArea = static_cast<double>(cumArea) / times;

    int nCostPositive = 0;
    prevCost = getCost(area, aspectRatio);
    for (int i = 0; i < times; i++){
        fp.perturb();
        fp.packLayout(layoutWidth, layoutHeight, area, aspectRatio);
        currCost = getCost(area, aspectRatio);
        diffCost = currCost - prevCost;
        if (diffCost > 0){
            cumUphill += (currCost - prevCost);
            nCostPositive++;
        }
        prevCost = currCost;
    }
    avgUphill = cumUphill / nCostPositive;

    if (isRecover)
        fp = originFP;
}

double SimulatedAnnealing::getCost(){
    int    layoutWidth;
    int    layoutHeight;
    int    area;
    double aspectRatio;

    fp.packLayout(layoutWidth, layoutHeight, area, aspectRatio);

    return getCost(static_cast<double>(area), aspectRatio);
}

double SimulatedAnnealing::getCost(double area, double aspectRatio){
    double ratioDiff;

    if (aspectRatio > fp.m_maxRatio)
        ratioDiff = aspectRatio - fp.m_maxRatio;
    else if (aspectRatio < fp.m_minRatio)
        ratioDiff = aspectRatio - fp.m_minRatio;
    else
        ratioDiff = 0.;
    
    ratioDiff = ratioDiff * ratioDiff;
    return COST_WEIGHT_AREA * (area / avgArea) + COST_WEIGHT_RATIO * ratioDiff;
}

void SimulatedAnnealing::updateLast(const Floorplanning& fp, double cost, double area, double aspectRatio){
    lastFP          = fp;
    lastCost        = cost;
    lastArea        = area;
    lastAspectRatio = aspectRatio;
}

void SimulatedAnnealing::updateBest(const Floorplanning& fp, double cost, double area, double aspectRatio){
    bestFP          = fp;
    bestCost        = cost;
    bestArea        = area;
    bestAspectRatio = aspectRatio;
}