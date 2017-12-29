// Implementation of the Gpt.
//
////////////////////////////////////////////////////////////////////////////////

#if defined(_PC_WINDOW_)
  #pragma warning(disable:4996)
#endif

#include <mutex>
#include <thread>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <typeinfo>

#include "LcType.h"
#include "LcBase.h"


//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// 2D, 3D basic structure

LC_POINT2::LC_POINT2() : x(0), y(0){}
LC_POINT2::LC_POINT2(INT X,INT Y) : x(X), y(Y){}

LC_POINT2::LC_POINT2(const LC_POINT2& v)   {   *this = v;      }
LC_POINT2& LC_POINT2::operator=(const LC_POINT2& v)
{
	x=v.x; y=v.y;
	return *this;
}

LC_POINT2::operator INT*()                          { return &x;             }
LC_POINT2::operator const INT*() const              { return (const INT*)&x; }
LC_POINT2& LC_POINT2::operator+=(const LC_POINT2& v){ x+=v.x;y+=v.y;return *this; }
LC_POINT2& LC_POINT2::operator-=(const LC_POINT2& v){ x-=v.x;y-=v.y;return *this; }
INT LC_POINT2::operator ==(const LC_POINT2& v) const{	return x == v.x && y == v.y;	}
INT LC_POINT2::operator !=(const LC_POINT2& v) const{	return x != v.x || y != v.y;	}


////////////////////////////////////////

LC_POINT3::LC_POINT3() : x(0), y(0), z(0){}
LC_POINT3::LC_POINT3(INT X,INT Y,INT Z) : x(X), y(Y), z(Z){}

LC_POINT3::LC_POINT3(const LC_POINT3& v)   {   *this = v;      }
LC_POINT3& LC_POINT3::operator=(const LC_POINT3& v)
{
	x=v.x; y=v.y; z=v.z;
	return *this;
}

LC_POINT3::operator INT*()                          { return &x;             }
LC_POINT3::operator const INT*() const              { return (const INT*)&x; }
LC_POINT3& LC_POINT3::operator+=(const LC_POINT3& v){ x+=v.x;y+=v.y;z+=v.z;return *this; }
LC_POINT3& LC_POINT3::operator-=(const LC_POINT3& v){ x-=v.x;y-=v.y;z-=v.z;return *this; }
INT LC_POINT3::operator ==(const LC_POINT3& v) const{ return x == v.x && y == v.y && z == v.z;	}
INT LC_POINT3::operator !=(const LC_POINT3& v) const{ return x != v.x || y != v.y || z != v.z;	}


////////////////////////////////////////

LC_POINT4::LC_POINT4() : x(0), y(0), z(0), w(0){}
LC_POINT4::LC_POINT4(INT X,INT Y,INT Z,INT W) : x(X), y(Y), z(Z), w(W){}

LC_POINT4::LC_POINT4(const LC_POINT4& v)   {   *this = v;      }
LC_POINT4& LC_POINT4::operator=(const LC_POINT4& v)
{
	x=v.x; y=v.y; z=v.z; w=v.w;
	return *this;
}

LC_POINT4::operator INT*()                          { return &x;             }
LC_POINT4::operator const INT*() const              { return (const INT*)&x; }
LC_POINT4& LC_POINT4::operator+=(const LC_POINT4& v){ x+=v.x;y+=v.y;z+=v.z;w+=v.w;return *this; }
LC_POINT4& LC_POINT4::operator-=(const LC_POINT4& v){ x-=v.x;y-=v.y;z-=v.z;w-=v.w;return *this; }
INT LC_POINT4::operator ==(const LC_POINT4& v) const{ return x == v.x && y == v.y && z == v.z && w == v.w;	}
INT LC_POINT4::operator !=(const LC_POINT4& v) const{ return x != v.x || y != v.y || z != v.z || w != v.w;	}


////////////////////////////////////////

LC_RECT::LC_RECT() : x(0), y(0), w(0), h(0){}
LC_RECT::LC_RECT(INT X,INT Y,INT W,INT H) : x(X), y(Y), w(W), h(H){}

LC_RECT::LC_RECT(const LC_RECT& v)   {   *this = v;      }
LC_RECT& LC_RECT::operator=(const LC_RECT& v)
{
	x=v.x; y=v.y; w=v.w; h=v.h;
	return *this;
}

LC_RECT::operator INT*()                            { return &x;             }
LC_RECT::operator const INT*() const                { return (const INT*)&x; }
INT LC_RECT::operator ==(const LC_RECT& v) const	{ return x == v.x && y == v.y && w == v.w && h == v.h;	}
INT LC_RECT::operator !=(const LC_RECT& v) const	{ return x != v.x || y != v.y || w != v.w || h != v.h;	}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


LC_BYTE3::LC_BYTE3() : x(0), y(0), z(0){}
LC_BYTE3::LC_BYTE3(BYTE X,BYTE Y,BYTE Z) : x(X), y(Y), z(Z){}

LC_BYTE3::LC_BYTE3(const LC_BYTE3& v)   {   *this = v;      }
LC_BYTE3& LC_BYTE3::operator=(const LC_BYTE3& v)
{
	x=v.x; y=v.y; z=v.z;
	return *this;
}

LC_BYTE3::operator BYTE*()                        { return &x;               }
LC_BYTE3::operator const BYTE*() const            { return (const BYTE*)&x; }
INT LC_BYTE3::operator ==(const LC_BYTE3& v) const{ return x == v.x && y == v.y && z == v.z;	}
INT LC_BYTE3::operator !=(const LC_BYTE3& v) const{ return x != v.x || y != v.y || z != v.z;	}


////////////////////////////////////////

LC_SHORT2::LC_SHORT2() : x(0), y(0){}
LC_SHORT2::LC_SHORT2(SHORT X,SHORT Y) : x(X), y(Y){}

LC_SHORT2::LC_SHORT2(const LC_SHORT2& v)   {   *this = v;      }
LC_SHORT2& LC_SHORT2::operator=(const LC_SHORT2& v)
{
	x=v.x; y=v.y;
	return *this;
}

LC_SHORT2::operator SHORT*()                        { return &x;              }
LC_SHORT2::operator const SHORT*() const            { return (const SHORT*)&x;}
INT LC_SHORT2::operator ==(const LC_SHORT2& v) const{	return x == v.x && y == v.y;	}
INT LC_SHORT2::operator !=(const LC_SHORT2& v) const{	return x != v.x || y != v.y;	}


////////////////////////////////////////

LC_WORD2::LC_WORD2() : x(0), y(0){}
LC_WORD2::LC_WORD2(WORD X,WORD Y) : x(X), y(Y){}

LC_WORD2::LC_WORD2(const LC_WORD2& v)   {   *this = v;      }
LC_WORD2& LC_WORD2::operator=(const LC_WORD2& v)
{
	x=v.x; y=v.y;
	return *this;
}

LC_WORD2::operator WORD*()                          { return &x;              }
LC_WORD2::operator const WORD*() const              { return (const WORD*)&x; }
INT LC_WORD2::operator ==(const LC_WORD2& v) const	{ return x == v.x && y == v.y;	}
INT LC_WORD2::operator !=(const LC_WORD2& v) const	{ return x != v.x || y != v.y;	}


////////////////////////////////////////

LC_SHORT3::LC_SHORT3() : x(0), y(0), z(0){}
LC_SHORT3::LC_SHORT3(SHORT X,SHORT Y,SHORT Z) : x(X), y(Y), z(Z){}

