
#include <vector>

#ifdef _WIN32
  #include <tchar.h>
#endif
#include <stdio.h>

#if defined(__APPLE__)
  #define GL_GLEXT_PROTOTYPES
  #import <OpenGLES/ES2/glext.h>
#else
  #include <GLES2/gl2.h>
#endif

#include "app_util.h"
#include "ogl_util.h"

void printGLString(const char *name, int s)
{
	const char *v = (const char *)glGetString(s);
	LOGI("GL %s = %s\n", name, v);
}

bool checkGLError(const char* functionLastCalled)
{
	int error;
	bool ret = false;
	while(GL_NO_ERROR != (error = glGetError())) {
		//WinErrorMessageBox(functionLastCalled, error);
		if(error == GL_INVALID_ENUM)
			break;
		LOGI("after %s() glError (0x%X)\n", functionLastCalled, error);
		ret = true;
	}
	return ret;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

GLTexture* GLTexture::createFromFile(const char* file_name, int filterMinMag, int wrapMode)
{
	FileData file_data(file_name);
	if(0>=file_data.size())
		return NULL;

	GLTexture* ret = new GLTexture;
	if(0>ret->Init(file_data.data(), file_data.size(), filterMinMag, wrapMode))
	{
		delete ret;
		return NULL;
	}
	return ret;
}

GLTexture* GLTexture::createFromBuffer(char* buffer, size_t size, int filterMinMag, int wrapMode)
{
	GLTexture* ret = new GLTexture;
	if(0>ret->Init(buffer, size, filterMinMag, wrapMode))
	{
		delete ret;
		return NULL;
	}
	return ret;
}

GLTexture::GLTexture()
	: m_tex(0)
	, m_filter(GL_LINEAR)
	, m_wrap  (GL_CLAMP_TO_EDGE)
{
}

GLTexture::~GLTexture()
{
	Destroy();
}

int GLTexture::Init(char* buffer, size_t size, int filterMinMag, int wrapMode)
{
	int				nImgW = 0;
	int				nImgH = 0;
	int				nImgD = 0;
	unsigned char*	pPxl  = NULL;

	int hr = LoadTGA(&nImgW, &nImgH, &nImgD, &pPxl, buffer, size);
	if(0>hr)
		return -1;

	glGenTextures(1, (GLuint *)&m_tex);
	if(0 >= m_tex)
		return -1;

	int tex_prev = 0;											// previous binding texture
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &tex_prev);			// get the stored texture

	glBindTexture(GL_TEXTURE_2D, m_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMinMag);//, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMinMag);//, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);//, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);//, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nImgW, nImgH, 0, GL_RGBA, GL_UNSIGNED_BYTE, pPxl);

	glBindTexture(GL_TEXTURE_2D, tex_prev);
	delete[] pPxl;


	m_filter = filterMinMag;
	m_wrap   = wrapMode;
	return (int)m_tex;
}

void GLTexture::Destroy()
{
	if(m_tex)
	{
		glDeleteTextures(1, (const GLuint *)&m_tex);
		m_tex = 0;
	}
}

