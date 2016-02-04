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


/* POWER LICH BY VLADD & JOBO */

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

void do_planeshift( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }   
  if (ch->pcdata->powers[NECROMANTIC] < 5)
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  if (IS_AFFECTED(ch, AFF_ETHEREAL))
  {
    REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);
    act("You regain your form and become solid.",  ch, NULL, NULL, TO_CHAR);
    act("$n regains $s form and becomes solid again.",  ch, NULL, NULL, TO_ROOM);
    return;
  }
  if (has_timer(ch)) return;
  SET_BIT(ch->affected_by, AFF_ETHEREAL);
  act("You phase into the plane of spirits.", ch, NULL, NULL, TO_CHAR);
  act("$n fades almost of of existance.", ch, NULL, NULL, TO_ROOM);
  return;
}


void do_pentagram(CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  CHAR_DATA *gch;
  int pcounter = 0;
  char arg [MAX_INPUT_LENGTH];
  ROOM_INDEX_DATA *location;

  one_argument( argument, arg );
   
  if (IS_NPC(ch)) return;
  if (ch->in_room == NULL) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[CON_LORE] < 5)
  {
    send_to_char("I think not.\n\r",ch);
    return;
  }
  if (ch->mana < 1500)
  {
    send_to_char("And how would you do that, you got no mana.\n\r",ch);
    return;
  }
  if ( arg[0] == '\0')
  {
    send_to_char("Whom do you wish to summon with your pentagram?\n\r", ch);
    return;
  }
  if ((victim = get_char_world(ch, arg)) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL  
    ||   (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON))
    ||   IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(ch->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(ch->in_room->room_flags, ROOM_ASTRAL)
    ||   IS_SET(victim->in_room->room_flags, ROOM_ASTRAL)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   victim->in_room->vnum == ch->in_room->vnum)
  {
    send_to_char( "You failed.\n\r", ch );
    return;
  }
  if (victim->position == POS_FIGHTING)
  {  
    send_to_char( "You failed, the victim was fighting someone.\n\r", ch );
    return;
  }
  gch = ch->in_room->people;
  while ((gch = gch->next_in_room) != NULL) pcounter++;
  if (pcounter > 30)
  {
    send_to_char("The room is to crowded.\n\r",ch);
    return;
  }
  location = ch->in_room;
  act("You draw a pentagram on the floor and start chanting.", ch, NULL, NULL, TO_CHAR);
  act("$n draws a pentagram on the floor and starts chanting.", ch, NULL, NULL, TO_ROOM);
  act("A pentagram forms under your feet and the world is suddenly a big blurr.", victim, NULL, NULL, TO_CHAR);
  act("A pentagram forms under $n and $e becomes blurry and fades out of existance.", victim, NULL, NULL, TO_ROOM);
  ch->mana -= 1500;
  char_from_room(victim);
  char_to_room(victim, location);
  do_look(victim, "auto");
  act("$N appears in your pentagram.", ch, NULL, victim, TO_CHAR);
  act("$N appears in $n's pentagram.", ch, NULL, victim, TO_ROOM);
  WAIT_STATE(ch, 8);
  return;
}

void do_soulsuck(CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_INPUT_LENGTH];
  int dam;

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH) && !IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (IS_CLASS(ch, CLASS_LICH) && ch->pcdata->powers[NECROMANTIC] < 3)
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT) && ch->pcdata->powers[SPIRIT] < 4)
  {
    send_to_char("You don't have that power yet. (spirit level 4).\n\r",ch);
    return;
  }
  if (ch->alignment > 0)
  {
    send_to_char("You simply can't get yourself to do this, it's way to evil.\n\r",ch);
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
  dam = number_range(250,1000);
  sprintf(buf,"You suck the soul out of $N and use the energy to heal yourself [%d]",dam);
  act(buf,ch,NULL,victim,TO_CHAR);
  sprintf(buf,"$n sucks at your soul, damn it's nasty, ooo, hurts to [%d]",dam);
  act(buf,ch,NULL,victim,TO_VICT);
  sprintf(buf,"$n looks at $N and grins.");
  act(buf,ch,NULL,victim,TO_NOTVICT);
  hurt_person(ch,victim,dam);
  ch->hit += dam;
  if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
  if (victim->position > POS_STUNNED)
  {
    if (victim->fighting == NULL) set_fighting(victim, ch);
    if (ch->fighting == NULL) set_fighting(ch, victim);
  }
  WAIT_STATE(ch,10);
  return;
}

