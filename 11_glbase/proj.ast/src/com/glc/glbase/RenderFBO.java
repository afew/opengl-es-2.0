package com.glc.glbase;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import android.opengl.GLES20;

public class RenderFBO
{
	private int[] m_tex = new int[8];	// texture
	private int[] m_rnd = new int[8];	// render buffer
	private int[] m_frm = new int[8];	// frame buffer
	private int[] store_tex = new int[8];
	private int[] store_rnd = new int[8];
	private int[] store_frm = new int[8];
	private int[] store_attrib = new int[8];
	private int[] store_prog = new int[8];

	private int         gl_prog = 0;
	private FloatBuffer vtx_pos;
	private FloatBuffer vtx_tex;

	private int us_tx0;

	public RenderFBO(int width, int height)
	{
		init(width, height);
	}
	public RenderFBO()
	{
		init(1080, 720);
	}

	protected int init(int w, int h)
	{
		GLES20.glGetIntegerv(GLES20.GL_TEXTURE_BINDING_2D  , store_tex, 0);
		GLES20.glGetIntegerv(GLES20.GL_RENDERBUFFER_BINDING, store_rnd, 0);
		GLES20.glGetIntegerv(GLES20.GL_FRAMEBUFFER_BINDING , store_frm, 0);

		GLES20.glGenTextures     (1, m_tex, 0);
		GLES20.glGenRenderbuffers(1, m_rnd, 0);
		GLES20.glGenFramebuffers (1, m_frm, 0);

		GLES20.glBindTexture  (GLES20.GL_TEXTURE_2D, m_tex[0]);
		GLES20.glTexImage2D   (GLES20.GL_TEXTURE_2D, 0, GLES20.GL_RGBA, w, h, 0, GLES20.GL_RGBA, GLES20.GL_UNSIGNED_BYTE, null);
		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);
		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
		GLES20.glBindTexture  (GLES20.GL_TEXTURE_2D, store_tex[0]);

		GLES20.glBindRenderbuffer   (GLES20.GL_RENDERBUFFER, m_rnd[0]);
		GLES20.glRenderbufferStorage(GLES20.GL_RENDERBUFFER, GLES20.GL_DEPTH_COMPONENT16, w, h);
		GLES20.glBindRenderbuffer   (GLES20.GL_RENDERBUFFER, store_rnd[0]);

		GLES20.glBindFramebuffer        (GLES20.GL_FRAMEBUFFER, m_frm[0]);
		GLES20.glFramebufferTexture2D   (GLES20.GL_FRAMEBUFFER, GLES20.GL_COLOR_ATTACHMENT0, GLES20.GL_TEXTURE_2D, m_tex[0], 0);
		GLES20.glFramebufferRenderbuffer(GLES20.GL_FRAMEBUFFER, GLES20.GL_DEPTH_ATTACHMENT, GLES20.GL_RENDERBUFFER, m_rnd[0]);
		GLES20.glBindFramebuffer        (GLES20.GL_FRAMEBUFFER, store_frm[0]);


		final float[] tvtx_pos =
		{
			-0.9F, -0.9F,
			 0.9F, -0.9F,
			 0.9F,  0.9F,
			-0.9F,  0.9F,
		};
		vtx_pos = ByteBuffer.allocateDirect(tvtx_pos.length * 4)//Float.SIZE/4)
					.order(ByteOrder.nativeOrder())
					.asFloatBuffer();
		vtx_pos.put(tvtx_pos).position(0);

		final float[] tvtx_tex =
		{
			0.0F, 0.0F,
			1.0F, 0.0F,
			1.0F, 1.0F,
			0.0F, 1.0F
		};
		vtx_tex = ByteBuffer.allocateDirect(tvtx_tex.length * 4)//Float.SIZE/4)
					.order(ByteOrder.nativeOrder())
					.asFloatBuffer();
		vtx_tex.put(tvtx_tex).position(0);


