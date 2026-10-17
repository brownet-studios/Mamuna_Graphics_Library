#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <Mamuna.h>

#include <stdio.h>
#include <string.h>

typedef struct DRAWCOMMAND {
	GLuint count;
	GLuint instanceCount;
	GLuint firstIndex;
	GLint baseVertex;
	GLuint baseInstance;
} DRAWCOMMAND;

typedef struct Mamuna_Buffer {
	void* ptr;
	GLuint buffer;
	size_t capacity;
	size_t bytesPerElement;
	size_t countToIncrease;
} Mamuna_Buffer;

typedef struct Mamuna_Shader {
	GLuint shader;
	Mamuna_Hashmap* ssbos;
	Mamuna_Hashmap* ubos;
} Mamuna_Shader;

typedef struct Mamuna_Texture {
	GLuint texture;
	MAMUNA_ENUM format;
	MAMUNA_ENUM type;
	size_t w, h, d, levels;
} Mamuna_Texture;

typedef struct Mamuna_VAO {
	GLuint vao;
	GLenum type;
} Mamuna_VAO;

typedef struct Mamuna_Renderer {
	GLFWwindow* window;
	Mamuna_Shader* currentShader;
	Mamuna_Texture** currentTextures;
	Mamuna_VAO* currentVAO;	
	DRAWCOMMAND* pRIBO;
	GLbitfield flags;
	GLenum mode;
	GLuint FBO;
	GLuint windowTexture;
	GLuint RB;
	GLuint RIBO;
	size_t countRIBO;
	size_t capacityRIBO;
	size_t currentW, currentH;
	size_t texturesCount;
	GLsync fence;
} Mamuna_Renderer;

void GLAPIENTRY Mamuna_DebugCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam ){
  fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}

void Mamuna_EnableDebugOutput(Mamuna_Renderer* renderer){
	if(!renderer) return;
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(Mamuna_DebugCallback, 0);
}
void Mamuna_DisableDebugOutput(Mamuna_Renderer* renderer){
	if(!renderer) return;
	glDisable(GL_DEBUG_OUTPUT);
}

void Mamuna_SyncGPU(Mamuna_Renderer* renderer){
	if(!renderer || !renderer->fence) return;
	GLenum result;
	do{
		result = glClientWaitSync(renderer->fence, GL_SYNC_FLUSH_COMMANDS_BIT, UINT64_MAX);
	} while(result == GL_TIMEOUT_EXPIRED);




	glDeleteSync(renderer->fence);
	renderer->fence = 0;
}

