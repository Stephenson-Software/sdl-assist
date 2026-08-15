GLC_FILES = ../GraphicsEnv.cpp ../Text.cpp ../Button.cpp

.PHONY: all init rectangles renderpage text button clean

all: init rectangles renderpage text button

init: testInit.cpp
	g++ testInit.cpp $(GLC_FILES) -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o init_test_executable

rectangles: testRectangles.cpp
	g++ testRectangles.cpp $(GLC_FILES) -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o rectangles_test_executable

renderpage: testRenderPage.cpp
	g++ testRenderPage.cpp $(GLC_FILES) -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o renderpage_test_executable

text: testText.cpp
	g++ testText.cpp $(GLC_FILES) -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o text_test_executable

button: testButton.cpp
	g++ testButton.cpp $(GLC_FILES) -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o button_test_executable

clean:
	rm -f init_test_executable rectangles_test_executable renderpage_test_executable text_test_executable button_test_executable