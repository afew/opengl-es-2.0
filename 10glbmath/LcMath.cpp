// Implementation of the LCX Math Library.
//
////////////////////////////////////////////////////////////////////////////////

#if defined(_PC_WINDOW_)
  #pragma warning(disable: 4996)
#endif


// Log Output Definition
#if defined(__AOS__)
  #include <android/log.h>
  //ANDROID_LOG_DEBUG   = 3,
  //ANDROID_LOG_INFO    = 4,
  //ANDROID_LOG_WARN    = 5,
  //ANDROID_LOG_ERROR   = 6,

  #define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO,  "G-pack", __VA_ARGS__))
  #define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN,  "G-pack", __VA_ARGS__))
  #define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "G-pack", __VA_ARGS__))

#elif defined(__TIZEN__)
  #include <FBase.h>	// for AppLog
  #define LOGI	AppLog
  #define LOGW	AppLog
  #define LOGE	AppLog

#else
  #define LOGI	printf
  #define LOGW	printf
  #define LOGE	printf

#endif

#include <math.h>
#include <stdlib.h>
#include <string.h>

//#ifndef LCX_FMAX
//  #if defined(_PC_WINDOW_)
//    #define LCX_FMAX         3.402823466e+38F
//  #else
//    #define LCX_FMAX         3.40282347e+38F		//3.402823466e+38F ==>0X7F7FFFFF
//  #endif
//#endif

#include "LcType.h"
#include "LcMath.h"


FLOAT LCX_InfinityFloat()
{
	static UINT t = 0x7F800000;
	FLOAT r = 0;

	memcpy(&r, &t, sizeof(r));

	//unsigned char t[8] {};
	//memcpy(t, &r, sizeof(r));

	return r;
}


DOUBLE LCX_InfinityDouble()
{
	//static unsigned char t[8]={0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0xF0, 0x7F};
	static UDINT t = 0x7FF0000000000000;
	DOUBLE r = 0;

	memcpy(&r, &t, sizeof(r));

	//r = -DBL_MAX;
	//memcpy(&t, &r, sizeof(r));

	return r;
}


FLOAT LCX_NaNFloat()
{
	static UINT t = 0x7FFFFFFF;
	FLOAT r = 0;
	memcpy(&r, &t, sizeof(r));
	return r;
}


DOUBLE LCX_NaNDouble()
{
	//static unsigned char t[8]={0xFF, 0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xF0, 0x7F};
	static UDINT  t = 0x7FF0FFFFFFFFFFFF;
	DOUBLE r = 0;
	memcpy(&r, &t, sizeof(r));
	return r;
}


INT LCX_IsNaN(FLOAT v)
{
	//+FLT_MAX: 0x7F7FFFFF
	//-FLT_MAX: 0xFF7FFFFF

	INT t = 0x0;

	memcpy(&t, &v, sizeof(v));

	if(0x7F7FFFFF < t || t< 0xFF7FFFFF)
		return 1;


	return 0;
}


INT LCX_IsNaN(DOUBLE v)
{
	//+DBL_MAX: 0x7FEFFFFFFFFFFFFF
	//-DBL_MAX: 0xFFEFFFFFFFFFFFFF

	DINT  t = 0x0;

	memcpy(&t, &v, sizeof(v));

	if(0x7FEFFFFFFFFFFFFF < t || t< 0xFFEFFFFFFFFFFFFF)
		return 1;


	return 0;
}


INT LCX_IsFinite(FLOAT v)
{
	INT t = 0x0;

	memcpy(&t, &v, sizeof(v));

	if(0x7F7FFFFF >= t && t >= 0xFF7FFFFF)
		return 1;


	return 0;
}


INT LCX_IsFinite(DOUBLE v)
{
	//+DBL_MAX: 0x7FEFFFFFFFFFFFFF
	//-DBL_MAX: 0xFFEFFFFFFFFFFFFF

	DINT t = 0x0;

	memcpy(&t, &v, sizeof(v));

	if(0x7FEFFFFFFFFFFFFF >= t && t >= 0xFFEFFFFFFFFFFFFF)
		return 1;


	return 0;
}



/*
static FLOAT TST_LCXMatDet2x2(FLOAT _11, FLOAT _12
							, FLOAT _21, FLOAT _22
						  )
{
	return (_11 * _22 - _12 * _21);
}


static FLOAT TST_LCXMatDet3x3(LCXMATRIX3X3* v)
{
	FLOAT r = 0;

	r  = v->_11 * (v->_22 * v->_33 - v->_23 * v->_32);
	r += v->_12 * (v->_23 * v->_31 - v->_21 * v->_33);
	r += v->_13 * (v->_21 * v->_32 - v->_22 * v->_31);

	return r;
}


static FLOAT TST_LCXMatDet4x4(FLOAT _11, FLOAT _12, FLOAT _13, FLOAT _14
							, FLOAT _21, FLOAT _22, FLOAT _23, FLOAT _24
							, FLOAT _31, FLOAT _32, FLOAT _33, FLOAT _34
							, FLOAT _41, FLOAT _42, FLOAT _43, FLOAT _44
			   )
{
	FLOAT r = 0;

	r += (_24 * _11 - _14 * _21) * (_32 * _43 - _33 * _42);
	r += (_24 * _12 - _14 * _22) * (_33 * _41 - _31 * _43);
	r += (_24 * _13 - _14 * _23) * (_31 * _42 - _32 * _41);

	r += (_44 * _31 - _34 * _41) * (_12 * _23 - _13 * _22);
	r += (_44 * _32 - _34 * _42) * (_13 * _21 - _11 * _23);
	r += (_44 * _33 - _34 * _43) * (_11 * _22 - _12 * _21);

	return r;
}



static void TST_LCXMatAgj3x3(FLOAT* Out, LCXMATRIX3X3* v)
{
	Out[0] = +(v->_22 * v->_33 - v->_23 * v->_32);
	Out[1] = -(v->_12 * v->_33 - v->_13 * v->_32);
	Out[2] = +(v->_12 * v->_23 - v->_13 * v->_22);

	Out[3] = -(v->_21 * v->_33 - v->_23 * v->_31);
	Out[4] = +(v->_11 * v->_33 - v->_13 * v->_31);
	Out[5] = -(v->_11 * v->_23 - v->_13 * v->_21);

	Out[6] = +(v->_21 * v->_32 - v->_22 * v->_31);
	Out[7] = -(v->_11 * v->_32 - v->_12 * v->_31);
	Out[8] = +(v->_11 * v->_22 - v->_12 * v->_31);
}


static void TST_LCXMatAgj4x4(FLOAT* Out, LCXMATRIX* v)
{
	FLOAT t1 = v->_33 * v->_44 - v->_34 * v->_43;
	FLOAT t2 = v->_34 * v->_42 - v->_32 * v->_44;
	FLOAT t3 = v->_32 * v->_43 - v->_33 * v->_42;

	Out[ 0]= +(v->_22 * t1 + v->_23 * t2 + v->_24 * t3);
	Out[ 1]= -(v->_12 * t1 + v->_13 * t2 + v->_14 * t3);


	t1 = v->_13 * v->_24 - v->_14 * v->_23;
	t2 = v->_14 * v->_22 - v->_12 * v->_24;
	t3 = v->_12 * v->_23 - v->_13 * v->_22;

	Out[ 2]= +(v->_42 * t1 + v->_43 * t2 + v->_44 * t3);
	Out[ 3]= -(v->_32 * t1 + v->_33 * t2 + v->_34 * t3);


	t1 = v->_33 * v->_44 - v->_34 * v->_43;
	t2 = v->_34 * v->_41 - v->_31 * v->_44;
	t3 = v->_31 * v->_43 - v->_33 * v->_41;

	Out[ 4]= -(v->_21 * t1 + v->_23 * t2 + v->_24 * t3);
	Out[ 5]= +(v->_11 * t1 + v->_13 * t2 + v->_14 * t3);


	t1 = v->_13 * v->_24 - v->_14 * v->_23;
	t2 = v->_14 * v->_21 - v->_11 * v->_24;
	t3 = v->_11 * v->_23 - v->_13 * v->_21;

	Out[ 6]= -(v->_41 * t1 + v->_43 * t2 + v->_44 * t3);
	Out[ 7]= +(v->_31 * t1 + v->_33 * t2 + v->_34 * t3);


	t1 = v->_32 * v->_44 - v->_34 * v->_42;
	t2 = v->_34 * v->_41 - v->_31 * v->_44;
	t3 = v->_31 * v->_42 - v->_32 * v->_41;

	Out[ 8]= +(v->_21 * t1 + v->_22 * t2 + v->_24 * t3);
	Out[ 9]= -(v->_11 * t1 + v->_12 * t2 + v->_14 * t3);


	t1 = v->_12 * v->_24 - v->_14 * v->_22;
	t2 = v->_14 * v->_21 - v->_11 * v->_24;
	t3 = v->_11 * v->_22 - v->_12 * v->_21;

	Out[10]= +(v->_41 * t1 + v->_42 * t2 + v->_44 * t3);
	Out[11]= -(v->_31 * t1 + v->_32 * t2 + v->_34 * t3);


	t1 = v->_32 * v->_43 - v->_33 * v->_42;
	t2 = v->_33 * v->_41 - v->_31 * v->_43;
	t3 = v->_31 * v->_42 - v->_32 * v->_41;

	Out[12]= -(v->_21 * t1 + v->_22 * t2 + v->_23 * t3);
	Out[13]= +(v->_11 * t1 + v->_12 * t2 + v->_13 * t3);


	t1 = v->_12 * v->_23 - v->_13 * v->_22;
	t2 = v->_13 * v->_21 - v->_11 * v->_23;
	t3 = v->_11 * v->_22 - v->_12 * v->_21;

	Out[14]= -(v->_41 * t1 + v->_42 * t2 + v->_43 * t3);
	Out[15]= +(v->_31 * t1 + v->_32 * t2 + v->_33 * t3);
}


static INT TST_LCXMatInv2x2(FLOAT* Out, FLOAT* det, LCXMATRIX2X2* v)
{
	FLOAT d = v->_11 * v->_22 - v->_12 * v->_21;

	if(det) *det = d;
	if(-0.00001F < d && d <0.00001F)
		return LC_EFAIL;

	d = 1.0F/d;


	Out[0] =  v->_11;
	Out[1] = -v->_12;
	Out[2] = -v->_21;
	Out[3] =  v->_22;

	Out[0] *= d;	Out[1] *= d;
	Out[2] *= d;	Out[3] *= d;

	return LC_OK;
}

static INT TST_LCXMatInv3x3(FLOAT* Out, FLOAT* det, LCXMATRIX3X3* v)
{
//	FLOAT d = TST_LCXMatDet3x3(v);
//
//	if(det) *det = d;
//	if(-0.00001F < d && d <0.00001F)
//		return LC_EFAIL;
//
//	d = 1.0F/d;
//
//
//	TST_LCXMatAgj3x3(Out, v);
//
//	Out[0] *= d;	Out[1] *= d;	Out[2] *= d;
//	Out[3] *= d;	Out[4] *= d;	Out[5] *= d;
//	Out[6] *= d;	Out[7] *= d;	Out[8] *= d;
//
//	return LC_OK;

	FLOAT d = 0;

	d += v->_11 * (v->_22 * v->_33 - v->_23 * v->_32);
	d += v->_12 * (v->_23 * v->_31 - v->_21 * v->_33);
	d += v->_13 * (v->_21 * v->_32 - v->_22 * v->_31);

	if(det) *det = d;
	if(-0.00001F < d && d <0.00001F)
		return LC_EFAIL;

	d = 1.0F/d;


	Out[0] = +(v->_22 * v->_33 - v->_23 * v->_32);
	Out[1] = -(v->_12 * v->_33 - v->_13 * v->_32);
	Out[2] = +(v->_12 * v->_23 - v->_13 * v->_22);

	Out[3] = -(v->_21 * v->_33 - v->_23 * v->_31);
	Out[4] = +(v->_11 * v->_33 - v->_13 * v->_31);
	Out[5] = -(v->_11 * v->_23 - v->_13 * v->_21);

	Out[6] = +(v->_21 * v->_32 - v->_22 * v->_31);
	Out[7] = -(v->_11 * v->_32 - v->_12 * v->_31);
	Out[8] = +(v->_11 * v->_22 - v->_12 * v->_31);


	Out[0] *= d;	Out[1] *= d;	Out[2] *= d;
	Out[3] *= d;	Out[4] *= d;	Out[5] *= d;
	Out[6] *= d;	Out[7] *= d;	Out[8] *= d;

	return LC_OK;
}


static INT TST_LCXMatInv4x4(FLOAT *Out, FLOAT* det, LCXMATRIX* v)
{
//  A^-1 = adj(Aij)/Det(A)
//
//	FLOAT d = TST_LCXMatDet4x4(_11, _12, _13, _14
//							,_21, _22, _23, _24
//							,_31, _32, _33, _34
//							,_41, _42, _43, _44);
//	if(det) *det = d;
//	if(-0.00001F < d && d <0.00001F)
//		return LC_EFAIL;
//
//	d = 1.0F/d;
//
//	TST_LCXMatAgj4x4(Out
//				, _11, _12, _13, _14
//				, _21, _22, _23, _24
//				, _31, _32, _33, _34
//				, _41, _42, _43, _44 );
//
//	Out[ 0] *= d;	Out[ 1] *= d;	Out[ 2] *= d;	Out[ 3] *= d;
//	Out[ 4] *= d;	Out[ 5] *= d;	Out[ 6] *= d;	Out[ 7] *= d;
//	Out[ 8] *= d;	Out[ 9] *= d;	Out[10] *= d;	Out[11] *= d;
//	Out[12] *= d;	Out[13] *= d;	Out[14] *= d;	Out[15] *= d;
//
//	return LC_OK;


	FLOAT d  = 0;
	FLOAT t1 = 0;
	FLOAT t2 = 0;
	FLOAT t3 = 0;

	d += (v->_24 * v->_11 - v->_14 * v->_21) * (v->_32 * v->_43 - v->_33 * v->_42);
	d += (v->_24 * v->_12 - v->_14 * v->_22) * (v->_33 * v->_41 - v->_31 * v->_43);
	d += (v->_24 * v->_13 - v->_14 * v->_23) * (v->_31 * v->_42 - v->_32 * v->_41);

	d += (v->_44 * v->_31 - v->_34 * v->_41) * (v->_12 * v->_23 - v->_13 * v->_22);
	d += (v->_44 * v->_32 - v->_34 * v->_42) * (v->_13 * v->_21 - v->_11 * v->_23);
	d += (v->_44 * v->_33 - v->_34 * v->_43) * (v->_11 * v->_22 - v->_12 * v->_21);

	if(det) *det = d;
	if(-0.00001F < d && d <0.00001F)
		return LC_EFAIL;

	d = 1.0F/d;


	t1 = v->_33 * v->_44 - v->_34 * v->_43;
	t2 = v->_34 * v->_42 - v->_32 * v->_44;
	t3 = v->_32 * v->_43 - v->_33 * v->_42;

	Out[ 0]= +(v->_22 * t1 + v->_23 * t2 + v->_24 * t3);
	Out[ 1]= -(v->_12 * t1 + v->_13 * t2 + v->_14 * t3);


	t1 = v->_13 * v->_24 - v->_14 * v->_23;
	t2 = v->_14 * v->_22 - v->_12 * v->_24;
	t3 = v->_12 * v->_23 - v->_13 * v->_22;

	Out[ 2]= +(v->_42 * t1 + v->_43 * t2 + v->_44 * t3);
	Out[ 3]= -(v->_32 * t1 + v->_33 * t2 + v->_34 * t3);


	t1 = v->_33 * v->_44 - v->_34 * v->_43;
	t2 = v->_34 * v->_41 - v->_31 * v->_44;
	t3 = v->_31 * v->_43 - v->_33 * v->_41;

	Out[ 4]= -(v->_21 * t1 + v->_23 * t2 + v->_24 * t3);
	Out[ 5]= +(v->_11 * t1 + v->_13 * t2 + v->_14 * t3);


	t1 = v->_13 * v->_24 - v->_14 * v->_23;
	t2 = v->_14 * v->_21 - v->_11 * v->_24;
	t3 = v->_11 * v->_23 - v->_13 * v->_21;

	Out[ 6]= -(v->_41 * t1 + v->_43 * t2 + v->_44 * t3);
	Out[ 7]= +(v->_31 * t1 + v->_33 * t2 + v->_34 * t3);


	t1 = v->_32 * v->_44 - v->_34 * v->_42;
	t2 = v->_34 * v->_41 - v->_31 * v->_44;
	t3 = v->_31 * v->_42 - v->_32 * v->_41;

	Out[ 8]= +(v->_21 * t1 + v->_22 * t2 + v->_24 * t3);
	Out[ 9]= -(v->_11 * t1 + v->_12 * t2 + v->_14 * t3);


	t1 = v->_12 * v->_24 - v->_14 * v->_22;
	t2 = v->_14 * v->_21 - v->_11 * v->_24;
	t3 = v->_11 * v->_22 - v->_12 * v->_21;

	Out[10]= +(v->_41 * t1 + v->_42 * t2 + v->_44 * t3);
	Out[11]= -(v->_31 * t1 + v->_32 * t2 + v->_34 * t3);


	t1 = v->_32 * v->_43 - v->_33 * v->_42;
	t2 = v->_33 * v->_41 - v->_31 * v->_43;
	t3 = v->_31 * v->_42 - v->_32 * v->_41;

	Out[12]= -(v->_21 * t1 + v->_22 * t2 + v->_23 * t3);
	Out[13]= +(v->_11 * t1 + v->_12 * t2 + v->_13 * t3);


	t1 = v->_12 * v->_23 - v->_13 * v->_22;
	t2 = v->_13 * v->_21 - v->_11 * v->_23;
	t3 = v->_11 * v->_22 - v->_12 * v->_21;

	Out[14]= -(v->_41 * t1 + v->_42 * t2 + v->_43 * t3);
	Out[15]= +(v->_31 * t1 + v->_32 * t2 + v->_33 * t3);

	// finally A^-1 = adj(Aij)/Det(A)
	Out[ 0] *= d;	Out[ 1] *= d;	Out[ 2] *= d;	Out[ 3] *= d;
	Out[ 4] *= d;	Out[ 5] *= d;	Out[ 6] *= d;	Out[ 7] *= d;
	Out[ 8] *= d;	Out[ 9] *= d;	Out[10] *= d;	Out[11] *= d;
	Out[12] *= d;	Out[13] *= d;	Out[14] *= d;	Out[15] *= d;

	return LC_OK;
}



static INT TST_LCXMatInv4x4(FLOAT* Out, FLOAT* det, FLOAT* v)
{
	FLOAT d = 0;

	FLOAT t1;
	FLOAT t2;
	FLOAT t3;

	d += (v[ 7] * v[ 0] - v[ 3] * v[ 4]) * (v[ 9] * v[14] - v[10] * v[13]);
	d += (v[ 7] * v[ 1] - v[ 3] * v[ 5]) * (v[10] * v[12] - v[ 8] * v[14]);
	d += (v[ 7] * v[ 2] - v[ 3] * v[ 6]) * (v[ 8] * v[13] - v[ 9] * v[12]);

	d += (v[15] * v[ 8] - v[11] * v[12]) * (v[ 1] * v[ 6] - v[ 2] * v[ 5]);
	d += (v[15] * v[ 9] - v[11] * v[13]) * (v[ 2] * v[ 4] - v[ 0] * v[ 6]);
	d += (v[15] * v[10] - v[11] * v[14]) * (v[ 0] * v[ 5] - v[ 1] * v[ 4]);

	if(det) *det = d;
	if(-0.00001F < d && d <0.00001F)
		return LC_EFAIL;

	d = 1.0F/d;


	t1 = v[10] * v[15] - v[11] * v[14];
	t2 = v[11] * v[13] - v[ 9] * v[15];
	t3 = v[ 9] * v[14] - v[10] * v[13];

	Out[ 0]= +(v[ 5] * t1 + v[ 6] * t2 + v[ 7] * t3);
	Out[ 1]= -(v[ 1] * t1 + v[ 2] * t2 + v[ 3] * t3);


	t1 = v[ 2] * v[ 7] - v[ 3] * v[ 6];
	t2 = v[ 3] * v[ 5] - v[ 1] * v[ 7];
	t3 = v[ 1] * v[ 6] - v[ 2] * v[ 5];

	Out[ 2]= +(v[13] * t1 + v[14] * t2 + v[15] * t3);
	Out[ 3]= -(v[ 9] * t1 + v[10] * t2 + v[11] * t3);


	t1 = v[10] * v[15] - v[11] * v[14];
	t2 = v[11] * v[12] - v[ 8] * v[15];
	t3 = v[ 8] * v[14] - v[10] * v[12];

	Out[ 4]= -(v[ 4] * t1 + v[ 6] * t2 + v[ 7] * t3);
	Out[ 5]= +(v[ 0] * t1 + v[ 2] * t2 + v[ 3] * t3);


	t1 = v[ 2] * v[ 7] - v[ 3] * v[ 6];
	t2 = v[ 3] * v[ 4] - v[ 0] * v[ 7];
	t3 = v[ 0] * v[ 6] - v[ 2] * v[ 4];

	Out[ 6]= -(v[12] * t1 + v[14] * t2 + v[15] * t3);
	Out[ 7]= +(v[ 8] * t1 + v[10] * t2 + v[11] * t3);


	t1 = v[ 9] * v[15] - v[11] * v[13];
	t2 = v[11] * v[12] - v[ 8] * v[15];
	t3 = v[ 8] * v[13] - v[ 9] * v[12];

	Out[ 8]= +(v[ 4] * t1 + v[ 5] * t2 + v[ 7] * t3);
	Out[ 9]= -(v[ 0] * t1 + v[ 1] * t2 + v[ 3] * t3);


	t1 = v[ 1] * v[ 7] - v[ 3] * v[ 5];
	t2 = v[ 3] * v[ 4] - v[ 0] * v[ 7];
	t3 = v[ 0] * v[ 5] - v[ 1] * v[ 4];

	Out[10]= +(v[12] * t1 + v[13] * t2 + v[15] * t3);
	Out[11]= -(v[ 8] * t1 + v[ 9] * t2 + v[11] * t3);


	t1 = v[ 9] * v[14] - v[10] * v[13];
	t2 = v[10] * v[12] - v[ 8] * v[14];
	t3 = v[ 8] * v[13] - v[ 9] * v[12];

	Out[12]= -(v[ 4] * t1 + v[ 5] * t2 + v[ 6] * t3);
	Out[13]= +(v[ 0] * t1 + v[ 1] * t2 + v[ 2] * t3);


	t1 = v[ 1] * v[ 6] - v[ 2] * v[ 5];
	t2 = v[ 2] * v[ 4] - v[ 0] * v[ 6];
	t3 = v[ 0] * v[ 5] - v[ 1] * v[ 4];

	Out[14]= -(v[12] * t1 + v[13] * t2 + v[14] * t3);
	Out[15]= +(v[ 8] * t1 + v[ 9] * t2 + v[10] * t3);

	// finally A^-1 = adj(Aij)/Det(A)
	Out[ 0] *= d;	Out[ 1] *= d;	Out[ 2] *= d;	Out[ 3] *= d;
	Out[ 4] *= d;	Out[ 5] *= d;	Out[ 6] *= d;	Out[ 7] *= d;
	Out[ 8] *= d;	Out[ 9] *= d;	Out[10] *= d;	Out[11] *= d;
	Out[12] *= d;	Out[13] *= d;	Out[14] *= d;	Out[15] *= d;

	return LC_OK;
}
*/



//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// 32-bit fixed point number

FIXED32::FIXED32() { _ = 0; }
FIXED32::FIXED32(const char  & v) { _ = v<< FIXED_PRECISION     ; }
FIXED32::FIXED32(const SHORT & v) { _ = v<< FIXED_PRECISION     ; }
FIXED32::FIXED32(const INT   & v) { _ = v<< FIXED_PRECISION     ; }
FIXED32::FIXED32(const FLOAT & v) { _ = INT(v * 65536.0F  )     ; }
FIXED32::FIXED32(const DOUBLE& v) { _ = INT(v * 65536.00  )     ; }
FIXED32::FIXED32(const DINT  & v) { _ = INT(v<< FIXED_PRECISION); }

FIXED32::FIXED32(const FIXED32& v){ *this = v; }
FIXED32& FIXED32::operator=(const FIXED32& v)
{
	this->_ = v._;
	return *this;
}

FIXED32 FIXED32::operator =(const char  & v) { _ = v<< FIXED_PRECISION     ; return _; }
FIXED32 FIXED32::operator =(const SHORT & v) { _ = v<< FIXED_PRECISION     ; return _; }
FIXED32 FIXED32::operator =(const INT   & v) { _ = v<< FIXED_PRECISION     ; return _; }
FIXED32 FIXED32::operator =(const FLOAT & v) { _ = INT(v * 65536.0F  )     ; return _; }
FIXED32 FIXED32::operator =(const DOUBLE& v) { _ = INT(v * 65536.00  )     ; return _; }
FIXED32 FIXED32::operator =(const DINT  & v) { _ = INT(v<< FIXED_PRECISION); return _; }

FIXED32::operator char   () const { return (char  )(_>> FIXED_PRECISION); }
FIXED32::operator SHORT  () const { return (SHORT )(_>> FIXED_PRECISION); }
FIXED32::operator INT    () const { return (INT   )(_>> FIXED_PRECISION); }
FIXED32::operator FLOAT  () const { return (FLOAT )_/65536.0F;            }
FIXED32::operator DOUBLE () const { return (DOUBLE)_/65536.00;            }
FIXED32::operator DINT   () const { return (DINT  )(_>> FIXED_PRECISION); }

// unary operators
FIXED32 FIXED32::operator +() const { return  _; }
FIXED32 FIXED32::operator -() const { return -_; }

// assignment operators
FIXED32& FIXED32::operator+= (const FIXED32& v){ _ += v._; return *this;	}
FIXED32& FIXED32::operator-= (const FIXED32& v){ _ -= v._; return *this;	}
FIXED32& FIXED32::operator*= (const FIXED32& v){ _ = FIXED32( ((DINT)_ * (DINT)v._) >> FIXED_PRECISION ); return *this;	}
FIXED32& FIXED32::operator/= (const FIXED32& v){ _ = FIXED32( ((DINT)_ / (DINT)v._) << FIXED_PRECISION ); return *this;	}

// binary operators
FIXED32 FIXED32::operator +(const FIXED32& v) const{ return (INT)_ + (INT)v._; };
FIXED32 FIXED32::operator -(const FIXED32& v) const{ return (INT)_ - (INT)v._; };
FIXED32 FIXED32::operator *(const FIXED32& v) const{ return FIXED32( ((DINT)_ * (DINT)v._) >> FIXED_PRECISION ); };
FIXED32 FIXED32::operator /(const FIXED32& v) const{ return FIXED32( ((DINT)_ / (DINT)v._) << FIXED_PRECISION ); };

FIXED32 operator +(const FIXED32& v1, const FIXED32& v2)
{
	FIXED32 t = v1._;
	t._ += v2._;
	return t;
}
FIXED32 operator -(const FIXED32& v1, const FIXED32& v2)
{
	FIXED32 t = v1._;
	t._ -= v2._;
	return t;
}

FIXED32 operator *(const FIXED32& v1, const FIXED32& v2)
{
	FIXED32 t;
	t._ = ((DINT)v1._ * (DINT)v2._) >> FIXED_PRECISION;
	return t;

	//return FIXED32( ((DINT)v1._ * (DINT)v2._) >> FIXED_PRECISION );
}

FIXED32 operator /(const FIXED32& v1, const FIXED32& v2)
{
	FIXED32 t;
	t._ = ((DINT)v1._ / (DINT)v2._) << FIXED_PRECISION;
	return t;
}

FLOAT FIXED32::FloatCut(FLOAT v)
{
	//FIXED32 t = v; FLOAT _ = t; return  _;
	return (FLOAT)(INT(v*65536.0F)>>FIXED_PRECISION);
}





//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
//  Short Int type vector

LCXVEC2s::LCXVEC2s()					{	x=0; y=0;		}
LCXVEC2s::LCXVEC2s(const SHORT* v)		{	if(!v) return;	x=v[0]; y=v[1];	}
LCXVEC2s::LCXVEC2s(const LCXVEC2s* v)	{	if(!v) return;	x=v->x; y=v->y;	}
LCXVEC2s::LCXVEC2s(SHORT X, SHORT Y)	{	x=X; y=Y;		}
LCXVEC2s::LCXVEC2s(const LC_SHORT2& v)  {   x=v.x; y=v.y;	}

LCXVEC2s::LCXVEC2s(const LCXVEC2s& v)   {   x=v.x; y=v.y;	}
LCXVEC2s& LCXVEC2s::operator=(const LCXVEC2s& v)
{
	x=v.x;	y=v.y;
	return *this;
}


// casting
LCXVEC2s::operator SHORT*()				{	return(SHORT*)&x;		}
LCXVEC2s::operator const SHORT*() const	{	return(const SHORT*)&x;	}
SHORT& LCXVEC2s::operator[](int n)		{	return *((&x)+ n);		}

// assignment operators
LCXVEC2s& LCXVEC2s::operator +=(const LCXVEC2s& v)	{	x += v.x; y += v.y;	return *this;	}
LCXVEC2s& LCXVEC2s::operator -=(const LCXVEC2s& v)	{	x -= v.x; y -= v.y;	return *this;	}
LCXVEC2s& LCXVEC2s::operator *=(SHORT v)			{	x *= v;	y *= v;		return *this;	}
LCXVEC2s& LCXVEC2s::operator /=(SHORT v)			{	x /= v;	y /= v;		return *this;	}

// unary operators
LCXVEC2s LCXVEC2s::operator+() const				{	return *this;						}
LCXVEC2s LCXVEC2s::operator-() const				{	return LCXVEC2s(-x, -y);			}

// binary operators
LCXVEC2s LCXVEC2s::operator+(const LCXVEC2s& v)const{	return LCXVEC2s(x + v.x, y + v.y);	}
LCXVEC2s LCXVEC2s::operator-(const LCXVEC2s& v)const{	return LCXVEC2s(x - v.x, y - v.y);	}
LCXVEC2s LCXVEC2s::operator*(SHORT v) const			{	return LCXVEC2s(x * v  , y * v  );	}
LCXVEC2s LCXVEC2s::operator/(SHORT v) const			{ 	return LCXVEC2s(x / v  , y / v  );	}
LCXVEC2s operator *(SHORT f, const LCXVEC2s& v)		{	return LCXVEC2s(f * v.x, f * v.y);	}

