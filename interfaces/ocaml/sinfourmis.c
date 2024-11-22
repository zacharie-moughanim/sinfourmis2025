#include <assert.h>
#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <string.h>

#include "sinfourmis.h"

/// Converts a caml value to a fourmi_action enum
fourmi_action val_to_fourmi_action(value val) {
    switch (Int_val(val)) {
        case 0:
            return DEPLACEMENT;
        case 1:
            return RAMASSE_NOURRITURE;
        case 2:
            return DEPOSE_PHEROMONE;
        case 3:
            return COMMENCE_CONSTRUCTION;
        case 4:
            return TERMINE_CONSTRUCTION;
        case 5:
            return ATTAQUE;
        case 6:
            return FOURMI_PASSE;
        default:
            fprintf(stderr,
                    "val_to_fourmi_action error: expected integer value between 0 and 6, got %d",
                    Int_val(val));
            return FOURMI_PASSE;
    }
}

/// Converts a caml value to a fourmi_retour struct
fourmi_retour val_to_fourmi_retour(value val) {
    value action_val = Field(val, 0);
    fourmi_retour result = {val_to_fourmi_action(action_val), Int32_val(Field(val, 1))};
    return result;
}

/// Converts a fourmi_etat struct to a caml value
value fourmi_etat_to_val(const fourmi_etat *etat) {
    value res = caml_alloc(4, 0);

    value vie = Val_int(etat->vie);
    Store_field(res, 0, vie);

    value memoire = caml_alloc_initialized_string(256, etat->memoire);
    Store_field(res, 1, memoire);

    value result = Val_int(etat->result);
    Store_field(res, 2, result);

    value eau = Val_int(etat->eau);
    Store_field(res, 3, result);

    return res;
}

/// Converts a salle struct to a caml value
value salle_to_val(const salle *salle) {
    value res = caml_alloc(3, 0);

    value type = Val_int(salle->type);
    Store_field(res, 0, type);

    value pheromone = Val_int(salle->pheromone);
    Store_field(res, 1, pheromone);

    value degre = Val_int(salle->degre);
    Store_field(res, 2, pheromone);

    return res;
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

fourmi_retour fourmi_activation(const fourmi_etat *etat, const salle *salle) {
    __caml_init();
    CAMLparam0();

    static const value *_fourmi_activation = NULL;
    if (!_fourmi_activation)
        _fourmi_activation = caml_named_value("fourmi_activation");

    CAMLlocal1(result);
    // TODO: convert arguments to caml value
    // maybe use callback2_exn ?
    result = caml_callback2(*_fourmi_activation, fourmi_etat_to_val(etat), salle_to_val(salle));
    assert(Tag_val(result) == 0); // 0 is the tag of a record

    CAMLreturnT(fourmi_retour, val_to_fourmi_retour(result));
}
