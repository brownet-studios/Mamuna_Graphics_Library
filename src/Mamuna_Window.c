#include <glad/gl.h>
#define GLFW_STATIC
#include <GLFW/glfw3.h>
#include <Mamuna.h>

typedef struct Mamuna_Window {
	GLFWwindow* base;
} Mamuna_Window;

static bool* keys = NULL;
static Mamuna_Button buttons = 0;
static double xScroll = 0.0;
static double yScroll = 0.0;
static unsigned int character = 0;
static vec4 rect = {-1.f};


int Mamuna_GetGLFWEnum(MAMUNA_ENUM hint){
	switch(hint){
		case MAMUNA_HINT_VISIBLE:
			return GLFW_VISIBLE;
		case MAMUNA_HINT_DECORATED:
			return GLFW_DECORATED;
		case MAMUNA_HINT_AUTO_ICONIFY:
			return GLFW_AUTO_ICONIFY;
		case MAMUNA_HINT_FLOATING:
			return GLFW_FLOATING;
		case MAMUNA_HINT_MAXIMIZED:
			return GLFW_MAXIMIZED;
		case MAMUNA_HINT_CENTER_CURSOR:
			return GLFW_CENTER_CURSOR;
		case MAMUNA_HINT_TRANSPARENT_FRAMEBUFFER:
			return GLFW_TRANSPARENT_FRAMEBUFFER;
		case MAMUNA_HINT_FOCUS_ON_SHOW:
			return GLFW_FOCUS_ON_SHOW;
		case MAMUNA_HINT_SCALE_TO_MONITOR:
			return GLFW_SCALE_TO_MONITOR;
		case MAMUNA_HINT_RED_BITS:
			return GLFW_RED_BITS;
		case MAMUNA_HINT_GREEN_BITS:
			return GLFW_GREEN_BITS;
		case MAMUNA_HINT_BLUE_BITS:
			return GLFW_BLUE_BITS;
		case MAMUNA_HINT_ALPHA_BITS:
			return GLFW_ALPHA_BITS;
		case MAMUNA_HINT_DEPTH_BITS:
			return GLFW_DEPTH_BITS;
		case MAMUNA_HINT_STENCIL_BITS:
			return GLFW_STENCIL_BITS;
		case MAMUNA_HINT_ACCUM_RED_BITS:
			return GLFW_ACCUM_RED_BITS;
		case MAMUNA_HINT_ACCUM_GREEN_BITS:
			return GLFW_ACCUM_GREEN_BITS;
		case MAMUNA_HINT_ACCUM_BLUE_BITS:
			return GLFW_ACCUM_BLUE_BITS;
		case MAMUNA_HINT_ACCUM_ALPHA_BITS:
			return GLFW_ACCUM_ALPHA_BITS;
		case MAMUNA_HINT_AUX_BUFFERS:
			return GLFW_AUX_BUFFERS;
		case MAMUNA_HINT_SAMPLES:
			return GLFW_SAMPLES;
		case MAMUNA_HINT_REFRESH_RATE:
			return GLFW_REFRESH_RATE;
		case MAMUNA_HINT_STEREO:
			return GLFW_STEREO;
		case MAMUNA_HINT_SRGB_CAPABLE:
			return GLFW_SRGB_CAPABLE;
		case MAMUNA_HINT_OPENGL_PROFILE:
			return GLFW_OPENGL_PROFILE;
		case MAMUNA_ANY_POSITION:
			return GLFW_ANY_POSITION;
		case MAMUNA_DONT_CARE:
			return GLFW_DONT_CARE;
		case MAMUNA_OPENGL_ANY_PROFILE:
			return GLFW_OPENGL_ANY_PROFILE;
		case MAMUNA_OPENGL_COMPAT_PROFILE:
			return GLFW_OPENGL_COMPAT_PROFILE;
		case MAMUNA_OPENGL_CORE_PROFILE:
			return GLFW_OPENGL_CORE_PROFILE;
	}
	return 10000;
}

bool Mamuna_Init(){
	keys = calloc(MAMUNA_KEY_LAST + 1, sizeof(bool));
	return glfwInit();
}

void Mamuna_Quit(){
	free(keys);
	glfwTerminate();
}

int Mamuna_GetError(const char** error){
	return glfwGetError(error);	
}


void Mamuna_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if(key == GLFW_KEY_UNKNOWN);

	if(action == GLFW_PRESS){
		keys[key] = true;	
	} else if(action == GLFW_RELEASE){
		keys[key] = false;
	}

}

void Mamuna_CharacterCallback(GLFWwindow* window, unsigned int codepoint){
	character = codepoint;	
}

void Mamuna_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods){

	if(action == GLFW_PRESS){
		buttons |= (1 << button);
	} else if(action == GLFW_RELEASE){
		buttons &= ~(1 << button);
	}
}

void Mamuna_MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset){
	xScroll = xoffset;
	yScroll = yoffset;
}