INT LCXVEC2s::operator ==(const LCXVEC2s& v) const	{	return x == v.x && y == v.y;		}
INT LCXVEC2s::operator !=(const LCXVEC2s& v) const	{	return x != v.x || y != v.y;		}




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
LCXVEC3s::LCXVEC3s()					{	x=0; y=0; z=0;			}
LCXVEC3s::LCXVEC3s(const SHORT* v)		{	if(!v) return;	x=v[0];	y=v[1];	z=v[2];	}
LCXVEC3s::LCXVEC3s(const LCXVEC3s* v)	{	if(!v) return;	x=v->x;	y=v->y;	z=v->z;	}
LCXVEC3s::LCXVEC3s(SHORT X,SHORT Y,SHORT Z){	x=X; y=Y; z=Z;		}
LCXVEC3s::LCXVEC3s(const LC_SHORT3& v)	{	x=v.x; y=v.y; z=v.z;	}

LCXVEC3s::LCXVEC3s(const LCXVEC3s& v)   {   *this = v;      }
LCXVEC3s& LCXVEC3s::operator=(const LCXVEC3s& v)
{
	x=v.x; y=v.y; z=v.z;
	return *this;
}


// casting
LCXVEC3s::operator SHORT*()				{	return(SHORT*)&x;		}
LCXVEC3s::operator const SHORT*() const	{	return(const SHORT*)&x;	}
SHORT& LCXVEC3s::operator[](int n)		{	return *((&x)+ n);		}

// assignment operators
LCXVEC3s& LCXVEC3s::operator+=(const LCXVEC3s& v)	{	x += v.x; y += v.y; z += v.z; return *this;	}
LCXVEC3s& LCXVEC3s::operator-=(const LCXVEC3s& v)	{	x -= v.x; y -= v.y; z -= v.z; return *this;	}
LCXVEC3s& LCXVEC3s::operator*=(SHORT v)				{	x *= v;	y *= v;	z *= v;		 return *this;	}
LCXVEC3s& LCXVEC3s::operator/=(SHORT v)				{	x /= v;	y /= v;	z /= v;		 return *this;	}

// unary operators
LCXVEC3s LCXVEC3s::operator+() const				{	return *this;								}
LCXVEC3s LCXVEC3s::operator-() const				{	return LCXVEC3s(-x, -y, -z);				}

// binary operators
LCXVEC3s LCXVEC3s::operator+(const LCXVEC3s& v)const{	return LCXVEC3s(x + v.x, y + v.y, z + v.z);	}
LCXVEC3s LCXVEC3s::operator-(const LCXVEC3s& v)const{	return LCXVEC3s(x - v.x, y - v.y, z - v.z);	}
LCXVEC3s LCXVEC3s::operator*(SHORT v) const			{	return LCXVEC3s(x * v  , y * v  , z * v  );	}
LCXVEC3s LCXVEC3s::operator/(SHORT v) const			{ 	return LCXVEC3s(x / v  , y / v  , z / v  );	}
LCXVEC3s operator *(SHORT f, const LCXVEC3s& v)		{	return LCXVEC3s(f * v.x, f * v.y, f * v.z);	}

INT LCXVEC3s::operator ==(const LCXVEC3s& v) const	{	return x == v.x && y == v.y && z == v.z;	}
INT LCXVEC3s::operator !=(const LCXVEC3s& v) const	{	return x != v.x || y != v.y || z != v.z;	}





//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// Colors Byte(RGBA)

LCXCOLORB::LCXCOLORB()					{ r=0xFF; g=0xFF; b=0xFF; a=0xFF;	}
LCXCOLORB::LCXCOLORB(const BYTE* v)		{ r=v[0]; g=v[1]; b=v[2]; a=v[3];	}
LCXCOLORB::LCXCOLORB(const FLOAT* v/*r[0],g[1],b[2],a[3]*/)
{
	r = BYTE(v[0] * 255.0F);
	g = BYTE(v[1] * 255.0F);
	b = BYTE(v[2] * 255.0F);
	a = BYTE(v[3] * 255.0F);
}

LCXCOLORB::LCXCOLORB(const LC_COLORF* v)
{
	r = BYTE(v->r * 255.0F);
	g = BYTE(v->g * 255.0F);
	b = BYTE(v->b * 255.0F);
	a = BYTE(v->a * 255.0F);
}

LCXCOLORB::LCXCOLORB(const LCXCOLORB& v){   *this = v;      }
LCXCOLORB& LCXCOLORB::operator=(const LCXCOLORB& v)
{
	r=v.r;  g=v.g;  b=v.b;  a=v.a;
	return *this;
}



LCXCOLORB::LCXCOLORB(BYTE R,BYTE G,BYTE B,BYTE A){ r=R; g=G; b=B; a=A;			}
LCXCOLORB::LCXCOLORB(COLOR32 v)			{	a=(BYTE)(v>>24); r=(BYTE)(v>>16); g=(BYTE)(v>>8); b=(BYTE)(v>>0);	}
LCXCOLORB::LCXCOLORB(const LC_COLORB& v){	r=v.r; g=v.g; b=v.b; a=v.a;			}

// casting
LCXCOLORB::operator COLOR32 () const	{	return (a<<24) | (r<<16) | (g<<8) | b;	}
LCXCOLORB::operator BYTE* ()			{	return (BYTE*) &r;						}
LCXCOLORB::operator const BYTE* () const{	return (const BYTE*) &r;				}
LCXCOLORB::operator LCXCOLOR () const	{	return LCXCOLOR( FLOAT(r)/255.0F, FLOAT(g)/255.0F, FLOAT(b)/255.0F, FLOAT(a)/255.0F);	}
BYTE& LCXCOLORB::operator[](int n)		{	return *((&r)+ n);						}

// assignment operators
LCXCOLORB& LCXCOLORB::operator+= (const LCXCOLORB& v){ r += v.r; g += v.g; b += v.b; a += v.a; return *this;	}
LCXCOLORB& LCXCOLORB::operator-= (const LCXCOLORB& v){ r -= v.r; g -= v.g; b -= v.b; a -= v.a; return *this;	}
LCXCOLORB& LCXCOLORB::operator*= (const LCXCOLORB& v){ r *= v.r; g *= v.g; b *= v.b; a *= v.a; return *this;	}
LCXCOLORB& LCXCOLORB::operator*= (BYTE f)			{  r *= f;   g *= f;   b *= f;   a *= f;   return *this;	}
LCXCOLORB& LCXCOLORB::operator/= (BYTE f)			{  r /= f;   g /= f;   b /= f;   a /= f;   return *this;	}

// unary operators
LCXCOLORB LCXCOLORB::operator+ () const				{ return *this;						}
LCXCOLORB LCXCOLORB::operator- () const				{ return LCXCOLORB(-r, -g, -b, -a);	}

// binary operators
LCXCOLORB LCXCOLORB::operator+(const LCXCOLORB& v)const{return LCXCOLORB(r + v.r, g + v.g, b + v.b, a + v.a);	}
LCXCOLORB LCXCOLORB::operator-(const LCXCOLORB& v)const{return LCXCOLORB(r - v.r, g - v.g, b - v.b, a - v.a);	}
LCXCOLORB LCXCOLORB::operator*(const LCXCOLORB& v)const{return LCXCOLORB(r * v.r, g * v.g, b * v.b, a * v.a);	}
LCXCOLORB LCXCOLORB::operator*(BYTE f) const		{	return LCXCOLORB(r * f  , g * f  , b * f  , a * f  );	}
LCXCOLORB LCXCOLORB::operator/(BYTE f) const		{	return LCXCOLORB(r / f  , g / f  , b / f  , a / f  );	}
LCXCOLORB operator* (BYTE f, const LCXCOLORB& v)	{	return LCXCOLORB(f * v.r, f * v.g, f * v.b, f * v.a);	}

INT LCXCOLORB::operator == (const LCXCOLORB& v) const{	return r == v.r && g == v.g && b == v.b && a == v.a;	}
INT LCXCOLORB::operator != (const LCXCOLORB& v) const{	return r != v.r || g != v.g || b != v.b || a != v.a;	}




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// Colors Byte(RGB)

LCXCOLORB3::LCXCOLORB3()					{ r=0xFF; g=0xFF; b=0xFF;	}
LCXCOLORB3::LCXCOLORB3(const LCXCOLORB& v)	{ r=v.r;  g=v.g;  b=v.b;	}
LCXCOLORB3::LCXCOLORB3(const BYTE* v)		{ r=v[0]; g=v[1]; b=v[2];	}
LCXCOLORB3::LCXCOLORB3(const FLOAT* v/*r[0],g[1],b[2],a[3]*/)
{
	r = BYTE( lc_clamp( v[0] * 255.0F, 0.0F, 255.0F) );
	g = BYTE( lc_clamp( v[1] * 255.0F, 0.0F, 255.0F) );
	b = BYTE( lc_clamp( v[2] * 255.0F, 0.0F, 255.0F) );
}

LCXCOLORB3::LCXCOLORB3(const LCXCOLOR& v)
{
	r = BYTE( lc_clamp( v[0] * 255.0F, 0.0F, 255.0F) );
	g = BYTE( lc_clamp( v[1] * 255.0F, 0.0F, 255.0F) );
	b = BYTE( lc_clamp( v[2] * 255.0F, 0.0F, 255.0F) );
}

LCXCOLORB3::LCXCOLORB3(const LC_COLORF* v)
{
	r = BYTE( lc_clamp( v->r * 255.0F, 0.0F, 255.0F) );
	g = BYTE( lc_clamp( v->g * 255.0F, 0.0F, 255.0F) );
	b = BYTE( lc_clamp( v->b * 255.0F, 0.0F, 255.0F) );
}

LCXCOLORB3::LCXCOLORB3(const LCXCOLORB3& v)   {   *this = v;      }
LCXCOLORB3& LCXCOLORB3::operator=(const LCXCOLORB3& v)
{
	r=v.r;	g=v.g;	b=v.b;
	return *this;
}

LCXCOLORB3::LCXCOLORB3(BYTE R,BYTE G,BYTE B){ r=R; g=G; b=B;			}
LCXCOLORB3::LCXCOLORB3(COLOR32 v)			{ r=(BYTE)(v>>16); g=(BYTE)(v>>8); b=(BYTE)(v>>0);	}
LCXCOLORB3::LCXCOLORB3(const LC_COLORB& v)	{ r=v.r; g=v.g; b=v.b;		}

// casting
LCXCOLORB3::operator COLOR32 () const	{	return 0xFF000000 | (r<<16) | (g<<8) | b;	}
LCXCOLORB3::operator BYTE* ()			{	return (BYTE*) &r;						}
LCXCOLORB3::operator const BYTE* () const{	return (const BYTE*) &r;				}
LCXCOLORB3::operator LCXCOLORB() const	{	return LCXCOLORB(r, g, b, 255);			}
LCXCOLORB3::operator LCXCOLOR () const	{	return LCXCOLOR( FLOAT(r)/255.0F, FLOAT(g)/255.0F, FLOAT(b)/255.0F, 1.0F);	}
BYTE& LCXCOLORB3::operator[](int n)		{	return *((&r)+ n);						}

// assignment operators
LCXCOLORB3& LCXCOLORB3::operator+= (const LCXCOLORB3& v){ r += v.r; g += v.g; b += v.b; return *this;	}
LCXCOLORB3& LCXCOLORB3::operator-= (const LCXCOLORB3& v){ r -= v.r; g -= v.g; b -= v.b; return *this;	}
LCXCOLORB3& LCXCOLORB3::operator*= (const LCXCOLORB3& v){ r *= v.r; g *= v.g; b *= v.b; return *this;	}
LCXCOLORB3& LCXCOLORB3::operator*= (BYTE f)				{ r *= f;   g *= f;   b *= f;   return *this;	}
LCXCOLORB3& LCXCOLORB3::operator/= (BYTE f)				{ r /= f;   g /= f;   b /= f;   return *this;	}

// unary operators
LCXCOLORB3 LCXCOLORB3::operator+ () const				{	return *this;					}
LCXCOLORB3 LCXCOLORB3::operator- () const				{	return LCXCOLORB3(-r, -g, -b);	}

// binary operators
LCXCOLORB3 LCXCOLORB3::operator+(const LCXCOLORB3& v)const{return LCXCOLORB3(r + v.r, g + v.g, b + v.b);	}
LCXCOLORB3 LCXCOLORB3::operator-(const LCXCOLORB3& v)const{return LCXCOLORB3(r - v.r, g - v.g, b - v.b);	}
LCXCOLORB3 LCXCOLORB3::operator*(const LCXCOLORB3& v)const{return LCXCOLORB3(r * v.r, g * v.g, b * v.b);	}
LCXCOLORB3 LCXCOLORB3::operator*(BYTE f) const			{  return LCXCOLORB3(r * f  , g * f  , b * f  );	}
LCXCOLORB3 LCXCOLORB3::operator/(BYTE f) const			{  return LCXCOLORB3(r / f  , g / f  , b / f  );	}
LCXCOLORB3 operator* (BYTE f, const LCXCOLORB3& v)		{  return LCXCOLORB3(f * v.r, f * v.g, f * v.b);	}

INT LCXCOLORB3::operator == (const LCXCOLORB3& v) const{   return r == v.r && g == v.g && b == v.b;	}
INT LCXCOLORB3::operator != (const LCXCOLORB3& v) const{   return r != v.r || g != v.g || b != v.b;	}





//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// Colors Byte(RGB)

LCXCOLORW4::LCXCOLORW4()
{
	r=g=b=a=0;
}

LCXCOLORW4::LCXCOLORW4(INT R, INT G, INT B, INT A)
{
	r=R; g=G; b=B; a=A;
}

LCXCOLORW4::LCXCOLORW4(COLOR16 rgba)
{
	d = rgba;
}

LCXCOLORW4::LCXCOLORW4(COLOR32 argb)
{
	LCXCOLORB	t(argb);
	r = t.r >>1;
	g = t.g >>1;
	b = t.b >>1;
	a = t.a >>1;
}


LCXCOLORW4::LCXCOLORW4(const LCXCOLORB& v)
{
	r = v.r >>1;
	g = v.g >>1;
	b = v.b >>1;
	a = v.a >>1;
}

LCXCOLORW4::LCXCOLORW4(const LCXCOLORW4& v)   {   *this = v;      }
LCXCOLORW4& LCXCOLORW4::operator=(const LCXCOLORW4& v)
{
	r = v.r;
	g = v.g;
	b = v.b;
	a = v.a;
	return *this;
}



LCXCOLORW4::operator COLOR16() const
{
	return d;
}

LCXCOLORW4::operator COLOR32() const
{
	INT R = r<<1;
	INT G = r<<1;
	INT B = r<<1;
	INT A = r<<1;
	INT D = (a<<24) | (r<<16) | (g<<8) | b;
	return D;
}

LCXCOLORW4::operator LCXCOLORB() const
{
	INT R = r<<1;
	INT G = r<<1;
	INT B = r<<1;
	INT A = r<<1;
	LCXCOLORB D(R,G,B,A);
	return D;
}

LCXCOLORW4::operator LCXCOLOR() const
{
	INT R = r<<1;
	INT G = r<<1;
	INT B = r<<1;
	INT A = r<<1;
	LCXCOLOR D(FLOAT(R)/255.0F, FLOAT(G)/255.0F, FLOAT(B)/255.0F, FLOAT(A)/255.0F);

	return D;
}

void LCXCOLORW4::R(INT v){ r= v; }
void LCXCOLORW4::G(INT v){ g= v; }
void LCXCOLORW4::B(INT v){ b= v; }
void LCXCOLORW4::A(INT v){ a= v; }

void LCXCOLORW4::ARGB(COLOR16 argb)
{
	a = (argb >> 12) & 0xF;
	r = (argb >>  8) & 0xF;
	g = (argb >>  4) & 0xF;
	b = (argb >>  0) & 0xF;
}

void LCXCOLORW4::RGBA(COLOR16 argb)
{
	r = (argb >> 12) & 0xF;
	g = (argb >>  8) & 0xF;
	b = (argb >>  4) & 0xF;
	a = (argb >>  0) & 0xF;
}

BYTE LCXCOLORW4::R(){ return r; }
BYTE LCXCOLORW4::G(){ return g; }
BYTE LCXCOLORW4::B(){ return b; }
BYTE LCXCOLORW4::A(){ return a; }




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// INT type vector

LCXVEC2i::LCXVEC2i()					{	x=0; y=0;					}
LCXVEC2i::LCXVEC2i(const INT* v)	{	if(!v) return; x=v[0]; y=v[1];	}
LCXVEC2i::LCXVEC2i(const LCXVEC2i* v){	if(!v) return; x=v->x; y=v->y;	}
LCXVEC2i::LCXVEC2i(INT X, INT Y)		{	x=X; y=Y;					}
LCXVEC2i::LCXVEC2i(FLOAT X,FLOAT Y)		{	x= INT(X);    y=INT(Y);		}
LCXVEC2i::LCXVEC2i(const FLOAT* v)		{	x= INT(v[0]); y=INT(v[1]);	}
LCXVEC2i::LCXVEC2i(const LC_INT2& v)	{	x=v.x; y=v.y;				}

LCXVEC2i::LCXVEC2i(const LCXVEC2i& v)   {   *this = v;      }
LCXVEC2i& LCXVEC2i::operator=(const LCXVEC2i& v)
{
	x=v.x; y=v.y;
	return *this;
}


// casting
LCXVEC2i::operator INT*()			{	return(INT*)&x;			}
LCXVEC2i::operator const INT*() const{	return(const INT*)&x;	}
INT& LCXVEC2i::operator[](int n)	{	return *((&x)+ n);		}

// assignment operators
LCXVEC2i& LCXVEC2i::operator +=(const LCXVEC2i& v)	{	x += v.x; y += v.y;	return *this;	}
LCXVEC2i& LCXVEC2i::operator -=(const LCXVEC2i& v)	{	x -= v.x; y -= v.y;	return *this;	}
LCXVEC2i& LCXVEC2i::operator *=(INT v)				{	x *= v;	y *= v;		return *this;	}
LCXVEC2i& LCXVEC2i::operator /=(INT v)				{	x /= v;	y /= v;		return *this;	}

// unary operators
LCXVEC2i LCXVEC2i::operator +() const				{	return *this;						}
LCXVEC2i LCXVEC2i::operator -() const				{	return LCXVEC2i(-x, -y);			}

// binary operators
LCXVEC2i LCXVEC2i::operator +(const LCXVEC2i& v) const{	return LCXVEC2i(x + v.x, y + v.y);	}
LCXVEC2i LCXVEC2i::operator -(const LCXVEC2i& v) const{	return LCXVEC2i(x - v.x, y - v.y);	}
LCXVEC2i LCXVEC2i::operator *(INT v) const			{	return LCXVEC2i(x * v  , y * v  );	}
LCXVEC2i LCXVEC2i::operator /(INT v) const			{	return LCXVEC2i(x / v  , y / v  );	}
LCXVEC2i operator *(INT f, const LCXVEC2i& v)		{	return LCXVEC2i(f * v.x, f * v.y);	}

INT LCXVEC2i::operator ==(const LCXVEC2i& v) const	{	return x == v.x && y == v.y;		}
INT LCXVEC2i::operator !=(const LCXVEC2i& v) const	{	return x != v.x || y != v.y;		}




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8

LCXVEC3i::LCXVEC3i()					{	x=0; y=0; z=0;							}
LCXVEC3i::LCXVEC3i(const INT* v)		{	if(!v) return;	x=v[0]; y=v[1]; z=v[2];	}
LCXVEC3i::LCXVEC3i(const LCXVEC3i* v)	{	if(!v) return;	x=v->x; y=v->y; z=v->z;	}
LCXVEC3i::LCXVEC3i(INT X,INT Y,INT Z)	{	x=X; y=Y; z=Z;							}
LCXVEC3i::LCXVEC3i(FLOAT X,FLOAT Y,FLOAT Z){x=INT(X); y=INT(Y); z=INT(Z);			}
LCXVEC3i::LCXVEC3i(const FLOAT* v)		{	x=INT(v[0]); y=INT(v[1]); z=INT(v[2]);	}
LCXVEC3i::LCXVEC3i(const LC_INT3& v)	{	x=v.x; y=v.y; z=v.z;				}
LCXVEC3i::LCXVEC3i(const LCXVEC2i& v,INT _z){	x=v.x; y=v.y; z=_z;				}

LCXVEC3i::LCXVEC3i(const LCXVEC3i& v)   {   *this = v;      }
LCXVEC3i& LCXVEC3i::operator=(const LCXVEC3i& v)
{
	x=v.x; y=v.y; z=v.z;
	return *this;
}


// casting
LCXVEC3i::operator INT*()					{	return(INT*)&x;								}
LCXVEC3i::operator const INT*() const		{	return(const INT*)&x;						}
INT& LCXVEC3i::operator[](int n)			{	return *((&x)+ n);							}

// assignment operators
LCXVEC3i& LCXVEC3i::operator +=(const LCXVEC3i& v)	{	x += v.x; y += v.y; z += v.z;	return *this;	}
LCXVEC3i& LCXVEC3i::operator -=(const LCXVEC3i& v)	{	x -= v.x; y -= v.y; z -= v.z;	return *this;	}
LCXVEC3i& LCXVEC3i::operator *=(INT v)				{	x *= v;	y *= v;	z *= v;			return *this;	}
LCXVEC3i& LCXVEC3i::operator /=(INT v)				{	x /= v;	y /= v;	z /= v;			return *this;	}

// unary operators
LCXVEC3i LCXVEC3i::operator +() const				{	return *this;									}
LCXVEC3i LCXVEC3i::operator -() const				{	return LCXVEC3i(-x, -y, -z);					}

// binary operators
LCXVEC3i LCXVEC3i::operator +(const LCXVEC3i& v) const{	return LCXVEC3i(x + v.x, y + v.y, z + v.z);		}
LCXVEC3i LCXVEC3i::operator -(const LCXVEC3i& v) const{	return LCXVEC3i(x - v.x, y - v.y, z - v.z);		}
LCXVEC3i LCXVEC3i::operator *(INT v) const			{	return LCXVEC3i(x * v  , y * v  , z * v  );		}
LCXVEC3i LCXVEC3i::operator /(INT v) const			{	return LCXVEC3i(x / v  , y / v  , z / v  );		}
LCXVEC3i operator *(INT f, const LCXVEC3i& v)		{	return LCXVEC3i(f * v.x, f * v.y, f * v.z);		}

INT LCXVEC3i::operator ==(const LCXVEC3i& v) const	{	return x == v.x && y == v.y && z == v.z;		}
INT LCXVEC3i::operator !=(const LCXVEC3i& v) const	{	return x != v.x || y != v.y || z != v.z;		}




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8

LCXVEC4i::LCXVEC4i()						{	x=0; y=0; z=0; w=0;								}
LCXVEC4i::LCXVEC4i(const INT* v)			{	if(!v) return;	x=v[0]; y=v[1]; z=v[2]; w=v[3];	}
LCXVEC4i::LCXVEC4i(const LCXVEC4i* v)		{	if(!v) return;	x=v->x; y=v->y; z=v->z; w=v->w;	}
LCXVEC4i::LCXVEC4i(INT X,INT Y,INT Z, INT W){	x=X; y=Y; z=Z; w=W;								}
LCXVEC4i::LCXVEC4i(FLOAT X,FLOAT Y,FLOAT Z,FLOAT W){x=INT(X);y=INT(Y);z=INT(Z);w=INT(W);		}
LCXVEC4i::LCXVEC4i(const LC_INT4& v)		{	x=v.x; y=v.y; z=v.z; w=v.w;						}
LCXVEC4i::LCXVEC4i(const LCXVEC3i& v, INT _w){	x=v.x; y=v.y; z=v.z; w=_w;						}

LCXVEC4i::LCXVEC4i(const LCXVEC4i& v)   {   *this = v;      }
LCXVEC4i& LCXVEC4i::operator=(const LCXVEC4i& v)
{
	x=v.x; y=v.y; z=v.z; w=v.w;
	return *this;
}

// casting
LCXVEC4i::operator INT*()					{	return(INT*)&x;										}
LCXVEC4i::operator const INT*() const		{	return(const INT*)&x;								}
INT& LCXVEC4i::operator[](int n)			{	return *((&x)+ n);									}

// assignment operators
LCXVEC4i& LCXVEC4i::operator+=(const LCXVEC4i& v){	x+=v.x; y+=v.y; z+=v.z; w+=v.w;	return *this;	}
LCXVEC4i& LCXVEC4i::operator-=(const LCXVEC4i& v){	x-=v.x; y-=v.y; z-=v.z; w-=v.w;	return *this;	}
LCXVEC4i& LCXVEC4i::operator*=(INT v)			{	x *=v;	y *=v;	z *=v;	w *=v;	return *this;	}
LCXVEC4i& LCXVEC4i::operator/=(INT v)			{	x /=v;	y /=v;	z /=v;	w /=v;	return *this;	}

// unary operators
LCXVEC4i LCXVEC4i::operator+() const			{	return *this;									}
LCXVEC4i LCXVEC4i::operator-() const			{	return LCXVEC4i(-x, -y, -z, -w);				}

// binary operators
LCXVEC4i LCXVEC4i::operator+(const LCXVEC4i& v)const{ return LCXVEC4i(x+v.x, y+v.y, z+v.z, w+v.w);	}
LCXVEC4i LCXVEC4i::operator-(const LCXVEC4i& v)const{ return LCXVEC4i(x-v.x, y-v.y, z-v.z, w-v.w);	}
LCXVEC4i LCXVEC4i::operator*(INT v) const			{ return LCXVEC4i(x * v, y * v, z * v, w * v);	}
LCXVEC4i LCXVEC4i::operator/(INT v) const			{ return LCXVEC4i(x / v, y / v, z / v, w / v);	}
LCXVEC4i operator *(INT f, const LCXVEC4i& v)		{ return LCXVEC4i(f*v.x, f*v.y, f*v.z, f*v.w);	}

INT LCXVEC4i::operator ==(const LCXVEC4i& v) const	{ return x == v.x && y == v.y && z == v.z;		}
INT LCXVEC4i::operator !=(const LCXVEC4i& v) const	{ return x != v.x || y != v.y || z != v.z;		}




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// FLOAT type vector

LCXVECTOR2::LCXVECTOR2()					{ x=0.0F; y=0.0F;					}
LCXVECTOR2::LCXVECTOR2(const FLOAT* v)		{ if(!v) return; x=v[0]; y=v[1];	}
LCXVECTOR2::LCXVECTOR2(const LCXVECTOR2* v)	{ if(!v) return; x=v->x; y=v->y;	}
LCXVECTOR2::LCXVECTOR2(FLOAT X, FLOAT Y)	{ x=X; y=Y;							}
LCXVECTOR2::LCXVECTOR2(const LC_FLOAT2& v)	{ x=v.x; y=v.y;						}

LCXVECTOR2::LCXVECTOR2(const LCXVECTOR2& v) {   *this = v;      }
LCXVECTOR2& LCXVECTOR2::operator=(const LCXVECTOR2& v)
{
	x=v.x; y=v.y;
	return *this;
}


// casting
LCXVECTOR2::operator FLOAT*()				{	return(FLOAT*)&x;		}
LCXVECTOR2::operator const FLOAT*() const	{	return(const FLOAT*)&x;	}
FLOAT& LCXVECTOR2::operator[](int n)		{	return *((&x)+ n);		}

// assignment operators
LCXVECTOR2& LCXVECTOR2::operator +=(const LCXVECTOR2& v){ x += v.x;	y += v.y; return *this;		}
LCXVECTOR2& LCXVECTOR2::operator -=(const LCXVECTOR2& v){ x -= v.x;	y -= v.y; return *this;		}
LCXVECTOR2& LCXVECTOR2::operator *=(FLOAT v)			{ x *= v;	y *= v;   return *this;		}
LCXVECTOR2& LCXVECTOR2::operator /=(FLOAT v)			{v=1.0F/v; x*= v;  y*= v; return *this;	}
LCXVECTOR2& LCXVECTOR2::operator = (const LCXVECTOR3& v){ x= v.x; y= v.y; return *this;	}

// unary operators
LCXVECTOR2 LCXVECTOR2::operator +() const	{	return *this;				}
LCXVECTOR2 LCXVECTOR2::operator -() const	{	return LCXVECTOR2(-x, -y);	}

// binary operators
LCXVECTOR2 LCXVECTOR2::operator+(const LCXVECTOR2& v)const{	return LCXVECTOR2(x + v.x, y + v.y);	}
LCXVECTOR2 LCXVECTOR2::operator-(const LCXVECTOR2& v)const{	return LCXVECTOR2(x - v.x, y - v.y);	}
LCXVECTOR2 LCXVECTOR2::operator*(FLOAT v) const			  {	return LCXVECTOR2(x * v  , y * v  );	}
LCXVECTOR2 LCXVECTOR2::operator/(FLOAT v) const	{ v=1.0F/v;	return LCXVECTOR2(x * v  , y * v  );	}
LCXVECTOR2 operator *(FLOAT f, const LCXVECTOR2& v)	{		return LCXVECTOR2(f * v.x, f * v.y);	}

//INT LCXVECTOR2::operator ==(const LCXVECTOR2& v) const {	return x == v.x && y == v.y;			}
//INT LCXVECTOR2::operator !=(const LCXVECTOR2& v) const {	return x != v.x || y != v.y;			}

INT LCXVECTOR2::operator ==(const LCXVECTOR2& v)const{ return LCXIsEqual(x, v.x) && LCXIsEqual(y, v.y);	}
INT LCXVECTOR2::operator !=(const LCXVECTOR2& v)const{ return LCXIsNotEqual(x,v.x) || LCXIsNotEqual(y,v.y);}


// Dot Product
FLOAT LCXVECTOR2::operator *(const LCXVECTOR2& v)			{	return x * v.x + y * v.y;			}
FLOAT operator *(const LCXVECTOR2& v1, const LCXVECTOR2& v2){	return v1.x * v2.x + v1.y * v2.y;	}

