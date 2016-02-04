/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license._doc' as well the Merc       *
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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#if !defined(WIN32)
#include <unistd.h> /* unlink() */
#endif
#include "merc.h"
#include "garou.h"


bool	check_social	args( ( CHAR_DATA *ch, char *command,
			    char *argument ) );
bool	check_xsocial	args( ( CHAR_DATA *ch, char *command,
			    char *argument ) );
void	make_preg	args( ( CHAR_DATA *mother, CHAR_DATA *father ) );


int can_interpret args( (CHAR_DATA *ch, int cmd) );

int can_interpret (CHAR_DATA *ch, int cmd)
{
    bool cando = FALSE;

    if (ch->level == 12)
	cando = TRUE;

    if (cmd_table[cmd].level > get_trust(ch)) return 0;

    if (cmd_table[cmd].race == 0
        && cmd_table[cmd].discipline == 0)
	cando = TRUE;

    if (cmd_table[cmd].race > 0
         && cmd_table[cmd].discipline == 0
         && ch->class == cmd_table[cmd].race)
	cando = TRUE;

    if (cmd_table[cmd].discipline > 0
         && ch->power [ cmd_table[cmd].discipline ] >= cmd_table[cmd].disclevel)
 	cando = TRUE;

    if (!cando) return 0;


    if ( ch->position < cmd_table[cmd].position )
	return -1; 

    return 1;
}

void do_racecommands( CHAR_DATA *ch, char *argument )
{
    int i, cmd, foundpower = 0;
    char buf[MAX_STRING_LENGTH];
    bool displayed;

    if ( (ch->class == 0 && !IS_IMMORTAL(ch)) || IS_NPC(ch) )
    {
	send_to_char("You don't have any special powers.\n\r", ch);
	return;
    }
    send_to_char("#0<>==<>==<>==<>==<>==<>==<>==<>==<>    #GPowers    #0<>==<>==<>==<>==<>==<>==<>==<>==<>#n\n\r", ch);

    for (i = 0 ; i < MAX_DISCIPLINES ; i++)
    {
	displayed = FALSE;
	if (ch->power[i] > 0 && strlen(discipline[i]) > 0
		&& (ch->level < 7 || argument[0] == '\0' 
		|| !str_prefix(argument, discipline[i])))
	{
	    foundpower = 1;

		for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
		{
		    if (cmd_table[cmd].discipline == i
			&& ch->power[i] >= cmd_table[cmd].disclevel
			&& IS_CLASS(ch, cmd_table[cmd].race) )
		    {
			if (!displayed)
			{
			    displayed = TRUE;
	    		    sprintf(buf, " %15s : ", discipline[i]);
			    send_to_char(buf, ch);
			}			    
			send_to_char(cmd_table[cmd].name, ch);
			send_to_char(" ", ch);
		    }
	        }
                
		if (displayed) send_to_char("\n\r", ch);
	}
    }
/* OBEAH CRAP HAVE TO ADD MANUAL......ANYONE HAVE ANOTHER WAY.....PLEASE BY ALL MEANS CHANGE IT */

if (IS_CLASS(ch, CLASS_VAMPIRE))
{
if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_OBEA] > 1)
                            {
                            sprintf(buf, "           obeah :");
                            send_to_char(buf, ch);
                            }
if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_OBEA] > 6)
{
                            sprintf(buf, " (#Gpurify#n)");
                            send_to_char(buf, ch);
}
if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_OBEA] > 8)
{
                            sprintf(buf, " (#rBeast control#n)");
                            send_to_char(buf, ch);
}

if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_OBEA] > 9)
{
			    sprintf(buf, " (#7Movement of pure magic#n)");
                            send_to_char(buf, ch);
}


send_to_char("\n\r", ch);
}
    i = 0;
    displayed = FALSE;
    for (cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++)
    {
	if ( IS_CLASS(ch, cmd_table[cmd].race)
	    && cmd_table[cmd].discipline == 0)
	{
	    strcpy(buf, "");
	    if (i == 0)
	    {
		switch (ch->class)
		{
		    case 1 : sprintf(buf, " %15s : ", "Demon Kind");break;
                    case 2 : sprintf(buf, " %15s : ", "Magi");break;
                    case 4 : sprintf(buf, " %15s : ", "Innate"); break;
                    case 8 : sprintf(buf, " %15s : ", "Innate");break;
                    case 16: sprintf(buf, " %15s : ", "Weaponsmaster");break;
                    case 32: sprintf(buf, " %15s : ", "Drow Abilities");break;
                    case 64: sprintf(buf, " %15s : ", "Monk Abilities");break;
                    case 128:sprintf(buf, " %15s : ", "Ninja Abilities");break;
		    case 256:sprintf(buf, " %15s : ", "Lich Abilities");break;
                    case 512:sprintf(buf, " %15s : ", "Shapeshifter");break;
                    case 1024:sprintf(buf, " %15s : ", "Tanar'ri Powers");break;
                    case 2048:sprintf(buf, " %15s : ", "Angel Abilities");break;
                    case 4096:sprintf(buf, " %15s : ", "Knight Powers");break;
                    case 8192:sprintf(buf, " %15s : ", "Drider Powers");break;
                    case 16384:sprintf(buf, " %15s : ", "Drone Powers");break;
                    case 32768:sprintf(buf, " %15s : ", "Hobbit Powers");break;
                    case 65536:sprintf(buf, " %15s : ", "Fae Powers");break;
                    case 131072:sprintf(buf, " %15s : ", "Giant Powers");break;
		}
	    }
	    else if (i % 4 == 0)
	    {
		sprintf(buf, "\n\r                   ");
	    }
	    i++;

	    send_to_char(buf, ch);
	    send_to_char(cmd_table[cmd].name,ch);
	    send_to_char(" ",ch);
	    displayed = TRUE;
	    foundpower++;
	}
    }

    if (displayed) send_to_char("\n\r",ch);
  
    if (foundpower == 0)
    {
        send_to_char("\n\r You do not yet possess any powers.\n\r", ch);
    }
    send_to_char("#0<>==<>==<>==<>==<>==<>==<>==<>==<>==<>==<>==<>==<>==<>==<>==<>==<>==<>==<>==<>==<>#n\n\r", ch);
    return;
}
	
/*
 * Command logging types.
 */
#define LOG_NORMAL	0
#define LOG_ALWAYS	1
#define LOG_NEVER	2



/*
 * Log-all switch.
 */
char *  last_command        = NULL;
int     thread_status       = 0;
bool    arena_open	    = FALSE;
bool    arena_base          = FALSE;
bool	arenaopen	    = FALSE;
bool    ragnarok            = FALSE;
bool	fLogAll		    = FALSE;
bool	global_exp	    = FALSE;
bool 	global_qp	    = FALSE;
bool    extra_log	    = FALSE;
int     players_logged      = 0;
int     thread_count        = 0;
int     ragnarok_cost       = 3000;
int     ragnarok_on_timer   = 0;
int     ragnarok_safe_timer = 60;
int     pulse_arena         = 60;
int     pulse_doubleexp     = -1;
int     pulse_doubleqp      = -1;
int     players_decap       = 0;
int     players_gstolen     = 0;
int	iDelete		    = 0;
int     closed_sockets      = 0;
int     removed_sockets     = 0;
int     total_descriptors   = 0;
bool  check_disabled (const struct cmd_type *command);
DISABLED_DATA *disabled_first;
BAN_DATA *ban_list;

pthread_mutex_t        memory_mutex = PTHREAD_MUTEX_INITIALIZER;

#define END_MARKER    "END" /* for load_disabled() and save_disabled()*/


/*
 * Command table.
 */
