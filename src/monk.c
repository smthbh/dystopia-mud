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
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"


void do_monkarmor( CHAR_DATA *ch, char *argument )
{
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];
  int vnum = 0;

  argument = one_argument( argument, arg );
  if (IS_NPC(ch)) return;
 	if(!IS_IMMORTAL(ch))
	{
    if (!IS_CLASS(ch, CLASS_MONK))
    {
      send_to_char("What?\n\r",ch);
      return;
    }
	}
  if (arg[0] == '\0')
  {
    send_to_char("Please specify which piece of monk armor you wish to make: Ring Collar Robe Helmet Shorts Boots Gloves Sleeves Cloak Belt Bracer Mask.\n\r",ch);
    return;
  }
  if ( ch->practice <60 )
  {
    send_to_char("It costs 60 points of primal to create monk equipment.\n\r",ch);
    return;
  }
  if      (!str_cmp(arg,"bracer"     )) vnum = 33029;
  else if (!str_cmp(arg,"cloak"   )) vnum = 33027;
  else if (!str_cmp(arg,"gloves"    )) vnum = 33025;
  else if (!str_cmp(arg,"ring"    )) vnum = 33020;
  else if (!str_cmp(arg,"collar"    )) vnum = 33021;
  else if (!str_cmp(arg,"robe"    )) vnum = 33022;
  else if (!str_cmp(arg,"helmet"    )) vnum = 33024;
  else if (!str_cmp(arg,"boots"    )) vnum = 33031;
  else if (!str_cmp(arg,"shorts"    )) vnum = 33023;
  else if (!str_cmp(arg,"sleeves"    )) vnum = 33026;
  else if (!str_cmp(arg,"belt"    )) vnum = 33028;
  else if (!str_cmp(arg,"mask"    )) vnum = 33030;
  else
  {
    do_monkarmor(ch,"");
    return;
  }
  if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
  {
    send_to_char("Missing object, please inform a God.\n\r",ch);
    return;
  }
  obj = create_object(pObjIndex, 50);
  obj->questowner = str_dup(ch->pcdata->switchname);
  obj_to_char(obj, ch);
  ch->practice -= 60;
  act("$p appears in your hands.",ch,obj,NULL,TO_CHAR);
  act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
  return;
}

void do_deathtouch( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;
  if ( !IS_CLASS(ch, CLASS_MONK) )
  {
    stc("Huh?\n\r",ch);
    return;
  }
  if ( ch->monkab[SPIRIT] < 4 )
  {
    stc("You need to obtain level 4 spirit to use Death touch.\n\r",ch);
    return;
  }
  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobiles.\n\r",ch);
    return;
  }
  if (is_safe(ch,victim) ) return;
  if ( victim == ch )
  {
    stc("That doesn't seem like a good idea.\n\r",ch);
    return;
  }
  WAIT_STATE(ch, 12);
  act("You place your hands on $N's head and channel negative energy into $m.",ch,NULL,victim,TO_CHAR);
  act("$n places $s hands on your head, and you scream in utter pain.",ch,NULL,victim,TO_VICT);
  act("$n places $s hands on $N's head and $N screams in pain.",ch,NULL,victim,TO_ROOM);
  SET_BIT(victim->monkstuff, MONK_DEATH);
  return;
}

void do_healingtouch( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;
  if ( !IS_CLASS(ch, CLASS_MONK) )
  {
    stc("Huh?\n\r",ch);
    return;
  }
  if ( ch->monkab[SPIRIT] < 3 )
  {
    stc("You need to obtain level 3 spirit to use Healing touch.\n\r",ch);
    return;
  }
  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobiles.\n\r",ch);
    return;
  }
  WAIT_STATE(ch, 12);
  
  if ( victim == ch )
  {
    stc("You focus your energy, and magical sparks leap out of your body.\n\r",ch);
    act("$n concentrates, and magical sparks leap out of $s body.",ch,NULL,NULL,TO_ROOM);
    SET_BIT(ch->monkstuff, MONK_HEAL);
    return;
  }
  act("You place your hands on $N's head and focus your energy on $M.",ch,NULL,victim,TO_CHAR);
  act("$n places $s hands on your head and you feel warmer.",ch,NULL,victim,TO_VICT);
  act("$n places $s hands on $N's head and concentrates.",ch,NULL,victim,TO_ROOM);
  SET_BIT(victim->monkstuff, MONK_HEAL);
  return;
}

