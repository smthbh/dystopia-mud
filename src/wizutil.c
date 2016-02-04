/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvements copyright (C) 1992, 1993 by Michael         *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefiting.  We hope that you share your changes too.  What goes       *
 *  around, comes around.                                                  *
 ***************************************************************************/


#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#if !defined(WIN32)
#include <unistd.h>
#endif
#include <string.h>
#include "merc.h"

/*
===========================================================================
This snippet was written by Erwin S. Andreasen, erwin@pip.dknet.dk. You may 
use this code freely, as long as you retain my name in all of the files. You
also have to mail me telling that you are using it. I am giving this,
hopefully useful, piece of source code to you for free, and all I require
from you is some feedback.

Please mail me if you find any bugs or have any new ideas or just comments.

All my snippets are publically available at:

http://pip.dknet.dk/~pip1773/

If you do not have WWW access, try ftp'ing to pip.dknet.dk and examine
the /pub/pip1773 directory.

===========================================================================
*/

extern ROOM_INDEX_DATA *       room_index_hash         [MAX_KEY_HASH];
typedef enum {exit_from, exit_to, exit_both} exit_status;
const sh_int opposite_dir [6] = { DIR_SOUTH, DIR_WEST, DIR_NORTH, DIR_EAST, DIR_DOWN, DIR_UP };

/*
 * get the 'short' name of an area (e.g. MIDGAARD, MIRROR etc.
 * assumes that the filename saved in the AREA_DATA struct is something like midgaard.are
 */
char * area_name (AREA_DATA *pArea)
{
  static char buffer[64];
  char  *period;

  assert (pArea != NULL);

  strncpy (buffer, pArea->filename, 64);
  period = strchr (buffer, '.');
  if (period)
    *period = '\0';
  return buffer;
}

void room_pair (ROOM_INDEX_DATA* left, ROOM_INDEX_DATA* right, exit_status ex, char *buffer)
{
  char *sExit;

  switch (ex)
  {
    default:
      sExit = "??"; break; /* invalid usage */
    case exit_from:
      sExit = "< "; break;
    case exit_to:
      sExit = " >"; break;
    case exit_both:
      sExit = "<>"; break;
  }
  sprintf (buffer, "%5d %-26.26s %s%5d %-26.26s(%-8.8s)\n\r",
    left->vnum, left->name, sExit, right->vnum, right->name, area_name(right->area));
  return;
}

/* for every exit in 'room' which leads to or from pArea but NOT both, print it */
void checkexits (ROOM_INDEX_DATA *room, AREA_DATA *pArea, char* buffer)
{
  char buf[MAX_STRING_LENGTH];
  int i;
  EXIT_DATA *exit;
  ROOM_INDEX_DATA *to_room;

  strcpy(buffer, "");

  for (i = 0; i < 6; i++)
  {
    exit = room->exit[i];
    if (!exit) continue;
    else to_room = exit->to_room;
    if (to_room)
    {
      if ( (room->area == pArea) && (to_room->area != pArea) )
      {
        if ( to_room->exit[opposite_dir[i]] && to_room->exit[opposite_dir[i]]->to_room == room )
          room_pair (room,to_room,exit_both,buf); /* <> */
        else
          room_pair (room,to_room,exit_to,buf); /* > */
        strcat (buffer, buf);				
      }			
      else			
      {
        if ( (room->area != pArea) && (exit->to_room->area == pArea) )
        { /* an exit from another area to our area */
          if  (!(to_room->exit[opposite_dir[i]] && to_room->exit[opposite_dir[i]]->to_room == room ))
          {/* two-way exits are handled in the other if */
            room_pair (to_room,room,exit_from,buf);
            strcat (buffer, buf);
          }
        } /* if room->area */
      } /* for */
    }
  }
  return;
}

/* for now, no arguments, just list the current area */

void do_exlist (CHAR_DATA *ch, char * argument)
{
  AREA_DATA* pArea;
  ROOM_INDEX_DATA* room;
  int i;
  char buffer[MAX_STRING_LENGTH];

  pArea = ch->in_room->area;
  for (i = 0; i < MAX_KEY_HASH; i++)
  {
    for (room = room_index_hash[i]; room != NULL; room = room->next)
    {
      checkexits (room, pArea, buffer);
      send_to_char (buffer, ch);
    }
  }
  return;
}

