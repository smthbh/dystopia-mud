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


void do_vampirearmor( CHAR_DATA *ch, char *argument )
{
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];
  int vnum = 0;

  argument = one_argument( argument, arg );
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_VAMPIRE))
  {
      send_to_char("What?\n\r",ch);
      return;
  }
  if (arg[0] == '\0')
  {
    send_to_char("Please specify which piece of vampire armor you wish to make: Ring Collar Plate Helmet Leggings Boots Gloves Sleeves Cape Belt Bracer Visor Dagger Longsword.\n\r",ch);
    return;
  }

    if ( ch->practice < 60 )
    {
        send_to_char("It costs 60 points of primal to create vampire equipment.\n\r",ch);
        return;
    }

    if      (!str_cmp(arg,"ring"	)) vnum = 33042;    
    else if (!str_cmp(arg,"plate"	)) vnum = 33044;
    else if (!str_cmp(arg,"helmet"	)) vnum = 33045;
    else if (!str_cmp(arg,"collar"	)) vnum = 33043;
    else if (!str_cmp(arg,"leggings" )) vnum = 33046;
    else if (!str_cmp(arg,"boots" )) vnum = 33047;
    else if (!str_cmp(arg,"gloves" )) vnum = 33048;
    else if (!str_cmp(arg,"sleeves" )) vnum = 33049;
    else if (!str_cmp(arg,"cape" )) vnum = 33050;
    else if (!str_cmp(arg,"belt" )) vnum = 33051;
    else if (!str_cmp(arg,"bracer" )) vnum = 33052;
    else if (!str_cmp(arg,"visor" )) vnum = 33053;
    else if (!str_cmp(arg,"longsword" )) vnum = 33040;
    else if (!str_cmp(arg,"dagger" )) vnum = 33041;
    else
    {
    do_vampirearmor(ch,"");
    return;
    }
    
    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
    send_to_char("Missing object, please inform a God.\n\r",ch);
    return;
    }

    ch->practice -= 60;
    obj = create_object(pObjIndex, 50);
    obj->questowner = str_dup(ch->pcdata->switchname);
    obj_to_char(obj, ch);
    act("$p appears in your hands.",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
    return;
}

void do_preserve( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    char arg [MAX_INPUT_LENGTH];
    
    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
    {
      stc("Huh?\n\r",ch);
      return;
    }

    if ( ch->power[DISC_VAMP_NECR] < 2 )
    {
      stc("You must obtain level 2 Necromancy to use Preserve.\n\r",ch);
      return;
    }

    if ( ( obj = get_obj_carry(ch, arg) ) == NULL )
    {
      stc("You do not have that item.\n\r",ch);
      return;
    }

    if ( obj->item_type == ITEM_HEAD )
    {
      stc("You cannot preserve that item.\n\r",ch);
      return;
    }

    if ( obj->timer <= 0 )
    {
      stc("That object has no timer.\n\r",ch);
      return;
    }

    obj->timer = -1;
    act("You place your hands on $p and concentrate on it.",ch,obj,NULL,TO_CHAR);
    act("$n places $s hands on $p and it glows brightly.",ch,obj,NULL,TO_ROOM);
    return;
}

void do_spiritguard( CHAR_DATA *ch, char *argument )
{
   
    if (IS_NPC(ch) ) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) )
    {
      stc("Huh?\n\r",ch);
      return;
    }

    if ( ch->power[DISC_VAMP_NECR] < 4 )
    {
      stc("You must obtain level 4 Necromancy to use Spirit Guardian.\n\r",ch);
      return;
    }

    if ( !IS_SET(ch->flag2, AFF_SPIRITGUARD) )
    {
      stc("You awaken your spirit guardian.\n\r",ch);
      SET_BIT(ch->flag2, AFF_SPIRITGUARD);
      return;
    }

    else
    {
      stc("You dismiss your spirit guardian.\n\r",ch);
      REMOVE_BIT(ch->flag2, AFF_SPIRITGUARD);
      return;
    }
  return;
}

void do_spiritgate( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
   
    argument = one_argument( argument, arg );
   
    if ( IS_NPC(ch) ) return;
  
    if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
    {
      stc("Huh?\n\r",ch);
      return;
    }

    if ( ch->power[DISC_VAMP_NECR] < 3 )
    {
      stc("You must obtain level 3 Necromancy to use SpiritGate.\n\r",ch);
      return;
    }

    if ( ( obj = get_obj_world( ch, arg ) ) == NULL )
    {
      stc("You cannot find that object.\n\r",ch);
      return;
    }

    if ( ch->pcdata->condition[COND_THIRST] < 66 )
    {
      stc("You do not have enough blood.\n\r",ch);
      return;
    }

    if ( obj->in_room == NULL )
    {
      stc("You cannot locate that object.\n\r",ch);
      return;
    }

    if ( obj->carried_by != NULL )
    {
      stc("That corpse is being carried by someone!\n\r",ch);
      return;
    }

    if ( obj->item_type != ITEM_CORPSE_PC && obj->item_type != ITEM_CORPSE_NPC )
    {
      stc("That's not even a corpse!\n\r",ch);
      return;
    }

    if ( obj->in_room->vnum == ch->in_room->vnum )
    {
      stc("You're already there!\n\r",ch);
      return;
    }

    act("$n steps into a spirit gate and vanishes.",ch,NULL,NULL,TO_ROOM);
    char_from_room(ch);
    char_to_room(ch, get_room_index( obj->in_room->vnum ));
    act("You step through a spirit gate and appear before $p.",ch,obj,NULL,TO_CHAR);
    act("$n steps out of a spirit gate in front of $p.",ch,obj,NULL,TO_ROOM);
    ch->pcdata->condition[COND_THIRST] -= 65;
    return;
}

void do_purification( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  int hps = ch->max_hit / 4;

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;

if (!IS_CLASS (ch, CLASS_VAMPIRE) || ch->power[DISC_VAMP_OBEA] < 7)
{
stc("Only the pure in heart can use purification!\n\r",ch);
      return;
}
if (ch->fight_timer>0)
{
stc("You have to be totally calm to purify yourself!\n\r",ch);
      return;
}
if (IS_AFFECTED(ch, AFF_PEACE))
{
stc("You can't hold yer majesty when purifying yourself.\n\r",ch);
      return;
}
if (IS_POLYAFF(ch, POLY_ZULOFORM))
{
stc("Only the pure in heart can use purification!\n\r",ch);
      return;
}
if (ch->beast > 0)
{
stc("Only the pure in heart can use purification!\n\r",ch);
      return;
}
if (ch->rage > 0)
{
stc("Only the pure in heart can use purification!\n\r",ch);
      return;
}
if (IS_AFFECTED(ch, AFF_POLYMORPH))
{
stc("Only the pure in heart can use purification!\n\r",ch);
      return;
}
if (ch->move < 5000)
{
stc("You are too exhausted to purify your mind\n\r",ch);
      return;
}

    act( "A bright halo glows above $n's head.", ch, NULL,NULL, TO_ROOM );
    send_to_char("You purify your mind.\n\r",ch);

WAIT_STATE(ch,30-ch->power[DISC_VAMP_OBEA]);
ch->move -= 5000;
ch->hit += hps;

if (ch->hit > ch->max_hit)
ch->hit = ch->max_hit;
return;
}

void do_scream( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   CHAR_DATA *mount;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );

   if ( IS_NPC(ch) ) return;
 
   if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
   {
     stc("Huh?\n\r",ch);
     return;
   }

   if ( ch->power[DISC_VAMP_MELP] < 1 )
   {
     stc("You must obtain level 1 Melpominee to use Scream.\n\r",ch);
     return;
   }

   if ( IS_SET(ch->in_room->room_flags, ROOM_SAFE) )
   {
     stc("Your screams will have no effect here.\n\r",ch);
     return;
   }

   if ( ch->pcdata->condition[COND_THIRST] < 50 )
   {
     stc("You have insufficient blood.\n\r",ch);
     return;
   }

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
        vch_next        = vch->next;

        if ( vch->in_room == NULL )
            continue;
        if ( !IS_NPC(vch) && vch->pcdata->chobj != NULL )
            continue;
        if ( ch == vch )
              continue;
        if ( vch->in_room == ch->in_room )
        {
            if ( ( mount = ch->mount ) != NULL )
            {
                if ( mount == vch )
                continue;
            }
            if (can_see(ch,vch))
            {
              if ( number_range( 1, (12 - ch->power[DISC_VAMP_MELP])) == 2 )
              {
                act("$n lets out an ear-popping scream!",ch,NULL,vch,TO_ROOM);
                stc("You fall to the ground, clutching your ears.\n\r",vch);
                vch->position = POS_STUNNED;
                act("$n falls to the ground, stunned.",vch,NULL,vch,TO_NOTVICT);
                return;
              }
	      else
	      {
                act("$n lets out an ear-popping scream!",ch,NULL,vch,TO_ROOM);
	        return;
	      }
	    }
	    else continue;

	    return;
        }
    }

  return;
}


void do_testemb(CHAR_DATA *ch, char *argument  )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    one_argument( argument,arg);
 
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch,CLASS_VAMPIRE))
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }
    if ( arg[0] == '\0' && ch->embracing==NULL)
    {
        send_to_char( "Who do you wish to embrace?\n\r", ch );
	return;
    }
if (ch->embracing != NULL)
{
    if ((victim=ch->embracing) != NULL)
    {
    send_to_char("You retract your fangs.",ch);
    act("$N retracts his fangs.",ch,NULL,NULL,TO_ROOM);
    stop_embrace(ch,victim);
    return;
    }
}
    if ((victim = get_char_room( ch, arg)) == NULL && ch->embracing==NULL)
    {
    send_to_char("They arent here.\n\r",ch);
    return;
    }
    if (IS_NPC(victim) || !IS_CLASS(ch,CLASS_VAMPIRE))
    {
    send_to_char("You can only embrace vampires.\n\r",ch);
    return;
    }
  
    if (victim->position != POS_MORTAL)
    {
    send_to_char("You can only embrace mortally wounded vampires.\n\r",ch);
    return;
    }
    sprintf(buf,
    "%s leaps toward %s baring his fangs.",ch->name,victim->name);
    act(buf,ch,NULL,NULL,TO_ROOM);
    sprintf(buf,
    "You leap toward %s baring your fangs.\n\r",victim->name);
    send_to_char(buf,ch);
    WAIT_STATE(ch, 15);
    
    sprintf(buf,
    "You sink your teeth into their throat.\n\r");
    send_to_char(buf,ch);
    sprintf(buf,
    "%s sinks their teeth into %s's throat."
    ,ch->name,victim->name);
    act(buf,ch,NULL,NULL,TO_ROOM);
    sprintf(buf,
    "%s sinks their teeth into your throat.\n\r",ch->name);
    send_to_char(buf,victim);
    victim->embraced=ch;
    ch->embracing=victim;
 
 
    return;
 
}

void do_conceal(CHAR_DATA *ch, char *argument)
{
char arg[MAX_STRING_LENGTH];
char buf[MAX_STRING_LENGTH];
OBJ_DATA *obj; 
one_argument(argument,arg);
if (IS_NPC(ch)) return;
if (!IS_CLASS(ch,CLASS_VAMPIRE))
{send_to_char("Huh?\n\r",ch);return;}
if (ch->power[DISC_VAMP_OBFU] < 5)
{send_to_char("You need obfuscate 5 to conceal items.\n\r",ch);return;}
if (arg[0]== '\0')
{send_to_char("Syntax: Conceal (item).\n\r",ch);return;}
if ((obj=get_obj_carry(ch,arg)) == NULL)
{send_to_char("You dont have that item.\n\r",ch);return;}
if (IS_SET(obj->extra_flags, ITEM_INVIS))
{
sprintf(buf,"%s fades into existance.",obj->short_descr);
send_to_char(buf,ch);act(buf,ch,NULL,NULL,TO_ROOM);
REMOVE_BIT(obj->extra_flags, ITEM_INVIS);
return;
}
if (!IS_SET(obj->extra_flags, ITEM_INVIS))
{
sprintf(buf,"%s fades out of existance.",obj->short_descr);
send_to_char(buf,ch);act(buf,ch,NULL,NULL,TO_ROOM);
SET_BIT(obj->extra_flags, ITEM_INVIS);
return;
}
 
return;
}

void do_fear( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
    {
	stc("Huh?\n\r",ch);
	return;
    }

    if ( ch->power[DISC_VAMP_DAIM] < 2 )
    {
      stc("You must obtain level 2 Daimoinon to use Fear.\n\r",ch);
      return;
    }

    if ( ( victim = get_char_room( ch, arg) ) == NULL )
    {
      stc("They aren't here.\n\r",ch);
      return;
    }

    arg[0] = UPPER(arg[0]);    

    if ( ch->fighting == NULL )
    {
      stc("You must be fighting to use Fear.\n\r",ch);
      return;
    }

    if ( victim->fighting == NULL )
    {
      sprintf(buf, "%s is not fighting anyone.", arg);
      stc(buf, ch);
      return;
    }  

    WAIT_STATE( ch, 16 );

    if ( IS_NPC(victim) )
    {
      act("You bare your fangs and growl at $N.",ch,NULL,victim,TO_CHAR);
      act("$n bares $s fangs and growls at you.",ch,NULL,victim,TO_VICT);
      act("$n bares $s fangs and growls at $N, and $N flees in terror.",ch,NULL,victim,TO_NOTVICT);
      do_flee( victim, "" );
      return;
    }

    if ( !IS_NPC(victim) )
    {
      if ( ch->power[DISC_VAMP_DAIM] < 5 )
      {
        if ( number_range( 1, 4 ) != 2 )
        {
          act("You bare your fangs and growl at $N, but nothing happens.",ch,NULL,victim,TO_CHAR);
          act("$n bares $s fangs and growls at you.",ch,NULL,victim,TO_VICT);
          act("$n bares $s fangs and growls at $N, but nothing happens.",ch,NULL,victim,TO_NOTVICT);
	  return;
        }
      }

      act("You bare your fangs and growl at $N.",ch,NULL,victim,TO_CHAR);
      act("$n bares $s fangs and growls at you.",ch,NULL,victim,TO_VICT);
      act("$n bares $s fangs and growls at $N, and $N flees in terror.",ch,NULL,victim,TO_NOTVICT);
      do_flee( victim, "" );
      return;
    }

  return;

}

void do_vtwist(CHAR_DATA *ch, char *argument)
{
        char arg1[MAX_STRING_LENGTH];
        char arg2[MAX_STRING_LENGTH];
        char arg3[MAX_STRING_LENGTH];
        OBJ_DATA *obj;  
        
        argument=one_argument(argument,arg1);  
        argument=one_argument(argument,arg2);
        strcpy(arg3,argument);
        
        if (IS_NPC(ch)) return;

  if (!IS_CLASS(ch,CLASS_VAMPIRE))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }

  if (ch->power[DISC_VAMP_DAIM] < 5)
  {
    send_to_char("You must obtain level 5 Daimoinon to use Twist.\n\r",ch);
    return;
  }

        if ((obj=get_obj_carry(ch,arg1)) == NULL)
        {send_to_char("You dont have that item.\n\r",ch);
        return;}
        
        if (!(!str_cmp(arg2,"short") || !str_cmp(arg2,"name")))
        {send_to_char("syntax : vtwist <item> <short/name> <newname>.\n\r",ch);
        return;}
 
        if (strlen(arg3) > 60 || strlen(arg3) < 3)
        {send_to_char("From 3 to 60 characters please.\n\r",ch);
        return;}

  if (has_bad_chars(ch, arg3))
  {     
    send_to_char("Illegal chars, please retry.\n\r",ch);
    return;
  }     

  if (IS_SET(obj->quest, QUEST_ARTIFACT) || IS_SET(obj->quest, QUEST_PRIZE))   
  {    
    send_to_char("Not on artifacts and prizes.\n\r",ch);
    return;
  }    

        if (!str_cmp(arg2,"name"))
        {free_string(obj->name);
        obj->name = str_dup(arg3);
        obj->questmaker = str_dup(ch->name);
        }
        if (!str_cmp(arg2,"short"))
        {free_string(obj->short_descr);
        obj->short_descr=str_dup(arg3);
        obj->questmaker = str_dup(ch->name);
        }
        send_to_char("Ok.\n\r",ch);
        
        return;

}

