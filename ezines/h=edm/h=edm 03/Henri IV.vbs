do 
reponse = inputbox("De quelle couleur �tait le cheval blanc d'Henri IV? (blanc, noir ou violet)")
select case reponse 
case "blanche"msgbox("Bah oui c'�tait une jument ( c ; "),vbinformation
exit do
case "blanc"msgbox("He he faut pas se fier aux rumeurs ! "),vbcritical
case "noir"msgbox("Arf alors la t'es vraiment le roi des d�biles ( t'inqui�tes pas �a se soigne ( c ; )"),vbcritical
case "violet"msgbox("Ah ah ah toi t'es t'es vraiment un bon un vrai : des gars qui voient des chevaux violets c'est vraiment pas courant. Je crois qu il y a pas de rem�des pour toi. ) c :"),vbcritical
Case else msgbox("He charlot je t'apprendrai � taper sur ton clavier"),vbexclamation
End select
loop