		gl_prog  = createProgram(gVertexShader, gFragmentShader);
		us_tx0 = GLES20.glGetUniformLocation(gl_prog, "us_tx0");
		return 0;
	}

	public void destroy()
	{
		if(0<m_rnd[0])
		{
			GLES20.glDeleteRenderbuffers (1, m_rnd, 0);
			m_rnd[0] = 0;
		}
		if(0<m_frm[0])
		{
			GLES20.glDeleteFramebuffers(1, m_frm, 0);
			m_frm[0] = 0;
		}
		if(0<m_tex[0])
		{
			GLES20.glDeleteTextures(1, m_tex, 0);
			m_tex[0] = 0;
		}
	}

	public void begin()
	{
		GLES20.glGetIntegerv    (GLES20.GL_FRAMEBUFFER , store_frm, 0);
		GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, m_frm[0]);							// binding Framebuffer
	}

	public void end()
	{
		GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, store_frm[0]);						// disable Framebuffer
	}

	public void draw()
	{
		GLES20.glGetIntegerv      (GLES20.GL_TEXTURE_2D  , store_tex, 0);
		GLES20.glGetIntegerv      (GLES20.GL_CURRENT_PROGRAM, store_prog, 0);
		GLES20.glGetVertexAttribiv(1, GLES20.GL_VERTEX_ATTRIB_ARRAY_ENABLED  , store_attrib, 0);

		GLES20.glUseProgram(gl_prog);

		vtx_pos.position(0);	GLES20.glEnableVertexAttribArray(0); GLES20.glVertexAttribPointer(0, 2, GLES20.GL_FLOAT, false, 0, vtx_pos);
		vtx_tex.position(0);	GLES20.glEnableVertexAttribArray(1); GLES20.glVertexAttribPointer(1, 2, GLES20.GL_FLOAT, false, 0, vtx_tex);

		int nStage = 0;
		GLES20.glActiveTexture(GLES20.GL_TEXTURE0 + nStage);
	    GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, m_tex[0]);

	    GLES20.glUniform1i(us_tx0, nStage);

	    GLES20.glDrawArrays(GLES20.GL_TRIANGLE_FAN, 0, 4);

	    GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, store_tex[0]);
	    if(0 ==store_attrib[0])
	    	GLES20.glDisableVertexAttribArray(1);
	    GLES20.glGetIntegerv(GLES20.GL_CURRENT_PROGRAM, store_prog, 0);
		GLES20.glUseProgram(store_prog[0]);
	}



	int loadShader(int shaderType, String pSource)
	{
		int shader = GLES20.glCreateShader(shaderType);
		if(0 != shader) {
			GLES20.glShaderSource(shader, pSource);
			GLES20.glCompileShader(shader);
			int[] compiled = new int[1];
			GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compiled, 0);
			if(0 == compiled[0]) {
				GLES20.glDeleteShader(shader);
				shader = 0;
			}
		}
		return shader;
	}


	private int createProgram(String pVertexSource, String pFragmentSource) {
		int vertexShader = loadShader(GLES20.GL_VERTEX_SHADER, pVertexSource);
		if(0 >= vertexShader) {
			return 0;
		}

		int pixelShader = loadShader(GLES20.GL_FRAGMENT_SHADER, pFragmentSource);
		if(0 >= pixelShader) {
			return 0;
		}

		int program = GLES20.glCreateProgram();
		if(0 ==program)
			return 0;

		GLES20.glAttachShader(program, vertexShader);
		GLES20.glAttachShader(program, pixelShader);
		GLES20.glBindAttribLocation(program, 0, "att_pos");

		GLES20.glLinkProgram(program);
		int[] isLinked = new int[1];
		GLES20.glGetProgramiv(program, GLES20.GL_LINK_STATUS, isLinked, 0);
		if(0 == isLinked[0])
		{
			GLES20.glDeleteProgram(program);
			program = 0;
		}
		return program;
	}

	static final String gVertexShader =
		  "precision mediump float;						\n"
		+ "												\n"
		+ "attribute vec4 att_pos;						\n"
		+ "attribute vec2 att_tx0;						\n"
		+ "												\n"
		+ "varying   vec2 var_tx0;						\n"
		+ "												\n"
		+ "void main() {								\n"
		+ "												\n"
		+ "  var_tx0     = att_tx0;						\n"
		+ "  gl_Position = att_pos;						\n"
		+ "}											\n";

	static final String gFragmentShader =
		  "precision mediump float;						\n"
		+ "												\n"
		+ "varying   vec2 var_tx0;						\n"
		+ "												\n"
		+ "uniform   sampler2D us_tx0;					\n"
		+ "												\n"
		+ "void main() {								\n"
		+ "												\n"
		+ "  vec4 dif  = texture2D(us_tx0, var_tx0);	\n"
		+ "  gl_FragColor = vec4(dif.rgb, 1.0);			\n"
		+ "}											\n";
}