const	struct	cmd_type	cmd_table	[] =
{
    /*
     * Common movement commands.
     */

    /* Name             Function    Min Position     Level  Log      Class,DiscLevel,DiscName*/

    { "north",  do_north, POS_STANDING,  0,  LOG_NORMAL, 0,0,0 },
    { "east",   do_east,  POS_STANDING,	 0,  LOG_NORMAL, 0,0,0 },
    { "south",  do_south, POS_STANDING,	 0,  LOG_NORMAL, 0,0,0 },
    { "west",   do_west,  POS_STANDING,	 0,  LOG_NORMAL, 0,0,0 },
    { "up",     do_up,    POS_STANDING,	 0,  LOG_NORMAL, 0,0,0 },
    { "down",   do_down,  POS_STANDING,	 0,  LOG_NORMAL, 0,0,0 },

    /*
     * Common other commands.
     * Placed here so one and two letter abbreviations work.
     */
    { "cast",		do_cast,				POS_FIGHTING,	0,  LOG_NORMAL, 0,0,0},
    { "call",		do_call,	POS_SLEEPING,	 0,  LOG_NORMAL, 0,0,0},
    { "consider",		do_consider,POS_SITTING,	 0,  LOG_NORMAL, 0,0,0},
    { "chi",            do_chi,POS_FIGHTING,3, LOG_NORMAL,64,0,0 },
    { "mesmerise",	do_command,	POS_SITTING,	 3,  LOG_ALWAYS, 8, DISC_VAMP_DOMI,2 },
    { "command",	do_command,	POS_SITTING,	 3, LOG_ALWAYS, 8,DISC_VAMP_DOMI,1},
    { "crack",		do_crack,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "diagnose",	do_diagnose,POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "dismount",	do_dismount,POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "enter",		do_enter,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "exits",		do_exits,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "get",		do_get,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "medit",    do_medit,       POS_DEAD,   10,  LOG_NORMAL, 0, 0,0 },
    { "inventory",	do_inventory,POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "kill",		do_kill,	POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "combatswitch",	do_combatswitch,	POS_FIGHTING, 0, LOG_NORMAL, 0,0,0},
    { "look",		do_look,	POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    { "ls",			do_look,	POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    { "meditate",		do_meditate,POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    { "mount",		do_mount,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "order",		do_order,	POS_SITTING,	 1,  LOG_ALWAYS, 0,0,0	},
    { "rest",		do_rest,	POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    { "reimb",           do_reimb,        POS_MEDITATING,  12,  LOG_NORMAL, 0,0,0  },   
    { "sit",		do_sit,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "stand",		do_stand,	POS_SLEEPING,	 0,  LOG_NORMAL, 0,0,0	},
    { "tell",		do_tell,	POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    { "whisper",		do_whisper,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "wield",		do_wear,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "wizhelp",		do_wizhelp,	POS_DEAD,	 	 4,  LOG_NORMAL, 0,0,0	},
    { "version",		do_version,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "linkdead",	do_linkdead, POS_DEAD, 7, LOG_NORMAL, 0,0,0},

    /*
     * Informational commands.
     */
    { "affects",          do_affects,       POS_DEAD,                0,  LOG_NORMAL, 0,0,0},
    { "areas",		do_areas,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0},
    { "commands",		do_commands,POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0},
    { "credits",		do_credits,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0},
    { "equipment",	do_equipment,POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0},
    { "examine",		do_examine,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0},
    { "help",		do_help,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0},
    { "idea",		do_idea,	POS_DEAD,	  	 0,  LOG_NORMAL, 0,0,0},
    { "report",		do_report,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0},
    { "reply",          do_reply,       POS_MEDITATING,  0,  LOG_NORMAL, 0,0,0 },
    { "autostance",	do_autostance,	POS_STANDING,	0, LOG_NORMAL,0,0,0},
    { "mastery",        do_mastery,     POS_STANDING,   3, LOG_ALWAYS,0,0,0},
    { "upgrade",        do_upgrade,    POS_STANDING,    3, LOG_ALWAYS,0,0,0},
    { "expcalc",        do_exp,  POS_FIGHTING,      1,  LOG_NORMAL,0,0,0},
    { "pkpowers",       do_pkpowers, POS_STANDING,    0,  LOG_NORMAL,0,0,0}, 
    { "gensteal",       do_gensteal, POS_STANDING,    3,  LOG_NORMAL,0,0,0},
    { "setstance",      do_setstance, POS_STANDING,   0,  LOG_NORMAL,0,0,0},
    { "mudstat",	do_mudstat,   POS_DEAD,       2,  LOG_NORMAL,0,0,0},
    { "level",            do_level,  POS_FIGHTING,      0, LOG_NORMAL,0,0,0},
    { "top",            do_top,  POS_FIGHTING,      0,  LOG_NORMAL, 0,0,0},
    { "topclear",       do_topclear, POS_DEAD,        12, LOG_NORMAL, 0,0,0},
    { "selfclass",     do_classself,  POS_STANDING,      3,  LOG_ALWAYS, 0,0,0},
    { "score",		do_score,	POS_DEAD,	 	 0,LOG_NORMAL, 0,0,0},
    { "spit",           do_spit,    POS_SITTING,       3,  LOG_NORMAL, 8,DISC_VAMP_QUIE,1},
    { "tongue",         do_tongue,  POS_FIGHTING,      3,  LOG_NORMAL, 8,DISC_VAMP_SERP,4},
    { "mindblast",      do_mindblast,POS_STANDING,     3,  LOG_NORMAL, 8,DISC_VAMP_PRES,2},
    { "stat",           do_stat,    POS_DEAD,          3,  LOG_NORMAL, 0,0,0},
    { "skill",		do_skill,	POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0 },
    { "spells",		do_spell,	POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0 },
    { "socials",		do_socials,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0 },
    { "time",		do_time,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0 },
    { "typo",		do_typo,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0 },
    { "who",		do_who,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0 },
    { "wizlist",		do_wizlist,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0 },
    { "xemot",		do_huh,	POS_DEAD,	 	 1,  LOG_NORMAL, 0,0,0 },
    { "xemote",		do_xemote,	POS_SITTING,	 1,  LOG_NORMAL, 0,0,0 },
    { "xsocial",		do_huh,	POS_DEAD,	 	 1,  LOG_NORMAL, 0,0,0 },
    { "xsocials",		do_xsocials,POS_DEAD,	 	 1,  LOG_NORMAL, 0,0,0 },
    { "group",          do_group,       POS_DEAD,                0,  LOG_NORMAL, 0,0,0  },

    /*
     * Configuration commands.
     */
    { "alignment",	do_alignment,POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "alias",          do_alias,  POS_STANDING,         2,  LOG_ALWAYS, 0,0,0 },
    { "config",		do_config,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "compres",      do_compres,    POS_DEAD,        0,  LOG_NORMAL, 0,0,0 },
    { "compress",      do_compress,    POS_DEAD,        0,  LOG_NORMAL, 0,0,0 },
    { "description",	do_description,POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	},
    { "password",		do_password,POS_DEAD,	 	 0,  LOG_NEVER, 0,0,0	},
    { "title",		do_title,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "ansi",		do_ansi,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "autoexit",		do_autoexit,POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "autoloot",		do_autoloot,POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "autosac",		do_autosac,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "blank",		do_blank,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "brief1",		 do_brief,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "brief2",          do_brief2,       POS_DEAD,                0,  LOG_NORMAL, 0,0,0  },
    { "brief3",          do_brief3,       POS_DEAD,                0,  LOG_NORMAL, 0,0,0  },
    { "brief4",          do_brief4,       POS_DEAD,                0,  LOG_NORMAL, 0,0,0  },
    { "cprompt",		do_cprompt,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "prompt",		do_prompt,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},

    /*
     * Communication commands.
     */
    { "flame",		do_flame,       POS_DEAD,                0,  LOG_NORMAL,0,0,0   },
    { "chat",		do_chat,	POS_DEAD,	 	 0,  LOG_NORMAL,0,0,0	},
    { ".",			do_chat,	POS_DEAD,2,LOG_NORMAL, 0,0,0	},
    { "clandisc",		do_clandisc,POS_SITTING,       3,  LOG_NORMAL, 0,0,0 },
    { "intro",		do_introduce,	POS_DEAD,	 3,LOG_NORMAL, 8,0,0, },
    { "intro",          do_introduce,   POS_DEAD,        3,LOG_NORMAL, 4,0,0, },
    { "emote",		do_xemote,	POS_SITTING,	 0,  LOG_NORMAL,0,0,0 },
    { ",",			do_xemote,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0},
    { "gtell",		do_gtell,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { ";",			do_gtell,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "howl",		do_howl,	POS_DEAD,	 	 1,  LOG_NORMAL, 0,0,0	},
    { "telepath",		do_telepath,	POS_DEAD,	 	 1,  LOG_NORMAL, 512,0,0	},
    { "music",		do_music,	POS_SLEEPING,	 2,  LOG_NORMAL,0,0,0 },
    { "note",		do_note,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "board",		do_board,	POS_DEAD,	0,	LOG_NORMAL,0,0,0},
    { "pose",		do_emote,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "quest",		do_quest,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "say",		do_say,		POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    { "'",		do_say,		POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    { "yell",		do_yell,	POS_SITTING,	 2,  LOG_NORMAL, 0,0,0 },
    { "powers", do_racecommands, POS_FIGHTING, 3, LOG_NORMAL,0,0,0 },

    /*
     * Object manipulation commands.
     */
    { "activate",		do_activate,POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "brandish",		do_brandish,POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "close",		do_close,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "draw",		do_draw,	POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "drink",		do_drink,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "drop",		do_drop,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "eat",		do_eat,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "empty",		do_empty,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "fill",		do_fill,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "give",		do_give,	POS_SITTING,	 0,  LOG_ALWAYS, 0,0,0	},
    { "gift",           do_gift,        POS_STANDING,    0,  LOG_NEVER, 0,0,0  },
    { "hold",		do_wear,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "lock",		do_lock,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "open",		do_open,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "pick",		do_pick,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "press",		do_press,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "pull",		do_pull,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "put",		do_put,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0 },
    { "quaff",		do_quaff,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "recite",		do_recite,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "remove",		do_remove,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "removealias",    do_removealias, POS_STANDING,    2,  LOG_NORMAL, 0,0,0  },
    { "sheath",		do_sheath,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "take",		do_get,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "throw",		do_throw,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "turn",		do_turn,	POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    { "twist",		do_twist,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "sacrifice",	do_sacrifice,	POS_SITTING,	 1,  LOG_NORMAL, 0,0,0	},
    { "unlock",		do_unlock,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "wear",		do_wear,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "voodoo",		do_voodoo,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "zap",		do_zap,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "junk",           do_sacrifice,POS_STANDING,     3,  LOG_NORMAL, 0,0,0 },

  	{ "stalk"	,do_stalk, POS_STANDING,	0, LOG_NORMAL,128,0,0 },


    { "ancestralpath",  do_hologramtransfer, POS_STANDING, 3, LOG_NORMAL, 16, 0, 0 },
    { "techniques",     do_bladespin,        POS_STANDING, 3, LOG_NORMAL, 16, 0, 0 },
    { "web",            do_web,              POS_FIGHTING, 3, LOG_NORMAL, 16, 0, 0 },
    { "focus",          do_focus,            POS_FIGHTING, 3, LOG_NORMAL, 16, 0 ,0 },
    { "slide",          do_slide,            POS_FIGHTING, 3, LOG_NORMAL, 16, 0, 0 },
    { "sidestep",       do_sidestep,         POS_FIGHTING, 3, LOG_NORMAL, 16, 0, 0 },
    { "block",          do_block,            POS_FIGHTING, 3, LOG_NORMAL, 16, 0, 0 },
    { "countermove",    do_countermove,      POS_FIGHTING, 3, LOG_NORMAL, 16, 0, 0 },
    { "martial",        do_martial,          POS_STANDING, 3, LOG_NORMAL, 16, 0, 0 },

   /*
     * Combat commands.
     */
    { "generation",     do_generation,POS_STANDING,   12,  LOG_ALWAYS,0,0,0 },
    { "class",          do_class,   POS_STANDING,     11, LOG_ALWAYS,0,0,0},
    { "backstab",		do_backstab,POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "berserk",		do_berserk,	POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "bs",			do_backstab,POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "disarm",		do_disarm,	POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "flee",		do_flee,	POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "fightstyle",	do_fightstyle,POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "hurl",		do_hurl,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "kick",		do_kick,	POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "servant",        do_servant, POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_DAIM,8 },
    { "punch",		do_punch,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "rescue",		do_rescue,	POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "stance",		do_stance,	POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},

   /* Demon pray - yep, kinda needed it here for some reason, damn tables. */

    { "pray",           do_pray,        POS_MEDITATING,  1,  LOG_NORMAL, 64,0,0 },

    /*
     * Lich commands.
     */
    
    { "lore",         do_lore,          POS_STANDING,   3,  LOG_NORMAL, 256,0,0},
    { "studylore",    do_studylore,     POS_STANDING,   3,  LOG_NORMAL, 256,0,0},
    { "lichtalk",     do_lichtalk,      POS_SLEEPING,   3,  LOG_NORMAL, 256,0,0},
    { "objectgate",   do_objectgate,    POS_STANDING,   3,  LOG_NORMAL, 256,0,0}, 
    { "fireball",     do_infernal,      POS_FIGHTING,   3,  LOG_NORMAL, 256,0,0},
    { "chaosmagic",   do_chaosmagic,    POS_FIGHTING,   3,  LOG_NORMAL, 256,0,0}, 
    { "chaossurge",   do_chaossurge,    POS_FIGHTING,   3,  LOG_NORMAL, 256,0,0},
    { "chaosshield",  do_chaosshield,   POS_STANDING,   3,  LOG_NORMAL, 256,0,0},
    { "golemsummon",  do_summongolem,   POS_FIGHTING,   3,  LOG_NORMAL, 256,0,0},
    { "planartravel", do_planartravel,  POS_STANDING,   3,  LOG_NORMAL, 256,0,0},
    { "truesight",    do_truesight,     POS_STANDING,   3,  LOG_NORMAL, 256,0,0},
    { "planarstorm",  do_planarstorm,   POS_FIGHTING,   3,  LOG_NORMAL, 256,0,0},
    { "readaura",     do_readaura,      POS_FIGHTING,   3,  LOG_NORMAL, 256,0,0},
    { "powertransfer",do_powertransfer, POS_STANDING,   3,  LOG_NORMAL, 256,0,0},
    { "polarity",     do_polarity,      POS_FIGHTING,   3,  LOG_NORMAL, 256,0,0},
    { "chillhand",    do_chillhand,     POS_FIGHTING,   3,  LOG_NORMAL, 256,0,0},
    { "creepingdoom", do_creepingdoom,  POS_FIGHTING,   3,  LOG_NORMAL, 256,0,0},
    { "painwreck",    do_painwreck,     POS_FIGHTING,   3,  LOG_NORMAL, 256,0,0},
    { "earthswallow", do_earthswallow,  POS_STANDING,   3,  LOG_NORMAL, 256,0,0},
    { "licharmor",    do_licharmor,     POS_STANDING,   3,  LOG_NORMAL, 256,0,0},
    { "soulsuck",     do_soulsuck,      POS_FIGHTING,   3,  LOG_NORMAL, 256,0,0},
    { "zombie",       do_zombie,        POS_STANDING,   3,  LOG_NORMAL, 256,0,0},
    { "pentagram",    do_pentagram,     POS_STANDING,   3,  LOG_NORMAL, 256,0,0},
    { "planeshift",   do_planeshift,    POS_STANDING,   3,  LOG_NORMAL, 256,0,0},

    { "channels",               do_channels,POS_DEAD,            0,  LOG_NORMAL, 0,0,0  },


    /*
     * Tanar'ri commands.
     */

    { "earthquake",	do_earthquake,  POS_FIGHTING, 3, LOG_NORMAL, 1024,0,0},
    { "tornado",	do_tornado,	POS_FIGHTING, 3, LOG_NORMAL, 1024,0,0},
    { "infernal",	do_infernal,    POS_FIGHTING, 3, LOG_NORMAL, 1024,0,0},
    { "bloodsacrifice", do_bloodsac,    POS_STANDING, 3, LOG_NORMAL, 1024,0,0},
    { "enmity",		do_enmity,	POS_FIGHTING, 3, LOG_NORMAL, 1024,0,0},
    { "enrage",		do_enrage,	POS_FIGHTING, 3, LOG_NORMAL, 1024,0,0},
    { "booming",        do_booming,     POS_FIGHTING, 3, LOG_NORMAL, 1024,0,0},
    { "truesight",      do_truesight,   POS_STANDING, 3, LOG_NORMAL, 1024,0,0},
    { "web",		do_web,		POS_FIGHTING, 3, LOG_NORMAL, 1024,0,0},
    { "claws",		do_claws,	POS_STANDING, 3, LOG_NORMAL, 1024,0,0},
    { "chaosgate",	do_chaosgate,	POS_STANDING, 3, LOG_NORMAL, 1024,0,0},
    { "fury",		do_fury,	POS_STANDING, 3, LOG_NORMAL, 1024,0,0},
    { "tantalk",	do_tantalk,	POS_DEAD,     3, LOG_NORMAL, 1024,0,0},
    { "taneq",		do_taneq,	POS_STANDING, 3, LOG_NORMAL, 1024,0,0},
    { "bloodrite",      do_unholyrite,  POS_STANDING, 3, LOG_NORMAL, 1024,0,0},
    { "lavablast",	do_lavablast,	POS_FIGHTING, 3, LOG_NORMAL, 1024,0,0},
    { "chaossurge",	do_chaossurge,	POS_FIGHTING, 3, LOG_NORMAL, 1024,0,0},

    /*
     * Undead Knight commands
     */

    { "knighttalk",     do_knighttalk, POS_DEAD,   3,  LOG_NORMAL, 4096,0,0},
    { "knightarmor",  do_knightarmor, POS_STANDING, 3,  LOG_NORMAL,4096,0,0},
    { "gain",		do_gain,	POS_STANDING, 3, LOG_NORMAL,4096,0,0},
    { "weaponpractice", do_weaponpractice, POS_STANDING, 3, LOG_NORMAL, 4096,0,0},
    { "powerword",      do_powerword, POS_FIGHTING, 3,	LOG_NORMAL, 4096,0,0},
    { "aura",		do_aura,	POS_STANDING, 3, LOG_NORMAL, 4096,0,0},
    { "command",	do_command,   POS_STANDING,  3, LOG_ALWAYS, 4096,0,0},
    { "unholysight",	do_truesight,  POS_STANDING, 3, LOG_NORMAL, 4096,0,0},
    { "bloodrite",	do_unholyrite, POS_STANDING, 3, LOG_NORMAL, 4096,0,0},
    { "ride",		do_ride,	POS_STANDING, 3, LOG_NORMAL, 4096,0,0},
    { "soulsuck",     do_soulsuck,      POS_FIGHTING,   3,  LOG_NORMAL, 4096,0,0},


    /*
     * Angel Commands.
     */

    { "prayer",        do_angeltalk,     POS_MEDITATING,3, LOG_NORMAL, 2048,0,0 },
    { "gpeace",        do_gpeace,        POS_STANDING,  3, LOG_NORMAL, 2048,0,0 },
    { "innerpeace",    do_innerpeace,    POS_STANDING,  3, LOG_NORMAL, 2048,0,0 },
    { "houseofgod",    do_houseofgod,    POS_STANDING,  3, LOG_NORMAL, 2048,0,0 },
    { "angelicaura",   do_angelicaura,   POS_FIGHTING,  3, LOG_NORMAL, 2048,0,0 },
    { "gbanish",       do_gbanish,       POS_FIGHTING,  3, LOG_NORMAL, 2048,0,0 },
    { "harmony",       do_harmony,       POS_FIGHTING,  3, LOG_NORMAL, 2048,0,0 },
    { "gsenses",       do_gsenses,       POS_STANDING,  3, LOG_NORMAL, 2048,0,0 },
    { "gfavor",        do_gfavor,        POS_STANDING,  3, LOG_NORMAL, 2048,0,0 },
    { "forgiveness",   do_forgivness,    POS_FIGHTING,  3, LOG_NORMAL, 2048,0,0 },
    { "martyr",        do_martyr,        POS_STANDING,  3, LOG_NORMAL, 2048,0,0 },
    { "swoop",         do_swoop,         POS_STANDING,  3, LOG_NORMAL, 2048,0,0 },
    { "awings",        do_awings,        POS_STANDING,  3, LOG_NORMAL, 2048,0,0 },
    { "halo",          do_halo,          POS_STANDING,  3, LOG_NORMAL, 2048,0,0 },
    { "sinsofthepast", do_sinsofthepast, POS_FIGHTING,  3, LOG_NORMAL, 2048,0,0 },
    { "eyeforaneye",   do_eyeforaneye,   POS_FIGHTING,  3, LOG_NORMAL, 2048,0,0 },
    { "angelicarmor",  do_angelicarmor,  POS_STANDING,  3, LOG_NORMAL, 2048,0,0 },
    { "touchofgod",    do_touchofgod,    POS_FIGHTING,  3, LOG_NORMAL, 2048,0,0 },
    { "spiritform",    do_spiritform,    POS_STANDING,  3, LOG_NORMAL, 2048,0,0 },

    /*
     * Shapeshifter Commands.
     */
    
    { "shift",       do_shift,    POS_FIGHTING,   3,  LOG_NORMAL, 512,0,0 },
    { "formlearn",   do_formlearn, POS_STANDING,  3,  LOG_NORMAL, 512,0,0 },
    { "camouflage",  do_camouflage, POS_STANDING, 3,  LOG_NORMAL, 512,0,0 },
    { "mask",        do_mask,	POS_STANDING,     3,  LOG_NORMAL, 512,0,0 },
    { "truesight",   do_truesight, POS_STANDING,  3,  LOG_NORMAL, 512,0,0 },
    { "shapeshift",  do_shapeshift, POS_STANDING, 3,  LOG_NORMAL, 512,0,0 },
    { "hatform",     do_hatform,   POS_STANDING,  3,  LOG_NORMAL, 512,0,0 },
    { "mistwalk",    do_mistwalk,  POS_STANDING,  3,  LOG_NORMAL, 512,0,0 },
    { "shapearmor",  do_shapearmor, POS_STANDING, 3,  LOG_NORMAL, 512,0,0 },
    { "roar",        do_shaperoar, POS_FIGHTING,  3,  LOG_NORMAL, 512,0,0 },
    { "charge",      do_charge,   POS_FIGHTING,   3,  LOG_NORMAL, 512,0,0 },
    { "fblink",      do_faerieblink, POS_FIGHTING,3,  LOG_NORMAL, 512,0,0 },
    { "stomp",       do_stomp,  POS_FIGHTING,     3,  LOG_NORMAL, 512,0,0 },
    { "faeriecurse", do_faeriecurse, POS_FIGHTING,3,  LOG_NORMAL, 512,0,0 },
    { "phase",       do_phase,   POS_FIGHTING,    3,  LOG_NORMAL, 512,0,0 },
    { "breath",      do_breath, POS_FIGHTING,     3,  LOG_NORMAL, 512,0,0 },

    /*
     * Mage Commands.
     */

    { "magics",       do_magics,    POS_STANDING,   3,  LOG_NORMAL, 2,0,0 },
    { "teleport",     do_teleport,  POS_STANDING,   3,  LOG_NORMAL, 2,0,0 },
    { "invoke",       do_invoke,    POS_STANDING,   3,  LOG_NORMAL, 2,0,0 },
    { "chant",        do_chant,     POS_MEDITATING, 3,  LOG_NORMAL, 258,0,0 },
    { "magearmor",    do_magearmor, POS_MEDITATING, 3,  LOG_NORMAL, 2,0,0 },
    { "objectgate",   do_objectgate,POS_STANDING,   3,  LOG_NORMAL, 2,0,0 },
    { "discharge",    do_discharge,	POS_FIGHTING,	  3,  LOG_NORMAL, 2,0,0 },
    { "scry",	      do_scry,      POS_FIGHTING,   3,  LOG_NORMAL, 2,0,0 },
    { "reveal",       do_reveal,    POS_STANDING,   3,  LOG_NORMAL, 2,0,0 },
    { "chaosmagic",   do_chaosmagic, POS_FIGHTING,  3,  LOG_NORMAL, 2,0,0 },

                    /* PK POWERS */

    { "eaglesight",       do_pkscry,    POS_STANDING,   3,  LOG_NORMAL,0,0,0},
    { "silverpath",       do_pkportal,  POS_STANDING,   3,  LOG_NORMAL,0,0,0},
    { "lifesense",	  do_pkaura,	POS_STANDING,   3,  LOG_NORMAL,0,0,0},
    { "sanctum",	  do_pkheal,    POS_STANDING,   3,  LOG_NORMAL,0,0,0},
    { "calltoarms",	  do_pkcall,	POS_FIGHTING,   3,  LOG_NORMAL,0,0,0},
    { "ironmind",	  do_pkmind,    POS_STANDING,   3,  LOG_NORMAL,0,0,0},
    { "objectscry",       do_pkobjscry, POS_STANDING,   3,  LOG_NORMAL,0,0,0},
    { "crystalsight",     do_pkvision,  POS_STANDING,   3,  LOG_NORMAL,0,0,0},


/*                      ****|  V-A-M-P-I-R-E-S  |****          */

// Melpominee
    { "scream",		do_scream,    POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_MELP, 1 },

// Daimoinon
    { "guardian",	do_guardian,  POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_DAIM, 1 },
    { "fear",		do_fear,      POS_FIGHTING,  3,  LOG_NORMAL, 8, DISC_VAMP_DAIM, 2 },
    { "portal",		do_gate,      POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_DAIM, 3 },
// Lvl 4 - curse ---
    { "vtwist",		do_vtwist,     POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_DAIM, 5 },
    { "wither", do_wither, POS_FIGHTING, 3, LOG_NORMAL, 4,DISC_WERE_HAWK,7 },

// Thanatosis
    { "hagswrinkles",	do_hagswrinkles,POS_STANDING, 3, LOG_NORMAL, 8, DISC_VAMP_THAN, 1 },
    { "putrefaction",   do_rot,	      POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_THAN, 2 },
/*    { "ashes",		do_ashes,     POS_STANDING,  3,
LOG_NORMAL, 8, DISC_VAMP_THAN, 3 }, */
    { "withering",	do_withering, POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_THAN, 4 },
    { "drainlife",	do_drain,     POS_FIGHTING,  3,  LOG_NORMAL, 8, DISC_VAMP_THAN, 5 },

// Necromancy
   
    { "preserve",	do_preserve,  POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_NECR, 2 },
    { "spiritgate",	do_spiritgate,POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_NECR, 3 },
    { "spiritguard",	do_spiritguard,POS_STANDING, 3,  LOG_NORMAL, 8, DISC_VAMP_NECR, 4 },


// Auspex
    { "truesight",      do_truesight, POS_STANDING,  3,  LOG_NORMAL, 4, DISC_WERE_HAWK, 3 },
    { "truesight",	do_truesight, POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_AUSP, 1 },
    { "readaura",	do_readaura,  POS_FIGHTING,  3,  LOG_NORMAL, 8, DISC_VAMP_AUSP, 2 },
    { "scry",		do_scry,      POS_FIGHTING,  3,  LOG_NORMAL, 8, DISC_VAMP_AUSP, 3 },
    { "astralwalk",	do_astralwalk,POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_AUSP, 4 },
    { "unveil",		do_unveil,    POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_AUSP, 5 },

// Obfuscate
    { "vanish",		do_vanish,    POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_OBFU, 1 },
    { "mask",		do_mask,      POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_OBFU, 2 },
    { "shield",		do_shield,    POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_OBFU, 3 },
    { "shield",         do_shield,    POS_STANDING,  3,  LOG_NORMAL, 4, DISC_WERE_OWL, 2 },

// Chimerstry
    { "mirror",		do_mirror,    POS_STANDING,  3, LOG_NORMAL, 8, DISC_VAMP_CHIM, 1 },
    { "formillusion",	do_formillusion,POS_STANDING, 3, LOG_NORMAL,8, DISC_VAMP_CHIM, 2 },
    { "controlclone",	do_control,   POS_STANDING,  3, LOG_NORMAL, 8, DISC_VAMP_CHIM, 4 },

    /*
     * Miscellaneous commands.
     */
    { "accep",		do_huh,		POS_STANDING,	 2,  LOG_NORMAL, 0,0,0	},
    { "accept",		do_accept,	POS_STANDING,	 2,  LOG_NORMAL, 0,0,0	},
    { "artifact",	do_artifact,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "birth",		do_birth,	POS_STANDING,	 1,  LOG_NORMAL, 0,0,0	},
    { "blindfold",	do_blindfold,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "breaku",		do_huh,		POS_STANDING,	 2,  LOG_NORMAL, 0,0,0	},
    { "breakup",	do_breakup,	POS_STANDING,	 2,  LOG_NORMAL, 0,0,0	},
    { "claim",		do_claim,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "complete",	do_complete,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "consen",		do_huh,		POS_STANDING,	 1,  LOG_NORMAL, 0,0,0	},
    { "consent",	do_consent,	POS_STANDING,	 1, LOG_NORMAL, 0,0,0	},
    { "finger",		do_finger,	POS_SITTING,	 1,  LOG_NORMAL, 0,0,0	},
    { "follow",		do_follow,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "gag",		do_gag,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "godsfavor",      do_godsfavor,POS_DEAD,         3,  LOG_NORMAL, 64,0,0 },
    { "hide",		do_hide,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "home",		do_home,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "hunt",		do_hunt,	POS_STANDING,	 1,  LOG_NORMAL, 0,0,0	},
    { "locate",		do_locate,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "practice",		do_practice,POS_SLEEPING,	 0,  LOG_NORMAL, 0,0,0	},
    { "propos",		do_huh,	POS_STANDING,	 2,  LOG_NORMAL, 0,0,0	},
    { "propose",		do_propose,	POS_STANDING,	 2,  LOG_NORMAL, 0,0,0	},
    { "qui",		do_qui,	POS_DEAD,		 0,  LOG_NORMAL, 0,0,0	},
    { "quit",		do_quit,	POS_SLEEPING,	 0,  LOG_NORMAL, 0,0,0	},
    { "recall",		do_recall,	POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "escape",		do_escape,	POS_DEAD,	 	 3,  LOG_NORMAL, 0,0,0	},
    { "/",			do_recall,	POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "recharge",		do_recharge,POS_STANDING,	 0,LOG_NORMAL, 0,0,0	},
    { "rent",		do_rent,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "safe",		do_safe,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "save",		do_save,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "sleep",		do_sleep,	POS_SLEEPING,	 0,  LOG_NORMAL, 0,0,0	},
    { "smother",		do_smother,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "sneak",		do_sneak,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "scan",		do_scan,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "spy",		do_spy,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "steal",		do_steal,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "notravel",		do_notravel,POS_DEAD,	 	 1,LOG_NORMAL, 0,0,0 },
    { "nosummon",		do_nosummon,POS_DEAD,	 	 1,  LOG_NORMAL, 0,0,0	},
    { "embrace",        do_embrace, POS_STANDING,      3,  LOG_NORMAL, 8,0,0 },
    { "diablerise",	do_diablerise, POS_STANDING, 3, LOG_NORMAL, 8,0,0 },
    { "assassinate",    do_assassinate,POS_STANDING,   3,  LOG_NORMAL, 8,DISC_VAMP_QUIE,4 },
    { "tendrils",           do_tendrils,POS_FIGHTING,      3,  LOG_NORMAL, 8,DISC_VAMP_SERP,4 },
    { "lamprey",        do_lamprey, POS_FIGHTING,      3,  LOG_NORMAL, 8,DISC_VAMP_OBTE,5 },
    { "entrance",       do_entrance,POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_PRES,3 },
    { "fleshcraft",     do_fleshcraft,POS_STANDING,    3,  LOG_NORMAL, 8,DISC_VAMP_VICI,2 },
    { "zombie",         do_zombie,  POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_NECR,5 },
    { "baal",           do_baal,    POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_DOMI,5 },
    { "dragonform",     do_dragonform,POS_STANDING,    3,  LOG_NORMAL, 8,DISC_VAMP_VICI,4 },
    { "spew",           do_spew,    POS_FIGHTING,      3,  LOG_NORMAL, 8,DISC_VAMP_THAU,6 },
    { "bloodwater",     do_bloodwater,POS_FIGHTING,    3,  LOG_NORMAL, 8,DISC_VAMP_NECR,5 },
    { "gourge",         do_gourge,  POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_THAU,8 },
    { "roar",		do_roar,    POS_FIGHTING,      3,  LOG_NORMAL, 4,DISC_WERE_BEAR,6 },
    { "jawlock",	do_jawlock, POS_STANDING,      3,  LOG_NORMAL, 4,DISC_WERE_RAPT,8 },
    { "perception",	do_perception,POS_STANDING,    3,  LOG_NORMAL, 4,DISC_WERE_RAPT,3 },
    { "skin",		do_skin,    POS_STANDING,      3,  LOG_NORMAL, 4,DISC_WERE_BEAR,7 },
    { "rend",		do_rend,    POS_STANDING,      3,  LOG_NORMAL, 4,DISC_WERE_BOAR,7 },
    { "slam",		do_slam,    POS_STANDING,      3,  LOG_NORMAL, 4,DISC_WERE_BEAR,8 },
    { "shred",		do_shred,   POS_STANDING,      3,  LOG_NORMAL, 4,DISC_WERE_RAPT,7 },
    { "diablerize",	do_diablerise, POS_STANDING,   3,  LOG_NORMAL, 8,0,0},
    { "taste",          do_taste,   POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_THAU,1 },
    { "pigeon",         do_pigeon,  POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_ANIM,3 },
    { "bloodagony",     do_bloodagony,POS_STANDING,    3,  LOG_NORMAL, 8,DISC_VAMP_QUIE,3 },
    { "tie",		do_tie,	POS_STANDING,	 3,  LOG_NORMAL, 0,0,0	},
    { "token",		do_token,	POS_STANDING,	 2,  LOG_NORMAL, 0,0,0	},
    { "track",		do_track,	POS_STANDING,	 1,  LOG_NORMAL, 0,0,0	},
    { "train",		do_train,   POS_STANDING,	 1,  LOG_NORMAL, 0,0,0	},
    { "unpolymorph",	do_unpolymorph,POS_STANDING,	 4,  LOG_NORMAL, 0,0,0 },
    { "untie",		do_untie,	POS_STANDING,	 1,  LOG_NORMAL, 0,0,0	},
    { "visible",		do_visible,	POS_SLEEPING,	 1,  LOG_NORMAL, 0,0,0 },
    { "wake",		do_wake,	POS_SLEEPING,	 0,  LOG_NORMAL, 0,0,0	},
    { "where",		do_where,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},

/* need it before forge */


     { "contraception", do_contraception, POS_DEAD,  1,      LOG_NORMAL, 0,0,0},


 /*
  * Monk
  */
    { "chaoshands",	do_chands,	POS_STANDING,	 3,  LOG_NORMAL, 64,0,0 },
    { "monkarmor",	do_monkarmor,	POS_STANDING,	 3,  LOG_NORMAL, 64,0,0 },
    { "ghold",		do_ghold,	POS_STANDING,    3,  LOG_NORMAL, 64,0,0 },
    { "godsheal",	do_godsheal,	POS_FIGHTING,	 3,  LOG_NORMAL, 64,0,0 },
    { "mantra",		do_mantra,	POS_STANDING,	 3, LOG_NORMAL, 64,0,0 },
    { "guide",		do_guide,	POS_STANDING,	 3,  LOG_NORMAL, 64,0,0 },
    { "wrathofgod",  do_wrathofgod, POS_FIGHTING, 3, LOG_NORMAL, 64,0,0},
    { "cloak",		do_cloak,	POS_STANDING,      3,  LOG_NORMAL, 64,0,0 },
    { "prayofages",	do_prayofages,POS_STANDING,    3,  LOG_NORMAL, 64,0,0 },
    { "sacredinvis",	do_sacredinvis, POS_STANDING,	 3,  LOG_NORMAL, 64,0,0 },
    { "flaminghands",   do_flaminghands,POS_STANDING,  3,  LOG_NORMAL, 64,0,0 },
    { "darkblaze",	do_darkblaze, POS_FIGHTING,	3, LOG_NORMAL, 64,0,0},
    { "adamantium",	do_adamantium,POS_STANDING,    3,  LOG_NORMAL, 64,0,0 },
    { "godseye",		do_godseye,	POS_STANDING,	 3,  LOG_NORMAL, 64,0,0 },
    { "celestial",	do_celestial,POS_STANDING,	 3,  LOG_NORMAL, 64,0,0 },
    { "steelskin",	do_steelskin,POS_STANDING,	 3,  LOG_NORMAL, 64,0,0 },
    { "godsbless",	do_godsbless,POS_FIGHTING,	 3,  LOG_NORMAL, 64,0,0 },
    { "thrustkick",	do_thrustkick,POS_FIGHTING,	3, LOG_NORMAL, 64,0,0 },
    { "spinkick",	do_spinkick,POS_FIGHTING,3,LOG_NORMAL,64,0,0 },
    { "backfist",	do_backfist,POS_FIGHTING,3,LOG_NORMAL,64,0,0 },
    { "elbow",		do_elbow,POS_FIGHTING,3,LOG_NORMAL,64,0,0 },
    { "sweep",		do_sweep,POS_FIGHTING,3,LOG_NORMAL,64,0,0 },
    { "reverse",	do_reverse,POS_FIGHTING,3,LOG_NORMAL,64,0,0 },
    { "knee",		do_knee,POS_FIGHTING,3,LOG_NORMAL,64,0,0 },
    { "shinkick",	do_shinkick,POS_FIGHTING,3,LOG_NORMAL,64,0,0 },
    { "palmstrike",	do_palmstrike,POS_FIGHTING,3,LOG_NORMAL,64,0,0 },
    { "spiritpower",	do_spiritpower,POS_FIGHTING,3,LOG_NORMAL,64,0,0},
    { "deathtouch",	do_deathtouch,POS_STANDING,3,LOG_NORMAL,64,0,0 },
    { "relax",		do_relax,POS_FIGHTING,3,LOG_NORMAL,64,0,0},
    { "monktalk",	do_monktalk,POS_DEAD,3,LOG_NORMAL,64,0,0 },
    { "disciplines",    do_disciplines,POS_STANDING,   3,  LOG_NORMAL, 0,0,0 },
    { "research",		do_research,POS_STANDING,      3,  LOG_NORMAL, 0,0,0 },


/*
 * Garou
 */
// Ahroun
    { "razorclaws",	do_razorclaws, POS_FIGHTING, 3, LOG_NORMAL, 4,DISC_WERE_WOLF,4 },

// Homid
//  Persuasion, automatic
    { "staredown",	do_staredown, POS_FIGHTING, 3, LOG_NORMAL, 4,DISC_WERE_OWL,5 },
    { "disquiet",	do_disquiet, POS_FIGHTING, 3 , LOG_NORMAL, 4,DISC_WERE_OWL,6 }, 
    { "reshape",	do_reshape,  POS_STANDING,  3,  LOG_NORMAL, 4,DISC_WERE_OWL,7 },
    { "cocoon",		do_cocoon,   POS_FIGHTING,  3,  LOG_NORMAL, 4,DISC_WERE_OWL, 8 },

// Metis
    { "quills",	do_quills, POS_FIGHTING, 3, LOG_NORMAL, 4,DISC_WERE_HAWK,5 },
    { "burrow", do_burrow, POS_FIGHTING, 3, LOG_NORMAL, 4,DISC_WERE_HAWK,6 },
    { "nightsight",do_nightsight,POS_FIGHTING,3,LOG_NORMAL,4,DISC_WERE_HAWK,1 },

    { "learn",		do_learn, POS_FIGHTING, 3, LOG_NORMAL, 64,0,0 },

    /*
     * Vampire and werewolf commands.
     */
    { "vamparmor",	do_vampirearmor, POS_STANDING, 3, LOG_NORMAL, 8, 0, 0 },
    { "bloodwall",	do_bloodwall,POS_STANDING,	 3,  LOG_NORMAL, 8,DISC_VAMP_DAIM,2 },
    { "conceal",     do_conceal,  POS_STANDING,  3,  LOG_NORMAL, 8,DISC_VAMP_OBFU,5 },
    { "sharpen",	do_sharpen,  POS_STANDING,     3,  LOG_NORMAL, 8,DISC_VAMP_QUIE,7 },
    { "purify",      do_purification, POS_STANDING, 3, LOG_NORMAL, 0, 0, 0 },

     /* Protean */ /* healing has to go after drow heal */
    /* Obtene */
    { "grab",		do_grab,	POS_STANDING,    	 3,  LOG_NORMAL, 8,DISC_VAMP_OBTE,8 },
    { "shadowgaze",	do_shadowgaze,POS_STANDING,    3,  LOG_NORMAL, 8,DISC_VAMP_OBTE,10 },

   /* Luna Powers */
    { "flameclaws",	do_flameclaws,  POS_STANDING,  3,  LOG_NORMAL, 4,DISC_WERE_LUNA,1 },
    { "motherstouch",	do_motherstouch,POS_FIGHTING,  3,  LOG_NORMAL, 4,DISC_WERE_LUNA,3 },
    { "gmotherstouch",  do_gmotherstouch,POS_FIGHTING, 3,  LOG_NORMAL, 4,DISC_WERE_LUNA,4 },
    { "sclaws",		do_sclaws,	POS_STANDING,      3,  LOG_NORMAL, 4,DISC_WERE_LUNA,5 },
    { "moonbeam",	do_moonbeam,POS_FIGHTING,      3,  LOG_NORMAL, 4,DISC_WERE_LUNA,8 },
    { "moonarmour",	do_moonarmour,POS_STANDING,    3,  LOG_NORMAL, 4,DISC_WERE_LUNA,2 },
    { "moongate",	do_moongate,POS_STANDING,      3,  LOG_NORMAL, 4,DISC_WERE_LUNA,6 },
   /* No more luna Powers */

    { "tribe",		do_tribe,	POS_STANDING,	3,LOG_NORMAL,4,0,0},
    { "info",		do_info,	POS_DEAD,      12,LOG_NORMAL,0,0,0 },
    { "demongate",	do_dgate,      POS_FIGHTING,   3, LOG_NORMAL, 1,0,0 },
    { "devour",		do_devour,     POS_STANDING,   3, LOG_NORMAL, 4,DISC_WERE_RAPT,5 },
    { "frostbreath",    do_frostbreath,POS_FIGHTING,   3, LOG_NORMAL, CLASS_DEMON,DISC_DAEM_GELU,2 },
    { "tick",		do_tick,       POS_DEAD,      12, LOG_ALWAYS, 0,0,0},
    { "resetarea",	do_resetarea,  POS_DEAD,      10, LOG_ALWAYS, 0,0,0},
    { "graft",		do_graft,      POS_STANDING,   3, LOG_NORMAL, CLASS_DEMON,DISC_DAEM_ATTA,5 },
    { "rage",		do_rage,       POS_FIGHTING,   3, LOG_NORMAL, CLASS_DEMON,DISC_DAEM_ATTA,3 },
    { "calm",		do_calm,       POS_STANDING,   3, LOG_NORMAL, CLASS_DEMON,DISC_DAEM_ATTA,4 },
    { "vamptalk",       do_vamptalk,   POS_DEAD,       1, LOG_NORMAL, 8,0,0 },      
    { "obtain",          do_obtain,  POS_STANDING,   3,  LOG_NORMAL, 0,0,0 },
    { "warps",          do_warps,  POS_STANDING,   3,  LOG_NORMAL, 0,0,0 },   
    { "claws",		do_claws,	POS_FIGHTING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_ATTA,1 },
    { "claws",          do_claws,       POS_FIGHTING,  3,  LOG_NORMAL, 4,DISC_WERE_WOLF,1 },
    { "fangs",		do_fangs,	POS_FIGHTING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_ATTA,2 },
    { "fangs",          do_fangs,       POS_FIGHTING,  3,  LOG_NORMAL, 4,DISC_WERE_WOLF,2 },
    { "horns",		do_horns,	POS_FIGHTING,  3, LOG_NORMAL, 1,DISC_DAEM_ATTA,4 },
    { "blink",		do_blink,	POS_FIGHTING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_ATTA,7 },
    { "inferno",	do_dinferno,	POS_DEAD,      3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_HELL, 3 },
    { "tail",		do_tail,	POS_FIGHTING,  3,  LOG_NORMAL, 1,0,0 },

/* Vamp */
    { "binferno",	do_inferno,	POS_STANDING,  3,  LOG_NORMAL, 1,DISC_VAMP_DAIM, 6 },
/* Vamp ^^^^ */

    { "immolate",	do_immolate,    POS_STANDING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_HELL, 2 },
    { "daemonseed",	do_seed,	POS_STANDING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_HELL, 7 },
    { "hellfire",	do_hellfire,	POS_STANDING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_HELL, 8 },
    { "ban",		do_ban,	POS_DEAD,		 12,  LOG_ALWAYS,0,0,0	},
    { "transfer",       do_transfer,    POS_DEAD,      7,  LOG_NORMAL,0,0,0 },
    { "runeeq",		do_runeeq,	POS_STANDING,    2,  LOG_NORMAL,0,0,0 },
    { "afk",      	do_afk,         POS_STANDING,    3,  LOG_NORMAL,0,0,0 },
    { "hedit",		do_hedit,	POS_STANDING, 10, LOG_NORMAL,0,0,0 },
    { "freeze",		do_freeze,      POS_DEAD,     9, LOG_ALWAYS,0,0,0 },  
    { "bitchslap", 	do_freeze,   POS_DEAD,     9, LOG_ALWAYS,0,0,0 },
    { "unnerve",	do_unnerve,     POS_FIGHTING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_DISC,1 },
    { "freezeweapon",	do_wfreeze,     POS_STANDING,  3, LOG_NORMAL, CLASS_DEMON,DISC_DAEM_GELU, 1 },
    { "chaosportal",	do_chaosport,   POS_FIGHTING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_DISC, 4 },
    { "caust",		do_caust,       POS_STANDING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_CORR, 4 },
    { "gust",		do_gust,	POS_STANDING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_GELU, 7 },
    { "entomb",		do_entomb,      POS_STANDING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_GELU, 6 },
    { "evileye",	do_evileye,     POS_STANDING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_DISC, 2 },
    { "leech",		do_leech,	POS_FIGHTING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_NETH,4 },
    { "deathsense",	do_deathsense,  POS_FIGHTING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_NETH, 2 },
    { "prefix",		do_prefix,	POS_DEAD,      10,  LOG_NORMAL, 0,0,0 },

    /* bugaboo lala Dunkirk Shit Lala mmm POOP Daemon Stuff */


    /* Start of OLC Shit. Hmm */
    { "hset",		do_hset,        POS_DEAD,      12, LOG_ALWAYS, 0,0,0 },
    { "hlist",		do_hlist,	POS_DEAD,      12, LOG_ALWAYS, 0,0,0 },

    { "talons",         do_talons,   POS_FIGHTING,     3,  LOG_NORMAL, 4,DISC_WERE_RAPT,10 },
    { "bonemod",		do_bonemod,	POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_VICI,3 },
    { "cauldron",		do_cauldron,POS_FIGHTING,      3, LOG_NORMAL, 8,DISC_VAMP_THAU,2 },
    { "flamehands",	do_flamehands,POS_FIGHTING,	 3,  LOG_NORMAL, 8,DISC_VAMP_PROT,5 },
   
    { "summon",		do_summon,	POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_PRES,4 },
    { "shroud",		do_shroud,	POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_OBTE,1 },
    { "share",		do_share,	POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_ANIM,4 },
    { "frenzy",		do_frenzy,	POS_FIGHTING,	 3,  LOG_NORMAL, 8,DISC_VAMP_ANIM,5 },
    { "far",		do_far,	POS_STANDING,	 3,  LOG_NORMAL, 8,DISC_VAMP_PRES,9 },
    { "awe",            do_awe,     POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_PRES,1 },


    { "forge",		do_forge,	POS_STANDING,  1,LOG_NORMAL,0,0,0 },
    { "forget",         do_forget,  POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_DOMI,8 },
    { "acid",           do_acid,    POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_DOMI,4 },
    { "vsilence",       do_death,   POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_QUIE,5 },
    { "flash",          do_flash,   POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_QUIE,9 },
    { "tide",   	do_tide,    POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_THAU,5 },
    { "coil",           do_coil,    POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_SERP,8 },
    { "infirmity",	do_infirmity,POS_FIGHTING,	 3,  LOG_NORMAL, 8,DISC_VAMP_QUIE,2 },
    { "klaive",         do_klaive,  POS_STANDING,      3,  LOG_NORMAL, 4,0,0 },
    { "calm",		do_calm,	POS_STANDING,	 3,  LOG_NORMAL, 4,DISC_WERE_WOLF,3	},
    { "change",		do_change,	POS_STANDING,	 3,  LOG_NORMAL, 8,DISC_VAMP_PROT,3	},
    { "katana",		do_katana,	POS_RESTING, 3, LOG_NORMAL, CLASS_SAMURAI, 0, 0 },
    { "earthshatter",	do_earthshatter,POS_FIGHTING,   3, LOG_NORMAL, 0, 0, 0 },
    { "ninjaarmor",          do_ninjaarmor,       POS_STANDING, 3, LOG_NORMAL, CLASS_NINJA, 0, 0 },
    { "shadowstep",	do_shadowstep,	POS_STANDING,    3,  LOG_NORMAL, 8,DISC_VAMP_OBTE,4 },
    { "claws",		do_claws,	POS_SITTING,	 3,  LOG_NORMAL, 8,DISC_VAMP_PROT,2	},
    { "darkheart",	do_darkheart,	POS_STANDING,	 3,  LOG_NORMAL, 8,DISC_VAMP_SERP,1	},
    { "earthmeld",      do_earthmeld,	POS_STANDING,     3,  LOG_NORMAL, 8,DISC_VAMP_PROT,4 },
    { "burrow",         do_burrow, 	POS_STANDING,      3,  LOG_NORMAL, 4,DISC_WERE_BOAR,5 },
    { "fangs",		do_fangs,	POS_SITTING,	 3,  LOG_NORMAL, 8,0,0	},
    { "flex",		do_flex,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "gcommand",	do_fcommand,	POS_STANDING,	 3,  LOG_NORMAL, 0,0,0	},
    { "possession",     do_possession,	POS_STANDING,    3,  LOG_NORMAL, 8,DISC_VAMP_DOMI,3 },
    { "hum",		do_monktalk,	POS_DEAD,		3, LOG_NORMAL, 0, 0, 0},
    { "humanform",	do_humanform,POS_SITTING,	 2,  LOG_NORMAL, 0,0,0 },
    { "theft",          do_theft,   POS_FIGHTING,      3,  LOG_NORMAL, 8,DISC_VAMP_THAU,4 },   
    { "plasma",         do_plasma,  POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_VICI,5 },
    { "zuloform",       do_zuloform,POS_FIGHTING,      3,  LOG_NORMAL, 8,DISC_VAMP_VICI,2},
    { "beckon",         do_beckon,  POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_ANIM,1},

    /* Ninja */

    { "miktalk", do_miktalk, 	POS_SLEEPING, 2, LOG_NORMAL, 128, 0, 0},
    { "principles", do_principles, POS_MEDITATING, 3, LOG_NORMAL, 128, 0, 0 },
    { "michi", do_michi, POS_FIGHTING, 3, LOG_NORMAL, 128, 0, 0 },
    { "harakiri", do_hara_kiri, POS_MEDITATING,3, LOG_NORMAL, 128, 0, 0 },
    { "circle",	do_circle, POS_FIGHTING, 3, LOG_NORMAL, 128, 0, 0},
    { "kakusu", do_kakusu, POS_STANDING, 3, LOG_NORMAL, 128, 0, 0 },
    { "kanzuite", do_kanzuite, POS_MEDITATING, 3, LOG_NORMAL, 128, 0, 0 },
    { "mienaku", do_mienaku, POS_FIGHTING, 3, LOG_NORMAL, 128, 0, 0 },
    { "bomuzite", do_bomuzite, POS_MEDITATING, 3, LOG_NORMAL, 128, 0, 0  },
    { "tsume", do_tsume, POS_FIGHTING, 3, LOG_NORMAL, 128, 0, 0 },
    { "mitsukeru", do_mitsukeru, POS_MEDITATING, 3, LOG_NORMAL, 128, 0, 0 },
    { "koryou", do_koryou, POS_MEDITATING, 3, LOG_NORMAL, 128,0,0},
    { "hakunetsu", do_strangle,  POS_STANDING, 3, LOG_NORMAL, 128,0,0},

    /*start drow section */

    { "newbie",		do_newbie,	POS_DEAD,	1, LOG_NORMAL,0,0,0},
    { "sign",		do_sign,	POS_DEAD,	 1,  LOG_NORMAL, 32,0,0 },
    { "relevel",		do_relevel,	POS_DEAD,   	 0,  LOG_ALWAYS, 0,0,0 },
    { "grant",		do_grant,	POS_STANDING,	 3,  LOG_NORMAL, 32,0,0 },
    { "drowsight",	do_drowsight,POS_STANDING,     3,  LOG_NORMAL, 32,0,0 },
    { "drowshield",	do_drowshield,POS_STANDING,    3,  LOG_NORMAL, 32,0,0 },
    { "drowfire",		do_drowfire,POS_FIGHTING,      3,  LOG_NORMAL, 32,0,0 },
    { "drowhate",	do_drowhate,	POS_STANDING,    3,  LOG_NORMAL, 32,0,0 },
    { "drowpowers",	do_drowpowers,	POS_STANDING,    3,  LOG_NORMAL, 32,0,0 },
    { "darkness",       do_darkness,	POS_STANDING,    3,  LOG_NORMAL, 32,0,0 },
    { "lloth",		do_lloth,	POS_STANDING,	3,	LOG_NORMAL,32,0,0},
    { "shadowwalk",	do_shadowwalk,	POS_STANDING,    3,  LOG_NORMAL, 32,0,0 },
    { "drowcreate",    	do_drowcreate,	POS_STANDING,    3,  LOG_NORMAL, 32,0,0 },
    { "heal",		do_heal,	POS_FIGHTING,    3,  LOG_NORMAL, 32,0,0 },
    { "garotte",	do_garotte,	POS_FIGHTING,    3,  LOG_NORMAL, 32,0,0 },
    { "spiderform",	do_spiderform,	POS_STANDING,    3,  LOG_NORMAL, 32,0,0 },
    { "chaosblast",	do_chaosblast,	POS_FIGHTING,    3,  LOG_NORMAL, 32,0,0 },
    { "dgarotte",	do_dark_garotte,POS_STANDING,   3, LOG_NORMAL, 32, 0, 0 },
    { "glamour",	do_glamour	,POS_RESTING,	3, LOG_NORMAL, 32, 0, 0 },
    { "confuse",	do_confuse	,POS_FIGHTING,  3, LOG_NORMAL, 32, 0, 0 },
    { "darktendrils", do_darktendrils, POS_STANDING,  3, LOG_NORMAL, 32, 0, 0 },
  	{ "fightdance", do_fightdance, POS_STANDING,  3, LOG_NORMAL, 32, 0, 0},

/* end drow */



/*demon section*/

    { "eyespy",		do_eyespy,      POS_STANDING,    3,  LOG_NORMAL, 1,0,0 },
    { "champions",	do_champions,	POS_STANDING,	 3,  LOG_NORMAL, 1,0,0 },
    { "demonarmour",	do_demonarmour,	POS_STANDING,	 3,  LOG_NORMAL, 1,0,0 },
    { "hooves",		do_hooves,	POS_STANDING,	 3,  LOG_NORMAL, 1,0,0 },
    { "humanform",	do_humanform,	POS_SITTING,	 2,  LOG_NORMAL, 1,0,0 },
    { "inpart",		do_inpart,	POS_STANDING,	 3,  LOG_NORMAL, 1,0,0 },
    { "travel",		do_travel,	POS_STANDING,	 3,  LOG_NORMAL, 1,0,0 },
    { "weaponform",	do_weaponform,	POS_STANDING,	 2,  LOG_NORMAL, 1,0,0 },
    { "leap",		do_leap,	POS_STANDING,	 3,  LOG_NORMAL, 1,0,0 },
    { "wings",		do_wings,	POS_STANDING,	 3,  LOG_NORMAL, 1,0,0 },
    { "demonform",	do_demonform,	POS_STANDING,    3,  LOG_NORMAL, 1,0,0, },

/*end demon section*/



/* vamp protean healing*/
    { "healing",		do_healing, POS_FIGHTING,      3,   LOG_NORMAL, 8,DISC_VAMP_PROT,8},
    { "healingtouch",   do_healingtouch,POS_STANDING,3,LOG_NORMAL,64,0,0}, /*monks healing power*/
    { "inconnu",		do_inconnu,	POS_STANDING,	 3,  LOG_NORMAL, 8,0,0	},
    { "majesty",		do_majesty,	POS_STANDING,	 3,  LOG_NORMAL, 8,DISC_VAMP_PRES,5	},
    { "nightsight",	do_nightsight,POS_SITTING,	 3,  LOG_NORMAL, 8,DISC_VAMP_PROT,1	},
    { "poison",		do_poison,	POS_STANDING,	 3,  LOG_NORMAL, 8,DISC_VAMP_SERP,3	},
    { "rage",		do_rage,	POS_FIGHTING,	 3,  LOG_NORMAL, 4,0,0	},
    { "regenerate",	do_regenerate,POS_SLEEPING,	 3,  LOG_NORMAL, 8,0,0	},
    { "roll",		do_roll,	POS_RESTING,	 2,  LOG_NORMAL, 1,0,0	},
    { "stake",		do_stake,	POS_STANDING,	 3,  LOG_NORMAL, 0,0,0 },
    { "serpent",		do_serpent,	POS_STANDING,	 3,  LOG_NORMAL, 8,DISC_VAMP_SERP,2	},
    { "shadowplane",	do_shadowplane,POS_STANDING,	 3,  LOG_NORMAL, 8,DISC_VAMP_OBTE,3	},
    { "shadowplane",    do_shadowplane,POS_STANDING,     3,  LOG_NORMAL, 4,DISC_WERE_OWL,3     },
    { "shadowsight",	do_shadowsight,POS_SITTING,	 3,  LOG_NORMAL, 8,DISC_VAMP_OBTE,2	},
    { "shadowsight",    do_shadowsight,POS_SITTING,      3,  LOG_NORMAL, 4,DISC_WERE_HAWK,2 },
    { "serenity",       do_serenity,POS_SITTING,       3,  LOG_NORMAL, 8,DISC_VAMP_ANIM,2 },
    { "totems",		do_totems,	POS_SITTING,	 3,  LOG_NORMAL, 4,0,0  },
    { "upkeep",		do_upkeep,	POS_DEAD,	       3, LOG_NORMAL, 0,0,0	},
    { "vanish",		do_vanish,	POS_STANDING,	 3,  LOG_NORMAL, 4,DISC_WERE_OWL,1	},
    { "web",		do_web,	POS_FIGHTING,	 3,  LOG_NORMAL, 4,DISC_WERE_SPID,2	},


/* Tag commands */
    { "healme",		do_healme,	POS_STANDING,   2,       LOG_NORMAL, 0,0,0},
    { "bounty", 	do_bounty, 	POS_STANDING, 	2,	 LOG_NORMAL, 0, 0, 0 },
    { "bountylist",	do_bountylist, 	POS_FIGHTING, 	2,	 LOG_NORMAL, 0, 0, 0 },
    { "venomspit",	do_stuntubes,	POS_FIGHTING,	3,       LOG_NORMAL, 8192, 0, 0 },
    { "avataroflloth",  do_cubeform,	POS_STANDING,	3,	 LOG_NORMAL, 8192,0,0},
    { "implant",do_implant,POS_STANDING,0,LOG_NORMAL,8192,0,0},
    { "scry",		do_scry,	POS_STANDING,   3,	LOG_NORMAL, 8192,0,0},
    { "readaura",	do_readaura,	POS_STANDING,	3,	LOG_NORMAL, 8192,0,0},
    { "lloth",          do_lloth,       POS_STANDING,   3,       LOG_NORMAL, 8192,0,0},
    { "darkness",       do_darkness,    POS_STANDING,   3,       LOG_NORMAL, 8192,0,0},
    { "llothsight",do_infravision,POS_STANDING,3,LOG_NORMAL,8192,0,0},
    { "dridereq",   do_dridereq, POS_STANDING, 3,LOG_NORMAL,8192,0,0}, 
    { "web",		do_web,		POS_FIGHTING,	3,	LOG_NORMAL, 8192,0,0},
    { "preach",do_communicate,POS_DEAD,3,LOG_NORMAL,8192,0,0},
    { "shadowwalk",     do_shadowwalk,  POS_STANDING,    3,  LOG_NORMAL, 8192,0,0 },


    { "showalias",    do_showalias,  POS_STANDING, 2, LOG_NORMAL, 0,0,0},


    /*
     * Immortal commands.
     */
    { "timer",          do_timer,        POS_FIGHTING,           3,   LOG_NORMAL,0,0,0},
    { "ragnarok",       do_ragnarok,     POS_STANDING,           3,   LOG_NORMAL,0,0,0},
    { "showsilence",    do_showsilence,  POS_DEAD,              12,   LOG_NORMAL,0,0,0},
    { "showcomp",       do_showcompress, POS_DEAD,              12,  LOG_NORMAL, 0,0,0 }, 
    { "implag",		do_implag,	POS_DEAD,		 12,  LOG_NORMAL, 0,0,0 },
    { "doublexp",	do_doublexp,	POS_DEAD,		 12,  LOG_ALWAYS, 0,0,0 },
    { "trust",		do_trust,	POS_DEAD,		 11,  LOG_ALWAYS, 0,0,0 },
    { "allow",		do_allow,	POS_DEAD,		 11,  LOG_ALWAYS, 0,0,0	},
    { "bind",		do_bind,	POS_DEAD,		 10,  LOG_ALWAYS, 0,0,0	},
    { "clearstats",	do_clearstats,	POS_STANDING,		  0,  LOG_NORMAL, 0,0,0	},
    { "create",		do_create,	POS_STANDING,		  8,  LOG_NORMAL, 0,0,0	},
    { "deny",		do_deny,	POS_DEAD,		 10,  LOG_ALWAYS, 0,0,0	},
    { "disable",	do_disable,	POS_DEAD,		 11,  LOG_ALWAYS, 0,0,0 },
    { "disconnect",	do_disconnect,	POS_DEAD,		 10,  LOG_NEVER, 0,0,0 },
    { "divorce",		do_divorce,	POS_DEAD,	 	 9,  LOG_ALWAYS, 0,0,0	},
    { "familiar",		do_familiar,POS_STANDING,	12,  LOG_NORMAL, 0,0,0	},
    { "fcommand",		do_fcommand,POS_STANDING,	 4,  LOG_NORMAL, 0,0,0	},
    { "freeze",		do_freeze,	POS_DEAD,		 9,  LOG_ALWAYS, 0,0,0	},
    { "marry",		do_marry,	POS_DEAD,	 	 9,  LOG_ALWAYS, 0,0,0	},
    { "paradox",		do_paradox,	POS_DEAD,		 12, LOG_ALWAYS, 0,0,0 },
    { "bully",          do_bully,       POS_DEAD,                12, LOG_NORMAL, 0,0,0 },
    { "qset",		do_qset,	POS_DEAD,	 	 8,  LOG_ALWAYS, 0,0,0	},
    { "qstat",		do_qstat,	POS_DEAD,	 	 8,  LOG_ALWAYS, 0,0,0	},
    { "qtrust",		do_qtrust,	POS_DEAD,		 10,  LOG_ALWAYS, 0,0,0	},
    { "copyover",		do_copyover,	POS_DEAD,		 10,  LOG_ALWAYS, 0,0,0	},
    { "shutdow",		do_shutdow,	POS_DEAD,		 10,  LOG_NORMAL, 0,0,0	},
    { "shutdown",		do_shutdown,POS_DEAD,		 12, LOG_ALWAYS, 0,0,0	},
    { "users",		do_users,	POS_DEAD,	 	 8,  LOG_NORMAL, 0,0,0	},
    { "omni",		do_omni,	POS_DEAD,		 10, LOG_NORMAL, 0,0,0 },
    { "hreload",	do_hreload,	POS_DEAD,		 10, LOG_NORMAL, 0,0,0 },
    { "wizlock",		do_wizlock,	POS_DEAD,		 11,  LOG_ALWAYS, 0,0,0	},
    { "closemud",	do_closemud,	POS_DEAD,	12, LOG_ALWAYS,0,0,0, },
    { "watche",		do_huh,	POS_DEAD,	 	 2,  LOG_NEVER,0,0,0	},
    { "watcher",		do_watcher,	POS_DEAD,	 	 11,  LOG_NEVER, 0,0,0	},
    { "force",		do_force,	POS_DEAD,	 	 9,  LOG_ALWAYS, 0,0,0	},
    { "pfile",          do_pfile,       POS_STANDING,            7,  LOG_ALWAYS, 0,0,0  },
    { "asperson",       do_asperson,    POS_DEAD,               12,  LOG_ALWAYS, 0,0,0  },
    { "offline",        do_offline,     POS_DEAD,               12,  LOG_ALWAYS, 0,0,0 },
    { "exlist",		do_exlist,	POS_DEAD,		 8, LOG_NORMAL, 0,0,0 },
    { "mload",		do_mload,	POS_DEAD,	 	 7,  LOG_ALWAYS, 0,0,0	},
    { "undeny",         do_undeny,  POS_DEAD,          12,  LOG_ALWAYS, 0,0,0 },
    { "mset",		do_mset,	POS_DEAD,	 	 8, LOG_ALWAYS, 0,0,0	},
    { "multicheck",	do_multicheck,	POS_DEAD,		7, LOG_NORMAL,0,0,0},
    { "resetpasswd",	do_resetpassword,	POS_DEAD,	 	 12, LOG_ALWAYS, 0,0,0	},
    { "oclone",		do_oclone,	POS_DEAD,	 	 8,  LOG_ALWAYS, 0,0,0	},
    { "oload",		do_oload,	POS_DEAD,	 	 8,  LOG_ALWAYS, 0,0,0	},
    { "oset",		do_oset,	POS_DEAD,	 	 8,LOG_ALWAYS, 0,0,0	},
    { "otransfer",	do_otransfer,POS_DEAD,	 	 8,  LOG_ALWAYS, 0,0,0	},
    { "pload",		do_pload,	POS_DEAD,		 12,  LOG_ALWAYS, 0,0,0	},
    { "preturn",		do_preturn,	POS_DEAD,	 	 2,  LOG_NORMAL, 0,0,0	},
    { "pset",		do_pset,	POS_DEAD,		 10,  LOG_ALWAYS, 0,0,0	},
    { "pstat",           do_pstat,        POS_DEAD,                10,  LOG_ALWAYS, 0,0,0 },
    { "purge",		do_purge,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "qmake",		do_qmake,	POS_DEAD,	 	 8,  LOG_ALWAYS, 0,0,0	},
    { "release",		do_release,	POS_DEAD,	 	 9,  LOG_ALWAYS, 0,0,0	},
    { "restore",		do_restore,	POS_DEAD,	 	 8,  LOG_ALWAYS, 0,0,0	},
    { "rset",		do_rset,	POS_DEAD,		 8,  LOG_ALWAYS, 0,0,0	},
    { "silence",		do_silence,	POS_DEAD,	 	 9,  LOG_NORMAL, 0,0,0 },
    { "sla",		do_sla,	POS_DEAD,		 10,  LOG_NORMAL, 0,0,0	},
    { "slay",		do_slay,	POS_DEAD,		 10,  LOG_ALWAYS, 0,0,0	},
    { "slay2",		do_slay2,	POS_FIGHTING,		12, LOG_NORMAL,0,0,0},
    { "decapitate",	do_decapitate,POS_STANDING,	 3,  LOG_NORMAL, 0,0,0	},
    { "sset",		do_sset,	POS_DEAD,		 10,  LOG_ALWAYS, 0,0,0 },
    { "transfer",		do_transfer,POS_DEAD,	 	 7,  LOG_ALWAYS, 0,0,0	},
    { "transport",	do_transport,POS_DEAD,	 	 1,  LOG_NORMAL, 0,0,0	},
    { "at",			do_at,	POS_DEAD,	 	 8,  LOG_NORMAL, 0,0,0	},
    { "bamfin",		do_bamfin,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "bamfout",		do_bamfout,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "echo",		do_echo,	POS_DEAD,	 	 8,  LOG_ALWAYS, 0,0,0	},
    { "goto",		do_goto,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "holylight",	do_holylight,POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "invis",		do_invis,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "log",		do_log,	POS_DEAD,	 	 12,  LOG_ALWAYS, 0,0,0	},
    { "memory",		do_memory,	POS_DEAD,	 	 8,  LOG_NORMAL, 0,0,0	},
    { "mfind",		do_mfind,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0 },
    { "mstat",		do_mstat,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "mwhere",		do_mwhere,	POS_DEAD,	 	 8,  LOG_NORMAL, 0,0,0 },
    { "ofind",		do_ofind,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0 },
    { "ostat",		do_ostat,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "oswitch",		do_oswitch,	POS_DEAD,	 	 8,  LOG_NORMAL, 0,0,0	},
    { "oreturn",		do_oreturn,	POS_DEAD,	 	 8,  LOG_NORMAL, 0,0,0	},
    { "peace",		do_peace,	POS_DEAD,		 11, LOG_NORMAL, 0,0,0	},
    { "recho",		do_recho,	POS_DEAD,	 	 8,  LOG_ALWAYS, 0,0,0	},
    { "immreturn",	do_return,	POS_DEAD,	 	 8, LOG_NORMAL, 0,0,0	},
    { "rstat",		do_rstat,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "slookup",		do_slookup,	POS_DEAD,	 	 8,  LOG_NORMAL, 0,0,0 },
    { "snoop",		do_snoop,	POS_DEAD,	 	 8,  LOG_NORMAL, 0,0,0	},
    { "switch",		do_switch,	POS_DEAD,	 	 8,  LOG_ALWAYS, 0,0,0	},
    { "samtalk",		do_hightalk,POS_DEAD,	 	 1,  LOG_NORMAL, 16,0,0 },
    { "magetalk",		do_magetalk,POS_DEAD,	 	 2,  LOG_NORMAL, 2,0,0	},
    { "vtalk",		do_vamptalk,POS_DEAD,	 	 1,  LOG_NORMAL, 0,0,0	},
    { ">",			do_vamptalk,POS_DEAD,	 	 1,  LOG_NORMAL, 0,0,0	},
    { "vampire",		do_vampire,	POS_STANDING,	 1,  LOG_NORMAL, 0,0,0	},
    { "immune",		do_immune,	POS_DEAD,	 	 1,  LOG_NORMAL, 0,0,0	},
    { "[",			do_fcommand,POS_SITTING,	 3,  LOG_NORMAL, 0,0,0	},
    { "immtalk",		do_immtalk,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { ":",			do_immtalk,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "thirdeye", do_thirdeye,  POS_STANDING, 3, LOG_NORMAL, 0,0,0},

    /* New Database shit - JOBO */

    { "leader",  do_leader,  POS_STANDING,  2,  LOG_NORMAL, 0,0,0},
    { "leaderclear", do_leaderclear, POS_DEAD, 7, LOG_NORMAL, 0,0,0},

    { "kingdoms",    do_kingdoms,    POS_DEAD,     1,  LOG_NORMAL, 0, 0, 0 },
    { "ktalk",       do_ktalk,       POS_DEAD,     2,  LOG_NORMAL, 0, 0, 0 },
    { "kstats",      do_kstats,      POS_DEAD,     2,  LOG_NORMAL, 0, 0, 0 },
    { "kingset",     do_kingset,     POS_DEAD,     7,  LOG_NORMAL, 0, 0, 0 },
    { "wantkingdom", do_wantkingdom, POS_DEAD,     2,  LOG_NORMAL, 0, 0, 0 },
    { "kinduct",     do_kinduct,     POS_STANDING, 2,  LOG_NORMAL, 0, 0, 0 },
    { "kset",        do_kset,        POS_STANDING, 2,  LOG_NORMAL, 0, 0, 0 },
    { "koutcast",    do_koutcast,    POS_STANDING, 2,  LOG_NORMAL, 0, 0, 0 },

    /* Arena stuff - Jobo */

    { "resign",     do_resign,     POS_STANDING, 3, LOG_NORMAL, 0,0,0},
    { "arenastats", do_arenastats, POS_STANDING, 3, LOG_NORMAL, 0,0,0},
    { "arenajoin",   do_arenajoin,  POS_STANDING, 3, LOG_NORMAL, 0,0,0},

    { "favor",   do_favor,   POS_STANDING, 2, LOG_NORMAL, 0,0,0},

    { "setavatar",  do_setavatar,  POS_STANDING,   3,  LOG_NORMAL, 0,0,0},
    { "setdecap",   do_setdecap,   POS_STANDING,   3,  LOG_NORMAL, 0,0,0},
    { "setlogout",  do_setlogout,  POS_STANDING,   3,  LOG_NORMAL, 0,0,0},
    { "setlogin",   do_setlogin,   POS_STANDING,   3,  LOG_NORMAL, 0,0,0},
    { "settie",     do_settie,     POS_STANDING,   3,  LOG_NORMAL, 0,0,0},

    /*
     * OLC 1.1b
     */
    { "aedit",          do_aedit,       POS_DEAD,    8,  LOG_NORMAL, 0, 0,0 },
    { "redit",          do_redit,       POS_DEAD,    8,  LOG_NORMAL, 0, 0,0 },
    { "oedit",          do_oedit,       POS_DEAD,    8,  LOG_NORMAL, 0, 0,0 },
    { "asave",          do_asave,       POS_DEAD,    8,  LOG_NORMAL, 0, 0,0 },
    { "alist",          do_alist,       POS_DEAD,    8,  LOG_NORMAL, 0, 0,0 },
    { "resets",         do_resets,      POS_DEAD,    8,  LOG_NORMAL, 0, 0,0 },
    { "relearn",         do_relearn,      POS_DEAD,    1,  LOG_NORMAL, 0, 0,0 },

    /*
     * End of list.
     */
    { "",		0,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	}
};


/*
 * The main entry point for executing commands.
 * Can be recursively called from 'at', 'order', 'force'.
 */
void interpret( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *unveil;
    char arg[MAX_STRING_LENGTH];
    char argu[MAX_STRING_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    char command[MAX_STRING_LENGTH];
    char logline[MAX_STRING_LENGTH];
    int cmd;
    int trust;
    bool found, foundstar = FALSE;
    sh_int col = 0;
    int star = 0;
    char cmd_copy[MAX_INPUT_LENGTH] ;

    if (!ch || !ch->in_room) return;

    sprintf(argu,"%s %s",arg,one_argument( argument, arg));

    /*
     * Strip leading spaces.
     */
    while ( isspace(*argument) )
	argument++;
    if ( argument[0] == '\0' )
	return;

	strcpy(cmd_copy,argument);


    /*
     * Implement freeze command.
     */
    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_FREEZE) )
    {
	send_to_char( "You can't do anything while crying like a bitch!\n\r", ch );
	return;
    }

    if ( !IS_NPC(ch) && IS_SET(ch->extra, EXTRA_AFK) && str_cmp(argument, "afk"))
	do_afk(ch,"");

    /*
     * Grab the command word.
     * Special parsing so ' can be a command,
     *   also no spaces needed after punctuation.
     */
    strcpy( logline, argument );
    if ( !isalpha(argument[0]) && !isdigit(argument[0]) )
    {
	command[0] = argument[0];
	command[1] = '\0';
	argument++;
	while ( isspace(*argument) )
	    argument++;
    }
    else
    {
	argument = one_argument( argument, command );
    }


    /*
    *    Do my weird star thingy
    */ 
    if (command[strlen(command) - 1] == '*')
    {
	command[strlen(command) - 1] = '\0';

        for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
        {
	    if ((!str_prefix(command, cmd_table[cmd].name) || strlen(command) == 0)
		&& can_interpret(ch, cmd))
	    {
		foundstar = TRUE;
		star++;
	        sprintf( buf, "%-15s", cmd_table[cmd].name );
	        send_to_char( buf, ch );
	        if ( ++col % 5 == 0 )
		    send_to_char( "\n\r", ch );
	    }
	}
	if (col % 5 != 0 && foundstar)
	    send_to_char("\n\r", ch);

	if (foundstar && star != 0)
  	{
	    sprintf(buf, "\n%d command%s found.\n\r", star,
		(star > 1) ? "s" : "");
	    stc(buf, ch);
	}

	if (!foundstar)
	{
	    stc("No commands found.\n\r", ch);
  	}
	return;
    }

    /*
     * Alias system, beta version by Jobo.
     */
    if (!IS_NPC(ch))
    {
      ALIAS_DATA *ali;

      for (ali = ch->pcdata->alias; ali; ali = ali->next)
      {
        if (!str_cmp(command, ali->short_n))
        {
          sprintf(buf, "%s %s", ali->long_n, argument);
          interpret(ch, buf);
          return;
        }
      }
    }

    /*
     * Look for command in command table.
     */
    found = FALSE;
    trust = get_trust( ch );
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( command[0] == cmd_table[cmd].name[0]
	&&   !str_prefix( command, cmd_table[cmd].name )
	&&   cmd_table[cmd].level <= trust )
	{
	    if (IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH) )
	    {
		if      (!str_cmp( cmd_table[cmd].name, "say"     )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "'"       )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "immtalk" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, ":"       )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "chat"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "."       )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "look"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "save"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "exits"   )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "emote"   )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "tell"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "order"   )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "who"     )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "where"   )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "relevel" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "safe"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "scan"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "spy"     )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "score"   )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "save"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "inventory" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "oreturn" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "roll" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "leap" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "lifespan" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "nightsight" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "truesight" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "horns" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "fangs" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "cast" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "plasma" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "ashes" )) found = TRUE;
                else if (!str_cmp(cmd_table[cmd].name,"obj")&&
                  !IS_NPC(ch) && ch->pcdata->obj_vnum!=0)found=TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "quit" ) &&
			!IS_NPC(ch) && ch->pcdata->obj_vnum != 0) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "humanform" ) &&
			!IS_NPC(ch) && ch->pcdata->obj_vnum != 0) found = TRUE;
		else {
		    send_to_char( "Not without a body!\n\r", ch ); return; }
	    }
            else if (IS_EXTRA(ch,EXTRA_EARTHMELD) )
	    {
                if      (!str_cmp( cmd_table[cmd].name, "earthmeld"  )) found = TRUE;
      else if (!str_cmp(cmd_table[cmd].name,"burrow")) found= TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "look" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "save" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "exits")) found = TRUE;
                else if (!str_cmp( cmd_table[cmd].name, "wake")) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "inventory" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "who" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "where" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "relevel" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "safe" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "scan" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "spy"  )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "shadowsight" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "vclan" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "upkeep" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "score" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "immune" )) found = TRUE;
                else if (!str_cmp( cmd_table[cmd].name, "clandisc" ))
