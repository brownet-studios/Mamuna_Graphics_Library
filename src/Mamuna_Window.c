#include <glad/gl.h>
#define GLFW_STATIC
#include <GLFW/glfw3.h>
#include <Mamuna.h>

typedef struct Mamuna_Window {
	GLFWwindow* base;
} Mamuna_Window;

bool* keys = NULL;
Mamuna_Button buttons = 0;
double xScroll = 0.0;
double yScroll = 0.0;
unsigned int character = 0;
vec4 rect = {-1.f};

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
	buttons &= ~buttons;
	if(action == GLFW_PRESS){
		buttons |= (1 << button);
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

Mamuna_Window* Mamuna_CreateWindow(int width, int height, const char* title, bool fullscreen, const char* version, bool profileCore){

	GLFWmonitor* monitor = NULL;
	if(fullscreen){
		monitor = glfwGetPrimaryMonitor();
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version[0] - 48);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version[1] - 48);
	glfwWindowHint(GLFW_OPENGL_PROFILE, profileCore ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);

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
