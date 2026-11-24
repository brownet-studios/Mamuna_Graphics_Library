#ifndef MAMUNA_H
#define MAMUNA_H



#define MAMUNA_KEY_SPACE 65
#define MAMUNA_KEY_APOSTROPHE 48 
#define MAMUNA_KEY_COMMA 59
#define MAMUNA_KEY_MINUS 20
#define MAMUNA_KEY_PERIOD 60
#define MAMUNA_KEY_SLASH 61
#define MAMUNA_KEY_0 19
#define MAMUNA_KEY_1 10
#define MAMUNA_KEY_2 11
#define MAMUNA_KEY_3 12
#define MAMUNA_KEY_4 13
#define MAMUNA_KEY_5 14
#define MAMUNA_KEY_6 15
#define MAMUNA_KEY_7 16
#define MAMUNA_KEY_8 17
#define MAMUNA_KEY_9 18
#define MAMUNA_KEY_SEMICOLON 47
#define MAMUNA_KEY_EQUAL 21
#define MAMUNA_KEY_A 38
#define MAMUNA_KEY_B 56
#define MAMUNA_KEY_C 54
#define MAMUNA_KEY_D 40
#define MAMUNA_KEY_E 26
#define MAMUNA_KEY_F 41
#define MAMUNA_KEY_G 42
#define MAMUNA_KEY_H 43
#define MAMUNA_KEY_I 31
#define MAMUNA_KEY_J 44
#define MAMUNA_KEY_K 45
#define MAMUNA_KEY_L 46
#define MAMUNA_KEY_M 58
#define MAMUNA_KEY_N 57
#define MAMUNA_KEY_O 32
#define MAMUNA_KEY_P 33
#define MAMUNA_KEY_Q 24
#define MAMUNA_KEY_R 27
#define MAMUNA_KEY_S 39
#define MAMUNA_KEY_T 28
#define MAMUNA_KEY_U 30
#define MAMUNA_KEY_V 55
#define MAMUNA_KEY_W 25
#define MAMUNA_KEY_X 53
#define MAMUNA_KEY_Y 29
#define MAMUNA_KEY_Z 52
#define MAMUNA_KEY_LEFT_BRACKET 34
#define MAMUNA_KEY_BACKSLASH 51
#define MAMUNA_KEY_RIGHT_BRACKET 35
#define MAMUNA_KEY_GRAVE_ACCENT 49
#define MAMUNA_KEY_ESCAPE 9
#define MAMUNA_KEY_RETURN 36
#define MAMUNA_KEY_TAB 23
#define MAMUNA_KEY_BACKSPACE 22
#define MAMUNA_KEY_INSERT 118
#define MAMUNA_KEY_DELETE 119
#define MAMUNA_KEY_RIGHT 114
#define MAMUNA_KEY_LEFT 113
#define MAMUNA_KEY_DOWN 116
#define MAMUNA_KEY_UP 111
#define MAMUNA_KEY_PAGE_UP 112
#define MAMUNA_KEY_PAGE_DOWN 117
#define MAMUNA_KEY_HOME 110
#define MAMUNA_KEY_END 115
#define MAMUNA_KEY_CAPS_LOCK 66
#define MAMUNA_KEY_SCROLL_LOCK 78
#define MAMUNA_KEY_NUM_LOCK 77
#define MAMUNA_KEY_PRINT_SCREEN 107
#define MAMUNA_KEY_PAUSE 127
#define MAMUNA_KEY_F1 67
#define MAMUNA_KEY_F2 68
#define MAMUNA_KEY_F3 69
#define MAMUNA_KEY_F4 70
#define MAMUNA_KEY_F5 71
#define MAMUNA_KEY_F6 72
#define MAMUNA_KEY_F7 73
#define MAMUNA_KEY_F8 74
#define MAMUNA_KEY_F9 75
#define MAMUNA_KEY_F10 76
#define MAMUNA_KEY_F11 76
#define MAMUNA_KEY_F12 77
#define MAMUNA_KEY_KP_0 90
#define MAMUNA_KEY_KP_1 87
#define MAMUNA_KEY_KP_2 88
#define MAMUNA_KEY_KP_3 89
#define MAMUNA_KEY_KP_4 83
#define MAMUNA_KEY_KP_5 84
#define MAMUNA_KEY_KP_6 85
#define MAMUNA_KEY_KP_7 79
#define MAMUNA_KEY_KP_8 80
#define MAMUNA_KEY_KP_9 81
#define MAMUNA_KEY_KP_DECIMAL 129
#define MAMUNA_KEY_KP_DIVIDE 106
#define MAMUNA_KEY_KP_MULTIPLY 63
#define MAMUNA_KEY_KP_SUBTRACT 82
#define MAMUNA_KEY_KP_ADD 86
#define MAMUNA_KEY_KP_ENTER 104
#define MAMUNA_KEY_KP_EQUAL 125
#define MAMUNA_KEY_LEFT_SHIFT 50
#define MAMUNA_KEY_LEFT_CONTROL 37
#define MAMUNA_KEY_LEFT_ALT 64
#define MAMUNA_KEY_LEFT_SUPER 133
#define MAMUNA_KEY_RIGHT_SHIFT 62
#define MAMUNA_KEY_RIGHT_CONTROL 105
#define MAMUNA_KEY_RIGHT_ALT 108
#define MAMUNA_KEY_RIGHT_SUPER 134
#define MAMUNA_KEY_MENU 135
#define MAMUNA_KEY_LAST MAMUNA_KEY_MENU

