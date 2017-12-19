// GalicSoft Game Library Collection. Copyright(C) GALICSOFT. All rights reserved.
//
// base types, values and architectures
//
////////////////////////////////////////////////////////////////////////////////

#if defined(_PC_WINDOW_)
  #pragma once
#endif

#ifndef _LcBase_H_
#define _LcBase_H_

struct TLC_ARGS;
struct TLC_ATTRB;

#if defined(_PC_WINDOW_)
  #pragma pack(push, 1)
#endif

struct QINT																		// for object id(128bit)::little endian
{
	union
	{
		struct{ INT    a, b, c, d; };
		struct{ INT   _0,_1,_2,_3; };
		struct{ SHORT s0,s1,s2,s3,s4,s5,s6,s7;};
		struct{ DINT   p,q;        };
		struct{ char  t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,ta,tb,tc,td,te,tf;};
		INT i[4]; SHORT s[8]; char t[16];										// 32, 64, 16 bit id
	};

	QINT();
	QINT(const QINT*);
	QINT(INT  _a, INT  _b=0,INT _c=0,INT _d=0);
	QINT(DINT _p, DINT _q=0);
	QINT(TLC_ARGS&);
	QINT(const QINT&);

	// casting
	operator INT();																// return this->a
	operator const INT() const;

	operator DINT();															// return n[0]
	operator const DINT() const;

	// assignment
	QINT& operator=(const QINT&);
	QINT& operator=(const INT  );

	// arithmetic
	//unary operators
	QINT operator +() const;
	QINT operator -() const;

	QINT operator ++(   );
	QINT operator ++(int);
	QINT operator --(   );
	QINT operator --(int);

	// binary operators
	QINT operator +(const QINT&) const;

	// compare
	INT  operator == (const QINT&) const;
	INT  operator != (const QINT&) const;


	QINT& Null    ();	// to 0x0000000000000000
	QINT& Default ();	// to 0xFFFFFFFFFFFFFFFF
}
#if defined(_PC_WINDOW_)
  #pragma pack(pop)
#else
  __attribute__((packed))
#endif
;


struct LC_QID : public QINT
{
	INT				Id(INT n=0);												// return the id
	CONF(INT*)		Ids() const;												// return the ids
	void			Ids(INT*, INT*, INT* =0, INT* =0);							// get the id

	void			Id(INT n, INT v);											// set the id
	void			Ids(const INT*);											// set the id int *4 array
	void			Ids(INT, INT =0, INT =0, INT =0);							// set the id

	SHORT			Wid(INT n=0);												// return the id
	CONF(SHORT*)	Wids() const;												// return the ids
	void			Wids(SHORT*   , SHORT*   , SHORT* =0, SHORT* =0
						,SHORT* =0, SHORT* =0, SHORT* =0, SHORT* =0);			// get the id

	void			Wid(INT n, SHORT v);										// set the id
	void			Wids(const SHORT*);											// set the id int *4 array
	void			Wids(SHORT   , SHORT =0, SHORT =0, SHORT =0
						,SHORT =0, SHORT =0, SHORT =0, SHORT =0);				// set the id
};


//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// 2D, 3D basic structure

struct LC_INT2_  { union{ struct{ INT x, y;			 }; INT m[2]; }; };
struct LC_INT3_  { union{ struct{ INT x, y, z;          }; INT m[3]; }; };
struct LC_INT4_  { union{ struct{ INT x, y, z, w;       }; INT m[4]; }; };
struct LC_INT6_  { union{ struct{ INT x, y, z, w, p, q; }; INT m[6]; }; };
struct LC_INT8_  { union{ struct{ INT _0,_1,_2,_3,_4,_5,_6,_7; }; INT m[8]; }; };

struct LC_FLOAT2_{ union{ struct{ FLOAT x, y;     }; INT m[2]; }; };
struct LC_FLOAT3_{ union{ struct{ FLOAT x, y, z;  }; INT m[3]; }; };
struct LC_FLOAT4_{ union{
		struct	{ FLOAT x, y, z, w; };
		struct	{ FLOAT a, b, c, d; };
		FLOAT m[4];
	};
};

struct LC_FLOAT6_ { union{ struct{ FLOAT x, y, z, w, p, q; }; FLOAT m[6]; }; };
struct LC_FLOAT8_ { union{ struct{ FLOAT _0,_1,_2,_3,_4,_5,_6,_7; }; FLOAT m[8]; }; };


struct LC_POINT2
{
	INT x, y;

	LC_POINT2()										;
	LC_POINT2(INT X, INT Y)							;
	LC_POINT2(const LC_POINT2&)						;
	LC_POINT2& operator=(const LC_POINT2&)			;

	operator INT*()									;
	operator const INT*() const						;
	LC_POINT2& operator+=(const LC_POINT2&)			;
	LC_POINT2& operator-=(const LC_POINT2&)			;
	INT operator == (const LC_POINT2&) const		;
	INT operator != (const LC_POINT2&) const		;
};


struct LC_POINT3
{
	INT x, y, z;

	LC_POINT3()										;
	LC_POINT3(INT X, INT Y, INT Z)					;
	LC_POINT3(const LC_POINT3&)						;
	LC_POINT3& operator=(const LC_POINT3&)			;

	operator INT*()									;
	operator const INT*() const						;
	LC_POINT3& operator+=(const LC_POINT3&)			;
	LC_POINT3& operator-=(const LC_POINT3&)			;
	INT operator == (const LC_POINT3&) const		;
	INT operator != (const LC_POINT3&) const		;
};


struct LC_POINT4
{
	INT x, y, z, w;

	LC_POINT4()										;
	LC_POINT4(INT X, INT Y, INT Z, INT W)			;
	LC_POINT4(const LC_POINT4&)						;
	LC_POINT4& operator=(const LC_POINT4&)			;

	operator INT*()									;
	operator const INT*() const						;
	LC_POINT4& operator+=(const LC_POINT4&)			;
	LC_POINT4& operator-=(const LC_POINT4&)			;
	INT operator == (const LC_POINT4&) const		;
	INT operator != (const LC_POINT4&) const		;
};


struct LC_RECT
{
	union
	{
		struct{ INT x,   y,  w,    h;     }			;	// w ==> width, h==>height
		struct{ INT l,   t,  r,    b;     }			;
		struct{ INT x0,  y0, x1,   y1;    }			;
		struct{ INT left,top,right,bottom;}			;
		FLOAT m[4];
	};

	LC_RECT()										;
	LC_RECT(INT X, INT Y, INT W, INT H)				;
	LC_RECT(const LC_RECT&)							;
	LC_RECT& operator=(const LC_RECT&)				;

	operator INT*()									;
	operator const INT*() const						;
	INT operator == (const LC_RECT&) const			;
	INT operator != (const LC_RECT&) const			;
};


#if defined(_PC_WINDOW_)
  #pragma pack(push, 1)
#endif
struct LC_BYTE3									// caution the byte order
{
	union
	{
		struct{ BYTE  a, b, c;};
		struct{ BYTE  x, y, z;};
		struct{ BYTE _0,_1,_2;};
		BYTE m[3];
	};

	LC_BYTE3()										;
	LC_BYTE3(BYTE X, BYTE Y, BYTE Z)				;
	LC_BYTE3(const LC_BYTE3&)						;
	LC_BYTE3& operator=(const LC_BYTE3&)			;

	operator BYTE*()								;
	operator const BYTE*() const					;
	INT operator == (const LC_BYTE3&) const			;
	INT operator != (const LC_BYTE3&) const			;
}
#if defined(_PC_WINDOW_)
  #pragma pack(pop)
#else
  __attribute__((packed))
#endif
;


#if defined(_PC_WINDOW_)
  #pragma pack(push, 1)
#endif
struct LC_SHORT2
{
	union
	{
		struct{ SHORT  a, b;};
		struct{ SHORT  x, y;};
		struct{ SHORT _0,_1;};
		SHORT m[2];
	};

	LC_SHORT2()										;
	LC_SHORT2(SHORT X, SHORT Y)						;
	LC_SHORT2(const LC_SHORT2&)						;
	LC_SHORT2& operator=(const LC_SHORT2&)			;

	operator SHORT*()								;
	operator const SHORT*() const					;
	INT operator == (const LC_SHORT2&) const		;
	INT operator != (const LC_SHORT2&) const		;
}
#if defined(_PC_WINDOW_)
  #pragma pack(pop)
#else
  __attribute__((packed))
#endif
;