LC_SHORT3::LC_SHORT3(const LC_SHORT3& v)   {   *this = v;      }
LC_SHORT3& LC_SHORT3::operator=(const LC_SHORT3& v)
{
	x=v.x; y=v.y; z=v.z;
	return *this;
}

LC_SHORT3::operator SHORT*()                        { return &x;               }
LC_SHORT3::operator const SHORT*() const            { return (const SHORT*)&x; }
INT LC_SHORT3::operator ==(const LC_SHORT3& v) const{ return x == v.x && y == v.y && z == v.z;	}
INT LC_SHORT3::operator !=(const LC_SHORT3& v) const{ return x != v.x || y != v.y || z != v.z;	}


////////////////////////////////////////

LC_WORD3::LC_WORD3() : x(0), y(0), z(0){}
LC_WORD3::LC_WORD3(WORD X,WORD Y,WORD Z) : x(X), y(Y), z(Z){}

LC_WORD3::LC_WORD3(const LC_WORD3& v)   {   *this = v;      }
LC_WORD3& LC_WORD3::operator=(const LC_WORD3& v)
{
	x=v.x; y=v.y; z=v.z;
	return *this;
}

LC_WORD3::operator WORD*()                          { return &x;              }
LC_WORD3::operator const WORD*() const              { return (const WORD*)&x; }
INT LC_WORD3::operator ==(const LC_WORD3& v) const	{ return x == v.x && y == v.y && z == v.z;	}
INT LC_WORD3::operator !=(const LC_WORD3& v) const	{ return x != v.x || y != v.y || z != v.z;	}


////////////////////////////////////////

LC_SHORT4::LC_SHORT4() : x(0), y(0), z(0), w(0){}
LC_SHORT4::LC_SHORT4(SHORT X,SHORT Y,SHORT Z,SHORT W) : x(X), y(Y), z(Z), w(W){}

LC_SHORT4::LC_SHORT4(const LC_SHORT4& v)   {   *this = v;      }
LC_SHORT4& LC_SHORT4::operator=(const LC_SHORT4& v)
{
	x=v.x; y=v.y; z=v.z; w=v.w;
	return *this;
}

LC_SHORT4::operator SHORT*()                        { return &x;               }
LC_SHORT4::operator const SHORT*() const            { return (const SHORT*)&x; }
INT LC_SHORT4::operator ==(const LC_SHORT4& v) const{ return x == v.x && y == v.y && z == v.z && w == v.w;	}
INT LC_SHORT4::operator !=(const LC_SHORT4& v) const{ return x != v.x || y != v.y || z != v.z || w != v.w;	}


////////////////////////////////////////

LC_WORD4::LC_WORD4() : x(0), y(0), z(0), w(0){}
LC_WORD4::LC_WORD4(WORD X,WORD Y,WORD Z,WORD W) : x(X), y(Y), z(Z), w(W){}

LC_WORD4::LC_WORD4(const LC_WORD4& v)   {   *this = v;      }
LC_WORD4& LC_WORD4::operator=(const LC_WORD4& v)
{
	x=v.x; y=v.y; z=v.z; w=v.w;
	return *this;
}

LC_WORD4::operator WORD*()                          { return &x;              }
LC_WORD4::operator const WORD*() const              { return (const WORD*)&x; }
INT LC_WORD4::operator ==(const LC_WORD4& v) const	{ return x == v.x && y == v.y && z == v.z && w == v.w;	}
INT LC_WORD4::operator !=(const LC_WORD4& v) const	{ return x != v.x || y != v.y || z != v.z || w != v.w;	}




////////////////////////////////////////

LC_INT2::LC_INT2() : x(0), y(0){}
LC_INT2::LC_INT2(INT X,INT Y) : x(X), y(Y){}

LC_INT2::LC_INT2(const LC_INT2& v)   {   *this = v;      }
LC_INT2& LC_INT2::operator=(const LC_INT2& v)
{
	x=v.x; y=v.y;
	return *this;
}

LC_INT2::operator INT*()                            { return &x;             }
LC_INT2::operator const INT*() const                { return (const INT*)&x; }
LC_INT2& LC_INT2::operator+=(const LC_INT2& v)      { x+=v.x;y+=v.y;return *this; }
LC_INT2& LC_INT2::operator-=(const LC_INT2& v)      { x-=v.x;y-=v.y;return *this; }
INT LC_INT2::operator ==(const LC_INT2& v) const	{ return x == v.x && y == v.y;	}
INT LC_INT2::operator !=(const LC_INT2& v) const	{ return x != v.x || y != v.y;	}


////////////////////////////////////////

LC_INT3::LC_INT3() : x(0), y(0), z(0){}
LC_INT3::LC_INT3(INT X,INT Y,INT Z) : x(X), y(Y), z(Z){}

LC_INT3::LC_INT3(const LC_INT3& v)   {   *this = v;      }
LC_INT3& LC_INT3::operator=(const LC_INT3& v)
{
	x=v.x; y=v.y; z=v.z;
	return *this;
}

LC_INT3::operator INT*()                            { return &x;             }
LC_INT3::operator const INT*() const                { return (const INT*)&x; }
LC_INT3& LC_INT3::operator+=(const LC_INT3& v)      { x+=v.x;y+=v.y;z+=v.z;return *this; }
LC_INT3& LC_INT3::operator-=(const LC_INT3& v)      { x-=v.x;y-=v.y;z-=v.z;return *this; }
INT LC_INT3::operator ==(const LC_INT3& v) const	{	return x == v.x && y == v.y && z == v.z;	}
INT LC_INT3::operator !=(const LC_INT3& v) const	{	return x != v.x || y != v.y || z != v.z;	}


////////////////////////////////////////

LC_INT4::LC_INT4() : x(0), y(0), z(0), w(0){}
LC_INT4::LC_INT4(INT X,INT Y,INT Z,INT W) : x(X), y(Y), z(Z), w(W){}

LC_INT4::LC_INT4(const LC_INT4& v)   {   *this = v;      }
LC_INT4& LC_INT4::operator=(const LC_INT4& v)
{
	x=v.x; y=v.y; z=v.z; w=v.w;
	return *this;
}

LC_INT4::operator INT*()                            { return &x;             }
LC_INT4::operator const INT*() const                { return (const INT*)&x; }
LC_INT4& LC_INT4::operator+=(const LC_INT4& v)      { x+=v.x;y+=v.y;z+=v.z;w+=v.w;return *this; }
LC_INT4& LC_INT4::operator-=(const LC_INT4& v)      { x-=v.x;y-=v.y;z-=v.z;w-=v.w;return *this; }
INT LC_INT4::operator ==(const LC_INT4& v) const	{ return x == v.x && y == v.y && z == v.z && w == v.w;	}
INT LC_INT4::operator !=(const LC_INT4& v) const	{ return x != v.x || y != v.y || z != v.z || w != v.w;	}


////////////////////////////////////////

LC_INT6::LC_INT6() : _0(0),_1(0),_2(0),_3(0),_4(0),_5(0){}
LC_INT6::LC_INT6(INT __0,INT __1,INT __2,INT __3,INT __4,INT __5):_0(__0),_1(__1),_2(__2),_3(__3),_4(__4),_5(__5){}

LC_INT6::LC_INT6(const LC_INT6& v)   {   *this = v;      }
LC_INT6& LC_INT6::operator=(const LC_INT6& v)
{
	_0=v._0; _1=v._1; _2=v._2; _3=v._3; _4=v._4; _5=v._5;
	return *this;
}

