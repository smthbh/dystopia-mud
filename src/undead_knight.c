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

/**************************
 * Undead knights by Jobo * 
 **************************/

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


void do_ride ( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_INPUT_LENGTH];
  argument = one_argument(argument, arg);
      
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }  
  if (ch->move < 600)
  {
    stc("You do not have enough vitality to do that!\n\r",ch);
    return;
  }
  if (arg[0] == '\0')
  {
    send_to_char( "Ride your skeleton steed to whom?\n\r", ch );
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
    send_to_char("They don't want you near them!\n\r",ch);
    return;
  }
  if ( room_is_private(victim->in_room ) )
  {
    send_to_char( "That room is private right now.\n\r", ch );
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
    sprintf(buf,"You ride your skeleton steed to %s!\n\r",victim->short_descr);
    send_to_char(buf, ch);
  }
  if (!IS_NPC(victim))
  {
    sprintf(buf,"You ride your skeleton steed to %s!\n\r",victim->name);
    send_to_char(buf, ch);
  }
  act("$n rides toward you on $n's skeleton steed!\n\r",ch,NULL,NULL,TO_ROOM);
  do_look(ch, "auto");
  ch->move -= 600;
  return;
}

void do_knightarmor( CHAR_DATA *ch, char *argument )
{
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];
  int vnum = 0;

  argument = one_argument( argument, arg );
  if (IS_NPC(ch)) return;
  if( !IS_CLASS(ch, CLASS_UNDEAD_KNIGHT) )
  {
    send_to_char("You are not one of the undead!!!\n\r",ch);
    return;
  }
  if (arg[0] == '\0')
  {
    send_to_char("Please specify which piece of unholy armor you wish to make: plate ring bracer collar helmet leggings boots gauntlets chains cloak belt visor longsword shortsword.\n\r",ch);
    return;
  }
  if ( ch->practice < 150 )
  {
    send_to_char("It costs 150 points of primal to create a piece of unholy armor.\n\r",ch);
    return;
  }
  if (!str_cmp(arg,"plate")) vnum = 29975;
  else if (!str_cmp(arg,"longsword")) vnum = 29976;
  else if (!str_cmp(arg,"shortsword")) vnum = 29977;
  else if (!str_cmp(arg,"ring")) vnum = 29978;
  else if (!str_cmp(arg,"bracer")) vnum = 29979;
  else if (!str_cmp(arg,"collar")) vnum = 29980;
  else if (!str_cmp(arg,"helmet")) vnum = 29981;
  else if (!str_cmp(arg,"leggings")) vnum = 29982;
  else if (!str_cmp(arg,"boots")) vnum = 29983;
  else if (!str_cmp(arg,"gauntlets")) vnum = 29984;
  else if (!str_cmp(arg,"chains")) vnum = 29985;
  else if (!str_cmp(arg,"cloak")) vnum = 29986;
  else if (!str_cmp(arg,"belt")) vnum = 29987;
  else if (!str_cmp(arg,"visor")) vnum = 29988;
  else
  {
    send_to_char("Please specify which piece of unholy armor you wish to make: plate ring bracer collar helmet leggings boots gauntlets chains cloak belt visor longsword shortsword.\n\r",ch);
    return;
  }
  if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
  {
    send_to_char("Missing object, please inform Jobo.\n\r",ch);
    return;
  }
  obj = create_object(pObjIndex, 50);
  obj->questowner = str_dup(ch->pcdata->switchname);
  obj_to_char(obj, ch);
  act("$p appears in your hands.",ch,obj,NULL,TO_CHAR);
  act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
  ch->practice -=150;
  return;
}

void do_unholyrite (CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if( !IS_CLASS(ch, CLASS_UNDEAD_KNIGHT) && !IS_CLASS(ch, CLASS_TANARRI))
  {
    send_to_char("Huh.\n\r",ch);
    return;
  }
  if (ch->mana < 500)
  {
    send_to_char("You don't have the mystical energies to make the rite.\n\r",ch);
    return;
  }
  if (ch->hit < ch->max_hit)
  {
    ch->hit += number_range(500,1000);
    send_to_char("You make a blood sacrifice to the god of Death.\n\r",ch);
  }
  if (ch->hit > ch->max_hit) ch->hit=ch->max_hit;
  ch->mana -= 500;
  WAIT_STATE(ch,18);
}

