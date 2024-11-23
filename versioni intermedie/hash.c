#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_AUTO 512

struct stazione{
    int chilometro;
    struct parco* automobili;
    int n_auto;
    int max_auto;
    int min_raggiungibile;
    struct stazione* next;
    struct stazione* min_prec;
    struct stazione* prev;

};

struct automobile {
    int autonomia;
    struct automobile *next;
    struct automobile *prev;
};

struct parco {
    struct automobile* table[MAX_AUTO];
};

int n_stazioni;
struct stazione *tail;

void inizializza_tabella(struct parco* ht) {
    for (int i = 0; i < MAX_AUTO; i++) {
        ht->table[i] = NULL;
    }
}

unsigned int funzione_hash(int key) {
    return key % MAX_AUTO;
}

void insert(struct parco* ht, int autonomia) {
    unsigned index = funzione_hash(autonomia);

    struct automobile* nuova_auto = malloc(sizeof(struct automobile));
    nuova_auto->autonomia = autonomia;

    if(ht->table[index] == NULL){
        nuova_auto->next = NULL;
        ht->table[index] = nuova_auto;
    }
    else{
        ht->table[index]->prev = nuova_auto;
        nuova_auto->next = ht->table[index];
        ht->table[index] = nuova_auto;
    }

    nuova_auto->prev = NULL;
}

struct automobile* search(struct parco* ht, int key) {
    unsigned index = funzione_hash(key);

    struct automobile* curr = ht->table[index];
    while (curr != NULL) {
        if (curr->autonomia == key) {
            return curr;
        }
        curr = curr->next;
    }

    return NULL;
}

struct stazione* getpointer(struct stazione** head, int chilometro){

    struct stazione* pointer = NULL;
    struct stazione *curr = tail;
    //Salvo i pointer dei nodi interessati
    while (curr != NULL) {
        if (curr->chilometro == chilometro){
            pointer = curr;
            return  pointer;
        }
        curr = curr->prev;
    }

    return NULL;
}

void insierisci_staz(struct stazione** head,int distanza, int n_auto, int *automobili){

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

    //alloco e collego il parco auto
    struct parco* nuovo_parco = malloc(sizeof (struct parco));
    inizializza_tabella(nuovo_parco);
    nuova_stazione->automobili = nuovo_parco;

    //Se c'è un parco auto lo inserisco altrimeni rimane a null
    if(nuova_stazione->n_auto != 0){
        for (int i = 0 ; i < n_auto ; i++) {
            insert(nuovo_parco,automobili[i]);
            if(automobili[i] > nuova_stazione->max_auto)
                nuova_stazione->max_auto = automobili[i];
        }
    }
    else
        nuova_stazione->automobili = NULL;

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
    printf("non aggiunta");
}

void cancella_staz(struct stazione** head,int distanza){
    struct stazione* curr = *head;

    if(curr->chilometro == distanza){
        //Se devo eliminare l'unica stazione
        if(curr->next == NULL){
            *head=NULL;
            tail = NULL;
            printf("demolita\n");
            n_stazioni = 0;
            return;
        }
        else{//elimino in testa

            //elimino auto
            for(int i=0;i < curr->n_auto; i++){
                struct automobile* macchina = curr->automobili->table[i];
                if (macchina != NULL)
                    free(macchina);
            }
            free(curr->automobili);

            *head = (*head)->next;
            (*head)->prev = NULL;
            free(curr);
            printf("demolita\n");
            n_stazioni--;
            return;
        }
    }

    //Se devo eliminare una stazione al centro
    while (curr != NULL){
        if(curr->chilometro == distanza){
            //elimino auto
            for(int i=0;i < curr->n_auto; i++){
                struct automobile* macchina = curr->automobili->table[i];
                if (macchina != NULL)
                    free(macchina);
            }
            free(curr->automobili);

            curr->prev->next = curr->next;
            //Se non elimino in coda
            if(curr->next != NULL)
                curr->next->prev = curr->prev;
            else
                tail = curr->prev;
            free(curr);
            printf("demolita\n");
            n_stazioni--;
            return;
        }
        curr = curr->next;
    }

    printf("non demolita\n");
}

