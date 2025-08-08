#pragma once

#include "torPoint.h"

struct doublePoint {
    torPoint* one;
    torPoint* two;
};


struct node {
    FPoint* p1 = nullptr;
    FPoint* p2 = nullptr;
    node* front = nullptr;
    node* back = nullptr;
    bool isGhost;
    bool notSet;
};

class GW_Sort {
public:
    static void sort(torPoint*);
    static node* root;
    static void showTree();
private:
    static torPoint* createWalls();
    static node* transcribe(torPoint*);
    static doublePoint findEqWalls(torPoint*);
    static bool isEq(torPoint*, torPoint*);
    static float getAngle(FPoint);
    static node* tree();
    static void recSort(node*, node*);
    static void recFEW(torPoint*);
    static FPoint* points;
    static int pointsLen;
    static FPoint* getInter(FPoint, FPoint, FPoint, FPoint);
    static bool isRight(FPoint a, FPoint b, FPoint c);
    static bool isSim(FPoint a, FPoint b);
    // Recursive Show Tree
    static void RST(node*);
    static void printNode(node*);
};