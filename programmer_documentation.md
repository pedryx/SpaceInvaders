# Trida Game
Hra je reprezentovaná třídou game, která obsahuje dvě důležité metody: *Game::update* a *Game::render*. Program začíná zavoláním metody *Game::run*, která obsahuje nekonečný cyklus, ve kterém se v každé iteraci nejprve obslouží události okna a poté se zavolají metody *Game::update* a *Game::render*. Metoda *Game::update* se stará o aktualizaci herní logiky a metoda *Game::render* o vykreslování. Třída *Game* obsahuje také metodu *Game::exit*, po jejímž zavolání dojde na konci současné iterace herního cyklu k ukončení hry.

# Herní stavy
Celá hra je rozdělena na herní šťávy, každý herní stav představuje jednu "obrazovku". Hra může mít aktivní v jednu chvíli pouze jeden herní stav. Pro změnu herního stavu slouží metoda *Game::setState*, která jako argument přijímá *std::shared_ptr* na nový aktivní herní stav. Každý herní stav je reprezentován třídou, která je potomkem třídy *GameState*.

Třída *GameState* obsahuje abstraktní metodu *GameState::init*, která slouží inicializací herního stavu. Potomci střídy *GameState* si zde vytvoří např. entity, případně provedou jiné věci pro přípravu herního stavu. Každý herní stav obsahuje kolekci entit (viz Entity). Entity lze do herního stavu přidávat pomocí *GameState::addEntity*, která přijímá *std::shared_ptr* na potomka třídy *Entity* jako argument. Entity je možné odebírat pomocí *GameState::removeEntity*, která jako argument přijme id entity (viz Entity) pro odebrání. Všechny entity, které jsou přidané/odebrané skrze tyto dvě metody budou přidané/odebrané na konci iterace. Metoda *GameState::getGame* navrátí ukazatel na hru, které herní stav přísluší (majitelé herního stavu). Každý herní stav může mít vlastní *ScoreBar*, pro přístup k němu lze využít metodu *GameState::getScoreBar*.

*GameState::update* je virtuální metoda, která se stará o aktualizaci herního stavu, pokud nedojde k *override*, tak jedině co tato metoda dělá je zavolání *Entity::update* na každé entitě, která náleží hernímu stavu a zároveň aktualizace kolekce s entity (přidávání/odebírání entit). *GameState::render* je virtuální metoda, která zavolá *Entity::render* na každé entitě, která hernímu stavu náleží. 

Na třídě je také definovaná metoda *Gamestate::collision*, která přijímá argument id (id entity, pro testování kolizí) a typ entity (typ entity se kterýma je kolize plátna), metoda navrací vektor, který obsahuje id každé entity, pro kterou je platná kolize a zároveň koliduje s testovanou entitou. Kolize jsou ověřovány pomocí *Entity::getRigid*.

# Entity
 Každá entita je reprezentována třídou, která dědí od třídy *Entity*. Při vytvoření entity (zavolání konstruktoru), je entitě přiděleno unikátní ID. Pro přístup k tomuto id lze využít metodu *Entity::getId*. Entity jsou rozděleny do několika typů, typ entity je reprezentován enum *EntityType*, kterým obsahuje následující typy: 
- None (0x0) - Entita nepatří do žádného typu.
- Player (0x1) - Entita je hráč.
- Structure (0x2) - Entita je struktura.
- Enemy (0x4) - Entita je nepřítel.
- UI (0x8) - Entita je prvek uživatelského rozhraní.
Každá entita může obsahovat vícero typu, typy se dají kombinovat pomocí operátorů |. Pro získání typu entity slouží metoda *Entity::getType*.

Pro vytvoření entity je potřeba herní stav, do kterého bude entita přiřazena, Vzhled, který je reprezentován potomkem třídy *IRenderable* (viz IRenderable) a typ. Pro přístup k hernímu stavu do, kterého patří slouží metoda *Entity::getGameState*, která navrací ukazatel na herní stav. Pro přístup k IRenderable slouží metoda *Entity::getSprite*. Tyto dvě metody jsou pouze *protected*.

Každá entity obsahuje *protected* field speed_, který představuje vektor jejího pohybu. Při zavolání virtuální metody *Entity::update* dojde k aktulizaci pozice dané entity. Virtuální metoda *Entity::render* zavolá *IRenderable::render*. Každá entita také obsahuje virtuální metodu *Entity::getRigid*, která vypočítá obdélník představující rigid body dané entity, tento obdélník slouží pro detekci kolizí.  

Na třídě *Entity* jdou také virtuální metody *Entity::onDead*, která se zavolá v případě, že entita bude odebrána z herního stavu, *Entity::respawn* která slouží k "restartování" entity (tato metoda je zavolá, pokud entita obsahuje lifebar a entita z něj ztratí život), *Entity::getScore*, která navrátí skóre, které bude přiděleno hráči, pokud entitu zničí.

## Life Bar
Každá entita může obsahovat life bar, jedná se o entitu, která představuje životy jiné entity. Tyto životy jsou graficky znázorněny na obrazovce. Pokud entity ztratí život a entitě zbývá více jak 0 životů tak dojde k zavolání *Entity::respawn*, pokud již entita nemá na *LifeBar* žádná životy, dojde k zavolání *Entity::onDead*.

# IRenderable
*IRenderable* je abstraktní třída představujicí grafickou vizualizaci daně entity (sprite). Každý sprite má nadefinovanou velikost a pozici, k těmto parametrům lze přistupovat pomoci: *IRenderable::setPos*, *IRenderable::getPos*, *IRenderable::setSize*, *IRenderable::getSize*. Na této třídě je také abstraktní metoda *IRenderable::render*, která slouží pro vykreslení daného spritu. K této třídě jsou také nadefinovány potomci, tyto potomci jsou označeni jako *final* (není možné vytvářet pro ně další potomky):
- RectSprite - Představuje obdélník njake barvy.
- TextSprite - Představuje text.
- ImageSprite - Představuje obrázek/animaci(spritesheet). 

# Texture Manager
Jedná se o singleton, který mapuje jména textur na jednotlivé textury. Pro načtení textury slouží *TextureManager::load* a pro získání textury *TextureManager::get*.  

# Možná vylepšení
Jeden z větších nedostatků projektu je to že příliš používá *std::shared_ptr*, režie kolem těchto ukazatelů není malá. Další věc, která by se dala zlepšit je nepoužívat virtuální metody, virtuální volání je poměrně časově náročné. Dále by bylo lepší, kdyby logika herního stavu, life baru a score barů byla dekomponována. 

Způsob získávání skóre od zabitých nepřátel není nejlepší, jelikož je to implementováno skrze virtuální metodu *Entity::getScore*, ovšem tato metoda nedává smysl, pokud entita není typu *EntityType::Enemy*. Dále způsob "skladováni" entit ve vektoru *entities_* na *GameState* není příliš vhodný. 

 Úloha by se také dala obohatit o zvuky a managery pro zvuky a fonty, momentálně je implementován pouze manager pro textury. Doxygen komentáře by bylo lepší psát průběžně, slouží jako dokumentace a je jednoduší s kódem pracovat. 