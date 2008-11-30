
#ifndef _WIIMOTE_IR_INTERNAL_FUNCTIONS_
#define _WIIMOTE_IR_INTERNAL_FUNCTIONS_

/* 
   Wiimote IR data calculation supplemental functions,
   internal use only. Adapted from Wiiuse lib 0.9 (wiiuse.net)
*/

#include <wiimote.h>

bool interpret_ir_data    (wiimote& wm, float& out_x, float& out_y, float& out_z);

#endif