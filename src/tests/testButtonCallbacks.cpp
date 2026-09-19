#include <string>

#include "../header/GraphicsEnv.h"

// the labels and the button are globals so the callbacks can reach them
Text titleText;
Text statusText;
Text sizeText;

Button myButton;

void showStatus(std::string status) {
	SDL_Color textColor = {0x00, 0x00, 0x00, 0xFF};
	statusText.loadText(status, textColor);
}

void onHover() {
	showStatus("Hovering");
}

void onMouseDown() {
	showStatus("Pressed");
}

void onMouseUp() {
	showStatus("Released");
}

int main(int argc, char* args[]) {
	// create environment
	GraphicsEnv environment;

	// set title
	environment.setTitle("GraphicsEnv Test 6");

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
	titleText.init(25, 25, environment.getFont(), environment.getRenderer(), "GraphicsEnv Test 6:", textColor);

	// wire the second label step by step instead of through init()
	statusText.setPosition(25, 75);
	statusText.setFont(environment.getFont());
	statusText.setRenderer(environment.getRenderer());
	statusText.loadText("Move over the button", textColor);

	// the button supplies no callback to init(); all three are attached through the setters
	myButton.init(100, 200, 50, 50, environment.getRenderer(), NULL);
	myButton.setHoverFunction(&onHover);
	myButton.setMouseDownFunction(&onMouseDown);
	myButton.setMouseUpFunction(&onMouseUp);

	// report the title label's measured size, read back through the getters
	std::string sizeReport = "Title is " + std::to_string(titleText.getWidth()) + " x " + std::to_string(titleText.getHeight());
	sizeText.init(25, 300, environment.getFont(), environment.getRenderer(), sizeReport, textColor);

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

		// outline the title label with a rectangle sized from its getters
		environment.setRenderColor(102, 178, 255, 0xFF);
		environment.drawRectangle(titleText.getX() - 5, titleText.getY() - 5, titleText.getWidth() + 10, titleText.getHeight() + 10);

		// render text
		titleText.render();
		statusText.render();
		sizeText.render();

		// render buttons
		myButton.render();

		// present environment
		environment.present();
	}

	// release the textures while their renderer still exists; these labels are
	// globals, so they would otherwise outlive cleanUp()
	titleText.free();
	statusText.free();
	sizeText.free();

	// clean up environment
	environment.cleanUp();
}
