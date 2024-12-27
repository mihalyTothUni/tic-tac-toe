#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include "debugmalloc.h"
#include "hatter.h"
#include "pvpjatek.h"
#include "pvejatek.h"


//főmenü
int main() {
    palya map;
    map.hely=NULL;
    setbuf(stdout, 0);
    char valaszt;
    bool kilepo=false;
    while(!kilepo) {
        printf("Mit szeretnel?\nUj jatek inditasa (U)\nMentett jatek folytatasa (F)\nKilepes (K)\n");
        if(scanf(" %c", &valaszt)!=1){
            printf("\nHibas bemeneti formatum!");
            exit(1);
        }
        valaszt = toupper(valaszt);
        switch (valaszt) {
            // új játék indítása
            case 'U':
                map=ujjatek();
                do{
                    if(map.pvp)
                        pvp(&map);
                    else
                        pve(&map);

                }while(vege(&map)!= -1 && visszavago(&map)); //kihasználja az && logikai rövidzár tulajdonságát
                palyafree(&map);
                break;
            //mentett játék folytatása
            case 'F':
                map = palyabetolt();
                if(map.hely != NULL) {
                    do{
                        if(map.pvp)
                            pvp(&map);
                        else
                            pve(&map);

                    }while(vege(&map)!= -1 && visszavago(&map)); //kihasználja az && logikai rövidzár tulajdonságát
                }
                palyafree(&map);
                break;
            //kilépés
            case 'K':
                kilepo=menukilep();
                break;
            default:
                break;
        }
    }
    return 0;
}
