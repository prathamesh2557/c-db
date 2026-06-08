#ifndef ROW_H
#define ROW_H

#include <stdint.h>

#define USERNAME_SIZE 32
#define EMAIL_SIZE 255

#define IS_DELETED_SIZE sizeof(uint8_t)
#define IS_DELETED_OFFSET 0

#define ID_SIZE sizeof(int64_t)
#define ID_OFFSET (IS_DELETED_OFFSET + IS_DELETED_SIZE)

#define USERNAME_OFFSET (ID_OFFSET + ID_SIZE)

#define EMAIL_OFFSET (USERNAME_OFFSET + USERNAME_SIZE)

#define ROW_SIZE (IS_DELETED_SIZE + ID_SIZE + USERNAME_SIZE + EMAIL_SIZE)

typedef struct {
    int64_t id;
    uint8_t is_deleted;
    char username[USERNAME_SIZE];
    char email[EMAIL_SIZE];
} Row;

void serialize_row(const Row* source, void* destination);
void deserialize_row(const void* source, Row* destination);

#endif