LC_INT6::operator INT*()                            { return &_0;             }
LC_INT6::operator const INT*() const                { return (const INT*)&_0; }
LC_INT6& LC_INT6::operator+=(const LC_INT6& v)      { _0+=v._0;_1+=v._1;_2+=v._2;_3+=v._3;_4+=v._4;_5+=v._5;return *this; }
LC_INT6& LC_INT6::operator-=(const LC_INT6& v)      { _0-=v._0;_1-=v._1;_2-=v._2;_3-=v._3;_4-=v._4;_5-=v._5;return *this; }
INT LC_INT6::operator ==(const LC_INT6& v) const	{ return _0 == v._0 && _1 == v._1 && _2 == v._2 && _3 == v._3 && _4 == v._4 && _5 == v._5;	}
INT LC_INT6::operator !=(const LC_INT6& v) const	{ return _0 != v._0 || _1 != v._1 || _2 != v._2 || _3 != v._3 || _4 != v._4 || _5 != v._5;	}


////////////////////////////////////////

LC_INT8::LC_INT8() : _0(0),_1(0),_2(0),_3(0),_4(0),_5(0),_6(0),_7(0){}
LC_INT8::LC_INT8(INT __0,INT __1,INT __2,INT __3,INT __4,INT __5,INT __6,INT __7)
:_0(__0),_1(__1),_2(__2),_3(__3)
,_4(__4),_5(__5),_6(__6),_7(__7){}

LC_INT8::LC_INT8(const LC_INT8& v)   {   *this = v;      }
LC_INT8& LC_INT8::operator=(const LC_INT8& v)
{
	_0=v._0; _1=v._1; _2=v._2; _3=v._3; _4=v._4; _5=v._5; _6=v._6; _7=v._7;
	return *this;
}

LC_INT8::operator INT*()                            { return &_0;             }
LC_INT8::operator const INT*() const                { return (const INT*)&_0; }
LC_INT8& LC_INT8::operator+=(const LC_INT8& v)      { _0+=v._0;_1+=v._1;_2+=v._2;_3+=v._3;_4+=v._4;_5+=v._5;_6+=v._6;_7+=v._7;return *this; }
LC_INT8& LC_INT8::operator-=(const LC_INT8& v)      { _0-=v._0;_1-=v._1;_2-=v._2;_3-=v._3;_4-=v._4;_5-=v._5;_6-=v._6;_7-=v._7;return *this; }
INT LC_INT8::operator ==(const LC_INT8& v) const	{ return _0 == v._0 && _1 == v._1 && _2 == v._2 && _3 == v._3 && _4 == v._4 && _5 == v._5 && _6 == v._6 && _7 == v._7;	}
INT LC_INT8::operator !=(const LC_INT8& v) const	{ return _0 != v._0 || _1 != v._1 || _2 != v._2 || _3 != v._3 || _4 != v._4 || _5 != v._5 || _6 != v._6 || _7 != v._7;	}


////////////////////////////////////////

LC_FLOAT2::LC_FLOAT2() : x(0), y(0){}
LC_FLOAT2::LC_FLOAT2(FLOAT X,FLOAT Y) : x(X), y(Y){}
LC_FLOAT2::LC_FLOAT2(const FLOAT* v) { x=v[0]; y=v[1]; }

LC_FLOAT2::LC_FLOAT2(const LC_FLOAT2& v)   {   *this = v;      }
LC_FLOAT2& LC_FLOAT2::operator=(const LC_FLOAT2& v)
{
	x=v.x; y=v.y;
	return *this;
}

// casting
LC_FLOAT2::operator FLOAT*()                             { return(FLOAT*)&x;		}
LC_FLOAT2::operator const FLOAT*() const                 { return(const FLOAT*)&x;	}
FLOAT& LC_FLOAT2::operator[](int n)                      { return *((&x)+ n);		}

// unary operators
LC_FLOAT2& LC_FLOAT2::operator +=(const LC_FLOAT2& v)    {           x += v.x; y += v.y; return *this;	}
LC_FLOAT2& LC_FLOAT2::operator -=(const LC_FLOAT2& v)    {           x -= v.x; y -= v.y; return *this;	}
LC_FLOAT2& LC_FLOAT2::operator *=(FLOAT v)               {           x *= v  ; y *=   v; return *this;	}
LC_FLOAT2& LC_FLOAT2::operator /=(FLOAT v)               { v=1.0F/v; x *=v   ; y *=   v; return *this;	}
LC_FLOAT2 LC_FLOAT2::operator +() const                  { return *this;				}
LC_FLOAT2 LC_FLOAT2::operator -() const                  { return LC_FLOAT2(-x, -y);	}

// binary operators
LC_FLOAT2 LC_FLOAT2::operator+(const LC_FLOAT2& v)const  {           return LC_FLOAT2(x + v.x, y + v.y); }
LC_FLOAT2 LC_FLOAT2::operator-(const LC_FLOAT2& v)const  {           return LC_FLOAT2(x - v.x, y - v.y); }
LC_FLOAT2 LC_FLOAT2::operator*(FLOAT v) const            {           return LC_FLOAT2(x * v  , y * v  ); }
LC_FLOAT2 LC_FLOAT2::operator/(FLOAT v) const            { v=1.0F/v; return LC_FLOAT2(x * v  , y * v  ); }

INT LC_FLOAT2::operator ==(const LC_FLOAT2& v) const     {	return x == v.x && y == v.y ;	}
INT LC_FLOAT2::operator !=(const LC_FLOAT2& v) const     {	return x != v.x || y != v.y ;	}


////////////////////////////////////////

LC_FLOAT3::LC_FLOAT3() : x(0), y(0), z(0){}
LC_FLOAT3::LC_FLOAT3(FLOAT X,FLOAT Y,FLOAT Z) : x(X), y(Y), z(Z){}
LC_FLOAT3::LC_FLOAT3(const FLOAT* v) { x=v[0]; y=v[1]; z=v[2];}

LC_FLOAT3::LC_FLOAT3(const LC_FLOAT3& v)   {   *this = v;      }
LC_FLOAT3& LC_FLOAT3::operator=(const LC_FLOAT3& v)
{
	x=v.x; y=v.y; z=v.z;
	return *this;
}

// casting
LC_FLOAT3::operator FLOAT*()                              { return(FLOAT*)&x;			}
LC_FLOAT3::operator const FLOAT*() const                  { return(const FLOAT*)&x;		}
FLOAT& LC_FLOAT3::operator[](int n)                       { return *((&x)+ n);			}

// unary operators
LC_FLOAT3& LC_FLOAT3::operator +=(const LC_FLOAT3& v)     {            x +=v.x; y +=v.y; z +=v.z; return *this;	}
LC_FLOAT3& LC_FLOAT3::operator -=(const LC_FLOAT3& v)     {            x -=v.x; y -=v.y; z -=v.z; return *this;	}
LC_FLOAT3& LC_FLOAT3::operator *=(FLOAT v)                {            x *= v ; y *= v ; z *= v ; return *this;	}
LC_FLOAT3& LC_FLOAT3::operator /=(FLOAT v)                { v =1.0F/v; x *= v ; y *= v ; z *= v ; return *this;	}
LC_FLOAT3 LC_FLOAT3::operator +() const                   {	return *this;					}
LC_FLOAT3 LC_FLOAT3::operator -() const                   {	return LC_FLOAT3(-x, -y, -z);	}