// Cross Product
FLOAT LCXVECTOR2::operator ^(const LCXVECTOR2& v)			{	return (   x * v.y  -    y * v.x );	}
FLOAT operator ^(const LCXVECTOR2& v1, const LCXVECTOR2& v2){	return (v1.x * v2.y - v1.y * v2.x);	}


// Transform: vector * Matrix
LCXVECTOR2 LCXVECTOR2::operator *(const LCXMATRIX3X3& t)
{
	FLOAT X = x * t._11 + y * t._21 + t._31;
	FLOAT Y = x * t._12 + y * t._22 + t._32;
	FLOAT W = x * t._13 + y * t._23 + t._33;

	W = 1.0F/W;
	return LCXVECTOR2(X * W, Y * W);
}

// Transform: Matrix * vector;
LCXVECTOR2 operator *(const LCXMATRIX3X3& t, const LCXVECTOR2& v)
{
	FLOAT X = v.x * t._11 + v.y * t._12 + t._13;
	FLOAT Y = v.x * t._21 + v.y * t._22 + t._23;
	FLOAT W = v.x * t._31 + v.y * t._32 + t._33;

	W = 1.0F/W;
	return LCXVECTOR2(X * W, Y * W);
}


FLOAT LCXVECTOR2::Length()			{	return (FLOAT)sqrt(x*x + y*y);		}
FLOAT LCXVECTOR2::LengthSq()		{	return(x*x + y*y);					}

LCXVECTOR2 LCXVECTOR2::Normalize()
{
	FLOAT l = (FLOAT)sqrt(x*x + y*y);
	if(0.0F == l)
		return *this;

	l = 1.0F/l;
	x *= l;	y *= l;
	return *this;
}


LCXVECTOR2 LCXVECTOR2::Normalize(const LCXVECTOR2* v)
{
	*this = v;
	FLOAT l = (FLOAT)sqrt(x*x + y*y);
	if(0.0F == l)
		return *this;

	l = 1.0F/l;
	x *= l;	y *= l;
	return *this;
}


FLOAT LCXVECTOR2::Cross(const LCXVECTOR2* v1, const LCXVECTOR2* v2)
{
	return (v1->x * v2->y  -  v1->y * v2->x);
}


LCXVECTOR2& LCXVECTOR2::Negative()
{
	x = -x;
	y = -y;
	return *this;
}


LCXVECTOR2& LCXVECTOR2::Lerp(const LCXVECTOR2* v1, const LCXVECTOR2* v2, FLOAT t) // this = (1-t)*v1 + t* v2
{
	FLOAT k = 1.0F - t;
	x = k * v1->x + t * v2->x;
	y = k * v1->y + t * v2->y;
	return *this;
}

LCXVECTOR2&	LCXVECTOR2::Multiple(const LCXVECTOR2& v)
{
	x *= v.x; y*=v.y;
	return *this;
}

LCXVECTOR2& LCXVECTOR2::Zero()  { x=0.0F; y=0.0F; return *this;}
LCXVECTOR2& LCXVECTOR2::One ()  { x=1.0F; y=1.0F; return *this;}
LCXVECTOR2& LCXVECTOR2::AxisX() { x=1.0F; y=0.0F; return *this;}
LCXVECTOR2& LCXVECTOR2::AxisY() { x=0.0F; y=1.0F; return *this;}

const LCXVECTOR2 LCXVECTOR2::ZERO = LCXVECTOR2(0.0F, 0.0F);
const LCXVECTOR2 LCXVECTOR2::ONE  = LCXVECTOR2(1.0F, 1.0F);
const LCXVECTOR2 LCXVECTOR2::AXISX= LCXVECTOR2(1.0F, 0.0F);
const LCXVECTOR2 LCXVECTOR2::AXISY= LCXVECTOR2(0.0F, 1.0F);



//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8

LCXVECTOR3::LCXVECTOR3()					{	x=0.0F; y=0.0F; z=0.0F;		}
LCXVECTOR3::LCXVECTOR3(const FLOAT* v)		{ if(!v) return; x=v[0]; y=v[1]; z=v[2]; }
LCXVECTOR3::LCXVECTOR3(const LCXVECTOR3* v)	{ if(!v) return; x=v->x; y=v->y; z=v->z; }
LCXVECTOR3::LCXVECTOR3(FLOAT X, FLOAT Y, FLOAT Z){	x=X;   y=Y;   z=Z;		}
LCXVECTOR3::LCXVECTOR3(const LC_FLOAT3& v)		{	x=v.x; y=v.y; z=v.z;	}
LCXVECTOR3::LCXVECTOR3(const LCXVECTOR2& v,FLOAT _z){x=v.x; y=v.y; z= _z;	}

LCXVECTOR3::LCXVECTOR3(const LCXVECTOR3& v) {   *this = v;      }
LCXVECTOR3& LCXVECTOR3::operator=(const LCXVECTOR3& v)
{
	x=v.x; y=v.y; z=v.z;
	return *this;
}

// casting
LCXVECTOR3::operator FLOAT*()				{	return(FLOAT*)&x;			}
LCXVECTOR3::operator const FLOAT*() const	{	return(const FLOAT*)&x;		}
FLOAT& LCXVECTOR3::operator[](int n)		{	return *((&x)+ n);			}

// assignment operators
LCXVECTOR3& LCXVECTOR3::operator +=(const LCXVECTOR3& v){ x +=v.x; y +=v.y; z +=v.z; return *this;	}
LCXVECTOR3& LCXVECTOR3::operator -=(const LCXVECTOR3& v){ x -=v.x; y -=v.y; z -=v.z; return *this;	}
LCXVECTOR3& LCXVECTOR3::operator *=(FLOAT v)			{ x *= v ; y *= v ; z *= v ; return *this;	}
LCXVECTOR3& LCXVECTOR3::operator /=(FLOAT v){ v = 1.0F/v; x *= v ; y *= v ; z *= v ; return *this;	}

LCXVECTOR3& LCXVECTOR3::operator +=(const LCXVECTOR4& v){ x +=v.x; y +=v.y; z +=v.z; return *this;	}
LCXVECTOR3& LCXVECTOR3::operator -=(const LCXVECTOR4& v){ x -=v.x; y -=v.y; z -=v.z; return *this;	}


// unary operators
LCXVECTOR3 LCXVECTOR3::operator +() const	{	return *this;					}
LCXVECTOR3 LCXVECTOR3::operator -() const	{	return LCXVECTOR3(-x, -y, -z);	}

// binary operators
LCXVECTOR3 LCXVECTOR3::operator +(const LCXVECTOR3& v) const {	return LCXVECTOR3(x + v.x, y + v.y, z + v.z);	}
LCXVECTOR3 LCXVECTOR3::operator -(const LCXVECTOR3& v) const {	return LCXVECTOR3(x - v.x, y - v.y, z - v.z);	}
LCXVECTOR3 LCXVECTOR3::operator *(FLOAT v) const			 {	return LCXVECTOR3(x * v  , y * v  , z * v  );	}
LCXVECTOR3 LCXVECTOR3::operator /(FLOAT v) const	{ v=1.0F/v;	return LCXVECTOR3(x * v  , y * v  , z * v  );	}
LCXVECTOR3 operator *(FLOAT f, const LCXVECTOR3& v)	{			return LCXVECTOR3(f * v.x, f * v.y, f * v.z);	}

//INT LCXVECTOR3::operator ==(const LCXVECTOR3& v) const {	return x == v.x && y == v.y && z == v.z;	}
//INT LCXVECTOR3::operator !=(const LCXVECTOR3& v) const {	return x != v.x || y != v.y || z != v.z;	}

INT LCXVECTOR3::operator ==(const LCXVECTOR3& v) const { return LCXIsEqual(x, v.x) && LCXIsEqual(y, v.y) && LCXIsEqual(z, v.z);		}
INT LCXVECTOR3::operator !=(const LCXVECTOR3& v) const { return LCXIsNotEqual(x,v.x) || LCXIsNotEqual(y,v.y) || LCXIsNotEqual(z,v.z);}

// Dot Product
FLOAT LCXVECTOR3::operator *(const LCXVECTOR3& v)			{	return x * v.x + y * v.y + z * v.z;				}
FLOAT operator *(const LCXVECTOR3& v1, const LCXVECTOR3& v2){	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;	}

// Cross Product
LCXVECTOR3 LCXVECTOR3::operator ^(const LCXVECTOR3& v)
{
	return LCXVECTOR3(y * v.z  -  z * v.y
					, z * v.x  -  x * v.z
					, x * v.y  -  y * v.x	);
}


// Cross Product
LCXVECTOR3 operator ^(const LCXVECTOR3& v1, const LCXVECTOR3& v2)
{
	return LCXVECTOR3(v1.y * v2.z  -  v1.z * v2.y
					, v1.z * v2.x  -  v1.x * v2.z
					, v1.x * v2.y  -  v1.y * v2.x	);
}


// Transform: vector * Matrix
LCXVECTOR3 LCXVECTOR3::operator *(const LCXMATRIX& t)
{
	FLOAT X = x * t._11 + y * t._21 + z * t._31 + t._41;
	FLOAT Y = x * t._12 + y * t._22 + z * t._32 + t._42;
	FLOAT Z = x * t._13 + y * t._23 + z * t._33 + t._43;
	FLOAT W = x * t._14 + y * t._24 + z * t._34 + t._44;

	W = 1.0F/W;
	return LCXVECTOR3(X * W, Y * W, Z* W);
}

// Transform: Matrix * vector;
LCXVECTOR3 operator *(const LCXMATRIX& t, const LCXVECTOR3& v)
{
	FLOAT X = v.x * t._11 + v.y * t._12 + v.z * t._13 + t._14;
	FLOAT Y = v.x * t._21 + v.y * t._22 + v.z * t._23 + t._24;
	FLOAT Z = v.x * t._31 + v.y * t._32 + v.z * t._33 + t._34;
	FLOAT W = v.x * t._41 + v.y * t._42 + v.z * t._43 + t._44;

	W = 1.0F/W;
	return LCXVECTOR3(X * W, Y * W, Z* W);
}


FLOAT LCXVECTOR3::Length()	{	return (FLOAT)sqrt(x*x + y*y + z*z);	}
FLOAT LCXVECTOR3::LengthSq(){	return(x*x + y*y + z*z);				}

LCXVECTOR3 LCXVECTOR3::Normalize()
{
	FLOAT l = (FLOAT)sqrt(x*x + y*y + z*z);
	if(0.0F == l)
		return *this;

	l = 1.0F/l;
	x *= l;	y *= l;	z *= l;
	return *this;
}


LCXVECTOR3 LCXVECTOR3::Normalize(const LCXVECTOR3* v)
{
	*this = v;
	FLOAT l = (FLOAT)sqrt(x*x + y*y + z*z);
	if(l == 0)
		return *this;

	l = 1.0F/l;
	x *= l;	y *= l;	z *= l;
	return *this;
}


LCXVECTOR3 LCXVECTOR3::Cross(const LCXVECTOR3* v1, const LCXVECTOR3* v2)
{
	x  = (v1->y * v2->z  -  v1->z * v2->y);
	y  = (v1->z * v2->x  -  v1->x * v2->z);
	z  = (v1->x * v2->y  -  v1->y * v2->x);
	return *this;
}


LCXVECTOR3& LCXVECTOR3::Negative()
{
	x = -x;
	y = -y;
	z = -z;
	return *this;
}


LCXVECTOR3& LCXVECTOR3::Lerp(const LCXVECTOR3* v1, const LCXVECTOR3* v2, FLOAT t) // this = (1-t)*v1 + t* v2
{
	FLOAT k = 1.0F - t;
	x = k * v1->x + t * v2->x;
	y = k * v1->y + t * v2->y;
	z = k * v1->z + t * v2->z;
	return *this;
}

LCXVECTOR3&	LCXVECTOR3::Multiple(const LCXVECTOR3& v)
{
	x *= v.x; y*=v.y; z*=v.z;
	return *this;
}

LCXVECTOR3& LCXVECTOR3::Zero () { x=0.0F; y=0.0F; z=0.0F; return *this;}
LCXVECTOR3& LCXVECTOR3::One  () { x=1.0F; y=1.0F; z=1.0F; return *this;}
LCXVECTOR3& LCXVECTOR3::AxisX() { x=1.0F; y=0.0F; z=0.0F; return *this;}
LCXVECTOR3& LCXVECTOR3::AxisY() { x=0.0F; y=1.0F; z=0.0F; return *this;}
LCXVECTOR3& LCXVECTOR3::AxisZ() { x=0.0F; y=0.0F; z=1.0F; return *this;}

const LCXVECTOR3 LCXVECTOR3::ZERO =LCXVECTOR3(0.0F, 0.0F, 0.0F);
const LCXVECTOR3 LCXVECTOR3::ONE  =LCXVECTOR3(1.0F, 1.0F, 1.0F);
const LCXVECTOR3 LCXVECTOR3::AXISX=LCXVECTOR3(1.0F, 0.0F, 0.0F);
const LCXVECTOR3 LCXVECTOR3::AXISY=LCXVECTOR3(0.0F, 1.0F, 0.0F);
const LCXVECTOR3 LCXVECTOR3::AXISZ=LCXVECTOR3(0.0F, 0.0F, 1.0F);




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8

LCXVECTOR4::LCXVECTOR4()						{	x=0.0F; y=0.0F; z=0.0F; w=0.0F;	}
LCXVECTOR4::LCXVECTOR4(const FLOAT* v)		    {	if(!v) return; x=v[0]; y=v[1]; z=v[2]; w=v[3];	}
LCXVECTOR4::LCXVECTOR4(const LCXVECTOR4* v)	    {	if(!v) return; x=v->x; y=v->y; z=v->z; w=v->w;	}
LCXVECTOR4::LCXVECTOR4(FLOAT X, FLOAT Y, FLOAT Z, FLOAT W){	x=X; y=Y; z=Z; w=W;	}
LCXVECTOR4::LCXVECTOR4(const LC_FLOAT4& v)		{	x=v.x; y=v.y; z=v.z; w=v.w;	}
LCXVECTOR4::LCXVECTOR4(const LCXVECTOR3& v,FLOAT _w){	x=v.x; y=v.y; z=v.z; w=_w;	}

LCXVECTOR4::LCXVECTOR4(const LCXVECTOR4& v)     {   *this = v;      }
LCXVECTOR4& LCXVECTOR4::operator=(const LCXVECTOR4& v)
{
	x=v.x; y=v.y; z=v.z; w=v.w;
	return *this;
}

// casting
LCXVECTOR4::operator FLOAT*()				{	return(FLOAT*)&x;			}
LCXVECTOR4::operator const FLOAT*() const	{	return(const FLOAT*)&x;		}
FLOAT& LCXVECTOR4::operator[](int n)		{	return *((&x)+ n);			}

// assignment operators
LCXVECTOR4& LCXVECTOR4::operator +=(const LCXVECTOR4& v){ x +=v.x; y +=v.y; z +=v.z; w +=v.w; return *this;	}
LCXVECTOR4& LCXVECTOR4::operator -=(const LCXVECTOR4& v){ x -=v.x; y -=v.y; z -=v.z; w -=v.w; return *this;	}
LCXVECTOR4& LCXVECTOR4::operator *=(FLOAT v)			{ x *= v;  y *=v;	z *=v;   w *= v;  return *this; }
LCXVECTOR4& LCXVECTOR4::operator /=(FLOAT v){	v = 1.0F/v; x*= v;  y *=v;   z *=v;   w *= v;  return *this;	}

// unary operators
LCXVECTOR4 LCXVECTOR4::operator +() const	{	return *this;						}
LCXVECTOR4 LCXVECTOR4::operator -() const	{	return LCXVECTOR4(-x,-y,-z,-w);		}

// binary operators
LCXVECTOR4 LCXVECTOR4::operator +(const LCXVECTOR4& v) const	{	return LCXVECTOR4(x + v.x, y + v.y, z + v.z, w + v.w);	}
LCXVECTOR4 LCXVECTOR4::operator -(const LCXVECTOR4& v) const	{	return LCXVECTOR4(x - v.x, y - v.y, z - v.z, w - v.w);	}
LCXVECTOR4 LCXVECTOR4::operator *(FLOAT v) const				{	return LCXVECTOR4(x * v  , y * v  , z * v  , w * v  );	}
LCXVECTOR4 LCXVECTOR4::operator /(FLOAT v) const	{ v = 1.0F/v;	return LCXVECTOR4(x * v  , y * v  , z * v  , w * v  );	}
LCXVECTOR4 operator *(FLOAT f, const LCXVECTOR4& v)	{				return LCXVECTOR4(f * v.x, f * v.y, f * v.z, f * v.w);	}

//INT LCXVECTOR4::operator ==(const LCXVECTOR4& v) const	{	return x == v.x && y == v.y && z == v.z && w == v.w;	}
//INT LCXVECTOR4::operator !=(const LCXVECTOR4& v) const	{	return x != v.x || y != v.y || z != v.z || w != v.w;	}

INT LCXVECTOR4::operator ==(const LCXVECTOR4& v) const{ return LCXIsEqual(x, v.x)    && LCXIsEqual(y, v.y)    && LCXIsEqual(z, v.z)    && LCXIsEqual(w,v.w);	}
INT LCXVECTOR4::operator !=(const LCXVECTOR4& v) const{ return LCXIsNotEqual(x, v.x) || LCXIsNotEqual(y, v.y) || LCXIsNotEqual(z, v.z) || LCXIsNotEqual(w,v.w);	}

// Dot Product
FLOAT LCXVECTOR4::operator *(const LCXVECTOR4& v)			{	return x    * v.x  + y    * v.y  + z    * v.z  + w * v.w;	  }
FLOAT operator *(const LCXVECTOR4& v1, const LCXVECTOR4& v2){	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w; }


// Transform: vector * Matrix4x4
LCXVECTOR4 LCXVECTOR4::operator *(const LCXMATRIX& t)
{
	FLOAT X = x * t._11 + y * t._21 + z * t._31 + t._41;
	FLOAT Y = x * t._12 + y * t._22 + z * t._32 + t._42;
	FLOAT Z = x * t._13 + y * t._23 + z * t._33 + t._43;
	FLOAT W = x * t._14 + y * t._24 + z * t._34 + t._44;

	return LCXVECTOR4(X, Y, Z, W);
}

// Transform: Matrix4x4 * vector;
LCXVECTOR4 operator *(const LCXMATRIX& t, const LCXVECTOR4& v)
{
	FLOAT X = v.x * t._11 + v.y * t._12 + v.z * t._13 + v.w * t._14;
	FLOAT Y = v.x * t._21 + v.y * t._22 + v.z * t._23 + v.w * t._24;
	FLOAT Z = v.x * t._31 + v.y * t._32 + v.z * t._33 + v.w * t._34;
	FLOAT W = v.x * t._41 + v.y * t._42 + v.z * t._43 + v.w * t._44;

	return LCXVECTOR4(X, Y, Z, W);
}


FLOAT LCXVECTOR4::Length()	{	return (FLOAT)sqrt(x*x + y*y + z*z + w*w);		}
FLOAT LCXVECTOR4::LengthSq(){	return(x*x + y*y + z*z + w*w);					}

LCXVECTOR4 LCXVECTOR4::Normalize()
{
	FLOAT l = (FLOAT)sqrt(x*x + y*y + z*z + w*w);
	if(0.0F == l)
		return *this;

	l = 1.0F/l;
	x *= l;	y *= l;	z *= l;	w *= l;
	return *this;
}


LCXVECTOR4 LCXVECTOR4::Normalize(const LCXVECTOR4* v)
{
	*this = v;
	FLOAT l = (FLOAT)sqrt(x*x + y*y + z*z + w*w);
	if(l == 0)
		return *this;

	l = 1.0F/l;
	x *= l;	y *= l;	z *= l;	w *= l;

	return *this;
}


LCXVECTOR4& LCXVECTOR4::Negative()
{
	x = -x;
	y = -y;
	z = -z;
	w = -w;
	return *this;
}


LCXVECTOR4& LCXVECTOR4::Lerp(const LCXVECTOR4* v1, const LCXVECTOR4* v2, FLOAT t) // this = (1-t)*v1 + t* v2
{
	FLOAT k = 1.0F - t;
	x = k * v1->x + t * v2->x;
	y = k * v1->y + t * v2->y;
	z = k * v1->z + t * v2->z;
	w = k * v1->w + t * v2->w;
	return *this;
}

LCXVECTOR4&	LCXVECTOR4::Multiple(const LCXVECTOR4& v)
{
	x *= v.x; y*=v.y; z*=v.z; w*=v.w;
	return *this;
}

LCXVECTOR4& LCXVECTOR4::Zero()  { x=0.0F; y=0.0F; z=0.0F; w=0.0F; return *this;}
LCXVECTOR4& LCXVECTOR4::One ()  { x=1.0F; y=1.0F; z=1.0F; w=1.0F; return *this;}

const LCXVECTOR4 LCXVECTOR4::ZERO =LCXVECTOR4(0.0F, 0.0F, 0.0F, 0.0F);
const LCXVECTOR4 LCXVECTOR4::ONE  =LCXVECTOR4(1.0F, 1.0F, 1.0F, 1.0F);



//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// Matrix

LCXMATRIX::LCXMATRIX()
{
	_11 = 1.0F;  _12 = 0.0F;  _13 = 0.0F;  _14 = 0.0F;
	_21 = 0.0F;  _22 = 1.0F;  _23 = 0.0F;  _24 = 0.0F;
	_31 = 0.0F;  _32 = 0.0F;  _33 = 1.0F;  _34 = 0.0F;
	_41 = 0.0F;  _42 = 0.0F;  _43 = 0.0F;  _44 = 1.0F;
}

LCXMATRIX::LCXMATRIX(const FLOAT* v)
{
	if(!v)
		return;

	_11 = v[ 0];	_12 = v[ 1];	_13 = v[ 2];	_14 = v[ 3];
	_21 = v[ 4];	_22 = v[ 5];	_23 = v[ 6];	_24 = v[ 7];
	_31 = v[ 8];	_32 = v[ 9];	_33 = v[10];	_34 = v[11];
	_41 = v[12];	_42 = v[13];	_43 = v[14];	_44 = v[15];
}

LCXMATRIX::LCXMATRIX(const LCXMATRIX* v)
{
	if(!v)
		return;

	_11 = v->_11;	_12 = v->_12;	_13 = v->_13;	_14 = v->_14;
	_21 = v->_21;	_22 = v->_22;	_23 = v->_23;	_24 = v->_24;
	_31 = v->_31;	_32 = v->_32;	_33 = v->_33;	_34 = v->_34;
	_41 = v->_41;	_42 = v->_42;	_43 = v->_43;	_44 = v->_44;
}


LCXMATRIX::LCXMATRIX(FLOAT v11, FLOAT v12, FLOAT v13, FLOAT v14,
					 FLOAT v21, FLOAT v22, FLOAT v23, FLOAT v24,
					 FLOAT v31, FLOAT v32, FLOAT v33, FLOAT v34,
					 FLOAT v41, FLOAT v42, FLOAT v43, FLOAT v44 )
{
	_11 = v11;	_12 = v12;	_13 = v13;	_14 = v14;
	_21 = v21;	_22 = v22;	_23 = v23;	_24 = v24;
	_31 = v31;	_32 = v32;	_33 = v33;	_34 = v34;
	_41 = v41;	_42 = v42;	_43 = v43;	_44 = v44;
}


LCXMATRIX::LCXMATRIX(const LCXMATRIX& v)   {   *this = v;      }
LCXMATRIX& LCXMATRIX::operator=(const LCXMATRIX& v)
{
	_11 = v._11;	_12 = v._12;	_13 = v._13;	_14 = v._14;
	_21 = v._21;	_22 = v._22;	_23 = v._23;	_24 = v._24;
	_31 = v._31;	_32 = v._32;	_33 = v._33;	_34 = v._34;
	_41 = v._41;	_42 = v._42;	_43 = v._43;	_44 = v._44;
	return *this;
}



// access grants
FLOAT& LCXMATRIX::operator()(int iRow, int iCol)	{   return m[iRow][iCol];	}
FLOAT  LCXMATRIX::operator()(int iRow, int iCol) const{ return m[iRow][iCol];	}

// casting operators
LCXMATRIX::operator FLOAT*()				{	return(FLOAT*)&_11;		}
LCXMATRIX::operator const FLOAT*() const	{	return(FLOAT*)&_11;		}
FLOAT& LCXMATRIX::operator[](int n)         {	return *((&_11)+ n);	}

// assignment operators
LCXMATRIX& LCXMATRIX::operator *=(const LCXMATRIX& v)
{
	LCXMATRIX t = *this;
	_11 = t._11 * v._11 +  t._12 * v._21 +  t._13 * v._31 +  t._14 * v._41;
	_12 = t._11 * v._12 +  t._12 * v._22 +  t._13 * v._32 +  t._14 * v._42;
	_13 = t._11 * v._13 +  t._12 * v._23 +  t._13 * v._33 +  t._14 * v._43;
	_14 = t._11 * v._14 +  t._12 * v._24 +  t._13 * v._34 +  t._14 * v._44;

	_21 = t._21 * v._11 +  t._22 * v._21 +  t._23 * v._31 +  t._24 * v._41;
	_22 = t._21 * v._12 +  t._22 * v._22 +  t._23 * v._32 +  t._24 * v._42;
	_23 = t._21 * v._13 +  t._22 * v._23 +  t._23 * v._33 +  t._24 * v._43;
	_24 = t._21 * v._14 +  t._22 * v._24 +  t._23 * v._34 +  t._24 * v._44;

	_31 = t._31 * v._11 +  t._32 * v._21 +  t._33 * v._31 +  t._34 * v._41;
	_32 = t._31 * v._12 +  t._32 * v._22 +  t._33 * v._32 +  t._34 * v._42;
	_33 = t._31 * v._13 +  t._32 * v._23 +  t._33 * v._33 +  t._34 * v._43;
	_34 = t._31 * v._14 +  t._32 * v._24 +  t._33 * v._34 +  t._34 * v._44;

	_41 = t._41 * v._11 +  t._42 * v._21 +  t._43 * v._31 +  t._44 * v._41;
	_42 = t._41 * v._12 +  t._42 * v._22 +  t._43 * v._32 +  t._44 * v._42;
	_43 = t._41 * v._13 +  t._42 * v._23 +  t._43 * v._33 +  t._44 * v._43;
	_44 = t._41 * v._14 +  t._42 * v._24 +  t._43 * v._34 +  t._44 * v._44;

	return * this;
}


LCXMATRIX& LCXMATRIX::operator +=(const LCXMATRIX& v)
{
	_11 += v._11;	_12 += v._12;	_13 += v._13;	_14 += v._14;
	_21 += v._21;	_22 += v._22;	_23 += v._23;	_24 += v._24;
	_31 += v._31;	_32 += v._32;	_33 += v._33;	_34 += v._34;
	_41 += v._41;	_42 += v._42;	_43 += v._43;	_44 += v._44;

	return * this;
}

LCXMATRIX& LCXMATRIX::operator -=(const LCXMATRIX& v)
{
	_11 -= v._11;	_12 -= v._12;	_13 -= v._13;	_14 -= v._14;
	_21 -= v._21;	_22 -= v._22;	_23 -= v._23;	_24 -= v._24;
	_31 -= v._31;	_32 -= v._32;	_33 -= v._33;	_34 -= v._34;
	_41 -= v._41;	_42 -= v._42;	_43 -= v._43;	_44 -= v._44;

	return * this;
}

LCXMATRIX& LCXMATRIX::operator *=(FLOAT v)
{
	_11 *= v;	_12 *= v;	_13 *= v;	_14 *= v;
	_21 *= v;	_22 *= v;	_23 *= v;	_24 *= v;
	_31 *= v;	_32 *= v;	_33 *= v;	_34 *= v;
	_41 *= v;	_42 *= v;	_43 *= v;	_44 *= v;

	return * this;
}

LCXMATRIX& LCXMATRIX::operator /=(FLOAT v)
{
	v = 1.0F/v;
	_11 *= v;	_12 *= v;	_13 *= v;	_14 *= v;
	_21 *= v;	_22 *= v;	_23 *= v;	_24 *= v;
	_31 *= v;	_32 *= v;	_33 *= v;	_34 *= v;
	_41 *= v;	_42 *= v;	_43 *= v;	_44 *= v;

	return * this;
}


// unary operators
LCXMATRIX LCXMATRIX::operator +() const
{
	return *this;
}

LCXMATRIX LCXMATRIX::operator -() const
{
	return LCXMATRIX(
		-_11, -_12, -_13, -_14,
		-_21, -_22, -_23, -_24,
		-_31, -_32, -_33, -_34,
		-_41, -_42, -_43, -_44);
}

// binary operators
LCXMATRIX LCXMATRIX::operator *(const LCXMATRIX& v) const
{
	LCXMATRIX t = *this;
	LCXMATRIX r;
	r._11 = t._11 * v._11 +  t._12 * v._21 +  t._13 * v._31 +  t._14 * v._41;
	r._12 = t._11 * v._12 +  t._12 * v._22 +  t._13 * v._32 +  t._14 * v._42;
	r._13 = t._11 * v._13 +  t._12 * v._23 +  t._13 * v._33 +  t._14 * v._43;
	r._14 = t._11 * v._14 +  t._12 * v._24 +  t._13 * v._34 +  t._14 * v._44;

	r._21 = t._21 * v._11 +  t._22 * v._21 +  t._23 * v._31 +  t._24 * v._41;
	r._22 = t._21 * v._12 +  t._22 * v._22 +  t._23 * v._32 +  t._24 * v._42;
	r._23 = t._21 * v._13 +  t._22 * v._23 +  t._23 * v._33 +  t._24 * v._43;
	r._24 = t._21 * v._14 +  t._22 * v._24 +  t._23 * v._34 +  t._24 * v._44;

	r._31 = t._31 * v._11 +  t._32 * v._21 +  t._33 * v._31 +  t._34 * v._41;
	r._32 = t._31 * v._12 +  t._32 * v._22 +  t._33 * v._32 +  t._34 * v._42;
	r._33 = t._31 * v._13 +  t._32 * v._23 +  t._33 * v._33 +  t._34 * v._43;
	r._34 = t._31 * v._14 +  t._32 * v._24 +  t._33 * v._34 +  t._34 * v._44;

	r._41 = t._41 * v._11 +  t._42 * v._21 +  t._43 * v._31 +  t._44 * v._41;
	r._42 = t._41 * v._12 +  t._42 * v._22 +  t._43 * v._32 +  t._44 * v._42;
	r._43 = t._41 * v._13 +  t._42 * v._23 +  t._43 * v._33 +  t._44 * v._43;
	r._44 = t._41 * v._14 +  t._42 * v._24 +  t._43 * v._34 +  t._44 * v._44;

	return r;
}


