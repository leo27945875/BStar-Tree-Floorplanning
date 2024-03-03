#pragma once

#include <vector>

class Block;
class Node;
class BStarTree;


class Block {
public:
    std::string name;
    int         number;
    int         width;
    int         height;
    int         x         = 0;
    int         y         = 0;
    bool        isRotated = false;

public:
    Block() = default;
    Block(std::string name, int number, int width, int height) : name(name), number(number), width(width), height(height) {}

    void rotate();
};


class Node {
public:
    int    number;
    Block* block;
    Node*  parent = nullptr;
    Node*  left   = nullptr;
    Node*  right  = nullptr;

public:
    Node() = default;
    Node(int number, Block* block) : number(number), block(block) {}

public:
    int  getID () const;
    void setX  (int x);
    void setY  (int y);
    void cut   (Node*& originParent, Node*& originLeft, Node*& originRight, bool& isOnParentLeft);
    bool isPure();

public:
    void updateLeftRightXs();
};

class BStarTree {
private:
    Node* root    = nullptr;

public:
    BStarTree() = default;

public:
    void  rotate  (Node* node);
    void  swap    (Node* node0, Node* node1);
    void  move    (Node* movedNode, Node* parent, bool isInsertLeft);
    void  pack    (const std::vector<Node*>& nodes, int& layoutWidth, int& layoutHeight);
    void  traverse(Node* start = nullptr);

public:
    Node* getRoot      () const;
    void  setRoot      (Node* newRoot);
    int   getRootNumber() const;

private:
    void remove(Node* node);
    void insert(Node* insertingNode, Node* insertedNode, bool isInsertLeft);
    void adopt (Node* insertingNode, Node* insertedNode, bool isInsertLeft);

public:
    void linkLeft              (Node* parent, Node* child);
    void linkRight             (Node* parent, Node* child);
    bool adoptBoundaryCondition(Node* insertingNode, Node* left, Node* right, bool isInsertLeft);
};


struct XY {
    int x;
    int y;
    XY* prev = nullptr;
    XY* next = nullptr;

    XY(int x, int y) : x(x), y(y) {};

    bool operator== (const XY& other);
};

class Contour {
public:
    XY* front;
    XY* back;

public:
    Contour();
    ~Contour();

public:
    bool isFront        (const XY* xy);
    bool isBack         (const XY* xy);
    int  getLenght      ();
    int  getMaxYInSpan  (const XY* searchXY, const int xspan[2]);
    void getMaxXY       (int& maxX, int& maxY);
    void addBlockAndSetY(Block* block);
    void printInfo      ();
    void deleteRange    (XY* searchXY0, XY* searchXY1);
};