GLenum Mamuna_GetGLEnum(MAMUNA_ENUM e){
	switch(e){
		case MAMUNA_FORMAT_TEXTURE_DEPTH_COMP32F:
			return GL_DEPTH_COMPONENT32F;
		case MAMUNA_FORMAT_TEXTURE_DEPTH_COMP24:
			return GL_DEPTH_COMPONENT24;
		case MAMUNA_FORMAT_TEXTURE_DEPTH_COMP16:
			return GL_DEPTH_COMPONENT16;
		case MAMUNA_FORMAT_TEXTURE_DEPTH32F_STENCIL8:
			return GL_DEPTH32F_STENCIL8;
		case MAMUNA_FORMAT_TEXTURE_DEPTH24_STENCIL8:
			return GL_DEPTH24_STENCIL8;
		case MAMUNA_FORMAT_TEXTURE_STENCIL_INDEX8:
			return GL_STENCIL_INDEX8;
		case MAMUNA_FORMAT_TEXTURE_R8:
			return GL_R8;
		case MAMUNA_FORMAT_TEXTURE_R8_SNORM:
			return GL_R8_SNORM;
		case MAMUNA_FORMAT_TEXTURE_R16:
			return GL_R16;
		case MAMUNA_FORMAT_TEXTURE_R16_SNORM:
			return GL_R16_SNORM;
		case MAMUNA_FORMAT_TEXTURE_RG8:
			return GL_RG8;
		case MAMUNA_FORMAT_TEXTURE_RG8_SNORM:
			return GL_RG8_SNORM;
		case MAMUNA_FORMAT_TEXTURE_RG16:
			return GL_RG16;
		case MAMUNA_FORMAT_TEXTURE_RG16_SNORM:
			return GL_RG16_SNORM;
		case MAMUNA_FORMAT_TEXTURE_R3_G3_B2:
			return GL_R3_G3_B2;
		case MAMUNA_FORMAT_TEXTURE_RGB4:
			return GL_RGB4;
		case MAMUNA_FORMAT_TEXTURE_RGB5:
			return GL_RGB5;
		case MAMUNA_FORMAT_TEXTURE_RGB8:
			return GL_RGB8;
		case MAMUNA_FORMAT_TEXTURE_RGB8_SNORM:
			return GL_RGB8_SNORM;
		case MAMUNA_FORMAT_TEXTURE_RGB10:
			return GL_RGB10;
		case MAMUNA_FORMAT_TEXTURE_RGB12:
			return GL_RGB12;
		case MAMUNA_FORMAT_TEXTURE_RGB16_SNORM:
			return GL_RGB16_SNORM;
		case MAMUNA_FORMAT_TEXTURE_RGBA2:
			return GL_RGBA2;
		case MAMUNA_FORMAT_TEXTURE_RGBA4:
			return GL_RGBA4;
		case MAMUNA_FORMAT_TEXTURE_RGB5_A1:
			return GL_RGB5_A1;
		case MAMUNA_FORMAT_TEXTURE_RGBA8:
			return GL_RGBA8;
		case MAMUNA_FORMAT_TEXTURE_RGBA8_SNORM:
			return GL_RGBA8_SNORM;
		case MAMUNA_FORMAT_TEXTURE_RGB10_A2:
			return GL_RGB10_A2;
		case MAMUNA_FORMAT_TEXTURE_RGB10_A2UI:
			return GL_RGB10_A2UI;
		case MAMUNA_FORMAT_TEXTURE_RGBA12:
			return GL_RGBA12;
		case MAMUNA_FORMAT_TEXTURE_RGBA16:
			return GL_RGBA16;
		case MAMUNA_FORMAT_TEXTURE_SRGB8:
			return GL_SRGB8;
		case MAMUNA_FORMAT_TEXTURE_SRGB8_ALPHA8:
			return GL_SRGB8_ALPHA8;
		case MAMUNA_FORMAT_TEXTURE_R16F:
			return GL_R16F;
		case MAMUNA_FORMAT_TEXTURE_RG16F:
			return GL_RG16F;
		case MAMUNA_FORMAT_TEXTURE_RGB16F:
			return GL_RGB16F;
		case MAMUNA_FORMAT_TEXTURE_RGBA16F:
			return GL_RGBA16F;
		case MAMUNA_FORMAT_TEXTURE_R32F:
			return GL_R32F;
		case MAMUNA_FORMAT_TEXTURE_RG32F:
			return GL_RG32F;
		case MAMUNA_FORMAT_TEXTURE_RGB32F:
			return GL_RGB32F;
		case MAMUNA_FORMAT_TEXTURE_RGBA32F:
			return GL_RGBA32F;
		case MAMUNA_FORMAT_TEXTURE_R11F_G11F_B10F:
			return GL_R11F_G11F_B10F;
		case MAMUNA_FORMAT_TEXTURE_RGB9_E5:
			return GL_RGB9_E5;
		case MAMUNA_FORMAT_TEXTURE_R8I:
			return GL_R8I;
		case MAMUNA_FORMAT_TEXTURE_R8UI:
			return GL_R8UI;
		case MAMUNA_FORMAT_TEXTURE_R16I:
			return GL_R16I;
		case MAMUNA_FORMAT_TEXTURE_R16UI:
			return GL_R16UI;
		case MAMUNA_FORMAT_TEXTURE_R32I:
			return GL_R32I;
		case MAMUNA_FORMAT_TEXTURE_R32UI:
			return GL_R32UI;
		case MAMUNA_FORMAT_TEXTURE_RG8I:
			return GL_RG8I;
		case MAMUNA_FORMAT_TEXTURE_RG8UI:
			return GL_RG8UI;
		case MAMUNA_FORMAT_TEXTURE_RG16I:
			return GL_RG16I;
		case MAMUNA_FORMAT_TEXTURE_RG16UI:
			return GL_RG16UI;
		case MAMUNA_FORMAT_TEXTURE_RG32I:
			return GL_RG32I;
		case MAMUNA_FORMAT_TEXTURE_RG32UI:
			return GL_RG32UI;
		case MAMUNA_FORMAT_TEXTURE_RGB8I:
			return GL_RGB8I;
		case MAMUNA_FORMAT_TEXTURE_RGB8UI:
			return GL_RGB8UI;
		case MAMUNA_FORMAT_TEXTURE_RGB16I:
			return GL_RGB16I;
		case MAMUNA_FORMAT_TEXTURE_RGB16UI:
			return GL_RGB16UI;
		case MAMUNA_FORMAT_TEXTURE_RGB32I:
			return GL_RGB32I;
		case MAMUNA_FORMAT_TEXTURE_RGB32UI:
			return GL_RGB32UI;
		case MAMUNA_FORMAT_TEXTURE_RGBA8I:
			return GL_RGBA8I;
		case MAMUNA_FORMAT_TEXTURE_RGBA8UI:
			return GL_RGBA8UI;
		case MAMUNA_FORMAT_TEXTURE_RGBA16I:
			return GL_RGBA16I;
		case MAMUNA_FORMAT_TEXTURE_RGBA16UI:
			return GL_RGBA16UI;
		case MAMUNA_FORMAT_TEXTURE_RGBA32I:
			return GL_RGBA32I;
		case MAMUNA_FORMAT_TEXTURE_RGBA32UI:
			return GL_RGBA32UI;

		case MAMUNA_PARAM_TEXTURE_DEPTH_STENCIL_MODE:
			return GL_DEPTH_STENCIL_TEXTURE_MODE;
		case MAMUNA_PARAM_TEXTURE_BASE_LEVEL:
			return GL_TEXTURE_BASE_LEVEL;
		case MAMUNA_PARAM_TEXTURE_BORDER_COLOR:
			return GL_TEXTURE_BORDER_COLOR;
		case MAMUNA_PARAM_TEXTURE_COMPARE_FUNC:
			return GL_TEXTURE_COMPARE_FUNC;
		case MAMUNA_PARAM_TEXTURE_COMPARE_MODE:
			return GL_TEXTURE_COMPARE_MODE;
		case MAMUNA_PARAM_TEXTURE_LOD_BIAS:
			return GL_TEXTURE_LOD_BIAS;
		case MAMUNA_PARAM_TEXTURE_MIN_FILTER:
			return GL_TEXTURE_MIN_FILTER;
		case MAMUNA_PARAM_TEXTURE_MAG_FILTER:
			return GL_TEXTURE_MAG_FILTER;
		case MAMUNA_PARAM_TEXTURE_MIN_LOD:
			return GL_TEXTURE_MIN_LOD;
		case MAMUNA_PARAM_TEXTURE_MAX_LOD:
			return GL_TEXTURE_MAX_LOD;
		case MAMUNA_PARAM_TEXTURE_MAX_LEVEL:
			return GL_TEXTURE_MAX_LEVEL;
		case MAMUNA_PARAM_TEXTURE_SWIZZLE_R:
			return GL_TEXTURE_SWIZZLE_R;
		case MAMUNA_PARAM_TEXTURE_SWIZZLE_G:
			return GL_TEXTURE_SWIZZLE_G;
		case MAMUNA_PARAM_TEXTURE_SWIZZLE_B:
			return GL_TEXTURE_SWIZZLE_B;
		case MAMUNA_PARAM_TEXTURE_SWIZZLE_A:
			return GL_TEXTURE_SWIZZLE_A;
		case MAMUNA_PARAM_TEXTURE_WRAP_S:
			return GL_TEXTURE_WRAP_S;
		case MAMUNA_PARAM_TEXTURE_WRAP_T:
			return GL_TEXTURE_WRAP_T;
		case MAMUNA_PARAM_TEXTURE_WRAP_R:
			return GL_TEXTURE_WRAP_R;

		case MAMUNA_WRAP_TEXTURE_CLAMP_TO_EDGE:
			return GL_CLAMP_TO_EDGE;
		case MAMUNA_WRAP_TEXTURE_CLAMP_TO_BORDER:
			return GL_CLAMP_TO_BORDER;
		case MAMUNA_WRAP_TEXTURE_MIRRORED_REPEAT:
			return GL_MIRRORED_REPEAT;
		case MAMUNA_WRAP_TEXTURE_REPEAT:
			return GL_REPEAT;
		case MAMUNA_WRAP_TEXTURE_MIRROR_CLAMP_TO_EDGE:
			return GL_MIRROR_CLAMP_TO_EDGE;

		case MAMUNA_TEXTURE_1D:
			return GL_TEXTURE_1D;
		case MAMUNA_TEXTURE_2D:
			return GL_TEXTURE_2D;
		case MAMUNA_TEXTURE_3D:
			return GL_TEXTURE_3D;
		case MAMUNA_TEXTURE_1D_ARRAY:
			return GL_TEXTURE_1D_ARRAY;
		case MAMUNA_TEXTURE_2D_ARRAY:
			return GL_TEXTURE_2D_ARRAY;
		case MAMUNA_TEXTURE_2D_MULTISAMPLE:
			return GL_TEXTURE_2D_MULTISAMPLE;
		case MAMUNA_TEXTURE_2D_MULTISAMPLE_ARRAY:
			return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;

		case MAMUNA_DEPTH_COMP:
			return GL_DEPTH_COMPONENT;
		case MAMUNA_STENCIL_INDEX:
			return GL_STENCIL_INDEX;
		
		case MAMUNA_TEXTURE_COMPARE_REF_TO_TEXTURE:
			return GL_COMPARE_REF_TO_TEXTURE;
		
		case MAMUNA_LEQUAL:
			return GL_LEQUAL;
		case MAMUNA_GEQUAL:
			return GL_GEQUAL;
		case MAMUNA_LESS:
			return GL_LESS;
		case MAMUNA_GREATER:
			return GL_GREATER;
		case MAMUNA_EQUAL:
			return GL_EQUAL;
		case MAMUNA_NOTEQUAL:
			return GL_NOTEQUAL;
		case MAMUNA_ALWAYS:
			return GL_ALWAYS;
		case MAMUNA_NEVER:
			return GL_NEVER;

		case MAMUNA_BUFFER_UBO:
			return GL_UNIFORM_BLOCK;
		case MAMUNA_BUFFER_SSBO:
			return GL_SHADER_STORAGE_BLOCK;

		case MAMUNA_POLYGON_POINT:
			return GL_POINT;
		case MAMUNA_POLYGON_LINE:
			return GL_LINE;
		case MAMUNA_POLYGON_FILL:
			return GL_FILL;

		case MAMUNA_DRAW_POINTS:
			return GL_POINTS;
		case MAMUNA_DRAW_LINES:
			return GL_LINES;
		case MAMUNA_DRAW_LINE_STRIP:
			return GL_LINE_STRIP;
		case MAMUNA_DRAW_LINE_LOOP:
			return GL_LINE_LOOP;
		case MAMUNA_DRAW_LINE_STRIP_ADJACENCY:
			return GL_LINE_STRIP_ADJACENCY;
		case MAMUNA_DRAW_LINES_ADJACENCY:
			return GL_LINES_ADJACENCY;
		case MAMUNA_DRAW_TRIANGLE_STRIP:
			return GL_TRIANGLE_STRIP;
		case MAMUNA_DRAW_TRIANGLE_FAN:
			return GL_TRIANGLE_FAN;
		case MAMUNA_DRAW_TRIANGLES:
			return GL_TRIANGLES;
		case MAMUNA_DRAW_TRIANGLE_STRIP_ADJACENCY:
			return GL_TRIANGLE_STRIP_ADJACENCY;
		case MAMUNA_DRAW_TRIANGLES_ADJACENCY:
			return GL_TRIANGLES_ADJACENCY;
		case MAMUNA_DRAW_PATCHES:
			return GL_PATCHES;

		case MAMUNA_INVALID_ENUM:
			return GL_INVALID_ENUM;
		case MAMUNA_ZERO:
			return GL_ZERO;
		case MAMUNA_RED:
			return GL_RED;
		case MAMUNA_GREEN:
			return GL_GREEN;
		case MAMUNA_BLUE:
			return GL_BLUE;
		case MAMUNA_ALPHA:
			return GL_ALPHA;
		case MAMUNA_ONE:
			return GL_ONE;
		case MAMUNA_FLOAT:
			return GL_FLOAT;
		case MAMUNA_BYTE:
			return GL_BYTE;
		case MAMUNA_SHORT:
			return GL_SHORT;
		case MAMUNA_INT:
			return GL_INT;
		case MAMUNA_FIXED:
			return GL_FIXED;
		case MAMUNA_DOUBLE:
			return GL_DOUBLE;
		case MAMUNA_UNSIGNED_BYTE:
			return GL_UNSIGNED_BYTE;
		case MAMUNA_UNSIGNED_SHORT:
			return GL_UNSIGNED_SHORT;
		case MAMUNA_UNSIGNED_INT:
			return GL_UNSIGNED_INT;
		case MAMUNA_BLEND:
			return GL_BLEND;
		case MAMUNA_COLOR_LOGIC_OP:
			return GL_COLOR_LOGIC_OP;
		case MAMUNA_CULL_FACE:
			return GL_CULL_FACE;
		case MAMUNA_DEPTH_CLAMP:
			return GL_DEPTH_CLAMP;
		case MAMUNA_DEPTH_TEST:
			return GL_DEPTH_TEST;
		case MAMUNA_DITHER:
			return GL_DITHER;
		case MAMUNA_FRAMEBUFFER_SRGB:
			return GL_FRAMEBUFFER_SRGB;
		case MAMUNA_LINE_SMOOTH:
			return GL_LINE_SMOOTH;
		case MAMUNA_POLYGON_OFFSET_FILL:
			return GL_POLYGON_OFFSET_FILL;
		case MAMUNA_POLYGON_OFFSET_LINE:
			return GL_POLYGON_OFFSET_LINE;
		case MAMUNA_POLYGON_OFFSET_POINT:
			return GL_POLYGON_OFFSET_POINT;
		case MAMUNA_POLYGON_SMOOTH:
			return GL_POLYGON_SMOOTH;
		case MAMUNA_PRIMITIVE_RESTART:
			return GL_PRIMITIVE_RESTART;
		case MAMUNA_PRIMITIVE_RESTART_FIXED_INDEX:
			return GL_PRIMITIVE_RESTART_FIXED_INDEX;
		case MAMUNA_SCISSOR_TEST:
			return GL_SCISSOR_TEST;
		case MAMUNA_STENCIL_TEST:
			return GL_STENCIL_TEST;
		case MAMUNA_PROGRAM_POINT_SIZE:
			return GL_PROGRAM_POINT_SIZE;
		case MAMUNA_SRC_COLOR:
			return GL_SRC_COLOR;
		case MAMUNA_ONE_MINUS_SRC_COLOR:
			return GL_ONE_MINUS_SRC_COLOR;
		case MAMUNA_DST_COLOR:
			return GL_DST_COLOR;
		case MAMUNA_ONE_MINUS_DST_COLOR:
			return GL_ONE_MINUS_DST_COLOR;
		case MAMUNA_SRC_ALPHA:
			return GL_SRC_ALPHA;
		case MAMUNA_ONE_MINUS_SRC_ALPHA:
			return GL_ONE_MINUS_SRC_ALPHA;
		case MAMUNA_DST_ALPHA:
			return GL_DST_ALPHA;
		case MAMUNA_ONE_MINUS_DST_ALPHA:
			return GL_ONE_MINUS_DST_ALPHA;
		case MAMUNA_CONSTANT_COLOR:
			return GL_CONSTANT_COLOR;
		case MAMUNA_ONE_MINUS_CONSTANT_COLOR:
			return GL_ONE_MINUS_CONSTANT_COLOR;
		case MAMUNA_CONSTANT_ALPHA:
			return GL_CONSTANT_ALPHA;
		case MAMUNA_ONE_MINUS_CONSTANT_ALPHA:
			return GL_ONE_MINUS_CONSTANT_ALPHA;
		case MAMUNA_SRC_ALPHA_SATURATE:
			return GL_SRC_ALPHA_SATURATE;
		case MAMUNA_SRC1_COLOR:
			return GL_SRC1_COLOR;
		case MAMUNA_ONE_MINUS_SRC1_COLOR:
			return GL_ONE_MINUS_SRC1_COLOR;
		case MAMUNA_SRC1_ALPHA:
			return GL_SRC1_ALPHA;
		case MAMUNA_CLEAR:
			return GL_CLEAR;
		case MAMUNA_SET:
			return GL_SET;
		case MAMUNA_COPY:
			return GL_COPY;
		case MAMUNA_COPY_INVERTED:
			return GL_COPY_INVERTED;
		case MAMUNA_NOOP:
			return GL_NOOP;
		case MAMUNA_INVERT:
			return GL_INVERT;
		case MAMUNA_AND:
			return GL_AND;
		case MAMUNA_NAND:
			return GL_NAND;
		case MAMUNA_OR:
			return GL_OR;
		case MAMUNA_NOR:
			return GL_NOR;
		case MAMUNA_XOR:
			return GL_XOR;
		case MAMUNA_EQUIV:
			return GL_EQUIV;
		case MAMUNA_AND_REVERSE:
			return GL_AND_REVERSE;
		case MAMUNA_AND_INVERTED:
			return GL_AND_INVERTED;
		case MAMUNA_OR_REVERSE:
			return GL_OR_REVERSE;
		case MAMUNA_OR_INVERTED:
			return GL_OR_INVERTED;
		case MAMUNA_FRONT:
			return GL_FRONT;
		case MAMUNA_BACK:
			return GL_BACK;
		case MAMUNA_FRONT_AND_BACK:
			return GL_FRONT_AND_BACK;
		case MAMUNA_CW:
			return GL_CW;
		case MAMUNA_CCW:
			return GL_CCW;
		case MAMUNA_KEEP:
			return GL_KEEP;
		case MAMUNA_REPLACE:
			return GL_REPLACE;
		case MAMUNA_INCR:
			return GL_INCR;
		case MAMUNA_INCR_WRAP:
			return GL_INCR_WRAP;
		case MAMUNA_DECR:
			return GL_DECR;
		case MAMUNA_DECR_WRAP:
			return GL_DECR_WRAP;
		case MAMUNA_POINT:
			return GL_POINT;
		case MAMUNA_LINE:
			return GL_LINE;
		case MAMUNA_FILL:
			return GL_FILL;
	}

	return GL_INVALID_ENUM;
}