LCXMATRIX LCXMATRIX::operator +(const LCXMATRIX& v) const
{
	return LCXMATRIX(
		_11 + v._11, _12 + v._12, _13 + v._13, _14 + v._14,
		_21 + v._21, _22 + v._22, _23 + v._23, _24 + v._24,
		_31 + v._31, _32 + v._32, _33 + v._33, _34 + v._34,
		_41 + v._41, _42 + v._42, _43 + v._43, _44 + v._44
		);
}

LCXMATRIX LCXMATRIX::operator -(const LCXMATRIX& v) const
{
	return LCXMATRIX(
		_11 - v._11, _12 - v._12, _13 - v._13, _14 - v._14,
		_21 - v._21, _22 - v._22, _23 - v._23, _24 - v._24,
		_31 - v._31, _32 - v._32, _33 - v._33, _34 - v._34,
		_41 - v._41, _42 - v._42, _43 - v._43, _44 - v._44
		);
}

LCXMATRIX LCXMATRIX::operator *(FLOAT v) const
{
	return LCXMATRIX(
		_11 * v, _12 * v, _13 * v, _14 * v,
		_21 * v, _22 * v, _23 * v, _24 * v,
		_31 * v, _32 * v, _33 * v, _34 * v,
		_41 * v, _42 * v, _43 * v, _44 * v
		);
}

LCXMATRIX LCXMATRIX::operator /(FLOAT v) const
{
	v = 1.0F / v;
	return LCXMATRIX(
		_11 * v, _12 * v, _13 * v, _14 * v,
		_21 * v, _22 * v, _23 * v, _24 * v,
		_31 * v, _32 * v, _33 * v, _34 * v,
		_41 * v, _42 * v, _43 * v, _44 * v
		);
}


LCXMATRIX operator *(FLOAT f, const LCXMATRIX& v)
{
	return LCXMATRIX(
		f * v._11, f * v._12, f * v._13, f * v._14,
		f * v._21, f * v._22, f * v._23, f * v._24,
		f * v._31, f * v._32, f * v._33, f * v._34,
		f * v._41, f * v._42, f * v._43, f * v._44
		);
}

LCXMATRIX operator *(const LCXMATRIX& v, FLOAT f)
{
	return LCXMATRIX(
		f * v._11, f * v._12, f * v._13, f * v._14,
		f * v._21, f * v._22, f * v._23, f * v._24,
		f * v._31, f * v._32, f * v._33, f * v._34,
		f * v._41, f * v._42, f * v._43, f * v._44
		);
}

LCXVECTOR4 operator *(LCXVECTOR4& p, const LCXMATRIX& v)
{
	return LCXVECTOR4(p.x * v._11 + p.y * v._21 + p.z * v._31 + p.w * v._41
					, p.x * v._12 + p.y * v._22 + p.z * v._32 + p.w * v._42
					, p.x * v._13 + p.y * v._23 + p.z * v._33 + p.w * v._43
					, p.x * v._14 + p.y * v._24 + p.z * v._34 + p.w * v._44
					);
}

LCXVECTOR4 operator *(const LCXMATRIX& v, LCXVECTOR4& p)
{
	return LCXVECTOR4(v._11 * p.x + v._12 * p.y + v._13 * p.z + v._14 * p.w
					, v._21 * p.x + v._22 * p.y + v._23 * p.z + v._24 * p.w
					, v._31 * p.x + v._32 * p.y + v._33 * p.z + v._34 * p.w
					, v._41 * p.x + v._42 * p.y + v._43 * p.z + v._44 * p.w
					);
}

LCXVECTOR3 operator *(LCXVECTOR3& p, const LCXMATRIX& v)
{
	// w = 1
	FLOAT x = p.x * v._11 + p.y * v._21 + p.z * v._31 + v._41;	// * p.w;
	FLOAT y = p.x * v._12 + p.y * v._22 + p.z * v._32 + v._42;	// * p.w;
	FLOAT z = p.x * v._13 + p.y * v._23 + p.z * v._33 + v._43;	// * p.w;
	FLOAT w = p.x * v._14 + p.y * v._24 + p.z * v._34 + v._44;	// * p.w;

	x /=w;
	y /=w;
	z /=w;
	w = 1;

	return LCXVECTOR3(x,y,z);
}

LCXVECTOR3 operator *(const LCXMATRIX& v, LCXVECTOR3& p)
{
	// w = 1
	FLOAT x = v._11 * p.x + v._12 * p.y + v._13 * p.z + v._14;	// * p.w;
	FLOAT y = v._21 * p.x + v._22 * p.y + v._23 * p.z + v._24;	// * p.w;
	FLOAT z = v._31 * p.x + v._32 * p.y + v._33 * p.z + v._34;	// * p.w;
	FLOAT w = v._41 * p.x + v._42 * p.y + v._43 * p.z + v._44;	// * p.w;

	x /=w;
	y /=w;
	z /=w;
	w = 1;

	return LCXVECTOR3(x,y,z);
}



INT LCXMATRIX::operator ==(const LCXMATRIX& v) const
{
	return	(
		 LCXIsEqual(_11,v._11) && LCXIsEqual(_12, v._12) && LCXIsEqual(_13, v._13) && LCXIsEqual(_14, v._14) &&
		 LCXIsEqual(_21,v._21) && LCXIsEqual(_22, v._22) && LCXIsEqual(_23, v._23) && LCXIsEqual(_24, v._24) &&
		 LCXIsEqual(_31,v._31) && LCXIsEqual(_32, v._32) && LCXIsEqual(_33, v._33) && LCXIsEqual(_34, v._34) &&
		 LCXIsEqual(_41,v._41) && LCXIsEqual(_42, v._42) && LCXIsEqual(_43, v._43) && LCXIsEqual(_44, v._44)
		);
}

INT LCXMATRIX::operator !=(const LCXMATRIX& v) const
{
	return	(
		LCXIsNotEqual(_11, v._11) || LCXIsNotEqual(_12, v._12) || LCXIsNotEqual(_13, v._13) || LCXIsNotEqual(_14, v._14) ||
		LCXIsNotEqual(_21, v._21) || LCXIsNotEqual(_22, v._22) || LCXIsNotEqual(_23, v._23) || LCXIsNotEqual(_24, v._24) ||
		LCXIsNotEqual(_31, v._31) || LCXIsNotEqual(_32, v._32) || LCXIsNotEqual(_33, v._33) || LCXIsNotEqual(_34, v._34) ||
		LCXIsNotEqual(_41, v._41) || LCXIsNotEqual(_42, v._42) || LCXIsNotEqual(_43, v._43) || LCXIsNotEqual(_44, v._44)
		);
}



LCXMATRIX& LCXMATRIX::Identity()
{
	_11 = 1.0F; _12 = 0.0F; _13 = 0.0F; _14 = 0.0F;
	_21 = 0.0F; _22 = 1.0F; _23 = 0.0F; _24 = 0.0F;
	_31 = 0.0F; _32 = 0.0F; _33 = 1.0F; _34 = 0.0F;
	_41 = 0.0F; _42 = 0.0F; _43 = 0.0F; _44 = 1.0F;
	return *this;
}

LCXMATRIX& LCXMATRIX::Zero()
{
	_11 = 0.0F; _12 = 0.0F; _13 = 0.0F; _14 = 0.0F;
	_21 = 0.0F; _22 = 0.0F; _23 = 0.0F; _24 = 0.0F;
	_31 = 0.0F; _32 = 0.0F; _33 = 0.0F; _34 = 0.0F;
	_41 = 0.0F; _42 = 0.0F; _43 = 0.0F; _44 = 1.0F;
	return *this;
}

LCXMATRIX& LCXMATRIX::Transpose()
{
	FLOAT t = 0.0F;

	t   = _12;	_12 = _21;	_21 = t;
	t   = _13;	_13 = _31;	_31 = t;
	t   = _14;	_14 = _41;	_41 = t;

	t   = _23;	_23 = _32;	_32 = t;
	t   = _24;	_24 = _42;	_42 = t;

	t   = _34;	_34 = _43;	_43 = t;

	return *this;
}

LCXMATRIX& LCXMATRIX::Transpose(const LCXMATRIX* v)
{
	_11 = v->_11;	_12 = v->_21;	_13 = v->_31;	_14 = v->_41;
	_21 = v->_12;	_22 = v->_22;	_23 = v->_32;	_24 = v->_42;
	_31 = v->_13;	_32 = v->_23;	_33 = v->_33;	_34 = v->_43;
	_41 = v->_14;	_42 = v->_24;	_43 = v->_34;	_44 = v->_44;

	return *this;
}


LCXMATRIX& LCXMATRIX::Inverse(FLOAT* det, INT nQuick)
{
	LCXMATRIX	t = *this;
	this->Inverse(det, &t, nQuick);
	return *this;
}

LCXMATRIX& LCXMATRIX::Inverse(FLOAT* det, const LCXMATRIX* pIn, INT nQuick)
{
	if(LC_TRUE == nQuick)
	{
		FLOAT d = 1.0F;

		// set determinant of 4x4 Matrix
		d =
			pIn->_11 * ( pIn->_22 * pIn->_33 - pIn->_23 * pIn->_32 ) +
			pIn->_12 * ( pIn->_23 * pIn->_31 - pIn->_21 * pIn->_33 ) +
			pIn->_13 * ( pIn->_21 * pIn->_32 - pIn->_22 * pIn->_31 );

		if(det) *det = d;
		if(-0.00001F < d && d <0.00001F)
			return *this;

		d = 1.0F/d;

		_11 =  d * ( pIn->_22 * pIn->_33 - pIn->_23 * pIn->_32 );
		_12 = -d * ( pIn->_12 * pIn->_33 - pIn->_13 * pIn->_32 );
		_13 =  d * ( pIn->_12 * pIn->_23 - pIn->_13 * pIn->_22 );
		_14 = 0.0F;

		_21 = -d * ( pIn->_21 * pIn->_33 - pIn->_23 * pIn->_31 );
		_22 =  d * ( pIn->_11 * pIn->_33 - pIn->_13 * pIn->_31 );
		_23 = -d * ( pIn->_11 * pIn->_23 - pIn->_13 * pIn->_21 );
		_24 = 0.0F;

		_31 =  d * ( pIn->_21 * pIn->_32 - pIn->_22 * pIn->_31 );
		_32 = -d * ( pIn->_11 * pIn->_32 - pIn->_12 * pIn->_31 );
		_33 =  d * ( pIn->_11 * pIn->_22 - pIn->_12 * pIn->_21 );
		_34 = 0.0F;

		_41 = -( pIn->_41 * this->_11 + pIn->_42 * this->_21 + pIn->_43 * this->_31 );
		_42 = -( pIn->_41 * this->_12 + pIn->_42 * this->_22 + pIn->_43 * this->_32 );
		_43 = -( pIn->_41 * this->_13 + pIn->_42 * this->_23 + pIn->_43 * this->_33 );
		_44 = 1.0F;

		return *this;
	}


	FLOAT* v = (FLOAT*)pIn;
	FLOAT* Out = &m[0][0];
	FLOAT d = 0;
	float k[16];

	k[ 0] = v[ 9] * v[14] - v[10] * v[13];
	k[ 1] = v[10] * v[12] - v[ 8] * v[14];
	k[ 2] = v[ 8] * v[13] - v[ 9] * v[12];
	k[ 3] = v[ 1] * v[ 6] - v[ 2] * v[ 5];

	k[ 4] = v[ 2] * v[ 4] - v[ 0] * v[ 6];
	k[ 5] = v[ 0] * v[ 5] - v[ 1] * v[ 4];
	k[ 6] = v[10] * v[15] - v[11] * v[14];
	k[ 7] = v[11] * v[13] - v[ 9] * v[15];

	k[ 8] = v[ 2] * v[ 7] - v[ 3] * v[ 6];
	k[ 9] = v[ 3] * v[ 5] - v[ 1] * v[ 7];
	k[10] = v[11] * v[12] - v[ 8] * v[15];
	k[11] = v[ 8] * v[14] - v[10] * v[12];

	k[12] = v[ 3] * v[ 4] - v[ 0] * v[ 7];
	k[13] = v[ 0] * v[ 6] - v[ 2] * v[ 4];
	k[14] = v[ 9] * v[15] - v[11] * v[13];
	k[15] = v[ 1] * v[ 7] - v[ 3] * v[ 5];

	d += ( k[ 8]) * k[ 2];
	d += ( k[ 6]) * k[ 5];

	d += (-k[12]) * k[ 0];
	d += (-k[ 9]) * k[ 1];

	d += (-k[10]) * k[ 3];
	d += (-k[ 7]) * k[ 4];


	if(det) *det = d;
	if(-0.00001F < d && d <0.00001F)
		return *this;

	d = 1.0F/d;

	Out[ 0]= +(v[ 5] * k[ 6] + v[ 6] * k[ 7] + v[ 7] * k[ 0]);
	Out[ 1]= -(v[ 1] * k[ 6] + v[ 2] * k[ 7] + v[ 3] * k[ 0]);
	Out[ 2]= +(v[13] * k[ 8] + v[14] * k[ 9] + v[15] * k[ 3]);
	Out[ 3]= -(v[ 9] * k[ 8] + v[10] * k[ 9] + v[11] * k[ 3]);
	Out[ 4]= -(v[ 4] * k[ 6] + v[ 6] * k[10] + v[ 7] * k[11]);
	Out[ 5]= +(v[ 0] * k[ 6] + v[ 2] * k[10] + v[ 3] * k[11]);
	Out[ 6]= -(v[12] * k[ 8] + v[14] * k[12] + v[15] * k[13]);
	Out[ 7]= +(v[ 8] * k[ 8] + v[10] * k[12] + v[11] * k[13]);
	Out[ 8]= +(v[ 4] * k[14] + v[ 5] * k[10] + v[ 7] * k[ 2]);
	Out[ 9]= -(v[ 0] * k[14] + v[ 1] * k[10] + v[ 3] * k[ 2]);
	Out[10]= +(v[12] * k[15] + v[13] * k[12] + v[15] * k[ 5]);
	Out[11]= -(v[ 8] * k[15] + v[ 9] * k[12] + v[11] * k[ 5]);
	Out[12]= -(v[ 4] * k[ 0] + v[ 5] * k[ 1] + v[ 6] * k[ 2]);
	Out[13]= +(v[ 0] * k[ 0] + v[ 1] * k[ 1] + v[ 2] * k[ 2]);
	Out[14]= -(v[12] * k[ 3] + v[13] * k[ 4] + v[14] * k[ 5]);
	Out[15]= +(v[ 8] * k[ 3] + v[ 9] * k[ 4] + v[10] * k[ 5]);

	Out[ 0] *= d;	Out[ 1] *= d;	Out[ 2] *= d;	Out[ 3] *= d;
	Out[ 4] *= d;	Out[ 5] *= d;	Out[ 6] *= d;	Out[ 7] *= d;
	Out[ 8] *= d;	Out[ 9] *= d;	Out[10] *= d;	Out[11] *= d;
	Out[12] *= d;	Out[13] *= d;	Out[14] *= d;	Out[15] *= d;

	return *this;
}


FLOAT LCXMATRIX::Determinant() const
{
	FLOAT t =0;

	t += (_24 * _11 - _14 * _21) * (_32 * _43 - _33 * _42);
	t += (_24 * _12 - _14 * _22) * (_33 * _41 - _31 * _43);
	t += (_24 * _13 - _14 * _23) * (_31 * _42 - _32 * _41);

	t += (_44 * _31 - _34 * _41) * (_12 * _23 - _13 * _22);
	t += (_44 * _32 - _34 * _42) * (_13 * _21 - _11 * _23);
	t += (_44 * _33 - _34 * _43) * (_11 * _22 - _12 * _21);

	return t;
}


LCXMATRIX& LCXMATRIX::Scaling(FLOAT X, FLOAT Y, FLOAT Z)
{
	_11 = X   ; _12 = 0.0F; _13 = 0.0F; _14 = 0.0F;
	_21 = 0.0F; _22 = Y   ; _23 = 0.0F; _24 = 0.0F;
	_31 = 0.0F; _32 = 0.0F; _33 =  Z  ; _34 = 0.0F;
	_41 = 0.0F; _42 = 0.0F; _43 = 0.0F; _44 = 1.0F;

	return *this;
}


LCXMATRIX& LCXMATRIX::Scaling(const LCXVECTOR3* vScl)
{
	return Scaling(vScl->x, vScl->y, vScl->z);
}


LCXMATRIX& LCXMATRIX::ViewD3dLH(const LCXVECTOR3* Eye, const LCXVECTOR3* LookAt, const LCXVECTOR3* Up)
{
	LCXVECTOR3 vcX;
	LCXVECTOR3 vcY;
	LCXVECTOR3 vcZ;

	vcZ = *LookAt - *Eye;
	vcZ.Normalize();

	vcX	= *Up ^ vcZ;
	vcX.Normalize();

	vcY = vcZ ^ vcX;

	_11 = vcX.x;         _12 = vcY.x;         _13 = vcZ.x;         _14 = 0.0F;
	_21 = vcX.y;         _22 = vcY.y;         _23 = vcZ.y;         _24 = 0.0F;
	_31 = vcX.z;         _32 = vcY.z;         _33 = vcZ.z;         _34 = 0.0F;
	_41 = - *Eye * vcX;  _42 = - *Eye * vcY;  _43 = - *Eye * vcZ;  _44 = 1.0F;

	return *this;
}


LCXMATRIX& LCXMATRIX::ViewD3dRH(const LCXVECTOR3* Eye, const LCXVECTOR3* LookAt, const LCXVECTOR3* Up)
{
	LCXVECTOR3 vcX;
	LCXVECTOR3 vcY;
	LCXVECTOR3 vcZ;

	vcZ = *Eye - *LookAt;
	vcZ.Normalize();

	vcX	= *Up ^ vcZ;
	vcX.Normalize();

	vcY = vcZ ^ vcX;

	_11 = vcX.x;         _12 = vcY.x;         _13 = vcZ.x;         _14 = 0.0F;
	_21 = vcX.y;         _22 = vcY.y;         _23 = vcZ.y;         _24 = 0.0F;
	_31 = vcX.z;         _32 = vcY.z;         _33 = vcZ.z;         _34 = 0.0F;
	_41 = - *Eye * vcX;  _42 = - *Eye * vcY;  _43 = - *Eye * vcZ;  _44 = 1.0F;

	return *this;
}


LCXMATRIX& LCXMATRIX::ViewGl(const LCXVECTOR3* Eye, const LCXVECTOR3* LookAt, const LCXVECTOR3* Up)
{
	LCXVECTOR3 vcX;
	LCXVECTOR3 vcY;
	LCXVECTOR3 vcZ;

	vcZ = *Eye - *LookAt;
	vcZ.Normalize();

	vcX	= *Up ^ vcZ;
	vcX.Normalize();

	vcY = vcZ ^ vcX;

	_11 = vcX.x;         _12 = vcY.x;         _13 = vcZ.x;         _14 = 0.0F;
	_21 = vcX.y;         _22 = vcY.y;         _23 = vcZ.y;         _24 = 0.0F;
	_31 = vcX.z;         _32 = vcY.z;         _33 = vcZ.z;         _34 = 0.0F;
	_41 = - *Eye * vcX;  _42 = - *Eye * vcY;  _43 = - *Eye * vcZ;  _44 = 1.0F;

	return *this;
}



//LCXMATRIX& LCXMATRIX::View3DMax(const LCXVECTOR3* Eye, const LCXVECTOR3* LookAt, const LCXVECTOR3* Up)
//{
//	LCXVECTOR3 vcX;
//	LCXVECTOR3 vcY;
//	LCXVECTOR3 vcZ;
//
//	vcY = *LookAt - *Eye;
//	vcY.Normalize();
//
//	vcX	= vcY^ *Up;
//	vcX.Normalize();
//
//	vcZ = vcX ^ vcY;
//
//	_11 = vcX.x;         _12 = vcY.x;         _13 = vcZ.x;         _14 = 0.0F;
//	_21 = vcX.y;         _22 = vcY.y;         _23 = vcZ.y;         _24 = 0.0F;
//	_31 = vcX.z;         _32 = vcY.z;         _33 = vcZ.z;         _34 = 0.0F;
//	_41 = - *Eye * vcX;  _42 = - *Eye * vcY;  _43 = - *Eye * vcZ;  _44 = 1.0F;
//
//	if(bExchangeYZ)
//	{
//		FLOAT tmp=0;
//		// X-Y 평면에 Z 축이 위로 향하는 공간에서 2, 3의 교환과 14, 24, 34의 반전이 필요
//		//mtViw._11 = m_mtViw._11; mtViw._12 = m_mtViw._13; mtViw._13 = -m_mtViw._12; mtViw._14 = -m_mtViw._14;
//		//mtViw._21 = m_mtViw._21; mtViw._22 = m_mtViw._23; mtViw._23 = -m_mtViw._22; mtViw._24 = -m_mtViw._24;
//		//mtViw._31 = m_mtViw._31; mtViw._32 = m_mtViw._33; mtViw._33 = -m_mtViw._32; mtViw._34 = -m_mtViw._34;
//		//mtViw._41 = m_mtViw._41; mtViw._42 = m_mtViw._43; mtViw._43 = -m_mtViw._42; mtViw._44 = +m_mtViw._44;
//		tmp = _12;	_12 = _13;	_13 = -tmp;
//		tmp = _22;	_22 = _23;	_23 = -tmp;
//		tmp = _32;	_32 = _33;	_33 = -tmp;
//		tmp = _42;	_42 = _43;	_43 = -tmp;
//	}
//
//	return *this;
//}


LCXMATRIX& LCXMATRIX::PerspectiveD3dLH(FLOAT Fov, FLOAT Asp, FLOAT Near, FLOAT Far)
{
	Fov	*=0.5f;
	//FLOAT cot = (FLOAT)cos(Fov)/(FLOAT)sin(Fov);
	FLOAT cot = LCXMathCos(Fov)/LCXMathSin(Fov);

	FLOAT h = cot;
	FLOAT w = cot/Asp;

	_11 = w   ;	_12 = 0.0F;	_13 = 0.0F;					_14 = 0.0F;
	_21 = 0.0F;	_22 = h   ;	_23 = 0.0F;					_24 = 0.0F;
	_31 = 0.0F;	_32 = 0.0F;	_33 =       Far/(Far-Near);	_34 = 1.0F;
	_41 = 0.0F;	_42 = 0.0F;	_43 = -Near*Far/(Far-Near);	_44 = 0.0F;

	return *this;
}


LCXMATRIX& LCXMATRIX::PerspectiveD3dRH(FLOAT Fov, FLOAT Asp, FLOAT Near, FLOAT Far)
{
	Fov	*=0.5f;
	//FLOAT cot = (FLOAT)cos(Fov)/(FLOAT)sin(Fov);
	FLOAT cot = LCXMathCos(Fov)/LCXMathSin(Fov);

	FLOAT h = cot;
	FLOAT w = cot/Asp;

	_11 = w   ;	_12 = 0.0F;	_13 = 0.0F;					_14 = 0.0F;
	_21 = 0.0F;	_22 = h   ;	_23 = 0.0F;					_24 = 0.0F;
	_31 = 0.0F;	_32 = 0.0F;	_33 =      Far/(Near-Far);	_34 =-1.0F;
	_41 = 0.0F;	_42 = 0.0F;	_43 = Near*Far/(Near-Far);	_44 = 0.0F;

	return *this;
}

LCXMATRIX& LCXMATRIX::PerspectiveGl(FLOAT Fov, FLOAT Asp, FLOAT Near, FLOAT Far)
{
	Fov	*=0.5f;
	//FLOAT cot = (FLOAT)cos(Fov)/(FLOAT)sin(Fov);
	FLOAT cot = LCXMathCos(Fov)/LCXMathSin(Fov);

	FLOAT h = cot;
	FLOAT w = cot/Asp;

	_11 = w   ;	_12 = 0.0F;	_13 = 0.0F;						_14 = 0.0F;
	_21 = 0.0F;	_22 = h   ;	_23 = 0.0F;						_24 = 0.0F;
	_31 = 0.0F;	_32 = 0.0F;	_33 =   (Near+Far)/(Near-Far);	_34 =-1.0F;
	_41 = 0.0F;	_42 = 0.0F;	_43 = 2*(Near*Far)/(Near-Far);	_44 = 0.0F;

	return *this;
}

LCXMATRIX& LCXMATRIX::OrthoGl(FLOAT Left, FLOAT Right, FLOAT Bottom, FLOAT Top, FLOAT Near, FLOAT Far)
{
	_11 = 2.0F/(Right-Left)        ; _12 = 0.0F                     ; _13 = 0.0F                 ; _14 = 0.0F;
	_21 = 0.0F                     ; _22 = 2.0F/(Top-Bottom)        ; _23 = 0.0F                 ; _24 = 0.0F;
	_31 = 0.0F                     ; _32 = 0.0F                     ; _33 = 2.0F/(Near-Far)      ; _34 = 0.0F;
	_41 = (Left+Right)/(Left-Right); _42 = (Bottom+Top)/(Bottom-Top); _43 = (Near+Far)/(Near-Far); _44 = 1.0F;

	return *this;
}


LCXMATRIX& LCXMATRIX::RotationX(FLOAT Rad)
{
	//FLOAT Cos = (FLOAT)cos(Rad);
	//FLOAT Sin = (FLOAT)sin(Rad);
	FLOAT Cos = LCXMathCos(Rad);
	FLOAT Sin = LCXMathSin(Rad);

	_11 = 1.0F; _12 = 0.0F;	_13 = 0.0F;	_14 = 0.0F;
	_21 = 0.0F; _22 = Cos ;	_23 = Sin ;	_24 = 0.0F;
	_31 = 0.0F; _32 =-Sin ;	_33 = Cos ;	_34 = 0.0F;
	_41 = 0.0F; _42 = 0.0F;	_43 = 0.0F;	_44 = 1.0F;

	return *this;
}


LCXMATRIX& LCXMATRIX::RotationY(FLOAT Rad)
{
	//FLOAT Cos = (FLOAT)cos(Rad);
	//FLOAT Sin = (FLOAT)sin(Rad);
	FLOAT Cos = LCXMathCos(Rad);
	FLOAT Sin = LCXMathSin(Rad);

	_11 = Cos ;  _12 = 0.0F;  _13 =-Sin ;  _14 = 0.0F;
	_21 = 0.0F;  _22 = 1.0F;  _23 = 0.0F;  _24 = 0.0F;
	_31 = Sin ;  _32 = 0.0F;  _33 = Cos ;  _34 = 0.0F;
	_41 = 0.0F;  _42 = 0.0F;  _43 = 0.0F;  _44 = 1.0F;

	return *this;
}


LCXMATRIX& LCXMATRIX::RotationZ(FLOAT Rad)
{
	//FLOAT Cos = (FLOAT)cos(Rad);
	//FLOAT Sin = (FLOAT)sin(Rad);
	FLOAT Cos = LCXMathCos(Rad);
	FLOAT Sin = LCXMathSin(Rad);

	_11 = Cos ;  _12 = Sin ;  _13 = 0.0F;  _14 = 0.0F;
	_21 =-Sin ;  _22 = Cos ;  _23 = 0.0F;  _24 = 0.0F;
	_31 = 0.0F;  _32 = 0.0F;  _33 = 1.0F;  _34 = 0.0F;
	_41 = 0.0F;  _42 = 0.0F;  _43 = 0.0F;  _44 = 1.0F;

	return *this;
}


LCXMATRIX& LCXMATRIX::RotationZXY(FLOAT r_x, FLOAT r_y, FLOAT r_z)
{
	//LCXMATRIX	m;
	//LCXMATRIX	mtRx;
	//LCXMATRIX	mtRy;
	//LCXMATRIX	mtRz;
	//this->Identity();
	//mtRx.RotationX(r_x);
	//mtRy.RotationY(r_y);
	//mtRz.RotationZ(r_z);
	//*this = mtRz * mtRx * mtRy;
	//return *this;

	FLOAT cos_x = LCXMathCos(r_x);
	FLOAT sin_x = LCXMathSin(r_x);
	FLOAT cos_y = LCXMathCos(r_y);
	FLOAT sin_y = LCXMathSin(r_y);
	FLOAT cos_z = LCXMathCos(r_z);
	FLOAT sin_z = LCXMathSin(r_z);

	_11 =  cos_y * cos_z + sin_x * sin_y * sin_z; _12 =  cos_x * sin_z;	_13 = -sin_y * cos_z + sin_x * cos_y * sin_z; _14 = 0.0F;
	_21 = -cos_y * sin_z + sin_x * sin_y * cos_z; _22 =  cos_x * cos_z;	_23 =  sin_y * sin_z + sin_x * cos_y * cos_z; _24 = 0.0F;
	_31 =  cos_x * sin_y                        ; _32 = -sin_x        ;	_33 =  cos_x * cos_y                        ; _34 = 0.0F;
	_41 =  0.0F                                 ; _42 =  0.0F         ; _43 =  0.0F                                 ; _44 = 1.0F;

	return *this;
}

