#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include "include/utils.hpp"
#include "include/structure.hpp"
#include "include/floorplanning.hpp"


Floorplanning::Floorplanning(std::string inputFilename){
    inputFromFile(inputFilename);
}

Floorplanning::Floorplanning(const Floorplanning& other){
    copy(other);
}

Floorplanning::~Floorplanning(){
    for (Block* b : m_blocks) delete b;
    for (Node*  n : m_nodes ) delete n;
}

Floorplanning& Floorplanning::operator= (const Floorplanning& other){
    return copy(other);
}

Floorplanning& Floorplanning::copy(const Floorplanning& other){

    this->clear();

    m_blocks.resize(other.m_blocks.size());
    for (const Block* block : other.m_blocks){
        Block* copyBlock = new Block();
        *copyBlock = *block;
        m_blocks[block->number] = copyBlock;
    }

    m_nodes.resize(other.m_nodes.size());
    for (const Node* node : other.m_nodes){
        Node* copyNode = new Node(node->number, m_blocks[node->getID()]);
        m_nodes[node->number] = copyNode;
    }
    for (const Node* node : other.m_nodes){
        Node* copyNode   = m_nodes[node->number];
        copyNode->parent = (node->parent)? m_nodes[node->parent->number]: nullptr;
        copyNode->left   = (node->left  )? m_nodes[node->left  ->number]: nullptr;
        copyNode->right  = (node->right )? m_nodes[node->right ->number]: nullptr;
    }

    int rootNum = other.m_tree.getRootNumber();
    m_tree.setRoot(m_nodes[rootNum]);

    m_maxRatio = other.m_maxRatio;
    m_minRatio = other.m_minRatio;

    return *this;
}


void Floorplanning::inputFromFile(std::string inputFilename){
    std::string   line, blockName;
    std::ifstream inputFile(inputFilename);
    int           width, height;

    std::stringstream ss;
    getline(inputFile, line, '\n');
    ss << line;
    ss >> m_minRatio >> m_maxRatio;

    int i = 0;
    while (getline(inputFile, line, '\n')){
        std::stringstream ss;
        ss << line;
        ss >> blockName >> width >> height;
        Block* b = new Block(blockName, i, width, height);
        Node*  n = new Node (i, b);
        m_blocks.push_back(b);
        m_nodes .push_back(n);
        i++;
    }

    m_tree.setRoot(m_nodes[0]);

    int nNode = m_nodes.size();
    for (int i = 0; i < nNode; i++){
        int leftIdx = 2 * i + 1, rightIdx = 2 * i + 2;
        if (leftIdx  < nNode){
            m_nodes[leftIdx ]->parent = m_nodes[i];
            m_nodes[i       ]->left   = m_nodes[leftIdx];

        }
        if (rightIdx < nNode){
            m_nodes[rightIdx]->parent = m_nodes[i];
            m_nodes[i       ]->right  = m_nodes[rightIdx];
        }
    }
}

void Floorplanning::outputToFile(std::string outputFilename){
    int    layoutWidth;
    int    layoutHeight;
    int    area;
    double aspectRatio;

    packLayout(layoutWidth, layoutHeight, area, aspectRatio);

    std::ofstream ss(outputFilename);
    ss << "A = " << area        << "\n";
    ss << "R = " << aspectRatio << "\n";
    for (const Block* block : m_blocks)
        ss << block->name << " " << block->x << " " << block->y << (block->isRotated? " R": "") <<"\n";

#if DEBUG == 1
    ouputDebugFile(area, aspectRatio);
#endif

}

void Floorplanning::ouputDebugFile(int area, double aspectRatio, std::string debugFileName){
    std::ofstream ss(debugFileName);
    ss << "A = " << area        << "\n";
    ss << "R = " << aspectRatio << "\n";
    for (const Block* block : m_blocks) 
        ss << block->name << " " << block->x << " " << block->y << " " << block->width << " " << block->height << "\n";
}

void Floorplanning::printBlockInfo(){
    LOG("Blocks:");
    for (const Block* b : m_blocks){
        LOGBLOCK((*b));
    }
    END_LINE;
}

void Floorplanning::packLayout(int& layoutWidth, int& layoutHeight, int& area, double& aspectRatio){
    m_tree.pack(m_nodes, layoutWidth, layoutHeight);
    area        = layoutWidth * layoutHeight;
    aspectRatio = static_cast<double>(layoutWidth) / layoutHeight;
}

void Floorplanning::clear(){
    for (Block* b : m_blocks) delete b;
    for (Node*  n : m_nodes ) delete n;
    m_blocks.clear();
    m_nodes .clear();
    m_minRatio = 0.;
    m_maxRatio = 0.;
    m_tree.setRoot(nullptr);
}

void Floorplanning::perturb(){

    int nBlock = m_blocks.size();

    // Op1: Rotation
    m_tree.rotate(m_nodes[RANDOM_INT(nBlock)]);
    // Op2: Moving a block
    m_tree.move(m_nodes[RANDOM_INT(nBlock)], m_nodes[RANDOM_INT(nBlock)], RANDOM_BOOL);
    // Op3: Swapping two blocks
    m_tree.swap(m_nodes[RANDOM_INT(nBlock)], m_nodes[RANDOM_INT(nBlock)]);
}