void do_dub(CHAR_DATA *ch, char *argument)
{
char arg1[MAX_STRING_LENGTH];
char arg2[MAX_STRING_LENGTH];
OBJ_DATA *obj; 
 
argument=one_argument(argument,arg1);
strcpy(arg2,argument);
if (IS_NPC(ch)) return;
if (!IS_CLASS(ch,CLASS_VAMPIRE))
{send_to_char("Huh?\n\r",ch);return;}
if (ch->power[DISC_VAMP_OBFU] < 4)
{send_to_char("You need obfuscate 4 to dub items.\n\r",ch);return;}
if (arg1 == NULL || arg2 == NULL)
{send_to_char("Syntax: Dub (item) (description)?\n\r",ch);return;}
if ((obj=get_obj_carry(ch,arg1)) == NULL)
{send_to_char("You dont have that item.\n\r",ch);return;}
if (strlen(arg2) > 40 || strlen(arg2) < 3)
{send_to_char("From 3 to 40 characters please.\n\r",ch);return;}
free_string(obj->name);
obj->name = str_dup(arg2);
free_string(obj->short_descr);
obj->short_descr=str_dup(arg2);
obj->questmaker = str_dup(ch->name);
send_to_char("Ok.\n\r",ch);
return;
}

void do_sharpen(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_STRING_LENGTH];
  OBJ_DATA *obj;
  argument=one_argument(argument,arg);
  if (IS_NPC(ch)) return;
 
  if (!IS_CLASS(ch,CLASS_VAMPIRE))
  {
     send_to_char("Huh?\n\r",ch);
     return;
  }

  if (ch->power[DISC_VAMP_QUIE] < 7)
  {
     send_to_char("You need Quetius 7 to sharpen.\n\r",ch);
     return;
  }

  if (arg[0] == '\0')
  {
     send_to_char("What do you wish to sharpen?\n\r",ch);
     return;
  }

  if ( ( obj = get_obj_carry(ch,arg) ) == NULL )
  {
     send_to_char("You dont have that weapon.\n\r",ch);
     return;
  }

  if (IS_SET(obj->quest, QUEST_ARTIFACT) || obj->item_type != ITEM_WEAPON)
  {
     send_to_char("You cant sharpen that item.\n\r",ch);
     return;
  }

  if (obj->value[0] == 18000)
  {
     send_to_char("This item is already Sharp!\n\r",ch);
     return;
  }

  if (obj->value[0] != 0)
  {
     send_to_char(" This weapon already has a power.\n\r",ch);
     return;
  }

  obj->value[0] = 18000;
  obj->value[1] = 20;
  obj->value[2] = 30;

  /*mite as well recycle the arg string*/
  sprintf(arg, "You grind away at %s until it is razor sharp!", obj->short_descr);
  send_to_char(arg,ch);
  sprintf(arg, "%s grinds away at %s until it is razor sharp!", ch->name,obj->short_descr);
  act(arg,ch,NULL,NULL,TO_ROOM);
  return;
}
 
 

/*gourge*/
void do_gourge(CHAR_DATA *ch, char *argument  )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
   one_argument( argument,arg);
 
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch,CLASS_VAMPIRE))
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }
   if (ch->power[DISC_VAMP_THAU] < 8)
   {
   send_to_char("You need Thaumaturgy 8 to Gourge.\n\r",ch);
   return;
   }
    if ( arg[0] == '\0' )
    {
        send_to_char( "Who do you wish to gourge?\n\r", ch );
	return;
    }
    if ((victim = get_char_room( ch, arg)) == NULL)
    {
    send_to_char("They arent here.\n\r",ch);
    return;
    }
	if (IS_NPC(victim) && IS_SET(victim->act, ACT_NOAUTOKILL)) {
	send_to_char("You can't do that to them.\n\r",  ch);
	return;}

    if (!IS_NPC(victim))
    {
    send_to_char("You cannot gourge a person.\n\r",ch);
    return;
    }
    if (victim->level > 30)
    {
    send_to_char(
    "Only small creatures are defenceless enough to be gourged on.\n\r",ch);
    return;
    }
    sprintf(buf,
    "%s leaps toward %s baring his fangs.",ch->name,victim->name);
    act(buf,ch,NULL,NULL,TO_ROOM);
    sprintf(buf,
    "You leap toward %s baring your fangs.\n\r",victim->name);
    send_to_char(buf,ch);
    WAIT_STATE(ch, 15);
    
    send_to_char(
    "You rip their throat out and gourge on the blood.\n\r",ch);
    sprintf(buf,
    "%s rips %s's throat out, gourging on all of their blood."
    ,ch->name,victim->name);
    act(buf,ch,NULL,NULL,TO_ROOM);
    ch->pcdata->condition[COND_THIRST] += number_range(100,200);
    if (ch->pcdata->condition[COND_THIRST] >= 1000 /
ch->generation)
    {
    send_to_char("Your bloodlust is sated.\n\r",ch);
    ch->pcdata->condition[COND_THIRST] = 1000/ch->generation;
    }
    sprintf(buf,
    "%s body falls to the ground lifeless.",victim->name);
    send_to_char(buf,ch);
    act(buf,ch,NULL,NULL,TO_ROOM);
    raw_kill( victim );
    return;
}

/*bloodwater*/
 
void do_bloodwater( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg1 [MAX_INPUT_LENGTH];
  int dam;
 
  argument = one_argument( argument, arg1 );
 
 
if ( IS_NPC(ch) )
    return;
 
 
if (!IS_CLASS(ch, CLASS_VAMPIRE))
     {
          send_to_char("Huh?\n\r",ch);
      return;
 }
 
if (ch->power[DISC_VAMP_NECR] < 5)
 {
send_to_char(
"You need at least level 5 Necromancy to use Blood Water.\n\r",ch);
    return;
 }
 
 
if ( arg1[0] == '\0' )
{
send_to_char( "Whose blood do you wish to turn to water?\n\r", ch );
   return;
}
 if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
{
    send_to_char( "They aren't here.\n\r", ch );
   return;
}
   if (ch==victim)
    {
        stc("not on yourself.\n\r",ch);
        return;
    }
 
 
  if (is_safe(ch,victim)) return;
  if (IS_NPC(victim)) dam=2000;
  if (!IS_NPC(victim)) dam=number_range(300,600);
  if (victim != NULL) set_fighting(ch,victim);
    act("$N screams in agony as you turn his blood to water.",ch, NULL, victim,TO_CHAR);
    act("$N screams in agony as $n turns his blood to water.",ch, NULL, victim,TO_NOTVICT);
    act("You scream in agony as $n turns your blood to water.", ch, NULL, victim,TO_VICT);
  if (!IS_CLASS(victim,CLASS_VAMPIRE)) hurt_person(ch,victim,dam);
  if (!IS_CLASS(victim,CLASS_VAMPIRE)) WAIT_STATE(ch,12);
  if (IS_CLASS(victim,CLASS_VAMPIRE))
  {
    WAIT_STATE(ch,6);
    victim->pcdata->condition[COND_THIRST] -= 150;
  }
  if (number_range(1,4) == 2 && victim->hit > 100)
  {
    act("Your eyes flare red as your lust for blood takes over.",ch, NULL, NULL, TO_CHAR);
    act("$n growls in lust.",ch, NULL, NULL, TO_ROOM);
    do_bloodwater(ch,arg1);
  }
  return;
}

/*spew*/
void do_spew( CHAR_DATA *ch, char *argument )
{
char  buf[MAX_INPUT_LENGTH];
int   sn;  
int   level;
int   spelltype;
 
 
    if ( IS_NPC(ch) )return;
 
if (!IS_CLASS(ch, CLASS_VAMPIRE))
        {
        send_to_char("Huh?\n\r", ch);
        return;
    }
    if ( IS_CLASS(ch, CLASS_VAMPIRE) &&
    ch->power[DISC_VAMP_THAU] < 6)
    {
    send_to_char(
    "You need level 6 Thaumaturgy to use this power.\n\r",ch);
    return; 
    }
if (ch->pcdata->condition[COND_THIRST] < 20)
{send_to_char("You need 20 blood to spew.\n\r",ch);return;}
ch->pcdata->condition[COND_THIRST] -= number_range(18,22);
 
if (IS_SET(ch->in_room->room_flags, ROOM_SAFE))
{
send_to_char("You cannot do that here.\n\r",ch);
return;
}
if ( ( sn = skill_lookup( "spew" ) ) < 0 )
{
sprintf(buf,"Yep, sn is bieng set to %d.",sn);
send_to_char(buf,ch);
return;
 
}
 
spelltype = skill_table[sn].target;
level = ch->power[DISC_VAMP_THAU];
(*skill_table[sn].spell_fun) ( sn, level, ch, NULL);
WAIT_STATE( ch, 12 );
return;
}

/*vampdarkness*/
void do_vampdarkness( CHAR_DATA *ch, char *argument )
{
char buf[MAX_STRING_LENGTH];
ROOM_INDEX_DATA *inroom;   
inroom= ch->in_room;
 
if (IS_NPC(ch)) return;
if (!IS_CLASS(ch, CLASS_VAMPIRE))
{
  send_to_char("Huh?\n\r", ch);
  return;
}
if (ch->power[DISC_VAMP_QUIE] < 6)
{
      send_to_char("You require level 6 Quietus to use Darkness of Death.\n\r",ch);
return;
    }
 
if (ch->pcdata->condition[COND_THIRST] < 300)
{
send_to_char("You have insufficient blood.\n\r", ch);
 return;
}
ch->pcdata->condition[COND_THIRST] -= 300;
SET_BIT(inroom->room_flags, ROOM_DARK);
sprintf(buf,"A look of concentration passes over %s's face.",
ch->name);
act(buf,ch,NULL,NULL,TO_ROOM);
send_to_char("A look of concentration passes over your face.\n\r",ch);
sprintf(buf,"A complete darkness fills the room.\n\r");
act(buf,ch,NULL,NULL,TO_ROOM);
send_to_char(buf,ch);
return;
}
 

/*dragon*/
void do_dragonform( CHAR_DATA *ch, char *argument )
{
char arg [MAX_INPUT_LENGTH];
char buf [MAX_STRING_LENGTH];
argument = one_argument( argument, arg );
 
if (IS_NPC(ch)) return;
 
if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
     send_to_char("Huh?\n\r",ch);
        return;
}
if (ch->power[DISC_VAMP_VICI] < 4)
{
send_to_char("You must obtain at least level 4 in Vicissitude to use Dragonform\n\r",ch);
return;
}
 
 
if (!IS_POLYAFF(ch, POLY_ZULOFORM))
{
send_to_char("You can only Dragonform while in Zuloform.\n\r",ch);
return;
}
 
if (IS_EXTRA(ch, EXTRA_DRAGON))
{
REMOVE_BIT(ch->extra,EXTRA_DRAGON);
act( "You transform back into zuloform.", ch, NULL, NULL, TO_CHAR );
    act( "$n shrinks back into a big black monster.", ch, NULL, NULL,
TO_ROOM );
    
 
if (ch->hit < 1) ch->hit = 1;
    ch->damroll = ch->damroll - 100;
    ch->hitroll = ch->hitroll - 100;
    free_string( ch->morph );
    ch->morph = str_dup( "A big black monster" );
return;
}
 
 
if ( ch->pcdata->condition[COND_THIRST] < 100 )
{
    send_to_char("You have insufficient blood.\n\r",ch);
    return;
}
 
if (ch->mounted == IS_RIDING) do_dismount(ch,"");
ch->pcdata->condition[COND_THIRST] -= number_range(200,400);
    act( "You transform into a large dragon.", ch, NULL, NULL, TO_CHAR );
    act( "$n's body grows and distorts into a large dragon.", ch, NULL,
NULL,TO_ROOM);
SET_BIT(ch->extra,EXTRA_DRAGON);
sprintf(buf, "%s, the huge rabid dragon", ch->name);
free_string( ch->morph );
ch->morph = str_dup( buf );
ch->damroll = ch->damroll + 100;
ch->hitroll = ch->hitroll + 100;
return;
}

/*obj*/
void do_obj( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;                              
    OBJ_DATA *obji;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_STRING_LENGTH];
    argument=one_argument(argument,arg);
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char( "Huh?\n\r", ch);
	return;
    }
    if (ch->power[DISC_VAMP_VICI] < 10)
    {
    send_to_char("You require Vicissitude 10 to Object.\n\r",ch);
    return;
    }
    if (IS_AFFECTED(ch,AFF_POLYMORPH) && ch->pcdata->chobj == NULL)
    {
	send_to_char( "You cannot do this while polymorphed.\n\r", ch);
	return;
    }
 
   if (ch->pcdata->obj_vnum != 0)
   {
   do_humanform(ch,"");
   return;
   }
 
   if (arg[0] == '\0')
   {
    send_to_char("Which object in the game do you want to become?\n\r",ch); 
    return;
   }
   if ((obji = get_obj_world(ch,arg)) == NULL)
    {
        send_to_char( "Nothing like that is in the game.\n\r", ch);
	return;
    }
    if (IS_AFFECTED(ch,AFF_WEBBED))
    {
	send_to_char( "Not with all this sticky webbing on.\n\r", ch);
	return;
    }
if (obji->item_type == ITEM_GEMSTONE ||
    obji->item_type == ITEM_HILT     ||
    obji->item_type == ITEM_COPPER   ||
    obji->item_type == ITEM_IRON     ||
    obji->item_type == ITEM_STEEL    ||
    obji->item_type == ITEM_ADAMANTITE||
    obji->item_type == ITEM_QUEST ||
    IS_SET(obji->quest,QUEST_ARTIFACT)
    || IS_SET(obji->quest, QUEST_RELIC))

{send_to_char("You are unable to take the form of that.\n\r",ch);return;}
 
    
   

    obj=create_object(get_obj_index(obji->pIndexData->vnum),60);
    obj_to_room(obj,ch->in_room);
    send_to_char("You concentrate deeply and transform into the basics of the item.\n\r",ch);
    act("$n's form shrinks and distorts into $p.",ch,obj,NULL,TO_ROOM);
    act("Your form shrinks and distorts into $p.",ch,obj,NULL,TO_CHAR);
    ch->pcdata->obj_vnum = obj->pIndexData->vnum;
    sprintf(buf,"%d",ch->pcdata->obj_vnum);
    send_to_char(buf,ch);
    obj->chobj = ch;
    ch->pcdata->chobj = obj;
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->extra, EXTRA_OSWITCH);
    free_string(ch->morph);
    ch->morph = str_dup(obj->short_descr);
    return;
}
 
 

