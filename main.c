#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "transformation_proc.h"

#define NUM_THREADS 13

int main(){
    int kernel;

    do {
        printf("Introduce un valor entre 55 y 155 para el efecto de blur: \n");
        scanf("%d", &kernel);
    } while (kernel < 55 || kernel > 155);

    FILE *fptr;
    char data[80] = "arc1.txt";

    fptr = fopen(data, "w");
    if (fptr == NULL){
        printf("Error\n");
        exit(1);
    }

    fprintf(fptr, "César Guerra Martínez - A01656774\n");
    fprintf(fptr, "Gerardo Deustúa Hernández - A01736455\n");
    fprintf(fptr, "José Luis Zago Guevara - A01736278\n\n");

    // Paralelismo
    unsigned long long total_leidas = 0;
    unsigned long long total_escritas = 0;
    omp_set_num_threads(NUM_THREADS);
    double start_time = omp_get_wtime();

    #pragma omp parallel for reduction(+:total_leidas, total_escritas)
        for (int i = 0; i < 100; i++) {
            char originalFile[50];
            char grayFile[50];
            char invHColorFile[50];
            char invVColorFile[50];
            char invHGrayFile[50];
            char invVGrayFile[50];
            char blurFile[50];
            int leidas = 0;
            int escritas = 0;

            snprintf(originalFile, sizeof(originalFile), "./img/img_%d.bmp", i);
            snprintf(grayFile, sizeof(grayFile), "img_%d_gray", i);
            snprintf(invHColorFile, sizeof(invHColorFile), "img_%d_invHColor", i);
            snprintf(invVColorFile, sizeof(invVColorFile), "img_%d_invVColor", i);
            snprintf(invHGrayFile, sizeof(invHGrayFile), "img_%d_invHGray", i);
            snprintf(invVGrayFile, sizeof(invVGrayFile), "img_%d_invVGray", i);
            snprintf(blurFile, sizeof(blurFile), "img_%d_blur", i);

            gray_img(grayFile, originalFile, &leidas, &escritas);
            total_leidas += leidas;
            total_escritas += escritas;
            
            invH_color_img(invHColorFile, originalFile, &leidas, &escritas);
            total_leidas += leidas;
            total_escritas += escritas;

            invV_color_img(invVColorFile, originalFile, &leidas, &escritas);
            total_leidas += leidas;
            total_escritas += escritas;
            
            invH_gray_img(invHGrayFile, originalFile, &leidas, &escritas);
            total_leidas += leidas;
            total_escritas += escritas;

            invV_gray_img(invVGrayFile, originalFile, &leidas, &escritas);
            total_leidas += leidas;
            total_escritas += escritas;

            blur_img(blurFile, originalFile, kernel, &leidas, &escritas);
            total_leidas += leidas;
            total_escritas += escritas;
        }

    double end_time = omp_get_wtime();
    fprintf(fptr, "Total de localidades leídas: %llu\n", total_leidas);
    fprintf(fptr, "Total de localidades escritas: %llu\n", total_escritas);
    fprintf(fptr, "Tiempo total de procesamiento: %.2f segundos\n\n", end_time - start_time);
    fprintf(fptr, "MIPS: %.2f\n", (20 * (total_leidas + total_escritas)) / (1000000 * (end_time - start_time)));

    fclose(fptr);
    return 0;
}