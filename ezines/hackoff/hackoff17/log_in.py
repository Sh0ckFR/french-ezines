#! /usr/bin/python

# On importe toutes les fonctions de la librairie "socket"
from socket import *

# On definit la fonction log_in,
# qui prend le numero de socket "sock" en argument
def log_in(sock):
	# On envoie les donnees !
	sock.send("NICK Pybot\nUSER fred . . Python c'est coool !\n")
