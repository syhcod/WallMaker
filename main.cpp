#include <stdio.h>
#include "WallMaker.h"
#include "GW_Sort.h"
#include "Visualizer.h"

int main() {
    Visualizer v;
    v.intitialize();
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
    wm.findCrossSection();
    // wm.showIn();
    // wm.showOut();
    wm.findFinal();
    // wm.showFinal();
    // exit(1);
    GW_Sort::sort(wm.getFinal());
    GW_Sort::showTree();
    printf("Done\n");
    GW_Sort::writeToFile();
    return 0;
}