// binary operators
LC_FLOAT3 LC_FLOAT3::operator +(const LC_FLOAT3& v) const {           return LC_FLOAT3(x+v.x, y+v.y, z+v.z); }
LC_FLOAT3 LC_FLOAT3::operator -(const LC_FLOAT3& v) const {           return LC_FLOAT3(x-v.x, y-v.y, z-v.z); }
LC_FLOAT3 LC_FLOAT3::operator *(FLOAT v) const	          {           return LC_FLOAT3(x*v  , y*v  , z*v  ); }
LC_FLOAT3 LC_FLOAT3::operator /(FLOAT v) const            { v=1.0F/v; return LC_FLOAT3(x*v  , y*v  , z*v  ); }

INT LC_FLOAT3::operator ==(const LC_FLOAT3& v) const      {	return x == v.x && y == v.y && z == v.z;	}
INT LC_FLOAT3::operator !=(const LC_FLOAT3& v) const      {	return x != v.x || y != v.y || z != v.z;	}


////////////////////////////////////////

LC_FLOAT4::LC_FLOAT4() : x(0), y(0), z(0), w(0){}
LC_FLOAT4::LC_FLOAT4(FLOAT X,FLOAT Y,FLOAT Z,FLOAT W) : x(X), y(Y), z(Z), w(W){}
LC_FLOAT4::LC_FLOAT4(const FLOAT* v) { x=v[0]; y=v[1]; z=v[2]; w=v[3]; }

LC_FLOAT4::LC_FLOAT4(const LC_FLOAT4& v)   {   *this = v;      }
LC_FLOAT4& LC_FLOAT4::operator=(const LC_FLOAT4& v)
{
	x=v.x; y=v.y; z=v.z; w=v.w;
	return *this;
}

// casting
LC_FLOAT4::operator FLOAT*()                              { return(FLOAT*)&x;       }
LC_FLOAT4::operator const FLOAT*() const                  { return(const FLOAT*)&x; }
FLOAT& LC_FLOAT4::operator[](int n)                       { return *((&x)+ n);      }

// assignment operators
LC_FLOAT4& LC_FLOAT4::operator +=(const LC_FLOAT4& v)     {            x +=v.x; y +=v.y; z +=v.z; w +=v.w; return *this; }
LC_FLOAT4& LC_FLOAT4::operator -=(const LC_FLOAT4& v)     {            x -=v.x; y -=v.y; z -=v.z; w -=v.w; return *this; }
LC_FLOAT4& LC_FLOAT4::operator *=(FLOAT v)                {            x *= v ; y *= v ; z *= v ; w *= v ; return *this; }
LC_FLOAT4& LC_FLOAT4::operator /=(FLOAT v)                { v =1.0F/v; x *= v ; y *=v  ; z *= v ; w *= v ; return *this; }

// unary operators
LC_FLOAT4 LC_FLOAT4::operator +() const                   {	return *this;					}
LC_FLOAT4 LC_FLOAT4::operator -() const                   {	return LC_FLOAT4(-x,-y,-z,-w);	}

// binary operators
LC_FLOAT4 LC_FLOAT4::operator +(const LC_FLOAT4& v) const {           return LC_FLOAT4(x+v.x, y+v.y, z+v.z, w+v.w); }
LC_FLOAT4 LC_FLOAT4::operator -(const LC_FLOAT4& v) const {           return LC_FLOAT4(x-v.x, y-v.y, z-v.z, w-v.w); }
LC_FLOAT4 LC_FLOAT4::operator *(FLOAT v) const            {           return LC_FLOAT4(x*v  , y*v  , z*v  , w*v  ); }
LC_FLOAT4 LC_FLOAT4::operator /(FLOAT v) const            { v=1.0F/v; return LC_FLOAT4(x*v  , y*v  , z*v  , w*v  ); }

INT LC_FLOAT4::operator ==(const LC_FLOAT4& v) const      {	return x == v.x && y == v.y && z == v.z && w == v.w;	}
INT LC_FLOAT4::operator !=(const LC_FLOAT4& v) const      {	return x != v.x || y != v.y || z != v.z || w != v.w;	}


////////////////////////////////////////

LC_FLOAT6::LC_FLOAT6() : _0(0),_1(0),_2(0),_3(0),_4(0),_5(0){}
LC_FLOAT6::LC_FLOAT6(FLOAT __0,FLOAT __1,FLOAT __2,FLOAT __3,FLOAT __4,FLOAT __5):_0(__0),_1(__1),_2(__2),_3(__3),_4(__4),_5(__5){}
LC_FLOAT6::LC_FLOAT6(const FLOAT* v) { 	_0=v[0]; _1=v[1]; _2=v[2]; _3=v[3];	_4=v[4]; _5=v[5]; }

LC_FLOAT6::LC_FLOAT6(const LC_FLOAT6& v)   {   *this = v;      }
LC_FLOAT6& LC_FLOAT6::operator=(const LC_FLOAT6& v)
{
	_0=v._0; _1=v._1; _2=v._2; _3=v._3; _4=v._4; _5=v._5;
	return *this;
}

// casting
LC_FLOAT6::operator FLOAT*()                              { return (FLOAT*)&_0;      }
LC_FLOAT6::operator const FLOAT*() const                  { return (const FLOAT*)&_0;}
FLOAT& LC_FLOAT6::operator[](int n)                       { return *((&_0)+ n);      }

// assignment operators
LC_FLOAT6& LC_FLOAT6::operator +=(const LC_FLOAT6& v)     {            _0 +=v._0; _1 +=v._1; _2 +=v._2; _3 +=v._3; return *this; }
LC_FLOAT6& LC_FLOAT6::operator -=(const LC_FLOAT6& v)     {            _0 -=v._0; _1 -=v._1; _2 -=v._2; _3 -=v._3; return *this; }
LC_FLOAT6& LC_FLOAT6::operator *=(FLOAT v)                {            _0 *= v ; _1 *= v ; _2 *= v ; _3 *= v ; return *this; }
LC_FLOAT6& LC_FLOAT6::operator /=(FLOAT v)                { v =1.0F/v; _0 *= v ; _1 *=v  ; _2 *= v ; _3 *= v ; return *this; }

// unary operators
LC_FLOAT6 LC_FLOAT6::operator +() const                   {	return *this;			}
LC_FLOAT6 LC_FLOAT6::operator -() const                   {	return LC_FLOAT6(-_0,-_1,-_2,-_3,-_4,-_5);	}

// binary operators
LC_FLOAT6 LC_FLOAT6::operator +(const LC_FLOAT6& v) const {           return LC_FLOAT6(_0+v._0, _1+v._1, _2+v._2, _3+v._3, _4+v._4, _5+v._5); }
LC_FLOAT6 LC_FLOAT6::operator -(const LC_FLOAT6& v) const {           return LC_FLOAT6(_0-v._0, _1-v._1, _2-v._2, _3-v._3, _4-v._4, _5-v._5); }
LC_FLOAT6 LC_FLOAT6::operator *(FLOAT v) const            {           return LC_FLOAT6(_0*v, _1*v, _2*v, _3*v, _4*v, _5*v); }
LC_FLOAT6 LC_FLOAT6::operator /(FLOAT v) const            { v=1.0F/v; return LC_FLOAT6(_0*v, _1*v, _2*v, _3*v, _4*v, _5*v); }

INT LC_FLOAT6::operator ==(const LC_FLOAT6& v) const      {	return _0 == v._0 && _1 == v._1 && _2 == v._2 && _3 == v._3 && _4 == v._4 && _5 == v._5; }
INT LC_FLOAT6::operator !=(const LC_FLOAT6& v) const      {	return _0 != v._0 || _1 != v._1 || _2 != v._2 || _3 != v._3 || _4 != v._4 || _5 != v._5; }


////////////////////////////////////////

