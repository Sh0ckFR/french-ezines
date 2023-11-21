# LOTFREE
import httplib2
import sys
import re
import urllib
import random
from threading import Thread
import time
import BeautifulSoup
import socks

PHPSESSID = ""
ok = 1

navigateur = 'Mozilla/5.0 (X11; U; Linux x86_64; en-US) Chrome/7.0.517.44 Safari/534.7'
h = {}
h["Accept"] = "application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5"
h["User-Agent"] = navigateur
h["Accept-Language"] = "fr-FR,fr;q=0.8,en-US;q=0.6,en;q=0.4"
h["Accept-Charset"] = "ISO-8859-1,utf-8;q=0.7,*;q=0.3"
h["Accept-Encoding"] = "identity"

cnx = httplib2.Http()

# Un thread qui gere la page de ping
class Ping(Thread):
    def __init__(self):
        Thread.__init__(self)
        
    def run(self):
        head = h.copy()
        while True:
            t = str(int(time.time() * 1000))
            ping_url = "http://www.super.com/php/ping.php?PHPSESSID=" + PHPSESSID
            ping_url += "&t=" + t
            head["Cookie"] = "PHPSESSID=" + PHPSESSID
            resp, data = cnx.request(ping_url, headers=head)
            print ping_url
            if data.strip() != "":
                print "ping response:", data.strip()
            time.sleep(30)
            if ok == 0:
                break
        

# Page des jeux qui nous renvoie un cookie + formulaire login
resp, data = cnx.request("http://www.hyper.com/portail/accueil_jeux", headers=h)

JSESSIONID = resp["set-cookie"].split("=")[1].split(';')[0]
h["Cookie"] = "JSESSIONID=" + JSESSIONID


d = {}
soup = BeautifulSoup.BeautifulSoup(data)
form_url = soup.form["action"]
for input in soup.form.findAll("input"):
    if input["type"] == "image":
        continue
    if input["name"].endswith("login}"):
        d[input["name"]] = "hacker"
    elif input["name"].endswith("password}"):
        d[input["name"]] = "c0ncours"

# Envoi des identifiants
h["Content-type"] = "application/x-www-form-urlencoded"
h["Referer"] = "http://www.hyper.com/portail/accueil_jeux"
resp, data = cnx.request(form_url, "POST", body = urllib.urlencode(d), headers = h)
soup = BeautifulSoup.BeautifulSoup(data)

h.pop("Content-type")
h.pop("Cookie")

# Le jeu en lui meme est joue dans une iframe
resp, data = cnx.request(soup.iframe["src"], headers = h)
PHPSESSID = resp["content-location"].split("=")[1]
h["Cookie"] = "PHPSESSID=" + PHPSESSID

resp, data = cnx.request("http://www.super.com/?PHPSESSID=" + PHPSESSID, headers = h)

# On declare un debut de partie
h.pop("Referer")
url = "http://www.super.com/php/dispatch.php?PHPSESSID=" + PHPSESSID
url += "&service=init&alea=" + str(random.randint(1000,11000))
resp, data = cnx.request(url, headers = h)

# Si notre login est dans la page, la cnx a fonctionne
if data.find("HACKER") > 1:
    print "Logged in :)"
else:
    print "Login error"
    sys.exit()
    

form_url = "http://www.super.com/php/dispatch.php?PHPSESSID=" + PHPSESSID
form_url += "&service=jeu&alea=" + str(random.randint(1000,11000))
h["Content-type"] = "application/x-www-form-urlencoded"
resp, data = cnx.request(form_url, "POST", body = "id%5Fjeu=13", headers = h)

soup = BeautifulSoup.BeautifulSoup(data)
id_partie = soup.infos.id_partie.next
print "id partie:", id_partie
mess_err = soup.infos.mess_err.next.strip()
if mess_err != "":
    print "!",mess_err,"!"
    sys.exit()

# On lance notre thread de ping
ping = Ping()
ping.start()

# On attend pour similer une partie en cours. Pendant ce temps ca ping.
time.sleep(160)

# On declare la fin de partie avec notre score
form_url = "http://www.super.com/php/dispatch.php?PHPSESSID=" + PHPSESSID
form_url += "&service=finjeu&alea=" + str(random.randint(1000,11000))
req = "data=0&id%5Fpartie=" + id_partie + "&score=5000"
resp, data = cnx.request(form_url, "POST", body = req, headers = h)
print data

ok = 0