void do_spiritpower( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MONK) || ch->monkab[BODY] < 3)
  {
    stc("Huh?\n\r",ch);
    return;
  }
  if ( IS_SET(ch->newbits, NEW_POWER) )
  {
    stc("Your spiritual power fades.\n\r",ch);
    ch->damroll -= 200;
    ch->hitroll -= 200;
    REMOVE_BIT(ch->newbits, NEW_POWER);
    return;
  }
  if ( ch->move < 100 )
  {
    stc("You are too exhausted.\n\r",ch);
    return;
  }
  SET_BIT(ch->newbits, NEW_POWER);
  ch->damroll += 200;
  ch->hitroll += 200;
  ch->move -= 25;
  stc("Your body pulses with spiritual energy.\n\r",ch);
  return;
}
    
void do_relax( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch) ) return;
  if ( !IS_CLASS(ch, CLASS_MONK) )
  {
    stc("Huh?\n\r",ch);
    return;
  }
  if ( ch->chi[CURRENT] < 1 )
  {
    stc("You are already totally relaxed.\n\r",ch);
    return;
  }
  if ( ch->position == POS_FIGHTING )
  {
    stc("You cannot relax your ch'i while fighting!\n\r",ch);
    return;
  }
  ch->chi[CURRENT]--;
  WAIT_STATE(ch, 12);
  stc("You breathe deeply and relax your focus.\n\r",ch);
  act("$n looks more relaxed.",ch,NULL,NULL,TO_ROOM);
  return;
}

void do_chi( CHAR_DATA *ch, char *argument )
{
  char buf[MAX_STRING_LENGTH];

  if (IS_NPC(ch) ) return;
  if (!IS_CLASS(ch, CLASS_MONK) )
  {
    stc("Huh?\n\r",ch);
    return;
  }
  if ( ch->fighting == NULL )
  {
    stc("You must be fighting to focus your ch'i.\n\r",ch);
    return;
  }

  if ( ch->chi[CURRENT] >= ch->chi[MAXIMUM] )
  {
    if (ch->chi[MAXIMUM] == 0) stc("You have not gained any control of your ch'i.\n\r",ch);
    else
    {
      sprintf(buf,"You cannot focus your ch'i past a level of %d.",ch->chi[MAXIMUM]);
      stc(buf,ch);
    }
    return;
  }
  if ( ch->move < 500 + ((ch->chi[CURRENT]+1) * 20) )
  {
    stc("You are too exhausted.\n\r",ch);
    return;
  }
  if ( ch->chi[CURRENT] == 0 )
  {
    stc("Your body flickers with energy.\n\r",ch);
    act("$n's body flickers with energy.",ch,NULL,NULL,TO_ROOM);
  }
  if ( ch->chi[CURRENT] == 1 )
  {
    stc("Your body pulses with energy.\n\r",ch);
    act("$n's body pulses with energy.",ch,NULL,NULL,TO_ROOM);
  }
  if ( ch->chi[CURRENT] == 2 )
  {
    stc("Your body glows with blue energy.\n\r",ch);
    act("$n's body glows with blue energy.",ch,NULL,NULL,TO_ROOM);
  }
  if ( ch->chi[CURRENT] == 3 )
  {
    stc("Your body glows bright red.\n\r",ch);
    act("$n's body glows bright red.",ch,NULL,NULL,TO_ROOM);
  }
  if ( ch->chi[CURRENT] == 4 )
  {
    stc("Your body flashes with power.\n\r",ch);
    act("$n's body flashes with power.",ch,NULL,NULL,TO_ROOM);
  }
  if ( ch->chi[CURRENT] == 5 )
  {
    stc("Your body emits sparks of energy as you fully focus your ch'i.\n\r",ch);
    act("$n's body emits sparks of energy.",ch,NULL,NULL,TO_ROOM);
  }
  WAIT_STATE(ch, 12);
  ch->chi[CURRENT]++;
  ch->move -= ((ch->chi[CURRENT] * 20));
  return;
}