#define MAMUNA_CONTEXT_GL_PROFILE_CORE_BIT (1 << 0)
#define MAMUNA_CONTEXT_GL_PROFILE_COMP_BIT (1 << 1)

#define MAMUNA_CONTEXT_GL_FLAG_DEBUG_BIT (1 << 0)
#define MAMUNA_CONTEXT_GL_FLAG_FORWARD_COMPATIBLE_BIT (1 << 1)

#include <cglm/cglm.h>

#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>
#include <uchar.h>
#include <glad/gl.h>




typedef enum Mamuna_Button {
	MAMUNA_LEFT_BUTTON = (1 << 0),
	MAMUNA_RIGHT_BUTTON = (1 << 1),
	MAMUNA_MIDDLE_BUTTON = (1 << 2),
	MAMUNA_BUTTON_4 = (1 << 3),
	MAMUNA_BUTTON_5 = (1 << 4),
	MAMUNA_BUTTON_6 = (1 << 5),
	MAMUNA_BUTTON_7 = (1 << 6),
	MAMUNA_BUTTON_8 = (1 << 7),
} Mamuna_Button;

typedef enum MAMUNA_ENUM {
	MAMUNA_FORMAT_TEXTURE_DEPTH_COMP32F,
	MAMUNA_FORMAT_TEXTURE_DEPTH_COMP24,
	MAMUNA_FORMAT_TEXTURE_DEPTH_COMP16,
	MAMUNA_FORMAT_TEXTURE_DEPTH32F_STENCIL8,
	MAMUNA_FORMAT_TEXTURE_DEPTH24_STENCIL8,
	MAMUNA_FORMAT_TEXTURE_STENCIL_INDEX8,

	MAMUNA_FORMAT_TEXTURE_R8,
	MAMUNA_FORMAT_TEXTURE_R8_SNORM,
	MAMUNA_FORMAT_TEXTURE_R16,
	MAMUNA_FORMAT_TEXTURE_R16_SNORM,
	MAMUNA_FORMAT_TEXTURE_RG8,
	MAMUNA_FORMAT_TEXTURE_RG8_SNORM,
	MAMUNA_FORMAT_TEXTURE_RG16,
	MAMUNA_FORMAT_TEXTURE_RG16_SNORM,
	MAMUNA_FORMAT_TEXTURE_R3_G3_B2,
	MAMUNA_FORMAT_TEXTURE_RGB4,
	MAMUNA_FORMAT_TEXTURE_RGB5,
	MAMUNA_FORMAT_TEXTURE_RGB8,
	MAMUNA_FORMAT_TEXTURE_RGB8_SNORM,
	MAMUNA_FORMAT_TEXTURE_RGB10,
	MAMUNA_FORMAT_TEXTURE_RGB12,
	MAMUNA_FORMAT_TEXTURE_RGB16_SNORM,
	MAMUNA_FORMAT_TEXTURE_RGBA2,
	MAMUNA_FORMAT_TEXTURE_RGBA4,
	MAMUNA_FORMAT_TEXTURE_RGB5_A1,
	MAMUNA_FORMAT_TEXTURE_RGBA8,
	MAMUNA_FORMAT_TEXTURE_RGBA8_SNORM,
	MAMUNA_FORMAT_TEXTURE_RGB10_A2,
	MAMUNA_FORMAT_TEXTURE_RGB10_A2UI,
	MAMUNA_FORMAT_TEXTURE_RGBA12,
	MAMUNA_FORMAT_TEXTURE_RGBA16,
	MAMUNA_FORMAT_TEXTURE_SRGB8,
	MAMUNA_FORMAT_TEXTURE_SRGB8_ALPHA8,
	MAMUNA_FORMAT_TEXTURE_R16F,
	MAMUNA_FORMAT_TEXTURE_RG16F,
	MAMUNA_FORMAT_TEXTURE_RGB16F,
	MAMUNA_FORMAT_TEXTURE_RGBA16F,
	MAMUNA_FORMAT_TEXTURE_R32F,
	MAMUNA_FORMAT_TEXTURE_RG32F,
	MAMUNA_FORMAT_TEXTURE_RGB32F,
	MAMUNA_FORMAT_TEXTURE_RGBA32F,
	MAMUNA_FORMAT_TEXTURE_R11F_G11F_B10F,
	MAMUNA_FORMAT_TEXTURE_RGB9_E5,
	MAMUNA_FORMAT_TEXTURE_R8I,
	MAMUNA_FORMAT_TEXTURE_R8UI,
	MAMUNA_FORMAT_TEXTURE_R16I,
	MAMUNA_FORMAT_TEXTURE_R16UI,
	MAMUNA_FORMAT_TEXTURE_R32I,
	MAMUNA_FORMAT_TEXTURE_R32UI,
	MAMUNA_FORMAT_TEXTURE_RG8I,
	MAMUNA_FORMAT_TEXTURE_RG8UI,
	MAMUNA_FORMAT_TEXTURE_RG16I,
	MAMUNA_FORMAT_TEXTURE_RG16UI,
	MAMUNA_FORMAT_TEXTURE_RG32I,
	MAMUNA_FORMAT_TEXTURE_RG32UI,
	MAMUNA_FORMAT_TEXTURE_RGB8I,
	MAMUNA_FORMAT_TEXTURE_RGB8UI,
	MAMUNA_FORMAT_TEXTURE_RGB16I,
	MAMUNA_FORMAT_TEXTURE_RGB16UI,
	MAMUNA_FORMAT_TEXTURE_RGB32I,
	MAMUNA_FORMAT_TEXTURE_RGB32UI,
	MAMUNA_FORMAT_TEXTURE_RGBA8I,
	MAMUNA_FORMAT_TEXTURE_RGBA8UI,
	MAMUNA_FORMAT_TEXTURE_RGBA16I,
	MAMUNA_FORMAT_TEXTURE_RGBA16UI,
	MAMUNA_FORMAT_TEXTURE_RGBA32I,
	MAMUNA_FORMAT_TEXTURE_RGBA32UI,

	MAMUNA_PARAM_TEXTURE_DEPTH_STENCIL_MODE,
	MAMUNA_PARAM_TEXTURE_BASE_LEVEL,
	MAMUNA_PARAM_TEXTURE_BORDER_COLOR,
	MAMUNA_PARAM_TEXTURE_COMPARE_FUNC,
	MAMUNA_PARAM_TEXTURE_COMPARE_MODE,
	MAMUNA_PARAM_TEXTURE_LOD_BIAS,
	MAMUNA_PARAM_TEXTURE_MIN_FILTER,
	MAMUNA_PARAM_TEXTURE_MAG_FILTER,
	MAMUNA_PARAM_TEXTURE_MIN_LOD,
	MAMUNA_PARAM_TEXTURE_MAX_LOD,
	MAMUNA_PARAM_TEXTURE_MAX_LEVEL,
	MAMUNA_PARAM_TEXTURE_SWIZZLE_R,
	MAMUNA_PARAM_TEXTURE_SWIZZLE_G,
	MAMUNA_PARAM_TEXTURE_SWIZZLE_B,
	MAMUNA_PARAM_TEXTURE_SWIZZLE_A,
	MAMUNA_PARAM_TEXTURE_WRAP_S,
	MAMUNA_PARAM_TEXTURE_WRAP_T,
	MAMUNA_PARAM_TEXTURE_WRAP_R,

	MAMUNA_FILTER_TEXTURE_NEAREST,
	MAMUNA_FILTER_TEXTURE_LINEAR,
	MAMUNA_FILTER_TEXTURE_MIPMAP_NEAREST_NEAREST,
	MAMUNA_FILTER_TEXTURE_MIPMAP_NEAREST_LINEAR,
	MAMUNA_FILTER_TEXTURE_MIPMAP_LINEAR_NEAREST,
	MAMUNA_FILTER_TEXTURE_MIPMAP_LINEAR_LINEAR,

	MAMUNA_WRAP_TEXTURE_CLAMP_TO_EDGE,
	MAMUNA_WRAP_TEXTURE_CLAMP_TO_BORDER,
	MAMUNA_WRAP_TEXTURE_MIRRORED_REPEAT,
	MAMUNA_WRAP_TEXTURE_REPEAT,
	MAMUNA_WRAP_TEXTURE_MIRROR_CLAMP_TO_EDGE,

	MAMUNA_TEXTURE_1D,
	MAMUNA_TEXTURE_2D,
	MAMUNA_TEXTURE_3D,
	MAMUNA_TEXTURE_1D_ARRAY,
	MAMUNA_TEXTURE_2D_ARRAY,
	MAMUNA_TEXTURE_2D_MULTISAMPLE,
	MAMUNA_TEXTURE_2D_MULTISAMPLE_ARRAY,

	MAMUNA_DEPTH_COMP,
	MAMUNA_STENCIL_INDEX,

	MAMUNA_TEXTURE_COMPARE_REF_TO_TEXTURE,

	MAMUNA_LEQUAL,
	MAMUNA_GEQUAL,
	MAMUNA_LESS,
	MAMUNA_GREATER,
	MAMUNA_EQUAL,
	MAMUNA_NOTEQUAL,
	MAMUNA_ALWAYS,
	MAMUNA_NEVER,

	MAMUNA_BUFFER_UBO,
	MAMUNA_BUFFER_SSBO,

	MAMUNA_POLYGON_POINT,
	MAMUNA_POLYGON_LINE,
	MAMUNA_POLYGON_FILL,

	MAMUNA_DRAW_POINTS,
	MAMUNA_DRAW_LINES,
	MAMUNA_DRAW_LINE_STRIP,
	MAMUNA_DRAW_LINE_LOOP,
	MAMUNA_DRAW_LINE_STRIP_ADJACENCY,
	MAMUNA_DRAW_LINES_ADJACENCY,
	MAMUNA_DRAW_TRIANGLE_STRIP,
	MAMUNA_DRAW_TRIANGLE_FAN,
	MAMUNA_DRAW_TRIANGLES,
	MAMUNA_DRAW_TRIANGLE_STRIP_ADJACENCY,
	MAMUNA_DRAW_TRIANGLES_ADJACENCY,
	MAMUNA_DRAW_PATCHES,

	MAMUNA_INVALID_ENUM,
	MAMUNA_ZERO,
	MAMUNA_RED,
	MAMUNA_GREEN,
	MAMUNA_BLUE,
	MAMUNA_ALPHA,
	MAMUNA_ONE,

	MAMUNA_FLOAT,
	MAMUNA_BYTE,
	MAMUNA_SHORT,
	MAMUNA_INT,
	MAMUNA_FIXED,
	MAMUNA_DOUBLE,
	MAMUNA_UNSIGNED_BYTE,
	MAMUNA_UNSIGNED_SHORT,
	MAMUNA_UNSIGNED_INT,

	MAMUNA_BLEND,
	MAMUNA_COLOR_LOGIC_OP,
	MAMUNA_CULL_FACE,
	MAMUNA_DEPTH_CLAMP,
	MAMUNA_DEPTH_TEST,
	MAMUNA_DITHER,
	MAMUNA_FRAMEBUFFER_SRGB,
	MAMUNA_LINE_SMOOTH,
	MAMUNA_POLYGON_OFFSET_FILL,
	MAMUNA_POLYGON_OFFSET_LINE,
	MAMUNA_POLYGON_OFFSET_POINT,
	MAMUNA_POLYGON_SMOOTH,
	MAMUNA_PRIMITIVE_RESTART,
	MAMUNA_PRIMITIVE_RESTART_FIXED_INDEX,
	MAMUNA_SCISSOR_TEST,
	MAMUNA_STENCIL_TEST,
	MAMUNA_PROGRAM_POINT_SIZE,

	MAMUNA_SRC_COLOR,
	MAMUNA_ONE_MINUS_SRC_COLOR,
	MAMUNA_DST_COLOR,
	MAMUNA_ONE_MINUS_DST_COLOR,
	MAMUNA_SRC_ALPHA,
	MAMUNA_ONE_MINUS_SRC_ALPHA,
	MAMUNA_DST_ALPHA,
	MAMUNA_ONE_MINUS_DST_ALPHA,
	MAMUNA_CONSTANT_COLOR,
	MAMUNA_ONE_MINUS_CONSTANT_COLOR,
	MAMUNA_CONSTANT_ALPHA,
	MAMUNA_ONE_MINUS_CONSTANT_ALPHA,
	MAMUNA_SRC_ALPHA_SATURATE,
	MAMUNA_SRC1_COLOR,
	MAMUNA_ONE_MINUS_SRC1_COLOR,
	MAMUNA_SRC1_ALPHA,

	MAMUNA_CLEAR,
	MAMUNA_SET,
	MAMUNA_COPY,
	MAMUNA_COPY_INVERTED,
	MAMUNA_NOOP,
	MAMUNA_INVERT,
	MAMUNA_AND,
	MAMUNA_NAND,
	MAMUNA_OR,
	MAMUNA_NOR, 
	MAMUNA_XOR,
	MAMUNA_EQUIV,
	MAMUNA_AND_REVERSE,
	MAMUNA_AND_INVERTED,
	MAMUNA_OR_REVERSE,
	MAMUNA_OR_INVERTED,

	MAMUNA_FRONT,
	MAMUNA_BACK,
	MAMUNA_FRONT_AND_BACK,

	MAMUNA_CW,
	MAMUNA_CCW,

	MAMUNA_KEEP,
	MAMUNA_REPLACE,
	MAMUNA_INCR,
	MAMUNA_INCR_WRAP,
	MAMUNA_DECR,
	MAMUNA_DECR_WRAP,

	MAMUNA_POINT,
	MAMUNA_LINE,
	MAMUNA_FILL,

	MAMUNA_IS_NOT_EXIST,
	MAMUNA_SUCCESS,
	MAMUNA_ERROR,
	
	MAMUNA_WIN_DEPTH_BUFFER_SIZE_HINT,
	MAMUNA_WIN_FB_LEVEL_HINT,

	MAMUNA_WIN_STEREO_HINT,
	MAMUNA_WIN_RED_SIZE_HINT,
	MAMUNA_WIN_GREEN_SIZE_HINT,
	MAMUNA_WIN_BLUE_SIZE_HINT,
	MAMUNA_WIN_ALPHA_SIZE_HINT,
	MAMUNA_WIN_DEPTH_SIZE_HINT,
	MAMUNA_WIN_STENCIL_SIZE_HINT,
	MAMUNA_WIN_SAMPLE_BUFFERS_HINT,
	MAMUNA_WIN_SAMPLES_HINT,
	MAMUNA_WIN_TRANSPARENT_TYPE_HINT,
	MAMUNA_WIN_TRANSPARENT_RED_VALUE_HINT,
	MAMUNA_WIN_TRANSPARENT_GREEN_VALUE_HINT,
	MAMUNA_WIN_TRANSPARENT_BLUE_VALUE_HINT,
	MAMUNA_WIN_TRANSPARENT_ALPHA_VALUE_HINT,
	

	MAMUNA_WIN_POS_X_SIZE_HINT,
	MAMUNA_WIN_POS_Y_SIZE_HINT,
	MAMUNA_WIN_WIDTH_SIZE_HINT,
	MAMUNA_WIN_HEIGHT_SIZE_HINT,
	MAMUNA_WIN_MIN_ASPECT_X_SIZE_HINT,
	MAMUNA_WIN_MIN_ASPECT_Y_SIZE_HINT,
	MAMUNA_WIN_MAX_ASPECT_X_SIZE_HINT,
	MAMUNA_WIN_MAX_ASPECT_Y_SIZE_HINT,
	MAMUNA_WIN_BASE_W_SIZE_HINT,
	MAMUNA_WIN_BASE_H_SIZE_HINT,
	MAMUNA_WIN_GRAVITY_SIZE_HINT,
	
	MAMUNA_CONTEXT_GL_MAJOR_VERSION_HINT,
	MAMUNA_CONTEXT_GL_MINOR_VERSION_HINT,
	MAMUNA_CONTEXT_GL_PROFILE_MASK_HINT,
	MAMUNA_CONTEXT_GL_FLAGS_HINT,

	MAMUNA_NONE,
	MAMUNA_TRANSPARENT_RGB,
} MAMUNA_ENUM;

