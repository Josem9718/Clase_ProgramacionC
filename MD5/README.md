En criptografía, MD5 (abreviatura de Message-Digest Algorithm 5, Algoritmo de Resumen del Mensaje 5) es un algoritmo de reducción criptográfico de 128 bits ampliamente usado. Uno de sus usos es el de comprobar que algún archivo no haya sido modificado.
MD5 es uno de los algoritmos de reducción criptográficos diseñados por el profesor Ronald Rivest del MIT (Massachusetts Institute of Technology, Instituto Tecnológico de Massachusetts). Fue desarrollado en 1991 como reemplazo del algoritmo MD4 después de que Hans Dobbertin descubriese su debilidad.
La codificación del MD5 de 128 bits es representada típicamente como un número de 32 símbolos hexadecimales. 

Ejemplo:MD5("Generando un MD5 de un texto") = 5df9f63916ebf8528697b629022993e8
V
El código anterior de 28 bytes ASCII fue tratado con MD5 y obtuvimos el correspondiente hash de salida.
En este documento "palabra" es una entidad de 4 bytes y un byte es una entidad de 8 bits. Una secuencia de bytes puede ser interpretada de manera natural como una secuencia de bits, donde cada grupo consecutivo de ocho bits se interpreta como un byte con el bit más significativo al principio. Similarmente, una secuencia de bytes puede ser interpretada como una secuencia de 32 bits (palabra), donde cada grupo consecutivo de cuatro bytes se interpreta como una palabra en la que el byte menos significativo está al principio.
Comenzamos definiendo la función LEFTROTATE que va a rotar a X hacia la izquierda ‘n’ cantidad de veces.

#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

Declaramos las variables que contendrán el hash final:
uint32_t h0, h1, h2, h3;

A continuación creamos una función llamada MD5 donde se llevara a cabo el cálculo del hash.

void md5(uint8_t *initial_msg, size_t initial_len)
La función recibirá como argumentos el mensaje y el tamaño del mensaje, en este caso usamos Inmsg y len.
// Nota: Todas las variables tienen 32 bits sin signo y se ajustan al módulo 2 ^ 32 al calcular

r fue creada  para especificar las cantidades de turno por ronda.
uint32_t r[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                    5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
                    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

K es una tabla de 64 elementos T[1 ... 64] construida con la función Seno. Se denota por T[i] el elemento i-ésimo de esta tabla, que será igual a la parte entera del valor absoluto del seno de 'i' 4294967296 veces. Y será utilizada al momento de procesar los bloques de 16 palabras. 
   uint32_t k[] = {        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
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

También usaremos un búfer de cuatro palabras (h0, h1, h2, h3) se usa para calcular el resumen del mensaje. Aquí cada una de las letras h0, h1, h2, h3 representa un registro de 32 bits. Estos registros los inicializamos con los siguientes valores hexadecimales: 
    h0 = 0x67452301;
    h1 = 0xefcdab89;
    h2 = 0x98badcfe;
    h3 = 0x10325476;

El siguiente bloque de código es la parte del pre-procesamiento, es la parte en la que agregamos un solo bit al mensaje, en este caso consideraremos los bytes de entrada como cadena de bits, donde el primer bit es el bit más significativo del byte. Al igual en este bloque se hace el rellenado de los 0s, se añade el bit ‘0‘hasta la longitud del mensaje en el bit = 448 
    int new_len = ((((initial_len + 8) / 64) + 1) * 64) - 8; 

msg también agrega bits "0" (asignamos también 64 bytes extra )
    msg = calloc(new_len + 64, 1);                                   
    memcpy(msg, initial_msg, initial_len);
    msg[initial_len] = 128;  
Se agrega el len                       uint32_t bits_len = 8*initial_len; 

Da in de bits al final del buffer    memcpy(msg + new_len, &bits_len, 4);           
    int offset;

Se procesa el mensaje en fragmentos sucesivos de 512 bits:
    for(offset=0; offset<new_len; offset += (512/8)) {

Esta sentencia divide el fragmento en dieciséis palabras de 32 bits.
uint32_t *w = (uint32_t *) (msg + offset);

Se definen las variables a utilizar en las funciones auxiliares:
        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
uint32_t i;

        for(i = 0; i<64; i++) {



 
            uint32_t f, g;
            
Para esta parte del código se utilizaron condicionales if para hacer el procesado del mensaje en bloques de 16 palabras, en cada condición se usaron operaciones auxiliares que evalúan tres palabras de 32 bits.
En el primer if  “ f“ toma el valor de la primera operación, esta operación en cada posición de cada bit actúa como una condicional .
Las operaciones utilizadas en los else if  son similares a la primera, ya que actúan bit a bit en paralelo.

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
                g = (7*i) % 16;  }

La rotación se separa para evitar re computación

            uint32_t temp = d;
            d = c;
            c = b;
            b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);
            a = temp;}

Se realizaron  las siguientes sumas para agregar el hash que se tiene  hasta ahora de cada uno de los fragmentos.
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d; 
    }
Creamos el int main, aquí es donde llamaremos al archivo, llamaremos la función md5 y imprimiremos el resultado.
int main(int argc, char **arg) {
Se inicializaron las variables a utilizar: 
    char input = 0, fileName[25] = {'%0'}, Inmsg[1000] = {'0'};
                                       int i = 0;
    
Se pide por medio de pantalla al usuario que ingrese el nombre del archivo que desea leer y se guarda el nombre.
    printf("\nIngrese el nombre del archivo:");
    gets(fileName);
Se declara in puntero para el archivo ya que es necesario para la comunicación entre el archivo y el programa. Posteriormente se abre el archivo por medio de la función fopen y se le indica con “r“que se quiere leer el archivo, si el archivo no existe se nos retornara un NULL. 
    FILE *fptr;
    fptr = fopen(fileName,"r");
Si un NULL fue retornado mandaremos un mensaje de error.
     If(fptr == NULL)
   {
      printf(“Error!”);   
      exit(1);             
   }
Imprime el mensaje que contiene el archivo.   
   for(input;(input = fgetc(fptr))!=EOF;){
        Inmsg[i] = input;
        printf(“%c”,input); i++;
   }
Se asigna la longitud del mensaje y se realiza el prototipo de la función md5 para iniciar el proceso del cálculo del hash.
   size_t len = (strlen(caux)-1);
   md5(Inmsg,len);
Finalmente se imprime el hash final.  

Se usa el if para en caso de que el archivo este vacio se imprima un mensaje de que el archivo no tiene contenido. 
if (len >= 1) {
    uint8_t *punt;
 
    punt=(uint8_t *)&h0;
    printf("%2.2x%2.2x%2.2x%2.2x", punt[0], punt[1], punt[2], punt[3], h0);
 
    punt=(uint8_t *)&h1;
    printf("%2.2x%2.2x%2.2x%2.2x", punt[0], punt[1], punt[2], punt[3], h1);
 
    punt=(uint8_t *)&h2;
    printf("%2.2x%2.2x%2.2x%2.2x", punt[0], punt[1], punt[2], punt[3], h2);
    
    punt=(uint8_t *)&h3;
    printf("%2.2x%2.2x%2.2x%2.2x", punt[0], punt[1], punt[2], punt[3], h3);
    
 }
else{
printf("El archivo no tiene contenido");
}
    return 0;
}


