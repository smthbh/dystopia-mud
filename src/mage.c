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
 * New mage class by Jobo * 
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

void do_reveal(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *ich;

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MAGE))
  {
    send_to_char("Huh?!?.\n\r",ch);
    return;
  }
  if (ch->mana < 5000)
  {
    send_to_char("You don't have the mystical energies to do this.\n\r",ch);
    return;
  } 
  act( "$n mumles a few words, and you are suddenly blinded by a flash.", ch, NULL, NULL, TO_ROOM );   
  send_to_char( "You reveal everything hidden in the room.\n\r", ch );
  for ( ich = ch->in_room->people; ich != NULL; ich = ich->next_in_room )
  {
    if (ich==ch || ich->trust > 6) continue;
    affect_strip ( ich, gsn_invis );
    affect_strip ( ich, gsn_mass_invis );
    affect_strip ( ich, gsn_sneak );
    if (IS_SET(ich->affected_by, AFF_HIDE))      REMOVE_BIT(ich->affected_by, AFF_HIDE);
    if (IS_SET(ich->affected_by, AFF_INVISIBLE)) REMOVE_BIT(ich->affected_by, AFF_INVISIBLE);
    if (IS_SET(ich->affected_by, AFF_SNEAK))     REMOVE_BIT(ich->affected_by, AFF_SNEAK);
    if (IS_SET(ich->act, PLR_WIZINVIS))          REMOVE_BIT(ich->act, PLR_WIZINVIS);
    if (IS_SET(ich->act, AFF_HIDE))              REMOVE_BIT(ich->act, AFF_HIDE);
    if (IS_SET(ich->affected_by, AFF_SHIFT))     REMOVE_BIT(ich->affected_by, AFF_SHIFT);
    if (IS_SET(ich->extra, EXTRA_EARTHMELD))     REMOVE_BIT(ich->extra, EXTRA_EARTHMELD);
    if (IS_AFFECTED(ch,AFF_SHADOWPLANE)) REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    if (IS_AFFECTED(ch, AFF_PEACE)) REMOVE_BIT(ch->affected_by, AFF_PEACE);
    if (IS_AFFECTED(ch,AFF_ETHEREAL)) REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);

    if (IS_CLASS(ich, CLASS_DROW) && IS_SET(ich->newbits, NEW_DARKNESS))
    {
      REMOVE_BIT(ich->newbits, NEW_DARKNESS);
      REMOVE_BIT(ich->in_room->room_flags, ROOM_TOTAL_DARKNESS);
    }
    send_to_char("You are suddenly very visible.\n\r",ich);
  }
  ch->mana -= 5000;
  return;
}

