#include "GW_Sort.h"
#include <stdio.h>
#include <vector>

FPoint* GW_Sort::points = nullptr;
int GW_Sort::pointsLen = 0;

void printWalls(torPoint* one, torPoint* two) {
    printf("L((%.2f, %.2f), (%.2f, %.2f))\n", one->p.x, one->p.y, two->p.x - one->p.x, two->p.y - one->p.y);
    printf("L((%.2f, %.2f), (%.2f, %.2f))\n", one->next->p.x, one->next->p.y, two->next->p.x - one->next->p.x, two->next->p.y - one->next->p.y);
}

void GW_Sort::sort(torPoint* walls) {
    node* root = transcribe(walls);
    recSort(root);
    doublePoint check = findEqWalls(walls);
}

doublePoint GW_Sort::findEqWalls(torPoint* Walls) {
    for (torPoint* first = Walls; true; first = first->intersect) {
    for (torPoint* sec = first->intersect; true; sec = sec->intersect) {
    torPoint* cur1 = first;
    torPoint* cur2 = sec;

    do {
        do {
            printWalls(cur1, cur2);
            if (isEq(cur1, cur2)) {
                doublePoint ret;
                ret.one = cur1;
                ret.two = cur2;
                return ret;
            }
            cur2 = cur2->next;
        } while (cur2 != sec);
        cur1 = cur1->next;
    } while (cur1 != first);

    if (sec->intersect == nullptr) break;
    }
    if (first->intersect == nullptr) break;
    }
    doublePoint ret;
    ret.one = nullptr;
    ret.two = nullptr;
    return ret;
}

bool GW_Sort::isEq(torPoint* first, torPoint* sec) {
    FPoint p1 = first->p;
    FPoint p = first->next->p;
    p = p - p1;
    FPoint d1 = sec->p;
    FPoint d = sec->next->p;
    d = d - d1;
    float ang1, ang2, ang3;
    ang1 = getAngle(p);
    ang2 = getAngle(d);
    d = d1 - p1;
    ang3 = getAngle(d);
    bool ret = (abs(ang1 - ang2) < 0.002 || (ang1 - ang2 > M_PI - 0.002 && ang1 - ang2 < M_PI + 0.002));
    ret = ret && (abs(ang1 - ang3) < 0.002 || (ang1 - ang3 > M_PI - 0.002 && ang1 - ang3 < M_PI + 0.002));
    ret = ret && (abs(ang3 - ang2) < 0.002 || (ang3 - ang2 > M_PI - 0.002 && ang3 - ang2 < M_PI + 0.002));

    return ret;
}

float GW_Sort::getAngle(FPoint p) {
    if (p.x < 0.002f && -p.x < 0.002f) return M_PI / 2.0f;
    else return atan2(p.y, p.x);
}

void GW_Sort::recSort(node* walls) {
    ;
}

node* GW_Sort::transcribe(torPoint* Walls) {
    std::vector<FPoint*> v;
    node* root = nullptr;
    node* current = root;
    for (torPoint* first = Walls; first != nullptr; first = first->intersect) {
        torPoint* cur = first;
        do {
            v.push_back(new FPoint(cur->p));
            cur = cur->next;
        } while (cur != first);
    }
    points = (FPoint*)malloc(sizeof(FPoint) * v.size());
    pointsLen = v.size();
    for (int i =0; i < pointsLen; i++) {
        points[i] = *(v[i]);
        delete v[i];
    }

    int i = -1;
    
    for (torPoint* first = Walls; first != nullptr; first = first->intersect) {
        torPoint* cur = first;
        int start = i + 1;
        do {
            i++;
            node* n = new node();
            n->p1 = &(points[i]);
            if (cur->next != first) n->p2 = &(points[i]);
            else n->p2 = &(points[start]);
            if (root == nullptr) {
                root = n;
                current = root;
            } else {
                current->front = n;
                current = n;
            }
            cur = cur->next;
        } while (cur != first);
    }
    return root;
}

