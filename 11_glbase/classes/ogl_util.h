#pragma once

#ifndef _OGL_UTIL_H_
#define _OGL_UTIL_H_

#include <map>

#ifndef GL_LINEAR
#define GL_LINEAR                         0x2601
#endif
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE                  0x812F
#endif

void printGLString(const char *name, int s);
bool checkGLError(const char* functionLastCalled);

class GLTexture
{
protected:
	int m_tex;
	int m_filter;
	int m_wrap;

public:
	static GLTexture* createFromFile(const char* file_name, int filterMinMag = GL_LINEAR, int wrapMode = GL_CLAMP_TO_EDGE);
	static GLTexture* createFromBuffer(char* buffer, size_t size, int filterMinMag = GL_LINEAR, int wrapMode = GL_CLAMP_TO_EDGE);

public:
	GLTexture();
	virtual ~GLTexture();

	int  Init(char* buffer, size_t size, int filterMinMag = GL_LINEAR, int wrapMode = GL_CLAMP_TO_EDGE);
	void Destroy();
	void BindStage   (int stage, int filterMinMag = 0, int wrapMode = 0);
	void UnBindStage (int stage);
	int  SetPixel    (int w, int h, int f, int t, void* pxl);
	int  TexID       () { return m_tex; }
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

class GLProgram
{
protected:
	unsigned int m_vert;
	unsigned int m_frag;
	unsigned int m_prog;
	std::map<int, GLTexture*>	m_texture;

public:
	static GLProgram* createFromFile(const char* file_vertexShaderSource, const char* file_fragmentShaderSource);
	static GLProgram* createFromBuffer(const char* vertexShaderSource, const char* fragmentShaderSource);

public:
	GLProgram();
	~GLProgram();

	int  Init(const char* vertexShaderSource, const char* fragmentShaderSource);
	int  InitFromFile(const char* vertexShaderFile, const char* fragmentShaderFile);
	void Destroy();
	void BeginProgram();
	void EndProgram();

	int UniformLocation(const char* uniform_name);
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

#endif

