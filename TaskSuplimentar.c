#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Apartament {
	int numar_apartament;
	char* proprietar;
	int numar_camere;
	float suprafata;
	float pret;
};

struct Apartament initializare(int numar_apartament, const char* proprietar, int numar_camere, float suprafata, float pret) {
	struct Apartament a;
	a.numar_apartament = numar_apartament;
	a.proprietar = (char*)malloc(sizeof(char) * (strlen(proprietar) + 1));
	strcpy(a.proprietar, proprietar);
	a.numar_camere = numar_camere;
	a.suprafata = suprafata;
	a.pret = pret;
	return a;
}

void afisareApartament(struct Apartament a) {
	if (a.proprietar != NULL) {
		printf("Apartamentul cu numarul %d, proprietar %s, %d camere, suprafata %.2f si pretul %.2f\n", a.numar_apartament, a.proprietar, a.numar_camere, a.suprafata, a.pret);
	}
	else {
		printf("Apartamentul cu numarul %d, proprietar necunoscut, %d camere, suprafata %.2f si pretul %.2f\n", a.numar_apartament, a.numar_camere, a.suprafata, a.pret);
	}
}

void dezalocareApartament(struct Apartament* a) {
	if (a->proprietar != NULL) {
		free(a->proprietar);
		a->proprietar = NULL;
	}
}

// functie pentru citirea unui apartament de la tastatura si returnarea acestuia
struct Apartament citireApartament() {
	struct Apartament a;
	printf("Numar apartament: ");
	scanf("%d", &a.numar_apartament);
	printf("Proprietar: ");
	char buffer[50];
	scanf("%s", buffer);
	a.proprietar = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(a.proprietar, buffer);
	printf("Numar camere: ");
	scanf("%d", &a.numar_camere);
	printf("Suprafata: ");
	scanf("%f", &a.suprafata);
	printf("Pret: ");
	scanf("%f", &a.pret);
	return a;
}

// calculam pretul in functie de suprafata si numarul de camere
float calculPretPiata(struct Apartament a) {
	return a.suprafata * 1000 + a.numar_camere * 5000;
}

// functie care modifica proprietarul unui apartament
void modificaProprietar(struct Apartament* a, const char* proprietar) {
	if (a->proprietar != NULL) {
		free(a->proprietar);
	}
	a->proprietar = (char*)malloc(sizeof(char) * (strlen(proprietar) + 1));
	strcpy(a->proprietar, proprietar);
}

void afisareVector(struct Apartament* vector, int nrApartamente) {
	for (int i = 0; i < nrApartamente; i++) {
		afisareApartament(vector[i]);
	}
}

// functie care va crea un vector nou in care se vor copia toate elementele din vectorul initial care indeplinesc conditia denumita prag
struct Apartament* apartamenteVectorPrag(struct Apartament* vector, int nrApartamente, float prag) {

	struct Apartament* vectorNou = (struct Apartament*)malloc(sizeof(struct Apartament) * nrApartamente);
	int nrApartamenteNou = 0;
	for (int i = 0; i < nrApartamente; i++) {
		if (vector[i].pret < prag) {
			vectorNou[nrApartamenteNou] = vector[i];
			nrApartamenteNou++;
		}
	}
	afisareVector(vectorNou, nrApartamenteNou);
	return vectorNou;
}

int main() {
	// task 1
	struct Apartament a;
	a = initializare(1, "Andrei", 3, 70, 100000);

	afisareApartament(a);

	struct Apartament b = citireApartament();
	afisareApartament(b);

	printf("Pretul real al apartamentului ales este %5.2f\n", calculPretPiata(a));

	printf("Modificam proprietarul apartamentului ->\n");
	modificaProprietar(&a, "Cristian");
	afisareApartament(a);

	// task 2
	int nrApartamente = 5;
	struct Apartament* vector = (struct Apartament*)malloc(sizeof(struct Apartament) * nrApartamente);
	printf("---------------------------------\n");
	printf("Vectorul de apartamente este:\n");
	vector[0] = a;
	vector[1] = b;
	vector[2] = initializare(3, "Gabriel", 2, 50, 40000);
	vector[3] = initializare(4, "Gigel", 4, 90, 120000);
	vector[4] = initializare(5, "Firicel", 1, 20, 17000);
	afisareVector(vector, 5);

	printf("---------------------------------\n");
	printf("Apartamentele cu pretul mai mic decat 50000 sunt:\n");
	struct Apartament* vectorNou = apartamenteVectorPrag(vector, nrApartamente, 50000);



	dezalocareApartament(&a);
	dezalocareApartament(&b);

	return 0;
}