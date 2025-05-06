//Emmanuel Torres
//Tecnologico de Monterrey
//Campus Puebla
//Octubre 2024
extern void itoa2(int N, char *str) {
    int i = 0;
  
    // Save the copy of the number for sign
    int sign = N;

    // If the number is negative, make it positive
    if (N < 0)
        N = -N;

    // Extract digits from the number and add them to the
    // string
    while (N > 0) {
      
        // Convert integer digit to character and store
      	// it in the str
        str[i++] = N % 10 + '0';
      	N /= 10;
    } 

    // If the number was negative, add a minus sign to the
    // string
    if (sign < 0) {
        str[i++] = '-';
    }

    // Null-terminate the string
    str[i] = '\0';

    // Reverse the string to get the correct order
    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
    }
}

extern void gray_img(char mask[10], char path[80], FILE* file) {
    FILE *image, *outputImage, *lecturas, *fptr;
    char add_char[80] = "./r_img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    image = fopen(path,"rb");          //Original Image
    outputImage = fopen(add_char,"wb");

	//Definition of variables
	int i, tam1;
    long ancho, alto, tam, bpp;
    unsigned char r, g, b, pixel;               //Pixel

    unsigned char xx[54];
    fread(xx, sizeof(unsigned char), 54, image);
    fwrite(xx, sizeof(unsigned char), 54, outputImage);

    tam = (long)xx[4]*65536+(long)xx[3]*256+(long)xx[2];
    bpp = (long)xx[29]*256+(long)xx[28];
    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
    tam1 = tam;

    unsigned char* arr_in = (unsigned char*)malloc(ancho*alto*3*sizeof(unsigned char));
    fread(arr_in, sizeof(unsigned char), ancho*alto*3, image);
    
    unsigned char* arr_out = (unsigned char*)malloc(ancho*alto*sizeof(unsigned char));

    #pragma omp parallel for private(r, g, b, pixel)
        for (i = 0; i < ancho*alto; i++) {
            r = arr_in[i * 3 + 2];
            g = arr_in[i * 3 + 1];
            b = arr_in[i * 3 + 0];
            pixel = 0.21 * r + 0.72 * g + 0.07 * b;
            arr_out[i] = pixel;
        }

    for (i = 0; i < ancho*alto; i++) {
        fputc(arr_out[i], outputImage);
        fputc(arr_out[i], outputImage);
        fputc(arr_out[i], outputImage);
    }

    #pragma omp critical
        {
            fprintf(file, "%s\n", add_char);
            fprintf(file, "tamano archivo %li\n", tam);
            fprintf(file, "bits por pixel %li\n", bpp);
            fprintf(file, "largo img %li\n", alto);
            fprintf(file, "ancho img %li\n", ancho);
            fprintf(file, "lectura de datos: %d\n", ancho * alto * 3);
            fprintf(file, "elementos faltantes: %d\n", tam1 - (ancho * alto * 3));
        }

    free(arr_in);
    free(arr_out);

    fclose(image);
    fclose(outputImage);

}

