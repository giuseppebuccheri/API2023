#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char comando[25];

    int distanza;
    int autonomia;
    int n_auto;
    int automobili[512];

    int partenza;
    int destinazione;

    char c;
    int dim = 0;

    while ((c = getc_unlocked(stdin)) != EOF) {
        if (c != ' ' && c != '\n')
            comando[dim] = c;

        if (c == ' ')
            comando[dim] = '\0';

        if (strcmp(comando, "aggiungi-stazione")==0) {
            dim = 0;

            if (fscanf(stdin, "%d %d",&distanza,&n_auto)!= EOF) {
                for (int i = 0; i < n_auto ; i++) {
                    if (fscanf(stdin, "%d",&automobili[i])!= EOF){
                    }
                    insierisci_staz(&head,distanza, n_auto, automobili);
                }
            }
        }

        if (strcmp(comando, "demolisci-stazione")==0) {
            dim = 0;

            if (fscanf(stdin, "%d",&distanza)!= EOF) {
                cancella_staz(&head,distanza);
            }
        }

        if (strcmp(comando, "aggiungi-auto")==0) {
            dim = 0;

            if (fscanf(stdin, "%d %d",&distanza,&autonomia)!= EOF) {
                printf("%s %d %d",comando,distanza,autonomia);
            }
        }

        if (strcmp(comando, "rottama-auto")==0) {
            dim = 0;

            if (fscanf(stdin, "%d %d",&distanza,&autonomia)!= EOF) {
                printf("%s %d %d",comando,distanza,autonomia);
            }
        }

        if (strcmp(comando, "pianifica-percorso")==0) {
            dim = 0;

            if (fscanf(stdin, "%d %d",&partenza,&destinazione)!= EOF) {
                printf("%s %d %d",comando,partenza,destinazione);
            }
        }

        dim++;
    }

    return 0;
}