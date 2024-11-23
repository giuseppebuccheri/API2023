#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX 1000

struct stazione{
    int chilometro;
    struct automobile* automobili;
    int n_auto;
    int max_auto;
    int min_raggiungibile;
    struct stazione* next;
    struct stazione* min_prec;
    struct stazione* precedente;
    struct stazione* prossima;
    struct stazione* prev;
};

struct automobile {
    int autonomia;
    struct automobile *next;
};

struct autostrada{
    struct stazione* table[MAX];
};

int n_stazioni;
struct stazione *tail;

unsigned int genera_indice(int chilometro) {
    return chilometro % MAX;
}

void carica_hash(struct autostrada* autostrada,struct stazione** stazione,int distanza) {

    unsigned int indice = genera_indice(distanza);

    struct stazione *curr = *stazione;

    if (autostrada->table[indice] == NULL) {
        curr->prossima = NULL;
        autostrada->table[indice] = curr;
    }
    else{
        autostrada->table[indice]->precedente = curr;
        curr->prossima =autostrada->table[indice];
        autostrada->table[indice] = curr;
    }

    curr->precedente = NULL;
}

struct stazione* search(struct autostrada* autostrada, int distanza) {
    unsigned index = genera_indice(distanza);

    struct stazione* scorr = autostrada->table[index];
    while (scorr != NULL) {
        if (scorr->chilometro == distanza) {
            return scorr;
        }
        scorr = scorr->prossima;
    }
    return NULL;
}

int calcolo_max_auto(struct automobile** head){
    struct automobile* curr = *head;
    int max = 0;

    while (curr != NULL) {
        if(curr->autonomia > max)
            max = curr->autonomia;
        curr = curr -> next;
    }
    return max;
}

void insierisci_staz(struct autostrada** autostrada,struct stazione** head,int distanza, int n_auto, int *automobili){


    struct stazione* verifica = *head;
    //precedente dell'ipotetica stazione da inserire
    struct stazione* curr = *head;

    while(verifica != NULL && verifica->chilometro <= distanza){
        if (verifica->chilometro == distanza) {
            printf("non aggiunta\n");
            return;
        }

        if(verifica->chilometro < distanza)
            curr = verifica;

        verifica = verifica->next;
    }

    //Alloca un nodo
    struct stazione* nuova_stazione = malloc(sizeof (struct stazione));

    //Carica dati da input
    nuova_stazione->chilometro = distanza;
    nuova_stazione->n_auto = n_auto;
    //valori default
    nuova_stazione->max_auto = 0;
    nuova_stazione->min_raggiungibile = -1;
    nuova_stazione->min_prec = NULL;
    nuova_stazione->automobili = NULL;
    nuova_stazione->next = NULL;
    nuova_stazione->prev = NULL;
    nuova_stazione->prossima = NULL;
    nuova_stazione->precedente = NULL;

    //Se c'è un parco auto lo inserisco altrimeni rimane a null
    if(nuova_stazione->n_auto != 0){
        for (int i = n_auto-1 ; i >= 0 ; i--) {
            struct automobile* nuova_auto = malloc(sizeof (struct automobile));
            nuova_auto->autonomia = automobili[i];
            if(nuova_auto->autonomia > nuova_stazione->max_auto)
                nuova_stazione->max_auto = nuova_auto->autonomia;
            nuova_auto->next = nuova_stazione->automobili;
            nuova_stazione->automobili = nuova_auto;
        }
    }
    else
        nuova_stazione->automobili = NULL;

    carica_hash(*autostrada,&nuova_stazione,distanza);

    //Se la testa è vuota oppure quella nuova è più vicina della testa inserisco in testa
    if(*head == NULL || (*head)->chilometro > nuova_stazione->chilometro){
        nuova_stazione->next = *head;
        nuova_stazione->prev = NULL;
        if (*head != NULL)
            (*head)->prev = nuova_stazione;
        if(*head == NULL)
            tail = nuova_stazione;
        *head = nuova_stazione;
        printf("aggiunta\n");
        n_stazioni++;
        return;
    }

    //Altrimenti scorro finchè non trovo quella prima della nuova
    if(*head != NULL) {
        //Se esiste già una stazione non aggiungo
        if(curr->chilometro == nuova_stazione->chilometro){
            printf("non aggiunta\n");
            return;
        }
        else{
            //Se è l'ultima inserisco in coda
            if (curr->next == NULL) {
                curr->next = nuova_stazione;
                nuova_stazione->next = NULL;
                nuova_stazione->prev = curr;
                printf("aggiunta\n");
                n_stazioni++;
                tail = nuova_stazione;
                return;
            }
            //Inserisco in mezzo
            if(nuova_stazione->chilometro < curr->next->chilometro) {
                nuova_stazione->next = curr->next;
                nuova_stazione->prev = curr;
                curr->next->prev=nuova_stazione;
                curr->next=nuova_stazione;
                printf("aggiunta\n");
                n_stazioni++;
                return;
            }
        }
    }
    printf("non aggiunta\n");
}

