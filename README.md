# :bomb: 3D Minesweeper
Svima dobro poznata igrica, igrana uglavnom kada nema interneta :cry:. Spas od dosade, a opet dovoljno zanimljiva da joj se uvek vratimo i pokušavamo da oborimo sopstveni rekord.
## Pravila i komande
Cilj je otvoriti sva polja pritom ne "gazeći" na minu. Na svakom otvorenom polju ispisan je broj mina oko tog polja. Moguće je obeležavanje polja za koja mislite da su mine time će se ona obojiti u crvenu boju. Takođe, omogućeno je da se pritiskom na otvoreno polje otvore sva susedna polja kako bi se ubrzalo rešavanje slagalice. Koristite pažljivo jer ukoliko ipak ima mina u okolini izgubili ste.

|  Taster|Komanda  |
|--------|---------|
| <kbd>w</kbd> <kbd>W</kbd> |Pomeranje na gore|
| <kbd>s</kbd> <kbd>S</kbd> |Pomeranje na dole|
| <kbd>a</kbd> <kbd>A</kbd> |Pomeranje ulevo|
| <kbd>d</kbd> <kbd>D</kbd> |Pomeranje udesno|
|<kbd>Space</kbd> | Otvaranje polja |
|<kbd>x</kbd> <kbd>X</kbd>| Obeležavanje polja / Brisanje obeleženog |
|<kbd>&uarr;</kbd> | Pomeranje kamere na gore|
|<kbd>&darr;</kbd> |Pomeranje kamere na dole|
|<kbd>&larr;</kbd> |  Pomeranje kamere ulevo|
|<kbd>&rarr;</kbd> | Pomeranje kamere udesno|
| <kbd>i</kbd> <kbd>I</kbd> |Zoom in|
| <kbd>o</kbd> <kbd>O</kbd> |Zoom out|
| <kbd>r</kbd> <kbd>R</kbd> |Restartovanje igre|
| <kbd>1</kbd> |Begginer|
| <kbd>2</kbd> |Intermediate|
| <kbd>3</kbd> |Expert|
|<kbd>Esc</kbd> |Izlaz iz igre|

| Nivo | Širina | Visina | Broj mina |
|------|--------|--------|-----------|
| Beginner | 9 | 9 | 10 |
| Intermediate | 16 | 16 | 40 |
| Expert | 30 | 16 | 99 |

## Instalacija / pokretanje
* ``` git clone https://github.com/MATF-RG18/RG163-3d-minesweeper ```
* ``` cd RG163-3d-minesweeper ```
* ``` cmake . ```
* ``` make ```
* ``` ./main ```
