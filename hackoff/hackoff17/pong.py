#! /usr/bin/python

from socket import *

# On definit la taille maximale d'une ligne IRC recue
BUFSIZE = 1024

# On definit la fonction pong
def pong(sock):
	# On attend les donnees et on les stock
	data = sock.recv(BUFSIZE)
	# Est-ce que ca commence par "PING" ?
	if data[:4] == 'PING' :
		# Oui, alors faire mettre dans data
		# PONG suivi de ce qui suivait PING
		# Ne pas oublier le <RETURN>
		data = 'PONG' + `data[4:]` + '\n'
		# Et on envoie !
		sock.send(data)
