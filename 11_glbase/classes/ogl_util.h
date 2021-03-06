﻿#pragma once

#ifndef _OGL_UTIL_H_
#define _OGL_UTIL_H_

#include <string>
#include <map>
#include "LcType.h"
#include "LcMath.h"

#ifndef GL_LINEAR
#define GL_LINEAR                         0x2601
#endif
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE                  0x812F
#endif

void printGLString(const char *name, int s);
bool checkGLError(const char* functionLastCalled);

struct RenderObject
{
	virtual ~RenderObject(){};
	virtual int Init      (CPVOID =NULL, CPVOID =NULL, CPVOID =NULL, CPVOID =NULL) =0;
	virtual int Destroy   () =0;
	virtual int FrameMove () =0;
	virtual int Render    () =0;
};

class GLTexture
{
public:
	enum
	{
		TYPE_NONE = 0,
		TYPE_2D   = 2,
		TYPE_3D   = 3,
		TYPE_CUBE = 6,
	};
protected:
	int m_type;
	int m_tex;
	int m_filter;
	int m_wrap;

public:
	static GLTexture* createFromFile(const char* file_name, int type=TYPE_2D, int filterMinMag = GL_LINEAR, int wrapMode = GL_CLAMP_TO_EDGE);

public:
	GLTexture();
	virtual ~GLTexture();

	int  Init2D(const char* file_name, int type, int filterMinMag, int wrapMode);
	int  InitCube(const char* file_name, int type, int filterMinMag, int wrapMode);
	void Destroy();
	void BindStage   (int stage, int filterMinMag = 0, int wrapMode = 0);
	void UnBindStage (int stage);
	int  SetPixel    (int w, int h, int f, int t, void* pxl);
	int  TexType     () const { return m_type; }
	int  TexID       () const { return m_tex; }
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

class GLProgram
{
protected:
	UINT m_vert;
	UINT m_frag;
	UINT m_prog;
	std::map<int, GLTexture*>	m_texture;

public:
	static GLProgram* createFromFile(const char* file_vertexShaderSource, const char* file_fragmentShaderSource, const std::vector<std::string>& ls_attr={std::string("at_pos")});
	static GLProgram* createFromBuffer(const char* vertexShaderSource, const char* fragmentShaderSource, const std::vector<std::string>& ls_attr={std::string("at_pos")});

public:
	GLProgram();
	virtual ~GLProgram();

	int  Init(const char* vertexShaderSource, const char* fragmentShaderSource, const std::vector<std::string>& ls_attr={std::string("at_pos")});
	int  InitFromFile(const char* vertexShaderFile, const char* fragmentShaderFile, const std::vector<std::string>& ls_attr={std::string("at_pos")});
	void Destroy();
	void BeginProgram();
	void EndProgram();

	int UniformLocation(const char* uniform_name);
	int Mat3x3  (const char* uniform_name, const float* float16);
	int Mat4x4  (const char* uniform_name, const float* float16);
	int Matrix16(const char* uniform_name, const float* float16);
	int Vector4 (const char* uniform_name, const float* float4);
	int Vector3 (const char* uniform_name, const float* float3);
	int Vector2 (const char* uniform_name, const float* float2);
	int Float   (const char* uniform_name, const float v);
	int Int     (const char* uniform_name, const int v);
	int Color4  (const char* uniform_name, const float* float4);
	int Color3  (const char* uniform_name, const float* float3);
	int Texture (const char* uniform_name, int stage);
	int Texture (const char* uniform_name, int stage, GLTexture* tex);
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class GLFBO
{
protected:
	int m_tex = 0;	// texture
	int m_rnd = 0;	// render buffer
	int m_frm = 0;	// frame buffer
	int store_tex = 0;
	int store_rnd = 0;
	int store_frm = 0;
    GLProgram* m_prg ={};
public:
	static GLFBO* create(int width, int height);

	GLFBO(int width=1080, int height=720);
	void begin();
	void end();
	void draw();

protected:
	int Init(int w, int h);
	void Destroy();
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

class GLCamera : public RenderObject
{
public:
	enum
	{
		GLCAM_NONE = 0,
		GLCAM_3D   = 1,
		GLCAM_2D   = 2,
		GLCAM_GUI  = 3,

	};
protected:
	MAT4X4	mt_viw;
	MAT4X4	mt_prj;
	LCXVEC3 v3_eye;		// camera position
	LCXVEC3 v3_look;	// camera look at position
	LCXVEC3 v3_up;		// camera up vector;
	bool	b_update;
	std::string m_name;
public:
	GLCamera();
	virtual ~GLCamera(){}

	static GLCamera*    create(int type, const char* name=NULL);
	static void         remove(GLCamera**);
	static void         globalCamera(const std::string& name, GLCamera*);
	static GLCamera*    globalCamera(const std::string& name);

	protected: virtual int Init      (CPVOID =NULL, CPVOID =NULL, CPVOID =NULL, CPVOID =NULL) final;
	protected: virtual int Destroy   () final{ return 0;};
	public   : virtual int FrameMove () override;
	protected: virtual int Render    () final{ return 0;};

public:
	void            Eye    (const LCXVEC3& eye);
	const LCXVEC3*  Eye    () const { return &v3_eye; }
	void            LookAt (const LCXVEC3& lookat);
	const LCXVEC3*  LookAt () const { return &v3_eye; }

	void       Param   (const LCXVEC3& eye, const LCXVEC3& lookat, const LCXVEC3& up);
	void       Up      (const LCXVEC3& up);

	const MAT4X4* View () const { return &mt_viw;}
	const MAT4X4* Proj () const { return &mt_prj;}

	const std::string& name() const { return m_name; }
};

//------------------------------------------------------------------------------

class GLCamera3D : public GLCamera
{
public:
	GLCamera3D();
	virtual ~GLCamera3D(){}
	public   : virtual int FrameMove () override;
};

//------------------------------------------------------------------------------

class GLCamera2D : public GLCamera
{
public:
	GLCamera2D();
	virtual ~GLCamera2D(){}
	public   : virtual int FrameMove () override;
};

//------------------------------------------------------------------------------

class GLCameraGui : public GLCamera
{
public:
	GLCameraGui();
	virtual ~GLCameraGui(){}
	public   : virtual int FrameMove () override;
};

//------------------------------------------------------------------------------

struct GLMesh
{
	UINT pos, nor, tzn, tex;	// position, normal, tanzent, texture id
	UINT idx;					// index id
};

struct GLMeshCube
{
	UINT pos, nor, tex;			// position, normal, texture id
	UINT idx;					// index id
};


#endif