void do_chands(CHAR_DATA *ch, char *argument )
{
  if(IS_NPC(ch)) return;
  if(!IS_CLASS(ch, CLASS_MONK))
  {
    stc("Huh?\n\r", ch);
    return;
  }
  
  if ( ch->pcdata->powers[PMONK] < 9 )
  {
    stc("You do not yet have this ability\n\r", ch);
    return;
  }

  if(IS_ITEMAFF(ch, ITEMA_CHAOSHANDS))
  {
    if(IS_ITEMAFF(ch, ITEMA_FIRESHIELD))
      REMOVE_BIT(ch->itemaffect, ITEMA_FIRESHIELD); 
    if(IS_ITEMAFF(ch, ITEMA_ACIDSHIELD))
      REMOVE_BIT(ch->itemaffect, ITEMA_ACIDSHIELD); 
    if(IS_ITEMAFF(ch, ITEMA_SHOCKSHIELD))
      REMOVE_BIT(ch->itemaffect, ITEMA_SHOCKSHIELD);  
    if(IS_ITEMAFF(ch, ITEMA_ICESHIELD))
      REMOVE_BIT(ch->itemaffect, ITEMA_ICESHIELD);  
    if(IS_ITEMAFF(ch, ITEMA_CHAOSSHIELD))
      REMOVE_BIT(ch->itemaffect, ITEMA_CHAOSSHIELD);  
    REMOVE_BIT(ch->itemaffect, ITEMA_CHAOSHANDS);
    act("The swirling colors around your hands disappear.",ch,NULL,NULL,TO_CHAR);
    act("The swirling colors around $n's hands disappear.",ch,NULL,NULL,TO_ROOM);
    return;
  }
  else
  {
    if(!IS_ITEMAFF(ch, ITEMA_FIRESHIELD))
      SET_BIT(ch->itemaffect, ITEMA_FIRESHIELD);  
    if(!IS_ITEMAFF(ch, ITEMA_ACIDSHIELD))
      SET_BIT(ch->itemaffect, ITEMA_ACIDSHIELD);  
    if(!IS_ITEMAFF(ch, ITEMA_SHOCKSHIELD))
      SET_BIT(ch->itemaffect, ITEMA_SHOCKSHIELD); 
    if(!IS_ITEMAFF(ch, ITEMA_ICESHIELD))
      SET_BIT(ch->itemaffect, ITEMA_ICESHIELD); 
    if(!IS_ITEMAFF(ch, ITEMA_CHAOSSHIELD))
      SET_BIT(ch->itemaffect, ITEMA_CHAOSSHIELD); 
    SET_BIT(ch->itemaffect, ITEMA_CHAOSHANDS);
    act("Swirling colors appear around your hands bringing up shields.",ch,NULL,NULL,TO_CHAR);
    act("Swirling colors appear around $n's hands.",ch,NULL,NULL,TO_ROOM);
    return;
  }
}

