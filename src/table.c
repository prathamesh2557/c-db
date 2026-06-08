#include "table.h"
#include "row.h"
#include "cursor.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void* row_slot(Table* table, uint32_t row_num) {
    uint32_t page_num = (row_num / ROWS_PER_PAGE) + 1;

    void* page = pager_get_page(table->pager, page_num);

    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;

    return (char*)page + byte_offset;
}

void table_insert(Table* table, const Row* row) {
    Row existing;

    for (uint32_t i = 0; i < table->num_rows; i++) {
        uint32_t page_num = (i / ROWS_PER_PAGE) + 1;
        uint32_t row_offset = i % ROWS_PER_PAGE;
        uint32_t byte_offset = row_offset * ROW_SIZE;

        void* page = pager_get_page(table->pager, page_num);
        void* source = (char*)page + byte_offset;

        deserialize_row(source, &existing);

        if (!existing.is_deleted &&
            existing.id == row->id) {

            printf("Error: Duplicate ID.\n");
            return;
        }
    }

    for (uint32_t i = 0; i < table->num_rows; i++) {
        uint32_t page_num = (i / ROWS_PER_PAGE) + 1;
        uint32_t row_offset = i % ROWS_PER_PAGE;
        uint32_t byte_offset = row_offset * ROW_SIZE;

        void* page = pager_get_page(table->pager, page_num);
        void* destination = (char*)page + byte_offset;

        deserialize_row(destination, &existing);

        if (existing.is_deleted) {
            Row new_row = *row;
            new_row.is_deleted = 0;

            serialize_row(&new_row, destination);

            printf("Inserted into reused slot.\n");
            return;
        }
    }

    if (table->num_rows >= ROWS_PER_PAGE * TABLE_MAX_PAGES) {
        printf("Error: Table full.\n");
        return;
    }

    void* destination = row_slot(table, table->num_rows);

    Row new_row = *row;
    new_row.is_deleted = 0;

    serialize_row(&new_row, destination);

    table->num_rows++; //
}

void table_select(const Table* table) {
    Row row; 

    Cursor* cursor = 
     table_start((Table*)table);

    while (!cursor->end_of_table){

        deserialize_row(
            cursor_value(cursor),
            &row
        );

        if(!row.is_deleted) {

            printf("(%lld, %s, %s)\n",
                    (long long)row.id,
                     row.username,
                     row.email);         
        }
        cursor_advance(cursor);
    } 

    free(cursor);
    
}

void table_delete(Table* table, int64_t id) {
    Row row;

    for (uint32_t i = 0; i < table->num_rows; i++) {
        uint32_t page_num = (i / ROWS_PER_PAGE) + 1;
        uint32_t row_offset = i % ROWS_PER_PAGE;
        uint32_t byte_offset = row_offset * ROW_SIZE;

        void* page = pager_get_page(table->pager, page_num);
        void* source = (char*)page + byte_offset;

        deserialize_row(source, &row);

        if (!row.is_deleted &&
            row.id == id) {

            row.is_deleted = 1; 
            serialize_row(&row, source);

            printf("Deleted id=%lld\n",(long long)id);
            return;
        }
    }

    printf("ID not found.\n");
}

void table_update(Table* table, int64_t id, const Row* new_data) {
    Row row;

    for (uint32_t i = 0; i < table->num_rows; i++) {
        uint32_t page_num = (i / ROWS_PER_PAGE) + 1;
        uint32_t row_offset = i % ROWS_PER_PAGE;
        uint32_t byte_offset = row_offset * ROW_SIZE;

        void* page = pager_get_page(table->pager, page_num);
        void* location = (char*)page + byte_offset;

        deserialize_row(location, &row);

        if (!row.is_deleted &&
            row.id == id) {

            Row updated = *new_data;

            updated.id = id; 
            updated.is_deleted = 0;

            serialize_row(&updated, location);

            printf("Updated.\n");
            return;
        }
    }

    printf("ID not found.\n");
}

void table_select_by_id(const Table* table, int64_t id) {
    Row row;

    for (uint32_t i = 0; i < table->num_rows; i++) {
        uint32_t page_num = (i / ROWS_PER_PAGE) + 1;
        uint32_t row_offset = i % ROWS_PER_PAGE;
        uint32_t byte_offset = row_offset * ROW_SIZE;

        void* page = pager_get_page(table->pager, page_num);
        void* source = (char*)page + byte_offset;

        deserialize_row(source, &row);

        if (!row.is_deleted &&
            row.id == id) {

            printf("(%lld, %s, %s)\n",
                   (long long)row.id,
                   row.username,
                   row.email);

            return;
        }
    }

    printf("ID not found.\n");
}

Table* table_open(const char* filename) {
    Pager* pager = pager_open(filename);

    Table* table = malloc(sizeof(Table));
    table->pager = pager;

    void* header_page = pager_get_page(pager, 0);

    DbHeader* header = (DbHeader*)header_page;

    table->num_rows = header->num_rows;

    return table;
}