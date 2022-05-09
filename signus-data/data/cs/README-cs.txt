Signus: The Artefact Wars
=========================

https://github.com/signus-game/signus

Signus je klasická česká válečná tahová strategie podobná známé sérii Battle
Isle. Původní komerční verze vyšla v roce 1998. Autoři pak zdrojové kódy i
data hry vydali jako open source v roce 2002.


Ovládání
--------

- Levé tlačítko myši: Výběr jednotky, zadávání příkazů
- Pravé tlačítko myši: Přepínání akcí

Nezapomeňte, že některé akce lze vybrat jen pokud myší ukazujete na vhodné
políčko. Například doplnění munice nebo paliva lze vybrat jen pokud ukazujete
na nějakou jednotku.

- F1: Rezervovat čas na 1 výstřel
- F2: Rezervovat čas na 2 výstřely
- F3: Rezervovat čas na 3 výstřely
- F5: Otevřít dialog pro načtení hry
- F6: Otevřít dialog pro uložení hry
- F10: Otevřít herní menu
- Tab: Přepnout na další zbraň jednotky
- Alt+Enter: Přepnout na celou obrazovku a zpět (bez změny rozlišení obrazovky)
- C: Vycentrovat vybranou jednotku
- D: Zapnout/vypnout ukazatel stavu jednotek
- E: Ukončit tah
- I: Otevřít popis jednotky
- M: Označit aktuální jednotku do konce tahu jako vyřízenou a vybrat další
     nevyřízenou jednotku
- N: Vybrat další jednotku
- 1: Přepnout akci na výběr jednotky
- 2: Přepnout akci na pohyb
- 3: Přepnout akci na útok
- 4: Přepnout akci na speciální akci jednotky


Nastavení hry
-------------

Nastavení hry lze měnit během mise z menu v postranním panelu nebo stisknutím
klávesy F10.

- Rychlost skrolování, rychlost pohybu jednotek, rychlost animací jednotek:
  Tyto posuvníky nastavují rychlost posouvání mapy a animovaných akcí
  jednotek. Vyšší hodnota znamená rychlejší pohyb. Nejvyšší rychlost závisí
  na výkonu vašeho počítače, takže pro největší pohodlí si tyto hodnoty musíte
  vyladit sami.

- Rychlost zobrazení nápovědy:
  Tento posuvník nastavuje prodlevu, po které se po najetí myší na tlačítko
  nebo herní objekt objeví jeho popisek. Vyšší hodnota znamená kratší prodlevu.

- Celá obrazovka:
  Toto zaškrtávátko přepíná zobrazení herního okna na celou obrazovku.

- Po spuštění přehrát intro:
  Toto zaškrtávátko přepíná přehrávání úvodních animací a první poloviny intra
  po každém spuštění Signusu. Druhá polovina intra, která se přehrává
  při zahájení nové hry, se přehraje bez ohledu na toto nastavení.

- Prostředí pro rychlé počítače:
  Toto zaškrtávátko přepíná přehrávání různých animací herních oken. Je to
  čistě kosmetické nastavení bez vlivu na hratelnost.

- Jednotka se zastaví při objevení nepřítele:
  Volba chování vašich jednotek ve hře, která usnadňuje bezpečný průzkum mapy.


Nastavení herních vylepšení
---------------------------

Tlačítko "Vylepšení" v nastaveních hry otevře další konfigurační dialog
s volbami, které mají dopad na hratelnost a nebyly součástí původní komerční
verze hry. Tyto volby jsou rozděleny na opravy chyb a volitelná vylepšení.

Opravy chyb řeší drobné problémy v herní logice, které byly zjevně neúmyslné,
ale jejich opravou se citelně mění hratelnost oproti původní komerční verzi.
Volitelná vylepšení řeší některé zvláště otravné vlastnosti hry a trochu
snižují celkovou obtížnost, ale po jejich zapnutí by měla být hra zábavnější.

Doporučené (a výchozí) nastavení je zapnout všechny opravy chyb a vypnout
všechna volitelná vylepšení. Pokud se chcete co nejvíce přiblížit hernímu
zážitku z původní komerční verze, vypněte všechny volby v tomto dialogu včetně
oprav chyb.


Hlášení chyb
------------

Pokud ve hře objevíte chybu, nahlašte ji v GitHub projektu:
https://github.com/signus-game/signus/issues

Pokud hra spadne v průběhu mise, můžete k hlášení o chybě přiložit i záchranný
soubor, ze kterého jde obnovit poslední tah před pádem. Najdete ho
v konfiguračním adresáři Signusu:
- Na Windows: %APPDATA%\signus\crashguard_saved_state
- Na Linuxu: ~/.signus/crashguard_saved_state

Pozor: Když Signus po pádu znovu spustíte a zvolíte možnost "Hlavní menu" nebo
"Obnovit", záchranný soubor se automaticky vymaže (pokud obnovení hry neskončí
dalším pádem). Když chcete nahlásit chybu a Signus se vás při spuštění zeptá,
jestli obnovit předchozí hru, tak pro zachování záchranného souboru zvolte
možnost "Ukončit hru."
