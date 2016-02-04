 /***************************************************************************
  *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
  *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
  *                                                                         *
  *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
  *  Chastain, Michael Quan, and Mitchell Tse.                              *
  *                                                                         *
  *  In order to use any part of this Merc Diku Mud, you must comply with   *
  *  both the original Diku license in 'license.doc' as well the Merc       *
  *  license in 'license.txt'.  In particular, you may not remove either of *
  *  these copyright notices.                                               *
  *                                                                         *
  *  Much time and thought has gone into this software and you are          *
  *  benefitting.  We hope that you share your changes too.  What goes      *
  *  around, comes around.                                                  *
  ***************************************************************************/
 
 #if defined(macintosh)
 #include <types.h>
 #else
 #include <sys/types.h>
 #endif
 #include <stdio.h>
 #include <string.h>
 #include <time.h>
 #include "merc.h"

#define MAX_SLAY_TYPES 6


/*
 * Attribute bonus tables.
 */
const	struct	str_app_type	str_app		[36]		=
{
    { -5, -4,   0,  0 },  /* 0  */
    { -5, -4,   3,  1 },  /* 1  */
    { -3, -2,   3,  2 },
    { -3, -1,  10,  3 },  /* 3  */
    { -2, -1,  25,  4 },
    { -2, -1,  55,  5 },  /* 5  */
    { -1,  0,  80,  6 },
    { -1,  0,  90,  7 },
    {  0,  0, 100,  8 },
    {  0,  0, 100,  9 },
    {  0,  0, 115, 10 }, /* 10  */
    {  0,  0, 115, 11 },
    {  0,  0, 140, 12 },
    {  0,  0, 140, 13 }, /* 13  */
    {  0,  1, 170, 14 },
    {  1,  1, 170, 15 }, /* 15  */
    {  1,  2, 195, 16 },
    {  2,  3, 220, 22 },
    {  2,  4, 250, 25 }, /* 18  */
    {  3,  5, 400, 30 },
    {  3,  6, 500, 35 }, /* 20  */
    {  4,  7, 600, 40 },
    {  5,  7, 700, 45 },
    {  6,  8, 800, 50 },
    {  8, 10, 900, 55 },
    { 10, 12, 999, 60 },  /* 25   */
    { 12, 14, 1000, 160 },  /* 26   */
    { 14, 16, 2000, 260 },  /* 27   */
    { 16, 18, 3000, 360 },  /* 28   */
    { 18, 20, 4000, 460 },  /* 29   */
    { 20, 22, 5000, 560 },  /* 30   */
    { 22, 24, 6000, 660 },  /* 31   */
    { 24, 26, 7000, 760 },  /* 32   */
    { 26, 28, 8000, 860 }, /* 33   */
    { 28, 30, 9000, 960 },  /* 34   */
    { 30, 32, 10000, 1060 }  /* 35   */
};



const	struct	int_app_type	int_app		[36]		=
 {
     {  1 },	/*  0 */
     {  1 },	/*  1 */
     {  1 },
     {  1 },	/*  3 */
     {  1 },
     {  1 },	/*  5 */
     {  1 },
     {  1 },
     {  1 },
     {  1 },
     {  1 },	/* 10 */
     {  1 },
     {  1 },
     {  2 },
     {  2 },
     {  2 },	/* 15 */
     {  2 },
     {  3 },
     {  3 },	/* 18 */
     {  3 },
     {  4 },	/* 20 */
     {  4 },
     {  4 },
     {  6 },
     {  6 },
    {  5 },	/* 25 */
    {  6 },
    {  6 },
    {  6 },
    {  6 },
    {  6 },	/* 30 */
    {  6 },	
    {  6 },
    {  6 },
    {  6 },
    {  6 }	/* 35 */
};
 
 
 
 const	struct	wis_app_type	wis_app		[36]		=
 {
    { 0 },	/*  0 */
    { 0 },	/*  1 */
    { 0 },
    { 0 },	/*  3 */
    { 0 },
    { 1 },	/*  5 */
    { 1 },
    { 1 },
     { 1 },
     { 2 },
     { 2 },	/* 10 */
     { 2 },
     { 2 },
     { 2 },
     { 2 },
     { 3 },	/* 15 */
     { 3 },
     { 4 },
     { 5 },	/* 18 */
     { 5 },
     { 5 },	/* 20 */
     { 6 },
     { 6 },
     { 6 },
     { 6 },
     { 7 },	/* 25 */
    { 7 },	/* 26 */
    { 7 },	/* 27 */
    { 7 },	/* 28 */
    { 7 },	/* 29 */
    { 7 },	/* 30 */
    { 7 },	/* 31 */
    { 7 },	/* 32 */
    { 7 },	/* 33 */
    { 7 },	
    { 7 }	/* 35 */
};



const	struct	dex_app_type	dex_app		[36]		=
 {
     {   60 },   /* 0 */
     {   50 },   /* 1 */
     {   50 },
     {   40 },
     {   30 },
     {   20 },   /* 5 */
     {   10 },
     {    0 },
     {    0 },
     {    0 },
     {    0 },   /* 10 */
     {    0 },
     {    0 },
     {    0 },
     {    0 },
     { - 10 },   /* 15 */
     { - 15 },
     { - 20 },
     { - 30 },
     { - 40 },
     { - 50 },   /* 20 */
     { - 60 },
     { - 75 },
     { - 90 },
     { -105 },
    { -120 },    /* 25 */
    { -135 },
    { -150 },
    { -165 },
    { -180 },
    { -195 },	/* 30 */
    { -210 },
    { -225 },
    { -240 },
    { -255 },
    { -270 }	/* 35 */
};

 
 
 const	struct	con_app_type	con_app		[36]		=
{
    { -4, 20 },   /*  0 */
     { -3, 25 },   /*  1 */
     { -2, 30 },
     { -2, 35 },	  /*  3 */
     { -1, 40 },
     { -1, 45 },   /*  5 */
     { -1, 50 },
     {  0, 55 },
     {  0, 60 },
     {  0, 65 },
     {  0, 70 },   /* 10 */
     {  0, 75 },
     {  0, 80 },
     {  0, 85 },
     {  0, 88 },
     {  1, 90 },   /* 15 */
     {  2, 95 },
     {  2, 97 },
     {  3, 99 },   /* 18 */
     {  3, 99 },
     {  4, 99 },   /* 20 */
     {  4, 99 },
     {  5, 99 },
     {  6, 99 },
     {  7, 99 },
    {  8, 99 },    /* 25 */
    {  9, 99 },
    {  10, 99 },
    {  11, 99 },
    {  12, 99 },
    {  13, 99 },  /* 30 */
    {  14, 99 },
    {  15, 99 },
    {  16, 99 },
    {  17, 99 },
    {  18, 99 }  /* 35 */
};

 
 
 /*
  * Liquid properties.
  * Used in world.obj.
  */
 const	struct	liq_type	liq_table	[LIQ_MAX]	=
 {
     { "water",			"clear",	{  0, 1, 10 }	},  /*  0 */
     { "beer",			"amber",	{  3, 2,  5 }	},
     { "wine",			"rose",		{  5, 2,  5 }	},
     { "ale",			"brown",	{  2, 2,  5 }	},
     { "dark ale",		"dark",		{  1, 2,  5 }	},
 
     { "whisky",			"golden",	{  6, 1,  4 }	},  /*  5 */
     { "lemonade",		"pink",		{  0, 1,  8 }	},
     { "firebreather",		"boiling",	{ 10, 0,  0 }	},
     { "local specialty",	"everclear",	{  3, 3,  3 }	},
     { "slime mold juice",	"green",	{  0, 4, -8 }	},
 
     { "milk",			"white",	{  0, 3,  6 }	},  /* 10 */
     { "tea",			"tan",		{  0, 1,  6 }	},
     { "coffee",			"black",	{  0, 1,  6 }	},
     { "blood",			"red",		{  0, 0,  5 }	},
     { "salt water",		"clear",	{  0, 1, -2 }	},
 
     { "cola",			"cherry",	{  0, 1,  5 }	},   /* 15 */
    { "cum",			"white",	{  0, 1,  5 }	},   /* 16 */
     { "cunt juice",		"clear",	{  0, 1,  5 }	},   /* 17 */
     { "urine",			"yellow",	{  0, 1,  5 }	}   /* 18 */
 };
 
 
 
 /*
  * The skill and spell table.
  * Slot numbers must never be changed as they appear in #OBJECTS sections.
  */
 #define SLOT(n)	n
 