LCXMATRIX& LCXMATRIX::RotationYXZ(FLOAT r_x, FLOAT r_y, FLOAT r_z)
{
	//LCXMATRIX	m;
	//LCXMATRIX	mtRx;
	//LCXMATRIX	mtRy;
	//LCXMATRIX	mtRz;
	//this->Identity();
	//mtRx.RotationX(r_x);
	//mtRy.RotationY(r_y);
	//mtRz.RotationZ(r_z);
	//*this = mtRy * mtRx * mtRz;
	//return *this;

	FLOAT cos_x = LCXMathCos(r_x);
	FLOAT sin_x = LCXMathSin(r_x);
	FLOAT cos_y = LCXMathCos(r_y);
	FLOAT sin_y = LCXMathSin(r_y);
	FLOAT cos_z = LCXMathCos(r_z);
	FLOAT sin_z = LCXMathSin(r_z);

	_11 =  cos_y * cos_z - sin_x * sin_y * sin_z; _12 =  cos_y * sin_z + sin_x * sin_y * cos_z; _13 = -cos_x * sin_y; _14 = 0.0F;
	_21 = -cos_x * sin_z                        ; _22 =  cos_x * cos_z                        ; _23 =  sin_x        ; _24 = 0.0F;
	_31 =  sin_y * cos_z + sin_x * cos_y * sin_z; _32 =  sin_y * sin_z - sin_x * cos_y * cos_z; _33 =  cos_x * cos_y; _34 = 0.0F;
	_41 =  0.0F                                 ; _42 =  0.0F                                 ; _43 =  0.0F         ; _44 = 1.0F;

	return *this;
}


LCXMATRIX& LCXMATRIX::RotationXYZ(FLOAT r_x, FLOAT r_y, FLOAT r_z)
{
	FLOAT cos_x = LCXMathCos(r_x);
	FLOAT sin_x = LCXMathSin(r_x);
	FLOAT cos_y = LCXMathCos(r_y);
	FLOAT sin_y = LCXMathSin(r_y);
	FLOAT cos_z = LCXMathCos(r_z);
	FLOAT sin_z = LCXMathSin(r_z);

	_11 =  cos_y * cos_z                        ; _12 =  cos_y * sin_z                        ; _13 = -sin_y        ; _14 = 0.0F;
	_21 = -cos_x * sin_z + sin_x * sin_y * cos_z; _22 =  cos_x * cos_z + sin_x * sin_y * sin_z; _23 =  sin_x * cos_y; _24 = 0.0F;
	_31 =  sin_x * sin_z + cos_x * sin_y * cos_z; _32 = -sin_x * cos_z - cos_x * sin_y * sin_z; _33 =  cos_x * cos_y; _34 = 0.0F;
	_41 =  0.0F                                 ; _42 =  0.0F                                 ; _43 =  0.0F         ; _44 = 1.0F;

	return *this;
}


LCXMATRIX& LCXMATRIX::RotationXZY(FLOAT r_x, FLOAT r_y, FLOAT r_z)
{
	FLOAT cos_x = LCXMathCos(r_x);
	FLOAT sin_x = LCXMathSin(r_x);
	FLOAT cos_y = LCXMathCos(r_y);
	FLOAT sin_y = LCXMathSin(r_y);
	FLOAT cos_z = LCXMathCos(r_z);
	FLOAT sin_z = LCXMathSin(r_z);

	_11 =  cos_y * cos_z                        ; _12 =  sin_z        ; _13 = -sin_y * cos_z                        ; _14 = 0.0F;
	_21 =  sin_x * sin_y - cos_x * cos_y * sin_z; _22 =  cos_x * cos_z; _23 =  sin_x * cos_y + cos_x * sin_y * sin_z; _24 = 0.0F;
	_31 =  cos_x * sin_y + sin_x * cos_y * sin_z; _32 = -sin_x * cos_z; _33 =  cos_x * cos_y - sin_x * sin_y * sin_z; _34 = 0.0F;
	_41 =  0.0F                                 ; _42 =  0.0F         ; _43 =  0.0F                                 ; _44 = 1.0F;

	return *this;
}


LCXMATRIX& LCXMATRIX::RotationYZX(FLOAT r_x, FLOAT r_y, FLOAT r_z)
{
	FLOAT cos_x = LCXMathCos(r_x);
	FLOAT sin_x = LCXMathSin(r_x);
	FLOAT cos_y = LCXMathCos(r_y);
	FLOAT sin_y = LCXMathSin(r_y);
	FLOAT cos_z = LCXMathCos(r_z);
	FLOAT sin_z = LCXMathSin(r_z);

	_11 =  cos_y * cos_z; _12 =  sin_x * sin_y + cos_x * cos_y * sin_z; _13 = -cos_x * sin_y + sin_x * cos_y * sin_z; _14 = 0.0F;
	_21 = -sin_z        ; _22 =  cos_x * cos_z                        ; _23 =  sin_x * cos_z                        ; _24 = 0.0F;
	_31 =  sin_y * cos_z; _32 = -sin_x * cos_y + cos_x * sin_y * sin_z; _33 =  cos_x * cos_y + sin_x * sin_y * sin_z; _34 = 0.0F;
	_41 =  0.0F         ; _42 =  0.0F                                 ; _43 =  0.0F                                 ; _44 = 1.0F;

	return *this;
}


LCXMATRIX& LCXMATRIX::RotationZYX(FLOAT r_x, FLOAT r_y, FLOAT r_z)
{
	FLOAT cos_x = LCXMathCos(r_x);
	FLOAT sin_x = LCXMathSin(r_x);
	FLOAT cos_y = LCXMathCos(r_y);
	FLOAT sin_y = LCXMathSin(r_y);
	FLOAT cos_z = LCXMathCos(r_z);
	FLOAT sin_z = LCXMathSin(r_z);

	_11 =  cos_y * cos_z; _12 =  cos_x * sin_z + sin_x * sin_y * cos_z; _13 =  sin_x * sin_z - cos_x * sin_y * cos_z; _14 = 0.0F;
	_21 = -cos_y * sin_z; _22 =  cos_x * cos_z - sin_x * sin_y * sin_z; _23 =  sin_x * cos_z + cos_x * sin_y * sin_z; _24 = 0.0F;
	_31 =  sin_y        ; _32 = -sin_x * cos_y                        ; _33 =  cos_x * cos_y                        ; _34 = 0.0F;
	_41 =  0.0F         ; _42 =  0.0F                                 ; _43 =  0.0F                                 ; _44 = 1.0F;

	return *this;
}


LCXMATRIX& LCXMATRIX::RotationYawPitchRoll(FLOAT r_y, FLOAT r_x, FLOAT r_z)
{
	return RotationYXZ(r_x, r_y, r_z);
}


LCXMATRIX& LCXMATRIX::RotationAxis(FLOAT x, FLOAT y, FLOAT z, FLOAT Rad)
{
	//FLOAT Cos = (FLOAT)cos(Rad);
	//FLOAT Sin = (FLOAT)sin(Rad);
	FLOAT Cos = LCXMathCos(Rad);
	FLOAT Sin = LCXMathSin(Rad);


	LCXVECTOR3 v(x,y,z);

	v.Normalize();

	//_11 = ( v.x * v.x ) * (1.0F - Cos) + Cos;
	//_12 = ( v.x * v.y ) * (1.0F - Cos) - (v.z * Sin);
	//_13 = ( v.x * v.z ) * (1.0F - Cos) + (v.y * Sin);

	//_21 = ( v.y * v.x ) * (1.0F - Cos) + (v.z * Sin);
	//_22 = ( v.y * v.y ) * (1.0F - Cos) + Cos ;
	//_23 = ( v.y * v.z ) * (1.0F - Cos) - (v.x * Sin);

	//_31 = ( v.z * v.x ) * (1.0F - Cos) - (v.y * Sin);
	//_32 = ( v.z * v.y ) * (1.0F - Cos) + (v.x * Sin);
	//_33 = ( v.z * v.z ) * (1.0F - Cos) + Cos;


	_11 = ( v.x * v.x ) * (1.0F - Cos) + Cos;
	_12 = ( v.y * v.x ) * (1.0F - Cos) + (v.z * Sin);
	_13 = ( v.z * v.x ) * (1.0F - Cos) - (v.y * Sin);

	_21 = ( v.x * v.y ) * (1.0F - Cos) - (v.z * Sin);
	_22 = ( v.y * v.y ) * (1.0F - Cos) + Cos ;
	_23 = ( v.z * v.y ) * (1.0F - Cos) + (v.x * Sin);

	_31 = ( v.x * v.z ) * (1.0F - Cos) + (v.y * Sin);
	_32 = ( v.y * v.z ) * (1.0F - Cos) - (v.x * Sin);
	_33 = ( v.z * v.z ) * (1.0F - Cos) + Cos;

											_14 = 0.0F;
											_24 = 0.0F;
											_34 = 0.0F;
	_41 = 0.0F;  _42 = 0.0F;  _43 = 0.0F;	_44 = 1.0F;

	return *this;
}


LCXMATRIX& LCXMATRIX::RotationAxis(const LCXVECTOR3* vAxis, FLOAT Rad)
{
	return RotationAxis(vAxis->x, vAxis->y, vAxis->z, Rad);
}


LCXMATRIX& LCXMATRIX::RotationAxis(const LCXVECTOR4* vAxis)
{
	return RotationAxis(vAxis->x, vAxis->y, vAxis->z, vAxis->w);
}


LCXMATRIX& LCXMATRIX::Translation(FLOAT x, FLOAT y, FLOAT z, INT bInit)
{
	if(bInit)
	{
		_11 = 1.0F;  _12 = 0.0F;  _13 = 0.0F;  _14 = 0.0F;
		_21 = 0.0F;  _22 = 1.0F;  _23 = 0.0F;  _24 = 0.0F;
		_31 = 0.0F;  _32 = 0.0F;  _33 = 1.0F;  _34 = 0.0F;
		_41 = 0.0F;  _42 = 0.0F;  _43 = 0.0F;  _44 = 1.0F;
	}

	_41 = x;
	_42 = y;
	_43 = z;

	return *this;
}


LCXMATRIX& LCXMATRIX::Translation(const LCXVECTOR3* vTrs, INT bInit)
{
	return Translation(vTrs->x, vTrs->y, vTrs->z, bInit);
}


LCXMATRIX& LCXMATRIX::From4X3(const LCXMATRIX4X3* v)
{
	_11 = v->_11;	_12 = v->_12;	_13 = v->_13;	_14 = 0.0F;
	_21 = v->_21;	_22 = v->_22;	_23 = v->_23;	_24 = 0.0F;
	_31 = v->_31;	_32 = v->_32;	_33 = v->_33;	_34 = 0.0F;
	_41 = v->_41;	_42 = v->_42;	_43 = v->_43;	_44 = 1.0F;

	return *this;
}


LCXMATRIX& LCXMATRIX::From3X3(const LCXMATRIX3X3* v)
{
	_11 = v->_11;	_12 = v->_12;	_13 = 0.0F; _14 = v->_13;
	_21 = v->_21;	_22 = v->_22;	_23 = 0.0F; _24 = v->_23;
	_31 = 0.000F;	_32 = 0.000F;	_33 = 1.0F; _34 = 0.000F;
	_41 = v->_31;	_42 = v->_32;	_43 = 0.0F; _44 = v->_33;

	return *this;
}


LCXMATRIX& LCXMATRIX::From3X2(const LCXMATRIX3X2* v)
{
	_11 = v->_11;	_12 = v->_12;	_13 = 0.0F;	_14 = 0.000F;
	_21 = v->_21;	_22 = v->_22;	_23 = 0.0F;	_24 = 0.000F;
	_31 = 0.000F;	_32 = 0.000F;	_33 = 1.0F; _34 = 0.000F;
	_41 = v->_31;	_42 = v->_32;	_43 = 0.0F;	_44 = 1.000F;

	return *this;
}



void LCXMATRIX::TransformCoord(LCXVECTOR3* Out, const LCXVECTOR3* pIn)
{
	FLOAT _x, _y, _z;
	FLOAT x, y, z, w;

	_x = pIn->x;
	_y = pIn->y;
	_z = pIn->z;

//#if defined(__AOS__)
//	LOGI("LCXMATRIX:: %f %f %f %f\n", _11, _21, _31, _41);
//	LOGI("LCXMATRIX:: %f %f %f %f\n", _12, _22, _32, _42);
//	LOGI("LCXMATRIX:: %f %f %f %f\n", _13, _23, _33, _43);
//	LOGI("LCXMATRIX:: %f %f %f %f\n", _14, _24, _34, _44);
//
//	LOGI("LCXMATRIX::TransformCoord0: %f %f %f\n", _x, _y, _z);
//#endif

	x = _11 * _x +  _21 * _y + _31 * _z   +  _41;
	y = _12 * _x +  _22 * _y + _32 * _z   +  _42;
	z = _13 * _x +  _23 * _y + _33 * _z   +  _43;
	w = _14 * _x +  _24 * _y + _34 * _z   +  _44;

	x /=w;
	y /=w;
	z /=w;
	//w =1.0F;

	Out->x =x;
	Out->y =y;
	Out->z =z;
}


void LCXMATRIX::TransformNormal(LCXVECTOR3* Out, const LCXVECTOR3* pIn)
{
	FLOAT _x, _y, _z;
	FLOAT x, y, z;

	_x = pIn->x;
	_y = pIn->y;
	_z = pIn->z;

	x = _11 * _x +  _21 * _y + _31 * _z;
	y = _12 * _x +  _22 * _y + _32 * _z;
	z = _13 * _x +  _23 * _y + _33 * _z;

	Out->x =x;
	Out->y =y;
	Out->z =z;
}


LCXMATRIX& LCXMATRIX::Negative()
{
	_11 = -_11;  _12 = -_12;  _13 = -_13;  _14 = -_14;
	_21 = -_21;  _22 = -_22;  _23 = -_23;  _24 = -_24;
	_31 = -_31;  _32 = -_32;  _33 = -_33;  _34 = -_34;
	_41 = -_41;  _42 = -_42;  _43 = -_43;  _44 = -_44;

	return *this;
}


LCXMATRIX& LCXMATRIX::Lerp(const LCXMATRIX* m1, const LCXMATRIX* m2, FLOAT t) // this = (1-t)*v1 + t* v2
{
	FLOAT k = 1.0F - t;
	_11 = k * m1->_11 + t * m2->_11;  _12 = k * m1->_12 + t * m2->_12;  _13 = k * m1->_13 + t * m2->_13;  _14 = k * m1->_14 + t * m2->_14;
	_21 = k * m1->_21 + t * m2->_21;  _22 = k * m1->_22 + t * m2->_22;  _23 = k * m1->_23 + t * m2->_23;  _24 = k * m1->_24 + t * m2->_24;
	_31 = k * m1->_31 + t * m2->_31;  _32 = k * m1->_32 + t * m2->_32;  _33 = k * m1->_33 + t * m2->_33;  _34 = k * m1->_34 + t * m2->_34;
	_41 = k * m1->_41 + t * m2->_41;  _42 = k * m1->_42 + t * m2->_42;  _43 = k * m1->_43 + t * m2->_43;  _44 = k * m1->_34 + t * m2->_34;

	return *this;
}


LCXMATRIX& LCXMATRIX::Lerp(const LCXMATRIX4X3* m1, const LCXMATRIX4X3* m2, FLOAT t) // this = (1-t)*v1 + t* v2
{
	FLOAT k = 1.0F - t;
	_11 = k * m1->_11 + t * m2->_11;  _12 = k * m1->_12 + t * m2->_12;  _13 = k * m1->_13 + t * m2->_13;  _14 = 0.0F;
	_21 = k * m1->_21 + t * m2->_21;  _22 = k * m1->_22 + t * m2->_22;  _23 = k * m1->_23 + t * m2->_23;  _24 = 0.0F;
	_31 = k * m1->_31 + t * m2->_31;  _32 = k * m1->_32 + t * m2->_32;  _33 = k * m1->_33 + t * m2->_33;  _34 = 0.0F;
	_41 = k * m1->_41 + t * m2->_41;  _42 = k * m1->_42 + t * m2->_42;  _43 = k * m1->_43 + t * m2->_43;  _44 = 1.0F;

	return *this;
}




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// Quaternion

LCXQUATERNION::LCXQUATERNION()						{	x=0.0F; y=0.0F; z=0.0F; w=1.0F;		}
LCXQUATERNION::LCXQUATERNION(const FLOAT* v)		{	if(!v) return; x=v[0]; y=v[1]; z=v[2]; w=v[3];	}
LCXQUATERNION::LCXQUATERNION(const LCXQUATERNION* v){	if(!v) return; x=v->x; y=v->y; z=v->z; w=v->w;	}
LCXQUATERNION::LCXQUATERNION(FLOAT X,FLOAT Y,FLOAT Z,FLOAT W){	x=X; y=Y; z=Z; w=W;	}
LCXQUATERNION::LCXQUATERNION(const LC_FLOAT4& v)	{	x=v.x; y=v.y; z=v.z; w=v.w;			}

LCXQUATERNION::LCXQUATERNION(const LCXQUATERNION& v)   {   *this = v;      }
LCXQUATERNION& LCXQUATERNION::operator=(const LCXQUATERNION& v)
{
	x=v.x; y=v.y; z=v.z; w=v.w;
	return *this;
}


// casting
LCXQUATERNION::operator FLOAT* ()				{	return (FLOAT *) &x;		}
LCXQUATERNION::operator const FLOAT* () const	{	return (const FLOAT *) &x;	}
FLOAT& LCXQUATERNION::operator[](int n)			{	return *((&x)+ n);			}

// assignment operators
LCXQUATERNION& LCXQUATERNION::operator += (const LCXQUATERNION& q ){ x +=q.x; y +=q.y; z +=q.z; w +=q.w; return *this;	}
LCXQUATERNION& LCXQUATERNION::operator -= (const LCXQUATERNION& q ){ x -=q.x; y -=q.y; z -=q.z; w -=q.w; return *this;	}
LCXQUATERNION& LCXQUATERNION::operator *= (const LCXQUATERNION& q )
{
	LCXQUATERNION q1 = *this;
	LCXQUATERNION q2 = q;

	w = q1.w * q2.w - ( q1.x * q2.x + q1.y * q2.y + q1.z * q2.z);

	x = q1.w * q2.x + q2.w * q1.x + (q1.y * q2.z - q1.z * q2.y);
	y = q1.w * q2.y + q2.w * q1.y + (q1.z * q2.x - q1.x * q2.z);
	z = q1.w * q2.z + q2.w * q1.z + (q1.x * q2.y - q1.y * q2.x);

	return *this;
}

LCXQUATERNION& LCXQUATERNION::operator *= ( FLOAT f){					x *=f; y *=f; z *=f; w *=f; return *this;	}
LCXQUATERNION& LCXQUATERNION::operator /= ( FLOAT f){	FLOAT t=1.0F/f;	x *=t; y *=t; z *=t; w *=t; return *this;	}

// unary operators
LCXQUATERNION LCXQUATERNION::operator +() const	{	return *this;							}
LCXQUATERNION LCXQUATERNION::operator -() const	{	return LCXQUATERNION(-x, -y, -z, -w);	}

// binary operators
LCXQUATERNION LCXQUATERNION::operator +(const LCXQUATERNION& v) const {	return LCXQUATERNION(x + v.x, y + v.y, z + v.z, w + v.w);	}
LCXQUATERNION LCXQUATERNION::operator -(const LCXQUATERNION& v) const {	return LCXQUATERNION(x - v.x, y - v.y, z - v.z, w - v.w);	}
LCXQUATERNION LCXQUATERNION::operator *(const LCXQUATERNION& q) const
{
	LCXQUATERNION q1 = *this;
	LCXQUATERNION q2 = q;

	LCXQUATERNION qT;

	qT.w = q1.w * q2.w - ( q1.x * q2.x + q1.y * q2.y + q1.z * q2.z);

	qT.x = q1.w * q2.x + q2.w * q1.x + (q1.y * q2.z - q1.z * q2.y);
	qT.y = q1.w * q2.y + q2.w * q1.y + (q1.z * q2.x - q1.x * q2.z);
	qT.z = q1.w * q2.z + q2.w * q1.z + (q1.x * q2.y - q1.y * q2.x);

	return qT;
}

LCXQUATERNION LCXQUATERNION::operator *(FLOAT v) const	{	return LCXQUATERNION(x * v  , y * v  , z * v  , w * v  );	}
LCXQUATERNION operator *(FLOAT f, const LCXQUATERNION& v){	return LCXQUATERNION(f * v.x, f * v.y, f * v.z, f * v.w);	}
LCXQUATERNION LCXQUATERNION::operator /(FLOAT v) const	{	return LCXQUATERNION(x / v  , y / v  , z / v  , w / v  );	}


//INT LCXQUATERNION::operator == (const LCXQUATERNION& v)const{	return x == v.x && y == v.y && z == v.z && w == v.w;	}
//INT LCXQUATERNION::operator != (const LCXQUATERNION& v)const{	return x != v.x || y != v.y || z != v.z || w != v.w;	}

INT LCXQUATERNION::operator ==(const LCXQUATERNION& v)const{ return LCXIsEqual(x, v.x)    && LCXIsEqual(y, v.y)    && LCXIsEqual(z, v.z)    && LCXIsEqual(w,v.w);	}
INT LCXQUATERNION::operator !=(const LCXQUATERNION& v)const{ return LCXIsNotEqual(x,v.x) || LCXIsNotEqual(y,v.y) || LCXIsNotEqual(z,v.z) || LCXIsNotEqual(w,v.w);	}



LCXQUATERNION& LCXQUATERNION::Identity()
{
	x=0.0F; y=0.0F; z=0.0F; w=1.0F;
	return *this;
}


LCXQUATERNION& LCXQUATERNION::Conjugate()
{
	x=-x; y=-y; z=-z;
	return *this;
}


LCXQUATERNION& LCXQUATERNION::Negative()
{
	x=-x; y=-y; z=-z; w=-w;
	return *this;
}


void LCXQUATERNION::SLerp(const LCXQUATERNION* q1, const LCXQUATERNION* q2, FLOAT t)
{
	//Q = (1/sin T )[  sin ( T *(1-t) ) * Q1 + sin ( T * t ) * Q2]

	FLOAT w1;
	FLOAT w2;
	FLOAT dt = q1->x * q2->x + q1->y * q2->y + q1->z * q2->z + q1->w * q2->w;


	if( dt<-0.9999F || dt > 0.9999F)
	{
		w1 = 1.0F - t;
		w2 = t;
	}
	else
	{
		//FLOAT Tht = (FLOAT)acos( fabsf(dt) );
		//FLOAT Sin = (FLOAT)sin ( Tht );
		//w1 = (FLOAT)sin( (1.0F-t) * Tht) / Sin;
		//w2 = (FLOAT)sin(       t  * Tht) / Sin;

		FLOAT Tht = LCXMathAcos( fabsf(dt) );
		FLOAT Sin = LCXMathSin ( Tht );

		w1 = LCXMathSin( (1.0F-t) * Tht) / Sin;
		w2 = LCXMathSin(       t  * Tht) / Sin;
	}

	if( dt < 0 )
		w2 = -w2;

	*this = *q1 * w1 + *q2 * w2;
}


void LCXQUATERNION::RotationMatrix(LCXMATRIX* Out, INT bLHR)
{
	FLOAT xx = x * x;
	FLOAT yy = y * y;
	FLOAT zz = z * z;

	FLOAT xy = x * y;
	FLOAT yz = y * z;
	FLOAT xz = z * x;

	FLOAT xw = x * w;
	FLOAT yw = y * w;
	FLOAT zw = z * w;


	if(bLHR)
	{
		// D3D
		Out->_11 = 1 - 2*yy - 2*zz;	Out->_21 =     2*xy - 2*zw;	Out->_31 =     2*xz + 2*yw;
		Out->_12 =     2*xy + 2*zw;	Out->_22 = 1 - 2*xx - 2*zz;	Out->_32 =     2*yz - 2*xw;
		Out->_13 =     2*xz - 2*yw;	Out->_23 =     2*yz + 2*xw;	Out->_33 = 1 - 2*xx - 2*yy;
	}
	else
	{
		Out->_11 = 1 - 2*yy - 2*zz;	Out->_12 =     2*xy - 2*zw;	Out->_13 =     2*xz + 2*yw;
		Out->_21 =     2*xy + 2*zw;	Out->_22 = 1 - 2*xx - 2*zz;	Out->_23 =     2*yz - 2*xw;
		Out->_31 =     2*xz - 2*yw;	Out->_32 =     2*yz + 2*xw;	Out->_33 = 1 - 2*xx - 2*yy;
	}


															Out->_14 = 0.0F;
															Out->_24 = 0.0F;
															Out->_34 = 0.0F;
	Out->_41 = 0.0F;  Out->_42 = 0.0F;  Out->_43 = 0.0F;    Out->_44 = 1.0F;
}

const LCXMATRIX LCXMATRIX::ZERO     = LCXMATRIX(0.0F, 0.0F, 0.0F, 0.0F,
												0.0F, 0.0F, 0.0F, 0.0F,
												0.0F, 0.0F, 0.0F, 0.0F,
												0.0F, 0.0F, 0.0F, 0.0F);

const LCXMATRIX LCXMATRIX::IDENTITY = LCXMATRIX(1.0F, 0.0F, 0.0F, 0.0F,
												0.0F, 1.0F, 0.0F, 0.0F,
												0.0F, 0.0F, 1.0F, 0.0F,
												0.0F, 0.0F, 0.0F, 1.0F);




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// Color

LCXCOLOR::LCXCOLOR()							{	r=1.0F; g=1.0F; b=1.0F; a=1.0F;	}
LCXCOLOR::LCXCOLOR(const FLOAT* v){	if(!v) return;	r=v[0];	g=v[1];	b=v[2];	a=v[3];	}
LCXCOLOR::LCXCOLOR(const LCXCOLOR* v){if(!v)return; r=v->r; g=v->g; b=v->b; a=v->a;	}
LCXCOLOR::LCXCOLOR(COLOR32 v)
{
	const FLOAT f = 1.0F / 255.0F;
	r = f * (FLOAT) ( (BYTE) (v >> 16) );
	g = f * (FLOAT) ( (BYTE) (v >>  8) );
	b = f * (FLOAT) ( (BYTE) (v >>  0) );
	a = f * (FLOAT) ( (BYTE) (v >> 24) );
}

LCXCOLOR::LCXCOLOR(const BYTE* v/*r[0],g[1],b[2],a[3]*/)
{
	r = FLOAT(v[0])/255.0F;
	g = FLOAT(v[1])/255.0F;
	b = FLOAT(v[2])/255.0F;
	a = FLOAT(v[3])/255.0F;
}

LCXCOLOR::LCXCOLOR(FLOAT R,FLOAT G,FLOAT B,FLOAT A)	{	r=R; g=G; b=B; a=A; }
LCXCOLOR::LCXCOLOR(const LC_COLORF& v)				{	r=v.r; g=v.g; b=v.b; a=v.a; }

LCXCOLOR::LCXCOLOR(const LCXCOLOR& v)   {   *this = v;      }
LCXCOLOR& LCXCOLOR::operator=(const LCXCOLOR& v)
{
	r=v.r ; g=v.g ; b=v.b ; a=v.a ;
	return *this;
}


// casting
LCXCOLOR::operator COLOR32 () const
{
	COLOR32 A = a >= 1.0F ? 0xff : a <= 0.0F ? 0x00 : (COLOR32) (a * 255.0F + 0.5f);
	COLOR32 R = r >= 1.0F ? 0xff : r <= 0.0F ? 0x00 : (COLOR32) (r * 255.0F + 0.5f);
	COLOR32 G = g >= 1.0F ? 0xff : g <= 0.0F ? 0x00 : (COLOR32) (g * 255.0F + 0.5f);
	COLOR32 B = b >= 1.0F ? 0xff : b <= 0.0F ? 0x00 : (COLOR32) (b * 255.0F + 0.5f);
	return (A << 24) | (R << 16) | (G << 8) | B;
}

LCXCOLOR::operator FLOAT *()			{	return (FLOAT*) &r;			}
LCXCOLOR::operator const FLOAT *() const{	return (const FLOAT*) &r;	}
FLOAT& LCXCOLOR::operator[](int n)		{	return *((&r)+ n);			}

// assignment operators
LCXCOLOR& LCXCOLOR::operator +=(const LCXCOLOR& v){	r +=v.r; g +=v.g; b +=v.b; a +=v.a;	return *this;	}
LCXCOLOR& LCXCOLOR::operator -=(const LCXCOLOR& v){	r -=v.r; g -=v.g; b -=v.b; a -=v.a; return *this;	}
LCXCOLOR& LCXCOLOR::operator *=(const LCXCOLOR& v){	r *=v.r; g *=v.g; b *=v.b; a *=v.a; return *this;	}
LCXCOLOR& LCXCOLOR::operator *=(FLOAT f)		{	r *=f;   g *=f;   b *=f;   a *= f;	return *this;	}
LCXCOLOR& LCXCOLOR::operator /=(FLOAT f){FLOAT t=1.0F/f; r *=t; g *=t; b *=t; a *=t;	return *this;	}

// unary operators
LCXCOLOR LCXCOLOR::operator +() const	{	return *this;								}
LCXCOLOR LCXCOLOR::operator -() const	{	return LCXCOLOR(1.0F-r, 1.0F-g, 1.0F-b, a);	}	// Negative

// binary operators
LCXCOLOR LCXCOLOR::operator +(const LCXCOLOR& v) const{		return LCXCOLOR(r + v.r, g + v.g, b + v.b, a + v.a);	}	// Add
LCXCOLOR LCXCOLOR::operator -(const LCXCOLOR& v) const{		return LCXCOLOR(r - v.r, g - v.g, b - v.b, a - v.a);	}	// Subtract
LCXCOLOR LCXCOLOR::operator *(const LCXCOLOR& v) const{		return LCXCOLOR(r * v.r, g * v.g, b * v.b, a * v.a);	}	// v
LCXCOLOR LCXCOLOR::operator *( FLOAT f)const		{		return LCXCOLOR(r * f  , g * f  , b * f  , a * f  );	}	// Modulate
LCXCOLOR LCXCOLOR::operator /( FLOAT f)const{FLOAT t=1.0F/f;return LCXCOLOR(r * t  , g * t  , b * t  , a * t  );	}	// Devide
LCXCOLOR operator *(FLOAT f, const LCXCOLOR& v)	{			return LCXCOLOR(f * v.r, f * v.g, f * v.b, f * v.a);	}	// Scaling

//INT LCXCOLOR::operator == (const LCXCOLOR& v) const	{	return r == v.r && g == v.g && b == v.b && a == v.a;	}
//INT LCXCOLOR::operator != (const LCXCOLOR& v) const	{	return r != v.r || g != v.g || b != v.b || a != v.a;	}

