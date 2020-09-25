#include <stdbool.h>
#include <stdio.h>

const char NO = 'N';
const int SIN_CAMPEON = 0;
const int EDAD_MINIMA = 17;
const char BUENA = 'B';
const char REGULAR = 'R';  
const char MALA = 'M';
const int BUENA_NUMERICA = 3;
const int REGULAR_NUMERICA = 2;
const int MALA_NUMERICA = 1;
const int MAXIMO_RANGO = 10;
const int MINIMO_RANGO = 0;


/* Precondicion: Funcion que recibe un caracter
   Postcondicion: Devuelve un tipo de dato bool con respecto a si el dato ingresado esta en el rango permitido, dentro = true, fuera = false. */
bool verificar_caracter (char caracter){
	bool esta_rango = false;
	if ( (caracter == REGULAR) || (caracter == MALA) || (caracter == BUENA) ){
		esta_rango = true;
	}
	return esta_rango;
}


/* Precondicion: Funcion que recibe un entero 
   Postcondicion: Devuelve un tipo de dato bool con respecto a si el dato ingresado esta en el rango permitido, dentro = true, fuera = false. */
bool verificar_entero (int entero){
	bool esta_rango = false;

	if ( (MINIMO_RANGO <= entero) && (entero <= MAXIMO_RANGO) ){
		esta_rango = true;
	}
	return esta_rango;
}


/* Precondicion: Procedimiento en el cual se le pide al usuario que reingrese el dato 
   Postcondicion: Devuelve por referencia el dato en el formato correcto */
void reingresar_caracter (char* caracter){
	while ( !verificar_caracter(*caracter) ) {

		printf ("Reingrese el dato en el formato/rango aclarado (%c = Mala, %c=Regular,%c=Buena):\n",MALA,REGULAR,BUENA);
		scanf ( " %c", caracter);

	}
}


/* Precondicion: Procedimiento en el cual se le pide al usuario que reingrese el dato
   Postcondicion: Devuelve por referencia el dato en el formato correcto */
void reingresar_entero (int* entero){

	while ( !verificar_entero (*entero) ) {

		printf ("Reingrese el dato en el formato/rango aclarado (de un rango del %i al %i):\n",MINIMO_RANGO,MAXIMO_RANGO);
		scanf ("%i",entero);
	}
} 


/* Tome como criterio asignarle un valor entero a cada capacidad magica y comparar esos valores numericos,
en vez de los caracteres. 
Precondicion: Funcion que recibe la capacidad magica en forma de un caracter 
Postcondicion: Se devuelve la capacidad magica en formato de un entero.*/
int asignar_numeracion (char capacidad){

	int capacidad_numerica;
	if ( capacidad == MALA ){
		capacidad_numerica = MALA_NUMERICA;

	} else if ( capacidad == REGULAR ){
		capacidad_numerica = REGULAR_NUMERICA;

	} else {
		capacidad_numerica = BUENA_NUMERICA;

	}
	return capacidad_numerica;
}


/* Precondicion: Procedimiento que pide los datos del campeon, la edad debe ingresarse en caracteres numericos
   Postcondicion: Devuelve los datos por parametro por referencia */
void pedir_datos(int* edad, int* fuerza,int* inteligencia,char* capacidad ){

	printf ("Ingrese los datos del campeon.\nEdad:");
	scanf ("%i",edad);


	if ( (*edad) >= EDAD_MINIMA ){

		printf ("Inteligencia(de un rango del %i al %i):\n",MINIMO_RANGO,MAXIMO_RANGO);
		scanf ("%i",inteligencia);

		if ( !verificar_entero (*inteligencia) ){
			reingresar_entero (inteligencia);
		}

		printf ("Fuerza(de un rango del %i al %i):\n",MINIMO_RANGO,MAXIMO_RANGO);
		scanf ("%i",fuerza);

		if ( !verificar_entero (*fuerza) ){
			reingresar_entero (fuerza);
		}

		printf ("Capacidad Magica(%c = Mala, %c=Regular,%c=Buena):\n",MALA,REGULAR,BUENA);
		scanf (" %c", capacidad);

		if ( !verificar_caracter (*capacidad) ){
			reingresar_caracter (capacidad);
		}
	}
}


