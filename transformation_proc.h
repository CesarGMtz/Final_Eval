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
    char add_char[80] = "./img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    fprintf(file, "%s\n", add_char);
    image = fopen(path,"rb");          //Original Image
    outputImage = fopen(add_char,"wb");

	//Definition of variables
	int i, j, tam1;
    long ancho, tam, bpp;
    long alto;
    unsigned char r, g, b, pixel;               //Pixel

    unsigned char xx[54];
    for (int i=0; i<54; i++) {
      xx[i] = fgetc(image);
      fputc(xx[i], outputImage);   //Copia cabecera a nueva imagen
    }
    tam = (long)xx[4]*65536+(long)xx[3]*256+(long)xx[2];
    bpp = (long)xx[29]*256+(long)xx[28];
    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
    fprintf(file, "tamano archivo %li\n", tam);
    tam1 = tam;
    fprintf(file, "bits por pixel %li\n", bpp);
    fprintf(file, "largo img %li\n",alto);
    fprintf(file, "ancho img %li\n",ancho);

    unsigned char* arr_in = (unsigned char*)malloc(ancho*alto*sizeof(unsigned char));
   
    j = 0;

    while (!feof(image)) {
        b = fgetc(image);
        g = fgetc(image);
        r = fgetc(image);
        pixel = 0.21 * r + 0.72 * g + 0.07 * b;

        fputc(pixel, outputImage);
        fputc(pixel, outputImage);
        fputc(pixel, outputImage);
        
        arr_in[j] = pixel;
        j++;
    }

    fprintf(file, "lectura de datos: %d\n", j * 3);
    fprintf(file, "elementos faltantes: %d\n", tam1 - (j * 3));
    
    free(arr_in);

    fclose(image);
    fclose(outputImage);

}

extern void invH_gray_img(char mask[10], char path[80], FILE* file) {
    FILE *image, *outputImage, *lecturas, *fptr;
    char add_char[80] = "./img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    fprintf(file, "%s\n", add_char);
    image = fopen(path,"rb");          //Original Image
    outputImage = fopen(add_char,"wb");

	//Definition of variables
	int i, j, tam1;
    long ancho, tam, bpp;
    long alto;
    unsigned char r, g, b, pixel;               //Pixel

    unsigned char xx[54];
    for (int i=0; i<54; i++) {
      xx[i] = fgetc(image);
      fputc(xx[i], outputImage);   //Copia cabecera a nueva imagen
    }
    tam = (long)xx[4]*65536+(long)xx[3]*256+(long)xx[2];
    bpp = (long)xx[29]*256+(long)xx[28];
    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
    fprintf(file, "tamano archivo %li\n", tam);
    tam1 = tam;
    fprintf(file, "bits por pixel %li\n", bpp);
    fprintf(file, "largo img %li\n",alto);
    fprintf(file, "ancho img %li\n",ancho);

    unsigned char* arr_in = (unsigned char*)malloc(ancho*alto*sizeof(unsigned char));
   
    j = 0;

    while (!feof(image)) {
        b = fgetc(image);
        g = fgetc(image);
        r = fgetc(image);
        pixel = 0.21 * r + 0.72 * g + 0.07 * b;
        arr_in[j] = pixel;
        j++;
    }

    fprintf(file, "lectura de datos: %d\n", j * 3);
    fprintf(file, "elementos faltantes: %d\n", tam1 - (j * 3));

    for (int i = alto; i > 0; i--) {
        
        for (int k = 0; k < ancho; k++) {
            fputc(arr_in [(i * ancho) + k], outputImage);
            fputc(arr_in [(i * ancho) + k], outputImage);
            fputc(arr_in [(i * ancho) + k], outputImage);
        }
        
    }

    free(arr_in);

    fclose(image);
    fclose(outputImage);

}