typedef struct Mamuna_Window Mamuna_Window;
typedef struct Mamuna_Context Mamuna_Context;

typedef struct Mamuna_Renderer Mamuna_Renderer;

typedef struct Mamuna_VAO Mamuna_VAO;

typedef struct Mamuna_Shader Mamuna_Shader;

typedef struct Mamuna_Buffer Mamuna_Buffer;

typedef struct Mamuna_Texture Mamuna_Texture;

typedef struct Mamuna_Image Mamuna_Image;

typedef struct Mamuna_Font Mamuna_Font;

typedef struct Mamuna_Hashmap Mamuna_Hashmap;
typedef struct Mamuna_Hashmap_Int Mamuna_Hashmap_Int;

typedef struct Mamuna_Vector Mamuna_Vector;

bool Mamuna_Init(); //Library Initialization
void Mamuna_Quit(); //Library Deinitialization

void Mamuna_SetCreationHint(MAMUNA_ENUM hint, int value);

Mamuna_Window* Mamuna_CreateWindow(const char* title, bool fullscreen); 
void Mamuna_DestroyWindow(Mamuna_Window* window);

MAMUNA_ENUM Mamuna_MoveWindow(Mamuna_Window* window, int x, int y);
MAMUNA_ENUM Mamuna_RenameWindow(Mamuna_Window* window, const char* name);

