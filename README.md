# sdl-assist
This library abstracts out the handling of Simple DirectMedia Layer, providing support for drawing rectangles, text and buttons to the screen.

## Dependencies

A C++ compiler (`g++`) and the SDL2 development packages:

```
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```

The same three packages are installed by `.devcontainer/Dockerfile` and by the
build workflow in `.github/workflows/build.yml`, so opening this repository in
the dev container provides a ready environment.

## Building

There is no library artifact to install. The build compiles the library sources
together with each demo program:

```
make -C src/tests all
```

This produces five executables in `src/tests/`:
`init_test_executable`, `rectangles_test_executable`, `renderpage_test_executable`,
`text_test_executable` and `button_test_executable`.

To remove them:

```
make -C src/tests clean
```

## Demo programs

Each program under `src/tests/` is a standalone `main()` that opens a real
window and runs until it receives `SDL_QUIT`, so a display is required to run
them. Building them does not require a display.

| Target | Source | What it demonstrates |
|--------|--------|----------------------|
| `init` | `testInit.cpp` | The minimal lifecycle: a 400x400 window with one black rectangle centered on a white background. |
| `rectangles` | `testRectangles.cpp` | Animation via `drawRectangle` — a black square moves toward a small green target that relocates to a random position each time it is reached. |
| `renderpage` | `testRenderPage.cpp` | Layout with rectangles — header, body and footer bands drawn in three shades of blue. |
| `text` | `testText.cpp` | `Text` rendering — two labels drawn with the font and renderer taken from the environment. |
| `button` | `testButton.cpp` | `Button` rendering and event handling — clicking the button runs a callback that rewrites a neighbouring `Text` label. |

## Usage

Consuming projects compile the library sources directly alongside their own,
as `src/tests/Makefile` does:

```
g++ main.cpp path/to/sdl-assist/src/GraphicsEnv.cpp \
              path/to/sdl-assist/src/Text.cpp \
              path/to/sdl-assist/src/Button.cpp \
              -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o my_program
```

A minimal program follows the shape used by every demo:

```cpp
#include "path/to/sdl-assist/src/header/GraphicsEnv.h"

int main(int argc, char* args[]) {
	GraphicsEnv environment;

	environment.setTitle("My Program");
	environment.setScreenWidth(400);
	environment.setScreenHeight(400);

	environment.init();
	environment.loadMedia();

	while (environment.isRunning()) {
		while (environment.pollEvent() != 0) {
			if (environment.getEvent()->type == SDL_QUIT) {
				environment.setRunning(false);
			}
		}

		environment.setRenderColor(0xFF, 0xFF, 0xFF, 0xFF);
		environment.clear();

		environment.setRenderColor(0x00, 0x00, 0x00, 0xFF);
		environment.drawRectangle(environment.getW()/2 - 50, environment.getH()/2 - 50, 100, 100);

		environment.present();
	}

	environment.cleanUp();
}
```

`Text` and `Button` are constructed separately and wired to the environment's
renderer and font:

```cpp
SDL_Color textColor = {0x00, 0x00, 0x00, 0xFF};

Text label;
label.init(25, 25, environment.getFont(), environment.getRenderer(), "Hello", textColor);

Button button;
button.init(100, 200, 50, 50, environment.getRenderer(), &onMouseDown);
```

Both are rendered inside the main loop with `label.render()` and
`button.render()`; `button.handleEvent(environment.getEvent())` is called from
the event loop so the button can react to the mouse.

## Known limitation

`GraphicsEnv::loadMedia()` opens `objects/bboron.ttf`, a path resolved relative
to the working directory. That file is not shipped with this repository — the
font present here is `src/fonts/lazy.ttf` — so the font fails to load unless the
expected file is supplied. `loadMedia()` responds to the failure by calling
`cleanUp()`, which destroys the renderer and window mid-run, so the `text` and
`button` demos crash rather than simply rendering without text. This is tracked
in [#7](https://github.com/Stephenson-Software/sdl-assist/issues/7),
[#9](https://github.com/Stephenson-Software/sdl-assist/issues/9) and
[#10](https://github.com/Stephenson-Software/sdl-assist/issues/10).

## License

MIT — see [LICENSE](LICENSE) and [COPYRIGHT.md](COPYRIGHT.md).