extern void invV_gray_img(char mask[10], char path[80], FILE* file) {
    FILE *image, *outputImage, *lecturas, *fptr;
    char add_char[80] = "./img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    fprintf(file, "%s\n", add_char);
    image = fopen(path,"rb");          //Original Image
    outputImage = fopen(add_char,"wb");

	//Definition of variables
	int i, j, tam1;
    long ancho, tam, bpp;
    long alto;
    unsigned char r, g, b, pixel;               //Pixel

    unsigned char xx[54];
    for (int i=0; i<54; i++) {
      xx[i] = fgetc(image);
      fputc(xx[i], outputImage);   //Copia cabecera a nueva imagen
    }
    tam = (long)xx[4]*65536+(long)xx[3]*256+(long)xx[2];
    bpp = (long)xx[29]*256+(long)xx[28];
    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
    fprintf(file, "tamano archivo %li\n", tam);
    tam1 = tam;
    fprintf(file, "bits por pixel %li\n", bpp);
    fprintf(file, "largo img %li\n",alto);
    fprintf(file, "ancho img %li\n",ancho);

    unsigned char* arr_in = (unsigned char*)malloc(ancho*alto*sizeof(unsigned char));
   
    j = 0;

    while (!feof(image)) {
        b = fgetc(image);
        g = fgetc(image);
        r = fgetc(image);
        pixel = 0.21 * r + 0.72 * g + 0.07 * b;
        arr_in[j] = pixel;
        j++;
    }

    fprintf(file, "lectura de datos: %d\n", j * 3);
    fprintf(file, "elementos faltantes: %d\n", tam1 - (j * 3));

    for (int i = 0; i < alto; i++) {
        
        for (int k = ancho; k > 0; k--) {
            fputc(arr_in [(i * ancho) + k], outputImage);
            fputc(arr_in [(i * ancho) + k], outputImage);
            fputc(arr_in [(i * ancho) + k], outputImage);
        }
        
    }

    free(arr_in);

    fclose(image);
    fclose(outputImage);

}

extern void invH_color_img(char mask[10], char path[80], FILE* file) {
    FILE *image, *outputImage, *lecturas, *fptr;
    char add_char[80] = "./img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    fprintf(file, "%s\n", add_char);
    image = fopen(path,"rb");          //Original Image
    outputImage = fopen(add_char,"wb");

	//Definition of variables
	int i, j, tam1;
    long ancho, tam, bpp;
    long alto;
    unsigned char r, g, b, pixel;               //Pixel

    unsigned char xx[54];
    for (int i=0; i<54; i++) {
      xx[i] = fgetc(image);
      fputc(xx[i], outputImage);   //Copia cabecera a nueva imagen
    }
    tam = (long)xx[4]*65536+(long)xx[3]*256+(long)xx[2];
    bpp = (long)xx[29]*256+(long)xx[28];
    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
    fprintf(file, "tamano archivo %li\n", tam);
    tam1 = tam;
    fprintf(file, "bits por pixel %li\n", bpp);
    fprintf(file, "largo img %li\n",alto);
    fprintf(file, "ancho img %li\n",ancho);

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

    fprintf(file, "lectura de datos: %d\n", j * 3);
    fprintf(file, "elementos faltantes: %d\n", tam1 - (j * 3));

    for (int i = alto; i > 0; i--) {
        
        for (int k = 0; k < ancho; k++) {
            fputc(arr_in_b [(i * ancho) + k], outputImage);
            fputc(arr_in_g [(i * ancho) + k], outputImage);
            fputc(arr_in_r [(i * ancho) + k], outputImage);
        }
        
    }

    free(arr_in_b);
    free(arr_in_g);
    free(arr_in_r);

    fclose(image);
    fclose(outputImage);

}

extern void invV_color_img(char mask[10], char path[80], FILE* file) {
    FILE *image, *outputImage, *lecturas, *fptr;
    char add_char[80] = "./img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    fprintf(file, "%s\n", add_char);
    image = fopen(path,"rb");          //Original Image
    outputImage = fopen(add_char,"wb");

	//Definition of variables
	int i, j, tam1;
    long ancho, tam, bpp;
    long alto;
    unsigned char r, g, b, pixel;               //Pixel

    unsigned char xx[54];
    for (int i=0; i<54; i++) {
      xx[i] = fgetc(image);
      fputc(xx[i], outputImage);   //Copia cabecera a nueva imagen
    }
    tam = (long)xx[4]*65536+(long)xx[3]*256+(long)xx[2];
    bpp = (long)xx[29]*256+(long)xx[28];
    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
    fprintf(file, "tamano archivo %li\n", tam);
    tam1 = tam;
    fprintf(file, "bits por pixel %li\n", bpp);
    fprintf(file, "largo img %li\n",alto);
    fprintf(file, "ancho img %li\n",ancho);

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

    fprintf(file, "lectura de datos: %d\n", j * 3);
    fprintf(file, "elementos faltantes: %d\n", tam1 - (j * 3));
    
    for (int i = 0; i < alto; i++) {
        
        for (int k = ancho; k > 0; k--) {
            fputc(arr_in_b [(i * ancho) + k], outputImage);
            fputc(arr_in_g [(i * ancho) + k], outputImage);
            fputc(arr_in_r [(i * ancho) + k], outputImage);
        }
        
    }

    free(arr_in_b);
    free(arr_in_g);
    free(arr_in_r);

    fclose(image);
    fclose(outputImage);

}

