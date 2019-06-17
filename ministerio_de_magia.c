#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ministerio_de_magia.h"


#define BUENA 'B'
#define REGULAR 'R'
#define MALA 'M'

#define DICIEMBRE 12
#define ENERO 1

#define ARCHIVO_AUXILIAR "archivo_auxiliar.dat"
#define LIBERADOS "liberados_"
#define DE_TEXTO ".txt"
#define ESCRITURA "w"
#define LECTURA "r"

#define ELEMENTOS_LEIDOS 1
#define SON_IGUALES 0
#define SECUENCIAL 1
#define CORRECTO 0


// Pos: Procedimiento que explicará al usuario el uso de todos los comandos.
void mostrar_ayuda(){

	printf( "Aclaraciones: \n\n -Los archivos binarios serán representados con una extensión del tipo " "\e[1m" ".dat" "\e[0m" "\n -Los archivos de texto serán representados con una extension del tipo " "\e[1m" ".txt" "\e[0m"
		"\n -¿Qué es un argumento en una línea de comando?\n\n  En el ejemplo: ./azkaban liberar <archivo_perdonables>.dat 201906 B \n\n  Argumentos: \n    1° liberar \n    2° <archivo_perdonables>.dat \n    3° 201906 \n    4° B" );

	printf( "\n\nComando " "\e[3m" "perdonables:" "\e[0m\n\n"
		"  De los dos archivos, crucio.dat y imperius.dat (binarios), creará uno con aquellos presos que pueden ser perdonados (también binario). El criterio para ser perdonados es que el preso solo aparezca en alguno de los dos archivos (crucio.dat e imperius.dat), es decir, el algoritmo hará una diferencia simétrica entre ambos archivos.\n"
			"  El segundo argumento enviado será el nombre del archivo a crear, el usuario decide el nombre. Si ya existe un archivo con el nombre ingresado se sobreescribirá.\n\n  Ejemplo:./azkaban perdonables <nombre_archivo>" );
		
	printf( "\n\nComando " "\e[3m" "liberar:" "\e[0m\n\n"
		"  Creará un archivo de texto con los nombres de los presos que cumplan con las condiciones necesarias:\n    -Nombre del archivo de presos perdonables.\n    -Año/Mes (formato aaaamm).\n    -Conducta (B: buena - R: regular - M: mala). "
			"\n\n  Ejemplo de la línea de comando:./azkaban liberar <archivo_perdonables>.dat 201906 B\n\n  El nombre de este archivo debe ser del tipo liberados_​aaaamm​ .txt siendo ​ " "\e[1m" "aaaamm​" "\e[0m" " la fecha recibida como argumento. Si ya existe un archivo para esa fecha, NO puede sobreescribirse." );
		
	printf( "\n\nComando " "\e[3m" "mostrar_liberados:" "\e[0m\n\n"
		"  Mostrará por pantalla el archivo de presos liberados en un determinado año/mes. El segundo argumento será la fecha del archivo que se quiere mostrar.\n\n  Ejemplo de la línea de comando:./azkaban mostrar_liberados 201906." );
	
	printf( "\n\nComando " "\e[3m" "actualizar:" "\e[0m\n\n"
		"  Actualizará los archivos de presos crucio.dat e imperius.dat, sacando los presos que están en el archivo de los liberados. El segundo argumento es la fecha del archivo que se toma para actualizar.\n\n  Ejemplo de la línea de comando:./azkaban actualizar 201906 (" "\e[1m" "aaaamm​" "\e[0m" ").\n" );

	char tecla;
	printf( "\n  Para salir ingrese cualquier tecla:" );
	scanf( "%c", &tecla );
}


// Pos: Procedimiento que notificará al usuario que ocurrió dicho error.
void notificar_error(){

	printf( "La línea de comando fue mal ingresada, reintente por favor. Para poder ver como se escribe correctamente las diferentes líneas de comando, puede utilizar la siguiente: ./azkaban ayuda\n");
}


// Pre: Procedimiento que recibe un entero, 0 sí es correcto o -1 sí no lo es.
// Pos: Informará al usuario si se finalizó o no correctamente la línea de comando ingresada.
void notificar_resultado( int resultado ){

	if( resultado == CORRECTO ){

		printf( "La acción se finalizó correctamente.\n" );
	}else{

		printf( "No se completó la acción, ocurrió algo inesperado." );
	}
}


