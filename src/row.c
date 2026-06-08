#include "row.h"
#include<string.h>

void serialize_row(const Row* source, void* destination) {
    memcpy((char*)destination + IS_DELETED_OFFSET,
           &source->is_deleted,
           IS_DELETED_SIZE);

    memcpy((char*)destination + ID_OFFSET,
           &source->id,
           ID_SIZE);

    memcpy((char*)destination + USERNAME_OFFSET,
           source->username,
           USERNAME_SIZE);

    memcpy((char*)destination + EMAIL_OFFSET,
           source->email,
           EMAIL_SIZE);
}

void deserialize_row(const void* source, Row* destination) {
    memcpy(&destination->is_deleted,
           (char*)source + IS_DELETED_OFFSET,
           IS_DELETED_SIZE);

    memcpy(&destination->id,
           (char*)source + ID_OFFSET,
           ID_SIZE);

    memcpy(destination->username,
           (char*)source + USERNAME_OFFSET,
           USERNAME_SIZE);

    memcpy(destination->email,
           (char*)source + EMAIL_OFFSET,
           EMAIL_SIZE);
}

