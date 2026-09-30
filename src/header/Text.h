#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Text {
	public:
		Text();
		~Text();
		void init(int x, int y, TTF_Font* f, SDL_Renderer* r, std::string text, SDL_Color textColor);
		void setPosition(int x, int y);
		void free();
		void loadText(std::string textToRender, SDL_Color textColor);
		void setFont(TTF_Font* f);
		void render();
		void setRenderer(SDL_Renderer* r);
		int getX();
		int getY();
		int getWidth();
		int getHeight();
		
	private:
		int xpos;
		int ypos;
		int width;
		int height;
		SDL_Texture* texture;
		std::string innerText;
		SDL_Renderer* renderer;
		TTF_Font* font;

		// the texture is owned, so a shallow copy would destroy it twice; copying is disallowed
		Text(const Text&);
		Text& operator=(const Text&);
};

#endif