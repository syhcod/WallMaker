#include "types.h"
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
private:
    struct torPoint {
        torPoint* next = nullptr;
        torPoint* intersect = nullptr;
        FPoint p;
    };
    FPoint** task;
    unsigned int num;
    unsigned int* ns;
    torPoint** IArray;
    torPoint** OArray;
    std::vector<torPoint*> intersections;

    torPoint* createCrossSection(FPoint coord, torPoint* next = nullptr);

    torPoint* makeInner(FPoint* tor, int num);
    torPoint* makeOuter(FPoint* tor, int num);
    FPoint getIPoint(FPoint p1, FPoint p2, FPoint p3);
    FPoint getOPoint(FPoint p1, FPoint p2, FPoint p3);
    FPoint* isCollision(FPoint p1, FPoint p2, FPoint p3, FPoint p4);

    torPoint* skipInter(torPoint* tp);
#ifdef DEBUG
    static void showTorPoints(torPoint* tp);
#endif
};