// Pre: Procedimiento que recibirá una cadena de caracteres (string).
// Pos: Notificará al usuario que ocurrió un problema con dicho archivo.
void notificar_problema( char nombre_archivo[MAX_ARCHIVO] ) {

	printf( " Hubo un error al intentar abrir el archivo " "\e[1m" "%s" "\e[0m" "\n", nombre_archivo );
} 


// Pre: Procedimiento recibe una cadena de caracter vacía, y otra que representa una fecha ingresada por el usuario.
// Pos: Devolverá inicializada la cadena vacía con el nombre del archivo de los presos liberados.
void armar_nombre_liberados( char nombre_liberados[MAX_NOMBRE], char fecha[MAX_FECHA] ){

	strcpy( nombre_liberados, LIBERADOS );
	strcat( nombre_liberados, fecha );
	strcat( nombre_liberados, DE_TEXTO );
}

// Pre: Función que recibirá una cadena de caracteres y no deberá contener ningún que caracter que no sea numérico.
// Pos: Devolverá true si el mes ingresado es valido, sino false.
bool es_fecha_correcta( char fecha[MAX_FECHA] ){

	int fecha_en_enteros = atoi( fecha );
	int mes = fecha_en_enteros%100;

	if( (mes >= ENERO) && (mes <= DICIEMBRE) ){

		return true;
	}
	
	return false;
}

// Pre: Función que recibirá una variable de tipo char.
// Pos: Devolverá true si la conducta ingresada es correspondiente al criterio utilizado, sino false,
bool es_conducta_correcta( char conducta ){

	if( (conducta == BUENA) || (conducta == REGULAR) || (conducta == MALA) ){

		return true;
	}

	return false;
}


// Pre: Función que recibe una cadena de caracteres y un archivo de texto ya abierto.
// Pos: Devolverá la cantidad de elementos que pudo leer, y devuelve NULL si ocurrió un error.
int leer_preso_txt( char nombre[MAX_NOMBRE], FILE* archivo ){

	return fscanf( archivo, "%[^;];\n", nombre );
}


// Pre: Función que recibe una cadena de caracteres y un archivo de texto ya abierto.
// Pos: Devolverá la cantidad de elementos que pudo escribir, y devuelve NULL si ocurrió un error.
int escribir_preso_txt( char nombre[MAX_NOMBRE], FILE* archivo ){

	return fprintf( archivo, "%s;\n", nombre );
}


// Pre: Función que recibe un puntero de una variable del tipo peso_t y un archivo ya abierto.
// Pos: Devolverá la cantidad de elementos que pudo leer, y devuelve NULL si ocurrió un error.
size_t leer_preso_dat( preso_t* preso, FILE* archivo ){

	return fread( preso, sizeof( preso_t ), SECUENCIAL, archivo );
}


// Pre: Función que recibe un puntero de una variable del tipo peso_t y un archivo ya abierto.
// Pos: Devolverá la cantidad de elementos que pudo escribir, y devuelve NULL si ocurrió un error.
size_t escribir_preso_dat( preso_t* preso, FILE* archivo ){

	return fwrite( preso, sizeof( preso_t ), SECUENCIAL, archivo );
}


