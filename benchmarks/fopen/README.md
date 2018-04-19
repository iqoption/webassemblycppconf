fopen function test benchmark.

// no heap copy
emcc fopen.cpp -o test.html -s WASM=1 -O3 --emit-symbol-map -g -s TOTAL_MEMORY=33554432 --no-heap-copy --preload-file test@/

// copy files to heap
emcc fopen.cpp -o test.html -s WASM=1 -O3 --emit-symbol-map -g -s TOTAL_MEMORY=33554432 --preload-file test@/