extern void invH_gray_img(char mask[10], char path[80], FILE* file) {
    FILE *image, *outputImage, *lecturas, *fptr;
    char add_char[80] = "./r_img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    image = fopen(path,"rb");          //Original Image
    outputImage = fopen(add_char,"wb");

	//Definition of variables
	int i, j, tam1;
    long ancho, alto, tam, bpp;
    unsigned char r, g, b, pixel;               //Pixel

    unsigned char xx[54];
    fread(xx, sizeof(unsigned char), 54, image);
    fwrite(xx, sizeof(unsigned char), 54, outputImage);

    tam = (long)xx[4]*65536+(long)xx[3]*256+(long)xx[2];
    bpp = (long)xx[29]*256+(long)xx[28];
    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
    tam1 = tam;

    unsigned char* arr_in = (unsigned char*)malloc(ancho*alto*3*sizeof(unsigned char));
    fread(arr_in, sizeof(unsigned char), ancho*alto*3, image);
    
    unsigned char* arr_out = (unsigned char*)malloc(ancho*alto*sizeof(unsigned char));

   
    #pragma omp parallel for private(r, g, b, pixel)
        for (i = 0; i < ancho*alto; i++) {
            r = arr_in[i * 3 + 2];
            g = arr_in[i * 3 + 1];
            b = arr_in[i * 3 + 0];
            pixel = 0.21 * r + 0.72 * g + 0.07 * b;
            arr_out[i] = pixel;
        }

    for (i = alto - 1; i > 0; i--) {
        for (j = 0; j < ancho; j++) {
            fputc(arr_out[(i * ancho) + j], outputImage);
            fputc(arr_out[(i * ancho) + j], outputImage);
            fputc(arr_out[(i * ancho) + j], outputImage);
        }
    }

    #pragma omp critical
        {
            fprintf(file, "%s\n", add_char);
            fprintf(file, "tamano archivo %li\n", tam);
            fprintf(file, "bits por pixel %li\n", bpp);
            fprintf(file, "largo img %li\n", alto);
            fprintf(file, "ancho img %li\n", ancho);
            fprintf(file, "lectura de datos: %d\n", ancho * alto * 3);
            fprintf(file, "elementos faltantes: %d\n", tam1 - (ancho * alto * 3));
        }

    free(arr_in);
    free(arr_out);

    fclose(image);
    fclose(outputImage);

}

extern void invV_gray_img(char mask[10], char path[80], FILE* file) {
    FILE *image, *outputImage, *lecturas, *fptr;
    char add_char[80] = "./r_img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    image = fopen(path,"rb");          //Original Image
    outputImage = fopen(add_char,"wb");

	//Definition of variables
	int i, j, tam1;
    long ancho, alto, tam, bpp;
    unsigned char r, g, b, pixel;               //Pixel

    unsigned char xx[54];
    fread(xx, sizeof(unsigned char), 54, image);
    fwrite(xx, sizeof(unsigned char), 54, outputImage);

    tam = (long)xx[4]*65536+(long)xx[3]*256+(long)xx[2];
    bpp = (long)xx[29]*256+(long)xx[28];
    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
    tam1 = tam;

    unsigned char* arr_in = (unsigned char*)malloc(ancho*alto*3*sizeof(unsigned char));
    fread(arr_in, sizeof(unsigned char), ancho*alto*3, image);
    
    unsigned char* arr_out = (unsigned char*)malloc(ancho*alto*sizeof(unsigned char));
   
    #pragma omp parallel for private(r, g, b, pixel)
        for (i = 0; i < ancho*alto; i++) {
            r = arr_in[i * 3 + 2];
            g = arr_in[i * 3 + 1];
            b = arr_in[i * 3 + 0];
            pixel = 0.21 * r + 0.72 * g + 0.07 * b;
            arr_out[i] = pixel;
        }

    for (i = 0; i < alto; i++) {
        for (j = ancho; j > 0; j--) {
            fputc(arr_out[(i * ancho) + j], outputImage);
            fputc(arr_out[(i * ancho) + j], outputImage);
            fputc(arr_out[(i * ancho) + j], outputImage);
        }
    }

    #pragma omp critical
        {
            fprintf(file, "%s\n", add_char);
            fprintf(file, "tamano archivo %li\n", tam);
            fprintf(file, "bits por pixel %li\n", bpp);
            fprintf(file, "largo img %li\n", alto);
            fprintf(file, "ancho img %li\n", ancho);
            fprintf(file, "lectura de datos: %d\n", ancho * alto * 3);
            fprintf(file, "elementos faltantes: %d\n", tam1 - (ancho * alto * 3));
        }

    free(arr_in);
    free(arr_out);

    fclose(image);
    fclose(outputImage);

}