#if defined(_PC_WINDOW_)
  #pragma pack(push, 1)
#endif
struct LC_WORD2
{
	union
	{
		struct{ WORD  a, b;};
		struct{ WORD  x, y;};
		struct{ WORD _0,_1;};
		WORD m[2];
	};

	LC_WORD2()										;
	LC_WORD2(WORD X, WORD Y)						;
	LC_WORD2(const LC_WORD2&)						;
	LC_WORD2& operator=(const LC_WORD2&)			;

	operator WORD*()								;
	operator const WORD*() const					;
	INT operator == (const LC_WORD2&) const			;
	INT operator != (const LC_WORD2&) const			;
}
#if defined(_PC_WINDOW_)
  #pragma pack(pop)
#else
  __attribute__((packed))
#endif
;


#if defined(_PC_WINDOW_)
  #pragma pack(push, 1)
#endif
struct LC_SHORT3
{
	union
	{
		struct{ SHORT  a, b, c;};
		struct{ SHORT  x, y, z;};
		struct{ SHORT _0,_1,_2;};
		SHORT m[3];
	};

	LC_SHORT3()										;
	LC_SHORT3(SHORT X, SHORT Y, SHORT Z)			;
	LC_SHORT3(const LC_SHORT3&)						;
	LC_SHORT3& operator=(const LC_SHORT3&)			;

	operator SHORT*()								;
	operator const SHORT*() const					;
	INT operator == (const LC_SHORT3&) const		;
	INT operator != (const LC_SHORT3&) const		;
}
#if defined(_PC_WINDOW_)
  #pragma pack(pop)
#else
  __attribute__((packed))
#endif
;


#if defined(_PC_WINDOW_)
  #pragma pack(push, 1)
#endif
struct LC_WORD3
{
	union
	{
		struct{ WORD  a, b, c;};
		struct{ WORD  x, y, z;};
		struct{ WORD _0,_1,_2;};
		WORD m[3];
	};

	LC_WORD3()										;
	LC_WORD3(WORD X, WORD Y, WORD Z)				;
	LC_WORD3(const LC_WORD3&)						;
	LC_WORD3& operator=(const LC_WORD3&)			;

	operator WORD*()								;
	operator const WORD*() const					;
	INT operator == (const LC_WORD3&) const			;
	INT operator != (const LC_WORD3&) const			;
}
#if defined(_PC_WINDOW_)
  #pragma pack(pop)
#else
  __attribute__((packed))
#endif
;


#if defined(_PC_WINDOW_)
  #pragma pack(push, 1)
#endif
struct LC_SHORT4
{
	union
	{
		struct{ SHORT  a, b, c, d;};
		struct{ SHORT  x, y, z, w;};
		struct{ SHORT _0,_1,_2,_3;};
		SHORT m[4];
	};

	LC_SHORT4()										;
	LC_SHORT4(SHORT X, SHORT Y, SHORT Z, SHORT W)	;
	LC_SHORT4(const LC_SHORT4&)						;
	LC_SHORT4& operator=(const LC_SHORT4&)			;

	operator SHORT*()								;
	operator const SHORT*() const					;
	INT operator == (const LC_SHORT4&) const		;
	INT operator != (const LC_SHORT4&) const		;
}
#if defined(_PC_WINDOW_)
  #pragma pack(pop)
#else
  __attribute__((packed))
#endif
;


#if defined(_PC_WINDOW_)
  #pragma pack(push, 1)
#endif
struct LC_WORD4
{
	union
	{
		struct{ WORD  a, b, c, d;};
		struct{ WORD  x, y, z, w;};
		struct{ WORD _0,_1,_2,_3;};
		WORD m[4];
	};

	LC_WORD4()										;
	LC_WORD4(WORD X, WORD Y, WORD Z, WORD W)		;
	LC_WORD4(const LC_WORD4&)						;
	LC_WORD4& operator=(const LC_WORD4&)			;

	operator WORD*()								;
	operator const WORD*() const					;
	INT operator == (const LC_WORD4&) const			;
	INT operator != (const LC_WORD4&) const			;
}
#if defined(_PC_WINDOW_)
  #pragma pack(pop)
#else
  __attribute__((packed))
#endif
;


struct LC_INT2
{
	union
	{
		struct{ INT  a, b;};
		struct{ INT  x, y;};
		struct{ INT _0,_1;};
		INT m[2];
	};

	LC_INT2()										;
	LC_INT2(INT X, INT Y)							;
	LC_INT2(const LC_INT2&)							;
	LC_INT2& operator=(const LC_INT2&)				;

	operator INT*()									;
	operator const INT*() const						;
	LC_INT2& operator+=(const LC_INT2&)				;
	LC_INT2& operator-=(const LC_INT2&)				;
	INT operator == (const LC_INT2&) const			;
	INT operator != (const LC_INT2&) const			;
};


struct LC_INT3
{
	union
	{
		struct{ INT  a, b, c;};
		struct{ INT  x, y, z;};
		struct{ INT _0,_1,_2;};
		INT m[3];
	};

	LC_INT3()										;
	LC_INT3(INT X, INT Y, INT Z)					;
	LC_INT3(const LC_INT3&)							;
	LC_INT3& operator=(const LC_INT3&)				;

	operator INT*()									;
	operator const INT*() const						;
	LC_INT3& operator+=(const LC_INT3&)				;
	LC_INT3& operator-=(const LC_INT3&)				;
	INT operator == (const LC_INT3&) const			;
	INT operator != (const LC_INT3&) const			;
};


struct LC_INT4
{
	union
	{
		struct{ INT  a, b, c, d;};
		struct{ INT  x, y, z, w;};
		struct{ INT _0,_1,_2,_3;};
		INT m[4];
	};

	LC_INT4()										;
	LC_INT4(INT X, INT Y, INT Z, INT W)				;
	LC_INT4(const LC_INT4&)							;
	LC_INT4& operator=(const LC_INT4&)				;

	operator INT*()									;
	operator const INT*() const						;
	LC_INT4& operator+=(const LC_INT4&)				;
	LC_INT4& operator-=(const LC_INT4&)				;
	INT operator == (const LC_INT4&) const			;
	INT operator != (const LC_INT4&) const			;
};


struct LC_INT6
{
	union
	{
		struct{ INT  a, b, c, d, e, f;};
		struct{ INT  x, y, z, w, p, q;};
		struct{ INT _0,_1,_2,_3,_4,_5;};
		INT m[6];
	};

	LC_INT6()										;
	LC_INT6(INT __0,INT __1,INT __2,
			INT __3,INT __4,INT __5)				;
	LC_INT6(const LC_INT6&)							;
	LC_INT6& operator=(const LC_INT6&)				;

	operator INT*()									;
	operator const INT*() const						;
	LC_INT6& operator+=(const LC_INT6&)				;
	LC_INT6& operator-=(const LC_INT6&)				;
	INT operator == (const LC_INT6&) const			;
	INT operator != (const LC_INT6&) const			;
};


struct LC_INT8
{
	union
	{
		struct{ INT  a, b, c, d, e, f, g, h;};
		struct{ INT  x, y, z, w, p, q, r, s;};
		struct{ INT _0,_1,_2,_3,_4,_5,_6,_7;};
		INT m[8];
	};

	LC_INT8()										;
	LC_INT8(INT __0,INT __1,INT __2,INT __3,
			INT __4,INT __5,INT __6,INT __7)		;
	LC_INT8(const LC_INT8&)							;
	LC_INT8& operator=(const LC_INT8&)				;

	operator INT*()									;
	operator const INT*() const						;
	LC_INT8& operator+=(const LC_INT8&)				;
	LC_INT8& operator-=(const LC_INT8&)				;
	INT operator == (const LC_INT8&) const			;
	INT operator != (const LC_INT8&) const			;
};


struct LC_FLOAT2
{
	union
	{
		struct{ FLOAT  x,  y; };
		struct{ FLOAT  a,  b; };
		struct{ FLOAT _0, _1; };
		struct{ FLOAT _w, _h; };
		FLOAT m[2];
	};

	LC_FLOAT2()										;
	LC_FLOAT2(FLOAT X, FLOAT Y)						;
	LC_FLOAT2(const FLOAT*)							;
	LC_FLOAT2(const LC_FLOAT2&)						;
	LC_FLOAT2& operator=(const LC_FLOAT2&)			;