void Mamuna_WindowGetSize(Mamuna_Window* window, int* width, int* height);

void Mamuna_PollEvents();
void Mamuna_WindowSwapBuffers(Mamuna_Window* window);
bool Mamuna_IsRunning();
bool Mamuna_WindowShouldClose(Mamuna_Window* window);
bool Mamuna_WindowIsActive(Mamuna_Window* window);

uint64_t Mamuna_GetPerformanceFrequency();
uint64_t Mamuna_GetPerformanceCounter();

Mamuna_Context* Mamuna_CreateContext(Mamuna_Window* window);
void Mamuna_DestroyContext(Mamuna_Context* context);
MAMUNA_ENUM Mamuna_MakeCurrent(Mamuna_Window* window, Mamuna_Context* context);
GLADapiproc Mamuna_GetProcAddress(const char* name);

void Mamuna_SetSwapInterval(Mamuna_Window* window, int mode);

void Mamuna_EnableDebugOutput(Mamuna_Renderer* renderer);
void Mamuna_DisableDebugOutput(Mamuna_Renderer* renderer);
Mamuna_Renderer* Mamuna_CreateRenderer(Mamuna_Window* window); //Create a renderer opengl
void Mamuna_DestroyRenderer(Mamuna_Renderer* renderer); //Destroy a renderer
void Mamuna_RendererChangeWindow(Mamuna_Renderer* renderer, Mamuna_Window* window);

