// MD 5- MIGUEL BERNAVIDES  JIMENEZ
// TAREA 4 PROGRAMACION EN C/C++

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
/*LEFTROTATE va a rotar a X hacia la izquierda 'n' cantidad de veces*/ 
#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))
/*Declaramos las variables que contendran el hash*/ 
uint32_t h0, h1, h2, h3;
/*Funcion md5 donde se lleva a cabo el caluclo de la huella*/ 
void md5(uint8_t *initial_msg, size_t initial_len) {
 
    uint8_t *msg = NULL;
/*Especifica las cantidades de turno por ronda*/
    uint32_t r[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                    5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
                    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};
/*Elemento utilizados para procesar los bloques de 16 palabras*/
    uint32_t k[] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};
 /*se utiliza un buffer de 4 palabras(h0,h1,h2,h3), se utiliza para el resumen del mensaje*/
    h0 = 0x67452301;
    h1 = 0xefcdab89;
    h2 = 0x98badcfe;
    h3 = 0x10325476;
 /*En este bloque de codigo se asigna un solo bit al mensaje y se rellena con 0s*/

    int new_len = ((((initial_len + 8) / 64) + 1) * 64) - 8;
    /*agrega bits '0' y asignamos 64 bytes extras */
    msg = calloc(new_len + 64, 1);
    memcpy(msg, initial_msg, initial_len);
    msg[initial_len] = 128; 
    /*se agregra el len */
    uint32_t bits_len = 8*initial_len; 
    memcpy(msg + new_len, &bits_len, 4);          
 
    int offset;
     /*Se procesa el mensaje en fragmentos sucesivos de 512 */
    for(offset=0; offset<new_len; offset += (512/8)) {
 
        uint32_t *w = (uint32_t *) (msg + offset);
   /*Variables para operaciones auxiliares*/
        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
 
        uint32_t i;
	/*Ciclo para realizar las 64 operaciones*/
        for(i = 0; i<64; i++) {
 
            uint32_t f, g;
           /* 
             switch ((i >> 4) & 3)
            {
                case 0:    // (i < 16)
                    f = (b & c) | ((~b) & d);
                    g = i;
                    break;
                case 1:    // (i < 32)
                    f = (d & b) | ((~d) & c);
                    g = (5 * i + 1) % 16;
                    break;
                case 2:    // (i < 48)
                    f = b ^ c ^ d;
                    g = (3 * i + 5) % 16;
                    break;
                case 3:    // other
                    f = c ^ (b | (~d));
                    g = (7 * i) % 16;
                    break;
            }*/
	/*Bloque de operaciones*/
            if (i < 16) {
                f = (b & c) | ((~b) & d);
                g = i;
            } else if (i < 32) {
                f = (d & b) | ((~d) & c);
                g = (5*i + 1) % 16;
            } else if (i < 48) {
                f = b ^ c ^ d;
                g = (3*i + 5) % 16;          
            } else {
                f = c ^ (b | (~d));
                g = (7*i) % 16;
            }

     /*Rotacion separada para evitar re computo*/
            uint32_t temp = d;
            d = c;
            c = b;
            b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);
            a = temp;

        }
      /*Se asigna hash obtenido hasta el momento*/
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d; 
    }

    free(msg);
}
 
int main(int argc, char **arg) {
  /*inicializacion de variables*/
    char input = 0, fileName[25] = {'\0'}, Inmsg[256] = {'\0'};
    int i = 0;
    /*Se pide al usuario ingrese el nombre del archivo y se guaarda en fileName*/
    printf("\nIngrese el nombre del archivo: ");
    gets(fileName);
    /*Se declara el puntero del archivo*/
    FILE *fptr;
    fptr = fopen(fileName,"r");/*Se lee el archivo*/
    /*verifixca que el archivo exista*/
     if(fptr == NULL)
   {
      printf("Error! Archivo no existente\n");   
      exit(1);             
   }
   /*Se imprime el mensaje del archivo y guarda el mensaje */
   printf("Contenido:");  
   for(input;(input = fgetc(fptr))!=EOF;){
        Inmsg[i] = input;
        printf("%c",input); i++;
   }

	
  /* for(i=0; i < 256; i++)
   {
	caux[i] = input; 
	input = fgetc(fptr);
	printf("%c",input);	

	if(input == EOF){
		break;
	}
   }*/
   /*largo del archivo */
   size_t len = (strlen(Inmsg)-1);
   /*Se manda a llamar la funcion MD5 para calcular el hash */
   md5(Inmsg,len);

   /*Se verifica que el archivo tenga mas de 1 bit*/
  if(len >= 1){

    uint8_t *punt;
    printf("\nHuella: ");
    punt=(uint8_t *)&h0;
    printf("%2.2x%2.2x%2.2x%2.2x", punt[0], punt[1], punt[2], punt[3], h0);
    punt=(uint8_t *)&h1;
    printf("%2.2x%2.2x%2.2x%2.2x", punt[0], punt[1], punt[2], punt[3], h1);
    punt=(uint8_t *)&h2;
    printf("%2.2x%2.2x%2.2x%2.2x", punt[0], punt[1], punt[2], punt[3], h2);
    punt=(uint8_t *)&h3;
    printf("%2.2x%2.2x%2.2x%2.2x\n", punt[0], punt[1], punt[2], punt[3], h3);
    
    }
  else
  {
  printf("No hay contenido en el archivo\n");
  }

    return 0;
}