INT LCXCOLOR::operator ==(const LCXCOLOR& v) const{	return LCXIsEqual(r, v.r)   && LCXIsEqual(g, v.g)   && LCXIsEqual(b, v.b)    && LCXIsEqual(a, v.a);	}
INT LCXCOLOR::operator !=(const LCXCOLOR& v) const{	return LCXIsNotEqual(r,v.r) || LCXIsNotEqual(g,v.g) || LCXIsNotEqual(b,v.b) || LCXIsNotEqual(a,v.a);}



LCXCOLOR& LCXCOLOR::Negative()
{
	r = 1.0F - r;
	g = 1.0F - g;
	b = 1.0F - b;
    return *this;
}

LCXCOLOR& LCXCOLOR::Lerp(const LCXCOLOR* v1, const LCXCOLOR* v2, FLOAT t) // this = (1-t)*v1 + t* v2
{
	FLOAT k = 1.0F - t;
	r = k * v1->r + t * v2->r;
	g = k * v1->g + t * v2->g;
	b = k * v1->b + t * v2->b;
	a = k * v1->a + t * v2->a;
	return *this;
}

LCXCOLOR& LCXCOLOR::FF000000(){	r=0.0F; g=0.0F; b=0.0F; a=1.0F; return *this;}
LCXCOLOR& LCXCOLOR::FFFFFFFF(){	r=1.0F; g=1.0F; b=1.0F; a=1.0F; return *this;}
LCXCOLOR& LCXCOLOR::FFFF0000(){	r=1.0F; g=0.0F; b=0.0F; a=1.0F; return *this;}
LCXCOLOR& LCXCOLOR::FF00FF00(){	r=0.0F; g=1.0F; b=0.0F; a=1.0F; return *this;}
LCXCOLOR& LCXCOLOR::FF0000FF(){	r=0.0F; g=0.0F; b=1.0F; a=1.0F; return *this;}
LCXCOLOR& LCXCOLOR::FFFFFF00(){	r=1.0F; g=1.0F; b=0.0F; a=1.0F; return *this;}
LCXCOLOR& LCXCOLOR::FFFF00FF(){	r=1.0F; g=0.0F; b=1.0F; a=1.0F; return *this;}
LCXCOLOR& LCXCOLOR::FF00FFFF(){	r=0.0F; g=1.0F; b=1.0F; a=1.0F; return *this;}
LCXCOLOR& LCXCOLOR::FF800080(){	r=0.5F; g=0.0F; b=0.5F; a=1.0F; return *this;}
LCXCOLOR& LCXCOLOR::FF8F00FF(){	r=0.5F; g=0.0F; b=0.5F; a=1.0F; return *this;}

const LCXCOLOR	LCXCOLOR::BLACK  (0.0F, 0.0F, 0.0F, 1.0F);
const LCXCOLOR	LCXCOLOR::WHITE  (1.0F, 1.0F, 1.0F, 1.0F);
const LCXCOLOR	LCXCOLOR::RED    (1.0F, 0.0F, 0.0F, 1.0F);
const LCXCOLOR	LCXCOLOR::GREEN  (0.0F, 1.0F, 0.0F, 1.0F);
const LCXCOLOR	LCXCOLOR::BLUE   (0.0F, 0.0F, 1.0F, 1.0F);
const LCXCOLOR	LCXCOLOR::YELLOW (1.0F, 1.0F, 0.0F, 1.0F);
const LCXCOLOR	LCXCOLOR::MAGENTA(1.0F, 0.0F, 1.0F, 1.0F);
const LCXCOLOR	LCXCOLOR::CYAN   (0.0F, 1.0F, 1.0F, 1.0F);
const LCXCOLOR	LCXCOLOR::PURPLE (0.5F, 0.0F, 0.5F, 1.0F);
const LCXCOLOR	LCXCOLOR::VIOLET (0.5F, 0.0F, 0.5F, 1.0F);




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// Plane(3D)

LCXPLANE::LCXPLANE()	{	a =0.0F; b = 0.0F; c = 0.0F; d = 0.0F;	}

LCXPLANE::LCXPLANE(const LCXVECTOR3& p0, const LCXVECTOR3& p1, const LCXVECTOR3& p2)
{
	LCXVECTOR3	l0 = ( p1 -  p0);
	LCXVECTOR3	l1 = ( p2 -  p0);
	LCXVECTOR3*	n  = (LCXVECTOR3*)&this->m[0];

	LCXVec3Cross(n, &l0, &l1);
	n->Normalize();

	d  =-( p0.x * n->x + p0.y + n->y + p0.z * n->z);
}

LCXPLANE::LCXPLANE(const LCXPLANE& v)   {   *this = v;      }
LCXPLANE& LCXPLANE::operator=(const LCXPLANE& v)
{
	a =v.a;	 b = v.b;  c = v.c;  d = v.d;
	return *this;
}


LCXPLANE::LCXPLANE(const FLOAT* v)					{	a =v[0];b = v[1];c = v[2];	d = v[3];	}
LCXPLANE::LCXPLANE( FLOAT A,FLOAT B,FLOAT C,FLOAT D){	a =A;	b = B;	 c = C;		d = D;		}

// casting
LCXPLANE::operator FLOAT* ()			{	return (FLOAT*) &a;			}
LCXPLANE::operator const FLOAT* () const{	return (const FLOAT*) &a;	}
FLOAT& LCXPLANE::operator[](int n)		{	return *((&a)+ n);			}

// unary operators
LCXPLANE LCXPLANE::operator + () const	{	return *this;					}
LCXPLANE LCXPLANE::operator - () const	{	return LCXPLANE(-a,-b,-c,-d);	}

// binary operators
INT LCXPLANE::operator ==(const LCXPLANE& p)const{ return a == p.a && b == p.b && c == p.c && d == p.d;	}
INT LCXPLANE::operator !=(const LCXPLANE& p)const{ return a != p.a || b != p.b || c != p.c || d != p.d;	}


void LCXPLANE::FromPointNormal(const LCXVECTOR3* point, const LCXVECTOR3* nor)
{
	a  = nor->x;
	b  = nor->y;
	c  = nor->z;
	d  =-LCXVec3Dot(point, nor);
}


void LCXPLANE::FromPoints(const LCXVECTOR3* p0, const LCXVECTOR3* p1, const LCXVECTOR3* p2)
{
	LCXVECTOR3	l0 = (*p1 - *p0);
	LCXVECTOR3	l1 = (*p2 - *p0);
	LCXVECTOR3*	n  = (LCXVECTOR3*)&this->m[0];

	LCXVec3Cross(n, &l0, &l1);
	n->Normalize();

	d  =-LCXVec3Dot(p0, n);
}

LCXPLANE2::LCXPLANE2()                   {	a =0.0F; b = 0.0F; d = 0.0F;	}
LCXPLANE2::LCXPLANE2(const LCXPLANE2& v) {   *this = v;      }
LCXPLANE2& LCXPLANE2::operator=(const LCXPLANE2& v)
{
	a =v.a;	 b = v.b;  d = v.d;
	return *this;
}


LCXPLANE2::LCXPLANE2(const LCXVECTOR2& p0, const LCXVECTOR2& p1)
{
	LCXVECTOR2	l0 = (p1 - p0);

	l0.Normalize();
	d = -( p0.x * l0.x + p0.y + l0.y);
}

LCXPLANE2::LCXPLANE2(const FLOAT* v)			{	a =v[0];b = v[1];d = v[2];	}
LCXPLANE2::LCXPLANE2( FLOAT A,FLOAT B,FLOAT D)	{	a =A;	b = B;	 d = D;		}

// casting
LCXPLANE2::operator FLOAT* ()			{	return (FLOAT*) &a;			}
LCXPLANE2::operator const FLOAT* () const{	return (const FLOAT*) &a;	}
FLOAT& LCXPLANE2::operator[](int n)		{	return *((&a)+ n);			}

// unary operators
LCXPLANE2 LCXPLANE2::operator + () const{	return *this;				}
LCXPLANE2 LCXPLANE2::operator - () const{	return LCXPLANE2(-a,-b,-d);	}

// binary operators
INT LCXPLANE2::operator ==(const LCXPLANE2& p)const{ return a == p.a && b == p.b && d == p.d;	}
INT LCXPLANE2::operator !=(const LCXPLANE2& p)const{ return a != p.a || b != p.b || d != p.d;	}


void LCXPLANE2::PointNormal(const LCXVECTOR2* point, const LCXVECTOR2* nor)
{
	a  = nor->x;
	b  = nor->y;
	d  =-( point->x * a + point->y * b);
}


void LCXPLANE2::Points(const LCXVECTOR2* p0, const LCXVECTOR2* p1)
{
	LCXVECTOR2	L = (*p1 - *p0);

	L.Normalize();
	d = -( p0->x * L.x + p0->y + L.y);
}




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// Matrix 4X3

LCXMATRIX4X3::LCXMATRIX4X3()
{
	_11 = 1.0F;  _12 = 0.0F;  _13 = 0.0F;
	_21 = 0.0F;  _22 = 1.0F;  _23 = 0.0F;
	_31 = 0.0F;  _32 = 0.0F;  _33 = 1.0F;
	_41 = 0.0F;  _42 = 0.0F;  _43 = 0.0F;
}

LCXMATRIX4X3::LCXMATRIX4X3(const FLOAT* v)
{
	if(!v)
		return;

	_11 = v[ 0]; _12 = v[ 1]; _13 = v[ 2];
	_21 = v[ 3]; _22 = v[ 4]; _23 = v[ 5];
	_31 = v[ 6]; _32 = v[ 7]; _33 = v[ 8];
	_41 = v[ 9]; _42 = v[10]; _43 = v[11];
}

LCXMATRIX4X3::LCXMATRIX4X3(const LCXMATRIX4X3* v)
{
	if(!v)
		return;

	_11 = v->_11;	_12 = v->_12;	_13 = v->_13;
	_21 = v->_21;	_22 = v->_22;	_23 = v->_23;
	_31 = v->_31;	_32 = v->_32;	_33 = v->_33;
	_41 = v->_41;	_42 = v->_42;	_43 = v->_43;
}



LCXMATRIX4X3::LCXMATRIX4X3(const LCXVECTOR3* v)
{
	if(!v)
		return;

	_11 = v[0].x;	_12 = v[0].y;	_13 = v[0].z;
	_21 = v[1].x;	_22 = v[1].y;	_23 = v[1].z;
	_31 = v[2].x;	_32 = v[2].y;	_33 = v[2].z;
	_41 = v[3].x;	_42 = v[3].y;	_43 = v[3].z;
}


LCXMATRIX4X3::LCXMATRIX4X3(const LCXMATRIX& v)
{
	_11 = v._11;	_12 = v._12;	_13 = v._13;
	_21 = v._21;	_22 = v._22;	_23 = v._23;
	_31 = v._31;	_32 = v._32;	_33 = v._33;
	_41 = v._41;	_42 = v._42;	_43 = v._43;
}

LCXMATRIX4X3::LCXMATRIX4X3(const LCXMATRIX* v)
{
	if(!v)
		return;

	_11 = v->_11;	_12 = v->_12;	_13 = v->_13;
	_21 = v->_21;	_22 = v->_22;	_23 = v->_23;
	_31 = v->_31;	_32 = v->_32;	_33 = v->_33;
	_41 = v->_41;	_42 = v->_42;	_43 = v->_43;
}




LCXMATRIX4X3::LCXMATRIX4X3(
					 FLOAT v11, FLOAT v12, FLOAT v13,
					 FLOAT v21, FLOAT v22, FLOAT v23,
					 FLOAT v31, FLOAT v32, FLOAT v33,
					 FLOAT v41, FLOAT v42, FLOAT v43
					 )
{
	_11 = v11;	_12 = v12;	_13 = v13;
	_21 = v21;	_22 = v22;	_23 = v23;
	_31 = v31;	_32 = v32;	_33 = v33;
	_41 = v41;	_42 = v42;	_43 = v43;
}


LCXMATRIX4X3::LCXMATRIX4X3(const LC_MATRIX& v)
{
	_11 = v._11;	_12 = v._12;	_13 = v._13;
	_21 = v._21;	_22 = v._22;	_23 = v._23;
	_31 = v._31;	_32 = v._32;	_33 = v._33;
	_41 = v._41;	_42 = v._42;	_43 = v._43;
}

LCXMATRIX4X3::LCXMATRIX4X3(const LCXMATRIX4X3& v)   {   *this = v;      }
LCXMATRIX4X3& LCXMATRIX4X3::operator=(const LCXMATRIX4X3& v)
{
	_11 = v._11; _12 = v._12; _13 = v._13;
	_21 = v._21; _22 = v._22; _23 = v._23;
	_31 = v._31; _32 = v._32; _33 = v._33;
	_41 = v._41; _42 = v._42; _43 = v._43;
	return *this;
}

// access grants
FLOAT& LCXMATRIX4X3::operator()(int iRow, int iCol)	{      return m[iRow][iCol];	}
FLOAT  LCXMATRIX4X3::operator()(int iRow, int iCol) const{ return m[iRow][iCol];	}

// casting operators
LCXMATRIX4X3::operator FLOAT*()				{	return(FLOAT*)&_11;		}
LCXMATRIX4X3::operator const FLOAT*() const	{	return(FLOAT*)&_11;		}
FLOAT& LCXMATRIX4X3::operator[](int n)		{	return *((&_11)+ n);	}

// assignment operators
LCXMATRIX4X3& LCXMATRIX4X3::operator *=(const LCXMATRIX4X3& v)
{
	LCXMATRIX4X3 t = *this;
	_11 = t._11 * v._11 +  t._12 * v._21 +  t._13 * v._31;	// +  0.0F * v._41;
	_12 = t._11 * v._12 +  t._12 * v._22 +  t._13 * v._32;	// +  0.0F * v._42;
	_13 = t._11 * v._13 +  t._12 * v._23 +  t._13 * v._33;	// +  0.0F * v._43;

	_21 = t._21 * v._11 +  t._22 * v._21 +  t._23 * v._31;	// +  0.0F * v._41;
	_22 = t._21 * v._12 +  t._22 * v._22 +  t._23 * v._32;	// +  0.0F * v._42;
	_23 = t._21 * v._13 +  t._22 * v._23 +  t._23 * v._33;	// +  0.0F * v._43;

	_31 = t._31 * v._11 +  t._32 * v._21 +  t._33 * v._31;	// +  0.0F * v._41;
	_32 = t._31 * v._12 +  t._32 * v._22 +  t._33 * v._32;	// +  0.0F * v._42;
	_33 = t._31 * v._13 +  t._32 * v._23 +  t._33 * v._33;	// +  0.0F * v._43;

	_41 = t._41 * v._11 +  t._42 * v._21 +  t._43 * v._31 + v._41; // 1.0F * v._41;
	_42 = t._41 * v._12 +  t._42 * v._22 +  t._43 * v._32 + v._42; // 1.0F * v._42;
	_43 = t._41 * v._13 +  t._42 * v._23 +  t._43 * v._33 + v._43; // 1.0F * v._43;

	return * this;
}

LCXMATRIX4X3& LCXMATRIX4X3::operator +=(const LCXMATRIX4X3& v)
{
	_11 += v._11;	_12 += v._12;	_13 += v._13;
	_21 += v._21;	_22 += v._22;	_23 += v._23;
	_31 += v._31;	_32 += v._32;	_33 += v._33;
	_41 += v._41;	_42 += v._42;	_43 += v._43;

	return * this;
}

LCXMATRIX4X3& LCXMATRIX4X3::operator -=(const LCXMATRIX4X3& v)
{
	_11 -= v._11;	_12 -= v._12;	_13 -= v._13;
	_21 -= v._21;	_22 -= v._22;	_23 -= v._23;
	_31 -= v._31;	_32 -= v._32;	_33 -= v._33;
	_41 -= v._41;	_42 -= v._42;	_43 -= v._43;

	return * this;
}

LCXMATRIX4X3& LCXMATRIX4X3::operator *=(FLOAT v)
{
	_11 *= v;	_12 *= v;	_13 *= v;
	_21 *= v;	_22 *= v;	_23 *= v;
	_31 *= v;	_32 *= v;	_33 *= v;
	_41 *= v;	_42 *= v;	_43 *= v;

	return * this;
}


LCXMATRIX4X3& LCXMATRIX4X3::operator /=(FLOAT v)
{
	v = 1.0F/v;
	_11 *= v;	_12 *= v;	_13 *= v;
	_21 *= v;	_22 *= v;	_23 *= v;
	_31 *= v;	_32 *= v;	_33 *= v;
	_41 *= v;	_42 *= v;	_43 *= v;

	return * this;
}


// unary operators
LCXMATRIX4X3 LCXMATRIX4X3::operator +() const
{
	return *this;
}

LCXMATRIX4X3 LCXMATRIX4X3::operator -() const
{
	return LCXMATRIX4X3(
		-_11, -_12, -_13,
		-_21, -_22, -_23,
		-_31, -_32, -_33,
		-_41, -_42, -_43);
}


// binary operators
LCXMATRIX4X3 LCXMATRIX4X3::operator *(const LCXMATRIX4X3& v) const
{
	LCXMATRIX4X3 t = *this;
	LCXMATRIX4X3 r;
	r._11 = t._11 * v._11 +  t._12 * v._21 +  t._13 * v._31;
	r._12 = t._11 * v._12 +  t._12 * v._22 +  t._13 * v._32;
	r._13 = t._11 * v._13 +  t._12 * v._23 +  t._13 * v._33;

	r._21 = t._21 * v._11 +  t._22 * v._21 +  t._23 * v._31;
	r._22 = t._21 * v._12 +  t._22 * v._22 +  t._23 * v._32;
	r._23 = t._21 * v._13 +  t._22 * v._23 +  t._23 * v._33;

	r._31 = t._31 * v._11 +  t._32 * v._21 +  t._33 * v._31;
	r._32 = t._31 * v._12 +  t._32 * v._22 +  t._33 * v._32;
	r._33 = t._31 * v._13 +  t._32 * v._23 +  t._33 * v._33;

	r._41 = t._41 * v._11 +  t._42 * v._21 +  t._43 * v._31 +  v._41;
	r._42 = t._41 * v._12 +  t._42 * v._22 +  t._43 * v._32 +  v._42;
	r._43 = t._41 * v._13 +  t._42 * v._23 +  t._43 * v._33 +  v._43;

	return r;
}

LCXMATRIX4X3 LCXMATRIX4X3::operator +(const LCXMATRIX4X3& v) const
{
	return LCXMATRIX4X3(
		_11 + v._11, _12 + v._12, _13 + v._13,
		_21 + v._21, _22 + v._22, _23 + v._23,
		_31 + v._31, _32 + v._32, _33 + v._33,
		_41 + v._41, _42 + v._42, _43 + v._43
		);
}

LCXMATRIX4X3 LCXMATRIX4X3::operator -(const LCXMATRIX4X3& v) const
{
	return LCXMATRIX4X3(
		_11 - v._11, _12 - v._12, _13 - v._13,
		_21 - v._21, _22 - v._22, _23 - v._23,
		_31 - v._31, _32 - v._32, _33 - v._33,
		_41 - v._41, _42 - v._42, _43 - v._43
		);
}

LCXMATRIX4X3 LCXMATRIX4X3::operator *(FLOAT v) const
{
	return LCXMATRIX4X3(
		_11 * v, _12 * v, _13 * v,
		_21 * v, _22 * v, _23 * v,
		_31 * v, _32 * v, _33 * v,
		_41 * v, _42 * v, _43 * v
		);
}

LCXMATRIX4X3 LCXMATRIX4X3::operator /(FLOAT v) const
{
	v = 1.0F / v;
	return LCXMATRIX4X3(
		_11 * v, _12 * v, _13 * v,
		_21 * v, _22 * v, _23 * v,
		_31 * v, _32 * v, _33 * v,
		_41 * v, _42 * v, _43 * v
		);
}



LCXMATRIX4X3 operator *(FLOAT f, const LCXMATRIX4X3& v)
{
	return LCXMATRIX4X3(
		f * v._11, f * v._12, f * v._13,
		f * v._21, f * v._22, f * v._23,
		f * v._31, f * v._32, f * v._33,
		f * v._41, f * v._42, f * v._43
		);
}

LCXMATRIX4X3 operator *(const LCXMATRIX4X3& v, FLOAT f)
{
	return LCXMATRIX4X3(
		f * v._11, f * v._12, f * v._13,
		f * v._21, f * v._22, f * v._23,
		f * v._31, f * v._32, f * v._33,
		f * v._41, f * v._42, f * v._43
		);
}

LCXVECTOR4 operator *(LCXVECTOR4& p, const LCXMATRIX4X3& v)
{
	return LCXVECTOR4(p.x * v._11 + p.y * v._21 + p.z * v._31 + p.w * v._41
					, p.x * v._12 + p.y * v._22 + p.z * v._32 + p.w * v._42
					, p.x * v._13 + p.y * v._23 + p.z * v._33 + p.w * v._43
					, 1.0F
					);
}

LCXVECTOR4 operator *(const LCXMATRIX4X3& v, LCXVECTOR4& p)
{
	return LCXVECTOR4(v._11 * p.x + v._12 * p.y + v._13 * p.z + p.w
					, v._21 * p.x + v._22 * p.y + v._23 * p.z + p.w
					, v._31 * p.x + v._32 * p.y + v._33 * p.z + p.w
					, v._41 * p.x + v._42 * p.y + v._43 * p.z + p.w
					);
}

LCXVECTOR3 operator *(LCXVECTOR3& p, const LCXMATRIX4X3& v)
{
	// w = 1
	FLOAT x = p.x * v._11 + p.y * v._21 + p.z * v._31 + v._41;	// * p.w;
	FLOAT y = p.x * v._12 + p.y * v._22 + p.z * v._32 + v._42;	// * p.w;
	FLOAT z = p.x * v._13 + p.y * v._23 + p.z * v._33 + v._43;	// * p.w;

	return LCXVECTOR3(x,y,z);
}

LCXVECTOR3 operator *(const LCXMATRIX4X3& v, LCXVECTOR3& p)
{
	// w = 1
	FLOAT x = v._11 * p.x + v._12 * p.y + v._13 * p.z;	// * p.w;
	FLOAT y = v._21 * p.x + v._22 * p.y + v._23 * p.z;	// * p.w;
	FLOAT z = v._31 * p.x + v._32 * p.y + v._33 * p.z;	// * p.w;

	return LCXVECTOR3(x,y,z);
}



INT LCXMATRIX4X3::operator ==(const LCXMATRIX4X3& v) const
{
	return	(
		 LCXIsEqual(_11,v._11) && LCXIsEqual(_12, v._12) && LCXIsEqual(_13, v._13) &&
		 LCXIsEqual(_21,v._21) && LCXIsEqual(_22, v._22) && LCXIsEqual(_23, v._23) &&
		 LCXIsEqual(_31,v._31) && LCXIsEqual(_32, v._32) && LCXIsEqual(_33, v._33) &&
		 LCXIsEqual(_41,v._41) && LCXIsEqual(_42, v._42) && LCXIsEqual(_43, v._43)
		);
}

INT LCXMATRIX4X3::operator !=(const LCXMATRIX4X3& v) const
{
	return	(
		LCXIsNotEqual(_11, v._11) || LCXIsNotEqual(_12, v._12) || LCXIsNotEqual(_13, v._13) ||
		LCXIsNotEqual(_21, v._21) || LCXIsNotEqual(_22, v._22) || LCXIsNotEqual(_23, v._23) ||
		LCXIsNotEqual(_31, v._31) || LCXIsNotEqual(_32, v._32) || LCXIsNotEqual(_33, v._33) ||
		LCXIsNotEqual(_41, v._41) || LCXIsNotEqual(_42, v._42) || LCXIsNotEqual(_43, v._43)
		);
}


LCXMATRIX4X3& LCXMATRIX4X3::Identity()
{
	_11 = 1.0F;  _12 = 0.0F;  _13 = 0.0F;
	_21 = 0.0F;  _22 = 1.0F;  _23 = 0.0F;
	_31 = 0.0F;  _32 = 0.0F;  _33 = 1.0F;
	_41 = 0.0F;  _42 = 0.0F;  _43 = 0.0F;

	return *this;
}



LCXMATRIX4X3& LCXMATRIX4X3::Matrix4x4(const LCXMATRIX* v)
{
	_11 = v->_11;	_12 = v->_12;	_13 = v->_13;
	_21 = v->_21;	_22 = v->_22;	_23 = v->_23;
	_31 = v->_31;	_32 = v->_32;	_33 = v->_33;
	_41 = v->_41;	_42 = v->_42;	_43 = v->_43;

	return *this;
}


void LCXMATRIX4X3::TransformCoord(LCXVECTOR3* Out, const LCXVECTOR3* pIn)
{
	FLOAT _x, _y, _z;

	_x = pIn->x;
	_y = pIn->y;
	_z = pIn->z;

	Out->x = _11 * _x +  _21 * _y + _31 * _z   +  _41;
	Out->y = _12 * _x +  _22 * _y + _32 * _z   +  _42;
	Out->z = _13 * _x +  _23 * _y + _33 * _z   +  _43;
}

void LCXMATRIX4X3::TransformNormal(LCXVECTOR3* Out, const LCXVECTOR3* pIn)
{
	FLOAT _x, _y, _z;

	_x = pIn->x;
	_y = pIn->y;
	_z = pIn->z;

	Out->x = _11 * _x +  _21 * _y + _31 * _z;
	Out->y = _12 * _x +  _22 * _y + _32 * _z;
	Out->z = _13 * _x +  _23 * _y + _33 * _z;
}


LCXMATRIX4X3& LCXMATRIX4X3::Negative()
{
	_11 = -_11;  _12 = -_12;  _13 = -_13;
	_21 = -_21;  _22 = -_22;  _23 = -_23;
	_31 = -_31;  _32 = -_32;  _33 = -_33;
	_41 = -_41;  _42 = -_42;  _43 = -_43;

	return *this;
}


LCXMATRIX4X3& LCXMATRIX4X3::Lerp(const LCXMATRIX4X3* m1, const LCXMATRIX4X3* m2, FLOAT t) // this = (1-t)*v1 + t* v2
{
	FLOAT k = 1.0F - t;
	_11 = k * m1->_11 + t * m2->_11;  _12 = k * m1->_12 + t * m2->_12;  _13 = k * m1->_13 + t * m2->_13;
	_21 = k * m1->_21 + t * m2->_21;  _22 = k * m1->_22 + t * m2->_22;  _23 = k * m1->_23 + t * m2->_23;
	_31 = k * m1->_31 + t * m2->_31;  _32 = k * m1->_32 + t * m2->_32;  _33 = k * m1->_33 + t * m2->_33;
	_41 = k * m1->_41 + t * m2->_41;  _42 = k * m1->_42 + t * m2->_42;  _43 = k * m1->_43 + t * m2->_43;

	return *this;
}




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// 3X3 Matrix

LCXMATRIX3X3::LCXMATRIX3X3()
{
	_11 = 1.0F;  _12 = 0.0F;  _13 = 0.0F;
	_21 = 0.0F;  _22 = 1.0F;  _23 = 0.0F;
	_31 = 0.0F;  _32 = 0.0F;  _33 = 1.0F;
}

LCXMATRIX3X3::LCXMATRIX3X3(const FLOAT* v)
{
	if(!v)
		return;

	_11 = v[ 0]; _12 = v[ 1]; _13 = v[ 2];
	_21 = v[ 3]; _22 = v[ 4]; _23 = v[ 5];
	_31 = v[ 6]; _32 = v[ 7]; _33 = v[ 8];
}

LCXMATRIX3X3::LCXMATRIX3X3(const LCXMATRIX3X3* v)
{
	if(!v)
		return;

	_11 = v->_11; _12 = v->_11; _13 = v->_11;
	_21 = v->_21; _22 = v->_21; _23 = v->_21;
	_31 = v->_31; _32 = v->_31; _33 = v->_31;
}


LCXMATRIX3X3::LCXMATRIX3X3(
					 FLOAT v11, FLOAT v12, FLOAT v13,
					 FLOAT v21, FLOAT v22, FLOAT v23,
					 FLOAT v31, FLOAT v32, FLOAT v33 )
{
	_11 = v11; _12 = v12; _13 = v13;
	_21 = v21; _22 = v22; _23 = v23;
	_31 = v31; _32 = v32; _33 = v33;
}


LCXMATRIX3X3::LCXMATRIX3X3(const LCXMATRIX& v)
{
	_11 = v._11; _12 = v._12; _13 = v._13;
	_21 = v._21; _22 = v._22; _23 = v._23;
	_31 = v._31; _32 = v._32; _33 = v._33;
}

LCXMATRIX3X3::LCXMATRIX3X3(const LCXMATRIX3X3& v)   {   *this = v;      }
LCXMATRIX3X3& LCXMATRIX3X3::operator=(const LCXMATRIX3X3& v)
{
	_11 = v._11; _12 = v._12; _13 = v._13;
	_21 = v._21; _22 = v._22; _23 = v._23;
	_31 = v._31; _32 = v._32; _33 = v._33;
	return *this;
}



// access grants
FLOAT& LCXMATRIX3X3::operator()(int iRow, int iCol)		{	return m[iRow][iCol];	}
FLOAT  LCXMATRIX3X3::operator()(int iRow, int iCol) const{	return m[iRow][iCol];	}
FLOAT& LCXMATRIX3X3::operator[](int n)					{	return *((&_11)+ n);	}

// casting operators
LCXMATRIX3X3::operator FLOAT*()				{	return(FLOAT*)&_11;		}
LCXMATRIX3X3::operator const FLOAT*() const	{	return(FLOAT*)&_11;		}

// assignment operators
LCXMATRIX3X3& LCXMATRIX3X3::operator *=(const LCXMATRIX3X3& v)
{
	LCXMATRIX3X3 t = *this;

	_11 = t._11 * v._11 +  t._12 * v._21 +  t._13 * v._31;
	_12 = t._11 * v._12 +  t._12 * v._22 +  t._13 * v._32;
	_13 = t._11 * v._13 +  t._12 * v._23 +  t._13 * v._33;

	_21 = t._21 * v._11 +  t._22 * v._21 +  t._23 * v._31;
	_22 = t._21 * v._12 +  t._22 * v._22 +  t._23 * v._32;
	_23 = t._21 * v._13 +  t._22 * v._23 +  t._23 * v._33;

	_31 = t._31 * v._11 +  t._32 * v._21 +  t._33 * v._31;
	_32 = t._31 * v._12 +  t._32 * v._22 +  t._33 * v._32;
	_33 = t._31 * v._13 +  t._32 * v._23 +  t._33 * v._33;

	return * this;
}


