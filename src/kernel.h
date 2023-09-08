#ifndef KERNEL_H    // CHecks if it is already defined, if not so, it includes the code belowe it until endif
#define KERNEL_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 20
#define MAX_PATH 108

void kernel_main();
void print(const char* str);

#define ERROR(value) (void*)value
#define ERROR_I(value) (int)value
#define ISERR(value) ((int)value < 0)
#endif