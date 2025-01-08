#pragma once

#ifdef __cplusplus
// disable name mangling
extern "C" {
#endif
#include <stdint.h>
#include <stdlib.h>

/// décrit le type d'une salle
typedef enum salle_type { VIDE, EAU, NOURRITURE, REINE } salle_type;

/// décrit l'action effectuée par une fourmi
typedef enum fourmi_action {
    DEPLACEMENT,
    RAMASSE_NOURRITURE,
    COMMENCE_CONSTRUCTION,
    TERMINE_CONSTRUCTION,
    ATTAQUE,
    ATTAQUE_TUNNEL,
    FOURMI_PASSE
} fourmi_action;

/// décrit l'action effectuée par la reine
typedef enum reine_action {
    CREER_FOURMI,
    ENVOYER_FOURMI,
    RECUPERER_FOURMI,
    AMELIORE_STOCKAGE,
    AMELIORE_PRODUCTION,
    AMELIORE_ENVOI,
    AMELIORE_VITESSE_AMELIORATION,
    AMELIORE_RAMASSAGE,
    AMELIORE_VIE,
    AMELIORE_EAU,
    AMELIORE_DEGATS,
    REINE_PASSE
} reine_action;

/// structure représentant une salle
typedef struct salle {
    salle_type type;
    uint8_t pheromone;
    int32_t degre; // nombre de salles voisines
                   // TODO add etat des arêtes (pour construction)
} salle;

/// structure représentant l'état d'une fourmi
///
/// Cette structure est utilisée lors de l'éveil d'une fourmi. Le champ `memoire`
/// n'est jamais modifié entre deux activations.
typedef struct fourmi_etat {
    uint8_t vie;
    char memoire[256]; // on va peut-être modifier la taille de ce tableau
    int32_t result; // prend une valeur qui va dépendre de l'action prédemment réalisée (une valeur
                    // négative correspond à une erreur)
    int32_t eau; // valeur de la réserve d'eau (en nombre de tours)
    int32_t nourriture;
} fourmi_etat;

/// structure décrivant une action de fourmi
///
/// Cette structure est renvoyée par la fonction d'activation des fourmis. Elle
/// décrit le type d'action réalisé à la fin du tour, et contient l'argument de
/// l'action correspondante (pas toujours nécessaire)
typedef struct fourmi_retour {
    fourmi_action action;
    int32_t arg; // utilisé en argument d'action, type pas gravé dans le marbre pour l'instant
    bool depose_pheromone;
    uint8_t pheromone;
} fourmi_retour;

/// structure représentant l'état d'une reine
///
/// Cette structure est utilisée lors de l'éveil d'une reine.
/// Elle contient les fourmis gérées par la reine, les points de nourriture
/// et le résultat de l'action précédente
typedef struct reine_etat {
    uint32_t nourriture;
    int32_t result;
    uint32_t max_nourriture;
    uint32_t max_eau;
    uint32_t max_vie;
    uint32_t max_degats;
    uint32_t duree_amelioration;
    uint32_t max_stockage;
    uint32_t max_production;
    uint32_t max_envoi;
} reine_etat;

/// structure décrivant une action de la reine
///
/// Cette structure est renvoyée par la fonction d'activation de la reine. Elle
/// décrit le type d'action réalisé à la fin du tour, et contient la mémoire
/// initiale de la fourmi à envoyer (si l'action est un envoi de fourmi)
typedef struct reine_retour {
    reine_action action;
    int32_t arg;
} reine_retour;

/// fonction d'activation de la reine
///
/// fourmis: si l'action précédente était un ramassage de fourmis, contient les
/// états des fourmis ramassées
/// nb_fourmis: nombre de fourmis ramassées
/// salle: l'état de la salle dans laquelle se trouve la reine
///
/// renvoie l'action à effectuer par la reine
reine_retour reine_activation(fourmi_etat fourmis[], const unsigned int nb_fourmis,
                              const reine_etat *etat, const salle *salle);

/// fonction d'activation des fourmis
///
/// etat: l'état de la fourmi au moment de l'activation
/// salle: l'état de la salle dans laquelle se trouve la fourmi
///
/// renvoie l'action à effectuer par la fourmi
fourmi_retour fourmi_activation(fourmi_etat *etat, const salle *salle);
#ifdef __cplusplus
}
#endif
