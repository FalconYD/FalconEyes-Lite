#pragma once
#include "OCLImport.h"

cl_program Lib_CreateProgram(cl_context context, cl_device_id device, const char* fileName);
bool Lib_SaveProgramBinary(cl_program program, cl_device_id deviece, const char* fileName);
cl_program Lib_CreateProgramFromBinary(cl_context context, cl_device_id device, const char* filename);
cl_mem LoadImage(cl_context context, char *fileName, int &width, int &height);