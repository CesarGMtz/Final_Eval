#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <mpi.h>
#include "transformation_proc.h"

#define NUM_THREADS 13

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const char *input_dir;
    const char *output_dir;
    const char *arc_file_path;
    int kernel, total_imgs;

    // Solo el maestro lee los argumentos
    if (rank == 0) {
        if (argc != 6) {
            fprintf(stderr, "Uso: %s <input_dir> <output_dir> <arc_file_path> <kernel> <total_imgs>\n", argv[0]);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        input_dir = argv[1];
        output_dir = argv[2];
        arc_file_path = argv[3];
        kernel = atoi(argv[4]);
        total_imgs = atoi(argv[5]);

        if (kernel < 1 || total_imgs < 1) {
            fprintf(stderr, "Error: kernel y total_imgs deben ser mayores que 0 (kernel=%d, total_imgs=%d)\n", kernel, total_imgs);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    // Variables globales compartidas
    char input_dir_buf[256], output_dir_buf[256], arc_file_path_buf[256];
    
    if (rank == 0) {
        strncpy(input_dir_buf, input_dir, sizeof(input_dir_buf));
        strncpy(output_dir_buf, output_dir, sizeof(output_dir_buf));
        strncpy(arc_file_path_buf, arc_file_path, sizeof(arc_file_path_buf));
    }

    // Broadcast de valores
    MPI_Bcast(&kernel, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&total_imgs, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(input_dir_buf, 256, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(output_dir_buf, 256, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(arc_file_path_buf, 256, MPI_CHAR, 0, MPI_COMM_WORLD);

    // Solo el maestro escribe el encabezado en el archivo
    if (rank == 0) {
        FILE *fptr = fopen(arc_file_path_buf, "w");
        if (fptr == NULL) {
            printf("Error: No pudo abrir arc1.txt en %s\n", arc_file_path_buf);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        fprintf(fptr, "César Guerra Martínez - A01656774\n");
        fprintf(fptr, "Gerardo Deustúa Hernández - A01736455\n");
        fprintf(fptr, "José Luis Zago Guevara - A01736278\n\n");
        fclose(fptr);
    }

    MPI_Barrier(MPI_COMM_WORLD); // Esperar a que el archivo esté listo

    // Variables para contar
    unsigned long long total_leidas = 0, total_escritas = 0;
    omp_set_num_threads(NUM_THREADS);
    double start_time = omp_get_wtime();

    // Repartir imágenes entre procesos MPI
    #pragma omp parallel for reduction(+:total_leidas, total_escritas)
    for (int i = rank; i < total_imgs; i += size) {
        char originalFile[256];
        char grayFile[256];
        char invHColorFile[256];
        char invVColorFile[256];
        char invHGrayFile[256];
        char invVGrayFile[256];
        char blurFile[256];
        int leidas = 0;
        int escritas = 0;

        snprintf(originalFile, sizeof(originalFile), "%s/%d.bmp", input_dir_buf, i);
        snprintf(grayFile, sizeof(grayFile), "%d_gray", i);
        snprintf(invHColorFile, sizeof(invHColorFile), "%d_invHColor", i);
        snprintf(invVColorFile, sizeof(invVColorFile), "%d_invVColor", i);
        snprintf(invHGrayFile, sizeof(invHGrayFile), "%d_invHGray", i);
        snprintf(invVGrayFile, sizeof(invVGrayFile), "%d_invVGray", i);
        snprintf(blurFile, sizeof(blurFile), "%d_blur", i);

        FILE *check_file = fopen(originalFile, "rb");
        if (check_file == NULL) {
            fprintf(stderr, "[Rank %d] Warning: Saltando archivo inexistente %s\n", rank, originalFile);
            continue;
        }
        fclose(check_file);

        gray_img(grayFile, originalFile, output_dir_buf, &leidas, &escritas); total_leidas += leidas; total_escritas += escritas;
        invH_color_img(invHColorFile, originalFile, output_dir_buf, &leidas, &escritas); total_leidas += leidas; total_escritas += escritas;
        invV_color_img(invVColorFile, originalFile, output_dir_buf, &leidas, &escritas); total_leidas += leidas; total_escritas += escritas;
        invH_gray_img(invHGrayFile, originalFile, output_dir_buf, &leidas, &escritas); total_leidas += leidas; total_escritas += escritas;
        invV_gray_img(invVGrayFile, originalFile, output_dir_buf, &leidas, &escritas); total_leidas += leidas; total_escritas += escritas;
        blur_img(blurFile, originalFile, kernel, output_dir_buf, &leidas, &escritas); total_leidas += leidas; total_escritas += escritas;
    }

    double end_time = omp_get_wtime();
    double total_processing_time = end_time - start_time;
    unsigned long long total_bytes = total_leidas + total_escritas;

    double *all_times = NULL;
    if (rank == 0) {
        all_times = (double *)malloc(sizeof(double) * size);
        if (all_times == NULL) {
            fprintf(stderr, "Error: Memory allocation failed for all_times on rank 0.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    // Gather all processing times to rank 0
    MPI_Gather(&total_processing_time, 1, MPI_DOUBLE, all_times, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int max_time_rank = 0;
    double max_time = 0.0;

    if (rank == 0) {
        max_time = all_times[0];
        for (int i = 1; i < size; ++i) {
            if (all_times[i] > max_time) {
                max_time = all_times[i];
                max_time_rank = i;
            }
        }
        free(all_times);
    }

    MPI_Bcast(&max_time_rank, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == max_time_rank) {
        FILE *fptr_append = fopen(arc_file_path_buf, "a");
        if (fptr_append == NULL) {
            printf("Error: No pudo abrir arc1.txt en %s para escribir el resultado final.\n", arc_file_path_buf);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        double rate_bytes_processed = 0.0;
        if (total_processing_time > 0) {
            rate_bytes_processed = (double)(total_bytes / total_processing_time) / 1000000;
        }

        fprintf(fptr_append, "[Nodo %d] (Tiempo Máximo)\n", rank);
        fprintf(fptr_append, "Localidades leídas: %E\n", (double)total_leidas);
        fprintf(fptr_append, "Localidades escritas: %E\n", (double)total_escritas);
        fprintf(fptr_append, "Tiempo procesamiento: %.2f s\n", total_processing_time);
        fprintf(fptr_append, "Tasa MB/s: %.2f\n", rate_bytes_processed);
        fprintf(fptr_append, "MIPS: %.2f\n\n", (20.0 * total_bytes) / (1000000.0 * total_processing_time));

        fclose(fptr_append);
    }


    MPI_Finalize();
    return 0;
}
