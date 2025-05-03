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

    /*fam.bmp, prueba1.bmp, sample.bmp, sample1.bmp */
    grey_img("grey_1" ,"./img/sample.bmp", fptr);
    invH_color_img("invH_color_1" ,"./img/sample.bmp", fptr);
    invV_color_img("invV_color_1" ,"./img/sample.bmp", fptr);
    invH_grey_img("invH_grey_1" ,"./img/sample.bmp", fptr);
    invV_grey_img("invV_grey_1" ,"./img/sample.bmp", fptr);
    fclose(fptr);
    return 0;
}