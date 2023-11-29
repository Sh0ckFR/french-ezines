do 
reponse = inputbox("De quelle couleur était le cheval blanc d'Henri IV? (blanc, noir ou violet)")
select case reponse 
case "blanche"msgbox("Bah oui c'était une jument ( c ; "),vbinformation
exit do
case "blanc"msgbox("He he faut pas se fier aux rumeurs ! "),vbcritical
case "noir"msgbox("Arf alors la t'es vraiment le roi des débiles ( t'inquiètes pas ça se soigne ( c ; )"),vbcritical
case "violet"msgbox("Ah ah ah toi t'es t'es vraiment un bon un vrai : des gars qui voient des chevaux violets c'est vraiment pas courant. Je crois qu il y a pas de remèdes pour toi. ) c :"),vbcritical
Case else msgbox("He charlot je t'apprendrai à taper sur ton clavier"),vbexclamation
End select
loop