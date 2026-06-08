#ifndef PARSER_H
#define PARSER_H

#include "row.h"

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT,
    STATEMENT_UNKNOWN,
    STATEMENT_DELETE,
    STATEMENT_UPDATE,
    STATEMENT_SELECT_BY_ID
} StatementType;

typedef struct {
    StatementType type;
    Row row;
    int64_t id;
} Statement;

int parse_input(const char* input, Statement* statement);

#endif
