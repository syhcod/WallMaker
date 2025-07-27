#include "types.h"
#include "torPoint.h"
#include <vector>

#define DEBUG

class WallMaker {
public:
    WallMaker(const char* dotsfile);
    FPoint* getDots(int i);
#ifdef DEBUG
    FPoint* getIn(int i);
    FPoint* getOut(int i);
    void showOut();
    void showIn();
#endif
    unsigned int getNum() const;
    unsigned int getNs(int n) const;
    void findCrossSection();
    void findFinal();
    void showFinal();

    torPoint* getFinal();
private:
    FPoint** task;
    unsigned int num;
    unsigned int* ns;
    torPoint** IArray;
    torPoint** OArray;
    std::vector<torPoint*> intersections;
    bool* isCol;
    bool* isnCol;

    torPoint* createCrossSection(FPoint coord, torPoint* next = nullptr);

    torPoint* makeInner(FPoint* tor, int num);
    torPoint* makeOuter(FPoint* tor, int num);
    FPoint getIPoint(FPoint p1, FPoint p2, FPoint p3);
    FPoint getOPoint(FPoint p1, FPoint p2, FPoint p3);
    FPoint* isCollision(FPoint p1, FPoint p2, FPoint p3, FPoint p4);
    bool isIntersect(FPoint p);
    void checkValidInner();

    bool arePointsSimilar(FPoint p1, FPoint p2);

    torPoint* skipInter(torPoint* tp);
    torPoint* skipToInter(torPoint* tp);
    float isClockwise(torPoint* tp);

    torPoint* createLoop(torPoint* tp);

    torPoint* theOne = nullptr;
#ifdef DEBUG
    static void showTorPoints(torPoint* tp);
#endif
};