void do_magearmor( CHAR_DATA *ch, char *argument )
{
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];
  int vnum = 0;

  argument = one_argument( argument, arg );
  if (IS_NPC(ch)) return;
 	if(!IS_IMMORTAL(ch))
	{
    if( !IS_CLASS(ch, CLASS_MAGE) )
    {
      send_to_char("What?\n\r",ch);
      return;
    }
	}
  if (arg[0] == '\0')
  {
    send_to_char("Please specify which piece of mage armor you wish to make: Dagger Staff Ring Collar Robe Cap Leggings Boots Gloves Sleeves Cape Belt Bracer Mask.\n\r",ch);
    return;
  }
  if ( ch->practice < 60 )
  {
    send_to_char("It costs 60 points of primal to create mage equipment.\n\r",ch);
    return;
  }
  if (!str_cmp(arg,"mask")) vnum = 33013;
  else if (!str_cmp(arg,"belt")) vnum = 33011;
  else if (!str_cmp(arg,"cape")) vnum = 33010;
  else if (!str_cmp(arg,"ring")) vnum = 33002;
  else if (!str_cmp(arg,"collar")) vnum = 33003;
  else if (!str_cmp(arg,"robe")) vnum = 33004;
  else if (!str_cmp(arg,"cap")) vnum = 33005;
  else if (!str_cmp(arg,"leggings")) vnum = 33006;
  else if (!str_cmp(arg,"boots")) vnum = 33007;
  else if (!str_cmp(arg,"gloves")) vnum = 33008;
  else if (!str_cmp(arg,"sleeves")) vnum = 33009;
  else if (!str_cmp(arg,"bracer")) vnum = 33012;
  else if (!str_cmp(arg,"staff")) vnum = 33000;
  else if (!str_cmp(arg,"dagger")) vnum = 33001;
  else
  {
    do_magearmor(ch,"");
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

void do_chaosmagic (CHAR_DATA *ch, char *argument)
{
  char arg[MAX_STRING_LENGTH];
  CHAR_DATA *victim;
  int sn, random, level;

  argument=one_argument(argument,arg);
  level = ch->spl[RED_MAGIC]/4;

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MAGE) && !IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (IS_CLASS(ch, CLASS_LICH) && ch->pcdata->powers[CHAOS_MAGIC] < 1)
  {
    send_to_char("Not yet, not yet.\n\r",ch);
    return;
  }
  if (ch->mana < 1500)
  {
    send_to_char("You need more mana.\n\r",ch);
    return;
  }
  if (ch->in_room != NULL)
  {
    if ( IS_SET(ch->in_room->room_flags,ROOM_SAFE) )
    {
      send_to_char( "You cannot invoke the forces of chaos in a safe room.\n\r", ch );
      return;
    }
  }
  if (arg[0] == '\0' && ch->fighting != NULL) victim = ch->fighting;
  else if ((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch );
    return;
  }
  random = number_range(1,12);
  if (random == 1) sn = skill_lookup("spirit kiss");
  else if (random == 2) sn = skill_lookup("desanct");
  else if (random == 3) sn = skill_lookup("imp heal");
  else if (random == 4) sn = skill_lookup("imp fireball");
  else if (random == 5) sn = skill_lookup("imp faerie fire");
  else if (random == 6) sn = skill_lookup("imp teleport");
  else if (random == 7) sn = skill_lookup("change sex");
  else if (random == 8) sn = skill_lookup("shield");
  else if (random == 9) sn = skill_lookup("readaura");
  else if (random == 10) sn = skill_lookup("earthquake");
  else if (random == 11) sn = skill_lookup("gate");
  else if (random == 12) sn = skill_lookup("dispel magic");
  else sn = 0;
  if (sn  > 0) (*skill_table[sn].spell_fun) (sn,level,ch,victim);
  ch->mana -= 1500;
  WAIT_STATE(ch,6);
  return;
}

/* Syntax : chant <type> <color> <target>
 * ex. chant bless red self 
 */