void aggiungi_auto(struct stazione** head, int distanza, int autonomia) {

    struct stazione *stazione = getpointer(head, distanza);
    if(stazione == NULL){
        printf("non aggiunta\n");
        return;
    }

    if(stazione->automobili != NULL){
        insert(stazione->automobili,autonomia);
        stazione->n_auto++;
    }
    else{
        struct parco* nuovo_parco = malloc(sizeof (struct parco));
        inizializza_tabella(nuovo_parco);
        stazione->automobili = nuovo_parco;
        insert(nuovo_parco,autonomia);
        stazione->n_auto++;
    }

    if(autonomia > stazione->max_auto)
        stazione->max_auto = autonomia;

    printf("aggiunta\n");
}


int calcolo_max_auto(struct parco* parco_auto) {
    int max = 0;

    for (unsigned int i = 0; i < MAX_AUTO; i++) {
        struct automobile* curr = parco_auto->table[i];
        while (curr != NULL) {
            if (curr->autonomia > max) {
                max = curr->autonomia;
            }
            curr = curr->next;
        }
    }

    return max;
}

void rottama_auto(struct stazione** head, int distanza, int autonomia){
   
    unsigned int index = funzione_hash(autonomia);
    struct automobile* curr_auto = NULL;
    curr_auto = search(stazione->automobili, autonomia);
    if(curr_auto == NULL){
        printf("non rottamata\n");
        return;
    }

    if(curr_auto->autonomia == autonomia){
        if (curr_auto->prev != NULL) {
            curr_auto->prev->next = curr_auto->next;
            if(curr_auto->next != NULL)
                curr_auto->next->prev = curr_auto->prev;
        } else {
            stazione->automobili->table[index] = curr_auto->next;
            if(stazione->automobili->table[index] != NULL)
                stazione->automobili->table[index]->prev = curr_auto->prev;
        }
        free(curr_auto);
        stazione->n_auto --;
        if(autonomia == stazione->max_auto)
            stazione->max_auto = calcolo_max_auto(stazione->automobili);
        printf("rottamata\n");
    }
    else
        printf("non rottamata\n");
}

void pianifica(struct stazione** head, int partenza, int destinazione){

    //il numero massimo di hop di un percorso è il numero totale di stazioni
    int percorso[n_stazioni];
    int hop = 0;
    //struct stazione* cambiato = NULL;

    struct stazione* curr = *head;
    struct stazione* scorr;

    struct stazione* pointer_partenza = getpointer(head,partenza);
    struct stazione* pointer_destinazione = getpointer(head,destinazione);

    if(partenza < destinazione) {

        curr = pointer_destinazione;

        int autonomia;

        while (curr != NULL && curr->chilometro >= partenza) {
            //se curr è la testa il min_raggiungibile rimane -1
            if(curr->prev != NULL){
                scorr = curr->prev;
                while (scorr != NULL && scorr->chilometro >= partenza) {
                    autonomia = scorr->chilometro + scorr->max_auto;
                    if(autonomia >= curr->chilometro)
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

        int autonomia;

        while (curr != NULL && curr != pointer_destinazione) {
            //se curr è la testa il min_raggiungibile rimane -1
            if(curr->prev != NULL){
                if(curr->max_auto > 0){
                    autonomia = curr->chilometro - curr->max_auto;
                    scorr = curr->prev;

                    while (scorr != NULL && autonomia <= scorr->chilometro) {
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
                    insierisci_staz(&head, distanza, n_auto, automobili);
                }
            } else if (strcmp(comando, "demolisci-stazione") == 0) {
                if (fscanf(stdin, "%d", &distanza) != EOF) {
                    cancella_staz(&head, distanza);
                }
            } else if (strcmp(comando, "aggiungi-auto") == 0) {
                if (fscanf(stdin, "%d %d", &distanza, &autonomia) != EOF) {
                    aggiungi_auto(&head, distanza, autonomia);
                }
            } else if (strcmp(comando, "rottama-auto") == 0) {
                if (fscanf(stdin, "%d %d", &distanza, &autonomia) != EOF) {
                    rottama_auto(&head, distanza, autonomia);
                }
            } else if (strcmp(comando, "pianifica-percorso") == 0) {
                if (fscanf(stdin, "%d %d", &partenza, &destinazione) != EOF) {
                    pianifica(&head, partenza, destinazione);
                }
            }
        }
    }

    return 0;
}
