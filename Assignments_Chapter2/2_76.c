#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void* c_alloc(size_t nmemb, size_t size) {
	if (nmemb == 0 || size == 0) {
		return NULL;
	}
	size_t buf_size = nmemb * size;
	if (nmemb == buf_size / size) {  // a way to check overflow
		void* ptr = malloc(buf_size);
		memset(ptr, 0, buf_size);
		return ptr;
	}
	return NULL;
}

int main(int argc, char* argv[]) {
	void* p;
	p = c_alloc(0x1234, 1);
	assert(p != NULL);
	free(p);

	p = c_alloc(SIZE_MAX, 2);
	assert(p == NULL);
	free(p);
	return 0;
}