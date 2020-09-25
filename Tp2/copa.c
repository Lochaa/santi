#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "laberinto.h"
#include "copa.h"
#include <math.h>

#define CANTIDAD_OBSTACULOS 3
#define CANTIDAD_AYUDAS 6
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
#define SIN_CURACION 0
#define DISTANCIA_MANHATTAN 10
#define VIDA 50
#define SIN_VIDA 0
#define POCA_VIDA 15
#define VIDA_PERDIDA_PASO 3
#define MAXIMO_PASOS 4
#define SIN_PASOS 0
#define SIN_AYUDAS 0
#define EN_CURSO 0
#define VICTORIA 1
#define DERROTA -1
#define BORDE_MINIMO 0
#define BORDE_MAXIMO 14

// Pos: Devuelve una posición aleatoria, sus filas y columnas tienen un rango de 15.
coordenada_t posicion_aleatoria(){

	coordenada_t aleatoria;

	aleatoria.col = rand()%TAMANIO;
	aleatoria.fil = rand()%TAMANIO;

	return aleatoria;
}


// Pre: Función que recibe el laberinto con las paredes ya inicializadas 
// Pos: Si la posición mandada es igual a vacío, es decir, si es valida devolvera true, sino false.
bool posicion_valida ( char laberinto[TAMANIO][TAMANIO], coordenada_t posicion ){

	bool es_valida = false;

	if ( laberinto[posicion.fil][posicion.col] == VACIO ){

		es_valida = true;
	}
	return es_valida;
}


// Pre: Función que recibe una matriz de rango TAMANIOxTAMANIO.
// Pos: Devuelve una posicion valida.
coordenada_t asignar_posicion ( char laberinto[TAMANIO][TAMANIO] ){

	coordenada_t posicion = posicion_aleatoria();

	while ( !posicion_valida( laberinto, posicion ) ){

		posicion = posicion_aleatoria();
	}
	return posicion; 
}


// Pre: Procedimiento que recibe una matriz de rango TAMANIOxTAMANIO.
// Pos: Imprimirá por pantalla el laberinto mandado.
void mostrar_laberinto( char laberinto[TAMANIO][TAMANIO] ){

	for ( int fila = 0; fila < TAMANIO; fila++ ){
		for ( int columna = 0; columna < TAMANIO; columna++ ){

			printf( "%c ", laberinto[fila][columna] );
		}
		printf("\n");
	}
}


// Pre: Procidimiento que recibe un puntero a una variable del registro juego_t y una matriz de rango TAMANIOxTAMANIO.
// Pos: Inicializará todos los obstáculos del juego.
void inicializar_obstaculos ( juego_t* juego, char laberinto_copia[TAMANIO][TAMANIO] ){

	char auxiliar[CANTIDAD_OBSTACULOS] = { ESCREGUTO, ACROMANTULA, BOGGART};
	int danio[CANTIDAD_OBSTACULOS] = { DANIO_ESCREGUTO, DANIO_ACROMANTULA, DANIO_BOGGART};

	juego->tope_obstaculos = CANTIDAD_OBSTACULOS;

	for ( int i = 0; i < CANTIDAD_OBSTACULOS; i++ ){

		juego->obstaculos[i].posicion = asignar_posicion ( laberinto_copia );
		juego->obstaculos[i].danio = danio[i];
		juego->obstaculos[i].codigo = auxiliar[i];
		laberinto_copia [juego->obstaculos[i].posicion.fil][juego->obstaculos[i].posicion.col] = auxiliar[i];
	}
}


// Pre: Procedimiento que recibe un puntero a una variable del registro juego_t y una matriz de rango TAMANIOxTAMANIO.
// Pos: Inicializará todos las ayudas del juego.
void inicializar_ayudas ( juego_t* juego , char laberinto_copia[TAMANIO][TAMANIO] ){

	char auxiliar[CANTIDAD_AYUDAS] = { IMPEDIMENTA, RIDDIKULUS, POCIONES, POCIONES, POCIONES, ESFINGE};

	juego->tope_ayudas = CANTIDAD_AYUDAS;

	for ( int i = 0; i < CANTIDAD_AYUDAS; i++ ){

		juego->ayudas[i].posicion = asignar_posicion ( laberinto_copia );
		juego->ayudas[i].codigo = auxiliar[i];
		laberinto_copia [juego->ayudas[i].posicion.fil][juego->ayudas[i].posicion.col] = auxiliar[i];

		if ( auxiliar[i] == POCIONES ){

			juego->ayudas[i].vida_a_recuperar = CURACION;
		}else {

			juego->ayudas[i].vida_a_recuperar = SIN_CURACION;
		}
	}
}


