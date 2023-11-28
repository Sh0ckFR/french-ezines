# LOTFREE
# -*- coding: utf-8 -*-
import httplib2
import random
import urllib
import sys
import re
import time

h = {'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64; rv:8.0) Gecko/20100101 Firefox/8.0'}

http = httplib2.Http()
response, content = http.request("http://www.crakobon.fr/", 'GET', headers=h)
h['Cookie'] = response['set-cookie'].split(";")[0]
print "Got session cookie", h["Cookie"]

h["Content-Type"] = "application/x-www-form-urlencoded"

def httpurl(d):
  l = []
  for k, v in d.items():
    if isinstance(k, unicode):
      k = k.encode("utf-8")
    if isinstance(v, unicode):
      v = v.encode("utf-8")
    k = urllib.quote(k)
    v = urllib.quote(v)
    l.append(k + "=" + v)
  return "&".join(l)


def make_req(service, params=None):
  url = "http://www.crakobon.fr/dispatch.php?%s&service=%s&alea=%s" % (h['Cookie'], service, str(random.randrange(2000,11000)))
  if service == "init":
    method = "GET"
    response, content = http.request(url, 'GET', headers=h)
  else:
    method = "POST"
    response, content = http.request(url, 'POST', headers=h, body=httpurl(params))
  return content

make_req("init")
make_req("tracking", {"page": "Start"})
make_req("tracking", {"page": "Home"})
buff = make_req("login", {'email': 'hacker@plop.com', 'pass': 'p4ssw0rd'}) # doit renvoyer le login et <parties_restant>X</parties_restant> 
if buff.find("hacker") < 0:
  print "Login error"
  sys.exit()
r1 = re.compile('<parties_restant>(.*?)</parties_restant>')
nb_parties = int(r1.search(buff).group(1))
print "Login ok"

if not nb_parties:
  print "Plus de parties pour aujourd'hui !"
  sys.exit()
print nb_parties, "parties a jouer."

make_req("user_info", {"pseudo": "hacker"})

r2 = re.compile('<id_partie>(.*?)</id_partie>')
i = 0
# On joue nos parties gagnantes
while nb_parties > 0:
  make_req("tracking", {"page": "Fight- Round 3"})

  buff = make_req("debut_partie", {"type": "IA"}) # renvoit <id_partie>X</id_partie>
  id_partie = r2.search(buff).group(1)

  make_req("tracking", {"page": "Jeu"})
  buff = make_req("fin_partie", {"winner": "Y", "id_partie": id_partie})  # renvoit <parties_restant>X</parties_restant>
  nb_parties =  int(r1.search(buff).group(1))

  make_req("tracking", {"page": u"Victory"})
  i += 1

# On enregistre les resultats
fd = open("/tmp/log_crakobon", "a")
fd.write("%s: %s parties jouees\n" % (time.asctime(), str(i)))
fd.close()