void do_guide( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg [MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MONK) )
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( arg[0] == '\0' )
  {
    send_to_char( "Guide whom?\n\r", ch );
    return;
  }
  if ((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char( "They aren't here.\n\r", ch );
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char( "Not on NPC's.\n\r", ch );
    return;
  }
  if (IS_IMMORTAL(victim))
  {
    send_to_char( "Not on Immortals's.\n\r", ch );
    return;
  }
  if (ch == victim)
  {
    send_to_char( "You cannot guide yourself.\n\r", ch );
    return;
  }

  if (IS_CLASS(victim, CLASS_MONK))
  {
    send_to_char( "They are already guided.\n\r", ch );
    return;
  }

  if (victim->level != LEVEL_AVATAR)
  {
    send_to_char( "You can only guide avatars.\n\r", ch );
    return;
  }
  if (victim->class != 0)
  {
    send_to_char("They already have a class.\n\r",ch);
    return;
  }
  if (!IS_IMMUNE(victim,IMM_VAMPIRE))
  {
    send_to_char( "You cannot guide an unwilling person.\n\r", ch );
    return;
  }
  if (ch->exp < 50000)
  {
    send_to_char("You cannot afford the 50000 exp required to guide them.\n\r",ch);
    return;
  }
  if (victim->exp < 50000)
  {
    send_to_char("They cannot afford the 50000 exp required to be guided from you.\n\r",ch);
    return;
  }
  
  ch->exp -= 50000;
  victim->exp -= 50000;

  act("You guide $N in the ways of god.", ch, NULL, victim, TO_CHAR);
  act("$n guide $N in the ways of god.", ch, NULL, victim, TO_NOTVICT);
  act("$n guide you in the way of god.", ch, NULL, victim, TO_VICT);
  victim->level = LEVEL_AVATAR;
  if (victim->trust > 3)
  victim->trust = victim->trust;
  else
  victim->trust = LEVEL_AVATAR;
  send_to_char( "You are now a monk.\n\r", victim );
  free_string(victim->lord);
  victim->lord = str_dup(ch->name);
  victim->class = CLASS_MONK;
  save_char_obj(ch);
  save_char_obj(victim);
  return;
}

void do_mantra( CHAR_DATA *ch, char *argument )
{
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MONK))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (arg1[0] == '\0' && arg2[0] == '\0')
  {
    sprintf(buf,"Mantras Learned (%d).\n\r", ch->pcdata->powers[PMONK]);
    send_to_char(buf,ch);
    send_to_char("Powers you have:\n\r", ch);
    if ( ch->pcdata->powers[PMONK] < 1 )
     send_to_char("You have no knowledge of any Mantras.\n\r", ch);
    if ( ch->pcdata->powers[PMONK] >= 1 )
     send_to_char("Eyes of God.\n\r", ch);
    if ( ch->pcdata->powers[PMONK] >= 2 )
     send_to_char("Shield/Read Aura/Scry\n\r", ch);
    if ( ch->pcdata->powers[PMONK] >= 3 )
     send_to_char("Sacred Invisibility\n\r", ch);
    if ( ch->pcdata->powers[PMONK] >= 4 )
     send_to_char("Wrath of God\n\r", ch);
    if ( ch->pcdata->powers[PMONK] >= 5 )
     send_to_char("Adamantium Hands\n\r", ch);
    if ( ch->pcdata->powers[PMONK] >= 6 )
     send_to_char("Skin of Steel\n\r", ch);
    if ( ch->pcdata->powers[PMONK] >= 7 )
     send_to_char("The Almighty Favor\n\r", ch);
    if ( ch->pcdata->powers[PMONK] >= 8 )
     send_to_char("Dark Blaze\n\r", ch);
    if ( ch->pcdata->powers[PMONK] >= 9 )
     send_to_char("Chaos Hands\n\r", ch);
    if ( ch->pcdata->powers[PMONK] >= 10 )
     send_to_char("Prayer of the Ages\n\r", ch);
    if ( ch->pcdata->powers[PMONK] >= 11 )
     send_to_char("Cloak of Life\n\r", ch);
    if ( ch->pcdata->powers[PMONK] >= 12 )
     send_to_char("Gods Heal.\n\r", ch);
    if ( ch->pcdata->powers[PMONK] >= 13 )
     send_to_char("Gods Hold.\n\r", ch);
    if ( ch->pcdata->powers[PMONK] >= 14 )
     send_to_char("You have completed your Mantra training.\n\r", ch);
  return;
  }
  if (!str_cmp(arg2,"improve"))
  {
    int improve;
    int cost;
    int max;

    if (!str_cmp(arg1,"power"   )) 
    {
      improve = PMONK;   
      max=14;
    }
    else
    {
      send_to_char("________________________________\n\r",ch);
      send_to_char("|                               |\n\r",ch);
      send_to_char("|             Mantras           |\n\r",ch);
      send_to_char("|_______________________________|\n\r",ch);
      send_to_char("| 1.        Eye of God          |\n\r",ch);
      send_to_char("| 2.   Shield/Read Aura/Scry    |\n\r",ch);
      send_to_char("| 3.    Sacred Invisibility     |\n\r",ch);
      send_to_char("| 4.       Wrath of God         |\n\r",ch);
      send_to_char("| 5.     Adamantium Hands       |\n\r",ch);
      send_to_char("| 6.       Flaming Hands        |\n\r",ch);
      send_to_char("| 7.       Skin of Steel        |\n\r",ch);
      send_to_char("| 8.    The Almighty's Favor    |\n\r",ch);
      send_to_char("| 9.        Dark Blaze          |\n\r",ch);
      send_to_char("| 10.     Celestial Path        |\n\r",ch);
      send_to_char("| 11.   Pray of the Ages        |\n\r",ch);
      send_to_char("| 12.     Cloak of Life         |\n\r",ch);
      send_to_char("| 13.      God's heal           |\n\r",ch);
      send_to_char("| 14.      God's hold           |\n\r",ch);
      send_to_char("|_______________________________|\n\r",ch);
      return;
    }             

    cost = (ch->pcdata->powers[improve]+1) * 10;
    arg1[0] = UPPER(arg1[0]);
    if ( ch->pcdata->powers[improve] >= max )
    {
      sprintf(buf,"You have already gained all the known Mantras.\n\r");
      send_to_char(buf,ch);
      return;
    }
    if ( cost > ch->practice )
    {
      sprintf(buf,"It costs you %d primal learn the Mantra.\n\r", cost);
      send_to_char(buf,ch);
      return;
    }

    ch->pcdata->powers[improve] += 1;
    ch->practice -= cost;
    sprintf(buf,"You learn a Mantra.\n\r");
    send_to_char(buf,ch);
  }
  else send_to_char("To learn a Mantra, type: Mantra power improve.\n\r",ch);
  return;
}

