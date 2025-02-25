#pragma once

/* multiboot ONE constants */
#define ALIGN       1<<0
#define MEMINFO     1<<1
#define FLAGS       ALIGN | MEMINFO
#define MAGIC       0x1BADB002
#define CHECKSUM    -(MAGIC + FLAGS)
