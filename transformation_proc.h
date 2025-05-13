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

extern void gray_img(char mask[10], char path[80], int *leidas, int *escritas) {
    FILE *image, *outputImage;
    char add_char[80] = "./r_img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    image = fopen(path,"rb");          //Original Image
    outputImage = fopen(add_char,"wb");

	//Definition of variables
	int i;
    long ancho, alto;
    unsigned char r, g, b, pixel;               //Pixel

    unsigned char xx[54];
    fread(xx, sizeof(unsigned char), 54, image);
    fwrite(xx, sizeof(unsigned char), 54, outputImage);

    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];

    int tam = ancho * alto;

    unsigned char *arr_in = (unsigned char*)malloc(tam * 3);
    unsigned char *arr_out = (unsigned char*)malloc(tam * 3);

    fread(arr_in, sizeof(unsigned char), tam * 3, image);
    *leidas = tam * 3;
    
    for (i = 0; i < ancho * alto; i++) {
        r = arr_in[i * 3 + 2];
        g = arr_in[i * 3 + 1];
        b = arr_in[i * 3 + 0];
        pixel = 0.21 * r + 0.72 * g + 0.07 * b;
        arr_out[i] = pixel;
    }

    for (i = 0; i < ancho * alto; i++) {
        fputc(arr_out[i], outputImage);
        fputc(arr_out[i], outputImage);
        fputc(arr_out[i], outputImage);
    }
    *escritas = tam * 3;

    free(arr_in);
    free(arr_out);

    fclose(image);
    fclose(outputImage);

}

extern void invH_gray_img(char mask[10], char path[80], int *leidas, int *escritas) {
    FILE *image, *outputImage;
    char add_char[80] = "./r_img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    image = fopen(path,"rb");          //Original Image
    outputImage = fopen(add_char,"wb");

	//Definition of variables
	int i, j;
    long ancho, alto;
    unsigned char r, g, b, pixel;               //Pixel

    unsigned char xx[54];
    fread(xx, sizeof(unsigned char), 54, image);
    fwrite(xx, sizeof(unsigned char), 54, outputImage);

    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];

    int padding = (4 - (ancho * 3) % 4) % 4;
    int tam = ancho * alto;

    unsigned char *arr_in = (unsigned char*)malloc(tam * 3);
    unsigned char *arr_out = (unsigned char*)malloc(tam * 3);
    
    fread(arr_in, sizeof(unsigned char), tam * 3, image);
    *leidas = tam * 3;
   
    for (i = 0; i < ancho * alto; i++) {
        r = arr_in[i * 3 + 2];
        g = arr_in[i * 3 + 1];
        b = arr_in[i * 3 + 0];
        pixel = 0.21 * r + 0.72 * g + 0.07 * b;
        arr_out[i] = pixel;
    }

    for (i = 0; i < alto; i++) {
        for (j = ancho; j > 0; j--) {
            int index = (i * ancho) + j;
            fputc(arr_out[index], outputImage);
            fputc(arr_out[index], outputImage);
            fputc(arr_out[index], outputImage);
        }
        for (int p = 0; p < padding; p++) {
            fputc(0x00, outputImage);
        }
    }
    *escritas = tam * 3;

    free(arr_in);
    free(arr_out);

    fclose(image);
    fclose(outputImage);

}

