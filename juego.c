#include <stdio.h>
#include "copa.h"
#include <stdlib.h>
#include <time.h>

#define TAMANIO 15
#define IZQUIERDA 'a'
#define DERECHA 'd'
#define ARRIBA 'w'
#define ABAJO 's'
#define VICTORIA 1
#define DERROTA -1

void ingresar_movimiento ( char* tecla ){

	printf("Por favor, ingrese la direccion en la cual desea moverse: ( %c = izquierda, %c = derecha, %c = arriba, %c = abajo)\n", IZQUIERDA, DERECHA, ARRIBA, ABAJO);
	scanf ( " %c", tecla );
}
void validar_movimiento ( juego_t* juego, char* tecla ){

	while ( !es_movimiento_valido (juego, *tecla) ){
		printf("Hay una pared, no es posible. ");
		ingresar_movimiento ( tecla );
		scanf ( " %c", tecla );		
	}
}

void definir_ganador ( juego_t juego ){

	if ( estado_juego ( juego ) == VICTORIA){
		printf(" GANASTE! :)\n");
	}else {
		printf(" PERDISTE... :(\n");
	}
}

int main (){
	juego_t juego;
	char direccion;
	bool hay_ganador = false;
	char laberinto_copia[TAMANIO][TAMANIO];

	srand ( (unsigned) time(NULL) );
	inicializar_laberinto ( &juego );

	while ( !hay_ganador ){

		actualizar_laberinto ( juego, laberinto_copia );
		mostrar_laberinto ( laberinto_copia );
		ingresar_movimiento ( &direccion );
		validar_movimiento ( &juego, &direccion );
		mover_jugador ( &(juego.jugador), direccion );
		mover_rival ( &juego );
		actualizar_juego ( &juego );
		mostrar_laberinto ( laberinto_copia );
		hay_ganador = estado_juego ( juego );
	}
	definir_ganador ( juego );

	return 0;
}