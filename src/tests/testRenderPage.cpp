#include "../header/GraphicsEnv.h"

GraphicsEnv environment;

void renderHeader() {
	environment.setRenderColor(0x75, 0xBB, 0xFD, 0xFF); // sky blue
	environment.drawRectangle(0, 0, environment.getW(), environment.getH()/10);
}

void renderBody() {
	environment.setRenderColor(0xA2, 0xCF, 0xFE, 0xFF); // baby blue
	environment.drawRectangle(0, environment.getH()/10, environment.getW(), environment.getH() - environment.getH()/10);
}

void renderFooter() {
	environment.setRenderColor(0x6A, 0x79, 0xF7, 0xFF); // cornflower
	environment.drawRectangle(0, environment.getH() - environment.getH()/10, environment.getW(), environment.getH()/10);
}

void drawBackground() {
	renderHeader();
	renderBody();
	renderFooter();
}

int main(int argc, char* args[]) {
	environment.setTitle("GraphicsEnv Test 4");
	environment.setScreenWidth(1000);
	environment.setScreenHeight(700);
	
	environment.init();
	
	environment.loadMedia();
	
	while (environment.isRunning()) {
		while (environment.pollEvent() != 0) {
			if (environment.getEvent()->type == SDL_QUIT) {
				environment.setRunning(false);
			}
		}
		environment.setRenderColor(0xFF, 0xFF, 0xFF, 0XFF);
		environment.clear();
		
		drawBackground();
		
		environment.present();
	}
	
	environment.cleanUp();
}