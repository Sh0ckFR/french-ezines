#include <idc.idc>				// Bibliothèque de fonctions

static main() {					// Point d'entrée du script
       auto J,address,count;			// Déclaration des variables
       address = 0x401042;			// Adresse de début de décryptage
							// Décrypte 49 octets
       for (count = 0x31; count>0; count--) {
           J = Byte(address);			// Récupère la valeur de l'octet à l'adresse courante
           J = J + count;				// Ajoute la valeur du compteur de boucle
           J = J ^ 0xD2;				// Xor 0D2h
           PatchByte(address,J);		// Patche l'adresse avec l'octet décrypté
           address++;				// Octet suivant
       }
       Message("Layer décrypté\n");
}