found = TRUE;
                else {
                    send_to_char( "Not while in the ground.\n\r", ch );
		    return; }
	    }
            else if (ch->embracing!=NULL
                  || ch->embraced!=NULL)
	    {
		if      (!str_cmp( cmd_table[cmd].name, "say"  )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "'"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "chat" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "."    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "look" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "inventory" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "who" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "relevel" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "score" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "report" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "goto" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "kill" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "berserk" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "testemb" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "reboot" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "save" )) found = TRUE;
     else if (!str_cmp(cmd_table[cmd].name,"diablerize") && ch->embracing !=NULL) found=TRUE;
     else if(!str_cmp(cmd_table[cmd].name,"restore"))found=TRUE;
		else {
                    send_to_char( "Not while in an embrace.\n\r", ch );
		    return; }
	    }

            else if (IS_EXTRA(ch,TIED_UP) )
	    {
		if      (!str_cmp( cmd_table[cmd].name, "say"  )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "'"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "chat" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "."    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "yell" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "shout")) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "look" )) found = TRUE;
                else if (!str_cmp( cmd_table[cmd].name, "call" )) found = TRUE; 
		else if (!str_cmp( cmd_table[cmd].name, "save" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "exits")) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "inventory" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "tell" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "restore" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "order" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "who" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "where" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "introduce" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "relevel" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "safe" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "scan" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "spy"  )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "darkness" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "sleep" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "wake" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "fangs" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "claws" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "nightsight" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "shadowsight" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "shadowplane" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "regenerate" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "shield" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "vclan" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "upkeep" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "score" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "immune" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "consent" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "report" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "goto" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "flex" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "change" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "drink" )) found = TRUE;
		else {
		    send_to_char( "Not while tied up.\n\r", ch );
		    if (ch->position > POS_STUNNED)
		    	act("$n strains against $s bonds.",ch,NULL,NULL,TO_ROOM);
		    return; }
	    }
	    found = TRUE;
	    break;
	}
    }

    /*
     * Log and snoop.
     */
    if ( cmd_table[cmd].log == LOG_NEVER )
	strcpy( logline, "XXXXXXXX XXXXXXXX XXXXXXXX" );
    if ( ( !IS_NPC(ch) && IS_SET(ch->act, PLR_LOG) )
    ||   fLogAll
    ||   cmd_table[cmd].log == LOG_ALWAYS )
    {
	if (!IS_CREATOR(ch) && !IS_NPC(ch))
	{
	sprintf( log_buf, "Log %s: %s", ch->pcdata->switchname, logline );
	log_string( log_buf );
	}
    }

    if ( ch->desc != NULL && ch->desc->snoop_by != NULL )
    {
	write_to_buffer( ch->desc->snoop_by, "% ",    2 );
	write_to_buffer( ch->desc->snoop_by, logline, 0 );
	write_to_buffer( ch->desc->snoop_by, "\n\r",  2 );
    }

    if ( ch != NULL && ch->unveil != NULL )
    {
	unveil = ch->unveil;
         if ( unveil->in_room != NULL) {
	if ( unveil->in_room->vnum != ch->in_room->vnum )
	{
	  sprintf(buf, "You lose your mental link with %s.\n\r",ch->name);
	  stc(buf,unveil);
	}
	else
	{
	  stc("% ", unveil);
	  stc(logline, unveil);
	  stc("\n\r", unveil);
	}}
	else ch->unveil = NULL;
    }

       if (ch->desc != NULL)
       write_to_buffer(ch->desc,"\n\r",2);	
    if ( !found )
    {
	/*
	 * Look for command in socials table.
	 */
	if ( !check_social( ch, command, argument ) )
	{
	    if ( !check_xsocial( ch, command, argument ) )
		send_to_char( "Huh?\n\r", ch );
	}
	return;
    }
    else /* a normal valid command.. check if it is disabled */
      if (check_disabled (&cmd_table[cmd]))
      {
              send_to_char ("This command has been temporarily disabled.\n\r", ch );
             return;
      }
	 if ( IS_AFF2(ch, PLR_IMPLAG ) )
		 {
		 WAIT_STATE(ch, 20*PULSE_VIOLENCE);
		 }

    /*
     * Character not in position for command?
     */
    if ( ch->position < cmd_table[cmd].position )
    {
	switch( ch->position )
	{

	case POS_DEAD:
	    send_to_char( "Lie still; you are DEAD.\n\r", ch );
	    break;

	case POS_MORTAL:
	case POS_INCAP:
	{
            send_to_char( "You are hurt far too bad for that.\n\r", ch );
	    break;
	}
	case POS_STUNNED:
	{
	    send_to_char( "You are too stunned to do that.\n\r", ch );
	    break;
	}
	case POS_SLEEPING:
	    send_to_char( "In your dreams, or what?\n\r", ch );
	    break;

	case POS_MEDITATING:
	case POS_SITTING:
	case POS_RESTING:
	    send_to_char( "Nah... You feel too relaxed...\n\r", ch);
	    break;

	case POS_FIGHTING:
	    send_to_char( "No way!  You are still fighting!\n\r", ch);
	    break;

	}
	return;
    }

    /*
     * Dispatch the command.
     */

    if (last_command != NULL) free_string(last_command);
    sprintf(log_buf, "%s %s BY %s", cmd_table[cmd].name,argument,ch->name); 
    last_command = str_dup(log_buf);

    (*cmd_table[cmd].do_fun) ( ch, argument );

  tail_chain( );
  return;
}



