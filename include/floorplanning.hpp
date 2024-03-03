#pragma once

#include <iostream>
#include "structure.hpp"

 
class Floorplanning{
public:
    std::vector<Block*> m_blocks;
    std::vector<Node*>  m_nodes;
    BStarTree           m_tree;

    double              m_minRatio, m_maxRatio;

public:
    Floorplanning () = default;
    Floorplanning (std::string inputFilename);
    Floorplanning (const Floorplanning& other);
    ~Floorplanning();

public:
    Floorplanning& operator= (const Floorplanning& other);

public:
    void clear         ();
    void inputFromFile (std::string inputFilename);
    void outputToFile  (std::string outputFilename);
    void ouputDebugFile(int area, double aspectRatio, std::string debugFileName = "./data/DEBUG.txt");
    void printBlockInfo();

public:
    void packLayout(int& layoutWidth, int& layoutHeight, int& area, double& aspectRatio);
    void perturb   ();

private:
    Floorplanning& copy(const Floorplanning& other);
};