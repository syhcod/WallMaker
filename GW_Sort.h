#pragma once

#include "torPoint.h"

struct doublePoint {
    torPoint* one;
    torPoint* two;
};


struct node {
    FPoint* p1;
    FPoint* p2;
    node* front;
    node* back;
    bool isGhost;
};

class GW_Sort {
public:
    static void sort(torPoint*);
private:
    static torPoint* createWalls();
    static node* transcribe(torPoint*);
    static doublePoint findEqWalls(torPoint*);
    static bool isEq(torPoint*, torPoint*);
    static float getAngle(FPoint);
    static node* tree();
    static void recSort(node*);
    static void recFEW(torPoint*);
    static FPoint* points;
    static int pointsLen;
};
