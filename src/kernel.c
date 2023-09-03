#include "kernel.h"

uint16_t * video_mem = 0;

// Color is shifted 8 bits to the left because color should be present in the higher byte. 
// THe lower byte is taken up by the character ASCII value. SO OR opertaion is preformed.
uint16_t terminal_make_char(char c, char color){
    return (color << 8) | c;
}

void terminal_initialise(){
    //Initialise a pointer of 16-bit integer type at the absolute location 0xB8000 which is the location 
    // we need to write into the video memory
    video_mem = (uint16_t *)(0xB8000);

    for(int i=0; i<VGA_HEIGHT; i++){
        for(int j=0; j<VGA_WIDTH; j++){
            // The equation determines the index in a 1D array for any coordinates
            video_mem[(i * VGA_WIDTH) + j] = terminal_make_char(' ', 0);
        }
    }
}


void kernel_main(){
    terminal_initialise();
    video_mem[0] = terminal_make_char('B', 3);
}