# LOTFREE
import time
import urllib
import httplib2

email = "hacker@plop.com"
score = 31337

h = time.localtime().tm_hour
m = time.localtime().tm_min

s = ""
for c in email[:5]:
    s += str(ord(c))

k = int(s) + score
k *= 42

k += h + m
print "%d:%d" % (h, m)
print "key:", k

d = {'email' : email,
    'key': str(k),
    'score' : str(score),
    'onLoad' : '[type Function]'}
print urllib.urlencode(d)

head = {"User-Agent" : "Opera/9.80 (Windows NT 5.1; U; fr) Presto/2.9.168 Version/11.52",
        'Content-type': 'application/x-www-form-urlencoded'}

cnx = httplib2.Http()
response, content = cnx.request("http://www.toplaclasse.com/pages/jeu-position.html",
                            'POST', headers=head, body=urllib.urlencode({"score": str(score)}))
print content

head["cookie"] = response["set-cookie"].split(";")[0]
print head["cookie"]

response, content = cnx.request("http://www.toplaclasse.com/pages/jeu-record.html",
                            'POST', headers=head, body=urllib.urlencode(d))
print content
