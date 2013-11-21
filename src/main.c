///////////////////////////////////
// PebblePieces: Reusable Pebble components.
// by Roger Braunstein 2013
// 
// This file serves as a demonstration of the available components.
//////////////////////////////////


#include <pebble.h>

// uncomment whichever one you wanna run MANNN
#include "test/ppbitmap_test.h"
//#include "test/pptoaster_test.h"
//#include "test/ppspinner_test.h"

int main(void) {

#ifdef TEST_PPBITMAP
    init_ppbitmap_test();
#endif
#ifdef TEST_PPTOASTER
    init_pptoaster_test();
#endif
#ifdef TEST_PPSPINNER
    init_ppspinner_test();
#endif
    
    app_event_loop();
    
#ifdef TEST_PPTOASTER
    deinit_pptoaster_test();
#endif
}