const	struct	skill_type	skill_table	[MAX_SKILL]	=
{

/*
 * Magic spells.
 */

    {
 	"reserved",		99,
 	0,			TAR_IGNORE,		POS_STANDING,
 	NULL,			SLOT( 0),	 0,	 0,
 	"",			""
     },
 
     {
 	"acid blast",		2,
 	spell_acid_blast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(70),	20,	12,
 	"acid blast",		"!Acid Blast!"
     },
     {
 	"armor",		1,
 	spell_armor,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
 	NULL,			SLOT( 1),	 5,	12,
 	"",			"You feel less protected."
     },
 
     {
	"bless",		1,
 	spell_bless,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT( 3),	 5,	12,
	"",			"You feel less righteous."
    },

     {
 	"blindness",		1,
 	spell_blindness,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	&gsn_blindness,		SLOT( 4),	 5,	12,
 	"",			"You can see again."
     },
 
     {
 	"burning hands",	2,
 	spell_burning_hands,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT( 5),	15,	12,
 	"burning hands",	"!Burning Hands!"
     },
 
     {
 	"call lightning",	2,
 	spell_call_lightning,	TAR_IGNORE,		POS_FIGHTING,
 	NULL,			SLOT( 6),	15,	12,
 	"lightning bolt",	"!Call Lightning!"
     },
 
    {
 	"cause critical",	2,
 	spell_cause_critical,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(63),	20,	12,
 	"spell",		"!Cause Critical!"
     },
 
    {
 	"cause light",		2,
 	spell_cause_light,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(62),	15,	12,
 	"spell",		"!Cause Light!"
     },
 
     {
 	"cause serious",	2,
 	spell_cause_serious,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(64),	17,	12,
 	"spell",		"!Cause Serious!"
     },
 
     {
 	"change sex",		2,
 	spell_change_sex,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(82),	15,	12,
 	"",			"Your body feels familiar again."
     },
 
     {
 	"charm person",		3,
 	spell_charm_person,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
 	&gsn_charm_person,	SLOT( 7),	 99,	12,
 	"",			"You feel more self-confident."
     },
 
     {
 	"chill touch",		2,
 	spell_chill_touch,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT( 8),	15,	12,
 	"chilling touch",	"You feel less cold."
     },
 
     {
 	"colour spray",		2,
 	spell_colour_spray,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(10),	15,	12,
 	"colour spray",		"!Colour Spray!"
     },
 
     {
 	"continual light",	1,
 	spell_continual_light,	TAR_IGNORE,		POS_STANDING,
 	NULL,			SLOT(57),	 7,	12,
 	"",			"!Continual Light!"
     },
 
     {
 	"control weather",	2,
 	spell_control_weather,	TAR_IGNORE,		POS_STANDING,
 	NULL,			SLOT(11),	25,	12,
 	"",			"!Control Weather!"
     },
 
     {
 	"create food",		1,
 	spell_create_food,	TAR_IGNORE,		POS_STANDING,
 	NULL,			SLOT(12),	 5,	12,
 	"",			"!Create Food!"
     },
 
     {
 	"create spring",	1,
 	spell_create_spring,	TAR_IGNORE,		POS_STANDING,
 	NULL,			SLOT(80),	20,	12,
 	"",			"!Create Spring!"
     },
 
     {
 	"create water",		2,
 	spell_create_water,	TAR_OBJ_INV,		POS_STANDING,
 	NULL,			SLOT(13),	 5,	12,
 	"",			"!Create Water!"
     },
 
     {
 	"cure blindness",	1,
 	spell_cure_blindness,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(14),	 5,	12,
 	"",			"!Cure Blindness!"
     },
 
     {
 	"cure critical",	2,
 	spell_cure_critical,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(15),	20,	12,
 	"",			"!Cure Critical!"
     },
 
     {
 	"cure light",		2,
 	spell_cure_light,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(16),	10,	12,
 	"",			"!Cure Light!"
     },
 
     {
 	"cure poison",		1,
 	spell_cure_poison,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
 	NULL,			SLOT(43),	 5,	12,
 	"",			"!Cure Poison!"
     },
 
     {
 	"cure serious",		2,
 	spell_cure_serious,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(61),	15,	12,
 	"",			"!Cure Serious!"
     },
 
     {
 	"curse",		1,
 	spell_curse,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
 	&gsn_curse,		SLOT(17),	20,	12,
 	"curse",		"The curse wears off."
     },
 
     {
 	"detect evil",		2,
 	spell_detect_evil,	TAR_CHAR_SELF,		POS_STANDING,
 	NULL,			SLOT(18),	 5,	12,
 	"",			"The red in your vision disappears."
     },
 
     {
 	"detect hidden",	1,
 	spell_detect_hidden,	TAR_CHAR_SELF,		POS_STANDING,
 	NULL,			SLOT(44),	 5,	12,
 	"",			"You feel less aware of your suroundings."
     },
 
     {
 	"detect invis",		1,
 	spell_detect_invis,	TAR_CHAR_SELF,		POS_STANDING,
 	NULL,			SLOT(19),	 5,	12,
 	"",			"You no longer see invisible objects."
     },
 
     {
 	"detect magic",		2,
 	spell_detect_magic,	TAR_CHAR_SELF,		POS_STANDING,
 	NULL,			SLOT(20),	 5,	12,
 	"",			"The detect magic wears off."
     },
 
     {
 	"detect poison",	2,
 	spell_detect_poison,	TAR_OBJ_INV,		POS_STANDING,
 	NULL,			SLOT(21),	 5,	12,
 	"",			"!Detect Poison!"
     },
 
     {
 	"dispel evil",		2,
 	spell_dispel_evil,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(22),	15,	12,
 	"dispel evil",		"!Dispel Evil!"
     },
 
     {
 	"dispel magic",		1,
 	spell_dispel_magic,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(59),	15,	12,
 	"",			"!Dispel Magic!"
     },
 
 
     {
 	"earthquake",		2,
 	spell_earthquake,	TAR_IGNORE,		POS_FIGHTING,
 	NULL,			SLOT(23),	15,	12,
 	"earthquake",		"!Earthquake!"
     },
 
     {
	"enchant weapon",	1,
	spell_enchant_weapon,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(24),	100,	24,
	"",			"!Enchant Weapon!"
    },

     {
 	"energy drain",		1,
 	spell_energy_drain,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(25),	35,	12,
 	"energy drain",		"!Energy Drain!"
     },
 
     {
 	"faerie fire",		2,
 	spell_faerie_fire,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(72),	 5,	12,
 	"faerie fire",		"The pink aura around you fades away."
     },
 
     {
 	"faerie fog",		2,
 	spell_faerie_fog,	TAR_IGNORE,		POS_STANDING,
 	NULL,			SLOT(73),	12,	12,
 	"faerie fog",		"!Faerie Fog!"
     },
 
     {
 	"fireball",		1,
 	spell_fireball,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(26),	15,	12,
 	"fireball",		"!Fireball!"
     },
 
     {
 	"flamestrike",		2,
 	spell_flamestrike,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(65),	20,	12,
 	"flamestrike",		"!Flamestrike!"
     },
 
     {
 	"fly",			1,
 	spell_fly,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
 	NULL,			SLOT(56),	10,	0,
 	"",			"You slowly float to the ground."
     },
 
     {
 	"gate",			7,
 	spell_gate,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(83),	50,	12,
 	"",			"!Gate!"
     },
 
     {
 	"giant strength",	1,
 	spell_giant_strength,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
 	NULL,			SLOT(39),	20,	12,
 	"",			"You feel weaker."
     },
 
     {
 	"harm",			1,
 	spell_harm,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(27),	35,	12,
 	"harm spell",		"!Harm!"
     },
 
     {
 	"heal",			1,
 	spell_heal,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(28),	50,	12,
 	"",			"!Heal!"
     },
 
    {
	"identify",		1,
	spell_identify,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(53),	12,	0,
	"",			"!Identify!"
    },

     {
 	"infravision",		1,
 	spell_infravision,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
 	NULL,			SLOT(77),	 5,	18,
 	"",			"You no longer see in the dark."
     },
 
     {
 	"invis",		1,
 	spell_invis,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
 	&gsn_invis,		SLOT(29),	 5,	12,
 	"",			"You are no longer invisible."
     },
 
     {
 	"know alignment",	2,
 	spell_know_alignment,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(58),	 9,	12,
 	"",			"!Know Alignment!"
     },
 
     {
 	"lightning bolt",	2,
 	spell_lightning_bolt,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(30),	15,	12,
 	"lightning bolt",	"!Lightning Bolt!"
     },
 
     {
 	"locate object",	1,
 	spell_locate_object,	TAR_IGNORE,		POS_STANDING,
 	NULL,			SLOT(31),	20,	18,
 	"",			"!Locate Object!"
     },
 
     {
 	"magic missile",	2,
 	spell_magic_missile,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(32),	15,	6,
 	"magic missile",	"!Magic Missile!"
     },
 
     {
 	"mass invis",		1,
 	spell_mass_invis,	TAR_IGNORE,		POS_STANDING,
 	&gsn_mass_invis,	SLOT(69),	20,	24,
 	"",			"!Mass Invis!"
     },
 
     {
 	"pass door",		1,
 	spell_pass_door,	TAR_CHAR_SELF,		POS_STANDING,
 	NULL,			SLOT(74),	20,	12,
 	"",			"You feel solid again."
     },
 
     {
 	"poison",		1,
 	spell_poison,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
 	&gsn_poison,		SLOT(33),	10,	12,
 	"poison",		"You feel less sick."
     },
 
     {
 	"protection",		1,
 	spell_protection,	TAR_CHAR_SELF,		POS_STANDING,
 	NULL,			SLOT(34),	 5,	12,
 	"",			"You feel less protected."
     },
 
     {
	"readaura",		12,
	spell_readaura,		TAR_CHAR_DEFENSIVE,	POS_MEDITATING,
	NULL,			SLOT(0),	1,	1,
	"",			"!readaura!"
    },


    {
 	"refresh",		2,
 	spell_refresh,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
 	NULL,			SLOT(81),	12,	18,
 	"refresh",		"!Refresh!"
     },
 
     {
 	"remove curse",		2,
 	spell_remove_curse,	TAR_IGNORE,		POS_STANDING,
 	NULL,			SLOT(35),	 5,	12,
 	"",			"!Remove Curse!"
     },
 
     {
 	"sanctuary",		1,
 	spell_sanctuary,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
 	NULL,			SLOT(36),	75,	12,
 	"",			"The white aura around your body fades."
     },
 
     {
 	"shield",		1,
 	spell_shield,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
 	NULL,			SLOT(67),	12,	18,
 	"",			"Your force shield shimmers then fades away."
     },
 
     {
 	"shocking grasp",	2,
 	spell_shocking_grasp,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(37),	15,	12,
 	"shocking grasp",	"!Shocking Grasp!"
     },
 
     {
 	"sleep",		4,
 	spell_sleep,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
 	&gsn_sleep,		SLOT(38),	99,	12,
 	"",			"You feel less tired."
     },
 
     {
 	"stone skin",		1,
 	spell_stone_skin,	TAR_CHAR_SELF,		POS_STANDING,
 	NULL,			SLOT(66),	12,	24,
 	"",			"Your skin feels soft again."
     },
 
     {
 	"summon",		2,
 	spell_summon,		TAR_IGNORE,		POS_STANDING,
 	NULL,			SLOT(40),	50,	12,
 	"",			"!Summon!"
     },
 
     {
 	"teleport",		2,
 	spell_teleport,		TAR_CHAR_SELF,		POS_FIGHTING,
 	NULL,	 		SLOT( 2),	35,	12,
 	"",			"!Teleport!"
     },
 
     {
 	"ventriloquate",	2,
 	spell_ventriloquate,	TAR_IGNORE,		POS_STANDING,
 	NULL,			SLOT(41),	 5,	12,
 	"",			"!Ventriloquate!"
     },
 
     {
 	"weaken",		2,
 	spell_weaken,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(68),	20,	12,
 	"spell",		"You feel stronger."
     },
 
     {
 	"word of recall",	9,
 	spell_word_of_recall,	TAR_CHAR_SELF,		POS_RESTING,
 	NULL,			SLOT(42),	 5,	12,
 	"",			"!Word of Recall!"
     },
 
 /*
  * Dragon breath
  */
     {
 	"acid breath",		1,
 	spell_acid_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(200),	 50,	 12,
 	"blast of acid",	"!Acid Breath!"
     },
 
     {
 	"fire breath",		1,
 	spell_fire_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(201),	 50,	 12,
 	"blast of flame",	"The smoke leaves your eyes."
     },
 
     {
 	"frost breath",		1,
 	spell_frost_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(202),	 50,	 12,
 	"blast of frost",	"!Frost Breath!"
     },
 
     {
 	"gas breath",		1,
 	spell_gas_breath,	TAR_IGNORE,		POS_FIGHTING,
 	NULL,			SLOT(203),	 300,	 12,
 	"blast of gas",		"!Gas Breath!"
     },
 
     {
 	"lightning breath",	1,
 	spell_lightning_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(204),	 50,	 12,
 	"blast of lightning",	"!Lightning Breath!"
     },
 
     {
         "Godbless",             12,
         spell_godbless,         TAR_CHAR_DEFENSIVE,     POS_STANDING,
         NULL,                   SLOT(205),      5,      12,
         "",                     "You feel God's blessing leave you."
     },
 
 /*
  * Fighter and thief skills.
  */
     {
 	"backstab",		1,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	&gsn_backstab,		SLOT( 0),	 0,	24,
 	"backstab",		"!Backstab!"
     },
 
     {
 	"garotte",		99,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	&gsn_garotte,		SLOT( 0),	0,	24,
 	"garotte",		"!Garotte!"
     },
 
      {
 	"spiderform",		99,
 	spell_null,		TAR_IGNORE,		POS_FIGHTING,
 	&gsn_spiderform,	SLOT(0),	0,	0,
 	"spidery arm",		"!Spiderform!"
     },
 
     {
 	"disarm",		1,
 	spell_null,		TAR_IGNORE,		POS_FIGHTING,
 	&gsn_disarm,		SLOT( 0),	 0,	24,
 	"",			"!Disarm!"
     },
 
     {
 	"hide",			1,
 	spell_null,		TAR_IGNORE,		POS_RESTING,
 	&gsn_hide,		SLOT( 0),	 0,	12,
 	"",			"!Hide!"
     },
 
     {
 	"hurl",			1,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	&gsn_hurl,		SLOT( 0),	 0,	24,
 	"",			"!Hurl!"
     },
     {
	"spirit kiss",		99,
	spell_spiritkiss,	TAR_IGNORE, 		POS_STANDING,
	NULL,			SLOT( 520),	15,	12,
	"",			"Your spirit blessing wears off."
    },
    {
	"jailwater",		99,
	spell_jailwater,	TAR_IGNORE, 		POS_STANDING,
	NULL,			SLOT( 521),	0,	0,
	"",			"Your spirit blessing wears off."
    },

    {
 	"kick",			1,
 	spell_null,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	&gsn_kick,		SLOT( 0),	 0,	 8,
 	"kick",			"!Kick!"
     },
 
     {
	"circle",		99,
	spell_null,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_circle,	SLOT(0),	0,	24,
	"swift circle attack",	"!Circle!"
    },

    {
 	"peek",			1,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	&gsn_peek,		SLOT( 0),	 0,	 0,
 	"",			"!Peek!"
     },
 
     {
 	"pick lock",		1,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	&gsn_pick_lock,		SLOT( 0),	 0,	12,
 	"",			"!Pick!"
     },
 
     {
 	"rescue",		1,
 	spell_null,		TAR_IGNORE,		POS_FIGHTING,
 	&gsn_rescue,		SLOT( 0),	 0,	12,
 	"",			"!Rescue!"
     },
 
     {
 	"sneak",		1,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	&gsn_sneak,		SLOT( 0),	 0,	12,
 	"",			"Your footsteps are no longer so quiet."
     },
 
     {
 	"steal",		1,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	&gsn_steal,		SLOT( 0),	 0,	24,
 	"",			"!Steal!"
     },
 
 /*
  * Spells for mega1.are from Glop/Erkenbrand.
  */
     {
 	"general purpose",	7,
 	spell_general_purpose,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(501),	0,	12,
 	"general purpose ammo",	"!General Purpose Ammo!"
     },
 
     {
 	"high explosive",	7,
 	spell_high_explosive,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(502),	0,	12,
 	"high explosive ammo",	"!High Explosive Ammo!"
     },
 
 /*
  * Spells added by KaVir.
  */
     {
 	"guardian",		12,
 	spell_guardian,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
 	NULL,			SLOT(600),	100,	12,
 	"",			"!Guardian!"
     },
 
     {
 	"soulblade",		1,
 	spell_soulblade,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
 	NULL,			SLOT(601),	100,	12,
 	"",			"!Soulblade!"
     },
 
     {
 	"mana",			1,
 	spell_mana,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
 	NULL,			SLOT(602),	 0,	12,
 	"",			"!Mana!"
     },
 
     {
 	"frenzy",		1,
 	spell_frenzy,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
 	NULL,			SLOT(603),	 20,	12,
 	"",			"Your bloodlust subsides."
     },
 
     {
 	"darkblessing",		1,
 	spell_darkblessing,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
 	NULL,			SLOT(604),	 20,	12,
 	"",			"You feel less wicked."
     },
 
     {
 	"portal",		1,
 	spell_portal,		TAR_IGNORE,		POS_STANDING,
 	NULL,			SLOT(604),	 50,	12,
 	"",			"!Portal!"
     },
 
    {
 	"energyflux",		2,
 	spell_energyflux,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
 	NULL,			SLOT(605),	 0,	12,
 	"",			"!EnergyFlux!"
     },
 
     {
 	"voodoo",		1,
 	spell_voodoo,		TAR_IGNORE,		POS_STANDING,
 	NULL,			SLOT(606),	 100,	12,
 	"",			"!Voodoo!"
     },
 
     {
 	"transport",		1,
 	spell_transport,	TAR_OBJ_INV,		POS_STANDING,
 	NULL,			SLOT(607),	12,	24,
 	"",			"!Transport!"
     },
 
     {
 	"regenerate",		1,
 	spell_regenerate,	TAR_OBJ_INV,		POS_STANDING,
 	NULL,			SLOT(608),	100,	12,
 	"",			"!Regenerate!"
     },
 
     {
 	"clot",			1,
 	spell_clot,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
 	NULL,			SLOT(609),	50,	12,
 	"",			"!Clot!"
     },
 
     {
 	"mend",			1,
 	spell_mend,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
 	NULL,			SLOT(610),	50,	12,
 	"",			"!Mend!"
     },
 
     {
 	"punch",		1,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	&gsn_punch,		SLOT( 0),	 0,	24,
 	"punch",		"!Punch!"
     },
 
     {
 	"elbow",		99,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	&gsn_elbow,		SLOT( 0),	 0,	24,
 	"elbow",		"!Elbow!"
     },
 
{
        "shards",              99,
        spell_null,            TAR_IGNORE,             POS_STANDING,
        &gsn_shards,          SLOT( 0),        0,     24,
        "shards",             "!Shards!"
    },

    {
        "magma",              99,
        spell_null,            TAR_IGNORE,             POS_STANDING,
        &gsn_spiket,          SLOT( 0),        0,     24,
        "blast of magma",             "!Magmablast!"
    },


    {
        "spiket",              99,
        spell_null,            TAR_IGNORE,             POS_STANDING,
        &gsn_spiket,          SLOT( 0),        0,     24,
        "tail spike",             "!Tailspike!"
    },

     {
        "venomt",              99,
        spell_null,            TAR_IGNORE,             POS_STANDING,
        &gsn_venomt,          SLOT( 0),        0,     24,
        "venomous tongue",             "!Venomt!"
    },

     {
 	"headbutt",		99,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	&gsn_headbutt,		SLOT( 0),	 0,	24,
 	"headbutt",		"!Headbutt!"
     },
 
    {
	"shiroken",		13,
	spell_null,		TAR_IGNORE,	POS_STANDING,
	&gsn_shiroken,		SLOT( 0),	0,	24,
	"shiroken",		"!Shiroken!"
    },

    {
	"blinky",		99,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_blinky,		SLOT( 0),	0,	24,
	"surprise attack",	"!BLINKY!"
    },

    {
	"inferno",		99,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_inferno,		SLOT( 0),	0,	24,
	"blazing flames",	"!Ooo Inferno!"
    },

    {
 	"fangs",		99,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	&gsn_fangs,		SLOT( 0),	0,	24,
 	"fangs",		"!Fangs!"
     },
 
     {
 	"buffet",		99,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	&gsn_buffet,		SLOT( 0),	0,	24,
 	"wing buffet",		"!Buffet!"
     },
 
     {
 	"sweep",		99,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	&gsn_sweep,		SLOT( 0),	 0,	24,
 	"tail sweep",		"!Sweep!"
     },
 
    {
	"backfist",		99,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_backfist,		SLOT( 0),	0,	24,
	"backfist",		"!Sweep!"
    },

    {
	"jumpkick",		99,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_jumpkick,		SLOT( 0),	0,	24,
	"jump kick",		"!JUMPKICK!"
    },

    {
	"spinkick",		99,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_spinkick,		SLOT( 0),	0,	24,
	"spin kick",		"!SPINKICK!"
    },

    {  "monksweep",		99,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_monksweep,		SLOT( 0),	0,	24,
	"foot sweep",		"!MONKSWEEEP!"
    },

    {
	"thrust kick",		99,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_thrustkick,	SLOT( 0),	0,	24,
	"thrust kick",		"!THRUST KICK!"
    },

    {
	"elbow",		99,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_elbow,		SLOT( 0),	0,	24,
	"elbow strike",		"!Elbow!"
    },

    {
	"shinkick",		99,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_shinkick,		SLOT( 0),	0,	24,
	"shin kick",		"lala"
    },

    {
	"palm strike",		99,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_palmstrike,	SLOT( 0),	0,	24,
	"palm strike",		"lala"
    },

    {
	"lightning kick",	99,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_lightningkick,	SLOT( 0),	0,	24,
	"lightning kick",	"lala"
    },

    {
	"tornado kick",		99,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_tornadokick,	SLOT( 0),	0,	24,
	"tornado kick",		"poop"
    },

    {
        "rfangs",                99,
         spell_null,             TAR_IGNORE,              POS_STANDING,
         &gsn_rfangs,            SLOT(0),          0,    24, 
         "razor fangs",           "!Rfangs!"
     },
     {
 	"knee",			99,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	&gsn_knee,		SLOT( 0),	 0,	24,
 	"knee strike",		"!Knee!"
     },
     { 
        "lightning slice",                 99,
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_lightningslash,              SLOT( 0),        0,     24,
        "lightning slice",          "!Lightning slice!"
     },

 
     {
 	"quest",		1,
 	spell_quest,		TAR_IGNORE,		POS_STANDING,
 	NULL,			SLOT(611),	 500,	12,
 	"",			"!Quest!"
     },
 
     {
 	"minor creation",	1,
 	spell_minor_creation,	TAR_IGNORE,		POS_STANDING,
 	NULL,			SLOT(612),	 500,	12,
 	"",			"!MinorCreation!"
     },
 
     {
 	"brew",			3,
 	spell_brew,		TAR_OBJ_INV,		POS_STANDING,
 	NULL,			SLOT(613),	 100,	12,
 	"",			"!Brew!"
     },
 
     {
 	"scribe",		3,
 	spell_scribe,		TAR_OBJ_INV,		POS_STANDING,
 	NULL,			SLOT(614),	 100,	12,
 	"",			"!Scribe!"
     },
 
     {
 	"carve",		3,
 	spell_carve,		TAR_OBJ_INV,		POS_STANDING,
 	NULL,			SLOT(615),	 200,	12,
 	"",			"!Carve!"
     },
 
     {
 	"engrave",		3,
 	spell_engrave,		TAR_OBJ_INV,		POS_STANDING,
 	NULL,			SLOT(616),	 300,	12,
 	"",			"!Engrave!"
     },
 
     {
 	"bake",			3,
 	spell_bake,		TAR_OBJ_INV,		POS_STANDING,
 	NULL,			SLOT(617),	 100,	12,
 	"",			"!Bake!"
     },
 
     {
 	"mount",		2,
 	spell_mount,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
 	NULL,			SLOT(618),	100,	12,
 	"",			"!Mount!"
     },
 
     {
 	"berserk",		2,
 	spell_null,		TAR_IGNORE,		POS_FIGHTING,
 	&gsn_berserk,		SLOT( 0),	 0,	24,
 	"",			"!Berserk!"
     },
 
     {
        "protection vs good",           1,
        spell_protection_vs_good,       TAR_CHAR_SELF,          POS_STANDING,
        NULL,                   SLOT(0),        5,     12,
        "",                     "You feel less protected."
     },

     {
 	"scan",			1,
 	spell_scan,		TAR_IGNORE,		POS_STANDING,
 	NULL,			SLOT(619),	6,	24,
 	"",			"!Scan!"
     },
 
     {
 	"repair",		2,
 	spell_repair,		TAR_IGNORE,		POS_STANDING,
 	NULL,			SLOT(620),	100,	24,
 	"",			"!Repair!"
     },
 
     {
 	"spellproof",		2,
 	spell_spellproof,	TAR_OBJ_INV,		POS_STANDING,
 	NULL,			SLOT(621),	50,	12,
 	"",			"!Spellproof!"
     },
 
     {
 	"preserve",		3,
 	spell_preserve,		TAR_OBJ_INV,		POS_STANDING,
 	NULL,			SLOT(550),	12,	24,
 	"",			"!Preserve!"
     },
 
     {
 	"track",		1,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	&gsn_track,		SLOT( 0),	 0,	0,
 	"",			"!Track!"
     },
 
 /*
    {
	"contraception",		3,
 	spell_contraception,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT( 3),	 5,	12,
	"",			"You feel less righteous."
    },
*/ 
 
     {
 	"purple sorcery",	99,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	NULL,			SLOT(554),	 100,	12,
 	"",			"The purple spell on you fades away."
     },
 
     {
 	"red sorcery",		99,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	NULL,			SLOT(555),	 100,	12,
 	"",			"The red spell on you fades away."
     },
 
     {
 	"blue sorcery",		99,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	NULL,			SLOT(556),	 100,	12,
 	"",			"The blue spell on you fades away."
     },
 
     {
 	"green sorcery",	99,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	NULL,			SLOT(557),	 100,	12,
 	"",			"The green spell on you fades away."
     },
 
     {
 	"yellow sorcery",	99,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	NULL,			SLOT(558),	 100,	12,
 	"",			"The yellow spell on you fades away."
     },
     {
 	"orange sorcery",	99,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	NULL,			SLOT(554),	 100,	12,
 	"",			"The orange spell on you fades away."
     },
     {
 	"indigo sorcery",	99,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	NULL,			SLOT(554),	 100,	12,
 	"",			"The indigo spell on you fades away."
     },
     {
 	"violet sorcery",	99,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	NULL,			SLOT(554),	 100,	12,
 	"",			"The violet spell on you fades away."
     },
 
     {
 	"chaos blast",		99,
 	spell_chaos_blast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
 	NULL,			SLOT(559),	20,	12,
 	"chaos blast",		"!Chaos Blast!"
     },
 
     {
 	"resistance",		1,
 	spell_resistance,	TAR_OBJ_INV,		POS_STANDING,
 	NULL,			SLOT(560),	50,	12,
 	"",			"!Resistance!"
     },
 
     {
 	"web",			99,
 	spell_web,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
 	&gsn_web,		SLOT(561),	 100,	12,
 	"",			"The web surrounding you breaks away."
     },
     {
 	"polymorph",		4,
 	spell_polymorph,	TAR_IGNORE,		POS_STANDING,
 	&gsn_polymorph,		SLOT(562),	50,	12,
 	"",			"You resume your normal form."
     },

 
     {
 	"find familiar",	4,
 	spell_find_familiar,	TAR_IGNORE,		POS_STANDING,
 	NULL,			SLOT(565),	100,	12,
 	"",			""
     },
 
     {
 	"improve",		3,
 	spell_improve,		TAR_OBJ_INV,		POS_STANDING,
 	NULL,			SLOT(566),	1500,	12,
 	"",			"!Improve!"
     },
 
     {
        "desanct",              13,
        spell_desanct,          TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(567),      1500,   12,
        "desanct",              "!Desanct!"
     },
     {
        "imp heal",              13,
        spell_imp_heal,         TAR_CHAR_DEFENSIVE,    POS_FIGHTING,
        NULL,                   SLOT(568),      1500,   12,
        "super heal",              "!Super Heal!" 
     },
     {
        "imp fireball",          13,
        spell_imp_fireball,     TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(569),      1500,   12,
        "super fireball",       "!Super Fireball!" 
     },
     {
        "imp faerie fire",      13,
        spell_imp_faerie_fire,    TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(570),      1500,   12,
        "super faerie fire",              "!Super Faerie Fire!" 
     },
     {
        "imp teleport",         13,
        spell_imp_teleport,     TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(571),      1500,   12,
        "super teleport",              "!Super Teleport!" 
     },
     { "spew",                   13,
       spell_spew,               TAR_IGNORE,        POS_STANDING,     
       &gsn_spew,                    SLOT(0),         10,     12,
       "spew of blood",         "!Spew!"
     },
     {
 	"infirmity",		13,
 	spell_infirmity,        	TAR_IGNORE,	POS_STANDING,
         NULL,		SLOT( 0),	 5,	12,
 	"infirmity",			"You feel stronger."
     },
     {
 	"shred",		13,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	&gsn_shred,		SLOT( 0),	 0,	24,
 	"shredding assault",		"!Shred!"
     },
 
 
     {
 	"cheapshot",		13,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	&gsn_cheapshot,		SLOT( 0),	 0,	24,
 	"cheapshot",		"!cheapsht!"
     },

     {
 	"multiplearms",		13,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	&gsn_multiplearms,		SLOT( 0),	 0,	24,
 	"multiplearms",		"!multiplearms!"
     },
    {
        "razorsharp claws",               13,
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_claws,            SLOT( 0),        0,     24,
        "razorsharp claws",         "razorsharp claws!"
     },
    {
        "bladespin",               13,
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_bladespin,            SLOT( 0),        0,     24,
        "bladespin",         "bladespin!"
     },
    {
        "hooves",               13,
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_hooves,            SLOT( 0),        0,     24,
        "hooves",         "hooves!"
     },
    {
        "fireball",               13,
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_fireball,            SLOT( 0),        0,     24,
        "fireball",         "fireball!"
     },
    {
        "lightning blast",               13,
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_lightning,            SLOT( 0),        0,     24,
        "lightning blast",         "lightning!"
     },

    {
        "aura of death",               13,
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_deathaura,            SLOT( 0),        0,     24,
        "aura of death",         "aura of death!"
     },

    {
        "heavenly aura",               13,
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_heavenlyaura,            SLOT( 0),        0,     24,
        "heavenly aura",         "heavenly aura!"
     },
    
    
    {
    "mageshield",  13,  spell_null,  TAR_IGNORE,  POS_STANDING,
    &gsn_mageshield,  SLOT( 0),  0,  24,  "mageshield",  "mageshield!"
    },
    {
    "wrath of god",  13,  spell_null,  TAR_IGNORE,  POS_STANDING,
    &gsn_wrathofgod,  SLOT( 0),  0,  24,  "wrath of god",  "wrath of god!"
    },
    {
    "darktendrils",  13,  spell_null,  TAR_IGNORE,  POS_STANDING,
    &gsn_darktendrils,  SLOT( 0),  0,  24,  "darktendrils",  "darktendrils!"
    },
    {
    "fiery aura",  13,  spell_null,  TAR_IGNORE,  POS_STANDING,
    &gsn_fiery,  SLOT( 0),  0,  24,  "fiery aura",  "fiery aura!"
    },
    {
    "chillhand",  13,  spell_null,  TAR_IGNORE,  POS_STANDING,
    &gsn_chillhand,  SLOT( 0),  0,  24,  "chillhand",  "chillhand!"
    },
    {
    "booming voice",  13,  spell_null,  TAR_IGNORE,  POS_STANDING,
    &gsn_booming,  SLOT( 0),  0,  24,  "booming voice",  "booming voice!"
    },

     {
 	"quills",		13,
 	spell_null,		TAR_IGNORE,		POS_STANDING,
 	&gsn_quills,		SLOT( 0),	 0,	24,
 	"razor quills",		"!quills!"
     },
 
     {
         "venom tongue",               13,
         spell_null,             TAR_IGNORE,             POS_STANDING,
         &gsn_venomtong,            SLOT( 0),        0,     24,
         "venom tongue",         "!VTong!"
     },  
 
     {
         "spiked tail",               13,
         spell_null,             TAR_IGNORE,             POS_STANDING,
         &gsn_spiketail,            SLOT( 0),        0,     24,
         "spiked tail",         "!stail!"
     },  

     {
	"venom spit",			99,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_stuntubes,		SLOT(0),	0,	0,
	"venom spit",		"!Venom Spit!"
     },
 
     {
	"laser",			99,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_laser,		SLOT(0),	0,	0,
	"laser",		"!LASER"
     },

     {
         "bad breath",               13,
         spell_null,             TAR_IGNORE,             POS_STANDING,
         &gsn_badbreath,            SLOT( 0),        0,     24,
         "bad breath",         "!BBreath!"
     },  
     {
        "poison stinger",                       99,
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_stinger,           SLOT(0),        0,      0,
        "poison stinger",               "!Poison Stinger"
     },
 
     {
         "hellfire",               13,
         spell_null,             TAR_IGNORE,             POS_STANDING,
         &gsn_hellfire,            SLOT( 0),        0,     24,
         "hellfire",         "!Hellfire!"
     },

     {
         "magma burst",               13,
         spell_null,             TAR_IGNORE,             POS_STANDING,
         &gsn_magma,            SLOT( 0),        0,     24,
         "magma burst",         "!Magma!"
     },     
 
     {
         "ice shards",               13,
         spell_null,             TAR_IGNORE,             POS_STANDING,
         &gsn_shards,            SLOT( 0),        0,     24,
         "ice shards",         "!IShards!"
     }, 
 
     {
 	"drowfire",	99,
 	spell_drowfire,		TAR_CHAR_OFFENSIVE,   POS_STANDING,
 	&gsn_drowfire,		SLOT(570),	100,	12,
 	"",			"The drowfire fades away."
 	},
 
     {
        "clay ball",                  99,
        spell_clay,              TAR_CHAR_OFFENSIVE,     POS_STANDING,
        NULL,               SLOT(0),       100,   12,
        "",                     "The clay on your body melts away."
     },
     {
        "group heal",                 1,
        spell_group_heal,             TAR_CHAR_DEFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(0),       50,     12,
        "",                     "!Group Heal!"
     },
     {
        "supreme attack",    99,   spell_null,   TAR_IGNORE,
        POS_STANDING, &gsn_supreme,  SLOT(0),  0,  24,
        "supreme attack",  "whatever"
     },
     {  
        "thrown pie",    99,   spell_null,   TAR_IGNORE,  
        POS_STANDING, &gsn_thrownpie,  SLOT(0),  0,  24,
        "thrown pie",  "whatever"
     },
     {
        "#C*#7SmAcK#C*#n",    99,   spell_null,   TAR_IGNORE,
        POS_STANDING, &gsn_smack,  SLOT(0),  0,  24,   
        "#C*#7SmAcK#C*#n",  "whatever"
     },
     {
        "#C*#7BaSh#C*#n",    99,   spell_null,   TAR_IGNORE,
        POS_STANDING, &gsn_bash,  SLOT(0),  0,  24,   
        "#C*#7BaSh#C*#n",  "whatever"
     },
     {
        "#C*#7ThWaCk#C*#n",    99,   spell_null,   TAR_IGNORE,
        POS_STANDING, &gsn_thwack,  SLOT(0),  0,  24,   
        "#C*#7ThWaCk#C*#n",  "whatever"
     },
     {
        "shimmering_plasma",    99,   spell_null,   TAR_IGNORE,
        POS_STANDING, &gsn_plasma,  SLOT(0),  0,  24,
        "shimmering plasma",  "whatever"
     },
     {
        "telekinetic burst",    99,   spell_null,   TAR_IGNORE,
        POS_STANDING, &gsn_telekinetic,  SLOT(0),  0,  24,
        "telekinetic burst",  "whatever"
     },
     {
        "burning hot mocha",    99,   spell_null,   TAR_IGNORE,
        POS_STANDING, &gsn_mocha,  SLOT(0),  0,  24,   
        "burning hot mocha",  "whatever"
     },
     {
        "potato grenade",    99,   spell_null,   TAR_IGNORE,
        POS_STANDING, &gsn_potato,  SLOT(0),  0,  24,
        "potato grenade",  "whatever"
     },
    {
    "breath",  13,  spell_null,  TAR_IGNORE,  POS_STANDING,
    &gsn_breath,  SLOT( 0),  0,  24,  "#RDeath Breath#n",  "breath!"
    },
     {
        "food frenzy",         99,
        spell_foodfrenzy,     TAR_CHAR_DEFENSIVE,     POS_STANDING,
        NULL,                   SLOT(604),       20,    12,
        "",                     "Your food frenzy subsides."
     },

    {
        "tentacle shower",               13,
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_tentacle,            SLOT( 0),        0,     24,
        "tentacle shower",         "!tentacle!"
     },

     {
        "visage",               13,
        spell_visage,           TAR_CHAR_DEFENSIVE,     POS_STANDING,
        NULL,                   SLOT(67),       12,     6,
        "",                     "Your horrid visage shimmers and fades."
     },

    {
 	"paradox",		13,
 	spell_null,		TAR_CHAR_SELF,	POS_STANDING,
 	&gsn_paradox,		SLOT( 90),	 5,	12,
 	NULL,			"Your paradox fades."
     },
 };
 
 
