

#ifndef NHF_PVPJATEK_H
#define NHF_PVPJATEK_H
#include "hatter.h"


bool szabad(palya *map,int x, int y);
void place_x(palya *map,int x, int y);
void place_o(palya *map,int x, int y);
char playertochar(palya *map);

void randomkezd(palya *map);
bool jatekkilep(palya *map);

bool nyert(palya *map);
bool betelt(palya *map);
int vege(palya *map);
void pvp(palya *map);
bool visszavago(palya *map);



#endif //NHF_PVPJATEK_H