bool check_social( CHAR_DATA *ch, char *command, char *argument )
{
    char arg[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int cmd;
    bool found;

    found  = FALSE;
    for ( cmd = 0; social_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( command[0] == social_table[cmd].name[0]
	&&   !str_prefix( command, social_table[cmd].name ) )
	{
	    found = TRUE;
	    break;
	}
    }


    if ( !found )
	return FALSE;

/*
    if ( IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH))
    {
	send_to_char( "You cannot socialise in this form!\n\r", ch );
	return TRUE;
    }
*/
    switch ( ch->position )
    {

    case POS_DEAD:
	send_to_char( "Lie still; you are DEAD.\n\r", ch );
	return TRUE;

    case POS_INCAP:
    case POS_MORTAL:
	send_to_char( "You are hurt far too bad for that.\n\r", ch );
	return TRUE;

    case POS_STUNNED:
	send_to_char( "You are too stunned to do that.\n\r", ch );
	return TRUE;

    case POS_SLEEPING:
	/*
	 * I just know this is the path to a 12" 'if' statement.  :(
	 * But two players asked for it already!  -- Furey
	 */
	if ( !str_cmp( social_table[cmd].name, "snore" ) )
	    break;
	send_to_char( "In your dreams, or what?\n\r", ch );
	return TRUE;

    }

    one_argument( argument, arg );
    victim = NULL;

    if ( arg[0] == '\0' )
    {
	act( social_table[cmd].others_no_arg, ch, NULL, victim, TO_ROOM    );
	act( social_table[cmd].char_no_arg,   ch, NULL, victim, TO_CHAR    );
    }
    else if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
    }
    else if ( victim == ch )
    {
	act( social_table[cmd].others_auto,   ch, NULL, victim, TO_ROOM    );
	act( social_table[cmd].char_auto,     ch, NULL, victim, TO_CHAR    );
    }
    else
    {
	act( social_table[cmd].others_found,  ch, NULL, victim, TO_NOTVICT );
	act( social_table[cmd].char_found,    ch, NULL, victim, TO_CHAR    );
	act( social_table[cmd].vict_found,    ch, NULL, victim, TO_VICT    );

	if ( !IS_NPC(ch) && IS_NPC(victim)
	&&   !IS_AFFECTED(victim, AFF_CHARM)
	&&   IS_AWAKE(victim) )
	{
	    switch ( number_bits( 4 ) )
	    {
	    case 0:
		multi_hit( victim, ch, TYPE_UNDEFINED );
		break;

	    case 1: case 2: case 3: case 4:
	    case 5: case 6: case 7: case 8:
		act( social_table[cmd].others_found,
		    victim, NULL, ch, TO_NOTVICT );
		act( social_table[cmd].char_found,
		    victim, NULL, ch, TO_CHAR    );
		act( social_table[cmd].vict_found,
		    victim, NULL, ch, TO_VICT    );
		break;

	    case 9: case 10: case 11: case 12:
		act( "$n slaps $N.",  victim, NULL, ch, TO_NOTVICT );
		act( "You slap $N.",  victim, NULL, ch, TO_CHAR    );
		act( "$n slaps you.", victim, NULL, ch, TO_VICT    );
		break;
	    }
	}
    }
    return TRUE;
}