/*baal*/
void do_baal(CHAR_DATA *ch, char *argument)
{
    int bloodpool;

    if (IS_SET(ch->newbits, NEW_TIDE))
    bloodpool = (3000 / ch->generation);
    else bloodpool = (2000 / ch->generation);


    if (IS_NPC(ch)) return;
if (!IS_CLASS(ch,CLASS_VAMPIRE))
{
send_to_char("Huh?\n\r",ch);
return;
}
if (ch->power[DISC_VAMP_DOMI] < 5)
{
send_to_char("You need Dominate 5 to summon the spirit of Baal.\n\r",ch);
return;
}

if (IS_SET(ch->extra, EXTRA_BAAL))
{
send_to_char("You already have the spirit of Baal with you!\n\r",ch);
return;
}
if (ch->pcdata->condition[COND_THIRST] < bloodpool)
{
send_to_char("You need maximum blood to use Baal.\n\r",ch);
return;
}

ch->pcdata->condition[COND_THIRST] = 0;

WAIT_STATE(ch,20);

SET_BIT(ch->extra, EXTRA_BAAL);
ch->power[DISC_VAMP_POTE] += 2;
ch->power[DISC_VAMP_CELE] += 2;
ch->power[DISC_VAMP_FORT] += 2;
send_to_char("You now carry the spirit of Baal.\n\r",ch);
return;
}
 

/*facade*/
void do_facade( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int value;
 
    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    strcpy( arg2, argument );
 
    if ( IS_NPC(ch) )
	return;
 
    if (!IS_CLASS(ch, CLASS_VAMPIRE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (ch->power[DISC_VAMP_PRES] < 6 )
    {
        send_to_char("You need presence 6 to facade.\n\r",ch);
	return;
    }
 
    if (arg1[0] != '\0' && !str_cmp(arg1,"on"))
    {
	if (IS_EXTRA(ch, EXTRA_FAKE_CON))
        {send_to_char("You already have Facade on.\n\r",ch); return;}
	SET_BIT(ch->extra, EXTRA_FAKE_CON);
        send_to_char("Your Facade is now ON.\n\r",ch);
	return;
    }
    if (arg1[0] != '\0' && !str_cmp(arg1,"off"))
    {
	if (!IS_EXTRA(ch, EXTRA_FAKE_CON))
        {send_to_char("You already have Facade off.\n\r",ch); return;}
	REMOVE_BIT(ch->extra, EXTRA_FAKE_CON);
        send_to_char("Your Facade is now OFF.\n\r",ch);
	return;
    }
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char("You have the following stats:\n\r",ch);
	sprintf(buf,"Hitroll: %d, Actual: %d.\n\r",
		ch->pcdata->fake_hit, char_hitroll(ch));
	send_to_char(buf, ch);
	sprintf(buf,"Damroll: %d, Actual: %d.\n\r",
		ch->pcdata->fake_dam, char_damroll(ch));
	send_to_char(buf, ch);
	sprintf(buf,"Armour: %d, Actual: %d.\n\r",
		ch->pcdata->fake_ac, char_ac(ch));
	send_to_char(buf, ch);
	sprintf(buf,"Hp: %d, Actual: %d.\n\r",
		ch->pcdata->fake_hp, ch->hit);
	send_to_char(buf, ch);
	sprintf(buf,"Mana: %d, Actual: %d.\n\r",
		ch->pcdata->fake_mana, ch->mana);
	send_to_char(buf, ch);
	sprintf(buf,"Move: %d, Actual: %d.\n\r",
		ch->pcdata->fake_move, ch->move);
	send_to_char(buf, ch);
	return;
    }
    value = is_number( arg2 ) ? atoi( arg2 ) : -10000;
    if (!str_cmp(arg1,"hit") || !str_cmp(arg1,"hitroll"))
    {
	if (value < 0 || value > 1000)
	{
	    send_to_char("Please enter a value between 0 and 1000.\n\r",ch);
	    return;
	}
	ch->pcdata->fake_hit = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }
    if (!str_cmp(arg1,"dam") || !str_cmp(arg1,"damroll"))
    {
	if (value < 0 || value > 1000)
	{
	    send_to_char("Please enter a value between 0 and 1000.\n\r",ch);
	    return;
	}
	ch->pcdata->fake_dam = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }
    if (!str_cmp(arg1,"ac") || !str_cmp(arg1,"armour") ||
!str_cmp(arg1,"armor"))
    {
	if (value < -1000 || value > 100)
	{
	    send_to_char("Please enter a value between -1000 and 100.\n\r",ch);
	    return;
	}
	ch->pcdata->fake_ac = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }
    if (!str_cmp(arg1,"hp") || !str_cmp(arg1,"hitpoints"))
    {
	if (value < 1 || value > 30000)
	{
	    send_to_char("Please enter a value between 1 and 30000.\n\r",ch);
	    return;
	}
	ch->pcdata->fake_hp = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }
    if (!str_cmp(arg1,"mana"))
    {
	if (value < 1 || value > 30000)
	{
	    send_to_char("Please enter a value between 1 and 30000.\n\r",ch);
	    return;
	}
	ch->pcdata->fake_mana = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }
    if (!str_cmp(arg1,"move"))
    {
	if (value < 1 || value > 30000)
	{
	    send_to_char("Please enter a value between 1 and 30000.\n\r",ch);
	    return;
	}
	ch->pcdata->fake_move = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }
    send_to_char("You can set: Hit, Dam, Ac, Hp, Mana, Move.\n\r",ch);
    return;
}

/*wall*/
void do_wall(CHAR_DATA *ch, char *argument)
{
OBJ_DATA *obj = NULL;
OBJ_DATA *objc;
char arg[MAX_STRING_LENGTH];
char wall[MAX_STRING_LENGTH];
char buf[MAX_STRING_LENGTH];
argument=one_argument(argument,arg);
if (IS_NPC(ch)) return;
if (!IS_CLASS(ch,CLASS_VAMPIRE))
{
send_to_char("Huh?\n\r",ch);
return;
}
if (ch->power[DISC_VAMP_DAIM] < 7)
{
send_to_char("You need Daimionon 7 to call Walls of Water.\n\r",ch);
return;
}
if (ch->pcdata->condition[COND_THIRST] < 50)
{
send_to_char("You dont have enough blood.\n\r",ch);
return;
}
if (arg[0] == '\0' || (str_cmp(arg,"n")
                   && str_cmp(arg,"s")
                   && str_cmp(arg,"e")
                   && str_cmp(arg,"w")
                   && str_cmp(arg,"u")
                   && str_cmp(arg,"d")))
{
send_to_char("You may wall n, w, s, e, d or u.\n\r",ch);
return;
}
ch->pcdata->condition[COND_THIRST] -= 50;
sprintf(buf,"A look of concentration passes over %s's face.",ch->name);
act(buf,ch,NULL,NULL,TO_ROOM);
send_to_char("A look of concentration passes over your face.\n\r",ch);
if (!str_cmp(arg,"n")) sprintf(wall,"walln");
if (!str_cmp(arg,"w")) sprintf(wall,"wallw");
if (!str_cmp(arg,"s")) sprintf(wall,"walls");
if (!str_cmp(arg,"e")) sprintf(wall,"walle");
if (!str_cmp(arg,"d")) sprintf(wall,"walld");
if (!str_cmp(arg,"u")) sprintf(wall,"wallu");
objc = get_obj_list(ch,wall,ch->in_room->contents);
if ( objc != NULL)
{
send_to_char("There is already a wall blocking that direction.\n\r",ch);
return;
}
WAIT_STATE(ch,25);
sprintf(buf,"A wall of water pours out of the ground.");
act(buf,ch,NULL,NULL,TO_ROOM);
send_to_char(buf,ch);
if (!str_cmp(arg,"n")) obj=create_object(get_obj_index(30043),0);
if (!str_cmp(arg,"s")) obj=create_object(get_obj_index(30044),0);
if (!str_cmp(arg,"e")) obj=create_object(get_obj_index(30045),0);
if (!str_cmp(arg,"w")) obj=create_object(get_obj_index(30046),0);
if (!str_cmp(arg,"d")) obj=create_object(get_obj_index(30047),0);
if (!str_cmp(arg,"u")) obj=create_object(get_obj_index(30048),0);
obj_to_room(obj,ch->in_room);
obj->timer=3;
obj->item_type = ITEM_WALL; 
return;
}
 
 
 

/*inferno*/
void do_inferno(CHAR_DATA *ch, char *argument)
{
char buf[MAX_STRING_LENGTH];
ROOM_INDEX_DATA *inroom;
 if (IS_NPC(ch)) return;
 inroom= ch->in_room;
 if (!IS_CLASS(ch,CLASS_VAMPIRE))
  {
  send_to_char("Huh?\n\r",ch);
  return;
  }
 if (ch->power[DISC_VAMP_DAIM] < 6)
  {
  send_to_char("You need Daimionon 6 to use Inferno.\n\r",ch);
  return;
  }
 if (IS_SET(ch->in_room->room_flags,ROOM_FLAMING))
  {
  send_to_char("This room is already engulfed in flames!\n\r",ch);
  return;
  }
 if (ch->pcdata->condition[COND_THIRST] < 100)
  {
  send_to_char("You dont have enough blood.\n\r",ch);
  return;
  }
if (IS_SET(ch->in_room->room_flags, ROOM_SAFE))
{
send_to_char("Somthing prevents you from torching this room.\n\r",ch);
return;
}

ch->pcdata->condition[COND_THIRST] -= 100;
SET_BIT(inroom->room_flags,ROOM_FLAMING);
sprintf(buf,"A look of concentration passes over %s's face.\n\r",ch->name);
act(buf,ch,NULL,NULL,TO_ROOM);
send_to_char("A look of concentration passes over your face.\n\r",ch);
sprintf(buf,"This room is engulfed in flames!");
send_to_char(buf,ch);
act(buf,ch,NULL,NULL,TO_ROOM);
return;
}
 
 
 
 

/*zombie*/
void do_zombie(CHAR_DATA *ch, char *argument )
{
    char buf2[MAX_STRING_LENGTH]; 
    char buf [MAX_INPUT_LENGTH];
    char arg [MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    AFFECT_DATA af;
    OBJ_DATA *obj;
    one_argument(argument,arg);
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_LICH))
     {
     send_to_char("Huh?\n\r",ch);
     return;
     }
    if (arg[0] == '\0')
    {
    send_to_char("Zombie what corpse?\n\r",ch);
    return;
    }
    if (ch->power[DISC_VAMP_NECR] < 5 && IS_CLASS(ch, CLASS_VAMPIRE))
    {
    send_to_char("You require level 5 Necromancy to create a zombie.\n\r",ch);
    return;
    }
   if (ch->pcdata->powers[NECROMANTIC] < 1 && IS_CLASS(ch, CLASS_LICH))
   {
     send_to_char("You don't have that power yet.\n\r",ch);
     return;
   }
    if (ch->pcdata->followers > 5)
    {
	send_to_char("Nothing happens.\n\r",ch);
	return;
    }
    if ((obj = get_obj_carry(ch,arg)) == NULL)
    {
    send_to_char("You dont have that corpse.",ch);
    return;
    }
    if (obj->item_type != ITEM_CORPSE_NPC || IS_SET(obj->quest, QUEST_ZOMBIE))
    {
    send_to_char("You can only Zombie original corpses.\n\r",ch);
    return;
    }
 
 
    ch->pcdata->followers++;
 
    
    victim=create_mobile( get_mob_index( obj->value[2] ) );
    sprintf(buf,"the zombie of %s",victim->short_descr);
    sprintf(buf2,"the zombie of %s is here.\n\r",victim->short_descr);
    free_string(victim->short_descr);
    victim->short_descr = str_dup(buf);
    free_string(victim->name);
    SET_BIT(victim->act, ACT_NOEXP);
    victim->name = str_dup(buf);
    free_string(victim->long_descr);
    victim->long_descr= str_dup(buf2);
    SET_BIT(victim->extra, EXTRA_ZOMBIE);
 	victim->spec_fun = NULL;
    sprintf(buf,"Rise corpse, and bow before me!");
    do_say( ch, buf );
    sprintf(buf, "%s clambers back up to its feet.\n\r",obj->short_descr);
    act(buf,ch,NULL,NULL,TO_ROOM);
    send_to_char(buf,ch);
 
 
    char_to_room( victim, ch->in_room );
 
 if (victim->level < 100)
{
    add_follower( victim, ch );
    af.type      = skill_lookup ("charm person");
    af.duration  = 666;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
}
WAIT_STATE(ch,10);
extract_obj(obj);
    return;
}

/*fleshcraft*/
void do_fleshcraft( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];
 
    argument = one_argument( argument, arg );
 
    if (IS_NPC(ch)) return;
 
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }
 
    if (ch->power[DISC_VAMP_VICI] < 2)
    {
	send_to_char("You need Vicissitude 2 to fleshcraft.\n\r",ch);
	return;
    }
 
    if ( arg[0] == '\0' )
    {
	send_to_char( "Change to look like whom?\n\r", ch );
	return;
    }
 
    if (IS_AFFECTED(ch,AFF_POLYMORPH) && !IS_VAMPAFF(ch,VAM_DISGUISED))
    {
	send_to_char( "Not while polymorphed.\n\r", ch );
	return;
    }
 
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
 
    if ( !IS_NPC(victim) && ch != victim)
    {
	send_to_char( "Not on Players.\n\r", ch );
	return;
    }
 
 
    if ( ch->pcdata->condition[COND_THIRST] < 40 )
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    ch->pcdata->condition[COND_THIRST] -= number_range(30,40);
 
    if ( ch == victim )
    {
	if (!IS_AFFECTED(ch,AFF_POLYMORPH) &&
!IS_VAMPAFF(ch,VAM_DISGUISED))
	{
	    send_to_char( "You already look like yourself!\n\r", ch );
	    return;
	}
        sprintf(buf,"Your flesh molds and transforms into %s.",ch->name);
	act(buf,ch,NULL,victim,TO_CHAR);
        sprintf(buf,"%s's flesh molds and transforms into %s.",ch->morph,ch->name);
	act(buf,ch,NULL,victim,TO_ROOM);
	REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    	free_string( ch->morph );
    	ch->morph = str_dup( "" );
	return;
    }
    if (IS_VAMPAFF(ch,VAM_DISGUISED))
    {
        sprintf(buf,"Your flesh molds and transforms into a clone of %s.",victim->short_descr);
    	act(buf,ch,NULL,victim,TO_CHAR);
        sprintf(buf,"%s's flesh molds and transforms into a clone of %s.",ch->morph,victim->short_descr);
    	act(buf,ch,NULL,victim,TO_NOTVICT);
        sprintf(buf,"%s's flesh mols and transforms into a clone of you!",ch->morph);
    	act(buf,ch,NULL,victim,TO_VICT);
    	free_string( ch->morph );
        ch->morph = str_dup( victim->short_descr );
	return;
    }
    sprintf(buf,"Your flesh molds and transforms into a clone of %s.",victim->short_descr);
    act(buf,ch,NULL,victim,TO_CHAR);
    sprintf(buf,"%s's flesh molds and transforms into a clone of %s.",ch->name,victim->short_descr);
    act(buf,ch,NULL,victim,TO_NOTVICT);
    sprintf(buf,"%s's flesh molds and transforms into a clone of you!",ch->name);
    act(buf,ch,NULL,victim,TO_VICT);
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    free_string( ch->morph );
    ch->morph = str_dup( victim->short_descr );
    return;
}