// Pre: Función que recibe dos tipos de datos coordenada_t;
// Pos: Devolvera true si cumple con las distancia manhattan, caso contrario false.
bool cumple_distancia_manhattan ( coordenada_t coordenada_uno, coordenada_t coordenada_dos ){

	bool si_cumple = false;
	int distancia_filas =  coordenada_uno.fil - coordenada_dos.fil;
	int distancia_columnas = coordenada_uno.col - coordenada_dos.col;

	if ( (abs( distancia_filas ) + abs( distancia_columnas )) > DISTANCIA_MANHATTAN ){

		si_cumple = true;
	}

	return si_cumple;
}

// Pre: Procidimiento que recibe un puntero a una variable del registro juego_t y una matriz de rango TAMANIOxTAMANIO.
// Pos: Inicializará la estructura rival_t.
void inicializar_rival ( juego_t* juego, char laberinto_copia[TAMANIO][TAMANIO] ){

	juego->rival.posicion = asignar_posicion (laberinto_copia);

	while ( !cumple_distancia_manhattan (juego->copa.posicion, juego->rival.posicion) ){

		juego->rival.posicion = asignar_posicion (laberinto_copia);
	}

	laberinto_copia [juego->rival.posicion.fil][juego->rival.posicion.col] = RIVAL;
	juego->rival.cantidad_pasos = MAXIMO_PASOS;
	juego->rival.direccion = DERECHA;
	juego->rival.codigo = RIVAL;
}


// Pre: Procidimiento que recibe un puntero a una variable del registro juego_t y una matriz de rango TAMANIOxTAMANIO.
// Pos: Inicializará la estructura jugador_t.
void inicializar_jugador ( juego_t * juego, char laberinto_copia[TAMANIO][TAMANIO] ){

	juego->jugador.posicion = asignar_posicion ( laberinto_copia );

	while ( !cumple_distancia_manhattan ( juego->copa.posicion, juego->jugador.posicion ) ){

		juego->jugador.posicion = asignar_posicion ( laberinto_copia );		
	}

	laberinto_copia [juego->jugador.posicion.fil][juego->jugador.posicion.col] = JUGADOR;	
	juego->jugador.tope_ayudas = SIN_AYUDAS;
	juego->jugador.vida = VIDA;
	juego->jugador.codigo = JUGADOR;
}


// Pre: Recibe dos matrices de igual tamaño, de rango TAMANIOxTAMANIO.
// Pos: Copia la primera matriz en la segunda.
void copiar_matriz ( char matriz_a_copiar[TAMANIO][TAMANIO], char matriz_copia[TAMANIO][TAMANIO] ){

	for ( int fila = 0; fila < TAMANIO; fila++ ){
		for ( int columna = 0; columna < TAMANIO; columna++ ){

			matriz_copia[fila][columna] = matriz_a_copiar[fila][columna];
		}
	}
}


// Pre: Procidimiento que recibe un puntero a una variable del registro juego_t y una matriz de rango TAMANIOxTAMANIO.
// Pos: Inicializará la estructura copa_t.
void inicializar_copa ( juego_t* juego, char laberinto_copia[TAMANIO][TAMANIO] ){

	juego->copa.posicion = asignar_posicion ( laberinto_copia );
	juego->copa.codigo = COPA;
	laberinto_copia [juego->copa.posicion.fil][juego->copa.posicion.col] = COPA;
}


