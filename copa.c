#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "laberinto.h"
#include <math.h>

#define TOTAL_OBSTACULOS 3
#define TOTAL_AYUDAS 6
#define TAMANIO 15
#define PARED '#'
#define VACIO ' '
#define IZQUIERDA 'a'
#define DERECHA 'd'
#define ARRIBA 'w'
#define ABAJO 's'
#define COPA 'C'
#define JUGADOR 'J'
#define RIVAL 'G'
#define POCIONES 'P'
#define IMPEDIMENTA 'I'
#define RIDDIKULUS 'R'
#define ESCREGUTO 'E'
#define ACROMANTULA 'A'
#define BOGGART 'B'
#define ESFINGE 'F'
#define DANIO_ESCREGUTO 20
#define DANIO_ACROMANTULA 10
#define DANIO_BOGGART 15
#define CURACION 15
#define DISTANCIA_MANHATTAN 10
#define VIDA 50
#define MAXIMO_PASOS 4
#define SIN_PASOS 0

//RESGISTROS...

typedef struct coordenada {
    int fil;
    int col;
} coordenada_t;

typedef struct obstaculo {
    char codigo;
    coordenada_t posicion;
    int danio;
} obstaculo_t;

typedef struct ayuda {
    char codigo;
    coordenada_t posicion;
    int vida_a_recuperar;
} ayuda_t;

typedef struct jugador {
    int vida;
    coordenada_t posicion;
    int tope_ayudas;
    ayuda_t ayudas[TOTAL_AYUDAS];
} jugador_t;

typedef struct rival {
    coordenada_t posicion;
    char direccion;
    int cantidad_pasos;
} rival_t;

typedef struct copa {
    char codigo;
    coordenada_t posicion;
} copa_t;

typedef struct juego{
    char laberinto_original[TAMANIO][TAMANIO];
    jugador_t jugador;
    rival_t rival;
    copa_t copa;
    obstaculo_t obstaculos[TOTAL_OBSTACULOS];
    int tope_obstaculos;
    ayuda_t ayudas[TOTAL_AYUDAS];
    int tope_ayudas;
} juego_t;

//FUNCIONES...

coordenada_t posicion_aleatoria(){
	coordenada_t aleatoria;
	srand ( (unsigned)time(NULL) );
	aleatoria.col = rand()%TAMANIO;
	aleatoria.fil = rand()%TAMANIO;

	return aleatoria;
}

bool posicion_valida ( char laberinto[TAMANIO][TAMANIO], coordenada_t posicion_aleatoria ){

	bool es_valida = false;
	if ( laberinto[posicion_aleatoria.fil][posicion_aleatoria.col] == VACIO ){
		es_valida = true;
	}
	return es_valida;
}

coordenada_t asignar_posicion ( char laberinto[TAMANIO][TAMANIO] ){

	coordenada_t posicion;

	posicion = posicion_aleatoria();

	while ( !posicion_valida( laberinto, posicion ) ){
		posicion = posicion_aleatoria();
	}
	return posicion; 
}

void mostrar_laberinto( char laberinto[TAMANIO][TAMANIO] ){
	for ( int fila = 0; fila < TAMANIO; fila++ ){
		for ( int columna = 0; columna < TAMANIO; columna++ ){
			printf( "%c ", laberinto[fila][columna] );
		}
		printf("\n");
	}
}

void inicializar_obstaculos ( juego_t* juego, char laberinto_copia[TAMANIO][TAMANIO] ){
	char obstaculos[TOTAL_OBSTACULOS] = { ESCREGUTO, ACROMANTULA, BOGGART};
	int danio_obstaculos[TOTAL_OBSTACULOS] = { DANIO_ESCREGUTO, DANIO_ACROMANTULA, DANIO_BOGGART};

	juego->tope_obstaculos = TOTAL_OBSTACULOS;

	for ( int i = 0; i < TOTAL_OBSTACULOS; i++ ){
	juego->obstaculos[i].posicion = asignar_posicion ( laberinto_copia );
	juego->obstaculos[i].danio = danio_obstaculos[i];
	juego->obstaculos[i].codigo = obstaculos[i];
	laberinto_copia [juego->obstaculos[i].posicion.fil][juego->obstaculos[i].posicion.col] = obstaculos[i];
	}
}

void inicializar_ayudas ( juego_t* juego , char laberinto_copia[TAMANIO][TAMANIO] ){
	char ayudas[TOTAL_AYUDAS] = { IMPEDIMENTA, RIDDIKULUS, POCIONES, POCIONES, POCIONES, ESFINGE};

	juego->tope_ayudas = TOTAL_AYUDAS;

	for ( int i = 0; i < TOTAL_AYUDAS; i++ ){
		juego->ayudas[i].posicion = asignar_posicion ( laberinto_copia );
		juego->ayudas[i].codigo = ayudas[i];
		laberinto_copia [juego->ayudas[i].posicion.fil][juego->ayudas[i].posicion.col] = ayudas[i];

		if ( ayudas[i] == POCIONES ){
			juego->ayudas[i].vida_a_recuperar = CURACION;
		}
	}
}

bool cumple_distancia_manhattan ( coordenada_t coordenada_uno, coordenada_t coordenada_dos ){
	bool cumple = false;
	int distancia_filas =  coordenada_uno.fil - coordenada_dos.fil;
	int distancia_columnas = coordenada_uno.col - coordenada_dos.col;

	if ( abs( distancia_filas ) + abs( distancia_columnas ) > DISTANCIA_MANHATTAN ){
		cumple = true;
	}
	return cumple;
}

