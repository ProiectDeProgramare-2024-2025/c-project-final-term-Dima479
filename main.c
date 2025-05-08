#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define nume_fisier "baza.txt"
#define RESET   "\x1b[0m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
struct vanzare {
    char nume[50];
    int buc;
    float pret;
};
char data[20];//data te azi(daca este introdusa)
int meniu();
void introducere();
void istoric();
void istoric_total();
void cea_mai_profitabila_zi();
int verifica_data_existenta();
void adaugare_data();
void media_cantitatilor_comandate();
void cel_mai_bine_vandut_produs();
int data_valida(char *data);

int main() {
    printf("Alegerea tipului de introducere a datelor.\n");
    printf("--------------------------------\n");
    printf("1.Adaugati in o zi noua\n2.Continuati adaugarea\n");
    int x;
    while(1)
    {
    scanf("%d",&x);
    if(x==1){adaugare_data();break;}
    else if(x==2)break;
    else {printf("Optiune invalida!!\n");}
    }
    system("cls");
    while (1) {
         meniu();
    }
    return 0;
}

int meniu() {
    int a;
    printf("Meniu\n");
    printf("--------------------------------\n");
    //printf("Introduceti o optiune\n");
    printf("1. Adauga vanzare\n");
    printf("2. Afisare istoric vanzari pentru o anmita zi\n");
    printf("3. Afisare istoric total\n");
    printf("4. Cea mai profitabila zi\n");
    printf("5. Cel mai bine vandut produs\n");
    printf("6. Media cantitatilor comandate\n");
    printf("7. Iesire\n");
    scanf("%d", &a);
        switch (a) {
        case 1:
            introducere();
            break;
        case 2:
            istoric();
            break;
        case 3:
            istoric_total();
            break;
        case 4:
            cea_mai_profitabila_zi();
            break;
        case 5:
            cel_mai_bine_vandut_produs();
            break;
        case 6:
            media_cantitatilor_comandate();
            break;
        case 7:
            exit(0);
            break;
    }
    system("cls");
    return a;
}
void introducere() {
    system("cls");
    printf("Introducerea unei noi vanzari.");
    FILE *file = fopen(nume_fisier, "a");
    if (file == NULL) {
        printf("Nu am putut deschide fisierul!\n");
        return;
    }
    struct vanzare v;
    char in[100];

        printf("Introduceti produsul in formatul: [nume]-[cantitate]bucati-[pret]roni\n");
    while(1) {
    scanf("%99s", in);
    if (sscanf(in, "%49[^-]-%dbucati-%froni", v.nume, &v.buc, &v.pret) != 3) {
        printf("Format invalid. Folositi formatul: [nume]-[cantitate]bucati-[pret]roni\n");
        fclose(file);
    }
        else {break;};
    }

    fprintf(file, "%s-%dbucati-%.2froni\n", v.nume, v.buc, v.pret);
    fclose(file);
    printf("Produs introdus cu succes!\n");
    system("pause");
}
void istoric() {
    system("cls");
    char data_cautata[20];
    printf("Introduceti data pentru care doriti istoricul (DD-LL-AAAA): ");
    scanf("%10s", data_cautata);
    FILE *file = fopen(nume_fisier, "r");
    if (file == NULL) {
        printf("Nu am putut deschide fisierul!\n");
        return;
    }
    char in[150];
    int in_sectiune = 0;
    int gasit = 0;
    printf("Istoric vanzari pentru data: %s\n", data_cautata);
    printf("--------------------------------\n");
    while (fgets(in, sizeof(in), file) != NULL) {
        if (strncmp(in, "<<", 2) == 0) {
            if (strstr(in, data_cautata) != NULL) {
                in_sectiune = 1;
                gasit = 1;
                continue;
            } else {
                in_sectiune = 0;
            }
        }

        if (in_sectiune) {
            struct vanzare v;
            if(sscanf(in, "%49[^-]-%dbucati-%froni", v.nume, &v.buc, &v.pret)==3) {
                printf("\x1b[32m%s\x1b[0m",  v.nume );
                printf("-");
                printf("\x1b[33m%d\x1b[0m",v.buc );
                printf("bucati-");
                printf("\x1b[34m%.2f\x1b[0m",v.pret );
                printf("roni\n" );
            }
        }
    }
    if (!gasit) {
        printf("Nu exista inregistrari pentru aceasta data.\n");
    }
    printf("--------------------------------\n");
    fclose(file);
    system("pause");
}
int data_valida(char *data) {
    int zi, luna, an;
    if (sscanf(data, "%2d-%2d-%4d", &zi, &luna, &an) != 3) {
        return 0; // format invalid
    }
    if (luna < 1 || luna > 12) {
        return 0;
    }
    int zile_luna[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if ((an % 4 == 0 && an % 100 != 0) || (an % 400 == 0)) {
        zile_luna[1] = 29;
    }

    if (zi < 1 || zi > zile_luna[luna - 1]) {
        return 0;
    }
    return 1; // data este validă
}
void adaugare_data() {
    do {
        printf("Introduceti data de astazi (DD-LL-AAAA): ");
        scanf("%10s", data);
        system("cls");

        if (!data_valida(data)) {
            printf("Data introdusa este invalida! Va rugam sa introduceti o data corecta.\n");
            continue;
        }

        if (verifica_data_existenta()) {
            printf("Aceasta data exista deja in baza de date! Introduceti o alta data.\n");
        } else {
            FILE *file = fopen(nume_fisier, "a");
            if (file == NULL) {
                printf("Eroare la deschiderea fisierului!\n");
                return;
            }
            fprintf(file, "\n<<%s>>\n", data);
            fclose(file);
            break;
        }
    } while (1);
}
int verifica_data_existenta() {
    FILE *file = fopen(nume_fisier, "r");
    if (file == NULL) {
        return 0;
    }

    char in[150];
    while (fgets(in, sizeof(in), file) != NULL) {
        if (strncmp(in, "<<", 2) == 0 && strstr(in, data) != NULL) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}
void istoric_total() {
    system("cls");

    FILE *file = fopen(nume_fisier, "r");
    if (file == NULL) {
        printf("Nu am putut deschide fisierul!\n");
        return;
    }

    char in[150];

    printf("Istoric total vanzari\n");
    printf("--------------------------------\n");

    while (fgets(in, sizeof(in), file) != NULL) {
        struct vanzare v;
        if(sscanf(in, "%49[^-]-%dbucati-%froni", v.nume, &v.buc, &v.pret)==3) {
            printf("\x1b[32m%s\x1b[0m",  v.nume );
            printf("-");
            printf("\x1b[33m%d\x1b[0m",v.buc );
            printf("bucati-");
            printf("\x1b[34m%.2f\x1b[0m",v.pret );
            printf("roni\n" );
        }
        else
            printf("%s\n", in);

    }

    printf("--------------------------------\n");

    fclose(file);
    system("pause");
}
void cea_mai_profitabila_zi() {
    system("cls");

    FILE *file = fopen(nume_fisier, "r");
    if (file == NULL) {
        printf("Nu am putut deschide fisierul!\n");
        return;
    }

    char in[150], data_curenta[20];
    float suma = 0.0, max_suma = 0.0;
    char cea_mai_buna_zi[20] = "";
    int in_sectiune = 0;

    while (fgets(in, sizeof(in), file) != NULL) {
        if (strncmp(in, "<<", 2) == 0) {
            if (in_sectiune && suma > max_suma) {
                max_suma = suma;
                strcpy(cea_mai_buna_zi, data_curenta);
            }

            sscanf(in, "<<%10[^>]>>", data_curenta);
            suma = 0.0;
            in_sectiune = 1;
        } else if (in_sectiune) {
            struct vanzare v;
            if (sscanf(in, "%49[^-]-%dbucati-%froni", v.nume, &v.buc, &v.pret) == 3) {
                suma += v.buc * v.pret;
            }
        }
    }

    if (in_sectiune && suma > max_suma) {
        max_suma = suma;
        strcpy(cea_mai_buna_zi, data_curenta);
    }

    fclose(file);
    printf("Cea mai profitabila zi\n");
    printf("--------------------------------\n");
    if (strlen(cea_mai_buna_zi) > 0) {

        printf("Cea mai profitabila zi este %s cu incasari de \x1b[34m%.2f\x1b[0m roni\n", cea_mai_buna_zi, max_suma);
    } else {
        printf("Nu exista date inregistrate!\n");
    }

    system("pause");
}
void cel_mai_bine_vandut_produs() {
    system("cls");

    FILE *file = fopen(nume_fisier, "r");
    if (file == NULL) {
        printf("Nu am putut deschide fisierul!\n");
        return;
    }

    struct vanzare v;
    char in[150];
    int max_buc = 0;
    char produs_max[50] = "";

    printf("Cel mai bine vandut produs:\n");
    printf("---------------------------\n");

    while (fgets(in, sizeof(in), file) != NULL) {
        if (sscanf(in, "%49[^-]-%dbucati-%froni", v.nume, &v.buc, &v.pret) == 3) {
            if (v.buc > max_buc) {
                max_buc = v.buc;
                strcpy(produs_max, v.nume);
            }
        }
    }

    if (strlen(produs_max) > 0) {
        printf("Cel mai bine vandut produs este:\x1b[32m %s\x1b[0m cu \x1b[33m%d\x1b[0m bucati.\n", produs_max, max_buc);
    } else {
        printf("Nu exista date de vanzari.\n");
    }

    fclose(file);
    system("pause");
}
void media_cantitatilor_comandate() {
    system("cls");

    FILE *file = fopen(nume_fisier, "r");
    if (file == NULL) {
        printf("Nu am putut deschide fisierul!\n");
        return;
    }

    struct vanzare v;
    char in[150];
    int total_buc = 0, numar_vanzari = 0;

    printf("Media cantitatilor comandate pentru fiecare produs:\n");
    printf("-----------------------------------------------\n");

    while (fgets(in, sizeof(in), file) != NULL) {
        if (sscanf(in, "%49[^-]-%dbucati-%froni", v.nume, &v.buc, &v.pret) == 3) {
            total_buc += v.buc;
            numar_vanzari++;
        }
    }

    if (numar_vanzari > 0) {
        float medie = (float) total_buc / numar_vanzari;
        printf("Media cantitatilor comandate este: \x1b[33m%.2f\x1b[0m bucati.\n", medie);
    } else {
        printf("Nu exista date de vanzari.\n");
    }

    fclose(file);
    system("pause");
}

