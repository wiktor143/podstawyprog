#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 512       /* Maksymalny rozmiar wczytywanego obrazu */
#define DL_LINII 1024 /* Dlugosc buforow pomocniczych */

/************************************************************************************
 * Funkcja wczytuje obraz PGM z pliku do tablicy       	       	       	       	    *
 *										    *
 * \param[in] plik_we uchwyt do pliku z obrazem w formacie PGM			    *
 * \param[out] obraz_pgm tablica, do ktorej zostanie zapisany obraz		    *
 * \param[out] wymx szerokosc obrazka						    *
 * \param[out] wymy wysokosc obrazka						    *
 * \param[out] odcienie liczba odcieni odcienie					    *
 * \return liczba wczytanych pikseli						    *
 ************************************************************************************/

int czytaj(FILE *plik_we, int obraz_pgm[][MAX], int *wymx, int *wymy, int *odcienie)
{
  char buf[DL_LINII]; /* bufor pomocniczy do czytania naglowka i komentarzy */
  int znak;           /* zmienna pomocnicza do czytania komentarzy */
  int koniec = 0;     /* czy napotkano koniec danych w pliku */
  int i, j;

  /*Sprawdzenie czy podano prawid�owy uchwyt pliku */
  if (plik_we == NULL)
  {
    fprintf(stderr, "Blad: Nie podano uchwytu do pliku\n");
    return (0);
  }

  /* Sprawdzenie "numeru magicznego" - powinien by� P2 */
  if (fgets(buf, DL_LINII, plik_we) == NULL) /* Wczytanie pierwszej linii pliku do bufora */
    koniec = 1;                              /* Nie udalo sie? Koniec danych! */

  if ((buf[0] != 'P') || (buf[1] != '2') || koniec)
  { /* Czy jest magiczne "P2"? */
    fprintf(stderr, "Blad: To nie jest plik PGM\n");
    return (0);
  }

  /* Pominiecie komentarzy */
  do
  {
    if ((znak = fgetc(plik_we)) == '#')
    {                                            /* Czy linia rozpoczyna sie od znaku '#'? */
      if (fgets(buf, DL_LINII, plik_we) == NULL) /* Przeczytaj ja do bufora                */
        koniec = 1;                              /* Zapamietaj ewentualny koniec danych */
    }
    else
    {
      ungetc(znak, plik_we);        /* Gdy przeczytany znak z poczatku linii */
    }                               /* nie jest '#' zwroc go                 */
  } while (znak == '#' && !koniec); /* Powtarzaj dopoki sa linie komentarza */
                                    /* i nie nastapil koniec danych         */

  /* Pobranie wymiarow obrazu i liczby odcieni odcienie */
  if (fscanf(plik_we, "%d %d %d", wymx, wymy, odcienie) != 3)
  {
    fprintf(stderr, "Blad: Brak wymiarow obrazu lub liczby stopni odcienie\n");
    return (0);
  }
  /* Pobranie obrazu i zapisanie w tablicy obraz_pgm*/
  for (i = 0; i < *wymy; i++)
  {
    for (j = 0; j < *wymx; j++)
    {
      if (fscanf(plik_we, "%d", &(obraz_pgm[i][j])) != 1)
      {
        fprintf(stderr, "Blad: Niewlasciwe wymiary obrazu\n");
        return (0);
      }
    }
  }
  return *wymx * *wymy; /* Czytanie zakonczone sukcesem    */
} /* Zwroc liczbe wczytanych pikseli */

/* Wyswietlenie obrazu o zadanej nazwie za pomoca programu "display"   */
void wyswietl(char *n_pliku)
{
  char polecenie[DL_LINII]; /* bufor pomocniczy do zestawienia polecenia */

  strcpy(polecenie, "display "); /* konstrukcja polecenia postaci */
  strcat(polecenie, n_pliku);    /* display "nazwa_pliku" &       */
  strcat(polecenie, " &");
  printf("%s\n", polecenie); /* wydruk kontrolny polecenia */
  system(polecenie);         /* wykonanie polecenia        */
}

