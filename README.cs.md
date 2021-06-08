# Signus: The Artefact Wars

## Potřebné knihovny

- libSDL 2.0.5
- SDL2\_mixer (pro přehrávání hudby je navíc potřeba libmikmod)
- SDL2\_ttf
- SDL2\_image
- libvorbis
- libjpeg
- libpng

## Instalace

Naklonujte si tento Git repozitář a zkompilujte podadresáře `signus` a `signus-data`:

    cd signus
    ./bootstrap
    ./configure
    make
    make install

    cd signus-data
    ./bootstrap
    ./configure
    make
    make install

## Ovládání

- Levé tlačítko myši: Výběr jednotky, zadávání příkazů
- Pravé tlačítko myši: Přepínání akcí

Nezapomeňte, že některé akce lze vybrat jen pokud myší ukazujete na vhodné políčko. Například doplnění munice nebo paliva lze vybrat jen pokud ukazujete na nějakou jednotku.

- F1: Rezervovat čas na 1 výstřel
- F2: Rezervovat čas na 2 výstřely
- F3: Rezervovat čas na 3 výstřely
- F5: Otevřít dialog pro načtení hry
- F6: Otevřít dialog pro uložení hry
- F10: Otevřít herní menu
- Tab: Přepnout na další zbraň jednotky
- Alt+Enter: Přepnout na celou obrazovku a zpět (bez změny rozlišení obrazovky)
- C: Vycentrovat vybranou jednotku
- D: Zapnout/vypnout ukazatel stovu jednotek
- E: Ukončit tah
- I: Otevřít popis jednotky
- M: Označit aktuální jednotku do konce tahu jako vyřízenou a vybrat další nevyřízenou jednotku
- N: Vybrat další jednotku
- 1: Přepnout akci na výběr jednotky
- 2: Přepnout akci na pohyb
- 3: Přepnout akci na útok
- 4: Přepnout akci na speciální akci jednotky