// Pre: Procidimiento que recibe un puntero a una variable del registro juego_t.
// Pos: Inicializará todas las estructuras del laberinto.
void inicializar_laberinto( juego_t* juego ){

	char laberinto_copia[TAMANIO][TAMANIO];

	inicializar_paredes_laberinto ( juego->laberinto_original );

	copiar_matriz ( juego->laberinto_original, laberinto_copia );

	inicializar_copa ( juego, laberinto_copia );

	inicializar_obstaculos ( juego, laberinto_copia );

	inicializar_ayudas ( juego, laberinto_copia );

	inicializar_rival ( juego, laberinto_copia );

	inicializar_jugador ( juego, laberinto_copia );
}


// Pre: Función que recibe una tecla (caracter) ingresada anteriormente.
// Pos: Valida si la tecla ingresada es valida.
bool tecla_valida ( char tecla ){

	bool es_valida = false;

	if ( (tecla == ABAJO) || (tecla == ARRIBA) || (tecla == DERECHA) || (tecla == IZQUIERDA) ){

		es_valida = true;
	}
	return es_valida;
}


// Pre: Función que recibe una dirección (un caracter) y una posición (coordenada_t).
// Pos: Se encargará de verificar que no se salga de la matriz al avanzar.
bool bordes_descartados ( char direccion, coordenada_t posicion ){

	bool descartado = false;

	if ( (posicion.fil == BORDE_MINIMO) && (direccion == ARRIBA) ){

		descartado = true;
	}
	if ( (posicion.fil == BORDE_MAXIMO) && (direccion == ABAJO) ){

		descartado = true;
	}
	if ( (posicion.col == BORDE_MINIMO) && (direccion == IZQUIERDA) ){

		descartado = true;
	}
	if ( (posicion.col == BORDE_MAXIMO) && (direccion == DERECHA) ){

		descartado = true;
	}
	return descartado;
}


// Pre: Función que recibe una matriz de rango TAMANIOxTAMANIO, una dirección y una posición. 
// Pos: Devolvera true si la dirección evaluada es valida, sino false.
bool es_direccion_valida ( char laberinto[TAMANIO][TAMANIO], char direccion, coordenada_t posicion ){
	
	bool es_valido = false;

	if ( !bordes_descartados ( direccion, posicion ) ){

		if ( (direccion == DERECHA) && (laberinto[posicion.fil][posicion.col+1] != PARED) ){

			es_valido = true;
		}
		if ( (direccion == ARRIBA) && (laberinto[posicion.fil-1][posicion.col] != PARED) ){

			es_valido = true;
		}
		if ( (direccion == IZQUIERDA) && (laberinto[posicion.fil][posicion.col-1] != PARED) ){

			es_valido = true;
		}
		if ( (direccion == ABAJO) && (laberinto[posicion.fil+1][posicion.col] != PARED) ){

			es_valido = true;
		}
	}
	return es_valido;
}


// Pre: Procidimiento que recibe un puntero a una variable del registro juego_t y un caracter
// Pos: Devolverá true en caso de que la tecla sea valida y además la dirección lo sea también.
bool es_movimiento_valido(juego_t* juego, char tecla){

	bool es_valido = false;

	if ( es_direccion_valida ( juego->laberinto_original, tecla, juego->jugador.posicion ) && tecla_valida ( tecla ) ){

		es_valido = true;
	}
	return es_valido;
}


//  Quiero aclarar que hice esta función ya que vi la forma de hacerla de una forma más general, 
// es decir, la termine reutilizando en la función mover_rival como en mover_jugador, y asi terminar escribiendo menos código.

// Pre: Recibe un puntero de una variable coordenada_t y una dirección ya validada.
// Pos: Hara el movimiento en la dirección recibida.
void hacer_movimiento (coordenada_t* posicion, char direccion){

	if ( direccion == DERECHA ){

		posicion->col++;
	}
	if ( direccion == ABAJO ){

		posicion->fil++;
	}
	if ( direccion == IZQUIERDA ){

		posicion->col--;
	}
	if ( direccion == ARRIBA ){

		posicion->fil--;
	}
}

// Tuve que dejar asi el procedimiento ya que no se puede modificar las firmas para que pase las pruebas.