	operator FLOAT*()								;
	operator const FLOAT*() const					;
	FLOAT& operator[](int)							;
	LC_FLOAT2& operator +=(const LC_FLOAT2&)		;
	LC_FLOAT2& operator -=(const LC_FLOAT2&)		;
	LC_FLOAT2& operator *=(FLOAT)					;
	LC_FLOAT2& operator /=(FLOAT)					;
	LC_FLOAT2 operator +() const					;
	LC_FLOAT2 operator -() const					;
	LC_FLOAT2 operator+(const LC_FLOAT2&) const		;
	LC_FLOAT2 operator-(const LC_FLOAT2&) const		;
	LC_FLOAT2 operator*(FLOAT) const				;
	LC_FLOAT2 operator/(FLOAT) const				;
	INT operator == (const LC_FLOAT2&) const		;
	INT operator != (const LC_FLOAT2&) const		;
};


struct LC_FLOAT3
{
	union
	{
		struct{ FLOAT  x,  y,  z; };
		struct{ FLOAT  a,  b,  c; };
		struct{ FLOAT _0, _1, _2; };
		struct{ FLOAT _w, _h, _s; };
		FLOAT m[3];
	};

	LC_FLOAT3()										;
	LC_FLOAT3(FLOAT X,FLOAT Y,FLOAT Z)				;
	LC_FLOAT3(const FLOAT*)							;
	LC_FLOAT3(const LC_FLOAT3&)						;
	LC_FLOAT3& operator=(const LC_FLOAT3&)			;

	// casting
	operator FLOAT*()								;
	operator const FLOAT*() const					;
	FLOAT& operator[](int)							;

	// assignment operators
	LC_FLOAT3& operator +=(const LC_FLOAT3&)		;
	LC_FLOAT3& operator -=(const LC_FLOAT3&)		;
	LC_FLOAT3& operator *=(FLOAT)					;
	LC_FLOAT3& operator /=(FLOAT)					;

	// unary operators
	LC_FLOAT3 operator +() const					;
	LC_FLOAT3 operator -() const					;

	// binary operators
	LC_FLOAT3 operator +(const LC_FLOAT3&) const	;
	LC_FLOAT3 operator -(const LC_FLOAT3&) const	;
	LC_FLOAT3 operator *(FLOAT) const				;
	LC_FLOAT3 operator /(FLOAT) const				;
	INT operator == (const LC_FLOAT3&) const		;
	INT operator != (const LC_FLOAT3&) const		;
};


struct LC_FLOAT4
{
	union
	{
		struct{ FLOAT  x,  y,  z,  w; };
		struct{ FLOAT  a,  b,  c,  d; };
		struct{ FLOAT _0, _1, _2, _3; };
		struct{ FLOAT _w, _h, _s, _t; };
		FLOAT m[4];
	};

	LC_FLOAT4()										;
	LC_FLOAT4(FLOAT X,FLOAT Y,FLOAT Z, FLOAT W)		;
	LC_FLOAT4(const FLOAT*)							;
	LC_FLOAT4(const LC_FLOAT4&)						;
	LC_FLOAT4& operator=(const LC_FLOAT4&)			;

	operator FLOAT*()								;
	operator const FLOAT*() const					;
	FLOAT& operator[](int)							;

	LC_FLOAT4& operator +=(const LC_FLOAT4&)		;
	LC_FLOAT4& operator -=(const LC_FLOAT4&)		;
	LC_FLOAT4& operator *=(FLOAT)					;
	LC_FLOAT4& operator /=(FLOAT)					;
	LC_FLOAT4 operator +() const					;
	LC_FLOAT4 operator -() const					;
	LC_FLOAT4 operator +(const LC_FLOAT4&) const	;
	LC_FLOAT4 operator -(const LC_FLOAT4&) const	;
	LC_FLOAT4 operator *(FLOAT) const				;
	LC_FLOAT4 operator /(FLOAT) const				;
	INT operator == (const LC_FLOAT4&) const		;
	INT operator != (const LC_FLOAT4&) const		;
};


struct LC_FLOAT6
{
	union
	{
		struct{ FLOAT    a,    b,    c,    d,    e,    f;};
		struct{ FLOAT    x,    y,    z,    w,    p,    q;};
		struct{ FLOAT   _0,   _1,   _2,   _3,   _4,   _5;};
		struct{ FLOAT rc_x, rc_y, rc_w, rc_h, rc_r, rc_s;};
		FLOAT m[6];
	};

	LC_FLOAT6()										;
	LC_FLOAT6(FLOAT __0,FLOAT __1,FLOAT __2,
			  FLOAT __3,FLOAT __4,FLOAT __5)		;
	LC_FLOAT6(const FLOAT*)							;
	LC_FLOAT6(const LC_FLOAT6&)						;
	LC_FLOAT6& operator=(const LC_FLOAT6&)			;

	operator FLOAT*()								;
	operator const FLOAT*() const					;
	FLOAT& operator[](int)							;

	LC_FLOAT6& operator +=(const LC_FLOAT6&)		;
	LC_FLOAT6& operator -=(const LC_FLOAT6&)		;
	LC_FLOAT6& operator *=(FLOAT)					;
	LC_FLOAT6& operator /=(FLOAT)					;
	LC_FLOAT6 operator +() const					;
	LC_FLOAT6 operator -() const					;
	LC_FLOAT6 operator +(const LC_FLOAT6&) const	;
	LC_FLOAT6 operator -(const LC_FLOAT6&) const	;
	LC_FLOAT6 operator *(FLOAT) const				;
	LC_FLOAT6 operator /(FLOAT) const				;
	INT operator == (const LC_FLOAT6&) const		;
	INT operator != (const LC_FLOAT6&) const		;
};


struct LC_FLOAT8
{
	union
	{
		struct{ FLOAT  a, b, c, d, e, f, g, h;};
		struct{ FLOAT  x, y, z, w, p, q, r, s;};
		struct{ FLOAT _0,_1,_2,_3,_4,_5,_6,_7;};
		FLOAT m[8];
	};

	LC_FLOAT8()										;
	LC_FLOAT8(FLOAT __0     , FLOAT __1=0.0F, FLOAT __2=0.0F, FLOAT __3=0.0F,
			  FLOAT __4=0.0F, FLOAT __5=0.0F, FLOAT __6=0.0F, FLOAT __7=0.0F);
	LC_FLOAT8(const FLOAT*)							;
	LC_FLOAT8(const LC_FLOAT8&)						;
	LC_FLOAT8& operator=(const LC_FLOAT8&)			;

	operator FLOAT*()								;
	operator const FLOAT*() const					;
	FLOAT& operator[](int)							;

	LC_FLOAT8& operator +=(const LC_FLOAT8&)		;
	LC_FLOAT8& operator -=(const LC_FLOAT8&)		;
	LC_FLOAT8& operator *=(FLOAT)					;
	LC_FLOAT8& operator /=(FLOAT)					;
	LC_FLOAT8 operator +() const					;
	LC_FLOAT8 operator -() const					;
	LC_FLOAT8 operator +(const LC_FLOAT8&) const	;
	LC_FLOAT8 operator -(const LC_FLOAT8&) const	;
	LC_FLOAT8 operator *(FLOAT) const				;
	LC_FLOAT8 operator /(FLOAT) const				;
	INT operator == (const LC_FLOAT8&) const		;
	INT operator != (const LC_FLOAT8&) const		;
};



struct LC_COLORF
{
	union{ struct{ FLOAT r,g,b,a; }; FLOAT m[4]; }	;

	LC_COLORF()										;
	LC_COLORF(FLOAT R,FLOAT G,FLOAT B,FLOAT A)		;
	LC_COLORF(COLOR32 D)							;
	LC_COLORF(const LC_COLORF&)						;
	LC_COLORF& operator=(const LC_COLORF&)			;

	operator COLOR32 () const						;
	operator FLOAT *()								;
	operator const FLOAT *() const					;
	FLOAT& operator[](int)							;

	LC_COLORF& operator +=(const LC_COLORF&)		;
	LC_COLORF& operator -=(const LC_COLORF&)		;
	LC_COLORF& operator *=(FLOAT)					;
	LC_COLORF& operator /=(FLOAT)					;
	LC_COLORF operator +() const					;
	LC_COLORF operator -() const					;	// Negative
	LC_COLORF operator +(const LC_COLORF& v) const	;
	LC_COLORF operator -(const LC_COLORF& v) const	;
	LC_COLORF operator *(FLOAT) const				;
	LC_COLORF operator /(FLOAT) const				;
	INT operator == (const LC_COLORF&) const		;
	INT operator != (const LC_COLORF&) const		;
};


