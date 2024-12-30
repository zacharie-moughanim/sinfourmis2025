# Interface OCaml

Ce répertoire définit une template de projet `dune` qui vous permet d'interfacer
avec le simulateur de cette édition de Sinfourmis.

Vous pouvez copier ce répertoire pour commencer votre projet.

Il vous faudra implémenter deux fonctions avec les interfaces suivantes (le nom
est donné à titre indicatif) :

```ocaml
open Sinfourmis
val fourmi_activation : fourmi_etat -> salle -> fourmi_retour
val reine_activation  : fourmi_etat array -> salle -> reine_retour
```

Puis il faudra indiquer à l'interface l'existence de ces fonctions avec le code suivant

```ocaml
let _ =
  Sinfourmis.register_functions fourmi_activation reine_activation
```

Vous pourrez trouver un exemple [ici](../examples/ocaml/)
