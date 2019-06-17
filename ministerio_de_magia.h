#ifndef __MINISTERIO_DE_MAGIA_H__
#define __MINISTERIO_DE_MAGIA_H__

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

void notificar_resultado( int resultado );

void armar_nombre_liberados( char nombre_liberados[MAX_NOMBRE], char fecha[MAX_FECHA] );

void notificar_error();

void mostrar_ayuda();

int mostrar_liberados( char nombre_liberados[MAX_NOMBRE], char fecha_establecida[MAX_FECHA] );

int actualizar_archivo( char nombre_del_archivo[MAX_ARCHIVO], char nombre_liberados[MAX_ARCHIVO] );

int crear_perdonables( char nombre_crucio[MAX_ARCHIVO], char nombre_imperius[MAX_ARCHIVO], char nombre_perdonables[MAX_ARCHIVO] );

int liberar_presos( char nombre_liberados[MAX_NOMBRE], char nombre_perdonables[MAX_NOMBRE], char fecha_establecida[MAX_FECHA], char conducta_minima );

#endif /* __MINISTERIO_DE_MAGIA_H__ */