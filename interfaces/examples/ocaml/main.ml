let fourmi_activation (etat : Sinfourmis.fourmi_etat) (salle : Sinfourmis.salle) : Sinfourmis.fourmi_retour =
  match Char.code etat.memoire.(0) with
  | 0 -> begin
    etat.memoire.(0) <- Char.chr 1;
    {action = DEPLACEMENT; arg = salle.degre - 1}
    end
  | 1 -> begin
    etat.memoire.(0) <- Char.chr 2;
    {action = RAMASSE_NOURRITURE; arg = -1}
    end
  | 2 -> begin
    etat.memoire.(0) <- Char.chr 3;
    {action = DEPOSE_PHEROMONE; arg = 128}
    end
  | 3 -> begin
    etat.memoire.(0) <- Char.chr 4;
    {action = COMMENCE_CONSTRUCTION; arg = -1}
    end
  | 4 -> begin
    etat.memoire.(0) <- Char.chr 5;
    {action = TERMINE_CONSTRUCTION; arg = -1}
    end
  | 5 -> begin
    etat.memoire.(0) <- Char.chr 6;
    {action = ATTAQUE; arg = 0} (* TODO: get a valid team value *)
    end
  | 6 -> begin
    etat.memoire.(0) <- Char.chr 0;
    {action = FOURMI_PASSE; arg = 10}
    end
  | _ -> failwith "unreachable"

let reine_activation (fourmis : Sinfourmis.fourmi_etat array) (reine : Sinfourmis.reine_etat) (salle : Sinfourmis.salle) : Sinfourmis.reine_retour =
  {action = REINE_PASSE; arg = 42}

let _ = Sinfourmis.register_functions fourmi_activation reine_activation