void GLTexture::BindStage(int stage, int filterMinMag, int wrapMode)
{
	int filter =  filterMinMag ? filterMinMag : m_filter;
	int  wrap  =  wrapMode     ? wrapMode     : m_wrap;

	glActiveTexture(GL_TEXTURE0 + stage);
	//glEnable(GL_TEXTURE_2D);													W/Adreno-ES20(19516): <get_simple_queries:1544>: GL_INVALID_ENUM
	glBindTexture(GL_TEXTURE_2D, m_tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
}

void GLTexture::UnBindStage(int stage)
{
	glActiveTexture(GL_TEXTURE0 + stage);
	glBindTexture(GL_TEXTURE_2D, 0);
	//if(0 == stage)
	//	glDisable(GL_TEXTURE_2D);												W/Adreno-ES20(19516): <get_simple_queries:1544>: GL_INVALID_ENUM

}

int GLTexture::SetPixel(int w, int h, int f, int t, void* pxl)
{
	if(0 < m_tex)
		glDeleteTextures(1, (const GLuint*)&m_tex);

	glGenTextures(1, (GLuint*)&m_tex);
	if(0 >= m_tex)
		return -1;

	glBindTexture(GL_TEXTURE_2D, m_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrap);
	glTexImage2D(GL_TEXTURE_2D, 0, f, w, h, 0, f, t, pxl);
	return 0;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

GLProgram* GLProgram::createFromFile(const char* file_vertexShaderSource, const char* file_fragmentShaderSource)
{
	FileData file_vtx(file_vertexShaderSource);
	FileData file_frg(file_fragmentShaderSource);
	if(0>=file_vtx.size() || 0>= file_frg.size())
		return NULL;

	GLProgram* ret = new GLProgram;
	if(0>ret->Init(file_vtx.data(), file_frg.data()))
	{
		delete ret;
		return NULL;
	}
	return ret;
}

GLProgram* GLProgram::createFromBuffer(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	GLProgram* ret = new GLProgram;
	if(0>ret->Init(vertexShaderSource, fragmentShaderSource))
	{
		delete ret;
		return NULL;
	}
	return ret;
}


GLProgram::GLProgram()
	: m_vert(0)
	, m_frag(0)
	, m_prog(0)
{
}

GLProgram::~GLProgram()
{
	Destroy();
}

int GLProgram::Init(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	if(!vertexShaderSource || !vertexShaderSource)
		return -1;

	GLint isShaderCompiled=0;

	m_frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_frag, 1, (const char**)&fragmentShaderSource, NULL);
	glCompileShader(m_frag);
	glGetShaderiv(m_frag, GL_COMPILE_STATUS, &isShaderCompiled);
	if(!isShaderCompiled)
	{
		int infoLogLength, charactersWritten;
		glGetShaderiv(m_frag, GL_INFO_LOG_LENGTH, &infoLogLength);

		std::vector<char> infoLog; infoLog.resize(infoLogLength);
		glGetShaderInfoLog(m_frag, infoLogLength, &charactersWritten, infoLog.data());
		WinErrorMessageBox(infoLogLength > 1 ? infoLog.data() : "compile fragment shader. (No information)", 0);
		return -1;
	}

	m_vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_vert, 1, (const char**)&vertexShaderSource, NULL);
	glCompileShader(m_vert);
	glGetShaderiv(m_vert, GL_COMPILE_STATUS, &isShaderCompiled);
	if(!isShaderCompiled)
	{
		int infoLogLength, charactersWritten;
		glGetShaderiv(m_vert, GL_INFO_LOG_LENGTH, &infoLogLength);

		std::vector<char> infoLog; infoLog.resize(infoLogLength);
		glGetShaderInfoLog(m_vert, infoLogLength, &charactersWritten, infoLog.data());
		WinErrorMessageBox(infoLogLength > 1 ? infoLog.data() : "compile vertex shader.(No information)", 0);
		return -1;
	}

	m_prog = glCreateProgram();
	glAttachShader(m_prog, m_vert);
	glAttachShader(m_prog, m_frag);

	glBindAttribLocation(m_prog, 0, "at_pos");

	GLint isLinked;
	glLinkProgram(m_prog);
	glGetProgramiv(m_prog, GL_LINK_STATUS, &isLinked);
	if(!isLinked)
	{
		int infoLogLength, charactersWritten;
		glGetProgramiv(m_prog, GL_INFO_LOG_LENGTH, &infoLogLength);

		std::vector<char> infoLog; infoLog.resize(infoLogLength);
		glGetProgramInfoLog(m_prog, infoLogLength, &charactersWritten, infoLog.data());
		WinErrorMessageBox(infoLogLength > 1 ? infoLog.data() : "link GL program object.(No information)", 0);
		return -1;
	}
	return 0;
}

int GLProgram::InitFromFile(const char* vertexShaderFile, const char* fragmentShaderFile)
{
	FileData vert_data(vertexShaderFile);
	FileData frag_data(fragmentShaderFile);

	char* vertShaderSource = vert_data.data();
	char* fragShaderSource = frag_data.data();
	return this->Init(vertShaderSource, fragShaderSource);
}

void GLProgram::Destroy()
{
	if(m_vert) { glDeleteShader(m_vert); m_vert = 0; }
	if(m_frag) { glDeleteShader(m_frag); m_frag = 0; }
	if(m_prog) { glDeleteProgram(m_prog); m_prog = 0; }
}