// funkcja zajmująca się negatywem obrazu
void negatyw(int obraz[][MAX], int wymx, int wymy, int odcienie)
{
  int i, j;

  for (i = 0; i < wymy; i++)
  {
    for (j = 0; j < wymx; j++)
    {
      obraz[i][j] = odcienie - obraz[i][j];
    }
  }
}

//funkcja zajmująca się progowaniem obrazu
void progowanie(int obraz[][MAX], int wymx, int wymy, int odcienie)
{
  int i, j;
  int prog = odcienie / 2;

  for (i = 0; i < wymy; i++)
  {
    for (j = 0; j < wymx; j++)
    {
      if (obraz[i][j] <= prog)
      {
        obraz[i][j] = 0;
      }
      else
      {
        obraz[i][j] = odcienie;
      }
    }
  }
}

void progowanie_czerni(int obraz[][MAX], int wymx, int wymy, int odcienie)
{
  int i, j;
  int prog = odcienie / 2;
  for (i = 0; i < wymy; i++)
  {
    for (j = 0; j < wymx; j++)
    {
      if (obraz[i][j] <= prog)
        obraz[i][j] = 0;
      else
        obraz[i][j] = obraz[i][j];
    }
  }
}



int main()
{
  int obraz[MAX][MAX];
  int wymx, wymy, odcienie;

  int odczytano = 0;
  FILE *plik;
  char nazwa[100];
  char nowanazwa[100];
  int opcja;
  int i, j;

  while (opcja != 6) //Proste menu do wyboru przez użytkownija za pomoca switcha
  {
    printf("Powiedz mi co chcesz zrobić ze swoim obrazem\n\n");
    printf("Proste menu: \n");
    printf("1-Wyświetl obraz który chcesz zmienić pamiętaj o .pgm!\n");
    printf("2-Wykonaj progowanien\n");
    printf("3-Wykonaj negatyw\n");
    printf("4-Wykonaj półprogowanie czerni\n");
    printf("5-Zapisz,wyświetl i zakończ\n");
    printf("6-Nie chce nic robić\n\n");
    printf("Chce wybrać opcje numer: ");
    scanf("%d", &opcja);
    switch (opcja)
    {
    case 1:
    {

      /*Wczytanie zawartosci wskazanego pliku do pamieci */
      printf("Podaj nazwe pliku:\n");
      scanf("%s", nazwa);
      plik = fopen(nazwa, "r");

      if (plik != NULL)
      { /* co spowoduje zakomentowanie tego warunku */
        odczytano = czytaj(plik, obraz, &wymx, &wymy, &odcienie);
        fclose(plik);
      }
      /* Wyswietlenie poprawnie wczytanego obraza zewnetrznym programem */
      if (odczytano != 0)
      {
        wyswietl(nazwa);
      }
      break;
    }
    case 2:
    {
      progowanie(obraz, wymx, wymy, odcienie);
      printf("Przetwarzam obraz\n\n\n");
      break;
    }
    case 3:
    {
      negatyw(obraz, wymx, wymy, odcienie);
      printf("Przetwarzam obraz\n\n\n");
      break;
    }
    case 4:
    {
      progowanie_czerni(obraz, wymx, wymy, odcienie);
      printf("Przetwarzam obraz\n\n\n");
      break;
    }
    case 5:
    {
      printf("Podaj nazwę pod jaką chcesz zapisać swój nowy plik .pgm:\n");
      scanf("%s", nowanazwa);
      plik = fopen(nowanazwa, "w");
      fprintf(plik, "P2\n");
      fprintf(plik, "%d %d\n%d\n", wymx, wymy, odcienie);
      for (i = 0; i < wymy; i++)
      {
        for (j = 0; j < wymx; j++)
        {
          fprintf(plik, " %d ", obraz[i][j]);
        }
        fprintf(plik, "\n");
      }
      wyswietl(nowanazwa);
      return odczytano;
      break;
    }
    case 6:
    }
  }
}