#include "WallMaker.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

const float THICKNESS = 1.0f;

WallMaker::WallMaker(const char* dotsfile) {
    std::ifstream dotF(dotsfile);
    std::string line; 
    std::getline(dotF, line);
    num = std::stoi(line);
    task = (FPoint**)malloc(sizeof(FPoint*) * num);
    ns = (unsigned int*)malloc(sizeof(int) * num);
    IArray = (torPoint**)malloc(sizeof(torPoint*) * num);
    OArray = (torPoint**)malloc(sizeof(torPoint*) * num);
    int nu = 0;
    while(std::getline(dotF, line)) {
        std::istringstream in(line);      //make a stream for the line itself

        unsigned int len;
        in >> len;
        ns[nu] = len;
        task[nu] = (FPoint*)malloc(sizeof(FPoint) * len);
        for (unsigned int i = 0; i < len; i++) {
            float a, b;
            in >> a;
            in >> b;
            task[nu][i] = FPoint(a, b);
        }
        if (++nu >= num) break;
    } 
    for (int i = 0; i < num; i++) {
        // printf("예아(%d))\n", i);
        IArray[i] = makeInner(task[i], ns[i]);
        OArray[i] = makeOuter(task[i], ns[i]);
        // printf("예아(%d)\n", i);
    }
}

FPoint* WallMaker::getDots(int num) {
    return task[num];
}

unsigned int WallMaker::getNum() const {
    return num;
}

unsigned int WallMaker::getNs(int n) const {
    if (n < num) return ns[n];
    else return 0;
}

WallMaker::torPoint* WallMaker::makeInner(FPoint* tor, int num) {
    torPoint* buffer = nullptr;
    torPoint* first = nullptr;
    // printf("(%d)\n", num);
    for (int i = 0; i < num - 2; i++) {
        FPoint p1 = tor[i];
        FPoint p2 = tor[i + 1];
        FPoint p3 = tor[i + 2];
        torPoint* tp = new torPoint();
        if (i==0) first = tp;
        else tp->next = buffer;
        buffer = tp;
        tp->p = getIPoint(p1, p2, p3);
    }
    torPoint* tp = new torPoint();
    tp->next = buffer;
    buffer = tp;
    tp->p = getIPoint(tor[num - 2], tor[num - 1], tor[0]);
    tp = nullptr;
    tp = new torPoint();
    tp->next = buffer;
    tp->p = getIPoint(tor[num - 1], tor[0], tor[1]);
    first->next = tp;
    return first;
}

WallMaker::torPoint* WallMaker::makeOuter(FPoint* tor, int num) {
    torPoint* buffer = nullptr;
    torPoint* first = nullptr;
    for (int i = 0; i < num - 2; i++) {
        FPoint p1 = tor[i];
        FPoint p2 = tor[i + 1];
        FPoint p3 = tor[i + 2];
        torPoint* tp = new torPoint();
        if (i==0) first = tp;
        else tp->next = buffer;
        buffer = tp;
        tp->p = getOPoint(p1, p2, p3);
    }
    torPoint* tp = new torPoint();
    tp->next = buffer;
    buffer = tp;
    tp->p = getOPoint(tor[num - 2], tor[num - 1], tor[0]);
    tp = nullptr;
    tp = new torPoint();
    tp->next = buffer;
    tp->p = getOPoint(tor[num - 1], tor[0], tor[1]);
    first->next = tp;
    return first;
}