struct LC_COLORB
{
	union{ struct{ BYTE r,g,b,a;}; BYTE m[4]; }		;	// Notice!! the byte order

	LC_COLORB()										;
	LC_COLORB(BYTE R, BYTE G, BYTE B, BYTE A)		;
	LC_COLORB(const LC_COLORF& v)					;
	LC_COLORB(COLOR32 v)							;	// argument must be a argb(0xAARRGGBB)
	LC_COLORB(const LC_COLORB&)						;
	LC_COLORB& operator=(const LC_COLORB&)			;

	operator COLOR32 () const						;
	operator BYTE* ()								;
	operator const BYTE* () const					;
	INT operator == (const LC_COLORB&) const		;
	INT operator != (const LC_COLORB&) const		;
};


struct LC_RECTF
{
	union
	{
		struct{  FLOAT x,   y,  w,    h;    }		;
		struct{  FLOAT l,   t,  r,    b;    }		;
		struct{  FLOAT x0,  y0, x1,   y1;   }		;
		struct{  FLOAT left,top,right,bottom;}		;
		FLOAT	m[4];
	};

	LC_RECTF()										;
	LC_RECTF(FLOAT X, FLOAT Y, FLOAT W,FLOAT H)		;
	LC_RECTF(const LC_RECTF&)						;
	LC_RECTF& operator=(const LC_RECTF&)			;

	operator FLOAT*()								;
	operator const FLOAT*() const					;
	INT operator == (const LC_RECTF&) const			;
	INT operator != (const LC_RECTF&) const			;
};


struct LC_MATRIX3X2
{
	union { struct {
			FLOAT _11, _12;
			FLOAT _21, _22;
			FLOAT _31, _32; };
			FLOAT m[3][2]; };

	LC_MATRIX3X2()									;
	LC_MATRIX3X2( FLOAT t11, FLOAT t12
				, FLOAT t21, FLOAT t22
				, FLOAT t31, FLOAT t32)				;

	LC_MATRIX3X2(const LC_MATRIX3X2&)				;
	LC_MATRIX3X2& operator=(const LC_MATRIX3X2&)	;

	operator FLOAT*()								;
	operator const FLOAT*() const					;
	INT operator == (const LC_MATRIX3X2&) const		;
	INT operator != (const LC_MATRIX3X2&) const		;
};


struct LC_MATRIX3X3
{
	union { struct {
			FLOAT _11, _12, _13;
			FLOAT _21, _22, _23;
			FLOAT _31, _32, _33; };
			FLOAT m[3][3]; };

	LC_MATRIX3X3()									;
	LC_MATRIX3X3( FLOAT t11, FLOAT t12, FLOAT t13
				, FLOAT t21, FLOAT t22, FLOAT t23
				, FLOAT t31, FLOAT t32, FLOAT t33)	;

	LC_MATRIX3X3(const LC_MATRIX3X3&)				;
	LC_MATRIX3X3& operator=(const LC_MATRIX3X3&)	;

	operator FLOAT*()								;
	operator const FLOAT*() const					;
	INT operator == (const LC_MATRIX3X3&) const		;
	INT operator != (const LC_MATRIX3X3&) const		;
};


struct LC_MATRIX2X2
{
	union { struct {
			FLOAT _11, _12;
			FLOAT _21, _22; };
			FLOAT m[2][2]; };

	LC_MATRIX2X2()									;
	LC_MATRIX2X2( FLOAT t11, FLOAT t12
				, FLOAT t21, FLOAT t22)				;

	LC_MATRIX2X2(const LC_MATRIX2X2&)				;
	LC_MATRIX2X2& operator=(const LC_MATRIX2X2&)	;

	operator FLOAT*()								;
	operator const FLOAT*() const					;
	INT operator == (const LC_MATRIX2X2&) const		;
	INT operator != (const LC_MATRIX2X2&) const		;
};


struct LC_MATRIX4X3
{
	union { struct {
			FLOAT _11, _12, _13;
			FLOAT _21, _22, _23;
			FLOAT _31, _32, _33;
			FLOAT _41, _42, _43; };
			FLOAT m[4][3]; };

	LC_MATRIX4X3()									;
	LC_MATRIX4X3(
			   FLOAT t11, FLOAT t12, FLOAT t13
			,  FLOAT t21, FLOAT t22, FLOAT t23
			,  FLOAT t31, FLOAT t32, FLOAT t33
			,  FLOAT t41, FLOAT t42, FLOAT t43)		;

	LC_MATRIX4X3(const LC_MATRIX4X3&)				;
	LC_MATRIX4X3& operator=(const LC_MATRIX4X3&)	;

	operator FLOAT*()								;
	operator const FLOAT*() const					;
	INT operator == (const LC_MATRIX4X3&) const		;
	INT operator != (const LC_MATRIX4X3&) const		;
};


struct LC_MATRIX
{
	union { struct {
			FLOAT _11, _12, _13, _14;
			FLOAT _21, _22, _23, _24;
			FLOAT _31, _32, _33, _34;
			FLOAT _41, _42, _43, _44; };
			FLOAT m[4][4]; };

	LC_MATRIX()										;
	LC_MATRIX(FLOAT t11, FLOAT t12, FLOAT t13, FLOAT t14
			, FLOAT t21, FLOAT t22, FLOAT t23, FLOAT t24
			, FLOAT t31, FLOAT t32, FLOAT t33, FLOAT t34
			, FLOAT t41, FLOAT t42, FLOAT t43, FLOAT t44);

	LC_MATRIX(const LC_MATRIX&)						;
	LC_MATRIX& operator=(const LC_MATRIX&)			;

	operator FLOAT*()								;
	operator const FLOAT*() const					;
	INT operator == (const LC_MATRIX&) const		;
	INT operator != (const LC_MATRIX&) const		;
};




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// glc argument interface

// glc argument structure
struct TLC_ARG
{
	union {  LC_HANDLE p; DOUBLE d; DINT n; UDINT u; };

	operator INT           (){ return (INT  )n;         };    operator UINT          (){ return (UINT )n;         };
	operator SHORT         (){ return (SHORT)n;         };    operator WORD          (){ return (WORD )n;         };
	operator QWORD         (){ return (QWORD)n;         };    operator DINT          (){ return (DINT )n;         };
	operator FLOAT         (){ return (FLOAT)d;         };    operator DOUBLE        (){ return        d;         };

	operator LC_HANDLE     (){ return			    p; };    operator C_BUF         (){ return (C_BUF        )p; };
	operator COSTR         (){ return (COSTR        )p; };    operator C_BUF*        (){ return (C_BUF*       )p; };
	operator COSTR*        (){ return (COSTR*       )p; };    operator BYTE*         (){ return (BYTE*        )p; };
	operator const BYTE*   (){ return (const BYTE*  )p; };    operator BYTE**        (){ return (BYTE**       )p; };
	operator const BYTE**  (){ return (const BYTE** )p; };

	operator INT*          (){ return (INT*         )p; };    operator const INT*    (){ return (const INT*   )p; };
	operator UINT*         (){ return (UINT*        )p; };    operator const UINT*   (){ return (const UINT*  )p; };
	operator SHORT*        (){ return (SHORT*       )p; };    operator const SHORT*  (){ return (const SHORT* )p; };

	operator WORD*         (){ return (WORD*        )p; };    operator const WORD*   (){ return (const WORD*  )p; };
	operator QWORD*        (){ return (QWORD*       )p; };    operator const QWORD*  (){ return (const QWORD* )p; };
	operator DINT*         (){ return (DINT*        )p; };    operator const DINT*   (){ return (const DINT*  )p; };

	operator FLOAT*        (){ return (FLOAT*       )p; };    operator const FLOAT*  (){ return (const FLOAT* )p; };
	operator DOUBLE*       (){ return (DOUBLE*      )p; };    operator const DOUBLE* (){ return (const DOUBLE*)p; };

	INT   operator =(const INT    &v){ n=v; return (INT   )n; }; UINT   operator =(const UINT   &v){ n=v; return (UINT  )n; };
	SHORT operator =(const SHORT  &v){ n=v; return (SHORT )n; }; USHORT operator =(const USHORT &v){ n=v; return (USHORT)n; };
	QWORD operator =(const QWORD  &v){ n=v; return (QWORD )n; }; DINT   operator =(const DINT   &v){ n=v; return (DINT  )n; };
	FLOAT operator =(const FLOAT  &v){ d=v; return (FLOAT )d; }; DOUBLE operator =(const DOUBLE &v){ d=v; return (DOUBLE)d; };

