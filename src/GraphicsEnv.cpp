#include "header/GraphicsEnv.h"

// tors
GraphicsEnv::GraphicsEnv() {
	title = "GraphicsEnv Default Title";
	screenWidth = 640;
	screenHeight = 480;
	running = false;
}

GraphicsEnv::~GraphicsEnv() { // dtor
	cleanUp();
}

// primary methods
void GraphicsEnv::init() {          
	log.open("log.txt");
	log << "Initializing!" << std::endl;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		log << "SDL could not initialize! SDL error: " << SDL_GetError() << std::endl;
		cleanUp();
		return;
	}

	if (TTF_Init() != 0) {
		log << "SDL_ttf could not initialize! TTF error: " << TTF_GetError() << std::endl;
		cleanUp();
		return;
	}

	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		log << "Window could not be created! SDL error: " << SDL_GetError() << std::endl;
		cleanUp();
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		log << "Renderer could not be created! SDL error: " << SDL_GetError() << std::endl;
		cleanUp();
		return;
	}

	running = true;
}
	
void GraphicsEnv::loadMedia() {
	font = TTF_OpenFont("objects/bboron.ttf", fontSize); // credit: https://www.1001fonts.com/arial-fonts.html
	if (font == NULL) {
		// the environment stays usable without a font; reporting is enough
		if (log.is_open()) {
			log << "Font is NULL! TTF error: " << TTF_GetError() << std::endl;
		}
	}
}

void GraphicsEnv::cleanUp() {
	if (log.is_open()) {
		log << "Cleaning up!" << std::endl;
	}

	if (font != NULL) {
		TTF_CloseFont(font);
		font = NULL;
	}

	if (renderer != NULL) {
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}

	if (window != NULL) {
		SDL_DestroyWindow(window);
		window = NULL;
	}

	// both are safe to call when the subsystem was never initialized, and
	// SDL_Quit() is documented as safe after a failed SDL_Init()
	TTF_Quit();
	SDL_Quit();

	running = false;

	if (log.is_open()) {
		log.close();
	}
}

// secondary methods
bool GraphicsEnv::isRunning() {
	return running;
}

bool GraphicsEnv::pollEvent() {
	return SDL_PollEvent(&event);
}

SDL_Event* GraphicsEnv::getEvent() {
	return &event;
}

// setters 
void GraphicsEnv::setRunning(bool b) {
	running = b;
}

void GraphicsEnv::setScreenWidth(int w) {
	screenWidth = w;
}

void GraphicsEnv::setScreenHeight(int h) {
	screenHeight = h;
}

void GraphicsEnv::setTitle(std::string newTitle) {
	title = newTitle;
}

void GraphicsEnv::setFontSize(int size) {
	fontSize = size;
}

// getters
int GraphicsEnv::getW() {
	return screenWidth;
}

int GraphicsEnv::getH() {
	return screenHeight;
}

SDL_Renderer* GraphicsEnv::getRenderer() {
	return renderer;
}

TTF_Font* GraphicsEnv::getFont() {
	return font;
}

// rendering methods
void GraphicsEnv::setRenderColor(int r, int g, int b, int a) {
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void GraphicsEnv::clear() {
	SDL_RenderClear(renderer);
}

void GraphicsEnv::present() {
	SDL_RenderPresent(renderer);
}

// drawing methods
void GraphicsEnv::drawRectangle(int x, int y, int w, int h) {
	SDL_Rect fillRect = {x, y, w, h};		
	SDL_RenderFillRect(renderer, &fillRect); // draw color using rect
}