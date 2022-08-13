# Projet-Arduino

## Matériels : 
* Afficheur LCD
* Capteur de température TMP36
* Photorésistance GL5528
* Buzzer
* Led 
* Bouton poussoir
* Divers compsants (résistances, fils, potar, etc)

## Fonctionnalités : 
* Un bouton poussoir permet d'activer/désactiver le mode
surveillance. Le mode est visible sur l'afficheur.
* Un capteur donne la température qui est affichée en
dégré celsius
* Un capteur permet d'obtenir l'intensité lumineuse qui est
affiché en lux
* L'afficheur donne l'heure en Heures : Minutes
* L'heure, dont la gestion doit être la plus précise possible,
est initialisée par communication série depuis le PC
* La led intégrée à la carte clignote très exactement toutes
les secondes (1 seconde allumée, 1 seconde éteinte)
Lorsque le mode surveillance est activé, si la
température descend en-dessous du seuil bas,
on émet un son aigu et un son grave quand il
dépasse le seuil haut (seuils haut et bas à
choisir lors des tests)
* Lorsque le mode surveillance est activé, une
led (qui symbolise une lumière) s'allume de
manière proportionnelle à l'obscurité détectée
(plus il fait sombre, plus elle est éclairée)

## Auteurs

* [Maxime DESGRANGES](https://github.com/Rogha-Max)
* [Yohann BREUIL](https://github.com/DJYohann)