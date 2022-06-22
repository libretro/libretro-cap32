/****************************************************************************
 *  Caprice32 libretro port
 *
 *  Copyright David Colmenero - D_Skywalk (2019-2022)
 *
 *  Redistribution and use of this code or any derivative works are permitted
 *  provided that the following conditions are met:
 *
 *   - Redistributions may not be sold, nor may they be used in a commercial
 *     product or activity.
 *
 *   - Redistributions that are modified from the original source must include the
 *     complete source code, including the source code for all components used by a
 *     binary built from the modified sources. However, as a special exception, the
 *     source code distributed need not include anything that is normally distributed
 *     (in either source or binary form) with the major components (compiler, kernel,
 *     and so on) of the operating system on which the executable runs, unless that
 *     component itself accompanies the executable.
 *
 *   - Redistributions must reproduce the above copyright notice, this list of
 *     conditions and the following disclaimer in the documentation and/or other
 *     materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************************/

#include <libretro.h>

// clean-cpc-db CRC database - final release 1.0
uint32_t dbclean[] = {
   0x96ecfc4f, // Armure Sacree d'Antiriad, L' (1986)(Palace Software)(fr).dsk
   0x5ecef1ab, // Adventures of Basildon Bond, The (1986)(Probe Software)[cr].dsk
   0xbf188ce3, // Abbey Of Crime, The (1988)(Opera Soft)(Hack).dsk
   0xebdc9ba9, // Adidas Championship Football (1990)(Ocean).dsk
   0x17dd8795, // Adventure D - Espionage Island (1985)(Artic Computing)[cr].dsk
   0xd68054e1, // Apprentice, The (1986)(Mastertronic)[tape].dsk
   0xf4aba310, // Afteroids (1988)(Zigurat Software)(es).dsk
   0x59f1bcd2, // Amnesia (1998)(DBT)(M2)[cr].dsk
   0x7037c06c, // Abu Simbel Profanation (1986)(Dinamic Software)(es)[cr].dsk
   0xf9a760ff, // Action Service (1988)(Cobra Soft).dsk
   0x2ec8ee25, // Arnold Goes to Somewhere Else (1984)(Nemesis)[cr].dsk
   0x76bbc7a0, // Atomic Battle (1988)(R.A. Waddilove)[cr].dsk
   0x43a65e0b, // Axys - The Last Battle (1991)(CPC Infos, Amstar)[cr][m].dsk
   0xe8b02164, // Amsoccer (1986)(Amsoft)[tape].dsk
   0x74458dd9, // Afterburner (1988)(Activision).dsk
   0xb884ccab, // Autre Aventure, L' (1987)(Genisoft)(fr)[cr][f].dsk
   0x28b56166, // America's Cup Challenge (1986)(Armchair Entertainment).dsk
   0x74ee6f60, // Animated Strip Poker (1985)(Knight Soft)[cr].dsk
   0xf3f06148, // Alien Highway - Encounter 2 (1986)(Vortex Software).dsk
   0xd207f993, // Arachnophobia (1991)(Titus)(M5)(Side B).dsk
   0x1f368d7b, // Arachnophobia (1991)(Titus)(M5)(Side A).dsk
   0xe9058607, // Amaurote (1987)(Mastertronic)[cr][f].dsk
   0x270e4f88, // Abadlend (1985)(Damstar)(fr)[cr].dsk
   0x53f17767, // Amstroids (1985)(CPC Computing)[tape].dsk
   0xe849458a, // Aquanaute (1988)(FIL)(fr).dsk
   0x1039467d, // Aqua - Viaje a la Superficie (2006)(CEZ Games Studio)[tape].dsk
   0x7d3fe833, // Aqua - Viaje a la Superficie (2006)(CEZ Games Studio)(es)[tape].dsk
   0x92e55932, // Alien Storm (1991)(US Gold)(v2)(Side A).dsk
   0x5dbbee95, // Alien Storm (1991)(US Gold)(v2)(Side B).dsk
   0x33e38c87, // Asterix Chez Rahazade (1988)(Coktel Vision)(fr)(Side A).dsk
   0xdcf9ae3e, // Asterix Chez Rahazade (1988)(Coktel Vision)(fr)(Side B).dsk
   0x551da885, // Archon (1988)(Electronic Arts)[cr].dsk
   0xbdc9f811, // Agent-X II (1987)(Mastertronic)[cr].dsk
   0x5a2f2ab0, // Angelique - A Grief Encounter (1985)(Nemesis)[tape].dsk
   0x7eaa9ca6, // Alinka (1991)(Eric Boucher)(fr)[cr].dsk
   0x3489ff00, // Amazing Shrinking Man (1986)(Infogrames)(M4).dsk
   0x705ef1ce, // Aton (1987)(Sygram)(es)[tape].dsk
   0xa529f3c4, // Alpine Games (1987)(Atlantis Software)[tape].dsk
   0x1905733c, // Adventure C - The Ship of Doom (1985)(Artic Computing)[cr].dsk
   0xb3137d19, // Ajedrez Tridimensional (1985)(Amsoft)(es)[cr].dsk
   0x471d7147, // Ashkeron (1985)(Mirrorsoft)[tape].dsk
   0x82ce300d, // After Shock (1986)(Interceptor Micros Software).dsk
   0xc963bfe0, // Asphalt (1987)(Ubi Soft)(fr)(v2).dsk
   0x1d7cea37, // Arkos (1988)(Zigurat Software)(es).dsk
   0xaa05ed4a, // Abadia del Crimen, La (1988)(Opera Soft)(es).dsk
   0x0fa8f55e, // Atomic Driver (1988)(Loriciels)(fr)[tape].dsk
   0xe468306f, // Autocrash (1991)(Zigurat Software)(es).dsk
   0x2c97c481, // Admiral Graf Spee (1984)(Temptation Software - Amsoft)[cr].dsk
   0xe3217d1a, // Admiral Graf Spee (1984)(Temptation Software - Amsoft)(es)[cr].dsk
   0x1eea288d, // Action Fighter (1989)(Firebird Software).dsk
   0xf6b68eb8, // Averno (1989)(Proein Soft Line)(es).dsk
   0x2458ff4d, // Ataque Nuclear (1987)(Amstrad User Club)(es)[cr].dsk
   0x17b1b38d, // Atomik (1988)(Loriciels)(fr)[tape].dsk
   0x00e4bf51, // Arkanoid (1987)(Imagine Software).dsk
   0x3db8fc89, // Army Moves (1986)(Dinamic Software).dsk
   0xe49fc694, // Army Moves (1986)(Dinamic Software)(es).dsk
   0x7b3fda27, // Ariane (1985)(21 St Century Soft)(fr)[cr].dsk
   0xbee977b4, // AMC - Astro Marine Corps (1989)(Dinamic Software)(es).dsk
   0xf43cb524, // AMC - Astro Marine Corps (1989)(Dinamic Software).dsk
   0x937a947c, // ACE - Air Combat Emulator (1986)(Cascade Games).dsk
   0x8232e909, // Activator (1986)(Cascade Games).dsk
   0x7f369255, // Andy Capp (1987)(Mirrorsoft).dsk
   0x349ae355, // Andy Capp (1987)(Mirrorsoft)(fr)[cr].dsk
   0x29f788a1, // Anneau de Zengara (1987)(Ubi Soft)(fr)(Side B).dsk
   0x934357ae, // Anneau de Zengara (1987)(Ubi Soft)(fr)(Side A).dsk
   0x6e1d6208, // Astro Plumber (1985)(Blue Ribbon Software)[tape].dsk
   0xbd70a734, // Astroball (1988)(The Power House)[tape].dsk
   0x2c17fb46, // Amsgolf (1984)(Amsoft).dsk
   0x77860bfe, // Amsgolf (1984)(Amsoft)(de).dsk
   0x87524abd, // Amnesia (1998)(DBT)(M2)[cr].dsk
   0xf53ce2a9, // Atomic Fiction (1987)(Chip)(fr).dsk
   0xad523cb2, // Adult One (19xx)(Simon Avery)[cr].dsk
   0xcb97a9e0, // Adidas Championship Tie-Break (1990)(Ocean).dsk
   0x5909d93b, // A320 (1989)(Loriciels)(fr)(Side B).dsk
   0xaa9f21c3, // A320 (1989)(Loriciels)(fr)(Side A).dsk
   0xd82c9f67, // Atlantis (1988)(Cobra Soft)(fr).dsk
   0xf56c5741, // ATV Simulator (1988)(Codemasters Software)[cr].dsk
   0xfa44813b, // APB - All Point Bulletin (1989)(Domark).dsk
   0x569c2c48, // Academy - Tau Ceti II (1987)(CRL Group).dsk
   0xf48d4bdc, // Academy - Tau Ceti II (1987)(CRL Group)(fr).dsk
   0xa63c8b7e, // Animal Vegetable Mineral (1984)(Amsoft)(fr)[cr].dsk
   0x42ac44d5, // Animal Vegetable Mineral (1984)(Amsoft)(es)[cr].dsk
   0x2ac1e169, // Animal Vegetable Mineral (1984)(Amsoft).dsk
   0x9c7a44e8, // Asterix En La India (1988)(Coktel Vision)(es)(Side B).dsk
   0x392fbb50, // Asterix En La India (1988)(Coktel Vision)(es)(Side A).dsk
   0x33f566e2, // Affaire Ravenhood, L' (1993)(Bruno Fonters)(fr)(Side A)[cr].dsk
   0x5497709a, // Affaire Ravenhood, L' (1993)(Bruno Fonters)(fr)(Side B)[cr].dsk
   0x4f1d2dd9, // Aliens US (1987)(Electric Dreams Software).dsk
   0x30e15ff9, // Armageddon Man, The (1987)(Martech Games).dsk
   0x14db334c, // Attentat (1986)(Rainbow Productions)(fr)(Side A).dsk
   0x1dcb721a, // Attentat (1986)(Rainbow Productions)(fr)(Side B).dsk
   0xf297d09c, // Attack of the Killer Tomatoes (1986)(Global Software).dsk
   0x56f7e773, // Aventures de Jack Burton, Les (1987)(Electric Dreams Software)(fr).dsk
   0xc989a491, // Aladdin's Cave (1985)(Artic Computing)[cr].dsk
   0x1323190c, // Astro Attack (1984)(Amsoft)[tape].dsk
   0xef2db36f, // Astro Attack (1984)(Amsoft)(es)[tape].dsk
   0xcb0dcfa0, // Amthello (1984)(Amstrad Computer User)[cr].dsk
   0xc488a2c4, // Amthello (1984)(Amstrad Computer User)(es)[cr].dsk
   0x37d63655, // Archers, The (1987)(Mosaic Publishing)[tape].dsk
   0x31d37ba2, // Asterix Im Morgenland (1988)(Coktel Vision)(de)(Side A).dsk
   0xc1508616, // Asterix Im Morgenland (1988)(Coktel Vision)(de)(Side B).dsk
   0x5eb0ef6f, // Acro Jet (1986)(US Gold).dsk
   0x95f8abaf, // Adventure B - Inca Curse (1985)(Artic Computing)[cr].dsk
   0xe6769f19, // Aventures de Pepito au Mexique, Les (1991)(Microids)(fr)(Side A).dsk
   0x9d6ee380, // Aventures de Pepito au Mexique, Les (1991)(Microids)(fr)(Side B).dsk
   0x5ee2116a, // Affaire Sydney, L' (1986)(Infogrames)(fr).dsk
   0x13a97ec5, // Arctic Fox (1988)(Electronic Arts).dsk
   0x6515e302, // American Tag Team Wrestling (1992)(Zeppelin Games)[cr].dsk
   0xaa08dafd, // Air Cobra (1987)(Unicornia Soft)(es)[cr].dsk
   0x041f9748, // Adios A La Casta - Episode 2 (2016)(4Mhz)(es)(v1.0)[cr].dsk
   0x64592815, // Aventura Original, La (1989)(Aventuras AD)(Retro Alacant Turbo)(es).dsk
   0x790a9ac2, // Arcade Flight Simulator (UK) (1989)[tape].dsk
   0xd5a4c3f2, // Affaire Vera Cruz, L' (1985)(Infogrames)(fr).dsk
   0xc809daf9, // Airwolf (1985)(Elite Systems - Amsoft).dsk
   0xa9a4ecd9, // Atlantis (1985)(Anirog Software)[cr].dsk
   0x4c117ec5, // Addams Family, The (1991)(Ocean).dsk
   0x68e5f2a1, // Advanced Pinball Simulator (1988)(Codemasters Software)[cr].dsk
   0xe5c4a056, // Armadura Sacrada De Antiriad, La (1986)(Palace Software)(es)[cr][f].dsk
   0x635c9042, // Airborne Ranger (1988)(Microprose Software)(Side B).dsk
   0x5fc272b4, // Airborne Ranger (1988)(Microprose Software)(Side A).dsk
   0x1fae9dcb, // Archon II - Adept (1989)(Electronic Arts)[cr].dsk
   0xea9bb85d, // Alex Higgins World Snooker (1985)(Amsoft).dsk
   0xa32c19fb, // Au Revoir Monty (1987)(Gremlin Graphics Software).dsk
   0x363cfc7c, // Advanced Destroyer Simulator (1990)(Futura)(es).dsk
   0xee5f060c, // Advanced Destroyer Simulator (1990)(Futura)(fr).dsk
   0x60c00eea, // Ace of Aces (1986)(US Gold).dsk
   0x98c7bf27, // Athlete (1987)(Microids)(fr).dsk
   0x4319969a, // Aventura Espacial, La (1990)(Aventuras AD)(es).dsk
   0xfadd2854, // Adios A La Casta - Episode 1 (2015)(4Mhz)(es).dsk
   0xe9e99d67, // Altered Beast (1989)(Activision)(Side A).dsk
   0x1dd7e040, // Altered Beast (1989)(Activision)(Side B).dsk
   0x52695660, // Artura (1989)(Gremlin Graphics Software).dsk
   0xf7dd74b9, // Action Force (1988)(Virgin Games).dsk
   0x45ede8ec, // Aaargh! (1989)(Melbourne House)(es).dsk
   0x506ce975, // Anna (1988)(Edsa)(de-en)[cr].dsk
   0xfc4837a4, // Anna (2019)(Edsa)(es)(Hack)[cr].dsk
   0x2996cc8b, // Anna (1988)(Edsa)(fr)[cr].dsk
   0x23768ed0, // Aussie Safari (1990)(U.S.Gold)(Side A).dsk
   0x01e31aa5, // Aussie Safari (1990)(U.S.Gold)(Side B).dsk
   0xdb1ca9d0, // Antares (1987)(Dro Soft)(es)[tape].dsk
   0x3342f662, // Amstrad Shuffle (1986)(Alpha Omega Software)[cr].dsk
   0x62d17d76, // After the War (1989)(Dinamic Software).dsk
   0x8b94c44c, // After the War (1989)(Dinamic Software)(es).dsk
   0xcc7496cc, // Aufwarts Zur Rettung - Climb-It (1984)(Tynesoft)[tape].dsk
   0x394bbc01, // Abracadabra et les Voleurs de Temps (1988)(Carraz Editions)(fr).dsk
   0x6720c16a, // Ali Candil y el Tesoro de Sierra Morena (1986)(Edisoft)(es)[cr].dsk
   0x17b5103e, // Asterix and the Magic Cauldron (1986)(Melbourne House).dsk
   0x00588f97, // Amsilvania Castle (1987)(Veosoft)(es)[cr].dsk
   0x7be35474, // A-Team, The (1989)(Zafiro Software Division)(es)(Side B).dsk
   0x2be8b9dd, // A-Team, The (1989)(Zafiro Software Division)(es)(LightGun)(Side A).dsk
   0x49389379, // A-Team, The (1989)(Zafiro Software Division)(es)(Side A).dsk
   0x47fc2edc, // A-Team, The (1989)(Zafiro Software Division)(es)(LightGun)(Side B).dsk
   0x160644e9, // Alien 8 (1985)(Ultimate Play The Game)[tape].dsk
   0x42d83aa2, // Australian Rules Football (1989)(Again Again)[cr].dsk
   0xb98aceb9, // Auftrag in der Bronx (1985)(Weeske Computer-Elektronik)(de).dsk
   0x3e909eff, // A la Pursuite de Carmen Sandiego dans le Monde (1990)(Broderbund Software)(fr)(Side B).dsk
   0xc0405f10, // A la Pursuite de Carmen Sandiego dans le Monde (1990)(Broderbund Software)(fr)(Side A).dsk
   0xfe232a4d, // African Trail Simulator (1990)(Positive)(es).dsk
   0xfea25877, // African Trail Simulator (1990)(Positive)(M2).dsk
   0x0ee0db8b, // Alive (1991)(Lankhor)(fr)(Side B).dsk
   0xe0a5e28a, // Alive (1991)(Lankhor)(fr)(Side A).dsk
   0x2dd0e609, // Alkahera (1985)(Rino Marketing)[tape].dsk
   0x90445ff6, // Air Traffic Control (1985)(Hewson)[cr].dsk
   0xa9dd0625, // Ayo - The Game of Africa (1987)(Computing With The Amstrad)[cr].dsk
   0x8d0de6e4, // Alien Syndrome (1987)(ACE Software).dsk
   0xbf04b50d, // Airwolf II (1987)(Encore)[cr].dsk
   0xb0c8cc9e, // Aventura Original, La (1989)(Aventuras AD)(es).dsk
   0xfccbe622, // Amsnake (1985)(Your Computer)[cr].dsk
   0xd276b5bc, // Affaire Vera Cruz, Die  (1986)(Infogrames)(de).dsk
   0xf4621437, // Atomic (1992)(Fraggle and Duck)(PD)[Extended Version][cr].dsk
   0x99d1e3b0, // Aliens (1986)(Electric Dreams Software).dsk
   0x5867b00c, // Al-Strad (1985)(Rhinosoft)[cr].dsk
   0xea6fd821, // ACE 2 - The Ultimate Head to Head Conflict (1987)(Cascade Games).dsk
   0xec7a64d2, // Ahhh!! (1985)(CRL Group)[cr].dsk
   0x5c74e277, // Auf Wiedersehen Monty (1987)(Gremlin Graphics Software).dsk
   0x77e75fc5, // Advanced Tactical Fighter (1988)(Digital Integration)[codes].dsk
   0x9da1dad9, // Advanced Tactical Fighter (1988)(Digital Integration)[cr].dsk
   0x2eb9de77, // Affaire Santa Fe, L' (1988)(Infogrames)(fr)(Side B).dsk
   0x2203c641, // Affaire Santa Fe, L' (1988)(Infogrames)(fr)(Side A).dsk
   0xa9e693e3, // American Turbo King (UK) (1990)[tape].dsk
   0x819ea71c, // Atom Smasher (1985)(Romik Software - Amsoft)[tape].dsk
   0x53599dd8, // Amstra-Dames (1985)(Cobra Soft)(fr).dsk
   0x6edf3b87, // Asphyxie (1985)(Ludo's Software Distribution)(fr)[cr].dsk
   0x9949bce5, // Alien Break-In (1985)(Romik Software - Amsoft)(es)[tape].dsk
   0x8878a11b, // Alien Break-In (1985)(Romik Software - Amsoft)[tape].dsk
   0x29664c4c, // Adventure A - The Planet of Death (1985)(Artic Computing)[cr].dsk
   0xffc80866, // Amo del Mundo (1990)(Positive)(es).dsk
   0x16d70b4a, // Adult Two (1990)(Simon Avery)[cr].dsk
   0xb10a71fd, // Argo Navis (1985)(Kuma Computers).dsk
   0x6d278d3e, // Angel Nieto Pole 500cc (1990)(Opera Soft)(es).dsk
   0xc6bffad9, // American Football (1984)(Mind Games)[tape].dsk
   0x926c30c8, // Aftermath (1988)(Alternative Software)[cr].dsk
   0x8714bccc, // Aspar GP Master (1988)(Dinamic Software).dsk
   0x503d22e7, // Aigle d'Or, L' (1986)(Loriciels)(fr).dsk
   0x219d1cc5, // Arkanoid II - Revenge of Doh (1988)(Imagine Software).dsk
   0xff2e13d2, // Aventures au Chateau (1985)(Micro Application)(fr)[cr].dsk
   0x799f2e1d, // Avenger (1986)(Gremlin Graphics Software).dsk
   0x1826ec6e, // Anniversaire de Bobby, L' (19xx)(Carraz Editions)(fr).dsk
   0x1789290e, // Abracadabra (1988)(Proein Soft Line)(es)(Side A).dsk
   0xdcb960f9, // Abracadabra (1988)(Proein Soft Line)(es)(Side B).dsk
   0x5d64313f, // Anduril (19xx)(Schingnitz)(de)[cr].dsk
   0xf16ab0f6, // Alex Higgins World Pool (1985)(Amsoft).dsk
   0xa9047f72, // Airballs (1989)(Playfield)[cr].dsk
   0xa81e9849, // Bomb Jack II (1987)(Elite Systems).dsk
   0xdf2066fb, // Bustout (1984)(Amstrad Computer User)(de)[cr].dsk
   0x4e0d9163, // Bustout (2013)(Amstrad Computer User)(fr)[cr].dsk
   0x5fed8f56, // Bustout (1984)(Amstrad Computer User)[cr].dsk
   0x0d5b365c, // Boulder Crash (1987)(Amstar)(fr)[cr].dsk
   0x9beae974, // Batty (1987)(Elite Systems)[cr].dsk
   0xded167ba, // Bugsy (1986)(CRL Group)[cr].dsk
   0x24fe433a, // Bard's Tale, The (1988)(Electronic Arts)(Side A).dsk
   0x507323ba, // Bard's Tale, The (1988)(Electronic Arts)(fr)(Side A).dsk
   0x30a2896f, // Bard's Tale, The (1988)(Electronic Arts)(fr)(Side B).dsk
   0xa2451c8e, // Bard's Tale, The (1988)(Electronic Arts)(Side B).dsk
   0xc0afb5cb, // Bard's Tale, The (1988)(Electronic Arts)(de)(Side B).dsk
   0x602aab4d, // Bard's Tale, The (1988)(Electronic Arts)(de)(Side A).dsk
   0x6253270d, // Bombarderos (1984)(Amstrad Users)(es)[cr].dsk
   0xd3a10e04, // Bubble Dizzy (1991)(Codemasters Software)[cr].dsk
   0x0332934f, // Bronx Street Cop (1989)(Virgin Mastertronic)[cr].dsk
   0xb0fb1bbc, // Boy Racer (1987)(Alligata Software)[cr].dsk
   0x32e86faf, // By Fair Means Or Foul (1988)(Alligata Software).dsk
   0x6276192d, // Blackbeard (1988)(Topo Soft)(es)[cr].dsk
   0xf0ab7aec, // Billy The Kid (1990)(Mastertronic)[cr].dsk
   0xd46b9059, // Bridge (1986)(Infogrames)(M2).dsk
   0x96d31131, // Buster Block (1986)(Kuma Computers)[cr].dsk
   0x5ebc61a1, // Boulder Dash 3 (1986)(First Star Software).dsk
   0xe028abd6, // Blasted Squares (1988)(Schneider Magazin)(de)[cr].dsk
   0xb4e66edc, // Bored of the Rings (1985)(Silversoft)(Side A).dsk
   0xff288abd, // Bored of the Rings (1985)(Silversoft)(Side B).dsk
   0x762cc177, // Boinggg (1988)(Atlantis Software)[cr].dsk
   0x5276dcb2, // Bionic Ninja (1989)(Zeppelin Games)[cr].dsk
   0x11bfe1e9, // Back to the Future II (1990)(Image Works)(Side B).dsk
   0x3cb7f8e6, // Back to the Future II (1990)(Image Works)(Side A).dsk
   0x9cb1e0a7, // Baba's Palace (2017)(cpcretrodev.byterealms).dsk
   0xdc845b6f, // Batman The Caped Crusader (1988)(Ocean Software)(v2).dsk
   0x811b8f8c, // Bataille Pour Midway(1985)(PSS)(fr).dsk
   0x0a44e83e, // Bacterik Dream (1987)(Chip)(fr).dsk
   0x8d933a44, // Battle Command (1991)(Ocean Software)(Side A)[b].dsk
   0x55ebbd9e, // Battle Command (1991)(Ocean Software)(Side B)[b].dsk
   0x109a104f, // Big Screen Hero (1988)(Skyslip Computers).dsk
   0xa9415a47, // Bonz (2003)(CPC Oxygen)[cr].dsk
   0x2cf176da, // Boulder Dash Construction Kit (1987)(First Star Software).dsk
   0x2174ec91, // Bronx (1989)(MCM Software)(es).dsk
   0x4e5fa31a, // BMX Kidz (1988)(Firebird)[cr].dsk
   0x4fc3ef0b, // Blue War (1987) (Free Game Blot).dsk
   0xbb83cd78, // Blue-War (1986)(Free Game Blot)(fr).dsk
   0x4e67bd82, // Barbarian II (1989)(Palace Software).dsk
   0x75069f0d, // Boredom (19xx)(Amstrad Adventure PD Library)[cr].dsk
   0x441f86e9, // Basil The Great Mouse Detective (1987)(Gremlin Graphics Software).dsk
   0x310d89f4, // Bob Morane - Chevalerie (1987)(Infogrames)(fr).dsk
   0xdbad43f6, // Bataille Pour R.F.A. (1987)(PSS)(fr).dsk
   0x821a0c95, // Buran (1990)(OMK)(es).dsk
   0xa16ec840, // Bells, The (1986)(Blaby Computer Games)[cr].dsk
   0xa0746010, // Brian Jacks Superstar Challenge (1985)(Martech)[cr].dsk
   0x845fe56d, // Base, The (1985)(The Adventure Workshop).dsk
   0xffc7e373, // BMX Freestyle (1989)(Codemasters Software)[cr].dsk
   0xac0510d0, // Bobsleigh (1987)(Digital Integration)[cr].dsk
   0x30003af9, // Bobsleigh (1987)(Digital Integration)[codes].dsk
   0x80312e57, // Beverly Hills Cop (1990)(Tynesoft).dsk
   0xa5a40741, // Bubble Ghost (1988)(ERE Informatique)(fr).dsk
   0xc579dc8b, // Biggles (1986)(Mirrorsoft).dsk
   0xfe1caccb, // Black Land (1995)(Bollaware)(Disk 2 Side A)[cr].dsk
   0x7f685488, // Black Land (1995)(Bollaware)(de)(Disk 2 Side B)[cr].dsk
   0x7542fb4f, // Black Land (1995)(Bollaware)(Disk 2 Side B)[cr].dsk
   0xd31b44ee, // Black Land (1995)(Bollaware)(de)(Disk 1 Side B)[cr].dsk
   0xaa5757b5, // Black Land (1995)(Bollaware)(de)(Disk 1 Side A)[cr].dsk
   0xb5315d62, // Black Land (1995)(Bollaware)(de)(Disk 2 Side A)[cr].dsk
   0x9d7d2cf0, // Black Land (1995)(Bollaware)(Disk 1 Side B)[cr].dsk
   0x2516908e, // Black Land (1995)(Bollaware)(Disk 3 Side A)[cr].dsk
   0x3c0251bf, // Black Land (1995)(Bollaware)(Disk 1 Side A)[cr].dsk
   0xb49cc4e0, // Bachou (1985)(Central Solutions LTD)[cr].dsk
   0x46b5fe2c, // Boulder Dash (1985)(MirrorSoft).dsk
   0x48fcdc1e, // Boulder Dash 6 (1986)(Michael Gordon)(Hack)[cr].dsk
   0x099c3c44, // Bagne de Nepharia, Le (1985)(Loriciels)(fr)(v2)[cr].dsk
   0xa1213ee3, // Blip! (1988)(Silverbird)[cr].dsk
   0x2c2eb1e9, // Beyond the Ice Palace (1988)(Elite Systems).dsk
   0x925bf86b, // Ballyhoo (1986)(Infocom)(Side A)[CPM].dsk
   0xa6dc3157, // Ballyhoo (1986)(Infocom)(Side B).dsk
   0xa741db1e, // Bactron (1986)(Loriciels)(fr).dsk
   0x8620b5dc, // Bump Set Spike (1987)(Mastertronic)[cr].dsk
   0x2a17ecd1, // Great Barrier Reef, The (1987)(The Power House)[cr].dsk
   0xf1598d85, // Bubbler (1987)(Ultimate Play The Game).dsk
   0x065effc5, // Braxx Bluff (1985)(Amsoft).dsk
   0x371e44f9, // Bigfoot (1988)(Codemasters Software).dsk
   0xe6fe3e2d, // Bob Morane Jungle 1 (1987)(Infogrames)(fr).dsk
   0x444c8866, // Billard Americain (1988) (CPC)(fr)[cr].dsk
   0x8b5a4eed, // Block Invasion (2002)(Richard Wilson).dsk
   0x5c140a84, // Booly (1991)(Loriciels)(M2)(Side A)[codes].dsk
   0xefd1a301, // Booly (1991)(Loriciels)(M2)(Side B)[codes].dsk
   0x31341abd, // Back to the Future (1986)(Activision).dsk
   0xd7140207, // Blue Tomb (1986)(ICS)[cr].dsk
   0xbb143978, // Balloon Buster (1989)(Blue Ribbon Software)[cr].dsk
   0x35245e40, // Billy 2 (1987)(Loriciels).dsk
   0xc532e616, // Bionic Commando (1988)(US Gold)(Green).dsk
   0x1f3fa9a4, // Bionic Commando (1988)(US Gold)(Colour).dsk
   0xfea7e2a7, // Batman The Movie (1989)(Ocean Software).dsk
   0x160cbb57, // Bobby Bearing (1986)(The Edge Software).dsk
   0x204b87ba, // Blazing Thunder (1990)(Hi-Tec Software)[cr][f].dsk
   0x90998c20, // Beach-Head (1985)(Amsoft)(M2).dsk
   0xc3283ce5, // Bullseye  (1985)(Macsen Software)(fr)(LightGun)[cr].dsk
   0x21491d7a, // Bulls Eye (1985)(Macsen Software)[cr].dsk
   0x474a9722, // Boulder Dash 2 (1985)(Peter Liepa)(Hack)(fr)[cr].dsk
   0xfed514e8, // Boulder Dash 2 (1985)(Peter Liepa)(Hack)[cr].dsk
   0xed191009, // Basket Coach (1988)(Rym Soft)(es)[cr].dsk
   0x8de80fd4, // Baloncesto - NBA (ABC Sports)(es)[cr].dsk
   0x207fb361, // Blokker (1985)(Computer Magazine)[cr].dsk
   0x257f11ff, // Book of the Dead (1987)(CRL Group)[cr].dsk
   0xb7045bc1, // Bobo (1988)(Inforgrames(fr)(Side B).dsk
   0xc36f1142, // Bobo (1988)(Inforgrames(fr)(Side A).dsk
   0x1ad5461c, // Blockbusters (1984)(Macsen Software)[cr].dsk
   0xd74b46f4, // Burglar (1991)(Lankhor)(fr)(side A)[cr].dsk
   0xfa4fbf3b, // Burglar (1991)(Lankhor)(fr)(side B)[cr].dsk
   0x5cb36887, // Bust Out II (1986)(Activision)[cr].dsk
   0xe3ce13f4, // Buggy Boy (1988)(Elite Systems).dsk
   0x00e0b66d, // Battle of the Planets (1986)(Mikrogen)[cr].dsk
   0xbc9f72c9, // Ballblazer (1986)(Activision).dsk
   0xf4292003, // Back to Reality (1986)(Mastertronic)[cr].dsk
   0x5a526d33, // Bridge Player 3 (1986)(CP Software).dsk
   0x320dff2a, // Backgammon (1985)(CP Software)(es).dsk
   0x2f292d29, // Blue Star (1990)(Free Game Blot)(fr).dsk
   0xbeca9867, // Bob Morane Ocean (1988)(Infogrames)(M2).dsk
   0xeb0c1927, // Barry McGuigan World Championship Boxing (1985)(Activision).dsk
   0x5a4129f6, // Blockbusters (1986)(Domark).dsk
   0x1e15ade7, // Buffalo Bill's Wild West Show (1989)(Tynesoft).dsk
   0x8d86c090, // Bosconian '87 (1987)(Mastertronic)[cr].dsk
   0x92730dfa, // Blasteroids (1989)(Image Works).dsk
   0x82e295b3, // Bad Cat (1987)(Rainbow Arts)(Side A).dsk
   0x5213bea6, // Bad Cat (1987)(Rainbow Arts)(Side B).dsk
   0x97eab65c, // Back to the Golden Age (1991)(Ubi Soft)(fr)(Side B).dsk
   0xe5c3e538, // Back to the Golden Age (1991)(Ubi Soft)(fr)(Side A).dsk
   0xb25430db, // Beach Volley (1989)(Ocean Software).dsk
   0x14017f7d, // Battle-Spheres (1994)(The Firm)[cr].dsk
   0x264c68fd, // Bomb Scare (1986)(Firebird)[cr].dsk
   0xeb9c2347, // Bo! (1994)(Bollaware)(de)[cr].dsk
   0xfb811d2b, // Bloodwych (1990)(Image Works)(v2).dsk
   0xfac68415, // Big League Soccer II (2018)(Viper)(es)(Hack)[cr].dsk
   0x4a33bfa8, // Big League Soccer II (1985)(Viper).dsk
   0xdd48c5ca, // Beta 2515 (1987)(G.T.S. Editorial)(es)[cr].dsk
   0x2e58903c, // Ball Bearing (1993)(Radical Software)[cr].dsk
   0x5fc8032d, // Boloncio (1986)(G.T.S. Editorial)(es)[cr].dsk
   0xc26b2a02, // Boeing 727 (19xx)(-)(de)[cr].dsk
   0x7759c6b0, // Bust Out (1987)(PC Schneider International)[cr].dsk
   0x254b189b, // Billy La Banlieue (1986)(Loriciels)(fr).dsk
   0xf822cc4a, // Break Thru (1986)(US Gold).dsk
   0xab95d498, // La Boule Infernale (1985)(Hebdogiciel)(fr)[cr].dsk
   0x5604a788, // Best of the Best (1992)(Loriciels).dsk
   0xae24fde5, // Battle Ships (1987)(Elite Systems).dsk
   0x2dc8c009, // Blueberry (1987)(Coktel Vision)(de)(Side A)[CPM].dsk
   0x49a750a5, // Blueberry (1987)(Coktel Vision)(de)(Side B).dsk
   0xf5dd8263, // Blueberry (1987)(Coktel Vision)(es)(Side B).dsk
   0xd81cd9fb, // Blueberry (1987)(Coktel Vision)(es)(Side A)[CPM].dsk
   0x48edfc14, // Bride Of Frankenstein (1987)(Ariolasoft)[tape].dsk
   0x417b6cbe, // Bomber (1985)(Amstrad Computer User)[cr].dsk
   0xf0dd6d74, // Blood Brothers (1988)(Gremlin Graphics Software)(v2).dsk
   0xc12d5edb, // Barbarian (1988)(Melbourne House)[cr].dsk
   0xdc214555, // Bubble Bobble 2 (1988)(Saturn-Soft)(Hack)[cr].dsk
   0x1de8ceaa, // Bunny Bricks (1992)(Silmarils)(Side A).dsk
   0x052a7610, // Bunny Bricks (1992)(Silmarils)(Side B).dsk
   0x22edc1ab, // Beat the Clock (1985)(Amstrad Computer User)[cr].dsk
   0x057ec7f2, // Buggy Ranger (1990)(Dinamic Software)(es).dsk
   0x9ac25387, // Ball Breaker (1987)(CRL Group)(v3).dsk
   0x98cd5629, // Bestial Warrior (1989)(Dinamic Software)(es)(LightGun).dsk
   0x50203397, // Bestial Warrior (1989)(Dinamic Software)(es).dsk
   0x2c7308d9, // Ball Breaker II (1988)(CRL Group).dsk
   0x0100bcc3, // Black Fountain, The (1987)(Incentive Software)[cr][f].dsk
   0x76c0f723, // Biff (1992)(Beyond Belief)[cr].dsk
   0x953030d9, // Bust Out (1986)(Activision)[cr].dsk
   0x10bd9015, // Beach Buggy Simulator (1988)(Silverbird Software)[cr][f].dsk
   0x95d9f222, // Bubble Bobble (1987)(Firebird).dsk
   0xee4137b7, // Bedlam (1988)(US Gold).dsk
   0xafdabd14, // Binky (1985)(Software Projects)[cr].dsk
   0xe147a91e, // Battlefield Germany (1987)(PSS)(v2).dsk
   0x60e1f5f0, // Barbarian (1987)(Palace Software)(Side B).dsk
   0xc761e738, // Barbarian (1987)(Palace Software)(Side A).dsk
   0x480c9e65, // Budokan (1990)(Electronic Arts)(Side A).dsk
   0x662a9bde, // Budokan (1990)(Electronic Arts)(Side B).dsk
   0x0814d462, // Batman (1986)(Ocean Software).dsk
   0xddfa0583, // Bumpy's Arcade Fantasy (1992)(Loriciels)(v2)(Side A).dsk
   0xc04fbd25, // Bumpy's Arcade Fantasy (1992)(Loriciels)(v2)(Side B).dsk
   0xd479a21a, // Bob Winner (1986)(Loriciels)(Side B).dsk
   0x0d11608f, // Bob Winner (1986)(Loriciels)(fr)(Side B).dsk
   0x305153d3, // Bob Winner (1986)(Loriciels)(fr)(Side A).dsk
   0xb94d8ef1, // Bob Winner (1986)(Loriciels)(Side A).dsk
   0x0365bb6a, // Buggy II (1986)(Chip)(fr)[cr].dsk
   0x07d6f24b, // Boggit, The (1986)(CRL Group)[cr].dsk
   0x44179cde, // Brainache (1987)(Codemasters Software)[cr].dsk
   0xd10b4b49, // Bridge It (1984)(Amsoft)[cr].dsk
   0xa88c4a27, // Bad Max (1985)(Transoft)(fr).dsk
   0xf3422b3d, // Black Tiger (1989)(US Gold).dsk
   0x35fbb25e, // Beach-Head II - The Dictator Strikes Back! (1986)(US Gold).dsk
   0xd55039f2, // Baca (1998)(Drazen)[cr].dsk
   0x7dd0a226, // Brian Bloodaxe (1985)(The Edge).dsk
   0xb0a4f51f, // Barbarian Part III (1988)(Des Soft)(fr)(Hack)[cr].dsk
   0x23096eca, // Barmelo 3D (1987)(Infomedia)(fr)[cr].dsk
   0x83f93b45, // Blade Runner (1986)(CRL Group)[cr].dsk
   0x4239f562, // Balldozer (1991)(Jeroen Erkens)(PD)[cr].dsk
   0x524f4cf7, // Big Trouble in Little China (1986)(Activision).dsk
   0xcb9f4162, // Burger Time (1985)(Hebdogiciel)(fr)[cr].dsk
   0x22a04a98, // Beer Hunter, The (1987)(Global Software)[cr].dsk
   0x5706fe0c, // Balrog (1987)(Gac User)[cr].dsk
   0x0dde0f4c, // B.A.T. (1991)(Ubi Soft)(Disk 1 Side B).dsk
   0x5503a29a, // B.A.T. (1991)(Ubi Soft)(Disk 2 Side B).dsk
   0xe8bbf1c7, // B.A.T. (1991)(Ubi Soft)(fr)(Disk 2 Side A).dsk
   0xfd51471e, // B.A.T. (1991)(Ubi Soft)(fr)(Disk 2 Side B).dsk
   0xc6349fe7, // B.A.T. (1991)(Ubi Soft)(Disk 1 Side A).dsk
   0x69bd3223, // B.A.T. (1991)(Ubi Soft)(fr)(Disk 1 Side A).dsk
   0xb5aee4f6, // B.A.T. (1991)(Ubi Soft)(Disk 2 Side A).dsk
   0xd3cfc505, // B.A.T. (1991)(Ubi Soft)(fr)(Disk 1 Side B).dsk
   0x2724fe4b, // Brick, The (1989)(Delta Software)(es).dsk
   0x2e87d8a8, // Badlands (1990)(Domark).dsk
   0x18af16eb, // Bombfusion (1989)(Mastertronic)[cr].dsk
   0x4bf042ca, // Bomb Jack (1986)(Elite Systems).dsk
   0x739d1232, // Barthels Dash (1990)(Bollaware)(de)[cr].dsk
   0x29c7970f, // Berks III (1985)(The Power House)[cr].dsk
   0x471c5eac, // Basket Master (1987)(Dinamic Software).dsk
   0xa4a0dc6e, // Brian Clough's Football Fortunes (1987)(CDS Software LTD).dsk
   0x20355e4e, // Brian Clough's Football Fortunes (2020)(CDS Software LTD)(es)(Hack)[cr].dsk
   0x6b339e59, // Brian Clough's Football Fortunes (1987)(CDS Software LTD)(de)[cr].dsk
   0x1e366bb6, // Barbarian Part II (1988)(Georgessoft)(Hack)(v2)[cr].dsk
   0x8e401eb1, // Booty (1984)(Firebird)[cr].dsk
   0xfdd2c922, // Bestiary (1987)(Amstrad Adventure PD Library)[cr].dsk
   0x1773adfb, // Bravestarr (1987)(US Gold).dsk
   0x351a8654, // Battle For Midway (1985)(PSS)[cr].dsk
   0x6d205cbc, // Big League Soccer (1985)(Viper)[cr].dsk
   0x7731514d, // BMX Ninja (1989)(Alternative Software)(M2).dsk
   0x142ab82b, // Battle Of Britain (1985)(PSS).dsk
   0xd389082e, // Balle De Match (1985)(Psion)(fr).dsk
   0x3c94e5b1, // Battle Beyond the Stars (1985)(Solar Software)[cr].dsk
   0xa756b4fa, // Blagger (1985)(Amsoft)[cr].dsk
   0x2932ed4f, // Blade Warrior (1989)(Codemasters Software)[cr].dsk
   0xf4064b7e, // Black Magic (1987)(US Gold).dsk
   0x5ea3f8fa, // Black Sea (2022)(Mananuk).dsk
   0xa5e349a1, // Bingo (1988)(Amstrad Magazine Publinfor)(es)[cr].dsk
   0xcee48cc7, // Bingo (1988)(Amstrad Magazine Publinfor)(pt)[cr].dsk
   0x6cf1612c, // BMX Simulator (1987)(Codemasters Software)[cr].dsk
   0xdee486dd, // Bounder (1986)(Gremlin Graphics Software).dsk
   0xcf73ebd6, // Bruce Lee (1984)(US Gold).dsk
   0x424afa2f, // BIAT (1988)(Microfutur)(fr)(Side B)[b].dsk
   0xcfccf0cf, // BIAT (1988)(Microfutur)(fr)(Side A)[b].dsk
   0xa8b16df6, // Bumpy (1989)(Loriciels)(fr).dsk
   0xde4e7b98, // Banda Salvaje, La (1985)(Firebird)(es)[cr].dsk
   0xbc08dbb8, // Bigtop Barney (1986)(Players)[cr].dsk
   0x52f53a63, // Beetle Mania (1985)(Software Exchange)[cr].dsk
   0xfd361799, // Big Sleaze, The (1987)(Piranha)[cr].dsk
   0x10336c2f, // Bundesliga Manager 3 (1993)(O+T Soft)(de)[cr].dsk
   0x8cbb85d9, // Baby Jo - Going Home (1991)(Imagine Software)(v2)(Side B).dsk
   0xd015805a, // Baby Jo - Going Home (1991)(Imagine Software)(v2)(Side A).dsk
   0xb27705e2, // Butcher Hill (1989)(Gremlin Graphics Software).dsk
   0x6d9aeec1, // Buzzlines (1985)(The Amstrad User)[cr].dsk
   0x680a8d67, // Castle Master (1990)(Incentive)(M3).dsk
   0x276db46d, // Castle Master (1990)(Incentive)(es).dsk
   0xe17d14cb, // Classiques Titus Volume 2 (1987)(Titus)(fr).dsk
   0x514d61ad, // Chuck Yeager's Advanced Flight Trainer (1989)(Electronic Arts)(v2)(Side A).dsk
   0x732f1945, // Chuck Yeager's Advanced Flight Trainer (1989)(Electronic Arts)(v2)(Side B).dsk
   0x95a1b7fe, // City for Ransom (1989)(Wow Software)[cr].dsk
   0x15d28b90, // Chateau du Diable, Le (1985)(Cobra Soft)(fr)[cr].dsk
   0x8019d184, // Curse of Sherwood, The (1987)(Mastertronic)[cr].dsk
   0x3a3dc732, // Crack Down (1990)(US Gold).dsk
   0xaadab65d, // Cray 5 (1987)(Topo Soft)(es).dsk
   0x8ea026c8, // Computer Maniac's Diary (1989)(Domark)[cr].dsk
   0xa5e1b296, // Crypte des Maudits, La (1991)(Lankhor)(fr)(Side A).dsk
   0x74ee94ac, // Crypte des Maudits, La (1991)(Lankhor)(fr)(Side B).dsk
   0x7fe22387, // Cyber Power (1992)(CPC Infos)(fr)[cr].dsk
   0xedeb95a4, // Canadair (1987)(Fil)(fr).dsk
   0x24971ea4, // Cop-Out (1986)(Mikrogen).dsk
   0xf89ab99b, // Cyber-Tennis (1997)(Wendos PDS)[cr].dsk
   0xa55c0498, // Centre Court Tennis (1984)(Amsoft).dsk
   0x871c499c, // Chomper (1984)(Pan Books)[cr].dsk
   0x0ff250a9, // Captain Kidd (1985)(MC Lothlorien)[cr][f].dsk
   0x09d98a34, // Climb-It (1984)(Tynesoft)[tape].dsk
   0x0261b785, // Comando Tracer (1989)(Dinamic Software)(es).dsk
   0x11b9e71d, // Chuckie Egg II (1985)(A&F Software).dsk
   0x3f6799b8, // Croco Magneto (1989)(Public Domain)(fr)[cr].dsk
   0x91808822, // Chamonix Challenge (1987)(Infogrames).dsk
   0xc74ff3de, // Combat Lynx (1985)(Durell Software).dsk
   0x2221ad10, // Cuatro en Raya, Las (1988)(Softbasco)(es)[cr].dsk
   0x6d7a58c4, // Crime Parfait (1987)(Logipresse)(fr)[cr].dsk
   0x0cd769a6, // Capitan Sevilla (1988)(Dinamic Software)(es).dsk
   0xa5c893d8, // Crawler (1986)(Amstrad Computer User)[cr].dsk
   0xf4ce35a8, // Crimen Perfecto, El (1987)(Logipresse)(es)[cr].dsk
   0x7b270970, // Cauldron (1985)(Palace Software).dsk
   0xa2996d21, // Cauldron (1985)(Palace Software)(fr).dsk
   0x386f4534, // Campeones Del Mundo De Rallyes (1985)(Indescomp)(es).dsk
   0x3e29a536, // Chickin Chase (1985)(Firebird)[cr].dsk
   0xaf8f2d05, // CORE (1986)(A&F Software)[cr].dsk
   0x84acf7c6, // Caves of Darkness (1991)(Emma Kingsmill)(PD)[cr].dsk
   0x6f367915, // Cursed be the City (1987)(The Adventure Workshop)[cr].dsk
   0x4396b4c3, // Classic Racing (1985)(Amsoft)[tape].dsk
   0x4696b247, // Crystann (1987)(Softbook)(fr)[cr].dsk
   0x24de4278, // Casanova (1989)(Iber Soft)(es).dsk
   0x5342f9a5, // Ci-U-Than Trilogy II - Los Templos Sagrados (1991)(Aventuras AD)(es).dsk
   0x8828c438, // Caves of Bewbews (1990)(Amstrad Adventure PD Library)(PD)[cr].dsk
   0x2669032b, // Camelot Warriors (1986)(Dinamic Software)(es)[cr].dsk
   0xd415b403, // Castle Master II - The Crypt (1990)(Incentive)[tape].dsk
   0x82be3d6c, // Crazy Brick (1987)(Ingo Bordach)(de)[cr].dsk
   0x9e263bd7, // Combat School (1987)(Ocean Software).dsk
   0xf5330657, // Codename Mat II (1984)(Domark).dsk
   0x353140d0, // Coloric (1984)(Free Game Blot)(fr)[cr].dsk
   0x234bea84, // Critical Mass (1985)(Durell Software)[cr][f].dsk
   0xf6158b1f, // Cine Clap (1986)(Ubi Soft)(fr)(Side B).dsk
   0x212f7414, // Cine Clap (1986)(Ubi Soft)(fr)(Side A).dsk
   0xe69a3242, // Cobra Force (1989)(Players Premier)[tape].dsk
   0x5d21b329, // Cutthroats (1984)(Infocom)[CPM].dsk
   0xe83cdc69, // Clever & Smart (1988)(Magic Bytes)(M3).dsk
   0xed5c028f, // Clever & Smart (1988)(Magic Bytes)(es).dsk
   0xc0cdbf41, // Cosmic Shock Absorber (1987)(Martech).dsk
   0x7bc607ba, // Crazyfruit (1998)(Wendos PDS)[cr].dsk
   0x84396a9a, // Crazy Cars 3 (UK) (1992) [Original].dsk
   0xcf430cd0, // Cosmos (1989)(Amstar & CPC)[cr].dsk
   0x9403589a, // Covenant, The (1985)(PSS)[cr].dsk
   0xd98ef21a, // Crazy Snake (1992)(Fraggle and Duck)(PD)[cr].dsk
   0x20d4b44a, // Cosmo (1986)(Infogrames)(fr)[cr].dsk
   0xb66fe575, // Count Duckula 2 (1992)(Alternative Software)[cr].dsk
   0x8564c2d7, // Championship Sprint (1988)(Activision).dsk
   0xefd57b7f, // Crazy Cars (1987)(Titus).dsk
   0x329ce832, // Caves of Doom (1985)(Mastertronic)[tape].dsk
   0x13b3edcf, // Carson City (1986)(Elice)(fr)[cr].dsk
   0xac748c5b, // Chase H.Q. (1989)(Ocean).dsk
   0xf1ce4daf, // Cessna Over Moskow (1987)(Cobra Soft).dsk
   0xead34ad9, // Cobra (1985)(Cobra Soft)(fr)[tape].dsk
   0x471586fa, // Comet Encounter (1986)(LiveWire Software)[cr].dsk
   0x017234a5, // Crossfire (1990)(Atlantis Software)[cr].dsk
   0x51bc68f5, // Commonwealth Games (1986)(Tynesoft)[cr].dsk
   0x80945c23, // Classic Adventure (1984)(Amsoft)[cr].dsk
   0xd5bad0a4, // Crystal Theft (1984)(Wicca Soft)[tape].dsk
   0xdc3c3cee, // Coloriage - Les Petits Malins I (1988)(Carraz Editions)(fr)(Side A).dsk
   0x5f421ae2, // Coloriage - Les Petits Malins I (1988)(Carraz Editions)(fr)(Side B).dsk
   0x80f7d82e, // Ci-U-Than Trilogy I - La Diosa de Cozumel (1990)(Aventuras AD)(es).dsk
   0x26273e82, // Cisco Heat (1991)(Image Works).dsk
   0x4a8fb3d4, // Caverns of Mars (1985)(-)[cr].dsk
   0xe580e156, // Castle Adventure Simulator (1993)(Rantanplan)(PD)(de)[cr].dsk
   0x504d8d95, // Cobra (1987)(Loriciels)(fr).dsk
   0x97ef2187, // Codename Mat (1984)(Amsoft).dsk
   0xfca8c1bc, // Colossus 4 Bridge (1986)(CDS Software LTD)(Side A).dsk
   0x9281bb35, // Colossus 4 Bridge (1986)(CDS Software LTD)(Side B).dsk
   0xfe8f45a0, // Castle of the Skull Lord (1984)(Samurai)[cr].dsk
   0x398a4cb7, // Cybor (1987)(Softhawk)(fr).dsk
   0x180c764c, // Chibi Akumas Episode 2 - Confrontation (2017)(Keith Sear)(Disk 2 Side B).dsk
   0x167edc38, // Chibi Akumas Episode 2 - Confrontation (2017)(Keith Sear)(Disk 1 Side B).dsk
   0x57005904, // Chibi Akumas Episode 2 - Confrontation (2017)(Keith Sear)(Disk 2 Side A).dsk
   0xf50b5634, // Chibi Akumas Episode 2 - Confrontation (2017)(Keith Sear)(Disk 1 Side A).dsk
   0xa2d0ad15, // Chain Reaction (1988)(Elite Systems)(Side B).dsk
   0xf82670fa, // Chain Reaction (1988)(Elite Systems)(Side A).dsk
   0x67520d74, // Chessmaster 2000, The (1990)(Ubi Soft)(es).dsk
   0x94d17670, // Cybernoid (1988)(Hewson).dsk
   0x9f07be43, // Cholo (1986)(Firebird).dsk
   0xabceb000, // Cubit (1984)(Amsoft)(de)[tape].dsk
   0x3e4d2b70, // Cubit (1984)(Amsoft)[tape].dsk
   0xaf529a6a, // Colossus Mahjong (1987)(CDS Software LTD)[CPM].dsk
   0x2bb7f2b9, // Crystal Castles (1986)(US Gold).dsk
   0xe00e41dd, // Captain Blood (1988)(Ere Informatique)(M5)[retail].dsk
   0x94903702, // Country Cottages (1984)(Sterling Software)(es)(Hack)[cr].dsk
   0x7d6f78b7, // Country Cottages (1984)(Sterling Software)[cr].dsk
   0xcc4ce9a1, // Country Cottages (1984)(Sterling Software)(de)[cr].dsk
   0xc089f764, // Car Massacre (1986)(Tilt)(fr)[cr].dsk
   0xbc3d0036, // Circus Games (1989)(Tynesort)(Side B).dsk
   0xa44432b6, // Circus Games (1989)(Tynesort)(Side A).dsk
   0xd3c89c04, // Cylu (1985)(Firebird)[cr].dsk
   0x9a88a784, // Coccinele, La (1988)(AM-Mag)(fr)[cr].dsk
   0x09718d06, // Chip's Challenge (1990)(US Gold).dsk
   0x6e5abb73, // Cyberball (1990)(Domark).dsk
   0x9512280f, // Colossus Backgammon (1985)(CDS Software LTD)[cr].dsk
   0x8e07cbc5, // Classiques Titus Volume 1 (1987)(Titus)(fr).dsk
   0x2653ac21, // Cross Horde (2021)(Caruso, Fabrizio)(PD)[cr].dsk
   0x1b3d0ddd, // Cyberboy (1995)(Amstrad Action)[cr].dsk
   0xa89233ab, // Comando Quatro (1989)(Zigurat Software)(es).dsk
   0xc7701343, // Cavemania (1991)(Atlantis Software).dsk
   0xb52c665d, // Cosa Nostra (1986)(Opera Soft)(es).dsk
   0xca879e1e, // Cosa Nostra (1986)(Opera Soft).dsk
   0x0961027f, // Castle Assault (1985)(CDS Software LTD)[tape].dsk
   0x3f81ef24, // Colony (1987)(Mastertronic)[cr].dsk
   0x3cdbf0c6, // Choy Lee Fut Kung-Fu Warrior (1990)(Positive)(es).dsk
   0x4f93cf60, // Cross Bomber (2021)(Caruso, Fabrizio)(PD)[cr].dsk
   0xd15335c5, // Chubby Gristle (1988)(Grandslam).dsk
   0x7b2efe8b, // Continental Circus (1989)(Virgin Games).dsk
   0x8b330fc7, // Croc' Madam' (1986)(Bug-Byte)(fr).dsk
   0x3ac5dd74, // Chiller (1986)(Mastertronic)[cr].dsk
   0xb431c920, // Carrier Command (1989)(Rainbird Software).dsk
   0xcd4b1856, // Centurions (1987)(Ariolasoft).dsk
   0x125d031b, // Coloriage - Les Petits Malins II (1988)(Carraz Editions)(fr)(Side A).dsk
   0x0b89d8cd, // Coloriage - Les Petits Malins II (1988)(Carraz Editions)(fr)(Side B).dsk
   0x4e9436b2, // Championship Baseball (1987)(Activision).dsk
   0x0823ac51, // Capitan Trueno (1989)(Dinamic Software)(es).dsk
   0x1913117d, // Carlos Sainz (1990)(Zigurat Software)(es).dsk
   0x98f9c4e5, // Caecilia Metella (1994)(Sleepwalker PDS)(PD)[cr].dsk
   0x5deb3022, // Cosmic Sheriff (1989)(Dinamic Software)(es)(LightGun).dsk
   0x998fb414, // California Games (1987)(US Gold)(Side A).dsk
   0x0f16f672, // California Games (1987)(US Gold)(Side B).dsk
   0xd851fde0, // Chibi Akuma's Episode 1 Invasion (2018)(Keith Sear)(v1.666)(Side A).dsk
   0x3d75a099, // Chibi Akuma's Episode 1 Invasion (2018)(Keith Sear)(v1.666)(Side B).dsk
   0xd832dd80, // Chevalier Blanc, Le (1987)(Cobra Soft)(fr).dsk
   0x14e21aee, // Computer Scrabble (1985)(Leisure Genius).dsk
   0x1aa02ae4, // Cricket-Crazy (1988)(The Dreaming Djinn)[cr].dsk
   0x816becd3, // Coliseum (1990)(Amstar & Cpc)[cr].dsk
   0x074ad861, // Cinco Iguales (1991)(Amstrad User Club)(es)[cr].dsk
   0x4f76cb64, // Crack-Up (1989)(Atlantis Software)[tape].dsk
   0xf4c4ad7b, // Curro Jimenez (1989)(Zigurat)(es).dsk
   0xc738216e, // Cabal (1989)(Ocean Software)[b].dsk
   0xcc83f97f, // Clasic Tenis (1989)(Amstrad User Club)[cr].dsk
   0x49c1f6e0, // Cub Bert (1985)(Microbyte).dsk
   0x3fe4b8b2, // Cid, El (1988)(Dro Soft)[cr][f].dsk
   0xdad766c7, // Cid, El (1988)(Dro Soft)(es)[cr][f].dsk
   0x9c78ae17, // Crystal Mission (1991)(Andreas Stroizcek)(PD)(M2)[cr].dsk
   0xaaa35b34, // Chip Factory, The (1984)(Hewson)[cr].dsk
   0x4dca3178, // Corsarios (1989)(Opera Soft).dsk
   0xead3004c, // Corona Magica, La (1990)(OMK Software)(es).dsk
   0x47207274, // Cross Chase (2018)(Caruso, Fabrizio)(PD)[cr].dsk
   0xbb0a782e, // Corridor Conflict (1987)(The Power House)[tape].dsk
   0xd15c383d, // Combat Zone Mentor (1987)(Alternative Software)[cr].dsk
   0x8b65d51a, // Colosal Piramide (1994)(Definitive Software)(es)[cr].dsk
   0x3ef4d51a, // Contraption (1985)(Audiogenic Software).dsk
   0x8d5d68a2, // Computer Scrabble Deluxe (1987)(Leisure Genius).dsk
   0x1f4e18a3, // Centipods (1987)(Computing With The Amstrad)[tape].dsk
   0x12354538, // Chuckie Egg (1985)(A&F Software)[tape].dsk
   0xc80fc7c0, // Charly Diams (1987)(Loriciels)(fr).dsk
   0x864c9951, // Custard Pie Factory (1985)(Tynesoft).dsk
   0xc567eb8a, // Cobra (1987)(Ocean).dsk
   0xeb42ce51, // Cyberbig (1989)(MCM Software).dsk
   0xa961c9ba, // Con-Quest (1986)(Mastertronic)[cr].dsk
   0xeff6c7b7, // Catch 23 (1987)(Martech).dsk
   0x95140ead, // Colour of Magic, The (1986)(Piranha)[cr].dsk
   0xe8e1c72a, // Captain America (1987)(Us Gold).dsk
   0xbe7e8c95, // Cross Shoot (2020)(Caruso, Fabrizio)[cr].dsk
   0x1e7bc1c1, // Cricket Captain (1985)(Allanson Computing)[tape].dsk
   0xce9c1fc3, // Cerberus (1986)(Players)[tape].dsk
   0x4743863a, // Chopper Squad (1985)(Interceptor Software)[cr].dsk
   0xda2ca225, // Ca Va Peter Zorglub (1988)(AM-Mag)[cr].dsk
   0x54d75c4a, // Classic Trainer (1989)(GTI Software)[cr].dsk
   0xe418e803, // Ci-U-Than Trilogy III - Chichen Itza (1992)(Aventuras AD)(es)[b].dsk
   0xd4dcd1ce, // Chimera (1985)(Firebird)[tape].dsk
   0x39aae782, // Cycit (1986)(Bug-Byte Software)(v2)[cr].dsk
   0x791eae64, // Commando (1985)(Elite Systems).dsk
   0x03b5ab32, // Commando - Space Invasion (1985)(Elite Systems)[cr].dsk
   0x8b680531, // Cricket Master (1989)(E&J Software).dsk
   0x9f07df32, // Crazy Cars II (1989)(Titus)[b].dsk
   0xf80df676, // Crazy Cars II (1989)(Titus)[cr][f].dsk
   0x998ecf41, // Colossus Chess 4 (1986)(CDS Software LTD).dsk
   0x658adf06, // CPC Invaders (1993)(SIS Software)(PD)[cr].dsk
   0x5c4fef97, // Coliseum (1988)(Topo Soft)[cr].dsk
   0x9a3cbab6, // Coliseum (1988)(Topo Soft)(es)[cr].dsk
   0x58ba46c3, // Cluedo (1985)(Leisure Genius).dsk
   0xac950a99, // Cubewars (1988)(Computing With The Amstrad)[cr].dsk
   0x32e80769, // Chicago 90 (1989)(Mircroids)(fr).dsk
   0xe1a074d8, // Cap-Man (1985)(Hebdogiciel)(fr)[cr].dsk
   0xd895249a, // Countdown (1987)(Macsen Software)[cr].dsk
   0xf9643529, // Chiffres et des Lettres, Des (1987)(Loriciels)(fr)(Side B)[Juin].dsk
   0x95272ba6, // Chiffres et des Lettres, Des (1987)(Loriciels)(fr)(Side A)[Juin].dsk
   0xc8a5ef6a, // Cauldron II (1986)(Palace Software)(es)[cr].dsk
   0xe69a8a8a, // Cauldron II (1986)(Palace Software)(fr).dsk
   0x003f24cf, // Cauldron II (1986)(Palace Software).dsk
   0x6e991c26, // Classic Muncher (1987)(Bubble Bus Software).dsk
   0x826c72a5, // Case of the Obscene Mural, The (1986)(Encief Routines)[cr].dsk
   0x0af3f96a, // Cycles, The (1989)(Accolade).dsk
   0xfd80301e, // Chinos, Los (1992)(Amstrad Users)(es)[cr].dsk
   0x001dbee4, // Crucial Test (1988)(Coktel Vision)(fr)(Side B)(Q2).dsk
   0xa64dc6a3, // Crucial Test (1988)(Coktel Vision)(fr)(Side A)(Q1).dsk
   0x0035fb2d, // Crazy Shoot (1989)(Loriciels)(fr)(West Phaser)(Side B).dsk
   0x7ce1ca31, // Crazy Shoot (1989)(Loriciels)(West Phaser).dsk
   0xce81cffa, // Crazy Shoot (1989)(Loriciels)(fr)(West Phaser)(Side A).dsk
   0x608ea715, // Crazy Golf (1984)(Amsoft)[tape].dsk
   0xe010f7d4, // Chicago 30's (1988)(Topo Soft).dsk
   0x4c2f8cdf, // Chicago 30's (1988)(Topo Soft)(es)[cr][f].dsk
   0x1b6c09ae, // Captain Planet (1990)(Mindscape).dsk
   0x6587dc17, // Corruption (1988)(Rainbird Software)(Side B).dsk
   0x92ce11b2, // Corruption (1988)(Rainbird Software)(Side A).dsk
   0x647bb62f, // City Slicker (1986)(Hewson).dsk
   0x7177bf58, // Cross Snake (2020)(Caruso, Fabrizio)(PD)[cr].dsk
   0xc48fc661, // Chronos (1987)(Mastertronic)[cr].dsk
   0xd0f4fc5f, // Corona, La (1988)(System 4)(es)[cr].dsk
   0xae25ee36, // Campeonato Del Mundo Boxing Manager (1991)(Goliath Games)(es).dsk
   0x651eaa76, // Castle Blackstar (1984)(CDS Software LTD)[cr].dsk
   0x2c70c790, // Cybernoid II (1988)(Hewson).dsk
   0xed61051f, // Confuzion (1985)(Incentive)[cr].dsk
   0xcb1fb4a2, // Cubemeleon (1986)(CPC)(fr)(v3)[cr].dsk
   0xc2f9ac6c, // Crash Garrett (1987)(ERE Informatique)(fr)(Side B).dsk
   0x76cd8f28, // Crash Garrett (1987)(ERE Informatique)(fr)(Side A).dsk
   0x11864f72, // Can I Cheat the Death (1990)(Amstrad Adventure PD Library)(PD)[cr].dsk
   0xd356f77a, // Crazy Stamps (1986)(D. Mueller)(PD)(de)[cr].dsk
   0x74d37183, // Comet Game, The (1986)(Firebird)[tape].dsk
   0x57eb7d63, // Crepuscule du Naja, Le (1987)(Chip)(fr).dsk
   0xa8a343c0, // Challenge of the Gobots (1987)(Ariolasoft).dsk
   0xa998c6ae, // Convoy Raider (1987)(Gremlin Graphics Software).dsk
   0x5be24f37, // Cobras Arc (1986)(Dinamic Software)(es)[cr][f].dsk
   0xea34c2da, // Death's Ticket (1990)(Micro Mag)[cr].dsk
   0xff9382c9, // Dark Power (1989)(Micro Mag)[cr].dsk
   0x627d56c5, // Dossier G (1985)(Cobra Soft)(fr).dsk
   0x3a35ea93, // Dervish (1989)(The Power House)[tape].dsk
   0x4ef6890a, // Defcom 1 (1989)(Iber Soft)(es).dsk
   0xd1e4eade, // Doppleganger (1985)(Alligata Software)[tape].dsk
   0x4ac6467e, // Danger Street (1987)(Chip)(fr).dsk
   0x8b845864, // Defenders of the Earth (1990)(Enigma Variations)(en-fr).dsk
   0x6b5b7e05, // Drakkar (1989)(Delta Software)(es).dsk
   0x4d6eb9ba, // Darkman (1991)(Ocean)(Side B).dsk
   0xd60d7d79, // Darkman (1991)(Ocean)(Side A).dsk
   0xaa0cf74a, // Defender (1987)(Amstrad Video-Play)(es)[cr].dsk
   0x852b80df, // Dragons of Flame (1990)(US Gold)(Side B).dsk
   0xc519182d, // Dragons of Flame (1990)(US Gold)(Side A).dsk
   0x0b1fc7cc, // Danger Mouse in Double Trouble (1985)(Creative Sparks)[tape].dsk
   0x5e19a278, // Dream Warrior (1988)(US Gold).dsk
   0x53ff9b48, // Dernier Metro, Le (1985)(Micro Programmes 5)(fr)[tape].dsk
   0xc29233ec, // D.C.A. (1991)(CPC Infos)[cr].dsk
   0x76f1d396, // Devilry 2 (1989)(Eric Cubizolle - Emmanuel Rigot)(fr)(Side A).dsk
   0x18464ac0, // Devilry 2 (1989)(Eric Cubizolle - Emmanuel Rigot)(fr)(Side B).dsk
   0xf30f6809, // Digger Barnes (1985)(Cable Software)[cr].dsk
   0x09b7c1f2, // Donkey Kong (1986)(Ocean Software)[BASIC 1.0].dsk
   0xe9bc89c8, // Desperado 2 (1989)(Topo Soft)(es).dsk
   0x9b69dabd, // Demon Attack (1986)(Rakoko)[cr].dsk
   0x60e4e33c, // Darts (1986)(Alligata Software)[cr].dsk
   0xcb00cc73, // Desperado (1987)(Topo Soft)(Side A)[cr][f].dsk
   0xd8584470, // Desperado (1987)(Topo Soft)(Side B)[cr].dsk
   0x245d5514, // Dive-Dive-Dive (1987)(Tynesoft)[cr].dsk
   0x63d1f9fe, // Dynamite Dan II (1986)(Mirrorsoft).dsk
   0x4019916e, // Detective (1984)(Amsoft)[tape].dsk
   0xb4723a22, // Drigus und Drolan (1989)(Bollaware)(PD)(de)[cr].dsk
   0x463cc950, // Dawnssley (1987)(Top Ten)[tape].dsk
   0xa20e81d5, // Deep Strike (1986)(Durell Software)[cr].dsk
   0x7ea86998, // Dr. Doom's Revenge (1989)(Empire Software)(M4)(Side B)[codes].dsk
   0x3443ac53, // Dr. Doom's Revenge (1989)(Empire Software)(M4)(Side A)[codes].dsk
   0x098afd20, // Double Dragon (1988)(Virgin Mastertronic).dsk
   0x1ce66ea0, // Daley Thompson's Supertest (1985)(Ocean).dsk
   0x4b67f27a, // Defi au Tarot (1986)(Run Informatique)(fr).dsk
   0xb0dd3611, // Deathsville (1986)(Bubblebus Software).dsk
   0x5a707d86, // Dragon's Lair (1985)(Software Projects).dsk
   0xc147e40e, // Defend or Die (1985)(Alligata Software)[tape].dsk
   0x0b20dfe5, // Dogsbody to the Rescue (1985)(Bug-Byte Software)[tape].dsk
   0xe9bd1055, // Dizzy II - Treasure Island Dizzy (UK) (1989) [Original].dsk
   0x819a0d81, // Domino (1985)(Idealogic)(es)[tape].dsk
   0x0f1fcc82, // Druids Moon (1987)(Top Ten)[cr].dsk
   0xc00be31a, // Diamant de l'Ile Maudite, Le (1985)(Loriciels)(fr)(Side A).dsk
   0x2c20e400, // Diamant de l'Ile Maudite, Le (1985)(Loriciels)(fr)(Side B).dsk
   0xa0388928, // Daleks (1989)(Amstrad Action)[tape].dsk
   0x6a441867, // Dwarf (1987)(Soft Hawk)(fr).dsk
   0xc2df6ea0, // Dan Dare III - The Escape (1990)(Virgin Games).dsk
   0x58f33108, // Doctor Who and the Mines of Terror (1986)(Micro Power).dsk
   0x76fd75b1, // Donald & Riri (1985)(Bahia Informatique)(fr).dsk
   0xd2fe8234, // Dandy (1987)(Activision).dsk
   0x5956333b, // Demineur (1993)(GPA)[cr].dsk
   0x550ac9f9, // Dynamite Dan (1985)(Mirrorsoft)[tape].dsk
   0x2f36c5cd, // Dynamic Duo (1989)(Firebird).dsk
   0x5c0016ec, // Dungeons, Amethysts, Alchemists (1987)(Atlantis Software)[tape].dsk
   0x2faa32d2, // Dr. Dustbin (1988)(PC Schneider International)(de)[cr].dsk
   0xb4fe5896, // Dustin (1986)(Dinamic Software)(es)[tape].dsk
   0x81115ea3, // Duck Dodgers (1985)(Amstrad Computer User)[cr].dsk
   0xfe44dc2a, // Danger Adventurer at Work Two (1991)(WoW Software)[tape].dsk
   0xae1d8116, // Daley Thompson's Decathlon (1985)(Ocean)[tape].dsk
   0x0662dcbe, // Desert Fox (1986)(US Gold).dsk
   0xb0481e4e, // Dark Star (1985)(Design Design)[tape].dsk
   0x656ec48f, // Dianne (1985)(Loriciels)(fr)[tape].dsk
   0x65f1ce24, // Dan Dare - Pilot of the Future (1986)(Virgin Games).dsk
   0x39a97e44, // Dodgy Geezers (1986)(Melbourne House)[D7][cr].dsk
   0xb1ca3b57, // Dames 3D Champion (1987)(Cobra Soft)(fr).dsk
   0x6e1a4da1, // Donald's Alphabet Chase (1988)(Disney)(M5)(Side B)[codes].dsk
   0x98021e34, // Donald's Alphabet Chase (1988)(Disney)(M5)(Side A)[codes].dsk
   0x1b671eb0, // Dash (1986)(Happy Computer)[cr].dsk
   0x68f975d7, // Duet - Commando '86 (1987)(Elite Systems)[cr].dsk
   0xf807ae09, // Defcom (1986)(Quicksilva)[cr].dsk
   0x7aa486cc, // Death or Glory (1987)(CRL Group).dsk
   0x2037a297, // Dr. Scrimes Spook School (1988)(Mastertronic)[tape].dsk
   0x8c5e3ea2, // Dragon Ninja (1988)(Ocean Software).dsk
   0xe9b8a1f9, // Double Dragon III - The Rosetta Stone (1991)(Storm Software)(Side B).dsk
   0xaed90545, // Double Dragon III - The Rosetta Stone (1991)(Storm Software)(Side A).dsk
   0x8b63fc86, // Devil's Castle (1986)(Chip)(fr).dsk
   0x8d4a1ab8, // Dominoes (1987)(Blue Ribbon Software)[tape].dsk
   0xdc891be2, // Dizzy VII - Crystal Kingdom Dizzy (1992)(Codemasters Software)[cr].dsk
   0x2e044103, // Dr. Jackle and Mr. Wide (1987)(Bulldog)[tape].dsk
   0x7bb8b2c5, // Dora (1988)(Acocsoft)(el)[cr].dsk
   0xc53e8be1, // Dragon Spirit (1989)(Domark)(Side A).dsk
   0x88aa6a8e, // Dragon Spirit (1989)(Domark)(Side B).dsk
   0xdc8a705f, // Donde Esta Carmen Sandiego (1990)(Broderbund Software)(es)(Side B)[codes].dsk
   0x02c5b434, // Donde Esta Carmen Sandiego (1990)(Broderbund Software)(es)(Side A)[codes].dsk
   0x2b1b70da, // Dieux de la Mer - Championship Water Skiing (1987)(Infogrames)(fr).dsk
   0x3c89466f, // Diamond Mine (1986)(CDS Software LTD)[tape].dsk
   0x1461c372, // Dame Scanner (1988)(Chip)(fr).dsk
   0x993bb9bd, // Deep, The (1989)(US Gold).dsk
   0x7710bc30, // Double Dragon (1988)(Dro Soft)(es).dsk
   0x3f0806cb, // Death Pit (1984)(Durell Software)[cr].dsk
   0xf0970e2c, // Dark Sceptre (1987)(Firebird Software)[f].dsk
   0x4d4620dc, // Deadline (1982)(Infocom)[CPM].dsk
   0x3c2111e0, // Dragon World (1989)(4-Mation Educational Resources)(Side B)[cr].dsk
   0xc2c59a45, // Dragon World (1989)(4-Mation Educational Resources)(Side A)[cr].dsk
   0xc9714dda, // Don't Panic (1985)(Firebird Software)[tape].dsk
   0x6b6b4872, // Dripzone (1987)(Potplant Productions)[cr].dsk
   0x791127ee, // Dracula (1986)(CRL Group).dsk
   0x596a99e0, // Dark Side (1988)(Incentive Software).dsk
   0x7a816ade, // Dead or Alive (1987)(Alternative Software)[tape].dsk
   0x9b38a47e, // Double Dragon II - The Revenge (1989)(Dro Soft)(es).dsk
   0x5f3afbed, // Death Wish 3 (1987)(Gremlin Graphics Software).dsk
   0x694032c2, // Diamond Trap (1987)(Computing With The Amstrad)[cr].dsk
   0x1d1e61db, // Dark Fusion (1988)(Gremlin Graphics Software).dsk
   0x0e1170a6, // Devil's Crown, The (1985)(Probe Software).dsk
   0x3b58b3e6, // D Day (1992)(Loriciels)(fr)[cr].dsk
   0x7182f979, // Dynasty Wars (1990)(US Gold).dsk
   0x98fcd2a5, // Danger Mouse in Making Whoopee! (1985)(Creative Sparks)[tape].dsk
   0xd6e9e35b, // Dragontorc (1985)(Hewson)[tape].dsk
   0xd9dc8f56, // Dragon Breed (1989)(Activision).dsk
   0xc9121001, // Dizzy - The Ultimate Cartoon Adventure (UK) (1988) [Original][b].dsk
   0x5e98fe2a, // Danger Adventurer at Work (1991)(WoW Software)[tape].dsk
   0x2e5eefce, // Doomlords (1988)(Wow Software)[cr].dsk
   0xa3bc8bfe, // Dom Camillo (1987)(Free Game Blot)(fr)[cr].dsk
   0x4065e278, // Dizzy VI - Prince of the Yolkfolk (1991)(Codemasters Software)[cr].dsk
   0x3105977f, // Devil Highway (1987)(Infomedia)[cr].dsk
   0x06e534dc, // Dragon's Lair II - Escape from Singe's Castle (1987)(Software Projects).dsk
   0xcceea209, // Don Quijote (1987)(Dinamic Software)(es).dsk
   0xfd300f5f, // Dogfight 2187 (1987)(Starlight Software).dsk
   0xc67212be, // Double Dragon II - The Revenge (1989)(Virgin Mastertronic).dsk
   0xd50d3b34, // Deadly Evil (1990)(Players Premier Software)(v2)[cr].dsk
   0xe041b78b, // Dam Busters, The (1986)(US Gold).dsk
   0x78f2ce58, // Dynamite Dux (1989)(Activision)(Side B).dsk
   0xb9d58e05, // Dynamite Dux (1989)(Activision)(Side A).dsk
   0x61a1bea2, // Deflektor (1987)(Gremlin Graphics Software).dsk
   0x2d114ead, // Duct, The (1988)(Gremlin Graphics Software)[tape].dsk
   0x3d624316, // Doomsdark's Revenge (1985)(Beyond Software)[tape].dsk
   0x73cff695, // Driller (1987)(Incentive Software)[cr].dsk
   0x24a54215, // Driller (1987)(Incentive Software)[codes].dsk
   0x260957b5, // Doodle Bug (1987)(Players Software)[cr].dsk
   0x8ccfb58e, // Dun Darach (1985)(Gargoyle Games).dsk
   0xabab1a11, // Dakar 4x4 (1987)(Coktel Vision)(fr)[cr].dsk
   0xa9e39d27, // Dynamix (1989)(Virgin Games)[tape].dsk
   0x13e32ec7, // Darkwurlde (1986)(Top Ten)[tape].dsk
   0x5bc860de, // Deactivators (1986)(Ariolasoft).dsk
   0x8ab1f797, // Dick Tracy (1990)(Titus).dsk
   0x2fa0073a, // Desolator (1986)(US Gold).dsk
   0xed93f172, // Dan Dare II - Mekon's Revenge (1988)(Virgin Games)[cr][f].dsk
   0xc04b912b, // Dan Dare II - Mekon's Revenge (1988)(Virgin Games)[b].dsk
   0x92a90631, // Dragon's Gold (1985)(Amsoft)[cr].dsk
   0x2c302745, // Dents de sa Mere, La (1986)(Transoft)(fr)[tape].dsk
   0x79d46a26, // Dominator (1989)(Electric Dreams Software).dsk
   0xdb63f848, // Enterprise v1.0 (1985)(Data Media)(de).dsk
   0x705055f9, // Enchanted (1989)(Positive)(es).dsk
   0x76f67d86, // Enduro Racer (1987)(Activision).dsk
   0x3d86c8c2, // Escape From The Planet Of Doom (1990)(Amstrad Adventure PD Library)[cr].dsk
   0xe6e831d8, // Eidolon, The (1986)(Activision).dsk
   0xbea63f11, // Elevator Action (1987)(Quicksilva).dsk
   0x002eeb53, // Escape (1985)(Entertainment & Computer Products)[cr].dsk
   0x0cca0759, // Enchanter (1984)(Infocom)[CPM].dsk
   0x67c371f3, // E-Motion (1990)(US Gold).dsk
   0x6c18db2f, // Evening Star (1987)(Hewson).dsk
   0x76132797, // Eve of Shadows (1991)(Amstrad Adventure PD Library)(PD)[cr][b].dsk
   0x14733dcc, // Energy Warrior (1987)(Mastertronic)[cr].dsk
   0x16dfaedf, // Etrange Castel (1990)(Fanatic)(fr)(pd)[cr].dsk
   0x334ea46a, // Elite (1985)(Firebird)(Side B)[GOLD].dsk
   0x327572f5, // Elite (1985)(Firebird)(Side A)[GOLD].dsk
   0xfa9a2d49, // Elite (1985)(Firebird)(fr)[cr][GOLD].dsk
   0x14859082, // Elite (1985)(Firebird)(de)[GOLD].dsk
   0x22787a91, // Esgrima Electrico (1985)(Amstrad Computer User)(es)[cr].dsk
   0x69219b4f, // Exolon (1989)(Hewson).dsk
   0x1c4a7ef4, // Erebus (1987)(Titus)(fr).dsk
   0xb4ba020b, // Endzone (1989)(Alternative Software)[tape][f].dsk
   0x773a2875, // Eliminator (1988)(Hewson)[b].dsk
   0x98730b35, // Express Raider (1987)(US Gold).dsk
   0xdfcfd017, // Equinox (1986)(Mikro-Gen).dsk
   0x7ba07120, // Explorer (1987)(Activision).dsk
   0xb66e94c0, // Escape From Khoshima (1986)(Atlantis Software)[cr].dsk
   0x077f567e, // Experience, The (1985)(Players)[tape].dsk
   0x4995c437, // Electric Runner (1985)(Hebdogiciel)(fr)[cr].dsk
   0x97a74eea, // Eva Strip Word (1988)(Amstrad Cent Pour Cent)(fr)[cr].dsk
   0xbe934e4d, // Eagle's Rider (1990)(Microids)[fr].dsk
   0x773113a3, // Everyone's a Wally (1985)(Mikro-Gen)[tape].dsk
   0xbefb9b11, // Escoba (19xx)(Marcsoft)(es)[cr].dsk
   0x9b7f3579, // Extreme (1991)(Digital Integration).dsk
   0x7a74e185, // Enlightenment - Druid II (1988)(Firebird).dsk
   0x90981904, // Electric Fencing (1985)(Amstrad Computer User)[cr].dsk
   0x1666d286, // European II - The European Cup (1989)(E&J Software)(es)(Hack)[cr].dsk
   0xbd571db5, // European II - The European Cup (1989)(E&J Software)[cr].dsk
   0x599212bd, // Evasion (1987)(Alain Massoumipour)(fr)(Side B)[cr].dsk
   0x525cb704, // Evasion (1987)(Alain Massoumipour)(fr)(Side A)[cr].dsk
   0x3befe4ce, // Elidon (1985)(Orpheus)[tape].dsk
   0xdaf5b8f7, // European Superleague (1990)(CDS Software LTD)(fr)[cr].dsk
   0xc00d99db, // European Superleague (1990)(CDS Software LTD).dsk
   0x510fea74, // European Superleague (1990)(CDS Software LTD)(es).dsk
   0xb0b9278e, // Ensembles, Les (1986)(Eurogiciel)(fr)[cr].dsk
   0xbc8270d3, // Erbschaft, Die (1986)(Infogrames)(de).dsk
   0xef8fdc30, // Escape from Planet of Robot Monsters (1990)(Domark).dsk
   0x71dd6753, // Every Second Counts (1988)(Domark).dsk
   0x368e910b, // Evasion (1985)(Entertainment & Computer Products)(fr)[cr].dsk
   0x21b5a146, // Eye (1987)(Endurance Games).dsk
   0x4f86b3c8, // Emilio Butragueno Futbol (1988)(Topo Soft)(es).dsk
   0x62453c06, // Er-Bert (1984)(Microbyte)[cr].dsk
   0xce112c59, // Endurance (1985)(CRL Group)[tape].dsk
   0x59a3cd11, // Edd the Duck (1990)(Zeppelin Games).dsk
   0x25ce4a0d, // Exploding Wall (1989)(Byte Back)[tape].dsk
   0x27b6254e, // European Champions (1990)(E&J Software)[cr].dsk
   0x1b56b252, // Emilio Butragueno Futbol II (1989)(Erbe Software)(es).dsk
   0x5795493d, // Espada Sagrada, La (1990)(Topo Soft)(es).dsk
   0xc2927916, // Evil Donjon (1989)(Genesis Soft).dsk
   0xa4f72e8d, // Echelon (1988)(US Gold).dsk
   0x8aefef48, // Elektra Glide (1986)(English Software).dsk
   0xe31d60df, // Emilio Sanchez Vicario (1989)(Zigurat Software)(es).dsk
   0x5d87fc09, // Emerald Isle (1985)(Level 9 Computing)[tape].dsk
   0x1b9045bd, // Espionage (1988)(Grandslam).dsk
   0xac0091ea, // Excalibur Quest (1988)(Excalibur)(fr)(Side A).dsk
   0xec2a7e20, // Excalibur Quest (1988)(Excalibur)(fr)(Side B).dsk
   0xf76d2da6, // Electro Freddy (1984)(Amsoft).dsk
   0x4897ca99, // Emlyn Hughes International Soccer (1988)(Audiogenic Software LTD).dsk
   0x21c8c7c7, // Exterminator (1991)(Audiogenic Software LTD).dsk
   0x9187899c, // Exchange (1986)(Cascade Games)[cr].dsk
   0x893a5fc8, // Escape from Wacci (1989)(Potplant Production)(PD)[cr].dsk
   0x577cf898, // Fres Man (1990)(Bollaware)(de)[cr].dsk
   0x887d173f, // Freddy Hardest in South Manhattan (1989)(Dinamic Software)(es).dsk
   0x3094c97e, // Firelord (1986)(Hewson).dsk
   0x28f0fc6e, // Final Fight (1991)(US Gold).dsk
   0x3978c90e, // Flimbo's Quest (1990)(System 3)(Side A).dsk
   0x2a36c58b, // Flimbo's Quest (1990)(System 3)(Side B).dsk
   0x6ca6e53d, // Fucki Demo (1990)(CPC Infos)(fr)[cr].dsk
   0xc2bc35c3, // Five A Side Soccer (1986)(Mastertronic)[cr].dsk
   0x55d399c6, // Formula 1 Simulator (1985)(Mastertronic)[cr][f].dsk
   0x08f2f58f, // Frost Byte (1986)(Mikrogen).dsk
   0xff2a8f73, // Fusion II (1988)(Loriciels)(fr).dsk
   0xfe89ad78, // Further Adventures of Fred, The (2020)(Sean McManus)(PD)[cr].dsk
   0x17726e14, // Forestland (1986)(Supersoft).dsk
   0x36618f61, // F-16 Combat Pilot (1991)(Digital Integration)(M3)[codes].dsk
   0xf8c38616, // F1 Tornado Simulator (1991)(Zeppelin Games).dsk
   0xe3ade0bf, // Froggy (1985)(R&B Marketing)[cr].dsk
   0xce13f5d2, // Fleche (1989)(Micro Mag)[cr].dsk
   0xe3f26c39, // Flight Simulation (1984)(Myrddin Software)[cr].dsk
   0x01dd7b07, // Fruit Machine Simulator (1988)(Codemasters Software)[cr].dsk
   0xc79a0c4f, // Flying Shark (1987)(Firebird Software).dsk
   0x778bc097, // Frestris v1.1 (1995)(Bollaware)(PD).dsk
   0x2a6a6691, // ForceField (19xx)(David Hall)(PD)[cr].dsk
   0x7f03a79b, // F-15 Strike Eagle (1986)(Microprose Software)[codes].dsk
   0x45d91d85, // F-15 Strike Eagle (1986)(Microprose Software)[cr].dsk
   0xbf8a83cf, // Flunky (1987)(Piranha)(M5).dsk
   0xec6a5258, // Fairlight - A Prelude (1985)(The Edge).dsk
   0xc9a350f6, // Football Manager 2 (1988)(Addictive Games).dsk
   0xb46403ae, // Football Manager 2 (1988)(Addictive Games)(es).dsk
   0x43e60a93, // Football Manager 2 (1988)(Addictive Games)(fr)[cr].dsk
   0xe5facd84, // Flight Path 737 (1984)(Anirog Software)(M5)[cr].dsk
   0xc3997a3d, // Fred (1984)(Zigurat)(es)[cr][f].dsk
   0x45b5d65d, // Fantomas (1986)(Hebdogiciel)(fr)[cr].dsk
   0x65528221, // Froggit (1987)(Happy Computer)[cr].dsk
   0x47dce9b2, // Forces (1987)(PC Schneider International)[cr].dsk
   0x3eea51e6, // Football Frenzy (1987)(Alternative Software)[cr].dsk
   0xff560a03, // Final Matrix, The (1987)(Gremlin Graphics Software).dsk
   0xea67f87e, // Frankenstein (1987)(CRL Group).dsk
   0x81ec6b2e, // Fighting Soccer (1989)(Activision).dsk
   0xdd846c34, // Firefox (1985)(Home Computing Weekly)[cr].dsk
   0x8317c75a, // Fantasia Diamond (1984)(Hewson)[cr].dsk
   0x1aabb041, // Fantasy Land (1986)(Cascade Games)[cr].dsk
   0x411d6548, // Fourtris (1988)(CPC Computing)[cr].dsk
   0x18b560ea, // Freedom Fighter (1988)(The Power House)[cr].dsk
   0xc73dd7e4, // Fruit Machine (1984)(Amsoft)[tape].dsk
   0x2ebe34c9, // Future Knight (1986)(Gremlin Graphics Software).dsk
   0x45eb3208, // Future Shock (1987)(Tynesoft Computer System)[cr].dsk
   0x48cdf786, // Famous Five On Treasure Island, The (1990)(Enigma Variations).dsk
   0x7b648261, // Firefox 2 (1987)(Yie Ar Soft)[cr].dsk
   0xef6a6ff5, // Fighting Warrior (1985)(Melbourne House)[cr].dsk
   0x993226d2, // Fighter Command (1986)(Cascade Games)[cr].dsk
   0x573d30b4, // Futur Racer (1989)(Amstar & Cpc)(fr)[cr].dsk
   0x0e55de6f, // Firestone, The (1990)(Amstrad Adventure PD Library)(PD)[cr].dsk
   0x0669d006, // Fruity Frank (1984)(Kuma Computers)(de)[cr].dsk
   0x114dce4a, // Fruity Frank (1984)(Kuma Computers).dsk
   0x71016c13, // Fruity Frank (1984)(Kuma Computers)(es)[cr].dsk
   0xdf940198, // Fu-Kung in Las Vegas (1985)(Amsoft).dsk
   0xc07ee7b1, // Fallout (1987)(Amstrad Action Magazine)[cr].dsk
   0xa47e0e62, // Fruity (1988)(Amstar & CPC)(fr)[cr].dsk
   0xf9ac5798, // Fire (1990)(New Deal Productions).dsk
   0xd6361b0e, // Formula One (1985)(CRL Group)[cr].dsk
   0x8079a844, // Fire and Forget II (1990)(Titus)[cr].dsk
   0x0b296a53, // Fourtron (1986)(Rafael Garcia Gonzales)(es)[cr].dsk
   0x43d4b751, // Fire and Forget (1988)(Titus).dsk
   0x9c474afb, // Footballer of the Year (1986)(Gremlin Graphics Software).dsk
   0xaed93596, // Fruit Machine 2 Simulator (1990)(Codemasters Software)[cr].dsk
   0xe03a48b5, // Foso Del Dragon, El (1986)(Geasa)(es)[cr].dsk
   0x0b09c221, // Feud (1987)(Mastertronic)[cr].dsk
   0x9b10bce1, // Finders Keepers (1985)(Mastertronic)[cr].dsk
   0xbe873eae, // Forest at World's End, The (1985)(Interceptor Software)[cr].dsk
   0x4524af85, // Fire Land (1991)(CPC Infos)(v2)[cr].dsk
   0x7a8fe41d, // Freddy Hardest (1987)(Dinamic Software).dsk
   0x874e9bb6, // Freddy Hardest (1987)(Dinamic Software)(es).dsk
   0x9f51a10d, // Fiendish Freddy's Big Top O' Fun (1990)(Mindscape)(Side B)[codes].dsk
   0xdf73e59f, // Fiendish Freddy's Big Top O' Fun (1990)(Mindscape)(Side A)[codes].dsk
   0x9e80ee3a, // Friss Man (1985)(Amsoft)[cr].dsk
   0xa526ba0e, // Frankie Goes to Hollywood (1986)(Ocean Software)[cr].dsk
   0x508f2885, // Firetrap (1987)(Activision).dsk
   0x5f933903, // Fighter Pilot (1985)(Digital Integration)(M3).dsk
   0xcc93182d, // For Gold or Glory (1988)(Alternative Software)[cr][f].dsk
   0x53f452e0, // Frank Bruno's Boxing (1985)(Elite Systems).dsk
   0x2c1e44ad, // Football Manager (1984)(Addictive Games)[cr].dsk
   0x9b449b3b, // Freedom (1988)(Coktel Vision)(fr)(Side A).dsk
   0xdc40fffa, // Freedom (1988)(Coktel Vision)(fr)(Side B).dsk
   0x900e99b6, // Flight Simulator (1987)(Amstrad Magazine)(fr)[cr].dsk
   0x96e2374e, // Fres Attack (1991)(Bollaware)(de)(PD)[cr].dsk
   0xfbdf3208, // Football Manager 3 (1992)(Addictive Games).dsk
   0x04ef1030, // Fire Ant (1984)(Mogul)[cr].dsk
   0x6f473e32, // Frankie Crashed on Jupiter (1985)(Kingsoft)(Side B).dsk
   0x165bf3ea, // Frankie Crashed on Jupiter (1985)(Kingsoft)(Side A).dsk
   0xca3cb7df, // Fast Food Dizzy (1989)(Codemasters Software).dsk
   0x24736392, // Flash Gordon (1988)(Mastertronic)[cr][b].dsk
   0x93dfda13, // Foret Infernale, La (1985)(Strategy Software)(fr)[cr].dsk
   0xeb63b31b, // Fres Fighter II Turbo v1.5 (1999)(Bollaware)(PD)(Disk 1 Side A)[cr].dsk
   0xf8bb19be, // Fres Fighter II Turbo v1.5 (1999)(Bollaware)(PD)(Disk 1 Side B)[cr].dsk
   0x56eff60d, // Fres Fighter II Turbo v1.5 (1999)(Bollaware)(PD)(Disk 2 Side A)[cr].dsk
   0x58e3e90c, // Fres Fighter II Turbo v1.5 (1999)(Bollaware)(PD)(Disk 2 Side B)[cr].dsk
   0xc61ff80e, // Frank N Stein (1985)(Amsoft)[cr].dsk
   0xd2671f44, // Foso, El (1985)(Jasap Software)(es)[cr].dsk
   0xb74e5f17, // Forgotten Worlds (1989)(US Gold).dsk
   0x70756ff6, // Flintstones, The (1988)(Grandslam).dsk
   0xb145f198, // Fumiga (1985)(Indescomp)[cr].dsk
   0xb685fd1d, // Fortune En Mer Du Nord (1985)(Kuma Computers)(fr)[cr].dsk
   0x35f2a29e, // Flash (1987)(Loriciels).dsk
   0x27d02694, // Fantastic Voyage, The (1985)(Amsoft).dsk
   0x95a5888b, // Friday The 13th - Viernes 13 (1986)(Domark).dsk
   0x9ea39068, // Force Quatre (1985)(Cobra Soft)(fr)[cr].dsk
   0xb64fe91d, // Fernandez Must Die (1988)(Image Works).dsk
   0xecf5c310, // Firescape (1987)(FIL)(fr).dsk
   0x1b366599, // Frontiers (1988)(Zafiro Software Division)(es).dsk
   0x66ac0bb2, // Fighter Bomber (1990)(Activision)(Side A).dsk
   0xdfe64e49, // Fighter Bomber (1990)(Activision)(Side B).dsk
   0xa5ef45ae, // Fox (1985)(Hebdogiciel)(fr)[cr].dsk
   0xb7609628, // Fairlight - A Trail Of Darkness (1986)(The Edge)[cr][f].dsk
   0x708351db, // Fernando Martin Basket Master (1987)(Dinamic Software)(es).dsk
   0x7af79096, // Footballer of the Year 2 (1989)(Gremlin Graphics Software).dsk
   0x2cdc32b5, // Fury, The (1988)(Martech Games).dsk
   0x0cc8bc65, // Football Manager World Cup Edition (1990)(Addictive Games)(de).dsk
   0x52f0add7, // Football Manager World Cup Edition (1990)(Addictive Games).dsk
   0xac7604a3, // Football Manager World Cup Edition (1990)(Addictive Games)(es)[cr].dsk
   0x2c2599ba, // Football Manager World Cup Edition (1990)(Addictive Games)(fr)[cr].dsk
   0xcbf78b75, // Griffes de la Nuit, Les (1993)(Bruno Fonters)(fr)(Side B)[cr].dsk
   0x12a1efea, // Griffes de la Nuit, Les (1993)(Bruno Fonters)(fr)(Side A)[cr].dsk
   0xbae9bb8f, // Green Beret (1986)(Ocean Software).dsk
   0xed14021d, // Games, The - Winter Edition (1988)(US Gold)(Side A).dsk
   0x75f9c549, // Games, The - Winter Edition (1988)(US Gold)(Side B).dsk
   0x20dcbf76, // Gabrielle (1987)(Ubi Soft)(fr).dsk
   0x080c97e6, // Golden Basket (1990)(Opera Soft)(es)[codes].dsk
   0xd3ce1181, // Golden Basket (1990)(Opera Soft)(es)[cr].dsk
   0x98aa854b, // Gnome Ranger (1987)(Level 9 Computing)(Side B).dsk
   0xb0a79287, // Gnome Ranger (1987)(Level 9 Computing)(Side A).dsk
   0x38a6771e, // Guillermo Tell (1989)(Opera Soft)(es)(LightGun).dsk
   0x1f01e07c, // Gob (1987)(TJC)[cr].dsk
   0x0edae7dd, // Grand Prix 500cc (1986)(Microids)(fr).dsk
   0xdad8d078, // Grand Prix 500cc (1986)(Microids)(es)[cr].dsk
   0x6e0e3657, // Grand Prix 500cc (1986)(Microids)[cr].dsk
   0xc978c90f, // Gun Dogs (1985)(Andromeda Software)[cr].dsk
   0x5d00c766, // Gilbert - Escape from Drill (1989)(Alternative Software)[tape].dsk
   0xb78bd384, // Great Giana Sisters, The (1988)(Rainbow Arts) (Prototype)[cr][f].dsk
   0xb6e80000, // Goonies, The (1986)(US Gold)[cr].dsk
   0xfdaf20b2, // Goldrush (1989)(PC Amstrad International)[cr].dsk
   0x6d45f398, // Galivan (1986)(Ocean).dsk
   0x4d723f39, // Game Over (1987)(Dinamic Software)(es).dsk
   0x8d8e3122, // Game Over (1987)(Dinamic Software).dsk
   0x31b561e4, // Get Dexter 2 (1988)(ERE Informatique).dsk
   0x79777618, // Gazza's Super Soccer (1990)(Empire Software)(es).dsk
   0xe4c3cfce, // Gazza's Super Soccer (1990)(Empire Software)(fr).dsk
   0x674fbced, // Galactic Plague, The (1984)(Amsoft)[cr].dsk
   0x36c70444, // Grand Prix Master (1988)(Dinamic Software).dsk
   0x5b5bb1bf, // Goliath (1986)(Rainbow Productions)(fr)[tape].dsk
   0x0d7c84b0, // Gazza II (1990)(Empire Software).dsk
   0x429f78f6, // Guild of Thieves, The (1988)(Rainbird Software)(v2)(Side A).dsk
   0xe9ddd253, // Guild of Thieves, The (1988)(Rainbird Software)(v2)(Side B).dsk
   0xd853a63d, // Gridrider (1990)(PC Amstrad International)[cr].dsk
   0x8960a2c9, // Gauntlet - Deeper Dungeon (1987)(US Gold)(Extension).dsk
   0xc8149ef6, // Ghostbusters (1986)(Activision).dsk
   0xc057aaa8, // Guzzler (1986)(Players)[cr].dsk
   0x801b1081, // Ghouls (1985)(Micro Power)[cr].dsk
   0xf10ee3fc, // Games, The - Summer Edition (1989)(US Gold)(Side A).dsk
   0x271a1461, // Games, The - Summer Edition (1989)(US Gold)(Side B).dsk
   0xbbc67708, // Glen Hoddle Soccer (1985)(Amsoft)(es).dsk
   0x83eb38d4, // Guerra de las Vajillas, La (1988)(Dinamic Software)(es).dsk
   0x746d9652, // Game Over II (1988)(Dinamic Software).dsk
   0xd5e3e43f, // Galaxian's Revenge (1984)(Amstrad Computer User)[cr].dsk
   0x287abc03, // Guardian (1986)(Your Computer)(es)[cr].dsk
   0xf89cc225, // Gauntlet II (1988)(US Gold).dsk
   0xa4bc521a, // Gorbaf el Vikingo (1987)(Software Center)(es)[cr].dsk
   0xd2bff6bd, // Gems of Stradus (1984)(Amsoft)[tape].dsk
   0xde622d6e, // Gutter (1985)(ERE Informatique)[tape].dsk
   0x8d478d60, // Great Escape, The (1986)(Ocean).dsk
   0x860c4bbb, // Greyfell (1987)(Starlight Software)[b].dsk
   0xa2b01221, // Gryzor (1987)(Ocean).dsk
   0x39dbbb77, // Guadalcanal (1987)(Activision).dsk
   0xd649f19f, // Gold Hunter (1987)(Schneider Magazin)[cr].dsk
   0x1eb29361, // GI Hero (1988)(Firebird).dsk
   0x445c6c5d, // Galactic Games (1987)(Activision)(Side B).dsk
   0xe963b88f, // Galactic Games (1987)(Activision)(Side A).dsk
   0x35cbfeb9, // Galaxias (1986)(Delta 4 Software)[cr].dsk
   0xbc911bd9, // Goody (1987)(Opera Soft)(es)[cr][f].dsk
   0x795638dc, // Goody (1987)(Opera Soft)(es)[b].dsk
   0xf74fc16d, // Grand Prix Simulator (1987)(Codemasters Software)[cr][f].dsk
   0x6c191159, // Garfield - Winter's Tail (1990)(The Edge Software).dsk
   0xd050c94e, // Ghosts (1985)(Cascade Games)[cr].dsk
   0xc8d7f575, // Ghouls 'n' Ghosts (1989)(US Gold).dsk
   0xe3ffbe7b, // Geoff Capes Strong Man (1985)(Martech)[tape].dsk
   0x2e4a23b2, // Galachip (1985)(Chip)(fr)[cr].dsk
   0x2cb38fd9, // Gremlins (1985)(Adventure International)(es)[cr].dsk
   0xe7c2bbbb, // Gremlins (1985)(Adventure International)(de)[cr].dsk
   0x880c2a34, // Gremlins (1985)(Adventure International)[cr].dsk
   0x8ceb4fa3, // Ghost 'n Goblins (1986)(Elite Systems)(v2).dsk
   0xd4279f25, // Galaxy Force (1989)(Activision).dsk
   0x8e1d4e1d, // Guardian Angel, The (1987)(Dinamic Software)[cr].dsk
   0x35e77f98, // Gremlins 2 (1990)(Topo Soft).dsk
   0x363985d0, // Gremlins 2 (1990)(Topo Soft)(es).dsk
   0xca159214, // Gunship (1988)(Microprose Software)(fr)(v3)(Side A).dsk
   0x648096f9, // Gunship (1988)(Microprose Software)(Side B).dsk
   0x43d7ece6, // Gunship (1988)(Microprose Software)(fr)(v3)(Side B).dsk
   0x7f1e18b6, // Gunship (1988)(Microprose Software)(Side A).dsk
   0x1309e3e5, // Gladiator (1986)(Domark)[tape].dsk
   0xe3350811, // Gunslinger (1989)(D. Walsh)[cr].dsk
   0x81459018, // Gary Lineker's Super Star Soccer (1987)(Gremlin Graphics Software)(fr).dsk
   0x9d449456, // Gary Lineker's Super Star Soccer (1987)(Gremlin Graphics Software).dsk
   0x80d513f4, // Grumpy Gumphrey - Super Sleuth (1986)(Gremlin Graphics Software)[cr].dsk
   0x82f434d2, // Glass (1986)(Quicksilva)[tape].dsk
   0x2ed04f45, // Guardians (1991)(Loriciels).dsk
   0x83236163, // Grand Prix Tennis (1988)(Mastertronic)[tape].dsk
   0x6a74fc40, // Gallitron (1987)(Mastertronic)[cr].dsk
   0x33365db0, // Gyroscope (1985)(Melbourne House)[cr].dsk
   0xce1233f5, // Gogly (1986)(Ace Software)(es).dsk
   0x7a636b5b, // Good Moon (1990)(Micro Mag)[cr].dsk
   0x9ee0e36b, // Galaxia (1984)(Kuma Computers)[cr].dsk
   0x8c2dacf2, // Great Gurianos (1987)(Elite Systems)[cr].dsk
   0xe3862c26, // Gauntlet (1985)(Micro Power)[cr].dsk
   0x4f4ed2ca, // Grid Trap (1985)(LiveWire Software)[tape].dsk
   0xa6483dd7, // Gravity (1988)(Schneider Magazin)[cr].dsk
   0x1c3b929e, // Gunsmoke (1987)(Topo Soft)(Side B).dsk
   0xd75d55f7, // Gunsmoke (1987)(Topo Soft)(Side A).dsk
   0xf4ee9de8, // Ghostbusters II (1989)(Activision)(v2).dsk
   0x651b2e79, // Gregory Loses his Clock (1985)(Mastertronic)[tape].dsk
   0x47d2ab2c, // Grell and Falla (1992)(Codemasters Software)[cr].dsk
   0x20786cf2, // Game of Dragons, The (1985)(Amsoft).dsk
   0x7b7ab214, // Golden Axe (1990)(Virgin Games)(Side B).dsk
   0xad37a08f, // Golden Axe (1990)(Virgin Games)(Side A).dsk
   0xe819bc33, // Growing Pains of Adrian Mole, The (1987)(Virgin Games)[tape].dsk
   0x8bd6cfef, // Glider Rider (1987)(Quicksilva).dsk
   0x8f51de4c, // G-Log R360 (1991)(US Gold).dsk
   0xb131ce39, // Ghost Hunters (1987)(Codemasters Software)[tape].dsk
   0xf020ee31, // Grand Prix Simulator 2 (1989)(Codemasters Software)[cr].dsk
   0x2071ce74, // GBA Championship Basketball (1987)(Activision).dsk
   0xdc1f7bd4, // GFL Championship Football (1987)(Activision).dsk
   0x33cf7b85, // Golden Path (1986)(Amsoft)(Side B).dsk
   0x7b14ade8, // Golden Path (1986)(Amsoft)(Side A).dsk
   0xc6d8fd83, // Grand Prix Driver (1984)(Amsoft)[tape].dsk
   0x067924d3, // Gary Lineker's Super Skills (1988)(Gremlin Graphics Software).dsk
   0x966760b8, // Gauntlet III - The Final Quest (1991)(US Gold)(Side A).dsk
   0x4327d6f2, // Gauntlet III - The Final Quest (1991)(US Gold)(Side B).dsk
   0x79911402, // Gothik (1987)(Firebird).dsk
   0x37d695f7, // Gusanin (1985)(Your Computer)(es)[cr].dsk
   0xa1d8563c, // Gary Lineker's Hot-Shot! (1988)(Gremlin Graphics Software).dsk
   0xb458d701, // Gonzzalezz (1989)(Opera Soft)(es)(v2).dsk
   0x6d128285, // Grand Prix 500cc 2 (1991)(Microids).dsk
   0xfe56bf61, // Gunfright (1985)(Ultimate Play The Game)[tape].dsk
   0x1b99bbeb, // Goblin Towers (1986)(Supersoft)[cr].dsk
   0x0900cf93, // Graham Gooch's Test Cricket (1985)(Audiogenic Software LTD).dsk
   0x12851664, // Gilligan's Gold (1984)(Ocean)[tape].dsk
   0xc0c97733, // Gemini Wing (1989)(Virgin Games).dsk
   0x5f83fbdd, // Grange Hill (1987)(Grandslam)(es)[cr].dsk
   0x18d12379, // Galactic Conqueror (1988)(Titus).dsk
   0x751d7ef6, // Genghis Khan (1991)(Positive)[tape].dsk
   0x7c16d340, // Gauntlet (1986)(US Gold)[f].dsk
   0xeebc403d, // Great Courts (1989)(Ubi Soft)[AZERTY][codes].dsk
   0x17258623, // Guerrilla War (1988)(Ocean Software).dsk
   0x193505d0, // Grand Prix Circuit (1990)(Accolade).dsk
   0x6e5c49bc, // Heritage, 'L (1986)(Infogrames)(fr).dsk
   0xb6fcedc6, // Highlander (1986)(Ocean Software)[cr].dsk
   0x4e6c1074, // Hawk Storm (1990)(Players Premier)[cr].dsk
   0x40c18e7e, // Hyper Sports (1985)(Ocean Software).dsk
   0xefa922a7, // Hive (1987)(Firebird).dsk
   0x3d8657f8, // House Of Spiders (1988)(Computing With The Amstrad)[cr].dsk
   0x2aa3ef0f, // Hammerfist (1990)(Activision).dsk
   0x4e784626, // Hunt for Red October, The - Based on the Movie (1991)(Grandslam Entertainments)(Side B).dsk
   0x555335b1, // Hunt for Red October, The - Based on the Movie (1991)(Grandslam Entertainments)(Side A).dsk
   0x522cb62f, // Howard the Duck (1987)(Activision).dsk
   0x7e49a54b, // Histoire d'Or (1987)(Cobra Soft)(fr)(Side B).dsk
   0x53fdb930, // Histoire d'Or (1987)(Cobra Soft)(fr)(Side A).dsk
   0x5256fc2f, // Highway Patrol II (1989)(Microids).dsk
   0xca1ccfa9, // Hair Boy (2016)(cpcretrodev.byterealms)[cr].dsk
   0x134e83f6, // Hibernated 1 This Place Is Death - Directors Cut (2022)(Puddle Soft).dsk
   0x0b6efce2, // Heavy On the Magick (1986)(Gargoyle Games).dsk
   0xb9126a6e, // Human Killing Machine (1989)(US Gold).dsk
   0x593b4abd, // Heroes of the Lance (1988)(US Gold)(Side B)[codes].dsk
   0x5d038c8e, // Heroes of the Lance (1988)(US Gold)(Side A)[codes].dsk
   0x61e8492a, // Hexenkueche - Cauldron (1985)(Palace Software)(de).dsk
   0xbda5bdfa, // Harvey Headbanger (1986)(Firebird)(v5-86)[cr].dsk
   0x9f193c0a, // Holiday, The (19xx)(Amstrad Adventure PD Library)(PD)[cr].dsk
   0x1dbdfde0, // Hounds of Hell (19xx)(Wow Software)[cr].dsk
   0x57c09d73, // Hirise (1986)(Bubble Bus Software).dsk
   0xffd6fb47, // Harricana (1990)(Loriciels)(fr).dsk
   0xe32fb98b, // Hot-Rod (1990)(Activision).dsk
   0xeaefa043, // Hunt for Red October, The (1987)(Grandslam)(de).dsk
   0x1bee658e, // Hunt for Red October, The (1987)(Grandslam).dsk
   0x6f3f1177, // High Epidemy (1988)(FIL)(fr).dsk
   0x0b157fed, // High Epidemy (1988)(FIL)[cr].dsk
   0x744472d3, // Heroes of Karn (1984)(Interceptor Software)[cr].dsk
   0xb4a56fbb, // Harry & Harry - Mission Torpedo (1986)(ERE Informatique)(fr)(Side B).dsk
   0x3f6dbd27, // Harry & Harry - Mission Torpedo (1986)(ERE Informatique)(fr)(Side A).dsk
   0x5b80efe4, // Hero of the Golden Talisman (1986)(Mastertronic)[cr].dsk
   0xa27228c7, // Hundra (1987)(Dinamic Software)[cr][f].dsk
   0x32461d45, // Horloger 2, L' (1984)(Amsoft)[cr].dsk
   0x55e8ece1, // Horloger 2, L' (1984)(Amsoft)(fr)[cr].dsk
   0xb789e864, // Heli-Bomber (1984)(Pan Books)[cr].dsk
   0x11ba9fce, // Hotshot (1988)(Addictive Games).dsk
   0x57fec6ae, // High Noon (1984)(Locomotive Soft)[cr].dsk
   0xb73aff3e, // Hold Up (1984)(ERE Informatique)(fr)[cr].dsk
   0x5c1309e4, // Herbert's Dummy Run (1986)(Mikrogen)[cr].dsk
   0x8f438125, // Herbert's Dummy Run (1986)(Mikrogen)(es)[cr].dsk
   0x881760cc, // Heilige Rustung Des Antiriad, Die (1986)(Palace Software)(de).dsk
   0x50032521, // Holocauste (1988)(MBC Informatique)(fr)(Side B).dsk
   0xa59bc588, // Holocauste (1988)(MBC Informatique)(fr)(Side A).dsk
   0x1a97c01b, // Hacker (1985)(Activision).dsk
   0x154002ca, // Hi Q Quiz (1989)(CDS Software LTD)[cr].dsk
   0x10283d11, // Hydrofool (1987)(FTL).dsk
   0x4d10c25d, // High Frontier (1987)(Activision).dsk
   0xb956fda4, // Hybrid (1987)(Starlight Software).dsk
   0xc01f31d7, // Halls of the Things (1985)(Design Design)[cr].dsk
   0x0762a657, // Harrier Attack! (1984)(Amsoft).dsk
   0x1551412e, // Han d'Islande (1988)(Loriciels)(fr)(Side A).dsk
   0x58399cb9, // Han d'Islande (1988)(Loriciels)(fr)(Side B).dsk
   0xa523527c, // Hitchhiker's Guide to the Galaxy, The (1984)(Infocom)[CPM].dsk
   0x9594c53f, // Hudson Hawk (1991)(Ocean).dsk
   0x3f3dd360, // Hyperbowl (1986)(Mastertronic)[cr].dsk
   0xa8a6bfd6, // Hammer Boy (1991)(Dinamic Software)[f].dsk
   0x74452b0e, // Hopping Mad (1988)(Elite Systems).dsk
   0x6552a91a, // Hate (1989)(Gremlin Graphics Software).dsk
   0xe842ba0a, // Hyperspace - Aventures Spatio-Temporelles (2018)(Cobra Soft)(es)(Hack)[cr].dsk
   0x6f9312fa, // Hyperspace - Time-Space Adventures (2019)(Cobra Soft)(Hack)[cr].dsk
   0xac7d53f0, // Hyperspace (1985)(Cobra Soft)(fr)[cr].dsk
   0xb3882b69, // Hagar (1989)(Micro Mag)[cr].dsk
   0x045c9620, // Hollywood Hijinx (1986)(Infocom)[CPM].dsk
   0xf8652e9e, // Hypsys (1989)(Dro Soft).dsk
   0x0cfb850e, // House of Usher (1984)(Anirog Software)[cr].dsk
   0xc613efa0, // Head over Heels (1987)(Ocean Software).dsk
   0x94340e6f, // Hostages - Operation Jupiter (1990)(Infogrames)(v2)(Side B).dsk
   0x2eee44c4, // Hostages - Operation Jupiter (1990)(Infogrames)(v2)(Side A).dsk
   0xc7b23244, // Hunchback - The Adventure (1986)(Ocean Software).dsk
   0x6863c03f, // Horloger 1, L' (1984)(Amsoft)(fr)[cr].dsk
   0x8c03c2c1, // Hacker (1985)(Activision)(de).dsk
   0xc7f2552b, // Hundra (1987)(Dinamic Software)(es).dsk
   0xa20e5157, // Hal (1992)(CPC Infos)(fr)[cr].dsk
   0x386ab47d, // Hero Quest - Return of the Witchlord (1991)(Gremlin Graphics Software)(es)(extension).dsk
   0xf1fff06c, // Hero Quest - Return of the Witchlord (1991)(Gremlin Graphics Software)(extension).dsk
   0x52f69491, // Hero Quest - Return of the Witchlord (1991)(Gremlin Graphics Software)(fr)(extension).dsk
   0x13210179, // Halls of Gold (1986)(Rainbow Arts).dsk
   0xbbbf68bb, // How to be a Hero (1987)(Mastertronic)[cr].dsk
   0xad2ba1b2, // Hawaii (1987)(Excalibur)(fr)(Side B)[cr].dsk
   0x74b874e2, // Hawaii (1987)(Excalibur)(fr)(Side A)[cr].dsk
   0x90668523, // Hopital Danger (1989)(B. & J. Dissoubret)(fr)(Side A)(PD)[cr].dsk
   0xa0ef0055, // Hopital Danger (1989)(B. & J. Dissoubret)(fr)(Side B)(PD)[cr].dsk
   0xe468e3e7, // Haunted Hedges (1984)(Amsoft)[cr].dsk
   0x86f6acfb, // Hydra (1991)(Domark)(Side A).dsk
   0xb555ade6, // Hydra (1991)(Domark)(Side B).dsk
   0x18173be7, // Hard Drivin' (1989)(Domark).dsk
   0xbb282a41, // Hobbit, The (1985)(Melbourne House).dsk
   0xdc0c5b25, // Herobotix (1988)(Hewson)[cr].dsk
   0x1828a382, // How to be a Complete Bastard (1987)(Virgin Games)[cr].dsk
   0x89713649, // Hijack (1986)(Activision).dsk
   0xd4e66999, // Hamil (1987)(Jonathan Partington)[cr].dsk
   0xc48ea9ad, // Hexenkueche II - Cauldron II (1986)(Palace Software)(de).dsk
   0xb8798b87, // Hard Hat Mack (1983)(Electronic Arts)[cr].dsk
   0x2e5e1912, // Highway Patrol (1989)(Microids).dsk
   0x60d38d8a, // Highway Encounter (1985)(Vortex Software).dsk
   0x7fed0fd7, // Hurlements (1988)(Ubi Soft)(fr)(Side B).dsk
   0x37376eba, // Hurlements (1988)(Ubi Soft)(fr)(Side A).dsk
   0xaa82fa6c, // Helicoptero 2000 (1985)(Interceptor Software)[cr].dsk
   0xec279622, // Hero Quest (1991)(Gremlin Graphics Software).dsk
   0xafe22434, // Hero Quest (1991)(Gremlin Graphics Software)(fr).dsk
   0x257af370, // Hero Quest (1991)(Gremlin Graphics Software)(es)[b].dsk
   0x542c8788, // Histo Quizz (1985)(Cobra Soft)(fr)[cr].dsk
   0xa4e3d8ff, // Havoc (1990)(Players)[cr].dsk
   0x45b84584, // Harry & Harry - La Boite De Rajmahal (1986)(ERE Informatique)(fr)(Side A).dsk
   0xf2e8b8b8, // Harry & Harry - La Boite De Rajmahal (1986)(ERE Informatique)(fr)(Side B).dsk
   0xb165c326, // Hercules - Slayer of the Damned (1988)(Gremlin Graphics Software).dsk
   0x357e7b6e, // Hardball (1986)(Accolade)(Side A).dsk
   0xa8d83e1a, // Hardball (1986)(Accolade)(Side B).dsk
   0xfa3063d0, // Hunter-Killer (1984)(Amsoft).dsk
   0x3abecdc4, // Hunchback (1985)(Amsoft).dsk
   0x759c1c27, // Horloger 1, L' (1984)(Amsoft)[cr].dsk
   0xe211e4bc, // High Steel (1989)(Screen 7).dsk
   0x66ebc2c6, // Humphrey (1988)(Zigurat Software)(es)[cr].dsk
   0x3005eb3a, // Inside Outing (1987)(The Edge).dsk
   0x52149225, // Impact (1988)(Audiogenic Software LTD).dsk
   0xfecc5418, // Impossible Mission (1986)(Epyx).dsk
   0x56780bdd, // Interieur (1985)(Sprites)(fr).dsk
   0x2367f5fa, // Italy 1990 (1990)(US Gold).dsk
   0x298e8919, // Inquisitor - Shade of Swords (1988)(Chip)(fr).dsk
   0x280ba9a9, // Impossamole (1990)(Gremlin Graphics Software).dsk
   0x9106f3ee, // Interdictor Pilot (1985)(Supersoft).dsk
   0x6c640566, // Indoor Bowling (1987)(Alligata Software)[cr].dsk
   0xaae46015, // Italia '90 - World Cup Soccer (1990)(Virgin Games).dsk
   0x6caa4a16, // International Rugby (1985)(Artic Computing)[cr].dsk
   0xed170c44, // Infiltrator (1986)(US Gold).dsk
   0x2c7b0274, // Ishido (1991)(Fraggle)[cr].dsk
   0x2ccbca1d, // Ingrid's Back (1988)(Level 9 Computing)(Side A).dsk
   0x6c0eece6, // Ingrid's Back (1988)(Level 9 Computing)(Side B).dsk
   0xcacc038a, // Into Oblivion (1986)(Mastertronic)[cr].dsk
   0xc3ce5121, // Ivan 'Ironman' Stewart's Super Off Road (1990)(Virgin Games).dsk
   0xdc3c19fc, // Infidel (1986)(Infocom)[CPM].dsk
   0x453b613c, // Ice Guardian, The (1991)(Strategy Software)(Side A)[cr].dsk
   0xbf060a3a, // Ice Guardian, The (1991)(Strategy Software)(Side B)[cr].dsk
   0x36d943ea, // Ikari Warriors (1988)(Elite Systems)(v2).dsk
   0xb3e18f77, // It's a Knockout (1986)(Ocean Software)[cr].dsk
   0x779bf2ae, // Indoor Soccer (1989)(Codemasters Software)[cr].dsk
   0x8e6f3981, // Indoor Soccer (1986)(Magnificent 7)[cr].dsk
   0xe6183529, // Infernal Runner (1985)(Loriciels)(fr).dsk
   0x66956afa, // Iron Trackers (1989)(Microids)(Side B).dsk
   0x67048019, // Iron Trackers (1989)(Microids)(Side A).dsk
   0xcee8a8bd, // Infection (1989)(Virgin Mastertronic)(Prototype)[cr].dsk
   0x3ea95ce1, // Incantation (1987)(Softhawk)(fr)(Side A).dsk
   0xda8301ab, // Incantation (1987)(Softhawk)(fr)(Side B).dsk
   0x51a3e14d, // Impossaball (1987)(Hewson).dsk
   0x111ad278, // Inhumanos, Los (1990)(Delta Software)(es).dsk
   0xf6eda395, // Imagination (1987)(Firebird)[cr].dsk
   0x6cdf1485, // Impossible Mission II (1988)(Epyx).dsk
   0x3febcdf8, // Ile Oubliee, L' (1993)(Bruno Fonters)(fr)(Side A).dsk
   0x5cb00041, // Ile Oubliee, L' (1993)(Bruno Fonters)(fr)(Side B).dsk
   0x7b3fe8d4, // Icon Jon (1986)(Mirrorsoft).dsk
   0xd189e414, // Ice Temple, The (1989)(CDS Software LTD)[cr].dsk
   0x96423d08, // International Rugby Simulator (1988)(Codemasters Software)[b].dsk
   0xf08c5b1c, // Italian Supercar (1990)(Codemasters Software)[tape].dsk
   0xbf064b59, // Iznogoud (1987)(Infogrames)(fr).dsk
   0x55b67795, // Ice Breaker (1990)(Topo Soft)(es).dsk
   0x730747d5, // Il Etait Une Fois (1989)(Carraz Editions)(fr).dsk
   0xce9761b5, // International Karate+ (1988)(System 3).dsk
   0xde6b8fc1, // Iron Lord (1990)(Ubi Soft)(fr)(Disk 2 Side A).dsk
   0x6d93b106, // Iron Lord (1990)(Ubi Soft)(fr)(Disk 2 Side B).dsk
   0xfb7739c8, // Iron Lord (1990)(Ubi Soft)(fr)(Disk 1 Side A).dsk
   0x8753a866, // Iron Lord (1990)(Ubi Soft)(fr)(Disk 1 Side B).dsk
   0x7d127037, // ISS - Incredible Shrinking Sphere (1989)(Activision).dsk
   0x036182ae, // Island (2002)(WiteKB)(PD)[cr].dsk
   0x586c9898, // Indiana Jones and the Last Crusade (1989)(US Gold)(v3)[codes].dsk
   0x6794c6c6, // Into the Eagle's Nest (1987)(Pandora).dsk
   0xc5b314e4, // Inheritance, The (1987)(Infogrames).dsk
   0xfb866692, // Indiana Jones and the Temple of Doom (1987)(US Gold).dsk
   0x6ffd24c3, // Ian Botham's Test Match (1985)(Tynesoft)[cr].dsk
   0xe3bb11ec, // Infodroid (1986)(Beyond Software).dsk
   0xadb50c58, // I, Ball II - Quest for the Past (1987)(Firebird)[cr].dsk
   0xe5c9f091, // International 3D Tennis (1990)(Palace Software)(v2).dsk
   0x5084e823, // Inertie (1987)(Ubi Soft)(fr)(Side A).dsk
   0x343d3776, // Inertie (1987)(Ubi Soft)(fr)(Side B).dsk
   0xdaaa48ae, // International Karate (1986)(Endurance Games).dsk
   0xaf33badf, // Island of Dr. Destructo, The (1987)(Mastertronic)[cr].dsk
   0x77d9ca8d, // Isoleur (1985)(Sprites)(fr)[cr].dsk
   0x0c735354, // Island, The (1985)(The Adventure Workshop).dsk
   0x56af7f27, // Indoor Sports (1987)(Design Star Consultants).dsk
   0x8fb048a1, // Indiana Jones and the Fate of Atlantis (1993)(US Gold)(Side A).dsk
   0xeddf9c73, // Indiana Jones and the Fate of Atlantis (1993)(US Gold)(Side B).dsk
   0x13823a00, // I, Ball (1987)(Firebird)[cr][f].dsk
   0xf578d894, // Isotopes (1988)(Amstrad Action)[cr].dsk
   0x23dcdb6b, // Jetsons, The (1992)(Hi-Tec Software).dsk
   0xff3ada47, // Jungle Jane (1986)(Bug-Byte)[cr][464].dsk
   0xbe7f2e78, // Juego del Ahorcado (19xx)(-)(PD)(M5)[cr].dsk
   0xc0fb0b5b, // Joe Blade 2 (1988)(Players)[cr].dsk
   0x7d18254a, // Jewels of Babylon, The (1984)(Interceptor Software)[cr].dsk
   0xcc600d0b, // Jocky Wilson's Compendium of Darts (1990)(Zeppelin Games)[cr].dsk
   0x9ac75672, // Jack the Ripper (1988)(CRL Group)[cr].dsk
   0x269f0d8a, // Jet Set Willy (1985)(Software Projects)[cr].dsk
   0xe0da82c4, // Jet Bike Simulator (1988)(Codemasters Software).dsk
   0x9f177d9b, // Jinxter (1988)(Rainbird Software)(Side A).dsk
   0x403735dd, // Jinxter (1988)(Rainbird Software)(Side B).dsk
   0xe76f838a, // Jai Alai (1991)(Opera Soft)(es)[codes].dsk
   0x13d90b7f, // Jungle Book (1989)(Coktel Vision)(Side B).dsk
   0xccc3a70b, // Jungle Book (1989)(Coktel Vision)(Side A).dsk
   0xcc18789c, // Jet-Boot Jack (1984)(Amsoft)[tape].dsk
   0x1b5a700f, // Joust (1986)(Quality Software)[cr].dsk
   0xf0da3efc, // Jet Set Willy - The Final Frontier (1985)(Software Projects)[cr].dsk
   0x02bec9bd, // Jack the Nipper II - In Coconut Capers (1987)(Gremlin Graphics Software).dsk
   0x92057eaf, // Jail Break (1986)(Konami).dsk
   0x89ecce07, // Jumpman (1986)(Blaby Computer Games)[cr].dsk
   0x5e9c938b, // Jump Jet (1985)(Anirog Software).dsk
   0x034de766, // Joe Blade 3 (1989)(Players Premier)[cr][f].dsk
   0x9039209f, // Juego de la Oca, El (1989)(Zafiro Software)(es).dsk
   0xd772dd9e, // Jack and the Beanstalk (1985)(Thor Software)[cr].dsk
   0x91b72895, // Johnny Reb (1984)(MC Lothlorien)[cr].dsk
   0x350f5b1a, // Jewels of Darkness - Colossal Trilogy (1986)(Rainbird Software).dsk
   0xd1b6e326, // James Clavell's Shogun (1986)(Virgin Games).dsk
   0xade65d61, // Jinks (1988)(Rainbow Arts).dsk
   0x23e7dd40, // Joe Blade (1987)(Players)[cr].dsk
   0xb06159a2, // Jack the Nipper (1986)(Gremlin Graphics Software).dsk
   0xafd4245e, // Jason of the Argonauts (1990)(The Guild)(PD)[cr].dsk
   0x8cb5b3fd, // Jonah Barrington's Squash (1987)(Mastertronic)[tape].dsk
   0xaafc2f5f, // Jim Power in Mutant Planet (1992)(Loriciels)(Side B).dsk
   0x4a3e1d19, // Jim Power in Mutant Planet (1992)(Loriciels)(Side A).dsk
   0x3db4af9c, // Jack Nicklaus Golf (1989)(Accolade)(v2)(Disk 1 Side A)[Program].dsk
   0xb0eb63e7, // Jack Nicklaus Golf (1989)(Accolade)(v2)(Disk 1 Side B)[Greatest].dsk
   0x1e427001, // Jack Nicklaus Golf (1989)(Accolade)(v2)(Disk 2 Side B)[Desert Mountain].dsk
   0x0388ea7d, // Jack Nicklaus Golf (1989)(Accolade)(v2)(Disk 2 Side A)[Castle Pines].dsk
   0x6d5e1931, // Joyaux De Babylone, Les (1985)(Interceptor Software)(v1.2)[cr].dsk
   0xd0d1542c, // Jackal (1986)(Konami).dsk
   0xb6553a69, // Jungle Warfare (1989)(Virgin Mastertronic)[cr].dsk
   0xa888572f, // Jaws (1989)(Screen 7).dsk
   0x2f2cc873, // Jugando al Ajedrez (1991)(Mexico Floppy)(es)[cr].dsk
   0x85010e9d, // Johnny Reb II (1986)(MC Lothlorien).dsk
   0xa169df18, // Jigsaw (1986)(Skyline Software)[cr].dsk
   0xf0926ae7, // Jabato (1989)(Aventuras AD)(es).dsk
   0xb5d6a0d6, // Jeux Sans Frontieres - Intervilles (1986)(Ocean Software)[cr].dsk
   0x0bac47f1, // Jeu du Roy, Le (1988)(FIL)(fr)[b].dsk
   0x43665e0d, // Juggernaut (1985)(CRL Group)[cr].dsk
   0x420cd6ec, // Jammin (1985)(Amsoft).dsk
   0x7a9f7d53, // Jungle Warrior (1990)(Zigurat Software)(es).dsk
   0x26d80460, // Jahangir Khan World Championship Squash (1991)(Krisalis Software)(M3)(Side B)[Tournament].dsk
   0xf4548f27, // Jahangir Khan World Championship Squash (1991)(Krisalis Software)(M3)(Side A)[Club].dsk
   0x551d921f, // Killer Ring (1987)(Ariolasoft)[cr].dsk
   0xab58ecd4, // K.Y.A. (1987)(Loriciels)(es).dsk
   0xb8c30fe5, // K.Y.A. (1987)(Loriciels)(fr).dsk
   0x4d2044c4, // Krypton Factor, The (1987)(Domark)(Side A)[f].dsk
   0x7dbbeed0, // Krypton Factor, The (1987)(Domark)(Side B).dsk
   0xf9b8eb95, // KTris (2003)(Richard Wilson).dsk
   0x6f0e5a7a, // Killapede (1986)(Players)[cr].dsk
   0xf6442af9, // Knight Games (1986)(English Software)(v2)(Side B).dsk
   0xf47b9241, // Knight Games (1986)(English Software)(es)(Hack)(Side B)[cr].dsk
   0xe0987148, // Knight Games (1986)(English Software)(es)(Hack)(Side A)[cr].dsk
   0x36c62e3a, // Knight Games (1986)(English Software)(v2)(Side A).dsk
   0xaf878a5f, // Kiloroid (2002)(Richard Wilson).dsk
   0xd33e0313, // Kessin (1992)(PD)(fr)[cr].dsk
   0xee16d403, // King Leonard (1986)(Mind Games España)(es)[cr].dsk
   0x96b53a41, // Klax (1990)(Domark).dsk
   0xcf97d092, // Karl's Treasure Hunt (1984)(Software Project)[cr].dsk
   0xddd09042, // Ke Rulen los Petas (1989)(Iber Soft)(es).dsk
   0xbc83b163, // Konami's Golf (1986)(Ocean Software).dsk
   0xb9e22201, // Karateka (1986)(Broderbund Software)(fr).dsk
   0x5fb2a6be, // Karateka (1986)(Broderbund Software)(es)(Side B).dsk
   0xb73c418b, // Karateka (1986)(Broderbund Software)(es)(Side A).dsk
   0x0200e7b4, // Koronis Rift (1987)(Activision)(v2).dsk
   0x66f60bf3, // Knight Tyme (1986)(Mastertronic)[cr].dsk
   0x64619305, // Kinetik (1987)(Firebird).dsk
   0x8b2ddad1, // Konami's Ping Pong (1986)(Ocean Software).dsk
   0x7a097c69, // Kenny Dalglish Soccer Match (1990)(Impressions)[tape].dsk
   0x47f50e26, // Kobyashi Naru (1987)(Mastertronic)[cr].dsk
   0xeed8ee05, // Kong Strikes Back (1984)(Ocean Software)[cr].dsk
   0x5804d4f1, // Kokotoni Wilf (1984)(Elite Systems)[cr].dsk
   0xaee30fe7, // Killer Cobra (1987)(Mastertronic)[cr].dsk
   0xc6628383, // Killed Until Dead (1987)(US Gold)(Side A).dsk
   0x9d5c9e25, // Killed Until Dead (1987)(US Gold)(Side B).dsk
   0x1c4f62da, // Kenny Dalglish Soccer Manager (1989)(Cognito)[cr].dsk
   0x33220da8, // Kat Trap (1987)(Domark).dsk
   0x624b9342, // Kong's Revenge (1991)(Zigurat Software)(es).dsk
   0xec2efccb, // Kwik Snax Dizzy (1990)(Codemasters Software).dsk
   0xe4b64206, // Kane (1986)(Mastertronic)[cr].dsk
   0xa2d5c9db, // Knight Lore (1984)(Ultimate Play The Game)[cr].dsk
   0x89cba118, // Kentucky Racing (1991)(Alternative Software)[cr].dsk
   0xdc4f94ca, // Killer Star (1986)(Loisitech)(fr)(Side A)[b].dsk
   0xd447d1c4, // Killer Star (1986)(Loisitech)(fr)(Side B)[b].dsk
   0x555ba7ba, // Kung-Fu (1986)(Bug-Byte)[cr].dsk
   0x5708f28a, // Krakout (1987)(Gremlin Graphics Software).dsk
   0x267dbdff, // Key Factor, The (1985)(Amsoft)[cr].dsk
   0xa2a1a0fa, // Kettle (1986)(Alligata Software).dsk
   0xe874a3ae, // Kung-Fu Master (1986)(US Gold).dsk
   0x0934c463, // Knight Orc (1987)(Level 9 Computing)(Side A).dsk
   0x13f89b91, // Knight Orc (1987)(Level 9 Computing)(Side B).dsk
   0x734bb556, // Knight Rider (1986)(Ocean Software).dsk
   0xfa322061, // Kentilla (1986)(Mastertronic)[cr].dsk
   0xe2dbc3c3, // Killer Ball (1991)(Microids)(M2).dsk
   0xf4990d9e, // Knightmare (1987)(Activision).dsk
   0x68ede3f2, // Killer Gorilla (1984)(Micro Power)[cr].dsk
   0xa83f0253, // Kristax (1989)(Deca Software)[cr].dsk
   0x6ca93012, // Karnov (1988)(Activision)(Side A).dsk
   0x35c91cec, // Karnov (1988)(Activision)(Side B).dsk
   0xb88541f9, // Kristal (1984)(Core).dsk
   0x872b06fa, // Kick Off 2 (1990)(Anco Software)(Side B).dsk
   0x6eb988cf, // Kick Off 2 (1990)(Anco Software)(fr)(Side B).dsk
   0x79b9b585, // Kick Off 2 (1990)(Anco Software)(Side A).dsk
   0xbc4f103d, // Kick Off 2 (1990)(Anco Software)(fr)(Side A).dsk
   0xc6737857, // Kick Off 2 (1990)(Anco Software)(es)[cr].dsk
   0x81cf1945, // Kick Off (1990)(Anco Software)(M5).dsk
   0x5f05890e, // Leviathan (1987)(English Software).dsk
   0x1d39d2e7, // Leaderboard (1986)(US Gold).dsk
   0x29e5876c, // Lotus Esprit Turbo Challenge (1990)(Gremlin Graphics Software).dsk
   0xf172183f, // Link (1992)(Amstrad Action)[cr].dsk
   0xd9dec288, // Lords of Chaos (1990)(Blade Software)(M2).dsk
   0xd2a48c7d, // Lords Of Chaos - Expansion Kit One (1990)(Blade Software)(M2).dsk
   0x5f963c09, // Labyrinthe D'Anglomania 1, Le (1990)(Retz)(fr)(Side A).dsk
   0x372019a4, // Labyrinthe D'Anglomania 1, Le (1990)(Retz)(fr)(Side B).dsk
   0xe12e6e01, // Lion (1991)(Amstrad Action)[cr].dsk
   0x7dd16bd1, // Labyrinth Hall (1987)(Amstrad Adventure PD Library)(PD)[cr].dsk
   0xf6943a86, // Labyrinthe aux Mille Calculs, Le - College (1989)(Retz)(fr)(Disk 1 Side A)[CPM].dsk
   0x26239b63, // Labyrinthe aux Mille Calculs, Le - College (1989)(Retz)(fr)(Disk 1 Side B).dsk
   0x539dfc6a, // Labyrinthe aux Mille Calculs, Le - College (1989)(Retz)(fr)(Disk 2 Side A).dsk
   0x7c599575, // Labyrinthe aux Mille Calculs, Le - College (1989)(Retz)(fr)(Disk 2 Side B).dsk
   0xb2837f6d, // Lifeterm (1987)(Alternative Software)[cr].dsk
   0xf2d8dffa, // Lights Out (1997)(Rudiger)(fr)(PD)[cr].dsk
   0xcc3c59ed, // Labyrinthe d'Errare, Le - College (1989)(Retz)(fr)(Disk 2 Side B).dsk
   0x77adf13c, // Labyrinthe d'Errare, Le - College (1989)(Retz)(fr)(Disk 2 Side A).dsk
   0xd096f9fd, // Labyrinthe d'Errare, Le - College (1989)(Retz)(fr)(Disk 1 Side B).dsk
   0xa4f28175, // Labyrinthe d'Errare, Le - College (1989)(Retz)(fr)(Disk 1 Side A)[CPM].dsk
   0x4e7fd8a2, // Lurking Horror, The (1987)(Infocom)[cr].dsk
   0x92b6a819, // Labyrinthe aux Cent Calculs, Le - Ecole (1989)(Retz)(fr)(Disk 1 Side A)[CPM].dsk
   0xe3955d4d, // Labyrinthe aux Cent Calculs, Le - Ecole (1989)(Retz)(fr)(Disk 1 Side B).dsk
   0xd7ad58e3, // Labyrinthe aux Cent Calculs, Le - Ecole (1989)(Retz)(fr)(Disk 2 Side B).dsk
   0xda04bc18, // Labyrinthe aux Cent Calculs, Le - Ecole (1989)(Retz)(fr)(Disk 2 Side A).dsk
   0x2943604c, // Lancelot (1988)(Level 9 Computing)(Side B).dsk
   0xc5ad3e3e, // Lancelot (1988)(Level 9 Computing)(Side A).dsk
   0x11b7b081, // Legend of Kage (1986)(Ocean Software).dsk
   0x42e5f481, // Lords of Midnight, The (1985)(Beyond Software).dsk
   0x4eec7616, // Liberator (1989)(Proein Soft Line).dsk
   0xbbe80a27, // Legend of Apache Gold, The (1986)(Incentive)(Hack)[cr].dsk
   0x1f8ce03a, // Lucky Luke - Nitroglycerine (1987)(Coktel Vision)(es)(Side A).dsk
   0x33307848, // Lucky Luke - Nitroglycerine (1987)(Coktel Vision)(es)(Side B).dsk
   0xdecbd246, // Los Angeles SWAT (1987)(Mastertronic)[cr].dsk
   0x59802f2a, // Loom (1996)(Arkos)(PD)[cr].dsk
   0xcf01470c, // Laser Squad - Kit 1 (1989)(Blade Software)(M2).dsk
   0x66ecb73b, // Laser Squad (1989)(Blade Software)(M2).dsk
   0x6fe2ef82, // Laberinto Sultan, El (1984)(Amsoft)(es)[cr].dsk
   0xeb127e47, // Leather Goddesses of Phobos (1986)(Infocom)[CPM].dsk
   0x695b81d3, // Labyrinthe des Pharaons, Le (1990)(Retz)(fr)(Disk 1 Side A).dsk
   0xd22c323c, // Labyrinthe des Pharaons, Le (1990)(Retz)(fr)(Disk 2 Side B).dsk
   0xd3d374ee, // Labyrinthe des Pharaons, Le (1990)(Retz)(fr)(Disk 2 Side A).dsk
   0xc319fe69, // Light Force (1986)(FTL).dsk
   0x1f6b8c98, // Last Ninja 2 Remix (1990)(System 3)(Side B).dsk
   0x7e0090aa, // Last Ninja 2 Remix (1990)(System 3)(Side A).dsk
   0xe3f994f0, // Labyrinthe d'Orthophus, Le - Ecole (1989)(Retz)(fr)(Disk 2 Side A).dsk
   0x4235f7ca, // Labyrinthe d'Orthophus, Le - Ecole (1989)(Retz)(fr)(Disk 1 Side B).dsk
   0xd5555594, // Labyrinthe d'Orthophus, Le - Ecole (1989)(Retz)(fr)(Disk 2 Side B).dsk
   0x342c720e, // Labyrinthe d'Orthophus, Le - Ecole (1989)(Retz)(fr)(Disk 1 Side A)[CPM].dsk
   0xd4bc3547, // Lisa Strip Pot (1985)(Ludo's Software Distribution)(PD)(fr)[cr].dsk
   0x35cf7ef3, // Last Duel (1989)(US Gold).dsk
   0xdf4abfd8, // Lubix (1988)(AM-Mag)(fr)[cr].dsk
   0xef801dfc, // Loogaroo (1988)(Top Ten)[cr].dsk
   0x4ccd5ef3, // Life Expectancy Zero (1985)(Blaby Computer Games)[cr].dsk
   0xa5f991ad, // Lone Wolf (1991)(Audiogenic Software LTD).dsk
   0xd7aa74be, // Livingstone Supongo (1986)(Opera Soft)(es).dsk
   0x8014ac9c, // Livingstone I Presume (1987)(Opera Soft).dsk
   0xc685e1b3, // Livingstone (1987)(Opera Soft)(fr).dsk
   0x63e7466d, // Labynotaure (1985)(Amstrad Magazine)(fr)[cr].dsk
   0x3e8a43fd, // Last Race, The (1989)(Amstar & Cpc)[cr].dsk
   0x03e02a4e, // Lemmings (1992)(Psygnosis)(Side B).dsk
   0xc5714f20, // Lemmings (1992)(Psygnosis)(Side A).dsk
   0xae273ff5, // Lost Caves (1989)(Players Premier)[cr].dsk
   0x7f951a1c, // Lost Crown, The (1985)(S. Sawdy)[cr].dsk
   0xb58d3ed5, // Lee Enfield - An Amazon Adventure (1987)(Infogrames).dsk
   0x74c68ce8, // Line of Fire (1990)(US Gold).dsk
   0xe93f03d2, // Loopz (1991)(Audiogenic Software).dsk
   0x9fa57d50, // Lee Enfield - Space Ace (1987)(Infogrames).dsk
   0x2ff0cc27, // Lost Phirious, The (19xx)(Wow Software)[cr].dsk
   0x3804f990, // Laser Base (1986)(Computing With The Amstrad)[cr].dsk
   0x0dc39b44, // Last Mission, The (1987)(Opera Soft)(fr).dsk
   0x0cde6333, // Last Mission, The (1987)(Opera Soft)(es).dsk
   0x1da0dcd4, // Last V8, The (1986)(Mastertronic)[cr].dsk
   0xe0fd670d, // Legions of Death (1987)(MC Lothlorien)(es)[cr].dsk
   0x410d2254, // Legions of Death (1987)(MC Lothlorien)[cr].dsk
   0x0d65ed5e, // Legend (1990)(Delta Software)(es)(v2)(Side A).dsk
   0x06bcc211, // Legend (1990)(Delta Software)(es)(v2)(Side B).dsk
   0x708f31d5, // Lorna (1990)(Topo Soft)(es).dsk
   0x7c2489ec, // Lord of the Rings - Game One (1986)(Melbourne House)(Side A).dsk
   0xfa983b46, // Lord of the Rings - Game One (1986)(Melbourne House)(Side B).dsk
   0x33a4f4d3, // Laserwarp (1985)(Amsoft)[tape].dsk
   0x2ebe385f, // Livingstone Supongo II (1989)(Opera Soft).dsk
   0x01dab581, // Lucky Luke - Nitroglycerine (1987)(Coktel Vision)(fr)(Side A)[CPM].dsk
   0x71a30503, // Lucky Luke - Nitroglycerine (1987)(Coktel Vision)(fr)(Side B).dsk
   0xb5dd9b44, // Light Corridor, The (1990)(Infogrames).dsk
   0xfac29692, // Lode Runner (1989)(Broderbund Software)(fr).dsk
   0xff869f60, // Last of the Smileys (1995)(Amstrad Action)(PD)[cr].dsk
   0xfb24d9ac, // Last Ninja 2 (1988)(System 3)(Side B).dsk
   0x2e386ba7, // Last Ninja 2 (1988)(System 3)(Side A).dsk
   0x873e1294, // Labyrinthe de Lexicos, Le (1990)(Retz)(fr)(Side B).dsk
   0xd21c33a6, // Labyrinthe de Lexicos, Le (1990)(Retz)(fr)(Side A).dsk
   0x3669b69f, // Leaderboard Tournament (1987)(US Gold).dsk
   0x9a2bd0a6, // Little Game (1987)(Lojze Dolenc)(PD)[cr].dsk
   0x99acff55, // Legende (1987)(FIL)(fr)(Side A).dsk
   0x6abd42c9, // Legende (1987)(FIL)(fr)(Side B).dsk
   0xe89e115e, // Lee Enfield - The Tournament Of Death (1988)(Infogrames).dsk
   0xe31090f5, // Lord of the Rings - Game Two - Shadows of Mordor (1987)(Melbourne House)[cr].dsk
   0x504e2a3f, // Little Puff in Dragonland (1990)(Codemasters Software).dsk
   0xa07d3bdf, // LED Storm (1988)(Us Gold).dsk
   0x1a7d1a90, // Lazer Tag (1987)(US Gold).dsk
   0x9cff63e2, // Long-Way (1988)(Amstrad Cent Pour Cent)[cr].dsk
   0x04aa3b1e, // Molecule (1990)(Computing With The Amstrad)[cr].dsk
   0x3e2b2f77, // Marque Jaune, La (1988)(Cobra Soft)(fr)(Side B).dsk
   0x51210f22, // Marque Jaune, La (1988)(Cobra Soft)(fr)(Side A).dsk
   0x0754859e, // Movie (1986)(Ocean Software).dsk
   0x7cd49cdc, // Meynaupede (1985)(Hebdogiciel)(fr)[cr].dsk
   0xcd2e8018, // Malediction de Thaar, La (1985)(Coktel Vision)(fr).dsk
   0x77b4db9c, // Mystere de Kikekankoi, Le (1985)(Loriciels)(fr)[cr].dsk
   0x2bb9ce08, // MLM 3D - Evasion de la Lune (1986)(Chip Avril)(fr).dsk
   0x0083cdf8, // Marsport (1985)(Gargoyle Games).dsk
   0x6abcc9b6, // Moon Cresta (1986)(Incentive).dsk
   0xa5c8df4b, // Mig 29 Soviet Fighter (1989)(Codemasters Software)[b].dsk
   0x9995ddbe, // Master Chess (1987)(Mastertronic)[cr].dsk
   0xbe4e34b2, // Mondes Paralleles, Les (1993)(Mchtml)(fr)(Side B)[cr].dsk
   0x6b8d3058, // Mondes Paralleles, Les (1993)(Mchtml)(fr)(Side A)[cr].dsk
   0xb55f3eec, // Maze Mania (1989)(Hewson).dsk
   0x9957a7c3, // Masters of Space, The (1994)(Radical Software)[cr].dsk
   0x3309d69e, // Mike Gunner (1988)(Dinamic Software)(es)(LightGun).dsk
   0x48cfba3d, // Missile Ground Zero (1989)(Virgin Games)(LightGun)[cr].dsk
   0xf353927d, // Mutant Monty (1984)(Artic Computing).dsk
   0xfac6531d, // Munsters, The (1989)(Again Again).dsk
   0xcc7e6ce8, // Madmazes (1992)(Pipeline Software)[cr].dsk
   0xf43e00bd, // Mag Max (1987)(Imagine Software).dsk
   0xd77ef6ed, // Moto Cross Simulator (1989)(Codemasters Software)[cr].dsk
   0x11868bff, // Momie Blue (1986)(Coktel Vision)(fr).dsk
   0x2dce1a57, // Monty Pythons Flying Circus (1990)(Virgin Games)[codes].dsk
   0x8e59d16a, // Moulinsar (1987)(Bretagne Edit presse)(fr)[cr].dsk
   0xfd20468f, // Mega-Bucks (1986)(Firebird)[cr].dsk
   0xfd3423b5, // Mission 2 (1987)(Gasoline Software)[cr].dsk
   0x148447dc, // Motos (1987)(Mastertronic)[cr].dsk
   0xfd2001f3, // Mercs (1991)(US Gold).dsk
   0x7c096b33, // Miss Input 2 (2020)(cpcretrodev.byterealms)[tape].dsk
   0x907b8195, // Miami Vice (1986)(Ocean Software).dsk
   0x18dcb1b4, // Mega-Apocalypse (1988)(Martech).dsk
   0x95def41f, // Metal Army (1988)(Players)[cr].dsk
   0xb88a5919, // Mission Jupiter (1987)(Codemasters Software)[cr].dsk
   0xc321d3b5, // Mr Freeze (1984)(Firebird)[cr].dsk
   0xff082b39, // Munch-it (1984)(Tynesoft)[cr].dsk
   0x77a58bee, // Meltdown (1986)(Alligata Software).dsk
   0xc4f3d1aa, // Myth - History in the Making (1989)(System 3)(Side B).dsk
   0x4d8464d3, // Myth - History in the Making (1989)(System 3)(Side A).dsk
   0xcd7c294d, // Minette (19xx)(Edmund Spicer)[cr].dsk
   0xfe03f194, // Master Rubik's Cube (1985)(Hebdogiciel)(fr)[cr].dsk
   0x5237c2e6, // Monopolic (1985)(Free Game Blot)(fr).dsk
   0x04d0e00d, // Monopoly (1986)(Leisure Genius)(fr).dsk
   0x73044172, // Monopoly (1986)(Leisure Genius).dsk
   0xa5302940, // Mandragore (1986)(Infogrames)(de)(Side A).dsk
   0xad5f8b5b, // Mandragore (1985)(Infogrames)(fr)(Side A).dsk
   0xb9dfe4d5, // Mandragore (1986)(Infogrames)(de)(Side B).dsk
   0x5d037c5c, // Mandragore (1985)(Infogrames)(Side A).dsk
   0x801969a5, // Mandragore (1985)(Infogrames)(Side B).dsk
   0x104b2669, // Mandragore (1985)(Infogrames)(fr)(Side B).dsk
   0x43aa21e6, // Mickey Mouse (1988)(Gremlin Graphics Software).dsk
   0x9998ba8f, // Mike & Moko (1988)(MBC Informatique)(Side B).dsk
   0xc70118be, // Mike & Moko (1988)(MBC Informatique)(Side A).dsk
   0xdadfca99, // Mission en Rafale (1987)(FIL)(M2).dsk
   0x29f601f4, // MGT - Magnetik Tank (1986)(Loriciels)(fr).dsk
   0x281b7099, // MGT - Magnetik Tank (1986)(Loriciels).dsk
   0xe698443a, // Memotron (1987)(Magazin Players Dream)(de)[cr].dsk
   0x31bddd66, // Moon Buggy (UK) (1985)[cr].dsk
   0xf59da67d, // Master of the Lamps (1985)(Activision)[cr].dsk
   0xe5d0fff0, // Maggot Splat (1985)(Your Computer)(es)[cr].dsk
   0x706d0ef3, // Maggot Splat (1985)(Your Computer)[cr].dsk
   0x998919cf, // Mystery of the Java Star, The (1985)(Shards Software)[cr].dsk
   0xce4e6c6e, // Mask (1987)(Gremlin Graphics Software).dsk
   0x3677d82c, // Monte Cristo (1988)(Coktel Vision)(fr)(Side B).dsk
   0x90f7f707, // Monte Cristo (1988)(Coktel Vision)(fr)(Side A)[CPM].dsk
   0x97b8b3a9, // Mad Mix Game - The Pepsi Challenge (1988)(Topo Soft).dsk
   0xc36d1efd, // Mad Mix Game (1988)(Topo Soft)(es)[cr].dsk
   0x745434d1, // Mad Ball (1993)(CPC Infos)(fr)[cr].dsk
   0x5745a375, // Mario Bros (1987)(Ocean Software).dsk
   0xa637cd4e, // Meurtre sur LAtlantique (1985)(Cobra Soft)(fr)(Side B)[cr].dsk
   0x901e677f, // Meurtre sur LAtlantique (1985)(Cobra Soft)(fr)(Side A)[cr].dsk
   0x0eeae24a, // Mortadelo y Filemon II (1989)(Dro Soft).dsk
   0xbc2e3016, // Mystical (1990)(Infogrames).dsk
   0x4596115b, // Megamix (1992)(LLC)(PD)[cr].dsk
   0xba4dd479, // Mewilo (1987)(Coktel Vision)(fr)(Side B).dsk
   0x8c14fcdd, // Mewilo (1987)(Coktel Vision)(de)(Side B).dsk
   0x05339e1d, // Mewilo (1987)(Coktel Vision)(de)(Side A)[CPM].dsk
   0x815c762b, // Mewilo (1987)(Coktel Vision)(fr)(Side A)[CPM].dsk
   0xd0880eb7, // Marius Tresor Foot (1985)(Loriciels)(fr).dsk
   0x32ca151b, // Menace sur l'Arctique (1987)(Chip)(fr)[cr].dsk
   0xb7d57795, // Midnight Resistance (1990)(Ocean Software)(Side B).dsk
   0xa7baa480, // Midnight Resistance (1990)(Ocean Software)(Side A).dsk
   0x1e6eb6ce, // Manager (1985)(ERE Informatique)(es)(hack)[cr].dsk
   0x20a0989b, // Manager (1985)(ERE Informatique)(fr)[b].dsk
   0x9f367a77, // Monument (1991)(Zeppelin Games)[cr].dsk
   0xaf3d9c8f, // Mac 2 (1990)(Scull PD Library)(PD)[cr].dsk
   0x416d5acd, // Mascotte (1988)(Coktel Vision)(fr)(Side A)[CPM].dsk
   0xe4175460, // Mascotte (1988)(Coktel Vision)(fr)(Side B).dsk
   0x58ecb546, // Malediction, La (1991)(Lankhor)(fr)(Side A).dsk
   0x499e78f7, // Malediction, La (1991)(Lankhor)(fr)(Side B).dsk
   0x63ba006b, // Morris Meets the Biker (1984)(Automata UK)[cr].dsk
   0x48684cf4, // Mikie (1986)(Ocean Software)[cr].dsk
   0x8349da70, // Maracaibo (1986)(Loriciels)(fr).dsk
   0x35c029d5, // Match Day II (1987)(Ocean Software).dsk
   0x53f3d54c, // Marble Madness Deluxe Edition (1987)(Melbourne House)[cr][f].dsk
   0xab3ea0dc, // Mask II (1988)(Gremlin Graphics Software).dsk
   0x037b1c79, // Mille Bornes (1985)(Free Games Blot)(fr).dsk
   0x7e1deece, // Mineur (1987)(Cobra Soft)(fr)[cr].dsk
   0x40a3cfd4, // Misterio del Nilo, El (1987)(Zigurat)(es)[cr].dsk
   0x976e97c3, // Microball (1987)(Alternative Software)[cr].dsk
   0x73ac7fd4, // Manchester United Europe (1991)(Krisalis Software)(M5).dsk
   0x3c2ccfc9, // Microprose Soccer (1989)(Microprose Software)(Side B)[Six-a-Side].dsk
   0x79799b5a, // Microprose Soccer (1989)(Microprose Software)(Side A)[Soccer].dsk
   0x00163387, // Mantis 1 (1990)(MCM Software)(es)[cr].dsk
   0x54b79dfb, // Martianoids (1987)(Ultimate Play The Game)[cr].dsk
   0x7f2003c3, // Magic Clock (1986)(Players)[cr].dsk
   0x62a4deae, // Mutan Zone (1989)(Opera Soft)(es).dsk
   0x85408c9d, // Micro Scrabble (1985)(Leisure Genius)(fr).dsk
   0xdaf34394, // Mata Hari (1988)(Loriciels)(fr).dsk
   0x5b9b2926, // Mata Hari (1988)(Loriciels)[cr].dsk
   0xed1262ad, // Mini-Centipede (2001)(PD)[cr].dsk
   0x1210c20f, // Mindshadow (1986)(Activision).dsk
   0x0f071249, // Moving Target (1989)(Players Premier)[cr].dsk
   0xdc93ff81, // Mr Pingo (1986)(Rainbow Arts)(fr)[cr].dsk
   0xbd6ed47c, // Mr Pingo (1986)(Rainbow Arts)[cr].dsk
   0x209fa05d, // Mr Pingo (1986)(Rainbow Arts)(M2)[cr].dsk
   0x8deb606e, // Mission Genocide (1987)(Firebird)[cr].dsk
   0xffc8baaf, // Mission (1987)(Loriciels).dsk
   0x174743db, // Mission (1987)(Loriciels)(fr).dsk
   0xe9d9f77e, // Mazie (1988)(Zeppelin Games)[cr].dsk
   0xdc036664, // Motor Massacre (1988)(Gremlin Graphics Software).dsk
   0x773e6ab7, // Mokowe (1991)(Lankhor)(fr)(Side B).dsk
   0x0b45fb5b, // Mokowe (1991)(Lankhor)(fr)(Side A).dsk
   0xa673019e, // Metropolis (1988)(The Power House)[cr].dsk
   0xb043f026, // Mission Elevator (1988)(Eurogold).dsk
   0x159ed0f1, // Meurtre a Grande Vitesse (1985)(Cobra Soft)(fr).dsk
   0x6a0357e8, // Manoir de Mortvielle, Le (1988)(Lankhor)(fr)(Side B).dsk
   0x697d2d31, // Manoir de Mortvielle, Le (1988)(Lankhor)(fr)(Side A).dsk
   0x0c5444f8, // Magic Sword, The (1984)(Database Educational Software).dsk
   0xf3dcb034, // Match Day (1985)(Ocean Software)[cr].dsk
   0xc8676092, // Meurtres a Venise (1988)(Cobra Soft)(fr)(Side B).dsk
   0xb13a3365, // Meurtres a Venise (1988)(Cobra Soft)(fr)(Side A).dsk
   0x55dc54ee, // Mahjong (1986)(Liffsoft)[cr].dsk
   0x4486bb08, // Mountains of Ket (1987)(Incentive)[cr].dsk
   0x11f43936, // Mr Heli (1989)(Firebird).dsk
   0x789e6383, // Mines (1994)(CRCT)(PD)[cr].dsk
   0x71f4ff32, // Metro 2018 (1985)(Initiel)(fr).dsk
   0x41332486, // Moontorc (1991)(Atlantis Software)[cr][f].dsk
   0xeed2b2e6, // Minisenso (1992)(JM)(PD)[cr].dsk
   0x5e917c38, // Meurtres en Serie (1987)(Cobra Soft)(fr)(Side A).dsk
   0xeca69e3e, // Meurtres en Serie (1987)(Cobra Soft)(fr)(Side B).dsk
   0xcbc0e70f, // Mach 3 (1987)(Loriciels).dsk
   0xdbc29c10, // Mach 3 (1987)(Loriciels)(fr).dsk
   0x6f132709, // Mundial de Futbol (1990)(Opera Soft)(es).dsk
   0x5a5f6699, // Moors Challenge, The (1984)(Timeslip Software)[cr].dsk
   0xc3e4ab94, // Mr Wong's Loopy Laundry (1984)(Amsoft)[cr].dsk
   0xec4ec694, // Malefice des Atlantes, Le (1987)(Chip)(fr).dsk
   0xe8d15029, // Miss Input (2019)(cpcretrodev.byterealms)[tape].dsk
   0xdfcc4b07, // Morpions (1985)(Depinoy Daniel)(fr)[cr].dsk
   0xb279d4c8, // Mad Mix 2 - En el Castillo de los Fantasmas (1990)(Topo Soft)(es).dsk
   0x48adf9cf, // Molecularr 2 (1991)(Amstrad Cent Pour Cent)[cr].dsk
   0xf5751383, // Match Point (1985)(Psion)[cr].dsk
   0xb70a9759, // Mercenary (1987)(Novagen Software).dsk
   0x4882c41a, // Mercenary (1987)(Novagen Software)(de).dsk
   0x54339699, // Mercenary (1987)(Novagen Software)(fr).dsk
   0x7f16b4bd, // Magic Maths (1986)(Players)[cr].dsk
   0xcd9dbf9c, // Manic Miner (1984)(Software Projects)[tape][BASIC 1.0].dsk
   0x82acc11e, // Mega-Corp (1987)(Dinamic Software)(es)[cr].dsk
   0xf4dfdb4a, // Mystery of Arkham Manor, The (1987)(Melbourne House)[cr].dsk
   0xfb0775c4, // Mot (1989)(Opera Soft)(es)(Side B).dsk
   0x14cc7504, // Mot (1989)(Opera Soft)(M2)(Side B).dsk
   0x08530c6e, // Mot (1989)(Opera Soft)(M2)(Side A).dsk
   0x70da91c0, // Mot (1989)(Opera Soft)(es)(Side A).dsk
   0xb69c3207, // Mano Negra (1990)(Micro Mag)(fr)[cr].dsk
   0xcad04368, // Military School (1989)(AM-Mag)[cr].dsk
   0x398be600, // Moonmist (1986)(Infocom)[CPM].dsk
   0x15d53d43, // Mac 1 (19xx)(Scull PD Library)(PD)[cr].dsk
   0xdfeb8592, // Mega Chess (1988)(CP Software)(es)[cr].dsk
   0xbd48490a, // Mega Phoenix (1991)(Dinamic Software).dsk
   0xc0334b8e, // Mantis 2 (1990)(MCM Software)(es)[cr].dsk
   0xcfc732ab, // Masters of the Universe (1987)(Gremlin Graphics Software).dsk
   0xdaf163ba, // Masters of the Universe (1987)(Gremlin Graphics Software)(fr).dsk
   0xccc7f59a, // Megawar (1990)(Genesis Software)[cr].dsk
   0x24af346e, // Maquina del Tiempo, La (1989)(Noel Llopis Artime)(es)[cr].dsk
   0xe002cf9c, // Mille et un Voyages, Les (1989)(Carraz Editions)(fr)(Side A).dsk
   0xd7167e35, // Mille et un Voyages, Les (1989)(Carraz Editions)(fr)(Side B).dsk
   0x3e51adc0, // Metaplex (1988)(Addictive Games).dsk
   0x71e4f836, // Mr Weems and the She Vampires (1987)(Piranha).dsk
   0x056040fa, // Max Headroom (1986)(Quicksilva)[cr].dsk
   0xe12ac3b6, // Mordon's Quest (1985)(Melbourne House)[tape].dsk
   0x62c0e892, // Message from Andromeda (1986)(Interceptor Software)[cr].dsk
   0x671e3957, // Moto Driver (1986)(Loriciels)(fr)[cr].dsk
   0xfb354864, // Mutants (1987)(Ocean Software).dsk
   0x0eae7e77, // Milk Race (1987)(Mastertronic)[cr].dsk
   0x4518ef00, // Mystery of the Nile, The (1987)(Zigurat).dsk
   0xd1fbd4c6, // Murder Off Miami (1986)(CRL Group)[cr].dsk
   0xbd99351f, // Mission 1 - Project Volcano (1985)(Mission Software)[cr].dsk
   0xca7f19e3, // Magic Johnson's Basketball (1990)(Dro Soft)(es).dsk
   0x01938d46, // Manoir de Rochebrune, Le (1985)(CPC)(fr)[cr][f].dsk
   0xa0469898, // Minesweeper (1992)(Dream Soft)(PD)[cr].dsk
   0x950f5102, // Marble Madness Construction Set (1986)(Melbourne House)[cr].dsk
   0x3b567f38, // Marauder (1988)(Hewson).dsk
   0x834d2a48, // Mermaid Madness (1986)(Activision).dsk
   0x858b7ddf, // Meganova - The Weapon (1988)(Dinamic Software)[cr][f].dsk
   0x9567d632, // Mobile Man (1989)(Loriciels)(fr).dsk
   0xaf62ab03, // Macadam Bumper (1985)(PSS)[cr].dsk
   0x7a30b534, // Monty on the Run (1986)(Gremlin Graphics Software).dsk
   0xb7f1a1c7, // Metropolis (1989)(Topo Soft)(es).dsk
   0xc82e3009, // Masters of Space, The - X Levels (1994)(Radical Software)[cr].dsk
   0x764ab702, // Mask III - Venom Strikes Back (1988)(Gremlin Graphics Software).dsk
   0x735be17b, // Marmelade (1987)(MBC Informatique)(fr)[cr].dsk
   0x3c7b9432, // Mercenaire (1986)(Rainbow Productions)[cr].dsk
   0x917f5380, // Meganova (1988)(Dinamic Software)(es).dsk
   0x9e91c2c9, // Mister Gas (1990)(Proein Soft Line)(es).dsk
   0x42646875, // Manhattan 95 (1986)(Ubi Soft).dsk
   0x90fdb297, // Manhattan 95 (1986)(Ubi Soft)(fr).dsk
   0xb40c5cb2, // Murder on the Atlantic (1985)(Cobra Soft)(Side A).dsk
   0x3c816404, // Murder on the Atlantic (1985)(Cobra Soft)(Side B).dsk
   0x80f7e58a, // Masterchess (1984)(Amsoft).dsk
   0xc14e1245, // Mambo (1989)(Positive)(es).dsk
   0x5ca84322, // Madballs (1988)(Ocean Software).dsk
   0xdf728125, // Mange Cailloux (1987)(Ubi Soft)(fr).dsk
   0x92f208ca, // La Mansion Encantada (1984)(Anirog Software)(es)[cr].dsk
   0xafc28cbf, // Manchester United (1990)(Krisalis Software)(M5)(Side B).dsk
   0x76225bd5, // Manchester United (1990)(Krisalis Software)(M5)(Side A).dsk
   0x1f07cc81, // Manchester United (1990)(Krisalis Software)(es).dsk
   0xd3c12dc7, // Maze (1985)(CPC)[cr].dsk
   0x29eb1f2a, // Montsegur (1985)(Norsoft)(fr)[cr].dsk
   0xaeeb7e36, // Muri (1987)(Happy Computer)[cr].dsk
   0x677a184f, // Manoir du Comte Frozarda, Le (1988)(MBC Informatique)(fr)[cr].dsk
   0x3b29d8fe, // Mystery of the Indus Valleys (1987)(Alternative Software)[cr].dsk
   0x055e2b83, // Mythos (1990)(Opera Soft).dsk
   0x73d376a3, // Malefices (1992)(Nicolas Cocaign)(fr)(Side A)[cr].dsk
   0xef8e9b95, // Malefices (1992)(Nicolas Cocaign)(fr)(Side B)[cr].dsk
   0x2d8d5877, // Metro Cross (1987)(US Gold).dsk
   0x2483557e, // Metropol (1988)(Zafiro Software)(es).dsk
   0x6eafbd80, // Moonwalker (1989)(US Gold).dsk
   0x797cd283, // Maddog (1987)(Titus)(fr).dsk
   0x2a5b9b60, // Maddog (1987)(Titus)[cr].dsk
   0xa1c366b6, // Nexus (1986)(Nexus).dsk
   0x363b6a03, // Narco Police (1990)(Dinamic Software)[f].dsk
   0xf048d226, // Nonamed (1986)(Dinamic Software)(es)[cr].dsk
   0xd38a0194, // North & South (1989)(Infogrames)(M3)(Side B).dsk
   0xbf564629, // North & South (1989)(Infogrames)(M3)(Side A).dsk
   0x9299697c, // Navy Moves (1988)(Dinamic Software).dsk
   0xacf62400, // Navy Moves (1988)(Dinamic Software)(es)(Hack)[cr].dsk
   0x89c0c313, // Nocturne (1986)(Alpha Omega Software)[cr].dsk
   0xaecc8465, // Nigel Mansell's Grand Prix (1988)(Martech).dsk
   0x6761ee78, // Night Hunter (1990)(Ubi Soft)(es)(Side A).dsk
   0x414a19d4, // Night Hunter (1990)(Ubi Soft)(fr)(Side B).dsk
   0x159d5a2c, // Night Hunter (1990)(Ubi Soft)(fr)(Side A).dsk
   0xe63d8282, // Night Hunter (1990)(Ubi Soft)(es)(Side B).dsk
   0x06cdf929, // Ninja Scooter Simulator (1988)(Silverbird)[cr].dsk
   0x38c99e42, // Nonterraqueous (1985)(Mastertronic)[cr].dsk
   0xdc356958, // Nemesis IV (1986)(Cascade Games)[cr].dsk
   0x47cd9774, // Nonamed (1986)(Dinamic Software)[cr][f].dsk
   0x480e33f5, // Newmarket (1988)(Computing With The Amstrad)[cr].dsk
   0x93743dc7, // Necris Dome (1987)(Codemasters Software)[cr].dsk
   0xf5b719cc, // Nemesis the Warlock (1987)(Martech Games).dsk
   0xbda05d27, // Ninja Massacre (1989)(Codemasters Software)[cr].dsk
   0x8f0b963f, // Nightshade (1985)(Ultimate Play The Game)[cr].dsk
   0xd9091cec, // Narc (1990)(Ocean Software)(Side B).dsk
   0xadc86f57, // Narc (1990)(Ocean Software)(Side A).dsk
   0xebd801cd, // Nigromante, El (2019)(Ubi Soft)(es)(Hack)(Side A)[cr].dsk
   0xf7e07877, // Nigromante, El (2019)(Ubi Soft)(es)(Hack)(Side B)[cr].dsk
   0xa084af0d, // Netherworld (1988)(Hewson).dsk
   0x99dcfd1b, // New Zealand Story, The (1989)(Ocean Software)(Side A).dsk
   0x8721ca59, // New Zealand Story, The (1989)(Ocean Software)(Side B).dsk
   0x7db9cd99, // Nebulus (1988)(Hewson Consultants)[f].dsk
   0x5ebefb4b, // Not a Penny More, Not a Penny Less (1987)(Domark)(Side A).dsk
   0x22d4030a, // Not a Penny More, Not a Penny Less (1987)(Domark)(Side B).dsk
   0xa26962d6, // Ninja Commando (1989)(Zeppelin Games)[cr].dsk
   0x6c2cdcea, // Nanako Descends To Hell (2019)(The Mojon Twins).dsk
   0xd9fe8817, // Ninja (1990)(Micro Mag)[cr].dsk
   0x358fc009, // Neil (1988)(Alternative Software)[cr].dsk
   0x5c06632f, // Night Raider (1988)(Gremlin Graphics Software).dsk
   0xa816b717, // Nigel Mansell's World Championship (1992)(Gremlin Graphics Software)(Side A).dsk
   0x76ec59d1, // Nigel Mansell's World Championship (1992)(Gremlin Graphics Software)(Side B).dsk
   0x722c18aa, // NeverEnding Story, The (1985)(Ocean Software).dsk
   0xe4ce7e73, // Nodes of Yesod (1986)(Odin Computer Graphics).dsk
   0x380c424f, // Nibbler (1984)(Mosaik Software)[cr].dsk
   0x2d67b1d6, // Nick Faldo Plays the Open (1985)(Mind Games).dsk
   0xb5396b62, // Ninja (1987)(Mastertronic)[cr].dsk
   0x5c2b72fd, // Night Breed (1990)(Ocean Software)(Side B).dsk
   0xf2f19689, // Night Breed (1990)(Ocean Software)(Side A).dsk
   0x10e96c91, // Nythyhel - Files of the Occult (1986)(50-50 Club Software)[cr].dsk
   0xfa3a4ad5, // Ninja Warriors, The (1989)(Virgin Games).dsk
   0x0b35ed50, // Nemesis (1987)(Konami).dsk
   0x1312d1b3, // Number 1 (1986)(Bug-Byte)[cr].dsk
   0xc8ae3fad, // North Star (1988)(Gremlin Graphics Software).dsk
   0x62573108, // Ninja Hamster (1987)(CRL Group).dsk
   0x63916ce0, // Nightmare Park (1985)(Strategy Software)[cr].dsk
   0xe920b1e1, // Night Booster (1985)(Cobra Soft)(fr)[cr].dsk
   0x5867c8e4, // Night Gunner (1989)(Digital Integration)(M3)[codes].dsk
   0xa43eb279, // Night Gunner (1989)(Digital Integration)(M3)[cr].dsk
   0x1e1b4feb, // Nakamoto (1987)(Alpha Omega Software)[cr].dsk
   0xfc53cce8, // Ninja Master (1986)(Firebird)[cr].dsk
   0xdfaa4154, // Nuclear Heist (1986)(Players)[cr].dsk
   0x7807f952, // Nervous Break Down (1985)(Hebdogiciel)(fr)[cr].dsk
   0xd53f9fdb, // No Exit (1990)(Coktel Vision).dsk
   0x803fc2a2, // N.E.X.O.R. (1986)(Design Design Software)[Custom, 64sectors].dsk
   0xeae0e80f, // Night Shift (1991)(US Gold).dsk
   0xb3f243a2, // Nightmare Maze (1986)(CDS Software)[cr].dsk
   0x2ff420da, // Oh Mummy! (1984)(Amsoft)[cr].dsk
   0x6abd6ce2, // Oh Mummy! (1984)(Amsoft)(es)[cr].dsk
   0x42eedb86, // Oriental Games (1990)(Micro Style)(Side B).dsk
   0x317e04e9, // Oriental Games (1990)(Micro Style)(Side A).dsk
   0x505e5041, // Olympiad '86 (1986)(Atlantis Software)[cr].dsk
   0x28dfb62b, // Ormuz (1988)(Iber Soft)(es)[tape].dsk
   0x5138310b, // Out Run Europa (1991)(US Gold)(Side B).dsk
   0x20de516d, // Out Run Europa (1991)(US Gold)(Side A).dsk
   0x3c1c3d56, // Olaf (1992)(Amstrad Cent pour Cent)(fr)[cr].dsk
   0x8099581a, // Omeyad (1989)(Ubi Soft)(fr).dsk
   0x2ce75743, // One Man and his Droid (1985)(Mastertronic)[cr].dsk
   0x5d77d588, // Othello Master (1985)(Power Master)(fr)[cr].dsk
   0x549dfa0a, // Overlander (1988)(Elite Systems).dsk
   0x525d1275, // Official Father Christmas Game, The (1989)(Alternative Software)[cr].dsk
   0x0d7df404, // Obsidian (1985)(Artic Computing)[cr].dsk
   0x4be97fcf, // Orion (Jasap Software)(Spain)[cr].dsk
   0x46d2775d, // Obliterator (1989)(Melbourne House).dsk
   0x2b92d514, // Ocean Conqueror (1988)(Hewson)[cr].dsk
   0x5a1d7d59, // Omega Planete Invisible (1987)(Infogrames)(fr)(Side A).dsk
   0x1fc470b9, // Omega Planete Invisible (1987)(Infogrames)(fr)(Side B).dsk
   0x2317e673, // Orphee (1985)(Loriciels)(fr)(Side A).dsk
   0xa5291e8c, // Orphee (1985)(Loriciels)(fr)(Side B).dsk
   0x523e055e, // Operation Thunderbolt (1989)(Ocean Software)(Side A).dsk
   0x5e0b98bb, // Operation Thunderbolt (1989)(Ocean Software)(Side B).dsk
   0x19f3e1c7, // Out Run (1988)(US Gold)(Side B).dsk
   0x6d119419, // Out Run (1988)(US Gold)(Side A).dsk
   0xffd9594e, // On the Oche (1984)(Artic Computing)[cr].dsk
   0xb9d5e5e3, // Operation Gunship (1989)(Codemasters Software)[cr].dsk
   0xe9cd997f, // Outboard (1990)(Loriciels)(fr).dsk
   0xfce7fa16, // On Cue (1987)(Mastertronic)[cr].dsk
   0x4d91810b, // Oxyda (1987)(CPC)(fr)[cr].dsk
   0xd77dde4b, // Off Shore Warrior (1989)(Titus).dsk
   0x0940d89a, // Operation Wolf (1988)(Ocean Software)(LightGun).dsk
   0x7c6c1e2e, // Operation Wolf (1988)(Ocean Software).dsk
   0xc61bddb3, // Orifice from Outer Space (19xx)(Plasma Touch)[cr].dsk
   0x80bf6a46, // Oeil de Set, L' (1987)(Ubi Soft)(fr)(v2)(Side B).dsk
   0x951a0c7d, // Oeil de Set, L' (1987)(Ubi Soft)(fr)(v2)(Side A).dsk
   0xf334ff41, // Out Of This World (2016)(Boxsoft)[cr].dsk
   0xe45fa8de, // Out of This World (1987)(Ariolasoft).dsk
   0xb8b1db15, // Omega Dimension (1989)(Positive)(es)[cr].dsk
   0x2083b8b5, // On the Run (1985)(Design Design Software)[cr].dsk
   0xf1f7aa40, // Pac-Ball 2 (1991)(CPC Infos)(fr)[cr].dsk
   0x21dc2abb, // Pub Trivia (1989)(Codemasters Software)[cr].dsk
   0x693b9445, // Pajaros de Bangkok, Los (1988)(Dinamic Software)(es).dsk
   0x1580b2b3, // Pipe Crazy! (1990)(Micro Mag)(fr)[cr].dsk
   0x4d86be11, // Paranoia Complex, The (1989)(Magic Bytes)(fr).dsk
   0xfcefc3b9, // Paranoia Complex, The (1989)(Magic Bytes)[cr].dsk
   0x7d6aa040, // Pac-Mania (1988)(Grandslam Entertainments).dsk
   0x25af0923, // Pro Tennis (1986)(Loriciels).dsk
   0x3b9f7b83, // Pac-Man (1986)(T and T)(PD)[cr].dsk
   0xe13a3cc9, // Porky (1987)(PC Schneider International)[cr].dsk
   0x3bf55cb1, // Pac Girl v1.2 (1993)(CPC Infos)[cr].dsk
   0xf1e21224, // Paperboy 2 (1991)(Mindscape).dsk
   0xaaf3476a, // Parabola (1987)(Firebird)[cr].dsk
   0xab5c25ca, // Passagers du Vent (1986)(Infogrames)(fr)(Side B).dsk
   0xd4680ee7, // Passagers du Vent (1986)(Infogrames)(fr)(Side A).dsk
   0x1d80f5ee, // Pirate (1990)(Black System)[cr].dsk
   0x194e6cf4, // Psycho Hopper (1989)(Mastertronic)[cr].dsk
   0x000fc745, // Peasants Tale, A (1988)(Crysys Software)[cr].dsk
   0x5f1a49d4, // Prehistorik 2 (1993)(Titus)(M3).dsk
   0x4cfeb8db, // Pousnik (1991)(CPC Infos)(fr)[cr].dsk
   0x2c878107, // Plundered Hearts (1987)(Infocom)[cr].dsk
   0xe2c87660, // Postman Pat 3 - To the Rescue (1992)(Alternative Software)[cr].dsk
   0xec6fbe67, // P-47 Thunderbolt - The Freedom Fighter (1990)(Firebird Software).dsk
   0x0735fecf, // Playbox (1985)(Norsoft)(fr)[cr].dsk
   0xc1831828, // Pipeline Plumber (1987)(Amstrad Computer User)[cr].dsk
   0xa4c910f5, // Plate-Forme (1988)(MBC Informatique)(fr)[cr].dsk
   0x457d9475, // Philosopher's Quest (1987)(Topologika)[cr].dsk
   0x4ebc8925, // Profanation 2 - Escape from Abu Simbel (2017)(4Mhz).dsk
   0x0ddda48d, // Profanation 2 - Escape from Abu Simbel (2017)(4Mhz)(es).dsk
   0xe0107438, // Poussy (1991)(Fanatic)(fr).dsk
   0xf49d6612, // Powerplay - Game of the Gods (1986)(Arcana Software Design)[cr].dsk
   0xad884f7d, // Park Patrol (1986)(Firebird)[cr].dsk
   0x903781fe, // Pro Golf Simulator (1990)(Codemasters Software)[cr].dsk
   0x26d85124, // Pictionary (1989)(Domark)(fr).dsk
   0xb877fe2c, // Pictionary (1989)(Domark)(es).dsk
   0xda8c298f, // Perestroika 2 - Le Retour (1991)(BLB Soft)(fr)[cr].dsk
   0xe3f0e142, // Psyborg (1992)(Loriciels).dsk
   0xcd719517, // Postman's Destiny (1989)(PC Amstrad International)[cr].dsk
   0x866335da, // Poule Position (1985)(Firebird)(fr)[cr].dsk
   0x4257037d, // Profanation (1987)(Chip).dsk
   0xaf1988fe, // Pheonix Mission, The (1987)(Entertainment & Computer Products)[cr].dsk
   0xc84aced2, // Poli Diaz Boxeo (1991)(Opera Soft)[o][codes].dsk
   0x24b0497a, // Passengers On The Wind (1987) (Infogrames)(Side A).dsk
   0x603876e6, // Passengers On The Wind (1987) (Infogrames)(Side B).dsk
   0x9af2061b, // Planetfall (1983)(Infocom)[CPM].dsk
   0xc071d82b, // Panza Kick Boxing (1990)(Loriciels)(fr).dsk
   0x71d99b9e, // Panza Kick Boxing (1990)(Loriciels).dsk
   0x259559b5, // Pinball Power (1989)(Mastertronic Plus).dsk
   0x89d771f2, // Pang (1990)(Schneider Magazin)[cr].dsk
   0xd290885b, // Pulsator (1988)(Martech).dsk
   0x3baa8f66, // Paragliding Simulation (1991)(Loriciels)(fr).dsk
   0x6677ce1f, // Psycho Pig UXB (1988)(US Gold).dsk
   0xbfa602b8, // Play Your Cards Right (1986)(Britannia Software)[cr].dsk
   0x98b87c3b, // Plasmatron (1988)(CRL Group).dsk
   0x797c7ff4, // Platoon (1988)(Ocean Software).dsk
   0x0c0ba704, // Pyjamarama (1984)(Mikrogen).dsk
   0x8af478eb, // Pit-Fighter (1991)(Domark)(Side A)[b].dsk
   0x1f135fe1, // Pit-Fighter (1991)(Domark)(Side B)[b].dsk
   0x525dd4fb, // Plaga Galactica, La (1984)(Amsoft)(es)[tape].dsk
   0x961c3ab4, // Patience (1987)(Computing With The Amstrad)[cr].dsk
   0x0adcd87e, // Prokyon (1986)(Pride Soft)(de)[cr].dsk
   0x3cc3f2a5, // Pacman (1989)(AM-Mag)[cr].dsk
   0x872b7d18, // Pawn, The (1987)(Rainbird Software)(Side A).dsk
   0xad41e315, // Pawn, The (1987)(Rainbird Software)(Side B).dsk
   0x185f6085, // Puzznix (1992)(CPC Amstrad International)[cr].dsk
   0x3618dcda, // Potsworth & Co. (1992)(Hi-Tec Software).dsk
   0x655fbeea, // Panic Dizzy (1991)(Codemasters Software)[cr].dsk
   0xe874e175, // Pac-Boy (1989)(Micro Mag)[cr].dsk
   0x615f4ff1, // Pick'N Pile (1990)(Ubi Soft).dsk
   0x33615414, // Punchy (1984)(Amsoft).dsk
   0x200c4423, // Passagers du Vent 2 (1987)(Infogrames)(fr)(Side A).dsk
   0x4e5758d1, // Passagers du Vent 2 (1987)(Infogrames)(fr)(Side B).dsk
   0x2475b163, // Passagers du Vent 2 (1987)(Infogrames)(Side B).dsk
   0xd1c258d1, // Passagers du Vent 2 (1987)(Infogrames)(Side A).dsk
   0xadd097a0, // Pilots (1990)(Bollaware)(de)(PD)[cr].dsk
   0x19071d88, // Paperboy (1987)(Elite Systems).dsk
   0x752e93aa, // Periscope Up (1989)(Atlantis Software)[cr].dsk
   0x1df0ab01, // Pool (1989)(Micro Mag)[cr].dsk
   0x12b949cc, // Psycho City (1989)(Players)[cr].dsk
   0x60d2e10f, // Poogaboo (1991)(Opera Soft)(es)[o][codes].dsk
   0xccaf43e1, // Poogaboo (1991)(Opera Soft)(es)[cr].dsk
   0x4f838837, // Phenix Noir (1987)(Chip)(fr).dsk
   0x5b1e7942, // Packy (1987)(CPC)(fr)[cr].dsk
   0x5864697e, // Phantom Club (1988)(Ocean).dsk
   0x9178822a, // Project Future (1985)(Gremlin Graphics Software)[cr].dsk
   0x42b3f15d, // Poseidon (1985)(Coktel Vision)(fr).dsk
   0x9298d89e, // Power and Magic (1990)(Zigurat Software)(es).dsk
   0xca45ab7d, // Prodigy (1986)(Activision).dsk
   0xf964db5d, // Prince of Persia (1990)(Broderbund Software).dsk
   0xd9282631, // Panzadrome (1985)(Ariolasoft)[cr].dsk
   0xb7296fc1, // Paladin (1987)(Bubblebus Software).dsk
   0xa60eba03, // Phantis (1987)(Dinamic Software)(es).dsk
   0xf9c8d1ad, // Power Tetris (1991)(PC Amstrad International)[cr].dsk
   0x0cc6a3f5, // Pacte, Le (1986)(Loriciels)(fr)(Side A).dsk
   0xe3f1b5db, // Pacte, Le (1986)(Loriciels)(fr)(Side B).dsk
   0xd93dd522, // Play Bac (1987)(Microids)(fr)(Side B).dsk
   0xcbab7a0d, // Play Bac (1987)(Microids)(fr)(Side A).dsk
   0xf73e3c45, // Puzznic (1990)(Ocean Software).dsk
   0xadc89071, // Penggy (1987)(Chip)(fr)[cr].dsk
   0xe7f8c6e2, // Pac Punk (1987)(Bretagne Edit presse)(fr)[cr].dsk
   0x09f1d651, // Pac-Land (1989)(Grandslam).dsk
   0x03d3a496, // Power Drift (1989)(Activision).dsk
   0x4c690319, // Piclos (1989)(Micro Mag)[cr].dsk
   0x632af5d7, // Pirates (1987)(Microprose Software)(Side B).dsk
   0xf1684e55, // Pirates (1987)(Microprose Software)(Side A).dsk
   0xdb889d3c, // Pirates (1987)(Microprose Software)(fr)(Side B).dsk
   0x793e37f5, // Pirates (1987)(Microprose Software)(fr)(Side A).dsk
   0x2f3d91e7, // Popeye (1986)(Piranha).dsk
   0x4a848c11, // Purple Saturn Day (1989)(Ere Informatique).dsk
   0xc4190bb5, // Pyramides d'Atlantys, Les (1986)(Microids)(fr).dsk
   0xbe5ec8f7, // Python (1986)(Chip)(fr)[cr].dsk
   0xb5c6ff7c, // Python (1986)(Chip)(es)[cr].dsk
   0xd8b60327, // Penguins (1992)(Amstrad Action)[cr].dsk
   0x55f1325d, // Peter Pack Rat (1989)(Silverbird Software)[cr][f].dsk
   0x65ee3dbb, // Pro Golf - Pebble Beach (1987)(Atlantis Software)[cr].dsk
   0x7fec6523, // Pro Golf - Sunningdale (1987)(Atlantis Software)[cr].dsk
   0x9aae636e, // Professional BMX Simulator (1988)(Codemasters Software)(Side A)[cr].dsk
   0x8c511282, // Professional BMX Simulator (1988)(Codemasters Software)(Side B)[cr].dsk
   0x815924e5, // Peter Shilton's Handball Maradona (1986)(Grandslam)[cr].dsk
   0x84539cdc, // Pipe Mania (1990)(Empire).dsk
   0xd4892cab, // Pogostick Olympics (1987)(Firebird)[cr].dsk
   0x33bfdb3a, // Perestroika (1988)(CPC)[cr].dsk
   0xa811651d, // Pharaon (1987)(Loriciels)(fr)(Side A).dsk
   0x99d29f4f, // Pharaon (1987)(Loriciels)(fr)(Side B).dsk
   0xdfaea603, // Pacplant (1988)(Potplant Productions)[cr].dsk
   0x6c595eda, // Penalty Soccer (1990)(Gamebusters)[cr].dsk
   0x409193ad, // Plumpy (1990)(CPC Infos)(fr)(Side B)[cr].dsk
   0xae4a38ad, // Plumpy (1990)(CPC Infos)(fr)(Side A)[cr].dsk
   0x4c89d3dd, // Passing Shot (1989)(Image Works).dsk
   0xd1c1a8f8, // Perico Delgado Maillot Amarillo (1989)(Topo Soft)(es).dsk
   0x9a8b051f, // Paws (1985)(Artic Computing)[cr].dsk
   0xe18ec52b, // Push (1995)(Eliot)(PD)[cr].dsk
   0xfe13d596, // Punk Star (1988)(Iber Soft)(es)[cr].dsk
   0x9d2a8c1a, // Plot, The (1987)(Firebird)[cr].dsk
   0xd4888386, // Paranoia (1994)(Power System)[cr].dsk
   0xd6318ce3, // Pipeline (1991)(PC Amstrad International)(fr)[cr].dsk
   0xc599b2d0, // Pipeline (1991)(PC Amstrad International)[cr].dsk
   0x7a74d9fd, // Pro Tennis Tour (1990)(Ubi Soft)[cr].dsk
   0x70af3425, // Pro Tennis Tour (1990)(Ubi Soft)[codes].dsk
   0x33d6eaad, // Poder Oscuro, El (1988)(Zigurat Software)(es).dsk
   0xe0fc510b, // Pharaon (2012)(Loriciels)(es)(Hack)(Side A)[cr].dsk
   0xc5cd314d, // Pharaon (2012)(Loriciels)(es)(Hack)(Side B)[cr].dsk
   0xf81c4337, // La Pulga (1984)(Indescomp)[cr].dsk
   0xf8f9dda6, // Pinaut (1986)(Minsoft)[cr].dsk
   0x5f58268a, // Pink Panther (1988)(Magic Bytes).dsk
   0x181ddcde, // Predator (1988)(Activision).dsk
   0xc5a3ce66, // Project-X - The Microman (1986)(Global Software)[cr].dsk
   0xb6c59177, // Pacific (1986)(ERE Informatique)(M3).dsk
   0x4a3b42c0, // Peter Beardsley's Soccer (1989)(Grandslam).dsk
   0xf5e8002e, // Pinball Magic (1990)(Loriciels)(M2)(v2)(Side B).dsk
   0x5dce5c79, // Pinball Magic (1990)(Loriciels)(M2)(v2)(Side A).dsk
   0xb279c7ea, // Passager du Temps, Le (1987)(ERE Informatique)(fr)(Side B).dsk
   0xfc391fd4, // Passager du Temps, Le (1987)(ERE Informatique)(fr)(Side A).dsk
   0xd6d70b73, // Peter Pan (1988)(Coktel Vision)(Side B).dsk
   0x10548a7a, // Peter Pan (1988)(Coktel Vision)(Side A)[CPM].dsk
   0xc93eb89b, // Pop-Up (1990)(Loriciels)(fr)(Side A).dsk
   0xcb25934f, // Pop-Up (1990)(Loriciels)(fr)(Side B).dsk
   0xc8c547d0, // Prince de Perse (1990)(Broderbund Software)(fr)[codes].dsk
   0x8b343b89, // Postman Pat (1988)(Alternative Software)[cr].dsk
   0x5ce924ba, // Professional Ski Simulator (1987)(Codemasters Software)[o].dsk
   0x5b9973af, // Prize, The (1985)(Amsoft)[tape].dsk
   0xb84d97ea, // Pulsoid (1988)(Mastertronic)[tape].dsk
   0xee11df0f, // Pinball Wizard (1985)(CP Software)(es).dsk
   0xcaf968a1, // Pinball Wizard (1985)(CP Software)[cr].dsk
   0x4294d4b2, // Party Machine (1987)(Computing With The Amstrad)[cr].dsk
   0x7b084af7, // PHM Pegasus (1986)(Electronics Arts).dsk
   0x0a96fed4, // Predator 2 (1991)(Image Works).dsk
   0xef4e2cae, // Prohibition (1987)(Infogrames)(M4).dsk
   0x66e9ffbf, // Pneumatic Hammers (1987)(Firebird)[cr].dsk
   0x4e185974, // Quest for the Golden Eggcup, The (1988)(Mastertronic)[cr].dsk
   0x80d595c7, // Qor (1986)(Maltdale)[cr].dsk
   0xb8a5d5c8, // Quack a Jack (1984)(Amsoft).dsk
   0x03c2e55b, // Quad (1987)(Microids)(fr).dsk
   0x4af53293, // A Question Of Scruples (1987)(Leisure Genius)(Side A).dsk
   0x9c2aba13, // A Question Of Scruples (1987)(Leisure Genius)(Side B).dsk
   0x4015d81d, // Quaterne (1985)(Sprites)(fr)[cr].dsk
   0x8122f603, // Quadrant (1993)(Sleepwalker PDS)[cr].dsk
   0xab988af2, // Quad-X II (1991) (Edmund Spicer)[cr].dsk
   0x3a0e0661, // Quete De La Pierre Mindstone (1987)(The Edge)(fr)[cr].dsk
   0x1c3811c5, // Quadrel (1991)(Loriciels)(fr).dsk
   0x0e58dcf4, // Quick Draw McGraw (1991)(Hi-Tec Software)[cr].dsk
   0x254b5168, // Quad-X (1991)(Edmund SPICER)[cr].dsk
   0x7f1fca7d, // Quarters (1987)(Computing With The Amstrad)[cr].dsk
   0xf91b4aae, // Questprobe 3 - Fantastic Four (1985)(US Gold)[cr].dsk
   0xacf00a30, // Quest for the Mindstone (1986)(The Edge)[cr].dsk
   0x855a0d40, // Q10 Tank Buster (1991)(Zeppelin Games).dsk
   0x3e40eb55, // Qabbalah (1986)(Amsoft)[cr].dsk
   0x4f55e966, // Quartet (1987)(Activision)[b].dsk
   0x313915c9, // Questor (1986)(Cascade Games).dsk
   0xee8f9219, // Question of Sport, A (1988)(Elite Systems).dsk
   0xd1a63dcf, // Room Ten (1986)(CRL Group)(M3).dsk
   0xf70eee66, // Robocop (1989)(Ocean Software)(fr).dsk
   0xb6017864, // Robocop (1989)(Ocean Software).dsk
   0xd7dd6963, // Renegade (1985)(Kuma Computers)[cr].dsk
   0xc42bdd32, // Romba (1990)(Micro Mag)(fr)[cr].dsk
   0xa0eb703d, // Rebel Planet (1986)(US Gold)[cr].dsk
   0x33cb07bc, // Robot Ron (1985)(A-Team)(es)[cr].dsk
   0x4948162e, // Robot Ron (1985)(A-Team)[cr].dsk
   0x72e302b8, // Reisende im Wind 2 (1987)(Infogrames)(de)(Side B).dsk
   0xcce626da, // Reisende im Wind 2 (1987)(Infogrames)(de)(Side A).dsk
   0x0324cfc6, // Roland on the Ropes (1984)(Zigurat).dsk
   0x8953a047, // Robozone (1991)(Image Works).dsk
   0x71101441, // Rambo - First Blood Part II (1986)(Ocean Software)[tape].dsk
   0x2556636c, // Rock Run (1989)(Swift Software)[cr].dsk
   0xd93567e6, // Riding the Rapids (1987)(Players)[cr].dsk
   0x07e5bc58, // Rock Hopper (1985)(Amstrad Computer User)(fr)[cr].dsk
   0xcdc84017, // Ricochet (1987)(Firebird)[tape].dsk
   0xe8a597cf, // Reward (1991)(JLMD)(fr)(PD)[cr].dsk
   0x04e8e31c, // Reflector (1988)(AM-Mag)(fr)[cr].dsk
   0xd6870307, // Rasterscan (1987)(Mastertronic)[cr].dsk
   0xd0f6eaf5, // Ralf (1990)(CPC)[cr].dsk
   0xf121ebe5, // Reversi Champion (1986)(Loriciels)(fr).dsk
   0x349f4dcd, // Ruff and Reddy in The Space Adventure (1990)(Hi-Tec Software)[cr].dsk
   0x2b82aab6, // Red L.E.D. (1987)(Starlight Software).dsk
   0x5c2c4ee3, // Revolver (1988)(Alternative Software)[cr].dsk
   0xfff25344, // R.B.I. 2 Baseball (1991)(Domark).dsk
   0x920d4d8e, // Rig Attack (1985)(Tynesoft)[cr].dsk
   0xf851e598, // Roland Goes Square Bashing (1984)(Amsoft)[tape].dsk
   0x5755688c, // Rick Dangerous (1989)(Firebird).dsk
   0xbff172a0, // Rock 'n Wrestle (1985)(Melbourne House)[tape].dsk
   0x8690ff53, // Rolling Thunder (1988)(US Gold).dsk
   0x29acc063, // Rody & Mastico (1988)(Lankhor)(fr)(Side A).dsk
   0xc114bf28, // Rody & Mastico (1988)(Lankhor)(fr)(Side B).dsk
   0xbd62835d, // Rollaround (1988)(Mastertronic)[cr].dsk
   0x266bb86d, // Redhawk (1986)(Melbourne House)[cr].dsk
   0x906f3991, // Rally Driver (1986)(Hill MacGibbon)[cr].dsk
   0x234854ab, // Reflex (1987)(Players)[cr].dsk
   0x87f88b92, // Running Man, The (1989)(Grandslam)(Side B).dsk
   0xd66297b4, // Running Man, The (1989)(Grandslam)(Side A).dsk
   0x23b750e8, // Rogue (1987)(Mastertronic)[tape].dsk
   0x66848ef1, // Revolution (1986)(US Gold).dsk
   0xef892c7f, // Renaud - Sidewalk (1987)(Infogrames)[cr].dsk
   0x20df0713, // Renaud - Sidewalk (2021)(Infogrames)(es)(Hack)[cr].dsk
   0x6fb15934, // Rally Simulator (1988)(Zeppelin Games)[cr].dsk
   0x108bd162, // Rock 'n Roller (1988)(Topo Soft)(es)[tape].dsk
   0xad3702d1, // Return to Oz (1986)(US Gold)[tape].dsk
   0x6acf4045, // Rock 'n Roll (1989)(Rainbow Arts)(Side B).dsk
   0x3f72c8bf, // Rock 'n Roll (1989)(Rainbow Arts)(Side A).dsk
   0xd4144a8b, // Race Against Time, The (1988)(Codemasters Software)[cr].dsk
   0x99baf9f9, // Roland Ahoy! (1984)(Amsoft).dsk
   0xe4057c25, // Rampage (1987)(Activision).dsk
   0x7789c21e, // Radzone (1986)(Mastertronic)[cr].dsk
   0x11d42e74, // Ravening (1992)(Sylvain Guehl)(PD)(fr)[cr].dsk
   0x0985c1ea, // Robin of Sherwood - The Touchstones of Rhiannon (1985)(Adventure International)[cr].dsk
   0x2706e957, // RFX (1993)(CPC Infos)(fr)[cr].dsk
   0x74ea3cb1, // Rock Raid (1985)(Kuma Computers)(es)[cr].dsk
   0x843d62ca, // Rock Raid (1985)(Kuma Computers).dsk
   0x268fff6a, // Rastan (1987)(Ocean Software)(Side A).dsk
   0xf083dfe6, // Rastan (1987)(Ocean Software)(Side B).dsk
   0xf01c4e6d, // Rocky - Rocco (1985)(Dinamic Software)[cr].dsk
   0x8fc2bbe2, // Reisende im Wind (1986)(Infogrames)(de)(Side B).dsk
   0x2238a34f, // Reisende im Wind (1986)(Infogrames)(de)(Side A).dsk
   0xed8cbab5, // Ring des Lichts, Der (1997)(Frank Senft)(PD)(de).dsk
   0x400bab11, // Revenge of the C5 (1986)(Atlantis Software)[cr].dsk
   0xffa14122, // Rex Hard (1987)(Mister Chip)(es).dsk
   0xaaa86825, // River Race (1990)(PC Amstrad International)[cr].dsk
   0xf79e9e5d, // Rescate en el Golfo (1990)(Opera Soft)(M2)[codes].dsk
   0xd4e464e0, // Realm (1987)(Firebird)[tape].dsk
   0xb3a24dbf, // Road Blasters (1988)(US Gold).dsk
   0x617f615b, // Ranarama (1987)(Hewson Consultants).dsk
   0x39e92a63, // Rescate Atlantida (1989)(Dinamic Software)(es).dsk
   0x827cdf1e, // Raster Runner (1990)(Mastertronic)[cr].dsk
   0x2f376c17, // Rugby Boss (1989)(Alternative Software)[tape].dsk
   0x404937ef, // Runestone (1985)(Firebird)[tape].dsk
   0x658cbef1, // Rocky Horror Show, The (1985)(CRL Group)(v1.1).dsk
   0x239a14e3, // Road Runner (1985)(US Gold)(v2).dsk
   0xad893638, // RAM (1990)(Topo Soft)(es).dsk
   0x3d857ff5, // Roland in Space (1984)(Amsoft).dsk
   0xd171a650, // Roland in Space (1984)(Amsoft)(es).dsk
   0xf2d804ab, // Red Heat (1989)(Ocean Software).dsk
   0xf468aa03, // Roland in the Haunted House (1985)(Amstrad Computer User)(es)[cr].dsk
   0xac08ec57, // Rigel's Revenge (1987)(Mastertronic)[cr].dsk
   0x0c0124f1, // Run for Gold (1985)(Five Ways Software)[tape].dsk
   0xb1e19b14, // Rally II (1985)(Loriciels)(fr).dsk
   0x93962139, // Rally II (1985)(Loriciels).dsk
   0x4a61c20c, // Rod-Land (1991)(Storm Software)(Side A).dsk
   0xc37f2175, // Rod-Land (1991)(Storm Software)(Side B).dsk
   0x6e33d7e5, // Robocide (19xx)(Asclepios Software)(PD)[cr].dsk
   0xbbf59693, // Ralf II (1993)(CPC Infos)[cr].dsk
   0xbbf3d2c6, // Rath-Tha (1989)(Positive)(es)[cr].dsk
   0x9e1ce74a, // Royal Quest, The (1984)(Timeslip Software)[cr].dsk
   0x32bb9762, // Roland Goes Digging (1984)(Amsoft).dsk
   0xd7e834b9, // Roland on the Run (1984)(Amsoft).dsk
   0xc6c84484, // R-Type (2012)(Easter Egg).dsk
   0x953353f4, // Rad Ramp Racer (1990)(Mastertronic)[cr].dsk
   0x68e69274, // Real Ghostbusters, The (1989)(Activision)(v2)(Side B).dsk
   0x42fb20b8, // Real Ghostbusters, The (1989)(Activision)(v2)(Side A).dsk
   0xf8b63b53, // Rat Connection (1988)(MBC Informatique)(fr)[cr].dsk
   0x558abe44, // Rygar (1987)(US Gold)(v2).dsk
   0x31f0ac5b, // Ramparts (1987)(US Gold).dsk
   0x3f3f84de, // Red Arrows, The (1985)(Database Educational Software).dsk
   0x50c8b233, // Relief Action (1987)(Loriciels)(fr).dsk
   0x02bde426, // Reveal (1988)(Mastertronic)[tape].dsk
   0x6fc53a3a, // Road Toad (1986)(Popular Computing Weekly)[cr].dsk
   0x2f40ccd7, // Race, The (1990)(Amstrad Adventure PD Library)[cr].dsk
   0xc40ae9b4, // Roog (1990)(Amstrad Adventure PD Library)[cr].dsk
   0xd961f6bb, // Run the Gauntlet (1989)(Ocean Software).dsk
   0xa4e6b5e8, // Redhawk II - Kwah! (1986)(Melbourne House)[cr].dsk
   0xd11aa568, // Rainbow Islands (1989)(Ocean Software).dsk
   0xfc540661, // Ring of Darkness, The (1985)(Wintersoft)[cr].dsk
   0xe17f30fb, // Rebotes (1984)(Amstrad Computer User)(es)[cr].dsk
   0x47d6fe0a, // Rally (1986)(CPC)(fr)[cr].dsk
   0xa6f175bf, // Rambo III (1988)(Ocean)[f].dsk
   0x465c7238, // RC Quest (1993)(Moving Pixels)[cr].dsk
   0xb5bb311e, // Robinson Crusoe (1987)(Coktel Vision)(fr)(Side B).dsk
   0xc3273263, // Robinson Crusoe (1987)(Coktel Vision)(fr)(Side A).dsk
   0xc2445018, // Ricochet (1986)(Blaby Computer Games)[tape].dsk
   0x6e9aadca, // Raffles - Inside Outing (1987)(The Edge).dsk
   0x0aca0e0f, // Roland in Time (1986)(Amsoft).dsk
   0x371eaac1, // Roland in the Caves (1984)(Indescomp).dsk
   0x3d52ff08, // Rescue on Fractalus (1986)(Activision)[b].dsk
   0xa7eabae7, // Rally Cross (1989)(Anco Software).dsk
   0x0edf0b5f, // Roulette (1987)(Computing With The Amstrad)[cr].dsk
   0xa53ffbf8, // Robbbot (1986)(ERE Informatique)(M3).dsk
   0x10712a0c, // Rescue Atlantis (1988)(Dinamic Software)[cr][f].dsk
   0xdf16b5d3, // Renegade (1987)(Ocean Software)[f].dsk
   0x89cc6b51, // Rogue Trooper (1986)(Piranha).dsk
   0x7a07199a, // Raid Sur Tenere (1985)(Coktel Vision)(fr).dsk
   0x371023fd, // Rick Dangerous II (1990)(Micro Style).dsk
   0x3b61fc45, // Radius (1987)(Players)[cr].dsk
   0x804acec4, // Renegade III - The Final Chapter (1989)(Imagine Software).dsk
   0xe85fafd6, // Reversi - Challenger (1985)(Cobra Soft)(M2).dsk
   0x0f7abcdf, // Retour d'Ali Gator, Le (1989)(Micro Mag)(fr)[tape].dsk
   0xa67ddad0, // RX 220 (1987)(Microids)(fr)[cr].dsk
   0x9854f2ff, // Reckless Rufus (1992)(Alternative Software)[cr].dsk
   0xf4e63364, // Rik the Roadie (1988)(Alternative Software)[tape].dsk
   0xe6a652f5, // Ramblas - El Caso Vega (1986)(Mind Games Espana)(es)[cr].dsk
   0x07cafada, // Sweevo's World (1986)(Gargoyle Games).dsk
   0xe97478db, // Slap Fight (1987)(Ocean Software).dsk
   0x86cd863f, // Saint Dragon (1990)(Storm Software).dsk
   0x41703c02, // Summer Games II (1989)(Epyx)(Side A).dsk
   0x3ed4f3cc, // Summer Games II (1989)(Epyx)(Side B).dsk
   0x62dd3a2b, // Stress (1985)(Cobra Soft)(fr)[cr].dsk
   0x7963c719, // Scalextric (1987)(Leisure Genius)[cr].dsk
   0x9f46b8f7, // Sydney Affair, The (1986)(Infogrames).dsk
   0xb9625be5, // Starbyte (1987)(Mister Chip)(es)[tape].dsk
   0x6e1d7ad5, // Skate or Die (1989)(Electronic Arts)(Side A).dsk
   0x6d19167a, // Skate or Die (1989)(Electronic Arts)(Side B).dsk
   0x31b7091c, // Sir Lancelot (1984)(Melbourne House)[tape].dsk
   0xb26585d2, // Super Ali (1990)(CPC Infos)(fr)[cr].dsk
   0x3343b8d7, // Sparse Invaders (1986)(FUK)[cr].dsk
   0xfd79cead, // Space Harrier (1986)(Elite Systems).dsk
   0x4675e22e, // Simulador Profesional de Tenis (1990)(Dinamic Software)(es).dsk
   0x438616e4, // Secte Noire, La (1990)(Lankhor)(fr)(Side B).dsk
   0x95b94081, // Secte Noire, La (1990)(Lankhor)(fr)(Side A).dsk
   0x5483ddb7, // Split Personalities (1986)(Domark).dsk
   0xd3de0256, // Super Chess (1984)(CP Software)[cr].dsk
   0x910dbea7, // Stunt Man Seymour (1992)(Codemasters Software)[cr].dsk
   0xa45e6806, // Secret Diary of Adrian Mole, The (1985)(Mosaic Publishing)[tape].dsk
   0x4aa7b255, // Star Wars - The Empire Strikes Back (1988)(Domark).dsk
   0xd4fe12b1, // Shockway Rider (1986)(FTL).dsk
   0xed2d0b83, // Street Fighter (1988)(Us Gold).dsk
   0x9547c0eb, // Shove a Sledge (1987)(Gremlin Graphics Software)[tape].dsk
   0xb0540671, // Sol Negro (1989)(Opera Soft)(es).dsk
   0xa02ee4f7, // Shark (1989)(Players Premier)[tape].dsk
   0x29348c34, // Street Gang (1988)(Players)[f].dsk
   0x30ea42d7, // Star Commando (1984)(Terminal software)[tape].dsk
   0x2652e26e, // Strip Takin (19xx)(Eric Hiller)(fr)[cr].dsk
   0xfcb65dde, // Solar Invasion (1989)(Mastertronic)(LightGun)[cr].dsk
   0x9018ae68, // Spy vs Spy III - Arctic Antics (1988)(Databyte).dsk
   0xb115061b, // Sky Hunter (1988)(ERE Informatique)(fr).dsk
   0x46f46c53, // Skull & Crossbones (1991)(Domark).dsk
   0xc1754e9e, // Subtera Puzlo (2013)(Egotrip, Redbox).dsk
   0xe04c8351, // Space Froggy 2 (19xx)(Glenco)[cr].dsk
   0xe84c1b56, // Samantha Fox Strip Poker (1986)(React Software).dsk
   0xae559ca9, // Super Ski (1987)(Microids)[f].dsk
   0x98ec5c36, // Super Ski (1987)(Microids)(fr)[f].dsk
   0x745df326, // Sailing (1987)(Activision).dsk
   0xc331b18d, // Simon Dice (19xx)(Amstrad Action)(PD)(es)[cr].dsk
   0xc25543ff, // Speed King (1986)(Mastertronic)[cr].dsk
   0x6c33a78e, // Speed King (1986)(Mastertronic)(es)[cr].dsk
   0x32baf3a8, // Silent Shadow (1988)(Topo Soft)(es).dsk
   0x6bc6492b, // Super Sam (1985)(Budgie)[cr].dsk
   0xa035e15b, // Short's Fuse (1984)(Firebird)[tape].dsk
   0x702b10da, // Starion (1985)(Melbourne House)[cr].dsk
   0xe2f225a3, // Shadow Warriors (1990)(Ocean Software).dsk
   0x0ad8c6d7, // Smash (1988)(PC Amstrad International)[cr].dsk
   0xe4c85712, // Swords & Sorcery (1985)(PSS).dsk
   0x8f69c29e, // Souls of Darkon (1985)(Taskset).dsk
   0x16bbd7b9, // Super Pipeline II (1985)(Amsoft).dsk
   0xa1754ab2, // Space Froggy (1990)(Glenco Software)[cr].dsk
   0xb1801fa7, // Secrets of Ur, The (19xx)(Amstrad Adventure PD Library)(PD)[cr].dsk
   0xe103b180, // Super Tetris (1997)(Frequency)(PD)[cr].dsk
   0xcb3f9396, // Stationfall (1987)(Infocom)[CPM].dsk
   0x335b8c5e, // Starstrike II (1986)(Firebird).dsk
   0xcc9a8724, // Snoball in Hell (1989)(Atlantis Software)[tape].dsk
   0x766a1630, // Skywar (1992)(CPC Infos)[cr].dsk
   0x2a17a0be, // Silicon Dreams (1986)(Level 9 Computing)(v2).dsk
   0xbae01a82, // Silent Service (1986)(Microprose Software).dsk
   0x1c2bc5e6, // Stryfe - The Everlasting Battle (1986)(ERE Informatique)(fr).dsk
   0xf279a823, // Screwball (1985)(CDS Software LTD)[cr].dsk
   0xc22c04c0, // Sport of Kings (1986)(Mastertronic)[tape].dsk
   0xfe604da8, // Salamander (1988)(Konami).dsk
   0xaeba8e2d, // Soko-Ban (1989)(Schneider Computer Partner)(de)[cr].dsk
   0xda93dcb2, // Spots (1993)(Jaysoft)(PD)[cr].dsk
   0x95204f0b, // Soldier of Light (1989)(ACE)(es).dsk
   0xa97a4c9b, // Skyfox (1985)(Electronics Arts).dsk
   0xe629f5aa, // Superman - The Game (1985)(First Star Software)[cr].dsk
   0xdf1fbc63, // Satelit (1988)(CPC)(fr)[cr].dsk
   0x07a97eed, // Super Skweek (1990)(Loriciels)(fr).dsk
   0x1993c727, // Super Skweek (1990)(Loriciels)[b].dsk
   0x583ff05a, // Stainless Steel (1986)(Mikrogen).dsk
   0xedf6e15d, // Sirwood (1990)(Opera Soft).dsk
   0xe72d55be, // Spaced Out (1990)(Amstrad Adventure PD Library)[tape].dsk
   0x1b3bf900, // Supertripper (1985)(Indescomp)(es).dsk
   0xf54901d3, // Star Control (1991)(Accolade).dsk
   0xfbd21696, // Samurai Warrior - The Battles of Usaji Yojimbo (1988)(Firebird).dsk
   0xc50b67b4, // Subway Vigilante (1989)(Players Premier)[tape].dsk
   0xe3fbe93f, // Scalextric (1987)(Leisure Genius)(fr)(v2).dsk
   0x7dd3f5cf, // Scramble Spirits (1990)(Grandslam).dsk
   0x0e9b2124, // Stunt Car Racer (1990)(Micro Style).dsk
   0x927f9670, // Soleil Noir (1989)(Opera Soft)(fr).dsk
   0xa32d702c, // Star Trap (1989)(Loriciels)(fr)(Side B)[cr].dsk
   0x7f758031, // Star Trap (1989)(Loriciels)(fr)(Side A)[cr].dsk
   0x85597be5, // Slightly Magic (1992)(Codemasters Software)[cr].dsk
   0x91a8d403, // Space Base (1986)(Computer Gamer)[cr].dsk
   0xe7e51b18, // Simon (1988)(Computing With The Amstrad)[cr].dsk
   0xc83f6f3b, // Storm Warrior (1989)(Elite Systems)[cr].dsk
   0x2a4e9503, // Saboteur 2 (1987)(Durell Software)[f].dsk
   0x6c98f5be, // Sexy Words (1988)(H. Monchatre)(PD)(fr)[cr].dsk
   0x1ba876a5, // Strip Ball (1995)(Digital Dream)(fr)[cr].dsk
   0x1ee9ee39, // Sub-Marine (1989)(Amstrad Cent Pour Cent)(fr)[cr].dsk
   0x73736e58, // Spitting Image (1988)(Domark).dsk
   0xbca83ee8, // Storm II - The Fear (1987)(Mastertronic)[cr].dsk
   0xd2d336bd, // Space Shuttle Simulateur (1985)(Loriciels)(fr)[cr].dsk
   0x7fe4abc2, // Survivant, Le (1985)(PSS)(fr)[cr][f].dsk
   0xc58820bf, // Sdaw (1990)(Lankhor)(fr)(Side A).dsk
   0x498bc11a, // Sdaw (1990)(Lankhor)(fr)(Side B).dsk
   0xb9c39b3d, // Star-Bores (1988)(Neutrino Software)[cr][CPM].dsk
   0x3f07d809, // Survivors (1988)(Atlantis Software)[tape].dsk
   0x4b96f419, // Star Trooper (1988)(Players)[tape].dsk
   0x35db55a2, // Speedzone (1988)(Mastertronic)[tape].dsk
   0x20fca483, // Space Duel, The (1990)(Andreas Stroiczek)(PD)(de)[cr].dsk
   0x1984568a, // Suicide Squad (1987)(Magazin Players Dream)(PD)[cr].dsk
   0x4e289ff4, // Space Mission (1986)(Cascade Games)[cr].dsk
   0x456874d4, // Spy Trek Adventure (1987)(Americana Software Ltd)[cr].dsk
   0x4ec6fcf8, // Skaal (1987)(Softhawk)(fr)(Side A).dsk
   0x13245ba1, // Skaal (1987)(Softhawk)(fr)(Side B).dsk
   0x1aa1cbe7, // Stockmarket (1985)(Amsoft).dsk
   0xc9f96b87, // Solar Empire (1990)(Players Premier)[tape].dsk
   0xf3e8bd31, // Subterranean Stryker (1985)(Amsoft).dsk
   0xfcf7fc5d, // Space Rider - Jet Pack Co. (1990)(Hi-Tec Software)[cr].dsk
   0x9364824b, // Super Wonderboy in Monster Land (1989)(Activision)(Side A).dsk
   0xc3d05794, // Super Wonderboy in Monster Land (1989)(Activision)(Side B).dsk
   0xd05a21f3, // Stomp (1984)(DK' Tronics)[cr].dsk
   0x284a343c, // Score 3020 (1988)(Topo Soft)(es).dsk
   0x953bf28b, // SOS 100% (1988)(Amstrad Cent Pour Cent)(fr)[cr].dsk
   0x80b4d1de, // Strip Poker Animado (1987)(Knight Soft)(es)(Hack)[cr].dsk
   0x4971cc63, // Saga of Erik the Viking, The (1984)(Level 9 Computing)[tape].dsk
   0x0ba1e0de, // Spherical (1989)(Rainbow Arts)(Side A).dsk
   0x196f47f0, // Spherical (1989)(Rainbow Arts)(Side B).dsk
   0x7830a74e, // Sai Combat (1986)(Mirrorsoft).dsk
   0x69927831, // Secreto De La Tumba, El (1987)(Loriciels)(es)[cr][f].dsk
   0xaf81d0ad, // Seas of Blood (1985)(Adventure International)(v2)[cr].dsk
   0x2792ba58, // Submarino Experimental Neptuno (19xx)(Ediciones Manali)(es)[cr][d7].dsk
   0x53c6e5d0, // Super Cauldron (1992)(Titus)(M3).dsk
   0x49b24afd, // Scramble (1988)(Computing With The Amstrad)[cr].dsk
   0x376c862a, // Space Racer (1988)(Loriciels).dsk
   0xcb87873b, // Starfox (1987)(Ariolasoft).dsk
   0xd4586894, // Sharkey's Moll (1991)(Zeppelin Games)[cr].dsk
   0x7434b021, // Star Wars - Return Of The Jedi (1989)(Domark).dsk
   0xfdf9ebc0, // Shadows Of Sergoth, The (2018)(PD)(M3)(v1.0)(Side B).dsk
   0x6d63f48a, // Shadows Of Sergoth, The (2018)(PD)(M3)(v1.0)(Side A).dsk
   0x6085a8be, // Solar Coaster (1987)(Optyx Software)[tape].dsk
   0x79b8bbcd, // Secret of Bastow Manor, The (1984)(Gameworx Software).dsk
   0xc40d22cb, // S.T.U.N. Runner (1991)(Domark).dsk
   0xe4ca7266, // Stack 'n' Smile (2003)(Nicholas Campbell).dsk
   0xd474d246, // Side Arms (1988)(US Gold).dsk
   0xaa7f2a56, // SDI - Strategic Defence Initiative (1988)(Activision)[tape].dsk
   0x2df2d8c6, // Space Harrier II (1990)(Grandslam)(Side B).dsk
   0x00c8282b, // Space Harrier II (1990)(Grandslam)(Side A).dsk
   0xdea061fc, // Simplex 3D (1990)(Micro Mag)(fr)[cr].dsk
   0x44d6bf89, // Scooby Doo and Scrappy Doo (1991)(Hi-Tec Software)[cr][f].dsk
   0xbb974b81, // Scoop Junior (1990)(Generation 5)(fr)(Side B).dsk
   0x39d956c4, // Scoop Junior (1990)(Generation 5)(fr)(Side A).dsk
   0xaffce575, // Super Pac (1987)(PC Schneider International)[cr].dsk
   0x00653097, // Sideral War (1990)(Delta Software)(es).dsk
   0x9fc324f7, // Septieme Continent, Le (1987)(L. Dplanque)(fr)(Side B).dsk
   0x6e83fa1f, // Septieme Continent, Le (1987)(L. Dplanque)(fr)(Side A).dsk
   0xb984201b, // Smiley Affair, The (1993)(M. Dowse)(PD)[cr].dsk
   0x160ee8e9, // Seastalker (1984)(Infocom)[CPM].dsk
   0xe5d8e7e9, // Splat! (1985)(Amsoft).dsk
   0x11b73896, // Ship Pilot (1986)(System 4)(es).dsk
   0x5ac9f3f4, // Skate Crazy (1988)(Gremlin Graphics Software)(Side B).dsk
   0xef7871dc, // Skate Crazy (1988)(Gremlin Graphics Software)(Side A).dsk
   0x43a174ac, // Sigma 7 (1987)(Durell Software).dsk
   0x687a0668, // Scary Tales (1985)(The Power House)[tape].dsk
   0x20e80d12, // Shadow Dancer (1991)(US Gold).dsk
   0x482fe514, // Smugglers Cove (1985)(CRL Group)[cr][BASIC 1.0].dsk
   0x75b52a3b, // Silkworm (1988)(Virgin Games).dsk
   0xdde58c80, // Stormbringer (1987)(Mastertronic)[tape].dsk
   0xca1c7f49, // Stretcher 2 - Mr. Line's Revenge (1992)(Pipeline Software)[cr].dsk
   0x2e01e637, // Serpentino (1988)(AM-Mag)(fr)[cr].dsk
   0xf88a48bd, // Strip Poker II+ (1988)(Anco Software).dsk
   0x4e6e569e, // Samurai Trilogy (1987)(Gremlin Graphics Software)(M4)(Side B).dsk
   0xf773074b, // Samurai Trilogy (1987)(Gremlin Graphics Software)(M4)(Side A).dsk
   0xfaa10dd6, // Saboteur (1986)(Durell Software)[f].dsk
   0x29c80cbd, // Star Avenger (1984)(Kuma Computers).dsk
   0x37ee404f, // Shinobu (2019)(cpcretrodev.byterealms)[cr].dsk
   0x3726c2cf, // Super Stock Car (1990)(Mastertronic)[tape].dsk
   0x5f89b89d, // Syntax (1988)(CDS Software LTD)[cr].dsk
   0x04090355, // Strip Poker Anime, Le (1985)(Knight Soft)(fr)[cr].dsk
   0xbfe7ed34, // Solomon's Key 3 v1.0 (1994)(Acid Team)(PD)[cr].dsk
   0xae64ea1d, // Satan (1989)(Dinamic Software).dsk
   0x825e0177, // Sudoku (2008)(Cronosoft).dsk
   0x4cbe0619, // Scott Winder Reporter (1987)(ERE Informatique)(fr)(Side B).dsk
   0xb2c6c7cd, // Scott Winder Reporter (1987)(ERE Informatique)(fr)(Side A).dsk
   0x0e387f66, // Snowstrike (1991)(US Gold).dsk
   0x5dbf9970, // Striker In The Crypts Of Trogan (1992)(Codemasters Software)[cr][CPC+].dsk
   0x81ad6524, // Solitaire (1986)(CPC)(fr)[464].dsk
   0x9181fe3c, // Steve Davis Snooker (1985)(CDS Software LTD).dsk
   0x5c4b84cc, // Starcross (1982)(Infocom)[cr][CPM].dsk
   0x1115c8c8, // Super Stuntman (1988)(Codemasters Software)[cr].dsk
   0x2c1804c8, // Space Racer (1988)(Loriciels)(fr).dsk
   0x0417c5aa, // Slaine (1988)(Martech).dsk
   0xdaa9cc5e, // Scooby Doo (1986)(Elite Systems).dsk
   0xee17125a, // Stardust (1987)(Topo Soft)(es)[cr].dsk
   0xae01ef0e, // Swap (1992)(Fraggle)(PD)[cr].dsk
   0xc1e745e8, // Skyx (1988)(Legend Software)(fr)(Side B).dsk
   0x1733c833, // Skyx (1988)(Legend Software)(fr)(Side A).dsk
   0xaf6de481, // Santa's Christmas Capers (1991)(Zeppelin Games)[cr].dsk
   0xd8f8a7e9, // Survivor (1987)(Topo Soft).dsk
   0x75de79e6, // Survivor (1987)(Topo Soft)(es)[cr].dsk
   0x63e0e255, // Summer Games (1988)(Epyx)(Side B).dsk
   0x192a42fa, // Summer Games (1988)(Epyx)(Side A).dsk
   0xeeb6bb92, // Snoopy and Peanuts (1989)(The Edge).dsk
   0x21332735, // Serpent Madness (1986)(CPC)(fr)[cr].dsk
   0xed5a1c75, // Suspect (1986)(Infocom).dsk
   0xbcd370e0, // Strip Poker (1987)(Logipresse)(fr)[cr].dsk
   0x9a4bca1c, // Soko Bam (1992)(CPC Infos)(M2)[cr].dsk
   0xe1fcec53, // Shanghai (1988)(-)(PD)[cr].dsk
   0xd41826f1, // Stop Ball (1987)(Dro Soft)(es)[cr].dsk
   0xa4bc9aab, // Strangeloop (1985)(Virgin Games)[tape].dsk
   0x819d6ed7, // Suspended (1983)(Infocom)[CPM].dsk
   0xd519f3ce, // Spellbreaker (1985)(Infocom)[CPM].dsk
   0x7ca067e2, // Space Shuttle - A Journey into Space (1986)(Activision).dsk
   0xd7e09b3d, // Scramble (2019)(PuzCPC)(PD)[cr].dsk
   0xfe3eb13a, // Space Thief (1990)(Micro Mag)[cr].dsk
   0xcccc6eea, // Special Operations (1984)(MC Lothlorien)[tape].dsk
   0x72d0dc7d, // Showdown In Tombstone City (1985)(Magazin Players Dream)(PD)[cr].dsk
   0x36596d2c, // Sabre Wulf (1984)(Ultimate Play The Game)[tape].dsk
   0x8c8a37bd, // Sgrizam (1986)(Dinamic Software)(es)[cr].dsk
   0x8090224b, // Super Trux (1988)(Elite Systems).dsk
   0x3a2ea285, // Soccer Challenge (1990)(Alternative Software)[tape].dsk
   0x365aba64, // Space Attack (1986)(Cascade Games)[cr].dsk
   0xe271c145, // Sceptre d'Anubis, Le (1985)(Micro Programmes 5)(fr)[cr].dsk
   0x829a71ce, // Star Wars (1987)(Domark).dsk
   0xb8b2a919, // Super Soccer (1987)(Ocean Software)[cr].dsk
   0x45c69639, // Super Soccer (1987)(Ocean Software)(fr)[cr].dsk
   0xf391fade, // Swap (1990)(Microids)(M2)(v2).dsk
   0xc18a84f9, // Smash TV (1991)(Ocean Software).dsk
   0x6a653770, // Super Cars (1990)(Gremlin Graphics Software).dsk
   0xca6ffd7a, // Savage (1988)(Firebird)(Side A).dsk
   0x9300cefc, // Savage (1988)(Firebird)(Side B).dsk
   0x6cfb64e0, // Spirits (1987)(Topo Soft)(es).dsk
   0x3a970d1c, // Shufflepuck Cafe (1989)(Broderbund Software)(fr).dsk
   0xa454e2b3, // Space Mazes (1991)(Amstrad Cent Pour Cent)[cr].dsk
   0xe99c43c4, // Star Fight (1985)(Hebdogiciel)(fr)[cr].dsk
   0x36f23396, // Search for Largo Grann, The (1987)(Amstrad Adventure PD Library)[cr].dsk
   0x22459515, // Saint Valentin (1990)(Sebastien Leloutre)(fr)[cr].dsk
   0xfab74dca, // Space Invaders Arcade Emulator (2017)(-)(PD)(v1.02)[cr].dsk
   0x881245c7, // Space (1990)(Fanatic)(fr)[cr].dsk
   0xbaab941c, // Super Cycle (1987)(Epyx)[D3M].dsk
   0x83bce66c, // Storm (1986)(Mastertronic)[cr].dsk
   0xa02fd402, // Sootland (1988)(Zafiro Software Division)(es).dsk
   0x761d9413, // Sootland (1988)(Zafiro Software Division)(es)(LightGun).dsk
   0x43157458, // Sultans Maze (1984)(Amsoft)[cr][f][BASIC 1.1].dsk
   0xc4769db6, // Scrounch (1985)(Hebdogiciel)(fr)[cr].dsk
   0x2314f143, // Stifflip & Co (1987)(Palace Software)(de)(Side A).dsk
   0x93c69fcd, // Stifflip & Co (1987)(Palace Software)(de)(Side B).dsk
   0xdd3b4296, // Stifflip & Co (1987)(Palace Software)(es)[cr].dsk
   0x595d8f94, // Stifflip & Co (1987)(Palace Software)[cr].dsk
   0xce0180d5, // Stifflip & Co (1987)(Palace Software)(fr)(Side A).dsk
   0xc578d019, // Stifflip & Co (1987)(Palace Software)(fr)(Side B).dsk
   0xca68e112, // Space Invaders (19xx)(Auntie John)(PD)[cr].dsk
   0x8ea6b3fd, // Super Robin Hood (1986)(Codemasters Software)[cr].dsk
   0x96c5402c, // Southern Belle (1985)(Hewson)[cr].dsk
   0xd0937642, // Sly Spy - Secret Agent (1990)(Ocean Software)(Side B).dsk
   0x62641a3b, // Sly Spy - Secret Agent (1990)(Ocean Software)(Side A).dsk
   0x7787b485, // Strike Force S.A.S. (1987)(Mikrogen)[cr].dsk
   0x443c7682, // SOS Ovoides (1986)(Andromeda Software)[cr].dsk
   0x2382e5ec, // Super Puzzle (1987)(Logipresse)(fr)[cr].dsk
   0xa68f4d96, // Super Seymour Saves the Planet (1991)(Codemasters Software)[cr].dsk
   0xf03ad98a, // Shao Lin's Road (1987)(The Edge).dsk
   0x96426e54, // Strip Poker (1985)(CORE)(fr).dsk
   0x7af9e63d, // Streaker (1987)(Mastertronic)[tape].dsk
   0x238d8926, // Super Pac (1985)(Loisitech)(fr).dsk
   0x4ba81057, // Stock Exchange (1987)(Top Ten)[cr].dsk
   0xa4d354a5, // Sun Star (1987)(CRL Group).dsk
   0x0997de36, // Synchronous (1987)(Bretagne Edit Press)(fr)[cr].dsk
   0x92345cde, // Sir Fred (1986)(Zigurat)(es).dsk
   0x1473cc7e, // Super Sapiens (1989)(Proein Soft Line)(es).dsk
   0x863ee312, // Spy vs Spy II - The Island Caper (1987)(Databyte).dsk
   0x1482816b, // SMASHED (1987)(Alternative Software)[tape].dsk
   0x5a1a3f9b, // Starglider (1987)(Rainbird Software).dsk
   0xd404b5df, // Super Sprint (1987)(Activision)(v3).dsk
   0x0a1d8e00, // Spindrone (1988)(Atlantis Software)[tape].dsk
   0xf122e580, // Streap-Poker (1987)(Denis Mezenge)(PD)(fr)[cr].dsk
   0x860b5808, // Sabrina (1989)(Iber Soft)(es)[cr][f].dsk
   0x0813f0c1, // Spiky Harold (1986)(Firebird)[cr].dsk
   0xcf0d5cff, // Spy Hunter (1986)(Amsoft)[cr][f].dsk
   0xde543883, // Sorcery+ (1985)(Amsoft).dsk
   0x7fa49d6e, // Star Bowls (1991)(Zigurat Software)(es).dsk
   0xa2c253de, // Sorcery (1985)(Virgin Games).dsk
   0x196a3c08, // Starboy (1986)(Gasoline Software)(fr)[cr].dsk
   0x01257f77, // Super Sports - The Olympic Challenge (1988)(Gremlin Graphics Software)(Side A).dsk
   0x1bfc098e, // Super Sports - The Olympic Challenge (1988)(Gremlin Graphics Software)(Side B).dsk
   0xfbad0848, // Samurai (1990)(Amstar & Cpc)[cr].dsk
   0x5a5b588a, // Strider (1989)(US Gold).dsk
   0xa1b54138, // Shoot (1990)(CPC)[cr].dsk
   0x61de3bf1, // Short Circuit (1987)(Ocean Software).dsk
   0x6ae1bd8b, // Superman - The Man of Steel (1989)(Tynesoft)(Side B).dsk
   0xf0a67f40, // Superman - The Man of Steel (1989)(Tynesoft)(Side A).dsk
   0x30f6e0fb, // Superkid (1990)(Atlantis Software)[cr].dsk
   0x8a436a1b, // Strangeloop+ (1986)(Virgin Games).dsk
   0xa5d86a23, // Space Pod Rescue (1986)(Cascade Games)[cr].dsk
   0x5873228b, // Space Taxi (1987)(Elmsoft Game Service)(PD)[cr].dsk
   0x9d746e4e, // Spindizzy (1986)(Activision).dsk
   0x7b81d352, // Stir Crazy Featuring Bobo (1988)(Infogrames)(Side B).dsk
   0x03d6720c, // Stir Crazy Featuring Bobo (1988)(Infogrames)(Side A).dsk
   0xfac5bdac, // Starquake (1986)(Bubble Bus Software).dsk
   0x135f1fc7, // SWIV (1991)(Storm Software).dsk
   0x2117bd08, // Switchblade (1991)(Gremlin Graphics Software).dsk
   0x0bd6c0af, // Shinobi - Remaster (1989)(Fmtrx)(v1.02)(Hack)[cr][f].dsk
   0x42f1778f, // Spots 94 (1994)(Jaysoft)[cr].dsk
   0xda1d3e90, // Star Wars Droids (1988)(Mastertronic)[tape].dsk
   0xbc8e8f76, // Slapshot (1985)(Indescomp).dsk
   0xcfc0a807, // Super Space Invaders (1991)(Domark)(Side B).dsk
   0xf3163ff3, // Super Space Invaders (1991)(Domark)(Side A).dsk
   0x6e1f628e, // Star-Wreck (1987)(Alternative Software)[tape].dsk
   0x776cd65e, // Shovel Adventure (2021)(Pat Morita Team)(JGN Edition).DSK
   0x0ab6f33f, // Spaced Out (1987)(Firebird)[tape].dsk
   0x4ec69c25, // Super Monaco GP (1991)(US Gold).dsk
   0x248f2c16, // Solo (1989)(Opera Soft)(es)(LightGun).dsk
   0xd8b1d3e4, // Sabotage (1988)(Zeppelin Games)[tape].dsk
   0x1b9371d2, // Saint and Greavsie (1989)(Grandslam).dsk
   0x75cf0cc7, // Steve Mc Queen Westphaser (1992)(Loriciels)(West Phaser)(Side B).dsk
   0x133e8046, // Steve Mc Queen Westphaser (1992)(Loriciels)(West Phaser)(Side A).dsk
   0xb0c72be6, // Shadowfire (1986)(Beyond Software).dsk
   0x92842960, // Soviet (1990)(Opera Soft).dsk
   0x6e328762, // Spheric (1989)(Amstrad Cent Pour Cent)(fr)[cr].dsk
   0x510b781b, // Sandman, The (1995)(TGS)(PD)(de)(Disk 1 Side B).dsk
   0x03964fe9, // Sandman, The (1995)(TGS)(PD)(de)(Disk 2 Side A).dsk
   0xab09170e, // Sandman, The (1995)(TGS)(PD)(de)(Disk 2 Side B).dsk
   0xb49383c1, // Sandman, The (1995)(TGS)(PD)(Disk 1 Side A).dsk
   0x167d042d, // Sandman, The (1995)(TGS)(PD)(Disk 2 Side B).dsk
   0x2545f08e, // Sandman, The (1995)(TGS)(PD)(Disk 2 Side A).dsk
   0xe2b5b1f5, // Sandman, The (1995)(TGS)(PD)(de)(Disk 1 Side A).dsk
   0x8189874c, // Sandman, The (1995)(TGS)(PD)(Disk 1 Side B).dsk
   0x5a32bd0d, // Strider II (1990)(US Gold).dsk
   0x9f9f22e3, // Sharpes Deeds (1987)(Incentive)(Hack)[cr].dsk
   0xd867cd03, // Stormlord (1989)(Hewson).dsk
   0xef32e345, // Serie Noire (1985)(Micro Application)(fr)[cr].dsk
   0x5d275e95, // Silva (1985)(Lankhor)(fr)(Side B).dsk
   0x6307eb75, // Silva (1985)(Lankhor)(fr)(Side A).dsk
   0x635dbb5b, // Street Hawk (1986)(Ocean Software).dsk
   0x3391e8a8, // Syndrome, Le (1986)(Alligata Software)(fr).dsk
   0xb4ca9087, // Shadow Skimmer (1987)(The Edge).dsk
   0x802695e9, // Superpix (2014)(Crack'n'Rom)(Side A)[f].dsk
   0x16bbc3da, // Superpix (2014)(Crack'n'Rom)(Side B)[f].dsk
   0xd66de504, // Satan (1989)(Dinamic Software)(es).dsk
   0x2a959d64, // Secret du Tombeau, Le (1986)(Loriciels)(fr).dsk
   0x26e35fea, // Scarabaeus (1985)(Ariolasoft)(es)[cr].dsk
   0xd6968fb8, // Star Raiders II (1987)(Activision).dsk
   0x39fdcbd1, // Smirking Horror, The (1991)(WoW Software)[cr].dsk
   0x4cd46327, // Soccer Director (1990)(GTI Software)[tape].dsk
   0x636a30d8, // Sorcerer (1984)(Infocom)[CPM].dsk
   0x375d4353, // Stormlord II - Deliverance (1990)(Hewson).dsk
   0x1bab7a1c, // Skyfox (1985)(Electronics Arts)(es)[cr].dsk
   0x61cc737a, // Super Hang-On (1986)(Activision).dsk
   0xd39ebb1d, // Shackled (1988)(US Gold)[b].dsk
   0x7990d859, // Skate Wars (1990)(Ubi Soft).dsk
   0x74c0c019, // Spannerman (1984)(Amsoft).dsk
   0x06225f9f, // Super Hero (1988)(Codemasters Software)[cr][f].dsk
   0x6af22741, // Shadow of the Beast (1990)(Gremlin Graphics Software)(Side B).dsk
   0xfff6b37b, // Shadow of the Beast (1990)(Gremlin Graphics Software)(Side A).dsk
   0x65986be4, // Spellbound (1985)(Mastertronic)[tape].dsk
   0xa6a27132, // Subsunk Adventure (1985)(Firebird)[tape].dsk
   0xc275e43a, // Sonic Boom (1990)(Activision).dsk
   0xa06446cf, // Sentinel, The (1987)(Firebird)(fr).dsk
   0x72929b47, // Sentinel, The (1987)(Firebird)(v2).dsk
   0xbe401909, // Strike Force Cobra (1986)(Piranha).dsk
   0x7adad7a0, // Simon (1986)(Amstrad Action)[cr].dsk
   0xc411455f, // Secret Defense (1988)(Loriciels)(fr).dsk
   0x2fded49c, // Sacred Armour of Antiriad, The (1986)(Palace Software).dsk
   0x2cc66470, // Survivre (1991)(Lankhor)(fr)(Side A).dsk
   0xce70d508, // Survivre (1991)(Lankhor)(fr)(Side B).dsk
   0x29671a87, // Solomon's Key (1987)(US Gold).dsk
   0xce1c4766, // Street Gang Football (1989)(Codemasters Software)[tape].dsk
   0x74901f54, // Space Invaders (19xx)(-)(PD)[cr].dsk
   0x37549059, // Seabase Delta (1985)(Firebird)[cr].dsk
   0x300c27db, // Salva las Letras (19xx)(-)(PD)(es)[cr].dsk
   0x2674fdf1, // Stairway to Hell (1986)(Software invasion).dsk
   0xa266c7d9, // Sphaira (1989)(Ubi Soft)(fr)(Side A).dsk
   0x3a697a24, // Sphaira (1989)(Ubi Soft)(fr)(Side B).dsk
   0xbdfbd2d0, // Simon (1990)(Amstrad Action)[cr].dsk
   0x49586e15, // Space Moves (2020)(Retrobytes Productions)(v2.2).dsk
   0x94159db0, // Stretcher (1992)(Pipeline Software)[cr].dsk
   0x1c2787f3, // Sim City (1989)(Infogrames)(Side A).dsk
   0x0b11f44c, // Sim City (1989)(Infogrames)(Side B)[Cities].dsk
   0x9a407cf0, // Space Mania (1986)(Amstrad Computer User)[cr].dsk
   0x6f1e3eb0, // Shinobi (1989)(Virgin Games).dsk
   0x24bbc94b, // SAS Combat Simulator (1988)(Codemasters Software)[b].dsk
   0xd3267a79, // Snooker (1984)(Amsoft).dsk
   0xe0c76ce1, // Saracen (1987)(US Gold).dsk
   0x27489824, // Scout Steps Out, The (1985)(Amsoft).dsk
   0x9469fe98, // Sherman M4 (1990)(Loriciels).dsk
   0x086e337d, // Sherman M4 (1990)(Loriciels)(fr)(v2).dsk
   0xcd5ede5a, // Shadow of the Bear, The (1985)(Kuma Computers)[cr].dsk
   0xc65a5218, // Seymour - Take One (1991)(Codemasters Software)(PD)[cr].dsk
   0x18e04d4c, // Space Moves (2015)(cpcretrodev.byterealms)(es)[tape].dsk
   0x625b38d8, // Star Ranger (1986)(Tynesoft)[cr].dsk
   0x602b16e5, // Skatin' USA (1990)(Atlantis Software).dsk
   0xa9f5fbd4, // Survivor, The (1984)(Anirog Software)[cr].dsk
   0x1149f9d0, // Street Machine (1986)(Software Invasion)(es).dsk
   0x85d18e60, // Sooty and Sweep (1990)(Alternative Software).dsk
   0xb715105e, // Subsunk (1988)(CPC Schneider International)(fr)[cr].dsk
   0x06edc711, // Subsunk (1988)(CPC Schneider International)(de)[cr].dsk
   0xb7b1a995, // Strike Force Harrier (1986)(Mirrorsoft)(v2).dsk
   0x245093fe, // Super Scramble Simulator (1989)(Gremlin Graphics Software).dsk
   0x627f4acf, // Simpsons - Bart vs the Space Mutants (1991)(Ocean Software)(v2)(Side B).dsk
   0xcc5f46b2, // Simpsons - Bart vs the Space Mutants (1991)(Ocean Software)(v2)(Side A).dsk
   0x0630c1a6, // Snakes and Hazards (1987)(Gremlin Graphics Software)[cr].dsk
   0x0e430e36, // Space Ace (1987)(Players)[tape].dsk
   0x268bba28, // Sapiens (1986)(Loriciels).dsk
   0x1e15c4c1, // Sapiens (1986)(Loriciels)(fr)(v2).dsk
   0x396d27ed, // Star Firebirds (1986)(Insight)[cr].dsk
   0x8385f78f, // Spy vs Spy (1985)(Beyond Software).dsk
   0x641dcd51, // Sito Pons 500cc Grand Prix (1990)(Zigurat)(es).dsk
   0xfdaf7cd2, // Space Crusade (1992)(Gremlin Graphics Software).dsk
   0x0d84c1bc, // Space Crusade (1992)(Gremlin Graphics Software)(fr).dsk
   0x4c5cd626, // Space Crusade (1992)(Gremlin Graphics Software)(es).dsk
   0xfefca5e7, // Sepulcri (1986)(Ariolasoft).dsk
   0x62a6e45d, // Snodgits (1985)(Thorn Emi Computer Software)[tape].dsk
   0xa041e903, // Space Hawks - Aguilas Del Espacio (1984)(Amsoft).dsk
   0xff92c5e5, // Shard of Inovar (1987)(Mastertronic)[tape].dsk
   0x5665d718, // Tuareg (1988)(Topo Soft)(es).dsk
   0xf6e7a23c, // Titanic (1988)(Topo Soft)[cr][f].dsk
   0xe2f4e53c, // Tom & Jerry (1989)(Magic Bytes).dsk
   0x6f8b6c61, // Titan (1988)(Titus)(fr).dsk
   0x05d7a217, // Thunder Zone (1987)(Firebird)[cr].dsk
   0x932fa54a, // Troglo (1986)(Ace Software)(es).dsk
   0xf4f79188, // Tai-Pan (1987)(Ocean Software)[f].dsk
   0x4e2776a9, // Tiger Road (1988)(US Gold).dsk
   0x4896b537, // Tusker (1989)(System 3)[f].dsk
   0x09a6c7c0, // Tales of the Arabian Nights (1985)(Interceptor Software)[cr].dsk
   0xec265b17, // Tennis Cup (1989)(Loriciels).dsk
   0x48fda440, // Tennis Cup (1989)(Loriciels)(fr).dsk
   0xa0a42352, // Thai Boxing (1986)(Anco Software).dsk
   0xcd0a0395, // Trailblazer (1986)(Gremlin Graphics Software).dsk
   0x594266d7, // Tarzan (1986)(Martech)(v2).dsk
   0x78acf281, // Trivia (1985)(Anirog Software)(fr).dsk
   0x71961d07, // Trivia (1985)(Anirog Software)[cr].dsk
   0xa7915c1c, // Trivia (1985)(Anirog Software)(de).dsk
   0x2f4e9778, // Throne of Fire (1987)(Melbourne House)[cr].dsk
   0x6a99b71d, // Trollie Wallie (1986)(Players)[cr].dsk
   0x733852a6, // Titanic (1988)(Topo Soft)(es)[cr].dsk
   0xdcce9d75, // Top Cat in Beverly Hills Cats (1990)(Hi-Tec Software)[cr].dsk
   0xb77647e6, // Thrust (1986)(Firebird)(v7-86)[cr].dsk
   0x55ebca5d, // Tau Ceti - Special Edition (1986)(CRL Group).dsk
   0x78935458, // Tresor d'Ali Gator (1991)(Lankhor)(fr)[o].dsk
   0x83705d03, // Tokyo Gang (1990)(GLL Software)(es).dsk
   0x725fb725, // Turris (1991)(CPC Amstrad International)(de)[cr].dsk
   0xcbaca92a, // Tesoro Perdido De Cuauhtemoc (2017)(4Mhz)(es)(v2).dsk
   0xcccead08, // Tiefe, Die (1985)(Sun Soft)(de)[cr].dsk
   0x37789b0e, // Tracer (1988)(Schneider Magazin)[cr].dsk
   0xe4154a1f, // Target Renegade (1988)(Ocean Software).dsk
   0x0ff7ddb7, // Tolki (1992)(CPC Infos)(fr)[cr].dsk
   0x29ed4286, // Test Drive II - The Duel (1989)(Accolade).dsk
   0x30e38ee5, // Tetris (1987)(Mirrorsoft).dsk
   0x2472f61f, // Tron (1989)(Bollaware)(PD)(de)[cr].dsk
   0xef69722f, // Tetrix (1989)(Playfield)(PD)[cr].dsk
   0xf938d725, // Terminator 2 - Judgement Day (1991)(Ocean Software)(v2).dsk
   0xad17f71f, // Turbo Cup (1988)(Loriciels)(fr).dsk
   0x77e00286, // Tobruk 1942 (1986)(PSS)(fr).dsk
   0x83497f3c, // Tank (1987)(Ocean Software).dsk
   0x1b78e4f9, // Tie Break (1985)(Sprites)(fr)[cr].dsk
   0x419f8f00, // Talisman of Power, The (1990)(Chris Bury)[cr].dsk
   0x79a1ab46, // Tac Tic (19xx)(PD)(fr)[cr].dsk
   0x6f968bd0, // Trance (1989)(M. Remon)[cr].dsk
   0xc9d35be1, // Terrorpods (1989)(Melbourne House)[cr][b].dsk
   0x17e032f3, // Turbo Chopper (1989)(Codemasters Software)[cr].dsk
   0x3854d175, // Trap (1987)(Alligata Software).dsk
   0x03b14d37, // Thing on a Spring (1986)(Gremlin Graphics Software).dsk
   0xbf3bb944, // Twins (1991)(Amstrad Cent Pour Cent)[cr].dsk
   0x01f645af, // Tour du Mond en 80 Jours, Le (1984)(Innelec)(fr)[cr].dsk
   0x5d6a048b, // Thunder Jaws (1991)(Domark)(Side A).dsk
   0x34785299, // Thunder Jaws (1991)(Domark)(Side B).dsk
   0x5c0b2351, // Tour 91 (1991)(Topo Soft)(es).dsk
   0x38b0cf7f, // Track and Field (1988)(Ocean Software)[b].dsk
   0x41f8051c, // Total Eclipse (1988)(Incentive).dsk
   0x96bc4c3e, // Twin Turbo V8 (1989)(Codemasters Software)[cr].dsk
   0x7ffbf9fb, // Target Ball (1990)(Micro Mag)[cr].dsk
   0x38dde7c9, // Time and Magik Trilogy (1988)(Level 9 Computing)(Side A).dsk
   0x07a969c1, // Time and Magik Trilogy (1988)(Level 9 Computing)(Side B).dsk
   0xb1062183, // Tizpan (1990)(The Adventure Workshop)[cr].dsk
   0x3365f862, // Trantor - The Last Stormtrooper (1987)(US Gold).dsk
   0x17f2e47a, // Terra Cognita (1986)(Codemasters Software)[cr].dsk
   0x000ef371, // Timelord (1985)(Alpha Omega Software)[cr].dsk
   0x298e5492, // Train, The - Escape to Normandy (1988)(Electronic Arts)(es)[cr].dsk
   0xbe71d9f4, // Train, The - Escape to Normandy (1988)(Electronic Arts).dsk
   0x09fa7625, // Terror en la Facultad (1987)(Edisoft)(es)[cr].dsk
   0x785f93dc, // Thing Bounces Back (1987)(Gremlin Graphics Software).dsk
   0xa015003f, // Twice Shy (1986)(Mosaic Publishing)[cr].dsk
   0x6bfe99c9, // Tank Command (1986)(Amsoft).dsk
   0x3c894e45, // Track Suit Manager 1.1 (1988)(Goliath Games)[cr].dsk
   0xa0f88ab9, // Trans Muter (1987)(Codemasters Software)[cr].dsk
   0x6c04428c, // Tribble Trouble (1985)(Amsoft)[cr].dsk
   0x909d60ac, // Tiddly Drinks (1986)(Gremlin Graphics Software)[cr].dsk
   0x6a652828, // Times of Lore (1988)(Origin Systems)(Side B).dsk
   0x361b6f38, // Times of Lore (1988)(Origin Systems)(Side A).dsk
   0x89f5ee30, // Tintin on the Moon (1989)(Infogrames)(M3).dsk
   0x37b5e3e9, // Think! (1985)(Ariolasoft)[cr].dsk
   0x9a62e291, // Theseus (1986)(50-50 Club Software)[cr].dsk
   0x66bb92e8, // Troll (1988)(Palace Software)[cr].dsk
   0x1d921cd9, // Touchdown USA (1987)(Creative Sparks)[cr].dsk
   0xb3b42030, // Tomb of Kuslak, The (2020)(Intrigue Software)(es)(Hack)[cr].dsk
   0x61940fb0, // Tomb of Kuslak, The (1985)(Intrigue Software)[cr].dsk
   0xafbe272a, // Tank Invader (1987)(Logipresse)(fr)[cr].dsk
   0x53a9230a, // Tetris (1990)(Poesy)(PD)[cr].dsk
   0x599ba9a4, // Tom & Jerry II (1989)(Magic Bytes).dsk
   0xd5160fc1, // Tau Ceti (1986)(CRL Group).dsk
   0x1a21baf5, // Time Out (1988)(Zafiro Software)(es).dsk
   0x39056da7, // Titan (1989)(Titus).dsk
   0xd0dc75a0, // Toi Acid Game (1990)(Iber Soft)(es)(Side A).dsk
   0x5e5eac17, // Toi Acid Game (1990)(Iber Soft)(es)(Side B).dsk
   0x8e84af41, // Trakers (1991)(Kennedy)[cr].dsk
   0x827e83ac, // Thanatos (1986)(Durell Software).dsk
   0xd13daabc, // Thunder Fighter (1988)(E.S.A.T. Software)(fr)[cr].dsk
   0x3ee5fb4a, // Time Machine (1990)(Activision).dsk
   0xa9f27d19, // Transat One (1985)(Innelec)(fr)[cr].dsk
   0x6ebece8b, // Teenage Mutant Hero Turtles 2 - Coin Op (1991)(Image Works)(Side B)[codes].dsk
   0x1524c73a, // Teenage Mutant Hero Turtles 2 - Coin Op (1991)(Image Works)(Side A)[codes].dsk
   0xcf1ef978, // Test Match (1984)(CRL Group)[cr].dsk
   0x62a0595f, // Triple Comando (1988)(Dro Soft)(es)[b].dsk
   0x30fec756, // Triple Comando (1988)(Dro Soft)(es)[tape][f].dsk
   0xfa3963aa, // Tchernmisland (1987)(CPC)(fr)[cr].dsk
   0x36831ef5, // Tetris 95 (1995)(Power System)(PD)[cr].dsk
   0xfd22747c, // Toad Runner (1986)(Ariolasoft).dsk
   0x0a73146f, // Thing (1988)(Players)[cr].dsk
   0x97e0e484, // Top Secret (1986)(Loriciels)(fr)(Side A).dsk
   0x6a1287c3, // Top Secret (1986)(Loriciels)(fr)(Side B).dsk
   0x7a34c973, // Tennis Manager (19xx)(Martin Huemmerich)(PD)(de)[cr].dsk
   0x80f85f80, // Turbo-Pacman (1987)(PD)(de)[cr].dsk
   0x34bc560d, // Tomahawk (1986)(Digital Integration)(M3)[codes].dsk
   0xfa94714b, // Tomahawk (1986)(Digital Integration)(M3)[cr].dsk
   0x94aa6cf3, // Trigger (1989)(Opera Soft)(es)(LightGun).dsk
   0x3078c995, // Tulgey Woods (1989)(Amstrad Adventure PD Library)(PD)[cr].dsk
   0xbf095b3c, // Traveller, The (19xx)(Recreation Re-Creation Software)[cr].dsk
   0x680c59c5, // Terrormolinos (1985)(Melbourne House)[cr].dsk
   0xdc5aaa0e, // Tank Busters (1985)(Design Design)[cr].dsk
   0xf4a9b201, // Turf-Form (1988)(CDS Software LTD)[cr].dsk
   0xfd49f2a2, // Trader Joe (19xx)(Ozzard)(PD)[cr].dsk
   0xfa823501, // Turlogh le Rodeur (1988)(Cobra Soft)(fr)(Side A).dsk
   0xe2b7909e, // Turlogh le Rodeur (1988)(Cobra Soft)(fr)(Side B).dsk
   0xcbdafd77, // Techno Cop (1988)(Gremlin Graphics Software).dsk
   0x1e737e6c, // Turbo Girl (1988)(Dinamic Software)[cr][f].dsk
   0x60a4a40f, // Three Weeks in Paradise (1985)(Mikrogen)[cr].dsk
   0x5eeb0371, // Through the Trap Door (1987)(Piranha).dsk
   0x1001a2c3, // Trik Trak (1987)(Wave Software)(PD)[cr].dsk
   0x9944aaf3, // Teenage Queen (1988)(ERE Informatique)(Side B).dsk
   0xa9be8cb2, // Teenage Queen (1988)(ERE Informatique)(Side A).dsk
   0xa2fcfa42, // Trans-Atlantic Balloon Challenge, The (1987)(Virgin Games).dsk
   0x857f0fc7, // Tuma 7 (1990)(Delta Software).dsk
   0xc465f4c5, // Toobin' (1989)(Domark).dsk
   0x484614fb, // Terminus (1986)(Mastertronic)[cr].dsk
   0xdd1eaf87, // Total Recall (1991)(Ocean Software).dsk
   0x87a9f7e8, // Tubaruba (1987)(Advance Software Promotions).dsk
   0x90779315, // Traffic (1985)(Amsoft).dsk
   0x02cce605, // Tomy en el Slalom (1987)(G.T.S. Editorial)(es)[cr].dsk
   0x3041c53e, // Table Football (1986)(Alligata Software)[cr].dsk
   0x53756a82, // TT Racer (1986)(Digital Integration)[cr].dsk
   0xe5f9a3a8, // TT Racer (1986)(Digital Integration)[codes].dsk
   0x3269939a, // Traitement Bzzz (1987)(Chip)(fr).dsk
   0xb699edb3, // Tapper (1986)(Amsoft)[cr].dsk
   0xc8dfac8f, // Tennis 3D (1986)(Loriciels)(fr).dsk
   0xf62b8fb8, // Time Scanner (1989)(Activision).dsk
   0xd177b94b, // Tempest (1986)(Activision).dsk
   0xc373f7ff, // Talisman d'Osiris, Le (1987)(Chip)(fr).dsk
   0xa08fdd10, // Turrican (1990)(Rainbow Arts)(Retail)(Side B).dsk
   0xa43e38d6, // Turrican (1990)(Rainbow Arts)(Side A).dsk
   0x89d0562f, // Turrican (1990)(Rainbow Arts)(Retail)(Side A)[f].dsk
   0x44e9ffdf, // Turrican (1990)(Rainbow Arts)(Side B).dsk
   0xaf2aac9e, // Top Gun (1986)(Ocean Software).dsk
   0x69ffaa9a, // Tornado Low Level - TLL (1985)(Vortex Software).dsk
   0x2c084343, // Turrican II - The Final Fight (1991)(Rainbow Arts)(Side B).dsk
   0xbd4ab417, // Turrican II - The Final Fight (1991)(Rainbow Arts)(Side A).dsk
   0x8d62e680, // Tony Truand (1985)(Loriciels)(fr).dsk
   0x3a5242b9, // Tres Luces de Glaurung, Las (1986)(Erbe Software)(es).dsk
   0x7ba6e032, // Tenebres (1987)(Rainbow Productions)(fr)(Side B)[cr].dsk
   0x81d57297, // Tenebres (1987)(Rainbow Productions)(fr)(Side A)[cr].dsk
   0xbeb35fad, // Tiny Skweeks, The (1992)(Loriciels)(fr).dsk
   0x4c50e144, // Tiny Skweeks, The (1992)(Loriciels).dsk
   0x5d3513c1, // Trouble in Space (1988)(Schneider Computer Partner)[cr].dsk
   0x8a357ad3, // Turbo Esprit (1986)(Durell Software)(es)[cr].dsk
   0x37c07361, // Turbo Esprit (1986)(Durell Software).dsk
   0xbfad3be9, // Terramex (1988)(Grandslam).dsk
   0xf0ec6a9d, // Turbo Out Run (1990)(US Gold)(Side A).dsk
   0x673571dc, // Turbo Out Run (1990)(US Gold)(Side B).dsk
   0x87ba6d6d, // Trivial Pursuit - Edition Genius (1986)(Domark)(fr)(Side A).dsk
   0x7638271c, // Trivial Pursuit - Edition Genius (1986)(Domark)(es)(Side A).dsk
   0xd1992306, // Trivial Pursuit - Edition Genius (1986)(Domark)(de)(Side A).dsk
   0xc6bc1409, // Trivial Pursuit - Edition Genius (1986)(Domark)(de)(Side B).dsk
   0xbdd66bf3, // Trivial Pursuit - Edition Genius (1986)(Domark)(v2)(Side A).dsk
   0xa55bb48d, // Trivial Pursuit - Edition Genius (1986)(Domark)(es)(Side B).dsk
   0x850f88d1, // Trivial Pursuit - Edition Genius (1986)(Domark)(fr)(Side B).dsk
   0x2ae181e1, // Trivial Pursuit - Edition Genius (1986)(Domark)(v2)(Side B).dsk
   0x43011123, // Tetris (1991)(Amstrad Action)[cr].dsk
   0x0f0b6fb0, // Temple du Soleil, Le (1986)(CPC)(fr)[cr].dsk
   0xaadd9ae3, // Times of Lore (1988)(Origin Systems)(fr)(v2)(Disk 2 Side A).dsk
   0x8878f148, // Times of Lore (1988)(Origin Systems)(fr)(v2)(Disk 1 Side A).dsk
   0x57d5919b, // Times of Lore (1988)(Origin Systems)(fr)(v2)(Disk 1 Side B).dsk
   0x85fb8bba, // Target Plus (1988)(Dinamic Software)(es)(LightGun).dsk
   0x8dd0cf27, // Teenage Mutant Hero Turtles (1990)(Image Works)[cr].dsk
   0x0ca4f0b5, // Teenage Mutant Hero Turtles (1990)(Image Works)[codes].dsk
   0x670c303d, // Tripods, The (1984)(Red Shift)[cr][f].dsk
   0xb00633ef, // Trial of Arnold Blackwood, The (1984)(Nemesis)[cr].dsk
   0xd8e6113b, // Total Eclipse II - The Sphinx Jinx (1989)(Incentive)[cr].dsk
   0x2fd6d077, // Time (1985)(Rainbow Arts)(M2)[cr].dsk
   0xd0a58cd5, // Tut's Pyramid (1986)(Artic Computing)[cr].dsk
   0x9ee6f2ad, // Toyota Celica GT Rally (1991)(Gremlin Graphics Software)[cr].dsk
   0xd12fb80e, // Toyota Celica GT Rally (1991)(Gremlin Graphics Software)[codes].dsk
   0x2e05e857, // Thunderbirds (1989)(Grandslam)(Side A).dsk
   0xf585193a, // Thunderbirds (1989)(Grandslam)(Side B).dsk
   0x2d153648, // Thunder Burner (1991)(Loriciels)[b].dsk
   0x92d89f49, // Typhoon (1988)(Ocean Software).dsk
   0xe3e92f9c, // Terror of the Deep (1987)(Mirrorsoft).dsk
   0x1b049205, // Tujad (1986)(Ariolasoft).dsk
   0xc90ffc0b, // Tron 90 (1990)(Micro Mag)(fr)[cr].dsk
   0x896dc22f, // Thundercats -The Lost Eye Of Thundera (1987)(Elite Systems).dsk
   0x94e4554b, // Targhan (1990)(Silmarils)(es)(Side A).dsk
   0x712fd650, // Targhan (1990)(Silmarils)(es)(Side B).dsk
   0x418d4134, // Targhan (1990)(Silmarils)(fr)(Side B).dsk
   0xa5e0ab2b, // Targhan (1990)(Silmarils)(fr)(Side A).dsk
   0x45b93021, // Thomas the Tank Engine (1990)(Alternative Software)[cr].dsk
   0xd49b4e2d, // Triaxos (1987)(Ariolasoft).dsk
   0x323b8bcb, // Tetris (2020)(Crazy Piri).dsk
   0x59348ec7, // Trivial Pursuit - Baby Boomer Edition (1986)(Domark)(Side A)[cr].dsk
   0x5600aa6f, // Trivial Pursuit - Baby Boomer Edition (1986)(Domark)(Side B)[cr].dsk
   0xb14db43d, // Trap Door, The (1986)(Piranha)[f].dsk
   0xc6f33707, // Triviation (1989)(Robot PD)[cr].dsk
   0x4e89b72e, // Tomcat (1989)(Players)[cr].dsk
   0x8b4b44c1, // Tensions (1986)(ERE Informatique)(M3).dsk
   0x3bef5d59, // Transx (1987)(G.T.S. Editorial)(es)[cr].dsk
   0x77c7c536, // Turbo Girl (1988)(Dinamic Software)(es).dsk
   0xb0153216, // Task Force (1989)(Players)[cr].dsk
   0xa5ca2404, // Untouchables, The (1989)(Ocean Software)(Side A).dsk
   0x057e89e3, // Untouchables, The (1989)(Ocean Software)(Side B).dsk
   0x7ff95fd6, // Uchi Mata (1987)(Martech).dsk
   0xfd402604, // Ultron 1 (1987)(Chip)(fr)[b].dsk
   0x6a27c3d0, // Une Affaire en Or (1985)(Free Game Blot)(fr)[cr].dsk
   0xfb2e07bb, // Unitrax (1987)(Domark).dsk
   0x1bd644cd, // Ultima Ratio (1987)(Firebird)[cr].dsk
   0x0e0149de, // UN Squadron (1990)(US Gold)(Side B)[b].dsk
   0x5d683df1, // UN Squadron (1990)(US Gold)(Side A)[b].dsk
   0x06aae413, // Uranium (1986)(Audimicro)(es)[cr].dsk
   0xda55de2a, // Ultimate Fight, The (1987)(CPC)(fr)[cr].dsk
   0xc2fb79b8, // Un Asunto de Dinero (2019)(Free Game Blot)(fr)(Hack)[cr].dsk
   0xcff9b5b4, // Ulises (1989)(Opera Soft).dsk
   0xfa4fa45b, // Uridium (1987)(Hewson)[tape].dsk
   0x5ed475ff, // Video Poker (1986)(Mastertronic)[cr].dsk
   0xbbce171e, // Viaje Al Centro De La Tierra - Version Extendida (2012)(Topo Soft).dsk
   0x0fb1b947, // Vagan Attack (1985)(Atlantis Software)[cr].dsk
   0xac94247b, // Venom (1988)(Mastertronic)[cr].dsk
   0x0113997b, // Very Big Cave Adventure, The (1986)(CRL Group)[cr].dsk
   0x15e2c423, // Vengeur (1990)(Eric Cubizolle)(fr)(PD)[cr].dsk
   0x4f6b8ae6, // Vera Cruz Affair, The (1986)(Infogrames).dsk
   0x7223a099, // Village of Lost Souls (1987)(Robico Software)[tape].dsk
   0xdeb966c7, // Vector Ball (1988)(Mastertronic)[cr].dsk
   0xdfb023f8, // Vendetta (1990)(System 3 Software).dsk
   0x9d417eac, // Virgin Atlantic Challenge (1986)(Virgin Games)[cr].dsk
   0xb1acd5c1, // Vol en Plein Pacifique (1999)(Patrick Meilhan)(fr)[cr].dsk
   0xb6753a1a, // Voyage au Centre de la Terre (1988)(Chip)(fr)(Side A).dsk
   0xed9df91d, // Voyage au Centre de la Terre (1988)(Chip)(fr)(Side B).dsk
   0xc1afcd65, // Venganza de Johny Comomolo, La (1986)(Dro Soft)(es)[cr].dsk
   0x234bc9ee, // Video Card Arcade (1988)(CDS Software LTD)[cr].dsk
   0xaf4b1161, // Victory Road - The Pathway To Fear (1988)(Ocean Software).dsk
   0x949cd9bb, // Vampire Killer (1984)(Scorpio Gamesworld)[cr].dsk
   0xd2228ffc, // Vikings, The (1986)(Kele Line).dsk
   0x98cc3562, // V The Visitors (1986)(Ocean Software).dsk
   0x1356d210, // Virus (1991)(LMC Software)(fr)[cr].dsk
   0x33c1c3cc, // Vindicator, The - Green Beret II (1988)(Ocean Software).dsk
   0x23fddaf0, // Viz - The Computer Game (1991)(Virgin Games)(Side B).dsk
   0xdef897b5, // Viz - The Computer Game (1991)(Virgin Games)(Side A).dsk
   0xe01abc52, // V2 (1988)(Ubi Soft)(fr)(Prototype)(Side B)[cr].dsk
   0x5fe4ae5f, // V2 (1988)(Ubi Soft)(fr)(Prototype)(Side A)[cr].dsk
   0x9ac0e9ef, // Vixen (1988)(Martech).dsk
   0x1b43e5bf, // Vindicators (1989)(Domark).dsk
   0xa6247be4, // VS4 (1987)(Bretagne Edit presse)(fr)[cr].dsk
   0x5568287b, // Voodoo Rage (1986)(Artic Computing)[cr].dsk
   0x481ce055, // Ville Infernale, La (1985)(Cobra Soft)(fr).dsk
   0x48738114, // Volley Ball (1987)(Chip)(fr).dsk
   0xcc16367e, // Vigilante (1989)(US Gold).dsk
   0x788ff9dc, // Viaje Alucinante (1986)(Idealogic)[tape].dsk
   0xa4eca0d7, // Viaje Al Centro De La Tierra (1989)(Topo Soft)(es).dsk
   0xb01c6377, // Visitor from Space, A (1984)(Games Computing)[cr].dsk
   0xa233f966, // World Class Rugby (1991)(Audiogenic Software).dsk
   0x42f0fa2b, // World Games (1987)(US Gold)(v2)(Side A).dsk
   0xd1427667, // World Games (1987)(US Gold)(v2)(Side B).dsk
   0xc6e6039a, // Who's Afraid of the Balrog (1993)(Amstrad Action Magazine)(PD)[cr].dsk
   0xb4dde954, // Wrath of Olympus (1986)(Alpha Omega Software)(v2)[cr].dsk
   0xc0e7a0b2, // Wells & Fargo (1988)(Topo Soft)(es)(Hack)[cr].dsk
   0xb49a4653, // Wells & Fargo (1988)(Topo Soft)(es)[cr].dsk
   0x71c3eff4, // War Cars Construction Set (1987)(Firebird)[cr].dsk
   0xdc07d499, // What IQ (1990)(Dave Andrews)[cr].dsk
   0xa302be17, // World Championship Soccer (1990)(Elite Systems).dsk
   0x06153c44, // World Cup (1991)(Videoplay SL)(es)[cr].dsk
   0xbf11d5ce, // Who Dares Wins II (1986)(Alligata Software).dsk
   0xa1b83838, // Warlock (1987)(The Edge).dsk
   0xb1026135, // Wonderboy (1987)(Activision).dsk
   0xc748f073, // Whist Wizard (1988)(Computing With The Amstrad)[cr].dsk
   0x42efcb30, // Winter Wonderland (1987)(Incentive)[cr].dsk
   0x1990f5a0, // Winter Wonderland (1987)(Incentive)(Hack)[cr].dsk
   0x3014d8d7, // Wiz-Biz (1987)(Alternative Software)[cr].dsk
   0x5732dcf3, // Wacky Darts (1992)(Codemasters Software)[cr].dsk
   0xdeae9918, // Wizball (1987)(Ocean Software)[f].dsk
   0x637d1c58, // Western Games (1988)(Magic Bytes)(Side A).dsk
   0x052a5a13, // Western Games (1988)(Magic Bytes)(Side B).dsk
   0x67bfa084, // Wizard's Lair (1985)(Bubble Bus Software).dsk
   0x4e22cf96, // Whopper Chase (1987)(Erbe Software)(es)[cr].dsk
   0xecade1c4, // Wings of Fury (1989)(Broderbund Software)(fr).dsk
   0xadf6c176, // Wriggler (1985)(Romantic Robot)[codes].dsk
   0xc88f468d, // Wriggler (1985)(Romantic Robot)[cr].dsk
   0xd891bffa, // Wise and Fool of Arnold Blackwood, The (1984)(Nemesis)[cr].dsk
   0xa6cc07e1, // Warrior (1986)(Rainbow Production)(fr)[cr].dsk
   0x24185bd5, // World Cup (1985)(Artic Computing)[cr].dsk
   0x361c4a1e, // West Bank (1985)(Dinamic Software)(es).dsk
   0x78cbb409, // Wulfpack (1988)(CDS Software LTD)[cr].dsk
   0x378d91dc, // Welladay (1990)(Amstrad Adventure PD Library)[cr].dsk
   0x09b248f5, // War Space (1988)(Amstrad Cent Pour Cent)(fr)[cr].dsk
   0x331a6c9c, // Wolfman (1988)(CRL Group)[cr].dsk
   0x2b5acc0a, // Who Said That (1994)(Radical Software)[cr].dsk
   0x0edfc1fd, // Willow Pattern Adventure, The (1985)(Firebird)[cr].dsk
   0xf9c01c85, // Wreckless Roger (1986)(Blaby Computer Games)[tape].dsk
   0x5ba4fa75, // Wacky Orchard (1985)(Argus Press Software)[cr].dsk
   0x099c7669, // Windsurf Willy (1989)(Silmarils)(es).dsk
   0x05022596, // Windsurf Willy (1989)(Silmarils)(fr).dsk
   0xb16409a2, // Witness, The (1984)(Infocom)[CPM].dsk
   0x06533fb5, // WWF Wrestlemania (1991)(Ocean Software).dsk
   0x45e767ce, // Welltris (1991)(Infogrames)[codes].dsk
   0xd396c1a5, // Wild Bunch, The (1985)(Firebird)[cr].dsk
   0x8103c23a, // War Game (1987)(Logipresse)(fr)[cr].dsk
   0x87b30b9a, // Wembley Greyhounds (1989)(D&H Games)[cr].dsk
   0x8e655c1e, // Wibstars (1987)(A&F Software)[cr].dsk
   0x4031468f, // Wooky and Moty (1987)(CRL Group)[cr].dsk
   0x6fda459c, // Way of Exploding Fist, The (1985)(Melbourne House)[tape].dsk
   0x54c40e54, // WEC Le Mans (1988)(Ocean Software).dsk
   0xdea21916, // World Cup (1992)(Cult)(es)[cr].dsk
   0xe814906d, // Winter Sports (1985)(Activision)(Side A).dsk
   0xb2bd3040, // Winter Sports (1985)(Activision)(Side B).dsk
   0xc55164a4, // World Championship Boxing Manager (1990)(Goliath Games)(m3).dsk
   0x701b03b6, // Wombles, The (1990)(Alternative Software)[cr].dsk
   0x294220ab, // World Soccer (1990)(Zeppelin Games)[cr].dsk
   0x5332c3ad, // Wishbringer (1985)(Infocom)[CPM].dsk
   0x60784c7d, // Wanderer 3D (1989)(Elite Systems).dsk
   0xd2d93aa9, // Way of the Tiger, The (1986)(Gremlin Graphics Software).dsk
   0xf45e1b37, // Wipeout (1990)(Mastertronic)[cr].dsk
   0x055b2c21, // Weetabix vs the Titchies (1984)(Romik Software)[tape].dsk
   0x8c37f466, // Wizard Willy (1989)(Codemasters Software)[cr].dsk
   0x4794cfb0, // Winter Games (1986)(Epyx)(v2)(Side B).dsk
   0xba91af38, // Winter Games (1986)(Epyx)(v2)(Side A).dsk
   0x0c2ec5b0, // Warlord (1985)(Interceptor Software)[cr].dsk
   0x78daed17, // War in Middle Earth (1989)(Melbourne House).dsk
   0xf114deb9, // War in Middle Earth (1989)(Melbourne House)(es).dsk
   0x275f65d5, // World Cup Carnival Mexico 86 (1986)(US Gold)(M3).dsk
   0x40d31555, // Winter Olympics (1986)(Tynesoft)[cr].dsk
   0xf33045ef, // Wizard Warz (1987)(Us Gold).dsk
   0xc7436ae1, // Well, The (1991)(Edmund Spicer)[cr].dsk
   0xe02ada0e, // Werewolves of London (1988)(Ariolasoft).dsk
   0x5b89d348, // World Series Baseball (1985)(Ocean Software)[cr].dsk
   0xe069c64a, // World Class Leadboard (1987)(US Gold)(Side B).dsk
   0x8d9ec690, // World Class Leadboard (1987)(US Gold)(Side A).dsk
   0x6dcaf69d, // Xeno (1986)(A&F Software).dsk
   0xd634c814, // Xybots (1989)(Domark).dsk
   0xbadd5079, // Xcel (1987)(Mastertronic)[cr].dsk
   0x8fbd4fc2, // X-Out (1990)(Rainbow Arts).dsk
   0x83ea5c5a, // Xanadu (1986)(-)[cr].dsk
   0x5743999a, // Xenon (1988)(Amstar & CPC)(fr)[cr].dsk
   0x1a70210d, // Xevious (1986)(US Gold).dsk
   0x0b122cad, // XOR (1987)(Logotron).dsk
   0x34cd836c, // Xenon (1988)(Melbourne House)(es).dsk
   0x91f26a80, // Xenon (1988)(Melbourne House).dsk
   0x5dfd5142, // Xenophobe (1989)(Micro Style).dsk
   0xb6ba6539, // Yarkon Blues II (1992)(WOW Software)[cr].dsk
   0x31468e60, // Yie Ar Kung-Fu II (1986)(Ocean Software).dsk
   0xd7a00940, // Yahtzee (1985)(L'Abominable)(PD)(fr)[cr].dsk
   0x8948e0ba, // Yahtzee (1989)(John Packham)(PD)[cr].dsk
   0x651cd158, // Yie Ar Kung-Fu II (1986)(Ocean Software).dsk
   0x1c0e8af7, // Yarkon Blues (1988)(Red Five Software)[cr].dsk
   0x29b7ec7c, // Young Ones, The (1986)(Orpheus)[cr].dsk
   0xddde36c9, // Yes Prime Minister (1985)(Mosaic Publishing)(Side B).dsk
   0xb010705d, // Yes Prime Minister (1985)(Mosaic Publishing)(Side A).dsk
   0x0fdaa611, // Yabba Dabba Doo! - Flintstones (1986)(Quicksilva)[cr].dsk
   0x66fb5d06, // Yogi Bear (1987)(Piranha)[cr].dsk
   0xe0240de6, // Zork II - The Wizard of Frobozz (1986)(Infocom)[CPM].dsk
   0x50827a8b, // Zarxas (1987)(Chip)(fr)[cr].dsk
   0x38696747, // Zombi (1986)(Ubi Soft)(Side A).dsk
   0xbdc6cb01, // Zombi (1986)(Ubi Soft)(Side B).dsk
   0x13f8cb86, // Z (1985)(Rino Marketing).dsk
   0xae24ce98, // Zorgos (1987)(Deltacom)(de)[cr].dsk
   0xf820f9ff, // Zygy (1986)(Infogrames)(fr)[cr].dsk
   0x81ab7c38, // Zaxx (1986)(Chip)(fr).dsk
   0x17fc88d6, // Zelda (1991)(Le Chat Cyril)(fr)[cr].dsk
   0xa54b45be, // Zap 't' Balls (1992)(CPC Amstrad International)[cr].dsk
   0xfd3724e2, // Zorro (1985)(US Gold).dsk
   0xcd86c6a1, // Zarkon (1987)(Budgie)[cr].dsk
   0x825af888, // Zona 0 (1991)(Topo Soft)(es).dsk
   0x39b134bc, // Zone (1989)(Micro Mag)[cr].dsk
   0xb503d731, // Zox 2099 (1987)(Loriciels)(fr).dsk
   0x2b45253e, // Zyclons (1992)(Wizcat)(PD)[cr].dsk
   0xa3fb554f, // Zork III - The Dungeon Master (1986)(Infocom)[CPM].dsk
   0x1339daf5, // Zampabolas (1988)(System 4)(es)[cr].dsk
   0x28a9cbcc, // Zombi Terror Reloaded (2016)(Bitzarro Games)(fr).dsk
   0x26b37cdd, // Zombi Terror Reloaded (2016)(Bitzarro Games).dsk
   0x60771b5f, // Zombi Terror Reloaded (2016)(Bitzarro Games)(es).dsk
   0xa831626e, // Zeichner, Der (1988)(Bollaware)(de)[cr].dsk
   0x7551f406, // Zub (1986)(Mastertronic)[cr].dsk
   0xa02c3758, // Zork I - The Great Underground Empire (1986)(Infocom)[CPM].dsk
   0xe6a9ad7d, // Zorakk The Conqueror (1985)(Icon Software)[cr].dsk
   0x526f5115, // Zipi y Zape (1989)(Dro Soft)(es)[f].dsk
   0x7ba6705f, // Zynaps (1987)(Hewson).dsk
   0xb729a281, // Ziggurat (1988)(Ariolasoft).dsk
   0x81c617ee, // Zodera Schloss des Todes (19xx)(Martin Stuehlmann)(de)[cr].dsk
   0x81df2ee2, // 007 - The Spy Who Loved Me (1990)(Domark)(Side A).dsk
   0x777acc7f, // 007 - The Spy Who Loved Me (1990)(Domark)(Side B)[64K].dsk
   0xeae4f4b0, // 3D Pool (1989)(Firebird Software).dsk
   0xb7c31b0d, // 007 - The Living Daylights (1987)(Domark).dsk
   0xb60b8c58, // 007 - Live and Let Die (1988)(Domark).dsk
   0x90c6d8cc, // 12 Lost Souls (1987)(Amstrad Adventure PD Library)[cr].dsk
   0x3892dad5, // 3DC (1987)(Encore)[cr].dsk
   0x16b25d5f, // 2 Player Soccer Squad (1986)(Cult Games)[cr].dsk
   0xf4bb9c85, // 4 Soccer Simulators (1989)(Codemasters Gold).dsk
   0x90b13e33, // 180 (1986)(Mastertronic)[cr].dsk
   0x84efcc59, // 750cc Grand Prix (1989)(Codemasters Software)[cr].dsk
   0xc14ee245, // 7 Card Stud (1986)(Martech Games).dsk
   0xd5369bd2, // 007 - A View To A Kill (1985)(Domark).dsk
   0xb6ca86e1, // 007 - A View To A Kill (1985)(Eureka)(fr).dsk
   0x6bb453d1, // 1815 (1985)(Cobra Soft)(fr).dsk
   0x91da0112, // 4x4 Off-Road Racing (1988)(US Gold).dsk
   0x1037bc27, // 20000 Lieus sous les Mers (1988)(Coktel Vision)(fr)(Side B).dsk
   0xb8b6a065, // 20000 Lieus sous les Mers (1988)(Coktel Vision)(fr)(Side A)[CPM].dsk
   0x0972c229, // 720 Degrees (1986)(US Gold).dsk
   0xb70c908c, // 2112 AD (1986)(Design Design Software)(M4).dsk
   0x8769d0b1, // 1943 - The Battle of Midway (1988)(US Gold).dsk
   0x5e15e228, // 3D Starstrike (1985)(Realtime Games Software)[cr].dsk
   0x104e2d14, // 3D Monster Chase (1984)(Romik Software)[cr].dsk
   0xeb184a31, // 1914 (1987)(Amstrad User)[cr].dsk
   0xb78305d5, // 1914 (1987)(Amstar)(fr)[cr].dsk
   0x1e1411be, // 10th Frame (1986)(US Gold).dsk
   0xddc7a9a8, // 20000 Avant J.C. (1987)(Chip)(fr).dsk
   0x12ed8818, // 1001 BC (1986)(ERE Informatique)(fr).dsk
   0xe1e61287, // 3D Grand Prix (1985)(Amsoft).dsk
   0x9cee0fb7, // 3D Fight (1985)(Loriciels)(fr).dsk
   0x3db6ed05, // 3D Morpion (1985)(Hebdogiciel)(fr)[cr].dsk
   0x65872552, // 4th Protocol, The (1986)(Century Communications)[cr].dsk
   0x9aaec7e3, // 3D-Sub (1985)(Loriciels)(fr)[cr].dsk
   0x4b2b8949, // 5th Axis, The (1985)(Loriciels).dsk
   0x0c2d33c9, // 5th Axis, The (1985)(Loriciels)(fr).dsk
   0x53e36bf4, // 3D Starfighter (1988)(Codemasters Software)[cr].dsk
   0xd9866b04, // 3D Voicechess (1985)(CP Software).dsk
   0x0611e83f, // 3D Voicechess (1985)(CP Software)(fr).dsk
   0xa5df0936, // 3D Voicechess (1985)(CP Software)(es).dsk
   0xab928e90, // 1942 (1986)(Elite Systems).dsk
   0x557729ab, // 3D Quark Invaders (1985)(Amsoft)[cr].dsk
   0xa4663877, // 3 Guerra Mundial (1989)(Pactum)(es)(Side B)[cr].dsk
   0x0009f378, // 3 Guerra Mundial (1989)(Pactum)(es)(Side A)[cr].dsk
};