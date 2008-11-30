
#ifndef _WIIMOTE_IR_INTERNAL_FUNCTIONS_
#define _WIIMOTE_IR_INTERNAL_FUNCTIONS_

/* 
   Wiimote IR data calculation supplemental functions,
   internal use only. Adapted from Wiiuse lib 0.9 (wiiuse.net)

   TODO: 

   Quite wrong with the global variables in the implementation file.
   those global variables will be shared among multiple wiimotes, 
   so that they are prone to be broken in certain/uncertain occasions.

*/

#include <wiimote.h>

bool interpret_ir_data    (wiimote& wm, float& out_x, float& out_y, float& out_z);

#endif