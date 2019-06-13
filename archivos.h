#ifndef __ARCHIVOS_H__
#define __ARCHIVOS_H__

#define MAX_FECHA 7
#define MAX_NOMBRE 200
#define MAX_ARCHIVO 50

typedef struct preso{
	
	char nombre[MAX_NOMBRE];
	unsigned int edad;
	char conducta; // B = buena, R = regular, M = mala
	unsigned int pabellon;
	unsigned int celda;
	char fecha[MAX_FECHA];
	int maldicion_realizada;

}preso_t;

int actualizar_archivo( char nombre_del_archivo[MAX_ARCHIVO], char nombre_liberados[MAX_ARCHIVO] );

int crear_perdonables( FILE* archivo_crucio, FILE* archivo_imperius, FILE* archivo_perdonables );

#endif /* __ARCHIVOS_H__ */