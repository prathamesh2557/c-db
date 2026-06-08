#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "table.h"

#define INPUT_SIZE 1024

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: tinydb <filename>\n");
        exit(1);
    }

    Table * table = table_open(argv[1]);

    char input[INPUT_SIZE];

    while (1) {
        printf("tinydb > ");
        fflush(stdout);

        if (!fgets(input, INPUT_SIZE, stdin)) {
            printf("\n");
            break;
        }

        // Meta commands
        if (input[0] == '.') {
            if (strcmp(input, ".exit\n") == 0) {
                printf("Bye!\n");
                break;
            } else {
                printf("Unrecognized meta-command\n");
                continue;
            }
        }

        Statement stmt;

        if (!parse_input(input, &stmt)) {
            printf("Parse error\n");
            continue;
        }

        if (stmt.type == STATEMENT_INSERT) {
            table_insert(table, &stmt.row);
        } else if (stmt.type == STATEMENT_SELECT) {
            table_select(table);
        }else if (stmt.type == STATEMENT_DELETE) {
            table_delete(table, stmt.id);
        }else if (stmt.type == STATEMENT_SELECT_BY_ID) {
            table_select_by_id(table, stmt.id);
        }else if (stmt.type == STATEMENT_UPDATE) {
            table_update(table, stmt.id, &stmt.row);
        } else {
            printf("Unknown statement\n");
        }
    }


        DbHeader* header =
            (DbHeader*) pager_get_page(table->pager, 0);

        header->num_rows = table->num_rows;

        for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        if (table->pager->pages[i] != NULL) {
            pager_flush(table->pager, i);
        }
    }

    return 0;
}