void rimuovi (struct autostrada* autostrada,struct stazione* stazione,int distanza) {

    unsigned int indice = genera_indice(distanza);

    //rimuovi dopo la testa
    if(stazione->precedente != NULL){
        stazione->precedente->prossima = stazione->prossima;
        //rimuovi in mezzo
        if(stazione->prossima != NULL)
            stazione->prossima->precedente = stazione->precedente;
    }
    //rimuovi in testa
    else{
        autostrada->table[indice] = stazione->prossima;
    }

    stazione->precedente = NULL;
}

void cancella_staz(struct autostrada** autostrada , struct stazione** head, int distanza){
    struct stazione* curr = search(*autostrada,distanza);

    if(curr == NULL){
        printf("non demolita\n");
        return;
    }

    if(curr != NULL && curr->chilometro == distanza){
        while (curr->automobili != NULL) {
            struct automobile* temp = curr->automobili;
            curr->automobili = curr->automobili->next;
            free(temp);
        }

        //elimino l'unico elemento
        if(curr == *head && curr->next == NULL){
            rimuovi(*autostrada,curr,distanza);
            free(curr);
            *head=NULL;
            tail = NULL;
            printf("demolita\n");
            n_stazioni = 0;
            return;
        }

        //elimino in testa
        if(curr == *head && curr->next != NULL){
            rimuovi(*autostrada,curr,distanza);
            *head = (*head)->next;
            curr->prev = NULL;
            free(curr);
            printf("demolita\n");
            n_stazioni--;
            return;
        }

        //elimino al centro
        if(curr->prev != NULL){
            curr->prev->next = curr->next;
            //Se non elimino in coda
            if(curr->next != NULL)
                curr->next->prev = curr->prev;
            else
                tail = curr->prev;
            rimuovi(*autostrada,curr,distanza);
            free(curr);
            printf("demolita\n");
            n_stazioni--;
            return;
        }
    }
}

void aggiungi_auto(struct autostrada** autostrada,int distanza, int autonomia) {

    struct stazione* stazione = search(*autostrada,distanza);

    if(stazione == NULL || stazione->chilometro != distanza){
        printf("non aggiunta\n");
        return;
    }

    struct automobile *nuova_auto = malloc(sizeof(struct automobile));
    nuova_auto->autonomia = autonomia;
    stazione->n_auto++;
    if (stazione->automobili == NULL)
        nuova_auto->next = NULL;
    else
        nuova_auto->next = stazione->automobili;
    stazione->automobili = nuova_auto;

    if(nuova_auto->autonomia > stazione->max_auto)
        stazione->max_auto = nuova_auto->autonomia;
    printf("aggiunta\n");
}

void rottama_auto(struct autostrada** autostrada, int distanza, int autonomia) {
    struct stazione* stazione = search(*autostrada,distanza);

    if(stazione == NULL || stazione->chilometro != distanza || stazione->n_auto == 0){
        printf("non rottamata\n");
        return;
    }

    struct automobile* testa = stazione->automobili;
    if(testa->autonomia == autonomia){
        stazione->automobili = stazione->automobili->next;
        stazione->max_auto = calcolo_max_auto(&(stazione->automobili));
        free(testa);
        printf("rottamata\n");
        return;
    }
    else{
        struct automobile* scorr = testa->next;
        struct automobile* prev = testa;
        while (scorr!=NULL){
            if(scorr->autonomia == autonomia){
                if(scorr->next == NULL){
                    prev->next = NULL;
                    free(scorr);
                    stazione->n_auto --;
                    stazione->max_auto = calcolo_max_auto(&(stazione->automobili));
                    printf("rottamata\n");
                    return;
                }
                else{
                    prev->next = scorr->next;
                    free(scorr);
                    stazione->n_auto --;
                    stazione->max_auto = calcolo_max_auto(&(stazione->automobili));
                    printf("rottamata\n");
                    return;
                }
            }
            prev=scorr;
            scorr=scorr->next;
        }
    }
    printf("non rottamata\n");
}