// Pre: Procedimiento que recibe un puntero de una variable juego_t y una direccion ya validada.
// Pos: Hara el movimiento en la direccion recibida.
void mover_jugador ( jugador_t* jugador, char direccion ){

	hacer_movimiento ( &(jugador->posicion), direccion );
}


// Pre: Procedimiento que recibe por referencia la direccion del rival.
// Pos: Cambiará su direccion a la siguiente, en su orden ya predefinido.
void cambiar_direccion_rival ( char* direccion ){

	if ( (*direccion) == DERECHA ){

		(*direccion) = ABAJO;
	}else if ( (*direccion) == ABAJO ){

		(*direccion) = IZQUIERDA;
	}else if ( (*direccion) == IZQUIERDA ){

		(*direccion) = ARRIBA;
	}else if ( (*direccion) == ARRIBA ){

		(*direccion) = DERECHA;
	}
}


// Pre: Procedimiento que recibe un puntero de una variable juego_t.
// Pos: Moverá al rival, teniendo en cuenta la cantidad de pasos y la dirección.
void mover_rival(juego_t* juego){

	bool se_movio = false;

	while ( !se_movio ){

		if ( es_direccion_valida ( juego->laberinto_original, juego->rival.direccion, juego->rival.posicion ) ){

			hacer_movimiento ( &(juego->rival.posicion), juego->rival.direccion );
			juego->rival.cantidad_pasos--;
			se_movio = true;

			if ( juego->rival.cantidad_pasos == SIN_PASOS ){

				cambiar_direccion_rival ( &(juego->rival.direccion) );
				juego->rival.cantidad_pasos = MAXIMO_PASOS;
			}
		}else {

			cambiar_direccion_rival ( &(juego->rival.direccion) );
			juego->rival.cantidad_pasos = MAXIMO_PASOS;
		}
	}
}

// Pre: Función que recibira dos coordenadas.
// Pos: Devolvera true si son iguales, sino false.
bool posiciones_iguales ( coordenada_t posicion_uno, coordenada_t posicion_dos ){

	bool son_iguales = false;

	if ( (posicion_uno.fil == posicion_dos.fil) && (posicion_uno.col == posicion_dos.col) ){

		son_iguales = true;
	}
	return son_iguales;
}


// Pre: Procedimiento que recibe por referencia la vida del jugador y por valor la vida que se curará.
// Pos: Sumará la vida a recuparar a la vida del jguaador, sin embargo, no se pasará de la vida establecida.
void tomar_pocion ( int* vida_jugador, int vida_a_recuperar ){

	(*vida_jugador) += vida_a_recuperar;

	if ( (*vida_jugador) > VIDA ){

		(*vida_jugador) = VIDA;
	}
}


// Pre: Procedimiento que recibe un puntero de una variable juego_t.
// Pos: Actualizará todas las ayudas restantes del juego.
void actualizar_ayudas ( juego_t* juego ){

	for ( int i = 0; i < juego->tope_ayudas ; i++ ){

		if ( posiciones_iguales (juego->jugador.posicion, juego->ayudas[i].posicion) ){

			if ( juego->ayudas[i].codigo == POCIONES ){

				tomar_pocion ( &(juego->jugador.vida), juego->ayudas[i].vida_a_recuperar );
			}else {

				juego->jugador.ayudas[juego->jugador.tope_ayudas].codigo = juego->ayudas[i].codigo; 
				juego->jugador.tope_ayudas++;
			}

			juego->ayudas[i] = juego->ayudas[juego->tope_ayudas-1];  
			juego->tope_ayudas--;

		}
	}
}


// Pre: Función que recibe la estructura jugador_t y un hechizo (un caracter).
// Pos: Devolverá true si el hechizo mandado pertenece al vector ayudas del jugador.
bool tiene_ayuda ( jugador_t jugador, char ayuda ){

	bool si_tiene = false;

	for ( int i = 0; i < jugador.tope_ayudas; i++ ){

		if ( jugador.ayudas[i].codigo == ayuda ){

			si_tiene = true;
		}
	}
	return si_tiene;
}


// Pre: Procedimiento que recibe por referencia la vida del jugador y por valor la vida que perderá.
// Pos: Restará a la vida del jugador.
void disminuir_vida ( int* vida, int vida_perdida ){// Tiene como objetivo aportar más legibilidad al código.

	(*vida) -= vida_perdida; 
}