void do_licharmor(CHAR_DATA *ch, char *argument )
{
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];
  int vnum = 0;

  argument = one_argument(argument,arg);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (arg[0] == '\0')
  {
    send_to_char("Please specify which piece of angel armor you wish to make: Scythe Bracer Amulet Ring Plate Helmet Leggings Boots Gauntlets Sleeves Cloak Belt Mask.\n\r",ch);
    return;
  }
  if ( ch->practice < 150 )
  {
    send_to_char("It costs 150 points of primal to create lich equipment.\n\r",ch);
    return;
  }
  if (!str_cmp(arg,"scythe")) vnum = 33220;
  else if (!str_cmp(arg,"bracer")) vnum = 33222;
  else if (!str_cmp(arg,"amulet")) vnum = 33223;
  else if (!str_cmp(arg,"ring")) vnum = 33221;
  else if (!str_cmp(arg,"plate")) vnum = 33224;
  else if (!str_cmp(arg,"helmet")) vnum = 33225;
  else if (!str_cmp(arg,"leggings")) vnum = 33231;
  else if (!str_cmp(arg,"boots")) vnum = 33230;
  else if (!str_cmp(arg,"gauntlets")) vnum = 33228;
  else if (!str_cmp(arg,"sleeves")) vnum = 33229;
  else if (!str_cmp(arg,"cloak")) vnum = 33232;
  else if (!str_cmp(arg,"belt")) vnum = 33226;
  else if (!str_cmp(arg,"mask")) vnum = 33227;
  else
  {
    do_licharmor(ch,"");
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

void do_earthswallow(CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  CHAR_DATA *mount;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[LIFE_LORE] < 3)
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  if (ch->mana < 5000)
  {
    send_to_char("You don't have enough mana.\n\r",ch);
    return;
  }
  if (has_timer(ch)) return;
  if (ch->in_room != NULL)
  {
    if (IS_SET (ch->in_room->room_flags, ROOM_ARENA))
    {
      send_to_char("Your in the arena.\n\r",ch);
      return;
    }
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
  ch->mana -= 5000;
  act( "$n is swallowed by the earth and disappears.", victim, NULL, NULL, TO_ROOM );
  char_from_room( victim );
  char_to_room( victim, get_room_index(ROOM_VNUM_ALTAR)); 
  act( "$n is flung out of a vulcano.", victim, NULL, NULL, TO_ROOM );
  do_look(victim, "auto");
  if ((mount = victim->mount) == NULL) return;
  char_from_room( mount );
  char_to_room(mount, victim->in_room);
  do_look(mount, "auto");
  WAIT_STATE(ch, 8);
  return;
}

void do_painwreck(CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  int dam;

  argument = one_argument( argument, arg );
  dam = number_range(100,200);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[DEATH_LORE] < 4)
  {
    send_to_char("You don't have that power yet.\n\r",ch);
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
  WAIT_STATE(ch, 18);
  hurt_person(ch, victim, dam);
  act("You channel unholy energies into $N and watch $M writhe in pain.",ch,NULL,victim,TO_CHAR);
  act("$n looks at you, and suddenly your body is filled with intense pain.",ch,NULL,victim,TO_VICT);
  if (number_range(1,3) == 1)
  {
    if (victim->position == POS_FIGHTING) stop_fighting(victim,TRUE);
    act("$N screams in pure agony and falls to the ground.",ch,NULL,victim,TO_CHAR);
    act("AARGH!!! The pain is to much, you drop to the ground.",ch,NULL,victim,TO_VICT);
    act("$N screams in agony and falls to the ground.",ch,NULL,victim,TO_NOTVICT);
    victim->position = POS_STUNNED;
  }
  return;
}

void do_creepingdoom(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *ich;
  CHAR_DATA *dummychar=ch;
  int totaldam = ch->hit;
  int dam;
  char buf1[MAX_STRING_LENGTH]; 
  char buf2[MAX_STRING_LENGTH];
  char buf3[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[DEATH_LORE] < 5)
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  if (ch->mana < 5000)
  {
    send_to_char("You don't have the power to summon the insects.\n\r",ch);
    return;
  }
  send_to_char("You send out a call for the insects in the area.\n\r",ch);
  for (ich = ch->in_room->people; dummychar != NULL; ich = dummychar )
  {
    if (totaldam <= 0) break;
    dummychar = ich->next_in_room;
    if (!IS_NPC(ich)) continue;
    if (is_safe(ch,ich)) continue;
    dam = UMIN(totaldam, ich->hit + 10);
    sprintf(buf1,"$N screams in horror as insects from all over swarm over $M and starts eating [#C%d#n]" , dam);
    sprintf(buf2,"Your insects bite $N [#C%d#n]" , dam);
    sprintf(buf3,"$n laughs as insects swarm from nowhere and attack you [#C%d#n]" , dam);
    act(buf1,ch,NULL,ich, TO_NOTVICT);
    act(buf2,ch,NULL,ich, TO_CHAR);
    act(buf3,ch,NULL,ich, TO_VICT);
    hurt_person(ch,ich,dam);
    totaldam -= dam;
    if (ich->position > POS_STUNNED && ich != ch)
    {
      if (ich->fighting == NULL) set_fighting(ich, ch);
      if (ch->fighting == NULL) set_fighting(ch, ich);
    }
  }
  ch->mana -= ch->mana/2;
  WAIT_STATE(ch, 24);
  return;
}

void do_chillhand(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  AFFECT_DATA af;
  char arg[MAX_INPUT_LENGTH];
  int dam = number_range(750,1250);
  int sn;

  argument = one_argument(argument, arg);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[DEATH_LORE] < 3)
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  if (arg[0] == '\0' && ch->fighting != NULL) victim = ch->fighting;
  else if ((victim = get_char_room(ch, arg)) == NULL)
  {
   send_to_char("They are not here.\n\r", ch );
   return;
  }
  if (victim == ch)
  {
    send_to_char("You really don't want to hurt yourself.\n\r", ch );
    return;
  }
  if (is_safe(ch, victim)) return;
  if (!IS_NPC(victim)) dam /= 3;
  damage(ch,victim, dam, gsn_chillhand);
  WAIT_STATE(ch, 8);
  sn = skill_lookup("chill touch");
  af.type      = sn;
  af.duration  = 6;
  af.location  = APPLY_STR;
  af.modifier  = -3;
  af.bitvector = 0;
  affect_to_char( victim, &af );
  return;
}

