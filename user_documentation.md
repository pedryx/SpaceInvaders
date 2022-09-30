# Hra
Hra se odehrává ve vesmíru, hráč je reprezentován jakožto vesmírná loď. Cílem hry je odrazit útok nepřátelských mimozemšťanů. Mimozemšťani jsou seskupení v clusteru, který se pohybuje z jedné strany obrazovky na druhou. Pokaždé když se cluster dotkne kraje obrazovky, tak otočí směr svého pohybu a zároveň se trochu posune dolů. K pohybu nepřátel dochází v pravidelných intervalech, tento interval se postupně zrychluje (stejně tak se zvyšuje šance na výstřel projektilů). Hráč má 3 životy, pokud je zasažen projektilem ztrácí jeden život (počet životů hráče je zobrazen v pravém horním rohu obrazovky). Hráč se před projektily může schovat pod domečky, kteří jsou složeny z bloků. Pokud projektil zasáhne blok tak ho zničí. Hráč má také možnost střílet, vystřelit může pouze každých 800ms. Pokud projektil vystřelený hráčem  zasáhne nepřítele, tak je nepřítel zničen a hráči se připočte 10 skóre. Aktuální počet skóre je zobrazen v levém horním rohu. Ve hře je také speciální typ nepřítele – Ufo, tento nepřítel má malou šanci na to, že se objeví a pokud se objeví, tak letí z pravé horní části obrazovky do levé horní (po výletu z obrazovky zmizí). Pokud se hráči povede zasáhnout Ufo, získává 50 skóre. 

# Konec hry
Hra končí ve chvíli, kdy hráč ztratil všechny životy – dochází k prohře, nebo když hráč zničí všechny nepřátele – dochází k výhře. 

# Ovladaní
- A, D - Hrac se pohybuje do leva/prava.
- Mezerník - Pokud hráč může (nevystřelil v posledních 800ms projektil), tak vystřelí projektil.