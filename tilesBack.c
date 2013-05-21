#include <stdio.h>
#include <stdlib.h>
#include "tilesBack.h"
#define PUNTO_FUERA(x, y) ((x) < 0 || (x) >= (y))

/* Enums */
enum {E, H, C, M, UNDO, SAVE, QUIT};

/* Funciones */

char **
crearTablero(tJuego * juego, int nivel)
{
  int i, j;

	printf("Creando tablero de %dx%d con %d colores...\n\n", juego->alto, juego->ancho, nivel+1);
	/* Creacion de filas */
	juego->tablero = malloc(juego->alto * sizeof(char *));
	
	for (i = 0; i < juego->alto; i++)
	{
		/* Creacion de columnas */
		juego->tablero[i] = malloc(juego->ancho * sizeof(char));
		
		for (j = 0; j < juego->ancho; j++)
			juego->tablero[i][j] = rand() % (nivel + 6) + 1;

	}
	
	return juego->tablero;
	
}

/* eliminar(int color, tPunto punto, tJuego * juego)
** Modo de uso:
** azulejosEliminados = eliminar(1, punto, juego);
** Elimina los azulejos adyacentes al punto 'punto' de color '1'
** en el tablero de 'juego' y devuelve cantidad de azulejos eliminados
*/

int
eliminar(int color, tPunto punto, tJuego * juego)
{
	int i, j = 0;
	tPunto puntosAdyacentes[4];
	
	/* Validacion del punto */
	if (PUNTO_FUERA(punto.x, juego->ancho) || PUNTO_FUERA(punto.y, juego->alto) || juego->tablero[punto.y][punto.x] == 0)
	{
		return 0;
	}
	
	/* Eliminacion */
	if (juego->tablero[punto.y][punto.x] != color)
		return 0;
	else
	{
		juego->tablero[punto.y][punto.x] = 0;
		crearAdyacentes(punto, puntosAdyacentes);
	
		/* Eliminar si corresponde los puntos
		* izquierda - arriba - abajo - derecha */
		for (i = 0; i < 4; i++)
			j += eliminar(color, puntosAdyacentes[i], juego);
			
		return 1 + j;
	}
}

/* Elimina la hilera 'hilera' en el tablero de 'juego' */
int
eliminarHilera(int hilera, tJuego * juego)
{
	int i, hayAzulejoVacio = 0;
	
	/* Verifica que al menos un azulejo no este vacio en la hilera */
	for (i = 0; i < juego->ancho && !hayAzulejoVacio; i++)
	{
		if (juego->tablero[hilera][i] == 0)
		{
			printf("No es posible eliminar la hilera\n");
			return 0;
		}
	}
	
	/* Ejecuta la eliminacion de la hilera */
	for (i = 0; i < juego->ancho; i++)
		juego->tablero[hilera][i] = 0;

	/*
	** TODO: Disminuir movimientos eliminarHilera
	*/
	
	return juego->ancho;
}

/* Elimina la columna 'columna' en el tablero de 'juego' */
int
eliminarColumna(int columna, tJuego * juego)
{
	int i, hayAzulejoVacio = 0;
	
	/* Verifica que no haya un azulejo vacio en la columna */
	for (i = 0; i < juego->alto && !hayAzulejoVacio; i++)
	{
		if (juego->tablero[i][columna] == 0)
		{	
			printf("No es posible eliminar la columna\n");
			printf("Hay un azulejo vacio\n");
			return 0;
		}
	}
	
	/* Elimina la columna */
	for (i = 0; i < juego->alto; i++)
		juego->tablero[i][columna] = 0;
		
	/*
	** TODO: Disminuir movimientos eliminarColumna
	*/
	
	return juego->alto;
}

/*
** Elimina los alrededores del parametro 'punto' si es posible
*/
int
eliminarMartillazo(tPunto punto, tJuego * juego)
{
	int i, j, azulejosEliminados = 0;
	
	/* Se vacian los alrededores y el centro */
	for (i = punto.y - 1; i <= punto.y; i++)
	{
		if (!PUNTO_FUERA(i, juego->alto))
		{
			for (j = punto.x - 1; j <= punto.x; j++)
			{
				if (!PUNTO_FUERA(j, juego->ancho) && juego->tablero[i][j] != 0)
				{
					juego->tablero[i][j] = 0;
					azulejosEliminados++;
				}
			}
		}
	}
	
	/*
	** TODO: Disminuir en uno los movimientos martillazo
	*/
	
	return azulejosEliminados;
}

/*
**
*/
int
hayColorAdyacente(tPunto punto, tJuego * juego)
{
	int color, i, hayAdyacente = 0, x, y;
	tPunto adyacentes[4];
	
	if (juego->tablero[punto.y][punto.x] == 0
	|| PUNTO_FUERA(punto.x, juego->ancho) || PUNTO_FUERA(punto.y, juego->alto))
	{
		printf("La coordenada (%d, %d) no es valida. \n", punto.x, punto.y);
		return 0;
	}
	
	color = juego->tablero[punto.y][punto.x];
	
	crearAdyacentes(punto, adyacentes);
	
	for (i = 0; i < 4 && !hayAdyacente; i++)
	{
		x = adyacentes[i].x;
		y = adyacentes[i].y;
		
		/* Validacion del punto cuando si no esta dentro del tablero */
		if (PUNTO_FUERA(x, juego->ancho) && PUNTO_FUERA(y, juego->alto))
			continue;
		
		if (juego->tablero[y][x] == color)
			hayAdyacente = 1;
	}
	
	return hayAdyacente;
}

/* crea 4 puntos adyacentes a 'punto' y los deja en 'adyacentes' */
void
crearAdyacentes(tPunto punto, tPunto adyacentes[])
{
	int i;

	for (i = 0; i < 4; i++)
	{
		/* Simplificacion de las posiciones de x: {x-1, x, x, x+1} */
		adyacentes[i].x = punto.x + (i+1) / 2 - 1;
		
		/* Posiciones de y: {x, x-1, x+1, x} */
		switch (i)
		{
			case 0:
			case 3:
				adyacentes[i].y = punto.y;
				break;
			case 1:
				adyacentes[i].y = punto.y - 1;
				break;
			case 2:
				adyacentes[i].y = punto.y + 1;
				break;
		}
	}
}

void
freeMemory(tJuego * juego)
{
	int i;
	
	for (i = juego->ancho; i; i--)
		free(juego->tablero[i]);
	
	free(juego->tablero);
	
	return;	
}