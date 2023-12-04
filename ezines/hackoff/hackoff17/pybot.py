#! /usr/bin/python

from socket import *

BUFSIZE = 1024

def connect():

	host = 'tibob.via.ecp.fr'
        hostaddr = gethostbyname(host)
	sock = socket(AF_INET, SOCK_STREAM)
	sock.connect(host, 6667)
	return sock

def log_in(sock):

	data = "NICK Pybot\nUSER fred . . Python c'est coool !\n"
	sock.send(data)

def pong(sock):
	data = sock.recv(BUFSIZE)
	print data
	if data[:4] == 'PING' :
		data = 'PONG' + `data[4:]` + '\n'
		sock.send(data)
		print 'sent', data

def join(sock, channel):
	data = 'join ' + channel + '\n'
	sock.send(data)
	print 'sent', data
	
def main():
	print 'Connecting...\n'
	sock = connect()
	print 'Logging in...\n'
	log_in(sock)
	print 'Joining #toto....\n'
	channel = '#toto'
	join(sock, channel)
	print 'Done.'
	while 1:
		pong(sock)

# On execute main, qui est la fonction ci-dessus, et qui va appeler toutes 
# les autres fonctions
main()