void do_aura (CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  
  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;
  if( !IS_CLASS(ch, CLASS_UNDEAD_KNIGHT) )
  {
    send_to_char("Huh.\n\r",ch);
    return;
  }
  if (arg[0] == '\0')
  {
    send_to_char("What aura do you wish to activate/deactivate ? [bog/death/fear/might].\n\r",ch);
    return;
  }
  if (!str_cmp(arg,"bog"))
  {
    if (ch->pcdata->powers[NECROMANCY] < 6)
    {
      send_to_char("You don't have that aura yet.\n\r",ch);
      return;
    }
    if (IS_SET(ch->pcdata->powers[AURAS], BOG_AURA))
    {
      REMOVE_BIT(ch->pcdata->powers[AURAS], BOG_AURA);
      send_to_char("Your aura of Bog fades.\n\r",ch);
      return;
    }
    else
    {
      send_to_char("An aura of bog surrounds you.\n\r",ch);
      SET_BIT(ch->pcdata->powers[AURAS], BOG_AURA);
      return;
    }
  }
  if (!str_cmp(arg,"might"))
  {
    if (ch->pcdata->powers[NECROMANCY] < 4)
    {
      send_to_char("You don't have that aura yet.\n\r",ch);
      return;
    }
    if (IS_SET(ch->pcdata->powers[AURAS], MIGHT_AURA))
    {
      REMOVE_BIT(ch->pcdata->powers[AURAS], MIGHT_AURA);
      send_to_char("Your aura of might fades.\n\r",ch);
      ch->damroll -= 300;
      ch->hitroll -= 300;
      return;
    }
    else
    {   
      send_to_char("An aura of might surrounds you.\n\r",ch);
      SET_BIT(ch->pcdata->powers[AURAS], MIGHT_AURA);
      ch->damroll += 300;
      ch->hitroll += 300;  
      return;
    }
  }  
  else if (!str_cmp(arg,"death"))
  {
    if (ch->pcdata->powers[NECROMANCY] < 2) 
    {
      send_to_char("You don't have that aura yet.\n\r",ch);
      return;
    }
    if (IS_SET(ch->pcdata->powers[AURAS], DEATH_AURA))
    {
      REMOVE_BIT(ch->pcdata->powers[AURAS], DEATH_AURA);
      send_to_char("Your aura of death fades away.\n\r",ch);
      return;
    }
    else
    {
      SET_BIT(ch->pcdata->powers[AURAS], DEATH_AURA);
      send_to_char("An aura of death surrounds you.\n\r",ch);
      return;
    }
  }
  else if (!str_cmp(arg,"fear"))
  {  
    if (ch->pcdata->powers[NECROMANCY] < 9)
    {
      send_to_char("You don't have that aura yet.\n\r",ch);
      return;
    }
    if (IS_SET(ch->pcdata->powers[AURAS], FEAR_AURA))
    {      
      REMOVE_BIT(ch->pcdata->powers[AURAS], FEAR_AURA);
      send_to_char("Your aura of fear fades away.\n\r",ch);
      return;
    }
    else
    {
      SET_BIT(ch->pcdata->powers[AURAS], FEAR_AURA);
      send_to_char("An aura of fear surrounds you.\n\r",ch);
      return;
    }
  }
  return;
}

