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

//creare structura pentru un nod dintr-o lista simplu inlantuita
struct Nod {
	Masina info;
	struct Nod* next;
};
typedef struct Nod N;

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

void afisareListaMasini(N* nod) {
	//afiseaza toate elemente de tip masina din lista simplu inlantuita
	//prin apelarea functiei afisareMasina()
	while (nod)
	{
		afisareMasina(nod->info);
		nod = nod->next;
	}
}

void adaugaMasinaInLista(N** lista, Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	N* nodNou = (N*)malloc(sizeof(N)); // nodul nou este nodul ce o sa contina masina noua
	nodNou->info = masinaNoua;
	nodNou->next = NULL;

	if ((*lista) == NULL) {
		(*lista) = nodNou;
	}
	else {
		N* aux = (*lista);
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nodNou;
	}
}

void adaugaLaInceputInLista(/*lista de masini*/ Masina masinaNoua) {
	//adauga la inceputul listei o noua masina pe care o primim ca parametru
}

N* citireListaMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	N* lista = NULL; // obligatoriu NULL ca sa intre in conditie de adaugare in lista

	while (!feof(f)) {
		Masina masinaNoua;
		masinaNoua = citireMasinaDinFisier(f);
		adaugaMasinaInLista(&lista, masinaNoua);
	}
	fclose(f);

	return lista;
}

void dezalocareListaMasini(N** lista) {
	while ((*lista)) {
		N* p = (*lista);
		free(p->info.model);
		free(p->info.numeSofer);
		(*lista) = (*lista)->next;
		free(p);
	}
}

float calculeazaPretMediu(N* lista) {
	//calculeaza pretul mediu al masinilor din lista.
	float suma = 0;
	int contor = 0;
	while (lista) {
		suma = suma + lista->info.pret;
		contor++;
		lista = lista->next;
	}
	if (contor == 0) {
		return 0;
	}

	return suma / contor;
}

void stergeMasiniDinSeria(char seriaCautata, N** lista) {
	//sterge toate masinile din lista care au seria primita ca parametru.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
	N* p = (*lista);
	N* aux = NULL;
	while (p) {
		if (p->info.serie == seriaCautata) {
			if (p == (*lista)) {
				aux = p;
				(*lista) = (*lista)->next;
				free(aux->info.model);
				free(aux->info.numeSofer);
				free(aux);
				p = (*lista);
			}
			else {
				aux->next = p->next;
				free(p->info.model);
				free(p->info.numeSofer);
				free(p);
				p = aux->next;
			}
		}
		else {
			aux = p;
			p = p->next;
		}
	}
}

float calculeazaPretulMasinilorUnuiSofer(N* lista, const char* numeSofer) {
	float suma = 0;

	while (lista) {
		if (strcmp(lista->info.numeSofer, numeSofer) == 0) {
			suma = suma + lista->info.pret;
		}
		lista = lista->next;
	}

	return suma;
}

int getNrUsiMasinaScumpa(N* lista) {
	if (lista) {
		float pretMaxim = lista->info.pret;
		int nrUsi = lista->info.nrUsi;
		lista = lista->next;
		while (lista) {
			if (lista->info.pret > pretMaxim) {
				nrUsi = lista->info.nrUsi;
				pretMaxim = lista->info.pret;
			}
			lista = lista->next;
		}
		return nrUsi;
	}
	return 0;
}


int main() {
	N* nod;
	nod = citireListaMasiniDinFisier("masinisem4.txt");
	afisareListaMasini(nod);

	float medie;
	medie = calculeazaPretMediu(nod);
	printf("Media este: %.2f", medie);

	float suma;
	suma = calculeazaPretulMasinilorUnuiSofer(nod, "Ionescu");
	printf("\n");
	printf("Suma este: %.2f", suma);

	int nr;
	nr = getNrUsiMasinaScumpa(nod);
	printf("\n");
	printf("Numar usi -> cea mai scumpa masina: %d", nr);

	stergeMasiniDinSeria('A', &nod);
	printf("\nDupa stergere:\n");
	afisareListaMasini(nod);

	dezalocareListaMasini(&nod);



	return 0;
}