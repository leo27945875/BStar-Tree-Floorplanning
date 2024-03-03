#include <iostream>
#include <stdexcept>
#include <stack>
#include <cmath>
#include <ctime>
#include "include/utils.hpp"
#include "include/floorplanning.hpp"
#include "include/simulated_annealing.hpp"

int main(int argc, char *argv[]){
    if (argc != 3)
        throw std::runtime_error("[main] There must be 2 arguments (input & output filenames).");

    std::string inputFilename  = argv[1];
    std::string outputFilename = argv[2];

    END_LINE;
    LOGKV(inputFilename);
    LOGKV(outputFilename);
    END_LINE;

    SimulatedAnnealing sa(inputFilename);
    sa.cast();
    sa.save(outputFilename);

/*
    int    layoutWidth;
    int    layoutHeight;
    int    area;
    double aspectRatio;

    Floorplanning fp0(inputFilename);
    Floorplanning fp1, fp2, fp3;

    fp0.packLayout(layoutWidth, layoutHeight, area, aspectRatio);
    fp1 = fp0;
    fp2 = fp1;
    fp3 = fp2;
    fp0.clear();
    fp1.clear();
    fp3.clear();

    DASH_LINE;
    fp2.printBlockInfo();
    DASH_LINE;
    fp2.m_tree.traverse();
*/


    // int    layoutWidth;
    // int    layoutHeight;
    // int    area;
    // double aspectRatio;

    // Node* originParent;
    // Node* originLeft;
    // Node* originRight;
    // bool isOnParentLeft;

    // Floorplanning fp(inputFilename);
    
    // for (int i = 0; i < fp.m_nodes.size(); i++)
    //     fp.m_nodes[i]->cut(originParent, originLeft, originRight, isOnParentLeft);

    // fp.m_tree.setRoot(fp.m_nodes[1]);
    // fp.m_tree.linkRight(fp.m_nodes[1], fp.m_nodes[0]);
    // fp.m_tree.linkRight(fp.m_nodes[0], fp.m_nodes[2]);
    // fp.m_tree.linkRight(fp.m_nodes[2], fp.m_nodes[3]);
    // fp.m_tree.traverse();

    // fp.m_tree.rotate(fp.m_nodes[2]);

    // fp.packLayout(layoutWidth, layoutHeight, area, aspectRatio);
    // fp.printBlockInfo();
    // fp.outputToFile(outputFilename);

    return 0;
}