Simply double to int benchmark.

// without test function
emcc double_to_int.cpp -o test.html -s WASM=1 -O3 --emit-symbol-map -s ALLOW_MEMORY_GROWTH=1

// with test function
emcc double_to_int.cpp -o test.html -s WASM=1 -O3 --emit-symbol-map -s ALLOW_MEMORY_GROWTH=1 -DSAFECAST

// clamp mode
emcc double_to_int.cpp -o test.html -s WASM=1 -O3 --emit-symbol-map -s ALLOW_MEMORY_GROWTH=1 -s "BINARYEN_TRAP_MODE='clamp'"