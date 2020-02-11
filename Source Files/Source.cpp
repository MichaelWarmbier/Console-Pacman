#include "Main.h"
#include "Game.h"
#include "TitleScreen.h"
#include "Pac.h"

int SpriteData[500][2]{
	// X, Y //
	// Alphabet	RED		//
	  0,  0, //		001 // [R] A
	  1,  0, //		002 // [R] B
	  2,  0, //		003 // [R] C
	  3,  0, //		004 // [R] D
	  4,  0, //		005 // [R] E
	  5,  0, //		006 // [R] F
	  6,  0, //		007 // [R] G
	  7,  0, //		008 // [R] H
	  8,  0, //		009 // [R] I
	  9,  0, //		010 // [R] J
	 10,  0, //		011 // [R] K
	 11,  0, //		012 // [R] L
	 12,  0, //		013 // [R] M
	 13,  0, //		014 // [R] N
	 14,  0, //		015 // [R] O
	  0,  1, //		016 // [R] P
	  1,  1, //		017 // [R] Q
	  2,  1, //		018 // [R] R
	  3,  1, //		019 // [R] S
	  4,  1, //		020 // [R] T
	  5,  1, //		021 // [R] U
	  6,  1, //		022 // [R] V
	  7,  1, //		023 // [R] W
	  8,  1, //		024 // [R] X
	  9,  1, //		025 // [R] Y
	 10,  1, //		026 // [R] Z
	 11,  1, //		027 // [R] !
	 12,  1, //		028 // [R] (C)
	  0,  2, //		029 // [R] 0
	  1,  2, //		030 // [R] 1
	  2,  2, //		031 // [R] 2
	  3,  2, //		032 // [R] 3
	  4,  2, //		033 // [R] 4
	  5,  2, //		034 // [R] 5
	  6,  2, //		035 // [R] 6
	  7,  2, //		036 // [R] 7
	  8,  2, //		037 // [R] 8
	  9,  2, //		038 // [R] 9
	 10,  2, //		039 // [R] /
	 11,  2, //		040 // [R] -
	 12,  2, //		041 // [R] "
	 // Alphabet PINK	//
	 15,  0, //		042 // [P] A
	 16,  0, //		043 // [P] B
	 17,  0, //		044 // [P] C
	 18,  0, //		045 // [P] D
	 19,  0, //		046 // [P] E
	 20,  0, //		047 // [P] F
	 21,  0, //		048 // [P] G
	 22,  0, //		049 // [P] H
	 23,  0, //		050 // [P] I
	 24,  0, //		051 // [P] J
	 25,  0, //		052 // [P] K
	 26,  0, //		053 // [P] L
	 27,  0, //		054 // [P] M
	 28,  0, //		055 // [P] N
	 29,  0, //		056 // [P] O
	 15,  1, //		057 // [P] P
	 16,  1, //		058 // [P] Q
	 17,  1, //		059 // [P] R
	 18,  1, //		060 // [P] S
	 19,  1, //		061 // [P] T
	 20,  1, //		062 // [P] U
	 21,  1, //		063 // [P] V
	 22,  1, //		064 // [P] W
	 23,  1, //		065 // [P] X
	 24,  1, //		066 // [P] Y
	 25,  1, //		067 // [P] Z
	 26,  1, //		068 // [P] !
	 27,  1, //		069 // [P] (C)
	 15,  2, //		070 // [P] 0
	 16,  2, //		071 // [P] 1
	 17,  2, //		072 // [P] 2
	 18,  2, //		073 // [P] 3
	 19,  2, //		074 // [P] 4
	 20,  2, //		075 // [P] 5
	 21,  2, //		076 // [P] 6
	 22,  2, //		077 // [P] 7
	 23,  2, //		078 // [P] 8
	 24,  2, //		079 // [P] 9
	 25,  2, //		080 // [P] /
	 26,  2, //		081 // [P] -
	 27,  2, //		082 // [P] "
	 // Alphabet BLUE	//
	  0,  3, //		083 // [B] A
	  1,  3, //		084 // [B] B
	  2,  3, //		085 // [B] C
	  3,  3, //		086 // [B] D
	  4,  3, //		087 // [B] E
	  5,  3, //		088 // [B] F
	  6,  3, //		089 // [B] G
	  7,  3, //		090 // [B] H
	  8,  3, //		091 // [B] I
	  9,  3, //		092 // [B] J
	 10,  3, //		093 // [B] K
	 11,  3, //		094 // [B] L
	 12,  3, //		095 // [B] M
	 13,  3, //		096 // [B] N
	 14,  3, //		097 // [B] O
	  0,  4, //		098 // [B] P
	  1,  4, //		099 // [B] Q
	  2,  4, //		100 // [B] R
	  3,  4, //		101 // [B] S
	  4,  4, //		102 // [B] T
	  5,  4, //		103 // [B] U
	  6,  4, //		104 // [B] V
	  7,  4, //		105 // [B] W
	  8,  4, //		106 // [B] X
	  9,  4, //		107 // [B] Y
	 10,  4, //		108 // [B] Z
	 11,  4, //		109 // [B] !
	 12,  4, //		110 // [B] (C)
	  0,  5, //		111 // [B] 0
	  1,  5, //		112 // [B] 1
	  2,  5, //		113 // [B] 2
	  3,  5, //		114 // [B] 3
	  4,  5, //		115 // [B] 4
	  5,  5, //		116 // [B] 5
	  6,  5, //		117 // [B] 6
	  7,  5, //		118 // [B] 7
	  8,  5, //		119 // [B] 8
	  9,  5, //		120 // [B] 9
	 10,  5, //		121 // [B] /
	 11,  5, //		122 // [B] -
	 12,  5, //		123 // [B] "
	  // Alphabet ORANGE//
	 15,  3, //		124 // [O] A
	 16,  3, //		125 // [O] B
	 17,  3, //		126 // [O] C
	 18,  3, //		127 // [O] D
	 19,  3, //		128 // [O] E
	 20,  3, //		129 // [O] F
	 21,  3, //		130 // [O] G
	 22,  3, //		131 // [O] H
	 23,  3, //		132 // [O] I
	 24,  3, //		133 // [O] J
	 25,  3, //		134 // [O] K
	 26,  3, //		135 // [O] L
	 27,  3, //		136 // [O] M
	 28,  3, //		137 // [O] N
	 29,  3, //		138 // [O] O
	 15,  4, //		139 // [O] P
	 16,  4, //		140 // [O] Q
	 17,  4, //		141 // [O] R
	 18,  4, //		142 // [O] S
	 19,  4, //		143 // [O] T
	 20,  4, //		144 // [O] U
	 21,  4, //		145 // [O] V
	 22,  4, //		146 // [O] W
	 23,  4, //		147 // [O] X
	 24,  4, //		148 // [O] Y
	 25,  4, //		149 // [O] Z
	 26,  4, //		150 // [O] !
	 27,  4, //		151 // [O] (C)
	 15,  5, //		152 // [O] 0
	 16,  5, //		153 // [O] 1
	 17,  5, //		154 // [O] 2
	 18,  5, //		155 // [O] 3
	 19,  5, //		156 // [O] 4
	 20,  5, //		157 // [O] 5
	 21,  5, //		158 // [O] 6
	 22,  5, //		159 // [O] 7
	 23,  5, //		160 // [O] 8
	 24,  5, //		161 // [O] 9
	 25,  5, //		162 // [O] /
	 26,  5, //		163 // [O] -
	 27,  5, //		164 // [O] "
	  // Alphabet YELLOW	//
	  0,  6, //		165 // [Y] A
	  1,  6, //		166 // [Y] B
	  2,  6, //		167 // [Y] C
	  3,  6, //		168 // [Y] D
	  4,  6, //		169 // [Y] E
	  5,  6, //		170 // [Y] F
	  6,  6, //		171 // [Y] G
	  7,  6, //		172 // [Y] H
	  8,  6, //		173 // [Y] I
	  9,  6, //		174 // [Y] J
	 10,  6, //		175 // [Y] K
	 11,  6, //		176 // [Y] L
	 12,  6, //		178 // [Y] M
	 13,  6, //		179 // [Y] N
	 14,  6, //		180 // [Y] O
	  0,  7, //		181 // [Y] P
	  1,  7, //		182 // [Y] Q
	  2,  7, //		182 // [Y] R
	  3,  7, //		183 // [Y] S
	  4,  7, //		184 // [Y] T
	  5,  7, //		185 // [Y] U
	  6,  7, //		186 // [Y] V
	  7,  7, //		187 // [Y] W
	  8,  7, //		188 // [Y] X
	  9,  7, //		189 // [Y] Y
	 10,  7, //		190 // [Y] Z
	 11,  7, //		191 // [Y] !
	 12,  7, //		192 // [Y] (C)
	  0,  8, //		193 // [Y] 0
	  1,  8, //		194 // [Y] 1
	  2,  8, //		195 // [Y] 2
	  3,  8, //		196 // [Y] 3
	  4,  8, //		197 // [Y] 4
	  5,  8, //		198 // [Y] 5
	  6,  8, //		199 // [Y] 6
	  7,  8, //		200 // [Y] 7
	  8,  8, //		201 // [Y] 8
	  9,  8, //		202 // [Y] 9
	 10,  8, //		203 // [Y] /
	 11,  8, //		204 // [Y] -
	 12,  8, //		205 // [Y] "
	  // Alphabet GRAY  //
	 15,  6, //		206 // [G] A
	 16,  6, //		207 // [G] B
	 17,  6, //		208 // [G] C
	 18,  6, //		209 // [G] D
	 19,  6, //		210 // [G] E
	 20,  6, //		211 // [G] F
	 21,  6, //		212 // [G] G
	 22,  6, //		213 // [G] H
	 23,  6, //		214 // [G] I
	 24,  6, //		215 // [G] J
	 25,  6, //		216 // [G] K
	 26,  6, //		217 // [G] L
	 27,  6, //		218 // [G] M
	 28,  6, //		219 // [G] N
	 29,  6, //		220 // [G] O
	 15,  7, //		221 // [G] P
	 16,  7, //		222 // [G] Q
	 17,  7, //		223 // [G] R
	 18,  7, //		224 // [G] S
	 19,  7, //		225 // [G] T
	 20,  7, //		226 // [G] U
	 21,  7, //		227 // [G] V
	 22,  7, //		228 // [G] W
	 23,  7, //		229 // [G] X
	 24,  7, //		230 // [G] Y
	 25,  7, //		231 // [G] Z
	 26,  7, //		232 // [G] !
	 27,  7, //		233 // [G] (C)
	 15,  8, //		234 // [G] 0
	 16,  8, //		235 // [G] 1
	 17,  8, //		236 // [G] 2
	 18,  8, //		237 // [G] 3
	 19,  8, //		238 // [G] 4
	 20,  8, //		239 // [G] 5
	 21,  8, //		240 // [G] 6
	 22,  8, //		241 // [G] 7
	 23,  8, //		242 // [G] 8
	 24,  8, //		243 // [G] 9
	 25,  8, //		244 // [G] /
	 26,  8, //		245 // [G] -
	 27,  8, //		246 // [G] "
	  // Wall Tileset WHITE //
	  0,  9, //		247 // [W] Thick Top Right Corner
	  1,  9, //		248 // [W] Thick Top Left Corner
	  2,  9, //		249 // [W] Thick Left Wall
	  3,  9, //		250 // [W] Thick Right Wall
	  4,  9, //		251 // [W] Thick Bottom Right Corner
	  5,  9, //		252 // [W] Thick Bottom Left Corner
	  6,  9, //		253 // [W] Thick Fork Wall R1
	  7,  9, //		254 // [W] Thick Fork Wall R2
	  8,  9, //		255 // [W] Thick Fork Wall R3
	  9,  9, //		256 // [W] Thick Fork Wall R4
	 10,  9, //		257 // [W] Thick Top Wall
	 11,  9, //		258 // [W] Thick Bottom Wall
	 12,  9, //		259 // [W] Thin Top Wall
	 13,  9, //		260 // [W] Long Thin Corner Bottom Right
	 14,  9, //		261 // [W] Long Thin Corner Bottom Left
	 15,  9, //		262 // [W] Long Thin Corner Top Right
	 16,  9, //		263 // [W] Long Thin Corner Top Left
	 17,  9, //		264 // [W] Thin Bottom Wall
	 18,  9, //		265 // [W] Thin Corner Top Right
	 19,  9, //		266 // [W] Thin Corner Top Left
	 20,  9, //		267 // [W] Thin Left Wall
	 21,  9, //		268 // [W] Thin Right Wall
	 22,  9, //		269 // [W] Thin Corner Bottom Right
	 23,  9, //		270 // [W] Thin Corner Bottom Left
	 24,  9, //		271 // [W] Box Top Right Corner
	 25,  9, //		272 // [W] Box Top Left Corner
	 26,  9, //		273 // [W] Box Bottom Right Corner
	 27,  9, //		274 // [W] Box Bottom Left Corner
	 28,  9, //		275 // [W] Gate End Right
	 29,  9, //		276 // [W] Gate End Left
	  0, 10, //		277 // [W] Alt Thin Corner Top Left
	  1, 10, //		278 // [W] Alt Thin Corner Top Right
	  2, 10, //		279 // [W] Alt Thin Corner Bottom Left
	  3, 10, //		280 // [W] Alt Thin Corner Bottom Right
	  8, 10, //		281 // [W] Alt Thick Fork Wall R1
	  9, 10, //		282 // [W] Alt Thick Fork Wall R2
	 11, 10, //		283 // [W] Power Pellet
	 12, 10, //		284 // [W] Star
	 13, 10, //		285 // [W] Energizer
	  // Wall Tileset BLUE //
	 14, 10, //		286 // [B] Thick Top Right Corner
	 15, 10, //		287 // [B] Thick Top Left Corner
	 16, 10, //		288 // [B] Thick Right Wall
	 17, 10, //		289 // [B] Thick Left Wall
	 18, 10, //		290 // [B] Thick Bottom Right Corner
	 19, 10, //		291 // [B] Thick Bottom Left Corner
	 20, 10, //		292 // [B] Thick Fork Wall R1
	 21, 10, //		293 // [B] Thick Fork Wall R2
	 22, 10, //		294 // [B] Thick Fork Wall R3
	 23, 10, //		295 // [B] Thick Fork Wall R4
	 24, 10, //		296 // [B] Thick Top Wall
	 25, 10, //		297 // [B] Thick Bottom Wall
	 26, 10, //		298 // [B] Thin Top Wall
	 27, 10, //		299 // [B] Long Thin Corner Bottom Right
	 28, 10, //		300 // [B] Long Thin Corner Bottom Left
	 29, 10, //		301 // [B] Long Thin Corner Top Right
	  0, 11, //		302 // [B] Long Thin Corner Top Left
	  1, 11, //		303 // [B] Thin Bottom Wall
	  2, 11, //		304 // [B] Thin Corner Top Right
	  3, 11, //		305 // [B] Thin Corner Top Left
	  4, 11, //		306 // [B] Thin Left Wall
	  5, 11, //		307 // [B] Thin Right Wall
	  6, 11, //		308 // [B] Thin Corner Bottom Right
	  7, 11, //		309 // [B] Thin Corner Bottom Left
	  8, 11, //		310 // [B] Box Top Right Corner
	  9, 11, //		311 // [B] Box Top Left Corner
	 10, 11, //		312 // [B] Box Bottom Right Corner
	 11, 11, //		313 // [B] Box Bottom Left Corner
	 12, 11, //		314 // [B] Gate End Right
	 13, 11, //		315 // [B] Gate End Left
	 14, 11, //		316 // [B] Alt Thin Corner Top Left
	 15, 11, //		317 // [B] Alt Thin Corner Top Right
	 16, 11, //		318 // [B] Alt Thin Corner Bottom Left
	 17, 11, //		319 // [B] Alt Thin Corner Bottom Right
	 22, 11, //		320 // [B] Alt Thick Fork Wall R1
	 23, 11, //		321 // [B] Alt Thick Fork Wall R2
	 25, 11, //		322 // [B] Power Pellet
	 26, 11, //		323 // [B] Star
	 27, 11, //		324 // [B] Energizer
	  // Points //
	  0, 18, //		325 // 200
	  1, 18, //		326 // 400
	  2, 18, //		327 // 800
	  3, 18, //		328 // 1600
	  4, 18, //		329 // 100
	  5, 18, //		330 // 300
	  6, 18, //		331 // 500
	  7, 18, //		332 // 700
	  8, 18, //		333 // 0
	  // Pacman //
	  2, 20, //		334 // P1
	  0, 20, //		335 // P3 Right
	  1, 20, //		336 // P2 Right
	  0, 21, //		337 // P3 Left
	  1, 21, //		338 // P2 Left
	  0, 22, //		339 // P3 Up
	  1, 22, //		340 // P2 Up
	  0, 23, //		341 // P3 Down
	  1, 23, //		342 // P2 Down
	  3, 20, //		343 // Death 1
	  4, 20, //		344 // Death 2
	  5, 20, //		345 // Death 3
	  6, 20, //		346 // Death 4
	  7, 20, //		347 // Death 5
	  8, 20, //		348 // Death 6
	  9, 20, //		349 // Death 7
	 10, 20, //		350 // Death 8
	 11, 20, //		351 // Death 9
	 12, 20, //		352 // Death 10
	  // Fruit [Active] //
	  2, 23, //		353 // Cherry
	  3, 23, //		354 // Strawberry
	  4, 23, //		355 // Orange
	  5, 23, //		356 // Apple
	  6, 23, //		357 // Melon
	  7, 23, //		358 // Spaceship
	  8, 23, //		359 // Bell
	  9, 23, //		360 // Key
	  // Ghosts //
	  8, 24, //		361 // Scared Blue 1
	  9, 24, //		362 // Scared Blue 2
	 10, 24, //		363 // Scared White 1
	 11, 24, //		364 // Scared White 2
	  8, 25, //		365 // Eyes Right
	  9, 25, //		366 // Eye Left
	 10, 25, //		367 // Eyes Up
	 11, 25, //		368 // Eyes Down
	  0, 24, //		369 // Blinky R1
	  1, 24, //		370 // Blinky R2
	  2, 24, //		371 // Blinky L1
	  3, 24, //		372 // Blinky L2
	  4, 24, //		373 // Blinky U1
	  5, 24, //		374 // Blinky U2
	  6, 24, //		375 // Blinky D1
	  7, 24, //		376 // Blinky D2
	  0, 25, //		377 // Pinky R1
	  1, 25, //		378 // Pinky R2
	  2, 25, //		379 // Pinky L1
	  3, 25, //		380 // Pinky L2
	  4, 25, //		381 // Pinky U1
	  5, 25, //		382 // Pinky U2
	  6, 25, //		383 // Pinky D1
	  7, 25, //		384 // Pinky D2
	  0, 26, //		385 // Inky R1
	  1, 26, //		386 // Inky R2
	  2, 26, //		387 // Inky L1
	  3, 26, //		388 // Inky L2
	  4, 26, //		389 // Inky U1
	  5, 26, //		390 // Inky U2
	  6, 26, //		391 // Inky D1
	  7, 26, //		392 // Inky D2
      0, 27, //		393 // Clyde R1
	  1, 27, //		394 // Clyde R2
	  2, 27, //		395 // Clyde L1
	  3, 27, //		396 // Clyde L2
	  4, 27, //		397 // Clyde U1
	  5, 27, //		398 // Clyde U2
	  6, 27, //		399 // Clyde D1
	  7, 27, //		400 // Clyde D2
	  // Misc //
	 28, 11, //		401 // Gate
	 14,  7, //		402 // Empty
	 12, 12, //		403 // Pacman Live Top Left
	 13, 12, //		404 // Pacman Live Top Right
	 12, 13, //		405 // Pacman Live Bottom Left
	 13, 13, //		406 // Pacman Live Bottom Right
	  // Fruit [Inactive] //
	  0, 16, //		407 // Cherry Top Left
	  1, 16, //		408 // Cherry Top Right
	  0, 17, //		409 // Cherry Bottom Left
	  1, 17, //		410 // Cherry Bottom Right
	  2, 16, //		411 // Strawberry Top Left
	  3, 16, //		412 // Strawberry Top Right
	  2, 17, //		413 // Strawberry Bottom Left
	  3, 17, //		414 // Strawberry Bottom Right
	  4, 16, //		415 // Orange Top Left
	  5, 16, //		416 // Orange Top Right
	  4, 17, //		417 // Orange Bottom Left
	  5, 17, //		418 // Orange Bottom Right
	  6, 16, //		419 // Apple Top Left
	  7, 16, //		420 // Apple Top Right
	  6, 17, //		421 // Apple Bottom Left
	  7, 17, //		422 // Apple Bottom Right
	  8, 16, //		423 // Melon Top Left
	  9, 16, //		424 // Melon Top Right
	  8, 17, //		425 // Melon Bottom Left
	  9, 17, //		426 // Melon Bottom Right
	 10, 16, //		427 // Spaceship Top Left
	 11, 16, //		428 // Spaceship Top Right
	 10, 17, //		429 // Spaceship Bottom Left
	 11, 17, //		430 // Spaceship Bottom Right
	 12, 16, //		431 // Bell Top Left
	 13, 16, //		432 // Bell Top Right
	 12, 17, //		433 // Bell Bottom Left
	 13, 17, //		434 // Bell Bottom Right
	 14, 16, //		435 // Key Top Left
	 15, 16, //		436 // Key Top Right
	 14, 17, //		437 // Key Bottom Left
	 15, 17, //		438 // Key Bottom Right
	  // Misc 2 //
	 14,  7, //		439 // Empty MINI
	 13, 20, //		440 // Issue #1
	 9, 18, //		441 // Issue #2
	 0, 29, //		442 // Logo
};