void do_gain (CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];

  argument = one_argument( argument, arg );
  if (IS_NPC(ch)) return;
  if( !IS_CLASS(ch, CLASS_UNDEAD_KNIGHT) )
  {
    send_to_char("Huh.\n\r",ch);
    return;
  }
  if (arg[0] == '\0')
  {
    sprintf(buf, "Current powers : #LNecromancy [#r%d#L]       Invocation [#r%d#L]         Spirit [#r%d#L]#n\n\r",
      ch->pcdata->powers[NECROMANCY], ch->pcdata->powers[INVOCATION], ch->pcdata->powers[SPIRIT]);
    send_to_char(buf,ch);
    send_to_char("What path of power do you wish to increase? [necromancy,invocation,spirit]\n\r",ch);
    return;
  }
  if (!str_cmp(arg,"necromancy"))  /* most powers over death */
  {
    if (ch->pcdata->powers[NECROMANCY]>9)
    {
      send_to_char("You have mastered the art of necromancy.\n\r",ch);
      return;
    }
    else if (ch->practice < ch->pcdata->powers[NECROMANCY] * 60 + 60)
    {
      send_to_char("Your control over the forces of life and death is not powerful enough.\n\r",ch);
      return;
    }
    else
    {
      ch->practice -= ch->pcdata->powers[NECROMANCY] * 60 + 60;
      send_to_char("Death and life is yours to command.\n\r",ch);
      ch->pcdata->powers[NECROMANCY] += 1;
      return;
    }
  }
  else if (!str_cmp(arg,"invocation"))  /* attacks like powerwords */
  {
    if (ch->pcdata->powers[INVOCATION]>4)
    {
      send_to_char("You have mastered the art of invocation.\n\r",ch);
      return;
    }
    else if (ch->practice < ch->pcdata->powers[INVOCATION] * 60 + 60)
    {
      send_to_char("You are not ready to advance in your magical studies.\n\r",ch);
      return;
    }
    else
    {
      ch->practice -= ch->pcdata->powers[INVOCATION] * 60 + 60;
      send_to_char("Your mastery of the ancient arts increase.\n\r",ch);
      ch->pcdata->powers[INVOCATION] += 1;
      return;
    }
  }
  else if (!str_cmp(arg,"spirit"))  /* toughness */
  {
    if (ch->pcdata->powers[UNDEAD_SPIRIT] > 9)
    {
      send_to_char("You have completely bound your spirit to this vessel.\n\r",ch);
      return;
    }
    if (ch->practice < ch->pcdata->powers[UNDEAD_SPIRIT] * 60 + 60)
    {
      send_to_char("You are not ready to bind more of your spirit yet.\n\r",ch);
      return;
    }
    else  
    {
      ch->practice -= ch->pcdata->powers[UNDEAD_SPIRIT] * 60 + 60;
      send_to_char("You channel more of your spirit from the abyss into this body.\n\r",ch);
      ch->pcdata->powers[UNDEAD_SPIRIT] += 1;
      return;
    }
  }
  return;
}

