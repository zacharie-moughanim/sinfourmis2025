# Interface Python

Il n'y a pas d'interface particulière pour Python. Il faut seulement implémenter
les fonctions suivantes dans un package Python (un dossier contenant un fichier
`__init__.py`):

```python
fourmi_activation(fourmi_etat, salle)

reine_activation(fourmis, reine_etat, salle)
```

Dans le fichier fournis, les fonctions `reine_retour` et `fourmi_retour` permettent de renvoyer les valeurs de retour de ces fonctions correctement formatées.

Les enum sont utilisables comme des enum C et sont comparables avec des entiers. Par exemple, pour vérifier que vous êtes dans une salle qui contient une reine, vous pouvez faire:

```python
if salle.type == SalleType.REINE:
    print("Il y a une reine dans cette salle")
```
