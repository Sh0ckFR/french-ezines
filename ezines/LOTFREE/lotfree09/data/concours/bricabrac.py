# LOTFREE
import httplib
import time
import re
import urllib
import sys
import random

navigateurs = ['Opera/9.80 (Windows NT 5.1; U; fr) Presto/2.6.30 Version/10.63',
                'Mozilla/5.0 (X11; U; Linux x86_64; en-US) AppleWebKit/534.7 (KHTML, like Gecko) Chrome/7.0.517.44 Safari/534.7',
                'Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.0; Trident/4.0; GTB6.6; SLCC1; .NET CLR 2.0.50727; Media Center PC 5.0;',
                'Mozilla/5.0 (Linux; U; Android 2.2; en-gb; Nexus One Build/FRF50)',
                'Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; MyIE2; InfoPath.2)',
                'Mozilla/5.0 (Windows; U; MSIE 9.0; WIndows NT 9.0; en-US)',
                'Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Avant Browser; .NET CLR 2.0.50727; MAXTHON 2.0)',
                'Mozilla/5.0 (Windows; U; Windows NT 5.1; fr; rv:1.9.1.8) Gecko/20100202 Firefox/3.5.8']

base_headers = {}
base_headers["Accept"] = "application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5"
base_headers["User-Agent"] = random.choice(navigateurs)
base_headers["Accept-Language"] = "fr-FR,fr;q=0.8,en-US;q=0.6,en;q=0.4"
base_headers["Accept-Charset"] = "ISO-8859-1,utf-8;q=0.7,*;q=0.3"

msg = {'G':'Gagne', 'P':'Perdu', 'N':'Nul'}

while True:
    # Selection d'un des perso propose au hazard
    personnage = random.choice(['nounours', 'magicien', 'rambo', 'oussama', 'bernadette'])
    cnx = httplib.HTTPConnection("www.concours-flash.fr")
    h = base_headers.copy()
    # Premiere connexion au site pour obtenir un cookie
    h["Referer"] = "http://www.bricabrac.com/concours/go.html"
    cnx.request("GET", "/bricabrac/index.php", headers = h)
    resp = cnx.getresponse()
    data = resp.read()
    cookie = resp.getheader("Set-Cookie", "").split(';')[0].strip()
    h["Cookie"] = cookie

    # On reduit le contenu de la page au formulaire
    start = data.find('id="formulaire"')
    end = data.find('</form>', start)
    form = data[start+1:end]

    start = form.find('name="key"')
    end = form.find(" />", start)
    # Les noms des champs etant generes aleatoirement on doit les recuperer
    # Heureusement la presence du type des champs (text et password) nous aide
    # et puis ils ressemblent a des hashs... donc un ptit coup de regex et c ok
    try:
      key = re.findall(r"([a-f\d]{32})", form[start:end])[0]
    except IndexError:
      continue

    start = form.find('type="text"', end)
    end = form.find(" />", start)
    try:
      email = re.findall(r"([a-f\d]{32})", form[start:end])[0]
    except IndexError:
      continue

    start = form.find('type="password"', end)
    end = form.find(" />", start)
    try:
      password = re.findall(r"([a-f\d]{32})", form[start:end])[0]
    except IndexError:
        continue

    # Maintenant qu'on a les champs, on envoi le formulaire
    d = {'key': key, 'action' : 'login', 'formulaire' : '2', email : 'hacker@plop.com', password : 'h4ck3r'}

    h["Referer"] = "http://www.concours-flash.fr/bricabrac/index.php"
    h["Content-Type"] = "application/x-www-form-urlencoded"
    cnx.request("POST", "/bricabrac/index.php", urllib.urlencode(d), headers = h)
    resp = cnx.getresponse()
    data = resp.read()

    # Page de transition
    h.pop("Content-Type")
    cnx.request("GET", "/bricabrac/index.php", headers = h)
    resp = cnx.getresponse()
    data = resp.read()

    # La encore cle aleatoire a extraire
    end = data.find('</form>"')
    start = data.rfind('name="key', 0, end)
    try:
      key = re.findall(r"([a-f\d]{32})", data[start:end])[0]
    except IndexError:
      continue
    d = { 'key' : key}

    h["Content-Type"] = "application/x-www-form-urlencoded"
    cnx.request("POST", "/bricabrac/index.php", urllib.urlencode(d), headers = h)
    resp = cnx.getresponse()
    data = resp.read()

    # Page de chargement du jeu Flash avec param timestamp
    h.pop("Content-Type")
    cnx.request("GET", "/bricabrac/index.php", headers = h)
    resp = cnx.getresponse()
    data = resp.read()

    start = data.find('value="timestamp=', end)
    end = data.find('&', start)
    timestamp = re.findall(r"([\d]{10})", data[start:end])[0]
    print "Timestamp:",timestamp


    cnx2 = httplib.HTTPConnection("www.concours-flash.fr")
    cnx2.request("GET", "/bricabrac/flash/jeu.swf", headers = h)
    resp = cnx2.getresponse()
    resp.read(50)
    cnx2.close()
    time.sleep(1)

    for x in range(0,15):
      for coup in range(1, 4):

        req = "perso=%s&var%%5Fcoup=%s&action=insert" % (personnage, coup)
        cnx.request("POST", "/bricabrac/coup.php", req, headers = h)
        resp = cnx.getresponse()
        data = resp.read()
        print "Round", coup, ":", data.split("=")[1]
        cnx.close()

        time.sleep(5.5)

        # Gagnant, perdant ou nul ?
        r = random.choice("GGGNNP")
        print msg[r]
        d = {'timestamp' : timestamp, 'perso' : personnage, 'resultat' : r, 'action' : 'update'}
        cnx = httplib.HTTPConnection("www.concours-flash.fr")
        cnx.request("POST", "/bricabrac/coup.php", urllib.urlencode(d), headers = h)
        try:
          resp = cnx.getresponse()
          data = resp.read()
        except httplib.BadStatusLine:
          print(":(")
          pass


      cnx.request("POST", "/bricabrac/victoire.php", "action=nombre%5Fvictoire", headers = h)
      resp = cnx.getresponse()
      data = resp.read()
      print "Nombre de victoires:", data.split("=")[1]

      cnx.request("POST", "/bricabrac/flashvars.php", "action=reload", headers = h)
      resp = cnx.getresponse()
      data = resp.read()
      if random.choice("GGGNNP") == "G":
        start = data.find("{") + 1
        end = data.rfind("}") - 1
        print "\n".join(data[start:end].split("}"))
      print
    cnx.close()

