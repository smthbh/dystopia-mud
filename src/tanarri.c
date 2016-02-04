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

/***************************************
 * Tanar'ri class by Jobo for Dystopia *
 ***************************************/

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

void do_taneq(CHAR_DATA *ch, char *argument)
{
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];
  int vnum = 0;
    
  argument = one_argument( argument, arg );
  if (IS_NPC(ch)) return;
  if( !IS_CLASS(ch, CLASS_TANARRI))
  {
    send_to_char("What?\n\r",ch);
    return;
  }
  if (arg[0] == '\0')
  {
    send_to_char("Please specify which piece of tanar'ri armor you wish to make: Claymore Bracer Collar Ring Plate Helmet Leggings Boots Gauntlets Sleeves Cloak Belt Visor.\n\r",ch);
    return;
  }
  if ( ch->practice < 150 )
  {
    send_to_char("It costs 150 points of primal to create Tanar'ri equipment.\n\r",ch);
    return;
  }
  if (!str_cmp(arg,"claymore")) vnum = 33200;
  else if (!str_cmp(arg,"bracer")) vnum = 33202;
  else if (!str_cmp(arg,"collar")) vnum = 33203;
  else if (!str_cmp(arg,"ring")) vnum = 33201;
  else if (!str_cmp(arg,"plate")) vnum = 33204;
  else if (!str_cmp(arg,"helmet")) vnum = 33205;
  else if (!str_cmp(arg,"leggings")) vnum = 33209;
  else if (!str_cmp(arg,"boots")) vnum = 33206;
  else if (!str_cmp(arg,"gauntlets")) vnum = 33207;
  else if (!str_cmp(arg,"sleeves")) vnum = 33208;
  else if (!str_cmp(arg,"cloak")) vnum = 33210;
  else if (!str_cmp(arg,"belt")) vnum = 33211;
  else if (!str_cmp(arg,"visor")) vnum = 33212;
  else
  {
    do_taneq(ch,"");
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
  ch->practice -= 150;
  act("$p appears in your hands.",ch,obj,NULL,TO_CHAR);
  act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
  return;
}

void do_chaossurge(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_INPUT_LENGTH];
  int dam;

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_TANARRI) && !IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (IS_CLASS(ch, CLASS_LICH) && ch->pcdata->powers[CHAOS_MAGIC] < 3)
  {
    send_to_char("Yes Master.\n\r",ch);
    return;
  }
  if ((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobiles.\n\r",ch);
    return;
  }
  if (is_safe(ch,victim)) return;
  if (victim == ch)
  {
    send_to_char("That doesn't seem like a good idea.\n\r",ch);
    return;
  }
  if (victim->alignment < -500)
  {
    send_to_char("They are too evil, you cannot affect them.\n\r",ch);
    return;
  }
  if (victim->alignment > 500) dam = 1500;
  else if (victim->alignment > 0) dam = 1000;
  else dam = 500;
  sprintf(buf,"Your let chaos envelope $N and $S is hurt [%d]",dam);
  act(buf,ch,NULL,victim,TO_CHAR);
  sprintf(buf,"$n points a wicked finger at you and chaos strikes your mind! [%d]",dam);
  act(buf,ch,NULL,victim,TO_VICT);
  sprintf(buf,"$n strikes $N down with a burst of pure #RCHAOS!#n.");
  act(buf,ch,NULL,victim,TO_NOTVICT);
  hurt_person(ch,victim,dam);
  WAIT_STATE(ch,12);
  return;
}