void Mamuna_FlushRenderer(Mamuna_Renderer* renderer); //This renders everything that has been sent to the graphics card
void Mamuna_SetRenderTargets(Mamuna_Renderer* renderer, size_t amount, Mamuna_Texture** textures); //This sets a renderTarget to Textures,
											    //if amount is 0 or textures is NULL
											    //this sets a renderTarget to windowTexture,
									//Therefore, it automatically uses a flush at the beginning

void Mamuna_ClearRenderer(Mamuna_Renderer* renderer, uint8_t r,
		uint8_t g, uint8_t b, uint8_t a); //This clears the renderer for a given color (0–255);
						  //Therefore, it automatically uses a flush at the beginning

void Mamuna_RenderPresent(Mamuna_Renderer* renderer, Mamuna_Window* window); //Update the screen with any rendering performed since the previous call.;
						  //Therefore, it automatically uses a flush at the beginning
void Mamuna_SetVAO(Mamuna_Renderer* renderer, Mamuna_VAO* vao);

void Mamuna_SetTextures(Mamuna_Renderer* renderer, size_t amount, Mamuna_Texture** textures);//This sets the textures to be used;
										      //if there are no textures,
										      //the renderer stops using textures

void Mamuna_SetShader(Mamuna_Renderer* renderer, Mamuna_Shader* shader); //This sets the shader that will handle the rendering;
								   //Therefore, it automatically uses a flush at the beginning
