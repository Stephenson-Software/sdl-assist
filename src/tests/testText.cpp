#include "../header/GraphicsEnv.h"

int main(int argc, char* args[]) {
	// create environment
	GraphicsEnv environment;
	
	// set title
	environment.setTitle("GraphicsEnv Test 2");
	
	// set width and height of the screen
	environment.setScreenWidth(500);
	environment.setScreenHeight(500);
	
	// initialize
	environment.init();

	// point at the shipped font (this is also the default; the demos are run from the repository root)
	environment.setFontPath("src/fonts/lazy.ttf");

	// set font size; this must precede loadMedia(), which fixes the size when it opens the font
	environment.setFontSize(32);

	// load media
	environment.loadMedia();

	Text myText;
	SDL_Color textColor = {0x00, 0x00, 0x00, 0xFF};
	myText.init(25, 25, environment.getFont(), environment.getRenderer(), "GraphicsEnv Test 2:", textColor);
	
	// reload the font at a smaller size; loadMedia() closes the font it opened above
	environment.setFontSize(20);
	environment.loadMedia();

	// the first label's texture is already drawn, but its font pointer was closed, so it is re-pointed
	myText.setFont(environment.getFont());

	Text myText2;
	textColor = {0x00, 0x00, 0x00, 0xFF};
	myText2.init(25, 75, environment.getFont(), environment.getRenderer(), "Text Support", textColor);
	
	// main loop
	while (environment.isRunning()) {
		while (environment.pollEvent() != 0) {
			if (environment.getEvent()->type == SDL_QUIT) {
				environment.setRunning(false);
			}
		}
		// clear environment
		environment.setRenderColor(0xFF, 0xFF, 0xFF, 0XFF);
		environment.clear();
		
		// draw rectangle
		environment.setRenderColor(0x00, 0x00, 0x00, 0xFF);
		environment.drawRectangle(environment.getW()/2 - 50, environment.getH()/2 - 50, 100, 100);
		
		// render text
		myText.render();
		myText2.render();
	
		// present environment
		environment.present();
	}

	// release the textures while their renderer still exists
	myText.free();
	myText2.free();

	// clean up environment
	environment.cleanUp();
}