FPoint WallMaker::getIPoint(FPoint p1, FPoint p2, FPoint p3) {
    float xi = p1.x;
    float xm = p2.x;
    float xf = p3.x;
    float yi = p1.y;
    float ym = p2.y;
    float yf = p3.y;
    // printf("(%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f)\n", xi, yi, xm, ym, xf, yf);
    float sin = ((xm - xi) * (yf - ym) - (ym - yi) * (xf - xm)) /
        sqrt((pow(xm - xi, 2.0f) + pow(ym - yi, 2.0f)) * 
        (pow(xm - xf, 2.0f) + pow(ym - yf, 2.0f)));
    float d1 = sqrt(pow(xf -xm, 2) + pow(yf - ym, 2));
    float d2 = sqrt(pow(xi -xm, 2) + pow(yi - ym, 2));
    FPoint n;
    n = (p3 - p2) / d1 + (p1 - p2) / d2;
    
    // printf("(%.2f, %.2f)\n", n.x, n.y);
    // printf("%f\n", sin);
    FPoint pat;
    pat = p2 + n * THICKNESS / sin;
    // printf("(%.2f, %.2f)\n", pat.x, pat.y);
    return pat;
}
FPoint WallMaker::getOPoint(FPoint p1, FPoint p2, FPoint p3) {
    float xi = p1.x;
    float xm = p2.x;
    float xf = p3.x;
    float yi = p1.y;
    float ym = p2.y;
    float yf = p3.y;
    // printf("(%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f)\n", xi, yi, xm, ym, xf, yf);
    float sin = ((xm - xi) * (yf - ym) - (ym - yi) * (xf - xm)) /
        sqrt((pow(xm - xi, 2.0f) + pow(ym - yi, 2.0f)) * 
        (pow(xm - xf, 2.0f) + pow(ym - yf, 2.0f)));
    float d1 = sqrt(pow(xf -xm, 2) + pow(yf - ym, 2));
    float d2 = sqrt(pow(xi -xm, 2) + pow(yi - ym, 2));
    FPoint n;
    n = (p3 - p2) / d1 + (p1 - p2) / d2;
    // printf("%f\n", sin);
    FPoint pat;
    pat = p2 - n * THICKNESS / sin;

    // printf("(%.2f, %.2f)\n", pat.x, pat.y);
    return pat;
}
#ifdef DEBUG
FPoint* WallMaker::getIn(int i) {
    int num = ns[i];
    // printf("예아\n");
    FPoint* list = (FPoint*)malloc(sizeof(FPoint) * num);
    // printf("예아1\n");
    torPoint* tp = IArray[i];
    // printf("예아2\n");
    for (int i = 0; i < num; i++) {
        list[i] = FPoint(tp->p);
        // printf("예아(%d)\n", i);
        tp = tp->next;
    }
    return list;
}
FPoint* WallMaker::getOut(int i) {
    int num = ns[i];
    FPoint* list = (FPoint*)malloc(sizeof(FPoint) * num);

    torPoint* tp = OArray[i];
    for (int i = 0; i < num; i++) {
        list[i] = FPoint(tp->p);
        tp = tp->next;
    }
    return list;
}

#endif

void WallMaker::findCrossSection() {
    unsigned int CSNum = 0;
    for (int i = 0; i < num; i++) {
        torPoint* Rot = OArray[i];
        do {
            bool didIntersect = false;
            for (int k = i + 1; k < num; k++) {
                torPoint* iRot = OArray[k];
                do {
                    FPoint p1 = Rot->p;
                    FPoint p2 = Rot->next->p;
                    FPoint p3 = iRot->p;
                    FPoint p4 = iRot->next->p;
                    FPoint* intersection = isCollision(p1, p2, p3, p4);
                    if (intersection != nullptr) {
                        // printf("Collision!\n");
                        FPoint inter = *intersection;
                        torPoint* tp1 = createCrossSection(inter, Rot->next);
                        torPoint* tp2 = createCrossSection(inter, iRot->next);
                        Rot->next = tp1;
                        iRot->next = tp2;
                        tp1->intersect = tp2;
                        tp2->intersect = tp1;
                        for(auto ittt : intersections) {
                            if (ittt->p == inter) {
                                printf("ERROR: Intersect point coincides!\n");
                                printf("(%.2lf, %.2lf)\n", inter.x, inter.y);
                                exit(1);
                            }
                        }
                        didIntersect = true;
                        intersections.push_back(tp1);
                        iRot = iRot->next;
                    }
                    iRot = iRot->next;
                } while (iRot != OArray[k]);
            }
            if (didIntersect) Rot = Rot->next;
            Rot = Rot->next;
        } while (Rot != OArray[i]);
    }
}