LC_FLOAT8::LC_FLOAT8() : _0(0),_1(0),_2(0),_3(0),_4(0),_5(0),_6(0),_7(0){}
LC_FLOAT8::LC_FLOAT8(FLOAT __0,FLOAT __1,FLOAT __2,FLOAT __3,FLOAT __4,FLOAT __5,FLOAT __6,FLOAT __7)
:_0(__0),_1(__1),_2(__2),_3(__3)
,_4(__4),_5(__5),_6(__6),_7(__7){}

LC_FLOAT8::LC_FLOAT8(const LC_FLOAT8& v)   {   *this = v;      }
LC_FLOAT8& LC_FLOAT8::operator=(const LC_FLOAT8& v)
{
	_0=v._0; _1=v._1; _2=v._2; _3=v._3; _4=v._4; _5=v._5; _6=v._6; _7=v._7;
	return *this;
}

LC_FLOAT8::LC_FLOAT8(const FLOAT* v)
{
	_0=v[0]; _1=v[1]; _2=v[2]; _3=v[3];
	_4=v[4]; _5=v[5]; _6=v[6]; _7=v[7];
}

// casting
LC_FLOAT8::operator FLOAT*()                              { return (FLOAT*)&_0;      }
LC_FLOAT8::operator const FLOAT*() const                  { return (const FLOAT*)&_0;}
FLOAT& LC_FLOAT8::operator[](int n)                       { return *((&_0)+ n);      }

// assignment operators
LC_FLOAT8& LC_FLOAT8::operator +=(const LC_FLOAT8& v)     {            _0 +=v._0; _1 +=v._1; _2 +=v._2; _3 +=v._3; return *this; }
LC_FLOAT8& LC_FLOAT8::operator -=(const LC_FLOAT8& v)     {            _0 -=v._0; _1 -=v._1; _2 -=v._2; _3 -=v._3; return *this; }
LC_FLOAT8& LC_FLOAT8::operator *=(FLOAT v)                {            _0 *= v ; _1 *= v ; _2 *= v ; _3 *= v ; return *this; }
LC_FLOAT8& LC_FLOAT8::operator /=(FLOAT v)                { v =1.0F/v; _0 *= v ; _1 *=v  ; _2 *= v ; _3 *= v ; return *this; }

// unary operators
LC_FLOAT8 LC_FLOAT8::operator +() const                   {	return *this;			}
LC_FLOAT8 LC_FLOAT8::operator -() const                   {	return LC_FLOAT8(-_0,-_1,-_2,-_3,-_4,-_5,-_6,-_7);	}

// binary operators
LC_FLOAT8 LC_FLOAT8::operator +(const LC_FLOAT8& v) const {           return LC_FLOAT8(_0+v._0, _1+v._1, _2+v._2, _3+v._3, _4+v._4, _5+v._5, _6+v._6, _7+v._7); }
LC_FLOAT8 LC_FLOAT8::operator -(const LC_FLOAT8& v) const {           return LC_FLOAT8(_0-v._0, _1-v._1, _2-v._2, _3-v._3, _4-v._4, _5-v._5, _6-v._6, _7-v._7); }
LC_FLOAT8 LC_FLOAT8::operator *(FLOAT v) const            {           return LC_FLOAT8(_0*v, _1*v, _2*v, _3*v, _4*v, _5*v, _6*v, _7*v); }
LC_FLOAT8 LC_FLOAT8::operator /(FLOAT v) const            { v=1.0F/v; return LC_FLOAT8(_0*v, _1*v, _2*v, _3*v, _4*v, _5*v, _6*v, _7*v); }

INT LC_FLOAT8::operator ==(const LC_FLOAT8& v) const      {	return _0 == v._0 && _1 == v._1 && _2 == v._2 && _3 == v._3 && _4 == v._4 && _5 == v._5 && _6 == v._6 && _7 == v._7; }
INT LC_FLOAT8::operator !=(const LC_FLOAT8& v) const      {	return _0 != v._0 || _1 != v._1 || _2 != v._2 || _3 != v._3 || _4 != v._4 || _5 != v._5 || _6 != v._6 || _7 != v._7; }


////////////////////////////////////////

LC_COLORF::LC_COLORF() : r(0), g(0), b(0), a(0){}
LC_COLORF::LC_COLORF(FLOAT R,FLOAT G,FLOAT B,FLOAT A) : r(R), g(G), b(B), a(A){}
LC_COLORF::LC_COLORF(COLOR32 D)
{
	const FLOAT f = 1.0F / 255.0F;
	a = f * (FLOAT) ( (BYTE) (D >> 24) );
	r = f * (FLOAT) ( (BYTE) (D >> 16) );
	g = f * (FLOAT) ( (BYTE) (D >>  8) );
	b = f * (FLOAT) ( (BYTE) (D >>  0) );
}

LC_COLORF::LC_COLORF(const LC_COLORF& v)   {   *this = v;      }
LC_COLORF& LC_COLORF::operator=(const LC_COLORF& v)
{
	r=v.r; g=v.g; b=v.b; a=v.a;
	return *this;
}

// casting
LC_COLORF::operator COLOR32 () const
{
	COLOR32 A = (COLOR32) lc_min( (a * 255.0F + 0.5F), 255.0F );
	COLOR32 R = (COLOR32) lc_min( (r * 255.0F + 0.5F), 255.0F );
	COLOR32 G = (COLOR32) lc_min( (g * 255.0F + 0.5F), 255.0F );
	COLOR32 B = (COLOR32) lc_min( (b * 255.0F + 0.5F), 255.0F );
	return (A << 24) | (R << 16) | (G << 8) | B;
}

LC_COLORF::operator FLOAT *()                              { return (FLOAT*) &r;		}
LC_COLORF::operator const FLOAT *() const                  { return (const FLOAT*) &r;	}
FLOAT& LC_COLORF::operator[](int n)                        { return *((&r)+ n);			}

// assignment operators
LC_COLORF& LC_COLORF::operator +=(const LC_COLORF& v)      {           r +=v.r; g +=v.g; b +=v.b; a +=v.a; return *this; }
LC_COLORF& LC_COLORF::operator -=(const LC_COLORF& v)      {           r -=v.r; g -=v.g; b -=v.b; a -=v.a; return *this; }
LC_COLORF& LC_COLORF::operator *=(FLOAT v)                 {           r *= v ; g *= v ; b *= v ; a *= v ; return *this; }
LC_COLORF& LC_COLORF::operator /=(FLOAT v)	               { v=1.0F/v; r *= v ; g *= v ; b *= v ; a *= v ; return *this; }

// unary operators
LC_COLORF LC_COLORF::operator +() const                    { return *this;									}
LC_COLORF LC_COLORF::operator -() const                    { return LC_COLORF(1.0F-r, 1.0F-g, 1.0F-b, a);	}	// Negative

// binary operators. Add, Subtract, Modulate, Devide
LC_COLORF LC_COLORF::operator +(const LC_COLORF& v) const  {           return LC_COLORF(r+v.r, g+v.g, b+v.b, a+v.a); }
LC_COLORF LC_COLORF::operator -(const LC_COLORF& v) const  {           return LC_COLORF(r-v.r, g-v.g, b-v.b, a-v.a); }
LC_COLORF LC_COLORF::operator *(FLOAT v)const              {           return LC_COLORF(r*v  , g*v  , b*v  , a*v  ); }
LC_COLORF LC_COLORF::operator /(FLOAT v)const              { v=1.0F/v; return LC_COLORF(r*v  , g*v  , b*v  , a*v  ); }

INT LC_COLORF::operator ==(const LC_COLORF& v) const       { return r == v.r && g == v.g && b == v.b && a == v.a;	}
INT LC_COLORF::operator !=(const LC_COLORF& v) const       { return r != v.r || g != v.g || b != v.b || a != v.a;	}


