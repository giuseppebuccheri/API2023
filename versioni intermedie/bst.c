#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 512

typedef struct stazione{
    int chilometro;
    int automobili[MAX];
    int max_auto;
    struct stazione* left;
    struct stazione* right;
    struct stazione* parent;
} stazione;

int n_stazioni;


//Funzioni di appoggio
stazione* crea_nodo(int chilometro){
    stazione *allocato = malloc(sizeof (stazione));
    if(allocato != NULL){
        allocato->left = NULL;
        allocato->right = NULL;
        allocato->max_auto = 0;
        memset(allocato->automobili, -1, sizeof(int) * 512);
        allocato->chilometro = chilometro;
    }
    return allocato;
}

stazione* trova_stazione(stazione* curr, int chilometro){

    while (curr != NULL && curr->chilometro != chilometro){
        if(chilometro < curr->chilometro)
            curr = curr->left;
        else
            curr = curr->right;
    }

    return curr;
}

void transplant(stazione** root, stazione* u, stazione* v) {
    if (u->parent == NULL) {
        *root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }

    if (v != NULL) {
        v->parent = u->parent;
    }
}

void stampa_auto(struct stazione* root)
{
    printf("-->");

    for (int i = 0; root->automobili[i] != -1; ++i) {
        printf(" %d ",root->automobili[i]);
    }
    printf("\tmax: %d", root->max_auto);
    printf("\n");
}

void inorder(struct stazione* root)
{
    if(root != NULL)
    {
        inorder(root->left);
        printf("%d ", root->chilometro);
        //stampa_auto(root);
        inorder(root->right);
    }
}

stazione* minimo(stazione* curr){
    while (curr != NULL){
        curr = curr->left;
    }
    return curr;
}

stazione* massimo(stazione* curr){

    while (curr != NULL){
        curr = curr->right;
    }

    return curr;
}

stazione* successore(stazione* nodo){

    if(nodo->right != NULL)
        return minimo(nodo->right);

    stazione* prev = nodo->parent;

    while (prev != NULL && nodo == prev->right){
        nodo = prev;
        prev = prev->parent;
    }

    return prev;
}

stazione* predecessore(stazione* nodo) {
    if (nodo->left != NULL) {
        stazione* curr = nodo->left;
        while (curr != NULL){
            if(curr->right == NULL){
                return curr;
            }
            curr = curr->right;
        }
        return curr;
    }

    stazione* prev = nodo->parent;

    while (prev != NULL && nodo == prev->left) {
        nodo = prev;
        prev = prev->parent;
    }

    return prev;
}

void max_auto(stazione* nodo){
    int i = 0;

    nodo->max_auto = 0;

    while (nodo->automobili[i] != -1){
        if(nodo->automobili[i] > nodo->max_auto)
            nodo->max_auto = nodo->automobili[i];
        i++;
    }
}

//Funzioni principali
stazione* insierisci_staz(stazione* root, int chilometro, int* automobili, int n_auto){

    stazione* nuova_stazione = crea_nodo(chilometro);
    if(nuova_stazione == NULL){
        printf("non aggiunta\n");
        return root;
    }

    if(root == NULL){
        printf("aggiunta\n");
        return nuova_stazione;
    }

    stazione* prev = NULL;
    stazione* curr = root;

    while (curr != NULL){
        prev = curr;
        if(nuova_stazione->chilometro == curr->chilometro){
            printf("non aggiunta\n");
            free(nuova_stazione); // Libera la memoria allocata
            return root;
        }
        if(nuova_stazione->chilometro < curr->chilometro)
            curr = curr->left;
        else if(nuova_stazione->chilometro > curr->chilometro)
            curr = curr->right;
        nuova_stazione->parent = prev;
    }

    // Aggiungi il nuovo nodo come figlio di prev
    if (nuova_stazione->chilometro < prev->chilometro)
        prev->left = nuova_stazione;
    else
        prev->right = nuova_stazione;

    for (int i = n_auto-1 ; i >= 0 ; i--) {
        nuova_stazione->automobili[i] = automobili[i];
        if(automobili[i] > nuova_stazione->max_auto)
            nuova_stazione->max_auto = automobili[i];
    }

    n_stazioni++;
    printf("aggiunta\n");
    return root;
}

stazione* cancella_staz(stazione* root, int chilometro){

    stazione* da_eliminare = trova_stazione(root, chilometro);

    if(da_eliminare == NULL){
        printf("non demolita\n");
        return root;
    }
    else{
        if(da_eliminare->left == NULL)
            transplant(&root, da_eliminare, da_eliminare->right);
        else if(da_eliminare->right == NULL)
            transplant(&root, da_eliminare, da_eliminare->left);
        else{
            stazione* y = minimo(da_eliminare->right);
            if(y->parent != NULL){
                transplant(&root,y,y->right);
                y->right = da_eliminare->right;
                y->right->parent = y;
            }

            transplant(&root, da_eliminare, y);
            y->left = da_eliminare->left;
            y->left->parent = y;
        }
    }

    n_stazioni--;
    free(da_eliminare);
    printf("demolita\n");
    return root;
}