void pianifica(struct autostrada** autostrada, struct stazione** head, int partenza, int destinazione){

    //il numero massimo di hop di un percorso è il numero totale di stazioni
    int percorso[n_stazioni];
    int hop = 0;
    //struct stazione* cambiato = NULL;

    struct stazione* curr = *head;
    struct stazione* scorr;

    struct stazione* pointer_partenza = search(*autostrada,partenza);
    if(pointer_partenza == NULL){
        printf("nessun percorso\n");
        return;
    }

    struct stazione* pointer_destinazione = search(*autostrada,destinazione);
    if(pointer_destinazione == NULL){
        printf("nessun percorso\n");
        return;
    }

    if(partenza < destinazione) {

        curr = pointer_destinazione;

        while (curr != NULL && curr->chilometro >= partenza){
            //se curr è la testa il min_raggiungibile rimane -1
            if(curr->prev != NULL){
                scorr = curr->prev;
                while (scorr != NULL && scorr->chilometro >= partenza) {
                    if(scorr->chilometro + scorr->max_auto >= curr->chilometro)
                        curr->min_prec = scorr; //salvo in min_raggiungibile di ogni stazione la stazione piu piccola che la raggiunge!
                    scorr = scorr->prev;
                }
            }
            curr = curr->prev;
        }

        curr = pointer_destinazione;
        struct stazione* next_hop = NULL;

        //Calcolo percorso
        while(curr!= NULL && curr->chilometro > partenza){
            if(curr->prev != NULL){
                next_hop = NULL;

                if(curr->min_prec != NULL){
                    //Selgo il più piccolo che arriva a curr se non è immediato
                    if(curr->min_prec->chilometro > partenza){
                        next_hop = curr->min_prec;
                    }
                    else{
                        next_hop = pointer_partenza;
                    }
                }

                if(next_hop == NULL){
                    printf("nessun percorso\n");
                    return;
                }

                //Scrivo il percorso al contrario nel vettore
                percorso[hop] = curr->chilometro;
                hop++;
                curr = next_hop;
                if(curr == pointer_partenza)
                    percorso[hop] = curr->chilometro;
            }
        }

        for (int i = hop; i >= 0 ; i--) {
            if(i>0)
                printf("%d ",percorso[i]);
            if(i==0)
                printf("%d\n",percorso[i]);
        }
    }

    if (partenza > destinazione) {
        curr = pointer_partenza;

        while (curr != NULL && curr != pointer_destinazione) {
            //se curr è la testa il min_raggiungibile rimane -1
            if(curr->prev != NULL){
                if(curr->max_auto > 0){
                    scorr = curr->prev;

                    while (scorr != NULL && curr->chilometro - curr->max_auto <= scorr->chilometro) {
                        curr->min_raggiungibile = scorr->chilometro;
                        scorr = scorr->prev;
                    }
                }
                else
                    curr->min_raggiungibile = -1;
            }
            curr = curr->prev;
        }

        curr = pointer_partenza;
        scorr = NULL;
        struct stazione* prec_hop = NULL;
        struct stazione* alternativa = NULL;
        struct stazione* cambiato = NULL;
        struct stazione* prec_cambiato = NULL;

        //calcolo percorso
        while(curr != NULL && curr->chilometro > destinazione){
            if(curr->prev != NULL){
                if(curr->prev->chilometro < curr->min_raggiungibile || curr->min_raggiungibile == -1){
                    printf("nessun percorso\n");
                    return;
                }

                struct stazione* next_hop = curr->prev;

                scorr = curr->prev;

                //Controllo se il percorso è immediato
                if (scorr != NULL && scorr->chilometro == destinazione){
                    if(curr->min_raggiungibile <= destinazione)
                        next_hop = scorr;
                    else{
                        printf("nessun percorso\n");
                        return;
                    }
                }

                //Controllo tutti i nodi che arrivano a current e scelgo quello che va più indietro di tutti (a parità quello minore)
                while (scorr != NULL && scorr->chilometro >= destinazione && scorr->chilometro >= curr->min_raggiungibile){
                    if(scorr->chilometro == destinazione){
                        next_hop = scorr;
                        break;
                    }

                    if(
                        (scorr->min_raggiungibile <= next_hop->min_raggiungibile && scorr->min_raggiungibile != -1) ||
                        (scorr->min_raggiungibile > next_hop->min_raggiungibile && next_hop->min_raggiungibile <= destinazione && scorr->min_raggiungibile <= destinazione)
                    )
                        next_hop = scorr;

                    scorr = scorr->prev;
                }

                if(next_hop == NULL){
                    printf("nessun percorso\n");
                    return;
                }

                int max=0;

                if(cambiato == NULL && prec_hop != NULL)
                    max = prec_hop->min_raggiungibile;
                if(cambiato != NULL && prec_hop != NULL && prec_hop->min_raggiungibile < cambiato->min_raggiungibile)
                    max = prec_hop->min_raggiungibile;
                if(cambiato != NULL && prec_hop != NULL && prec_hop->min_raggiungibile >= cambiato->min_raggiungibile){
                    max = cambiato->min_raggiungibile;
                }

                alternativa = curr->prev;
                while (alternativa != NULL && prec_hop != NULL && alternativa->chilometro >= max){
                    if(alternativa->chilometro < curr->chilometro && alternativa->min_raggiungibile <= next_hop->chilometro ){
                        if(prec_hop->min_raggiungibile <= alternativa->chilometro){
                            cambiato = curr;
                            curr = alternativa;
                        }
                        else if(cambiato != NULL && cambiato->min_raggiungibile <= alternativa->chilometro){
                            if(prec_cambiato != NULL && prec_cambiato->min_raggiungibile <= cambiato->chilometro && prec_cambiato->chilometro < percorso[hop-2]){
                                percorso[hop-2] = prec_cambiato->chilometro;
                            }
                            prec_cambiato = prec_hop;
                            prec_hop = cambiato;
                            percorso[hop-1] = prec_hop->chilometro;
                            cambiato = curr;
                            curr = alternativa;
                        }
                    }
                    alternativa = alternativa->prev;
                }

                //Scrivo il percorso nel vettore//Scrivo il percorso nel vettore
                percorso[hop] = curr->chilometro;
                hop++;
                prec_hop = curr;
                curr = next_hop;

                if(curr->chilometro == destinazione)
                    percorso[hop] = curr->chilometro;
                }
        }

        for (int i = 0; i <= hop ; i++) {
            if(i<hop)
                printf("%d ",percorso[i]);
            if(i==hop)
                printf("%d\n",percorso[i]);
        }
    }

    //boh
    if (partenza == destinazione){
        printf("%d\n",partenza);
    }
}

