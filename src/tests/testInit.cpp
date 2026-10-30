#include "../header/GraphicsEnv.h"

int main(int argc, char* args[]) {
	// create environment
	GraphicsEnv environment;
	
	// set title
	environment.setTitle("GraphicsEnv Test 1");
	
	// set width and height of the screen
	environment.setScreenWidth(400);
	environment.setScreenHeight(400);
	
	// initialize
	environment.init();
	
	// load media
	environment.loadMedia();

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
		
		// present environment
		environment.present();
	}
	
	// clean up environment
	environment.cleanUp();
}