void do_cloak( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MONK))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[PMONK] < 11 )
  {
    send_to_char("You do not have the mantra of the Cloak of Life.\n\r",ch);
    return;
  }
  if (ch->move < 1000)
  {
    send_to_char("You do not have enough vitality to do that.\n\r",ch);
    return;
  }
  if (IS_SET(ch->newbits, NEW_MONKCLOAK))
  {
    REMOVE_BIT(ch->newbits, NEW_MONKCLOAK);
    send_to_char("You are no longer protected by the Almighty's cloak.\n\r",ch);
    act("$n is no longer protected by the Almighty's cloak of life.\n\r",ch,NULL,NULL,TO_ROOM);
    return;
  }
  if (!IS_SET(ch->newbits,NEW_MONKCLOAK))
  {
    SET_BIT(ch->newbits, NEW_MONKCLOAK);
    send_to_char("You are protected by the Almighty's cloak.\n\r",ch);
    act("$n is protected by the Almighty's cloak of life.\n\r",ch,NULL,NULL,TO_ROOM);
    ch->move = ch->move - 1000;
    return;
  }
}

void do_prayofages( CHAR_DATA *ch, char *argument )
{
  char arg [MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );
/*    
  ***            Useless                     ***
  ** Used to be some sort of teleport command **
  ***                                        ***
*/  

  send_to_char("No effect, this command does nothing, your welcome to post ideas for something it could do.\n\r", ch);
  return;
}

