log() test benchmark.

// wasm
emcc log.cpp -o test.html -s WASM=1 -O3 -s ALLOW_MEMORY_GROWTH=1 --emit-symbol-map -g

// asm
emcc log.cpp -o test.html -O3 -s ALLOW_MEMORY_GROWTH=1 --emit-symbol-map -g