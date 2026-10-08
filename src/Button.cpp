#include "header/Button.h"

Button::Button() {
	xpos = 0;
	ypos = 0;
	width = 0;
	height = 0;
	renderer = NULL;
}

void Button::init(int x, int y, int w, int h, SDL_Renderer* r, void (*funcMouseDown)()) {
	xpos = x;
	ypos = y;
	width = w;
	height = h;
	setRenderer(r);
	mouseDownFunc = funcMouseDown;
}

void Button::setRenderer(SDL_Renderer* r) {
	renderer = r;
}

void Button::render() {
	SDL_Rect fillRect = {xpos, ypos, width, height};
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
	SDL_RenderFillRect(renderer, &fillRect);
}

void Button::handleEvent(SDL_Event* e) {
	// if mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP) {
		// get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);
		
		// check if mouse is in button
		bool inside = true;
		
		// if mouse is left of the button
		if (x < xpos) {
			inside = false;
		}
		
		// if mouse is right of the button
		else if (x > xpos + width) {
			inside = false;
		}
		
		// if mouse is above the button
		else if (y < ypos) {
			inside = false;
		}
		
		// if mouse is below the button
		else if (y > ypos + height) {
			inside = false;
		}
		
		// mouse is inside button
		else {
			// set mouse over sprite
			switch (e->type) {
				case SDL_MOUSEMOTION:
					if (hoverFunc != NULL) {
						(*hoverFunc)();
					}
					break;
					
				case SDL_MOUSEBUTTONDOWN:
					if (mouseDownFunc != NULL) {
						(*mouseDownFunc)();
					}
					break;
				
				case SDL_MOUSEBUTTONUP:
					if (mouseUpFunc != NULL) {
						(*mouseUpFunc)();
					}
					break;
			}
		}
	}
}
	
void Button::setHoverFunction(void (*passedFunc)()) {
	hoverFunc = passedFunc;
}

void Button::setMouseDownFunction(void (*passedFunc)()) {
	mouseDownFunc = passedFunc;
}

void Button::setMouseUpFunction(void (*passedFunc)()) {
	mouseUpFunc = passedFunc;
}