void do_chant (CHAR_DATA *ch, char *argument)
{
  char arg1[MAX_STRING_LENGTH];
  char arg2[MAX_STRING_LENGTH];
  char buf1[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  char buf3[MAX_STRING_LENGTH];
  CHAR_DATA *victim;
  AFFECT_DATA af;
  int i, dam, count, sn;
  int red_magic = ch->spl[RED_MAGIC];
  int blue_magic = ch->spl[BLUE_MAGIC];
  int green_magic = ch->spl[GREEN_MAGIC];
  int purple_magic = ch->spl[PURPLE_MAGIC];
  int yellow_magic = ch->spl[YELLOW_MAGIC];
  int magic_power = ch->spl[RED_MAGIC] + ch->spl[BLUE_MAGIC] + ch->spl[GREEN_MAGIC] + ch->spl[PURPLE_MAGIC] + ch->spl[YELLOW_MAGIC];
    
  argument=one_argument(argument,arg1);
  strcpy(arg2,argument);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MAGE) && !IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (arg1[0] == '\0')
  {  
    send_to_char("Syntax : chant <bless/curse/damage/heal> <target>.\n\r", ch);
    return;
  }  
  if (IS_ITEMAFF(ch, ITEMA_AFFENTROPY)) 
  {
    magic_power +=100;
    red_magic +=20;
    yellow_magic +=20;
    purple_magic +=20;
    green_magic +=20;
    blue_magic +=20;
  }
  if (!str_cmp(arg1, "heal"))
  {
    if (arg2[0] == '\0') victim = ch;
    else if ((victim = get_char_room(ch, arg2)) == NULL)
    {
      send_to_char("They are not here.\n\r", ch );
      return;
    }
    if (ch->mana < 1500)
    {
      send_to_char("You need 1500 mana.\n\r",ch);
      return;
    }    
    act("$n channels lifeforce from the five elements into $N.",ch,NULL,victim,TO_NOTVICT);
    act("You channel lifeforce from the five elements into $N.",ch,NULL,victim,TO_CHAR);
    act("$n channels lifeforce from the five elements into you.",ch,NULL,victim,TO_VICT);
    WAIT_STATE(ch,8);
    ch->mana -= 1500;
    victim->hit = UMIN(victim->hit + (magic_power * 1.5), victim->max_hit);
    return;
  }
  else if (!str_cmp(arg1, "damage"))
  {
    if (arg2[0] == '\0' && ch->fighting != NULL) victim = ch->fighting;
    else if ((victim = get_char_room(ch, arg2)) == NULL)
    {
      send_to_char("They are not here.\n\r", ch );
      return;
    }
    if (ch->mana <1000)
    {
      send_to_char("You need 1000 mana.\n\r",ch);
      return;
    }
    if (victim == ch)
    {
      send_to_char("You really don't want to hurt yourself.\n\r", ch );
      return;
    }
    if (is_safe(ch, victim)) return;
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room)
      return;
    if (IS_AFFECTED(ch, AFF_PEACE)) REMOVE_BIT(ch->affected_by, AFF_PEACE);
    if (victim->position > POS_STUNNED)
    {
      if (victim->fighting == NULL) set_fighting(victim, ch);
      if (ch->fighting == NULL) set_fighting(ch, victim);
    }
    WAIT_STATE(ch,8);
    ch->mana -= 1000;
    dam = number_range(red_magic*4, red_magic*5);
    dam += char_damroll(ch);
    dam = cap_dam(ch,victim,dam);
    if (!IS_NPC(victim))
    {
//      if (IS_CLASS(ch, CLASS_MAGE) && dam > 600) dam = number_range(580,620);
      if (dam > 1000) dam = number_range(950,1050);
    }
    sprintf(buf1,"$n calls for the forces of #Rhell#n to destroy $N [#C%d#n]" , dam);
    sprintf(buf2,"You call for the forces of #Rhell#n to destroy $N [#C%d#n]" , dam);
    sprintf(buf3,"$n calls for the forces of #Rhell#n to destroy you [#C%d#n]" , dam);
    act(buf1,ch,NULL,victim, TO_NOTVICT);
    act(buf2,ch,NULL,victim, TO_CHAR);
    act(buf3,ch,NULL,victim, TO_VICT);
    hurt_person(ch, victim, dam);
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room) return;
    dam = number_range(blue_magic*4, blue_magic*5);
    dam += char_damroll(ch);
    dam = cap_dam(ch,victim,dam);
    if (!IS_NPC(victim))
    {
//      if (IS_CLASS(ch, CLASS_MAGE) && dam > 600) dam = number_range(580,620);
      if (dam > 1000) dam = number_range(950,1050);
    }
    sprintf(buf1,"$n summons a supernatural #Lstorm#n to strike $N with lightning [#C%d#n]" , dam);
    sprintf(buf2,"You summon a supernatural #Lstorm#n to strike $N with lightning [#C%d#n]" , dam);
    sprintf(buf3,"$n summons a supernatural #Lstorm#n to strike you with lightning [#C%d#n]" , dam);
    act(buf1,ch,NULL,victim, TO_NOTVICT);
    act(buf2,ch,NULL,victim, TO_CHAR);
    act(buf3,ch,NULL,victim, TO_VICT);
    hurt_person(ch, victim, dam);
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room) return;
    dam = number_range(green_magic*4, green_magic*5);
    dam += char_damroll(ch);
    dam = cap_dam(ch,victim,dam);
    if (!IS_NPC(victim))
    {
//      if (IS_CLASS(ch, CLASS_MAGE) && dam > 600) dam = number_range(580,620);
      if (dam > 1000) dam = number_range(950,1050);
    }
    sprintf(buf1,"$n calls for #Gthorns#n to grow from the earth and pierce $N [#C%d#n]" , dam);
    sprintf(buf2,"You call for #Gthorns#n to grow from the earth and pierce $N [#C%d#n]" , dam);
    sprintf(buf3,"$n calls for #Gthorns#n to grow from the earth and pierce You [#C%d#n]" , dam);
    act(buf1,ch,NULL,victim, TO_NOTVICT);
    act(buf2,ch,NULL,victim, TO_CHAR);
    act(buf3,ch,NULL,victim, TO_VICT);
    hurt_person(ch, victim, dam);
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room) return;
    dam = number_range(purple_magic*4, purple_magic*5);
    dam += char_damroll(ch);
    dam = cap_dam(ch,victim,dam);
    if (!IS_NPC(victim))
    {
//      if (IS_CLASS(ch, CLASS_MAGE) && dam > 600) dam = number_range(580,620);
      if (dam > 1000) dam = number_range(950,1050);
    }
    sprintf(buf1,"$n calls on the powers of the #punderdark#n to shatter $N's bones [#C%d#n]" , dam);
    sprintf(buf2,"You call on the powers of the #punderdark#n to shatter $N's bones [#C%d#n]" , dam);
    sprintf(buf3,"$n calls on the powers of the #punderdark#n to shatter your bones [#C%d#n]" , dam);
    act(buf1,ch,NULL,victim, TO_NOTVICT);
    act(buf2,ch,NULL,victim, TO_CHAR);
    act(buf3,ch,NULL,victim, TO_VICT);
    hurt_person(ch, victim, dam);
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room) return;
    dam = number_range(yellow_magic*4, yellow_magic*5);
    dam += char_damroll(ch);
    dam = cap_dam(ch,victim,dam);
    if (!IS_NPC(victim))
    {
//      if (IS_CLASS(ch, CLASS_MAGE) && dam > 600) dam = number_range(580,620);
      if (dam > 1000) dam = number_range(950,1050);
    }
    sprintf(buf1,"$n spews forth a shower of #yacid#n striking $N [#C%d#n]" , dam);
    sprintf(buf2,"you spew forth a shower of #yacid#n striking $N [#C%d#n]" , dam);
    sprintf(buf3,"$n spews forth a shower of #yacid#n striking you [#C%d#n]" , dam);
    act(buf1,ch,NULL,victim, TO_NOTVICT);
    act(buf2,ch,NULL,victim, TO_CHAR);
    act(buf3,ch,NULL,victim, TO_VICT);
    hurt_person(ch, victim, dam);
    return;
  }
  else if (!str_cmp(arg1, "bless"))
  {
    if ((victim = get_char_room(ch, arg2)) == NULL)
    {
      send_to_char("They are not here.\n\r", ch );
      return;
    }
    if (ch->mana <2500)
    {
      send_to_char("You need 2500 mana.\n\r",ch);
      return;
    }
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room)
      return;
    count = 0;
    for (i = 0; i<5 ; i++)
    {
      if (i==0) {sn = skill_lookup("purple sorcery");dam=purple_magic;}
      if (i==1) {sn = skill_lookup("yellow sorcery");dam=yellow_magic;}
      if (i==2) {sn = skill_lookup("green sorcery");dam=green_magic;}
      if (i==3) {sn = skill_lookup("red sorcery");dam=red_magic;}
      if (i==4) {sn = skill_lookup("blue sorcery");dam=blue_magic;}
      if (is_affected(victim, sn))
        continue;
      
      count++;
      
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_HIT;
      af.modifier  = dam*2;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_AC;
      af.modifier  = -dam/2;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_MOVE;
      af.modifier  = dam*2;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_MANA;
      af.modifier  = dam*2;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_HITROLL;
      af.modifier  = dam/3;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_DAMROLL;
      af.modifier  = dam/3;
      af.bitvector = 0;
      affect_to_char(victim, &af);
    }
    ch->mana -= count * 500;
    WAIT_STATE(ch,6);
    sprintf(buf1, "You bless $N with the power of the elements.");
    sprintf(buf2, "The power of the five elements fills your body.");
    act(buf1, ch, NULL, victim, TO_CHAR);
    act(buf2, ch, NULL, victim, TO_VICT);
    return;
  }
  else if (!str_cmp(arg1, "curse"))
  {
    if ((victim = get_char_room(ch, arg2)) == NULL)
    {
      send_to_char("They are not here.\n\r", ch );
      return;
    }
    if (ch->mana <2500)
    {
      send_to_char("You need 2500 mana.\n\r",ch);
      return;
    }
    if (victim->level < 3)
    {
      send_to_char("Not on mortal, thier bodies cant handles the strain....poor little buggers.\n\r",ch);
      return;
    }
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room)
      return;
    count = 0;
    for (i = 0;i<5;i++)
    {
      if (i==0) {sn = skill_lookup("purple sorcery");dam=purple_magic;}
      if (i==1) {sn = skill_lookup("yellow sorcery");dam=yellow_magic;}
      if (i==2) {sn = skill_lookup("green sorcery");dam=green_magic;}
      if (i==3) {sn = skill_lookup("red sorcery");dam=red_magic;}
      if (i==4) {sn = skill_lookup("blue sorcery");dam=blue_magic;}
      if (is_affected(victim, sn))
        continue;
      
      count++;

      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_HIT;
      af.modifier  = -dam*2;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_AC;
      af.modifier  = dam/2;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_MOVE;
      af.modifier  = -dam*2;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_MANA;
      af.modifier  = -dam*2;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_HITROLL;
      af.modifier  = -dam/3;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_DAMROLL;
      af.modifier  = -dam/3;
      af.bitvector = 0;
      affect_to_char(victim, &af);
    }
    ch->mana -= count * 500;
    WAIT_STATE(ch,6);
    sprintf(buf1, "You curse $N with the power of the elements.");
    sprintf(buf2, "The power of the five elements wrecks your body.");
    act(buf1, ch, NULL, victim, TO_CHAR);
    act(buf2, ch, NULL, victim, TO_VICT);
    return;      
  }
  send_to_char("Syntax : chant <bless/curse/damage/heal> <target>.\n\r", ch);
  return;
}

