#include "GW_Sort.h"
#include <stdio.h>
#include <vector>

FPoint* GW_Sort::points = nullptr;
int GW_Sort::pointsLen = 0;

node* GW_Sort::root = nullptr;

void printWalls(torPoint* one, torPoint* two) {
    printf("L((%.2f, %.2f), (%.2f, %.2f))\n", one->p.x, one->p.y, two->p.x - one->p.x, two->p.y - one->p.y);
    printf("L((%.2f, %.2f), (%.2f, %.2f))\n", one->next->p.x, one->next->p.y, two->next->p.x - one->next->p.x, two->next->p.y - one->next->p.y);
}

void GW_Sort::sort(torPoint* walls) {
    root = transcribe(walls);
    node* c = root->front;
    root->front = nullptr;
    recSort(c, root);
    doublePoint check = findEqWalls(walls);
}

doublePoint GW_Sort::findEqWalls(torPoint* Walls) {
    for (torPoint* first = Walls; true; first = first->intersect) {
    for (torPoint* sec = first->intersect; true; sec = sec->intersect) {
    torPoint* cur1 = first;
    torPoint* cur2 = sec;

    do {
        do {
            // printWalls(cur1, cur2);
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

bool GW_Sort::isRight(FPoint a, FPoint b, FPoint c) {
    float res = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    return res < 0;
}

void GW_Sort::recSort(node* walls, node* root) {
    printNode(walls);
    printNode(root);
    if (walls == nullptr) return;
    FPoint rp1 = *(root->p1);
    FPoint rp2 = *(root->p2);
    FPoint* inter = getInter(*(walls->p1), *(walls->p2), rp1, rp2);
    FPoint mid = (rp1 + rp2) / 2.0f;
    if (inter == nullptr) {
        bool right = isRight(*(walls->p1), *(walls->p2), mid);
        node* next = nullptr;
        if (right) next = root->front;
        else next = root->back;
        if (next == nullptr) {
            if (right) root->front = walls;
            else root->back = walls;
        } else recSort(walls, next);
    }
    else {
        node* a = new node();
        node* b = new node();
        FPoint* st = root->p1, *ed = root->p2;
        a->p1 = st;
        a->p2 = inter;
        b->p1 = inter;
        b->p2 = ed;
        if (isRight(*(walls->p1), *(walls->p2), *(root->p1))) {
            recSort(a, root->front);
            recSort(b, root->back);
        } else {
            recSort(a, root->back);
            recSort(b, root->front);
        }
    }
    node* c = walls->front;
    walls->front = nullptr;
    recSort(c, GW_Sort::root);
}

void GW_Sort::showTree() {
    RST(root);
}

void GW_Sort::RST(node* n) {
    if (n == nullptr) return;
    RST(n->back);
    printNode(n);
    RST(n->front);
}

void GW_Sort::printNode(node* n) {
    FPoint s = *(n->p1);
    FPoint e = *(n->p2);
    e = e - s;
    printf("L((%.2f, %.2f), (%.2f, %.2f))\n", s.x, s.y, e.x, e.y);
}

bool GW_Sort::isSim(FPoint a, FPoint b) {
    float res = (pow(a.x - b.x, 2.0f) + pow(a.y - b.y, 2.0f));
    if (res < 0.0001f) return true;
    else return false;
}

// p3, p4 must be the wall that divides!!
FPoint* GW_Sort::getInter(FPoint p1, FPoint p2, FPoint p3, FPoint p4) {
    if (p1 == p3 || p1 == p4 || p2 == p3 || p2 == p4) return nullptr;
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

        
        // printf("\tWay Gone\n");
        fp = new FPoint(x,y);
        if (isSim(p1, *fp) || isSim(p2, *fp)) return nullptr;
        return fp;
    }
    return fp;
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
            n->notSet = true;
            n->isGhost = false;
            n->p1 = &(points[i]);
            if (cur->next != first) n->p2 = &(points[i + 1]);
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

