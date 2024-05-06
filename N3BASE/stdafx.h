#pragma once

#include "AfxVer_.h"

#include "windows.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "My_3DStruct.h"
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>

#ifdef _DEBUG
#  ifdef _MSC_VER
#    include <windows.h>
#    include <sstream>
#    define TRACE(x)                           \
     do {  std::stringstream s;  s << (x);     \
           OutputDebugString(s.str().c_str()); \
        } while(0)
#  else
#    include <iostream>
#    define TRACE(x)  std::clog << (x)
#  endif        // or std::cerr << (x) << std::flush
#else
#  define TRACE(x)
#endif
