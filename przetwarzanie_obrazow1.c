#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1024            // Maksymalny rozmiar wczytywanego obrazu 
#define DL_LINII 1024      // Dlugosc buforow pomocniczych

 int wczytaj(FILE *plik_we,int obraz_pgm[][MAX],int *wymx,int *wymy, int *szarosci) {
           char buf[DL_LINII];      // Bufor pomocniczy do czytania naglowka i komentarzy
           int znak;                // Zmienna pomocnicza do czytania komentarzy
           int koniec=0;            // Czy napotkano koniec danych w pliku
           int i,j;

           // Sprawdzenie czy podano prawidlowy uchwyt pliku
           if (plik_we==NULL) {
           fprintf(stderr,"Blad: Nie podano uchwytu do pliku\n");
           return(0);}

           // Sprawdzenie "numeru magicznego"
           if (fgets(buf,DL_LINII,plik_we)==NULL)   // Wczytanie pierwszej linii pliku do bufora
           koniec=1;                                // Koniec danych
           if ( (buf[0]!='P') || (buf[1]!='2') || koniec) {  // Czy jest numer magiczny
           fprintf(stderr,"Blad: To nie jest plik PGM\n");
           return(0);}

           // Pominiecie komentarzy
           do {
               if ((znak=fgetc(plik_we))=='#') {    // Czy linia rozpoczyna sie od znaku '#'
                   if (fgets(buf,DL_LINII,plik_we)==NULL)  // Przeczytaj ja do bufora                
	                     koniec=1;                   // Zapamietaj ewentualny koniec danych
                       } else {
                       ungetc(znak,plik_we);}                   // Gdy przeczytany znak z poczatku linii nie jest '#' zwroc go
              } while (znak=='#' && !koniec);   // Powtarzaj dopoki sa linie komentarza i nie nastapil koniec danych

           // Pobranie wymiarow obrazu i liczby odcieni szarosci
           if (fscanf(plik_we,"%d %d %d",wymx,wymy,szarosci)!=3) {
               fprintf(stderr,"Blad: Brak wymiarow obrazu lub liczby stopni szarosci\n");
               return(0);}

           // Pobranie obrazu i zapisanie w tablicy obraz_pgm*
           for (i=0;i<*wymy;i++) {
                for (j=0;j<*wymx;j++) {
                     if (fscanf(plik_we,"%d",&(obraz_pgm[i][j]))!=1) {
	                       fprintf(stderr,"Blad: Niewlasciwe wymiary obrazu\n");
	                       return(0);
                                 }    }                              }
           return *wymx**wymy;}   // Czytanie zakonczone sukcesem                 

void wyswietl (char *n_pliku) {
              char polecenie[DL_LINII];      // Bufor pomocniczy do zestawienia polecenia
              strcpy(polecenie,"display ");  // Konstrukcja polecenia postaci
              strcat(polecenie,n_pliku);     // display "nazwa_pliku" &
              strcat(polecenie," &");
              printf("%s\n",polecenie);      // Wydruk kontrolny polecenia
              system(polecenie);             // Wykonanie polecenia
                              }

void zapisz (FILE *plik_wy,int obraz_pgm[][MAX],int *wymx,int *wymy,int *szarosci) {
           int koniec=0;            // Czy napotkano koniec danych w pliku
           int i,j;

           // Sprawdzenie czy podano prawidlowy uchwyt pliku
           if (plik_wy==NULL) {
              fprintf(stderr,"Blad: Nie podano uchwytu do pliku\n");
              return;}

           // Wpisanie "numeru magicznego"
           fputs("P2\n", plik_wy);

           // Wpisanie wymiaru
           fprintf(plik_wy,"%d %d %d\n",*wymx,*wymy,*szarosci);

           // Zapis obrazu
           for (i=0;i<*wymy;i++) {
               for (j=0;j<*wymx;j++) {
                   if (j < *wymx)
                      {fprintf(plik_wy,"%d ",obraz_pgm[i][j]);
                      } else {
                      fprintf(plik_wy,"%d\n",obraz_pgm[i][j]);}
                                  }   }                                           }

