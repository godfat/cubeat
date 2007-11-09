
/* 2007.11.9
  This file is deprecated, reserved only for reference uses,
  useless code blocks are removed.

  This file won't compile unless further changes made.
*/

#ifndef _SHOOTING_CUBE_INPUT_
#define _SHOOTING_CUBE_INPUT_

class Input
{
public:
	static Input& i() {
		static Input singleton;
		return singleton;
	}

    /* Damn, Just redo it. */
};

#endif