////////////////////////////////////////

LC_COLORB::LC_COLORB() : r(0), g(0), b(0), a(0){}
LC_COLORB::LC_COLORB(BYTE R,BYTE G,BYTE B,BYTE A) : r(R), g(G), b(B), a(A){}

LC_COLORB::LC_COLORB(const LC_COLORF& v)
{
	a = (BYTE) lc_min( (v.a * 255.0F), 255.0F );
	r = (BYTE) lc_min( (v.r * 255.0F), 255.0F );
	g = (BYTE) lc_min( (v.g * 255.0F), 255.0F );
	b = (BYTE) lc_min( (v.b * 255.0F), 255.0F );
}

LC_COLORB::LC_COLORB(COLOR32 v)                      { a=BYTE(v>>24);r=BYTE(v>>16);g=BYTE(v>>8);b=BYTE(v>>0);} // v must be a argb
LC_COLORB::LC_COLORB(const LC_COLORB& v)             { *this = v;      }
LC_COLORB& LC_COLORB::operator=(const LC_COLORB& v)
{
	r=v.r; g=v.g; b=v.b; a=v.a;
	return *this;
}

LC_COLORB::operator COLOR32 () const                 { return (a<<24) | (r<<16) | (g<<8) | b;	}
LC_COLORB::operator BYTE* ()                         { return &r;              }
LC_COLORB::operator const BYTE* () const             { return (const BYTE*)&r; }

INT LC_COLORB::operator ==(const LC_COLORB& v) const { return r == v.r && g == v.g && b == v.b && a == v.a;	}
INT LC_COLORB::operator !=(const LC_COLORB& v) const { return r != v.r || g != v.g || b != v.b || a != v.a;	}


////////////////////////////////////////

LC_COLORB3::LC_COLORB3() : r(0), g(0), b(0){}
LC_COLORB3::LC_COLORB3(BYTE R,BYTE G,BYTE B) : r(R), g(G), b(B){}

LC_COLORB3::LC_COLORB3(const LC_COLORF& v)
{
	r = (BYTE) lc_min( (v.r * 255.0F), 255.0F );
	g = (BYTE) lc_min( (v.g * 255.0F), 255.0F );
	b = (BYTE) lc_min( (v.b * 255.0F), 255.0F );
}

LC_COLORB3::LC_COLORB3(COLOR32 v)                    { r=BYTE(v>>16);g=BYTE(v>>8);b=BYTE(v>>0);} // v must be a rgb
LC_COLORB3::LC_COLORB3(const LC_COLORB3& v)          { *this = v;      }
LC_COLORB3& LC_COLORB3::operator=(const LC_COLORB3& v)
{
	r=v.r; g=v.g; b=v.b;
	return *this;
}

LC_COLORB3::operator COLOR32 () const                 { return 0xFF000000 | (r<<16) | (g<<8) | b;	}
LC_COLORB3::operator BYTE* ()                         { return &r;              }
LC_COLORB3::operator const BYTE* () const             { return (const BYTE*)&r; }

INT LC_COLORB3::operator ==(const LC_COLORB3& v) const { return r == v.r && g == v.g && b == v.b ;	}
INT LC_COLORB3::operator !=(const LC_COLORB3& v) const { return r != v.r || g != v.g || b != v.b ;	}


////////////////////////////////////////

LC_RECTF::LC_RECTF() : x(0), y(0), w(0), h(0){}
LC_RECTF::LC_RECTF(FLOAT X,FLOAT Y,FLOAT W,FLOAT H) : x(X), y(Y), w(W), h(H){}

LC_RECTF::LC_RECTF(const LC_RECTF& v)   {   *this = v;      }
LC_RECTF& LC_RECTF::operator=(const LC_RECTF& v)
{
	x=v.x; y=v.y; w=v.w; h=v.h;
	return *this;
}

LC_RECTF::operator FLOAT*()                        { return &x;               }
LC_RECTF::operator const FLOAT*() const            { return (const FLOAT*)&x; }
INT LC_RECTF::operator ==(const LC_RECTF& v) const { return x == v.x && y == v.y && w == v.w && h == v.h;	}
INT LC_RECTF::operator !=(const LC_RECTF& v) const { return x != v.x || y != v.y || w != v.w || h != v.h;	}



//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// 2d transform matrix 3x2

LC_MATRIX3X2::LC_MATRIX3X2()
: _11(1), _12(0)
, _21(0), _22(1)
, _31(0), _32(0){}

LC_MATRIX3X2::LC_MATRIX3X2(
  FLOAT t11, FLOAT t12
, FLOAT t21, FLOAT t22
, FLOAT t31, FLOAT t32)
: _11(t11), _12(t12)
, _21(t21), _22(t22)
, _31(t31), _32(t32){}


LC_MATRIX3X2::LC_MATRIX3X2(const LC_MATRIX3X2& v)   {   *this = v;      }
LC_MATRIX3X2& LC_MATRIX3X2::operator=(const LC_MATRIX3X2& v)
{
	_11 = v._11;	_12 = v._12;
	_21 = v._21;	_22 = v._22;
	_31 = v._31;	_32 = v._32;
	return *this;
}

LC_MATRIX3X2::operator FLOAT*()                           { return &_11;               }
LC_MATRIX3X2::operator const FLOAT*() const               { return (const FLOAT*)&_11; }
INT LC_MATRIX3X2::operator ==(const LC_MATRIX3X2& v) const
{
	return _11 == v._11 && _12 == v._12
		&& _21 == v._21 && _22 == v._22
		&& _31 == v._31 && _32 == v._32
		;
}

INT LC_MATRIX3X2::operator !=(const LC_MATRIX3X2& v) const
{
	return _11 != v._11 || _12 != v._12
		|| _21 != v._21 || _22 != v._22
		|| _31 != v._31 || _32 != v._32
		;
}


////////////////////////////////////////
// 2d transform matrix 3x3

LC_MATRIX3X3::LC_MATRIX3X3()
: _11(1), _12(0), _13(0)
, _21(0), _22(1), _23(0)
, _31(0), _32(0), _33(1){}

LC_MATRIX3X3::LC_MATRIX3X3(
  FLOAT t11, FLOAT t12, FLOAT t13
, FLOAT t21, FLOAT t22, FLOAT t23
, FLOAT t31, FLOAT t32, FLOAT t33)
: _11(t11), _12(t12), _13(t13)
, _21(t21), _22(t22), _23(t23)
, _31(t31), _32(t32), _33(t33){}

LC_MATRIX3X3::LC_MATRIX3X3(const LC_MATRIX3X3& v)   {   *this = v;      }
LC_MATRIX3X3& LC_MATRIX3X3::operator=(const LC_MATRIX3X3& v)
{
	_11 = v._11;	_12 = v._12;	_13 = v._13;
	_21 = v._21;	_22 = v._22;	_23 = v._23;
	_31 = v._31;	_32 = v._32;	_33 = v._33;
	return *this;
}

LC_MATRIX3X3::operator FLOAT*()                           { return &_11;               }
LC_MATRIX3X3::operator const FLOAT*() const               { return (const FLOAT*)&_11; }
INT LC_MATRIX3X3::operator ==(const LC_MATRIX3X3& v) const
{
	return _11 == v._11 && _12 == v._12 && _13 == v._13
		&& _21 == v._21 && _22 == v._22 && _23 == v._23
		&& _31 == v._31 && _32 == v._32 && _33 == v._33;
}