void aggiungi_auto(stazione* root, int chilometro, int autonomia){

    stazione* target = trova_stazione(root,chilometro);

    if(target == NULL){
        printf("non aggiunta\n");
        return;
    }

    int i = 0;

    while (target->automobili[i] != -1){
        i++;
    }

    target->automobili[i] = autonomia;
    if(target->max_auto < autonomia)
        target->max_auto = autonomia;

    printf("aggiunta\n");
}

void rottama_auto(stazione* root, int chilometro, int autonomia){
    stazione* target = trova_stazione(root,chilometro);

    if(target == NULL){
        printf("non rottamata\n");
        return;
    }

    int i = 0;

    while (target->automobili[i] != -1){
       if(target->automobili[i] == autonomia){
           for (int j = i; target->automobili[j] != -1; ++j) {
               target->automobili[j] = target->automobili[j+1];
           }
           max_auto(target);
           printf("rottamata\n");
           return;
       }
        i++;
    }

    printf("non rottamata\n");
}

void pianifica(stazione* root, int partenza, int destinazione){

    //il numero massimo di hop di un percorso è il numero totale di stazioni
    int percorso[n_stazioni];
    memset(percorso, -1, sizeof(percorso));
    int hop = 0;

    struct stazione* pointer_partenza = trova_stazione(root,partenza);
    struct stazione* pointer_destinazione = trova_stazione(root,destinazione);

    if(pointer_partenza == NULL || pointer_destinazione == NULL){
        printf("nessun percorso\n");
        return;
    }

    if(partenza < destinazione) {
        struct stazione* curr = pointer_destinazione;

        while (curr != NULL && curr->chilometro > partenza){
            stazione* next_hop = NULL;
            stazione* scorr = predecessore(curr);
            if(scorr == NULL){
                printf("nessun percorso\n");
                return;
            }

            while (scorr != NULL && scorr->chilometro + scorr->max_auto >= curr->chilometro){
                next_hop = scorr;
                scorr = predecessore(scorr);
            }

            if(next_hop == NULL){
                printf("nessun percorso\n");
                return;
            }

            if(next_hop->chilometro < partenza){
                next_hop = pointer_partenza;
            }

            percorso[hop] = curr->chilometro;
            hop++;
            curr = next_hop;
            if(curr == pointer_partenza)
                percorso[hop] = curr->chilometro;

        }

        for (int i = hop; i >= 0 ; i--) {
            if(i>0)
                printf("%d ",percorso[i]);
            if(i==0)
                printf("%d\n",percorso[i]);
        }
    }
    else if(partenza > destinazione){
        struct stazione* curr = pointer_partenza;

        while (curr != NULL && curr->chilometro > destinazione){
            stazione* next_hop = NULL;
            stazione* scorr = predecessore(curr);
            if(scorr == NULL){
                printf("nessun percorso\n");
                return;
            }

            while (scorr != NULL && curr->chilometro - curr->max_auto <= scorr->chilometro){
                next_hop = scorr;
                scorr = predecessore(scorr);
            }

            if(next_hop == NULL){
                printf("nessun percorso\n");
                return;
            }

            if(next_hop->chilometro < destinazione){
                next_hop = pointer_destinazione;
            }

            percorso[hop] = curr->chilometro;
            hop++;
            curr = next_hop;
            if(curr == pointer_destinazione)
                percorso[hop] = curr->chilometro;

        }

        for (int i = 0; i <= hop ; i++) {
            if(i<hop)
                printf("%d ",percorso[i]);
            if(i==hop)
                printf("%d\n",percorso[i]);
        }
    }
    else
        printf("%d\n",partenza);
}

int main() {
    n_stazioni = 0;
    stazione* root = NULL;

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
                    root = insierisci_staz(root,distanza, automobili, n_auto);
                }
            } else if (strcmp(comando, "demolisci-stazione") == 0) {
                if (fscanf(stdin, "%d", &distanza) != EOF) {
                    root = cancella_staz(root,distanza);
                }
            } else if (strcmp(comando, "aggiungi-auto") == 0) {
                if (fscanf(stdin, "%d %d", &distanza, &autonomia) != EOF) {
                    aggiungi_auto(root, distanza, autonomia);
                }
            } else if (strcmp(comando, "rottama-auto") == 0) {
                if (fscanf(stdin, "%d %d", &distanza, &autonomia) != EOF) {
                    rottama_auto(root,distanza, autonomia);
                }
            } else if (strcmp(comando, "pianifica-percorso") == 0) {
                if (fscanf(stdin, "%d %d", &partenza, &destinazione) != EOF) {
                    pianifica(root, partenza, destinazione);
                }
            }
        }
    }

    //inorder(root);

    return 0;
}