/* "slay_type" for use with new slay command. [lvl58+]
    The _msg strings are parsed through the "act" function, 
    instead of "send_to_char", so the following macros may be used:

    $t  Result is the 'arg1' argument interpreted as a string.

    $T  Result is the 'arg2' argument interpreted as a string.

    $n  Result is the name of 'ch'.  If 'ch' is not visible to the target
        character, result is the string 'someone'.

    $N  Result is the name of 'arg2' (considered as a victim).  If 'arg2'
	is not visible to the target character, result is the string
        'someone'.

    $e  Result is 'he', 'she', or 'it', depending on the sex of 'ch'.

    $E  Result is 'he', 'she', or 'it', depending on the sex of 'arg2'
        (considered as a victim).

    $m  Result is 'him', 'her', or 'it', depending on the sex of 'ch'.
    $M  Result is 'him', 'her', or 'it', depending on the sex of 'arg2'
        (considered as a victim).

    $s  Result is 'his', 'her', or 'its', depending on the sex of 'ch'.

    $S  Result is 'his', 'her', or 'its', depending on the sex of 'arg2'
        (considered as a victim).

    $p  Result is the short description of 'arg1' (considered as an
	object).
        If 'arg1' is invisible to the target character, result is the
	string 'something'.

    $P  Result is the short description of 'arg2' (considered as an
	object). If 'arg2' is invisible to the target character, result is the
	string 'something'.

    $d  Result is the first word in 'arg2', considered as a string.  If
	'arg2' is NULL, result is the string 'door'.  This is meant for
	extracting the name from a door's keyword list, but may be used in 
	general for other keyword lists.

*/

