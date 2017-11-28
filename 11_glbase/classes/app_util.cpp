#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if defined(_ANDROID_) || defined(__ANDROID__)
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

static AAssetManager* g_assm = NULL;
void setAAssetManager(void* assm)
{
	g_assm = (AAssetManager*)assm;
}

#elif defined(_WIN32)
extern const char* AppResourcePath(const char* fileName);

#elif __ANDROID__
const char* AppResourcePath(const char* fileName)
{
	return NULL;
}
#endif



#include "app_util.h"

void WinErrorMessageBox(const char* functionLastCalled, int errorCode)
{
	LOGE(" %s : 0x%X\n", functionLastCalled, errorCode);
}

struct TGAHEADER
{
	unsigned char	ImgT;				// imageTypeCode
	short			ImgW;				// Width
	short			ImgH;				// Height
	unsigned char	ImgB;				// Bit Count
};

int LoadTGA(int* oW, int* oH, int* oD, unsigned char** oB, const char* _src_buf, size_t size)
{
	char*			 src_buf = (char*)_src_buf;
	unsigned char	charBad;					// garbage data
	short			sintBad;					// garbage data
	long			imageSize;					// size of TGA image

	TGAHEADER		Tga;

	int				nImgB;
	unsigned char*	src_pxl = NULL;


	// read first two bytes of garbage
	memcpy(&charBad, src_buf, sizeof(unsigned char) * 1);	src_buf += sizeof(unsigned char) * 1;
	memcpy(&charBad, src_buf, sizeof(unsigned char) * 1);	src_buf += sizeof(unsigned char) * 1;

	// read in the image type
	memcpy(&Tga.ImgT, src_buf, sizeof(Tga.ImgT) * 1);	src_buf += sizeof(Tga.ImgT) * 1;

	// for our purposes, the image type should be either a 2 or a 3
	if((Tga.ImgT != 2) && (Tga.ImgT != 3))
		return -1;


	// read 13 bytes of garbage data
	memcpy(&sintBad, src_buf, sizeof(short)         * 1);	src_buf += sizeof(short) * 1;
	memcpy(&sintBad, src_buf, sizeof(short)         * 1);	src_buf += sizeof(short) * 1;
	memcpy(&charBad, src_buf, sizeof(unsigned char) * 1);	src_buf += sizeof(unsigned char) * 1;
	memcpy(&sintBad, src_buf, sizeof(short)         * 1);	src_buf += sizeof(short) * 1;
	memcpy(&sintBad, src_buf, sizeof(short)         * 1);	src_buf += sizeof(short) * 1;

	// read image dimensions
	memcpy(&Tga.ImgW, src_buf, sizeof(short)        * 1);	src_buf += sizeof(short) * 1;
	memcpy(&Tga.ImgH, src_buf, sizeof(short)        * 1);	src_buf += sizeof(short) * 1;

	// read bit depth
	memcpy(&Tga.ImgB, src_buf, sizeof(unsigned char) * 1);	src_buf += sizeof(unsigned char) * 1;

	// read garbage
	memcpy(&charBad, src_buf, sizeof(unsigned char) * 1);	src_buf += sizeof(unsigned char) * 1;


	// colormode -> 3 = BGR, 4 = BGRA
	nImgB		= Tga.ImgB>>3;
	imageSize	= Tga.ImgW * Tga.ImgH * nImgB;
	src_pxl		= new unsigned char[imageSize]{};

	memcpy(src_pxl, src_buf, sizeof(unsigned char) * imageSize);	src_buf += sizeof(unsigned char) * imageSize;

	*oW	= Tga.ImgW;
	*oH	= Tga.ImgH;
	*oD	= 4;

	unsigned char* dst_pxl	= new unsigned char[Tga.ImgW * Tga.ImgH* 4]{};
	*oB = dst_pxl;

	// BGR
	if(3 == nImgB)
	{
		for(int y=0; y<Tga.ImgH; ++y)
		{
			for(int x=0; x<Tga.ImgW; ++x)
			{
				int n1 = (y*Tga.ImgW + x)* 3;
				int n2 = (y*Tga.ImgW + x)* 4;

				unsigned char B = src_pxl[n1+0];
				unsigned char G = src_pxl[n1+1];
				unsigned char R = src_pxl[n1+2];

				dst_pxl[n2+0] = R;
				dst_pxl[n2+1] = G;
				dst_pxl[n2+2] = B;
				dst_pxl[n2+3] = 0xFF;
			}
		}
	}

	// BGRA
	else if(4 == nImgB)
	{
		for(int y=0; y<Tga.ImgH; ++y)
		{
			for(int x=0; x<Tga.ImgW; ++x)
			{
				int n1 = (y*Tga.ImgW + x)* 4;
				int n2 = (y*Tga.ImgW + x)* 4;

				unsigned char B = src_pxl[n1+0];
				unsigned char G = src_pxl[n1+1];
				unsigned char R = src_pxl[n1+2];
				unsigned char A = src_pxl[n1+3];

				dst_pxl[n2+0] = R;
				dst_pxl[n2+1] = G;
				dst_pxl[n2+2] = B;
				dst_pxl[n2+3] = A;
			}
		}
	}

	delete[] src_pxl;
	return 0;
}

int LoadRAW(int* oW, int* oH, int* oD, unsigned char** oB, const char *sFile)
{
	return 0;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

FileData::FileData(const char* fileName)
: m_size(0)
, m_data(0)
{
#ifdef __ANDROID__
	AAsset* asst = AAssetManager_open(g_assm, fileName, AASSET_MODE_UNKNOWN);
	if(NULL == asst)
		return;


	m_size = (int)AAsset_getLength(asst);
	int size_4   = int((m_size+7)/4) * 4;
	m_data  = (char*)malloc(size_4);
	memset(m_data, 0, size_4);
	AAsset_read(asst, m_data, m_size);
	AAsset_close(asst);
#else
	const char* full_path = AppResourcePath(fileName);
	FILE* fp = fopen(full_path, "rb");
	if(fp)
	{
		fseek(fp, 0, SEEK_END);
		m_size = (int)ftell(fp);
		if(0<m_size)
		{
			m_data = (char*)malloc(m_size+1);
			fseek(fp, 0, SEEK_SET);
			fread(m_data, 1, m_size, fp);
			m_data[m_size]=0;
		}
		fclose(fp);
	}
#endif
}

FileData::~FileData()
{
	if(m_data)
	{
		m_size = 0;
		free(m_data);
		m_data = 0;
	}
}