bool Mamuna_ShaderIsCompile(GLuint shader){
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success){
		int len;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
		char buff[len];
		glGetShaderInfoLog(shader, len, NULL, buff);
		printf("%s\n", buff);
		return false;
	}
	return true;
}

bool Mamuna_ProgramIsLinked(GLuint prog){
	int success;
	glGetProgramiv(prog, GL_LINK_STATUS, &success);
	if(!success){
		int len;
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
		char buff[len];
		glGetProgramInfoLog(prog, len, NULL, buff);
		printf("%s\n", buff);
		return false;
	}

	return true;
}

Mamuna_Renderer* Mamuna_CreateRenderer(Mamuna_Window* window){
	if(!window) return NULL;
	Mamuna_Renderer* renderer = malloc(sizeof(Mamuna_Renderer));
	renderer->window = (GLFWwindow*)window;

	glfwMakeContextCurrent(renderer->window);

	if(!gladLoadGL(glfwGetProcAddress)){
		return NULL;
	}

	renderer->flags = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
	glCreateBuffers(1, &renderer->RIBO);
	glNamedBufferStorage(renderer->RIBO, sizeof(DRAWCOMMAND) * 10, NULL, renderer->flags);
	renderer->countRIBO = 0;
	renderer->capacityRIBO = 10;
	renderer->pRIBO = glMapNamedBufferRange(renderer->RIBO, 0, sizeof(DRAWCOMMAND) * 10, renderer->flags);


	glCreateFramebuffers(1, &renderer->FBO);

	
	int w, h;
	glfwGetFramebufferSize((GLFWwindow*)window, &w, &h);
	glViewport(0,0,w,h);

	int samples;
	glGetIntegerv(GL_SAMPLES, &samples);

	glCreateRenderbuffers(1, &renderer->RB);


	if(samples != 0){
		glEnable(GL_MULTISAMPLE);
		glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &renderer->windowTexture);
		glTextureStorage2DMultisample(renderer->windowTexture, samples, GL_RGBA8, w, h, GL_TRUE);
		glNamedRenderbufferStorageMultisample(renderer->RB, samples, GL_DEPTH24_STENCIL8, w, h);

	} else {
		glCreateTextures(GL_TEXTURE_2D, 1, &renderer->windowTexture);
		glTextureStorage2D(renderer->windowTexture, 1, GL_RGBA8, w, h);
		glNamedRenderbufferStorage(renderer->RB, GL_DEPTH24_STENCIL8, w, h);
	}


	glTextureParameteri(renderer->windowTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(renderer->windowTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(renderer->windowTexture, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTextureParameteri(renderer->windowTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(renderer->windowTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);




	glNamedFramebufferTexture(renderer->FBO, GL_COLOR_ATTACHMENT0, renderer->windowTexture, 0);
	glNamedFramebufferRenderbuffer(renderer->FBO, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderer->RB);

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glNamedFramebufferDrawBuffers(renderer->FBO, 1, drawBuffers);

	renderer->currentShader = NULL;
	renderer->currentTextures = NULL;
	renderer->texturesCount = 0;
	renderer->mode = GL_TRIANGLES;

	renderer->currentW = w;
	renderer->currentH = h;

	renderer->fence = NULL;

	return renderer;
}

void Mamuna_DestroyRenderer(Mamuna_Renderer* renderer){
	if(!renderer) return;
	renderer->window = NULL;

	glDeleteTextures(1, &renderer->windowTexture);
	glDeleteFramebuffers(1, &renderer->FBO);
	glDeleteRenderbuffers(1, &renderer->RB);
	glUnmapNamedBuffer(renderer->RIBO);
	glDeleteBuffers(1, &renderer->RIBO);


	free(renderer);
	renderer = NULL;
}

void Mamuna_FlushRenderer(Mamuna_Renderer* renderer){
	if(!renderer) return;
	if(renderer->countRIBO == 0){
		return;
	}



	Mamuna_SyncGPU(renderer);
	{
		GLint max;
		glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &max);
		for(GLint i = 0; i < max; ++i){
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i, 0);
		}
	}
	{
		GLint max;
		glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &max);
		for(GLint i = 0; i < max; ++i){
			glBindBufferBase(GL_UNIFORM_BUFFER, i, 0);
		}
	}
	{
		GLint max;
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max);
		for(GLint i = 0; i < max; ++i){
			glBindTextureUnit(i, 0);
		}
	}
	GLenum type = GL_UNSIGNED_INT;
	if(renderer->currentVAO){
		glBindVertexArray(renderer->currentVAO->vao);
		type = renderer->currentVAO->type;
	}

	if(renderer->currentShader){
		glUseProgram(renderer->currentShader->shader);
		{
			size_t count;
			Mamuna_Buffer** tmp = Mamuna_HashmapGetAll(renderer->currentShader->ssbos, &count);
			for(size_t i = 0; i < count; ++i){
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i, tmp[i]->buffer);
			}
		}
		{
			size_t count;
			Mamuna_Buffer** tmp = Mamuna_HashmapGetAll(renderer->currentShader->ubos, &count);
			for(size_t i = 0; i < count; ++i){
				glBindBufferBase(GL_UNIFORM_BUFFER, i, tmp[i]->buffer);
			}
		}

	}

	if(renderer->texturesCount && renderer->currentTextures){
		for(size_t i = 0; i < renderer->texturesCount; ++i){
			glBindTextureUnit(i, renderer->currentTextures[i]->texture);
		}
	}

	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, renderer->RIBO);
	glBindFramebuffer(GL_FRAMEBUFFER, renderer->FBO);

	glViewport(0,0, renderer->currentW, renderer->currentH);
	
	glMultiDrawElementsIndirect(renderer->mode, type, 0, renderer->countRIBO, sizeof(DRAWCOMMAND));	
	
	renderer->countRIBO = 0;

	renderer->fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

}

