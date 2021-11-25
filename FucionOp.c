#include <stdio.h>

int suma (int,int);
int resta (int,int);
int div (int,int);
int modulo (int,int);


int main(){

int a=0, b=0,opc=0,res=0;
int (*funcOp[])(int,int) = {suma,resta,div,modulo};

printf("\n\t1)SUMA\t2)RESTA\t3)DIVISION  4)MODULO\n\n Seleccione una opcion: ");
scanf("%d",&opc);
opc-=1;

printf("\nIngresa dos numeros que quieras operar: ");
scanf("%d%d",&a,&b);

res = (*funcOp[opc])(a,b);

printf("El resultado de la operacion es: %d\n ",res);

return 0;
}

int suma (int a, int b)
{
	return a + b;
}

int resta (int a, int b)
{
	return a - b;
}

int div (int a, int b)
{
	return a / b;
}

int modulo (int a, int b)
{
	return a % b;
}