INT LC_MATRIX3X3::operator !=(const LC_MATRIX3X3& v) const
{
	return _11 != v._11 || _12 != v._12 || _13 != v._13
		|| _21 != v._21 || _22 != v._22 || _23 != v._23
		|| _31 != v._31 || _32 != v._32 || _33 != v._33;
}


////////////////////////////////////////
// 2d transform matrix 2x2

LC_MATRIX2X2::LC_MATRIX2X2()
: _11(1), _12(0)
, _21(0), _22(1){}

LC_MATRIX2X2::LC_MATRIX2X2(
  FLOAT t11, FLOAT t12
, FLOAT t21, FLOAT t22)
: _11(t11), _12(t12)
, _21(t21), _22(t22){}

LC_MATRIX2X2::LC_MATRIX2X2(const LC_MATRIX2X2& v)   {   *this = v;      }
LC_MATRIX2X2& LC_MATRIX2X2::operator=(const LC_MATRIX2X2& v)
{
	_11 = v._11;	_12 = v._12;
	_21 = v._21;	_22 = v._22;
	return *this;
}

LC_MATRIX2X2::operator FLOAT*()                           { return &_11;               }
LC_MATRIX2X2::operator const FLOAT*() const               { return (const FLOAT*)&_11; }
INT LC_MATRIX2X2::operator ==(const LC_MATRIX2X2& v) const
{
	return _11 == v._11 && _12 == v._12
		&& _21 == v._21 && _22 == v._22;
}

INT LC_MATRIX2X2::operator !=(const LC_MATRIX2X2& v) const
{
	return _11 != v._11 || _12 != v._12
		|| _21 != v._21 || _22 != v._22;
}


////////////////////////////////////////
// 3d transform matrix 4x3

LC_MATRIX4X3::LC_MATRIX4X3()
: _11(1), _12(0), _13(0)
, _21(0), _22(1), _23(0)
, _31(0), _32(0), _33(1)
, _41(0), _42(0), _43(0){}

LC_MATRIX4X3::LC_MATRIX4X3(
  FLOAT t11, FLOAT t12, FLOAT t13
, FLOAT t21, FLOAT t22, FLOAT t23
, FLOAT t31, FLOAT t32, FLOAT t33
, FLOAT t41, FLOAT t42, FLOAT t43)
: _11(t11), _12(t12), _13(t13)
, _21(t21), _22(t22), _23(t23)
, _31(t31), _32(t32), _33(t33)
, _41(t41), _42(t42), _43(t43){}

LC_MATRIX4X3::LC_MATRIX4X3(const LC_MATRIX4X3& v)   {   *this = v;      }
LC_MATRIX4X3& LC_MATRIX4X3::operator=(const LC_MATRIX4X3& v)
{
	_11 = v._11;	_12 = v._12;	_13 = v._13;
	_21 = v._21;	_22 = v._22;	_23 = v._23;
	_31 = v._31;	_32 = v._32;	_33 = v._33;
	_41 = v._41;	_32 = v._42;	_33 = v._43;
	return *this;
}

LC_MATRIX4X3::operator FLOAT*()                           { return &_11;               }
LC_MATRIX4X3::operator const FLOAT*() const               { return (const FLOAT*)&_11; }
INT LC_MATRIX4X3::operator ==(const LC_MATRIX4X3& v) const
{
	return _11 == v._11 && _12 == v._12 && _13 == v._13
		&& _21 == v._21 && _22 == v._22 && _23 == v._23
		&& _31 == v._31 && _32 == v._32 && _33 == v._33
		&& _41 == v._41 && _42 == v._42 && _43 == v._43
		;
}

INT LC_MATRIX4X3::operator !=(const LC_MATRIX4X3& v) const
{
	return _11 != v._11 || _12 != v._12 || _13 != v._13
		|| _21 != v._21 || _22 != v._22 || _23 != v._23
		|| _31 != v._31 || _32 != v._32 || _33 != v._33
		|| _41 != v._41 || _42 != v._42 || _43 != v._43
		;
}


////////////////////////////////////////
// 3d transform matrix 4x4

LC_MATRIX::LC_MATRIX()
: _11(1), _12(0), _13(0), _14(0)
, _21(0), _22(1), _23(0), _24(0)
, _31(0), _32(0), _33(1), _34(0)
, _41(0), _42(0), _43(0), _44(1){}

LC_MATRIX::LC_MATRIX(
  FLOAT t11, FLOAT t12, FLOAT t13, FLOAT t14
, FLOAT t21, FLOAT t22, FLOAT t23, FLOAT t24
, FLOAT t31, FLOAT t32, FLOAT t33, FLOAT t34
, FLOAT t41, FLOAT t42, FLOAT t43, FLOAT t44)
: _11(t11), _12(t12), _13(t13), _14(t14)
, _21(t21), _22(t22), _23(t23), _24(t24)
, _31(t31), _32(t32), _33(t33), _34(t34)
, _41(t41), _42(t42), _43(t43), _44(t44){}

LC_MATRIX::LC_MATRIX(const LC_MATRIX& v)   {   *this = v;      }
LC_MATRIX& LC_MATRIX::operator=(const LC_MATRIX& v)
{
	_11 = v._11;	_12 = v._12;	_13 = v._13;	_14 = v._14;
	_21 = v._21;	_22 = v._22;	_23 = v._23;	_24 = v._24;
	_31 = v._31;	_32 = v._32;	_33 = v._33;	_34 = v._34;
	_41 = v._41;	_32 = v._42;	_33 = v._43;	_34 = v._44;
	return *this;
}

LC_MATRIX::operator FLOAT*()                        { return &_11;               }
LC_MATRIX::operator const FLOAT*() const            { return (const FLOAT*)&_11; }
INT LC_MATRIX::operator ==(const LC_MATRIX& v) const
{
	return _11 == v._11 && _12 == v._12 && _13 == v._13 && _14 == v._14
		&& _21 == v._21 && _22 == v._22 && _23 == v._23 && _24 == v._24
		&& _31 == v._31 && _32 == v._32 && _33 == v._33 && _34 == v._34
		&& _41 == v._41 && _42 == v._42 && _43 == v._33 && _44 == v._34
		;
}

INT LC_MATRIX::operator !=(const LC_MATRIX& v) const
{
	return _11 != v._11 || _12 != v._12 || _13 != v._13 || _14 != v._14
		|| _21 != v._21 || _22 != v._22 || _23 != v._23 || _24 != v._24
		|| _31 != v._31 || _32 != v._32 || _33 != v._33 || _34 != v._34
		|| _41 != v._41 || _42 != v._42 || _43 != v._43 || _44 != v._44
		;
}


//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// glc argument interface

static std::recursive_mutex g_lock_mutex;


//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8

TLC_VAR::TLC_VAR()
: _i0(0),  _i1(0),  _i2(0),  _i3(0),  _i4(0),  _i5(0),  _i6(0),  _i7(0)
, _i8(0),  _i9(0),  _ia(0),  _ib(0),  _ic(0),  _id(0),  _ie(0),  _if(0)
{}

TLC_VAR::TLC_VAR(
  INT _0, INT _1, INT _2, INT _3
, INT _4, INT _5, INT _6, INT _7
, INT _8, INT _9, INT _a, INT _b
, INT _c, INT _d, INT _e, INT _f)
: _i0(_0),  _i1(_1),  _i2(_2),  _i3(_3),  _i4(_4),  _i5(_5),  _i6(_6),  _i7(_7)
, _i8(_8),  _i9(_9),  _ia(_a),  _ib(_b),  _ic(_c),  _id(_d),  _ie(_e),  _if(_f)
{}