void Mamuna_SetRenderTargets(Mamuna_Renderer* renderer, size_t amount, Mamuna_Texture** textures){
	if(!renderer) return;
	Mamuna_FlushRenderer(renderer);
	Mamuna_SyncGPU(renderer);
	GLint max;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &max);
	for(size_t i = 0; i < max; ++i){
		glNamedFramebufferTexture(renderer->FBO, GL_COLOR_ATTACHMENT0 + i, 0, 0);
	}

	
	if(amount == 0 || !textures){
		glNamedFramebufferTexture(renderer->FBO, GL_COLOR_ATTACHMENT0, renderer->windowTexture, 0);
		int w, h;
		glfwGetFramebufferSize((GLFWwindow*)renderer->window, &w, &h);
		renderer->currentW = w;
		renderer->currentH = h;
		GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
		glNamedFramebufferDrawBuffers(renderer->FBO, 1, drawBuffers);
		return;
	}

	GLenum drawBuffers[amount];
	memset(drawBuffers, 0, amount * sizeof(GLenum));
	

	for(size_t i = 0; i < amount && i < max; ++i){
		Mamuna_Texture* tex = textures[i];
		if(!tex) break;
		if(i == 0){
			renderer->currentW = tex->w;
			renderer->currentH = tex->h;
			
		}
		glNamedFramebufferTexture(renderer->FBO, GL_COLOR_ATTACHMENT0 + i, tex->texture, 0);
		drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	
	glNamedFramebufferDrawBuffers(renderer->FBO, amount, drawBuffers);
}