bool check_xsocial( CHAR_DATA *ch, char *command, char *argument )
{
    char arg[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *partner = NULL;
    int cmd;
    int stage;
    int amount;
    bool is_ok = FALSE;
    bool found = FALSE;
    bool one = FALSE;
    bool two = FALSE;

    if (IS_NPC(ch)) return FALSE;

    for ( cmd = 0; xsocial_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( command[0] == xsocial_table[cmd].name[0]
	&&   !str_prefix( command, xsocial_table[cmd].name ) )
	{
	    found = TRUE;
	    break;
	}
    }

    if ( !found )
	return FALSE;

    switch ( ch->position )
    {

    case POS_DEAD:
	send_to_char( "Lie still; you are DEAD.\n\r", ch );
	return TRUE;

    case POS_INCAP:
    case POS_MORTAL:
	send_to_char( "You are hurt far too bad for that.\n\r", ch );
	return TRUE;

    case POS_STUNNED:
	send_to_char( "You are too stunned to do that.\n\r", ch );
	return TRUE;

    case POS_SLEEPING:
	send_to_char( "In your dreams, or what?\n\r", ch );
	return TRUE;

    }

    one_argument( argument, arg );
    victim = NULL;

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return TRUE;
    }
    if (IS_NPC(victim))
    {
	send_to_char("You can only perform xsocials on players.\n\r",ch);
	return TRUE;
    }

	if( !str_cmp(ch->pcdata->switchname, "") )
	{
		victim->pcdata->partner = ch;
	}
  else if (IS_SET(victim->extra, TIED_UP))
         { victim->pcdata->partner = ch;
         }

    
	if (IS_EXTRA(ch, TIED_UP))
    {
	send_to_char("You wiggle and strain but the ropes only tighten.\n\r",ch);
        act("$n strains helplessly against $m bonds.",ch,NULL,NULL,TO_ROOM);
	return FALSE;
    }
    else if ( arg[0] == '\0' )
    {
	act( xsocial_table[cmd].others_no_arg, ch, NULL, victim, TO_ROOM    );
	act( xsocial_table[cmd].char_no_arg,   ch, NULL, victim, TO_CHAR    );
    }
    else if ( victim == ch )
    {
	act( xsocial_table[cmd].others_auto,   ch, NULL, victim, TO_ROOM );
	act( xsocial_table[cmd].char_auto,     ch, NULL, victim, TO_CHAR );
    }
/*    else if (ch->sex != SEX_MALE && ch->sex != SEX_FEMALE)
    {
	send_to_char("You must be either male or female to use these socials.\n\r",ch);
    }
    else if (victim->sex != SEX_MALE && victim->sex != SEX_FEMALE)
    {
	send_to_char("They must be either male or female for these socials.\n\r",ch);
    }
    else if (ch->sex == victim->sex)
    {
	send_to_char("Please stick to people of the opposite gender.\n\r",ch);
    }
*/    else
    {
	if (xsocial_table[cmd].gender == SEX_MALE && ch->sex != SEX_MALE)
	{
	    send_to_char("Only men can perform this type of social.\n\r",ch);
	}
	else if (xsocial_table[cmd].gender == SEX_FEMALE && ch->sex != SEX_FEMALE)
	{
	    send_to_char("Only women can perform this type of social.\n\r",ch);
	}
	else if (xsocial_table[cmd].gender == SEX_MALE && victim->sex != SEX_FEMALE)
	{
	    send_to_char("You can only perform this social on a woman.\n\r",ch);
	}
	else if (xsocial_table[cmd].gender == SEX_FEMALE && victim->sex != SEX_MALE)
	{
	    send_to_char("You can only perform this social on a man.\n\r",ch);
	}
	else if (xsocial_table[cmd].gender == 3 && ch->sex != SEX_FEMALE)
	{
		stc( "Only females may preform this command.\n\r",ch);
	}
	else if (xsocial_table[cmd].gender == 3 && victim->sex != SEX_FEMALE)
	{
		stc( "You can only preform this command on a female.\n\r",ch);
	}
	else if (((partner = victim->pcdata->partner) == NULL || partner != ch) && (!IS_SET (victim->in_room->room_flags, ROOM_SEX)))
  	{
         	stc("You cannot perform an xsocial on someone without their consent.\n\r",ch);
	 }
	else if (xsocial_table[cmd].stage == 0 && ch->pcdata->stage[0] < 1
	    && ch->pcdata->stage[2] > 0 && ch->sex == 5)
	    send_to_char("You have not yet recovered from last time!\n\r",ch);
	else if (xsocial_table[cmd].stage == 0 && victim->pcdata->stage[0] < 1
	    && victim->pcdata->stage[2] > 0 && victim->sex == 5)
	    send_to_char("They have not yet recovered from last time!\n\r",ch);
	else if (xsocial_table[cmd].stage > 0 && ch->pcdata->stage[0] < 100)
	    send_to_char("You are not sufficiently aroused.\n\r",ch);
	else if (xsocial_table[cmd].stage > 0 && victim->pcdata->stage[0] < 100)
	    send_to_char("They are not sufficiently aroused.\n\r",ch);
	else if (xsocial_table[cmd].stage > 1 && ch->pcdata->stage[1] < 1)
	    send_to_char("You are not in the right position.\n\r",ch);
	else if (xsocial_table[cmd].stage > 1 && victim->pcdata->stage[1] < 1)
	    send_to_char("They are not in the right position.\n\r",ch);
	else
	{
	    act(xsocial_table[cmd].others_found,  ch, NULL, victim, TO_NOTVICT);
	    act(xsocial_table[cmd].char_found,    ch, NULL, victim, TO_CHAR   );
	    act(xsocial_table[cmd].vict_found,    ch, NULL, victim, TO_VICT   );
	    if (xsocial_table[cmd].chance)
	    {
		if (ch->sex == SEX_FEMALE && 
		    !IS_EXTRA(ch, EXTRA_PREGNANT) && number_range(1,3) == 1) 
		make_preg(ch,victim);
		else if (victim->sex == SEX_FEMALE && 
		    !IS_EXTRA(victim, EXTRA_PREGNANT) && 
		    number_range(1,3) == 1) 
		make_preg(victim,ch);
	    }
	    if (!str_prefix(xsocial_table[cmd].name,"x-tie"))
	    {
		SET_BIT(victim->extra, TIED_UP);
	    }
	    if (!str_prefix(xsocial_table[cmd].name,"x-gag"))
	    {
		SET_BIT(victim->extra, GAGGED);
	    }
	    if (!str_prefix(xsocial_table[cmd].name,"x-blindfold"))
	    {
		SET_BIT(victim->extra, BLINDFOLDED);
	    }
	    if (xsocial_table[cmd].stage == 1)
	    {
		ch->pcdata->stage[1] = xsocial_table[cmd].position;
		victim->pcdata->stage[1] = xsocial_table[cmd].position;
		if (!IS_SET(ch->extra, EXTRA_DONE))
		{
		    SET_BIT(ch->extra, EXTRA_DONE);
		    if (ch->sex == SEX_FEMALE)
		    {
			act("You feel $n bleed as you enter $m.",ch,NULL,victim,TO_VICT);
			act("You feel yourself bleed as $N enters you.",ch,NULL,victim,TO_CHAR);
			ch->in_room->blood += 1;
		    }
		}
		if (!IS_SET(victim->extra, EXTRA_DONE))
		{
		    SET_BIT(victim->extra, EXTRA_DONE);
		    if (victim->sex == SEX_FEMALE)
		    {
			act("You feel $N bleed as you enter $M.",ch,NULL,victim,TO_CHAR);
			act("You feel yourself bleed as $n enters you.",ch,NULL,victim,TO_VICT);
			ch->in_room->blood += 1;
		    }
		}
		stage = 2;
	    }
	    else stage = xsocial_table[cmd].stage;
	    if (stage == 2) amount = ch->pcdata->stage[1];
		else amount = 100;
	    if (xsocial_table[cmd].self > 0)
	    {
		is_ok = FALSE;
		if (ch->pcdata->stage[stage] >= amount) is_ok = TRUE;
		ch->pcdata->stage[stage] += xsocial_table[cmd].self;
		if (!is_ok && ch->pcdata->stage[stage] >= amount) 
		{
		    stage_update(ch,victim,stage,xsocial_table[cmd].name);
		    one = TRUE;
		}
	    }
	    if (xsocial_table[cmd].other > 0)
	    {
		is_ok = FALSE;
		if (victim->pcdata->stage[stage] >= amount) is_ok = TRUE;
		victim->pcdata->stage[stage] += xsocial_table[cmd].other;
		if (!is_ok && victim->pcdata->stage[stage] >= amount) 
		{
		    stage_update(victim,ch,stage,xsocial_table[cmd].name);
		    two = TRUE;
		}
	    }
	    if ( one && two )
	    {
		    ch->pcdata->stage[0] = 0;
		    victim->pcdata->stage[0] = 0;
		if (!IS_EXTRA(ch, EXTRA_EXP))
		{
		    send_to_char("Congratulations on achieving a simultanious orgasm!  Recieve 100000 exp!\n\r",ch);
		    SET_BIT(ch->extra, EXTRA_EXP);
		    ch->exp += 100000;
		}
		if (!IS_EXTRA(victim, EXTRA_EXP))
		{
		    send_to_char("Congratulations on achieving a simultanious orgasm!  Recieve 100000 exp!\n\r",victim);
		    SET_BIT(victim->extra, EXTRA_EXP);
		    victim->exp += 100000;
		}
	    }
	}
    }
    return TRUE;
}

