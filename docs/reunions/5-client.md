R�union du 16/12/2013

Dans le cahier des charges, il faudrait ajouter:
- ex de trace
- ex de fichier xml de hwloc
- ex de fichier xml transform�

Pr�ciser les diff�rentes politiques d'entrelacement des threads:
- 1 par un
- thread par thread
- x d'un thread, y d'un autre etc...

Une execution doit pouvoir �tre rejou�e.
Si introduction d'al�atoire -> plutot du pseudo al�atoire reproductible.

Quand un L1 fait un miss:
      soit snooping sur le bus avec les autres L1
      soit L2 demande aux autres L1 et on continue de meme avec le L3

L1i pas g�nant car utile seulement au d�but de l'execution du programme (code compact)

Un cache inclusif l'est pour tous les caches en dessous.

Quand on �crit, on remet � jour dans les caches de niveau sup�rieur. Pour nous, on change juste le flag modified et les flages update.

Avec un cache non inclusif, faire du WB n'est plus obligatoire -> int�ret de l'�tat owned.

L'�tat F de MESIF est utile au niveau des bus, il ne sert donc � rien de le consid�rer dans notre �tude.