// Pre: Función que recibe dos nombres de dos archivos.
// Pos: Hará la diferencia del primer archivo con el segundo, además devolverá -1 si hubo algún problema al abrir archivos, si salió todo bien devuelve 0.
int actualizar_archivo( char nombre_del_archivo[MAX_ARCHIVO], char nombre_liberados[MAX_ARCHIVO] ){

	FILE* archivo_liberados = fopen( nombre_liberados, LECTURA );
	if( !archivo_liberados ){

		notificar_problema( nombre_liberados );
		return -1;
	}

	FILE* archivo_a_actualizar = fopen( nombre_del_archivo, LECTURA );
	if( !archivo_a_actualizar ){

		notificar_problema( nombre_del_archivo );
		fclose( archivo_liberados );
		return -1;
	}

	FILE* archivo_auxiliar = fopen( ARCHIVO_AUXILIAR, ESCRITURA );
	if( !archivo_auxiliar ){

		notificar_problema( ARCHIVO_AUXILIAR );
		fclose( archivo_liberados );
		fclose( archivo_a_actualizar );
		return -1;
	}

	preso_t preso_auxiliar;
	char nombre_del_liberado[MAX_NOMBRE];

	leer_preso_dat( &preso_auxiliar, archivo_a_actualizar );
	int leidos_liberados = leer_preso_txt( nombre_del_liberado, archivo_liberados );

	while( (!feof( archivo_a_actualizar )) && (leidos_liberados == ELEMENTOS_LEIDOS) ){

		if( strcmp( preso_auxiliar.nombre, nombre_del_liberado ) == SON_IGUALES ){

			leer_preso_dat( &preso_auxiliar, archivo_a_actualizar );
			leidos_liberados = leer_preso_txt( nombre_del_liberado, archivo_liberados );
		}else if( strcmp( preso_auxiliar.nombre, nombre_del_liberado ) < 0 ){

			escribir_preso_dat( &preso_auxiliar, archivo_auxiliar );
			leer_preso_dat( &preso_auxiliar, archivo_a_actualizar );
		}else {

			leidos_liberados = leer_preso_txt( nombre_del_liberado, archivo_liberados );			
		}
	}

	while( !feof( archivo_a_actualizar ) ){

		escribir_preso_dat( &preso_auxiliar, archivo_auxiliar );
		leer_preso_dat( &preso_auxiliar, archivo_a_actualizar );
	}

	fclose( archivo_liberados );
	fclose( archivo_a_actualizar );
	fclose( archivo_auxiliar );

	remove( nombre_del_archivo );
	rename( ARCHIVO_AUXILIAR, nombre_del_archivo );

	return 0;
}	


// Pre: Función que recibe 3 nombres de archivos.
// Pos: Llenará el tercero archivo con la diferencia simétrica de los dos primeros.
int crear_perdonables( char nombre_crucio[MAX_ARCHIVO], char nombre_imperius[MAX_ARCHIVO], char nombre_perdonables[MAX_ARCHIVO] ){

	FILE* archivo_perdonables = fopen( nombre_perdonables, ESCRITURA );
	if( !archivo_perdonables ){

		notificar_problema( nombre_perdonables );
		return -1;
	}

	FILE* archivo_crucio = fopen( nombre_crucio, LECTURA );
	if( !archivo_crucio ){

		notificar_problema( nombre_crucio );
		fclose( archivo_perdonables );
		return -1;
	}

	FILE* archivo_imperius = fopen( nombre_imperius, LECTURA );
	if( !archivo_imperius ){

		notificar_problema( nombre_imperius );
		fclose( archivo_perdonables );
		fclose( archivo_crucio );
		return -1;
	}

	preso_t preso_crucio;
	preso_t preso_imperius;

	leer_preso_dat( &preso_crucio, archivo_crucio );
	leer_preso_dat( &preso_imperius, archivo_imperius );

	while( (!feof( archivo_crucio )) && (!feof( archivo_imperius )) ){

		if( strcmp( preso_crucio.nombre, preso_imperius.nombre ) == SON_IGUALES ){

			leer_preso_dat( &preso_crucio, archivo_crucio );
			leer_preso_dat( &preso_imperius, archivo_imperius );
		}else if( strcmp( preso_crucio.nombre, preso_imperius.nombre ) > 0 ){

			escribir_preso_dat( &preso_imperius, archivo_perdonables );
			leer_preso_dat( &preso_imperius, archivo_imperius );
		}else {

			escribir_preso_dat( &preso_crucio, archivo_perdonables );
			leer_preso_dat( &preso_crucio, archivo_crucio );
		}
	}

	while( !feof( archivo_crucio ) ){

		escribir_preso_dat( &preso_crucio, archivo_perdonables );
		leer_preso_dat( &preso_crucio, archivo_crucio );				
	}

	while( !feof( archivo_imperius ) ){

		escribir_preso_dat( &preso_imperius, archivo_perdonables );
		leer_preso_dat( &preso_imperius, archivo_imperius );
	}

	fclose( archivo_perdonables );
	fclose( archivo_crucio );
	fclose( archivo_imperius );

	return 0;
}


