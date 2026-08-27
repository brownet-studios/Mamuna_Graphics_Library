#include <Mamuna.h>

int main(){
	Mamuna_Init();

	Mamuna_Window* window = Mamuna_CreateWindow(1280, 768, "Mamuna black screen", false, "460", true);
	Mamuna_Renderer* renderer = Mamuna_CreateRenderer(window);

	while(Mamuna_IsRunning(window)){
		Mamuna_ClearRenderer(renderer, 0, 0, 0, 255);

		Mamuna_RenderPresent(renderer);
		Mamuna_PollEvents();
	}

	Mamuna_Quit();

	return 0;
}