void do_polarity(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  int dam;

  argument = one_argument(argument, arg);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[LIFE_LORE] < 5)
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  if (arg[0] == '\0' && ch->fighting != NULL) victim = ch->fighting;
  else if ((victim = get_char_room(ch, arg)) == NULL)
  {
   send_to_char("They are not here.\n\r", ch );
   return;
  }
  if (victim == ch)
  {
    send_to_char("You really don't want to hurt yourself.\n\r", ch );
    return;
  }
  if (is_safe(ch, victim)) return;
  dam = number_range(2000,4000);
  dam = UMIN(dam, victim->mana);
  victim->mana -= dam;
  ch->hit += dam/2;
  if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
  send_to_char("You feel your mystical energies being sucked out of your body!!!\n\r",victim);
  send_to_char("You drain their mana and heal yourself with it.\n\r",ch);
  WAIT_STATE(ch,12);
  return;
}

void do_powertransfer(CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->mana < 5000)
  {
    send_to_char("You don't have the mana.\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[LIFE_LORE] < 4)
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  if (ch->hit >= ch->max_hit)
  {
    send_to_char("You don't need to transfer mana to life.\n\r",ch);
    return;
  }
  if (has_timer(ch)) return;
  ch->hit += number_range(7500, 10000);
  if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
  ch->mana -= 5000;
  send_to_char("You transfer the mystical energies in your body into life energy and heal your wounds.\n\r",ch);
  WAIT_STATE(ch,18);
  return;
}

void do_planarstorm(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *ich;
  CHAR_DATA *dummychar=ch;
  int dam;  
  char buf1[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  char buf3[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->mana < 5000)
  {
    send_to_char("You can't control the planes.\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[NECROMANTIC] < 4)
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  send_to_char("You merge the plane of chaos and the plane of death with the prime material plane.\n\r",ch);
  for (ich = ch->in_room->people; dummychar != NULL; ich = dummychar )
  {
    dummychar = ich->next_in_room;
    if (is_safe(ch,ich)) continue;
    dam = number_range(7500,12500);
    dam = cap_dam(ch,ich,dam);
    if (ich == ch) dam = dam/2;
    sprintf(buf1,"$N screams in pain as $S body is spread over the planes [#C%d#n]" , dam);
    sprintf(buf2,"Your cackle with glee as the planeshift fills $N with agonising pain [#C%d#n]" , dam);
    sprintf(buf3,"$n cackles with glee as the planshift fills you with extreme pain [#C%d#n]" , dam);
    act(buf1,ch,NULL,ich, TO_NOTVICT);
    act(buf2,ch,NULL,ich, TO_CHAR);
    act(buf3,ch,NULL,ich, TO_VICT);
    hurt_person(ch,ich,dam);
    if (ich->position > POS_STUNNED && ich != ch)
    {
      if (ich->fighting == NULL) set_fighting(ich, ch);
      if (ch->fighting == NULL) set_fighting(ch, ich);
    }
  }
  ch->mana -= ch->mana/2;
  WAIT_STATE(ch, 24);
  return;
}


/* They need a portal, so they can take their golems with them*/
void do_planartravel(CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  OBJ_DATA  *obj;
  char arg [MAX_INPUT_LENGTH];   
  int duration;

  one_argument( argument, arg );

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[CON_LORE] < 3)
  {
    send_to_char("I think not.\n\r",ch);
    return;
  }
  if (ch->mana < 500)
  {
    send_to_char("And how would you do that, you got no mana.\n\r",ch);
    return;
  }
  if ( arg[0] == '\0')
  {
    send_to_char("Whom do you wish to planartravel to?\n\r", ch);
    return;
  }
  if ((victim = get_char_world(ch, arg)) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||   (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON))
    ||   IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(ch->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(ch->in_room->room_flags, ROOM_ASTRAL)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(victim->in_room->room_flags, ROOM_ASTRAL)
    ||   victim->in_room->vnum == ch->in_room->vnum)
    {
        send_to_char( "You failed.\n\r", ch );
        return;
    }
  /* A portal to the victim */
  duration = number_range(2,3);
  obj = create_object( get_obj_index( OBJ_VNUM_PORTAL ), 0 );
  obj->value[0] = victim->in_room->vnum;
  obj->value[3] = ch->in_room->vnum;
  obj->timer = duration;
  if (IS_AFFECTED(ch,AFF_SHADOWPLANE)) obj->extra_flags = ITEM_SHADOWPLANE;
  obj_to_room( obj, ch->in_room );
  /* and a portal that leads the other way */
  obj = create_object( get_obj_index( OBJ_VNUM_PORTAL ), 0 );
  obj->value[0] = ch->in_room->vnum;
  obj->value[3] = victim->in_room->vnum;
  obj->timer = duration;
  if (IS_AFFECTED(victim,AFF_SHADOWPLANE)) obj->extra_flags = ITEM_SHADOWPLANE;
  obj_to_room( obj, victim->in_room );
  ch->mana -= 500;
  act( "$p appears in front of $n.", ch, obj, NULL, TO_ROOM );
  act( "$p appears in front of you.", ch, obj, NULL, TO_CHAR );
  act( "$p appears in front of $n.", victim, obj, NULL, TO_ROOM );
  act( "$p appears in front of you.", ch, obj, victim, TO_VICT );
  return;
}

void do_chaosshield(CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[CHAOS_MAGIC] < 5)
  {
    send_to_char("No, I don't think so.\n\r",ch);
    return;
  }
  if (!IS_IMMUNE(ch,IMM_SHIELD2))
  {
    send_to_char("You shield your aura from those around you.\n\r",ch);
    SET_BIT(ch->immune, IMM_SHIELD2);
    return;
  }
  send_to_char("You stop shielding your aura.\n\r",ch);
  REMOVE_BIT(ch->immune, IMM_SHIELD2);
  return;
}

void do_lore( CHAR_DATA *ch, char *argument )
{
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  char buf [MAX_STRING_LENGTH];
  char lin [MAX_STRING_LENGTH];

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (arg1[0] == '\0' && arg2[0] == '\0')
  {
    sprintf(lin,"#R============================================================================#n\n\r");
    stc(lin, ch);
    stc("                                #7Undead Lore#n\n\r",ch);
    stc(lin, ch);
    sprintf(buf,"#0        Conjuring     [ %d ]       Death    [ %d ]         Life       [ %d ]\n\r#n",
                ch->pcdata->powers[CON_LORE], ch->pcdata->powers[DEATH_LORE], ch->pcdata->powers[LIFE_LORE] );
    stc(buf, ch);
    stc(lin, ch);
    stc("                                #7Necromancy#n\n\r",ch);
    stc(lin, ch);
    sprintf(buf,"#0                   Necromantic     [ %d ]       Chaos Magic    [ %d ]       \n\r#n",
                ch->pcdata->powers[NECROMANTIC], ch->pcdata->powers[CHAOS_MAGIC] );
    stc(buf, ch);
    stc(lin, ch);
  }
  return;
}

void do_studylore( CHAR_DATA *ch, char *argument )
{
  char arg [MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (arg[0] == '\0')
  {
    send_to_char("Study what dear?\n\r",ch);
    return;
  }
  if (!str_cmp(arg,"Conjuring"))
  {
    if (ch->pcdata->powers[CON_LORE] > 4)
    {
      send_to_char("You cannot learn any more.\n\r",ch);
      return;
    }
    if (ch->exp < 10000000 * (ch->pcdata->powers[CON_LORE] + 1))
    {
      send_to_char("Sorry, but it's 10 million exp pr. level, and you ain't got it.\n\r",ch);
      return;
    }
    ch->pcdata->powers[CON_LORE] += 1;
    ch->exp -= 10000000 * ch->pcdata->powers[CON_LORE];
    send_to_char("Ok.\n\r",ch);
  }
  else if (!str_cmp(arg,"Death"))
  {
    if (ch->pcdata->powers[DEATH_LORE] > 4)
    {
      send_to_char("You cannot learn any more.\n\r",ch);
      return;
    }
    if (ch->exp < 10000000 * (ch->pcdata->powers[DEATH_LORE] + 1))
    {
      send_to_char("Sorry, but it's 10 million exp pr. level, and you ain't got it.\n\r",ch);
      return;
    }
    ch->pcdata->powers[DEATH_LORE] += 1;
    ch->exp -= 10000000 * ch->pcdata->powers[DEATH_LORE];
    send_to_char("Ok.\n\r",ch);
  }
  else if (!str_cmp(arg,"Life"))
  {
    if (ch->pcdata->powers[LIFE_LORE] > 4)
    {
      send_to_char("You cannot learn any more.\n\r",ch);
      return;
    }
    if (ch->exp < 10000000 * (ch->pcdata->powers[LIFE_LORE] + 1))
    {
      send_to_char("Sorry, but it's 10 million exp pr. level, and you ain't got it.\n\r",ch);
      return;
    }
    ch->pcdata->powers[LIFE_LORE] += 1;
    ch->exp -= 10000000 * ch->pcdata->powers[LIFE_LORE];
    send_to_char("Ok.\n\r",ch);
  }
  else if (!str_cmp(arg,"Necromantic"))
  {
    if (ch->pcdata->powers[NECROMANTIC] > 4)
    {
      send_to_char("You cannot learn any more.\n\r",ch);
      return;
    }
    if (ch->exp < 10000000 * (ch->pcdata->powers[NECROMANTIC] + 1))
    {
      send_to_char("Sorry, but it's 10 million exp pr. level, and you ain't got it.\n\r",ch);
      return;
    }
    ch->pcdata->powers[NECROMANTIC] += 1;
    ch->exp -= 10000000 * ch->pcdata->powers[NECROMANTIC];
    send_to_char("Ok.\n\r",ch);
  }
  else if (!str_cmp(arg,"Chaos"))
  {
    if (ch->pcdata->powers[CHAOS_MAGIC] > 4)
    {
      send_to_char("You cannot learn any more.\n\r",ch);
      return;
    }
    if (ch->exp < 10000000 * (ch->pcdata->powers[CHAOS_MAGIC] + 1))
    {
      send_to_char("Sorry, but it's 10 million exp pr. level, and you ain't got it.\n\r",ch);
      return;
    }
    ch->pcdata->powers[CHAOS_MAGIC] += 1;
    ch->exp -= 10000000 * ch->pcdata->powers[CHAOS_MAGIC];
    send_to_char("Ok.\n\r",ch);
  }
  else send_to_char("That's not party of your study.\n\r",ch);
  return;
}

void do_summongolem( CHAR_DATA *ch, char *argument )
{
  char arg1[MAX_STRING_LENGTH];
  CHAR_DATA *victim;
  AFFECT_DATA af;

  argument=one_argument(argument,arg1);

  if (IS_NPC(ch)) return;

  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[CON_LORE] < 4)
  {
    send_to_char("No sir!\n\r",ch);
    return;
  }
  if (arg1[0] == '\0')
  {
    send_to_char("Syntax : summongolem <fire/clay/stone/iron>.\n\r", ch);
    return;
  }
  if (!str_cmp(arg1, "fire"))
  {
    if (IS_SET(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_FIRE))
    {
      send_to_char("You cannot summon any more fire golems atm.\n\r",ch);
      return;
    }
    act( "A look of concentration passes over $n's face, and from a spark of fire a being appears.", ch, NULL,NULL, TO_ROOM );
    stc( "You create a fire golem.\n\r",ch);
    victim=create_mobile( get_mob_index( MOB_VNUM_FIRE ));
    free_string(victim->short_descr);
    free_string(victim->name);
    free_string(victim->long_descr);
    victim->name=str_dup("fire golem");
    victim->short_descr=str_dup("#Ra deadly fire golem#n");
    victim->long_descr=str_dup("#Ra Huge fire golem stands here.#n\n\r");
    victim->level = ch->spl[RED_MAGIC] / 2;
    victim->hit = 100 * ch->spl[RED_MAGIC]; 
    victim->max_hit = 100 * ch->spl[RED_MAGIC];
    victim->hitroll =  ch->spl[RED_MAGIC]; 
    victim->damroll =  ch->spl[RED_MAGIC];  
    char_to_room( victim, ch->in_room );
    add_follower( victim, ch );
    af.type      = skill_lookup ("charm person");
    af.duration  = 666;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    SET_BIT(victim->act,ACT_NOEXP);
    SET_BIT(victim->act,PLR_HOLYLIGHT);
    do_group(ch,victim->name);
    SET_BIT(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_FIRE);
  }
  else if (!str_cmp(arg1, "clay"))
  { 
    if (IS_SET(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_CLAY))
    {
      send_to_char("You cannot summon any more clay golems atm.\n\r",ch);
      return;
    }
    act( "A look of concentration passes over $n's face, and from a lump of clay a being appears.", ch, NULL,NULL, TO_ROOM );
    stc( "You create a clay golem.\n\r",ch);
    victim=create_mobile( get_mob_index( MOB_VNUM_CLAY ));
    free_string(victim->short_descr);
    free_string(victim->name);
    free_string(victim->long_descr);   
    victim->name=str_dup("clay golem");
    victim->short_descr=str_dup("#oa huge clay golem#n");
    victim->long_descr=str_dup("#oA huge clay golem stands here.#n\n\r");
    victim->level = ch->spl[YELLOW_MAGIC] / 2;
    victim->hit = 100 * ch->spl[YELLOW_MAGIC];
    victim->max_hit = 100 * ch->spl[YELLOW_MAGIC];
    victim->hitroll = ch->spl[YELLOW_MAGIC];
    victim->damroll = ch->spl[YELLOW_MAGIC];
    char_to_room( victim, ch->in_room );
    add_follower( victim, ch );
    af.type      = skill_lookup ("charm person");                                                                                    
    af.duration  = 666;                                                                        
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    SET_BIT(victim->act,ACT_NOEXP);
    SET_BIT(victim->act,PLR_HOLYLIGHT);
    do_group(ch,victim->name);
    SET_BIT(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_CLAY);
  }
  else if (!str_cmp(arg1, "stone"))
  {
    if (IS_SET(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_STONE))
    {
      send_to_char("You cannot summon any more stone golems atm.\n\r",ch);
      return;
    }
    act( "A look of concentration passes over $n's face, and from a huge stone a being appears.", ch, NULL,NULL, TO_ROOM );
    stc( "You create a stone golem.\n\r",ch);
    victim=create_mobile( get_mob_index( MOB_VNUM_STONE ));
    free_string(victim->short_descr);
    free_string(victim->name);
    free_string(victim->long_descr);
    victim->name=str_dup("stone golem");
    victim->short_descr=str_dup("#La huge stone golem#n");
    victim->long_descr=str_dup("#LA huge stone golem stands here.#n\n\r");
    victim->level = ch->spl[GREEN_MAGIC] / 2;
    victim->hit = 100 * ch->spl[GREEN_MAGIC];
    victim->max_hit = 100 * ch->spl[GREEN_MAGIC];
    victim->hitroll = ch->spl[GREEN_MAGIC]; 
    victim->damroll = ch->spl[GREEN_MAGIC];
    char_to_room( victim, ch->in_room );
    add_follower( victim, ch );
    af.type      = skill_lookup ("charm person");
    af.duration  = 666;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    SET_BIT(victim->act,ACT_NOEXP);
    SET_BIT(victim->act,PLR_HOLYLIGHT);
    do_group(ch,victim->name);
    SET_BIT(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_STONE);
  }
  else if (!str_cmp(arg1, "iron"))
  {
    if (IS_SET(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_IRON))
    {
      send_to_char("You cannot summon any more iron golems atm.\n\r",ch);
      return;
    }
    act( "A look of concentration passes over $n's face, and from a blokc of iron a being appears.", ch, NULL,NULL, TO_ROOM ); 
    stc( "You create an iron golem.\n\r",ch);
    victim=create_mobile( get_mob_index( MOB_VNUM_IRON ));
    free_string(victim->short_descr);
    free_string(victim->name);
    free_string(victim->long_descr);
    victim->name=str_dup("iron golem");
    victim->short_descr=str_dup("#Ca huge iron golem#n");
    victim->long_descr=str_dup("#CA huge iron golem stands here.#n\n\r");
    victim->level = ch->spl[BLUE_MAGIC] / 2;
    victim->hit = 100 * ch->spl[BLUE_MAGIC];
    victim->max_hit = 100 * ch->spl[BLUE_MAGIC];
    victim->hitroll = ch->spl[BLUE_MAGIC];  
    victim->damroll = ch->spl[BLUE_MAGIC]; 
    char_to_room( victim, ch->in_room );
    add_follower( victim, ch );
    af.type      = skill_lookup ("charm person");
    af.duration  = 666;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    SET_BIT(victim->act,ACT_NOEXP);
    SET_BIT(victim->act,PLR_HOLYLIGHT);
    do_group(ch,victim->name);
    SET_BIT(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_IRON);
  }
  else send_to_char("No such golem.\n\r",ch);
  return;
}
