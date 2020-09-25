#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "ministerio_de_magia.h"

#define ESCRITURA "w"
#define LECTURA "r"

#define ARCHIVO_CRUCIO "crucio.dat"
#define ARCHIVO_IMPERIUS "imperius.dat"

#define SON_IGUALES 0
#define COMANDO_AYUDA "ayuda"
#define COMANDO_MOSTRAR_LIBERADOS "mostrar_liberados"
#define COMANDO_LIBERAR "liberar"
#define COMANDO_PERDONABLES "perdonables"
#define COMANDO_ACTUALIZAR "actualizar"


// argc, cantidad de argumentos recibidos por el programa.
// argv, vector de strings que contiene los parámetros pasados en el mismo orden que fueron escritos,
int main( int argc, char* argv[] ){

	if( (argc ==  2) && (strcmp( argv[1], COMANDO_AYUDA ) == SON_IGUALES) ){

		mostrar_ayuda();
		return 0;

	}else if ( (argc == 3) && ( (strcmp( argv[1], COMANDO_MOSTRAR_LIBERADOS ) == SON_IGUALES) || (strcmp( argv[1], COMANDO_ACTUALIZAR ) == SON_IGUALES) || (strcmp( argv[1], COMANDO_PERDONABLES ) == SON_IGUALES) ) ){

		if( strcmp( argv[1], COMANDO_PERDONABLES ) == SON_IGUALES ){

			char nombre_perdonables[MAX_NOMBRE];
			strcpy( nombre_perdonables, argv[2] );

			notificar_resultado( crear_perdonables( ARCHIVO_CRUCIO, ARCHIVO_IMPERIUS, nombre_perdonables ) ); 

			return 0;

		}else if( strcmp( argv[1], COMANDO_ACTUALIZAR ) == SON_IGUALES ){

			char nombre_liberados[MAX_FECHA];
			armar_nombre_liberados( nombre_liberados, argv[2] );

			notificar_resultado( actualizar_archivo( ARCHIVO_CRUCIO, nombre_liberados ) );
			notificar_resultado( actualizar_archivo( ARCHIVO_IMPERIUS, nombre_liberados ) );

			return 0;

		}else if( strcmp( argv[1], COMANDO_MOSTRAR_LIBERADOS ) == SON_IGUALES ){
		
			char nombre_liberados[MAX_NOMBRE];
			armar_nombre_liberados( nombre_liberados, argv[2] );

			char fecha_establecida[MAX_FECHA];
			strcpy( fecha_establecida, argv[2] );

			notificar_resultado( mostrar_liberados( nombre_liberados, fecha_establecida ) );

			return 0;
		}
	}else if( (argc == 5) && (strcmp( argv[1], COMANDO_LIBERAR ) == SON_IGUALES) ){

		char nombre_perdonables[MAX_NOMBRE];
		strcpy( nombre_perdonables, argv[2]);

		char fecha_establecida[MAX_FECHA];
		strcpy( fecha_establecida, argv[3] );

		char conducta_minima = argv[4][0];

		char nombre_liberados [MAX_NOMBRE];
		armar_nombre_liberados( nombre_liberados, fecha_establecida );

		notificar_resultado( liberar_presos( nombre_liberados, nombre_perdonables, fecha_establecida, conducta_minima ) );

		return 0;

	}else {

		printf( "La línea de comando fue mal ingresada, reintente por favor." );
		notificar_error();
		return -1;
	}
}