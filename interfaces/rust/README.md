# Interface rust

Ce répertoire définit une crate rust qui permet d'interfacer avec le simulateur
de cette édition de Sinfourmis.

Pour l'utiliser, il vous faut créer un nouveau projet rust avec `cargo new NOM --lib`.

Le fichier `Cargo.toml` doit ensuite ressembler à ça :

```toml
[package]
name = "NOM"

[lib]
crate-type = ["cdylib"] # Creates dynamic lib

[dependencies]
sinfourmis = { git = "https://gitlab.ens-rennes.fr/sinfonie/sinfourmis2025" }
```

Puis, votre projet doit définir deux fonctions avec les interfaces suivantes :

```rust
/// fonction d'activation des fourmis
///
/// etat: l'état de la fourmi au moment de l'activation
/// salle: l'état de la salle dans laquelle se trouve la fourmi
///
/// renvoie l'action à effectuer par la fourmi
#[no_mangle]
pub extern "C" fn fourmi_activation(etat: *const FourmiEtat, salle: *const Salle) -> FourmiRetour {
    // VOTRE CODE ICI
}

/// fonction d'activation de la reine
///
/// fourmis: si l'action précédente était un ramassage de fourmis, contient les
/// états des fourmis ramassées
/// nb_fourmis: nombre de fourmis ramassées
/// salle: l'état de la salle dans laquelle se trouve la reine
///
/// renvoie l'action à effectuer par la reine
#[no_mangle]
pub extern "C" fn reine_activation(fourmis_ptr: usize, nb_fourmis: usize, salle: *const Salle) -> ReineRetour {
    unsafe {
        let fourmis = std::slice::from_raw_parts(fourmis_ptr as *const FourmiEtat, nb_fourmis);
    }
    // VOTRE CODE ICI
}
```

Vous pourrez trouver un exemple [ici](../examples/rust/)
