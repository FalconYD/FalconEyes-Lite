#pragma once
#include "./CL/cl.h"
#include "./CL/cl.hpp"

#ifdef _WIN64
#pragma comment (lib, "../lib/OpenCL.lib")
#else
#pragma comment (lib, "../lib/x86/OpenCL.lib")
#endif