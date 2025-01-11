#include "sinfourmis.h"
#include <iostream>

typedef enum type_ouvrieres {
    BAT_ATTAQUE, // 0
    BAT_EXPLORA, // 1
    BAT_NOU_TRA, // 2
    BAT_NOU_TUN, // 3
    BAT_NOU_CRS, // 4
} type_ouvrieres_e;

type_ouvrieres_e int_to_type_fourmi(char n) {
    switch(n) {
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
    char last_moves[85];
    int i_moves;
} infourmis_t;

infourmis_t info_mem_ouvriere(char mem[256]) {
    infourmis_t res;
    // Récupère le type de fourmis

    // Récupère les mouvements

    // Récupère l'indice de mouvements
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