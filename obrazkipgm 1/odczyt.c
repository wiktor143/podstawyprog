#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 512       /* Maksymalny rozmiar wczytywanego obrazu */
#define DL_LINII 1024 /* Dlugosc buforow pomocniczych */

/************************************************************************************
 * Funkcja wczytuje obraz PGM z pliku do tablicy       	       	       	       	    *
 *										                                                              *
 * \param[in] plik_we uchwyt do pliku z obrazem w formacie PGM			                *
 * \param[out] obraz_pgm tablica, do ktorej zostanie zapisany obraz		              *
 * \param[out] wymx szerokosc obrazka						                                    *
 * \param[out] wymy wysokosc obrazka						                                    *
 * \param[out] odcienie liczba odcieni odcienie					                            *
 * \return liczba wczytanych pikseli						                                    *
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
    fprintf(stderr, "Blad: To nie jest plik PGM, rozpoczni od nowa.\n");
    exit(1);
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
      obraz[i][j] = odcienie - obraz[i][j]; // w pętli zmieniami wartości danych wejściowych na podstawie podanego wzoru
    }
  }
}

// funkcja zajmująca się progowaniem obrazu na 50%
void progowanie(int obraz[][MAX], int wymx, int wymy, int odcienie)
{
  int i, j;
  int prog = odcienie / 2; // tworzymy próg na podstawie maksymalnej szarości

  for (i = 0; i < wymy; i++)
  {
    for (j = 0; j < wymx; j++)
    {
      if (obraz[i][j] <= prog) // wartością poniżej 50% szarości przypisiwane jest 0
      {
        obraz[i][j] = 0;
      }
      else
      {
        obraz[i][j] = odcienie; // dla wartości powyżej progi przypisywana jest szarość maksymalna
      }
    }
  }
}
// funkcja zajmująca sie pół progowaniem czerni
void polprogowanie_czerni(int obraz[][MAX], int wymx, int wymy, int odcienie)
{
  int i, j;
  int prog = odcienie / 2;
  for (i = 0; i < wymy; i++)
  {
    for (j = 0; j < wymx; j++)
    {
      if (obraz[i][j] <= prog) // wartością poniżej 50% szarości przypisiwane jest 0
        obraz[i][j] = 0;
      else
        obraz[i][j] = obraz[i][j]; // wartości powyżej 50% szarości pozostają bez zmian
    }
  }
}
int zapisz(FILE *plik_ost, char *tabnazwa, int obrazkon[][MAX], int wymx, int wymy, int odcienie)
{
  int i, j;
  printf("Podaj nazwę pod jaką chcesz zapisać swój nowy plik .pgm:\n");
  scanf("%s", tabnazwa);                                  // zapis nazwy podanej przez użytkownika do tablicy
  plik_ost = fopen(tabnazwa, "w");                        // tworzenie pliku o podanej nazwie
  fprintf(plik_ost, "P2\n");                              // wpisanie do nowo utworzonego pliku, magicznego P2,
  fprintf(plik_ost, "%d %d\n%d\n", wymx, wymy, odcienie); // rozmiarów i szarości
  for (i = 0; i < wymy; i++)
  {
    for (j = 0; j < wymx; j++)
    {
      fprintf(plik_ost, " %d ", obrazkon[i][j]); // podmienie orginalnych wartości przez funkcje która była
    }                                            // użyta w menu wyboru
    fprintf(plik_ost, "\n");
  }
  printf("Utworzono nowy plik w aktualnym foldzerze o nazwie %s\n", tabnazwa);
  return 0;
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

  printf("Witaj w programie do przetwarzania obrazów pgm\n\n");
  do
  {
    printf("Podaj nazwe swojego pliku:\n");            // prosimy użytkownika o nazwę pliku wejściowego
    printf("(koniecznie pamiętaj o końcówce .pgm)\n"); // przypominamy aby plik kończył sie .pgm
    scanf("%s", nazwa);
    plik = fopen(nazwa, "r");

    if (plik == NULL)
    {
      printf("Blad: Nie ma takiego pliku!\n\n"); // jeżeli nie podamy uchwytu do pliku wyskakuje taki komunikat
    }

    if (plik != NULL)
    {
      odczytano = czytaj(plik, obraz, &wymx, &wymy, &odcienie);
      printf("liczba pikseli w pliku %s wynosi: %d", nazwa, odczytano); // program wyświetli nam liczbę pikseli w pliku
      fclose(plik);
    }
  } while (plik == NULL); // powtarzamy wczytywanie dopóki uzytkownik nie poda istniejącgo pliku

  do // Proste menu do wyboru przez użytkownika za pomoca switcha, wybranie cyfry 6 kończy działanie programu
  {
    printf("\n\n");
    printf("Powiedz mi co chcesz zrobić ze swoim obrazem, wybierz jedną z opcji poniżej.\n");
    printf("Proste menu: \n");
    printf("1-Chce wyświetlić wczytany obraz\n");
    printf("2-Wykonaj progowanie\n");
    printf("3-Wykonaj negatyw\n");
    printf("4-Wykonaj półprogowanie czerni\n");
    printf("5-Chce zobaczyć mój nowy obraz, zapisać go i wyjść\n");
    printf("6-Nie chce dzisiaj nic przerabiać\n\n");
    printf("Chce wybrać opcje numer: ");
    scanf("%d", &opcja);
    switch (opcja)
    {
    case 1: // opcja nr 1. Wyświetlenie podanego przez użytkownika obrazu
    {

      // Wyswietlenie poprawnie wczytanego obraza zewnetrznym programem
      if (odczytano != 0)
      {
        wyswietl(nazwa);
      }
      break;
    }
    case 2: // opcja nr 2. progowanie obrazu wczytanego do programu
    {
      progowanie(obraz, wymx, wymy, odcienie);
      printf("Przetwarzam obraz\n\n\n");
      break;
    }
    case 3: // opcja nr 3. negatyw obrazu wczytanego do programu
    {
      negatyw(obraz, wymx, wymy, odcienie);
      printf("Przetwarzam obraz\n\n\n");
      break;
    }
    case 4: // opcja nr 4. półprogowanie czerni w obrazie wczytanym do programu
    {
      polprogowanie_czerni(obraz, wymx, wymy, odcienie);
      printf("Przetwarzam obraz\n\n\n");
      break;
    }
    case 5: // opcja nr 5. zapis pod wskazaną przez użytkownika nazwą i zapis
    {
      zapisz(plik, nowanazwa, obraz, wymx, wymy, odcienie); // zapis przerobionego obrazu za pomocą funkcji zapisz
      wyswietl(nowanazwa);
      fclose(plik);
      return 0;
    }
    case 6: // opcja nr 6. zakończenie działania programu
    {
      printf("Mam nadzieje, że jeszcze kiedyś się zobaczymy :)\n\n");
    }
    }
  } while (opcja != 6); // warunek zamknięcia pętli
  return 0;
}