void Mamuna_SetModeDraw(Mamuna_Renderer* renderer, MAMUNA_ENUM type, float pointSize); //This sets mode draw, POINT, LINE, TRIANGLES;
								//Therefore, it automatically uses a flush at the beginning

void Mamuna_DrawVertexes(Mamuna_Renderer* renderer, size_t countIndices, size_t instances, size_t firstIndex,
		size_t prevVertexes, size_t baseInstance); //It tells the graphics card which vertices are to be rendered;
				   //in other words, it issues a draw call.

Mamuna_Shader* Mamuna_CreateShader(Mamuna_Renderer* renderer, const char* format, ...);

void Mamuna_DestroyShader(Mamuna_Shader* shader); //This destroys a shader
//This creates an SSBO (Shader Storage Buffer Object) or UBO (Uniform Buffer Object) for your shader
Mamuna_Buffer* Mamuna_CreateBuffer(Mamuna_Renderer* renderer, size_t bytesPerElement, size_t countToIncrease);
//This connects your buffer to another shader, meaning one buffer can be used by multiple shaders
void Mamuna_ShaderAttachBuffer(Mamuna_Shader* shader, Mamuna_Buffer** buffer, const char* nameBuffer, MAMUNA_ENUM type);

void Mamuna_ShaderDetachBuffer(Mamuna_Renderer* renderer, 
		Mamuna_Shader* shader, const char* nameBuffer, MAMUNA_ENUM type);
