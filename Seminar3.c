#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

void afisareMasina(Masina masina) {
	printf("ID: %d\n", masina.id);
	printf("Nr usi: %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);

}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	//afiseaza toate elemente de tip masina din vector
	//prin apelarea functiei afisareMasina()
	int i;
	for (i = 0; i < nrMasini; i++)
	{
		afisareMasina(masini[i]);
	}
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
	//adauga in vectorul primit o noua masina pe care o primim ca parametru
	//ATENTIE - se modifica numarul de masini din vector;
	Masina* aux = (Masina*)malloc(sizeof(Masina) * ((*nrMasini) + 1));
	for (int i = 0; i < (*nrMasini); i++)
	{
		aux[i] = (*masini)[i];
	}

	aux[(*nrMasini)] = masinaNoua;
	free((*masini));

	(*masini) = aux;
	(*nrMasini) = (*nrMasini) + 1;
}

Masina citireMasinaFisier(FILE* file) {
	//functia citeste o masina dintr-un stream deja deschis
	//masina citita este returnata;
	Masina m;
	char buffer[100];
	char delimiter[4] = ",;\n";
	fgets(buffer, 100, file);

	char* aux;
	aux = strtok(buffer, delimiter);
	m.id = atoi(aux);
	m.nrUsi = atoi(strtok(NULL, delimiter)); // punem NULL ca sa ramana pe acelasi rand si sa treaca de id de exemplu
	m.pret = atoi(strtok(NULL, delimiter));

	// pentru model care e char*
	aux = strtok(NULL, delimiter);
	m.model = malloc(strlen(aux) + 1);
	strcpy_s(m.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, delimiter);
	m.numeSofer = malloc(strlen(aux) + 1); // daca cumva nu merge din cauza compilatorului, trebuie sa dam cast
	strcpy_s(m.numeSofer, strlen(aux) + 1, aux);

	aux = strtok(NULL, delimiter);
	m.serie = aux[0]; // serie e char

	return m;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaFisier()
	//numarul de masini este determinat prin numarul de citiri din fisier
	//ATENTIE - la final inchidem fisierul/stream-ul

	FILE* f = fopen(numeFisier, "r");
	Masina* masini = NULL; // vectorul de masini

	while (!feof(f)) { // cat timp nu e end of file
		Masina m = citireMasinaFisier(f); // o singura masina
		adaugaMasinaInVector(&masini, nrMasiniCitite, m);
	}
	fclose(f);

	return masini;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	for (int i = 0; i < (*nrMasini); i++)
	{
		free((*vector)[i].model);
		free((*vector)[i].numeSofer);
	}
	free(*vector);
	(*vector) = NULL;
	(*nrMasini) = 0;
}

int main() {

	Masina* masini = NULL;
	int nrMasini = 0;
	masini = citireVectorMasiniFisier("masinisem3.txt", &nrMasini);
	afisareVectorMasini(masini, nrMasini);
	dezalocareVectorMasini(&masini, &nrMasini);

	return 0;
}