void set_switchname( CHAR_DATA *ch, char *title )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
    {
        bug( "Set_switchname: NPC.", 0 );
        return;
    }
    strcpy( buf, title );
    free_string( ch->pcdata->switchname );
    ch->pcdata->switchname = str_dup( buf );
    return;
}

void set_pc_name( CHAR_DATA *ch, char *title )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
    {
        bug( "Set_pc_name: NPC.", 0 );
        return;
    }
    strcpy( buf, title );
    free_string( ch->name);
    ch->name = str_dup( buf );
    return;
}

void do_classself(CHAR_DATA *ch, char *argument)
{
  char arg1[MAX_STRING_LENGTH];
  argument = one_argument( argument, arg1 );

  if (IS_NPC(ch))return;

  if (!IS_NPC(ch) && ch->class != 0)
  {   
    send_to_char("You already have a class.\n\r",ch);
    return;
  }
  if (ch->level < 3)
  {
    send_to_char("You must be avatar to selfclass.\n\r",ch);
    return;
  }
if ( arg1[0] == '\0' )
{
  send_to_char("Classes: Type selfclass <class> to get classed.\n\r\n\r",ch);
  send_to_char("#R[#0Demon#R]#n             #y((#LWerewolf#y))#n         #P.o0#0Drow#P0o.#n\n\r",ch);
  send_to_char("#C***#yNinja#C***#n         #0<<#RVampire#0>>#n          #0.x[#lMonk#0]x.\n\r",ch);
  send_to_char("#n{{#CBattlemage#n}}\n\r", ch);
  return;
}
do_clearstats2(ch,"");
if (!str_cmp(arg1,"demon"))
{
ch->class = CLASS_DEMON;
set_learnable_disciplines(ch);
send_to_char( "You have chosen the #RDemonic#n path, may god have mercy on yer soul.\n\r", ch);
}
 
else if (!str_cmp(arg1,"werewolf"))
{
ch->class = CLASS_WEREWOLF;
set_learnable_disciplines(ch);
send_to_char( "You have chosen the path of the #0Garou#n, may gaia guide you.\n\r", ch);
}
      
else if (!str_cmp(arg1,"drow"))
{
ch->class = CLASS_DROW;
send_to_char( "Choose your profession, and #PLloth#n will guide you.\n\r", ch);
}  
    
else if (!str_cmp(arg1,"ninja"))
{
ch->class = CLASS_NINJA;
send_to_char( "You have chosen a life in the #0shadows#n, assassinate at will.\n\r", ch);
}

else if (!str_cmp(arg1,"vampire"))
{
ch->class = CLASS_VAMPIRE;
ch->beast = 30;
set_learnable_disciplines(ch);
send_to_char( "Fear the #ySun#n nosferatu, God's curse lives in you.\n\r", ch);
}
 
else if (!str_cmp(arg1,"monk"))
{
ch->class = CLASS_MONK;
ch->level = 3;
send_to_char( "You faith in God will guide you, destroy #7EVIL#n.\n\r", ch);
}
 
else if (!str_cmp(arg1,"mage") || !str_cmp(arg1,"battlemage"))
{
  if (ch->max_mana >= 5000 && ch->spl[RED_MAGIC]>99 && ch->spl[BLUE_MAGIC]>99
  && ch->spl[YELLOW_MAGIC]>99 && ch->spl[GREEN_MAGIC]>99 && ch->spl[PURPLE_MAGIC]>99)
  {
    ch->class = CLASS_MAGE;
    ch->level = 3;
    send_to_char( "You start down the path of power, the #Rarcane#n is your weapon.\n\r", ch);
  }
  else
  {
    send_to_char("You need 5K mana and 100 in all your spellcolors.\n\r",ch);
    return;
  }
}
  else do_classself(ch,"");   
  return;
}
 