void negatyw (int obraz_pgm[][MAX],int *wymx,int *wymy,int *szarosci) {
             int i,j;
             for (i=0;i<*wymy;i++) {
                 for (j=0;j<*wymx;j++) {
                     obraz_pgm[i][j]=*szarosci-obraz_pgm[i][j];
                                   }   }                              }

void progowanie (int obraz_pgm[][MAX],int *wymx,int *wymy,int *szarosci,int *prog){
                int i,j;
                for (i=0;i<*wymy;i++) {
                    for (j=0;j<*wymx;j++) {
                        if (obraz_pgm[i][j]<=*prog)
                            {obraz_pgm[i][j]=0;
                            } else {
                            obraz_pgm[i][j]=*szarosci;}
                                      }   }                                       }

void konturowanie (int obraz_pgm[][MAX],int *wymx,int *wymy,int *szarosci) {    //Funkcja konturowania zrealizowana poprzez zmniejszenie obrazu
                  int i,j;
                  for (i=0;i<*wymy-1;i++) {
                      for (j=0;j<*wymx-1;j++) {
                          obraz_pgm[i][j]=abs(obraz_pgm[i+1][j]-obraz_pgm[i][j])+abs(obraz_pgm[i][j+1]-obraz_pgm[i][j]);
                                          }   }                            }

void rozmycie (int obraz_pgm[][MAX],int *wymx,int *wymy,int *szarosci) {    //Funkcja rozmycia poziomego zrealizowana poprzez zmniejszenie obrazu
              int i,j;
              for (i=0;i<*wymy-1;i++) {
                  for (j=1;j<*wymx-1;j++) {
                      obraz_pgm[i][j]=(obraz_pgm[i][j-1]+obraz_pgm[i][j]+obraz_pgm[i][j+1])/3;
                                      }   }                            }

int main(){

FILE *plik;
char nazwa[100];
int wybor;
int obraz[MAX][MAX];
int wymx,wymy,odcieni;
int odczytano = 0;
int prog;

do {   
    printf("Wybierz opcje:\n");
    printf("1. Wczytaj obraz\n2. Negatywuj\n3. Proguj\n4. Konturuj\n5. Rozmyj poziomo\n6. Zapisz obraz\n7. Wyswietl obraz\n8. Zakoncz dzialanie programu\n");
    printf("Twoj wybor:\n");
    scanf("%d",&wybor);
    switch (wybor) {
    case 1:
        printf("Podaj nazwe pliku do odczytu:\n");
        scanf("%s",nazwa);
        plik=fopen(nazwa,"r");
        if (plik != NULL) {
            wczytaj(plik,obraz,&wymx,&wymy,&odcieni);
            fclose(plik);}
        break;
    case 2:
        negatyw(obraz,&wymx, &wymy, &odcieni);
        break;
    case 3:
        printf("Podaj próg (nie większy niz: %d):\n",odcieni);
        scanf("%d",&prog);
        progowanie(obraz,&wymx,&wymy,&odcieni,&prog);
        break;
    case 4:
        konturowanie(obraz,&wymx,&wymy,&odcieni);
        break;
    case 5:
        rozmycie(obraz,&wymx,&wymy,&odcieni);
        break;
    case 6:
        printf("Podaj nazwe pliku do zapisu:\n");
        scanf("%s",nazwa);
        plik=fopen(nazwa,"w");
        if (plik != NULL) {
            zapisz(plik,obraz,&wymx,&wymy,&odcieni);
            fclose(plik);}
        break;
    case 7:
        printf("Podaj nazwe pliku: \n");
        scanf("%s", nazwa);
        wyswietl(nazwa);
        break;
    case 8:
        break;
    default:
        printf("Wybrano niedostepna opcje\n");
        break;}
  } while (wybor != 8);
        }