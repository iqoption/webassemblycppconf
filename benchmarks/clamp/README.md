Clamp test benchmark.

// wasm
emcc clamp.cpp -o test.html -s WASM=1 -O3 --emit-symbol-map -s ALLOW_MEMORY_GROWTH=1

// wasm + clamp
emcc clamp.cpp -o test.html -s WASM=1 -O3 --emit-symbol-map -s ALLOW_MEMORY_GROWTH=1 -s "BINARYEN_TRAP_MODE='clamp'"

// asm
emcc clamp.cpp -o test.html -O3 --emit-symbol-map -s ALLOW_MEMORY_GROWTH=1