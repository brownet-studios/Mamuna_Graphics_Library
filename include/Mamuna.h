#ifndef MAMUNA_H
#define MAMUNA_H



#define MAMUNA_KEY_SPACE 32
#define MAMUNA_KEY_APOSTROPHE 39
#define MAMUNA_KEY_COMMA 44
#define MAMUNA_KEY_MINUS 45
#define MAMUNA_KEY_PERIOD 46
#define MAMUNA_KEY_SLASH 47
#define MAMUNA_KEY_0 48
#define MAMUNA_KEY_1 49
#define MAMUNA_KEY_2 50
#define MAMUNA_KEY_3 51
#define MAMUNA_KEY_4 52
#define MAMUNA_KEY_5 53
#define MAMUNA_KEY_6 54
#define MAMUNA_KEY_7 55
#define MAMUNA_KEY_8 56
#define MAMUNA_KEY_9 57
#define MAMUNA_KEY_SEMICOLON 59
#define MAMUNA_KEY_EQUAL 61
#define MAMUNA_KEY_A 65
#define MAMUNA_KEY_B 66
#define MAMUNA_KEY_C 67
#define MAMUNA_KEY_D 68
#define MAMUNA_KEY_E 69
#define MAMUNA_KEY_F 70
#define MAMUNA_KEY_G 71
#define MAMUNA_KEY_H 72
#define MAMUNA_KEY_I 73
#define MAMUNA_KEY_J 74
#define MAMUNA_KEY_K 75
#define MAMUNA_KEY_L 76
#define MAMUNA_KEY_M 77
#define MAMUNA_KEY_N 78
#define MAMUNA_KEY_O 79
#define MAMUNA_KEY_P 80
#define MAMUNA_KEY_Q 81
#define MAMUNA_KEY_R 82
#define MAMUNA_KEY_S 83
#define MAMUNA_KEY_T 84
#define MAMUNA_KEY_U 85
#define MAMUNA_KEY_V 86
#define MAMUNA_KEY_W 87
#define MAMUNA_KEY_X 88
#define MAMUNA_KEY_Y 89
#define MAMUNA_KEY_Z 90
#define MAMUNA_KEY_LEFT_BRACKET 91
#define MAMUNA_KEY_BACKSLASH 92
#define MAMUNA_KEY_RIGHT_BRACKET 93
#define MAMUNA_KEY_GRAVE_ACCENT 96
#define MAMUNA_KEY_WORLD_1 161
#define MAMUNA_KEY_WORLD_2 162
#define MAMUNA_KEY_ESCAPE 256
#define MAMUNA_KEY_ENTER 257
#define MAMUNA_KEY_TAB 258
#define MAMUNA_KEY_BACKSPACE 259
#define MAMUNA_KEY_INSERT 260
#define MAMUNA_KEY_DELETE 261
#define MAMUNA_KEY_RIGHT 262
#define MAMUNA_KEY_LEFT 263
#define MAMUNA_KEY_DOWN 264
#define MAMUNA_KEY_UP 265
#define MAMUNA_KEY_PAGE_UP 266
#define MAMUNA_KEY_PAGE_DOWN 267
#define MAMUNA_KEY_HOME 268
#define MAMUNA_KEY_END 269
#define MAMUNA_KEY_CAPS_LOCK 280
#define MAMUNA_KEY_SCROLL_LOCK 281
#define MAMUNA_KEY_NUM_LOCK 282
#define MAMUNA_KEY_PRINT_SCREEN 283
#define MAMUNA_KEY_PAUSE 284
#define MAMUNA_KEY_F1 290
#define MAMUNA_KEY_F2 291
#define MAMUNA_KEY_F3 292
#define MAMUNA_KEY_F4 293
#define MAMUNA_KEY_F5 294
#define MAMUNA_KEY_F6 295
#define MAMUNA_KEY_F7 296
#define MAMUNA_KEY_F8 297
#define MAMUNA_KEY_F9 298
#define MAMUNA_KEY_F10 299
#define MAMUNA_KEY_F11 300
#define MAMUNA_KEY_F12 301
#define MAMUNA_KEY_F13 302
#define MAMUNA_KEY_F14 303
#define MAMUNA_KEY_F15 304
#define MAMUNA_KEY_F16 305
#define MAMUNA_KEY_F17 306
#define MAMUNA_KEY_F18 307
#define MAMUNA_KEY_F19 308
#define MAMUNA_KEY_F20 309
#define MAMUNA_KEY_F21 310
#define MAMUNA_KEY_F22 311
#define MAMUNA_KEY_F23 312
#define MAMUNA_KEY_F24 313
#define MAMUNA_KEY_F25 314
#define MAMUNA_KEY_KP_0 320
#define MAMUNA_KEY_KP_1 321
#define MAMUNA_KEY_KP_2 322
#define MAMUNA_KEY_KP_3 323
#define MAMUNA_KEY_KP_4 324
#define MAMUNA_KEY_KP_5 325
#define MAMUNA_KEY_KP_6 326
#define MAMUNA_KEY_KP_7 327
#define MAMUNA_KEY_KP_8 328
#define MAMUNA_KEY_KP_9 329
#define MAMUNA_KEY_KP_DECIMAL 339
#define MAMUNA_KEY_KP_DIVIDE 331
#define MAMUNA_KEY_KP_MULTIPLY 332
#define MAMUNA_KEY_KP_SUBTRACT 333
#define MAMUNA_KEY_KP_ADD 334
#define MAMUNA_KEY_KP_ENTER 335
#define MAMUNA_KEY_KP_EQUAL 336
#define MAMUNA_KEY_LEFT_SHIFT 340
#define MAMUNA_KEY_LEFT_CONTROL 341
#define MAMUNA_KEY_LEFT_ALT 342
#define MAMUNA_KEY_LEFT_SUPER 343
#define MAMUNA_KEY_RIGHT_SHIFT 344
#define MAMUNA_KEY_RIGHT_CONTROL 345
#define MAMUNA_KEY_RIGHT_ALT 346
#define MAMUNA_KEY_RIGHT_SUPER 347
#define MAMUNA_KEY_MENU 348
#define MAMUNA_KEY_LAST MAMUNA_KEY_MENU

