#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "./ncurses-5.9/include/curses.h"
#include <string.h>


/**
 * Stala, ktora okresla maksymalna ilosc rownan (i jednoczesnie niewiadomych) - zakladam symetrie w maksymalnym, brzegowym przypadku
**/
const int MAX_N = 12; //maksymalny rozmiar macierzy wertykalny i horyzontalny


/**
 * Ustawia kazdy element tablicy na -1.0. Dzieki temu macierz nie przechowuje jakis dziwnych wartosci.
 * @param t Macierz, ktora chcemy wyczyscic
 * @result Funkcja jest typu void. Zmienia wartosc macierzy t podanej jako argument
**/
void clearMatrix(double t[MAX_N][MAX_N]);
/**
 * Ustawia kazdy element tablicy na i+j, gdzie i oraz j to odpowiednio numer wiersza i kolumny tej macierzy (numerowany od zera). Dzieki temu macierz nie przechowuje jakis dziwnych wartosci.
 * @param t Macierz, ktora chcemy wyczyscic
 * @result Funkcja jest typu void. Zmienia wartosc macierzy t podanej jako argument
**/
void clearMatrixN(double t[MAX_N][MAX_N]);
/**
 * Drukuje na standardowe wyjscie macierz
 * @param m Macierz, ktora chcemy wydrukowac
 * @param hSize Poziomy rozmiar macierzy
 * @param vSize Pionowy rozmiar macierzy
 * @result Funkcja jest typu void. Drukuje macierz na standardowe wyjscie
**/
void printfMatrix(double m[MAX_N][MAX_N], int hSize, int vSize);
/**
 * Funkcja zmienia macierz m wymiaru hSize na vSize podana w argumencie na macierz z wyrzuconym I-tym wierszem i J-ta kolumna. Ma ona oczywiscie wymiary (hSize-1) na (vSize-1).
 * @param m Macierz, ktora chcemy zmienic
 * @param hSize Poziomy rozmiar macierzy
 * @param vSize Pionowy rozmiar macierzy
 * @param I Wiersz, ktorego chcemy sie pozbyc z macierzy
 * @param J Kolumna, ktorej chcemy sie pozbyc z macierzy
 * @result Funkcja jest typu void. Zmienia macierz m podana jako argument.
**/
void matrixWithoutIJ(double m[MAX_N][MAX_N], int hSize, int vSize, int I, int J);
/**
 * Funkcja kopiuje macierz druga m2 do macierzy pierwszej w zakresie wyznaczonym przez zmienne: hSize (poziomo) oraz vSize (pionowo)
 * @param m1 Macierz, na ktora kopiujemy dane
 * @param m2 Macierz, z ktorej kopiujemy dane
 * @param hSize Poziomy rozmiar macierzy
 * @param vSize Pionowy rozmiar macierzy
 * @result Funkcja jest typu void. Zmienia macierz m1 podana jako argument.
**/
void matrix_S2F(double m1[MAX_N][MAX_N], double m2[MAX_N][MAX_N], int hSize, int vSize);
/**
 * Funkcja ustawia elementy macierzy m (podanej jako argument) na takie, jakie chce uzytkownik. Ustawia rowniez wymiary odpowiednio poziome (hSize) oraz pionowe (vSize) podane jako zmienne wskaznikowe w argumencie funkcji.
 * @param m Macierz, ktorej elementy chcemy ustawic
 * @param *hSize Wskaznik na poziomy rozmiar macierzy
 * @param *vSize Wskaznik na pionowy rozmiar macierzy
 * @result Funkcja jest typu void. Zmienia macierz m tak, jak tego chce uzytkownik (to znaczy ustawia rozmiar macierzy [vSize, hSize] oraz zmienia wartosci macierzy w danym zakresie)..
**/
void setMatrix(double m[MAX_N][MAX_N], int *hSize, int *vSize);
/**
 * Funkcja liczy wyznacznik danej macierzy. Jest to realizowane zgodnie z rekurencyjna definicjia wyznacznika popdrzez rozwiniecie Laplace'a.
 * @param m Macierz, ktorej elementy chcemy ustawic
 * @param hSize Poziomy rozmiar macierzy
 * @param vSize Pionowy rozmiar macierzy
 * @result Liczba double, bedaca wartoscia wyznacznika z macierzy m podanej jako argument funkcji.
**/
double matrix2Det(double m[MAX_N][MAX_N],int hSize, int vSize);


/**
 * Program rozwiazuje uklady rownan.
 * @author Tomasz Potanski - 238 756
 **/