/*entrance*/
void do_entrance( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    int sn;
    int level;
    int spelltype;
    char buf[MAX_STRING_LENGTH];
    argument = one_argument( argument, arg );
 
    if (IS_NPC(ch)) return;
 
    if (!IS_CLASS(ch, CLASS_VAMPIRE)) 
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (ch->power[DISC_VAMP_PRES] < 3)
    {
 send_to_char("You need presence 3 to entrance.\n\r",ch);
 return;
 }
 
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
 
    if ( ch == victim )
    {
  send_to_char( "You cannot entrance yourself.\n\r", ch );
	return;
    }
 
    if ( IS_AFFECTED(victim, AFF_ETHEREAL) )
    {
   send_to_char( "You cannot entrance an ethereal person.\n\r", ch );
	return;
    }
if (is_safe(ch,victim) == TRUE) return;
sprintf(buf,"A look of concentration crosses your face.");
act(buf,ch,NULL,NULL,TO_CHAR);
sprintf(buf,"A look of concentration crosses over $n's face.\n\r");
act(buf,ch,NULL,victim,TO_ROOM);
 
    if ( ( sn = skill_lookup( "charm" ) ) < 0 ) return;
    spelltype = skill_table[sn].target;
    level = ch->power[DISC_VAMP_PRES] * 40;
    (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
    WAIT_STATE( ch, 12 );
    return;
}

/*tendrils*/
void do_tendrils( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    int sn;
    int level;
    int spelltype;
 
    argument = one_argument( argument, arg );
 
    if (IS_NPC(ch)) return;
 
    if (!IS_CLASS(ch, CLASS_VAMPIRE)) 
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (ch->power[DISC_VAMP_SERP] < 4)
    {
 send_to_char("you need level 4 serpentis to use Arms of the Abyss.\n\r",ch);
 return;
 }
 
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
 
    if ( ch == victim )
    {
  send_to_char( "You cannot use Arms of the Abyss on yourself.\n\r", ch );
	return;
    }
 
    if ( IS_AFFECTED(victim, AFF_ETHEREAL) )
    {
   send_to_char( "You cannot Arms of Abyss an ethereal person.\n\r", ch );
	return;
    }
 if (is_safe(ch,victim) == TRUE) return;
    if ( ( sn = skill_lookup( "web" ) ) < 0 ) return;
    spelltype = skill_table[sn].target;
    level = ch->spl[spelltype] * 0.25;
    (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
    WAIT_STATE( ch, 12 );
    return;
}

/*lamprey*/
void do_lamprey( CHAR_DATA *ch, char *argument )
{
CHAR_DATA *victim;
 
char buf[MAX_STRING_LENGTH];
 
int dam;
int bloodpool;

    {
    if (IS_SET(ch->newbits, NEW_TIDE))
    bloodpool = (3000 / ch->generation);
    else bloodpool = (2000 / ch->generation);
    } 
if (IS_NPC(ch)) return;
if (!IS_CLASS(ch,CLASS_VAMPIRE))
{
send_to_char("Huh?\n\r",ch);
return;
}
if (ch->power[DISC_VAMP_OBTE] < 5)
{
send_to_char("You need level 5 Obtenebration to Lamprey.\n\r",ch);
    return;
 
send_to_char("You need level 5 Obtenebration to Lamprey.\n\r",ch);
    return;
    }
 
 
if ( ( victim = ch->fighting ) == NULL )
{
send_to_char( "You aren't fighting anyone.\n\r", ch );
      return;
 
}
WAIT_STATE( ch, 5 );
 
 
if (!IS_NPC(victim))
{
dam = ch->power[DISC_VAMP_OBTE] * 20;
}
else if (IS_NPC(victim))
{
dam = ch->power[DISC_VAMP_OBTE] * 100;
}
if ( !IS_NPC(victim) && IS_CLASS(victim, CLASS_WEREWOLF) )
{
    if (victim->power[DISC_WERE_BOAR] > 2 ) dam *= 0.5;
}
if (is_safe(ch,victim) == TRUE) return; 
dam += number_range(1,30);
if ( dam <= 0 )
   dam = 1;
 

sprintf(buf,"Your tendrils of darkness hits $N incredibly hard! [%d]\n\r",dam);
act(buf,ch,NULL,victim,TO_CHAR);
sprintf(buf,"$n's tendrils of darkness hits you incredibly hard! [%d]\n\r",dam);
act(buf,ch,NULL,victim,TO_VICT);
sprintf(buf,"$n's tendrils of darkness hits $N incredibly hard! [%d]\n\r",dam);
act(buf,ch,NULL,victim,TO_NOTVICT);
 

send_to_char("\n\r",ch);
hurt_person(ch,victim,dam);
ch->pcdata->condition[COND_THIRST] += number_range(40,50);
if (ch->pcdata->condition[COND_THIRST]>bloodpool)
{
send_to_char("Your bloodlust is sated.\n\r",ch);
ch->pcdata->condition[COND_THIRST] = bloodpool;
}
return;
}
 

/*assassinate*/
void do_assassinate( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    int dam, chance = 5;
    argument = one_argument( argument, arg );
 
 
    if(!IS_CLASS(ch,CLASS_VAMPIRE))
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }
    if (ch->power[DISC_VAMP_QUIE] < 4)
    {
    send_to_char("You need Quietus level 4 to Assassinate.\n\r",ch);
    return;
    }
  if (ch->in_room != NULL)
  {
    if (IS_SET (ch->in_room->room_flags, ROOM_ARENA))
    {
      send_to_char("Your in the arena.\n\r",ch);
      return;
    }
  }
 
    if( ch->pcdata->rank == AGE_LA_MAGRA ) chance = 10;
    else if( ch->pcdata->rank == AGE_TRUEBLOOD ) chance = 15;

    if ( arg[0] == '\0' )
    {
        send_to_char( "Assassinate whom?\n\r", ch );
	return;
    }
 
 
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if( victim->position == POS_SLEEPING ) chance *= 2;
 
    if ( victim == ch )
    {
        send_to_char( "How can you assassinate yourself?\n\r", ch );
	return;
    }

    if( !IS_NPC(victim) ) chance = 5;
 
    if (is_safe(ch,victim) == TRUE) return;
 
    if ( victim->fighting != NULL )
    {
        send_to_char( "You can't assassinate a fighting person.\n\r", ch
);
	return;
    }
 
    if ( victim->hit < victim->max_hit )
    {
	act( "$N is hurt and suspicious ... you can't sneak up.",
	    ch, NULL, victim, TO_CHAR );
	return;
    }
    if (!IS_AFFECTED(ch, AFF_HIDE))
    {
    send_to_char("You must be hidden to assassinate!\n\r",ch);
    return;
    }
    WAIT_STATE(ch,15);
    if (number_percent( ) < chance)
    {
    if( !IS_NPC(victim) ){
      one_hit(ch, victim, gsn_garotte,1);
      one_hit(ch, victim, gsn_garotte,1);
      one_hit(ch, victim, gsn_garotte,1);
      one_hit(ch, victim, gsn_garotte,1); 
    }
    else {
    victim->hit = 1;
    ch->fighting = victim;
    send_to_char("You spin around and throw a headbutt to finish him.\n\r",ch);
    if( IS_NPC(ch) )
    {
        switch( ch->pIndexData->vnum ){
        case 30003:
        case 20050:
        case 30704:
        case 30703:
        case 10000:
        case 10001:
        case 10002:
        case 10003:
        case 10004:
                break;
        default:
            {  
		victim->hit = victim->max_hit - 100;
	    }
	}
    }
    damage(ch,victim,11,gsn_headbutt);
    return;
    }
    }
    if (!IS_NPC(victim)) dam = ch->power[DISC_VAMP_QUIE] * 200;
    if (IS_NPC(victim)) dam = ch->power[DISC_VAMP_QUIE] * 400;
dam += number_range(1,20);
   set_fighting(ch,victim);

sprintf(buf,"Your assassination hits $N incredibly hard! [%d]",dam);
act(buf,ch,NULL,victim,TO_CHAR);
sprintf(buf,"$n's assassination hits you incredibly hard! [%d]\n\r",dam);
act(buf,ch,NULL,victim,TO_VICT);
sprintf(buf,"$n's assassination hits $N incredibly hard! [%d]\n\r",dam);
act(buf,ch,NULL,victim,TO_NOTVICT);
 
   if (dam > victim->hit) dam = victim->hit-1;
   hurt_person(ch,victim,dam);
   return;
}

/*mindblast*/
void do_mindblast( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int dam;
 
    argument = one_argument(argument,arg);
 
    if ( IS_NPC(ch) ) return;
    if (!IS_CLASS(ch,CLASS_VAMPIRE))
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }
    if (ch->power[DISC_VAMP_PRES] < 2)
    {
    send_to_char("You require presence 2 to Mindblast.\n\r",ch);
    return;
    }
 
    if (arg[0] == '\0')
    {
    send_to_char("Mindblast Whom?\n\r",ch);
    return;
    }
 
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They arent here.\n\r", ch );
	return;
    }
 
    if ( ch == victim )
    {
        send_to_char( "You cannot Mindblast yourself!\n\r", ch );
	return;
    }
    if (is_safe(ch,victim)== TRUE) return;
    if ( victim->hit < victim->max_hit )
    {
        send_to_char( "They are hurt and alert.\n\r", ch );
	return;
    }
 
 
    act("You close your eyes and concentrate on $N.",ch,NULL,victim,TO_CHAR);
    act("$n closes $s eyes and concentrates on you.",ch,NULL,victim,TO_VICT);
    act("$n closes $s eyes and concentrates on $N.",ch,NULL,victim,TO_NOTVICT);
    WAIT_STATE( ch, 12 );
    if ( number_percent( ) < 50 )
    	dam = number_range(1,4);
    else
    {
    	dam = 0;
        send_to_char("You failed.\n\r",ch);
	return;
    }
 
    dam += char_damroll(ch);
    if ( !IS_AWAKE(victim) )
	dam *= 2;
     if ( dam <= 0 )	dam = 1;
    set_fighting(ch,victim);
    act("You clutch your head in agony!",victim,NULL,NULL,TO_CHAR);
    act("$n clutches his head in agony!",victim,NULL,NULL,TO_ROOM);
    hurt_person(ch,victim,dam);

    return;
}

/*tongue*/
 
void do_tongue( CHAR_DATA *ch, char *argument )
{
CHAR_DATA *victim;
 
char buf[MAX_STRING_LENGTH];
 
int dam;
if (IS_NPC(ch)) return;
if (!IS_CLASS(ch,CLASS_VAMPIRE))
{
send_to_char("Huh?\n\r",ch);
return;
}
if (ch->power[DISC_VAMP_SERP] < 4)
{
send_to_char("You need level 4 Serpentis to tongue.\n\r",ch);
    return;
 
send_to_char("You need level 4 Serpentis to tongue.\n\r",ch);
    return;
    }
 
 
if ( ( victim = ch->fighting ) == NULL )
{
send_to_char( "You aren't fighting anyone.\n\r", ch );
      return;
 
}
WAIT_STATE( ch, 5 );
 
if (!IS_NPC(victim))
{
dam = ch->power[DISC_VAMP_SERP] * 25;
}
else if (IS_NPC(victim))
{
dam = ch->power[DISC_VAMP_SERP] * 125;
}
if ( !IS_NPC(victim) && IS_CLASS(victim, CLASS_WEREWOLF) )
{
    if (victim->power[DISC_WERE_BOAR] > 2 ) dam *= 0.5;
}
 if (is_safe(ch,victim)== TRUE) return;
dam += number_range(1,30);
if ( dam <= 0 )
   dam = 1;
set_fighting(ch,victim);

sprintf(buf,"Your tongue of the serpent hits $N incredibly hard! [%d]\n\r",dam);
act(buf,ch,NULL,victim,TO_CHAR);
sprintf(buf,"$n's tongue of the serpent hits you incredibly hard! [%d]\n\r",dam);
act(buf,ch,NULL,victim,TO_VICT);
sprintf(buf,"$n's tongue of the serpent hits $N incredibly hard! [%d]\n\r",dam);
act(buf,ch,NULL,victim,TO_NOTVICT);
 

send_to_char("\n\r",ch);
hurt_person(ch,victim, dam);
return;
}
 
 
void do_objmask( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    char      buf [MAX_STRING_LENGTH];
    char      arg [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
    {
      stc("Huh?\n\r",ch);
      return;
    }

    if ( ch->power[DISC_VAMP_CHIM] < 5 )
    {
      stc("You must obtain level 5 Chimerstry to Mask an Object.\n\r",ch);
      return;
    }

    if ( IS_SET(ch->flag2, VAMP_OBJMASK ) )
    {
      stc("You return to your normal form.\n\r",ch);
      sprintf(buf, "%s transforms back into %s.\n\r",ch->morph, ch->name );
      act(buf,ch,NULL,NULL,TO_ROOM);
      ch->morph = str_dup("");
      ch->objdesc = str_dup("");
      ch->long_descr = str_dup("");
      ch->short_descr = str_dup(ch->name);
      REMOVE_BIT(ch->flag2, VAMP_OBJMASK);
      REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
      return;
    }

    if (has_timer(ch)) return;

    if ( ch->pcdata->condition[COND_THIRST] < 50 )
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    
    ch->pcdata->condition[COND_THIRST] -= number_range(40, 50);

    if ( arg[0] == '\0' )
    {
      stc("What object do you wish to mask yourself as?\n\r",ch);
      return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) == NULL )
    {
	send_to_char( "That object is not here.\n\r", ch );
	return;
    }

    SET_BIT(ch->flag2, VAMP_OBJMASK );
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    act("You mask yourself as $p.",ch,obj,NULL,TO_CHAR);
    act("$n masks $mself as $p.",ch,obj,NULL,TO_ROOM);
    free_string(ch->morph);
    ch->morph = str_dup(obj->short_descr);
    free_string(ch->objdesc);
    ch->objdesc = str_dup(obj->description);
    ch->long_descr = str_dup("");
    return;
}

void do_mirror( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char buf[MAX_INPUT_LENGTH];

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
    {
      stc("Huh?\n\r",ch);
      return;
    }

    if ( ch->power[DISC_VAMP_CHIM] < 1 )
    {
      stc("You must obtain level 1 Chimersty to use Mirror Image.\n\r",ch);
      return;
    }

    if ( ch->pcdata->condition[COND_THIRST] < 20 )
    {
      stc("You do not have enough blood to create a Mirror Image of yourself.\n\r",ch);
      return;
    }

    if (ch->pcdata->followers > 4)
    {
	send_to_char("Nothing happens.\n\r",ch);
	return;
    }

    ch->pcdata->followers++;


      victim = create_mobile( get_mob_index( 33004 ) );
      victim->short_descr = str_dup(ch->name);
      sprintf(buf, "%s is hovering here.\n\r",ch->name);
      victim->long_descr = str_dup(buf);
      victim->name = str_dup(ch->name);
      victim->level = 20;
      victim->max_hit = 2000;
      victim->hit = 2000;
      victim->mana = 2000;
      victim->max_mana = 2000;
      victim->max_move = 2000;
      victim->move = 2000;
      SET_BIT(victim->flag2, VAMP_CLONE);
	SET_BIT(victim->act, ACT_NOEXP);
      ch->pcdata->condition[COND_THIRST] -= 20;
	char_to_room( victim, ch->in_room );
	act( "You concentrate your powers and form a mirror image of yourself.", ch, NULL, victim, TO_CHAR );
	act( "$n waves $s hands and a mirror image of $mself appears.", ch, NULL, victim, TO_ROOM );
	return;

}