void do_reimb(CHAR_DATA *ch, char *argument)
{
        CHAR_DATA       *vch;
        char            arg[MAX_INPUT_LENGTH];
        char            arg2[MAX_INPUT_LENGTH];
        int v;
 
        argument = one_argument(argument, arg);
        if (arg[0] == '\0') {
                send_to_char("Reimburse <target> <xp|qp> <amount>\n\r", ch);
                return;
        }
        if ((vch = get_char_world(ch, arg)) == NULL) {
                send_to_char("They aren't logged on.\n\r", ch);
                return;
        }
        if (IS_NPC(vch)) {
                send_to_char("That is a mob.\n\r", ch);
                return;
        }
        argument = one_argument(argument, arg);
        argument = one_argument(argument, arg2);
        v = atoi(arg2);
        if (arg[0] == '\0' || arg2[0] == '\0'
          || (!is_number(arg2) && v >= 0)) {
                do_reimb(ch, "");
                return;
        }
        if (!str_cmp(arg, "xp")) {
                vch->exp += v;
                vch->pcdata->score[SCORE_TOTAL_XP] += v;
        } else if (!str_cmp(arg, "qp")) {   
                vch->pcdata->quest += v;
                vch->pcdata->questtotal += v;
        } else {
                send_to_char("Please specify XP or QP.\n\r", ch);
                return;
        }
        if (vch->mkill < 5) {
                vch->mkill = 5;
                do_autosave(ch, "");
        }
        sprintf(arg2, "%s reimbursed %d %s.\n\r", vch->name, v, arg);
        send_to_char(arg2, ch);
        sprintf(arg2, "%s has reimbursed you %d %s.\n\r", ch->name, v, arg);
        send_to_char(arg2, vch);
}
        
void do_affects( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   AFFECT_DATA *paf;
        
     if (IS_NPC(ch)) return;
        
     if (IS_ITEMAFF(ch, ITEMA_ICESHIELD))   
       send_to_char("#CIceshield#n.\n\r",ch);
     if (IS_ITEMAFF(ch, ITEMA_SHOCKSHIELD))
       send_to_char("#LLightningshield#n.\n\r",ch);
     if (IS_ITEMAFF(ch, ITEMA_ACIDSHIELD))
       send_to_char("#GAcidshield#n.\n\r",ch);
     if (IS_ITEMAFF(ch, ITEMA_FIRESHIELD))
       send_to_char("#RFireshield#n.\n\r",ch);
     if (IS_ITEMAFF(ch, ITEMA_CHAOSSHIELD))
       send_to_char("#pChaosshield#n.\n\r",ch);
     if (IS_AFFECTED(ch, AFF_SANCTUARY))
       send_to_char("#7Sanctuary#n.\n\r",ch);
     if ( IS_AFFECTED(ch, AFF_PROTECT))
       send_to_char("#LProtection from evil#n.\n\r",ch);
     if ( IS_AFFECTED(ch, AFF_PROTECT_GOOD))
       send_to_char("#LProtection from good#n.\n\r",ch);
     if ( IS_AFFECTED(ch, AFF_FLYING))
       send_to_char("#cFly#n\n\r",ch);
     send_to_char("\n\r",ch);
     if ( ch->affected == NULL )
        {
                send_to_char( "You have nothing affecting you at this time.\n\r", ch);
        return;
        }
     if ( ch->affected != NULL )
       {
   
         send_to_char( "You are affected by:\n\r", ch );
         for ( paf = ch->affected; paf != NULL; paf = paf->next )
            {
               sprintf( buf, "Spell: '%s'", skill_table[paf->type].name );
               send_to_char( buf, ch );     
               if ( ch->level >= 0 )
                   {
   sprintf( buf, " modifies %s by %d for %d hours with bits %s.\n\r",
                      affect_loc_name( paf->location ),
                      paf->modifier,
                      paf->duration,
                      affect_bit_name( paf->bitvector ) );
                      send_to_char( buf, ch );
                   }
     
            }
     
     }
     
     return;
     
}
char * plr_bit_name( int arg )
{
    static char buf[512];
                      
    buf[0] = '\0';
                      
    if ( arg & PLR_IS_NPC       ) strcat( buf, " npc"           );
    if ( arg & PLR_AUTOEXIT     ) strcat( buf, " autoexit"      );
    if ( arg & PLR_AUTOLOOT     ) strcat( buf, " autoloot"      );
    if ( arg & PLR_AUTOSAC      ) strcat( buf, " autosac"       );
    if ( arg & PLR_BLANK        ) strcat( buf, " blank"         );
    if ( arg & PLR_BRIEF        ) strcat( buf, " brief"         );
    if ( arg & PLR_COMBINE      ) strcat( buf, " combine"       );
    if ( arg & PLR_PROMPT       ) strcat( buf, " prompt"        );
    if ( arg & PLR_TELNET_GA    ) strcat( buf, " telnet_ga"     );
    if ( arg & PLR_HOLYLIGHT    ) strcat( buf, " holylight"     );
    if ( arg & PLR_WIZINVIS     ) strcat( buf, " wizinvis"      );
    if ( arg & PLR_ANSI         ) strcat( buf, " ansi"          );
    if ( arg & PLR_SILENCE      ) strcat( buf, " silenced"      );
    if ( arg & PLR_NO_TELL      ) strcat( buf, " no_tell"       );
    if ( arg & PLR_LOG          ) strcat( buf, " log"           );
    if ( arg & PLR_FREEZE       ) strcat( buf, " freeze"        );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}
   
