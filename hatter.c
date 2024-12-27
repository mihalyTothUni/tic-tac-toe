#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include "debugmalloc.h"
#include "hatter.h"
#include "pvpjatek.h"

//kilépés fv
bool menukilep(){
    char i=0;
    printf("Biztos ki akarsz lepni? (I/N) ");
    if(scanf(" %c", &i)!=1){
        printf("\nHibas bemeneti formatum!");
        exit(1);
    }
    i=toupper(i);
    if (i == 'I') {
        return true;
    } else return false;
}
//pálya memóriaterületének foglalása és inicializálása
void palyafoglal(palya *map) {
    //négyzet alakú pálya
    //oldalhossz = map.meret
    //sorok pointerek amik az oszlopokra mutatnak
    char **p=NULL;
    //hibás beolvasást szűrni
    if(map->meret > 0){
        p = (char**)calloc(map->meret, sizeof(char*));
    }
    //oszlop pointerek
    if(p==NULL){
        printf("\nSikertelen memrioafoglalas!\n");
        return;
    }
    for(int i=0; i<map->meret; i++){
        p[i] = (char*)calloc(map->meret, sizeof(char*));
    }
    map->hely = p;
    printf("\nMemoriafoglalas sikeres\n");
}
//pálya memóriaterületének felszabadítása
void palyafree(const palya *map) {
    if(map->hely == NULL){
        return;
    }
    for(int i=0; i<map->meret; i++){
        free(map->hely[i]);
    }
    free(map->hely);
}
//új játék kezdése
palya ujjatek() {
    palya map = {NULL,0};
    printf("Mekkora legyen a palya? (5-20)");
    int meret;
    if(scanf(" %d",&meret)!=1){
        printf("\nHibas bemeneti formatum!");
        exit(1);
    }
    printf("Hany elem kell a gyozelemhez? (3-7)");
    int gyozelem;
    if(scanf(" %d", &gyozelem)!=1){
        printf("\nHibas bemeneti formatum!");
        exit(1);
    }
    printf("Gep vagy jatekos elleni jatek? (G/J)");
    char ellenfel;
    if(scanf(" %c", &ellenfel)!=1){
        printf("\nHibas bemeneti formatum!");
        exit(1);
    }
    ellenfel = toupper(ellenfel);
    map.meret = meret;
    map.gyozelem = gyozelem;
    map.szabad = meret*meret;
    randomkezd(&map);
    if(ellenfel == 'J')
        map.pvp = true;
    else
        map.pvp = false;
    palyafoglal(&map);
    return map;
}

//aktuális állás kirajzolása
void palyakiir(const palya *map){
    printf("\n%3c",0);
    for (int k = 0; k < map->meret; k++)
        printf("%2c",'A'+k);
    printf("\n");
    for(int i=0; i<map->meret; i++){
        printf("%2d|",i+1);
        for(int j=0; j<map->meret; j++){
            if(map->hely[i][j]=='X' || map->hely[i][j]=='O')
                printf("%c ",map->hely[i][j]);
            else
                printf("  ");
        }
        printf("\n");

    }
}
//pálya mentése txt-be
void palyament(const palya *map){
    printf("fajl neve (max 50 karakter): ");
    char nev[55]; //nev+ .txt + lezáró
    if(scanf("%s",nev)!=1){
        printf("\nHibas bemeneti formatum!");
        exit(1);
    }
    strcat(nev,".txt");
    FILE *ki;
    ki = fopen(nev,"w");
    if(ki != NULL){
        if (map->pvp)
            fprintf(ki,"j\n");
        else
            fprintf(ki,"g\n");
        if (map->soron == 1)
            fprintf(ki,"p1\n");
        else
            fprintf(ki,"p2\n");
        fprintf(ki,"%d\n",map->meret);
        fprintf(ki,"%d\n",map->gyozelem);
        for (int i=0; i<map->meret; i++){
            for (int j = 0; j < map->meret; j++) {
                if(j==0) {
                    if(map->hely[i][j]==0){
                        fprintf(ki,"-\t");
                    }
                    else{
                        fprintf(ki,"%c\t",map->hely[i][j]);
                    }
                }
                else{
                    if(map->hely[i][j]==0){
                        fprintf(ki,"\t-\t");
                    }
                    else{
                        fprintf(ki,"\t%c\t",map->hely[i][j]);
                    }
                }
            }
            fprintf(ki,"\n");
        }
        fprintf(ki,"!");
        fclose(ki);
        printf("\nMentes sikeres!\n");
    }
    else printf("\nSikertelen mentes!\n");
}
//pálya betöltése txt-ből
palya palyabetolt() {
    char temp = 'I';
    palya map={NULL,0};
    do {
        printf("\nfajl neve: ");
        char nev[55]; //nev+ .txt + lezáró
        if(scanf("%s", nev)!=1){
            printf("\nHibas bemeneti formatum!");
            exit(1);
        }
        strcat(nev, ".txt");
        FILE *be;
        be = fopen(nev, "r");
        if (be != NULL) {
            char temps[3];
            fscanf(be, " %s", temps);
            if (temps[0] == 'g') map.pvp = false;
            else map.pvp = true;
            fscanf(be, " %s", temps);
            if (strcmp(temps, "p1") == 0) map.soron = 1;
            else map.soron = 2;
            fscanf(be, " %s", temps);
            sscanf(temps, "%d", &map.meret);

            fscanf(be, " %s", temps);
            sscanf(temps, "%d", &map.gyozelem);

            map.szabad = map.meret * map.meret;

            palyafoglal(&map);

            for (int i = 0; i < map.meret; i++) {
                for (int j = 0; j < map.meret; j++) {
                    fscanf(be," %c",&temp);
                    if(temp == '-'){
                        map.hely[i][j] = 0;
                    }
                    else{
                        map.hely[i][j] = temp;
                    }
                }
            }
            fclose(be);
            printf("\nBeolvasas sikeres");
            return map;
        }
        else printf("\nFajl megnyitasa sikertelen, megprobalod ujra? (I/N) ");
        if(scanf(" %c", &temp)!=1){
            printf("\nHibas bemeneti formatum!");
            exit(1);
        }
        temp = toupper(temp);
    } while (temp == 'I');
    return map;
}

