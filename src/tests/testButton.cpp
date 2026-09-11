#include <string>

#include "../header/GraphicsEnv.h"

// create environment
GraphicsEnv environment;

Text myText;
Text myText2;
Text myText3;

Button myButton;

int timesClicked;

void myButtonFunc() {
	timesClicked++;
	SDL_Color textColor = {0x00, 0x00, 0x00, 0xFF};
	if (timesClicked == 1) {
		myText3.loadText("Test complete", textColor);
	}
	else if (timesClicked == 2) {
		myText3.loadText("Again?", textColor);
	}
	else if (timesClicked == 3) {
		myText3.loadText("Jeepers", textColor);
	};
	// the label keeps the position given to init(); no setPosition() is needed after a reload
}

int main(int argc, char* args[]) {
	// create environment
	GraphicsEnv environment;
	
	// set title
	environment.setTitle("GraphicsEnv Test 3");
	
	// set width and height of the screen
	environment.setScreenWidth(500);
	environment.setScreenHeight(500);
	
	// initialize
	environment.init();
	
	// set font size
	environment.setFontSize(32);
	
	// load media
	environment.loadMedia();
	

	SDL_Color textColor = {0x00, 0x00, 0x00, 0xFF};
	myText.init(25, 25, environment.getFont(), environment.getRenderer(), "GraphicsEnv Test 3:", textColor);
	
	textColor = {0x00, 0x00, 0x00, 0xFF};
	myText2.init(25, 75, environment.getFont(), environment.getRenderer(), "Button Support", textColor);
	
	myButton.init(100, 200, 50, 50, environment.getRenderer(), &myButtonFunc);

	textColor = {0x00, 0x00, 0x00, 0xFF};
	myText3.init(160, 200, environment.getFont(), environment.getRenderer(), "<--- A Button", textColor);
	
	// main loop
	while (environment.isRunning()) {
		while (environment.pollEvent() != 0) {
			if (environment.getEvent()->type == SDL_QUIT) {
				environment.setRunning(false);
			}
			myButton.handleEvent(environment.getEvent());
		}
		// clear environment
		environment.setRenderColor(0xFF, 0xFF, 0xFF, 0XFF);
		environment.clear();
		
		// draw rectangle
		environment.setRenderColor(102, 178, 255, 0xFF);
		environment.drawRectangle(0, 0, environment.getW(), 125);
		
		// render text
		myText.render();
		myText2.render();
		myText3.render();
		
		// render buttons
		myButton.render();
	
		// present environment
		environment.present();
	}

	// release the textures while their renderer still exists; these labels are
	// globals, so they would otherwise outlive cleanUp()
	myText.free();
	myText2.free();
	myText3.free();

	// clean up environment
	environment.cleanUp();
}