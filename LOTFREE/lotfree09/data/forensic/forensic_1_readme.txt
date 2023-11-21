 _____                        _
|  ___|__  _ __ ___ _ __  ___(_) ___ 
| |_ / _ \| '__/ _ \ '_ \/ __| |/ __|
|  _| (_) | | |  __/ | | \__ \ | (__             _  _     _
|_|  \___/|_|  \___|_| |_|___/_|\___|          _| || |_  / |
                                              |_  ..  _| | |
  ____ _           _ _                        |_      _| | |
 / ___| |__   __ _| | | ___ _ __   __ _  ___    |_||_|   |_|
| |   | '_ \ / _` | | |/ _ \ '_ \ / _` |/ _ \
| |___| | | | (_| | | |  __/ | | | (_| |  __/
 \____|_| |_|\__,_|_|_|\___|_| |_|\__, |\___|
                                  |___/


Mise en situation
-----------------

Vous êtes Gilbert Descloux, expert dans le domaine de l'informatique forensic.
La société Chaussette Powaaa SA, un  des leaders mondiaux sur le marché de la
chaussette, a fait appel à vos services. Apparemment, ils suspectent un de leurs
employés de vendre des informations à la concurrence.

Voici le mail que le responsable informatique de Chaussette Powaaa SA vous a
envoyé :

--------------------------------------------------------------------------------
Subject: Demande d'analyse concernant le poste 042
From: Raoul Jugnot <raoul.jugnot@chaussette-powaaa.com>
Date: Thu, 29 Jul 2009 14:44:36 +0200
To: Gilbert Descloux <gilbert.descloux@hotmail.com>

Monsieur Descloux,

Suite à nos précédents mails, nous avons décidé de vous confier cette affaire.
Nous suspectons l'utilisateur du poste 042 de fournir des informations à nos
concurrents. C'est pourquoi, lundi, nous avons capturé le trafic réseau sortant
et entrant de son poste de travail. Nous avons également effectué un backup de
sa partition /home.

Votre travail sera de déterminer si effectivement il fait sortir certaines
informations de l'entreprise. Et si c'est le cas, trouvez à qui il a fournit
quels informations et comment.

Voici quelques informations concernant les fichier joints à ce mail :

- L'employé utilise le poste 042 dont l'adresse IP est 192.168.101.42
- Nos machines tournent sur Linux Debian Lenny
- Le système de fichier utilisé est l'ext3
- Nous utilisons un client mail propre à l'entreprise (chpowa-mailclient)
- Les mails sont stockés au format EML dans le dossier .chpowa-mail

En espérant que ces informations suffiront, je vous souhaite bonne chance. Tenez
nous au courant de votre avancement.

--
Raoul Jugnot
Responsable informatique
Chaussette Powaaa SA
--------------------------------------------------------------------------------


Votre objectif
--------------

Votre mission, si vous l'acceptez, sera d'aider ce pauvre Raoul. Il y a plus-
ieurs informations clés à découvrir. Chaque information vaut un certain nombre
de points en fonction de la difficulté à la trouver. Le but final est de savoir
quels documents ont étés vendus et ce qu'ils contenaient.

Pour ce faire vous disposez d'une trace pcap effectuée avec tcpdump et d'une
image de la partition home faite avec la commande dd.

Lorsque vous pensez avoir obtenu toutes les informations utiles ou que vous
n'arrivez plus à en obtenir d'autres, envoyez moi un mail avec ces informations
et votre nom ou pseudo. Je mettrai alors à jour votre score sur la page suivante

http://membres.lycos.fr/stockage01/forensic_challenge_1.html

Mon adresse e-mail : f a r g o t h  {ateu}  h o t m a i l  {pouin}  c o m
Indiquez "Forensic Challenge #1" comme sujet svp.

Bonne chance,

fargoth
