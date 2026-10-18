#!/usr/bin/env bash
# Regenerates the README images in docs/images/ from the real demo programs.
# Requires: Xvfb, xdotool, ImageMagick (import/convert), plus the SDL2 build deps.
# Usage (from the repository root): bash docs/capture.sh [display-number]
set -euo pipefail
N="${1:-71}"
OUT=docs/images
TMP="$(mktemp -d)"
mkdir -p "$OUT"

make -C src/tests all >/dev/null

# software GL keeps Xvfb from loading a host GPU driver
export LIBGL_ALWAYS_SOFTWARE=1 GALLIUM_DRIVER=llvmpipe
export XDG_RUNTIME_DIR="${XDG_RUNTIME_DIR:-$TMP}"

Xvfb ":$N" -screen 0 1280x800x24 >/dev/null 2>&1 &
XPID=$!
trap 'kill $XPID 2>/dev/null; rm -rf "$TMP"' EXIT
export DISPLAY=":$N"
sleep 2

# start a demo, wait for its window, print the window id
launch() {
	"./src/tests/$1_test_executable" &
	PID=$!
	sleep 1.5
	WIN=$(xdotool search --pid "$PID" | tail -1)
}

# grab the demo window; draw a small pointer at (x, y) when given
grab() {
	import -window "$WIN" "$1"
	if [ $# -eq 3 ]; then
		convert "$1" -fill white -stroke black -strokewidth 1 \
			-draw "polygon $2,$3 $(( $2 )),$(( $3 + 18 )) $(( $2 + 5 )),$(( $3 + 13 )) $(( $2 + 12 )),$(( $3 + 13 ))" "$1"
	fi
	border "$1"
}

# a thin grey frame keeps the white window visible on a white README page
border() { mogrify -bordercolor '#d0d7de' -border 1 "$1"; }

# 1. buttoncallbacks: hover, press and release the button; the status label follows
launch buttoncallbacks
i=0
frame() { grab "$TMP/bc_$(printf %02d $i).png" "$1" "$2"; i=$((i + 1)); }
xdotool mousemove --window "$WIN" 300 380; sleep 0.3; frame 300 380
for p in "260 330" "210 280" "160 240"; do set -- $p; xdotool mousemove --window "$WIN" "$1" "$2"; sleep 0.3; frame "$1" "$2"; done
xdotool mousemove --window "$WIN" 125 225; sleep 0.3; frame 125 225; frame 125 225
xdotool mousedown 1; sleep 0.3; frame 125 225; frame 125 225
xdotool mouseup 1; sleep 0.3; frame 125 225; frame 125 225
kill "$PID"; wait "$PID" 2>/dev/null || true
convert -delay 60 -loop 0 "$TMP"/bc_*.png "$OUT/buttoncallbacks.gif"

# 2. rectangles: the square chases its target
launch rectangles
for f in $(seq -w 0 39); do import -window "$WIN" "$TMP/re_$f.png"; border "$TMP/re_$f.png"; sleep 0.05; done
kill "$PID"; wait "$PID" 2>/dev/null || true
convert -delay 8 -loop 0 "$TMP"/re_*.png -layers Optimize "$OUT/rectangles.gif"

# 3. stills of the remaining demos
for t in text renderpage; do
	launch "$t"
	grab "$OUT/$t.png"
	kill "$PID"; wait "$PID" 2>/dev/null || true
done
echo "wrote $(ls "$OUT" | tr '\n' ' ')"
