

#ifndef NHF_HATTER_H
#define NHF_HATTER_H



//struktúra az adatok tárolására
typedef struct palya {
    char **hely;        //pointer a dinamikus két dimenziós tömbre
    int meret;          //pálya mérete
    int gyozelem;       //gyózelem feltétele
    bool pvp;           //játékos ellen játszik-e a játékos (ha false -> gép ellen)
    int soron;          //p1 vagy p2 következik soron
    int szabad;         //hány szabad mező van még a pályán
    int prioritas[20][20]; //pve játék esetén használja a program a lépés meghatáráozásához
}palya;


bool menukilep();
void palyafoglal(palya *map);
void palyafree(const palya *map);
palya ujjatek();
void palyakiir(const palya *map);
void palyament(const palya *map);
palya palyabetolt();


#endif //NHF_HATTER_H
