#include <stdio.h>
#include <string.h>

#define DE_TEXTO ".txt"
#define BINARIO ".dat"
#define SON_IGUALES 0 
#define LECTURA "r"
#define ESCRITURA "w"
#define MAX_ARCHIVO 100
#define MAX_FECHA 7
#define MAX_NOMBRE 200
#define SECUENCIAL 1

typedef struct preso{
	
	char nombre[MAX_NOMBRE];
	unsigned int edad;
	char conducta; // B = buena, R = regular, M = mala
	unsigned int pabellon;
	unsigned int celda;
	char fecha[MAX_FECHA];
	int maldicion_realizada;

}preso_t;

int main( int argc, char* argv[] ){
// argv[1] nombre del archivo a abrir (sin el formato).
// argv[2] formato del archivo a abrir

	char nombre_del_archivo[MAX_ARCHIVO];
	strcpy( nombre_del_archivo, argv[1] );
	strcat( nombre_del_archivo, argv[2] );//nombre + formato

	FILE* archivo = fopen( nombre_del_archivo, LECTURA );
	if( !archivo ){

		printf("Error al abrir el archivo.\n");
		return -1;
	}

	preso_t preso;
	char nuevo_nombre[MAX_ARCHIVO];

	if( strcmp( argv[2], DE_TEXTO ) == SON_IGUALES ){

		strcpy( nuevo_nombre, argv[1] );
		strcat( nuevo_nombre, BINARIO );

		FILE* auxiliar_uno = fopen( nuevo_nombre, ESCRITURA );
		if( !auxiliar_uno ){

			printf("No hay espacio para crear otro archivo.\n");
			return -1;
		}

		int leidos = fscanf( archivo, "%[^;];%u;%c;%u;%u;%[^;];%i\n", preso.nombre, &(preso.edad), &(preso.conducta), &(preso.pabellon), &(preso.celda), preso.fecha, &(preso.maldicion_realizada) );

		while( leidos == 7 ){

			fwrite( &preso, sizeof( preso_t ), SECUENCIAL, auxiliar_uno );
			leidos = fscanf( archivo, "%[^;];%u;%c;%u;%u;%[^;];%i\n", preso.nombre, &(preso.edad), &(preso.conducta), &(preso.pabellon), &(preso.celda), preso.fecha, &(preso.maldicion_realizada) );
		}

		fclose( archivo );
		fclose( auxiliar_uno );

		return 0;

	}else if( strcmp( argv[2], BINARIO ) == SON_IGUALES ){//Quiere decir que al archivo que abro es binario.

		strcpy( nuevo_nombre, argv[1] );
		strcat( nuevo_nombre, DE_TEXTO );

		FILE* auxiliar_dos = fopen( nuevo_nombre, ESCRITURA );
		if( !auxiliar_dos ){

			printf("No hay espacio para crear otro archivo.\n");
			return -1;
		}

		fread( &preso, sizeof( preso_t ), SECUENCIAL, archivo );

		while( !feof( archivo ) ){

			fprintf( auxiliar_dos, "%s;%u;%c;%u;%u;%s;%i\n", preso.nombre, preso.edad, preso.conducta, preso.pabellon, preso.celda, preso.fecha, preso.maldicion_realizada );
			fread( &preso, sizeof( preso_t ), SECUENCIAL, archivo );
		}

		fclose( archivo );
		fclose( auxiliar_dos );
		return 0;
	}
	return 0;
}