	LC_HANDLE operator =(const LC_HANDLE &v){ p=(LC_HANDLE)v; return (LC_HANDLE)p;};
	C_BUF     operator =(COSTR           &v){ p=(LC_HANDLE)v; return (C_BUF )p;   };
	//COSTR   operator =(COSTR           &v){ p=(LC_HANDLE)v; return (COSTR )p;   };
	BYTE*     operator =(const BYTE*     &v){ p=(LC_HANDLE)v; return (BYTE* )p;   };
	C_BUF*    operator =(COSTR*          &v){ p=(LC_HANDLE)v; return (C_BUF*)p;   };
	BYTE**    operator =(const BYTE**    &v){ p=(LC_HANDLE)v; return (BYTE**)p;   };
};


// glc argument list
struct TLC_ARGS
{
	union {
		struct{ TLC_ARG _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_a,_b,_c,_d,_e,_f,_g,_h,_i,_j; };
		struct{ TLC_ARG p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,pa,pb,pc,pd,pe,pf,pg,ph,pi,pj; };
		TLC_ARG m[20];
	};
	TLC_ARGS()																		        {_0= 0;_1= 0;_2= 0;_3= 0;_4= 0;_5= 0;_6= 0;_7= 0;_8= 0;_9= 0;_a=0;_b=0;_c=0;_d=0;_e=0;_f=0;_g=0;_h=0;_i=0;_j=0;}
	TLC_ARGS(DINT v0)																		 {_0=v0;_1= 0;_2= 0;_3= 0;_4= 0;_5= 0;_6= 0;_7= 0;_8= 0;_9= 0;_a=0;_b=0;_c=0;_d=0;_e=0;_f=0;_g=0;_h=0;_i=0;_j=0;}
	TLC_ARGS(DINT v0,DINT v1)															     {_0=v0;_1=v1;_2= 0;_3= 0;_4= 0;_5= 0;_6= 0;_7= 0;_8= 0;_9= 0;_a=0;_b=0;_c=0;_d=0;_e=0;_f=0;_g=0;_h=0;_i=0;_j=0;}
	TLC_ARGS(DINT v0,DINT v1,DINT v2)												         {_0=v0;_1=v1;_2=v2;_3= 0;_4= 0;_5= 0;_6= 0;_7= 0;_8= 0;_9= 0;_a=0;_b=0;_c=0;_d=0;_e=0;_f=0;_g=0;_h=0;_i=0;_j=0;}
	TLC_ARGS(DINT v0,DINT v1,DINT v2,DINT v3)												 {_0=v0;_1=v1;_2=v2;_3=v3;_4= 0;_5= 0;_6= 0;_7= 0;_8= 0;_9= 0;_a=0;_b=0;_c=0;_d=0;_e=0;_f=0;_g=0;_h=0;_i=0;_j=0;}
	TLC_ARGS(DINT v0,DINT v1,DINT v2,DINT v3,DINT v4)									     {_0=v0;_1=v1;_2=v2;_3=v3;_4=v4;_5= 0;_6= 0;_7= 0;_8= 0;_9= 0;_a=0;_b=0;_c=0;_d=0;_e=0;_f=0;_g=0;_h=0;_i=0;_j=0;}
	TLC_ARGS(DINT v0,DINT v1,DINT v2,DINT v3,DINT v4,DINT v5)						         {_0=v0;_1=v1;_2=v2;_3=v3;_4=v4;_5=v5;_6= 0;_7= 0;_8= 0;_9= 0;_a=0;_b=0;_c=0;_d=0;_e=0;_f=0;_g=0;_h=0;_i=0;_j=0;}
	TLC_ARGS(DINT v0,DINT v1,DINT v2,DINT v3,DINT v4,DINT v5,DINT v6)						 {_0=v0;_1=v1;_2=v2;_3=v3;_4=v4;_5=v5;_6=v6;_7= 0;_8= 0;_9= 0;_a=0;_b=0;_c=0;_d=0;_e=0;_f=0;_g=0;_h=0;_i=0;_j=0;}
	TLC_ARGS(DINT v0,DINT v1,DINT v2,DINT v3,DINT v4,DINT v5,DINT v6,DINT v7)			     {_0=v0;_1=v1;_2=v2;_3=v3;_4=v4;_5=v5;_6=v6;_7=v7;_8= 0;_9= 0;_a=0;_b=0;_c=0;_d=0;_e=0;_f=0;_g=0;_h=0;_i=0;_j=0;}
	TLC_ARGS(DINT v0,DINT v1,DINT v2,DINT v3,DINT v4,DINT v5,DINT v6,DINT v7,DINT v8)         {_0=v0;_1=v1;_2=v2;_3=v3;_4=v4;_5=v5;_6=v6;_7=v7;_8=v8;_9= 0;_a=0;_b=0;_c=0;_d=0;_e=0;_f=0;_g=0;_h=0;_i=0;_j=0;}
	TLC_ARGS(DINT v0,DINT v1,DINT v2,DINT v3,DINT v4,DINT v5,DINT v6,DINT v7,DINT v8,DINT v9) {_0=v0;_1=v1;_2=v2;_3=v3;_4=v4;_5=v5;_6=v6;_7=v7;_8=v8;_9=v9;_a=0;_b=0;_c=0;_d=0;_e=0;_f=0;_g=0;_h=0;_i=0;_j=0;}
};
typedef TLC_ARGS* PLC_ARGS;


#define  _MAKE_ARG1(v,v0)\
	TLC_ARGS v{}; (v)._0=(v0);

#define  _MAKE_ARG2(v,v0,v1)\
	TLC_ARGS v{}; (v)._0=(v0);(v)._1=(v1);

#define  _MAKE_ARG3(v,v0,v1,v2)\
	TLC_ARGS v{}; (v)._0=(v0);(v)._1=(v1);(v)._2=(v2);

#define  _MAKE_ARG4(v,v0,v1,v2,v3)\
	TLC_ARGS v{}; (v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3);

#define  _MAKE_ARG5(v,v0,v1,v2,v3,v4)\
	TLC_ARGS v{}; (v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3);(v)._4=(v4);

#define  _MAKE_ARG6(v,v0,v1,v2,v3,v4,v5)\
	TLC_ARGS v{}; (v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3);(v)._4=(v4);(v)._5=(v5);

#define  _MAKE_ARG7(v,v0,v1,v2,v3,v4,v5,v6)\
	TLC_ARGS v{}; (v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3);(v)._4=(v4);(v)._5=(v5);(v)._6=(v6);

#define  _MAKE_ARG8(v,v0,v1,v2,v3,v4,v5,v6,v7)\
	TLC_ARGS v{}; (v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3);(v)._4=(v4);(v)._5=(v5);(v)._6=(v6);(v)._7=(v7);

#define  _MAKE_ARG9(v,v0,v1,v2,v3,v4,v5,v6,v7,v8)\
	TLC_ARGS v{}; (v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3);(v)._4=(v4);(v)._5=(v5);(v)._6=(v6);(v)._7=(v7);\
	(v)._8=(v8);

#define _MAKE_ARG10(v,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9)\
	TLC_ARGS v{}; (v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3);(v)._4=(v4);(v)._5=(v5);(v)._6=(v6);(v)._7=(v7);\
	(v)._8=(v8);(v)._9=(v9);

#define _MAKE_ARG11(v,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,va)\
	TLC_ARGS v{}; (v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3);(v)._4=(v4);(v)._5=(v5);(v)._6=(v6);(v)._7=(v7);\
	(v)._8=(v8);(v)._9=(v9);(v)._a=(va);

#define _MAKE_ARG12(v,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,va,vb)\
	TLC_ARGS v{}; (v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3);(v)._4=(v4);(v)._5=(v5);(v)._6=(v6);(v)._7=(v7);\
	(v)._8=(v8);(v)._9=(v9);(v)._a=(va);(v)._b=(vb);

#define _MAKE_ARG13(v,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,va,vb,vc)\
	TLC_ARGS v{}; (v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3);(v)._4=(v4);(v)._5=(v5);(v)._6=(v6);(v)._7=(v7);\
	(v)._8=(v8);(v)._9=(v9);(v)._a=(va);(v)._b=(vb);(v)._c=(vc);

