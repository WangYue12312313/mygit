#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct Vector {
    uint32_t* data;
    size_t size;
    size_t capacity;
};



struct Vector* create_vector(uint32_t value) {
    struct Vector* vec = (struct Vector*)malloc(sizeof(struct Vector));
    if (vec == NULL) {
        // 处理内存分配失败的情况
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    vec->data = (uint32_t*)malloc(sizeof(uint32_t));
    if (vec->data == NULL) {
        // 处理内存分配失败的情况
        free(vec);
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    vec->data[0] = value;
    vec->size = 1;
    vec->capacity = 1;
    return vec;
}

void push_back(struct Vector* vec, uint32_t value) {
    if (vec->size >= vec->capacity) {
        vec->capacity *= 2;
        uint32_t* new_data = (uint32_t*)realloc(vec->data, vec->capacity * sizeof(uint32_t));
        if (new_data == NULL) {
            // 处理内存分配失败的情况
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        vec->data = new_data;
    }
    vec->data[vec->size] = value;
    vec->size++;
}

struct Vector* find_mdp_simple(uint32_t value, uint32_t base) {
    uint32_t exp = base;
    struct Vector* ret = create_vector(value);
    uint32_t prev = value;

    while (exp < value) {
        if ((value + 1) % exp != 0) {
            uint32_t temp = (value / exp) * exp - 1;
            if (prev != temp) {
                push_back(ret, temp);
                prev = temp;
            }
        }
        exp *= base;
    }
    return ret;
}

int main() {
    uint32_t value = 1399; // Your value
    uint32_t base = 10;   // Your base
    struct Vector* result = find_mdp_simple(value, base);

    printf("Result vector: ");
    for (size_t i = 0; i < result->size; i++) {
        printf("%u ", result->data[i]);
    }
    printf("\n");

    free(result->data);
    free(result);

    return 0;
}