char * extra_plr_bit_name( int arg )
{       
    static char buf[512];
        
    buf[0] = '\0';
               
    if ( arg & EXTRA_TRUSTED            ) strcat( buf, " q_trusted" );
    if ( arg & EXTRA_NEWPASS            ) strcat( buf, " newpass" );
    if ( arg & EXTRA_OSWITCH            ) strcat( buf, " oswitch" );
    if ( arg & EXTRA_SWITCH             ) strcat( buf, " switch" );
    if ( arg & EXTRA_FAKE_CON           ) strcat( buf, " fake_con" );
    if ( arg & TIED_UP                  ) strcat( buf, " tied_up" );
    if ( arg & GAGGED                   ) strcat( buf, " gagged" );
    if ( arg & BLINDFOLDED              ) strcat( buf, " blindfolded" );
    if ( arg & EXTRA_DONE               ) strcat( buf, " non_virgin" );
    if ( arg & EXTRA_EXP                ) strcat( buf, " got_exp" );
    if ( arg & EXTRA_PREGNANT           ) strcat( buf, " pregnant" );
    if ( arg & EXTRA_LABOUR             ) strcat( buf, " labour" );
    if ( arg & EXTRA_BORN               ) strcat( buf, " born" ); 
    if ( arg & EXTRA_PROMPT             ) strcat( buf, " prompt" );
    if ( arg & EXTRA_MARRIED            ) strcat( buf, " married" );
    if ( arg & EXTRA_CALL_ALL           ) strcat( buf, " call_all" );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}
   char * get_position_name( int arg )
{
    switch( arg )
    {
        case 0: return "dead";
        case 1: return "mortal";
        case 2: return "incap";
        case 3: return "stunned";   
        case 4: return "sleeping";
        case 5: return "meditating";
        case 6: return "sitting";
        case 7: return "resting";
        case 8: return "fighting";
        case 9: return "standing";
    }
    bug( "Get_position_name: unknown type %d.", arg );
    return "(unknown)";
}
    
/*
 * Itemaffect bit names :)
 */
char * itemaffect_bit_name( int arg )
{
    static char buf[512];
    
    buf[0] = '\0';
    
    if ( arg & ITEMA_SHOCKSHIELD        ) strcat( buf, " Shockshield"  );
    if ( arg & ITEMA_FIRESHIELD         ) strcat( buf, " Fireshield"    );
    if ( arg & ITEMA_ICESHIELD          ) strcat( buf, " Iceshield"     );
    if ( arg & ITEMA_ACIDSHIELD         ) strcat( buf, " Acidshield"    );
    if ( arg & ITEMA_CHAOSSHIELD        ) strcat( buf, " Chaoshield"    );
    if ( arg & ITEMA_ARTIFACT           ) strcat( buf, " Artifact"      );
    if ( arg & ITEMA_REGENERATE         ) strcat( buf, " Regeneration"  );
    if ( arg & ITEMA_SPEED              ) strcat( buf, " Speed"         );
    if ( arg & ITEMA_VORPAL             ) strcat( buf, " Vorpal"        );
    if ( arg & ITEMA_PEACE              ) strcat( buf, " Peace"         );
    if ( arg & ITEMA_RIGHT_SILVER       ) strcat( buf, " Right Silver"  );
    if ( arg & ITEMA_LEFT_SILVER        ) strcat( buf, " Left Silver"   );
    if ( arg & ITEMA_REFLECT            ) strcat( buf, " Darkshield"    );
    if ( arg & ITEMA_RESISTANCE         ) strcat( buf, " Resistance"    );
    if ( arg & ITEMA_VISION             ) strcat( buf, " Vision"        );
    if ( arg & ITEMA_STALKER            ) strcat( buf, " Stalker"       );
    if ( arg & ITEMA_VANISH             ) strcat( buf, " Vanish"        );
    if ( arg & ITEMA_RAGER              ) strcat( buf, " Rager"         );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}
 