void Mamuna_ClearRenderer(Mamuna_Renderer* renderer, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	if(!renderer) return;
	Mamuna_FlushRenderer(renderer);
	Mamuna_SyncGPU(renderer);

	glBindFramebuffer(GL_FRAMEBUFFER, renderer->FBO);

	glClearColor((float)r / 255.f, (float)g / 255.f, (float)b / 255.f, (float)a / 255.f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
			GL_STENCIL_BUFFER_BIT);
}

void Mamuna_RenderPresent(Mamuna_Renderer* renderer){
	if(!renderer) return;
	Mamuna_FlushRenderer(renderer);

	Mamuna_SyncGPU(renderer);

	
	int w,h;
	glfwGetFramebufferSize((GLFWwindow*)renderer->window, &w, &h);

	glBlitNamedFramebuffer(renderer->FBO, 0,
			0, 0,
			renderer->currentW, renderer->currentH,
			0, 0,
			w, h,
			GL_COLOR_BUFFER_BIT,
			GL_NEAREST
			);

	renderer->fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

	glfwSwapBuffers(renderer->window);

	
}

void Mamuna_SetVAO(Mamuna_Renderer* renderer, Mamuna_VAO* vao){
	if(!renderer) return;
	Mamuna_FlushRenderer(renderer);
	Mamuna_SyncGPU(renderer);
	renderer->currentVAO = !vao || !glIsVertexArray(vao->vao) ? NULL : vao;
}

void Mamuna_SetShader(Mamuna_Renderer* renderer, Mamuna_Shader* shader){
	if(!renderer) return;
	Mamuna_FlushRenderer(renderer);
	Mamuna_SyncGPU(renderer);

	renderer->currentShader = shader;
}

void Mamuna_SetTextures(Mamuna_Renderer* renderer, size_t amount, Mamuna_Texture** textures){
	if(!renderer) return;
	Mamuna_FlushRenderer(renderer);

	Mamuna_SyncGPU(renderer);

	if(amount == 0 || !textures){
		GLint max;
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max);
		for(GLint i = 0; i < max; ++i){
			glBindTextureUnit(i, 0);
		}
		renderer->texturesCount = 0;
		renderer->currentTextures = textures;

		return;
	}


	renderer->texturesCount = amount;
	renderer->currentTextures = textures;

}



void Mamuna_SetModeDraw(Mamuna_Renderer* renderer, MAMUNA_ENUM type, float pointSize){
	if(!renderer) return;
	GLenum glType = Mamuna_GetGLEnum(type);
	if(glType == GL_INVALID_ENUM) return;
	Mamuna_FlushRenderer(renderer);
	
	renderer->mode = glType;
	glPointSize(pointSize);
}

void Mamuna_DrawVertexes(Mamuna_Renderer* renderer, size_t countIndices, size_t instances, size_t firstIndex, size_t prevVertexes, size_t baseInstance){
	if(!renderer) return;
	Mamuna_SyncGPU(renderer);

	if(renderer->countRIBO + 1 >= renderer->capacityRIBO){
		GLuint newBuffer;
		glCreateBuffers(1, &newBuffer);
		glNamedBufferStorage(newBuffer, sizeof(DRAWCOMMAND) * (renderer->capacityRIBO + 10), NULL, renderer->flags);
		void* newPtr = glMapNamedBufferRange(newBuffer, 0, sizeof(DRAWCOMMAND) * (renderer->capacityRIBO + 10), renderer->flags);

		memcpy(newPtr, renderer->pRIBO, sizeof(DRAWCOMMAND) * renderer->capacityRIBO);
		glUnmapBuffer(renderer->RIBO);
		glDeleteBuffers(1, &renderer->RIBO);
		renderer->RIBO = newBuffer;
		renderer->pRIBO = newPtr;
		renderer->capacityRIBO += 10;
	}

	DRAWCOMMAND command = {
		countIndices,
		instances,
		firstIndex,
		prevVertexes,
		baseInstance,
	};

	memcpy(renderer->pRIBO + renderer->countRIBO++, &command, sizeof(DRAWCOMMAND));
} 

Mamuna_Shader* Mamuna_CreateShader(Mamuna_Renderer* renderer, const char* format, ...){
	if(!renderer || !format) return NULL;

	size_t count = 0;
	size_t len = strlen(format);

	Mamuna_Vector* vector = Mamuna_CreateVector(sizeof(GLuint), 3);
	for(size_t i = 0; i < len; ++i){
		if(format[i] == ' ') ++i;
		if(format[i++] == '%') ++count;
		if(i >= len) break;
		switch(format[i]){
			case 'v':
			case 'V':
			{
				GLuint shader = glCreateShader(GL_VERTEX_SHADER);
				Mamuna_VectorPushBack(vector, &shader);
			} break;
			case 'f':
			case 'F':
			{
				GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
				Mamuna_VectorPushBack(vector, &shader);
			} break;
			case 'g':
			case 'G':
			{
				GLuint shader = glCreateShader(GL_GEOMETRY_SHADER);
				Mamuna_VectorPushBack(vector, &shader);
			} break;
			case 'c':
			case 'C':
			{
				GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
				Mamuna_VectorPushBack(vector, &shader);
			} break;
			case 't':
			case 'T':
			{
				if(i + 1 >= len) break;
				switch(format[++i]){
					case 'c':
					case 'C':
					{
						GLuint shader = glCreateShader(GL_TESS_CONTROL_SHADER);
						Mamuna_VectorPushBack(vector, &shader);
					} break;
					case 'e':
					case 'E':
					{
						GLuint shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
						Mamuna_VectorPushBack(vector, &shader);
					} break;
					default:
					{
						printf("Shader Bad Format!!!\n");
					} break;
				}
			} break;

			default:
			{
				printf("Shader Bad Format!!!\n");
			} break;
		}
	}

	size_t countVector;
	GLuint* shaders = Mamuna_DestroyVector(vector, &countVector);
	
	va_list sources;
	va_start(sources, format);

	GLuint program = glCreateProgram();

	for(size_t i = 0; i < countVector; ++i){
		const char* source = va_arg(sources, char*);
		glShaderSource(shaders[i], 1, &source, NULL);
		glCompileShader(shaders[i]);

		if(Mamuna_ShaderIsCompile(shaders[i])){
			glAttachShader(program, shaders[i]);
		}
	}

	va_end(sources);

	glLinkProgram(program);
	for(size_t i = 0; i < countVector; ++i){
		glDeleteShader(shaders[i]);
	}
	free(shaders);
	if(!Mamuna_ProgramIsLinked(program)){
		glDeleteProgram(program);
		return NULL;
	}

	Mamuna_Shader* shader = calloc(1, sizeof(Mamuna_Shader));
	shader->shader = program;
	shader->ssbos = Mamuna_CreateHashmap(sizeof(Mamuna_Buffer*), 5);
	shader->ubos = Mamuna_CreateHashmap(sizeof(Mamuna_Buffer*), 5);

	return shader;
}

void Mamuna_DestroyShader(Mamuna_Shader* shader){
	if(!shader) return;
	size_t count;
	free(Mamuna_DestroyHashmap(shader->ssbos, &count));
	free(Mamuna_DestroyHashmap(shader->ubos, &count));

	glDeleteProgram(shader->shader);
	free(shader);
	shader = NULL;
}