#define _MAKE_ARG14(v,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,va,vb,vc,vd)\
	TLC_ARGS v{}; (v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3);(v)._4=(v4);(v)._5=(v5);(v)._6=(v6);(v)._7=(v7);\
	(v)._8=(v8);(v)._9=(v9);(v)._a=(va);(v)._b=(vb);(v)._c=(vc);(v)._d=(vd);

#define _MAKE_ARG15(v,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,va,vb,vc,vd,ve)\
	TLC_ARGS v{}; (v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3);(v)._4=(v4);(v)._5=(v5);(v)._6=(v6);(v)._7=(v7);\
	(v)._8=(v8);(v)._9=(v9);(v)._a=(va);(v)._b=(vb);(v)._c=(vc);(v)._d=(vd);(v)._e=(ve);

#define _MAKE_ARG16(v,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,va,vb,vc,vd,ve,vf)\
	TLC_ARGS v{}; (v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3);(v)._4=(v4);(v)._5=(v5);(v)._6=(v6);(v)._7=(v7);\
	(v)._8=(v8);(v)._9=(v9);(v)._a=(va);(v)._b=(vb);(v)._c=(vc);(v)._d=(vd);(v)._e=(ve);(v)._f=(vf);

#define _MAKE_ARG17(v,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,va,vb,vc,vd,ve,vf,vg)\
	TLC_ARGS v{}; (v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3);(v)._4=(v4);(v)._5=(v5);(v)._6=(v6);(v)._7=(v7);\
	(v)._8=(v8);(v)._9=(v9);(v)._a=(va);(v)._b=(vb);(v)._c=(vc);(v)._d=(vd);(v)._e=(ve);(v)._f=(vf);\
	(v)._g=(vg);

#define _MAKE_ARG18(v,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,va,vb,vc,vd,ve,vf,vg,vh)\
	TLC_ARGS v{}; (v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3);(v)._4=(v4);(v)._5=(v5);(v)._6=(v6);(v)._7=(v7);\
	(v)._8=(v8);(v)._9=(v9);(v)._a=(va);(v)._b=(vb);(v)._c=(vc);(v)._d=(vd);(v)._e=(ve);(v)._f=(vf);\
	(v)._g=(vg);(v)._h=(vh);

#define _MAKE_ARG19(v,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,va,vb,vc,vd,ve,vf,vg,vh,vi)\
	TLC_ARGS v{}; (v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3);(v)._4=(v4);(v)._5=(v5);(v)._6=(v6);(v)._7=(v7);\
	(v)._8=(v8);(v)._9=(v9);(v)._a=(va);(v)._b=(vb);(v)._c=(vc);(v)._d=(vd);(v)._e=(ve);(v)._f=(vf);\
	(v)._g=(vg);(v)._h=(vh);(v)._i=(vi);

#define _MAKE_ARG20(v,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,va,vb,vc,vd,ve,vf,vg,vh,vi,vj)\
	TLC_ARGS v{}; (v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3);(v)._4=(v4);(v)._5=(v5);(v)._6=(v6);(v)._7=(v7);\
	(v)._8=(v8);(v)._9=(v9);(v)._a=(va);(v)._b=(vb);(v)._c=(vc);(v)._d=(vd);(v)._e=(ve);(v)._f=(vf);\
	(v)._g=(vg);(v)._h=(vh);(v)._i=(vi);(v)._j=(vj);


#define  MAKE_ARG1(v,v0)\
	(v)._0=(v0); (v)._1=L_NIL;(v)._2=L_NIL;(v)._3=L_NIL;(v)._4=L_NIL;(v)._5=L_NIL;(v)._6=L_NIL;(v)._7=L_NIL;\
	(v)._8=L_NIL;(v)._9=L_NIL;(v)._a=L_NIL;(v)._b=L_NIL;(v)._c=L_NIL;(v)._d=L_NIL;(v)._e=L_NIL;(v)._f=L_NIL;\
	(v)._g=L_NIL;(v)._h=L_NIL;(v)._i=L_NIL;(v)._j=L_NIL;

#define  MAKE_ARG2(v,v0,v1)\
	(v)._0=(v0); (v)._1=(v1); (v)._2=L_NIL;(v)._3=L_NIL;(v)._4=L_NIL;(v)._5=L_NIL;(v)._6=L_NIL;(v)._7=L_NIL;\
	(v)._8=L_NIL;(v)._9=L_NIL;(v)._a=L_NIL;(v)._b=L_NIL;(v)._c=L_NIL;(v)._d=L_NIL;(v)._e=L_NIL;(v)._f=L_NIL;\
	(v)._g=L_NIL;(v)._h=L_NIL;(v)._i=L_NIL;(v)._j=L_NIL;

#define  MAKE_ARG3(v,v0,v1,v2)\
	(v)._0=(v0); (v)._1=(v1); (v)._2=(v2); (v)._3=L_NIL;(v)._4=L_NIL;(v)._5=L_NIL;(v)._6=L_NIL;(v)._7=L_NIL;\
	(v)._8=L_NIL;(v)._9=L_NIL;(v)._a=L_NIL;(v)._b=L_NIL;(v)._c=L_NIL;(v)._d=L_NIL;(v)._e=L_NIL;(v)._f=L_NIL;\
	(v)._g=L_NIL;(v)._h=L_NIL;(v)._i=L_NIL;(v)._j=L_NIL;

#define  MAKE_ARG4(v,v0,v1,v2,v3)\
	(v)._0=(v0); (v)._1=(v1); (v)._2=(v2); (v)._3=(v3); (v)._4=L_NIL;(v)._5=L_NIL;(v)._6=L_NIL;(v)._7=L_NIL;\
	(v)._8=L_NIL;(v)._9=L_NIL;(v)._a=L_NIL;(v)._b=L_NIL;(v)._c=L_NIL;(v)._d=L_NIL;(v)._e=L_NIL;(v)._f=L_NIL;\
	(v)._g=L_NIL;(v)._h=L_NIL;(v)._i=L_NIL;(v)._j=L_NIL;

#define  MAKE_ARG5(v,v0,v1,v2,v3,v4)\
	(v)._0=(v0); (v)._1=(v1); (v)._2=(v2); (v)._3=(v3); (v)._4=(v4); (v)._5=L_NIL;(v)._6=L_NIL;(v)._7=L_NIL;\
	(v)._8=L_NIL;(v)._9=L_NIL;(v)._a=L_NIL;(v)._b=L_NIL;(v)._c=L_NIL;(v)._d=L_NIL;(v)._e=L_NIL;(v)._f=L_NIL;\
	(v)._g=L_NIL;(v)._h=L_NIL;(v)._i=L_NIL;(v)._j=L_NIL;

#define  MAKE_ARG6(v,v0,v1,v2,v3,v4,v5)\
	(v)._0=(v0); (v)._1=(v1); (v)._2=(v2); (v)._3=(v3); (v)._4=(v4); (v)._5=(v5); (v)._6=L_NIL;(v)._7=L_NIL;\
	(v)._8=L_NIL;(v)._9=L_NIL;(v)._a=L_NIL;(v)._b=L_NIL;(v)._c=L_NIL;(v)._d=L_NIL;(v)._e=L_NIL;(v)._f=L_NIL;\
	(v)._g=L_NIL;(v)._h=L_NIL;(v)._i=L_NIL;(v)._j=L_NIL;

#define  MAKE_ARG7(v,v0,v1,v2,v3,v4,v5,v6)\
	(v)._0=(v0); (v)._1=(v1); (v)._2=(v2); (v)._3=(v3); (v)._4=(v4); (v)._5=(v5); (v)._6=(v6);(v)._7=L_NIL;\
	(v)._8=L_NIL;(v)._9=L_NIL;(v)._a=L_NIL;(v)._b=L_NIL;(v)._c=L_NIL;(v)._d=L_NIL;(v)._e=L_NIL;(v)._f=L_NIL;\
	(v)._g=L_NIL;(v)._h=L_NIL;(v)._i=L_NIL;(v)._j=L_NIL;

#define  MAKE_ARG8(v,v0,v1,v2,v3,v4,v5,v6,v7)\
	(v)._0=(v0); (v)._1=(v1); (v)._2=(v2); (v)._3=(v3); (v)._4=(v4); (v)._5=(v5); (v)._6=(v6); (v)._7=(v7);\
	(v)._8=L_NIL;(v)._9=L_NIL;(v)._a=L_NIL;(v)._b=L_NIL;(v)._c=L_NIL;(v)._d=L_NIL;(v)._e=L_NIL;(v)._f=L_NIL;\
	(v)._g=L_NIL;(v)._h=L_NIL;(v)._i=L_NIL;(v)._j=L_NIL;

