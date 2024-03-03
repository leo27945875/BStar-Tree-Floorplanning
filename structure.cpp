#include <iostream>
#include <queue>
#include <stack>
#include <limits.h>
#include <stdexcept>
#include "include/utils.hpp"
#include "include/structure.hpp"


bool XY::operator== (const XY& other){
    return (x == other.x) && (y == other.y);
}


Contour::Contour(){
    front = new XY(0      , 0);
    back  = new XY(INT_MAX, 0);
    front->next = back;
    back ->prev = front;
}

Contour::~Contour(){
    XY *nowXY = front, *nextXY;
    while (nowXY != nullptr){
        nextXY = nowXY->next;
        delete nowXY;
        nowXY = nextXY;
    }
}

void Contour::printInfo(){
    XY *nowXY = front;
    std::cout << "[Contour] (";
    while (nowXY != nullptr){
        std::cout << "<" << nowXY->x << ", " << nowXY->y << ">, ";
        nowXY = nowXY->next;
    }
    std::cout << ")\n";
}

bool Contour::isFront(const XY* xy){
    return xy->x == 0 && xy->y == 0;
}

bool Contour::isBack (const XY* xy){
    return xy->x == INT_MAX && xy->y == 0;
}

int Contour::getLenght(){
    int length = 0;
    XY* nowXY = front;
    while (nowXY != nullptr){
        length++;
        nowXY = nowXY->next;
    }
    return length;
}

int Contour::getMaxYInSpan(const XY* searchXY, const int xspan[2]){
    int maxY = 0;
    while (searchXY != nullptr)
    {
        if (searchXY->x > xspan[0] && searchXY->y > maxY)
            maxY = searchXY->y;
        if (searchXY->x >= xspan[1])
            break;

        searchXY = searchXY->next;
    }
    return maxY;
}

void Contour::getMaxXY(int& maxX, int& maxY){
    int _maxX = 0, _maxY = 0;
    XY* nowXY = front;
    while (nowXY != nullptr && nowXY != back){
        if (nowXY->x > _maxX) _maxX = nowXY->x;
        if (nowXY->y > _maxY) _maxY = nowXY->y;
        nowXY = nowXY->next;
    }
    maxX = _maxX;
    maxY = _maxY;
}

void Contour::addBlockAndSetY(Block* block){
    int xspan    [2] = {block->x, block->x + block->width};
    XY* insertXYs[3];
    XY* searchXY0;
    XY* searchXY1;

    searchXY0 = front;
    while (searchXY0->x <= xspan[0])
        searchXY0 = searchXY0->next;

    searchXY0 = searchXY0->prev;

    int x = block->x;
    int y = getMaxYInSpan(searchXY0, xspan);
    block->y = y;

    insertXYs[0] = new XY(x               , y + block->height);
    insertXYs[1] = new XY(x + block->width, y + block->height);
    insertXYs[2] = new XY(x + block->width, y                );

    if (!isFront(searchXY0))
        searchXY0 = searchXY0->prev;
    if ((*insertXYs[0]) == (*searchXY0))
        searchXY0 = searchXY0->prev;

    searchXY1 = back;
    while (searchXY1->x >= xspan[1])
        searchXY1 = searchXY1->prev;
    
    searchXY1 = searchXY1->next;
    if ((*insertXYs[2]) == (*searchXY1))
        searchXY1 = searchXY1->next;

    deleteRange(searchXY0, searchXY1);
    
    searchXY0   ->next = insertXYs[0];
    insertXYs[0]->next = insertXYs[1];
    insertXYs[1]->next = insertXYs[2];
    insertXYs[2]->next = searchXY1;

    searchXY1   ->prev = insertXYs[2];
    insertXYs[2]->prev = insertXYs[1];
    insertXYs[1]->prev = insertXYs[0];
    insertXYs[0]->prev = searchXY0   ;
}

void Contour::deleteRange(XY* searchXY0, XY* searchXY1){
    XY* now = searchXY0->next;
    XY* next;
    while (now != searchXY1){
        next = now->next;
        delete now;
        now = next;
    }
}


void Block::rotate(){
    int temp  = width;
    width     = height;
    height    = temp;
    isRotated = !isRotated;
}


int Node::getID() const {
    return block->number;
}

void Node::setX(int x){
    block->x = x;
}

void Node::setY(int y){
    block->y = y;
}

void Node::cut(Node*& originParent, Node*& originLeft, Node*& originRight, bool& isOnParentLeft){
    originParent   = parent;
    originLeft     = left;
    originRight    = right;
    isOnParentLeft = (parent != nullptr && parent->left == this);

    if (parent)
        if (isOnParentLeft)
            parent->left  = nullptr;
        else
            parent->right = nullptr;

    if (left ) left ->parent = nullptr;
    if (right) right->parent = nullptr;

    parent = left = right = nullptr;
}

bool Node::isPure(){
    return parent == nullptr && left == nullptr && right == nullptr;
}

void Node::updateLeftRightXs(){
    if (left)  left ->setX(block->x + block->width);
    if (right) right->setX(block->x);
}