extern void invV_gray_img(char mask[10], char path[80], int *leidas, int *escritas) {
    FILE *image, *outputImage;
    char add_char[80] = "./r_img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    image = fopen(path,"rb");          //Original Image
    outputImage = fopen(add_char,"wb");

	//Definition of variables
	int i, j;
    long ancho, alto;
    unsigned char r, g, b, pixel;               //Pixel

    unsigned char xx[54];
    fread(xx, sizeof(unsigned char), 54, image);
    fwrite(xx, sizeof(unsigned char), 54, outputImage);

    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];

    int padding = (4 - (ancho * 3) % 4) % 4;
    int tam = ancho * alto;

    unsigned char *arr_in = (unsigned char*)malloc(tam * 3);
    unsigned char *arr_out = (unsigned char*)malloc(tam * 3);
    
    fread(arr_in, sizeof(unsigned char), tam * 3, image);
    *leidas = tam * 3;
   
    for (i = 0; i < ancho*alto; i++) {
        r = arr_in[i * 3 + 2];
        g = arr_in[i * 3 + 1];
        b = arr_in[i * 3 + 0];
        pixel = 0.21 * r + 0.72 * g + 0.07 * b;
        arr_out[i] = pixel;
    }

    for (i = alto - 1; i > 0; i--) {
        for (j = 0; j < ancho; j++) {
            int index = (i * ancho) + j;
            fputc(arr_out[index], outputImage);
            fputc(arr_out[index], outputImage);
            fputc(arr_out[index], outputImage);
        }
        for (int p = 0; p < padding; p++) {
            fputc(0x00, outputImage);
        }
    }
    *escritas = tam * 3;

    free(arr_in);
    free(arr_out);

    fclose(image);
    fclose(outputImage);

}

extern void invH_color_img(char mask[10], char path[80], int *leidas, int *escritas) {
    FILE *image, *outputImage;
    char add_char[80] = "./r_img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    image = fopen(path,"rb");          //Original Image
    outputImage = fopen(add_char,"wb");

	//Definition of variables
	int i, j, k;
    long ancho, alto;
    unsigned char r, g, b, pixel;               //Pixel

    unsigned char xx[54];
    fread(xx, sizeof(unsigned char), 54, image);
    fwrite(xx, sizeof(unsigned char), 54, outputImage);

    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];

    int padding = (4 - (ancho * 3) % 4) % 4;
    int tam = ancho * alto;

    unsigned char *arr_in_b = (unsigned char*)malloc(tam);
    unsigned char *arr_in_g = (unsigned char*)malloc(tam);
    unsigned char *arr_in_r = (unsigned char*)malloc(tam);
   
    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            b = fgetc(image);
            g = fgetc(image);
            r = fgetc(image);

            int index = (i * ancho) + j;
            arr_in_b[index] = b;
            arr_in_g[index] = g;
            arr_in_r[index] = r;
        }
        fseek(image, padding, SEEK_CUR);
    }
    *leidas = tam * 3;

    for (i = 0; i < alto; i++) {
        for (k = ancho; k > 0; k--) {
            int index = (i * ancho) + k;
            fputc(arr_in_b[index], outputImage);
            fputc(arr_in_g[index], outputImage);
            fputc(arr_in_r[index], outputImage);
        }
        for (int p = 0; p < padding; p++) {
            fputc(0x00, outputImage);
        }
    }
    *escritas = tam * 3;

    free(arr_in_b);
    free(arr_in_g);
    free(arr_in_r);

    fclose(image);
    fclose(outputImage);

}

extern void invV_color_img(char mask[10], char path[80], int *leidas, int *escritas) {
    FILE *image, *outputImage;
    char add_char[80] = "./r_img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    image = fopen(path,"rb");          //Original Image
    outputImage = fopen(add_char,"wb");

	//Definition of variables
	int i, j, k;
    long ancho, alto;
    unsigned char r, g, b, pixel;               //Pixel

    unsigned char xx[54];
    fread(xx, sizeof(unsigned char), 54, image);
    fwrite(xx, sizeof(unsigned char), 54, outputImage);

    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];

    int padding = (4 - (ancho * 3) % 4) % 4;
    int tam = ancho * alto;

    unsigned char *arr_in_b = (unsigned char*)malloc(tam);
    unsigned char *arr_in_g = (unsigned char*)malloc(tam);
    unsigned char *arr_in_r = (unsigned char*)malloc(tam);

    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            b = fgetc(image);
            g = fgetc(image);
            r = fgetc(image);

            int index = (i * ancho) + j;
            arr_in_b[index] = b;
            arr_in_g[index] = g;
            arr_in_r[index] = r;
        }
        fseek(image, padding, SEEK_CUR);
    }
    *leidas = tam * 3;


    for (i = alto - 1; i > 0; i--) {
        for (k = 0; k < ancho; k++) {
            int index = (i * ancho) + k;
            fputc(arr_in_b[index], outputImage);
            fputc(arr_in_g[index], outputImage);
            fputc(arr_in_r[index], outputImage);
        }
        for (int p = 0; p < padding; p++) {
            fputc(0x00, outputImage);
        }
    }
    *escritas = tam * 3;

    free(arr_in_b);
    free(arr_in_g);
    free(arr_in_r);

    fclose(image);
    fclose(outputImage);

}

