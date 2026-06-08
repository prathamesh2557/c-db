#include "parser.h"
#include <string.h>
#include <stdlib.h>

int parse_input(const char* input, Statement* statement) {
    char buffer[1024];
    strncpy(buffer, input, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    char* token = strtok(buffer, " \n");
    if (token == NULL) {
        return 0;
    }

    if (strcmp(token, "select") == 0) {
        char* id = strtok(NULL, " \n");

        if (id) {
            statement->type = STATEMENT_SELECT_BY_ID;
            statement->id = atoll(id);
        } else {
            statement->type = STATEMENT_SELECT;
        }

        return 1;
    }

    if (strcmp(token, "delete") == 0) {
        char* id = strtok(NULL, " \n");

        if (!id) {
            return 0;
        }

        statement->type = STATEMENT_DELETE;
        statement->id = atoll(id);
        return 1;
    }

    if (strcmp(token, "insert") == 0) {
        char* id = strtok(NULL, " \n");
        char* username = strtok(NULL, " \n");
        char* email = strtok(NULL, " \n");

        if (!id || !username || !email) {
            return 0;
        }

        statement->type = STATEMENT_INSERT;
        statement->row.id = atoll(id);

        strncpy(statement->row.username, username, USERNAME_SIZE);
        statement->row.username[USERNAME_SIZE - 1] = '\0';

        strncpy(statement->row.email, email, EMAIL_SIZE);
        statement->row.email[EMAIL_SIZE - 1] = '\0';

        return 1;
    }

    if (strcmp(token, "update") == 0) {
        char* id = strtok(NULL, " \n");
        char* username = strtok(NULL, " \n");
        char* email = strtok(NULL, " \n");

        if (!id || !username || !email) {
            return 0;
        }

        statement->type = STATEMENT_UPDATE;
        statement->id = atoll(id);

        strncpy(statement->row.username, username, USERNAME_SIZE);
        statement->row.username[USERNAME_SIZE - 1] = '\0';

        strncpy(statement->row.email, email, EMAIL_SIZE);
        statement->row.email[EMAIL_SIZE - 1] = '\0';

        return 1;
    }

    statement->type = STATEMENT_UNKNOWN;
    return 0;
}