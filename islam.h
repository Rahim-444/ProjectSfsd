#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define blockSegments 300
#define MAX_OTHER_INFO_SIZE 250

typedef struct Contact {
    bool isDeleted;
    int iD;
    char name[31];
    char phoneNumber[11];
    char email[31];
    char otherInfo[MAX_OTHER_INFO_SIZE];
    struct Contact* next; 
} Contact;


typedef struct IndexFile {
    char id[10];
    char Pblock[20];
    struct IndexFile* next;
} IndexFile;

typedef struct Block {
  int blockNumber;  // num de block
  int ocupiedSpace; // in chars
  char Contacts[blockSegments];
  struct Block *nextBlock;
} Block;

int compareIds(const void *a, const void *b) {
    return strcmp(((IndexFile *)a)->id, ((IndexFile *)b)->id);
}

char* rechercheDichotomique(IndexFile* arr, int left, int right, const char *searchId) {
	
    if (right >= left) {
        int middle = left + (right - left) / 2;

        // Si l'élément est présent au milieu
        if (strcmp(arr[middle].id, searchId) == 0) {
		printf("%d, %08s, %s.\n",middle,arr[middle].id,arr[middle].Pblock);
        return arr[middle].Pblock ;
        }

        // Si l'élément est plus petit que celui au milieu, recherchez à gauche
        if (strcmp(arr[middle].id, searchId) > 0) {
            return rechercheDichotomique(arr, left, middle - 1, searchId);
        }

        // Sinon, recherchez à droite
        return rechercheDichotomique(arr, middle + 1, right, searchId);
    }
    printf("le Id: %08s n'existe pas!",searchId);
    // Si l'élément n'est pas présent dans le tableau

}

void merge(IndexFile arr[], int left, int middle, int right) {
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;

    // Créer des tableaux temporaires
    IndexFile leftArr[n1], rightArr[n2];

    // Copier les données dans les tableaux temporaires leftArr[] et rightArr[]
    for (i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        rightArr[j] = arr[middle + 1 + j];

    // Fusionner les tableaux temporaires dans arr[left..right]
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = left; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (strcmp(leftArr[i].id, rightArr[j].id) <= 0) {
            arr[k] = leftArr[i];
            i++;
        } else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    // Copier les éléments restants de leftArr[], s'il y en a
    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    // Copier les éléments restants de rightArr[], s'il y en a
    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}

void mergeSort(IndexFile* arr, int left, int right) {
    if (left < right) {
        // Same as (left+right)/2, but avoids overflow for large left and right
        int middle = left + (right - left) / 2;

        // Trier la première et la deuxième moitié
        mergeSort(arr, left, middle);
        mergeSort(arr, middle + 1, right);

        // Fusionner les deux moitiés triées
        merge(arr, left, middle, right);
    }
}

void afficherTableau(IndexFile* tableau, int tailleTableau) {
    printf("\nTableau :\n");
    //printf("%p",tableau[0].Pblock);
    for (int i = 0; i < tailleTableau; i++) {
    	if(i>=0&&i<=9){
    	printf("Index 0%d : ID = %s, PBlock: %s\n", i, tableau[i].id,tableau[i].Pblock);
		}else{
        printf("Index %d : ID = %s, PBlock: %s\n", i, tableau[i].id,tableau[i].Pblock);}
    }
}

void libererTableau(IndexFile* tableau) {
    free(tableau);
}

IndexFile* IndexFile_to_tableau(int* tailleTableau) {
    FILE *file = fopen("IndexFile.bin", "rb");
    IndexFile tmp;

    if (file != NULL) {
        *tailleTableau = 0; // Initialiser la taille du tableau

        // Compter le nombre d'éléments dans le fichier
        while (fscanf(file, "%8s,%11s\n", tmp.id, tmp.Pblock) == 2) {
            (*tailleTableau)++;
        }

        rewind(file); // Rembobiner le fichier

        // Allouer de la mémoire pour le tableau
        IndexFile* tableau = (IndexFile*)malloc(*tailleTableau * sizeof(IndexFile));
        if (tableau == NULL) {
            fprintf(stderr, "Erreur d'allocation de mémoire\n");
            exit(EXIT_FAILURE);
        }

        // Lire les données du fichier dans le tableau
        for (int i = 0; i < *tailleTableau; i++) {
            fscanf(file, "%8s,%11s\n", tableau[i].id, tableau[i].Pblock);
        }

        fclose(file); // Fermer le fichier
        return tableau;
    } else {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
}


int main() {

    int sizetab = 0;

    IndexFile* ptab = IndexFile_to_tableau(&sizetab);
   
    mergeSort(ptab, 0,sizetab-1);

    afficherTableau(ptab, sizetab);
    void* adresse;
    char tmp[16]="0x7ffd5e2b5b5f";
//	strcpy(tmp,rechercheDichotomique(ptab, 0, sizetab - 1, "10000019"));
    sscanf(tmp, "%p", &adresse);
   
	printf("%p",adresse);

    libererTableau(ptab);


    return 0;
}