void do_enmity(CHAR_DATA *ch, char *argument)
{
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  CHAR_DATA *victim1;
  CHAR_DATA *victim2;

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_TANARRI))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_EMNITY))
  {
    send_to_char("you don't have that power yet.\n\r",ch);
    return;
  }
  if ((victim1 = get_char_room(ch, arg1)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if ((victim2 = get_char_room(ch, arg2)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (IS_NPC(victim1) || IS_NPC(victim2))
  {
    send_to_char("Not on NPC's\n\r",ch);
    return;
  }
  do_say(ch,"Xenus, Morkain, Hau!");
  if (number_percent() > 60) do_kill(victim1, victim2->pcdata->switchname);
  else send_to_char("#RSomeone is trying to control your actions!!!#n\n\r",victim1);
  if (number_percent() > 60) do_kill(victim2, victim1->pcdata->switchname);
  else send_to_char("#RSomeone is trying to control your actions!!!#n\n\r",victim2);
  WAIT_STATE(ch,24);
  return;
}

void do_enrage(CHAR_DATA *ch, char *argument) 
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_TANARRI))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_ENRAGE))
  {
    send_to_char("you don't have that power yet.\n\r",ch);
    return;
  }
  if ((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (IS_NPC(victim) || victim->level < 3)
  {
    send_to_char("Not on them\n\r",ch);
    return;
  }
  do_say(ch, "Kara, Kara, Xenos!");
  if (number_percent() > 40) do_berserk2(victim,"");
  else send_to_char("#RSomeone is trying to control your actions!!!#n\n\r",victim);
  WAIT_STATE(ch,18);
  return;
}

void do_bloodsac(CHAR_DATA *ch, char *argument)
{
  int cost = 2;
  int i;

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_TANARRI))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->alignment != -1000) ch->alignment = -1000;
  if (ch->pcdata->rank == 0)
  {
    send_to_char("You are not even a real Tanar'ri.\n\r",ch);
    return;
  }
  if (ch->pcdata->rank*3 <= ch->pcdata->powers[TANARRI_POWER_COUNTER])
  {
    send_to_char("You have gained all powers for your current status.\n\r",ch);
    return;
  }
  for (i = 1; i < ch->pcdata->rank;i++)
    cost = cost * 2;
  if (ch->pcdata->stats[TPOINTS] < 10000 * cost)
  {
    send_to_char("You don't have enough tanar'ri points",ch);
    return;
  }
  if (ch->pcdata->rank == 1)
  {
    if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_TRUESIGHT))
    {
      send_to_char("You now have the power of truesight.\n\r",ch);
      SET_BIT(ch->pcdata->powers[TANARRI_POWER], TANARRI_TRUESIGHT);
    }
    else if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_CLAWS))
    {
      send_to_char("You now have the ability to grow claws.\n\r",ch);
      SET_BIT(ch->pcdata->powers[TANARRI_POWER], TANARRI_CLAWS);
    }
    else if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_EARTHQUAKE))
    {
      send_to_char("You now have the power to summon an earthquake.\n\r",ch);
      SET_BIT(ch->pcdata->powers[TANARRI_POWER], TANARRI_EARTHQUAKE);
    }
    else
    {
      send_to_char("Your char is bugged, note an immortal.\n\r",ch);
      return;
    }
  }
  else if (ch->pcdata->rank == 2)
  {
    if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_EXOSKELETON))
    {
      send_to_char("Your entire body is made from massive bones.\n\r",ch);
      SET_BIT(ch->pcdata->powers[TANARRI_POWER], TANARRI_EXOSKELETON);
    }
    else if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_FANGS))
    {
      send_to_char("long pointy fangs slides out your gums.\n\r",ch);
      SET_BIT(ch->pcdata->powers[TANARRI_POWER], TANARRI_FANGS);
    }
    else if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_TORNADO))
    {
      send_to_char("You now have the power to summon a tornado storm.\n\r",ch);
      SET_BIT(ch->pcdata->powers[TANARRI_POWER], TANARRI_TORNADO);
    }
    else
    { 
      send_to_char("Your char is bugged, note an immortal.\n\r",ch);
      return;
    }
  }
  if (ch->pcdata->rank == 3)
  {
    if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_SPEED))
    {
      send_to_char("You now move with abyssal speed.\n\r",ch);
      SET_BIT(ch->pcdata->powers[TANARRI_POWER], TANARRI_SPEED);
    }
    else if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_MIGHT))
    {
      send_to_char("You feel your satanic might grow.\n\r",ch);
      SET_BIT(ch->pcdata->powers[TANARRI_POWER], TANARRI_MIGHT);
    }
    else if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_CHAOSGATE))
    {
      send_to_char("You learn how to open a chaosgate.\n\r",ch);
      SET_BIT(ch->pcdata->powers[TANARRI_POWER], TANARRI_CHAOSGATE);
    }
    else
    { 
      send_to_char("Your char is bugged, note an immortal.\n\r",ch);
      return;
    }
  }
  if (ch->pcdata->rank == 4)
  {
    if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_FIERY))
    {
      send_to_char("You skin turns red hot, and your body becomes enveloped in flames.\n\r",ch);
      SET_BIT(ch->pcdata->powers[TANARRI_POWER], TANARRI_FIERY);
    }
    else if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_FURY))
    {
      send_to_char("You now have the power to let your fury take over.\n\r",ch);
      SET_BIT(ch->pcdata->powers[TANARRI_POWER], TANARRI_FURY);
    }
    else if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_HEAD))
    {
      send_to_char("An extra head grows from your neck.\n\r",ch);
      SET_BIT(ch->pcdata->powers[TANARRI_POWER], TANARRI_HEAD);
    }
    else
    { 
      send_to_char("Your char is bugged, note an immortal.\n\r",ch);
      return;
    }
  }
  if (ch->pcdata->rank == 5)
  {
    if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_BOOMING))
    {
      send_to_char("Your voice carries the power of a legion.\n\r",ch);
      SET_BIT(ch->pcdata->powers[TANARRI_POWER], TANARRI_BOOMING);
    }
    else if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_ENRAGE))
    {
      send_to_char("You have learned the power to enrage your opponents.\n\r",ch);
      SET_BIT(ch->pcdata->powers[TANARRI_POWER], TANARRI_ENRAGE);
    }
    else if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_FLAMES))
    {
      send_to_char("You can call upon the flames from the abyss at will.\n\r",ch);
      SET_BIT(ch->pcdata->powers[TANARRI_POWER], TANARRI_FLAMES);
    }
    else
    { 
      send_to_char("Your char is bugged, note an immortal.\n\r",ch);
      return;
    }
  }
  if (ch->pcdata->rank == 6)
  {
    if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_TENDRILS))
    {
      send_to_char("You grow tendrils from every part of your body.\n\r",ch);
      SET_BIT(ch->pcdata->powers[TANARRI_POWER], TANARRI_TENDRILS);
    }
    else if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_LAVA))
    {
      send_to_char("Your blood is now running lava.\n\r",ch);
      SET_BIT(ch->pcdata->powers[TANARRI_POWER], TANARRI_LAVA);
    }
    else if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_EMNITY))
    {
      send_to_char("You now have the power to cause emnity between people.\n\r",ch);
      SET_BIT(ch->pcdata->powers[TANARRI_POWER], TANARRI_EMNITY);
    }
    else
    { 
      send_to_char("Your char is bugged, note an immortal.\n\r",ch);
      return;
    }
  }
  ch->pcdata->powers[TANARRI_POWER_COUNTER]++;
  ch->pcdata->stats[TPOINTS] -= 10000 * cost;
  return;
}