int main() {
    n_stazioni = 0;

    struct stazione* head = NULL;
    tail = NULL;

    struct autostrada* tangenziale = malloc(sizeof (struct autostrada));
    for (int i = 0; i < MAX; i++) {
        tangenziale->table[i] = NULL;
    }

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
        if (c != ' ' && c != '\n') {
            comando[dim++] = c;
        } else {
            comando[dim] = '\0';  // Termina la stringa
            dim = 0;  // Resetta l'indice

            if (strcmp(comando, "aggiungi-stazione") == 0) {
                if (fscanf(stdin, "%d %d", &distanza, &n_auto) != EOF) {
                    for (int i = 0; i < n_auto ; i++) {
                        if (fscanf(stdin, "%d", &automobili[i]) != EOF) {
                        }
                    }
                    insierisci_staz(&tangenziale, &head, distanza, n_auto, automobili);
                }
            } else if (strcmp(comando, "demolisci-stazione") == 0) {
                if (fscanf(stdin, "%d", &distanza) != EOF) {
                    cancella_staz(&tangenziale, &head,distanza);
                }
            } else if (strcmp(comando, "aggiungi-auto") == 0) {
                if (fscanf(stdin, "%d %d", &distanza, &autonomia) != EOF) {
                    aggiungi_auto(&tangenziale,distanza, autonomia);
                }
            } else if (strcmp(comando, "rottama-auto") == 0) {
                if (fscanf(stdin, "%d %d", &distanza, &autonomia) != EOF) {
                    rottama_auto(&tangenziale, distanza, autonomia);
                }
            } else if (strcmp(comando, "pianifica-percorso") == 0) {
                if (fscanf(stdin, "%d %d", &partenza, &destinazione) != EOF) {
                    pianifica(&tangenziale, &head, partenza, destinazione);
                }
            }
        }
    }

    return 0;
}