int main() {
	SetConsoleTitle("Console Pacman");
	Main* Program = new Main;
	int StoredLevel = 1, StoredLives = 3, StoredScore = 0, StoredHighScore = Program->ReadFile(), StoredBoard[BH][BW];
	Program->SetWindowDimensions(29, 39);
	do {
		TransferBoardData(DefaultBoard, StoredBoard);
		TitleScreen* Menu = new TitleScreen;
		do {
			Menu->Draw();
			Menu->Input();
			Menu->Logic();
			while (GetConsoleWindow() != GetForegroundWindow()) {}
		} while (!Menu->MenuStatus());
		system("CLS");
		if (Menu->EXIT)
			Program->ExitProgram();
		delete Menu;
		bool PlayGame = true;
		while (PlayGame && !Program->ProgramStatus()) {
			Game* Pacman = new Game(StoredLevel, StoredLives, StoredScore, StoredHighScore, StoredBoard);
			while (!Pacman->GameStatus()) {
				Pacman->Draw();
				Pacman->Input();
				Pacman->Logic();
				while (GetConsoleWindow() != GetForegroundWindow()) {}
			}
			if (!Pacman->Death)
				StoredLevel = ++Pacman->Level;
			else
				StoredLevel = Pacman->Level;
			StoredLives = Pacman->Lives, StoredScore = Pacman->Score, StoredHighScore = Pacman->HighScore;
			if (Pacman->LevelUp)
				TransferBoardData(DefaultBoard, StoredBoard);
			else
				TransferBoardData(Pacman->Board, StoredBoard);
			if (Program->ReadFile() < Pacman->HighScore)
				Program->WriteFile(StoredHighScore);
			if (Pacman->EXIT)
				PlayGame = false;
			delete Pacman;
			system("CLS");
		}
		system("CLS");
	} while (!Program->ProgramStatus());
	delete Program;
	return EXIT_SUCCESS;
}