void do_lavablast(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
      
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_TANARRI))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_LAVA))
  {
    send_to_char("Perhaps you should learn that power first.\n\r",ch);
    return;
  }
  if (ch->mana < 1000 || ch->move < 1000)
  {
    send_to_char("Your not up to it, you ain't got the fire in ya.\n\r",ch);
    return;
  }
  if ((victim = ch->fighting) == NULL)
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  ch->mana -= 1000;
  ch->move -= 1000;
  one_hit(ch,victim,gsn_magma,1);
  one_hit(ch,victim,gsn_magma,1);
  one_hit(ch,victim,gsn_magma,1);
  if (!IS_AFFECTED(victim, AFF_FLAMING))
    SET_BIT(victim->affected_by, AFF_FLAMING);
  WAIT_STATE(ch,18);
  return;
}

void do_chaosgate(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  ROOM_INDEX_DATA *location; 
  CHAR_DATA *victim;
  int random_room;

  one_argument (argument, arg);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_TANARRI))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_CHAOSGATE))
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  if ((victim = get_char_world(ch, arg)) == NULL)
  {
    send_to_char("Chaosgate to whom?\n\r", ch );
    return;
  }
  if (IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "You can't find it's room.\n\r",ch);
    return;
  }
  if (IS_IMMUNE(victim, IMM_TRAVEL) && !IS_NPC(victim))
  {
    send_to_char("I don't think they want you to do that.\n\r",ch);
    return;
  }
  if (IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "Your room is not connected to the astral plane.\n\r",ch);
    return;
  }
  if (ch->move < 1000)
  {
    send_to_char("You don't have the move to open a chaosgate.\n\r", ch );
    return;
  }
  if (number_range(1,15)!=1) location = victim->in_room;
  else
  {
    send_to_char("Something went wrong.\n\r",ch);
    for ( ; ; )
    {
       random_room = number_range(1000, 65535);
       location = get_room_index(random_room);  
       if (location != NULL)
         if (!IS_SET(location->room_flags, ROOM_PRIVATE) && !IS_SET(location->room_flags, ROOM_SOLITARY)
         &&  !IS_SET(location->room_flags, ROOM_NO_TELEPORT)) break;
    }
  }
  act("You open a gate made from pure chaos and steps into it.", ch, NULL, NULL, TO_CHAR);
  act("$n opens a gate of pure chaos and steps into it.", ch, NULL, NULL, TO_ROOM);
  ch->move -= 1000;
  char_from_room(ch);
  char_to_room(ch, location);
  do_look(ch, "auto");
  act("You step out of the gate.", ch, NULL, NULL, TO_CHAR);  
  act("A gate appears from out of nowhere and $n steps out of it.", ch, NULL, NULL, TO_ROOM);
  return;
}

