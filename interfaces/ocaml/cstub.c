#include <assert.h>
#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <string.h>

#include "sinfourmis.h"

/// Converts a caml value to a fourmi_action enum
fourmi_action val_to_fourmi_action(value val) {
    CAMLparam1(val);
    switch (Int_val(val)) {
        case 0:
            CAMLreturn(DEPLACEMENT);
        case 1:
            CAMLreturn(RAMASSE_NOURRITURE);
        case 2:
            CAMLreturn(DEPOSE_PHEROMONE);
        case 3:
            CAMLreturn(COMMENCE_CONSTRUCTION);
        case 4:
            CAMLreturn(TERMINE_CONSTRUCTION);
        case 5:
            CAMLreturn(ATTAQUE);
        case 6:
            CAMLreturn(FOURMI_PASSE);
        default:
            fprintf(stderr,
                    "val_to_fourmi_action error: expected integer value between 0 and 6, got %d",
                    Int_val(val));
            CAMLreturn(FOURMI_PASSE);
    }
}

/// Converts a caml value to a reine_action enum
reine_action val_to_reine_action(value val) {
    CAMLparam1(val);
    switch (Int_val(val)) {
        case 0:
            CAMLreturn(CREER_FOURMI);
        case 1:
            CAMLreturn(ENVOYER_FOURMI);
        case 2:
            CAMLreturn(RECUPERER_FOURMI);
        case 3:
            CAMLreturn(AMELIORE_REINE);
        case 4:
            CAMLreturn(AMELIORE_ENVOI);
        case 5:
            CAMLreturn(AMELIORE_RAMASSAGE);
        case 6:
            CAMLreturn(AMELIORE_VIE);
        case 7:
            CAMLreturn(AMELIORE_EAU);
        case 8:
            CAMLreturn(AMELIORE_DEGATS);
        case 9:
            CAMLreturn(REINE_PASSE);
        default:
            fprintf(stderr,
                    "val_to_reine_action error: expected integer value between 0 and 9, got %d",
                    Int_val(val));
            CAMLreturn(REINE_PASSE);
    }
}

/// Converts a caml value to a fourmi_retour struct
fourmi_retour val_to_fourmi_retour(value val) {
    CAMLparam1(val);
    fourmi_retour result = {val_to_fourmi_action(Field(val, 0)), Int_val(Field(val, 1))};
    CAMLreturnT(fourmi_retour, result);
}

/// converts a caml value to a reine_retour struct
reine_retour val_to_reine_retour(value val) {
    CAMLparam1(val);
    reine_retour result;
    result.action = val_to_reine_action(Field(val, 0));
    for (size_t i = 0; i < 256; i++) {
        result.memoire[i] = Int_val(Field(Field(val, 1), i));
    }
    CAMLreturnT(reine_retour, result);
}

/// Converts a fourmi_etat struct to a caml value
value fourmi_etat_to_val(const fourmi_etat *etat) {
    CAMLparam0();
    CAMLlocal5(res, vie, memoire, result, eau);
    res = caml_alloc(4, 0);

    vie = Val_int(etat->vie);
    Store_field(res, 0, vie);

    memoire = caml_alloc(256, 0);
    for (size_t i = 0; i < 256; i++) {
        Store_field(memoire, i, Val_int(etat->memoire[i]));
    }
    Store_field(res, 1, memoire);

    result = Val_int(etat->result);
    Store_field(res, 2, result);

    eau = Val_int(etat->eau);
    Store_field(res, 3, result);

    CAMLreturn(res);
}

/// Converts a salle struct to a caml value
value salle_to_val(const salle *salle) {
    CAMLparam0();
    CAMLlocal4(res, type, pheromone, degre);
    res = caml_alloc(3, 0);

    type = Val_int(salle->type);
    Store_field(res, 0, type);

    pheromone = Val_int(salle->pheromone);
    Store_field(res, 1, pheromone);

    degre = Val_int(salle->degre);
    Store_field(res, 2, degre);

    CAMLreturn(res);
}

void __caml_init() {
    // Or pthread_once if you need it threadsafe.
    static int once = 0;

    if (once == 0) {
        // Fake an argv by prentending we're an executable `./ocaml_startup`.
        char *argv[] = {"ocaml_startup", NULL};

        // Initialize the OCaml runtime
        caml_startup(argv);

        once = 1;
    }
}

fourmi_retour fourmi_activation(fourmi_etat *etat, const salle *salle) {
    __caml_init();
    CAMLparam0();

    static const value *_fourmi_activation = NULL;
    if (!_fourmi_activation)
        _fourmi_activation = caml_named_value("fourmi_activation");

    CAMLlocal3(result, val_etat, val_salle);
    val_etat = fourmi_etat_to_val(etat);
    val_salle = salle_to_val(salle);

    result = caml_callback2(*_fourmi_activation, val_etat, val_salle);
    assert(Tag_val(result) == 0); // 0 is the tag of a record
    assert(Wosize_val(result) == 2);

    // write back etat->memoire because it is currently stored in val_etat
    for (size_t i = 0; i < 256; i++) {
        etat->memoire[i] = Int_val(Field(Field(val_etat, 1), i));
    }

    CAMLreturnT(fourmi_retour, val_to_fourmi_retour(result));
}

reine_retour reine_activation(const fourmi_etat fourmis[], size_t nb_fourmis, const salle *salle) {
    __caml_init();
    CAMLparam0();

    static const value *_reine_activation = NULL;
    if (!_reine_activation)
        _reine_activation = caml_named_value("reine_activation");

    CAMLlocal3(result, val_etats, val_salle);
    val_etats = caml_alloc(nb_fourmis, 0);
    for (size_t i = 0; i < nb_fourmis; i++) {
        Store_field(val_etats, i, fourmi_etat_to_val(&fourmis[i]));
    }
    val_salle = salle_to_val(salle);

    result = caml_callback2(*_reine_activation, val_etats, val_salle);
    assert(Tag_val(result) == 0);
    assert(Wosize_val(result) == 2);

    CAMLreturnT(reine_retour, val_to_reine_retour(result));
}
