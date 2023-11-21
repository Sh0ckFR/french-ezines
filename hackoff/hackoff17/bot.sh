#!/bin/sh -x

# Fichier temporaire ou l'on stocke ce qu'il faut envoyer au serveur
INPUT=/tmp/Zebot$$

# On met dans INPUT les donnes de LogIn
echo "USER zebot x y :Le bot Zero !
NICK Zebot" > $INPUT

# tail -f fait une boucle infinie : il attend les donnes dans INPUT
# ces donnees partent dans un telnet sur le serveur IRC
tail -f $INPUT | telnet tibob.via.ecp.fr 6667 | \
# Du telnet, il ressort ce que renvoie le serveur
# On lit ligne a ligne
while read LINE
do
  # On met chaque mot de la ligne dans $1, $2, etc...
  set $LINE
  # Si le premier mot est $1
  case "$1" in
    # Alors on envoie un PONG a l'envoyeur ($2)
    "PING" )    echo "PONG $2";;
  esac
# Ce qui sort de cette procedure (les "echos") est redirige vers INPUT 
done >> $INPUT

# On efface le fichier temporaire.
rm -f $INPUT