void do_control( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *familiar;
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
    {
      stc("Huh?\n\r",ch);
      return;
    }

    if ( ch->power[DISC_VAMP_CHIM] < 4 )
    {
      stc("You must obtain level 4 Chimerstry to use Control the Clone.\n\r",ch);
      return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Which clone do you wish to control?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "Become your own familiar?\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) )
    {
      stc("Not on players.\n\r",ch);
      return;
    }

    if ( !IS_SET(victim->flag2, VAMP_CLONE) )
    {
      stc("That is not a clone.\n\r",ch);
      return;
    }

    if ( ( familiar = ch->pcdata->familiar ) != NULL ) 
	familiar->wizard = NULL;
    ch->pcdata->familiar = victim;
    victim->wizard = ch;
    act("You take control of $N's mind.",ch,NULL,victim,TO_CHAR);
    act("$n takes control of $N's mind.",ch,NULL,victim,TO_ROOM);

    return;
}


void do_formillusion( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char buf[MAX_INPUT_LENGTH];

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
    {
      stc("Huh?\n\r",ch);
      return;
    }

    if ( ch->power[DISC_VAMP_CHIM] < 2 )
    {
      stc("You must obtain level 2 Chimersty to Form an Illusion of yourself.\n\r",ch);
      return;
    }

    if ( ch->pcdata->condition[COND_THIRST] < 30 )
    {
      stc("You do not have enough blood to Form an Illusion of yourself.\n\r",ch);
      return;
    }

    if (ch->pcdata->followers > 4)
    {
	send_to_char("Nothing happens.\n\r",ch);
	return;
    }

    ch->pcdata->followers++;


	victim = create_mobile( get_mob_index( 33004 ) );
      victim->short_descr = str_dup(ch->name);
      sprintf(buf, "%s is hovering here.\n\r",ch->name);
      victim->long_descr = str_dup(buf);
      victim->name = str_dup(ch->name);
      victim->level = 200;
      victim->max_hit  = ch->max_hit;
      victim->hit      = victim->max_hit;
      victim->max_mana = ch->max_mana;
      victim->mana     = victim->max_mana;
      victim->max_move = ch->max_move;
      victim->move     = victim->max_move;
      victim->hitroll  = ch->hitroll;
      victim->damroll  = ch->damroll;
      SET_BIT(victim->flag2, VAMP_CLONE);
	SET_BIT(victim->act, ACT_NOEXP);
	char_to_room( victim, ch->in_room );
      ch->pcdata->condition[COND_THIRST] -= 30;
	act( "You concentrate your powers and form an illusion of yourself.", ch, NULL, victim, TO_CHAR );
	act( "$n waves $s hands and splits in two.", ch, NULL, victim, TO_ROOM );
	return;

}

/*
void do_ashes( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    
     if ( IS_NPC(ch) )
	return;
 
    if (!IS_CLASS(ch, CLASS_VAMPIRE)) 
    {
	send_to_char("Huh?\n\r", ch);
	return;
    }

    if (ch->power[DISC_VAMP_THAN] < 3)
    {
       send_to_char("You must obtain level 3 in Thanatosis to use Body of Ashes.\n\r", ch);
       return;
    }
 
    save_char_obj(ch);

    if ((obj = create_object(get_obj_index( 33055 ),60)) == NULL)
    {
        send_to_char( "Error - Please inform Dunkirk.\n\r", ch);
	  return;
    }

    if ( IS_SET(ch->flag2, VAMP_ASHES) )
    {
      ch->pcdata->obj_vnum = 0;
      REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
      REMOVE_BIT(ch->extra, EXTRA_OSWITCH);
      REMOVE_BIT(ch->flag2, VAMP_ASHES);
      ch->pcdata->chobj = NULL;
      obj->chobj = NULL;
      free_string(ch->morph);
      ch->morph = str_dup("");
      act("$p transforms into $n.",ch,obj,NULL,TO_ROOM);
      act("Your reform your human body.",ch,obj,NULL,TO_CHAR);
      extract_obj( obj );
      return;
                
    }
    
    if ( IS_AFFECTED(ch, AFF_POLYMORPH) )
    {
	send_to_char( "Not while polymorphed.\n\r", ch );
	return;
    }

    act("$n transforms into $p and falls to the ground.",ch,obj,NULL,TO_ROOM);
    act("You transform into $p and falls to the ground.",ch,obj,NULL,TO_CHAR);
    ch->pcdata->obj_vnum = ch->pcdata->powers[VPOWER_OBJ_VNUM];
    obj->chobj = ch;
    ch->pcdata->chobj = obj;
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->extra, EXTRA_OSWITCH);
    SET_BIT(ch->flag2, VAMP_ASHES);
    free_string(ch->morph);
    ch->morph = str_dup("a pile of ashes");
    obj_to_room(obj,ch->in_room);
    return;
}
*/

void do_unveil( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( !IS_CLASS(ch, CLASS_VAMPIRE))
    {
      stc("Huh?\n\r",ch);
      return;
    }

    if ( ch->power[DISC_VAMP_AUSP] < 3 && IS_CLASS(ch, CLASS_VAMPIRE))
    {
      stc("You must obtain level 3 Auspex to use Unveil the Shrouded Mind.\n\r",ch);
      return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Whose mind do you wish to Unveil?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	stc("Why would you want to unveil your own mind?\n\r",ch);
	return;
    }

    if ( victim->level > ch->level )
    {
      stc("Their mind is far too powerful for you to Unveil.\n\r",ch);
      return;
    }

    if ( victim->spl[BLUE_MAGIC] > ch->spl[BLUE_MAGIC] / 2 )
    {
      stc("Their mind is far too powerful for you to Unveil.\n\r",ch);
      return;
    }

    victim->unveil = ch;
    act("You gaze deeply into $N's eyes.\n\rYou have unveiled $S mind.\n\r", ch,NULL,victim,TO_CHAR );
    return;
}

void do_astralwalk( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
	return;

    argument = one_argument (argument, arg);

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
       send_to_char("Huh?\n\r",ch);
       return;
    }

    if ( ch->power[DISC_VAMP_AUSP] < 4 )
    {
        send_to_char("You must obtain level 4 Auspex to use Astral Walk.\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Astral Walk to who?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
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

    if (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON)) 
    {
      send_to_char("You are unable to locate them.\n\r",ch);
      return;
    }


    if (ch == victim)
    {
        send_to_char("But you're already at yourself!\n\r",ch);
	return;
    }

    if (victim->in_room == ch->in_room)
    {
	send_to_char("But you're already there!\n\r",ch);
      return;
    }

    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    act("You step through a rift in the Astral plane.\n\rYou leap out of the Astral Plane before $N.", ch, NULL, victim, TO_CHAR);
    act("$n leaps out of a rift in the Astral Plane.", ch, NULL, victim, TO_NOTVICT);
    act("$n leaps out of a rift in the Astral Plane.", ch, NULL, victim, TO_VICT);
    do_look(ch,"auto");
    return;
}

void do_hagswrinkles( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }
    if (ch->power[DISC_VAMP_THAN] < 1  )
    {
      send_to_char("You must obtain at least level 1 in Thanatosis to use Hagswrinkles.\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Change to look like whom?\n\r", ch );
	return;
    }

    if (IS_AFFECTED(ch,AFF_POLYMORPH) && !IS_VAMPAFF(ch,VAM_DISGUISED))
    {
	send_to_char( "Not while polymorphed.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( IS_IMMORTAL(victim) && victim != ch )
    {
	send_to_char( "You can only use hagswrinkles on Avatars or lower.\n\r", ch );
	return;
    }

    if ( ch->pcdata->condition[COND_THIRST] < 40 )
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    ch->pcdata->condition[COND_THIRST] -= number_range(30,40);

    if ( ch == victim )
    {
	if (!IS_AFFECTED(ch,AFF_POLYMORPH) && !IS_VAMPAFF(ch,VAM_DISGUISED))
	{
	    send_to_char( "You already look like yourself!\n\r", ch );
	    return;
	}
	sprintf(buf,"Your body wrinkles and reshapes as %s.",ch->name);
	act(buf,ch,NULL,victim,TO_CHAR);
	sprintf(buf,"%s's body wrinkles and reshapes as %s.",ch->morph,ch->name);
	act(buf,ch,NULL,victim,TO_ROOM);
	REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    	free_string( ch->morph );
    	ch->morph = str_dup( "" );
	return;
    }
    if (IS_VAMPAFF(ch,VAM_DISGUISED))
    {
    	sprintf(buf,"Your body wrinkles and reshapes as %s.",victim->name);
    	act(buf,ch,NULL,victim,TO_CHAR);
    	sprintf(buf,"%s's body wrinkles and reshapes as %s.",ch->morph,victim->name);
    	act(buf,ch,NULL,victim,TO_NOTVICT);
    	sprintf(buf,"%s's body wrinkles and reshapes as you!",ch->morph);
    	act(buf,ch,NULL,victim,TO_VICT);
    	free_string( ch->morph );
    	ch->morph = str_dup( victim->name );
	return;
    }
    sprintf(buf,"Your body wrinkles and reshapes as %s.",victim->name);
    act(buf,ch,NULL,victim,TO_CHAR);
    sprintf(buf,"%s's body wrinkles and reforms as %s.",ch->name,victim->name);
    act(buf,ch,NULL,victim,TO_NOTVICT);
    sprintf(buf,"%s's body wrinkles and reforms as you!",ch->name);
    act(buf,ch,NULL,victim,TO_VICT);
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    free_string( ch->morph );
    ch->morph = str_dup( victim->name );
    return;
}

/*gate*/

void do_gate(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    char       arg [MAX_INPUT_LENGTH];
 
    one_argument( argument, arg );
 
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }
    if (ch->power[DISC_VAMP_DAIM] < 3)
    {
    send_to_char("You require Daimoinon level 3 to gate.\n\r",ch);
    return;
    }
    if ( arg[0] == '\0')
    {
        send_to_char( "Who do you wish to gate to?\n\r", ch );
	return;
    }
 
    if ( (victim = get_char_world( ch, arg ) ) == NULL )
    {
    send_to_char("They arent here.\n\r",ch);
    return;
    }
 
 
    if ( (victim == ch) 
    ||   victim->in_room == NULL
    ||   IS_NPC(victim)
    ||   (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON) )
    ||   IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(ch->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
    ||   victim->in_room->vnum == ch->in_room->vnum)
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }
 
 
    obj = create_object( get_obj_index( OBJ_VNUM_GATE ), 0 );
    obj->value[0] = victim->in_room->vnum;
    obj->value[3] = ch->in_room->vnum;
    obj->timer = 5;
    if (IS_AFFECTED(ch,AFF_SHADOWPLANE))
	obj->extra_flags = ITEM_SHADOWPLANE;
    obj_to_room( obj, ch->in_room );
 
    obj = create_object( get_obj_index( OBJ_VNUM_GATE ), 0 );
    obj->value[0] = ch->in_room->vnum;
    obj->value[3] = victim->in_room->vnum;
    obj->timer = 5;
    if (IS_AFFECTED(victim,AFF_SHADOWPLANE))
	obj->extra_flags = ITEM_SHADOWPLANE;
    obj_to_room( obj, victim->in_room );
    act( "A look of concentration passes over $n's face.", ch, NULL,NULL,
TO_ROOM );
    send_to_char("A look of concentration passes over your face.\n\r",ch);
    act( "$p appears in front of $n in a blast of flames.", ch, obj, NULL,
TO_ROOM );
    act( "$p appears in front of you in a blast of flames.", ch, obj,
NULL, TO_CHAR );
    act( "$p appears in front of $n in a blast of flames.", victim, obj,
NULL, TO_ROOM );
    act( "$p appears in front of you in a blast of flames.", ch, obj,
victim, TO_VICT );
    return;
}

/*pigeon*/
void do_pigeon( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *victim;
 
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
 
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }
    if (ch->power[DISC_VAMP_ANIM] < 3)
    {
    send_to_char("You require Aimalism level 3 to pigeon.\n\r",ch);
    return;
    }
    if ( arg1[0] == '\0' )
    {
        send_to_char( "Pigeon which object?\n\r", ch );
        return;
    }
 
    if ( arg2[0] == '\0' )
    {
        send_to_char( "Pigeon what to whom?\n\r", ch );
        return;
    }
    if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "You are not carrying that item.\n\r", ch );
	return;
    }
    victim = get_char_world(ch,arg2);
    if ( (victim = get_char_world( ch, arg2 ) ) == NULL )
    {
	send_to_char( "They arent here.\n\r", ch );
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

    if (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_TRANSPORT))
    {
        send_to_char( "They dont want anything transported to them.\n\r",
ch );
	return;
    }
 
    act("You place $p in a pigeon's beak and it flies away.",
ch,obj,NULL,TO_CHAR);
    act("$n places $p in a pigeon's beak and it flies away."
,ch,obj,NULL,TO_ROOM);
    obj_from_char(obj);
    obj_to_char(obj,victim);
 act("A pigeon lands on your shoulders and flies away after handing you $p."
   ,victim,obj,NULL,TO_CHAR);
 act("A pigeon lands on $n's shoulders and flies away after handing him $p."
   ,victim,obj,NULL,TO_ROOM);
    do_autosave(ch,"");
    do_autosave(victim,"");
    return;
}

/*bloodagony*/
void do_bloodagony(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_STRING_LENGTH];
    int value;
    int pcost;
    OBJ_DATA *obj; 
    one_argument( argument,arg);

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
         send_to_char("Huh?\n\r",ch);
         return;
    }

    value = ch->power[DISC_VAMP_QUIE];
    pcost = ch->power[DISC_VAMP_QUIE] * 60;
 
    if ( arg[0] == '\0')
    {
    send_to_char("Bloodagony what?\n\r",ch);
    return;
    }
 
    if (IS_NPC(ch)) return;
    if ((obj= get_obj_carry(ch,arg)) == NULL)
    {
    send_to_char("You dont have that weapon.\n\r",ch);
    return;
    }
 
    if (ch->power[DISC_VAMP_QUIE] < 3 )
    {
         send_to_char("You need level 3 Quietus to use bloodagony.\n\r",ch);
         return;
    }
 
    if ( obj->item_type != ITEM_WEAPON
    ||   IS_SET(obj->quest, QUEST_ARTIFACT)
    ||   obj->chobj != NULL 
    ||  IS_SET(obj->quest, QUEST_BLOODA))
    {
        send_to_char("You are unable to bloodagony this weapon.\n\r",ch);
	return;
    }
//    ch->pcdata->quest += pcost;
    oset_affect(ch,obj,value,APPLY_DAMROLL,TRUE);
    oset_affect(ch,obj,value,APPLY_HITROLL,TRUE);
//    obj->points -= pcost;
    SET_BIT(obj->quest, QUEST_BLOODA);
 
        act( "You cut your wrist and smear your blood on $p.", ch, obj,
NULL, TO_CHAR );
        act( "$n cuts his wrist and smears blood on $p.", ch, obj, NULL,
TO_ROOM );
    return;
}

