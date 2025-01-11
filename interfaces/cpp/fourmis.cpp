#include "sinfourmis.h"
#include <iostream>

#define N_MEM_MOVES 85

typedef enum type_ouvrieres {
    BAT_ATTAQUE, // 0
    BAT_EXPLORA, // 1
    BAT_NOU_TRA, // 2
    BAT_NOU_TUN, // 3
    BAT_NOU_CRS, // 4
} type_ouvrieres_e;

type_ouvrieres_e int_to_type_fourmi(char n) {
    unsigned char mask = (unsigned char)224;
    unsigned char int_type = ((unsigned char)n) & mask;
    int_type = int_type >> 5;
    switch(int_type) {
        case 0 :
            return BAT_ATTAQUE;
        break;
        case 1 :
            return BAT_EXPLORA;
        break;
        case 2 :
            return BAT_NOU_TRA;
        break;
        case 3 :
            return BAT_NOU_TUN;
        break;
        case 4 :
            return BAT_NOU_CRS;
        break;
        default :
            std::cout << "Invalid fourmitype"; 
            free(NULL);
            return BAT_ATTAQUE;
    }
}

typedef struct infourmis {
    type_ouvrieres_e type_fourmi;
    char last_moves[N_MEM_MOVES];
    int i_moves;
} infourmis_t;

char get_half(int i, int offset, int mem[256]) {
    int i_tab = i/2;
    int half = i%2;
    unsigned char res = (unsigned char)mem[i_tab];
    unsigned char mask  = 15;
    if(half == 0) {
        res = res >> 4;
        res = res & mask;
    } else {
        res = res & mask;
    }
    return res;
}

void set_half(int i, char value, int mem[256]) {
    int i_tab = i/2;
    int half = i%2;
    unsigned char res = (unsigned char)mem[i_tab];
    
    if(half == 0) {
        unsigned char mask  = 15;
        res = res & mask;
        res = res ^ (value << 4);
    } else {
        unsigned char mask  = 240;
        res = res & mask;
        res = res ^ value;
    }
    mem[i_tab] = res;
}

char get_move(int i_move, char mem[256]) {
    int ind_start = i_move*6+4; // 3 bits for types, 1 of offset and 6 times the number of previous moves
    int d = ind_start/8;
    int r = ind_start%8;
    if(r < 3) {
        // All move is within a char
        unsigned char cur = (unsigned char)mem[d];
        cur = cur >> (2-r);
        unsigned char mask = (char)63;
        cur = cur & mask;
        return cur;
    } else {
        // The move is between two chars
        unsigned char half_big = (unsigned char)mem[d];
        unsigned char half_lil = (unsigned char)mem[d+1];
        unsigned char mask_big = (1 << (8-r)) - 1;
        int n = r-2;
        unsigned char mask_lil = ((1 << n) - 1) << (8-n);
        half_big = half_big & mask_big;
        half_lil = half_lil & mask_lil;
        unsigned char res;
        res = (half_big << (r-2)) + (half_lil >> n);
        return res;
    }
}

char get_i_move(char mem[256]) {

}

infourmis_t info_mem_ouvriere(char mem[256]) {
    infourmis_t res;
    // Récupère le type de fourmis
    res.type_fourmi = int_to_type_fourmi(mem[0]);
    // Récupère les mouvements
    for(int i = 0; i < N_MEM_MOVES; ++i) {
        res.last_moves[i] = get_move(i, mem);
    }
    // Récupère l'indice de mouvements
    res.i_moves = get_i_move(mem);
    /*
    switch(type_fourmi) {
        case BAT_ATTAQUE :
        break;
        case BAT_EXPLORA :
        break;
        case BAT_NOU_TRA :
        break;
        case BAT_NOU_TUN :
        break;
        case BAT_NOU_CRS :
        break;
    }*/
    return res;
}

reine_retour cpp_reine_activation(fourmi_etat fourmis[], const unsigned int nb_fourmis,
                                  const reine_etat *etat, const salle *salle) {
    return {.action = REINE_PASSE, .arg = 0};
}

fourmi_retour cpp_fourmi_activation(fourmi_etat *etat, const salle *salle) {
    return {.action = FOURMI_PASSE, .arg = 0, .depose_pheromone = NO_PHEROMONE, .pheromone = 0};
}

extern "C" {
reine_retour reine_activation(fourmi_etat fourmis[], const unsigned int nb_fourmis,
                              const reine_etat *etat, const salle *salle) {
    return cpp_reine_activation(fourmis, nb_fourmis, etat, salle);
}

fourmi_retour fourmi_activation(fourmi_etat *etat, const salle *salle) {
    return cpp_fourmi_activation(etat, salle);
}
}