#include <cglm/cglm.h>

#include <stdbool.h>
#include <stdint.h>
#include <uchar.h>


typedef enum MAMUNA_TYPE_DRAW {
	MAMUNA_POINT,
	MAMUNA_LINE,
	MAMUNA_TRIANGLES,
} MAMUNA_TYPE_DRAW;

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

typedef struct Mamuna_Window Mamuna_Window;

typedef struct Mamuna_Renderer Mamuna_Renderer;

typedef struct Mamuna_Shader Mamuna_Shader;

typedef struct Mamuna_Buffer Mamuna_Buffer;

typedef struct Mamuna_Texture Mamuna_Texture;

typedef struct Mamuna_Image Mamuna_Image;

typedef struct Mamuna_Font Mamuna_Font;

typedef struct Mamuna_Hashmap Mamuna_Hashmap;

typedef struct Mamuna_Vector Mamuna_Vector;

typedef struct VERTEX {
	vec3 pos;
	vec3 normals;
	vec2 uv;
} VERTEX;

bool Mamuna_Init(); //Library Initialization
void Mamuna_Quit(); //Library Deinitialization

Mamuna_Window* Mamuna_CreateWindow(int width, int height, const char* title,
		bool fullscreen, const char* version, bool profileCore); //Create a Window.
									//version is an OpenGL version,
									//profileCore indicates whether we want to use the OpenGL core
									//version or the compatibility version
void Mamuna_DestroyWindow(Mamuna_Window* window); //Destroy a Window
int Mamuna_GetError(const char** error); //This returns the error code and the GLFW error description

void Mamuna_PollEvents(); //This is Port GLFW_PollEvents
bool Mamuna_IsRunning(Mamuna_Window* window); //This returns the truth value when the program is running
bool Mamuna_WindowIsActive(Mamuna_Window* window); //This returns the truth value when window is Active
void Mamuna_SetVSync(int mode); //This set Vsync, 0 is off, 1 is on, this is port from GLFW
void Mamuna_SetMouseRect(Mamuna_Window* window, vec4 Rect); //This sets the area within which the mouse can move

float Mamuna_GetTime(); //This returns the program's execution time
uint64_t Mamuna_GetPerformanceCounter(); //This is port from GLFW_GetPerformanceCounter
uint64_t Mamuna_GetPerformanceFrequency(); //This is port from GLFW_GetPerformanceFrequency

unsigned int Mamuna_GetCharacterInput(); //This returns the character that the user wants to enter.

const bool* Mamuna_GetKeyboardState(); //This returns pointer to keyboardmap, max index is MAMUNA_KEY_LAST
Mamuna_Button Mamuna_GetMouseState(Mamuna_Window* window, double* x, double* y,
		double* xoffset, double* yoffset); //This returns a map of the mouse buttons and their positions relative to the window,
						   //as well as input from the mouse wheel

Mamuna_Renderer* Mamuna_CreateRenderer(Mamuna_Window* window); //Create a renderer opengl
void Mamuna_DestroyRenderer(Mamuna_Renderer* renderer); //Destroy a renderer

void Mamuna_FlushRenderer(Mamuna_Renderer* renderer); //This renders everything that has been sent to the graphics card
void Mamuna_SetRenderTargets(Mamuna_Renderer* renderer, size_t amount, Mamuna_Texture** textures); //This sets a renderTarget to Textures,
											    //if amount is 0 or textures is NULL
											    //this sets a renderTarget to windowTexture,
									//Therefore, it automatically uses a flush at the beginning