void do_sacredinvis( CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MONK))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[PMONK] < 3)
  {
    send_to_char("You do not have the mantra of Sacred Invisibility.\n\r",ch);
    return;
  }
  if (ch->fight_timer >0) {
    send_to_char("Not until your fight timer expires.\n\r", ch );
    return;
  }
  if ( IS_SET(ch->act, AFF_HIDE) )
  {
    REMOVE_BIT(ch->act, AFF_HIDE);
    send_to_char( "You emerge from your cloak of invisibility.\n\r", ch );
    act("$n slowly emerges from a cloak of holy invisibility.",ch,NULL,NULL,TO_ROOM);
  }
  else
  {
    if (ch->move < 500) 
    {
      send_to_char("You do not have enough movement to do this.\n\r", ch );
      return;
    }
    send_to_char( "You slowly fade into a cloak of invisibility.\n\r", ch );
    act("$n slowly fades into a cloak of holy invisibility..",ch,NULL,NULL,TO_ROOM);
    SET_BIT(ch->act, AFF_HIDE);
    ch->move -= 500;
  }
  return;
}

void do_flaminghands( CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MONK))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[PMONK] <5)
  {
    send_to_char("You do not have the mantra of Flaming Hands.\n\r",ch);
    return;
  }
  if (IS_SET(ch->newbits, NEW_MONKFLAME))
  {
    REMOVE_BIT(ch->newbits, NEW_MONKFLAME);
    send_to_char("Your hands are no longer engulfed by flames.\n\r",ch);
    act("$n's hands are no longer engulfed by flames.\n\r",ch,NULL,NULL,TO_ROOM);
    return;
  }
  if (!IS_SET(ch->newbits, NEW_MONKFLAME))
  {
    SET_BIT(ch->newbits, NEW_MONKFLAME);
    send_to_char("Your hands are engulfed by flames!\n\r",ch);
    act("$n's hands are engulfed by flames!\n\r",ch,NULL,NULL,TO_ROOM);
    return;
  }
}

void do_adamantium( CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MONK))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->monkab[BODY] < 1)
  {
    send_to_char("You have not learned this ability yet.\n\r",ch);
    return;
  }
  if (IS_SET(ch->newbits, NEW_MONKADAM))
  {
    REMOVE_BIT(ch->newbits, NEW_MONKADAM);
    send_to_char("Your hands resume their normal toughness.\n\r",ch);
    act("$n's hands resume their normal toughness.\n\r",ch,NULL,NULL,TO_ROOM);
    return;
  }
  if (!IS_SET(ch->newbits, NEW_MONKADAM))
  {
    SET_BIT(ch->newbits, NEW_MONKADAM);
    send_to_char("Your hands turn as hard as adamantium!\n\r",ch);
    act("$n's hands turn as hard as adamantium!\n\r",ch,NULL,NULL,TO_ROOM);
    return;
  }
}


void do_celestial( CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_INPUT_LENGTH];
  argument = one_argument(argument, arg);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MONK))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[PMONK] < 10)
  {
    send_to_char("You must obtain level 10 in mantras to use Celestial Path.\n\r",ch);
    return;
  }
  if (ch->move < 250)
  {
    stc("You do not have enough vitality to do that!\n\r",ch);
    return;
  }
  if (arg[0] == '\0')
  {
    send_to_char( "Open a celestial path to who?\n\r", ch );
    return;
  }
  if ( ( victim = get_char_world( ch, arg ) ) == NULL )
  {
    send_to_char( "They aren't here.\n\r", ch );
    return;
  }
  if( victim == ch )
  {
    stc ( "Not to yourself.\n\r",ch);
    return;
  }
  if( IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "You can't find it's room.\n\r",ch);
    return;
  }
  if( IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "Your room is not connected to the astral plane.\n\r",ch);
    return;
  }
  if  (!IS_NPC(victim) && !IS_IMMUNE(victim,IMM_SUMMON) )
  {
    send_to_char("There summon is off!\n\r",ch);
    return;
  }
  if ( room_is_private(victim->in_room ) )
  {    
    send_to_char( "That room is private right now.\n\r", ch );    
    return;    
  }    
  if (ch == victim)
  {
    send_to_char("But you're already at yourself!\n\r",ch);
    return;
  }
  if (victim->in_room->vnum == ch->in_room->vnum)
  {
    send_to_char("But you're already there!\n\r",ch);
    return;
  }

  char_from_room(ch);
  char_to_room(ch,victim->in_room);
  if (IS_NPC(victim))
  {
    sprintf(buf,"You concentrate and open a Celestial path to %s!\n\r",victim->short_descr);
    send_to_char(buf, ch);
  }
  if (!IS_NPC(victim))
  {
    sprintf(buf,"You concentrate and open a Celestial path to %s!\n\r",victim->name);
    send_to_char(buf, ch);
  }
  act("$n appears before you through a Celestial portal!\n\r",ch,NULL,NULL,TO_ROOM);
  do_look(ch, "auto");
  ch->move -= 250;
  return;
}

