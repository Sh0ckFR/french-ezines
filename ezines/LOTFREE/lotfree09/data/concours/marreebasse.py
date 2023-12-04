# LOTFREE
import time
import urllib
import httplib2
import random
import sys
import os

score = 6957
user = 1363

head = {"User-Agent" : "Mozilla/5.0 (Windows NT 6.1; rv:9.0) Gecko/20100101 Firefox/9.0",
        'Content-type': 'application/x-www-form-urlencoded'}

cnx = httplib2.Http()

# Ouverture de la session
response, content = cnx.request("http://www.marreebasse.com/scripts/submit.php",
                            'POST', headers = head, body = "user_id=%d" % (user))

cookie = response["set-cookie"].split(";")[0]
head["Cookie"] = cookie
print cookie

# Envoi du score
response, content = cnx.request("http://www.marreebasse.com/scripts/submit.php",
                            'POST', headers = head, body = "score=%d" % (score))

# Lecture captcha
response, content = cnx.request("http://www.marreebasse.com/scripts/captcha.php",
                            'GET', headers = head)
fd = open("captcha.jpg", "wb")
fd.write(content)
fd.close()

os.startfile("captcha.jpg")
captcha = raw_input("Code: ").strip()

d = {'captcha' : captcha,
     'email' : 'hacker@plop.com',
     'score' : score,
     'pseudo' : 'hacker',
     'lastname' : 'HACKER',
     'firstname' : 'Leet'}
    
# Conformation score + captcha
response, content = cnx.request("http://www.marreebasse.com/scripts/valider.php",
                            'POST', headers = head, body = urllib.urlencode(d))

print content
