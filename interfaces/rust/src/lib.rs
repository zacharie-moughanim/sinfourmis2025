/// décrit le type d'une salle
#[repr(C)]
pub enum SalleType {
    Vide,
    Eau,
    Nourriture,
}

/// décrit l'action effectuée par une fourmi
#[repr(C)]
pub enum FourmiAction {
    Deplacement,
    RamasseNourriture,
    DeposePheromone,
    CommenceConstruction,
    TermineConstruction,
    Attaque,
    AttaqueTunnel,
    Passe,
}

/// décrit l'action effectuée par la reine
#[repr(C)]
pub enum ReineAction {
    CreerFourmi,
    EnvouerFourmi,
    RecupererFourmi,
    AmelioreReine,
    AmelioreEnvoi,
    AmelioreRamassage,
    AmelioreVie,
    AmelioreEau,
    AmelioreDegats,
    Passe,
}

/// structure représentant une salle
#[repr(C)]
pub struct Salle {
    pub salle_type: SalleType,
    pub pheromone: u8,
    /// nombre de salles voisines
    pub degre: i32,
}

/// structure représentant l'état d'une fourmi
///
/// Cette structure est utilisée lors de l'éveil d'une fourmi. Le champ `memoire`
/// n'est jamais modifié entre deux activations.
#[repr(C)]
pub struct FourmiEtat {
    pub vie: u8,
    pub memoire: [u8; 256],
    pub result: i32,
    /// valeur dela réserve d'eau (en nombre de tours)
    pub eau: i32,
    pub nourriture: i32,
}

/// structure décrivant une action de fourmi
///
/// Cette structure est renvoyée par la fonction d'activation des fourmis. Elle
/// décrit le type d'action réalisé à la fin du tour, et contient l'argument de
/// l'action correspondante (pas toujours nécessaire)
#[repr(C)]
pub struct FourmiRetour {
    pub action: FourmiAction,
    pub arg: i32,
}

/// structure décrivant une action de la reine
///
/// Cette structure est renvoyée par la fonction d'activation de la reine. Elle
/// décrit le type d'action réalisé à la fin du tour, et contient la mémoire
/// initiale de la fourmi à envoyer (si l'action est un envoi de fourmi)
#[repr(C)]
pub struct ReineRetour {
    pub action: ReineAction,
    pub memoire: [u8; 256],
}
