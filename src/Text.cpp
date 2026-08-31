#include "header/Text.h"

Text::Text() {
	xpos = 0;
	ypos = 0;
	width = 0;
	height = 0;
	texture = NULL;
	innerText = "";
	renderer = NULL;
	font = NULL;
}

Text::~Text() {
	free();
}

void Text::init(int x, int y, TTF_Font* f, SDL_Renderer* r, std::string text, SDL_Color textColor) {
	setPosition(x, y);
	setFont(f);
	setRenderer(r);
	loadText(text, textColor);
}

void Text::setPosition(int x, int y) {
	xpos = x;
	ypos = y;
}

void Text::free() {
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
		texture = NULL;
		xpos = 0;
		ypos = 0;
		width = 0;
		height = 0;
	}
}

void Text::loadText(std::string textToRender, SDL_Color textColor) {
	free();
	if (font == NULL) {
		// Text has no log stream of its own, so SDL's own logging is used
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Text::loadText() was given a NULL font");
		return;
	}
	SDL_Texture* newTexture = NULL;
	SDL_Surface* temp_surface = TTF_RenderText_Solid(font, textToRender.c_str(), textColor);
	if (temp_surface == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Text::loadText() could not render text: %s", TTF_GetError());
		return;
	}
	newTexture = SDL_CreateTextureFromSurface(renderer, temp_surface);
	if (newTexture == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Text::loadText() could not create texture: %s", SDL_GetError());
		SDL_FreeSurface(temp_surface);
		return;
	}
	width = temp_surface->w;
	height = temp_surface->h;
	SDL_FreeSurface(temp_surface);
	texture = newTexture;
}

void Text::setFont(TTF_Font* f) {
	font = f;
}

void Text::render() {
	if (texture == NULL) {
		return;
	}
	SDL_Rect renderQuad = {xpos, ypos, width, height};
	SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
}

void Text::setRenderer(SDL_Renderer* r) {
	renderer = r;
}

int Text::getX() {
	return xpos;
}

int Text::getY() {
	return ypos;
}

int Text::getWidth() {
	return width;
}

int Text::getHeight() {
	return height;
}