void Mamuna_MouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos){
	//if(glfwGetWindowAttrib(window, GLFW_FOCUSED)) return;

	double newX = xpos;
	double newY = ypos;

	if(rect[0] >= 0.f && newX <= rect[0]){
		newX = rect[0];
	} else if(rect[2] >= 0.f && newX >= rect[2]){
		newX = rect[2];
	} if(rect[1] >= 0.f && newY <= rect[1]){
		newY = rect[1];
	} else if(rect[3] >= 0.f && newY >= rect[3]){
		newY = rect[3];
	}

	if(newX != xpos || newY != ypos){
		glfwSetCursorPos(window, newX, newY);
	}
}

void Mamuna_SetWindowCreationHint(MAMUNA_ENUM pname, int value){
	int gPname = Mamuna_GetGLFWEnum(pname);	
	int gValue = Mamuna_GetGLFWEnum(value);

	switch(gPname){
		case GLFW_VISIBLE:
		case GLFW_DECORATED:
		case GLFW_FOCUSED:
		case GLFW_AUTO_ICONIFY:
		case GLFW_FLOATING:
		case GLFW_MAXIMIZED:
		case GLFW_CENTER_CURSOR:
		case GLFW_TRANSPARENT_FRAMEBUFFER:
		case GLFW_FOCUS_ON_SHOW:
		case GLFW_SCALE_TO_MONITOR:
		case GLFW_SCALE_FRAMEBUFFER:
		case GLFW_MOUSE_PASSTHROUGH:
		case GLFW_STEREO:
		case GLFW_SRGB_CAPABLE:
			glfwWindowHint(gPname, value ? GLFW_TRUE : GLFW_FALSE);
			break;
		case GLFW_POSITION_X:
		case GLFW_POSITION_Y:
			glfwWindowHint(gPname, gValue == GLFW_ANY_POSITION ? gValue : value);
			break;
		case GLFW_RED_BITS:
		case GLFW_GREEN_BITS:
		case GLFW_BLUE_BITS:
		case GLFW_ALPHA_BITS:
		case GLFW_DEPTH_BITS:
		case GLFW_STENCIL_BITS:
		case GLFW_ACCUM_RED_BITS:
		case GLFW_ACCUM_GREEN_BITS:
		case GLFW_ACCUM_BLUE_BITS:
		case GLFW_ACCUM_ALPHA_BITS:
		case GLFW_AUX_BUFFERS:
		case GLFW_SAMPLES:
		case GLFW_REFRESH_RATE:
			glfwWindowHint(gPname, gValue == GLFW_DONT_CARE ? gValue : value);
			break;
		case GLFW_OPENGL_PROFILE:
			glfwWindowHint(gPname, gValue);
			break;
		default:
			break;

	}
}

Mamuna_Window* Mamuna_CreateWindow(int width, int height, const char* title, bool fullscreen){

	GLFWmonitor* monitor = NULL;
	if(fullscreen){
		monitor = glfwGetPrimaryMonitor();
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);


	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(width, height, title, monitor, NULL);

	if(!window){
		glfwTerminate();
		return NULL;
	}

	glfwSetKeyCallback(window, Mamuna_KeyCallback);
	glfwSetMouseButtonCallback(window, Mamuna_MouseButtonCallback);
	glfwSetScrollCallback(window, Mamuna_MouseScrollCallback);
	glfwSetCharCallback(window, Mamuna_CharacterCallback);

	return (Mamuna_Window*)window;
}

void Mamuna_DestroyWindow(Mamuna_Window* window){
	if(!window) return;
	GLFWwindow* win = (GLFWwindow*)window;

	glfwDestroyWindow(win);

	
}

void Mamuna_PollEvents(){
	xScroll = 0;
	yScroll = 0;
	character = 0;
	glfwPollEvents();
}

bool Mamuna_IsRunning(Mamuna_Window* window){
	return !glfwWindowShouldClose((GLFWwindow*)window);
}

bool Mamuna_WindowIsActive(Mamuna_Window* window){
	return glfwGetWindowAttrib((GLFWwindow*)window, GLFW_FOCUSED);
}

const bool* Mamuna_GetKeyboardState(){
	return keys;
}

Mamuna_Button Mamuna_GetMouseState(Mamuna_Window* window, double* x, double* y,
		double* xoffset, double* yoffset){
	glfwGetCursorPos((GLFWwindow*)window, x, y);
	*xoffset = xScroll;
	*yoffset = yScroll;

	return buttons;
}

void Mamuna_SetVSync(int mode){
	glfwSwapInterval(mode);
}

float Mamuna_GetTime(){
	return (float)glfwGetTime();
}

void Mamuna_SetMouseRect(Mamuna_Window* window, vec4 Rect){
	glm_vec4_copy(Rect, rect);

	glfwSetCursorPosCallback((GLFWwindow*)window, Mamuna_MouseCursorPosCallback);
}

uint64_t Mamuna_GetPerformanceCounter(){
	return glfwGetTimerValue();
}

uint64_t Mamuna_GetPerformanceFrequency(){
	return glfwGetTimerFrequency();
}

unsigned int Mamuna_GetCharacterInput(){
	return character;
}