LCXMATRIX3X3& LCXMATRIX3X3::operator +=(const LCXMATRIX3X3& v)
{
	_11 += v._11;	_12 += v._12;	_13 += v._13;
	_21 += v._21;	_22 += v._22;	_23 += v._23;
	_31 += v._31;	_32 += v._32;	_33 += v._33;

	return * this;
}

LCXMATRIX3X3& LCXMATRIX3X3::operator -=(const LCXMATRIX3X3& v)
{
	_11 -= v._11;	_12 -= v._12;	_13 -= v._13;
	_21 -= v._21;	_22 -= v._22;	_23 -= v._23;
	_31 -= v._31;	_32 -= v._32;	_33 -= v._33;

	return * this;
}

LCXMATRIX3X3& LCXMATRIX3X3::operator *=(FLOAT v)
{
	_11 *= v;	_12 *= v;	_13 *= v;
	_21 *= v;	_22 *= v;	_23 *= v;
	_31 *= v;	_32 *= v;	_33 *= v;

	return * this;
}

LCXMATRIX3X3& LCXMATRIX3X3::operator /=(FLOAT v)
{
	v = 1.0F/v;

	_11 *= v;	_12 *= v;	_13 *= v;
	_21 *= v;	_22 *= v;	_23 *= v;
	_31 *= v;	_32 *= v;	_33 *= v;

	return * this;
}


// unary operators
LCXMATRIX3X3 LCXMATRIX3X3::operator +() const
{
	return *this;
}

LCXMATRIX3X3 LCXMATRIX3X3::operator -() const
{
	return LCXMATRIX3X3(-_11, -_12, -_13,
						-_21, -_22, -_23,
						-_31, -_32, -_33   );
}

// binary operators
LCXMATRIX3X3 LCXMATRIX3X3::operator *(const LCXMATRIX3X3& v) const
{
	LCXMATRIX3X3 t = *this;
	LCXMATRIX3X3 r;

	r._11 = t._11 * v._11 +  t._12 * v._21 +  t._13 * v._31;
	r._12 = t._11 * v._12 +  t._12 * v._22 +  t._13 * v._32;
	r._13 = t._11 * v._13 +  t._12 * v._23 +  t._13 * v._33;

	r._21 = t._21 * v._11 +  t._22 * v._21 +  t._23 * v._31;
	r._22 = t._21 * v._12 +  t._22 * v._22 +  t._23 * v._32;
	r._23 = t._21 * v._13 +  t._22 * v._23 +  t._23 * v._33;

	r._31 = t._31 * v._11 +  t._32 * v._21 +  t._33 * v._31;
	r._32 = t._31 * v._12 +  t._32 * v._22 +  t._33 * v._32;
	r._33 = t._31 * v._13 +  t._32 * v._23 +  t._33 * v._33;

	return r;
}


LCXMATRIX3X3 LCXMATRIX3X3::operator +(const LCXMATRIX3X3& v) const
{
	return LCXMATRIX3X3(
		_11 + v._11, _12 + v._12, _13 + v._13,
		_21 + v._21, _22 + v._22, _23 + v._23,
		_31 + v._31, _32 + v._32, _33 + v._33
		);
}

LCXMATRIX3X3 LCXMATRIX3X3::operator -(const LCXMATRIX3X3& v) const
{
	return LCXMATRIX3X3(
		_11 - v._11, _12 - v._12, _13 - v._13,
		_21 - v._21, _22 - v._22, _23 - v._23,
		_31 - v._31, _32 - v._32, _33 - v._33
		);
}

LCXMATRIX3X3 LCXMATRIX3X3::operator *(FLOAT v) const
{
	return LCXMATRIX3X3(
		_11 * v, _12 * v, _13 * v,
		_21 * v, _22 * v, _23 * v,
		_31 * v, _32 * v, _33 * v
		);
}

LCXMATRIX3X3 LCXMATRIX3X3::operator /(FLOAT v) const
{
	v = 1.0F / v;

	return LCXMATRIX3X3(
		_11 * v, _12 * v, _13 * v,
		_21 * v, _22 * v, _23 * v,
		_31 * v, _32 * v, _33 * v
		);
}


LCXMATRIX3X3 operator *(FLOAT f, const LCXMATRIX3X3& v)
{
	return LCXMATRIX3X3(
		f * v._11, f * v._12, f * v._13,
		f * v._21, f * v._22, f * v._23,
		f * v._31, f * v._32, f * v._33
		);
}

LCXMATRIX3X3 operator *(const LCXMATRIX3X3& v, FLOAT f)
{
	return LCXMATRIX3X3(
		f * v._11, f * v._12, f * v._13,
		f * v._21, f * v._22, f * v._23,
		f * v._31, f * v._32, f * v._33
		);
}

LCXVECTOR3 operator *(LCXVECTOR3& p, const LCXMATRIX3X3& v)
{
	return LCXVECTOR3(p.x * v._11 + p.y * v._21 + p.z * v._31
					, p.x * v._12 + p.y * v._22 + p.z * v._32
					, p.x * v._13 + p.y * v._23 + p.z * v._33
					);
}

LCXVECTOR3 operator *(const LCXMATRIX3X3& v, LCXVECTOR3& p)
{
	return LCXVECTOR3(v._11 * p.x + v._12 * p.y + v._13 * p.z
					, v._21 * p.x + v._22 * p.y + v._23 * p.z
					, v._31 * p.x + v._32 * p.y + v._33 * p.z
					);
}

LCXVECTOR2 operator *(LCXVECTOR2& p, const LCXMATRIX3X3& v)
{
	// z = 1
	FLOAT x = p.x * v._11 + p.y * v._21 + v._31;	// * p.z;
	FLOAT y = p.x * v._12 + p.y * v._22 + v._32;	// * p.z;
	FLOAT z = p.x * v._13 + p.y * v._23 + v._33;	// * p.z;

	x /=z;
	y /=z;
	z = 1;

	return LCXVECTOR2(x, y);
}

LCXVECTOR2 operator *(const LCXMATRIX3X3& v, LCXVECTOR2& p)
{
	// z = 1
	FLOAT x = v._11 * p.x + v._12 * p.y + v._13;	// * p.z;
	FLOAT y = v._21 * p.x + v._22 * p.y + v._23;	// * p.z;
	FLOAT z = v._31 * p.x + v._32 * p.y + v._33;	// * p.z;

	x /=z;
	y /=z;
	z = 1;

	return LCXVECTOR2(x, y);
}


INT LCXMATRIX3X3::operator ==(const LCXMATRIX3X3& v) const
{
	return	(
		 LCXIsEqual(_11,v._11) && LCXIsEqual(_12, v._12) && LCXIsEqual(_13, v._13)&&
		 LCXIsEqual(_21,v._21) && LCXIsEqual(_22, v._22) && LCXIsEqual(_23, v._23)&&
		 LCXIsEqual(_31,v._31) && LCXIsEqual(_32, v._32) && LCXIsEqual(_33, v._33)
		);
}

INT LCXMATRIX3X3::operator !=(const LCXMATRIX3X3& v) const
{
	return	(
		LCXIsNotEqual(_11, v._11) || LCXIsNotEqual(_12, v._12) || LCXIsNotEqual(_13, v._13) ||
		LCXIsNotEqual(_21, v._21) || LCXIsNotEqual(_22, v._22) || LCXIsNotEqual(_23, v._23) ||
		LCXIsNotEqual(_31, v._31) || LCXIsNotEqual(_32, v._32) || LCXIsNotEqual(_33, v._33)
		);
}



// Length
LCXMATRIX3X3& LCXMATRIX3X3::Identity()
{
	_11 = 1.0F;  _12 = 0.0F;  _13 = 0.0F;
	_21 = 0.0F;  _22 = 1.0F;  _23 = 0.0F;
	_31 = 0.0F;  _32 = 0.0F;  _33 = 1.0F;

	return *this;
}


LCXMATRIX3X3& LCXMATRIX3X3::Transpose()
{
	FLOAT t =0.0F;

	t   = _12;	_12 = _21;	_21 = t;
	t   = _13;	_13 = _31;	_31 = t;

	t   = _23;	_23 = _32;	_32 = t;

	return *this;
}


LCXMATRIX3X3& LCXMATRIX3X3::Transpose(const LCXMATRIX3X3* v)
{
	_11 = v->_11;	_12 = v->_21;	_13 = v->_31;
	_21 = v->_12;	_22 = v->_22;	_23 = v->_32;
	_31 = v->_13;	_32 = v->_23;	_33 = v->_33;

	return *this;
}


LCXMATRIX3X3& LCXMATRIX3X3::Inverse(FLOAT* det)
{
	LCXMATRIX3X3	t = *this;
	this->Inverse(det, &t);
	return *this;
}


LCXMATRIX3X3& LCXMATRIX3X3::Inverse(FLOAT* det, const LCXMATRIX3X3* v)
{
	FLOAT* Out = &m[0][0];
	FLOAT d = 0;

	d += v->_31 * (v->_12 * v->_23 - v->_13 * v->_22);
	d += v->_32 * (v->_13 * v->_21 - v->_11 * v->_23);
	d += v->_33 * (v->_11 * v->_22 - v->_12 * v->_21);

	if(det) *det = d;
	if(-0.00001F < d && d <0.00001F)
		return *this;

	d = 1.0F/d;


	Out[0] = +(v->_22 * v->_33 - v->_23 * v->_32);
	Out[1] = -(v->_12 * v->_33 - v->_13 * v->_32);
	Out[2] = +(v->_12 * v->_23 - v->_13 * v->_22);

	Out[3] = -(v->_21 * v->_33 - v->_23 * v->_31);
	Out[4] = +(v->_11 * v->_33 - v->_13 * v->_31);
	Out[5] = -(v->_11 * v->_23 - v->_13 * v->_21);

	Out[6] = +(v->_21 * v->_32 - v->_22 * v->_31);
	Out[7] = -(v->_11 * v->_32 - v->_12 * v->_31);
	Out[8] = +(v->_11 * v->_22 - v->_12 * v->_21);


	Out[0] *= d;	Out[1] *= d;	Out[2] *= d;
	Out[3] *= d;	Out[4] *= d;	Out[5] *= d;
	Out[6] *= d;	Out[7] *= d;	Out[8] *= d;

	return *this;
}


FLOAT LCXMATRIX3X3::Determinant()
{
	FLOAT d =0;

	d += _31 * (_12 * _23 - _13 * _22);
	d += _32 * (_13 * _21 - _11 * _23);
	d += _33 * (_11 * _22 - _12 * _21);

	return d;
}


LCXMATRIX3X3& LCXMATRIX3X3::Scaling(FLOAT X, FLOAT Y)
{
	_11 = X   ;  _12 = 0.0F;  _13 = 0.0F;
	_21 = 0.0F;  _22 = Y   ;  _23 = 0.0F;
	_31 = 0.0F;  _32 = 0.0F;  _33 = 1.0F;

	return *this;
}


LCXMATRIX3X3& LCXMATRIX3X3::Scaling(const LCXVECTOR2* vScl)
{
	return Scaling(vScl->x, vScl->y);
}


LCXMATRIX3X3& LCXMATRIX3X3::Rotation(FLOAT Rad)
{
	//FLOAT Cos = (FLOAT)cos(Rad);
	//FLOAT Sin = (FLOAT)sin(Rad);
	FLOAT Cos = LCXMathCos(Rad);
	FLOAT Sin = LCXMathSin(Rad);

	_11 =+Cos;	_12 = -Sin;	_13 = 0.0F;
	_21 =+Sin;	_22 = +Cos;	_23 = 0.0F;
	_31 = 0.0F;	_32 = 0.0F;	_33 = 1.0F;

	return *this;
}

LCXMATRIX3X3& LCXMATRIX3X3::Translation(FLOAT x,FLOAT y, INT bInit)
{
	if(bInit)
	{
		_11 = 1.0F;  _12 = 0.0F;	_13 = 0.0F;
		_21 = 0.0F;  _22 = 1.0F;	_23 = 0.0F;
									_33 = 1.0F;
	}

	_31 = x;
	_32 = y;

	return *this;
}


LCXMATRIX3X3& LCXMATRIX3X3::Translation(const LCXVECTOR2* vTrs, INT bInit)
{
	return this->Translation(vTrs->x, vTrs->y, bInit);
}


void LCXMATRIX3X3::TransformCoord(LCXVECTOR2* Out, const LCXVECTOR2* pIn)
{
	FLOAT _x, _y;
	FLOAT x, y, z;

	_x = pIn->x;
	_y = pIn->y;

	x = _11 * _x  +  _21 * _y   +  _31;
	y = _12 * _x  +  _22 * _y   +  _32;
	z = _13 * _x  +  _23 * _y   +  _33;

	x /=z;
	y /=z;
	//z =1.0F;

	Out->x =x;
	Out->y =y;
}

void LCXMATRIX3X3::TransformNormal(LCXVECTOR2* Out, const LCXVECTOR2* pIn)
{
	FLOAT _x, _y;
	FLOAT x, y;

	_x = pIn->x;
	_y = pIn->y;

	x = _11 * _x  +  _21 * _y;
	y = _12 * _x  +  _22 * _y;

	Out->x =x;
	Out->y =y;
}


LCXMATRIX3X3& LCXMATRIX3X3::Negative()
{
	_11 = -_11;  _12 = -_12;  _13 = -_13;
	_21 = -_21;  _22 = -_22;  _23 = -_23;
	_31 = -_31;  _32 = -_32;  _33 = -_33;

	return *this;
}


LCXMATRIX3X3& LCXMATRIX3X3::Lerp(const LCXMATRIX3X3* m1, const LCXMATRIX3X3* m2, FLOAT t) // this = (1-t)*v1 + t* v2
{
	FLOAT k = 1.0F - t;
	_11 = k * m1->_11 + t * m2->_11;  _12 = k * m1->_12 + t * m2->_12;  _13 = k * m1->_13 + t * m2->_13;
	_21 = k * m1->_21 + t * m2->_21;  _22 = k * m1->_22 + t * m2->_22;  _23 = k * m1->_23 + t * m2->_23;
	_31 = k * m1->_31 + t * m2->_31;  _32 = k * m1->_32 + t * m2->_32;  _33 = k * m1->_33 + t * m2->_33;

	return *this;
}




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// 3X2 Matrix

LCXMATRIX3X2::LCXMATRIX3X2()
{
	_11 = 1.0F;  _12 = 0.0F;
	_21 = 0.0F;  _22 = 1.0F;
	_31 = 0.0F;  _32 = 0.0F;
}

LCXMATRIX3X2::LCXMATRIX3X2(const FLOAT* v)
{
	if(!v)
		return;

	_11 = v[ 0]; _12 = v[ 1];
	_21 = v[ 2]; _22 = v[ 3];
	_31 = v[ 4]; _32 = v[ 5];
}

LCXMATRIX3X2::LCXMATRIX3X2(const LCXMATRIX3X2* v)
{
	if(!v)
		return;

	_11 = v->_11; _12 = v->_11;
	_21 = v->_21; _22 = v->_21;
	_31 = v->_31; _32 = v->_31;
}


LCXMATRIX3X2::LCXMATRIX3X2(
					 FLOAT v11, FLOAT v12,
					 FLOAT v21, FLOAT v22,
					 FLOAT v31, FLOAT v32)
{
	_11 = v11; _12 = v12;
	_21 = v21; _22 = v22;
	_31 = v31; _32 = v32;
}


LCXMATRIX3X2::LCXMATRIX3X2(const LCXMATRIX3X3& v)
{
	_11 = v._11; _12 = v._12;
	_21 = v._21; _22 = v._22;
	_31 = v._31; _32 = v._32;
}

LCXMATRIX3X2::LCXMATRIX3X2(const LCXMATRIX3X2& v){   *this = v;      }
LCXMATRIX3X2& LCXMATRIX3X2::operator=(const LCXMATRIX3X2& v)
{
	_11 = v._11; _12 = v._12;
	_21 = v._21; _22 = v._22;
	_31 = v._31; _32 = v._32;
	return *this;
}



// access grants
FLOAT& LCXMATRIX3X2::operator()(int iRow, int iCol)	{	return m[iRow][iCol];	}
FLOAT  LCXMATRIX3X2::operator()(int iRow, int iCol) const{	return m[iRow][iCol];	}

// casting operators
LCXMATRIX3X2::operator FLOAT*()				{	return(FLOAT*)&_11;		}
LCXMATRIX3X2::operator const FLOAT*() const	{	return(FLOAT*)&_11;		}
FLOAT& LCXMATRIX3X2::operator[](int n)		{	return *((&_11)+ n);	}

// assignment operators
LCXMATRIX3X2& LCXMATRIX3X2::operator *=(const LCXMATRIX3X2& v)
{
	LCXMATRIX3X2 t = *this;

	_11 = t._11 * v._11 +  t._12 * v._21;// +  0 * v._31;
	_12 = t._11 * v._12 +  t._12 * v._22;// +  0 * v._32;
//	_13 = t._11 *     0 +  t._12 *     0    +  0 * 1    ;

	_21 = t._21 * v._11 +  t._22 * v._21;// +  0 * v._31;
	_22 = t._21 * v._12 +  t._22 * v._22;// +  0 * v._32;
//	_23 = t._21 *     0 +  t._22 *     0    +  0 * 1    ;

	_31 = t._31 * v._11 +  t._32 * v._21;// +  1 * v._31;
	_32 = t._31 * v._12 +  t._32 * v._22;// +  1 * v._32;
//	_33 = t._31 *     0 +  t._32 *     0    +  1 * 1    ;

	return * this;
}


LCXMATRIX3X2& LCXMATRIX3X2::operator +=(const LCXMATRIX3X2& v)
{
	_11 += v._11;	_12 += v._12;//	_13 += v._13;
	_21 += v._21;	_22 += v._22;//	_23 += v._23;
	_31 += v._31;	_32 += v._32;//	_33 += v._33;

	return * this;
}

LCXMATRIX3X2& LCXMATRIX3X2::operator -=(const LCXMATRIX3X2& v)
{
	_11 -= v._11;	_12 -= v._12;//	_13 -= v._13;
	_21 -= v._21;	_22 -= v._22;//	_23 -= v._23;
	_31 -= v._31;	_32 -= v._32;//	_33 -= v._33;

	return * this;
}

LCXMATRIX3X2& LCXMATRIX3X2::operator *=(FLOAT v)
{
	_11 *= v;	_12 *= v;//	_13 *= v;
	_21 *= v;	_22 *= v;//	_23 *= v;
	_31 *= v;	_32 *= v;//	_33 *= v;

	return * this;
}

LCXMATRIX3X2& LCXMATRIX3X2::operator /=(FLOAT v)
{
	v = 1.0F/v;

	_11 *= v;	_12 *= v;//	_13 *= v;
	_21 *= v;	_22 *= v;//	_23 *= v;
	_31 *= v;	_32 *= v;//	_33 *= v;

	return * this;
}


// unary operators
LCXMATRIX3X2 LCXMATRIX3X2::operator +() const
{
	return *this;
}

LCXMATRIX3X2 LCXMATRIX3X2::operator -() const
{
	return LCXMATRIX3X2(-_11, -_12,
						-_21, -_22,
						-_31, -_32   );
}

// binary operators
LCXMATRIX3X2 LCXMATRIX3X2::operator *(const LCXMATRIX3X2& v) const
{
	LCXMATRIX3X2 t = *this;
	LCXMATRIX3X2 r;

	r._11 = t._11 * v._11 +  t._12 * v._21;// +  0 * v._31;
	r._12 = t._11 * v._12 +  t._12 * v._22;// +  0 * v._32;
//	r._13 = t._11 *     0 +  t._12 *     0    +  0 * 1    ;

	r._21 = t._21 * v._11 +  t._22 * v._21;// +  0 * v._31;
	r._22 = t._21 * v._12 +  t._22 * v._22;// +  0 * v._32;
//	r._23 = t._21 *     0 +  t._22 *     0    +  0 * 1    ;

	r._31 = t._31 * v._11 +  t._32 * v._21;// +  1 * v._31;
	r._32 = t._31 * v._12 +  t._32 * v._22;// +  1 * v._32;
//	r._33 = t._31 *     0 +  t._32 *     0    +  1 * 1    ;

	return r;
}


LCXMATRIX3X2 LCXMATRIX3X2::operator +(const LCXMATRIX3X2& v) const
{
	return LCXMATRIX3X2(
		_11 + v._11, _12 + v._12,
		_21 + v._21, _22 + v._22,
		_31 + v._31, _32 + v._32
		);
}

LCXMATRIX3X2 LCXMATRIX3X2::operator -(const LCXMATRIX3X2& v) const
{
	return LCXMATRIX3X2(
		_11 - v._11, _12 - v._12,
		_21 - v._21, _22 - v._22,
		_31 - v._31, _32 - v._32
		);
}

LCXMATRIX3X2 LCXMATRIX3X2::operator *(FLOAT v) const
{
	return LCXMATRIX3X2(
		_11 * v, _12 * v,
		_21 * v, _22 * v,
		_31 * v, _32 * v
		);
}

LCXMATRIX3X2 LCXMATRIX3X2::operator /(FLOAT v) const
{
	v = 1.0F / v;

	return LCXMATRIX3X2(
		_11 * v, _12 * v,
		_21 * v, _22 * v,
		_31 * v, _32 * v
		);
}


LCXMATRIX3X2 operator *(FLOAT f, const LCXMATRIX3X2& v)
{
	return LCXMATRIX3X2(
		f * v._11, f * v._12,
		f * v._21, f * v._22,
		f * v._31, f * v._32
		);
}

LCXMATRIX3X2 operator *(const LCXMATRIX3X2& v, FLOAT f)
{
	return LCXMATRIX3X2(
		f * v._11, f * v._12,
		f * v._21, f * v._22,
		f * v._31, f * v._32
		);
}

LCXVECTOR2 operator *(LCXVECTOR2& p, const LCXMATRIX3X2& v)
{
	// z = 1
	FLOAT x = p.x * v._11 + p.y * v._21 + v._31;	// + 1 * v._31
	FLOAT y = p.x * v._12 + p.y * v._22 + v._32;	// + 1 * v._32
	//FLOAT z = p.x * 0     + p.y * 0     + 1 * 1;
	//x /=z;
	//y /=z;

	return LCXVECTOR2(x, y);
}

//LCXVECTOR2 operator *(const LCXMATRIX3X2& v, LCXVECTOR2& p)
//{
//	FLOAT x = v._11 * p.x + v._12 * p.y;
//	FLOAT y = v._21 * p.x + v._22 * p.y;
//	FLOAT z = v._31 * p.x + v._32 * p.y + 1* 1;
//	x /=z;
//	y /=z;
//
//	return LCXVECTOR2(x, y);
//}


INT LCXMATRIX3X2::operator ==(const LCXMATRIX3X2& v) const
{
	return	(
		 LCXIsEqual(_11,v._11) && LCXIsEqual(_12, v._12) &&
		 LCXIsEqual(_21,v._21) && LCXIsEqual(_22, v._22) &&
		 LCXIsEqual(_31,v._31) && LCXIsEqual(_32, v._32)
		);
}

INT LCXMATRIX3X2::operator !=(const LCXMATRIX3X2& v) const
{
	return	(
		LCXIsNotEqual(_11, v._11) || LCXIsNotEqual(_12, v._12) ||
		LCXIsNotEqual(_21, v._21) || LCXIsNotEqual(_22, v._22) ||
		LCXIsNotEqual(_31, v._31) || LCXIsNotEqual(_32, v._32)
		);
}



// Length
LCXMATRIX3X2& LCXMATRIX3X2::Identity()
{
	_11 = 1.0F;  _12 = 0.0F;
	_21 = 0.0F;  _22 = 1.0F;
	_31 = 0.0F;  _32 = 0.0F;

	return *this;
}


LCXMATRIX3X2& LCXMATRIX3X2::Inverse(FLOAT* det)
{
	LCXMATRIX3X2	t = *this;
	this->Inverse(det, &t);
	return *this;
}


LCXMATRIX3X2& LCXMATRIX3X2::Inverse(FLOAT* det, const LCXMATRIX3X2* v)
{
	FLOAT d = (v->_11 * v->_22 - v->_12 * v->_21);

	if(det) *det = d;
	if(-0.00001F < d && d <0.00001F)
		return *this;

	d = 1.0F/d;

	_11 =  v->_22;
	_12 = -v->_12;
	_21 = -v->_21;
	_22 =  v->_11;
	_31 = +(v->_21 * v->_32 - v->_22 * v->_31);
	_32 = -(v->_11 * v->_32 - v->_12 * v->_31);

	_11 *= d;
	_12 *= d;
	_21 *= d;
	_22 *= d;
	_31 *= d;
	_32 *= d;

	return *this;
}


FLOAT LCXMATRIX3X2::Determinant()
{
	FLOAT d = (_11 * _22 - _12 * _21);
	return d;
}


LCXMATRIX3X2& LCXMATRIX3X2::Scaling(FLOAT X, FLOAT Y)
{
	_11 = X   ;  _12 = 0.0F;
	_21 = 0.0F;  _22 = Y   ;
	_31 = 0.0F;  _32 = 0.0F;

	return *this;
}


LCXMATRIX3X2& LCXMATRIX3X2::Scaling(const LCXVECTOR2* vScl)
{
	return Scaling(vScl->x, vScl->y);
}


LCXMATRIX3X2& LCXMATRIX3X2::Rotation(FLOAT Rad)
{
	//FLOAT Cos = (FLOAT)cos(Rad);
	//FLOAT Sin = (FLOAT)sin(Rad);
	FLOAT Cos = LCXMathCos(Rad);
	FLOAT Sin = LCXMathSin(Rad);

	_11 =+Cos;	_12 = -Sin;
	_21 =+Sin;	_22 = +Cos;
	_31 = 0.0F;	_32 = 0.0F;

	return *this;
}

LCXMATRIX3X2& LCXMATRIX3X2::Translation(FLOAT x,FLOAT y, INT bInit)
{
	if(bInit)
	{
		_11 = 1.0F;  _12 = 0.0F;
		_21 = 0.0F;  _22 = 1.0F;
	}

	_31 = x;
	_32 = y;

	return *this;
}


LCXMATRIX3X2& LCXMATRIX3X2::Translation(const LCXVECTOR2* vTrs, INT bInit)
{
	return this->Translation(vTrs->x, vTrs->y, bInit);
}


void LCXMATRIX3X2::TransformCoord(LCXVECTOR2* Out, const LCXVECTOR2* pIn)
{
	FLOAT _x, _y;
	FLOAT x, y;

	_x = pIn->x;
	_y = pIn->y;

	x = _11 * _x  +  _21 * _y   +  _31;
	y = _12 * _x  +  _22 * _y   +  _32;

	Out->x =x;
	Out->y =y;
}

void LCXMATRIX3X2::TransformNormal(LCXVECTOR2* Out, const LCXVECTOR2* pIn)
{
	FLOAT _x, _y;
	FLOAT x, y;

	_x = pIn->x;
	_y = pIn->y;

	x = _11 * _x  +  _21 * _y;
	y = _12 * _x  +  _22 * _y;

	Out->x =x;
	Out->y =y;
}


LCXMATRIX3X2& LCXMATRIX3X2::Negative()
{
	_11 = -_11;  _12 = -_12;
	_21 = -_21;  _22 = -_22;
	_31 = -_31;  _32 = -_32;

	return *this;
}


LCXMATRIX3X2& LCXMATRIX3X2::Lerp(const LCXMATRIX3X2* m1, const LCXMATRIX3X2* m2, FLOAT t) // this = (1-t)*v1 + t* v2
{
	FLOAT k = 1.0F - t;
	_11 = k * m1->_11 + t * m2->_11;  _12 = k * m1->_12 + t * m2->_12;
	_21 = k * m1->_21 + t * m2->_21;  _22 = k * m1->_22 + t * m2->_22;
	_31 = k * m1->_31 + t * m2->_31;  _32 = k * m1->_32 + t * m2->_32;

	return *this;
}




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// 2X2 Matrix

LCXMATRIX2X2::LCXMATRIX2X2()
{
	_11 = 1.0F;  _12 = 0.0F;
	_21 = 0.0F;  _22 = 1.0F;
}

LCXMATRIX2X2::LCXMATRIX2X2(const FLOAT* v)
{
	if(!v)
		return;

	_11 = v[ 0]; _12 = v[ 1];
	_21 = v[ 2]; _22 = v[ 3];
}

LCXMATRIX2X2::LCXMATRIX2X2(const LCXMATRIX2X2* v)
{
	if(!v)
		return;

	_11 = v->_11; _12 = v->_11;
	_21 = v->_21; _22 = v->_21;
}


LCXMATRIX2X2::LCXMATRIX2X2(
					 FLOAT v11, FLOAT v12,
					 FLOAT v21, FLOAT v22  )
{
	_11 = v11; _12 = v12;
	_21 = v21; _22 = v22;
}


LCXMATRIX2X2::LCXMATRIX2X2(const LCXMATRIX& v)
{
	_11 = v._11; _12 = v._12;
	_21 = v._21; _22 = v._22;
}

LCXMATRIX2X2::LCXMATRIX2X2(const LCXMATRIX2X2& v)   {   *this = v;      }
LCXMATRIX2X2& LCXMATRIX2X2::operator=(const LCXMATRIX2X2& v)
{
	_11 = v._11; _12 = v._12;
	_21 = v._21; _22 = v._22;
	return *this;
}



// access grants
FLOAT& LCXMATRIX2X2::operator()(int iRow, int iCol)		{	return m[iRow][iCol];	}
FLOAT  LCXMATRIX2X2::operator()(int iRow, int iCol) const{	return m[iRow][iCol];	}
FLOAT& LCXMATRIX2X2::operator[](int n)					{	return *((&_11)+ n);	}

// casting operators
LCXMATRIX2X2::operator FLOAT*()				{	return(FLOAT*)&_11;		}
LCXMATRIX2X2::operator const FLOAT*() const	{	return(FLOAT*)&_11;		}