/* Precondicion: Funcion que recibe los datos del campeon actual con el alunmo recientemente ingresado 
   Postcondicion: Devuelve true si el alunmo nuevo es mejor que al campeon actual, caso contrario, un false */
bool es_mejor_campeon (int inteligencia,int inteligencia_campeon,int fuerza,int fuerza_campeon,int capacidad_numerica,int capacidad_campeon_numerica){
	bool es_mejor = false;
	if ( inteligencia > inteligencia_campeon ){
		es_mejor = true;

	}else if ( (inteligencia == inteligencia_campeon) && (fuerza > fuerza_campeon) ){
		es_mejor = true;

	}else if ( (inteligencia == inteligencia_campeon) && (fuerza == fuerza_campeon) && (capacidad_numerica > capacidad_campeon_numerica) ){
		es_mejor = true;

	}
	return es_mejor;
}


/* Precondicion: Procedimiento que se llama solo si es_mejor_campeon es true,
 y recibe los datos del campeon actual y del alumno recientemente ingresado
   Postcondicion: Los datos del alumno ingresado pasan a ser los datos del campeon actual */
void cambiar_campeon (int inteligencia,int* inteligencia_campeon,int fuerza,int* fuerza_campeon,char capacidad,char* capacidad_campeon,int alumno,int* campeon_actual,int capacidad_numerica,int* capacidad_campeon_numerica){
	
	(*inteligencia_campeon) = inteligencia;
	(*fuerza_campeon) = fuerza;
	(*capacidad_campeon) = capacidad;
	(*campeon_actual) = alumno;
	(*capacidad_campeon_numerica) = capacidad_numerica;

}


/* Precondicion: Recibe el campeon actual
   Postcondicion: Imprime el campeon actual */
void imprimir_campeon (int campeon_actual){
	if ( campeon_actual == SIN_CAMPEON ){
		printf ("No hay campeón :(\n");
	}else {
		printf ("El campeón es el número %i :)\n",campeon_actual);
	}
}

/* Precondicion: Procedimiento que pregunta si quiere ingresar un alumno 
   Postcondicion: Devuelve por referencia la respuesta del usuario */
void nuevo_ingreso (char* respuesta){

	printf ("Desea ingresar un alumno? (N=no, cualquier otra tecla para si)\n");
	scanf (" %c",respuesta);
}

/*
Precondicion: Programa que recibe los datos de todos los alumnos que se quieran ingresar
Postcondicion: Se informara el alumno mas apto para ser campeon, tambien en caso de no haber un campeon */
int main (){
	int edad, inteligencia, fuerza,capacidad_numerica;
	char capacidad;
	char respuesta;
	int alumno = 0;
	int campeon_actual = 0;
	int inteligencia_campeon = 0, fuerza_campeon = 0,capacidad_campeon_numerica = 0;
	char capacidad_campeon;

	nuevo_ingreso (&respuesta);

	while (respuesta != NO){
		pedir_datos (&edad, &fuerza,&inteligencia,&capacidad);

		if (edad >= EDAD_MINIMA){
			capacidad_numerica = asignar_numeracion (capacidad);
		
		}
		alumno ++;

		if( es_mejor_campeon (inteligencia, inteligencia_campeon, fuerza, fuerza_campeon, capacidad_numerica, capacidad_campeon_numerica) ){
			cambiar_campeon (inteligencia, &inteligencia_campeon, fuerza, &fuerza_campeon, capacidad, &capacidad_campeon, alumno, &campeon_actual, capacidad_numerica, &capacidad_campeon_numerica);
		}
		nuevo_ingreso (&respuesta);

	}
	imprimir_campeon (campeon_actual);

	return 0;
}