Mamuna_Buffer* Mamuna_CreateBuffer(Mamuna_Renderer* renderer, size_t bytesPerElement, size_t countToIncrease){
	if(!renderer || !bytesPerElement || !countToIncrease) return NULL;
	
	GLuint buffer;
	glCreateBuffers(1, &buffer);

	size_t capacity = countToIncrease * bytesPerElement;

	glNamedBufferStorage(buffer, capacity, NULL, renderer->flags);
	void* ptr = glMapNamedBufferRange(buffer, 0, capacity, renderer->flags);

	Mamuna_Buffer* Mbuffer = calloc(1, sizeof(Mamuna_Buffer));
	Mbuffer->buffer = buffer;
	Mbuffer->ptr = ptr;
	Mbuffer->capacity = capacity;
	Mbuffer->bytesPerElement = bytesPerElement;
	Mbuffer->countToIncrease = countToIncrease;

	return Mbuffer;
}

void Mamuna_ShaderAttachBuffer(Mamuna_Shader* shader, Mamuna_Buffer** buffer, const char* nameBuffer, MAMUNA_ENUM type){
	if(!buffer || !shader || !nameBuffer) return;
	if(!glIsBuffer(buffer[0]->buffer) ||
		!glIsProgram(shader->shader)) return;

	GLenum glType = Mamuna_GetGLEnum(type);

	GLuint index = glGetProgramResourceIndex(shader->shader, glType, nameBuffer);

	switch(type){
		case MAMUNA_BUFFER_SSBO:
		{
			GLint max;
			glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &max);
			if(Mamuna_HashmapGetCount(shader->ssbos) >= max) break;

			glShaderStorageBlockBinding(shader->shader, index, Mamuna_HashmapGetCount(shader->ssbos));
			Mamuna_HashmapAddElement(shader->ssbos, nameBuffer, buffer);
		} break;
		case MAMUNA_BUFFER_UBO:
		{
			GLint max;
			glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &max);
			if(Mamuna_HashmapGetCount(shader->ubos) >= max) break;

			glUniformBlockBinding(shader->shader, index, Mamuna_HashmapGetCount(shader->ubos));
			Mamuna_HashmapAddElement(shader->ubos, nameBuffer, buffer);
		} break;
	}


}

void Mamuna_ShaderDetachBuffer(Mamuna_Renderer* renderer, 
		Mamuna_Shader* shader, const char* nameBuffer, MAMUNA_ENUM type){
	if(!shader || !nameBuffer) return;
	if(!glIsProgram(shader->shader)) return;

	Mamuna_SyncGPU(renderer);

	Mamuna_Buffer** tmp = Mamuna_HashmapRemoveElement(type == MAMUNA_BUFFER_SSBO ? shader->ssbos :
			type == MAMUNA_BUFFER_UBO ? shader->ubos : NULL, nameBuffer);
	if(!tmp) return;

	free(tmp);
}

void Mamuna_DestroyBuffer(Mamuna_Buffer* buffer){
	if(!buffer) return;

	glUnmapNamedBuffer(buffer->buffer);
	glDeleteBuffers(1, &buffer->buffer);

	free(buffer);
	buffer = NULL;
}


bool Mamuna_BufferSetElement(Mamuna_Renderer* renderer, Mamuna_Buffer* buffer, size_t index, size_t count, void* element){
	if(!renderer || !buffer || !glIsBuffer(buffer->buffer)) return false;

	Mamuna_SyncGPU(renderer);

	size_t sum = index + count;
	if(index > sum) return false;
	bool resize = false;
	if(sum >= buffer->capacity){
		GLuint newBuffer;
		glCreateBuffers(1, &newBuffer);
		
		size_t newCapacity = buffer->capacity + (sum - buffer->capacity);
		if(newCapacity < buffer->capacity) return false;
		glNamedBufferStorage(newBuffer, newCapacity * buffer->bytesPerElement, NULL, renderer->flags);
		void* ptr = glMapNamedBufferRange(newBuffer, 0, newCapacity * buffer->bytesPerElement, renderer->flags);

		memcpy(ptr, buffer->ptr, buffer->bytesPerElement * buffer->capacity);

		glUnmapNamedBuffer(buffer->buffer);
		glDeleteBuffers(1, &buffer->buffer);
		
		buffer->capacity = newCapacity;
		buffer->buffer = newBuffer;
		buffer->ptr = ptr;
		resize = true;
	}
	if(!buffer->ptr) return false;

	memcpy(buffer->ptr + (buffer->bytesPerElement * index), element, buffer->bytesPerElement * count);

	return resize;
}

void Mamuna_BufferClearRange(Mamuna_Renderer* renderer, Mamuna_Buffer* buffer, size_t offset, size_t range){
	if(!renderer || !buffer || !glIsBuffer(buffer->buffer) || !range) return;
	if(!buffer->ptr) return;
	if((offset + range) >= buffer->capacity) return;

	Mamuna_SyncGPU(renderer);
	
	memset(buffer->ptr + (offset * buffer->bytesPerElement), 0, (range * buffer->bytesPerElement));
}

void* Mamuna_BufferGetElement(Mamuna_Renderer* renderer, Mamuna_Buffer* buffer, size_t index){
	if(!renderer || !buffer || !glIsBuffer(buffer->buffer)) return NULL;
	if(index >= buffer->capacity) return NULL;
	Mamuna_SyncGPU(renderer);
	return buffer->ptr + (buffer->bytesPerElement * index);
}

void Mamuna_BufferClear(Mamuna_Renderer* renderer, Mamuna_Buffer* buffer){
	if(!renderer || !buffer || !glIsBuffer(buffer->buffer)) return;

	Mamuna_SyncGPU(renderer);

	memset(buffer->ptr, 0, buffer->bytesPerElement * buffer->capacity);
}

size_t Mamuna_BufferGetRealSize(Mamuna_Buffer* buffer){
	if(!buffer) return 0;
	return buffer->capacity * buffer->bytesPerElement;	
}

Mamuna_VAO* Mamuna_CreateVAO(Mamuna_Renderer* renderer){
	if(!renderer) return NULL;
	Mamuna_VAO* vao = malloc(sizeof(Mamuna_VAO));
	glCreateVertexArrays(1, &vao->vao);
	vao->type = GL_UNSIGNED_INT;
	return vao;
}

void Mamuna_DestroyVAO(Mamuna_VAO* vao){
	if(!vao) return;
	if(!glIsVertexArray(vao->vao));
	glDeleteVertexArrays(1, &vao->vao);
}

void Mamuna_VAOBindVertexBuffer(Mamuna_VAO* vao, Mamuna_Buffer* vbo, uint32_t index, uint32_t offset, size_t stride, uint32_t divisor){
	if(!vao) return;
	if(!glIsVertexArray(vao->vao)) return;
	if(!vbo || !glIsBuffer(vbo->buffer)){
		glVertexArrayVertexBuffer(vao->vao, index, 0, 0, 16);
		return;
	}
	if(offset < 0) offset = 0;
	if(stride < 1) return;
	int maxVAB;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIB_BINDINGS, &maxVAB);
	if(maxVAB < (index + 1)) return;

	int maxStride;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIB_STRIDE, &maxStride);
	if(maxStride < stride) return;

	glVertexArrayVertexBuffer(vao->vao, index, vbo->buffer, offset, stride);
	glVertexArrayBindingDivisor(vao->vao, index, divisor);
}

void Mamuna_VAOBindElementBuffer(Mamuna_VAO* vao, Mamuna_Buffer* ebo, MAMUNA_ENUM type){
	if(!vao) return;
	if(!glIsVertexArray(vao->vao))return;
	glVertexArrayElementBuffer(vao->vao, !ebo || !glIsBuffer(ebo->buffer) ? 0 : ebo->buffer);
	GLenum glType = Mamuna_GetGLEnum(type);
	if(glType == GL_INVALID_ENUM){
		vao->type = GL_UNSIGNED_INT;
		return;
	}
	vao->type = glType;
}