// assignment operators
LCXMATRIX2X2& LCXMATRIX2X2::operator *=(const LCXMATRIX2X2& v)
{
	LCXMATRIX2X2 t = *this;

	_11 = t._11 * v._11 +  t._12 * v._21;
	_12 = t._11 * v._12 +  t._12 * v._22;

	_21 = t._21 * v._11 +  t._22 * v._21;
	_22 = t._21 * v._12 +  t._22 * v._22;

	return * this;
}


LCXMATRIX2X2& LCXMATRIX2X2::operator +=(const LCXMATRIX2X2& v)
{
	_11 += v._11;	_12 += v._12;
	_21 += v._21;	_22 += v._22;

	return * this;
}

LCXMATRIX2X2& LCXMATRIX2X2::operator -=(const LCXMATRIX2X2& v)
{
	_11 -= v._11;	_12 -= v._12;
	_21 -= v._21;	_22 -= v._22;

	return * this;
}

LCXMATRIX2X2& LCXMATRIX2X2::operator *=(FLOAT v)
{
	_11 *= v;	_12 *= v;
	_21 *= v;	_22 *= v;

	return * this;
}

LCXMATRIX2X2& LCXMATRIX2X2::operator /=(FLOAT v)
{
	v = 1.0F/v;

	_11 *= v;	_12 *= v;
	_21 *= v;	_22 *= v;

	return * this;
}


// unary operators
LCXMATRIX2X2 LCXMATRIX2X2::operator +() const
{
	return *this;
}

LCXMATRIX2X2 LCXMATRIX2X2::operator -() const
{
	return LCXMATRIX2X2(-_11, -_12,
						-_21, -_22 );
}

// binary operators
LCXMATRIX2X2 LCXMATRIX2X2::operator *(const LCXMATRIX2X2& v) const
{
	LCXMATRIX2X2 t = *this;
	LCXMATRIX2X2 r;

	r._11 = t._11 * v._11 +  t._12 * v._21;
	r._12 = t._11 * v._12 +  t._12 * v._22;

	r._21 = t._21 * v._11 +  t._22 * v._21;
	r._22 = t._21 * v._12 +  t._22 * v._22;

	return r;
}


LCXMATRIX2X2 LCXMATRIX2X2::operator +(const LCXMATRIX2X2& v) const
{
	return LCXMATRIX2X2(
		_11 + v._11, _12 + v._12,
		_21 + v._21, _22 + v._22 );
}

LCXMATRIX2X2 LCXMATRIX2X2::operator -(const LCXMATRIX2X2& v) const
{
	return LCXMATRIX2X2(
		_11 - v._11, _12 - v._12,
		_21 - v._21, _22 - v._22  );
}

LCXMATRIX2X2 LCXMATRIX2X2::operator *(FLOAT v) const
{
	return LCXMATRIX2X2(
		_11 * v, _12 * v,
		_21 * v, _22 * v  );
}

LCXMATRIX2X2 LCXMATRIX2X2::operator /(FLOAT v) const
{
	v = 1.0F / v;

	return LCXMATRIX2X2(
		_11 * v, _12 * v,
		_21 * v, _22 * v );
}


LCXMATRIX2X2 operator *(FLOAT f, const LCXMATRIX2X2& v)
{
	return LCXMATRIX2X2(
		f * v._11, f * v._12,
		f * v._21, f * v._22 );
}

LCXMATRIX2X2 operator *(const LCXMATRIX2X2& v, FLOAT f)
{
	return LCXMATRIX2X2(
		f * v._11, f * v._12,
		f * v._21, f * v._22  );
}

LCXVECTOR2 operator *(LCXVECTOR2& p, const LCXMATRIX2X2& v)
{
	FLOAT x = p.x * v._11 + p.y * v._21;
	FLOAT y = p.x * v._12 + p.y * v._22;

	return LCXVECTOR2(x, y);
}

LCXVECTOR2 operator *(const LCXMATRIX2X2& v, LCXVECTOR2& p)
{
	FLOAT x = v._11 * p.x + v._12 * p.y;
	FLOAT y = v._21 * p.x + v._22 * p.y;

	return LCXVECTOR2(x, y);
}

INT LCXMATRIX2X2::operator ==(const LCXMATRIX2X2& v) const
{
	return	(
		 LCXIsEqual(_11,v._11) && LCXIsEqual(_12, v._12) &&
		 LCXIsEqual(_21,v._21) && LCXIsEqual(_22, v._22)
		);
}

INT LCXMATRIX2X2::operator !=(const LCXMATRIX2X2& v) const
{
	return	(
		LCXIsNotEqual(_11, v._11) || LCXIsNotEqual(_12, v._12) ||
		LCXIsNotEqual(_21, v._21) || LCXIsNotEqual(_22, v._22)
		);
}



// Length
LCXMATRIX2X2& LCXMATRIX2X2::Identity()
{
	_11 = 1.0F;  _12 = 0.0F;
	_21 = 0.0F;  _22 = 1.0F;

	return *this;
}


LCXMATRIX2X2& LCXMATRIX2X2::Transpose()
{
	FLOAT t =0.0F;

	t   = _12;	_12 = _21;	_21 = t;

	return *this;
}


LCXMATRIX2X2& LCXMATRIX2X2::Transpose(const LCXMATRIX2X2* v)
{
	_11 = v->_11;	_12 = v->_21;
	_21 = v->_12;	_22 = v->_22;

	return *this;
}


LCXMATRIX2X2& LCXMATRIX2X2::Inverse(FLOAT* det)
{
	LCXMATRIX2X2	t = *this;
	this->Inverse(det, &t);
	return *this;
}


LCXMATRIX2X2& LCXMATRIX2X2::Inverse(FLOAT* det, const LCXMATRIX2X2* v)
{
	FLOAT d = v->_11 * v->_22 - v->_12 * v->_21;

	if(det) *det = d;
	if(-0.00001F < d && d <0.00001F)
		return *this;

	d = 1.0F/d;

	this->_11 =  v->_11;
	this->_12 = -v->_12;
	this->_21 = -v->_21;
	this->_21 =  v->_22;

	this->_11 *= d;	this->_12 *= d;
	this->_21 *= d;	this->_22 *= d;

	return *this;
}


FLOAT LCXMATRIX2X2::Determinant()
{
	FLOAT d = _11 * _22 - _12 * _21;
	return d;
}


LCXMATRIX2X2& LCXMATRIX2X2::Scaling(FLOAT X, FLOAT Y)
{
	_11 = X   ;  _12 = 0.0F;
	_21 = 0.0F;  _22 = Y   ;

	return *this;
}


LCXMATRIX2X2& LCXMATRIX2X2::Scaling(const LCXVECTOR2* vScl)
{
	return Scaling(vScl->x, vScl->y);
}


LCXMATRIX2X2& LCXMATRIX2X2::Rotation(FLOAT Rad)
{
	FLOAT Cos = LCXMathCos(Rad);
	FLOAT Sin = LCXMathSin(Rad);

	_11 =+Cos;	_12 = -Sin;
	_21 =+Sin;	_22 = +Cos;

	return *this;
}

void LCXMATRIX2X2::Transform(LCXVECTOR2* Out, const LCXVECTOR2* pIn)
{
	FLOAT _x, _y;
	FLOAT x, y;

	_x = pIn->x;
	_y = pIn->y;

	x = _11 * _x  +  _21 * _y;
	y = _12 * _x  +  _22 * _y;

	Out->x =x;
	Out->y =y;
}

LCXMATRIX2X2& LCXMATRIX2X2::Negative()
{
	_11 = -_11;  _12 = -_12;
	_21 = -_21;  _22 = -_22;

	return *this;
}


LCXMATRIX2X2& LCXMATRIX2X2::Lerp(const LCXMATRIX2X2* m1, const LCXMATRIX2X2* m2, FLOAT t) // this = (1-t)*v1 + t* v2
{
	FLOAT k = 1.0F - t;
	_11 = k * m1->_11 + t * m2->_11;  _12 = k * m1->_12 + t * m2->_12;
	_21 = k * m1->_21 + t * m2->_21;  _22 = k * m1->_22 + t * m2->_22;

	return *this;
}




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// LCXRECT

LCXRECT::LCXRECT()						{	x=0.0F; y=0.0F; w=0.0F; h=0.0F;	}
LCXRECT::LCXRECT(const FLOAT* v)		{	if(!v) return; x=v[0]; y=v[1]; w=v[2]; h=v[3];	}
LCXRECT::LCXRECT(const LCXRECT* v)		{	if(!v) return; x=v->x; y=v->y; w=v->w; h=v->h;	}
LCXRECT::LCXRECT(FLOAT X, FLOAT Y, FLOAT W, FLOAT H){	x=X; y=Y; w=W; h=H;	}
LCXRECT::LCXRECT(const LC_RECTF& v)	{	x=v.x; y=v.y; w=v.w; h=v.h;	}
LCXRECT::LCXRECT(const LC_RECT& v)	{	x=(FLOAT)v.x; y=(FLOAT)v.y; w=(FLOAT)v.w; h=(FLOAT)v.h;	}

LCXRECT::LCXRECT(const LCXRECT& v)  {   *this = v;      }
LCXRECT& LCXRECT::operator=(const LCXRECT& v)
{
	x=v.x; y=v.y; w=v.w; h=v.h;
	return *this;
}


// casting
LCXRECT::operator FLOAT*()				{	return(FLOAT*)&x;			}
LCXRECT::operator const FLOAT*() const	{	return(const FLOAT*)&x;		}
FLOAT& LCXRECT::operator[](int n)		{	return *((&x)+ n);			}

// unary operators
LCXRECT LCXRECT::operator +() const	{	return *this;					}
LCXRECT LCXRECT::operator -() const	{	return LCXRECT(-x,-y,w,h);		}

INT LCXRECT::operator ==(const LCXRECT& v) const{ return LCXIsEqual(x, v.x)    && LCXIsEqual(y, v.y)    && LCXIsEqual(w, v.w)    && LCXIsEqual(h,v.h);	}
INT LCXRECT::operator !=(const LCXRECT& v) const{ return LCXIsNotEqual(x, v.x) || LCXIsNotEqual(y, v.y) || LCXIsNotEqual(w, v.w) || LCXIsNotEqual(h,v.h);	}

LCXRECT& LCXRECT::operator+=(const LCXVECTOR2& v) {	x += v.x; y += v.y;	return *this;	};
LCXRECT& LCXRECT::operator-=(const LCXVECTOR2& v) {	x -= v.x; y -= v.y;	return *this;	};
LCXRECT& LCXRECT::operator*=(FLOAT v)             {	x *= v;	y *= v; w *=v; h *=v; return *this;	};
LCXRECT& LCXRECT::operator/=(FLOAT v)             {	x /= v;	y /= v; w /=v; h /=v; return *this;	};

void LCXRECT::ToRegion()
{
	w = right  - left;
	h = bottom - top;
}

void LCXRECT::ToRegion(LCXRECT* v)
{
	x = v->left;
	y = v->top;
	w = v->right  - v->left;
	h = v->bottom - v->top;
}

void LCXRECT::ToRegion(FLOAT tLeft, FLOAT tTop, FLOAT tRight, FLOAT tBottom)
{
	x = tLeft;
	y = tTop;
	w = tRight  - tLeft;
	h = tBottom - tTop;
}

void LCXRECT::ToRect()
{
	right = x + w;
	bottom= y + h;
}

void LCXRECT::ToRect(LCXRECT* v)
{
	left  = v->x;
	top   = v->y;
	right = v->x + v->w;
	bottom= v->y + v->h;
}

INT LCXRECT::IsInRect(const FLOAT* v)
{
	if( x <= v[0] && v[0] <= x + w &&
		y <= v[1] && v[1] <= y + h )
		return LC_OK;

	return LC_EFAIL;
}

INT LCXRECT::IsInRect(const LCXVECTOR2* v)
{
	if( x <= v->x && v->x <= x + w &&
		y <= v->y && v->y <= y + h )
		return LC_OK;

	return LC_EFAIL;
}

INT LCXRECT::IsInRect(const LCXRECT* v)
{
	if( w >= v->w &&
		w >= v->h &&

		x <= v->x && v->x <= x + w &&
		y <= v->y && v->y <= y + h &&

		v->x <= x && x <= v->x + v->w &&
		v->y <= y && y <= v->x + v->h
		)
		return LC_OK;

	return LC_EFAIL;
}


INT LCXRECT::Collision(const FLOAT* v)
{
	if(	x     <= v[0] + v[2] &&	// left   <= v->right
		x + w >= v[0]        &&	// right  >= v->left
		y     <= v[1] + v[3] &&	// top    <= v->bottom
		y + h >= v[1]			// bottom >= v->top
		)
		return LC_OK;

	return LC_EFAIL;
}


INT LCXRECT::Collision(const LCXRECT* v)
{
	if(	x     <= v->x + v->w &&	// left   <= v->right
		x + w >= v->x        &&	// right  >= v->left
		y     <= v->y + v->h &&	// top    <= v->bottom
		y + h >= v->y			// bottom >= v->top
		)
		return LC_OK;

	return LC_EFAIL;
}

LCXVECTOR2 LCXRECT::Get00()	   { return LCXVECTOR2(x  , y  ); }
LCXVECTOR2 LCXRECT::Get10()    { return LCXVECTOR2(x+w, y  ); }
LCXVECTOR2 LCXRECT::Get01()    { return LCXVECTOR2(x  , y+h); }
LCXVECTOR2 LCXRECT::Get11()    { return LCXVECTOR2(x+w, y+h); }
LCXVECTOR2 LCXRECT::GetCenter(){ return LCXVECTOR2(x+ w*.5F, y+ h*.5F); }
LC_RECT    LCXRECT::GetRECT()  { return LC_RECT( INT(x), INT(y), INT(x+w), INT(y+h) ); }
LCXRECT&   LCXRECT::Zero()     { x=0.0F; y=0.0F; w=0.0F; h=0.0F; return *this;}
LCXRECT&   LCXRECT::One ()     { x=1.0F; y=1.0F; w=1.0F; h=1.0F; return *this;}



//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// line

LCXLINE::LCXLINE(){p.x=p.y=p.z=t.x=t.y=t.z=0;}
LCXLINE::LCXLINE(const FLOAT* v)		{	if(v){p.x=v[0];p.y=v[1];p.z=v[2];t.x=v[3];t.y=v[4];t.z=v[5];}else{p.x=p.y=p.z=t.x=t.y=t.z=0;}}
LCXLINE::LCXLINE(FLOAT Px,FLOAT Py,FLOAT Pz,FLOAT Tx,FLOAT Ty,FLOAT Tz) : p(Px,Py,Pz), t(Tx,Ty,Tz){}
LCXLINE::LCXLINE(const LCXVECTOR3& P, const LCXVECTOR3& T) : p(P), t(T){}

LCXLINE::LCXLINE(const LCXLINE& v)   {   *this = v;      }
LCXLINE& LCXLINE::operator=(const LCXLINE& v)
{
	p = v.p; t = v.t;
	return *this;
}


LCXLINE::operator FLOAT*()						{	return (FLOAT *) &p.x;			}
LCXLINE::operator const FLOAT*() const			{	return (const FLOAT *) &p.x;	}
LCXLINE LCXLINE::operator+() const				{	return LCXLINE(p.x, p.y, p.z, t.x, t.y, t.z);		}
LCXLINE LCXLINE::operator-() const				{	return LCXLINE(-p.x, -p.y, -p.z, -t.x, -t.y, -t.z);	}
bool LCXLINE::operator==(const LCXLINE& v) const{	return p.x == v.p.x && p.y == v.p.y && p.z == v.p.z && t.x == v.t.x && t.y == v.t.y && t.z == v.t.z;	}
bool LCXLINE::operator!=(const LCXLINE& v) const{	return p.x != v.p.x || p.y != v.p.y || p.z != v.p.z || t.x != v.t.x || t.y != v.t.y || t.z != v.t.z;	}




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// sphere

LCXSPHERE::LCXSPHERE(){p.x=p.y=p.z=r=0;}
LCXSPHERE::LCXSPHERE(const FLOAT* v)			{	if(v){p.x=v[0];p.y=v[1];p.z=v[2];r=v[3];}else{p.x=p.y=p.z=r=0;}}
LCXSPHERE::LCXSPHERE( FLOAT X,FLOAT Y,FLOAT Z,FLOAT R){	p.x=X;p.y=Y;p.z=Z;r=R;}

LCXSPHERE::LCXSPHERE(const LCXSPHERE& v)   {   *this = v;      }
LCXSPHERE& LCXSPHERE::operator=(const LCXSPHERE& v)
{
	p = v.p; r = v.r;
	return *this;
}


LCXSPHERE::operator FLOAT*()					{	return (FLOAT *) &p.x;				}
LCXSPHERE::operator const FLOAT*() const		{	return (const FLOAT *) &p.x;		}
LCXSPHERE LCXSPHERE::operator+() const			{	return *this;													}
LCXSPHERE LCXSPHERE::operator-() const			{	return LCXSPHERE(-p.x, -p.y, -p.z, r);							}
bool LCXSPHERE::operator==(const LCXSPHERE& v) const{	return p.x == v.p.x && p.y == v.p.y && p.z == v.p.z && r == v.r;	}
bool LCXSPHERE::operator!=(const LCXSPHERE& v) const{	return p.x != v.p.x || p.y != v.p.y || p.z != v.p.z || p.y != v.r;	}



//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8



LCXELLIPSOID::LCXELLIPSOID()
{
	a = 0.0F;
	b = 0.0F;
	c = LCXVECTOR2(0.0F, 0.0F);
}

LCXELLIPSOID::LCXELLIPSOID(const LCXELLIPSOID& v)
{
	*this = v;
}

LCXELLIPSOID& LCXELLIPSOID::operator=(const LCXELLIPSOID& v)
{
	a = v.a;
	b = v.b;
	c = v.c;
	return *this;
}

INT LCXELLIPSOID::Collision(const LCXVECTOR2* p)
{
	LCXVECTOR2	v = *p - this->c;

	FLOAT Det = (v.x * v.x)/(this->a * this->a) + (v.y * v.y)/(this->b * this->b);

	if(Det<1.0F)
		return LC_OK;


	return LC_EFAIL;
}


INT LCXEllipseToPointCollision(LCXELLIPSOID* e, const LCXVECTOR2* p)
{
	return e->Collision(p);
}


//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// Cylinder

LCXCYLINDER::LCXCYLINDER()
{
}

LCXCYLINDER::LCXCYLINDER(const FLOAT* v)
{
	if(v)
	{
		p.x=v[0];p.y=v[1];p.z=v[2];
		t.x=v[3];t.y=v[4];t.z=v[5];
		r=v[6];  h=v[7];
	}
	else
	{
		p.x=p.y=p.z= t.x=t.y=t.z=r=h=0;
	}
}

LCXCYLINDER::LCXCYLINDER(FLOAT Px,FLOAT Py,FLOAT Pz,FLOAT Tx,FLOAT Ty,FLOAT Tz, FLOAT R, FLOAT H)
{
	p.x = Px; p.y = Py; p.z = Pz;
	t.x = Tx; t.y = Ty; t.z = Tz;
	r	= R;  h	= H;
}


LCXCYLINDER::LCXCYLINDER(const LCXVECTOR3& P, const LCXVECTOR3& T, FLOAT R, FLOAT H)
{
	p = P;
	t = T;
	r = R;
	h = H;
}

LCXCYLINDER::LCXCYLINDER(const LCXCYLINDER& v)   {   *this = v;      }
LCXCYLINDER& LCXCYLINDER::operator=(const LCXCYLINDER& v)
{
	p = v.p;
	t = v.t;
	r = v.r;
	h = v.h;
	return *this;
}


LCXCYLINDER::operator FLOAT*()
{
	return (FLOAT *) &p.x;
}


LCXCYLINDER::operator const FLOAT*() const
{
	return (const FLOAT *) &p.x;
}


bool LCXCYLINDER::operator==(const LCXCYLINDER& v) const
{
	return	p.x == v.p.x && p.y == v.p.y && p.z == v.p.z &&
			t.x == v.t.x && t.y == v.t.y && t.z == v.t.z &&
			r == v.r &&
			h == v.h;
}


bool LCXCYLINDER::operator!=(const LCXCYLINDER& v) const
{
	return	p.x != v.p.x || p.y != v.p.y || p.z != v.p.z ||
			t.x != v.t.x || t.y != v.t.y || t.z != v.t.z ||
			r != v.r ||
			h != v.h;
}

void LCXCYLINDER::Set(const LCXVECTOR3& p0, const LCXVECTOR3& p1, FLOAT R, FLOAT H)
{
	p = p0;
	t = p1 - p;
	r = R;
	h = H;

	LCXVec3Normalize(&t, &t);
}




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// transform object

LCXTRANSFORM::LCXTRANSFORM()
{
	Clear();
}

LCXTRANSFORM::LCXTRANSFORM(const LCXTRANSFORM& v)   {   *this = v;      }
LCXTRANSFORM& LCXTRANSFORM::operator=(const LCXTRANSFORM& v)
{
	m = v.m;
	s = v.s;
	t = v.t;
	r = v.r;
	e = v.e;
	return *this;
}


void LCXTRANSFORM::Clear()
{
	s.x = s.y = s.z = 1.0F;
	r.x = r.y = r.w = r.z = 0.0F;
	t.x = t.y = t.z = 0.0F;
	e.x = e.y = e.z = e.w = 0.0F;

	m._11 = m._22 = m._33 = m._44 = 1.0F;
	        m._12 = m._13 = m._14 = 0.0F;
	m._21 =         m._23 = m._24 = 0.0F;
	m._31 = m._32 =         m._34 = 0.0F;
	m._41 = m._42 = m._43         = 0.0F;
}

void LCXTRANSFORM::Scaling(const LCXVECTOR3* v)
{
	if( s.x == v->x && s.y == v->y && s.z == v->z)
		return;

	s   =*v;
	Setup();
}

void LCXTRANSFORM::Scaling(FLOAT x,FLOAT y,FLOAT z)
{
	if( s.x == x    && s.y ==    y && s.z == z   )
		return;

	s.x = x; s.y = y; s.z = z;
	Setup();
}

void LCXTRANSFORM::Rotation(const LCXVECTOR4* v)
{
	if( r.x == v->x && r.y == v->y && r.z == v->z && r.w == v->w)
		return;

	r   =*v;
	Setup();
}

void LCXTRANSFORM::Rotation (FLOAT x,FLOAT y,FLOAT z, FLOAT w)
{
	if( r.x == x    && r.y ==    y && r.z == z    && r.w == z   )
		return;

	r.x = x; r.y = y; r.z = z; r.w = w;
	Setup();
}

void LCXTRANSFORM::Translation(const LCXVECTOR3* v)
{
	if( t.x == v->x && t.y == v->y && t.z == v->z)
		return;

	t   = *v;
	m._41 = t.x + e.x;
	m._42 = t.y + e.y;
	m._43 = t.z + e.z;
}

void LCXTRANSFORM::Translation(FLOAT x,FLOAT y,FLOAT z)
{
	if( t.x == x    && t.y ==    y && t.z == z   )
		return;

	t.x = x; t.y = y; t.z = z;
	m._41 = t.x + e.x;
	m._42 = t.y + e.y;
	m._43 = t.z + e.z;
}


void LCXTRANSFORM::Epsilon(const LCXVECTOR3* v)
{
	if( e.x == v->x && e.y == v->y && e.z == v->z)
		return;

	e.x = v->x; e.y = v->y; e.z = v->z;
	m._41 = t.x + e.x;
	m._42 = t.y + e.y;
	m._43 = t.z + e.z;
}


void LCXTRANSFORM::Epsilon(const LCXVECTOR4* v)
{
	if( e.x == v->x && e.y == v->y && e.z == v->z && e.w == v->w)
		return;

	e.x = v->x; e.y = v->y; e.z = v->z; e.w = v->w;
	m._41 = t.x + e.x;
	m._42 = t.y + e.y;
	m._43 = t.z + e.z;
}


void LCXTRANSFORM::Epsilon(FLOAT v)
{
	if( e.w == v)
		return;

	e.w = v;
	Setup();
}



void LCXTRANSFORM::TM(LCXMATRIX* _m){ m= *_m;                                      }

const LCXVECTOR3* const LCXTRANSFORM::Scaling     () const { return (LCXVECTOR3*)&s.x;   }
const LCXVECTOR4* const LCXTRANSFORM::Rotation    () const { return (LCXVECTOR4*)&r.x;   }
const LCXVECTOR3* const LCXTRANSFORM::Translation () const { return (LCXVECTOR3*)&t.x;   }
const LCXMATRIX*  const LCXTRANSFORM::TM          () const { return (LCXMATRIX* )&m._11; }
const LCXVECTOR4* const LCXTRANSFORM::Epsilon     () const { return (LCXVECTOR4*)&e.x;   }

void LCXTRANSFORM::Setup()
{
	LCXMATRIX	v;

	v.RotationAxis(r.x, r.y, r.z, r.w + e.w);

	m.Scaling(&s);
	m *= v;

	m._41 = t.x + e.x;
	m._42 = t.y + e.y;
	m._43 = t.z + e.z;
}




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8


LCXTRANSFORM2D::LCXTRANSFORM2D()
{
	Clear();
}

LCXTRANSFORM2D::LCXTRANSFORM2D(const LCXTRANSFORM2D& v)   {   *this = v;      }
LCXTRANSFORM2D& LCXTRANSFORM2D::operator=(const LCXTRANSFORM2D& v)
{
	m = v.m;
	s = v.s;
	t = v.t;
	r = v.r;
	e = v.e;
	return *this;
}


void LCXTRANSFORM2D::Clear()
{
	s.x = s.y = 1.0F;
	r.x = r.z = r.y = 0.0F;
	t.x = t.y = t.z = 0.0F;
	e.x = e.y = e.z = e.w = 0.0F;

	m._11 = m._22 = m._33 = m._44 = 1.0F;
	        m._12 = m._13 = m._14 = 0.0F;
	m._21 =         m._23 = m._24 = 0.0F;
	m._31 = m._32 =         m._34 = 0.0F;
	m._41 = m._42 = m._43         = 0.0F;
}


void LCXTRANSFORM2D::Scaling(const LCXVECTOR2* v)
{
	if(s.x == v->x && s.y == v->y)
		return;

	s = *v;
	Setup();
}

void LCXTRANSFORM2D::Scaling(FLOAT x, FLOAT y)
{
	if(s.x == x && s.y == y )
		return;

	s.x = x; s.y = y;
	Setup();
}

void LCXTRANSFORM2D::Rotation(const LCXVECTOR3* v)
{
	if(r.x == v->x && r.y == v->y && r.z == v->z)
		return;

	r = *v;
	Setup();
}

void LCXTRANSFORM2D::Rotation(FLOAT x,FLOAT y,FLOAT z)
{
	if( r.x == x && r.y == y && r.z == z)
		return;

	r.x = x; r.y = y; r.z = z;
	Setup();
}

void LCXTRANSFORM2D::Translation (const LCXVECTOR2* v)
{
	if(t.x == v->x && t.y == v->y)
		return;

	t.x =v->x; t.y =v->y; m._41 = t.x; m._42=t.y;
}

void LCXTRANSFORM2D::Translation (FLOAT x,FLOAT y)
{
	if( t.x == x && t.y == y)
		return;

	t.x = x; t.y = y;
	m._41 = t.x; m._42=t.y;
}

void LCXTRANSFORM2D::Zvalue(const FLOAT z)
{
	if( t.z == z)
		return;

	t.z = z;
	m._43 = t.z;
}

void LCXTRANSFORM2D::TM(LCXMATRIX* _m)
{
	m= *_m;
}


void LCXTRANSFORM2D::Epsilon(const LCXVECTOR3* v)
{
	if( e.x == v->x && e.y == v->y && e.z == v->z)
		return;

	e.x = v->x; e.y = v->y; e.z = v->z;
	m._41 = t.x + e.x;
	m._42 = t.y + e.y;
	m._43 = t.z + e.z;
}


void LCXTRANSFORM2D::Epsilon(const LCXVECTOR4* v)
{
	if( e.x == v->x && e.y == v->y && e.z == v->z && e.w == v->w)
		return;

	e.x = v->x; e.y = v->y; e.z = v->z; e.w = v->w;
	m._41 = t.x + e.x;
	m._42 = t.y + e.y;
	m._43 = t.z + e.z;
}


void LCXTRANSFORM2D::Epsilon(FLOAT v)
{
	if( e.w == v)
		return;

	e.w = v;
	Setup();
}


const LCXMATRIX*  const LCXTRANSFORM2D::TM          () const { return (LCXMATRIX* )&m._11; }
const LCXVECTOR3* const LCXTRANSFORM2D::Scaling     () const { return (LCXVECTOR3*)&s.x;   }
const LCXVECTOR4* const LCXTRANSFORM2D::Rotation    () const { return (LCXVECTOR4*)&r.x;   }
const LCXVECTOR3* const LCXTRANSFORM2D::Translation () const { return (LCXVECTOR3*)&t.x;   }
FLOAT                   LCXTRANSFORM2D::Zvalue      () const { return m._43;               }
const LCXVECTOR4* const LCXTRANSFORM2D::Epsilon     () const { return (LCXVECTOR4*)&e.x;   }

void LCXTRANSFORM2D::Setup()
{
	//LCXVECTOR3 t(m._41, m._42, m._43);
	FLOAT rad = r.z + e.w;

	FLOAT COS = (FLOAT)LCXMathCos(rad);
	FLOAT SIN = (FLOAT)LCXMathSin(rad);

	LCXMATRIX3X3 mtRot(COS, -SIN,  0,  SIN, COS, 0,       0,      0, 1);
	LCXMATRIX3X3 mtTrI(  1,    0,  0,    0,   1, 0,  -r.x, -r.y, 1);
	LCXMATRIX3X3 mtTrO(  1,    0,  0,    0,   1, 0,   r.x,  r.y, 1);

	LCXMATRIX3X3 mtWld = (mtTrI * mtRot * mtTrO);

	LCXMatrix4X4From3X3Tm( (LCXMATRIX*)&m, &mtWld);

	LCXVECTOR3 vcScl(s.x, s.y, 1.0F);
	LCXMATRIX mtScl;
	mtScl.Scaling(&vcScl);

	m = mtScl * m;

	m._41 = t.x + e.x;
	m._42 = t.y + e.y;
	m._43 = t.z + e.z;
}