void GLProgram::BeginProgram()
{
	glUseProgram(m_prog);
	//checkGLError("glUseProgram");
}

void GLProgram::EndProgram()
{
	for(std::map<int, GLTexture*>::reverse_iterator it= m_texture.rbegin(); it != m_texture.rend(); ++it)
	{
		int stage = it->first;
		GLTexture * tex = it->second;
		tex->UnBindStage(stage);
	}
	m_texture.clear();

	glUseProgram(0);
}

int GLProgram::UniformLocation(const char* uniform_name)
{
	return glGetUniformLocation(m_prog, uniform_name);
}

int GLProgram::Matrix16(const char* uniform_name, const float* float16)
{
	int loc= glGetUniformLocation(m_prog, uniform_name);
	if(0>loc)
		return -1;
	glUniformMatrix4fv(loc, 1, GL_FALSE, float16);
	//if(!checkGLError("glUniformMatrix4fv"))
	//	return -1;
	return 0;
}

int GLProgram::Vector4(const char* uniform_name, const float* float4)
{
	int loc= glGetUniformLocation(m_prog, uniform_name);
	if(0>loc)
		return -1;
	glUniform4fv(loc, 1, float4);
	//if(!checkGLError("glUniform4fv"))
	//	return -1;
	return 0;
}

int GLProgram::Vector3(const char* uniform_name, const float* float3)
{
	int loc= glGetUniformLocation(m_prog, uniform_name);
	if(0>loc)
		return -1;
	glUniform3fv(loc, 1, float3);
	//if(!checkGLError("glUniform3fv"))
	//	return -1;
	return 0;
}

int GLProgram::Vector2(const char* uniform_name, const float* float2)
{
	int loc= glGetUniformLocation(m_prog, uniform_name);
	if(0>loc)
		return -1;
	glUniform2fv(loc, 1, float2);
	//if(!checkGLError("glUniform2fv"))
	//	return -1;
	return 0;
}

int GLProgram::Float(const char* uniform_name, const float v)
{
	int loc= glGetUniformLocation(m_prog, uniform_name);
	if(0>loc)
		return -1;
	glUniform1f(loc, v);
	//if(!checkGLError("glUniform1f"))
	//	return -1;
	return 0;
}

int GLProgram::Int(const char* uniform_name, const int v)
{
	int loc= glGetUniformLocation(m_prog, uniform_name);
	if(0>loc)
		return -1;
	glUniform1i(loc, v);
	//if(!checkGLError("glUniform1i"))
	//	return -1;
	return 0;
}

int GLProgram::Color4(const char* uniform_name, const float* float4)
{
	return this->Vector4(uniform_name, float4);
}

int GLProgram::Color3(const char* uniform_name, const float* float3)
{
	return this->Vector4(uniform_name, float3);
}

int GLProgram::Texture(const char* uniform_name, int stage)
{
	return this->Int(uniform_name, stage);
}

int GLProgram::Texture(const char* uniform_name, int stage, GLTexture* tex)
{
	if(!tex || 0>this->Texture(uniform_name, stage))
		return -1;

	m_texture.insert(std::pair<int, GLTexture*>(stage, tex));
	tex->BindStage(stage);
	return 0;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

GLFBO* GLFBO::create(int width, int height)
{
	GLFBO* ret = new GLFBO;
	if(0>ret->Init(width, height))
	{
		delete ret;
		return NULL;
	}
	return ret;
}



GLFBO::GLFBO(int width, int height)
	: m_tex(0)
	, m_rnd(0)
	, m_frm(0)
	, m_prg(NULL)
{
	Init(width, height);
}

int GLFBO::Init(int w, int h)
{
	m_prg = GLProgram::createFromFile("media/shader/tex2d.vert", "media/shader/tex2d.frag");
	if(!m_prg)
		return -1;

	glGetIntegerv(GL_TEXTURE_2D, &store_tex);
	glGetIntegerv(GL_RENDERBUFFER_BINDING, &store_rnd);
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &store_frm );

	glGenTextures(1, (GLuint*)&m_tex);
	glGenRenderbuffers(1, (GLuint*)&m_rnd);
	glGenFramebuffers(1, (GLuint*)&m_frm);

	glBindTexture(GL_TEXTURE_2D, m_tex);
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, store_tex);
	}

	glBindRenderbuffer(GL_RENDERBUFFER, m_rnd);
	{
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h);
		glBindRenderbuffer(GL_RENDERBUFFER, store_rnd);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_frm);
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rnd);
		glBindFramebuffer(GL_FRAMEBUFFER, store_frm);
	}

	return 0;
}

