#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "debugmalloc.h"
#include "hatter.h"
#include "pvpjatek.h"
#include "pvejatek.h"


//átváltja a soron lévő játékost
void soron_valt(palya *map){
    if(map->soron == 1) map->soron = 2;
    else map->soron = 1;
}
//ellenőrzi, hogy érvényes-e a lépés
bool szabad(palya *map, int x, int y) {
    if (x >= map->meret || y >= map->meret){
        printf("X vagy Y tul nagy!");
        return false;
    }
    if (x < 0 || y < 0){
        printf("X vagy Y tul kicsi!");
        return false;
    }

    if (map->hely[x][y] != 0) {
        printf("Foglalt a mezo!");
        return false;
    }
    return true;
}

//x lerakása
void place_x(palya *map,int x, int y){
    if(szabad(map,x,y)){
        map->hely[x][y] = 'X';
        map->szabad--;
        soron_valt(map);
    }
}

//o lerakása
void place_o(palya *map,int x, int y){
    if(szabad(map,x,y)){
        map->hely[x][y] = 'O';
        map->szabad--;
        soron_valt(map);
    }
}

//visszaadja az előző körben lépett játékos karakterét
//teszteléshez, hogy nyertes lépést rakott-e le
//p1 az O; p2 az X
char playertochar(palya *map){
    if(map->soron == 1) return 'X';
    else return 'O';
}
//win condition kereső fv-ek
//bejárják a mátrixot
//vízszintesen keres
bool kelet(palya *map){
    int sor;
    char jatekoskarakter = playertochar(map);
    //sorok
    for (int i = 0; i < map->meret; i++) {
        //oszlopok
        for (int j = 0; j < map->meret; j++) {
            //ha talál jatekoskarakter-t
            if(map->hely[i][j] == jatekoskarakter){
                //megszámolja hány jatekoskarakter van vízszintesen egy sorban
                sor = 0;
                for (int k = 0; k < map->gyozelem; k++) {
                    if(!(j+k >= map->meret)){       //ha túlindexelne álljon le a keresés
                        if(map->hely[i][j+k] == jatekoskarakter) sor++;
                    }
                }
                //ha ez elég, visszatér
                if (sor == map->gyozelem) return true;
            }
        }
    }
    //amúgy hamis
    return false;
}
//átlósan jobbra keres
bool delkelet(palya *map){
    int sor;
    char jatekoskarakter = playertochar(map);
    //sorok
    for (int i = 0; i < map->meret; i++) {
        //oszlopok
        for (int j = 0; j < map->meret; j++) {
            //ha talál jatekoskarakter-t
            if(map->hely[i][j] == jatekoskarakter){
                //megszámolja hány jatekoskarakter van átlósan egy sorban
                sor=0;
                for (int k = 0; k < map->gyozelem; k++) {
                    if(!(i+k >= map->meret || j+k >= map->meret)){    //ha túlindexelne álljon le a keresés
                        if(map->hely[i+k][j+k] == jatekoskarakter) sor++;
                    }
                }
                //ha ez elég, visszatér
                if (sor == map->gyozelem) return true;
            }
        }
    }
    //amúgy hamis
    return false;
}
//függőlegesen keres
bool del(palya *map){
    int sor;
    char jatekoskarakter = playertochar(map);
    //sorok
    for (int i = 0; i < map->meret; i++) {
        //oszlopok
        for (int j = 0; j < map->meret; j++) {
            //ha talál jatekoskarakter-t
            if(map->hely[i][j] == jatekoskarakter){
                //megszámolja hány jatekoskarakter van függőlegesen egy sorban
                sor=0;
                for (int k = 0; k < map->gyozelem; k++) {
                    if(!(i+k >= map->meret)){       //ha túlindexelne álljon le a keresés
                        if(map->hely[i+k][j] == jatekoskarakter) sor++;
                    }

                }
                //ha ez elég, visszatér
                if (sor == map->gyozelem) return true;
            }
        }
    }
    //amúgy hamis
    return false;
}
//átlósan balra keres
bool delnyugat(palya *map){
    int sor;
    char jatekoskarakter = playertochar(map);
    //sorok
    for (int i = 0; i < map->meret; i++) {
        //oszlopok
        for (int j = 0; j < map->meret; j++) {
            //ha talál jatekoskarakter-t
            if(map->hely[i][j] == jatekoskarakter){
                //megszámolja hány jatekoskarakter van átlósan egy sorban
                sor=0;
                for (int k = 0; k < map->gyozelem; k++) {
                    if (!(j-k < 0 || i+k >= map->meret)) { //ha túlindexelne álljon le a keresés
                        if (map->hely[i + k][j - k] == jatekoskarakter) sor++;
                    }

                }
                //ha ez elég, visszatér
                if (sor == map->gyozelem) return true;
            }
        }
    }
    //amúgy hamis
    return false;
}