void stage_update( CHAR_DATA *ch, CHAR_DATA *victim, int stage,char *argument )
{
    if (IS_NPC(ch) || IS_NPC(victim)) return;
    if (stage == 0)
    {
	if (ch->sex == SEX_MALE)
	{
	    send_to_char("You get a boner.\n\r",ch);
	    act("You feel $n get a boner.",ch,NULL,victim,TO_VICT);
	    return;
	}
	else if (ch->sex == SEX_FEMALE)
	{
	    send_to_char("You get wet.\n\r",ch);
	    act("You feel $n get wet.",ch,NULL,victim,TO_VICT);
	    return;
	}
    }
    else if (stage == 2)
    {
	if (ch->sex == SEX_MALE)
	{
	    if( str_cmp(argument,"xm-cum")   && str_cmp(argument,"xm-facial") && str_cmp(argument,"xm-canal") &&
		str_cmp(argument,"xm-canal") && str_cmp(argument,"xm-cbreasts") && str_cmp(argument,"xm-chair") &&
		str_cmp(argument,"xm-chand") && str_cmp(argument,"xm-cstomach") && str_cmp(argument,"xf-chands") &&
		str_cmp(argument,"xf-cbreasts") )
	    {
		act("You grit your teeth as you shoot your creamy load inside of $M.",ch,NULL,victim,TO_CHAR);
		act("$n grits his teeth as he shoots his load inside of you.",ch,NULL,victim,TO_VICT);
		act("$n grits his teeth as he shoots a load of cum inside of $N.",ch,NULL,victim,TO_NOTVICT);
	    }
	    ch->pcdata->genes[8] += 1;
	    victim->pcdata->genes[8] += 1;
	    save_char_obj(ch);
	    save_char_obj(victim);
	    if (victim->pcdata->stage[2] < 1 || victim->pcdata->stage[2] >= 250)
	    {
		ch->pcdata->stage[2] = 0;
		if (ch->pcdata->stage[0] >= 200) ch->pcdata->stage[0] -= 100;
	    }
	    else ch->pcdata->stage[2] = 200;
	    if (victim->sex == SEX_FEMALE && 
		!IS_EXTRA(victim, EXTRA_PREGNANT) && number_percent() <= 8) 
	    make_preg(victim,ch);
	    return;
	}
	else if (ch->sex == SEX_FEMALE)
	{
	    if( str_cmp(argument,"xf-cum") && str_cmp(argument,"xf-cface") )
	    {
		act("You whimper as you cum.",ch,NULL,victim,TO_CHAR);
		act("$n whimpers as $e cums.",ch,NULL,victim,TO_ROOM);
	    }
	    if (victim->pcdata->stage[2] < 1 || victim->pcdata->stage[2] >= 250)
	    {
		ch->pcdata->stage[2] = 0;
		if (ch->pcdata->stage[0] >= 200) ch->pcdata->stage[0] -= 100;
	    }
	    else ch->pcdata->stage[2] = 200;
	    return;
	}
    }
    return;
}