extern void blur_img(char mask[10], char path[80], FILE* file, int kernel) {
    FILE *image, *outputImage, *lecturas, *fptr;
    char add_char[80] = "./img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    fprintf(file, "%s\n", add_char);
    image = fopen(path,"rb");          //Original Image
    outputImage = fopen(add_char,"wb");

	//Definition of variables
	int i, j, tam1;
    long ancho, tam, bpp;
    long alto;
    unsigned char r, g, b, pixel;               //Pixel

    unsigned char xx[54];
    for (int i=0; i<54; i++) {
      xx[i] = fgetc(image);
      fputc(xx[i], outputImage);   //Copia cabecera a nueva imagen
    }
    tam = (long)xx[4]*65536+(long)xx[3]*256+(long)xx[2];
    bpp = (long)xx[29]*256+(long)xx[28];
    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
    fprintf(file, "tamano archivo %li\n", tam);
    tam1 = tam;
    fprintf(file, "bits por pixel %li\n", bpp);
    fprintf(file, "largo img %li\n",alto);
    fprintf(file, "ancho img %li\n",ancho);

    int rowPadding = (4 - (ancho * 3) % 4) % 4;

    unsigned char* arr_in_b = (unsigned char*)malloc(ancho * alto * sizeof(unsigned char));
    unsigned char* arr_in_g = (unsigned char*)malloc(ancho * alto * sizeof(unsigned char));
    unsigned char* arr_in_r = (unsigned char*)malloc(ancho * alto * sizeof(unsigned char));

    j = 0;
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            unsigned char b = fgetc(image);
            unsigned char g = fgetc(image);
            unsigned char r = fgetc(image);

            arr_in_b[j] = b;
            arr_in_g[j] = g;
            arr_in_r[j] = r;
            j++;
        }

        fseek(image, rowPadding, SEEK_CUR);
    }

    fprintf(file, "lectura de datos: %d\n", j * 3);
    fprintf(file, "elementos faltantes: %d\n", tam1 - (j * 3));

    int kernelRadius = (kernel - 1) / 2;

    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            unsigned int bSum = 0;
            unsigned int gSum = 0;
            unsigned int rSum = 0;
            int pond = 0;

            for (int ky = -kernelRadius; ky <= kernelRadius; ky++) {
                for (int kx = -kernelRadius; kx <= kernelRadius; kx++) {
                    int i = y + ky;
                    int j = x + kx;

                    if (i < 0 || i >= alto || j < 0 || j >= ancho) continue;

                    int index = (i * ancho) + j;
                    bSum += arr_in_b[index];
                    gSum += arr_in_g[index];
                    rSum += arr_in_r[index];
                    pond++;
                }
            }

            arr_in_b[(y * ancho) + x] = bSum / pond;
            arr_in_g[(y * ancho) + x] = gSum / pond;
            arr_in_r[(y * ancho) + x] = rSum / pond;
        }
    }

    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            fputc(arr_in_b[(y * ancho) + x], outputImage);
            fputc(arr_in_g[(y * ancho) + x], outputImage);
            fputc(arr_in_r[(y * ancho) + x], outputImage);
        }

        for (int p = 0; p < rowPadding; p++) {
            fputc(0, outputImage);
        }
    }

    int newImageSize = ancho * alto * 3 + alto * rowPadding;
    *(int*)&xx[34] = newImageSize;
    *(int*)&xx[2] = 54 + newImageSize;

    fseek(outputImage, 0, SEEK_SET);
    for (int i = 0; i < 54; i++) {
        fputc(xx[i], outputImage);
    }

    free(arr_in_b);
    free(arr_in_g);
    free(arr_in_r);

    fclose(image);
    fclose(outputImage);
}