void inicializar_participantes ( juego_t* juego, char laberinto_copia[TAMANIO][TAMANIO] ){
	juego->rival.posicion = asignar_posicion ( laberinto_copia );

	while ( !cumple_distancia_manhattan ( juego->copa.posicion, juego->rival.posicion ) ){
		juego->rival.posicion = asignar_posicion ( laberinto_copia );
	}
	laberinto_copia [juego->rival.posicion.fil][juego->rival.posicion.col] = RIVAL;
	juego->rival.cantidad_pasos = MAXIMO_PASOS;
	juego->rival.direccion = DERECHA;

	juego->jugador.posicion = asignar_posicion ( laberinto_copia );

	while ( !cumple_distancia_manhattan ( juego->copa.posicion, juego->jugador.posicion ) ){
		juego->jugador.posicion = asignar_posicion ( laberinto_copia );		
	}
	laberinto_copia [juego->jugador.posicion.fil][juego->jugador.posicion.col] = JUGADOR;	
	juego->jugador.tope_ayudas = TOTAL_AYUDAS;
	juego->jugador.vida = VIDA; 

}

void inicializar_laberinto( juego_t* juego ){
	char laberinto_copia[TAMANIO][TAMANIO];

	inicializar_paredes_laberinto ( juego->laberinto_original );

	for ( int fila = 0; fila < TAMANIO; fila++ ){
		for ( int columna = 0; columna < TAMANIO; columna++ ){
			laberinto_copia[fila][columna] = juego->laberinto_original[fila][columna];
		}
	}

	juego->copa.posicion = asignar_posicion ( juego->laberinto_original );
	laberinto_copia [juego->copa.posicion.fil][juego->copa.posicion.col] = COPA;

	inicializar_obstaculos ( juego, laberinto_copia );

	inicializar_ayudas ( juego, laberinto_copia );

	inicializar_participantes ( juego, laberinto_copia );
}

bool es_movimiento_valido(juego_t* juego, char tecla){
	bool es_valido = false;

	if ( (tecla == IZQUIERDA) || (tecla == DERECHA) || (tecla == ARRIBA) || (tecla == ABAJO) ){
		if ( (tecla == DERECHA) && (juego->laberinto_original[juego->jugador.posicion.fil][juego->jugador.posicion.col+1] != PARED) ){
			es_valido = true;
		}
		if ( (tecla == ARRIBA) && (juego->laberinto_original[juego->jugador.posicion.fil-1][juego->jugador.posicion.col] != PARED) ){
			es_valido = true;
		}
		if ( (tecla == IZQUIERDA) && (juego->laberinto_original[juego->jugador.posicion.fil][juego->jugador.posicion.col-1] != PARED) ){
			es_valido = true;
		}
		if ( (tecla == ABAJO) && (juego->laberinto_original[juego->jugador.posicion.fil+1][juego->jugador.posicion.col] != PARED) ){
			es_valido = true;
		}
	}

	return es_valido;
}

char obtener_movimiento ( juego_t* juego ){
	char tecla;

	printf("Ingrese el la direccion la cual desea moverse: ( %c = izquierda, %c = derecha, %c = arriba, %c = abajo)\n", IZQUIERDA, DERECHA, ARRIBA, ABAJO);
	scanf ( " %c", &tecla );

	es_movimiento_valido ( juego, tecla );
	while ( !es_movimiento_valido ( juego, tecla ) ){
		scanf ( " %c", &tecla );
		es_movimiento_valido ( juego, tecla );
	}
	return tecla;
}

void mover_jugador ( jugador_t jugador, char direccion ){
	if ( direccion == DERECHA ){
		jugador.posicion.col++;
	}
	if ( direccion == ABAJO ){
		jugador.posicion.fil++;
	}
	if ( direccion == IZQUIERDA ){
		jugador.posicion.col--;
	}
	if ( direccion == ARRIBA ){
		jugador.posicion.fil--;
	}
}

void cambiar_direccion_rival ( rival_t *rival ){
	rival->cantidad_pasos = MAXIMO_PASOS;

	if ( rival->direccion == DERECHA ){
		rival->direccion = ABAJO;
	}
	if ( rival->direccion == ABAJO ){
		rival->direccion = IZQUIERDA;
	}
	if ( rival->direccion == IZQUIERDA ){
		rival->direccion = ARRIBA;
	}
	if ( rival->direccion == ARRIBA ){
		rival->direccion = DERECHA;
	}
}

void mover_rival(juego_t* juego){

	es_movimiento_valido ( juego, juego->rival.direccion );
	juego->rival.cantidad_pasos--;
	while ( !es_movimiento_valido ( juego, juego->rival.direccion ) ){
		es_movimiento_valido (  juego, juego->rival.direccion );
		juego->rival.cantidad_pasos--;
		
		if ( juego->rival.cantidad_pasos == SIN_PASOS ){
			cambiar_direccion_rival ( &(juego->rival) );
		}
	}
	if ( juego->rival.direccion == DERECHA ){
		juego->rival.posicion.col++;
	}
	if ( juego->rival.direccion == ABAJO ){
		juego->rival.posicion.fil++;
	}
	if ( juego->rival.direccion == IZQUIERDA ){
		juego->rival.posicion.col--;
	}
	if ( juego->rival.direccion == ARRIBA ){
		juego->rival.posicion.fil--;
	}	
}

void actualizar_juego ( juego_t* juego ){

}