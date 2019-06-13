#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "archivos.h"

#define SON_IGUALES 0
#define SECUENCIAL 1
#define FORMATO "%[^;];%u;%c;%u;%u;%[^;];%i"
#define ARCHIVO_AUXILIAR "archivo_auxiliar.dat"

#define CORRECTAMENTE 0 

#define ESCRITURA "w"
#define LECTURA "r"

/*
int leer_preso_txt( preso_t* preso, FILE* archivo ){

	return fscanf( archivo, FORMATO, preso->nombre, preso->edad, preso->conducta, preso->pabellon, preso->celda, preso->fecha, preso->maldicion_realizada );
}
*/

int leer_nombre_del_preso( char nombre[MAX_NOMBRE], FILE* archivo ){

	return fscanf( archivo, "%s", nombre );
}


int leer_preso_dat( preso_t* preso, FILE* archivo ){

	return fread( preso, sizeof( preso_t ), SECUENCIAL, archivo );
}


int escribir_preso_dat( preso_t* preso, FILE* archivo ){

	return fwrite( preso, sizeof( preso_t ), SECUENCIAL, archivo );
}


// Voy a tener que pasarle los nombres de los archivos y abrirlos en esta funcion ya que debo trabajar con un tercer archivo
// y despues borrar el original y renombrar el tercer archivo.
int actualizar_archivo( char nombre_del_archivo[MAX_ARCHIVO], char nombre_liberados[MAX_ARCHIVO] ){

	FILE* archivo_liberados = fopen( nombre_liberados, LECTURA );
	if( !archivo_liberados ){

		return -1;
	}

	FILE* archivo_a_actualizar = fopen( nombre_del_archivo, LECTURA );
	if( !archivo_a_actualizar ){

		fclose( archivo_liberados );
		return -1;
	}

	FILE* archivo_auxiliar = fopen( ARCHIVO_AUXILIAR, ESCRITURA );
	if( !archivo_auxiliar ){

		fclose( archivo_liberados );
		fclose( archivo_a_actualizar );
		return -1;
	}

	preso_t preso_auxiliar;
	char nombre_del_liberado[MAX_NOMBRE];

	leer_preso_dat( &preso_auxiliar, archivo_a_actualizar );
	leer_nombre_del_preso( nombre_del_liberado, archivo_liberados );

	while( (!feof( archivo_a_actualizar )) && (!feof( archivo_liberados )) ){

		if( strcmp( preso_auxiliar.nombre, nombre_del_liberado ) == SON_IGUALES ){

			leer_preso_dat( &preso_auxiliar, archivo_a_actualizar );
			leer_nombre_del_preso( nombre_del_liberado, archivo_liberados );
		}else if( strcmp( preso_auxiliar.nombre, nombre_del_liberado ) > 0 ){

			leer_nombre_del_preso( nombre_del_liberado, archivo_liberados );			
		}else {

			escribir_preso_dat( &preso_auxiliar, archivo_auxiliar );
			leer_preso_dat( &preso_auxiliar, archivo_a_actualizar );
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


int crear_perdonables( FILE* archivo_crucio, FILE* archivo_imperius, FILE* archivo_perdonables ){

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

	return 0;
}