struct slay_type                slay_table      [MAX_SLAY_TYPES]        =
{
/* owner        title           char_msg        vict_msg        room_msg */
    {
    "Jester",
    "Straight Jacket"
    "Placing $N in a straight jacket and send them to the insane asylum",
    "$n places you in a straight jacket, and places you in an insane asylum!"
    "$n dresses $N in a straight jacket and takes them to an insane asylum!",
  },
  {
    "Kip",
    "Kip's balefire",
    "Siezing Saidin, you blast $N with a bolt of blazing balefire!",
    "Your vision goes negative as $n's searing bolt of balefire anhilates you!",    
    "$n's blast of balefire rips $N from the pattern!"
  },
  {
    "Kip",
    "Peanuts",
    "Chanting in the mystics tongue you conjure a hord up from the peanut gallery to kill $N!",
    "You hear $n chant in a strange tongue and all of a sudden a huge horde of peanuts rise up to kill you!",
    "$n chants in a strange tongue and all of a sudden you see a huge horde of peanuts rise up to kill $N!",
  },
  {
   "",
   "demon attack",
   "You gesture, and a slavering demon appears.  With a horrible grin, the foul creature turns on $N, who screams in panic before being eaten alive.",
   "$n gestures, and a slavering demon appears.  The foul creature turns on you\n\r with a horrible grin.   You scream in panic before being eaten alive.",
   "$n gestures, and a slavering demon appears.  With a horrible grin, the foul creature turns on $N, who screams in panic before being eaten alive."
  },
  {
   "",
   "Elmo",
   "You start the elmo giggling and give it to $N causing $M to shudder and convulse saying \"kill it, please make it stop!\" and slumps to the ground dead from the giggling.",
   "$n starts the elmo giggling and gives it to you causing you to shudder and convulse saying \"kill it, please make it stop!\" and slumps to the ground dead from the giggling.",
   "$n starts the elmo giggling and gives it to $N causing $M to shudder and convulse saying \"kill it, please make it stop!\" and slumps to the ground dead from the giggling."
  },
  {
   "",
   "CD",
   "You fling a razor sharp CD at $N slicing their head clean off.",
   "$n flings a razor sharp CD at you slicing your head clean off.",
   "$n flings a razor sharp CD at $N slicing their head clean off."
  }
};

