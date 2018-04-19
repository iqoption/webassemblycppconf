Complex benchmark with multiplying matrices for testing try_int_cast function.

// without test function
emcc complex_try_int_cast.cpp -o test.html -s WASM=1 -O3 --emit-symbol-map -s ALLOW_MEMORY_GROWTH=1

// with test function
emcc complex_try_int_cast.cpp -o test.html -s WASM=1 -O3 --emit-symbol-map -s ALLOW_MEMORY_GROWTH=1 -DSAFECAST

// clamp mode
emcc complex_try_int_cast.cpp -o test.html -s WASM=1 -O3 --emit-symbol-map -s ALLOW_MEMORY_GROWTH=1 -s "BINARYEN_TRAP_MODE='clamp'"