R�union 25/11/2013

On ne va pas g�rer les architectures h�t�rog�nes.

G�rer les threads en lua?
Une trace par thread/processeur
Trace = acces aux coeurs
Configurer l'ordenancement des traces par coeur

L1d et L1i?
On ne fait rien sur le L1i

Caches inclusifs ou non?
Sur intel64, L3 inclusifs dans L2 et L1
L2 pas inclusif par rapport au L1

En cas de miss, on ne laisse pas la donn�e sur le cache non inclusif (en l'occurence L2)

Implementation en mode snooping bus ne sert pas � grand chose.
On peut supposer que le L3 contient toutes les donn�es

Protocoles de coherence:
MESI, MSI
MESIF, MUESI