/*  
 * code by Tijer
 */
void do_pstat ( CHAR_DATA *ch, char *argument )
{
    char        arg[MAX_INPUT_LENGTH];
    char        buf[MAX_STRING_LENGTH];
    CHAR_DATA   *victim;
    
    argument = one_argument( argument, arg );
    
    if ( arg[0] == '\0' )
    {
        send_to_char("Pstat whom?\n\r", ch );
        return;
    }
    
    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
       send_to_char("They aren't here.\n\r", ch );
        return;
    }
    
    sprintf( buf, "Name : %s.\n\r",
        IS_NPC( victim )        ? victim->short_descr : victim->name );   
  send_to_char( buf, ch );
    sprintf( buf, "Sex : %s. Room : %d. Align : %d. Primal : %d. Quest : %d.\n\r",
        victim->sex == SEX_MALE         ? "Male"        :
      victim->sex == SEX_FEMALE       ? "Female"      : "None",
        victim->in_room == NULL         ? 0             : victim->in_room->vnum,
        victim->alignment,
        victim->practice,
        IS_NPC( victim )                ? 0             : victim->pcdata->quest
        );
    send_to_char( buf, ch );
 
    sprintf( buf, "Level : %d. Trust : %d. Gold : %d. Exp : %d.\n\r",
        victim->level,
        victim->trust,  
        victim->gold,
        victim->exp );
   send_to_char( buf, ch );
    
       sprintf( buf, "Hit : %d. Dam : %d. AC : %d. Position : %s\n\r",
       char_hitroll( victim ),
        char_damroll( victim ),
        char_ac( victim ),
        capitalize( get_position_name( victim->position ) ));
    send_to_char( buf, ch );
     
    sprintf( buf, "HP %d/%d. Mana %d/%d. Move %d/%d.\n\r",
        victim->hit, victim->max_hit,
        victim->mana, victim->max_mana,
        victim->move, victim->max_move );
    send_to_char( buf, ch );
        
    sprintf( buf, "Str: %d.  Int: %d.  Wis: %d.  Dex: %d.  Con: %d.\n\r",
      get_curr_str(victim),
        get_curr_int(victim),
        get_curr_wis(victim),
        get_curr_dex(victim),
        get_curr_con(victim) );
    send_to_char( buf, ch );
        
    sprintf( buf, "Fighting : %s. (%d)\n\r",
        victim->fighting ? victim->fighting->name  : "(None)",
        victim->fighting ? victim->fighting->level : 0 );
   send_to_char( buf, ch );
        
    sprintf( buf, "Pkill : %d. Pdeath : %d. Mkill : %d. Mdeath : %d.\n\r",
        IS_NPC( victim ) ? 0 : victim->pkill,
        IS_NPC( victim ) ? 0 : victim->pdeath,
        IS_NPC( victim ) ? 0 : victim->mkill,
       IS_NPC( victim ) ? 0 : victim->mdeath );
    send_to_char( buf, ch );
       
    sprintf( buf, "TotExp  : %12d. TotMobLev  : %10d. TotQuestPoints : %10d.\n\r",
        IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_TOTAL_XP],
        IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_TOTAL_LEVEL],
        IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_QUEST] );
    send_to_char( buf, ch );
   
    sprintf( buf, "HighExp : %12d. HighMobLev : %10d. Tot#Quests     : %10d.\n\r",
      IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_HIGH_XP],
        IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_HIGH_LEVEL],
        IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_NUM_QUEST] );
    send_to_char( buf, ch );
    
    if ( !IS_NPC( victim ) )
   {
        sprintf( buf, "Unarmed : %4d.", victim->wpn[0] );
        send_to_char( buf, ch );
        sprintf( buf, " Slice   : %4d.", victim->wpn[1] );
        send_to_char( buf, ch );
        sprintf( buf, " Stab    : %4d.", victim->wpn[2] );
        send_to_char( buf, ch );
        sprintf( buf, " Slash   : %4d.", victim->wpn[3] );
        send_to_char( buf, ch );
        sprintf( buf, " Whip    : %4d.\n\r", victim->wpn[4] );
        send_to_char( buf, ch );
        sprintf( buf, "Claw    : %4d.", victim->wpn[5] );
        send_to_char( buf, ch );
        sprintf( buf, " Blast   : %4d.", victim->wpn[6] );
        send_to_char( buf, ch );
        sprintf( buf, " Pound   : %4d.", victim->wpn[7] );
send_to_char( buf, ch );
        sprintf( buf, " Crush   : %4d.", victim->wpn[8] );
        send_to_char( buf, ch );
        sprintf( buf, " Grep    : %4d.\n\r", victim->wpn[9] );
        send_to_char( buf, ch );
        sprintf( buf, "Bite    : %4d.", victim->wpn[10] );
     send_to_char( buf, ch );
        sprintf( buf, " Pierce  : %4d.", victim->wpn[11] );
        send_to_char( buf, ch );
        sprintf( buf, " Suck    : %4d.\n\r",victim->wpn[12] );
        send_to_char( buf, ch );
        
        sprintf( buf, "%-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %3d.\n\r",
            "Purple",   victim->spl[PURPLE_MAGIC],
            "Red",      victim->spl[RED_MAGIC],
            "Blue",     victim->spl[BLUE_MAGIC],
           "Green",    victim->spl[GREEN_MAGIC],
            "Yellow",   victim->spl[YELLOW_MAGIC] );
        send_to_char( buf, ch );
        sprintf( buf, "%-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %3d.\n\r",
            "Viper",    victim->stance[STANCE_VIPER],
            "Crane",    victim->stance[STANCE_CRANE],
            "Crab",     victim->stance[STANCE_CRAB],
            "Mongoose", victim->stance[STANCE_MONGOOSE],
            "Bull",     victim->stance[STANCE_BULL] );
        send_to_char( buf, ch );
        
        sprintf( buf, "%-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %-3d. %-8s : %3d.\n\r",
            "Mantis",   victim->stance[STANCE_MANTIS],
            "Dragon",   victim->stance[STANCE_DRAGON],
            "Tiger",    victim->stance[STANCE_TIGER],
            "Monkey",   victim->stance[STANCE_MONKEY],
           "Swallow",  victim->stance[STANCE_SWALLOW] );  
        
        send_to_char( buf, ch );
        sprintf( buf, "%-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %3d.\n\r",
            "ss1",   victim->stance[STANCE_SS1],
            "ss2",   victim->stance[STANCE_SS2],
            "ss3",    victim->stance[STANCE_SS3],
            "ss4",   victim->stance[STANCE_SS4],
            "ss5",  victim->stance[STANCE_SS5] );
        send_to_char( buf, ch );
        
        
         sprintf( buf, "Act         : %s\n\r", plr_bit_name(victim->act ));
        send_to_char( buf, ch );
        sprintf( buf, "Extra       : %s\n\r",   
        victim->extra <= 0 ? "(None)" : extra_plr_bit_name( victim->extra ) );
        send_to_char( buf, ch );
        sprintf( buf, "ItemAff     : %s\n\r",
        victim->itemaffect <= 0 ? "(None)" : itemaffect_bit_name(victim->itemaffect ) );
        send_to_char( buf, ch );
            
              sprintf( buf, "Affected by : %s.\n\r",
        affect_bit_name( victim->affected_by ) );
        send_to_char( buf, ch );
        
    return;
}
}