/* Syntax : invoke <list/learn/spell> <none/power/power>
 * ex. invoke learn stoneskin
 * ex. invoke spell stoneskin
 */
void do_invoke (CHAR_DATA *ch, char *argument)
{
  char arg1[MAX_STRING_LENGTH];
  char arg2[MAX_STRING_LENGTH];
  char buf[MAX_STRING_LENGTH];
  int shieldcost=25;
  int steelshieldcost=5;
  int beastcost=10;
  int illusioncost=5;
  int defshieldcost=5;

  argument=one_argument(argument,arg1);
  strcpy(arg2,argument);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MAGE))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (arg1[0]== '\0')
  {
    if(ch->pcdata->powers[PINVOKE] > 0)
      send_to_char("* You have the power to teleport yourself anywhere.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 1)
      send_to_char("* You know how to invoke mageshields.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 2)
      send_to_char("* You have the power to scry on far away places.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 3)
      send_to_char("* You can discharge your shields in a shower of power.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 4)
      send_to_char("* A deflector shield defends you in combat.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 5)
      send_to_char("* You have the power to turn your skin into solid steel.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 7)
      send_to_char("* You have mastered the power of illusions.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 8)
      send_to_char("* You can invoke the power of the beast.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 6 && ch->pcdata->powers[PINVOKE] < 10)
      send_to_char("* You have a deeper understanding of magic.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 9)
      send_to_char("* You have mastered the art of magic.\n\r",ch);
    sprintf(buf,"invokes Learned (%d).\n\r", ch->pcdata->powers[PINVOKE]);
    send_to_char(buf,ch);
    return;  
  }
  if (!str_cmp(arg1,"learn"))
  {
    int cost = (ch->pcdata->powers[PINVOKE]+1) * 20;
    if ( ch->pcdata->powers[PINVOKE] >= 10 )
    {
      send_to_char("You have already gained all the known invokes.\n\r",ch);
      return;
    }
    else if (cost > ch->practice)
    {
      send_to_char("you don't have enough primal.\n\r",ch);
      return;
    }
    else
    {
      ch->pcdata->powers[PINVOKE] +=1;
      ch->practice -= cost;
      send_to_char("You gain a deeper understanding of the mystical powers.\n\r",ch);
    }
    return;
  }
  else if (!str_cmp(arg1,"steelshield"))
  {
    if (ch->pcdata->powers[PINVOKE] < 6)
    {
      send_to_char("You don't have that invoke yet.\n\r",ch);
      return;
    }
    if (!IS_ITEMAFF(ch, ITEMA_STEELSHIELD) && ch->practice >= steelshieldcost)
    {
      SET_BIT(ch->itemaffect, ITEMA_STEELSHIELD);
      ch->practice -= steelshieldcost;
      send_to_char("Your skin turns to solid steel.\n\r",ch);
      return;
    }
    else if (IS_ITEMAFF(ch, ITEMA_STEELSHIELD))
    {
      send_to_char("Your skin cannot become any harder.\n\r",ch);
      return;
    }
    else
    {
      send_to_char("it costs 5 primal to activate steelskin.\n\r",ch);
      return;
    }
  }
  else if (!str_cmp(arg1,"beast"))
  {
    if (ch->pcdata->powers[PINVOKE] < 9)
    {
      send_to_char("You don't have that invoke yet.\n\r",ch);
      return;
    }
    if (!IS_ITEMAFF(ch, ITEMA_BEAST) && ch->practice >= beastcost)
    {
      SET_BIT(ch->itemaffect, ITEMA_BEAST);
      ch->practice -= beastcost;
      send_to_char("Your let your inner beast take control over your actions.\n\r",ch);
      return;
    }
    else if (IS_ITEMAFF(ch, ITEMA_BEAST))
    {
      send_to_char("You feel the beast inside of you roar, but nothing else happens.\n\r",ch);
      return;
    }
    else
    {
      send_to_char("it costs 10 primal to let the beast take over.\n\r",ch);
      return;
    }
  }
  else if (!str_cmp(arg1,"illusions"))
  {
    if (ch->pcdata->powers[PINVOKE] < 8)
    {
      send_to_char("You don't have that invoke yet.\n\r",ch);
      return;
    }
    if (!IS_ITEMAFF(ch, ITEMA_ILLUSIONS) && ch->practice >= illusioncost)
    {
      SET_BIT(ch->itemaffect, ITEMA_ILLUSIONS);
      ch->practice -= illusioncost;
      send_to_char("You conjure several images of yourself, making it impossibly to tell which is really you.\n\r",ch);
      return;
    }
    else if (IS_ITEMAFF(ch, ITEMA_ILLUSIONS))
    {
      send_to_char("You cannot summon any more illusions.\n\r",ch);
      return;
    }
    else
    {
      send_to_char("it costs 5 primal to invoke the illusions of yourself.\n\r",ch);
      return;
    }
    return;
  }
  else if (!str_cmp(arg1,"deflector"))
  {
    if (ch->pcdata->powers[PINVOKE] < 5)
    {
      send_to_char("You don't have that invoke yet.\n\r",ch);
      return;
    }
    if (!IS_ITEMAFF(ch, ITEMA_DEFLECTOR) && ch->practice >= defshieldcost)
    {
      SET_BIT(ch->itemaffect, ITEMA_DEFLECTOR);
      ch->practice -= defshieldcost;
      send_to_char("A shield appears in the air, preventing people from hitting you.\n\r",ch);
      return;
    }
    else if (IS_ITEMAFF(ch, ITEMA_DEFLECTOR))
    {
      send_to_char("You cannot summon any more shields.\n\r",ch);
      return;
    }
    else
    {
      send_to_char("it costs 5 primal to summon a shield.\n\r",ch);
      return;
    }
  }
  else if (!str_cmp(arg1,"mageshield") || !str_cmp(arg1,"shield"))
  {
    if (ch->pcdata->powers[PINVOKE] < 2)
    {
      send_to_char("You don't have that invoke yet.\n\r",ch);
      return;
    }
    if (!IS_SET(ch->newbits, NEW_MULTIARMS))
      SET_BIT(ch->newbits, NEW_MULTIARMS);
    if (!IS_ITEMAFF(ch, ITEMA_MAGESHIELD) && ch->practice>=shieldcost)
    {
      SET_BIT(ch->itemaffect, ITEMA_MAGESHIELD);
      ch->practice -= shieldcost;
      send_to_char("A layer of mystic shields envelopes your body.\n\r",ch);
      return;
    }
    else if (IS_ITEMAFF(ch, ITEMA_MAGESHIELD))
    {
      send_to_char("You are allready shielded.\n\r",ch);
      return;
    }
    else
    {
      send_to_char("The cost is 25 primal.\n\r",ch);
      return;
    }
  }
  else if (!str_cmp(arg1,"all"))
  {
    int totalcost = 0;
    if (ch->pcdata->powers[PINVOKE] < 9)
    {
      send_to_char("You don't have all invokes, so theres no point in invoking them all.\n\r",ch);
      return;
    }
    if (!IS_ITEMAFF(ch, ITEMA_MAGESHIELD)) totalcost += shieldcost;
    if (!IS_ITEMAFF(ch, ITEMA_DEFLECTOR)) totalcost += defshieldcost;
    if (!IS_ITEMAFF(ch, ITEMA_ILLUSIONS)) totalcost += illusioncost;
    if (!IS_ITEMAFF(ch, ITEMA_BEAST)) totalcost += beastcost;
    if (!IS_ITEMAFF(ch, ITEMA_STEELSHIELD)) totalcost += steelshieldcost;
    if (totalcost == 0)
    {
      send_to_char("You already have all invokes on.\n\r",ch);
      return;
    }
    if (ch->practice < totalcost)
    {
      send_to_char("You don't have the primal.\n\r",ch);
      return;
    }
    if (!IS_ITEMAFF(ch, ITEMA_MAGESHIELD)) SET_BIT(ch->itemaffect, ITEMA_MAGESHIELD);
    if (!IS_ITEMAFF(ch, ITEMA_DEFLECTOR)) SET_BIT(ch->itemaffect, ITEMA_DEFLECTOR);
    if (!IS_ITEMAFF(ch, ITEMA_ILLUSIONS)) SET_BIT(ch->itemaffect, ITEMA_ILLUSIONS);
    if (!IS_ITEMAFF(ch, ITEMA_BEAST)) SET_BIT(ch->itemaffect, ITEMA_BEAST);
    if (!IS_ITEMAFF(ch, ITEMA_STEELSHIELD)) SET_BIT(ch->itemaffect, ITEMA_STEELSHIELD);
    send_to_char("You invoke all your powers.\n\r",ch);
    ch->practice -= totalcost;
    return;
  }
  send_to_char("invoke <learn/mageshield/deflector/steelshield/illusions/beast/all>.\n\r",ch);
  return;
}

/* syntax : magics
 * shows all class powers in effect
 */
void do_magics (CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MAGE))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (IS_ITEMAFF(ch, ITEMA_MAGESHIELD))
    send_to_char("* Your skin is covered in a layer of mystical energies.\n\r",ch);
  if (IS_ITEMAFF(ch, ITEMA_BEAST))
    send_to_char("* Your inner beast is in control of your actions.\n\r",ch);
  if (IS_ITEMAFF(ch, ITEMA_STEELSHIELD))
    send_to_char("* Your skin is solid steel.\n\r",ch);
  if (IS_ITEMAFF(ch, ITEMA_DEFLECTOR))
    send_to_char("* A shield hovers in the air, defending you.\n\r",ch);
  if (IS_ITEMAFF(ch, ITEMA_ILLUSIONS))
    send_to_char("* Several images of you stand by your side.\n\r",ch);
  return;
}

/* syntax : discharge
 * (drops all elementalshields in one BIG blast)
 */
void do_discharge (CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  CHAR_DATA *mount;
  char buf1[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  char buf3[MAX_STRING_LENGTH];
  int magic_power = ch->spl[RED_MAGIC] + ch->spl[BLUE_MAGIC] + ch->spl[GREEN_MAGIC] + ch->spl[PURPLE_MAGIC] + ch->spl[YELLOW_MAGIC];
  int dam;

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MAGE))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[PINVOKE] < 4) /* 4 is just a test */
  {
    send_to_char("You don't have that invoke yet.\n\r",ch);
    return;
  }
  if (IS_ITEMAFF(ch, ITEMA_AFFENTROPY)) 
    magic_power +=100;
  if (IS_ITEMAFF(ch, ITEMA_MAGESHIELD))
  {
    REMOVE_BIT(ch->itemaffect, ITEMA_MAGESHIELD);
    for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
      vch_next = vch->next_in_room;
      if (vch != ch)
      {
        if (is_safe(ch,vch)) continue;
        if ((mount = ch->mount) != NULL)
          if (mount == vch) continue;
        dam = number_range(magic_power*7/2, magic_power*9/2) + ch->damroll;
        if (IS_AFFECTED(vch, AFF_SANCTUARY)) dam *= 0.5;
        sprintf(buf1,"$n's mystical shields explodes in a shower of flames striking $N [#C%d#n]",dam);
        sprintf(buf2,"Your mystical shields explodes in a shower of flames striking $N [#C%d#n]",dam);
        sprintf(buf3,"$n's mystical shields explodes in a shower of flames striking you [#C%d#n]",dam);
        act(buf1,ch,NULL,vch, TO_NOTVICT);
        act(buf2,ch,NULL,vch, TO_CHAR);
        act(buf3,ch,NULL,vch, TO_VICT);
        hurt_person(ch,vch,dam);
        if (vch->position > POS_STUNNED)
        {
          if (vch->fighting == NULL) set_fighting(vch, ch);
          if (ch->fighting == NULL) set_fighting(ch, vch);
        }
      }
    }
  }
  return;
}


/* syntax : teleport <target>
 * ex : teleport satan
 */
void do_teleport (CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  ROOM_INDEX_DATA *location;
  CHAR_DATA *victim;

  one_argument (argument, arg);
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MAGE))
  {
    send_to_char("Huh?\n\r", ch );
    return;
	}
  if (ch->pcdata->powers[PINVOKE] < 1) /* 1 is just a test */
  {
    send_to_char("You don't have that invoke yet.\n\r",ch);
    return;
  }
  if ((victim = get_char_world(ch, arg)) == NULL) 
  {
    send_to_char("Teleport to whom?\n\r", ch );
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
	location = victim->in_room;
  if (ch->mana < 250) 
  {
    send_to_char("You don't have the mana to cast teleport.\n\r", ch );
    return;
  }
	act("You utter a single powerword.", ch, NULL, NULL, TO_CHAR);
	act("$n utters a strange sounding word and disappers.", ch, NULL, NULL, TO_ROOM);
	ch->mana -= 250;
	char_from_room(ch);
	char_to_room(ch, location);
	do_look(ch, "auto");
	act("You materialize.", ch, NULL, NULL, TO_CHAR);
	act("$n suddenly appear from out of nowhere.", ch, NULL, NULL, TO_ROOM);
  return;
}

/* syntax : objectgate <object>
 * ex. objectgate 2.bells
 */
void do_objectgate (CHAR_DATA *ch, char *argument)
{
  OBJ_DATA *obj;
  char arg1[MAX_INPUT_LENGTH];

  argument = one_argument(argument, arg1);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MAGE) && !IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r", ch );
    return;
  }
  if (IS_CLASS(ch, CLASS_LICH) && ch->pcdata->powers[CON_LORE] < 1)
  {
    send_to_char("But you don't know how to do that silly.\n\r",ch);
    return;
  }
  if (arg1[0] == '\0')
  {
    send_to_char("What object do you want to attempt to gate?\n\r",ch);
    return;
  }
  if ((obj = get_obj_world(ch, arg1)) == NULL)
  {
    send_to_char("No such object.\n\r",ch);
    return;
  }
  if (IS_SET(obj->quest, QUEST_ARTIFACT) || IS_SET(obj->quest, QUEST_RELIC))
  {
    send_to_char("You cannot grab artifacts or relics.\n\r",ch);
    return;
  }
  if (obj->carried_by != NULL)
  {
    send_to_char("Someone is holding the item.\n\r",ch);
    return;
  }
  if (obj->in_room == NULL)
  {
    send_to_char("It seems to be inside something.\n\r",ch);
    return;
  }
  if (!IS_SET(obj->wear_flags, ITEM_TAKE))
  {
    send_to_char("This item is too big.\n\r",ch);
    return;
  }
  if( IS_SET(obj->in_room->room_flags, ROOM_ASTRAL))
  {
    send_to_char( "You can't find it's room.\n\r",ch);
    return;
  }
  if(IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
  {
    send_to_char( "Your room is not connected to the astral plane.\n\r",ch);
    return;
  }
  obj_from_room( obj );
  obj_to_char( obj, ch );
  send_to_char("The item appears in your hand.\n\r",ch);
  return;
}

/* 
 * for affect spells
 */

void do_bewitch (CHAR_DATA *ch, char *argument)
{
  /*ged*/
  WAIT_STATE(ch, 2);
  return;
}


/*
void do_mgate( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char_to_room( create_mobile( get_mob_index(MOB_VNUM_VAMPIRE) ),	ch->in_room );
    return;
}
*/
