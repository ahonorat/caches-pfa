Compte rendu s�ance 27/11/2013

Utiliser le format .md pour les comptes rendus

Faire des "override": notion d'include
On inclut dans notre fichier de config, les confs de hwloc

Si valeur dans xml on prend valeur, sinon dans config par d�fault, sinon en dur

D�finir un "contrat" pour les objets, les pointeurs de fonction, comme flags, flags_new_line...

Flags_new_line -> set_flags_new_line: pour plus de clart� dans le code

Rajouter des fonctions, par ex � la place de current->cache->writes_back++
Avec des macros cela peut etre efficace
Cela �vite de faire des setters incompr�hensibles partout

G�n�rateur d'automates: ragel
G�n�re des fichiers .c
Permet de mettre des actions sur des transitions / des �tats...
Permet d'abstraire un peu plus

Ajouter un pointeur de fonction dans share_level � la place de modify
