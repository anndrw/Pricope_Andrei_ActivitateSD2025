#include<stdio.h>
#include<malloc.h>
//
struct Masina {
	int id;
	int anFabricatie;
	char* sofer;
	float kilometriiParcursi;
	char initialaProducator;
};

struct Masina initializare(int id, int anFabricatie, const char* sofer, float kilometriiParcursi, char initialaProducator) {
	struct Masina m;
	m.id = id;
	m.anFabricatie = anFabricatie;
	if (sofer != NULL)
	{
		m.sofer = (char*)malloc(strlen(sofer) + 1);
		strcpy_s(m.sofer, strlen(sofer) + 1, sofer);
	}
	else {
		m.sofer = NULL;
	}

	m.kilometriiParcursi = kilometriiParcursi;
	m.initialaProducator = initialaProducator;
	return m;
}

void afisare(struct Masina m) {
	printf("ID: %d \n", m.id);
	printf("An fabricatie: %d \n", m.anFabricatie);
	printf("Sofer: %s \n", m.sofer);
	printf("Kilometrii Parcursi : %f \n", m.kilometriiParcursi);
	printf("Initiala producator: %c \n", m.initialaProducator);
}
//
void afisareVector(struct Masina* vector, int nrElemente) {
	//afisarea elementelor din vector apeland functia afisare
	if (vector != NULL && nrElemente > 0)
	{
		for (int i = 0; i < nrElemente; i++)
		{
			printf("\n");
			afisare(vector[i]);
		}
	}
}

struct Masina* copiazaPrimeleNElemente(struct Masina* vector, int nrElemente, int nrElementeCopiate) {
	//copiem intr-un vector nou pe care il vom returna primele nrElementeCopiate
	struct Masina* vectorNou = NULL;
	if (vector != NULL && nrElementeCopiate <= nrElemente && nrElementeCopiate > 0)
	{
		vectorNou = malloc(sizeof(struct Masina) * nrElementeCopiate);
		for (int i = 0; i < nrElementeCopiate; i++)
		{
			vectorNou[i] = initializare(vector[i].id, vector[i].anFabricatie, vector[i].sofer, vector[i].kilometriiParcursi, vector[i].initialaProducator); // initializam din nou ca sa eveitam shallow copy -> facem deep copy asa

		}
	}

	return vectorNou;
}

void dezalocare(struct Masina** vector, int* nrElemente) {
	//dezalocam elementele din vector si vectorul | atentie avem **, iar sofer e char*
	if ((*vector) != NULL && (*nrElemente) > 0)
	{
		for (int i = 0; i < (*nrElemente); i++)
		{
			free((*vector)[i].sofer);
		}
		free(*vector);
		(*vector) = NULL;
		(*nrElemente) = 0;
	}
}

void copiazaMasiniCuMultiKm(struct Masina* vector, char nrElemente, float prag, struct Masina** vectorNou, int* dimensiune) {
	//parametrul prag poate fi modificat in functie de 
	// tipul atributului ales pentru a indeplini o conditie
	//este creat un nou vector cu elementele care indeplinesc acea conditie
	if (vector != NULL && nrElemente > 0)
	{
		(*dimensiune) = 0;
		for (int i = 0; i < nrElemente; i++)
		{
			if (vector[i].kilometriiParcursi > prag)
			{
				(*dimensiune)++;
			}
		}
		(*vectorNou) = malloc(sizeof(struct Masina) * (*dimensiune));
		int contor = 0;
		for (int i = 0; i < nrElemente; i++)
		{
			if (vector[i].kilometriiParcursi > prag)
			{
				(*vectorNou)[contor] = vector[i]; // asta e shallow copy, recomandat e deep copy totusi adica folosim initializare()
				(*vectorNou)[contor].sofer = malloc(sizeof(char) * (strlen(vector[i].sofer) + 1)); // alocam memorie pentru sofer
				strcpy_s((*vectorNou)[contor].sofer, strlen(vector[i].sofer) + 1, vector[i].sofer);
				contor++;
			}
		}
	}
}

struct Masina getPrimaMasinaDupaSofer(struct Masina* vector, int nrElemente, const char* soferCautat) {
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.
	struct Masina s;
	for (int i = 0; i < nrElemente; i++)
	{
		if (strcmp(soferCautat, vector[i].sofer) == 0)
		{
			return vector[i];
		}
	}


	return initializare(-1, -1, NULL, 0, '-');
}



int main() {

	int nrElemente = 3;
	struct Masina* vector = malloc(sizeof(struct Masina) * nrElemente);

	vector[0] = initializare(1, 2010, "Marian", 2100, 'F');
	vector[1] = initializare(2, 2008, "Cristi", 1500.6, 'A');
	vector[2] = initializare(3, 2017, "Stefan", 7500, 'M');

	afisareVector(vector, nrElemente);

	// facem un nou vector si lucram pe el pentru a nu strica vectorul initial, lucram pe o copie a lui si cautam sa luam doar primele 2 elemente
	int nrElementeCopiate = 2;
	struct Masina* vectorNou = copiazaPrimeleNElemente(vector, nrElemente, nrElementeCopiate); // copiem primele 2 masini

	printf("\nMasini copiate");
	afisareVector(vectorNou, nrElementeCopiate); // afisam vectorul nou

	dezalocare(&vectorNou, &nrElementeCopiate); // dezalocam vectorul nou

	float prag = 2000;
	copiazaMasiniCuMultiKm(vector, nrElemente, prag, &vectorNou, &nrElementeCopiate);
	printf("\nMasini cu multi km:\n");
	afisareVector(vectorNou, nrElementeCopiate);
	dezalocare(&vectorNou, &nrElementeCopiate); 

	printf("\nPrima masina dupa cautarea 'Marian'\n ");
	struct Masina primaMasina = getPrimaMasinaDupaSofer(vector, nrElemente, "Marian");
	afisare(primaMasina);

	dezalocare(&vector, &nrElemente); 

	return 0;
}