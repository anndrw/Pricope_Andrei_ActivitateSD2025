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

//creare structura pentru un nod dintr-o lista dublu inlantuita

struct Nod {
	Masina info;
	struct Nod* urmator;
	struct Nod* precedent;
};
typedef struct Nod Nod;

//creare structura pentru Lista Dubla 

struct ListaDubla {
	Nod* prim;
	Nod* ultim;
};
typedef struct ListaDubla Lista;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasiniInceput(Lista lista) {
	//afiseaza toate elemente de tip masina din lista dublu inlantuita
	//prin apelarea functiei afisareMasina()
	Nod* p = lista.prim;
	while (p) {
		afisareMasina(p->info);
		p = p->urmator;
	}
}

void afisareListaMasiniSfarsit(Lista lista) {
	Nod* p = lista.ultim;
	while (p) {
		afisareMasina(p->info);
		p = p->precedent;
	}
}

void adaugaMasinaInLista(Lista* lista, Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	Nod* p = (Nod*)malloc(sizeof(Nod));

	p->info = masinaNoua;
	p->precedent = lista->ultim;
	p->urmator = NULL;

	if (lista->ultim != NULL) {
		lista->ultim->urmator = p;
	}
	else {
		lista->prim = p;
	}

	lista->ultim = p;
}

void adaugaLaInceputInLista(Lista* lista, Masina masinaNoua) {
	//adauga la inceputul listei dublu inlantuite o noua masina pe care o primim ca parametru
	Nod* p = (Nod*)malloc(sizeof(Nod));
	p->info = masinaNoua;
	p->urmator = lista->prim;
	p->precedent = NULL;

	if (lista->prim != NULL)
	{
		lista->prim->precedent = p;
	}
	else {
		lista->ultim = p;
	}
	lista->prim = p;
}

Lista citireLDMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");

	Lista lista;
	lista.prim = NULL;
	lista.ultim = NULL;

	while (!feof(f)) {
		//adaugaMasinaInLista(&lista, citireMasinaDinFisier(f)); // in ordine
		adaugaLaInceputInLista(&lista, citireMasinaDinFisier(f)); // invers
	}
	fclose(f);

	return lista;
}

void dezalocareLDMasini(Lista* lista) {
	//sunt dezalocate toate masinile si lista dublu inlantuita de elemente
	Nod* p = lista->prim;
	while (p->urmator != NULL) {
		free(p->info.numeSofer);
		free(p->info.model);
		p = p->urmator;
		free(p->precedent);
	}
	// ultimul nod ramas ->
	free(p->info.numeSofer);
	free(p->info.model);
	free(p);

	lista->prim = NULL;
	lista->ultim = NULL;
}

float calculeazaPretMediu(Lista lista) {
	//calculeaza pretul mediu al masinilor din lista.
	Nod* aux = lista.prim;
	float suma = 0;
	float contor = 0;

	while (aux) {
		suma = suma + aux->info.pret;
		contor++;
		aux = aux->urmator;
	}

	return suma / contor;
}

void stergeMasinaDupaID(/*lista masini*/ int id) {
	//sterge masina cu id-ul primit.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
}

char* getNumeSoferMasinaScumpa(/*lista dublu inlantuita*/) {
	//cauta masina cea mai scumpa si 
	//returneaza numele soferului acestei maasini.
	return NULL;
}

int main() {
	Lista lista;
	lista = citireLDMasiniDinFisier("masinisem4.txt");
	afisareListaMasiniInceput(lista);
	printf("\n");
	afisareListaMasiniSfarsit(lista);

	float pretMediu = calculeazaPretMediu(lista);
	printf("\nPretul mediu este %.2f", pretMediu);

	dezalocareLDMasini(&lista);

	return 0;
}