bool DrawSprite(int xpos, int ypos, int ID) {
	SelectObject(hdc, bmap);
	if (ID == 322 || ID == 439)
		BitBlt(console, xpos + 6, ypos + 6, 4, 4, hdc, SpriteData[ID - 1][0] * res + 6, SpriteData[ID - 1][1] * res +  6, SRCCOPY);
	else if ((ID >= 325 && ID <= 333) || ID == 441)
		BitBlt(console, xpos, ypos, res, 9, hdc, SpriteData[ID - 1][0] * res, SpriteData[ID - 1][1] * res, SRCCOPY);
	else if (ID == 442)
		BitBlt(console, xpos, ypos, 356, 81, hdc, SpriteData[ID - 1][0] * res, SpriteData[ID - 1][1] * res, SRCCOPY);
	else
		BitBlt(console, xpos, ypos, res, res, hdc, SpriteData[ID - 1][0] * res, SpriteData[ID - 1][1] * res, SRCCOPY);
	DeleteObject(bmap);
	return true;
}

bool KeyIsDown(char key, bool pressed, bool held) {
	int keyState = GetAsyncKeyState(static_cast<int>(key));
	return (pressed && (keyState & 1)) || (held && (keyState & 0xA000));
}

double GetTime() {
	return time_point_cast<nanoseconds>(high_resolution_clock::now()).time_since_epoch().count() / 1e9;
}

double GetTimeSince(double startTime) {
	return time_point_cast<nanoseconds>(high_resolution_clock::now()).time_since_epoch().count() / 1e9 - startTime;
}

double Wait(double waitTime) {
	double startTime = GetTime();

	while (waitTime > GetTimeSince(startTime)) {}

	return GetTimeSince(startTime + waitTime);
}

void ShowConsoleCursor(bool showFlag) {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(out, &cursorInfo);
}

void TransferBoardData(int a1[BH][BW], int a2[BH][BW]) {
	for (int y = 0; y < BH; y++) {
		for (int x = 0; x < BW; x++) {
			a2[y][x] = a1[y][x];
		}
	}
}

void TransferBoardData(const int a1[BH][BW], int a2[BH][BW]) {
	for (int y = 0; y < BH; y++) {
		for (int x = 0; x < BW; x++) {
			a2[y][x] = a1[y][x];
		}
	}
}