void do_diablerise(CHAR_DATA *ch, char *argument  )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    one_argument( argument,arg);
 
    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch,CLASS_VAMPIRE))
    {
      send_to_char("Huh?\n\r",ch);
      return;
    }

    if ( arg[0] == '\0' && ch->embracing==NULL)
    {
        send_to_char( "Who do you wish to embrace?\n\r", ch );
	  return;
    }

    if (ch->embracing != NULL)
    {
      if ( ( victim = ch->embracing) != NULL)
      {
        send_to_char("You retract your fangs.",ch);
        act("$N retracts his fangs.",ch,NULL,NULL,TO_ROOM);
        stop_embrace(ch,victim);
        return;
      }
    }

    if ((victim = get_char_room( ch, arg)) == NULL && ch->embracing==NULL)
    {
      send_to_char("They arent here.\n\r",ch);
      return;
    }

    if (IS_NPC(victim) || !IS_CLASS(victim,CLASS_VAMPIRE))
    {
      send_to_char("You can only embrace vampires.\n\r",ch);
      return;
    }
  
    if (victim->position != POS_MORTAL)
    {
      send_to_char("You can only embrace mortally wounded vampires.\n\r",ch);
      return;
    }

    sprintf(buf,"%s leaps toward %s baring his fangs.",ch->name,victim->name);
    act(buf,ch,NULL,NULL,TO_ROOM);
    sprintf(buf,"You leap toward %s baring your fangs.\n\r",victim->name);
    send_to_char(buf,ch);
    WAIT_STATE(ch, 15);
    
    sprintf(buf,"You sink your teeth into their throat.\n\r");
    send_to_char(buf,ch);
    sprintf(buf,"%s sinks their teeth into %s's throat.",ch->name,victim->name);
    act(buf,ch,NULL,NULL,TO_ROOM);
    sprintf(buf,"%s sinks their teeth into your throat.\n\r",ch->name);
    send_to_char(buf,victim);
    victim->embraced = ch;
    ch->embracing = victim;
 
    return;
 }

/*embrace*/
void do_embrace(CHAR_DATA *ch, char *argument  )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    int bloodpool;
    one_argument( argument, arg );

    
    if (IS_SET(ch->newbits, NEW_TIDE))
    bloodpool = (3000 / ch->generation);
    else bloodpool = (2000 / ch->generation);
    
 
    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch,CLASS_VAMPIRE))
    {
      send_to_char("Huh?\n\r",ch);
      return;
    }


    if ( arg[0] == '\0' )
    {
        send_to_char( "Who do you wish to embrace?\n\r", ch );
	return;
    }

    if ((victim = get_char_room( ch, arg)) == NULL)
    {
      send_to_char("They arent here.\n\r",ch);
      return;
    }

    if (IS_NPC(victim) && IS_SET(victim->act, ACT_NOAUTOKILL)) 
    {
	send_to_char("You can't do that to them.\n\r", ch);
	return;
    }

	/* Shaktis check for level */

	if (victim->level > 75 * ch->spl[RED_MAGIC])
	{
		send_to_char("They are too powerful to embrace!\n\r",ch);
		return;
	}

	
    if (!IS_NPC(victim))
    {
      send_to_char("You cannot embrace a person.\n\r",ch);
      return;
    }

    if (IS_SET(victim->in_room->room_flags, ROOM_SAFE))
    {
	send_to_char("You cannot embrace them here.\n\r",ch);
	return;
    }

    sprintf(buf,"%s leaps toward %s baring his fangs.\n\r",ch->name,victim->short_descr);
    act(buf,ch,NULL,NULL,TO_ROOM);
    sprintf(buf,"You leap toward %s baring your fangs.\n\r",victim->short_descr);
    send_to_char(buf,ch);
    WAIT_STATE(ch, 15);
    
    if (victim->position != POS_STUNNED && victim->position != POS_SLEEPING && victim->position != POS_DEAD)
    {
      send_to_char("They lunge away from you.\n\r",ch);
      sprintf(buf,"%s lunges away from %s.",victim->name,ch->short_descr);
      act(buf,ch,NULL,NULL,TO_ROOM);
      return;
    }

    send_to_char("You bury your fangs into their neck, and begins an orgy of blood-sucking!\n\r",ch);
    sprintf(buf,"%s buries his fangs into %s's neck, and begins an orgy of blood-sucking!\n\r",ch->name,victim->short_descr);
    act(buf,ch,NULL,NULL,TO_ROOM);
	
	/* New update send routine, allows for anything to get embraced.         */
	/* and sets a mobs blood to its level *8.Shakti	     					 */

	victim->embraced = ch;
    ch->embracing = victim;
	if (IS_NPC(victim))
	{
		(victim->practice = victim->level  );
		(victim->practice *= 8);
	}	

	return ;


}


/*decay*/
void do_withering (CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH];

    argument = one_argument (argument, arg);
        
    
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }

    if (ch->power[DISC_VAMP_THAN] < 4 )
    {
    send_to_char("You must obtain level 4 Thanatosis to use Withering.\n\r",ch);
    return;
    }
    if ( arg[0] == '\0' )
    {
    send_to_char("Who do you wish to wither?\n\r",ch);
    return;
    }
    victim = get_char_room(ch, arg);
    if ( ( victim = get_char_room( ch, arg) ) == NULL )
    {
     send_to_char("They arent here.\n\r",ch);
     return;
    }

   if (IS_SET(victim->in_room->room_flags, ROOM_SAFE))
   {
     send_to_char("You cannot attack them here.\n\r",ch);
     return;
   }

   if (is_safe(ch,victim) == TRUE) 
     return;
 
   WAIT_STATE(ch,35);
    sprintf(buf,"A look of concentration crosses over %s's face.",ch->name);
    act(buf,ch,NULL,NULL,TO_ROOM);
    send_to_char("A look of concentration crosses over your face.\n\r",ch);
    if (IS_ARM_R(victim,LOST_ARM) && IS_ARM_L(victim,LOST_ARM) &&
        IS_LEG_R(victim,LOST_LEG) && IS_LEG_L(victim,LOST_LEG))
        {
        send_to_char("All of their limbs are severed already!\n\r",ch);
        return;
        }
       if (number_percent ( ) < 15 )
       {
        if (!IS_ARM_R(victim,LOST_ARM))
        {
        SET_BIT(victim->loc_hp[3],LOST_ARM);
        if ((obj = get_eq_char( victim, WEAR_HOLD )) != NULL )
           take_item(victim,obj);
        if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
           take_item(victim,obj);
        if ((obj = get_eq_char( victim, WEAR_WRIST_R )) != NULL)
           take_item(victim,obj);
        if ((obj = get_eq_char( victim, WEAR_FINGER_R )) != NULL)
           take_item(victim,obj);
        make_part(victim,"arm");
        sprintf(buf,
        "A supernatural force rips %s's arm off!",victim->name);
        act(buf,ch,NULL,NULL,TO_ROOM);
        send_to_char(buf,ch);
        return;
        }
        if (!IS_ARM_L(victim,LOST_ARM))
        {
        SET_BIT(victim->loc_hp[2],LOST_ARM);
        if ((obj = get_eq_char( victim, WEAR_HOLD )) != NULL )
           take_item(victim,obj);
        if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
           take_item(victim,obj);
        if ((obj = get_eq_char( victim, WEAR_WRIST_L )) != NULL)
           take_item(victim,obj);
        if ((obj = get_eq_char( victim, WEAR_FINGER_L )) != NULL)
           take_item(victim,obj);
        make_part(victim,"arm");
        sprintf(buf,
        "A supernatural force rips %s's arm off!",victim->name);
        act(buf,ch,NULL,NULL,TO_ROOM);
        send_to_char(buf,ch);
        return;
        }
        if (!IS_LEG_R(victim,LOST_LEG))
        {
        SET_BIT(victim->loc_hp[5],LOST_LEG);
        if ((obj = get_eq_char( victim, WEAR_FEET )) != NULL )
           take_item(victim,obj);
        if ((obj = get_eq_char( victim, WEAR_LEGS )) != NULL)
           take_item(victim,obj);
        make_part(victim,"leg");
        sprintf(buf,
        "A supernatural force rips %s's leg off!",victim->name);
        act(buf,ch,NULL,NULL,TO_ROOM);
        send_to_char(buf,ch);
        return;
        }
        if (!IS_LEG_L(victim,LOST_LEG))
        {
        SET_BIT(victim->loc_hp[4],LOST_LEG);
        if ((obj = get_eq_char( victim, WEAR_FEET )) != NULL )
           take_item(victim,obj);
        if ((obj = get_eq_char( victim, WEAR_LEGS )) != NULL)
           take_item(victim,obj);
        make_part(victim,"leg");
        sprintf(buf,
        "A supernatural force rips %s's leg off!",victim->name);
        act(buf,ch,NULL,NULL,TO_ROOM);
        send_to_char(buf,ch);
        return;
        }
        }
send_to_char("You failed.\n\r",ch);
 
return;
}
 
 
 

/* Infirmity Thinger */
void do_infirmity( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char  arg[MAX_INPUT_LENGTH];
    char  buf[MAX_INPUT_LENGTH];
    int   sn;
    int   level;
    int   spelltype;
    
    argument = one_argument (argument, arg);

    if ( IS_NPC(ch) )
	return;

	
  	if (!IS_CLASS(ch, CLASS_VAMPIRE))
        {
  	send_to_char("Huh?\n\r", ch);
        return;
        }
     	if ( IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_QUIE] < 2 )
        {
        send_to_char("You need level 2 Quietus to use this power.\n\r",ch);
        return; 
        }
       if (arg[0] == '\0')
	{
	   send_to_char("Who do you wish to use Infirmity on?\n\r",ch);
	   return;
	}
  
  	
      if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
if (is_safe(ch,victim) == TRUE) return;  	
    if ( ( sn = skill_lookup( "infirmity" ) ) < 0 ) 
{
sprintf(buf,"Yep, sn is bieng set to %d.",sn);
send_to_char(buf,ch);
return;

}  

    spelltype = skill_table[sn].target;
    level = ch->power[DISC_VAMP_QUIE];
    (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
    WAIT_STATE( ch, 12 );
    return; 
}


/* Guardian DEmon for Vamps */
void do_guardian(CHAR_DATA *ch, char *argument )
{
 
    char buf [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    AFFECT_DATA af;
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
     {
     send_to_char("Huh?\n\r",ch);
     return;
     }
    if (ch->power[DISC_VAMP_DAIM] < 1)
    {
    send_to_char("You require level 1 Daimoinon to create a guardian.\n\r",ch);
    return;
    }
 
    if (ch->pcdata->followers > 5)
    {
	send_to_char("Nothing happens.\n\r",ch);
	return;
    }
    ch->pcdata->followers++;
 
    victim=create_mobile( get_mob_index( MOB_VNUM_GUARDIAN ) );
    victim->level = 200;
    victim->hit = 5000;
    victim->max_hit = 5000;
    victim->hitroll = 50;
    victim->damroll = 50;
    victim->armor = 300;
    SET_BIT(victim->act, ACT_NOEXP); 

    sprintf(buf,"Come forth, creature of darkness, and do my bidding!");
    do_say( ch, buf );
 
    send_to_char( "A demon bursts from the ground and bows before you.\n\r",ch );
    act( "$N bursts from the ground and bows before $n.", ch, NULL, victim, TO_ROOM );
 
    char_to_room( victim, ch->in_room );
 
 
    add_follower( victim, ch );
    af.duration  = 666;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    return;

}

/*demonic servants*/
void do_servant(CHAR_DATA *ch, char *argument )
{
 
    char buf [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    AFFECT_DATA af;
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
     {
     send_to_char("Huh?\n\r",ch);
     return;
     }
    if (ch->power[DISC_VAMP_DAIM] < 8)
    {
    send_to_char("You require level 8 Daimoinon to create a servant.\n\r",ch);
    return;
    }
 
    if (ch->pcdata->followers > 5)
    {
	send_to_char("Nothing happens.\n\r",ch);
	return;
    }
    ch->pcdata->followers++;
 
    victim=create_mobile( get_mob_index( MOB_VNUM_SERVANT ) );
    victim->level = 300;
    victim->hit = 30000;
    victim->max_hit = 30000;
    victim->hitroll = 100;
    victim->damroll = 100;
    victim->armor = 700;
 
    sprintf(buf,"Come forth, creature of darkness, and do my bidding!");
    do_say( ch, buf );
 
    send_to_char( "A demonic servant bursts from the ground and bows before you.\n\r",ch );
    act( "$N bursts from the ground and bows before $n.", ch, NULL, victim, TO_ROOM );
 
    char_to_room( victim, ch->in_room );
 
 
    add_follower( victim, ch );
    af.duration  = 666;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    SET_BIT(victim->act, ACT_NOEXP); 
    return;
}
/* Beckon */
void do_beckon(CHAR_DATA *ch, char *argument  )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    one_argument( argument,arg);
 
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch,CLASS_VAMPIRE))
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }
    if (ch->power[DISC_VAMP_ANIM] < 1)
    {
    send_to_char("You require level 1 animalism to beckon the wild.\n\r",ch);
    return;
    }
    if ( arg[0] == '\0' )
    {
        send_to_char( "You can beckon a frog, a raven, a cat or a dog.\n\r", ch );
	return;
    }
 
    if (ch->pcdata->followers > 4 )
    {
    send_to_char("Nothing happened.\n\r",ch);
    return;
    }
    
    if ( !str_cmp( argument, "frog" ) )
    {
	victim = create_mobile( get_mob_index( MOB_VNUM_FROG ) );
	if (victim == NULL)
        {send_to_char("Error - please inform Infidel.\n\r",ch); return;}
    }
    else if ( !str_cmp( argument, "raven" ) )
    {
	victim = create_mobile( get_mob_index( MOB_VNUM_RAVEN ) );
	if (victim == NULL)
        {send_to_char("Error - please inform Infidel.\n\r",ch); return;}
    }
    else if ( !str_cmp( argument, "cat" ) )
    {
	victim = create_mobile( get_mob_index( MOB_VNUM_CAT ) );
	if (victim == NULL)
        {send_to_char("Error - please inform Infidel.\n\r",ch); return;}
    }
    else if ( !str_cmp( argument, "dog" ) )
    {
	victim = create_mobile( get_mob_index( MOB_VNUM_DOG ) );
	if (victim == NULL)
        {send_to_char("Error - please inform Infidel.\n\r",ch); return;}
    }
    else 
    {
        send_to_char( "Your can beckon a frog, a raven, a cat or a dog.\n\r", ch );
	return;
    }
 
    act( "You whistle loudly and $N walks in.", ch, NULL, victim, TO_CHAR);
    act( "$n whistles loudly and $N walks in.", ch, NULL, victim, TO_ROOM);
    char_to_room( victim, ch->in_room );
    ch->pcdata->followers += 1;
    SET_BIT(victim->act, ACT_NOEXP);
    return;
}
void do_spit( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;

    char buf[MAX_STRING_LENGTH];
  
  int dam;
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch,CLASS_VAMPIRE)) 
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }
    if (ch->power[DISC_VAMP_QUIE] < 1)
    {
    send_to_char("You need level 1 Quietus to spit.\n\r",ch);
    return;
    }
 
 
    if ( ( victim = ch->fighting ) == NULL )
    {
send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;

    }
    if (ch->pcdata->condition[COND_THIRST] < 5)
    {
    send_to_char("You dont have enough blood.\n\r",ch); 
    return; 
    }
    WAIT_STATE( ch, 12 );
    if (!IS_NPC(victim))
    {
    dam = ch->power[DISC_VAMP_QUIE] * 50;
    } 
    else if (IS_NPC(victim))
    {
    dam = ch->power[DISC_VAMP_QUIE] * 250;
    }
    ch->pcdata->condition[COND_THIRST] -= 5;
    if ( !IS_NPC(victim) && IS_CLASS(victim, CLASS_WEREWOLF) )
    {
	if (victim->power[DISC_WERE_BOAR] > 2 ) dam *= 0.5;
    }
 
    dam += number_range(1,30);
    if ( dam <= 0 )
       dam = 1;
    set_fighting(ch,victim);

