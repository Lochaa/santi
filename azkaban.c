#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "archivos.h"

#define MAX_NOMBRE 200
#define SON_IGUALES 0

#define ESCRITURA "w"
#define LECTURA "r"
#define DE_TEXTO ".txt"
#define LIBERADOS "liberados_"
#define SECUENCIAL 1

#define ARCHIVO_CRUCIO "crucio.dat"
#define ARCHIVO_IMPERIUS "imperius.dat"

#define COMANDO_AYUDA "ayuda"
#define COMANDO_MOSTRAR_LIBERADOS "mostrar_liberados"
#define COMANDO_LIBERAR "liberar"
#define COMANDO_PERDONABLES "perdonables"
#define COMANDO_ACTUALIZAR "actualizar"


void armar_nombre_liberados( char nombre_liberados[MAX_NOMBRE], char fecha[MAX_FECHA] ){

	strcpy( nombre_liberados, LIBERADOS );
	strcat( nombre_liberados, fecha );
	strcat( nombre_liberados, DE_TEXTO );
}

// argc, cantidad de argumentos recibidos por el programa.
// argv, vector de strings que contiene los parametros pasados en el mismo orden que fueron escritos,
int main( int argc, char* argv[] ){

	if( (argc ==  2) && (strcmp( argv[1], COMANDO_AYUDA ) == SON_IGUALES) ){//TERMINADOOOO

		/*
		Mostrara y explicara por pantalla cada comando que hay.
		*/
		printf("\nComando " "\e[3m" "perdonables:" "\e[0m\n\n"
			"  De los dos archivos, crucio.dat y imperius.dat (binarios), creará uno con aquellos presos que pueden ser perdonados (también binario). \n"
				"El segundo argumento enviado será el nombre del archivo a crear, el usuario decide el nombre. Si ya existe un archivo con el nombre ingresado se sobreescribirá.\n  Ejemplo:./azkaban perdonables <nombre_archivo>.");
		
		printf("\n\nComando " "\e[3m" "liberar:" "\e[0m\n\n"
			"  Creará un archivo de texto con los nombres de los presos que cumplan con las condiciones necesarias:\n    -Nombre del archivo de presos perdonables.\n    -Año/Mes (formato aaaamm).\n    -Conducta (B: buena - R: regular - M: mala). "
				"\n  Ejemplo de la línea de comando:./azkaban liberar <archivo_perdonables>.dat 201906 B.\n  El nombre de este archivo debe ser del tipo liberados_​aaaamm​ .txt siendo ​ " "\e[1m" "aaaamm​" "\e[0m" " la fecha recibida como argumento. Si ya existe un archivo para esa fecha, NO puede sobreescribirse.");
		
		printf("\n\nComando " "\e[3m" "mostrar_liberados:" "\e[0m\n\n"
			"  Mostrará por pantalla el archivo de presos liberados en un determinado año/mes. El segundo argumento será la fecha del archivo que se quiere mostrar.\n  Ejemplo de la línea de comando:./azkaban mostrar_liberados 201906.");
		
		printf("\n\nComando " "\e[3m" "actualizar:" "\e[0m\n\n"
			"  Actualizará los archivos de presos crucio.dat e imperius.dat, sacando los presos que están en el archivo de los liberados. El segundo argumento es la fecha del archivo que se toma para actualizar.\n  Ejemplo de la línea de comando:./azkaban actualizar 201906 (" "\e[1m" "aaaamm​" "\e[0m" ").\n");

	}else if ( (argc == 3) && ( (strcmp( argv[1], COMANDO_PERDONABLES ) == SON_IGUALES) || (strcmp( argv[1], COMANDO_ACTUALIZAR ) == SON_IGUALES) || (strcmp( argv[1], COMANDO_MOSTRAR_LIBERADOS ) == SON_IGUALES) ) ){

		if( strcmp( argv[1], COMANDO_PERDONABLES ) == SON_IGUALES ){//TERMINADOOOO

		/*
		creara un archivo que contenga todos los perdonables.
		*/

			FILE* archivo_perdonables = fopen( argv[2], ESCRITURA );
			if( !archivo_perdonables ){

				return -1;
			}

			FILE* archivo_crucio = fopen( ARCHIVO_CRUCIO, LECTURA );
			if( !archivo_crucio ){

				fclose( archivo_perdonables );
				return -1;
			}

			FILE* archivo_imperius = fopen( ARCHIVO_IMPERIUS, LECTURA );
			if( !archivo_imperius ){

				fclose( archivo_perdonables );
				fclose( archivo_crucio );
				return -1;
			}

			crear_perdonables( archivo_crucio, archivo_imperius, archivo_perdonables ); 

			fclose( archivo_perdonables );
			fclose( archivo_crucio );
			fclose( archivo_imperius );

		}else if( strcmp( argv[1], COMANDO_ACTUALIZAR ) == SON_IGUALES ){

		/*
		actualizara los archivos crucio e imperius.
		*/
			char nombre_liberados[MAX_FECHA];
			armar_nombre_liberados( nombre_liberados, argv[2] );//HACER UNA FUNCION QUE VERIFIQUE SI LA FECHA INGRESADA ESTA BIEN.

			actualizar_archivo( ARCHIVO_CRUCIO, nombre_liberados );
			actualizar_archivo( ARCHIVO_IMPERIUS, nombre_liberados );

			return 0;

		}else if( strcmp( argv[1], COMANDO_MOSTRAR_LIBERADOS ) == SON_IGUALES ){

		/*
		mostrara por pantalla el archivo liberado en determinado año/mes.
		*/
		
			char nombre_liberados[MAX_FECHA];
			armar_nombre_liberados( nombre_liberados, argv[2] );//HACER UNA FUNCION QUE VERIFIQUE SI LA FECHA INGRESADA ESTA BIEN.

			FILE* liberados = fopen( nombre_liberados, LECTURA );
		}
	}else if( (argc == 4) && (strcmp( argv[1], COMANDO_LIBERAR ) == SON_IGUALES) ){

		/*
		Creara un archivo de texto en el cual se almacenan los presos que cuentan con las condiciones para ser liberados.
		*/
	}else {

		printf("La línea de comando fue mal ingresada, reintente por favor. Para poder ver como se escribe correctamente las diferentes líneas de comando, puede utilizar la siguiente: ./azkaban ayuda\n");
		return 0;
	}
} 