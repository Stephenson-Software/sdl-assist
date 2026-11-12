#include "../header/GraphicsEnv.h"

int main(int argc, char* args[]) {
	// create environment
	GraphicsEnv environment;
	
	// set title
	environment.setTitle("GraphicsEnv Test 1");
	
	// set width and height of the screen
	environment.setScreenWidth(400);
	environment.setScreenHeight(400);

	// a non-positive size is reported and ignored, so the window stays 400x400 with the rectangle centered
	environment.setScreenWidth(0);
	environment.setScreenHeight(-1);

	// initialize
	environment.init();
	
	// a font that cannot be opened is reported to log.txt and leaves the environment usable, with getFont() returning NULL
	environment.setFontPath("src/fonts/missing.ttf");

	// load media
	environment.loadMedia();

	// a label given the NULL font reports through SDL_LogError() and draws nothing, while the rectangle below still draws
	Text label;
	SDL_Color textColor = {0x00, 0x00, 0x00, 0xFF};
	label.init(25, 25, environment.getFont(), environment.getRenderer(), "Never drawn", textColor);

	// an environment that is never initialized must not shut SDL down for this one when it is destroyed
	{
		GraphicsEnv unused;
	}

	// main loop
	while (environment.isRunning()) {
		while (environment.pollEvent() != 0) {
			if (environment.getEvent()->type == SDL_QUIT) {
				environment.setRunning(false);
			}
		}
		// clear environment; a channel above 0xFF is clamped, so this still clears to white rather than wrapping to black
		environment.setRenderColor(0x100, 0x100, 0x100, 0x100);
		environment.clear();
		
		// draw rectangle
		environment.setRenderColor(0x00, 0x00, 0x00, 0xFF);
		environment.drawRectangle(environment.getW()/2 - 50, environment.getH()/2 - 50, 100, 100);

		// render text; the label holds no texture, so this draws nothing
		label.render();

		// present environment
		environment.present();
	}
	
	// release the label while its renderer still exists, as the other demos do
	label.free();

	// clean up environment
	environment.cleanUp();
}