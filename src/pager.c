#include "pager.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>    
#include <string.h>     
#include <unistd.h>     
#include <fcntl.h>      
#include <sys/stat.h>   

Pager* pager_open(const char* filename) {
    int fd = open(
        filename,
        O_RDWR | O_CREAT,
        S_IWUSR | S_IRUSR
    );

    if (fd == -1) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    off_t file_length = lseek(fd, 0, SEEK_END);

    Pager* pager = malloc(sizeof(Pager));
    pager->fd = fd;
    pager->file_length = file_length;

    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        pager->pages[i] = NULL;
    }

    return pager;
}

void* pager_get_page(Pager* pager, uint32_t page_num) {
    if (page_num >= TABLE_MAX_PAGES) {
        printf("Page number out of bounds.\n");
        exit(EXIT_FAILURE);
    }

    if (pager->pages[page_num] != NULL) {
        return pager->pages[page_num];
    }

    void* page = malloc(PAGE_SIZE);

    uint32_t num_pages = pager->file_length / PAGE_SIZE;

    if (pager->file_length % PAGE_SIZE != 0) {
        num_pages += 1;
    }

    if (page_num < num_pages) {
        lseek(pager->fd, page_num * PAGE_SIZE, SEEK_SET);
        read(pager->fd, page, PAGE_SIZE);
    } else {
        memset(page, 0, PAGE_SIZE);
    }

    pager->pages[page_num] = page;

    return page;
}

void pager_flush(Pager* pager, uint32_t page_num) {
    if (pager->pages[page_num] == NULL) {
        return;
    }

    lseek(pager->fd, page_num * PAGE_SIZE, SEEK_SET);
    write(pager->fd, pager->pages[page_num], PAGE_SIZE);
}