void Mamuna_ClearRenderer(Mamuna_Renderer* renderer, uint8_t r,
		uint8_t g, uint8_t b, uint8_t a); //This clears the renderer for a given color (0–255);
						  //Therefore, it automatically uses a flush at the beginning

void Mamuna_RenderPresent(Mamuna_Renderer* renderer); //Update the screen with any rendering performed since the previous call.;
						  //Therefore, it automatically uses a flush at the beginning

void Mamuna_SetTextures(Mamuna_Renderer* renderer, size_t amount, Mamuna_Texture** textures);//This sets the textures to be used;
										      //if there are no textures,
										      //the renderer stops using textures

void Mamuna_SetShader(Mamuna_Renderer* renderer, Mamuna_Shader* shader); //This sets the shader that will handle the rendering;
								   //Therefore, it automatically uses a flush at the beginning
void Mamuna_SetModeDraw(Mamuna_Renderer* renderer, MAMUNA_TYPE_DRAW type, float size); //This sets mode draw, POINT, LINE, TRIANGLES;
								//Therefore, it automatically uses a flush at the beginning

void Mamuna_DrawVertexes(Mamuna_Renderer* renderer,	size_t countVertexes,
		VERTEX* vertexes, size_t countIndices, uint32_t* indices,
		size_t instances); //It tells the graphics card which vertices are to be rendered;
				   //in other words, it issues a draw call.

Mamuna_Shader* Mamuna_CreateShader(Mamuna_Renderer* renderer,
		const char* vs, const char* fs); //This creates a shader consisting of a vertex shader and a fragment shader

void Mamuna_DestroyShader(Mamuna_Shader* shader); //This destroys a shader
//This creates an SSBO (Shader Storage Buffer Object) for your shader
Mamuna_Buffer* Mamuna_BufferCreate(Mamuna_Renderer* renderer, Mamuna_Shader* shader, const char* nameBuffer, size_t bytesPerElement);
//This connects your buffer to another shader, meaning one buffer can be used by multiple shaders
void Mamuna_BufferAlignToOtherShader(Mamuna_Buffer* buffer, Mamuna_Shader* shader, const char* nameBuffer);
//This destroys your buffer
void Mamuna_BufferDestroy(Mamuna_Buffer* buffer);
//This sets the element with the specified index in your buffer
void Mamuna_BufferSetElement(Mamuna_Renderer* renderer, Mamuna_Buffer* buffer, size_t index, void* element);
//This returns the element with the specified index from your buffer
void* Mamuna_BufferGetElement(Mamuna_Buffer* buffer, size_t index);
//This clears your buffer
void Mamuna_BufferClear(Mamuna_Renderer* renderer, Mamuna_Buffer* buffer);

//This creates a texture; “channels” refers to the number of channels you want in the texture; 1 means only the red channel (R8);
//2 means the red and green channels (RG8);
//3 means the red, green, and blue channels (RGB8);
//4 means the red, green, blue, and alpha channels (RGBA8) 
Mamuna_Texture* Mamuna_CreateTexture(int w, int h, bool linear, int channels);
//This creates a texture using a photo
Mamuna_Texture* Mamuna_CreateTextureFromImage(Mamuna_Renderer* renderer, Mamuna_Image* image, bool linear);
//This destroys texture
void Mamuna_DestroyTexture(Mamuna_Texture* texture);
//This copies your texture
Mamuna_Texture* Mamuna_CopyTexture(Mamuna_Texture* texture, bool linear);
//This returns size of your texture
void Mamuna_GetTextureSize(Mamuna_Texture* texture, int* w, int* h);
//This loads Text File; example source of your vertex shader or fragment shader
char* Mamuna_LoadTextFile(const char* path);
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
float Mamuna_FontGetSizeXChar(Mamuna_Font* font, char32_t c);
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
Mamuna_Hashmap* Mamuna_HashmapCreate(size_t bytesPerElement, size_t countToIncrease);
//This destroys hashmap; returns a array of your objects and count; You have to handle their destruction
void* Mamuna_HashmapDestroy(Mamuna_Hashmap* hashmap, size_t* count);
//This returns array of your elements in hashmap and count
void* Mamuna_HashmapGetAll(Mamuna_Hashmap* hashmap, size_t* count);
//This adds your element to hashmap
void Mamuna_HashmapAddElement(Mamuna_Hashmap* hashmap, const char* id, void* element);
//This removes your element with specified id from hashmap; You have to handle destroying it
void* Mamuna_HashmapRemoveElement(Mamuna_Hashmap* hashmap, const char* id);
//This returns your element with specified id
void* Mamuna_HashmapGetElement(Mamuna_Hashmap* hashmap, const char* id);

//This creates vector; countToIncrease by how much should it increase
Mamuna_Vector* Mamuna_VectorCreate(size_t bytesPerElement, size_t countToIncrease);
//This destroys vector; returns a array of your objects and count; You have to handle their destruction
void* Mamuna_VectorDestroy(Mamuna_Vector* vector, size_t* count);
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
#endif