extern void invH_color_img(char mask[10], char path[80], FILE* file) {
    FILE *image, *outputImage, *lecturas, *fptr;
    char add_char[80] = "./r_img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    image = fopen(path,"rb");          //Original Image
    outputImage = fopen(add_char,"wb");

	//Definition of variables
	int i, j, k, tam1;
    long ancho, alto, tam, bpp;
    unsigned char r, g, b, pixel;               //Pixel

    unsigned char xx[54];
    fread(xx, sizeof(unsigned char), 54, image);
    fwrite(xx, sizeof(unsigned char), 54, outputImage);

    tam = (long)xx[4]*65536+(long)xx[3]*256+(long)xx[2];
    bpp = (long)xx[29]*256+(long)xx[28];
    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
    tam1 = tam;

    unsigned char* arr_in_b = (unsigned char*)malloc(ancho*alto*sizeof(unsigned char));
    unsigned char* arr_in_g = (unsigned char*)malloc(ancho*alto*sizeof(unsigned char));
    unsigned char* arr_in_r = (unsigned char*)malloc(ancho*alto*sizeof(unsigned char));
   
    j = 0;

    while (!feof(image)) {
        b = fgetc(image);
        g = fgetc(image);
        r = fgetc(image);
        
        arr_in_b[j] = b;
        arr_in_g[j] = g;
        arr_in_r[j] = r;
        j++;
    }


    for (i = alto - 1; i > 0; i--) {
        for (k = 0; k < ancho; k++) {
            fputc(arr_in_b [(i * ancho) + k], outputImage);
            fputc(arr_in_g [(i * ancho) + k], outputImage);
            fputc(arr_in_r [(i * ancho) + k], outputImage);
        }
    }

    #pragma omp critical
        {
            fprintf(file, "%s\n", add_char);
            fprintf(file, "tamano archivo %li\n", tam);
            fprintf(file, "bits por pixel %li\n", bpp);
            fprintf(file, "largo img %li\n", alto);
            fprintf(file, "ancho img %li\n", ancho);
            fprintf(file, "lectura de datos: %d\n", j * 3);
            fprintf(file, "elementos faltantes: %d\n", tam1 - (j * 3));
        }

    free(arr_in_b);
    free(arr_in_g);
    free(arr_in_r);

    fclose(image);
    fclose(outputImage);

}

extern void invV_color_img(char mask[10], char path[80], FILE* file) {
    FILE *image, *outputImage, *lecturas, *fptr;
    char add_char[80] = "./r_img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    image = fopen(path,"rb");          //Original Image
    outputImage = fopen(add_char,"wb");

	//Definition of variables
	int i, j, k, tam1;
    long ancho, alto, tam, bpp;
    unsigned char r, g, b, pixel;               //Pixel

    unsigned char xx[54];
    fread(xx, sizeof(unsigned char), 54, image);
    fwrite(xx, sizeof(unsigned char), 54, outputImage);

    tam = (long)xx[4]*65536+(long)xx[3]*256+(long)xx[2];
    bpp = (long)xx[29]*256+(long)xx[28];
    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
    tam1 = tam;

    unsigned char* arr_in_b = (unsigned char*)malloc(ancho*alto*sizeof(unsigned char));
    unsigned char* arr_in_g = (unsigned char*)malloc(ancho*alto*sizeof(unsigned char));
    unsigned char* arr_in_r = (unsigned char*)malloc(ancho*alto*sizeof(unsigned char));
   
    j = 0;

    while (!feof(image)) {
        b = fgetc(image);
        g = fgetc(image);
        r = fgetc(image);
        
        arr_in_b[j] = b;
        arr_in_g[j] = g;
        arr_in_r[j] = r;
        j++;
    }

    for (i = 0; i < alto; i++) {
        for (k = ancho; k > 0; k--) {
            fputc(arr_in_b [(i * ancho) + k], outputImage);
            fputc(arr_in_g [(i * ancho) + k], outputImage);
            fputc(arr_in_r [(i * ancho) + k], outputImage);
        }
    }

    #pragma omp critical
        {
            fprintf(file, "%s\n", add_char);
            fprintf(file, "tamano archivo %li\n", tam);
            fprintf(file, "bits por pixel %li\n", bpp);
            fprintf(file, "largo img %li\n", alto);
            fprintf(file, "ancho img %li\n", ancho);
            fprintf(file, "lectura de datos: %d\n", j * 3);
            fprintf(file, "elementos faltantes: %d\n", tam1 - (j * 3));
        }

    free(arr_in_b);
    free(arr_in_g);
    free(arr_in_r);

    fclose(image);
    fclose(outputImage);

}

