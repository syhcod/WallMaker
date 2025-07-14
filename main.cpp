#include <stdio.h>
#include "WallMaker.h"
#include "Visualizer.h"

int main() {
    WallMaker wm = WallMaker("./dots");
    unsigned int num = wm.getNum();
    /*
    for (unsigned int i = 0; i < num; i++) {
        Visualizer v = Visualizer(wm.getDots(i), wm.getNs(i));
        v.show(!i);
    }
    wm.findCrossSection();
    for (unsigned int i = 0; i < num; i++) {
        Visualizer v = Visualizer(wm.getIn(i), wm.getNs(i));
        v.show(false);
    }
    for (unsigned int i = 0; i < num; i++) {
        Visualizer v = Visualizer(wm.getOut(i), wm.getNs(i));
        v.show(false);
    } */
    Visualizer v;
    v.intitialize();
    wm.findCrossSection();
    wm.showIn();
    wm.showOut();
    // wm.findFinal();
    wm.showFinal();
    return 0;
}