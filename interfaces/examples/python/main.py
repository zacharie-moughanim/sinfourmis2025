from enum import IntEnum, unique

@unique
class SalleType(IntEnum):
    VIDE = 0
    EAU = 1
    NOURRITURE = 2
    REINE = 3

@unique
class FourmiAction(IntEnum):
    DEPLACEMENT = 0
    RAMASSE_NOURRITURE = 1
    COMMENCE_CONSTRUCTION = 2
    TERMINE_CONSTRUCTION = 3
    ATTAQUE = 4
    ATTAQUE_TUNNEL = 5
    FOURMI_PASSE = 6

@unique
class ReineAction(IntEnum):
    CREER_FOURMI = 0
    ENVOYER_FOURMI = 1
    RECUPERER_FOURMI = 2
    AMELIORE_STOCKAGE = 3
    AMELIORE_PRODUCTION = 4
    AMELIORE_ENVOI = 5
    AMELIORE_VITESSE_AMELIORATION = 6
    AMELIORE_RAMASSAGE = 7
    AMELIORE_VIE = 8
    AMELIORE_EAU = 9
    AMELIORE_DEGATS = 10
    REINE_PASSE = 11

def fourmi_retour(action, arg, depose_pheromone = False, pheromone = 0):
    return {
        "action": action.value,
        "arg": arg,
        "depose_pheromone": depose_pheromone,
        "pheromone": pheromone
    }
    
def reine_retour(action, arg):
    return {
        "action": action.value,
        "arg": arg,
    }

def fourmi_activation(fourmi_etat, salle):
    fourmi_etat["memoire"][0] += 1
    if fourmi_etat["memoire"][0] == 1:
        print("Fourmi: Je suis une fourmi")
    else:
        print("Fourmi: Je suis une fourmi et je sais compter")
    
    return fourmi_retour(FourmiAction.FOURMI_PASSE, 0)

nb_sent = 0
def reine_activation(fourmis, reine_etat, salle):   
    global nb_sent
    print("Sent: ", nb_sent) 
    if nb_sent > 2:
        return reine_retour(ReineAction.REINE_PASSE, 0)
    

    if len(fourmis) < 1:
        print("Création d'une fourmis")
        return reine_retour(ReineAction.CREER_FOURMI, 1)
    
    for i in range(256):
        fourmis[0]["memoire"][i] = i
     
    print("Envoi d'une fourmi")
    nb_sent += 1
    return reine_retour(ReineAction.ENVOYER_FOURMI, 1)