extern void blur_img(char mask[10], char path[80], FILE* file, int kernel) {
    FILE *image, *outputImage, *lecturas, *fptr;
    char add_char[80] = "./r_img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    image = fopen(path,"rb");          //Original Image
    outputImage = fopen(add_char,"wb");

	//Definition of variables
	int i, j, k, tam1;
    long ancho, alto, tam, bpp;
    unsigned char r, g, b, pixel;               //Pixel

    unsigned char xx[54];
    fread(xx, sizeof(unsigned char), 54, image);
    fwrite(xx, sizeof(unsigned char), 54, outputImage);
    
    tam = (long)xx[4]*65536+(long)xx[3]*256+(long)xx[2];
    bpp = (long)xx[29]*256+(long)xx[28];
    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
    tam1 = tam;

    unsigned char* arr_in_b = (unsigned char*)malloc(ancho * alto * sizeof(unsigned char));
    unsigned char* arr_in_g = (unsigned char*)malloc(ancho * alto * sizeof(unsigned char));
    unsigned char* arr_in_r = (unsigned char*)malloc(ancho * alto * sizeof(unsigned char));
    unsigned char* arr_out_b = (unsigned char*)malloc(ancho * alto * sizeof(unsigned char));
    unsigned char* arr_out_g = (unsigned char*)malloc(ancho * alto * sizeof(unsigned char));
    unsigned char* arr_out_r = (unsigned char*)malloc(ancho * alto * sizeof(unsigned char));
    
    j = 0;

    while (!feof(image)) {
        b = fgetc(image);
        g = fgetc(image);
        r = fgetc(image);
        
        arr_in_b[j] = b;
        arr_in_g[j] = g;
        arr_in_r[j] = r;
        j++;
    }

    int kernelRadius = (kernel - 1) / 2;

    #pragma omp parallel for collapse(2) // Para los 2 ciclos
        for (int y = 0; y < alto; y++) {
            for (int x = 0; x < ancho; x++) {
                unsigned int bSum = 0;
                unsigned int gSum = 0;
                unsigned int rSum = 0;
                int pond = 0;

                for (int ky = -kernelRadius; ky <= kernelRadius; ky++) {
                    for (int kx = -kernelRadius; kx <= kernelRadius; kx++) {
                        int ny = y + ky;
                        int nx = x + kx;

                        if (ny < 0 || ny >= alto || nx < 0 || nx >= ancho) 
                            continue;

                        bSum += arr_in_b[(ny * ancho) + nx];
                        gSum += arr_in_g[(ny * ancho) + nx];
                        rSum += arr_in_r[(ny * ancho) + nx];
                        pond++;
                    }
                }

                arr_out_b[(y * ancho) + x] = bSum / pond;
                arr_out_g[(y * ancho) + x] = gSum / pond;
                arr_out_r[(y * ancho) + x] = rSum / pond;
            }
        }

    for (i = 0; i < alto; i++) {
        for (k = 0; k < ancho; k++) {
            fputc(arr_out_b[(i * ancho) + k], outputImage);
            fputc(arr_out_g[(i * ancho) + k], outputImage);
            fputc(arr_out_r[(i * ancho) + k], outputImage);
        }
    }

    #pragma omp critical
        {
            fprintf(file, "%s\n", add_char);
            fprintf(file, "tamano archivo %li\n", tam);
            fprintf(file, "bits por pixel %li\n", bpp);
            fprintf(file, "largo img %li\n", alto);
            fprintf(file, "ancho img %li\n", ancho);
            fprintf(file, "lectura de datos: %d\n", j * 3);
            fprintf(file, "elementos faltantes: %d\n", tam1 - (j * 3));
        }
        
    free(arr_in_b);
    free(arr_in_g);
    free(arr_in_r);
    free(arr_out_b);
    free(arr_out_g);
    free(arr_out_r);

    fclose(image);
    fclose(outputImage);
}
