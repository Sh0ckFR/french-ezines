#include <idc.idc>				// Biblioth�que de fonctions

static main() {					// Point d'entr�e du script
       auto J,address,count;			// D�claration des variables
       address = 0x401042;			// Adresse de d�but de d�cryptage
							// D�crypte 49 octets
       for (count = 0x31; count>0; count--) {
           J = Byte(address);			// R�cup�re la valeur de l'octet � l'adresse courante
           J = J + count;				// Ajoute la valeur du compteur de boucle
           J = J ^ 0xD2;				// Xor 0D2h
           PatchByte(address,J);		// Patche l'adresse avec l'octet d�crypt�
           address++;				// Octet suivant
       }
       Message("Layer d�crypt�\n");
}