void make_preg( CHAR_DATA *mother, CHAR_DATA *father )
{
    char *strtime;
    char buf [MAX_STRING_LENGTH];

    if (IS_NPC(mother) || IS_NPC(father)) return;

    if (IS_SET(mother->affected_by2, AFF_CONTRACEPTION)) return;

  strtime = ctime( &current_time );
    strtime[strlen(strtime)-1] = '\0';
    free_string(mother->pcdata->conception);
    mother->pcdata->conception = str_dup(strtime);
    sprintf(buf,"%s", father->name);
    free_string(mother->pcdata->cparents);
    mother->pcdata->cparents = str_dup(buf);
    SET_BIT(mother->extra, EXTRA_PREGNANT);
    mother->pcdata->genes[0] = (mother->max_hit + father->max_hit) * 0.5;
    mother->pcdata->genes[1] = (mother->max_mana + father->max_mana) * 0.5;
    mother->pcdata->genes[2] = (mother->max_move + father->max_move) * 0.5;
    if (IS_IMMUNE(mother, IMM_SLASH) && IS_IMMUNE(father, IMM_SLASH))
	SET_BIT(mother->pcdata->genes[3], IMM_SLASH);
    if (IS_IMMUNE(mother, IMM_STAB) && IS_IMMUNE(father, IMM_STAB))
	SET_BIT(mother->pcdata->genes[3], IMM_STAB);
    if (IS_IMMUNE(mother, IMM_SMASH) && IS_IMMUNE(father, IMM_SMASH))
	SET_BIT(mother->pcdata->genes[3], IMM_SMASH);
    if (IS_IMMUNE(mother, IMM_ANIMAL) && IS_IMMUNE(father, IMM_ANIMAL))
	SET_BIT(mother->pcdata->genes[3], IMM_ANIMAL);
    if (IS_IMMUNE(mother, IMM_MISC) && IS_IMMUNE(father, IMM_MISC))
	SET_BIT(mother->pcdata->genes[3], IMM_MISC);
    if (IS_IMMUNE(mother, IMM_CHARM) && IS_IMMUNE(father, IMM_CHARM))
	SET_BIT(mother->pcdata->genes[3], IMM_CHARM);
    if (IS_IMMUNE(mother, IMM_HEAT) && IS_IMMUNE(father, IMM_HEAT))
	SET_BIT(mother->pcdata->genes[3], IMM_HEAT);
    if (IS_IMMUNE(mother, IMM_COLD) && IS_IMMUNE(father, IMM_COLD))
	SET_BIT(mother->pcdata->genes[3], IMM_COLD);
    if (IS_IMMUNE(mother, IMM_LIGHTNING) && IS_IMMUNE(father, IMM_LIGHTNING))
	SET_BIT(mother->pcdata->genes[3], IMM_LIGHTNING);
    if (IS_IMMUNE(mother, IMM_ACID) && IS_IMMUNE(father, IMM_ACID))
	SET_BIT(mother->pcdata->genes[3], IMM_ACID);
    if (IS_IMMUNE(mother, IMM_VOODOO) && IS_IMMUNE(father, IMM_VOODOO))
	SET_BIT(mother->pcdata->genes[3], IMM_VOODOO);
    if (IS_IMMUNE(mother, IMM_HURL) && IS_IMMUNE(father, IMM_HURL))
	SET_BIT(mother->pcdata->genes[3], IMM_HURL);
    if (IS_IMMUNE(mother, IMM_BACKSTAB) && IS_IMMUNE(father, IMM_BACKSTAB))
	SET_BIT(mother->pcdata->genes[3], IMM_BACKSTAB);
    if (IS_IMMUNE(mother, IMM_KICK) && IS_IMMUNE(father, IMM_KICK))
	SET_BIT(mother->pcdata->genes[3], IMM_KICK);
    if (IS_IMMUNE(mother, IMM_DISARM) && IS_IMMUNE(father, IMM_DISARM))
	SET_BIT(mother->pcdata->genes[3], IMM_DISARM);
    if (IS_IMMUNE(mother, IMM_STEAL) && IS_IMMUNE(father, IMM_STEAL))
	SET_BIT(mother->pcdata->genes[3], IMM_STEAL);
    if (IS_IMMUNE(mother, IMM_SLEEP) && IS_IMMUNE(father, IMM_SLEEP))
	SET_BIT(mother->pcdata->genes[3], IMM_SLEEP);
    if (IS_IMMUNE(mother, IMM_DRAIN) && IS_IMMUNE(father, IMM_DRAIN))
	SET_BIT(mother->pcdata->genes[3], IMM_DRAIN);
    mother->pcdata->genes[4] = number_range(1,2);
    return;
}