//elfogytak-e a szabad helyek
bool betelt(palya *map){
    if (map->szabad == 0) return true;
    return false;
}

//van-e nyertes bármelyik irányban
bool nyert(palya *map){
     return (kelet(map) || delkelet(map) || del(map) || delnyugat(map));
}

//vége van-e a játéknak
//-1 nincs vége
//0  döntetlen
//1  p1 nyert pvp módban    11 pve módban
//2  p2 nyert pvp módban    12 pve módban
//átírni hogy enumot használjon WIP
int vege(palya *map){
    if(betelt(map)) return 0;
    if (map->pvp){
        //legutolsó lépéssel megnyerte a játékot de lépés után megváltozik az aktív játékos
        if(nyert(map) && map->soron == 1) return 2;
        if(nyert(map) && map->soron == 2) return 1;
    }
    else{
        if(nyert(map) && map->soron == 1) return 12;
        if(nyert(map) && map->soron == 2) return 11;
    }
    return -1;
}
//a kezdés randomizáltságáért felel
void randomkezd(palya *map){
    srand(time(0));
    map->soron = rand() % 2 + 1;
}

//kilépés mentéssel vagy anélkül a játékból
bool jatekkilep(palya *map){
    char i=0;
    printf("Biztos ki akarsz lepni? (I/N) ");
    if(scanf(" %c", &i)!=1){
        printf("\nHibas bemeneti formatum!");
        exit(1);
    }
    i=toupper(i);
    if (i == 'I') {
        printf("Szeretned menteni az allast? (I/N) ");
        if(scanf(" %c",&i)!=1){
            printf("\nHibas bemeneti formatum!");
            exit(1);
        }
        i= toupper(i);
        if (i == 'I'){
            palyament(map);
        }
        printf("Kilepes a fomenube...\n");
        return true;
    }
    else
        return false;
}

//beolvasás és eldönti hogy kilépés vagy koordináta
bool beolvas_es_rak(palya *map){
    char sor[5];
    if(scanf(" %s",sor)!=1){
        printf("\nHibas bemeneti formatum!");
        exit(1);
    }
    if(strcasecmp(sor,"K")==0) {
        return jatekkilep(map);
    }
    else {
        int x;
        char y;
        sscanf(sor," %c %d",&y, &x);
        y = toupper(y);
        x-=1;
        y = y - 'A';
        if(map->soron == 1) place_o(map,x,y);
        else place_x(map,x,y);
    }
}

//a pvp módú játék megvalósítása
void pvp(palya *map){
    if(map->hely==NULL || map->gyozelem<0){
        return;
    }
    bool kilep=false;
    while (vege(map) == -1 && !kilep){
        palyakiir(map);
        printf( map->soron == 2 ? "\nX jon" : "\nO jon");
        printf("\nKilepes a fomenube (K) \nLepes koordinataval (oszlop+sor) pl A1: ");
        kilep = beolvas_es_rak(map);
    }
    switch (vege(map)) {
        case 0:
            printf("\nDontetlen!");
            palyakiir(map);
            break;
        case 1:
            printf("\nPlayer 1 (O) nyert!");
            palyakiir(map);
            break;
        case 2:
            printf("\nPlayer 2 (X) nyert!");
            palyakiir(map);
            break;
        case 11:
            printf("\nPlayer (O) nyert!");
            palyakiir(map);
            break;
        case 12:
            printf("\nSzamitogep (X) nyert!");
            palyakiir(map);
            break;
    }

}
//ha visszavágót akar, akkor ture return érték és "kiüríti" a táblát
bool visszavago(palya *map) {
    printf("Akarsz visszavagot? (I/N) ");
    char vissza = 'N';
    if(scanf(" %c", &vissza)!=1){
        printf("\nHibas bemeneti formatum!");
        exit(1);
    }
    vissza = toupper(vissza);
    if (vissza == 'I') {
        palyafree(map);
        palyafoglal(map);
        map->szabad = map->meret *map->meret;
        return true;
    }
    return false;
}