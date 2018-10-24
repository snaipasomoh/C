#include <stdio.h>
#include <stdarg.h>
#include <complex.h>
#include <math.h>

int myPrintf (char *fmt, ...){
	int counter = 0;
	char *p;
	va_list ap;
	int i;
	double d;
	double complex cd;
	va_start(ap, fmt);
	
	for (p = fmt; *p; p++){
		if (*p != '%'){
			putchar(*p);
		}
		else{
			p++;
			if (*p == 'd'){
				i = va_arg(ap, int);
				printf("%d", i);
				counter++;
			}
			else if (*p == 'f'){
				d = va_arg(ap, double);
				printf("%lf", d);
				counter++;
			}
			else if (*p == 'c'){
				cd = va_arg(ap, double complex);
				printf("%lf%c%lfi", creal(cd),
					   cimag(cd) >= 0.0 ? '+' : '-', fabs(cimag(cd)));
				counter++;
			}
			else{
				putchar(*p);
			}
		}
	}
	return counter;
}

int main(int argc, char **argv){
	int a = 10;
	double b = 3.1415;
	double complex c = 3.14 - 0.11 * I;
	myPrintf("int =\t%d%d%d\ndouble =\t%f\ncomplex =\t%c\n", a, a, a, b, c);
	return 0;
}
