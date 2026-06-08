#ifndef TABLE_H
#define TABLE_H
#include "pager.h"
#include <stdint.h>
#include <stdbool.h>
#include "row.h"
#define PAGE_SIZE 4096
#define TABLE_MAX_PAGES 100
#define ROWS_PER_PAGE (PAGE_SIZE / ROW_SIZE)

typedef struct {
    Pager * pager;
    uint32_t num_rows;
} Table;

typedef struct {
    uint32_t num_rows;
} DbHeader;
 
Table* table_create(void);
void table_insert(Table* table, const Row* row);
void table_select(const Table* table);
void table_delete(Table* table, int64_t id);
void table_select_by_id(const Table* table, int64_t id);
void table_update(Table* table, int64_t id, const Row* new_data);
Table* table_open(const char* filename);

#endif