#define  MAKE_ARG9(v,v0,v1,v2,v3,v4,v5,v6,v7,v8)\
	(v)._0=(v0); (v)._1=(v1); (v)._2=(v2); (v)._3=(v3); (v)._4=(v4); (v)._5=(v5); (v)._6=(v6); (v)._7=(v7);\
	(v)._8=(v8);(v)._9=L_NIL;(v)._a=L_NIL;(v)._b=L_NIL;(v)._c=L_NIL;(v)._d=L_NIL;(v)._e=L_NIL;(v)._f=L_NIL;\
	(v)._g=L_NIL;(v)._h=L_NIL;(v)._i=L_NIL;(v)._j=L_NIL;

#define MAKE_ARG10(v,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9)\
	(v)._0=(v0);(v)._1=(v1);(v)._2=(v2); (v)._3=(v3); (v)._4=(v4); (v)._5=(v5); (v)._6=(v6); (v)._7=(v7);\
	(v)._8=(v8);(v)._9=(v9);(v)._a=L_NIL;(v)._b=L_NIL;(v)._c=L_NIL;(v)._d=L_NIL;(v)._e=L_NIL;(v)._f=L_NIL;\
	(v)._g=L_NIL;(v)._h=L_NIL;(v)._i=L_NIL;(v)._j=L_NIL;

#define MAKE_ARG11(v,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,va)\
	(v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3); (v)._4=(v4); (v)._5=(v5); (v)._6=(v6); (v)._7=(v7);\
	(v)._8=(v8);(v)._9=(v9);(v)._a=(va);(v)._b=L_NIL;(v)._c=L_NIL;(v)._d=L_NIL;(v)._e=L_NIL;(v)._f=L_NIL;\
	(v)._g=L_NIL;(v)._h=L_NIL;(v)._i=L_NIL;(v)._j=L_NIL;

#define MAKE_ARG12(v,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,va,vb)\
	(v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3);(v)._4=(v4); (v)._5=(v5); (v)._6=(v6); (v)._7=(v7);\
	(v)._8=(v8);(v)._9=(v9);(v)._a=(va);(v)._b=(vb);(v)._c=L_NIL;(v)._d=L_NIL;(v)._e=L_NIL;(v)._f=L_NIL;\
	(v)._g=L_NIL;(v)._h=L_NIL;(v)._i=L_NIL;(v)._j=L_NIL;

#define MAKE_ARG13(v,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,va,vb,vc)\
	(v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3);(v)._4=(v4);(v)._5=(v5); (v)._6=(v6); (v)._7=(v7);\
	(v)._8=(v8);(v)._9=(v9);(v)._a=(va);(v)._b=(vb);(v)._c=(vc);(v)._d=L_NIL;(v)._e=L_NIL;(v)._f=L_NIL;\
	(v)._g=L_NIL;(v)._h=L_NIL;(v)._i=L_NIL;(v)._j=L_NIL;

#define MAKE_ARG14(v,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,va,vb,vc,vd)\
	(v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3);(v)._4=(v4);(v)._5=(v5);(v)._6=(v6); (v)._7=(v7);\
	(v)._8=(v8);(v)._9=(v9);(v)._a=(va);(v)._b=(vb);(v)._c=(vc);(v)._d=(vd);(v)._e=L_NIL;(v)._f=L_NIL;\
	(v)._g=L_NIL;(v)._h=L_NIL;(v)._i=L_NIL;(v)._j=L_NIL;

#define MAKE_ARG15(v,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,va,vb,vc,vd,ve)\
	(v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3);(v)._4=(v4);(v)._5=(v5);(v)._6=(v6);(v)._7=(v7);\
	(v)._8=(v8);(v)._9=(v9);(v)._a=(va);(v)._b=(vb);(v)._c=(vc);(v)._d=(vd);(v)._e=(ve);(v)._f=L_NIL;\
	(v)._g=L_NIL;(v)._h=L_NIL;(v)._i=L_NIL;(v)._j=L_NIL;

#define MAKE_ARG16(v,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,va,vb,vc,vd,ve,vf)\
	(v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3);(v)._4=(v4);(v)._5=(v5);(v)._6=(v6);(v)._7=(v7);\
	(v)._8=(v8);(v)._9=(v9);(v)._a=(va);(v)._b=(vb);(v)._c=(vc);(v)._d=(vd);(v)._e=(ve);(v)._f=(vf);\
	(v)._g=L_NIL;(v)._h=L_NIL;(v)._i=L_NIL;(v)._j=L_NIL;

#define MAKE_ARG17(v,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,va,vb,vc,vd,ve,vf,vg)\
	(v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3);(v)._4=(v4);(v)._5=(v5);(v)._6=(v6);(v)._7=(v7);\
	(v)._8=(v8);(v)._9=(v9);(v)._a=(va);(v)._b=(vb);(v)._c=(vc);(v)._d=(vd);(v)._e=(ve);(v)._f=(vf);\
	(v)._g=(vg);(v)._h=L_NIL;(v)._i=L_NIL;(v)._j=L_NIL;

#define MAKE_ARG18(v,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,va,vb,vc,vd,ve,vf,vg,vh)\
	(v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3);(v)._4=(v4);(v)._5=(v5);(v)._6=(v6);(v)._7=(v7);\
	(v)._8=(v8);(v)._9=(v9);(v)._a=(va);(v)._b=(vb);(v)._c=(vc);(v)._d=(vd);(v)._e=(ve);(v)._f=(vf);\
	(v)._g=(vg);(v)._h=(vh);(v)._i=L_NIL;(v)._j=L_NIL;

#define MAKE_ARG19(v,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,va,vb,vc,vd,ve,vf,vg,vh,vi)\
	(v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3);(v)._4=(v4);(v)._5=(v5);(v)._6=(v6);(v)._7=(v7);\
	(v)._8=(v8);(v)._9=(v9);(v)._a=(va);(v)._b=(vb);(v)._c=(vc);(v)._d=(vd);(v)._e=(ve);(v)._f=(vf);\
	(v)._g=(vg);(v)._h=(vh);(v)._i=(vi);(v)._j=L_NIL;

#define MAKE_ARG20(v,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,va,vb,vc,vd,ve,vf,vg,vh,vi,vj)\
	(v)._0=(v0);(v)._1=(v1);(v)._2=(v2);(v)._3=(v3);(v)._4=(v4);(v)._5=(v5);(v)._6=(v6);(v)._7=(v7);\
	(v)._8=(v8);(v)._9=(v9);(v)._a=(va);(v)._b=(vb);(v)._c=(vc);(v)._d=(vd);(v)._e=(ve);(v)._f=(vf);\
	(v)._g=(vg);(v)._h=(vh);(v)._i=(vi);(v)._j=(vj);


void      LcArgs_Reset    (PLC_ARGS _dst);												// reset the args to zero
void      LcArgs_FromInt  (PLC_ARGS _dst, const INT*     _src, INT cnt);				// copy from int array[max 8]
void      LcArgs_ToInt    (INT*     _src, const PLC_ARGS _dst, INT cnt);				// copy to int array[max 8]
void      LcArgs_FromFloat(PLC_ARGS _dst, const FLOAT*   _src, INT cnt);				// copy from float array[max 8]
void      LcArgs_ToFloat  (FLOAT*   _src, const PLC_ARGS _dst, INT cnt);				// copy to float array[max 8]
LC_HANDLE LcArgs_ToHandle (LC_HANDLE p);												// static casting from args element
LC_HANDLE LcArgs_ToHandle (const PLC_ARGS _src, INT index);								// static casting from args index


//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// to save the GLOBAL VARIABLEs chunk for system and application