void do_godseye( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if ( !IS_CLASS(ch, CLASS_MONK) )
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if ( ch->pcdata->powers[PMONK] < 1 )
  {
    send_to_char("You have not learned the Eyes of God Mantra.\n\r", ch);
    return;
  }
  if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
  {
    REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
    send_to_char( "Your view shimmers into mortal vision.\n\r", ch );
  }
  else
  {
    send_to_char( "God answers your prayers and gives you his vision.\n\r", ch );
    SET_BIT(ch->act, PLR_HOLYLIGHT);
  }
  return;
}

void do_steelskin( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MONK))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if (ch->pcdata->powers[PMONK] < 6)
  {
    send_to_char("You have not learned the Steel of Skin Mantra.\n\r", ch);
    return;
  }
  if ( IS_SET(ch->newbits, NEW_MONKSKIN) )
  {
    REMOVE_BIT( ch->newbits, NEW_MONKSKIN );
    send_to_char( "Your steel shield disappears.\n\r", ch );
  }
  else
  {
    send_to_char( "You are surrounded by a steel shield\n\r", ch );
    SET_BIT(ch->newbits, NEW_MONKSKIN);
  }
  return;
}

void do_godsbless( CHAR_DATA *ch, char *argument )
{
  int sn;
  int level;
  int spelltype;

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MONK))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if ( IS_CLASS(ch, CLASS_MONK) && ch->pcdata->powers[PMONK] < 7 )
  {
    send_to_char("You have not learned the Almighty Favor Mantra.\n\r", ch);
    return;
  }
  if (ch->mana < 3000) 
  {
    send_to_char("You don't have enough mana.\n\r", ch );
    return;
  }
  if ( ( sn = skill_lookup( "godbless" ) ) < 0 ) return;
  spelltype = skill_table[sn].target;
  level = 500;
  (*skill_table[sn].spell_fun) ( sn, level, ch, ch );
  WAIT_STATE( ch, 12 );
  ch->mana = ch->mana - 3000;
  return;
}


void do_wrathofgod( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;

  if (IS_NPC(ch)) return; 
  if (!IS_CLASS(ch, CLASS_MONK))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( IS_CLASS(ch, CLASS_MONK) && ch->pcdata->powers[PMONK] < 4 )
  {
    send_to_char("You have not learned to call on the wrath of god yet.\n\r", ch);
    return;
  }
  if (ch->fighting != NULL) victim = ch->fighting;
  else return;
  if (!IS_NPC(victim)) return;
  one_hit(ch,victim,gsn_wrathofgod,1);
  one_hit(ch,victim,gsn_wrathofgod,1);
  one_hit(ch,victim,gsn_wrathofgod,1);
  one_hit(ch,victim,gsn_wrathofgod,1);
  WAIT_STATE(ch,8);
  return;
}


void do_godsfavor( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return; 
  if (!IS_CLASS(ch, CLASS_MONK))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( IS_CLASS(ch, CLASS_MONK) && ch->pcdata->powers[PMONK] < 8 )
  {
    send_to_char("You have not learned the Almighty's Favor Mantra.\n\r", ch);
    return;
  }
  if (ch->move < 1500)
  {
    send_to_char("You do not have the vitality to do that!\n\r",ch);
    return;
  }
  if (!IS_SET(ch->newbits, NEW_MONKFAVOR))
  {
    SET_BIT(ch->newbits, NEW_MONKFAVOR);
    send_to_char("God smiles upon you and you feel strengthened.\n\r",ch);
    act("$n is blessed by the Almighty.",ch,NULL,NULL,TO_ROOM);
    ch->move = ch->move - 1500;
    WAIT_STATE(ch, 4);
    return;
  }
  if (IS_SET(ch->newbits, NEW_MONKFAVOR))
  {
    REMOVE_BIT(ch->newbits, NEW_MONKFAVOR);
    send_to_char("You feel weaker as The Almighty's favor leaves you.\n\r",ch);
    act("$n no longer looks so holy.",ch,NULL,NULL,TO_ROOM);
    return;
  }
}