// Pre: Función que recibe dos fechas, ninguna deberá contener algún caracter que no sea numérico.
// Pos: Devolverá true si la fecha del preso es anterior a la fecha establecida, sino false.
bool cumple_con_la_fecha( char fecha_establecida[MAX_FECHA], char fecha_preso[MAX_FECHA] ){

	int fecha_establecida_ent = atoi( fecha_establecida );
	int fecha_preso_ent = atoi( fecha_preso );

	if( fecha_preso_ent <= fecha_establecida_ent ){//Si la fecha del preso es menor quiere decir que ya pasó

		return true;
	}

	return false;
}


// Pre: Función que recibe dos caracteres, para que funcione correctamente ambos deberán tener un valor perteneciente al criterio usado.
// Pos: Devolverá true si la conducta del preso es mejor o igual a la conducta establecida, según el criterio usado.
bool cumple_con_la_conducta( char conducta_minima, char conducta_preso ){

	if( conducta_minima == MALA ){

		return true;
	}else if( conducta_minima == REGULAR ){

		if( conducta_preso == MALA ){

			return false;
		}else{

			return true;
		}
	}else{

		if( conducta_preso == BUENA ){

			return true;
		}else {

			return false;
		}
	}
}


// Pre: Función que recibe 3 cadenas de caracteres y una variable de tipo de char.
// Pos: Creará un archivo con los nombres de los presos que esten en condiciones de ser liberados, además devolverá -1 si hubo algún problema al abrir archivos, si salió todo bien devuelve 0.
int liberar_presos( char nombre_liberados[MAX_NOMBRE], char nombre_perdonables[MAX_NOMBRE], char fecha_establecida[MAX_FECHA], char conducta_minima ){

	FILE* archivo_liberados = fopen( nombre_liberados, ESCRITURA );
	if( !archivo_liberados ){

		notificar_problema( nombre_liberados );
		return -1;
	}

	if( es_fecha_correcta( fecha_establecida ) ){

		if( es_conducta_correcta( conducta_minima ) ){

			FILE* archivo_perdonables = fopen( nombre_perdonables, LECTURA );
			if( !archivo_perdonables ){

				fclose( archivo_liberados );
				notificar_problema( nombre_perdonables );
				return -1;
			}

			preso_t preso;
			leer_preso_dat( &preso, archivo_perdonables );

			while( !feof( archivo_perdonables ) ){

				if( cumple_con_la_conducta( conducta_minima, preso.conducta ) && cumple_con_la_fecha( fecha_establecida, preso.fecha ) ){

					escribir_preso_txt( preso.nombre, archivo_liberados );
				}
				leer_preso_dat( &preso, archivo_perdonables );
			}

			fclose( archivo_perdonables );
			fclose( archivo_liberados );
			return 0;
		}else{

			printf("La conducta introducida no es correspondiente al criterio implementado.\n");
			return -1;
		}
	}else{

		printf("La fecha introducida es incorrecta.\n");
		return -1;
	}
}


// Pre: Procedimiento que recibe un archivo de texto ya abierto, contiene nombres separados por ;.
// Pos: Mostrará por pantalla los nombres que esten en el archivo.
void mostrar_nombres( FILE* archivo ){

	char nombre[MAX_NOMBRE];
	printf( "Los presos liberados son:\n" );

	int leidos = leer_preso_txt( nombre, archivo );
	while( leidos == ELEMENTOS_LEIDOS ){

		printf( "- %s.\n", nombre );
		leidos = leer_preso_txt( nombre, archivo );
	}	
}


// Pre: Función que recibe el nombre de un archivo y una fecha representada por una cadena de caracteres, esta no debe contener caracteres no numéricos.
// Pos: Mostrará por pantalla el archivo liberados, con un determinado formato, y si hubo problemas al abrir el archivo devolverá -1, si salió todo bien devolverá 0.
int mostrar_liberados( char nombre_liberados[MAX_NOMBRE], char fecha_establecida[MAX_FECHA] ){

	if( es_fecha_correcta( fecha_establecida ) ){

		FILE* archivo_liberados = fopen( nombre_liberados, LECTURA );
		if( !archivo_liberados ){

			notificar_problema( nombre_liberados );
			return -1;
		}

		mostrar_nombres( archivo_liberados );
		return 0;
	}else{

		printf("La fecha introducida es incorrecta.\n");
		return -1;
	}
}