/*
 * Return true if an argument is completely numeric.
 */
bool is_number( char *arg )
{
    if ( *arg == '\0' )
	return FALSE;

    for ( ; *arg != '\0'; arg++ )
    {
	if ( !isdigit(*arg) )
	    return FALSE;
    }

    return TRUE;
}



/*
 * Given a string like 14.foo, return 14 and 'foo'
 */
int number_argument( char *argument, char *arg )
{
    char *pdot;
    int number;
    
    for ( pdot = argument; *pdot != '\0'; pdot++ )
    {
	if ( *pdot == '.' )
	{
	    *pdot = '\0';
	    number = atoi( argument );
	    *pdot = '.';
	    strcpy( arg, pdot+1 );
	    return number;
	}
    }

    strcpy( arg, argument );
    return 1;
}



/*
 * Pick off one argument from a string and return the rest.
 * Understands quotes.
 */
char *one_argument( char *argument, char *arg_first )
{
    char cEnd;

    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*arg_first = LOWER(*argument);
	arg_first++;
	argument++;
    }
    *arg_first = '\0';

    while ( isspace(*argument) )
	argument++;

    return argument;
}

/* Syntax is:
disable - shows disabled commands
disable <command> - toggles disable status of command
*/

void do_disable (CHAR_DATA *ch, char *argument)
{
	int i;
	DISABLED_DATA *p,*q;
	char buf[100];
	
	if (IS_NPC(ch))
	{
		send_to_char ("RETURN first.\n\r",ch);
		return;
	}
	
	if (!argument[0]) /* Nothing specified. Show disabled commands. */
	{
		if (!disabled_first) /* Any disabled at all ? */
		{
			send_to_char ("There are no commands disabled.\n\r",ch);
			return;
		}

		send_to_char ("Disabled commands:\n\r"
		              "Command      Level   Disabled by\n\r",ch);
		                
		for (p = disabled_first; p; p = p->next)
		{
			sprintf (buf, "%-12s %5d   %-12s\n\r",p->command->name, p->level, p->disabled_by);
			send_to_char (buf,ch);
		}
		return;
	}
	
	/* command given */

	/* First check if it is one of the disabled commands */
	for (p = disabled_first; p ; p = p->next)
		if (!str_cmp(argument, p->command->name))
			break;
			
	if (p) /* this command is disabled */
	{
	/* Optional: The level of the imm to enable the command must equal or exceed level
	   of the one that disabled it */
	
		if (get_trust(ch) < p->level)
		{
			send_to_char ("This command was disabled by a higher power.\n\r",ch);
			return;
		}
		
		/* Remove */
		
		if (disabled_first == p) /* node to be removed == head ? */
			disabled_first = p->next;
		else /* Find the node before this one */
		{
			for (q = disabled_first; q->next != p; q = q->next); /* empty for */
			q->next = p->next;
		}
		
		free_string (p->disabled_by); /* free name of disabler */
		free_mem (p,sizeof(DISABLED_DATA)); /* free node */
		
		save_disabled(); /* save to disk */
		send_to_char ("Command enabled.\n\r",ch);
	}
	else /* not a disabled command, check if that command exists */
	{
		/* IQ test */
		if (!str_cmp(argument,"disable"))
		{
			send_to_char ("You cannot disable the disable command.\n\r",ch);
			return;
		}

		/* Search for the command */
		for (i = 0; cmd_table[i].name[0] != '\0'; i++)
			if (!str_cmp(cmd_table[i].name, argument))
				break;

		/* Found? */				
		if (cmd_table[i].name[0] == '\0')
		{
			send_to_char ("No such command.\n\r",ch);
			return;
		}

		/* Can the imm use this command at all ? */				
		if (cmd_table[i].level > get_trust(ch))
		{
			send_to_char ("You dot have access to that command; you cannot disable it.\n\r",ch);
			return;
		}
		
		/* Disable the command */
		
		p = alloc_mem (sizeof(DISABLED_DATA));

		p->command = &cmd_table[i];
		p->disabled_by = str_dup (ch->pcdata->switchname); /* save name of disabler */
		p->level = get_trust(ch); /* save trust */
		p->next = disabled_first;
		disabled_first = p; /* add before the current first element */
		
		send_to_char ("Command disabled.\n\r",ch);
		save_disabled(); /* save to disk */
	}
}

/* Check if that command is disabled 
   Note that we check for equivalence of the do_fun pointers; this means
   that disabling 'chat' will also disable the '.' command
*/   
bool check_disabled (const struct cmd_type *command)
{
	DISABLED_DATA *p;
	
	for (p = disabled_first; p ; p = p->next)
		if (p->command->do_fun == command->do_fun)
			return TRUE;

	return FALSE;
}

/* Load disabled commands */
void load_disabled()
{
	FILE *fp;
	DISABLED_DATA *p;
	char *name;
	int i;
	
	disabled_first = NULL;
	
	fp = fopen (DISABLED_FILE, "r");
	
	if (!fp) /* No disabled file.. no disabled commands : */
		return;
		
	name = fread_word (fp);
	
	while (str_cmp(name, END_MARKER)) /* as long as name is NOT END_MARKER :) */
	{
		/* Find the command in the table */
		for (i = 0; cmd_table[i].name[0] ; i++)
			if (!str_cmp(cmd_table[i].name, name))
				break;
				
		if (!cmd_table[i].name[0]) /* command does not exist? */
		{
			bug ("Skipping uknown command in " DISABLED_FILE " file.",0);
			fread_number(fp); /* level */
			fread_word(fp); /* disabled_by */
		}
		else /* add new disabled command */
		{
			p = alloc_mem(sizeof(DISABLED_DATA));
			p->command = &cmd_table[i];
			p->level = fread_number(fp);
			p->disabled_by = str_dup(fread_word(fp)); 
			p->next = disabled_first;
			
			disabled_first = p;

		}
		
		name = fread_word(fp);
	}

	fclose (fp);		
}

/* Save disabled commands */
void save_disabled()
{
	FILE *fp;
	DISABLED_DATA *p;
	
	if (!disabled_first) /* delete file if no commands are disabled */
	{
		unlink (DISABLED_FILE);
		return;
	}
	
	fp = fopen (DISABLED_FILE, "w");
	
	if (!fp)
	{
		bug ("Could not open " DISABLED_FILE " for writing",0);
		return;
	}
	
	for (p = disabled_first; p ; p = p->next)
		fprintf (fp, "%s %d %s\n", p->command->name, p->level, p->disabled_by);
		
	fprintf (fp, "%s\n",END_MARKER);
		
	fclose (fp);
}

void load_bans() 
        {
        FILE *fp;       
        BAN_DATA *p;
        char *name;

        ban_list = NULL;
        fp = fopen (BAN_LIST, "r");
        
        if (!fp) return;
        
        name = fread_word(fp);
  
        while (str_cmp(name, END_MARKER)) {
                p = alloc_mem(sizeof(BAN_DATA));
                p->name = str_dup(name);
                p->reason = fread_string(fp);
                p->next = ban_list;
                ban_list = p;
                name = fread_word(fp);}
                
        fclose(fp);
}        


void save_bans()
{
        FILE *fp;
        BAN_DATA *p;  

        if (!ban_list)       {
                unlink (BAN_LIST);
                return;
        }

        fp = fopen (BAN_LIST, "w");

        if (!fp) {bug("could not open ban.txt",0);
                return;}
       for (p = ban_list; p; p = p->next)
       {
         fprintf(fp, "%s\n", p->name);
         fprintf(fp, "%s~\n", p->reason);
       }
        
        fprintf (fp, "%s\n",END_MARKER);
        
        fclose (fp);
}