void do_weaponpractice (CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if( !IS_CLASS(ch, CLASS_UNDEAD_KNIGHT) )
  {
    send_to_char("You are not one of the undead!!!\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[WEAPONSKILL] > 9)
  {
    send_to_char("You have already mastered the art of combat.\n\r",ch);
    return;
  }
  if (ch->practice < ch->pcdata->powers[WEAPONSKILL]*60 + 60)
  {
    send_to_char("You are not ready to train your weaponskill.\n\r",ch);
    return;
  }
  else  
  {
    ch->practice -= ch->pcdata->powers[WEAPONSKILL]*60 + 60;
    send_to_char("You feel your skills with weapons increase as you make your bloodsacrifice.\n\r",ch);
    ch->hit = 1;
    ch->mana = 1;
    ch->move = 1;
    ch->pcdata->powers[WEAPONSKILL] += 1;
    return;
  }
}

void do_powerword (CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  char buf1[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  char buf3[MAX_STRING_LENGTH];
  AFFECT_DATA af;
  CHAR_DATA *ich;
  CHAR_DATA *ich_next;
  CHAR_DATA *victim;
  
  argument = one_argument( argument, arg );
  argument = one_argument( argument, arg2 );
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
  {
    send_to_char("Huh.\n\r",ch);
    return;
  }
  if (arg[0] == '\0')
  {
    send_to_char("Valid powerwords are kill,stun,blind and flames.\n\r",ch);
    return;
  }
  if (!str_cmp(arg,"stun") && ch->pcdata->powers[INVOCATION] > 4)
  {
    if (!(arg2[0] == '\0') || ch->fighting != NULL)
    {
      if (arg2[0] == '\0') victim = ch->fighting;
      else if ((victim = get_char_room(ch, arg2)) == NULL)
      {
        send_to_char("They are not here.\n\r", ch );
        return;
      }
      if (ch->pcdata->powers[POWER_TICK] > 0)
      {
        send_to_char("You cannot cast another powerword yet.\n\r",ch);
        return;
      }
      if (is_safe(ch, victim)) return;
      act("$n points his finger at $N and says '#rFREEZE!!!#n'.",ch,NULL,victim,TO_NOTVICT);
      act("You point your finger at $N and say '#rFREEZE!!!#n'.",ch,NULL,victim,TO_CHAR);
      act("$n points his finger at $N and says '#rFREEZE!!!#n'.",ch,NULL,victim,TO_VICT);
      WAIT_STATE(victim, 24);
      WAIT_STATE(ch, 8);
      ch->pcdata->powers[POWER_TICK] = 4;
      return;
    }
    else
    {
      send_to_char("Stun whom?\n\r",ch);
      return;
    }
  }
  else if (!str_cmp(arg,"blind") && ch->pcdata->powers[INVOCATION] > 0)
  {
    if (!(arg2[0] == '\0') || ch->fighting != NULL)
    {
      if (arg2[0] == '\0') victim = ch->fighting;
      else if ((victim = get_char_room(ch, arg2)) == NULL)
      {
        send_to_char("They are not here.\n\r", ch );
        return;
      }
      if (ch->pcdata->powers[POWER_TICK] > 0)
      {
        send_to_char("You cannot cast another powerword yet.\n\r",ch);
        return;
      }
      if (is_safe(ch, victim)) return;
      act("$n touches $N's forehead and screams '#rMIDNIGHT!!!#n'.",ch,NULL,victim,TO_NOTVICT);
      act("You touch $N's forehead and screams '#rMIDNIGHT!!!#n'.",ch,NULL,victim,TO_CHAR);
      act("$n touches your forehead and screams '#rMIDNIGHT!!!#n'.",ch,NULL,victim,TO_VICT);
      if (IS_SET(victim->act,PLR_HOLYLIGHT)) REMOVE_BIT(victim->act,PLR_HOLYLIGHT);
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
      WAIT_STATE(ch,12);
      ch->pcdata->powers[POWER_TICK] = 3;
      return;
    }
    else
    {
      send_to_char("Blind whom?\n\r",ch);
      return;
    }
  }
  else if (!str_cmp(arg,"kill") && ch->pcdata->powers[INVOCATION] > 2)
  {
    if (!(arg2[0] == '\0'))
    {
      if ((victim = get_char_room(ch, arg2)) == NULL)
      {
        send_to_char("They are not here.\n\r", ch );
        return;
      }
      if (ch->pcdata->powers[POWER_TICK] > 0)
      {
        send_to_char("You cannot cast another powerword yet.\n\r",ch);
        return;
      }
      if (ch == victim)
      {
        send_to_char("Not yourself, silly!\n\r",ch);
        return;
      }
      if (is_safe(ch, victim)) return;
      act("$n looks at $N and utters '#rDIE!!!#n'.",ch,NULL,victim,TO_NOTVICT);
      act("You look at $N and say '#rDIE!!!#n'.",ch,NULL,victim,TO_CHAR);
      act("$n looks at you and utters '#rDIE!!!#n'.",ch,NULL,victim,TO_VICT);
      if (victim->position > POS_STUNNED)
      {
        if (victim->fighting == NULL) set_fighting(victim, ch);
        if (ch->fighting == NULL) set_fighting(ch, victim);
      }
      if (!IS_NPC(victim) || victim->level > 100)
      {
        int dam = victim->hit *.1;
        if (IS_NPC(victim) && dam > 5000) dam = 5000;
        if (!IS_NPC(victim) && dam > 1500) dam = 1500;
        hurt_person(ch,victim,dam);
        sprintf(buf1,"$n's powerword strikes $N [#C%d#n]" , dam);
        sprintf(buf2,"Your powerword strikes $N [#C%d#n]" , dam);
        sprintf(buf3,"$n's powerword strikes you [#C%d#n]" , dam);
        act(buf1,ch,NULL,victim, TO_NOTVICT);
        act(buf2,ch,NULL,victim, TO_CHAR);
        act(buf3,ch,NULL,victim, TO_VICT);
        ch->pcdata->powers[POWER_TICK] = 2;
        return;
      }
      ch->level = 12;
      do_slay(ch, arg2);
      ch->level = 3;
      ch->pcdata->powers[POWER_TICK] = 2;
      return;
    }
    else
    {
      send_to_char("Kill whom?\n\r",ch);
      return;
    }
  }
  else if (!str_cmp(arg,"flames") && ch->pcdata->powers[INVOCATION] > 3)
  {
    if (ch->pcdata->powers[POWER_TICK] > 0)
    {
      send_to_char("You cannot cast another powerword yet.\n\r",ch);
      return;
    }
    stc("#gYou raise your hands to encompass the room and scream, #n'#rBURN!!!#n'\n\r",ch);
    ich_next = ch->in_room->people;
    ich = ich_next;
    while (ich_next != NULL)
    {
      ich_next = ich->next_in_room;
      if (ich != ch)
      {
         if (is_safe(ch,ich)) break;
         one_hit(ch,ich,gsn_fireball,1);
         one_hit(ch,ich,gsn_fireball,1);
      }
      ich = ich_next;
    }
    WAIT_STATE(ch,12);
    ch->pcdata->powers[POWER_TICK] = 2;
    return;
  }
  else
  {
    send_to_char("You have not learned that powerword yet.\n\r",ch);
    return;
  }
}
