# LOTFREE
import hashlib
import httplib2
import sys
import re
import urllib

# On demande les infos a envoyer sur la console
email = raw_input("Email: ")
passwd = raw_input("Password: ")
score = raw_input("Score: ")

if not score.isdigit():
    print "Le score doit etre un chiffre!!!"
    sys.exit()

if int(score) > 8000:
    print "score trop grand!"
    sys.exit()

navigateur = 'Mozilla/5.0 (X11; U; Linux x86_64; en-US) Chrome/7.0.517.44 Safari/534.7'
h = {}
h["Accept"] = "application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5"
h["User-Agent"] = navigateur
h["Accept-Language"] = "fr-FR,fr;q=0.8,en-US;q=0.6,en;q=0.4"
h["Accept-Charset"] = "ISO-8859-1,utf-8;q=0.7,*;q=0.3"
h["Referer"] = "http://www.cornedbeef.fr/jeu/"

# Obtenir le formulaire de login + cookie
cnx = httplib2.Http()
resp, data = cnx.request("http://www.cornedbeef.fr/beef/login", headers=h)

cookie_name = resp["set-cookie"].split("=")[0]
cookie = resp["set-cookie"].split(";")[0] + "; mt_redirect=true"
h["Cookie"] = cookie
print
print "Connecting..."
print "Cookie:", h["Cookie"]
print


start = data.find('form_build_id" id="') + 19
end = data.find('"', start)
form_id = data[start:end]
print "Found form_id", form_id

# Identification
h["Content-type"] = "application/x-www-form-urlencoded"
h["Referer"] = "http://www.cornedbeef.fr/beef/login"
req =  "name=" + urllib.quote_plus(email)
req += "&pass=" + urllib.quote_plus(passwd)
req += "&remember_me=1&form_build_id=" + form_id + "&form_id=user_login&op=connecter"
resp, data = cnx.request("http://www.cornedbeef.fr/beef/login", "POST", body = req, headers = h)
new_val = re.findall(r"=([a-f\d]{32});", resp["set-cookie"])[0]

cookie = cookie_name + "=" + new_val + "; mt_redirect=true"
print "Login in..."
print "New cookie:", cookie

h["Cookie"] = cookie

start = data.find("<status>") + 8
end = data.find("</status>", start)
if data[start:end] == "0":
    print "Erreur de login!"

start = data.find("<id>") + 4
end = data.find("</id>", start)
id = data[start:end]

start = data.find("<prenom>") + 8
end = data.find("</prenom>", start)
firstname = data[start:end]

start = data.find("<nom>") + 5
end = data.find("</nom>", start)
lastname = data[start:end]

print "Logged in with userid %s (%s %s)" % (id, firstname, lastname)


text = score + ":" + firstname + ":" + id
h.pop("Referer")
hacked_id = hashlib.md5(text).hexdigest()
req = "score=%s&lastname=%s&steak=%s&id=%s&firstname=%s" % \
        (score, lastname, id, hacked_id, firstname)
print "Generating score ID", hacked_id

print "Resultats:"
h["Content-type"] = "application/x-www-form-urlencoded"
resp, data = cnx.request("http://www.cornedbeef.fr/jeu/php/envoiscore.php", "POST", req, headers = h)
print data
h.pop("Content-type")

# Affichage resultat
resp, data = cnx.request("http://www.cornedbeef.fr/jeu/php/userinfo.php?steak=" + id, headers = h)
print data

resp, data = cnx.request("http://www.cornedbeef.fr/jeu/php/top5.php", headers = h)
print data
