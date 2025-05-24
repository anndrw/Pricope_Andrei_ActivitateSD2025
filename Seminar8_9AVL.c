#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
} Masina;

typedef struct Nod {
	Masina info;
	struct Nod* stanga;
	struct Nod* dreapta;
	int gradEchilibru;
} Nod;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	if (!fgets(buffer, 100, file)) return (Masina) { 0 };

	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

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

int inaltime(Nod* rad) {
	if (!rad) {
		return 0;
	}
	int hSt = inaltime(rad->stanga);
	int hDr = inaltime(rad->dreapta);
	return 1 + (hSt > hDr ? hSt : hDr);
}

void rotireStanga(Nod** radacina) {
	Nod* aux = (*radacina)->dreapta;
	(*radacina)->dreapta = aux->stanga;
	aux->stanga = (*radacina);
	(*radacina) = aux;
}

void rotireDreapta(Nod** radacina) {
	Nod* aux = (*radacina)->stanga;
	(*radacina)->stanga = aux->dreapta;
	aux->dreapta = (*radacina);
	(*radacina) = aux;
}

void adaugaMasinaInArboreEchilibrat(Nod** radacina, Masina m) {
	if (*radacina == NULL) {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = m;
		nou->stanga = NULL;
		nou->dreapta = NULL;
		nou->gradEchilibru = 0;
		*radacina = nou;
	}
	else if (m.id < (*radacina)->info.id) {
		adaugaMasinaInArboreEchilibrat(&(*radacina)->stanga, m);
	}
	else {
		adaugaMasinaInArboreEchilibrat(&(*radacina)->dreapta, m);
	}

	// recalcul echilibru
	int st = inaltime((*radacina)->stanga);
	int dr = inaltime((*radacina)->dreapta);
	(*radacina)->gradEchilibru = st - dr;


	if ((*radacina)->gradEchilibru > 1) {
		if (m.id < (*radacina)->stanga->info.id)
			rotireDreapta(radacina);
		else {
			rotireStanga(&(*radacina)->stanga);
			rotireDreapta(radacina);
		}
	}
	else if ((*radacina)->gradEchilibru < -1) {
		if (m.id > (*radacina)->dreapta->info.id)
			rotireStanga(radacina);
		else {
			rotireDreapta(&(*radacina)->dreapta);
			rotireStanga(radacina);
		}
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (!f) return NULL;
	Nod* rad = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		if (m.model) {
			adaugaMasinaInArboreEchilibrat(&rad, m);
		}
	}
	fclose(f);
	return rad;
}

void afisareMasiniDinArborePreOrdine(Nod* rad) {
	if (rad) {
		afisareMasina(rad->info);
		afisareMasiniDinArborePreOrdine(rad->stanga);
		afisareMasiniDinArborePreOrdine(rad->dreapta);
	}
}

void dezalocareArboreDeMasini(Nod* rad) {
	if (rad) {
		dezalocareArboreDeMasini(rad->stanga);
		dezalocareArboreDeMasini(rad->dreapta);
		free(rad->info.model);
		free(rad->info.numeSofer);
		free(rad);
	}
}

Masina getMasinaByID(Nod* rad, int id) {

	if (rad->info.id == id) {
		return rad->info;
	}

	if (id < rad->info.id) {
		return getMasinaByID(rad->stanga, id);
	}
	else {
		return getMasinaByID(rad->dreapta, id);
	}

}

int determinaNumarNoduri(Nod* rad) {
	if (!rad) {
		return 0;
	}

	return 1 + determinaNumarNoduri(rad->stanga) + determinaNumarNoduri(rad->dreapta);
}

float calculeazaPretTotal(Nod* rad) {
	if (!rad) {
		return 0;
	}
	return rad->info.pret + calculeazaPretTotal(rad->stanga) + calculeazaPretTotal(rad->dreapta);
}

float calculeazaPretulMasinilorUnuiSofer(Nod* rad, const char* sofer) {
	if (!rad) {
		return 0;
	}
	float s = strcmp(rad->info.numeSofer, sofer) == 0 ? rad->info.pret : 0;

	return s + calculeazaPretulMasinilorUnuiSofer(rad->stanga, sofer) + calculeazaPretulMasinilorUnuiSofer(rad->dreapta, sofer);
}

int main() {
	Nod* arbore = citireArboreDeMasiniDinFisier("masini_arbore.txt");

	printf("Afisare preordine:\n");
	afisareMasiniDinArborePreOrdine(arbore);

	printf("\nInaltime arbore: %d\n", inaltime(arbore));
	printf("Numar total noduri: %d\n", determinaNumarNoduri(arbore));
	printf("Pret total masini: %.2f\n", calculeazaPretTotal(arbore));

	int idCautat = 2;
	Masina m = getMasinaByID(arbore, idCautat);
	if (m.model) {
		printf("\nMasina cu ID %d:\n", idCautat);
		afisareMasina(m);
	}
	else {
		printf("\nMasina cu ID %d nu a fost gasita.\n", idCautat);
	}

	printf("Pretul total al masinilor soferului Ionescu: %.2f\n", calculeazaPretulMasinilorUnuiSofer(arbore, "Ionescu"));

	dezalocareArboreDeMasini(arbore);
	return 0;
}