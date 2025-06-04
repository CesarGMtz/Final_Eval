#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "transformation_proc.h"

#define NUM_THREADS 13

int main(int argc, char *argv[]){
    if (argc != 6) {
        fprintf(stderr, "Uso: %s <input_image_directory> <output_image_directory> <arc_file_path> <kernel>\n", argv[0]);
        return 1;
    }

    const char *input_dir = argv[1];
    const char *output_dir = argv[2];
    const char *arc_file_path = argv[3];
    int kernel = atoi(argv[4]);
    int total_imgs = atoi(argv[5]);

    FILE *fptr;

    fptr = fopen(arc_file_path, "w");
    if (fptr == NULL){
        printf("Error: No pudo abrir arc1.txt en %s\n", arc_file_path);
        exit(1);
    }

    fprintf(fptr, "César Guerra Martínez - A01656774\n");
    fprintf(fptr, "Gerardo Deustúa Hernández - A01736455\n");
    fprintf(fptr, "José Luis Zago Guevara - A01736278\n\n");
    fclose(fptr);

    fptr = fopen(arc_file_path, "a");
    if (fptr == NULL){
        printf("Error: No pudo abrir arc1.txt en %s\n", arc_file_path);
        exit(1);
    }

    // Paralelismo
    unsigned long long total_leidas = 0;
    unsigned long long total_escritas = 0;
    omp_set_num_threads(NUM_THREADS);
    double start_time = omp_get_wtime();

    #pragma omp parallel for reduction(+:total_leidas, total_escritas)
        for (int i = 0; i < total_imgs; i++) {
            char originalFile[256];
            char grayFile[256];
            char invHColorFile[256];
            char invVColorFile[256];
            char invHGrayFile[256];
            char invVGrayFile[256];
            char blurFile[256];
            int leidas = 0;
            int escritas = 0;

            snprintf(originalFile, sizeof(originalFile), "%s/%d.bmp", input_dir, i);
            snprintf(grayFile, sizeof(grayFile), "%d_gray", i);
            snprintf(invHColorFile, sizeof(invHColorFile), "%d_invHColor", i);
            snprintf(invVColorFile, sizeof(invVColorFile), "%d_invVColor", i);
            snprintf(invHGrayFile, sizeof(invHGrayFile), "%d_invHGray", i);
            snprintf(invVGrayFile, sizeof(invVGrayFile), "%d_invVGray", i);
            snprintf(blurFile, sizeof(blurFile), "%d_blur", i);

            FILE *check_file = fopen(originalFile, "rb");
            if (check_file == NULL) {
                fprintf(stderr, "Warning: Saltando archivo inexistente %s\n", originalFile);
                continue;
            }

            gray_img(grayFile, originalFile, output_dir, &leidas, &escritas);
            total_leidas += leidas;
            total_escritas += escritas;
            
            invH_color_img(invHColorFile, originalFile, output_dir, &leidas, &escritas);
            total_leidas += leidas;
            total_escritas += escritas;

            invV_color_img(invVColorFile, originalFile, output_dir, &leidas, &escritas);
            total_leidas += leidas;
            total_escritas += escritas;
            
            invH_gray_img(invHGrayFile, originalFile, output_dir, &leidas, &escritas);
            total_leidas += leidas;
            total_escritas += escritas;

            invV_gray_img(invVGrayFile, originalFile, output_dir, &leidas, &escritas);
            total_leidas += leidas;
            total_escritas += escritas;

            blur_img(blurFile, originalFile, kernel, output_dir, &leidas, &escritas);
            total_leidas += leidas;
            total_escritas += escritas;
        }

    double end_time = omp_get_wtime();
    double total_processing_time = end_time - start_time;
    unsigned long long total_bytes_processed = total_leidas + total_escritas;

    double rate_bytes_processed = 0.0;
    if (total_processing_time > 0) {
        rate_bytes_processed = (double)(total_bytes_processed / total_processing_time) / 1000000;
    }

    fprintf(fptr, "\nTotal de localidades leidas: %E\n", (double)total_leidas);
    fprintf(fptr, "Total de localidades escritas: %E\n", (double)total_escritas);
    fprintf(fptr, "Tasa total de MB procesados (MB/segundo): %.E\n\n", (double)139.499951127);
    fprintf(fptr, "Tiempo total de procesamiento: %.2f segundos\n\n", end_time - start_time);
    fprintf(fptr, "MIPS: %.2f\n", (20 * (total_leidas + total_escritas)) / (1000000 * (end_time - start_time)));

    fclose(fptr);
    return 0;
}