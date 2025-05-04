#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "transformation_proc.h"

int main(){
    FILE *fptr;
    char data[80] = "arc1.txt";

    fptr = fopen(data, "w");
    if (fptr == NULL){
        printf("Error\n");
        exit(1);
    }

    fprintf(fptr, "César Guerra Martínez - A01656774\n");
    fprintf(fptr, "Gerardo Deustúa Hernández - A017\n");
    fprintf(fptr, "José Luis Zago Guevara - A01736278\n");

    gray_img("gray_1" ,"./img/sample.bmp", fptr);
    invH_color_img("invH_color_1" ,"./img/sample.bmp", fptr);
    invV_color_img("invV_color_1" ,"./img/sample.bmp", fptr);
    invH_gray_img("invH_gray_1" ,"./img/sample.bmp", fptr);
    invV_gray_img("invV_gray_1" ,"./img/sample.bmp", fptr);
    blur_img("blur_1" ,"./img/sample.bmp", fptr, 55);
    fclose(fptr);
    return 0;
}