WallMaker::torPoint* WallMaker::createCrossSection(FPoint coord, WallMaker::torPoint* next) {
    torPoint* tp = new torPoint();
    tp->p = coord;
    tp->next = next;
    return tp;
}

FPoint* WallMaker::isCollision(FPoint p1, FPoint p2, FPoint p3, FPoint p4) {
    FPoint* fp = nullptr;
    float xi=p1.x;
    float yi=p1.y;
    float xf=p2.x;
    float yf=p2.y;
    float a=p3.x;
    float b=p3.y;
    float c=p4.x;
    float d=p4.y;
    float det=((d-b)*(xf-xi)-(c-a)*(yf-yi));
    if (det == 0.0f) {
        return nullptr;
    }
    float t=((yi-b)*(xf-xi)-(xi-a)*(yf-yi))/det;
    float s=((yi-b)*(c-a)-(xi-a)*(d-b))/det;
    /*
    if (xi > 30 && xi < 35) {
        printf("LAKAKLDS\n");
        printf("L((%.2f, %.2f),(%.2f, %.2f))\n", p1.x, p1.y, p2.x - p1.x, p2.y - p1.y);
        printf("L((%.2f, %.2f),(%.2f, %.2f))\n", p3.x, p3.y, p4.x - p3.x, p4.y - p3.y);
    }

    if (xf > 30 && xf < 35) {
        printf("ijfsd\n");
        printf("L((%.2f, %.2f),(%.2f, %.2f))\n", p1.x, p1.y, p2.x - p1.x, p2.y - p1.y);
        printf("L((%.2f, %.2f),(%.2f, %.2f))\n", p3.x, p3.y, p4.x - p3.x, p4.y - p3.y);
    }
    */
    if (0<=s && s<=1) {
        // printf("\tGOne\n");
        float x = xi + (xf - xi) * s;
        float y = yi + (yf - yi) * s;

        if (0<=t && t<=1) {
            // printf("\tWay Gone\n");
            fp = new FPoint(x,y);
            return fp;
        }
    }
    return fp;
}

void WallMaker::showTorPoints(torPoint* tp) {
    torPoint* iter = tp;
    do {
        FPoint p1 = iter->p;
        iter = iter->next;
        FPoint p2 = iter->p;
        printf("L((%.2f, %.2f),(%.2f, %.2f))\n", p1.x, p1.y, p2.x - p1.x, p2.y - p1.y);
    } while (iter != tp);
}

void WallMaker::showIn() {
    for (unsigned int i = 0; i < num; i++) {
        showTorPoints(IArray[i]);
    }
}

void WallMaker::showOut() {
    for (unsigned int i = 0; i < num; i++) {
        showTorPoints(OArray[i]);
    }
}

WallMaker::torPoint* WallMaker::skipInter(WallMaker::torPoint* tp) {
    torPoint* tap = tp;
    FPoint inter;
    bool flag = true;
    while (flag) {
        flag = false;
        tap = tap->next;
        inter = tap->p;
        if (tap == tp) {
            printf("ERROR: Every point is intersection!\n");
            exit(1);
        }
        for(auto ittt : intersections) {
            if (ittt->p == inter) { // Means still Intersect point
                flag = true;
                break;
            }
        }
    }
    return tap;
}

void WallMaker::findFinal() {
    auto cpy = intersections;
    for(auto& it : intersections) {
        if (it == nullptr) continue;
        torPoint* tp = it->next;
        while (tp != it) {
            bool falg = true;
            for (auto itt : cpy) {
                if (itt->p == tp->next->p) {
                    falg = true;
                    break;
                }
            }
            if (falg) {
                WallMaker::torPoint* tap = new WallMaker::torPoint;
                tap->p = tp->next->p;
                tap->next = tp->next->next;
                tp->next = tap;
                auto iter = std::find(intersections.begin(), intersections.end(), it);
                while (++iter != intersections.end()) {
                    if ((*iter)->p == tap->p) *iter = nullptr;
                }
                tp = tap;
            } else {

            }

            tp = tp->next;
        }
        it = tp;
    }
}

void WallMaker::showFinal() {
    for (auto& it : intersections) {
        if (it == nullptr) continue;
        showTorPoints(it);
    }
}