#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
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

    // Paralelismo
    double start_time = omp_get_wtime();
    omp_set_nested(1);

    #pragma omp parallel for
        for (int i = 0; i < 5; i++) {
            char originalFile[50];
            char grayFile[50];
            char invHColorFile[50];
            char invVColorFile[50];
            char invHGrayFile[50];
            char invVGrayFile[50];
            char blurFile[50];

            snprintf(originalFile, sizeof(originalFile), "./img/img_%d.bmp", i);
            snprintf(grayFile, sizeof(grayFile), "img_%d_gray", i);
            snprintf(invHColorFile, sizeof(invHColorFile), "img_%d_invHColor", i);
            snprintf(invVColorFile, sizeof(invVColorFile), "img_%d_invVColor", i);
            snprintf(invHGrayFile, sizeof(invHGrayFile), "img_%d_invHGray", i);
            snprintf(invVGrayFile, sizeof(invVGrayFile), "img_%d_invVGray", i);
            snprintf(blurFile, sizeof(blurFile), "img_%d_blur", i);

            gray_img(grayFile, originalFile, fptr);
            invH_color_img(invHColorFile, originalFile, fptr);
            invV_color_img(invVColorFile, originalFile, fptr);
            invH_gray_img(invHGrayFile, originalFile, fptr);
            invV_gray_img(invVGrayFile, originalFile, fptr);
            blur_img(blurFile, originalFile, fptr, 55 + i);
        }

    double end_time = omp_get_wtime();
    printf("Elapsed time: %f seconds\n", end_time - start_time);

    fclose(fptr);
    return 0;
}