void do_darkblaze( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  AFFECT_DATA af;  

  
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MONK))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if ( IS_CLASS(ch, CLASS_MONK) && ch->pcdata->powers[PMONK] < 8 )
  {
    send_to_char("You have not learned the Dark Blaze Mantra.\n\r", ch);
    return;
  }
  if (ch->fighting != NULL) victim = ch->fighting;
  else return;
  if (IS_SET(victim->act,PLR_HOLYLIGHT))
  {
    REMOVE_BIT(victim->act, PLR_HOLYLIGHT);
  }
  if (IS_SET(victim->affected_by, AFF_DETECT_HIDDEN))
    REMOVE_BIT(victim->affected_by, AFF_DETECT_HIDDEN);
  if (IS_SET(victim->affected_by, AFF_DETECT_INVIS))
    REMOVE_BIT(victim->affected_by, AFF_DETECT_INVIS);
  af.type      = skill_lookup("blindness");
  af.location  = APPLY_HITROLL;
  af.modifier  = -4; 
  af.duration  = 60;
  af.bitvector = AFF_BLIND;
  affect_to_char( victim, &af );
  send_to_char("Your hands blaze up, blinding your opponent.\n\r",ch);
  send_to_char("Your opponents hands flashes with a blinding light.\n\r",victim);
  WAIT_STATE(ch,18);
  return;
} 


void do_godsheal( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if( !IS_CLASS(ch, CLASS_MONK) )
  {
    stc( "Huh?\n\r",ch);
    return;
  }
  if ( IS_CLASS(ch, CLASS_MONK) && ch->pcdata->powers[PMONK] < 12 )
  {
    send_to_char("You need to obtain heal to use heal.\n\r",ch);
    return;
  }
  if (ch->mana < 300)
  {
    send_to_char("You don't have enough energy to use heal.\n\r",ch);
    return;
  }
  if (ch->fighting != NULL)
  {
    send_to_char("You are surrounded in a glowing halo of energy.\n\r",ch);
    act("$n is surrounded by a glowing halo, and looks very relaxed.\n\r",ch,NULL,NULL,TO_ROOM);
    ch->hit += 150;
    if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
    ch->mana -= 400;
    WAIT_STATE(ch,12);
    return;
  }
  if (ch->fighting == NULL)
  {
    send_to_char("You feel a mighty force lay his hands on you.\n\r",ch);
    act("$n is surrounded by a glowing halo, and looks very relaxed.\n\r",ch,NULL,NULL,TO_ROOM);
    ch->hit += 500;
    if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
    ch->mana -= 400;
    WAIT_STATE(ch,8);
    return;
  }
}

void do_ghold( CHAR_DATA *ch,char *argument )
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch,CLASS_MONK))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( IS_CLASS(ch, CLASS_MONK) && ch->pcdata->powers[PMONK] < 13 )
  {
    send_to_char("You need Mantra 13 to use gods hold.\n\r",ch);
    return;
  }
  if (!IS_SET(ch->newbits, NEW_JAWLOCK))
  {
    send_to_char("God joins you at your side to hold your foe.\n\r",ch);
    SET_BIT(ch->newbits, NEW_JAWLOCK);
    return;
  }
  else if (IS_SET(ch->newbits, NEW_JAWLOCK))
  { 
    send_to_char("God leaves your side.\n\r",ch);
    REMOVE_BIT(ch->newbits,NEW_JAWLOCK);
    return;
  }
}