void Mamuna_VAOVertexAttrib(Mamuna_VAO* vao, uint32_t index, uint32_t countOfValues, MAMUNA_ENUM type, bool normalized, size_t relativeOffset, uint32_t bufferIndex){
	if(!vao) return;
	if(!glIsVertexArray(vao->vao)) return;
	int maxVA;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVA);
	if(index >= maxVA) return;
	int maxRO;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET, &maxRO);
	if(relativeOffset > maxRO) return;

	GLenum glType = Mamuna_GetGLEnum(type);
	if(glType == GL_INVALID_INDEX) return;
	
	switch(type){
		case MAMUNA_FLOAT:
			glVertexArrayAttribFormat(vao->vao, index, countOfValues, glType, normalized ? GL_TRUE : GL_FALSE, relativeOffset);
			break;
		case MAMUNA_INT:
		case MAMUNA_BYTE:
		case MAMUNA_SHORT:
		case MAMUNA_FIXED:
		case MAMUNA_UNSIGNED_BYTE:
		case MAMUNA_UNSIGNED_SHORT:
		case MAMUNA_UNSIGNED_INT:
			glVertexArrayAttribIFormat(vao->vao, index, countOfValues, glType, relativeOffset);
			break;
		case MAMUNA_DOUBLE:
			glVertexArrayAttribLFormat(vao->vao, index, countOfValues, glType, relativeOffset);
			break;
		default:
			return;
	}

	glVertexArrayAttribBinding(vao->vao, index, bufferIndex);
	glEnableVertexArrayAttrib(vao->vao, index);
}

bool Mamuna_CanCreateTexture(MAMUNA_ENUM type, MAMUNA_ENUM format, size_t w, size_t h, size_t d, size_t levels){
	GLenum internal = Mamuna_GetGLEnum(format);
	if(internal == GL_INVALID_ENUM) return false;
	GLenum glType = Mamuna_GetGLEnum(type);
	if(internal == GL_INVALID_ENUM) return false;

	switch(type){
		case MAMUNA_TEXTURE_1D:
			{
				glTexStorage1D(GL_PROXY_TEXTURE_1D, levels, internal, w);
				int width;
				glGetTexLevelParameteriv(GL_PROXY_TEXTURE_1D, 0, GL_TEXTURE_WIDTH, &width);
				return width > 0;
			} break;
		case MAMUNA_TEXTURE_1D_ARRAY:
			{
				glTexStorage2D(GL_PROXY_TEXTURE_1D_ARRAY, levels, internal, w, h);
				int width;
				glGetTexLevelParameteriv(GL_PROXY_TEXTURE_1D_ARRAY, 0, GL_TEXTURE_WIDTH, &width);
				return width > 0;
			} break;
		case MAMUNA_TEXTURE_2D:
			{
				glTexStorage2D(GL_PROXY_TEXTURE_2D, levels, internal, w, h);
				int width;
				glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
				return width > 0;
			} break;
		case MAMUNA_TEXTURE_2D_ARRAY:
			{
				glTexStorage3D(GL_PROXY_TEXTURE_2D_ARRAY, levels, internal, w, h, d);
				int width;
				glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_WIDTH, &width);
				return width > 0;
			} break;
		case MAMUNA_TEXTURE_3D:
			{
				glTexStorage3D(GL_PROXY_TEXTURE_3D, levels, internal, w, h, d);
				int width;
				glGetTexLevelParameteriv(GL_PROXY_TEXTURE_3D, 0, GL_TEXTURE_WIDTH, &width);
				return width > 0;
			} break;
	}

	return false;
}

Mamuna_Texture* Mamuna_CreateTexture(Mamuna_Renderer* renderer, MAMUNA_ENUM type, MAMUNA_ENUM format, size_t w, size_t h, size_t d, size_t levels){
	if(!renderer || !levels) return NULL;
	GLenum internal = Mamuna_GetGLEnum(format);
	if(internal == GL_INVALID_ENUM) return NULL;
	GLenum glType = Mamuna_GetGLEnum(type);
	if(glType == GL_INVALID_ENUM) return NULL;

	if(!Mamuna_CanCreateTexture(type, format, w, h, d, levels)) return NULL;

	Mamuna_Texture* Texture = malloc(sizeof(Mamuna_Texture));
	glCreateTextures(glType, 1, &Texture->texture);
	
	switch(type){
		case MAMUNA_TEXTURE_1D:
			glTextureStorage1D(Texture->texture, levels, internal, w);
			break;
		case MAMUNA_TEXTURE_1D_ARRAY:
		case MAMUNA_TEXTURE_2D:
			glTextureStorage2D(Texture->texture, levels, internal, w, h);
			break;
		case MAMUNA_TEXTURE_2D_ARRAY:
		case MAMUNA_TEXTURE_3D:
			glTextureStorage3D(Texture->texture, levels, internal, w, h, d);
			break;
	}


	Texture->w = w;
	Texture->h = type == MAMUNA_TEXTURE_1D ? 1 : h;
	Texture->d = type != MAMUNA_TEXTURE_3D && type != MAMUNA_TEXTURE_2D_ARRAY ? 1 : d;
	Texture->format = format;
	Texture->levels = levels;
	Texture->type = type;

	return Texture;
}



void Mamuna_DestroyTexture(Mamuna_Texture* texture){
	if(!texture) return;
	glDeleteTextures(1, &texture->texture);
	free(texture);
}

void Mamuna_TextureSetParameteri(Mamuna_Texture* texture, MAMUNA_ENUM param, int value){
	if(!texture) return;
	GLenum glParam = Mamuna_GetGLEnum(param);
	if(glParam == GL_INVALID_ENUM) return;

	switch(param){
		case MAMUNA_PARAM_TEXTURE_DEPTH_STENCIL_MODE:
		case MAMUNA_PARAM_TEXTURE_COMPARE_FUNC:
		case MAMUNA_PARAM_TEXTURE_COMPARE_MODE:
		case MAMUNA_PARAM_TEXTURE_MIN_FILTER:
		case MAMUNA_PARAM_TEXTURE_MAG_FILTER:
		case MAMUNA_PARAM_TEXTURE_SWIZZLE_R:
		case MAMUNA_PARAM_TEXTURE_SWIZZLE_G:
		case MAMUNA_PARAM_TEXTURE_SWIZZLE_B:
		case MAMUNA_PARAM_TEXTURE_SWIZZLE_A:
		case MAMUNA_PARAM_TEXTURE_WRAP_S:
		case MAMUNA_PARAM_TEXTURE_WRAP_T:
		case MAMUNA_PARAM_TEXTURE_WRAP_R:
			{
				GLenum glValue = Mamuna_GetGLEnum(value);
				if(glValue == GL_INVALID_ENUM) return;
				glTextureParameteri(texture->texture, glParam, glValue);
			} break;
		case MAMUNA_PARAM_TEXTURE_MAX_LEVEL:
		case MAMUNA_PARAM_TEXTURE_BASE_LEVEL:
			glTextureParameteri(texture->texture, glParam, value);
			break;
	}
}

void Mamuna_TextureSetParameterf(Mamuna_Texture* texture, MAMUNA_ENUM param, float value){
	if(!texture) return;
	GLenum glParam = Mamuna_GetGLEnum(param);
	if(glParam == GL_INVALID_ENUM) return;

	switch(param){
		case MAMUNA_PARAM_TEXTURE_LOD_BIAS:
		case MAMUNA_PARAM_TEXTURE_MIN_LOD:
		case MAMUNA_PARAM_TEXTURE_MAX_LOD:
			glTextureParameterf(texture->texture, glParam, value);
			break;
	}
}


