#include "header/GraphicsEnv.h"

// tors
GraphicsEnv::GraphicsEnv() {
	title = "GraphicsEnv Default Title";
	screenWidth = 640;
	screenHeight = 480;
	fontPath = "src/fonts/lazy.ttf"; // the shipped font, relative to the repository root
	running = false;
}

GraphicsEnv::~GraphicsEnv() { // dtor
	cleanUp();
}

// primary methods
void GraphicsEnv::init() {
	if (window != NULL) {
		// a second init() would reopen the open log and leak the existing window and renderer
		reportError("init() was called while already initialized; call cleanUp() first to reinitialize");
		return;
	}

	log.open("log.txt");
	if (!log.is_open()) {
		reportError("log.txt could not be opened; failures will be reported through SDL_LogError() instead");
	}
	log << "Initializing!" << std::endl;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		reportError(std::string("SDL could not initialize! SDL error: ") + SDL_GetError());
		cleanUp();
		return;
	}
	sdlInitialized = true;

	if (TTF_Init() != 0) {
		reportError(std::string("SDL_ttf could not initialize! TTF error: ") + TTF_GetError());
		cleanUp();
		return;
	}
	ttfInitialized = true;

	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		reportError(std::string("Window could not be created! SDL error: ") + SDL_GetError());
		cleanUp();
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		// SDL's software renderer never matches the accelerated flag, so it is only reachable by asking again without it
		reportError(std::string("Accelerated renderer unavailable, falling back to any available renderer without vsync. SDL error: ") + SDL_GetError());
		renderer = SDL_CreateRenderer(window, -1, 0);
	}
	if (renderer == NULL) {
		reportError(std::string("Renderer could not be created! SDL error: ") + SDL_GetError());
		cleanUp();
		return;
	}

	running = true;
}
	
void GraphicsEnv::loadMedia() {
	if (font != NULL) {
		// a second call replaces the open font; any Text still holding the old pointer must be re-pointed
		TTF_CloseFont(font);
		font = NULL;
	}

	font = TTF_OpenFont(fontPath.c_str(), fontSize); // the path is resolved against the working directory
	if (font == NULL) {
		// the environment stays usable without a font; reporting is enough
		reportError("Font could not be opened from " + fontPath + "! TTF error: " + TTF_GetError());
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

	// only what this environment's init() acquired is released, so destroying an
	// uninitialized environment, or a second call, leaves another one running
	if (ttfInitialized) {
		TTF_Quit(); // reference-counted by SDL_ttf
		ttfInitialized = false;
	}

	if (sdlInitialized) {
		// unlike SDL_Quit(), SDL_QuitSubSystem() is reference-counted
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
		sdlInitialized = false;

		// the last environment out still shuts SDL down fully
		if (SDL_WasInit(SDL_INIT_EVERYTHING) == 0) {
			SDL_Quit();
		}
	}

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
	if (window != NULL) {
		// init() reads the size only once, so an open window is resized to keep getW() truthful
		SDL_SetWindowSize(window, screenWidth, screenHeight);
	}
}

void GraphicsEnv::setScreenHeight(int h) {
	screenHeight = h;
	if (window != NULL) {
		// init() reads the size only once, so an open window is resized to keep getH() truthful
		SDL_SetWindowSize(window, screenWidth, screenHeight);
	}
}

void GraphicsEnv::setTitle(std::string newTitle) {
	title = newTitle;
	if (window != NULL) {
		SDL_SetWindowTitle(window, title.c_str());
	}
}

void GraphicsEnv::setFontSize(int size) {
	fontSize = size;
	if (font != NULL) {
		// the size is fixed when loadMedia() opens the font, so a later change waits for the next loadMedia()
		reportError("setFontSize(" + std::to_string(size) + ") was called after loadMedia(); the open font keeps its size until loadMedia() is called again");
	}
}

void GraphicsEnv::setFontPath(std::string path) {
	fontPath = path;
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

// narrowing an int straight to Uint8 would wrap an out-of-range channel to an unrelated value, so it is clamped instead
static Uint8 clampChannel(int value) {
	if (value < 0) {
		return 0;
	}
	if (value > 0xFF) {
		return 0xFF;
	}
	return (Uint8) value;
}

void GraphicsEnv::setRenderColor(int r, int g, int b, int a) {
	SDL_SetRenderDrawColor(renderer, clampChannel(r), clampChannel(g), clampChannel(b), clampChannel(a));
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

// private methods
void GraphicsEnv::reportError(std::string message) {
	if (log.is_open()) {
		log << message << std::endl;
	}
	else {
		// the log is closed outside init()..cleanUp(), and stays closed if log.txt could not be opened, so SDL's own logging is used;
		// the application category defaults to the INFO priority, so an ERROR message is printed rather than filtered out
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", message.c_str());
	}
}