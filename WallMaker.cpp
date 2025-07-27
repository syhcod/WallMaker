#include "WallMaker.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

const float THICKNESS = 0.5f;

WallMaker::WallMaker(const char* dotsfile) {
    std::ifstream dotF(dotsfile);
    std::string line; 
    std::getline(dotF, line);
    num = std::stoi(line);
    task = (FPoint**)malloc(sizeof(FPoint*) * num);
    ns = (unsigned int*)malloc(sizeof(int) * num);
    IArray = (torPoint**)malloc(sizeof(torPoint*) * num);
    OArray = (torPoint**)malloc(sizeof(torPoint*) * num);
    isCol = (bool*)malloc(sizeof(bool) * num);
    isnCol = (bool*)malloc(sizeof(bool) * num);
    for (int i = 0; i < num; i++) {
        isCol[i] = false;
        isnCol[i] = false;
    }
    int nu = 0;
    
    while(std::getline(dotF, line)) {
        std::istringstream in(line);      //make a stream for the line itself

        unsigned int len;
        in >> len;
        ns[nu] = len;
        task[nu] = (FPoint*)malloc(sizeof(FPoint) * len);
        FPoint buff;
        for (unsigned int i = 0; i < len; i++) {
            float a, b;
            in >> a;
            in >> b;
            task[nu][i] = FPoint(a, b);
            
            // if (i != 0)
            //     printf("L((%.2f, %.2f),(%.2f, %.2f))\n", buff.x, buff.y, task[nu][i].x - buff.x, task[nu][i].y - buff.y);
            // buff = FPoint(a,b);
        }
        // printf("L((%.2f, %.2f),(%.2f, %.2f))\n", task[nu][0].x, task[nu][0].y, buff.x - task[nu][0].x, buff.y - task[nu][0].y);
        if (++nu >= num) break;
    } 
    for (int i = 0; i < num; i++) {
        IArray[i] = makeInner(task[i], ns[i]);
        OArray[i] = makeOuter(task[i], ns[i]);
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

torPoint* WallMaker::makeInner(FPoint* tor, int num) {
    torPoint* buffer = nullptr;
    torPoint* first = nullptr;
    // printf("(%d)\n", num);
    for (int i = num - 1; i >= 2; i--) {
        FPoint p3 = tor[i];
        FPoint p2 = tor[i - 1];
        FPoint p1 = tor[i - 2];
        torPoint* tp = new torPoint();
        if (i==num - 1) first = tp;
        else tp->next = buffer;
        buffer = tp;
        tp->p = getIPoint(p1, p2, p3);
    }
    torPoint* tp = new torPoint();
    tp->next = buffer;
    buffer = tp;
    tp->p = getIPoint(tor[num - 1], tor[0], tor[1]);
    tp = nullptr;
    tp = new torPoint();
    tp->next = buffer;
    tp->p = getIPoint(tor[num-2], tor[num - 1], tor[0]);
    first->next = tp;
    return first;
}

torPoint* WallMaker::makeOuter(FPoint* tor, int num) {
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
/*
void WallMaker::findCrossSection() {
    unsigned int CSNum = 0;
    for (int i = 0; i < num; i++) {
        torPoint* Rot = OArray[i];
        bool didColl = false;
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
                        didColl = true;
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

                        isCol[i] = didColl;
                        isCol[k] = didColl;
                    }
                    iRot = iRot->next;
                } while (iRot != OArray[k]);
            }
            if (didIntersect) Rot = Rot->next;
            Rot = Rot->next;
        } while (Rot != OArray[i]);
        do {
            bool didIntersect = true;
            for (int k = 0; k < num; k++) {
                torPoint* iRot = IArray[k];
                do {
                    FPoint p1 = Rot->p;
                    FPoint p2 = Rot->next->p;
                    FPoint p3 = iRot->p;
                    FPoint p4 = iRot->next->p;
                    FPoint* intersection = isCollision(p1, p2, p3, p4);
                    if (intersection != nullptr) {
                        didColl = true;
                        // printf("Collision!\n");
                        FPoint inter = *intersection;
                        torPoint* tp1 = createCrossSection(inter, Rot->next);
                        torPoint* tp2 = createCrossSection(inter, iRot->next);
                        Rot->next = tp1;
                        iRot->next = tp2;
                        tp1->intersect = tp2;
                        tp2->intersect = tp1;
                        
                        didIntersect = true;
                        intersections.push_back(tp1);
                        iRot = iRot->next;

                        isCol[i] = didColl;
                        isCol[k] = didColl;
                    }
                    iRot = iRot->next;
                } while (iRot != OArray[k]);
            }
            if (didIntersect) Rot = Rot->next;
            Rot = Rot->next;
        } while (Rot != OArray[i]);
    }
}
*/

bool WallMaker::arePointsSimilar(FPoint p1, FPoint p2) {
    float x = p1.x - p2.x;
    float y = p1.y - p2.y;
    // printf("%f\n", sqrt(x * x + y * y));
    return sqrt(x * x + y * y) < 0.001f;
}

void WallMaker::checkValidInner() {
    for (int i = 0; i < num; i++) {
        bool flag = false;
        torPoint* inner = IArray[i];
        if (isClockwise(inner) < 0) {
            IArray[i] = nullptr;
            continue;
        }
        torPoint* i1 = inner;
        torPoint* i2 = inner->next->next;
        do {
            do {
                FPoint* inter = isCollision(i1->p, i1->next->p, i2->p, i2->next->p);
                if (inter != nullptr) {
                    FPoint fp = *inter;
                    bool valid = arePointsSimilar(i1->p, fp) || arePointsSimilar(i1->next->p, fp);
                    valid = valid || arePointsSimilar(i2->p, fp) || arePointsSimilar(i2->next->p, fp);
                    
                    if (valid) {
                        i2 = i2->next;
                        continue;
                    }
                    IArray[i] = nullptr;
                    flag = true;
                    break;
                }
                i2 = i2->next;
            } while (i2->next != i1);
            i1 = i1->next;
            i2 = i1->next;
        } while (i1 != inner && !flag);
    }
}

void WallMaker::findCrossSection() {
    checkValidInner();
    for (int i = 0; i < num; i++) {
        torPoint* outer1 = OArray[i];
        for (int j = i + 1; j < num; j++) {
            torPoint* outer2 = OArray[j];
            do {
                do {
                    FPoint* inter = isCollision(outer1->p, outer1->next->p, outer2->p, outer2->next->p);
                    if (inter != nullptr) {
                        if (isIntersect(*inter)) {
                            FPoint inta = *inter;
                            bool valid = arePointsSimilar(outer1->p, inta) || arePointsSimilar(outer1->next->p, inta);
                            valid = valid || arePointsSimilar(outer2->p, inta) || arePointsSimilar(outer2->next->p, inta);
                            if (valid) {
                                outer2 = outer2->next;
                                continue;
                            } else {
                                printf("ERROR: Multiple of the same intersection\n");
                                printf("Please check (%.2f, %.2f)", inta.x, inta.y);
                                exit(1);
                            }
                        }
                        isCol[i] = true;
                        isCol[j] = true;
                        torPoint* neext1 = createCrossSection(*inter, outer1->next);
                        torPoint* neext2 = createCrossSection(*inter, outer2->next);
                        outer1->next = neext1;
                        outer2->next = neext2;
                        neext1->intersect = neext2;
                        neext2->intersect = neext1;
                        // outer1 = neext1;
                        outer2 = neext2;
                        isCol[i] = true;
                        isCol[j] = true;
                        intersections.push_back(neext1);
                    }
                    outer2 = outer2->next;
                } while (outer2 != OArray[j]);
                outer1 = outer1->next;
            } while (outer1 != OArray[i]);
        }
        for (int j = 0; j < num; j++) {
            if (j == i || IArray[j] == nullptr) continue;
            torPoint* inner = IArray[j];
            do {
                do {
                    FPoint* inter = isCollision(outer1->p, outer1->next->p, inner->p, inner->next->p);
                    if (inter != nullptr) {
                        if (isIntersect(*inter)) {
                            FPoint inta = *inter;
                            bool valid = arePointsSimilar(outer1->p, inta) || arePointsSimilar(outer1->next->p, inta);
                            valid = valid || arePointsSimilar(inner->p, inta) || arePointsSimilar(inner->next->p, inta);
                            if (valid) {
                                inner = inner->next;
                                continue;
                            } else {
                                printf("ERROR: Multiple of the same intersection\n");
                                printf("Please check (%.2f, %.2f)", inta.x, inta.y);
                                exit(1);
                            }
                        }
                        isnCol[j] = true;
                        torPoint* neext1 = createCrossSection(*inter, outer1->next);
                        torPoint* neext2 = createCrossSection(*inter, inner->next);
                        outer1->next = neext1;
                        inner->next = neext2;
                        neext1->intersect = neext2;
                        neext2->intersect = neext1;
                        // outer1 = neext1;

                        inner = neext2;
                        isCol[i] = true;
                        isCol[j] = true;

                        intersections.push_back(neext1);
                    }
                    inner = inner->next;
                } while (inner != IArray[j]);
                outer1 = outer1->next;
            } while (outer1 != OArray[i]);
        }
    }
}

bool WallMaker::isIntersect(FPoint p) {
    for(auto ittt : intersections) {
        if (ittt->p == p) {
            return true;
        }
    }
    return false;
}

torPoint* WallMaker::createCrossSection(FPoint coord, torPoint* next) {
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
    if (tp == nullptr) return;
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

torPoint* WallMaker::skipInter(torPoint* tp) {
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

torPoint* WallMaker::skipToInter(torPoint* tp) {
    torPoint* tap = tp;
    FPoint inter;
    for(auto ittt : intersections) {
        if (ittt->p == tp->p) { // Means still Intersect point
            return tp;
        }
    }

    bool flag = true;
    while (flag) {
        tap = tap->next;
        inter = tap->p;
        if (tap == tp) {
            printf("ERROR: Every point is intersection!\n");
            exit(1);
        }
        for(auto ittt : intersections) {
            if (ittt->p == inter) { // Means still Intersect point
                flag = false;
                break;
            }
        }
    }
    return tap;
}

void WallMaker::findFinal() {
    torPoint* cur = nullptr;
    for (int i = 0; i < num; i++) {
        if (!isCol[i]) {
            if (theOne == nullptr) {
                theOne = OArray[i];
                cur = theOne;
            }
            else {
                cur->intersect = OArray[i];
                cur = cur->intersect;
            }
        }
    }
    for (int i = 0; i < num; i++) {
        if (IArray[i] == nullptr) continue;
        if (!isnCol[i]) {
            if (theOne == nullptr) {
                theOne = IArray[i];
                cur = theOne;
            } else {
                cur->intersect = IArray[i];
                cur = cur->intersect;
            }
        }
    }
    int p = 0;
    for (torPoint* it : intersections) {
        // printf("NOT %d\n", p++);
        if (it == nullptr) continue;
        torPoint* trep = createLoop(it);
        torPoint* trep2 = createLoop(it->intersect);
        float t = isClockwise(trep);
        float t2 = isClockwise(trep2);
        if (t2 > 0 && t > 0) {
            trep = t > t2 ? trep : trep2;
        } else {
            trep = t > 0 ? trep : trep2;
        }
        if (theOne == nullptr) {
            theOne = trep;
            cur = trep;
        } else {
            cur->intersect = trep;
            cur = trep;
        }
    }
}

torPoint* WallMaker::createLoop(torPoint* tp) {
    torPoint* tap = new torPoint();
    tap->p = tp->p;
    torPoint* start = tap;
    torPoint* cur = tp;
    while (true) {
        cur = cur->next;
        if (cur == tp || cur->intersect == tp) {
            tap->next = start;
            break;
        }
        tap->next = new torPoint();
        tap = tap->next;
        if (cur->intersect != nullptr) {
            cur = cur->intersect;
            for (auto& et : intersections) {
                if (et == nullptr) continue;
                if (et->p == cur->p) et = nullptr;
            }
        }
        tap->p = cur->p;
    }
    return start;
}

// if return value is <0, then the result is clockwise.
float WallMaker::isClockwise(torPoint* tp) {
    torPoint* tap = tp;
    float sum = 0;
    do {
        sum += tap->p.x * tap->next->p.y - tap->next->p.x * tap->p.y;
        tap = tap->next;
    } while (tap != tp);
    return sum;
}

void WallMaker::showFinal() {
    torPoint* cur = theOne;
    while (cur != nullptr) {
        showTorPoints(cur);
        cur = cur->intersect;
    }
}

torPoint* WallMaker::getFinal() {
    return theOne;
}