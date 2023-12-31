//
// Created by edoua on 14/11/2023.
//

#include "edouard.h"

#include <stdio.h>
#include <stdlib.h>



int lireNombreOperations(const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur de l'ouverture du fichier :  %s\n", nomFichier);
        exit(0);
    }
    int nombreOperations = 0;
    char ligne[100]; // Assurez-vous que cette taille est suffisante pour contenir une ligne complète

    // Lecture du fichier ligne par ligne
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        nombreOperations++;
    }
    fclose(fichier);
    return nombreOperations;

}

void lireContraintes(const char *nomFichier, int contraintes[][2], int *nombreContraintes) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur de l'ouverture du fichier :  %s\n", nomFichier);
        exit(0);
    }
    *nombreContraintes = 0;
    while (fscanf(fichier, "%d %d", &contraintes[*nombreContraintes][0], &contraintes[*nombreContraintes][1]) == 2) {
        (*nombreContraintes)++;
    }
    fclose(fichier);
}

Graphe creerGraphe(int contraintes[][2], int nombreContraintes) {
    Graphe graphe;
    graphe.nombreSommets = 0;

    for (int i = 0; i < nombreContraintes; i++) {
        if (contraintes[i][0] > graphe.nombreSommets) {
            graphe.nombreSommets = contraintes[i][0];
        }
        if (contraintes[i][1] > graphe.nombreSommets) {
            graphe.nombreSommets = contraintes[i][1];
        }
    }

    graphe.matriceAdjacence = (int **)malloc((graphe.nombreSommets + 1) * sizeof(int *));
    if (graphe.matriceAdjacence == NULL) {
        printf("Erreur \n");
        exit(0);
    }

    for (int i = 0; i <= graphe.nombreSommets; i++) {
        graphe.matriceAdjacence[i] = (int *)calloc((graphe.nombreSommets + 1), sizeof(int));
        if (graphe.matriceAdjacence[i] == NULL) {
            printf("Erreur \n");
            exit(0);
        }
    }
    for (int i = 0; i < nombreContraintes; i++) {
        graphe.matriceAdjacence[contraintes[i][0]][contraintes[i][1]] = 1;
        graphe.matriceAdjacence[contraintes[i][1]][contraintes[i][0]] = 1;
    }
    return graphe;
}


// Coloration d'un graphe grâce à l'algorithme naif
void colorerGraphe(Graphe graphe, Station stations[], int *nombreStations) {
    int *couleurs = (int *)calloc((graphe.nombreSommets + 1), sizeof(int));
    if (couleurs == NULL) {
        printf( "Erreur \n");
        exit(0);
    }
    for (int i = 1; i <= graphe.nombreSommets; i++) {
        couleurs[i] = 0;
    }
    for (int sommet = 1; sommet <= graphe.nombreSommets; sommet++) {
        int couleurDisponible = 1;

        for (int voisin = 1; voisin <= graphe.nombreSommets; voisin++) {
            if (graphe.matriceAdjacence[sommet][voisin] && couleurs[voisin] != 0) {
                couleurDisponible = 0;
                break;
            }
        }
        if (couleurDisponible) {
            couleurs[sommet] = 1;
        } else {
            for (int couleur = 2; couleur <= *nombreStations + 1; couleur++) {
                int couleurLibre = 1;
                for (int voisin = 1; voisin <= graphe.nombreSommets; voisin++) {
                    if (graphe.matriceAdjacence[sommet][voisin] && couleurs[voisin] == couleur) {
                        couleurLibre = 0;
                        break;
                    }
                }
                if (couleurLibre) {
                    couleurs[sommet] = couleur;
                    break;
                }
            }
        }
        if (couleurs[sommet] > *nombreStations) {
            *nombreStations = couleurs[sommet];
        }
    }
    for (int i = 1; i <= *nombreStations; i++) {
        stations[i].operation = (int *)malloc(graphe.nombreSommets * sizeof(int));
        stations[i].nb_operation = 0;
    }
    // Ajout de chaque opération à la station associée
    for (int sommet = 1; sommet <= graphe.nombreSommets; sommet++) {
        stations[couleurs[sommet]].operation[stations[couleurs[sommet]].nb_operation++] = sommet;
    }
    free(couleurs);
}

void afficherStations(Station stations[], int nombreStations, int nombreOperations) {
    printf("Nombre total d'operations : %d\n", nombreOperations);
    printf("Repartition des stations :\n");
    for (int i = 1; i <= nombreStations; i++) {
        printf("Station %d : \n", i);
        printf("Operations : ");
        for (int j = 0; j < stations[i].nb_operation; j++) {
            printf(" %d ", stations[i].operation[j]);
        }
        printf("\n");
    }

    printf("Lorsqu'on considere que les contraintes d'exclusion, nous obtenons %d stations \n", nombreStations);
}

void repartition_station_exclusion(char *fichier, char*operations) {
    int nombre_operations = lireNombreOperations(operations);
    int contraintes[nombre_operations][2];
    int nombreContraintes;
    Graphe graphe;
    Station stations[nombre_operations];
    int nombreStations = 0;
    lireContraintes(fichier, contraintes, &nombreContraintes);
    graphe = creerGraphe(contraintes, nombreContraintes);
    colorerGraphe(graphe, stations, &nombreStations);
    afficherStations(stations, nombreStations, nombre_operations);
}
int liretempscycle(char *nomFichier) {
    int temps;
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur de l'ouverture du fichier :  %s\n", nomFichier);
        exit(0);
    }
    fscanf(fichier, "%d", &temps) ;
    printf("temps_cycle %d ", temps);
    fclose(fichier);
    return temps ;
}

int main() {

    Graphe graphe ;
    char *operation = "../operations.txt";
    char *precedences = "../precedences.txt";
    char *exclusion = "../exclusions.txt";
    char *temps_cycle = "../temps_cycle.txt";
    repartition_station_exclusion(exclusion, operation);
    liretempscycle(temps_cycle);
    return 0;
}