extern void blur_img(char mask[10], char path[80], int kernel, int *leidas, int *escritas) {
    FILE *image, *outputImage;
    char add_char[80] = "./r_img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    image = fopen(path,"rb");          //Original Image
    outputImage = fopen(add_char,"wb");

	//Definition of variables
	int i, j, k;
    long ancho, alto;
    unsigned char r, g, b, pixel;               //Pixel

    unsigned char xx[54];
    fread(xx, sizeof(unsigned char), 54, image);
    fwrite(xx, sizeof(unsigned char), 54, outputImage);
    
    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];

    int padding = (4 - (ancho * 3) % 4) % 4;
    int tam = ancho * alto;

    unsigned char *arr_in_b = malloc(tam);
    unsigned char *arr_in_g = malloc(tam);
    unsigned char *arr_in_r = malloc(tam);
    unsigned char *arr_out_b = malloc(tam);
    unsigned char *arr_out_g = malloc(tam);
    unsigned char *arr_out_r = malloc(tam);

    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            b = fgetc(image);
            g = fgetc(image);
            r = fgetc(image);

            int index = (alto - 1 - i) * ancho + j;
            arr_in_b[index] = b;
            arr_in_g[index] = g;
            arr_in_r[index] = r;
        }
        fseek(image, padding, SEEK_CUR);
    }
    *leidas = (tam * 3) + (kernel * kernel) - 1;

    int kernelRadius = (kernel - 1) / 2;

    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            unsigned int bSum = 0, gSum = 0, rSum = 0;
            int count = 0;

            for (int kx = -kernelRadius; kx <= kernelRadius; kx++) {
                int nx = x + kx;
                if (nx >= 0 && nx < ancho) {
                    int index = y * ancho + nx;
                    bSum += arr_in_b[index];
                    gSum += arr_in_g[index];
                    rSum += arr_in_r[index];
                    count++;
                }
            }

            int index = y * ancho + x;
            arr_out_b[index] = bSum / count;
            arr_out_g[index] = gSum / count;
            arr_out_r[index] = rSum / count;
        }
    }

    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            unsigned int bSum = 0, gSum = 0, rSum = 0;
            int count = 0;

            for (int ky = -kernelRadius; ky <= kernelRadius; ky++) {
                int ny = y + ky;
                if (ny >= 0 && ny < alto) {
                    int index = ny * ancho + x;
                    bSum += arr_out_b[index];
                    gSum += arr_out_g[index];
                    rSum += arr_out_r[index];
                    count++;
                }
            }

            int index = y * ancho + x;
            arr_out_b[index] = bSum / count;
            arr_out_g[index] = gSum / count;
            arr_out_r[index] = rSum / count;
        }
    }

    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            int index = (alto - 1 - i) * ancho + j;
            fputc(arr_out_b[index], outputImage);
            fputc(arr_out_g[index], outputImage);
            fputc(arr_out_r[index], outputImage);
        }
        for (int p = 0; p < padding; p++) {
            fputc(0x00, outputImage);
        }
    }
    *escritas = tam * 3;

    free(arr_in_b);
    free(arr_in_g);
    free(arr_in_r);
    free(arr_out_b);
    free(arr_out_g);
    free(arr_out_r);

    fclose(image);
    fclose(outputImage);

}