int main()
{


    int i, j;
    double matrix[MAX_N][MAX_N];
//    double matrix2[MAX_N][MAX_N];

    initscr();
		raw();
		start_color();
		//noecho();
		keypad(stdscr, TRUE);
    clearMatrix(matrix);
    int hSize, vSize, row, col;

		getmaxyx(stdscr, row, col);
		char msg1[] = "Witaj, ten program sluzy do rozwiazywania ukladow rownan.";

		char msg2[] = "Autorem jest Tomasz Potanski - 238 756";
		char msg3[] = "Projekt 2.2 PRI.";
		init_pair(1, COLOR_CYAN, COLOR_BLACK);

		attron(A_STANDOUT | COLOR_PAIR(1));
	mvprintw(row/2 - 3, (col - strlen(msg3))/2 ,"%s", msg3);
	attroff(A_STANDOUT | COLOR_PAIR(1));
	mvprintw(row/2 - 2, (col - strlen(msg1))/2 ,"%s", msg1);
	attron(A_UNDERLINE);


	mvprintw(row/2-1, (col - strlen(msg2))/2 ,"%s", msg2);
	attroff(A_UNDERLINE);
	char msg5[] = "<< Nacisnij jakis klawisz, aby przejsc dalej >> ";


	mvprintw(row/2+3, (col - strlen(msg5))/2 ,"%s", msg5);
	move(row/2+4, col/2);
	curs_set(0);
	refresh();
	getch();

	clear();
    msg1[] = "Program rozwiazuje dany przez Ciebie uklad rownan metoda macierzowa/wyznacznikowa.";
    msg2[] = "Pobiera macierz. Elementy w jednym wierszu odpowiadaja wspolczynnika przy odpowiednich niewiadomych.";
    msg3[] = "Im wiekszy jest rozmiar pionowy, tym wiecej jest rownan pozwalajacych rozwiazac dany uklad.";
    msg4[] = "Jezeli rownan jest mniej niz niewiadomych, to uklad ma nieskonczenie wiele rozwiazan.";
    msg5[] = "Powyzszy uklad nie zostanie rozwiazany.";
    char msg6[] = "Nie zostanie rozwiazany takze uklad sprzeczny.";
    //char msg7[] = "Nie zostanie rozwiazany takze uklad sprzeczny.";
    char msg7[] = "<< Jezeli zrozumiales, nacisnij klawisz, aby rozpoczac wprowadzanie macierzy >>";

    mvprintw(row/2+3, (col - strlen(msg1))/2 ,"%s", msg1);
    mvprintw(row/2+2, (col - strlen(msg2))/2 ,"%s", msg2);
    mvprintw(row/2+1, (col - strlen(msg3))/2 ,"%s", msg3);
    mvprintw(row/2, (col - strlen(msg4))/2 ,"%s", msg4);
    mvprintw(row/2-1, (col - strlen(msg5))/2 ,"%s", msg5);
    mvprintw(row/2-2, (col - strlen(msg6))/2 ,"%s", msg6);
    mvprintw(row/2-5, (col - strlen(msg7))/2 ,"%s", msg7);

	refresh();
	getch();

   // setMatrix(matrix, &hSize, &vSize);
  //  double det = matrix2Det(matrix, hSize, vSize);
   // printf("\nWyznacznik wynosi: %lf", det);
   // printf("\n\n");
   // printfMatrix(matrix, hSize, vSize);
   // printf("\n\n");

    //printfMatrix(matrix2, 5, 5);

    //printfMatrix(matrix, 4, 4);



    endwin();
    return 0;
}

void setMatrix(double m[MAX_N][MAX_N], int *hSize, int *vSize){
printf("\nWprowadz rozmiar vertykalny macierzy (<%d): ", MAX_N);
scanf("%d", vSize);
printf("\nWprowadz rozmiar horyzontalny macierzy (<%d): ", MAX_N);
scanf("%d", hSize);
printf("\n-----------\nWprowadz kolejne elementy macierzy");

int i, j;
for (i = 0; i < *vSize; i++){
    for (j = 0; j < *hSize; j++){
        printf("\nWprowadz element znajdujacy sie w %d wierszu i w %d kolumnie: ", i+1, j+1);
        double a;
        scanf("%lf", &a);
        m[i][j] = a;
    }
}

}

void matrix_S2F(double m1[MAX_N][MAX_N], double m2[MAX_N][MAX_N], int hSize, int vSize){
    int i, j;
    for (i = 0 ; i < vSize; i++){
        for (j = 0; j < hSize; j++){
            m1[i][j] = m2[i][j];
        }
    }
}

void matrixWithoutIJ(double m[MAX_N][MAX_N], int hSize, int vSize, int I, int J){
    if (I < vSize-1){
        int i;
        for (i = I+1 ; i <= vSize ; i++){
            int h;
            for (h = J+1; h <= hSize ; h++){
                m[i-1][h-1] = m[i][h];
            }
        }
    }
        int i, k;
        for (k = 0 ; k < I ; k++){
            for (i = J+1 ; i <= hSize ; i++){
                m[k][i-1] = m[k][i];
            }
        }

        for (k = 0; k < J ; k++){
            for (i = I+1; i <= vSize ; i++){
                m[i-1][k] = m[i][k];
            }
        }

}

double matrix2Det(double m[MAX_N][MAX_N],int hSize, int vSize){
    double result=0.0;
    if (hSize == 1 && vSize == 1){
        result = m[0][0];
    } else if (hSize == 2 && vSize == 2){
        result = m[0][0]*m[1][1]-m[1][0]*m[0][1];
    } else if (hSize > 2 && vSize > 2){
        double newMatrix[MAX_N][MAX_N];
        clearMatrix(newMatrix);
        matrix_S2F(newMatrix, m, hSize, vSize);
        int i;
        int j = 0;
        for (i = 0; i < vSize ; i++){
            matrix_S2F(newMatrix, m, hSize, vSize);
            matrixWithoutIJ(newMatrix, hSize, vSize, i, j);
            result+=pow(-1, i+j)*m[i][j]*matrix2Det(newMatrix, hSize-1, vSize-1);
        }

    }


    return result;

}

void clearMatrix(double t[MAX_N][MAX_N]){
    int i, j;
    for (i = 0; i < MAX_N ; i++){
        for (j = 0 ; j < MAX_N ; j++){
            t[i][j] = -1.0;
        }
    }
    //return 3.0;
}

void clearMatrixN(double t[MAX_N][MAX_N]){
    int i, j;
    for (i = 0; i < MAX_N ; i++){
        for (j = 0 ; j < MAX_N ; j++){
            t[i][j] = i+j;
        }
    }
    //return 3.0;
}

void printfMatrix(double m[MAX_N][MAX_N], int hSize, int vSize){
    int i, j;
    for (i = 0; i < vSize ; i++){
        printf("| ");
        for (j = 0 ; j < hSize ; j++){
            printf("%lf ", m[i][j]);
        }
        printf("|\n");
    }
}
