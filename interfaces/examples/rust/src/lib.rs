extern crate sinfourmis;

use sinfourmis::{FourmiAction, FourmiEtat, FourmiRetour, ReineAction, ReineRetour, Salle};

#[no_mangle]
pub extern "C" fn fourmi_activation(etat: *const FourmiEtat, salle: *const Salle) -> FourmiRetour {
    FourmiRetour {
        action: FourmiAction::DeposePheromone,
        arg: 10,
    }
}

#[no_mangle]
pub extern "C" fn reine_activation(
    fourmis_ptr: usize,
    nb_fourmis: usize,
    salle: *const Salle,
) -> ReineRetour {
    unsafe {
        let fourmis = std::slice::from_raw_parts(fourmis_ptr as *const FourmiEtat, nb_fourmis);
    }
    let mut mem = [0; 256];
    mem[0] = 1;
    ReineRetour {
        action: ReineAction::RecupererFourmi,
        memoire: mem,
    }
}