TLC_VAR::TLC_VAR(
  DINT _0, DINT _1, DINT _2, DINT _3
, DINT _4, DINT _5, DINT _6, DINT _7)
: _n0(_0),  _n1(_1),  _n2(_2),  _n3(_3),  _n4(_4),  _n5(_5),  _n6(_6),  _n7(_7)
{}

TLC_VAR::TLC_VAR(
  FLOAT _0, FLOAT _1, FLOAT _2, FLOAT _3
, FLOAT _4, FLOAT _5, FLOAT _6, FLOAT _7
, FLOAT _8, FLOAT _9, FLOAT _a, FLOAT _b
, FLOAT _c, FLOAT _d, FLOAT _e, FLOAT _f)
: _f0(_0),  _f1(_1),  _f2(_2),  _f3(_3),  _f4(_4),  _f5(_5),  _f6(_6),  _f7(_7)
, _f8(_8),  _f9(_9),  _fa(_a),  _fb(_b),  _fc(_c),  _fd(_d),  _fe(_e),  _ff(_f)
{}


TLC_VAR::TLC_VAR(
  DOUBLE _0, DOUBLE _1, DOUBLE _2, DOUBLE _3
, DOUBLE _4, DOUBLE _5, DOUBLE _6, DOUBLE _7)
: _d0(_0),  _d1(_1),  _d2(_2),  _d3(_3),  _d4(_4),  _d5(_5),  _d6(_6),  _d7(_7)
{}


TLC_VAR::TLC_VAR(
  LC_HANDLE _0, LC_HANDLE _1,  LC_HANDLE _2,  LC_HANDLE _3
, LC_HANDLE _4, LC_HANDLE _5,  LC_HANDLE _6,  LC_HANDLE _7)
: _p0(_0),  _p1(_1),  _p2(_2),  _p3(_3),  _p4(_4),  _p5(_5),  _p6(_6),  _p7(_7)
{}


TLC_VAR& TLC_VAR::operator = (const TLC_VAR& v)
{
	_i0 = v._i0; _i1 = v._i1; _i2 = v._i2; _i3 = v._i3;
	_i4 = v._i4; _i5 = v._i5; _i6 = v._i6; _i7 = v._i7;
	_i8 = v._i8; _i9 = v._i9; _ia = v._ia; _ib = v._ib;
	_ic = v._ic; _id = v._id; _ie = v._ie; _if = v._if;

	return *this;
}


TLC_VAR& TLC_VAR::operator = (const TLC_VAR* v)
{
	_i0 = v->_i0; _i1 = v->_i1; _i2 = v->_i2; _i3 = v->_i3;
	_i4 = v->_i4; _i5 = v->_i5; _i6 = v->_i6; _i7 = v->_i7;
	_i8 = v->_i8; _i9 = v->_i9; _ia = v->_ia; _ib = v->_ib;
	_ic = v->_ic; _id = v->_id; _ie = v->_ie; _if = v->_if;

	return *this;
}


INT TLC_VAR::operator == (const TLC_VAR& v) const
{
	return	_i0 == v._i0 && _i1 == v._i1 && _i2 == v._i2 && _i3 == v._i3 &&
			_i4 == v._i4 && _i5 == v._i5 && _i6 == v._i6 && _i7 == v._i7 &&
			_i8 == v._i8 && _i9 == v._i9 && _ia == v._ia && _ib == v._ib &&
			_ic == v._ic && _id == v._id && _ie == v._ie && _if == v._if ;
}


INT TLC_VAR::operator != (const TLC_VAR& v) const
{
	return	_i0 != v._i0 || _i1 != v._i1 || _i2 != v._i2 || _i3 != v._i3 ||
			_i4 != v._i4 || _i5 != v._i5 || _i6 != v._i6 || _i7 != v._i7 ||
			_i8 != v._i8 || _i9 != v._i9 || _ia != v._ia || _ib != v._ib ||
			_ic != v._ic || _id != v._id || _ie != v._ie || _if != v._if ;
}


void TLC_VAR::Clear()
{
	_i0 = _i1 = _i2 = _i3 = _i4 = _i5 = _i6 = _i7 = 0;
	_i8 = _i9 = _ia = _ib = _ic = _id = _ie = _if = 0;
}


void TLC_VAR::Int8  (INT index, INT8      v){	if(64<= index) return; v_b[index] = v; }
void TLC_VAR::Int16 (INT index, INT16     v){	if(32<= index) return; v_s[index] = v; }
void TLC_VAR::Int32 (INT index, INT32     v){	if(16<= index) return; v_i[index] = v; }
void TLC_VAR::Int64 (INT index, INT64     v){	if( 8<= index) return; v_n[index] = v; }
void TLC_VAR::Float (INT index, FLOAT     v){	if(16<= index) return; v_f[index] = v; }
void TLC_VAR::Double(INT index, DOUBLE    v){	if( 8<= index) return; v_d[index] = v; }
void TLC_VAR::Handle(INT index, LC_HANDLE v){	if(16<= index) return; v_p[index] = v; }

void TLC_VAR::ArrayInt8  (INT8     * v, INT count){ if(64< count) count = 64; memcpy(v_b, v, count*sizeof(INT8     )); }
void TLC_VAR::ArrayInt16 (INT16    * v, INT count){ if(32< count) count = 32; memcpy(v_s, v, count*sizeof(INT16    )); }
void TLC_VAR::ArrayInt32 (INT32    * v, INT count){ if(16< count) count = 16; memcpy(v_i, v, count*sizeof(INT32    )); }
void TLC_VAR::ArrayInt64 (INT64    * v, INT count){ if( 8< count) count =  8; memcpy(v_n, v, count*sizeof(INT64    )); }
void TLC_VAR::ArrayFloat (FLOAT    * v, INT count){ if(16< count) count = 16; memcpy(v_f, v, count*sizeof(FLOAT    )); }
void TLC_VAR::ArrayDouble(DOUBLE   * v, INT count){ if( 8< count) count =  8; memcpy(v_d, v, count*sizeof(DOUBLE   )); }
void TLC_VAR::ArrayHandle(LC_HANDLE* v, INT count){ if(16< count) count = 16; memcpy(v_p, v, count*sizeof(LC_HANDLE)); }
void TLC_VAR::String     (char     * v, INT count){ if(60< count) count = 60; memcpy(v_p, v, count*sizeof(char     )); }

INT8      TLC_VAR::Int8  (INT index){ return v_b[index]; }
INT16     TLC_VAR::Int16 (INT index){ return v_s[index]; }
INT32     TLC_VAR::Int32 (INT index){ return v_i[index]; }
INT64     TLC_VAR::Int64 (INT index){ return v_n[index]; }
FLOAT     TLC_VAR::Float (INT index){ return v_f[index]; }
DOUBLE    TLC_VAR::Double(INT index){ return v_d[index]; }
LC_HANDLE TLC_VAR::Handle(INT index){ return v_p[index]; }

INT8     * TLC_VAR::ArrayInt8  (){ return (INT8*)v_b; }
INT16    * TLC_VAR::ArrayInt16 (){ return v_s; }
INT32    * TLC_VAR::ArrayInt32 (){ return v_i; }
INT64    * TLC_VAR::ArrayInt64 (){ return v_n; }
FLOAT    * TLC_VAR::ArrayFloat (){ return v_f; }
DOUBLE   * TLC_VAR::ArrayDouble(){ return v_d; }
LC_HANDLE* TLC_VAR::ArrayHandle(){ return v_p; }
char     * TLC_VAR::String     (){ return (char*)v_b; }