void do_booming(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_TANARRI))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_BOOMING))
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  if ((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (IS_NPC(victim)) one_hit(ch,victim, gsn_booming, 1);
  else
  {
    if (victim == ch)
    {
      send_to_char("That doesn't seem like a good idea.\n\r",ch);
      return;
    }
    one_hit(ch,victim, gsn_booming, 1);
    if (victim->position == POS_FIGHTING && number_range(1,4)==2)
    {
      send_to_char("Your booming voice stuns your opponent.\n\r",ch);
      send_to_char("The pain is to much, and you drop to the ground.\n\r",victim);
      stop_fighting(victim,TRUE);
      victim->position = POS_STUNNED;
    }
  }
  WAIT_STATE(ch,12);
  return;
}

void do_fury(CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_TANARRI))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_FURY))
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[TANARRI_FURY_ON] == 1)
  {
    ch->hitroll -= 250;
    ch->damroll -= 250;
    send_to_char("You calm your fury.\n\r",ch);
    ch->pcdata->powers[TANARRI_FURY_ON] = 0;
  }
  else
  {
    ch->damroll += 250;
    ch->hitroll += 250;
    send_to_char("You become enraged.\n\r",ch);
    ch->pcdata->powers[TANARRI_FURY_ON] = 1;
  }
  return;
}

void do_earthquake(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *ich;
  CHAR_DATA *ich_next;
  
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_TANARRI))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_EARTHQUAKE))
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  if (ch->mana < 1000)
  {
    send_to_char("You need more mana to summon an earthquake.\n\r",ch);
    return;
  }
  send_to_char("You summon an earthquake.\n\r",ch);
  ich_next = ch->in_room->people;
  ich = ich_next;
  while (ich_next != NULL)
  {
    ich_next = ich->next_in_room;
    if (ich != ch && !IS_AFFECTED(ich, AFF_FLYING))
    {
      if (is_safe(ch,ich)) break;
      one_hit(ch,ich,32,1);
    }
    ich = ich_next;
  }
  ch->mana -= 1000;
  WAIT_STATE(ch, 12);
  return;
}

void do_tornado(CHAR_DATA *ch, char *argument)
{  
  CHAR_DATA *ich;
  CHAR_DATA *ich_next;

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_TANARRI))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_TORNADO))
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  if (ch->mana < 1500)
  {
    send_to_char("You need more mana to summon a storm.\n\r",ch);
    return;
  }
  send_to_char("You summon a supernatural storm.\n\r",ch);
  ich_next = ch->in_room->people;
  ich = ich_next;
  while (ich_next != NULL)
  {
    ich_next = ich->next_in_room;
    if (ich != ch && IS_AFFECTED(ich, AFF_FLYING))
    {
      if (is_safe(ch,ich)) break;
      one_hit(ch,ich,gsn_lightning,1);
      one_hit(ch,ich,gsn_lightning,1);
    }
    ich = ich_next;
  }
  ch->mana -= 1500;
  WAIT_STATE(ch, 12);
  return;
}

void do_infernal(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *ich;
  CHAR_DATA *ich_next;

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_TANARRI) && !IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_FLAMES) && IS_CLASS(ch, CLASS_TANARRI))
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[CON_LORE] < 2 && IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  if (ch->mana < 2000)
  {
    if (IS_CLASS(ch, CLASS_TANARRI)) send_to_char("You need more mana to summon the infernal flames of the abyss.\n\r",ch);
    else send_to_char("Not enough mana.\n\r",ch);
    return;
  }
  if (IS_CLASS(ch, CLASS_TANARRI)) send_to_char("You summon the infernal flames of the abyss.\n\r",ch);
  else send_to_char("You mutter an incantation and a fireball flies from your hands.\n\r",ch);
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
  ch->mana -= 2000;
  WAIT_STATE(ch, 12);
  return;
}  