//This destroys your buffer
void Mamuna_DestroyBuffer(Mamuna_Buffer* buffer);
//This sets the element with the specified index in your buffer
bool Mamuna_BufferSetElement(Mamuna_Renderer* renderer, Mamuna_Buffer* buffer, size_t index, size_t count, void* element);
//This returns the element with the specified index from your buffer
void* Mamuna_BufferGetElement(Mamuna_Renderer* renderer, Mamuna_Buffer* buffer, size_t index);
//This clears your buffer
void Mamuna_BufferClearRange(Mamuna_Renderer* renderer, Mamuna_Buffer* buffer, size_t offset, size_t range);
void Mamuna_BufferClear(Mamuna_Renderer* renderer, Mamuna_Buffer* buffer);
size_t Mamuna_BufferGetRealSize(Mamuna_Buffer* buffer);

Mamuna_VAO* Mamuna_CreateVAO(Mamuna_Renderer* renderer);
void Mamuna_DestroyVAO(Mamuna_VAO* vao);
void Mamuna_VAOBindVertexBuffer(Mamuna_VAO* vao, Mamuna_Buffer* vbo, uint32_t index, uint32_t offset, size_t stride, uint32_t divisor);
void Mamuna_VAOBindElementBuffer(Mamuna_VAO* vao, Mamuna_Buffer* ebo, MAMUNA_ENUM type);
void Mamuna_VAOVertexAttrib(Mamuna_VAO* vao, uint32_t index, uint32_t countOfValues, MAMUNA_ENUM type, bool normalized, size_t relativeOffset, uint32_t bufferIndex);

Mamuna_Texture* Mamuna_CreateTexture(Mamuna_Renderer* renderer, MAMUNA_ENUM type, MAMUNA_ENUM format, size_t w, size_t h, size_t d, size_t levelsOrSamples);
//This destroys texture
void Mamuna_DestroyTexture(Mamuna_Texture* texture);
void Mamuna_TextureSetParameteri(Mamuna_Texture* texture, MAMUNA_ENUM param, int value);
void Mamuna_TextureSetParameterf(Mamuna_Texture* texture, MAMUNA_ENUM param, float value);
void Mamuna_TextureLoadImage(Mamuna_Texture* texture, Mamuna_Image* image, int xoffset, int yoffset, int zoffset,
		size_t width, size_t height, size_t depth, size_t level, MAMUNA_ENUM type);
//This returns size of your texture
void Mamuna_GetTextureSize(Mamuna_Texture* texture, int* w, int* h, int* d, int* levels);
MAMUNA_ENUM Mamuna_GetTextureFormat(Mamuna_Texture* texture);
MAMUNA_ENUM Mamuna_GetTextureType(Mamuna_Texture* texture);

void Mamuna_Enable(Mamuna_Renderer* renderer, MAMUNA_ENUM opt);
void Mamuna_Disable(Mamuna_Renderer* renderer, MAMUNA_ENUM opt);
void Mamuna_PolygonOffset(Mamuna_Renderer* renderer, float factor, float units, float clamp);
void Mamuna_PrimitiveRestartIndex(Mamuna_Renderer* renderer, uint32_t index);
void Mamuna_BlendFunc(Mamuna_Renderer* renderer, MAMUNA_ENUM sfactor, MAMUNA_ENUM dfactor);
void Mamuna_ColorLogicOp(Mamuna_Renderer* renderer, MAMUNA_ENUM opcode);
void Mamuna_CullFace(Mamuna_Renderer* renderer, MAMUNA_ENUM mode);
void Mamuna_FrontFace(Mamuna_Renderer* renderer, MAMUNA_ENUM mode);
void Mamuna_DepthRange(Mamuna_Renderer* renderer, float near, float far);
void Mamuna_DepthFunc(Mamuna_Renderer* renderer, MAMUNA_ENUM func);
void Mamuna_LineWidth(Mamuna_Renderer* renderer, float width);
void Mamuna_Scissor(Mamuna_Renderer* renderer, int x, int y, size_t width, size_t height);
void Mamuna_StencilFunc(Mamuna_Renderer* renderer, MAMUNA_ENUM func, int ref, uint32_t mask);
void Mamuna_StencilOp(Mamuna_Renderer* renderer, MAMUNA_ENUM sFail, MAMUNA_ENUM dpFail, MAMUNA_ENUM dpPass);
void Mamuna_PolygonMode(Mamuna_Renderer* renderer, MAMUNA_ENUM mode);

void Mamuna_DispatchCompute(Mamuna_Renderer* renderer, uint32_t num_Gr_x, uint32_t num_Gr_y, uint32_t num_Gr_z);

