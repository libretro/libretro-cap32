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

// clean-cpc-db CRC database - not working games on cap32
uint32_t dbfail[] = {
   0xbb2a95f6, // Amelie Minuit (1985)(ERE Informatique)(fr).dsk
   0x4c35740d, // Asterix et la Potion Magique (1988)(Coktel Vision)(fr).dsk
   0x6a1948eb, // Antre de Gork, L' (1987)(Excalibur)(fr)(Side B)[f].dsk
   0xea140f7f, // Antre de Gork, L' (1987)(Excalibur)(fr)(Side A)[f].dsk
   0x483d8366, // Alphakhor (1989)(Loriciels)(fr)(Side A).dsk
   0x2a7ad414, // Alphakhor (1989)(Loriciels)(fr)(Side B).dsk
   0x4cffaa69, // Aventures de Moktar, Les (1992)(Titus)(fr).dsk
   0xdd29fe62, // Basun (1986)(Multicrom).dsk
   0x1bb90675, // Birdie (1987)(Ere Informatique).dsk
   0x925415df, // Birdie (1987)(Ere Informatique)(fr).dsk
   0x646b68a9, // Beurk Roger (1985)(Amstrad Computer User)(fr)(Hack)[cr].dsk
   0xe0e1a064, // Blue Crystal, The (1985)(Rainbow Arts)(fr)(Side A).dsk
   0x2db6da2a, // Blaue Kristall, Der (1985)(Rainbow Arts)(de)(Side B).dsk
   0x47659579, // Blue Crystal, The (1985)(Rainbow Arts)(fr)(Side B).dsk
   0x0e63c83e, // Blaue Kristall, Der (1985)(Rainbow Arts)(de)(Side A).dsk
   0x0c27ad3d, // Battle of the Bulge (1990)(Cases Computer Simulators).dsk
   0x43ce2bae, // Bob Morane - Science Fiction (1987)(Infogrames)(fr).dsk
   0x3bf52bf5, // Bounty Hunter (19xx)(The Adventure Workshop)[cr].dsk
   0x84052472, // Blues Brothers, The (1991)(Titus).dsk
   0x9f759eb0, // Back To The Future III (1990)(Image Works)(Side A).dsk
   0x2fc30262, // Back To The Future III (1990)(Image Works)(Side B).dsk
   0x304b0e30, // Back To The Future III (1990)(Image Works)(es)(Side A).dsk
   0x891011d8, // Back To The Future III (1990)(Image Works)(es)(Side B).dsk
   0x7234d56e, // Big League Soccer (1985)(Viper)(es)(Hack)[cr].dsk
   0xe23c20be, // Bataille d'Angleterre, La (1985)(PSS)(fr).dsk
   0x02914bd1, // Blockhaus (1986)(Amstrad Magazine)(fr)[cr].dsk
   0x12a094b3, // Bivouac - Chamonix Challenge (1987)(Infogrames)(fr).dsk
   0xe55eb7d9, // Bonanza Bros (1991)(US Gold).dsk
   0x4f3022bc, // Camelot Warriors (1987)(Dinamic Software).dsk
   0x585cbc31, // Cite Perdu, La (1987)(Excalibur)(fr)(Disk 2 Side A).dsk
   0xf490084a, // Cite Perdu, La (1987)(Excalibur)(fr)(Disk 1 Side A).dsk
   0xeeb62090, // Cite Perdu, La (1987)(Excalibur)(fr)(Disk 2 Side B).dsk
   0xff52f67e, // Cite Perdu, La (1987)(Excalibur)(fr)(Disk 1 Side B).dsk
   0x17d4c34e, // Crafton & Xunk (1986)(ERE Informatique)(M3).dsk
   0x95724912, // Crafton & Xunk (1986)(ERE Informatique)(es).dsk
   0x1dc4b790, // Chose de Grotemburg, La (1987)(Ubi Soft)(fr)(Side B).dsk
   0x25aeae06, // Chose de Grotemburg, La (1987)(Ubi Soft)(fr)(Side A).dsk
   0xfc3f6c0c, // Conspiration De L'An III (1988)(Ubi Soft)(fr)(Side B).dsk
   0x75e574cd, // Conspiration De L'An III (1988)(Ubi Soft)(fr)(Side A).dsk
   0xbef7ff31, // Clash (1987)(ERE Informatique)(fr)(Side B).dsk
   0xbb2d6aff, // Clash (1987)(ERE Informatique)(fr)(Side A).dsk
   0xb2f8a240, // Dakar Moto (1987)(Coktel Vision)(fr).dsk
   0x66e32c4c, // Disc (1990)(Loriciels).dsk
   0xeb8ab156, // Daley Thompson's Olympic Challenge (1988)(Ocean).dsk
   0x6b36fb72, // Derniere Mission, La (1988)(MBC Informatique)(fr)(Side A).dsk
   0x7b411704, // Derniere Mission, La (1988)(MBC Informatique)(fr)(Side B).dsk
   0xbe2eb246, // Dark Century (1990)(Titus).dsk
   0xb857f5ca, // Defender of the Crown (1987)(Cinemaware)(fr)(Side A).dsk
   0xff0a5212, // Defender of the Crown (1987)(Cinemaware)(fr)(Side B).dsk
   0x3bd70a17, // Despotik Design (1987)(ERE Informatique)(M3).dsk
   0xa82993c8, // Duel 2000 (1986)(Coktel Vision)(fr).dsk
   0x1aebb6a6, // Dan Silver (1989)(MBC Informatique)(fr)(Side B)[cr].dsk
   0xebf36ee7, // Dan Silver (1989)(MBC Informatique)(fr)(Side A)[cr].dsk
   0x1135fab1, // Druid (1986)(Firebird).dsk
   0xfce191c0, // Doomsday Blues (1985)(ERE Informatique).dsk
   0x9bed1bbf, // Detective (1990)(News by Disc)(fr)[cr].dsk
   0x68233cd0, // Eden Blues (1985)(ERE Informatique)(fr).dsk
   0x6eaebfc4, // Eden Blues (1985)(ERE Informatique)(es).dsk
   0xa4bcfc77, // Electric Wonderland (1986)(ERE Informatique).dsk
   0x8442d3bb, // Exit (1988)(Ubi Soft)(fr)(Side A).dsk
   0x0e6de21c, // Exit (1988)(Ubi Soft)(fr)(Side B).dsk
   0xacf41d84, // E-SWAT (1990)(US Gold).dsk
   0xca2062f6, // Faial (1986)(Excalibur)(fr)(Side B).dsk
   0xbca5acb2, // Faial (1986)(Excalibur)(fr)(Side A).dsk
   0xdd86f049, // Fantome City (1986)(Coktel Vision)(fr).dsk
   0x7e46ba9d, // Fer & Flamme (1986)(Ubi Soft)(fr)(Disk 1 Side B).dsk
   0x3f36917a, // Fer & Flamme (1986)(Ubi Soft)(fr)(Disk 2 Side A).dsk
   0xbc7b47c3, // Fer & Flamme (1986)(Ubi Soft)(fr)(Disk 1 Side A).dsk
   0x4bc9d603, // Fer & Flamme (1986)(Ubi Soft)(fr)(Disk 2 Side B).dsk
   0xb10f45ff, // Get Dexter (1986)(ERE Informatique).dsk
   0xfbe93c98, // Globe-Trotter (1986)(Excalibur)(fr)(Disk 2 Side A).dsk
   0x1a07c6cc, // Globe-Trotter (1986)(Excalibur)(fr)(Disk 2 Side B).dsk
   0xc8d40fe6, // Globe-Trotter (1986)(Excalibur)(fr)(Disk 1 Side A).dsk
   0x9951e870, // Globe-Trotter (1986)(Excalibur)(fr)(Disk 1 Side B).dsk
   0x47f7f71f, // Hamsters en Folie (1989)(Generation 5)(fr).dsk
   0x1ac09f99, // Infernal House (1991)(Lankhor)(fr)(Side A).dsk
   0xce141c60, // Infernal House (1991)(Lankhor)(fr)(Side B).dsk
   0xc874fc01, // Jaws - Le Dernier Etalon (1988)(MBC Informatique)(fr)(Side B).dsk
   0xb21fb6b0, // Jaws - Le Dernier Etalon (1988)(MBC Informatique)(fr)(Side A).dsk
   0xfaa04721, // James Debug - El Misterio De La Isla Perdida (1986)(Coktel Vision)(es)[cr].dsk
   0x21de51e7, // James Debug - Mystere de l'Ile Perdue (1986)(Coktel Vision).dsk
   0x0e81f696, // Knight Force (1989)(Titus)(Side A).dsk
   0xfb10b3bb, // Knight Force (1989)(Titus)(Side B).dsk
   0x70021b43, // Labyrinthe D'Anglomania 2, Le (1990)(Retz)(fr)(Side B).dsk
   0xb0a1e816, // Labyrinthe D'Anglomania 2, Le (1990)(Retz)(fr)(Side A).dsk
   0xaa6728e1, // Maitre Absolu, Le (1989)(Ubi Soft)(fr)(Side B).dsk
   0x23af9eb4, // Maitre Absolu, Le (1989)(Ubi Soft)(fr)(Side A).dsk
   0x888d910d, // Macrocosmica (1986)(Datacom Publications).dsk
   0x3edfb68f, // Mission Delta (1985)(ERE Informatique)(fr).dsk
   0xb84f3627, // M'enfin - Gaston (1987)(Ubi Soft)(fr)(Side B).dsk
   0x34dc1faa, // M'enfin - Gaston (1987)(Ubi Soft)(fr)(Side A).dsk
   0xf9bc30df, // Molecularr (1990)(Amstrad Cent Pour Cent)[cr].dsk
   0x82f0d494, // Mindfighter (1988)(Activision).dsk
   0xaa35d5c2, // Macadam Bumper (1985)(PSS)(es)[cr].dsk
   0x65bdaadd, // Macadam Bumper (1985)(PSS).dsk
   0x16c28934, // Macadam Bumper (1985)(PSS)(de)[cr].dsk
   0xfeb65eff, // Masque (1986)(Ubi Soft)(fr)(Side A).dsk
   0xd50bb5a1, // Masque (1986)(Ubi Soft)(fr)(Side B).dsk
   0x57f4d790, // Mountie Mick's Death Ride (1987)(Reaktor).dsk
   0x061fb016, // Moon Blaster (1990)(Loriciels).dsk
   0xd5fc1c36, // Megablasters (1994)(Radical Software)(Disk 2 Side B).dsk
   0x1eb1e1e1, // Megablasters (1994)(Radical Software)(Disk 2 Side A).dsk
   0x8f71b89c, // Megablasters (1994)(Radical Software)(Disk 1 Side B).dsk
   0x75a53bec, // Megablasters (1994)(Radical Software)(Disk 1 Side A).dsk
   0x22dc1fb5, // Necromancien, Le (1987)(Ubi Soft)(fr)(Side B).dsk
   0x3d981755, // Necromancien, Le (1987)(Ubi Soft)(fr)(Side A).dsk
   0x5579ba92, // Nosferatu the Vampyre (1986)(Piranha)[b].dsk
   0x365d9220, // Oxphar (1987)(ERE Informatique)(fr)(Side A).dsk
   0x05329e84, // Oxphar (1987)(ERE Informatique)(fr)(Side B).dsk
   0x1959a428, // One (1986)(D3M).dsk
   0xc09cd00c, // Peur Sur Amityville (2019)(Ubi Soft)(es)(Hack)(Side A)[cr].dsk
   0xaffa2688, // Peur Sur Amityville (2019)(Ubi Soft)(es)(Hack)(Side B)[cr].dsk
   0x72046022, // Peur Sur Amityville (1987)(Ubi Soft)(fr)(Side A).dsk
   0xd3d2e2fa, // Peur Sur Amityville (1987)(Ubi Soft)(fr)(Side B).dsk
   0x7789187a, // Prehistorik (1991)(Titus).dsk
   0x0d4da0fb, // Puffy's Saga (1989)(Ubi Soft)(Side A).dsk
   0x70cd64bc, // Puffy's Saga (1989)(Ubi Soft)(Side B).dsk
   0xad4e58e2, // Pepe Bequilles (1987)(Softhawk)(fr).dsk
   0x06f28d8f, // Pepe Bequilles (1987)(Softhawk)(es).dsk
   0xcb8ae96e, // PFC - PFS (19xx)(MEBC)(fr)(PD)[cr].dsk
   0x35d2ed86, // Palitron (1986)(The Edge Software).dsk
   0x93b95448, // Quiwi (1986)(Kingsoft)(de)(Side A)[BASIC 1.0].dsk
   0x7196f5c3, // Quiwi (1986)(Kingsoft)(de)(Side B)[BASIC 1.0].dsk
   0xf9521465, // Qin (1987)(ERE Informatique)(fr)(Disk 1 Side A).dsk
   0xdc7255b4, // Qin (1987)(ERE Informatique)(fr)(Disk 1 Side B).dsk
   0x39e1a11b, // Qin (1987)(ERE Informatique)(fr)(Disk 2 Side B).dsk
   0x7cad58fd, // Qin (1987)(ERE Informatique)(fr)(Disk 2 Side A).dsk
   0x1e4269a4, // Robin of Sherlock (1986)(CRL Group)[cr].dsk
   0x62c50234, // Robbbot (1986)(ERE Informatique)(es)(Side B).dsk
   0xd6a9cd52, // Robbbot (1986)(ERE Informatique)(es)(Side A).dsk
   0x928f12b2, // Renaud (1987)(Infogrames)(fr).dsk
   0x4bb21249, // Rock Hopper (1985)(Amstrad Computer User)[cr].dsk
   0xcce626da, // Reisende im Wind 2 (1987)(Infogrames)(de)(Side B).dsk
   0x72e302b8, // Reisende im Wind 2 (1987)(Infogrames)(de)(Side A).dsk
   0x04ae8758, // Rodeo (1986)(Microids)(fr).dsk
   0x3aa1ad57, // Ripoux, Les (1987)(Cobra Soft)(fr)(Side B).dsk
   0x229083cc, // Ripoux, Les (1987)(Cobra Soft)(fr)(Side A).dsk
   0x1068d4af, // R-Type (UK) (Face 1) (1989) (Spain retail version) [Original].dsk
   0x369a0e05, // R-Type (UK) (Face 2) (1989) (Spain retail version) [Original].dsk
   0x86f87f1d, // Recherche Miss X Deseperement (1989)(Fanatic)(fr).dsk
   0x75ac076d, // Rana (1990)(PC Amstrad International)(de)[cr].dsk
   0x401dbe1e, // Sliders (1991)(Microids)[f].dsk
   0xba4c03dc, // Saga (1990)(Lankhor)(fr)(Side B).dsk
   0xce5c2785, // Saga (1990)(Lankhor)(fr)(Side A).dsk
   0x25723968, // Scoop Senior (1990)(Generation 5)(fr)(Side A).dsk
   0x91f263de, // Scoop Senior (1990)(Generation 5)(fr)(Side B).dsk
   0xdf48e423, // Star Driver (1994)(Radical Software).dsk
   0x167ea88a, // Soccer 86 (1985)(Loriciels).dsk
   0xe94c9501, // Skweek (1989)(Loriciels)(fr).dsk
   0x1e51e321, // Skweek (1989)(Loriciels).dsk
   0x84e5676e, // Skateball (1989)(Ubi Soft).dsk
   0xddc50888, // Scapeghost (1989)(Level 9 Computing)(Side A).dsk
   0x04b155b5, // Scapeghost (1989)(Level 9 Computing)(Side B).dsk
   0x600fad6d, // Scientific (1987)(Chip)(fr).dsk
   0xfd464030, // Subbuteo - The Computer Game (1990)(Electronic Zoo)(es).dsk
   0xe20b9533, // Subbuteo - The Computer Game (1990)(Electronic Zoo).dsk
   0x8cecd42c, // Super Flippard (1987)(Free Game Blot)(fr).dsk
   0xd611e454, // SRAM 2 (1986)(ERE Informatique)(fr)(Side A).dsk
   0x2bfd2901, // SRAM 2 (1986)(ERE Informatique)(fr)(Side B).dsk
   0xbe597faa, // SRAM (1986)(ERE Informatique)(M3)(Side B).dsk
   0x3f4b2026, // SRAM (1986)(ERE Informatique)(M3)(Side A).dsk
   0xd5e3633d, // Satellite Warrior (1985)(Amsoft).dsk
   0xeea7a690, // Sai Combat (1986)(Mirrorsoft)(fr).dsk
   0x0ccf7406, // Turbo Cup (1988)(Loriciels).dsk
   0x3036e305, // They Stole a Million (1986)(39 Steps)(fr).dsk
   0x21b3e058, // They Stole a Million (1986)(39 Steps).dsk
   0xd36c1fdc, // They Stole a Million (1986)(39 Steps)(de).dsk
   0xb2ea5762, // Titus the Fox (1992)(Titus).dsk
   0x98fc3b45, // Thomas The Tank Engine's Fun With Words (1990)(Alternative Software).dsk
   0xb9ddb20d, // Tensions (1986)(ERE Informatique)(es).dsk
   0x0e7422d2, // Thunder Blade (1988)(US Gold).dsk
   0x425b6165, // Top Level (1988)(MBC Informatique)(fr)(Side B).dsk
   0xc1de5e93, // Top Level (1988)(MBC Informatique)(fr)(Side A).dsk
   0x01945b8c, // Terre et Conquerants (1989)(Ubi Soft)(fr)(Side A).dsk
   0xc2fa28ae, // Terre et Conquerants (1989)(Ubi Soft)(fr)(Side B).dsk
   0x648dc666, // Wild Streets (1990)(Titus).dsk
   0xdbce3abc, // Winchester (1988)(Chip)(fr).dsk
   0x5b80457c, // Warrior Plus (1986)(Rainbow Productions)(fr)(Side A).dsk
   0x6ce693b1, // Warrior Plus (1986)(Rainbow Productions)(fr)(Side B).dsk
   0x80ae71bb, // Xarq (1986)(Activision).dsk
   0x557ae5ef, // Zania (1985)(Myrddin Software).dsk
   0xe4f758d3, // Zombi (1986)(Ubi Soft)(fr)(Side B).dsk
   0xc13903e5, // Zombi (1986)(Ubi Soft)(es)(Side A).dsk
   0x777bce70, // Zombi (1986)(Ubi Soft)(de)(Side A).dsk
   0x6b941913, // Zombi (1986)(Ubi Soft)(fr)(Side A).dsk
   0xf4fe2bba, // Zombi (1986)(Ubi Soft)(de)(Side B).dsk
   0x7526075f, // Zombi (1986)(Ubi Soft)(es)(Side B).dsk
   0xf23f30d4, // 3D Boxing (1985)(Amsoft).dsk
   0x638c76d0, // 3D Stunt Rider (1985)(Amsoft).dsk
};