/* -----------------------------------------------------------------------
The following snippet was written by Gary McNickle (dharvest) for
Rom 2.4 specific MUDs and is released into the public domain. My thanks to
the originators of Diku, and Rom, as well as to all those others who have
released code for this mud base.  Goes to show that the freeware idea can
actually work. ;)  In any case, all I ask is that you credit this code
properly, and perhaps drop me a line letting me know it's being used.

from: gary@dharvest.com
website: http://www.dharvest.com
or http://www.dharvest.com/resource.html (rom related)

Send any comments, flames, bug-reports, suggestions, requests, etc... 
to the above email address.
----------------------------------------------------------------------- */



/** Function: do_slay
  * Descr   : Slays (kills) a player, optionally sending one of several
  *           predefined "slay option" messages to those involved.
  * Returns : (void)
  * Syntax  : slay (who) [option]
  * Written : v1.0 12/97
  * Author  : Gary McNickle <gary@dharvest.com>
  */
void do_slay( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char type[MAX_INPUT_LENGTH];
    char who[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int i=0;
    bool found=FALSE;

    argument = one_argument(argument, who);
    argument = one_argument(argument, type);

    if ( !str_prefix(who, "list") || who == NULL )
    {
      send_to_char("\n\rSyntax: slay [who] <type>\n\r", ch);
      send_to_char("where type is one of the following...\n\r\n\r", ch);

      for ( i=0; i < MAX_SLAY_TYPES-1; i++ )

        if ( (slay_table[i].owner == NULL)               ||
             (!str_prefix(slay_table[i].owner,ch->name)  &&
              slay_table[i].title[0] != '\0') || strlen(slay_table[i].owner) < 2)
        {
           sprintf(buf, "%s\n\r", slay_table[i].title);
           send_to_char(buf, ch);
        }

       send_to_char(
          "\n\rTyping just 'slay <player>' will work too...\n\r",ch);
       return;
     }

     if ( ( victim = get_char_room( ch, who ) ) == NULL )
     {
        send_to_char( "They aren't here.\n\r", ch );
        return;
     } else

     if ( ch == victim )
     {
        send_to_char( "The creator has fixed his cannon against self slaughter!\n\r", ch);
        return;
     } else

     if ( !IS_NPC(victim) && get_trust( victim ) >= get_trust( ch ) )
     {
        send_to_char( "You failed.\n\r", ch );
        return;
     } else

     if ( type[0] == '\0' )
       {
	  act( "You brutely slay $N.",ch,NULL,victim,TO_CHAR);
	  act( "$n brutely slays you.",ch,NULL,victim,TO_VICT);
	  act( "$n brutely slays $N.",ch,NULL,victim,TO_NOTVICT);
          raw_kill(victim);
          return;
       }
     else

     for (i=0; i < MAX_SLAY_TYPES; i++)
     {
       if (
            !str_prefix(type, slay_table[i].title) &&
             ( slay_table[i].owner ==  NULL        ||
              !str_prefix(slay_table[i].owner, ch->name) )
          )
          {
             found=TRUE;
             sprintf(buf, "%s\n\r", slay_table[i].char_msg);
             act(buf, ch, NULL, victim, TO_CHAR );
             sprintf(buf, "%s\n\r", slay_table[i].vict_msg);
             act(buf, ch, NULL, victim, TO_VICT );
             sprintf(buf, "%s\n\r", slay_table[i].room_msg);
             act(buf, ch, NULL, victim, TO_NOTVICT );
             raw_kill(victim);
             return;
          }
       }

    if (!found)
      send_to_char("Slay type not defined. Type \"slay list\" for a complete listing of types available to you.\n\r", ch);

    return;
} /* end of func: "do_slay" */