//This loads Text File; example source of your vertex shader or fragment shader
char* Mamuna_LoadFile(const char* path, size_t* bytes);
//This loads image file from memory
Mamuna_Image* Mamuna_LoadImageFromMem(const unsigned char* buffer, size_t bytes);
//This loads an image
Mamuna_Image* Mamuna_LoadImage(const char* path);
//This unloads an image
void Mamuna_UnloadImage(Mamuna_Image* image);
//This loads a font
Mamuna_Font* Mamuna_LoadFont(const char* path, int pixelHeight, size_t countChars);
//This destroys font
void Mamuna_DestroyFont(Mamuna_Font* font);
//This returns font bitmap in the form of a image; image has one channel RED
Mamuna_Image* Mamuna_FontGetBitmap(Mamuna_Font* font);
//This returns the UV coordinates of your character in the font bitmap and adds the character's x-axis
//size to cursorX so that the text displays correctly,
//and returns the positions so that the text displays correctly
void Mamuna_FontGetUVsChar(Mamuna_Font* font, char32_t c, float* cursorX, float sizeFont,
		float baselineY, vec4 uv, vec4 pos);
//This returns the x-dimension of your character
float Mamuna_FontGetSizeXChar(Mamuna_Font* font, char32_t c, float sizeFont);
//This returns sizeYfont
int Mamuna_FontGetSizeYFont(Mamuna_Font* font, float sizeFont);
//This creates image; example You're building a tilemap editor that also needs to save the texture in the map file,
//so you have to load it correctly as well
Mamuna_Image* Mamuna_CreateImage(unsigned char* pixels, int w, int h, int channels);
//This returns in w and h, image size
void Mamuna_GetImageSize(Mamuna_Image* image, int* w, int *h);
//This returns num of channels in image
int Mamuna_GetImageChannels(Mamuna_Image* image);
//This returns pixels from image
unsigned char* Mamuna_GetImagePixels(Mamuna_Image* image);

//This creates hashmap; example A scene must contain objects that you can reference by their IDs.
//countToIncrease by how much should it increase
Mamuna_Hashmap* Mamuna_CreateHashmap(size_t bytesPerElement, size_t countToIncrease);
//This destroys hashmap; returns a array of your objects and count; You have to handle their destruction
void* Mamuna_DestroyHashmap(Mamuna_Hashmap* hashmap, size_t* count);
//This returns array of your elements in hashmap and count
void* Mamuna_HashmapGetAll(Mamuna_Hashmap* hashmap, size_t* count);
//This adds your element to hashmap
void Mamuna_HashmapAddElement(Mamuna_Hashmap* hashmap, const char* id, void* element);
//This removes your element with specified id from hashmap; You have to handle destroying it
void* Mamuna_HashmapRemoveElement(Mamuna_Hashmap* hashmap, const char* id);
//This returns your element with specified id
void* Mamuna_HashmapGetElement(Mamuna_Hashmap* hashmap, const char* id);
//This returns count of elements in hashmap
size_t Mamuna_HashmapGetCount(Mamuna_Hashmap* hashmap);

Mamuna_Hashmap_Int* Mamuna_CreateHashmapInt(size_t bytesPerElement, size_t countToIncrease);
void* Mamuna_DestroyHashmapInt(Mamuna_Hashmap_Int* hashmap, size_t* count);
void* Mamuna_HashmapIntGetAll(Mamuna_Hashmap_Int* hashmap, size_t* count);
void Mamuna_HashmapIntAddElement(Mamuna_Hashmap_Int* hashmap, size_t id, void* element);
void* Mamuna_HashmapIntRemoveElement(Mamuna_Hashmap_Int* hashmap, size_t id);
void* Mamuna_HashmapIntGetElement(Mamuna_Hashmap_Int* hashmap, size_t id);
size_t Mamuna_HashmapIntGetCount(Mamuna_Hashmap_Int* hashmap);

//This creates vector; countToIncrease by how much should it increase
Mamuna_Vector* Mamuna_CreateVector(size_t bytesPerElement, size_t countToIncrease);
//This destroys vector; returns a array of your objects and count; You have to handle their destruction
void* Mamuna_DestroyVector(Mamuna_Vector* vector, size_t* count);
//This returns array of your elements in vector and count
void* Mamuna_VectorGetAll(Mamuna_Vector* vector, size_t* count);
//This adds your element to end of array
void Mamuna_VectorPushBack(Mamuna_Vector* vector, void* element);
//This removes and returns element from end of array
void* Mamuna_VectorPopBack(Mamuna_Vector* vector);
//This adds your element to start of array
void Mamuna_VectorPushFront(Mamuna_Vector* vector, void* element);
//This removes and returns element from start of array
void* Mamuna_VectorPopFront(Mamuna_Vector* vector);
//This return pointer to front of array
void* Mamuna_VectorFront(Mamuna_Vector* vector);
//This return pointer to back of array
void* Mamuna_VectorBack(Mamuna_Vector* vector);
//This returns count of elements in vector
size_t Mamuna_VectorGetCount(Mamuna_Vector* vector);
#endif