void BStarTree::pack(const std::vector<Node *> &nodes, int &layoutWidth, int &layoutHeight){
    Contour           contour;
    std::stack<Node*> stack;
    Node*             nowNode;

    root->setX(0);
    root->setY(0);

    stack.push(root); 
    while (!stack.empty()){
        nowNode = stack.top();
        nowNode->updateLeftRightXs();
        contour.addBlockAndSetY(nowNode->block);
        stack.pop();
        if (nowNode->right != nullptr) stack.push(nowNode->right);
        if (nowNode->left  != nullptr) stack.push(nowNode->left);
    }
    contour.getMaxXY(layoutWidth, layoutHeight);
}

void BStarTree::rotate(Node* node){
    node->block->rotate();
}

void BStarTree::swap(Node* node0, Node* node1){
    if (node0 == node1) return;
    Block* temp  = node0->block;
    node0->block = node1->block;
    node1->block = temp        ;
}

void BStarTree::move(Node* movedNode, Node* parent, bool isInsertLeft){
    if (movedNode == parent) return;
    remove(movedNode);
    insert(parent, movedNode, isInsertLeft);
}

void BStarTree::insert(Node* insertingNode, Node* insertedNode, bool isInsertLeft){
    if (!insertedNode->isPure()) throw std::runtime_error("[BStarTree::insert] [insertedNode] must be pure.");
    if (isInsertLeft){
        linkLeft(insertedNode, insertingNode->left);
        linkLeft(insertingNode, insertedNode);
    }
    else {
        linkRight(insertedNode, insertingNode->right);
        linkRight(insertingNode, insertedNode);
    }
}

void BStarTree::remove(Node* node){
    Node* originParent;
    Node* originLeft;
    Node* originRight;
    bool  isOnParentLeft;

    // Move the node out from the tree and make node->parent->(left or right) = node->left->parent = node->right->parent = nullptr
    node->cut(originParent, originLeft, originRight, isOnParentLeft);

    // Update the root of this B*-tree
    bool isChooseLeft = RANDOM_BOOL;
    if (node == root) {
        if (originLeft && originRight){
            if (isChooseLeft) root = originLeft;
            else              root = originRight;
        }
        else if (originRight) root = originRight;
        else if (originLeft ) root = originLeft;
    }

    // Cut node and its children recursively
    Node* nowNode = node;
    Node* newLeftNode;
    Node* newRightNode;
    Node* insertingNode = originParent;
    bool  isInsertLeft  = isOnParentLeft;
    while (!adoptBoundaryCondition(insertingNode, originLeft, originRight, isInsertLeft)){
        if (isChooseLeft){
            nowNode      = originLeft;
            newLeftNode  = nullptr;
            newRightNode = originRight;
        }
        else {
            nowNode      = originRight;
            newLeftNode  = originLeft;
            newRightNode = nullptr;
        }

        nowNode->cut(originParent, originLeft, originRight, isOnParentLeft); // [originParent] = nullptr; [isOnParentLeft] = false.
        adopt(insertingNode, nowNode, isInsertLeft);
        if (newLeftNode)  linkLeft (nowNode, newLeftNode );
        if (newRightNode) linkRight(nowNode, newRightNode);
        
        insertingNode = nowNode;
        isInsertLeft  = isChooseLeft;
        isChooseLeft  = RANDOM_BOOL;
    }
}

bool BStarTree::adoptBoundaryCondition(Node* insertingNode, Node* left, Node* right, bool isInsertLeft){
    if (left && right)
        return false;
    else if (right) adopt(insertingNode, right, isInsertLeft);
    else if (left ) adopt(insertingNode, left , isInsertLeft);
    return true;
}

void BStarTree::adopt(Node* insertingNode, Node* insertedNode, bool isInsertLeft){
    if (isInsertLeft)
        linkLeft(insertingNode, insertedNode);
    else 
        linkRight(insertingNode, insertedNode);
}

void BStarTree::linkLeft(Node* parent, Node* child){
    if (parent) parent->left  = child;
    if (child ) child->parent = parent;
}

void BStarTree::linkRight(Node* parent, Node* child){
    if (parent) parent->right = child;
    if (child ) child->parent = parent;
}

void BStarTree::traverse(Node* start){
    Node             *now, *left, *right;
    std::queue<Node*> queue;

    LOG("Traversal:");

    if (start == nullptr)
        start = root;
    if (start == nullptr)
        return;

    queue.push(start);
    while (!queue.empty()){
        now = queue.front();
        queue.pop();
        left  = now->left;
        right = now->right;

        std::cout << "[Block" << (now->getID() + 1) << " (" << now << ")] -> ";
        if (left){
            std::cout << "[L Block" << (left->getID() + 1) << " (" << left << ")] ";
            queue.push(left);
        }
        if (right){
            std::cout << "[R Block" << (right->getID() + 1) << " (" << right << ")] ";
            queue.push(right);
        }
        END_LINE;
    }
    END_LINE;
}

Node* BStarTree::getRoot() const {
    return root;
}

void BStarTree::setRoot(Node* newRoot){
    root = newRoot;
    if (root) root->parent = nullptr;
}

int BStarTree::getRootNumber() const {
    if (root) return root->number;
    return -1;
}