#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "pvejatek.h"
#include "hatter.h"

//p1 az O, szamitogep az X

void init(palya *map){
    //minden mezőt -1-re állítunk
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 20; ++j) {
            map->prioritas[i][j] = -1;
        }
    }
    //a ténylegesen játéktéren lévő mezőket 1-re állítjuk
    for (int i = 0; i < map->meret; ++i) {
        for (int j = 0; j < map->meret; ++j) {
            map->prioritas[i][j] = 1;
        }
    }
}

bool pveszabad(palya *map, int x, int y) {
    if (x >= map->meret || y >= map->meret){
        return false;
    }
    if (x < 0 || y < 0){
        return false;
    }

    if (map->hely[x][y] != 0) {
        return false;
    }
    return true;
}

void pvekelet(palya *map,char jatekoskarakter){
    int sor;
    //sorok
    for (int i = 0; i < map->meret; i++) {
        //oszlopok
        for (int j = 0; j < map->meret; j++) {
            //ha talál jatekoskarakter-t
            if(map->hely[i][j] == jatekoskarakter){
                //megszámolja hány jatekoskarakter van vízszintesen egy sorban
                sor = 0;
                bool vanelem=true;
                while(vanelem){
                    if(!(j+sor >= map->meret)){       //ha túlindexelne álljon le a keresés
                        if(map->hely[i][j+sor] == jatekoskarakter) sor++;
                        else vanelem = false;
                    }
                    else vanelem = false;
                }
                //az elemek sora előtti és utáni elemek prioritását növeljük meg a sor hosszával
                if(j>0) {map->prioritas[i][j-1] +=100/(map->gyozelem-sor);}
                map->prioritas[i][j+sor] +=100/(map->gyozelem-sor);
            }
        }
    }
}

void pvedelkelet(palya *map,char jatekoskarakter) {
    int sor;
    //sorok
    for (int i = 0; i < map->meret; i++) {
        //oszlopok
        for (int j = 0; j < map->meret; j++) {
            //ha talál jatekoskarakter-t
            if (map->hely[i][j] == jatekoskarakter) {
                //megszámolja hány jatekoskarakter van átlósan lefelé egy sorban
                sor = 0;
                bool vanelem=true;
                while(vanelem) {
                    if (!(i + sor >= map->meret || j + sor >= map->meret)) {    //ha túlindexelne álljon le a keresés
                        if (map->hely[i + sor][j + sor] == jatekoskarakter) sor++;
                        else vanelem = false;
                    }
                    else vanelem = false;
                }
                 //az elemek sora előtti és utáni elemek prioritását növeljük meg a sor hosszával
                 if (j > 0 && i > 0) {map->prioritas[i - 1][j - 1] += 100/(map->gyozelem-sor);}
                 map->prioritas[i + sor][j + sor] += 100/(map->gyozelem-sor);
            }
        }
    }
}


void pvedel(palya *map,char jatekoskarakter){
    int sor;
    //sorok
    for (int i = 0; i < map->meret; i++) {
        //oszlopok
        for (int j = 0; j < map->meret; j++) {
            //ha talál jatekoskarakter-t
            if(map->hely[i][j] == jatekoskarakter){
                //megszámolja hány jatekoskarakter van vízszintesen egy sorban
                sor = 0;
                bool vanelem=true;
                while(vanelem) {
                    if(!(i+sor >= map->meret)){       //ha túlindexelne álljon le a keresés
                        if(map->hely[i+sor][j] == jatekoskarakter) sor++;
                        else vanelem = false;
                    }
                    else vanelem = false;
                }
                //az elemek sora előtti és utáni elemek prioritását növeljük meg a sor hosszával
                if(i>0) map->prioritas[i-1][j] +=100/(map->gyozelem-sor);
                map->prioritas[i+sor][j] +=100/(map->gyozelem-sor);
            }
        }
    }
}

void pvedelnyugat(palya *map,char jatekoskarakter){
    int sor;
    //sorok
    for (int i = 0; i < map->meret; i++) {
        //oszlopok
        for (int j = 0; j < map->meret; j++) {
            //ha talál jatekoskarakter-t
            if(map->hely[i][j] == jatekoskarakter){
                //megszámolja hány jatekoskarakter van vízszintesen egy sorban
                sor = 0;
                bool vanelem=true;
                while(vanelem) {
                    if (!(j-sor < 0 || i+sor >= map->meret)) { //ha túlindexelne álljon le a keresés
                        if (map->hely[i + sor][j - sor] == jatekoskarakter) sor++;
                        else vanelem = false;
                    }
                    else vanelem = false;
                }

                //az elemek sora előtti és utáni elemek prioritását növeljük meg a sor hosszával
                if(i>0) map->prioritas[i-1][j+1] +=100/(map->gyozelem-sor);
                map->prioritas[i+sor][j-sor] +=100/(map->gyozelem-sor);
            }
        }
    }
}


legjobb elemzo(palya *map){
    //tiszta lap minden lépésnél
    init(map);
    //prioritásokat beállítjuk
    pvekelet(map,'O');
    pvedelkelet(map,'O');
    pvedel(map,'O');
    pvedelnyugat(map,'O');

    pvekelet(map,'X');
    pvedelkelet(map,'X');
    pvedel(map,'X');
    pvedelnyugat(map,'X');

    //foglalt mezőket nullázzuk
    for (int i = 0; i < map->meret; ++i) {
        for (int j = 0; j < map->meret; ++j) {
            if(!pveszabad(map,i,j)) map->prioritas[i][j] = 0;
        }
    }
    //prioritások legnagyobb megkeres, lerak
    legjobb lep={0,0,0};
    for (int i = 0; i < map->meret; ++i) {
        for (int j = 0; j < map->meret; ++j) {
            if(map->prioritas[i][j] > lep.ertek) {
                lep.x = i;
                lep.y = j;
                lep.ertek = map->prioritas[i][j];
            }
        }
    }
    return lep;

}
//bool mert meg tudja hívni a jatekkilep-et
bool pve_beolvas_es_rak(palya *map, legjobb lep){
    if(map->soron == 2){
        place_x(map,lep.x,lep.y);
        return false;
    }
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
        place_o(map,x,y);
    }
}
void pve(palya *map){
    legjobb lepes;
    bool kilep=false;
    while (vege(map) == -1 && !kilep){
        palyakiir(map);
        if(map->soron == 1){
            printf("\nO jon (jatekos)");
            printf("\nKilepes a fomenube (K) \nLepes koordinataval (oszlop+sor) pl A1: ");
        }
        else{
            printf("\nX jon (szamitogep)");
            lepes = elemzo(map);
        }

        kilep = pve_beolvas_es_rak(map,lepes);
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



