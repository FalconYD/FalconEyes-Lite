#include "OCLHelper.h"

cl_program Lib_CreateProgram(cl_context context, cl_device_id device, const char* fileName)
{
	cl_int errNum;
	cl_program program;

	ifstream kernelFile(fileName, ios::in);
	if (!kernelFile.is_open())
	{
		cerr << "Failed to open file for reading: " << fileName << endl;
		return NULL;
	}

	ostringstream oss;
	oss << kernelFile.rdbuf();

	string srcStdStr = oss.str();
	const char *srcStr = srcStdStr.c_str();
	program = clCreateProgramWithSource(context, 1, (const char**)&srcStr, NULL, NULL);
	if (program == NULL)
	{
		// ������ ���� ������ �����Ѵ�.
		char buildLog[16384];
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buildLog), buildLog, NULL);

		cerr << "Error in Kernel: " << endl;
		cerr << buildLog;
		clReleaseProgram(program);
		return NULL;
	}

	return program;
}

bool Lib_SaveProgramBinary(cl_program program, cl_device_id deviece, const char* fileName)
{
	cl_uint numDevices = 0;
	cl_int errNum;

	// 1 - ���α׷��� ������ ����̽����� ���� �����Ѵ�.
	errNum = clGetProgramInfo(program, CL_PROGRAM_NUM_DEVICES, sizeof(cl_uint), &numDevices, NULL);

	if (errNum != CL_SUCCESS)
	{
		std::cerr << "Error querying for number of devices." << std::endl;
		return false;
	}

	// 2 - ��ü ����̽� ID���� ���Ѵ�.
	cl_device_id *devices = new cl_device_id[numDevices];
	errNum = clGetProgramInfo(program, CL_PROGRAM_DEVICES, sizeof(cl_device_id) * numDevices, devices, NULL);
	if (errNum != CL_SUCCESS)
	{
		std::cerr << "Error querying for devices." << std::endl;
		delete[] devices;
		return false;
	}

	// 3 - �� ���α׷� ���̳ʸ��� ũ�⸦ �����Ѵ�.
	size_t *programBinarySizes = new size_t[numDevices];
	errNum = clGetProgramInfo(program, CL_PROGRAM_BINARY_SIZES, sizeof(size_t) * numDevices, programBinarySizes, NULL);

	if (errNum != CL_SUCCESS)
	{
		std::cerr << "Error querying for program binary sizes." << std::endl;
		delete[] devices;
		delete[] programBinarySizes;
		return false;
	}

	unsigned char ** programBinaries = new unsigned char*[numDevices];
	for (cl_uint i = 0; i < numDevices; ++i)
	{
		programBinaries[i] = new unsigned char[programBinarySizes[i]];
	}

	// 4 - ��� ���α׷� ���̳ʸ��� ���Ѵ�.
	errNum = clGetProgramInfo(program, CL_PROGRAM_BINARIES, sizeof(unsigned char*) * numDevices, programBinaries, NULL);
	if (errNum != CL_SUCCESS)
	{
		std::cerr << "Error querying for program binaries" << std::endl;

		delete[] devices;
		delete[] programBinarySizes;
		for (cl_uint i = ; i < numDevices; ++i)
		{
			delete[] programBinaries[i];
		}
		delete[] programBinaries;
		return false;
	}

	// 5 - ���������� ��û�� ����̽��� ���� ���̳ʸ���
	// ���߿� ���� ���� ����� ��ũ�� �����Ѵ�.
	for (cl_uint i = 0; i < numDevices; ++i)
	{
		// ��û�� ����̽��� ���ؼ��� �����Ѵ�.
		// ���� ����̽��� ���Ǵ� �ó�����������
		// ��� ���̳ʸ����� ���⿡ ������ ���̴�.
		if (devices[i] == devices)
		{
			FILE *fp = fopen(fileName, "wb");
			fwrite(programBinaries[i], 1, programBinarySizes[i], fp);
			fclose(fp);
			break;
		}
	}

	// �����ϱ�
	delete[] devices;
	delete[] programBinarySizes;
	for (cl_uint i = 0; i < numDevices; ++i)
	{
		delete[] programBinaries[i];
	}
	delete[] programBinaries;
}

cl_program Lib_CreateProgramFromBinary(cl_context context, cl_device_id device, const char* filename)
{
	FILE *fp = fopen(fileName, "rb");
	if (fp == NULL)
	{
		return NULL;
	}

	// ���̳ʸ��� ũ�⸦ �����Ѵ�.
	size_t binarySize;
	fseek(fp, 0, SEEK_END);
	binarySize = ftell(fp);
	rewind(fp);

	// ��ũ���� ���̳ʸ��� �����Ѵ�.
	unsigned char *programBinary = new unsigned char[binarySize];
	fread(programBinary, 1, binarySize, fp);
	fclose(fp);

	cl_int errNum = 0;
	cl_program program;
	cl_int binaryStatus;

	program = clCreateProgramWithBinary(context, 1, &device, &binarySize, (const unsigned char**)&programBinary, &binaryStatus, &errNum);

	delete[] programBinary;
	if (errNum != CL_SUCCESS)
	{
		std::cerr << "Error loading program binary." << std::endl;
		return NULL;
	}
	if (binaryStatus != CL_SUCCESS)
	{
		std::cerr << "Invalid binary for device" << std::endl;
		return NULL;
	}

	errNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (errNum != CL_SUCCESS)
	{
		// Determine the reason for the error
		char buildLog[16384];
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buildLog), buildLog, NULL);
		std::cerr << "Error in program: " << std::endl;
		std::cerr << buildLog << std::endl;
		clReleaseProgram(program);
		return NULL;
	}

	return program;
}

cl_mem LoadImage(cl_context context, char *fileName, int &width, int &height)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName, 0);
	FIBITMAP* image = FreeImage_Load(format, fileName);

	// 32-��Ʈ �̹����� ��ȯ�Ѵ�.
	FIBITMAP* temp = image;
	image = FreeImage_ConvertTo32Bits(image);
	FreeImage_Unload(temp);
	width = FreeImage_GetWidth(image);
	height = FreeImage_GetHeight(image);

	char *buffer = new char[width * height * 4];
	memcpy(buffer, FreeImage_GetBits(image), width * height * 4);

	FreeImage_Unload(image);

	// OpenCL �̹����� �����Ѵ�.
	cl_image_format clImageFormat;
	clImageFormat.image_channel_order = CL_RGBA;
	clImageFormat.image_channel_data_type = CL_UNORM_INT8;

	cl_int errNum;
	cl_mem clImage;
	clImage = clCreateImage2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, &clImageFormat, width, height, 0, buffer, &errNum);
	// imageObject[1] = clCreateImage2D(context, CL_MEM_WRITE_ONLY, &clImageFormat, width, height, 0, NULL, &errNum);
	if (errNum != CL_SUCCESS)
	{
		std::cerr << "Error create CL image object" << std::endl;
		return 0;
	}

	return clImage;
}