// Pre: Procedimiento que recibe un puntero de una variable juego_t.
// Pos: Actualizará todas las obstáculos restantes del juego.
void actualizar_obstaculos ( juego_t* juego ){

	for ( int i = 0; i < juego->tope_obstaculos; i++ ){

		if ( posiciones_iguales (juego->jugador.posicion, juego->obstaculos[i].posicion) ){

			if ( (juego->obstaculos[i].codigo == ESCREGUTO) && (!tiene_ayuda (juego->jugador, IMPEDIMENTA)) ){

				disminuir_vida (&(juego->jugador.vida), juego->obstaculos[i].danio);
			}
			if ( (juego->obstaculos[i].codigo == BOGGART) && (!tiene_ayuda (juego->jugador, RIDDIKULUS)) ){

				disminuir_vida (&(juego->jugador.vida), juego->obstaculos[i].danio);
			}
			if ( juego->obstaculos[i].codigo == ACROMANTULA ){

				disminuir_vida (&(juego->jugador.vida), juego->obstaculos[i].danio);
			}

			juego->obstaculos[i] = juego->obstaculos[juego->tope_obstaculos-1];  
			juego->tope_obstaculos--;									
		}
	}
}


// Pre: Procedimiento que recibe un puntero de una variable juego_t.
// Pos: Actualizará todas las estructuras del juego.
void actualizar_juego ( juego_t* juego ){

	disminuir_vida (&(juego->jugador.vida), VIDA_PERDIDA_PASO);

	if ( juego->jugador.vida > SIN_VIDA ){

		actualizar_ayudas ( juego );

		actualizar_obstaculos ( juego );
	}
}


// Pre: Función que recibe el registro juego_t.
// Pos: Devolverá 1 si gano el usuario, 0 si esta en curso y -1 si gana el rival.
int estado_juego(juego_t juego){

	int estado = EN_CURSO;

	if ( juego.jugador.vida <= SIN_VIDA ){

		estado = DERROTA;
	}else if ( posiciones_iguales (juego.jugador.posicion, juego.copa.posicion) ){

		estado = VICTORIA;
	}else if ( posiciones_iguales (juego.rival.posicion, juego.copa.posicion) ){

		estado = DERROTA;
	}
	return estado;
}


// Pre: Función que recibe el registro juego_t 
// Pos: Devolverá true si el usuario puede ver la copa, sino false. 
bool puede_ver_la_copa ( juego_t juego ){

	bool si_puede = false;

	if ( (juego.jugador.vida <= POCA_VIDA) || (tiene_ayuda ( juego.jugador, ESFINGE )) ){

		si_puede = true;
	}
	return si_puede;
}


// Pos: Copiará el elemento en la matriz auxiliar.
void actualizar_elemento ( char laberinto[TAMANIO][TAMANIO], char elemento, coordenada_t posicion ){

	laberinto[posicion.fil][posicion.col] = elemento;
}

// Pre: Procedimiento que recibe el registro juego_t y una matriz de rango TAMANIOxTAMANIO.
// Pos: Actualizará todas las estructuras del juego.
void actualizar_laberinto( juego_t juego, char laberinto[TAMANIO][TAMANIO] ){

	copiar_matriz ( juego.laberinto_original, laberinto );

	if ( puede_ver_la_copa (juego) ){

		actualizar_elemento ( laberinto, juego.copa.codigo, juego.copa.posicion );
	}

	for ( int i = 0; i < juego.tope_obstaculos; i++ ){

		actualizar_elemento	( laberinto, juego.obstaculos[i].codigo, juego.obstaculos[i].posicion );
	}

	for ( int i = 0; i < juego.tope_ayudas; i++ ){

		actualizar_elemento ( laberinto, juego.ayudas[i].codigo, juego.ayudas[i].posicion );		
	}

	actualizar_elemento	( laberinto, juego.rival.codigo, juego.rival.posicion );
	actualizar_elemento ( laberinto, juego.jugador.codigo, juego.jugador.posicion );
}