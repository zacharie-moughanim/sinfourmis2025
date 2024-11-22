type salle_type =
  | VIDE
  | EAU
  | NOURRITURE

type fourmi_action =
  | DEPLACEMENT
  | RAMASSE_NOURRITURE
  | DEPOSE_PHEROMONE
  | COMMENCE_CONSTRUCTION
  | TERMINE_CONSTRUCTION
  | ATTAQUE
  | FOURMI_PASSE

type reine_action =
  | CREER_FOURMI
  | ENVOYER_FOURMI
  | RECUPERER_FOURMI
  | AMELIORE_REINE
  | AMELIORE_ENVOI
  | AMELIORE_RAMMASSAGE
  | AMELIORE_VIE
  | AMELIORE_EAU
  | AMELIORE_DEGATS
  | REINE_PASSE

type salle = {
  salle_type : salle_type;
  pheromone : int
}

type fourmi_etat = {
  vie : int;
  memoire : char array;
  result : int
}

type fourmi_retour = {
  action : fourmi_action;
  arg : int
}

type reine_retour = {
  action : reine_action;
  memoire : char array
}

(*
module type Interface = sig
  val fourmi_activation : fourmi_etat -> salle -> fourmi_retour

  val reine_activation : fourmi_etat list -> salle -> reine_retour
end
*)

let register_functions fourmi_activation reine_activation =
  Callback.register "fourmi_activation" fourmi_activation;
  Callback.register "reine_activation" reine_activation