sprintf(buf,"Your spit of acid hits $N incredibly hard! [%d]",dam);
act(buf,ch,NULL,victim,TO_CHAR);
sprintf(buf,"$n's spit of acid hits you incredibly hard! [%d]",dam);
act(buf,ch,NULL,victim,TO_VICT);
sprintf(buf,"$n's spit of acid hits $N incredibly hard! [%d]",dam);
act(buf,ch,NULL,victim,TO_NOTVICT);
 

    send_to_char("\n\r",ch);
    hurt_person(ch,victim,dam);
    return;
}

void do_scales(CHAR_DATA *ch,char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch,CLASS_VAMPIRE))
  {
  send_to_char("Huh?\n\r",ch);
  return;
  }
  if (ch->power[DISC_VAMP_SERP] < 5)
  {
  send_to_char("You need Serpentis 6 to get scales.\n\r",ch);
  return;
  }
  if (!IS_SET(ch->newbits, NEW_SKIN))
  {
  send_to_char("Scales slide out of your body and surround you.\n\r",ch);
  ch->armor -= 100 ;
  SET_BIT(ch->newbits, NEW_SKIN);
  return;
  }
  else if (IS_SET(ch->newbits, NEW_SKIN))
  { 
  send_to_char("Your scales slide back into your body.\n\r",ch);
  ch->armor += 100;
  REMOVE_BIT(ch->newbits,NEW_SKIN);
  return;
  }
return;
}

void do_cserpent(CHAR_DATA *ch, char *argument  )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    one_argument( argument,arg);
 
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch,CLASS_VAMPIRE))
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }
    if (ch->power[DISC_VAMP_SERP] < 4)
    {
    send_to_char("You require level 5 serpentis to call a serpent.\n\r",ch);
    return;
    }
/*
    if ( arg[0] == '\0' )
    {
        send_to_char( "You can beckon a serpent at anytime.\n\r", ch );
	return;
    }
*/ 

    if (ch->pcdata->followers > 5 )
    {
    send_to_char("Nothing happened.\n\r",ch);
    return;
    }
    
	victim = create_mobile( get_mob_index( MOB_VNUM_SERPENT ) );
	if (victim == NULL)
        {send_to_char("Error - please inform Trace.\n\r",ch); return;}
 
    act( "You close your eyes and concentrate a huge serpent walks in.", ch, NULL, victim, TO_CHAR);
    act( "$n closes his eyes and concentrates a huge $N walks in.", ch, NULL, victim, TO_ROOM);
    char_to_room( victim, ch->in_room );
    ch->pcdata->followers += 1;
    SET_BIT(victim->act, ACT_NOEXP);
    SET_BIT(victim->act, ACT_MOUNT);
    return;
}

void do_illusion(CHAR_DATA *ch, char *argument  )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    one_argument( argument,arg);
 
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch,CLASS_VAMPIRE))
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }
    if (ch->power[DISC_VAMP_CHIM] < 1)
    {
    send_to_char("You require level 1 chim to make an illusion.\n\r",ch);
    return;
    } 

    if (ch->pcdata->followers > 5 )
    {
    send_to_char("Nothing happened.\n\r",ch);
    return;
    }
    
	victim = create_mobile( get_mob_index( MOB_VNUM_ILLUSION ) );
	if (victim == NULL)
        {send_to_char("Error - please inform Trace.\n\r",ch); return;}
 
    act( "You close your eyes and concentrate an illusion appears in the room.", ch, NULL, victim, TO_CHAR);
    act( "$n closes his eyes and concentrates an $N appears in the room.", ch, NULL, victim, TO_ROOM);
    char_to_room( victim, ch->in_room );
    ch->pcdata->followers += 1;
    SET_BIT(victim->act, ACT_NOEXP);
    return;
}

void do_bloodwall(CHAR_DATA *ch, char *argument)
{
OBJ_DATA *obj = NULL;
OBJ_DATA *objc;
char arg[MAX_STRING_LENGTH];
char arg2[MAX_STRING_LENGTH];
char wall[MAX_STRING_LENGTH];
char buf[MAX_STRING_LENGTH];
int value;

argument=one_argument(argument,arg);
argument=one_argument(argument,arg2);

if (IS_NPC(ch)) return;

if (!IS_CLASS(ch,CLASS_VAMPIRE))
{
send_to_char("Huh?\n\r",ch);
return;
}
if (ch->power[DISC_VAMP_DAIM] < 2)
{
send_to_char("You need Daiminion 2 to call Blood Walls.\n\r",ch);
return;
}

  if (!str_cmp(ch->in_room->area->name, "diku    midgaard"))
  {
    send_to_char("Not in Midgaard.\n\r",ch);
    return;
  }


if (arg[0] == '\0' || (str_cmp(arg,"n")
                   && str_cmp(arg,"s")
                   && str_cmp(arg,"e")
                   && str_cmp(arg,"w")
                   && str_cmp(arg,"u")
                   && str_cmp(arg,"d")))
{
send_to_char("You may wall n, w, s, e, d or u.\n\r",ch);
return;
}

if (arg2[0] == '\0')
{
send_to_char("Please supply the amount of blood to expend.\n\r",ch);
return;
}

value = is_number( arg2 ) ? atoi( arg2 ) : -1;

if (value > 5 || value < 1)
{
send_to_char("Please select a value between 1 and 5.\n\r",ch);
return;
}

ch->pcdata->condition[COND_THIRST] -= value;
sprintf(buf,"A look of concentration passes over %s's face.",ch->name);
act(buf,ch,NULL,NULL,TO_ROOM);
send_to_char("A look of concentration passes over your face.\n\r",ch);
if (!str_cmp(arg,"n")) sprintf(wall,"walln");
if (!str_cmp(arg,"w")) sprintf(wall,"wallw");
if (!str_cmp(arg,"s")) sprintf(wall,"walls");
if (!str_cmp(arg,"e")) sprintf(wall,"walle");
if (!str_cmp(arg,"d")) sprintf(wall,"walld");
if (!str_cmp(arg,"u")) sprintf(wall,"wallu");
objc = get_obj_list(ch,wall,ch->in_room->contents);
if ( objc != NULL)
{
send_to_char("There is already a wall blocking that direction.\n\r",ch);
return;
}
WAIT_STATE(ch,25);
sprintf(buf,"A wall of blood pours out of the ground.");
act(buf,ch,NULL,NULL,TO_ROOM);
send_to_char(buf,ch);
if (!str_cmp(arg,"n"))
{
obj=create_object(get_obj_index(30043),0);
sprintf(buf,"A wall of blood is here, blocking your exit north.");
free_string(obj->description);
obj->description = str_dup(buf);
}
if (!str_cmp(arg,"s"))
{
obj=create_object(get_obj_index(30044),0);
sprintf(buf,"A wall of blood is here, blocking your exit south.");
free_string(obj->description);
obj->description = str_dup(buf);
}
if (!str_cmp(arg,"e"))
{
obj=create_object(get_obj_index(30045),0);
sprintf(buf,"A wall of blood is here, blocking your exit east.");
free_string(obj->description);
obj->description = str_dup(buf);
}
if (!str_cmp(arg,"w"))
{
obj=create_object(get_obj_index(30046),0);
sprintf(buf,"A wall of blood is here, blocking your exit west.");
free_string(obj->description);
obj->description = str_dup(buf);
}
if (!str_cmp(arg,"d"))
{
obj=create_object(get_obj_index(30047),0);   
sprintf(buf,"A wall of blood is here, blocking your exit down.");
free_string(obj->description);
obj->description = str_dup(buf);
}
if (!str_cmp(arg,"u"))
{
obj=create_object(get_obj_index(30048),0);
sprintf(buf,"A wall of blood is here, blocking your exit up.");
free_string(obj->description);
obj->description = str_dup(buf);
}
obj_to_room(obj,ch->in_room); 
obj->timer=value;
obj->item_type = ITEM_WALL;
return;
}

void do_bloodrod( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
 
    if (ch->power[DISC_VAMP_THAU] < 2)
    {
        send_to_char("You need to obtain level 2 Thaumaturgy to create a Blood Rod.\n\r",ch);
        return;
    }

    if ( 10 > ch->practice)
    {
        send_to_char("It costs 10 points of primal to create a Blood Rod.\n\r",ch);
        return;
    }
        ch->practice   -= 10;
        obj = create_object(get_obj_index(OBJ_VNUM_BROD)  ,0 );
        obj_to_char(obj, ch);
        act("A Rod of Blood appears in your hands.",ch,NULL,NULL,TO_CHAR);
        act("A Rod of blood appears in $n's hands.",ch,NULL,NULL,TO_ROOM);
        return;
}
 
void do_shadowgaze(CHAR_DATA *ch, char *argument  )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
   one_argument( argument,arg); 
 
    if (IS_NPC(ch)) return;   
    if (!IS_CLASS(ch,CLASS_VAMPIRE))
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }
   if (ch->power[DISC_VAMP_OBTE] < 10)
   {
   send_to_char("You need level 10 Obtenebration to use Shadow Gaze.\n\r",ch);
   return;
   }

   if (!IS_AFFECTED(ch, AFF_SHADOWPLANE))
   { 
        send_to_char("You need to be in the Shadowplane, first!\n\r",ch);
        return;
   } 
 
    
    if ( arg[0] == '\0' )
    {
        send_to_char( "Who do you wish to use Shadow Gaze on?\n\r", ch );
        return;
    }
    if ((victim = get_char_room( ch, arg)) == NULL)
    {
    send_to_char("They arent here.\n\r",ch);
    return;
    }
        
    if (IS_AFFECTED(victim, AFF_SHADOWPLANE))
   {
        send_to_char("They are already in the Shadowplane!\n\r",ch);
        return;
   }
 
    if (!IS_NPC(victim))
    {
    send_to_char("You cannot Shadow Gaze a person.\n\r",ch);
    return;
    }
    sprintf(buf,
    "%s gazes intently at %s.",ch->name,victim->short_descr);
    act(buf,ch,NULL,NULL,TO_ROOM);
    sprintf(buf,
    "You gaze deeply into %s's eyes.\n\r",victim->short_descr);
    send_to_char(buf,ch);
    WAIT_STATE(ch, 8);
    sprintf(buf,
    "You pull %s into the Shadowplane!",victim->short_descr);
    act(buf,ch,NULL,NULL,TO_CHAR);
    sprintf(buf,
    "%s rips %s into the Shadowplane!"
    ,ch->name,victim->short_descr);
    act(buf,ch,NULL,NULL,TO_ROOM);
    SET_BIT(victim->affected_by, AFF_SHADOWPLANE);
    return;
}    
        
        
void do_grab(CHAR_DATA *ch, char *argument  )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;   
    char buf[MAX_STRING_LENGTH];
   one_argument( argument,arg);
        
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch,CLASS_VAMPIRE))
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }
   if (ch->power[DISC_VAMP_OBTE] < 8)
   {
   send_to_char("You need level 8 Obtenebration to use Grab.\n\r",ch);
   return;
   }
    
   if (!IS_AFFECTED(ch, AFF_SHADOWPLANE))
   {
        send_to_char("You need to be in the Shadowplane, first!\n\r",ch);
        return;
   }
  if (ch->in_room != NULL)
  {
    if (IS_SET (ch->in_room->room_flags, ROOM_ARENA))
    {
      send_to_char("Your in the arena.\n\r",ch);
      return;
    }
  }  
    if ( arg[0] == '\0' )
    {
        send_to_char( "Who do you wish to Grab?\n\r", ch );
        return;
    }
    if ((victim = get_char_room( ch, arg)) == NULL)
    {
    send_to_char("They arent here.\n\r",ch);
    return;
    }
    
     if (IS_AFFECTED(victim, AFF_SHADOWPLANE))
   {
        send_to_char("They are already in the Shadowplane!\n\r",ch);
        return;
   } 
        
    if (IS_NPC(victim))
    {
    send_to_char("You cannot Grab a mobile!\n\r",ch);
    return;
    }
    sprintf(buf,
    "%s pulls %s into the Shadowplane.",ch->name,victim->name);
    act(buf,ch,NULL,NULL,TO_ROOM);
    sprintf(buf,
    "You pull %s into the Shadowplane!\n\r",victim->name);
    send_to_char(buf,ch);
    sprintf(buf,
    "%s pulls you into the Shadowplane!\n\r",ch->name);
    act(buf,ch,NULL,victim,TO_VICT);
    WAIT_STATE(ch, 8);
    SET_BIT(victim->affected_by, AFF_SHADOWPLANE);
    return;
}
    
    
void do_share( CHAR_DATA *ch, char *argument )
{   
    CHAR_DATA *victim;
    CHAR_DATA *familiar;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    
    one_argument( argument, arg );
     
    if ( IS_NPC(ch) )
        return;
     
    if (!IS_CLASS(ch, CLASS_VAMPIRE) )
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
   
    if (ch->power[DISC_VAMP_ANIM] < 4)
    {
        send_to_char("You must obtain at least level 4 in Animalism to use Share Spirits.\n\r",ch);
        return;
    }
        
    if ( ( familiar = ch->pcdata->familiar ) != NULL)
    {
        sprintf(buf,"You release %s.\n\r",familiar->short_descr);
        send_to_char( buf, ch );
        familiar->wizard = NULL;
        ch->pcdata->familiar = NULL;
        return;
    }

    if ( arg[0] == '\0' )
    {
        send_to_char( "What do you wish to Share spirits with?\n\r", ch );
        return;
    }
    
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( ch == victim )
    {
        send_to_char( "What an intelligent idea!\n\r", ch );
        return;
    }
    
    if ( !IS_NPC(victim) )
    {
        send_to_char( "Not on players.\n\r", ch );
        return;
    }
    
    if (victim->wizard != NULL)
    {
        send_to_char( "You are unable to Share Spirits them.\n\r", ch );
        return;
    }
    
    if (victim->level > (ch->spl[RED_MAGIC] * 0.25))
    {
        send_to_char( "They are too powerful.\n\r", ch );
        return;
    }   
    
    if ( ch->pcdata->condition[COND_THIRST] < 25 )
    {
        send_to_char("You have insufficient blood.\n\r",ch);
        return;
    }
        
    ch->pcdata->condition[COND_THIRST] -= 25;
    ch->pcdata->familiar = victim;
    victim->wizard = ch; 
    act("You share your being with $N.",ch,NULL,victim,TO_CHAR);
    act("$n is staring at you!",ch,NULL,victim,TO_VICT);
    act("$n starts staring at $N",ch,NULL,victim,TO_NOTVICT);
    return;
}   
    
