# CDB

A simple SQLite-inspired database built from scratch in C.

## About

CDB is a learning project focused on understanding how databases work internally. The goal is to implement core database components such as:

- Row serialization and deserialization
- Persistent storage using files
- Page management with a pager
- Table abstraction
- Cursor-based row traversal
- B-Tree storage engine (work in progress)

This project follows database implementation concepts similar to SQLite while aiming to understand the underlying data structures and storage mechanisms.

## Features

Current features:

- Insert rows
- Select rows
- Persistent storage on disk
- Page caching
- Row serialization
- Cursor implementation

Planned features:

- B-Tree leaf nodes
- B-Tree splitting
- Indexed lookups
- Deletion
- Query optimization

## Build

```bash
gcc *.c -o cdb
