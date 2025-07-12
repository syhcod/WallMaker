#include "types.h"
#include <stdio.h>

#pragma once

class Visualizer {
private:
	int wallNo;
	FPoint* Vecs;
	bool printed1 = false;
	bool printed2 = false;
public:
	Visualizer(FPoint* Vecs, int wallNo) {
		this->Vecs = Vecs;
		this->wallNo = wallNo;
	}
	Visualizer() {
		this->Vecs = nullptr;
		this->wallNo = 0;
	}

	static void showWalls(FPoint* Vecs, int wallNo) {
		for (int i = 0; i < wallNo - 1; i++) {
			double wx = -Vecs[i].x + Vecs[i+1].x;
			double wy = -Vecs[i].y + Vecs[i+1].y;
			printf("L((%.2lf, %.2lf),(%.2lf, %.2lf))\n", Vecs[i].x, Vecs[i].y, wx, wy);
		}
		int i = wallNo - 1;
		double wx = -Vecs[i].x + Vecs[0].x;
		double wy = -Vecs[i].y + Vecs[0].y;
		printf("L((%.2lf, %.2lf),(%.2lf, %.2lf))\n", Vecs[i].x, Vecs[i].y, wx, wy);
	}

	static void intitialize() {
		printf("V\\left(a,p\\right)=\\operatorname{polygon}\\left(a,a+p,a+p+\\frac{1}{2}R_{a}\\left(p\\right),a+p,a+p+\\frac{1}{2}R_{b}\\left(p\\right),a+p,a\\right)\n");
		printf("c_{is}\\left(\\theta\\right)=\\left(\\cos\\theta,\\sin\\theta\\right)\n");
		printf("R_{a}\\left(v\\right)=c_{is}\\left(\\arctan\\left(v.y,v.x\\right)-\\frac{9\\pi}{10}\\right)\n");
		printf("R_{b}\\left(v\\right)=c_{is}\\left(\\arctan\\left(v.y,v.x\\right)+\\frac{9\\pi}{10}\\right)\n");
		printf("L\\left(a,p\\right)=\\operatorname{polygon}\\left(a,p+a,a\\right)\n");
	}
	void show(bool ifInitialize) {
		if (ifInitialize) {
			intitialize();
		}
		showWalls(Vecs, wallNo);
	}
};