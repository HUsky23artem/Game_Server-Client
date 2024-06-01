#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

// Размеры окна
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Цвета
const SDL_Color WHITE = {255, 255, 255, 255};
const SDL_Color YELLOW = {255, 255, 0, 255};

// Структура пункта меню
typedef struct {
char* text;
SDL_Rect rect;
} MenuItem;

int main(int argc, char* args[]) {
	// Инициализация SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Ошибка инициализации: %s\n", SDL_GetError());
		return 1;
	}

	// Создание окна
	SDL_Window* window = SDL_CreateWindow("Пример меню SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		fprintf(stderr, "Ошибка создания окна: %s\n", SDL_GetError());
		return 1;
	}

	// Создание рендера
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		fprintf(stderr, "Ошибка создания рендера: %s\n", SDL_GetError());
		return 1;
	}

	// Инициализация пунктов меню
	MenuItem menuItems[] = {
		{"Пункт 1", {10, 10, 100, 50}},
		{"Пункт 2", {10, 70, 100, 50}},
		{"Пункт 3", {10, 130, 100, 50}}
	};
	const int numMenuItems = sizeof(menuItems) / sizeof(MenuItem);

	// Основной цикл обработки событий
	int quit = 0;
	int selectedItem = 0;
	while (!quit) {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = 1;
			}

			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_UP) {
					selectedItem = (selectedItem - 1 + numMenuItems) % numMenuItems;
				} else if (e.key.keysym.sym == SDLK_DOWN) {
					selectedItem = (selectedItem + 1) % numMenuItems;
				}
			}
		}

		// Очистка экрана
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		// Отрисовка меню
		for (int i = 0; i < numMenuItems; i++) {
			SDL_Color color = (i == selectedItem) ? YELLOW : WHITE;
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
			SDL_RenderFillRect(renderer, &menuItems[i].rect);
		}

		// Обновление экрана
		SDL_RenderPresent(renderer);
	}

	// Освобождение памяти и завершение работы
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
