#ifndef CONFIG_H
#define CONFIG_H

#define TOTAL_INTERRUPTS 512
#define KERNEL_CODE_SELECTOR 0x08
#define KERNEL_DATA_SELECTOR 0x10
#define HEAP_SIZE_BYTES 104857600       // 100MB heap size to be declared
#define HEAP_BLOCK_SIZE 4096
#define HEAP_ADDRESS 0x01000000
#define HEAP_TABLE_ADDRESS 0x00007E00
#define SECTOR_SIZE 512
#define MAX_FILESYSTEMS 12
#define MAX_FILE_DESCRIPTORS 512

#endif