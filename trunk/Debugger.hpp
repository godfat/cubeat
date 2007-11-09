/*2007.11.9 
  Most of the things remain unchanged, only did some clean up here.
  
  All Virtools SDK dependencies ripped.
*/     

#ifndef _SHOOTING_CUBE_DEBUGGER_
#define _SHOOTING_CUBE_DEBUGGER_

#include <string>
#include <sstream>

class Debugger
{
public:
	std::ostringstream in_;
	void trace(std::string);
    template <class T>
    void trace(T const& t){
        #ifndef NDEBUG
        in_.str("");
        in_ << t << "\n";
        trace(in_.str());
        #endif
    }

	static Debugger& i() {
		static Debugger singleton;
		return singleton;
	}
private:
	Debugger() {}
};

#endif
