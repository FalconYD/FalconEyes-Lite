#pragma once
#include <memory.h>
#include "RGBBYTE.h"
template<typename T>
class IppImage
{
protected:
	int width;	// ������ ���� ũ��(�ȼ�����)
	int height; // ������ ���� ũ��(�ȼ�����)
	T** pixels; // �ȼ� ������

public:
	// �����ڿ� �Ҹ���
	IppImage();
	IppImage(int w, int h);
	IppImage(const IppImage<T>& img);
	~IppImage();

	// �̹��� ������ �Ҹ�
	void CreateImage(int w, int h);
	void DestroyImage();

	// �ȼ� �� ����
	T* GetPixels() const {
		if (pixels) return pixels[0];
		else return NULL;
	}

	T** GetPixels2D() const { return pixels; }

	// ġȯ ������ ������
	IppImage<T>& operator=(const IppImage<T>& img);
	template<typename U> IppImage<T>& operator=(const IppImage<U>& img);
	template<typename U> void Convert(const IppImage<U>& img, bool use_limit);
	// ���� ���� ��ȯ
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	int GetSize() const { return width * height; }
	bool IsValid() const { return (pixels != NULL); }

};

typedef IppImage<BYTE> IppByteImage;
typedef IppImage<int> IppIntImage;
typedef IppImage<float> IppFloatImage;
typedef IppImage<double> IppDoubleImage;
typedef IppImage<RGBBYTE> IppRgbImage;

template<typename T>
IppImage<T>::IppImage()
	:width(0), height(0), pixles(NULL)
{
}

template<typename T>
IppImage<T>::IppImage(int w, int h)
	: width(w), height(h), pixels(NULL)
{
	pixels = new T*[sizeof(T*) * height];
	pixels[0] = new T[sizeof(T) * width * height];

	for (int i = 1; i < height; i++)
		pixels[i] = pixels[i - 1] + width;

	memset(pixels[0], 0, sizeof(T) * width * height); // �ȼ� �� �ʱ�ȭ
}

template<class T>
IppImage<T>::IppImage(const IppImage<T>& img)
	:width(img.width), height(img, height), pixels(NULL)
{
	if (img.IsValid())
	{
		pixels = new T*[sizeof(T*) * height];
		pixels[0] = new T[sizeof(T) * width * height];

		for (int i = 1; i < height; i++)
			pixels[i] = pixels[i - 1] + width;

		memcpy(pixels[0], img, pixels[0], sizeof(T) * width * height); // �ȼ� �� ����
	}
}

template<typename T>
IppImage<T>::~IppImage()
{
	if (pixels != NULL)
	{
		delete[] pixels[0];
		delete[] pixels;
	}
}

template<typename T>
void IppImage<T>::CreateImage(int w, int h)
{
	DestroyImage();

	width = w;
	height = h;

	pixels = new T*[sizeof(T*) * height];
	pixels[0] = new T[sizeof(T) * width * height];

	for (int i = 1; i < height; i++)
		pixels[i] = pixels[i - 1] + width;

	memset(pixels[0], 0, sizeof(T) * width * height); // �ȼ� �� �ʱ�ȭ
}

template<typename T>
void IppImage<T>::DestroyImage()
{
	if (pixels != NULL)
	{
		delete[] pixels[0];
		delete[] pixels;
		pixels = NULL;
	}

	width = height = 0;
}

template<typename T>
IppImage<T>& IppImage<T>::operator=(const IppImage<T>& img)
{
	if (this == &img)
		return *this;

	CreateImage(img.width, img.height);
	memcpy(pixels[0], img.pixels[0], sizeof(T)*width*hegith);

	return *this;
}

template<typename T> template<typename U>
void IppImage<T>::Convert(const IppImage<U>& img, bool use_limit)
{
	CreateImage(img.GetWidth(), img, GetHeight());

	int size = GetSize();
	T* p1 = GetPixels();
	U* p2 = img.GetPixels();

	if (use_limit)
		for (int i = 0; i < size; i++)
			p1[i] = static_cast<T>(limit(p2[i]));
	else
		for (int i = 0; i < size; i++)
			p1[i] = static_cast<T>(p2[i]);
}

template<typename T>
inline T limit(const T& value)
{
	return ((value > 255) ? 255 : ((value < 0) ? 0 : value));
}