struct TLC_VAR
{
	union {
		struct{ INT       _i0, _i1, _i2, _i3, _i4, _i5, _i6, _i7, _i8, _i9, _ia, _ib, _ic, _id, _ie, _if; };
		struct{ FLOAT     _f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7, _f8, _f9, _fa, _fb, _fc, _fd, _fe, _ff; };
		struct{ LC_HANDLE _p0, _p1, _p2, _p3, _p4, _p5, _p6, _p7; };
		struct{ DOUBLE    _d0, _d1, _d2, _d3, _d4, _d5, _d6, _d7; };
		struct{ DINT      _n0, _n1, _n2, _n3, _n4, _n5, _n6, _n7; };
		struct{ UDINT     _u0, _u1, _u2, _u3, _u4, _u5, _u6, _u7; };
		struct{ SHORT     _s0, _s1, _s2, _s3, _s4, _s5, _s6, _s7, _s8, _s9, _sa, _sb, _sc, _sd, _se, _sf; };
		struct{ INT       _cr, _cg, _cb, _ca, _cd, _cs, _cw, _ch, _cm, _scx,_scy,_scw,_sch,_sct; };
		struct{ FLOAT     _x,   _y,  _z,  _w; };
		struct{ FLOAT     _fx, _fy, _fw, _fh, _frx, _fry; };
		struct{ FLOAT     _11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44; };
		INT8   v_b[64];    INT16  v_s[32];    INT32  v_i[16];    INT64  v_n[ 8];
		FLOAT  v_f[16];    DOUBLE v_d[ 8];    LPVOID v_p[ 8];
	};

	TLC_VAR();
	TLC_VAR(  INT _0  , INT _1=0, INT _2=0, INT _3=0
			, INT _4=0, INT _5=0, INT _6=0, INT _7=0
			, INT _8=0, INT _9=0, INT _a=0, INT _b=0
			, INT _c=0, INT _d=0, INT _e=0, INT _f=0);

	TLC_VAR(  DINT _0  , DINT _1=0, DINT _2=0, DINT _3=0
			, DINT _4=0, DINT _5=0, DINT _6=0, DINT _7=0);

	TLC_VAR(  FLOAT  _0     , FLOAT _1=0.0F, FLOAT _2=0.0F, FLOAT _3=0.0F
			, FLOAT  _4=0.0F, FLOAT _5=0.0F, FLOAT _6=0.0F, FLOAT _7=0.0F
			, FLOAT  _8=0.0F, FLOAT _9=0.0F, FLOAT _a=0.0F, FLOAT _b=0.0F
			, FLOAT  _c=0.0F, FLOAT _d=0.0F, FLOAT _e=0.0F, FLOAT _f=0.0F);

	TLC_VAR(  DOUBLE  _0    , DOUBLE _1=0.0, DOUBLE _2=0.0, DOUBLE _3=0.0
			, DOUBLE  _4=0.0, DOUBLE _5=0.0, DOUBLE _6=0.0, DOUBLE _7=0.0);

	TLC_VAR(  LC_HANDLE _0     , LC_HANDLE _1=NULL, LC_HANDLE _2=NULL, LC_HANDLE _3=NULL
			, LC_HANDLE _4=NULL, LC_HANDLE _5=NULL, LC_HANDLE _6=NULL, LC_HANDLE _7=NULL);

	TLC_VAR& operator =  (const TLC_VAR&);
	TLC_VAR& operator =  (const TLC_VAR*);
	INT      operator == (const TLC_VAR&) const	;
	INT      operator != (const TLC_VAR&) const	;

	void     Clear();

	void     Int8  (INT index, INT8      val);
	void     Int16 (INT index, INT16     val);
	void     Int32 (INT index, INT32     val);
	void     Int64 (INT index, INT64     val);
	void     Float (INT index, FLOAT     val);
	void     Double(INT index, DOUBLE    val);
	void     Handle(INT index, LC_HANDLE val);

	void     ArrayInt8  (INT8     * val, INT count);							// maximum 64
	void     ArrayInt16 (INT16    * val, INT count);							// maximum 32
	void     ArrayInt32 (INT32    * val, INT count);							// maximum 16
	void     ArrayInt64 (INT64    * val, INT count);							// maximum  8
	void     ArrayFloat (FLOAT    * val, INT count);							// maximum 16
	void     ArrayDouble(DOUBLE   * val, INT count);							// maximum  8
	void     ArrayHandle(LC_HANDLE* val, INT count);							// maximum 16
	void     String     (C_BUF      val, INT count);							// maximum 60

	INT8      Int8  (INT index);
	INT16     Int16 (INT index);
	INT32     Int32 (INT index);
	INT64     Int64 (INT index);
	FLOAT     Float (INT index);
	DOUBLE    Double(INT index);
	LC_HANDLE Handle(INT index);

	INT8*      ArrayInt8  ();
	INT16*     ArrayInt16 ();
	INT32*     ArrayInt32 ();
	DINT*      ArrayInt64 ();
	FLOAT*     ArrayFloat ();
	DOUBLE*    ArrayDouble();
	LC_HANDLE* ArrayHandle();
	C_BUF      String     ();
};
typedef TLC_VAR* PLC_VAR;


//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// for application attribute

#if defined(_PC_WINDOW_)
  #pragma pack(push, 1)
#endif

struct TLC_ATTRB
{
	union
	{
		struct { FLOAT     f00,f01,f02,f03,f04,f05,f06,f07,f08,f09,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19; };
		struct { FLOAT     sx , sy, sz, rx, ry, rz, rw, re, px, py, pz, pe, spd;						    };	// e: epsilon
		struct { INT       n00,n01,n02,n03,n04,n05,n06,n07,n08,n09,n10,n11,n12,n13,n14,n15,st0,st1,st2,st3; };	//
		struct { LC_HANDLE h00,h01,h02,h03,h04,h05,h06,h07,h08,h09,h10,h11,h12,h13,h14,h15,h16,h17,h18,h19; };
		char			   b[80];
		WORD			   w[40];
		LC_HANDLE          h[20];
		INT			    i[20];
	};
}
#if defined(_PC_WINDOW_)
  #pragma pack(pop)
#else
  __attribute__((packed))
#endif
;


//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// for image

enum
{
	_GL_F_ALPHA         = 0x1906,				// GL_ALPHA
	_GL_F_RGB           = 0x1907,				// GL_RGB
	_GL_F_RGBA          = 0x1908,				// GL_RGBA
	_GL_F_LUMINANCE     = 0x1909,				// GL_LUMINANCE
	_GL_F_L8			= 0x190A,				// GL_LUMINANCE_ALPHA
	_GL_T_UBYTE         = 0x1401,				// GL_UNSIGNED_BYTE
	_GL_T_W565          = 0x8363,				// GL_UNSIGNED_SHORT_5_6_5
	_GL_T_W4444         = 0x8033,				// GL_UNSIGNED_SHORT_4_4_4_4
	_GL_T_W5551         = 0x8034,				// GL_UNSIGNED_SHORT_5_5_5_1
	_GL_GEN_MIPMAP      = 0x8191,				// GL_GENERATE_MIPMAP
};

struct TLC_IMG
{
	UINT	_k, _t, _f;							// Image file class [LC_OBJ_BMP, LC_OBJ_RNT], Pixel Type: GL_UNSIGNED_SHORT, Pixel Format: GL_RGB, RGBA, ....
	INT		_w, _h, _d;							// Image Width, Height, Depth:  1byte), 2(byte), 3(byte), 4(byte)
	COLOR32	_c;									// Image Color Key
	INT		_m;									// Image mipmap level
};


#if defined(_PC_WINDOW_)
  #pragma pack(push, 1)
#endif

typedef struct tag_Tinf_src
{
	SIZE32	fs;									// file size
	TIME32	ts[2];								// creation time
}
#if defined(_PC_WINDOW_)
  #pragma pack(pop)
#else
  __attribute__((packed))
#endif
Tinf_src;


#if defined(_PC_WINDOW_)
  #pragma pack(push, 1)
#endif

typedef struct tag_Tinf_pf
{
	C_BUF	src_n;								// File Name
	INT		atts [8];							// Attribute
}
#if defined(_PC_WINDOW_)
  #pragma pack(pop)
#else
  __attribute__((packed))
#endif
Tinf_pf;


#if defined(_PC_WINDOW_)
  #pragma pack(push, 1)
#endif

typedef struct tag_Tinf_lf
{
	UINT	fv[2];								// File Version
	TIME32	ts[2];								// modification:0 and creation time:1 of original file. it needs dword * 2
}
#if defined(_PC_WINDOW_)
  #pragma pack(pop)
#else
  __attribute__((packed))
#endif
Tinf_lf;


#if defined(_PC_WINDOW_)
  #pragma pack(push, 1)
#endif

typedef struct tag_Tinf_gpt
{
	Tinf_lf	 inf_l;
	TLC_IMG  inf_i;
}
#if defined(_PC_WINDOW_)
  #pragma pack(pop)
#else
  __attribute__((packed))
#endif
Tinf_gpt;

#endif	//_LcBase_H_

