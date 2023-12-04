email=inputbox("Veuillez entrer votre adresse E-mail.")
If instr(email,"@")<2 then
msgbox"Ceci n'est pas une adresse E-mail valide."
Else
msgbox"Merci."
end if