void do_frenzy( CHAR_DATA *ch, char *argument )
{
/*
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    
    argument = one_argument (argument, arg);
*/
        
    if ( IS_NPC(ch) )
        return;
    
     
    if ( ch->power[DISC_VAMP_ANIM] < 5 )
    {
        send_to_char("You need level 5 Animalism to use Frenzy.\n\r",ch);
        return;
    }
/*   
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
     
    if ( IS_NPC(victim) )
        {
          send_to_char("Not on NPC's!\n\r",ch);
          return;
        }
     
        
        
    if (IS_CLASS(victim, CLASS_VAMPIRE) && arg[0] != '\0')
    {   
        if ( victim->rage >= 25 )
        {
            send_to_char("Their beast is already controlling them!\n\r",ch);
            return;
        }
    
      if (number_percent ( )  <= 50)
        {
            send_to_char("They shake off your attempt.\n\r",ch);
            act("You shake off $n's attempt to frenzy you.\n\r",ch,NULL,victim,TO_VICT);
            act("$N shakes off $n's attempt to frenzy $m.\n\r",ch,NULL,victim,TO_ROOM);
          return;
      }

      if (victim->beast == 0 )
        {
          send_to_char("They are too calm to frenzy!\n\r",ch);
          return;
        }
    
        if (!IS_VAMPAFF(victim, VAM_NIGHTSIGHT)) do_nightsight(victim,"");
        if (!IS_VAMPAFF(victim, VAM_FANGS)) do_fangs(victim,"");
        if (!IS_VAMPAFF(victim, VAM_CLAWS)) do_claws(victim,"");
      act("$n sends you into a frenzied rage!",ch,NULL,victim,TO_VICT);
        act("You excite the beast within $n!",victim,NULL,NULL,TO_CHAR); 
        act("$n bares $s fangs and growls as $s inner beast consumes $m.",ch,NULL,NULL,TO_ROOM);
        ch->rage += number_range(10,20);
        if (victim->beast > 0) do_beastlike(victim,"");
        WAIT_STATE(ch,12);
        return;
    }
    
    send_to_char("But they are already in a rage!\n\r",ch);
    return;
*/
         
    if (IS_CLASS(ch, CLASS_VAMPIRE))
    {
      if (ch->beast >= 20)
      {
        if ( ch->rage >= ch->beast )
        {
            send_to_char("Your beast is already controlling you.\n\r",ch);
            return;
        }
      }  
      if (ch->beast <= 19)
      {
        if ( ch->rage >= 20 )
        {
            send_to_char("Your beast is already controlling you.\n\r",ch);
            return;
        }
      }
            
/*
        if (ch->beast == 0)
        {
           send_to_char("You are too calm to use frenzy.\n\r",ch);
           return;
        }
        
*/
    
        if (!IS_VAMPAFF(ch, VAM_NIGHTSIGHT)) do_nightsight(ch,"");
        if (!IS_VAMPAFF(ch, VAM_FANGS)) do_fangs(ch,"");
        if (!IS_VAMPAFF(ch, VAM_CLAWS)) do_claws(ch,"");
        send_to_char("You bare your fangs and growl as your inner beast consumes you.\n\r",ch);
        act("$n bares $s fangs and growls as $s inner beast consumes $m.",ch,NULL,NULL,TO_ROOM);
        ch->rage += number_range(20,30);
        if (ch->beast > 0) do_beastlike(ch,"");
        WAIT_STATE(ch,12);
        return;
    }
     
    send_to_char("But you are already in a rage!\n\r",ch);
    return;
}
  
void do_shroud( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
        return;
       
    if( !IS_CLASS(ch, CLASS_VAMPIRE) )
    {
        stc( "Huh?\n\r", ch);
        return;
    }
         
    if (IS_CLASS(ch, CLASS_VAMPIRE) && (ch->power[DISC_VAMP_OBTE] < 1))
     { 
          send_to_char("You need level 1 Obtenebration to use this power.\n\r",ch);
          return;
     }
            
    if (ch->fight_timer > 0) {
      send_to_char("Not with a fighttimer.\n\r",ch);
      return;
    }
        
    if ( IS_SET(ch->act, AFF_HIDE) )
    {
        REMOVE_BIT(ch->act, AFF_HIDE);
        send_to_char( "You step out from the shadows.\n\r", ch );
        act("$n emerges from the shadows.",ch,NULL,NULL,TO_ROOM);
    }
    else
    {
        send_to_char( "You slowly blend with the shadows.\n\r", ch );
        act("$n blends in with the shadows and disappears.",ch,NULL,NULL,TO_ROOM);
        SET_BIT(ch->act, AFF_HIDE);
    }
    return;
}
        
void do_summon(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    CHAR_DATA *mount;
    char arg [MAX_INPUT_LENGTH];
 
    argument = one_argument (argument,arg);

 
    if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
    {
      do_nosummon(ch,"");
      return;
    }
        
    if (arg[0] == '\0')
    {
        send_to_char("Summon whom?\n\r",ch);
        return;
    }  
          
    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    { 
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
      
     
    if   (victim == ch
    ||   victim->in_room == NULL
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
    ||   victim->level >= ch->level + 5
    ||   victim->fighting != NULL
    ||   (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON) ))
     
    {
        send_to_char( "You cannot locate them.\n\r", ch );
        return;
    }
 
    if (ch->power[DISC_VAMP_PRES] < 4)
    {
        send_to_char("You need level 4 in Presence to use this.\n\r",ch);
        return;
    }
    WAIT_STATE(ch,10);
    act( "$n disappears suddenly.", victim, NULL, NULL, TO_ROOM );
    char_from_room( victim );
    char_to_room( victim, ch->in_room );
    act( "$n arrives suddenly.", victim, NULL, NULL, TO_ROOM );
    act( "$N has summoned you!", victim, NULL, ch,   TO_CHAR );
    do_look( victim, "auto" );
    if ( (mount = victim->mount) == NULL ) return;
    char_from_room( mount );
    char_to_room( mount, get_room_index(victim->in_room->vnum) );
    do_look( mount, "auto" );
    return;
}
        
        
void do_drain( CHAR_DATA *ch, char *argument )
{         
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    int dam;
        
    argument = one_argument( argument, arg );
      
    if ( IS_NPC(ch) )
        return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
     {
          send_to_char("Huh?\n\r",ch);
          return;
     }
    
    if (ch->power[DISC_VAMP_THAN] < 5 )
     {
        send_to_char("You must obtain level 5 Thanatosis to use Drain Life.\n\r",ch);
        return;
     }
     
 
   if ( ( victim = get_char_room( ch, arg ) ) == NULL && (ch->fighting) == NULL)
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
   if ((ch->fighting) != NULL)
   {
   victim = ch->fighting;
   }
 
   if ( arg[0] == '\0' && (ch->fighting) == NULL)
    {
        send_to_char( "Whose life do you wish to drain?\n\r", ch );
        return;
    }
    
    if (victim->hit < 500)
        {
           send_to_char("They are far to hurt to do that!\n\r",ch);
           return;
        }
          
         if (is_safe(ch, victim))
         {
       send_to_char("Not while in a safe room!\n\r", ch);
       return;
    }
      
    if (IS_NPC(victim))
        {
          dam = ch->power[DISC_VAMP_THAN] * 10;
        victim->hit = (victim->hit - dam);
        if (dam > 500) dam = number_range( 450, 550 );
        ch->hit = (ch->hit + dam);
        if (ch->hit > ch->max_hit + 1000)
        ch->hit = ch->max_hit + 1000;
        act("You drain the lifeforce out of $N.", ch, NULL, victim, TO_CHAR);
        act("$n drains the lifeforce out of $N.", ch, NULL, victim, TO_NOTVICT);
        act("You feel $n draining your lifeforce.", ch, NULL, victim, TO_VICT);
        WAIT_STATE(ch,12);
          return;
      }
     
    if (!IS_NPC(victim))
        {
          dam = ch->power[DISC_VAMP_THAN] * 50;
        victim->hit = (victim->hit - dam);
        if (dam > 400) dam = number_range( 350, 450 );
        ch->hit = (ch->hit + dam);
        if (ch->hit > ch->max_hit + 1000)
        ch->hit = ch->max_hit + 1000;
        act("You drain the lifeforce out of $N.", ch, NULL, victim, TO_CHAR);
        act("$n drains the lifeforce out of $N.", ch, NULL, victim, TO_NOTVICT);
        act("You feel $n draining your lifeforce.", ch, NULL, victim, TO_VICT);
        WAIT_STATE(ch,12);
          return;
      }
        
     
}   
    
void do_flamehands( CHAR_DATA *ch, char *argument)
{
    if ( IS_NPC(ch) )
     return;
          
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
          send_to_char("Huh?\n\r",ch);
          return;
    }
      
    if (ch->power[DISC_VAMP_PROT] < 5)
    {
          send_to_char("You need level 5 Protean to use flaming hands.\n\r",ch);
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
        
void do_cauldron( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    int value;   
    int dam;
        
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    
    value = is_number( arg2 ) ? atoi( arg2 ) : -1;
 
    if ( IS_NPC(ch) )
        return;
          
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
     {
          send_to_char("Huh?\n\r",ch);
          return;
     }
      
    if (ch->power[DISC_VAMP_THAU] < 2)
     {
        send_to_char("You need at least level 2 Thaumaturgy to use this power.\n\r",ch);
        return;  
     }
        
    
    if ( arg1[0] == '\0' )
    {
        send_to_char( "Whose blood do you wish to boil?\n\r", ch );
        return;
    }
     if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
          
    if (arg2[0] == '\0' )
    {
        send_to_char( "How much blood do you want to use?\n\r", ch );
        return;
    }
 
    if (value > 200)  
    {
        stc("You can only use 200 blood on this power.\n\r",ch);
        return;  
    }
        
    
    if (ch->pcdata->condition[COND_THIRST] < value )
        {
           send_to_char("You don't have that much blood!\n\r",ch);
           return;
        }
        
if (is_safe(ch,victim) == TRUE) return;
    
    if (IS_NPC(victim))
        {
        dam = (value * 4);
        ch->pcdata->condition[COND_THIRST] = ch->pcdata->condition[COND_THIRST] - value;
        act("$N screams in agony as $m blood begins to boil.", ch, NULL, victim, TO_CHAR);
        act("$N screams in agony as $n causes his blood to boil.", ch, NULL, victim, TO_NOTVICT);
        act("You scream in agony as $n causes your blood to boil.", ch, NULL, victim, TO_VICT);
        hurt_person(ch, victim, dam);
        WAIT_STATE(ch,12);
          return;
      } 
    
    if (!IS_NPC(victim))  
        {
          dam = value * 1.5;
        hurt_person(ch, victim, dam);
        if (victim->pcdata->condition[COND_THIRST] < value)
        victim->pcdata->condition[COND_THIRST] = 1;
        victim->pcdata->condition[COND_THIRST] = victim->pcdata->condition[COND_THIRST] - value;
        ch->pcdata->condition[COND_THIRST] = ch->pcdata->condition[COND_THIRST] - value;
        act("$N screams in agony as $m blood begins to boil.", ch, NULL, victim, TO_CHAR);
        act("$N screams in agony as $n causes his blood to boil.", ch, NULL, victim, TO_NOTVICT);
        act("You scream in agony as $n causes your blood to boil.", ch, NULL, victim, TO_VICT);
        WAIT_STATE(ch,12);
          return;
     }
        
     
}
    
     
void do_bonemod( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
        
    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
         
           
        if (IS_NPC(ch)) return;
         
        if (!IS_CLASS(ch, CLASS_VAMPIRE))
        {
           send_to_char("Huh?\n\r",ch);
           return;
        }
        if (ch->power[DISC_VAMP_VICI] < 3)
        {
           send_to_char("You need level 3 Vicissitude to use this power.\n\r",ch);
           return;
        }

    if ( arg1[0] == '\0') 
    {
        send_to_char( "Syntax: bonemod <modification>\n\r",     ch );
        send_to_char( "Horns, Wings, Head, Exoskeleton, Tail\n\r",ch);
        return;
    }
          
        
    /*
     * Set something.
     */
    if ( !str_cmp( arg1, "horns" ) )
    {
        
        if (!IS_VAMPAFF(ch, VAM_HORNS))
         {
          SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_HORNS);
          send_to_char("You pull a set of horns out of your head.\n\r",ch);
          act("$n pulls a set of horns from his head!\n\r",ch,NULL,NULL,TO_ROOM);
          return;
       }
    
        if (IS_VAMPAFF(ch, VAM_HORNS))
         {
          REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_HORNS);
          send_to_char("You push your horns back into your head.\n\r",ch);
          act("$n pushes $n's horns back into $n head.\n\r",ch,NULL,NULL,TO_ROOM);
          return;
    
         }
    }      
 
    if ( !str_cmp( arg1, "wings" ) )
    {
         
        if (!IS_VAMPAFF(ch, VAM_WINGS))
         {
          SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_WINGS);
          send_to_char("You pull a pair of leathery wings from your back.\n\r",ch);
          act("$n pulls a pair of leathery wings from $s back!\n\r",ch,NULL,NULL,TO_ROOM);
          return;
       }
         
        if (IS_VAMPAFF(ch, VAM_WINGS))
         {
          REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_WINGS);
          send_to_char("You push your wings into your back.\n\r",ch);
          act("$n pushes $s wings into $s back.\n\r",ch,NULL,NULL,TO_ROOM);
          return;
     
         }
    }   
      
     if ( !str_cmp( arg1, "exoskeleton" ) )
    {  
    
        if (!IS_VAMPAFF(ch, VAM_EXOSKELETON))
         {
          SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_EXOSKELETON);
          send_to_char("Your skin is covered by a hard exoskeleton.\n\r",ch);
          act("$n's skin is covered by a hard exoskeleton!\n\r",ch,NULL,NULL,TO_ROOM);
          return;
       }
     
        if (IS_VAMPAFF(ch, VAM_EXOSKELETON))
         {
          REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_EXOSKELETON);
          send_to_char("Your exoskeleton slowly disappears under your skin.\n\r",ch);
          act("$n's hard exoskeleton disappears under $s skin.\n\r",ch,NULL,NULL,TO_ROOM);
          return;
        
         }
    }
          
     if ( !str_cmp( arg1, "tail" ) )
    {
    
        if (!IS_VAMPAFF(ch, VAM_TAIL))
         {
          SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_TAIL);
          send_to_char("Your spine extends out into a long, pointed tail.\n\r",ch);
          act("$n's spine extends to form a long pointed tail!\n\r",ch,NULL,NULL,TO_ROOM);
          return;
       }
          
        if (IS_VAMPAFF(ch, VAM_TAIL))
         {
          REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_TAIL);
          send_to_char("Your tail slowly retracts into your spine.\n\r",ch);
          act("$n's tail shrinks and vanishes into $s spine.\n\r",ch,NULL,NULL,TO_ROOM);
          return;
          
         }
    }
          
     if ( !str_cmp( arg1, "head" ) )
    { 
     
        if (!IS_VAMPAFF(ch, VAM_HEAD))
         {
          SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_HEAD);
          send_to_char("Your head transforms into that of a fierce lion.\n\r",ch);
          act("$n's head transforms into that of a fierce lion!\n\r",ch,NULL,NULL,TO_ROOM);
          return;
       }
          
        if (IS_VAMPAFF(ch, VAM_HEAD))
         {
          REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_HEAD);
          send_to_char("Your head slowly resumes it's normal form.\n\r",ch);
          act("$n's head resumes its normal form.\n\r",ch,NULL,NULL,TO_ROOM);
          return;
          
         }
    }   
          
     
          
     
    /*
     * Generate usage message.
     */
    do_bonemod( ch, "" );
    return;
}