void Mamuna_TextureLoadImage(Mamuna_Texture* texture, Mamuna_Image* image, int xoffset, int yoffset, int zoffset,
		size_t width, size_t height, size_t depth, size_t level, MAMUNA_ENUM type){
	if(!texture || !image) return;
	GLenum glFormat = Mamuna_GetGLEnum(texture->format);
	if(glFormat == GL_INVALID_ENUM) return;
	if(level >= texture->levels) level = texture->levels - 1;
	if(width > texture->w) width = texture->w;
	if(height > texture->h) height = texture->h;
	if(depth > texture->d) depth = texture->d;

	unsigned char* pixels = Mamuna_GetImagePixels(image);
	int iW, iH;
	Mamuna_GetImageSize(image, &iW, &iH);
	if(iW < width) width = iW;
	if(iH < height) height = iH;

	switch(texture->type){
		case MAMUNA_TEXTURE_1D:
			{
				glTextureSubImage1D(texture->texture, level, xoffset, width, glFormat, GL_UNSIGNED_BYTE, pixels);	
			} break;
		case MAMUNA_TEXTURE_1D_ARRAY:
		case MAMUNA_TEXTURE_2D:
			{
				glTextureSubImage2D(texture->texture, level, xoffset, yoffset, width, height, glFormat, GL_UNSIGNED_BYTE, pixels);
			} break;
		case MAMUNA_TEXTURE_2D_ARRAY:
		case MAMUNA_TEXTURE_3D:
			{
				glTextureSubImage3D(texture->texture, level, xoffset, yoffset, zoffset, width, height, depth, glFormat, GL_UNSIGNED_BYTE, pixels);
			} break;
		default:
			break;
	}
}

void Mamuna_GetTextureSize(Mamuna_Texture* texture, int* w, int* h, int* d, int* levels){
	if(!texture) return;
	*w = texture->w;
	*h = texture->h;
	*d = texture->d;
	*levels = texture->levels;
}

MAMUNA_ENUM Mamuna_GetTextureFormat(Mamuna_Texture* texture){
	if(!texture) return MAMUNA_INVALID_ENUM;
	return texture->format;
}

MAMUNA_ENUM Mamuna_GetTextureType(Mamuna_Texture* texture){
	if(!texture) return MAMUNA_INVALID_ENUM;
	return texture->type;
}

void Mamuna_Enable(Mamuna_Renderer* renderer, MAMUNA_ENUM opt){
	if(!renderer) return;
	GLenum glopt = Mamuna_GetGLEnum(opt);
	glEnable(glopt);
}

void Mamuna_Disable(Mamuna_Renderer* renderer, MAMUNA_ENUM opt){
	if(!renderer) return;
	GLenum glopt = Mamuna_GetGLEnum(opt);
	glDisable(glopt);
}

void Mamuna_PolygonOffset(Mamuna_Renderer* renderer, float factor, float units, float clamp){
	if(!renderer) return;
	glPolygonOffsetClamp(factor, units, clamp);
}

void Mamuna_BlendFunc(Mamuna_Renderer* renderer, MAMUNA_ENUM sfactor, MAMUNA_ENUM dfactor){
	if(!renderer) return;
	GLenum glSfactor = Mamuna_GetGLEnum(sfactor);
	GLenum glDfactor = Mamuna_GetGLEnum(dfactor);
	glBlendFunc(glSfactor, glDfactor);
}

void Mamuna_ColorLogicOp(Mamuna_Renderer* renderer, MAMUNA_ENUM opcode){
	if(!renderer) return;
	GLenum glOp = Mamuna_GetGLEnum(opcode);
	glLogicOp(glOp);
}

void Mamuna_CullFace(Mamuna_Renderer* renderer, MAMUNA_ENUM mode){
	if(!renderer) return;
	GLenum glMode = Mamuna_GetGLEnum(mode);
	glCullFace(glMode);
}

void Mamuna_FrontFace(Mamuna_Renderer* renderer, MAMUNA_ENUM mode){
	if(!renderer) return;
	GLenum glMode = Mamuna_GetGLEnum(mode);
	glFrontFace(glMode);
}

void Mamuna_DepthRange(Mamuna_Renderer* renderer, float near, float far){
	if(!renderer) return;
	glDepthRangef(near, far);
}

void Mamuna_DepthFunc(Mamuna_Renderer* renderer, MAMUNA_ENUM func){
	if(!renderer) return;
	GLenum glFunc = Mamuna_GetGLEnum(func);
	glDepthFunc(glFunc);
}

void Mamuna_LineWidth(Mamuna_Renderer* renderer, float width){
	if(!renderer) return;
	glLineWidth(width);
}

void Mamuna_Scissor(Mamuna_Renderer* renderer, int x, int y, size_t width, size_t height){
	if(!renderer) return;
	glScissor(x, y, width, height);
}

void Mamuna_StencilFunc(Mamuna_Renderer* renderer, MAMUNA_ENUM func, int ref, uint32_t mask){
	if(!renderer) return;
	GLenum glFunc = Mamuna_GetGLEnum(func);
	glStencilFunc(glFunc, ref, mask);
}

void Mamuna_StencilOp(Mamuna_Renderer* renderer, MAMUNA_ENUM sFail, MAMUNA_ENUM dpFail, MAMUNA_ENUM dpPass){
	if(!renderer) return;
	GLenum glsFail = Mamuna_GetGLEnum(sFail);
	GLenum gldpFail = Mamuna_GetGLEnum(dpFail);
	GLenum gldpPass = Mamuna_GetGLEnum(dpPass);
	glStencilOp(glsFail, gldpFail, gldpPass);
}

void Mamuna_DispatchCompute(Mamuna_Renderer* renderer, uint32_t num_Gr_x, uint32_t num_Gr_y, uint32_t num_Gr_z){
	if(!renderer) return;

	Mamuna_SyncGPU(renderer);
	{
		GLint max;
		glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &max);
		for(GLint i = 0; i < max; ++i){
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i, 0);
		}
	}
	{
		GLint max;
		glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &max);
		for(GLint i = 0; i < max; ++i){
			glBindBufferBase(GL_UNIFORM_BUFFER, i, 0);
		}
	}
	{
		GLint max;
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max);
		for(GLint i = 0; i < max; ++i){
			glBindTextureUnit(i, 0);
		}
	}

	if(renderer->currentShader){
		glUseProgram(renderer->currentShader->shader);
		{
			size_t count;
			Mamuna_Buffer** tmp = Mamuna_HashmapGetAll(renderer->currentShader->ssbos, &count);
			for(size_t i = 0; i < count; ++i){
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i, tmp[i]->buffer);
			}
		}
		{
			size_t count;
			Mamuna_Buffer** tmp = Mamuna_HashmapGetAll(renderer->currentShader->ubos, &count);
			for(size_t i = 0; i < count; ++i){
				glBindBufferBase(GL_UNIFORM_BUFFER, i, tmp[i]->buffer);
			}
		}

	}

	if(renderer->texturesCount && renderer->currentTextures){
		for(size_t i = 0; i < renderer->texturesCount; ++i){
			glBindTextureUnit(i, renderer->currentTextures[i]->texture);
		}
	}

	glDispatchCompute(num_Gr_x, num_Gr_y, num_Gr_z);

	renderer->fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
}

void Mamuna_PolygonMode(Mamuna_Renderer* renderer, MAMUNA_ENUM mode){
	if(!renderer) return;
	GLenum glMode = Mamuna_GetGLEnum(mode);

	glPolygonMode(GL_FRONT_AND_BACK, glMode);
}