void GLFBO::Destroy()
{
	if(0<m_rnd)
	{
		glDeleteRenderbuffers(1, (GLuint*)m_rnd);
		m_rnd = 0;
	}
	if(0<m_frm)
	{
		glDeleteFramebuffers(1, (GLuint*)m_frm);
		m_frm = 0;
	}
	if(0<m_tex)
	{
		glDeleteTextures(1, (GLuint*)m_tex);
		m_tex = 0;
	}
	SAFE_DELETE(m_prg);
}

void GLFBO::begin()
{
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &store_frm );
	glBindFramebuffer(GL_FRAMEBUFFER, m_frm);
	checkGLError("GLFBO::begin()");
}

void GLFBO::end()
{
	glBindFramebuffer(GL_FRAMEBUFFER, store_frm);						// disable Framebuffer
}

void GLFBO::draw()
{
	m_prg->BeginProgram();

	float vtx_pos[] =
	{
		-0.99F, -0.99F,
		 0.99F, -0.99F,
		 0.99F,  0.99F,
		-0.99F,  0.99F,
	};
	float vtx_tex[] =
	{
		0.0F, 0.0F,
		1.0F, 0.0F,
		1.0F, 1.0F,
		0.0F, 1.0F
	};

	glEnableVertexAttribArray(0); glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, vtx_pos);
	glEnableVertexAttribArray(1); glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, vtx_tex);

	int nStage = 0;
	glActiveTexture(GL_TEXTURE0 + nStage);
	glBindTexture(GL_TEXTURE_2D, m_tex);

	m_prg->Int("us_tx0", nStage);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glUseProgram(0);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

RenderObject* GLCamera::create()
{
	GLCamera* ret = new GLCamera;
	if(0>ret->Init())
	{
		delete ret;
		return NULL;
	}
	return ret;
}

static std::map<std::string, GLCamera*> global_cam;

void GLCamera::globalCamera(const std::string& name, RenderObject* cam)
{
	if(global_cam.find(name) != global_cam.end())
		return;
	global_cam.emplace(name, (GLCamera*)cam);
}

GLCamera* GLCamera::globalCamera(const std::string& name)
{
	auto it = global_cam.find(name);
	if(it == global_cam.end())
		return NULL;

	GLCamera* ret = it->second;
	return ret;
}

GLCamera::GLCamera()
	: v3_eye (0, -20, 0)
	, v3_look(0,   0, 0)
	, v3_up  (0,   0, 1)
	, b_update(true)
{
}

int GLCamera::Init()
{
	this->FrameMove();
	return 0;
}

int GLCamera::FrameMove()
{
	if(!b_update)
		return 0;

	float	vpt[16]={0};
	glGetFloatv(GL_VIEWPORT, vpt);
	float scnW = vpt[2];
	float scnH = vpt[3];

	float	Asp = scnW/scnH;
	float	Near = 1.F;
	float	Far	 = 1000.F;
	float	Fov  = 45.F;
	mt_prj.PerspectiveD3dRH( LCXToRadian(Fov), Asp, Near, Far);
	mt_viw.ViewGl(&v3_eye, &v3_look, &v3_up);
	b_update = false;
	return 0;
}

void GLCamera::Param (const LCXVEC3& eye, const LCXVEC3& lookat, const LCXVEC3& up)
{
	v3_eye  = eye;
	v3_look = lookat;
	v3_up   = up;
	b_update = true;
}

void GLCamera::Eye   (const LCXVEC3& eye)
{
	v3_eye  = eye;
	b_update = true;
}

void GLCamera::LookAt(const LCXVEC3& lookat)
{
	v3_look = lookat;
	b_update = true;
}

void GLCamera::Up(const LCXVEC3& up)
{
	v3_up = up;
	b_update = true;
}

