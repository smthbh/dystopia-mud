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

#define MAX_SLAY_TYPES 3
#define MONK_AUTODROP 12


/*
 * Local functions.
 */
void  autodrop        args((CHAR_DATA *ch));
void  dropinvis       args((CHAR_DATA *ch));
bool  check_dodge     args((CHAR_DATA *ch, CHAR_DATA *victim, int dt));
bool  check_parry     args((CHAR_DATA *ch, CHAR_DATA *victim, int dt));
void  dam_message     args((CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt));
void  death_cry       args((CHAR_DATA *ch));
void  group_gain      args((CHAR_DATA *ch, CHAR_DATA *victim));
int   xp_compute      args((CHAR_DATA *gch, CHAR_DATA *victim));
void  set_fighting    args((CHAR_DATA *ch, CHAR_DATA *victim));
bool  can_counter     args((CHAR_DATA *ch));
bool  can_bypass      args((CHAR_DATA *ch, CHAR_DATA *victim));
int   number_attacks  args((CHAR_DATA *ch, CHAR_DATA *victim));
int   dambonus        args((CHAR_DATA *ch, CHAR_DATA *victim, int dam, int stance));
void  update_damcap   args((CHAR_DATA *ch, CHAR_DATA *victim));
void  decap_message   args((CHAR_DATA *ch, CHAR_DATA *victim));
void  angel_eye       args((CHAR_DATA *ch, CHAR_DATA *victim, int dam));
void  ragnarokdecap   args((CHAR_DATA *ch, CHAR_DATA *victim));

/*
 * Control the fights going on.
 * Called periodically by update_handler.
 */
void violence_update( void )
{
  CHAR_DATA *ch;
  CHAR_DATA *ch_next;
  CHAR_DATA *emb;
  CHAR_DATA *victim;
  CHAR_DATA *rch;
  CHAR_DATA *rch_next;
  CHAR_DATA *mount;
  int chance;

  chance = number_percent();

  for ( ch = char_list; ch != NULL; ch = ch->next )
  {
    victim = ch->blinkykill;

    if (victim != NULL) {

    if (IS_SET(ch->flag3, AFF3_BLINK_1ST_RD))
    {
      REMOVE_BIT(ch->flag3, AFF3_BLINK_1ST_RD);
      SET_BIT(ch->flag3, AFF3_BLINK_2ND_RD);
    }
    else if (IS_SET(ch->flag3, AFF3_BLINK_2ND_RD))
    {
      REMOVE_BIT(ch->flag3, AFF3_BLINK_2ND_RD);
      REMOVE_BIT(ch->flag3, AFF3_BLINK_1ST_RD);
      /* Has the victim left? */
      if (victim->in_room != ch->in_room)
      {
        SET_BIT(ch->affected_by2,EXTRA_BLINKY);
        victim = NULL;
        act("$n pops back into existence.", ch, NULL, NULL, TO_ROOM);
        stc("You pop back into existence.\n\r", ch);
        stc("Your victim is no longer in the room.\n\r", ch);
        break;
      }
      SET_BIT(ch->affected_by2,EXTRA_BLINKY);
      act("$n pops back into existence next to $N.", ch,NULL,victim, TO_NOTVICT);
      act("You pop back into existence next to $N.", ch, NULL,victim, TO_CHAR);
      act("$n pops back into existence next to you.", ch, NULL,victim, TO_VICT);
      if (!is_safe(ch,victim)) {
        if (ch->hit > 0) {
        set_fighting(ch, victim);
        do_say(ch, "Muhahahahahaha");
        multi_hit(ch, victim, gsn_blinky);
        multi_hit(ch, victim, gsn_blinky);
        if (IS_SET(ch->newbits, THIRD_HAND) && get_eq_char(ch, WEAR_THIRD) != NULL)
          multi_hit(ch, victim, gsn_blinky);
        if (IS_SET(ch->newbits, FOURTH_HAND) && get_eq_char(ch, WEAR_FOURTH) != NULL)
          multi_hit(ch, victim, gsn_blinky);
        update_pos(victim);
        }
      }
    }
    } /*end of blink*/
    ch_next = ch->next;
    if (ch->fight_timer > 0) ch->fight_timer--;
    if (ch->rage > 0 && IS_CLASS(ch, CLASS_DEMON)) ch->rage--;
    if ( ch->embracing != NULL )
    {
      emb = ch->embracing;
      if ( emb == NULL ) ch->embracing = NULL;
      if ( ch->in_room != emb->in_room )
      {
        if ( emb->embraced != NULL )
        {
          emb->embraced = NULL;
          ch->embracing = NULL;
        }
        else ch->embracing = NULL;
      }
    }
    if ( ch->embraced != NULL )
    {
      emb = ch->embraced;
      if ( emb == NULL ) ch->embraced = NULL;  
      if ( ch->in_room != emb->in_room )
      {
        if ( emb->embracing != NULL )
        {
          emb->embracing = NULL;
          ch->embraced   = NULL;
        }
        else ch->embraced = NULL;
      }
    }
   if ( IS_SET(ch->monkstuff, MONK_DEATH) )
    {  
      if ( ch->hit > (ch->max_hit /2 ) )
      {
        ch->hit -=  number_range( 50,200 );
        stc("Your writhe in agony as magical energies tear you asunder.\n\r",ch);
        act("$n writhes in agony as magical forces tear apart $s body.",ch,NULL,NULL,TO_ROOM);
      }
      else
      {
        if ( number_range(1,2) == 1 )
        {
          stc("You feel the magical forces leave your body.\n\r",ch);
          act("The magical forces leave $n's body.",ch,NULL,NULL,TO_ROOM);
          REMOVE_BIT(ch->monkstuff, MONK_DEATH);
        }
        else
        {
          ch->hit -=  number_range( 50,200 );
          stc("Your writhe in agony as magical energies tear you asunder.\n\r",ch);
          act("$n writhes in agony as magical forces tear apart $s body.",ch,NULL,NULL,TO_ROOM);
        }
      }
    }
    if (IS_SET(ch->monkstuff, MONK_HEAL))
    {
      if (ch->hit < (ch->max_hit /2 ) && ch->hit > 0)
      {
        if (ch->hit < ch->max_hit) ch->hit +=  number_range( 200,400 );
        if ( ch->move < ch->max_move ) ch->move += number_range( 175,400 );
        stc("Your body emits glowing sparks.\n\r",ch);
        act("$n's body emits glowing sparks and fizzes.",ch,NULL,NULL,TO_ROOM);
      }
      else
      {  
        if ( number_range( 1,2 ) == 1 )
        {
           stc("The sparks fizzle and die.\n\r",ch);
           act("The sparks around $n's body fizzle and die.",ch,NULL,NULL,TO_ROOM);
           REMOVE_BIT(ch->monkstuff, MONK_HEAL);
        }
        else
        {
          if ( ch->hit < ch->max_hit )
          ch->hit +=  number_range( 200,400 );
          if ( ch->move < ch->max_move )
          ch->move += number_range( 175,400 );
          stc("Your body emits glowing sparks.\n\r",ch);
          act("$n's body emits glowing sparks and fizzes.",ch,NULL,NULL,TO_ROOM);
        }
      }
    }
    if ((victim = ch->fighting ) == NULL || ch->in_room == NULL) continue;
    if (!IS_NPC(ch) && !IS_NPC(victim) && !is_safe(ch, victim) &&!is_safe(victim,ch))
    {
      if (ch->fight_timer < 10) ch->fight_timer = 10;
      else if (ch->fight_timer < 25) ch->fight_timer += 3;
    }
    if (IS_AWAKE(ch) && IS_AWAKE(victim) && ch->in_room == victim->in_room) multi_hit( ch, victim, TYPE_UNDEFINED );
    else stop_fighting(ch, FALSE);
    if ((victim = ch->fighting ) == NULL) continue;
    /*   
     * Fun for the whole family!
     */
    for ( rch = ch->in_room->people; rch != NULL; rch = rch_next )
    {
      rch_next = rch->next_in_room;
      if ( IS_AWAKE(rch) && rch->fighting == NULL )
      {
      /*
       * Mount's auto-assist their riders and vice versa.
       */
        if ( (mount = rch->mount) != NULL )
        {
          if (mount == ch) multi_hit( rch, victim, TYPE_UNDEFINED );
          continue;
        }
        /*
         * PC's auto-assist others in their group.
         */
        if ( !IS_NPC(ch) || IS_AFFECTED(ch, AFF_CHARM) )
        {
          if ((!IS_NPC(rch) || IS_AFFECTED(rch, AFF_CHARM) ) && is_same_group(ch, rch))
            multi_hit( rch, victim, TYPE_UNDEFINED );
          continue;
        }
        /*
         * NPC's assist NPC's of same type or 12.5% chance regardless.
         */
        if ( IS_NPC(rch) && !IS_AFFECTED(rch, AFF_CHARM) )
        {   
          if (rch->pIndexData == ch->pIndexData || number_bits(3) == 0)
          {
            CHAR_DATA *vch;
            CHAR_DATA *target;
            int number;
          
            target = NULL;
            number = 0;
            for ( vch = ch->in_room->people; vch; vch = vch->next )
            {
              if (can_see(rch,vch) && is_same_group(vch, victim) && number_range(0,number)==0)
              {
                target = vch;  
                number++;
              }
            }
            if ( target != NULL )
            multi_hit( rch, target, TYPE_UNDEFINED );
          }
        }
      }
    }
  }
  return;
}

/*
 * Do one group of attacks.
 */
void multi_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
  OBJ_DATA *wield;
  OBJ_DATA *wield1;
  OBJ_DATA *wield2;
  OBJ_DATA *wield3;
  OBJ_DATA *wield4;
  char buf[MAX_STRING_LENGTH];
  int sn, level, chance, unarmed, maxcount,countup,x,wieldie,i;
  AFFECT_DATA af;
  int wieldorig = 0;
  int wieldtype = 0;

  if ( ch->position < POS_SLEEPING ) return;
  if ( IS_CREATOR(ch) || IS_CLASS(ch, CLASS_MONK) )
  {
    if (!IS_NPC(ch) && wieldorig == 0 )
    {
      countup = number_range(0,2);
      if (countup == 0) unarmed = number_range(0,3);
      else if (countup == 1) unarmed = number_range(4,7);
      else unarmed = number_range(0,7);
      if (!IS_NPC(ch) && ch->cmbt[unarmed] != 0 && wieldorig == 0)
        fightaction(ch,victim,ch->cmbt[unarmed], dt, wieldtype);
    } 
  }
  if ( dt == gsn_headbutt )
  {
    one_hit( ch, victim, dt, 1 );
    return;
  }
  if ( dt == gsn_hooves )
  {
    one_hit( ch, victim, dt, 1 );
    one_hit( ch, victim, dt, 1 );
    if (number_range(1,3) != 2)
    {
      one_hit( ch, victim, dt, 1 );
      one_hit( ch, victim, dt, 1 );
    }
    return;
  }
  if (dt == gsn_shiroken)
  {
    for(i = number_range(3,5);i>0;i--)
      one_hit(ch, victim, dt, 1); 
    if(ch->pcdata->powers[NPOWER_NINGENNO] >=4)
      spell_poison(gsn_poison,(ch->level*number_range(50,60)),ch,victim); 
    return;
  }
  if ( dt == gsn_laser )
  {
    one_hit(ch,victim,dt,1);
    one_hit(ch,victim,dt,1);
    return;
  }
  if ( dt == gsn_stinger )
  {
    one_hit(ch,victim,dt,1);
    one_hit(ch,victim,dt,1);
    return;
  }
  if (dt == gsn_claws)
  {
    one_hit( ch, victim, dt, 1 );
    one_hit( ch, victim, dt, 1 );
    return;
  }
  if (dt == gsn_tentacle)
  {
    int x;

    act("You shoot forth a shower of tentacles to destroy $N.", ch, NULL, victim, TO_CHAR);
    act("$n shoots forth a shower of tentacles aiming straight at you.", ch, NULL, victim, TO_VICT);
    act("$n shoots forth a shower of tentacles striking $N.", ch, NULL, victim, TO_NOTVICT);

    x = number_range(2, 5);
    while (--x >= 0) one_hit(ch, victim, dt, 1);
    return;
  }
  if ( dt == gsn_fangs )
  {
    one_hit( ch, victim, dt, 1 );
    return;
  } 
  if ( dt== gsn_quills)
  {
    if ( (IS_CLASS(ch,CLASS_WEREWOLF)) && (ch->power[DISC_WERE_PAIN] > 9))
    {
      one_hit(ch,victim,dt,1);
      one_hit(ch,victim,dt,1);
      one_hit(ch,victim,dt,1);
    }
    one_hit(ch,victim,dt,1);
    return;
  }
  if (dt == gsn_darktendrils && IS_CLASS(ch, CLASS_DROW))
  {
    one_hit(ch,victim,dt,1);
    one_hit(ch,victim,dt,1);
    one_hit(ch,victim,dt,1);
    one_hit(ch,victim,dt,1);
    return;
  }
  if ( dt==gsn_venomtong)
  {
    one_hit(ch,victim,dt,1);
    return;
  }
  if ( dt==gsn_spiketail)
  {
    one_hit(ch,victim,dt,1);
    return;
  }  
  if ( dt==gsn_badbreath)
  {
    one_hit(ch,victim,dt,1);
    return;
  }  
  if ( dt==gsn_magma)
  {
    one_hit(ch,victim,dt,1);
    return;
  }  
  if ( dt==gsn_shards)
  {
    one_hit(ch,victim,dt,1);
    return;
  }  
  if ( dt==gsn_cheapshot)
  {
    send_to_char("You stun them with a shoulder charge!\n\r",ch);
    send_to_char("You are stunned by a shoulder charge!\n\r",victim);
    one_hit(ch,victim,dt,1);    
    one_hit(ch,victim,dt,1);
    one_hit(ch,victim,dt,1);
    return;
  } 
  if ( dt == gsn_buffet )
  {
    one_hit( ch, victim, dt, 1 );
    return;
  } 
  if ( dt == gsn_sweep )
  {
    one_hit( ch, victim, dt, 1 );
    return;
  }
  if ( dt == gsn_rfangs )
  {
    one_hit( ch, victim, dt, 1);
    return;
  }
  if (dt==gsn_heavenlyaura)
  {
    if (IS_CLASS(ch, CLASS_MONK)) x = number_range(1,4);
    else
    {
      if (IS_CLASS(victim, CLASS_SAMURAI)) x = number_range(3,5); // angels don't like em samurais.
      else x = number_range(2,5);
    }
    for (i=0; i < x; i++)
      one_hit(ch,victim,dt,1);
    return;
  }
  if (dt==gsn_mageshield)
  {
    if (IS_ITEMAFF(ch, ITEMA_MAGESHIELD))
    {
      one_hit(ch,victim,dt,1);
      one_hit(ch,victim,dt,1);
      one_hit(ch,victim,dt,1);
      one_hit(ch,victim,dt,1);
      one_hit(ch,victim,dt,1);
    }
    return;
  }
  if ( dt == gsn_bladespin)
  {
    if (number_range(1,2)==1) one_hit(ch,victim,dt,1);
    if (number_range(1,2)==1) one_hit(ch,victim,gsn_lightningkick,1);
    if (number_range(1,2)==1) one_hit(ch,victim,gsn_knee,1);
    if (number_range(1,2)==1) one_hit(ch,victim,gsn_lightningslash,1);
    if ((ch->wpn[0] >= 1000)  && (ch->wpn[3] >= 1000)
    && (ch->wpn[6] >= 1000) && (ch->wpn[9] >= 1000)
    && (ch->wpn[1] >= 1000) && (ch->wpn[4] >= 1000)
    && (ch->wpn[7] >= 1000)
    && (ch->wpn[10] >= 1000) && (ch->wpn[2] >= 1000)
    && (ch->wpn[5] >= 1000) && (ch->wpn[8] >= 1000)
    && (ch->wpn[11] >= 1000) && (ch->wpn[12] >= 1000))
    {
      if (number_range(1,2)==1) one_hit(ch,victim,dt,1);
      if (number_range(1,2)==1) one_hit(ch,victim,gsn_lightningkick,1);
      if (number_range(1,2)==1) one_hit(ch,victim,gsn_knee,1);
      if (number_range(1,2)==1) one_hit(ch,victim,gsn_lightningslash,1);
    }
    return;
  }
  wield1 = get_eq_char( ch, WEAR_WIELD );
  wield2 = get_eq_char( ch, WEAR_HOLD );
  wield3 = get_eq_char( ch, WEAR_THIRD );
  wield4 = get_eq_char( ch, WEAR_FOURTH);
  if ( wield1 != NULL && wield1->item_type == ITEM_WEAPON ) wieldorig  = 1;
  if ( wield2 != NULL && wield2->item_type == ITEM_WEAPON ) wieldorig += 2;
  if ( wield3 != NULL && wield3->item_type == ITEM_WEAPON ) wieldorig += 4;
  if ( wield4 != NULL && wield4->item_type == ITEM_WEAPON ) wieldorig += 8;
  wieldtype = wieldorig;
  wieldie = number_range(1,4);
  if ( wieldorig == 15 ) 
  { 
    if (wieldie == 2) wieldtype = 8; 
    else wieldtype = 4;
  }
  if ( wieldorig == 14 )
  {
    if (wieldie == 2) wieldtype = 8;
    else if (wieldie == 1) wieldtype = 4;
    else if (wieldie == 3) wieldtype = 2;
    else wieldtype = 1;
  }
  if ( wieldorig == 13 )
  {
    if ( wieldie == 1) wieldtype = 8;
    else if (wieldie == 2) wieldtype = 4;
    else wieldtype = 1;    
  }
  if ( wieldorig == 12 )
  {
    if ( wieldie == 1 ) wieldtype = 8;
    else wieldtype = 4;
  }
  if ( wieldorig == 11 )
  {
    if ( wieldie == 1 ) wieldtype = 8;
    if ( wieldie == 2 ) wieldtype = 2;
    else wieldtype = 1;
  }
  if ( wieldorig == 10 )
  {  
    if ( wieldie == 1 ) wieldtype = 8;
    else wieldtype = 2;    
  }
  if ( wieldorig == 9 )
  {
    if ( wieldie == 1 ) wieldtype = 8;
    else wieldtype = 1;
  }
  if ( wieldorig == 8 ) wieldtype = 8;
  if ( wieldorig == 7 )
  {
    if ( wieldie == 1 ) wieldtype = 4;
    else if ( wieldie == 2 ) wieldtype = 2;
    else wieldtype = 1;
  }
  if ( wieldorig == 6 )
  {
    if ( wieldie == 1 || wieldie == 2) wieldtype = 2;
    else wieldtype = 4;
  } 
  if ( wieldorig == 5 )
  {
    if ( wieldie == 1) wieldtype = 4;
    else wieldtype = 1;
  }
  if ( wieldorig == 4 ) wieldtype = 4;
  if ( wieldorig == 3 )    
  {
    if ( wieldie == 2 || wieldie == 4) wieldtype = 2;
    else wieldtype = 1;
  }
  if ( wieldorig == 2 ) wieldtype = 2;
  if ( wieldorig == 1 ) wieldtype = 1;

  if ( wieldtype == 8 ) wield = wield4;
  else if ( wieldtype == 4 ) wield = wield3;
  else if ( wieldtype == 2 ) wield = wield2;
  else wield = wield1;

  if (!IS_NPC(ch) && ch->stance[0] > 0 && number_percent() == 5)
  {
    int stance = ch->stance[0];
    if (ch->stance[stance] >= 200)
    {
      special_move(ch,victim);
      return;
    }    
  }

  unarmed = 0;
  if (IS_SET( ch->flag2, VAMP_OBJMASK) )
  {
    act("You return to your normal form.",ch,NULL,NULL,TO_CHAR);
    sprintf(buf,"%s reforms as %s.\n\r",ch->morph, ch->name );
    act(buf,ch,NULL,NULL,TO_ROOM);
    free_string(ch->morph);
    free_string(ch->objdesc);
    ch->long_descr = str_dup("");
    REMOVE_BIT(ch->flag2, VAMP_OBJMASK);
    REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
  }
  one_hit( ch, victim, dt, wieldtype );

  if ( victim == NULL || victim->position != POS_FIGHTING ) return;

  /* Only want one spell per round from spell weapons...otherwise it's 
   * too powerful, and would unbalance player killing (as this is a PK mud).
   */

  if ( dt == TYPE_UNDEFINED )
  {
    dt = TYPE_HIT;
    if ( wield != NULL && wield->item_type == ITEM_WEAPON )
    {
      dt += wield->value[3];
      if (wield->value[0] >= 1)
      {
        /* Look, just don't ask...   KaVir */
        if (wield->value[0] >= 1000) sn = wield->value[0] - ((wield->value[0] / 1000) * 1000);
        else sn = wield->value[0];
        if (sn != skill_lookup("gas breath") && sn != skill_lookup("desanct") && sn != skill_lookup("sleep") && sn != 0)
        {
          if (victim->position == POS_FIGHTING) (*skill_table[sn].spell_fun) (sn,wield->level,ch,victim);
        }
      }
    }
  }
  if ( victim == NULL || victim->position != POS_FIGHTING ) return;
  if ( ch->fighting != victim || dt == gsn_backstab || dt == gsn_shiroken
    || dt == gsn_garotte || dt == gsn_headbutt || dt == gsn_circle ) return;
  maxcount = number_attacks(ch, victim);
  if (!IS_NPC(ch))
  {
    chance = 0;
    if (wield != NULL && wield->item_type == ITEM_WEAPON)
    {
      int tempnum = wield->value[3];
      chance = (ch->wpn[tempnum]) * 0.5;
    }
    else chance = (ch->wpn[0]) * 0.5;
    if (number_percent() <= chance) maxcount += 1;
  }
  if (wieldorig == 3) maxcount += 1;
  for (countup = 0; countup <= maxcount; countup++)
  {
    wieldtype = wieldorig;
    if ( wieldorig == 15 ) 
    { 
      if (wieldie == 2)  wieldtype = 8; 
      else wieldtype = 4;
    }
    if ( wieldorig == 14 )
    {
      if (wieldie == 2) wieldtype = 8;
      else if (wieldie == 1) wieldtype = 4;
      else if (wieldie == 3) wieldtype = 2;
      else wieldtype = 1;
    }
    if ( wieldorig == 13 )
    {
      if ( wieldie == 1) wieldtype = 8;
      else if (wieldie == 2) wieldtype = 4;
      else wieldtype = 1;    
    }
    if ( wieldorig == 12 )
    {
      if ( wieldie == 1 ) wieldtype = 8;
      else wieldtype = 4;
    }
    if ( wieldorig == 11 )
    {
      if ( wieldie == 1 ) wieldtype = 8;
      if ( wieldie == 2 ) wieldtype = 2;
      else wieldtype = 1;
    }
    if ( wieldorig == 10 )
    {
      if ( wieldie == 1 ) wieldtype = 8;
      else wieldtype = 2;
    }
    if ( wieldorig == 9 )
    {
      if ( wieldie == 1 ) wieldtype = 8;
      else wieldtype = 1;
    }
    if ( wieldorig == 7 )
    {
      if ( wieldie == 1 ) wieldtype = 4;
      else if ( wieldie == 2 ) wieldtype = 2;
      else wieldtype = 1;
    }
    if ( wieldorig == 6 )
    {
      if ( wieldie == 1 || wieldie == 2) wieldtype = 2;
      else wieldtype = 4;
    } 
    if ( wieldorig == 5 )
    {
      if ( wieldie == 1) wieldtype = 4;
      else wieldtype = 1;
    }
    if ( wieldorig == 3 )
    {
      if ( wieldie == 2 || wieldie == 4) wieldtype = 2;
      else wieldtype = 1;
    }
    one_hit( ch, victim, -1, wieldtype );
    if ( victim == NULL || victim->position != POS_FIGHTING ) return;
    if ( ch->fighting != victim ) return;
  }
  if (!IS_NPC(ch) && IS_VAMPAFF(ch,VAM_FANGS))
  {
     if (!IS_NPC(victim)) {
       one_hit(ch,victim, (TYPE_HIT + 10),0);
       one_hit( ch, victim, ( TYPE_HIT + 10 ), 0 );
     }
     else {
       one_hit(ch,victim, (TYPE_HIT + 10),1);
       one_hit( ch, victim, ( TYPE_HIT + 10 ), 1 );
     }
  }
  if ( IS_CLASS(ch, CLASS_DEMON) && IS_SET(ch->warp, WARP_SPIKETAIL) && number_range(1,3) == 1)
  {
    if (IS_NPC(victim)) {
      one_hit( ch, victim, gsn_spiket, 0);
      one_hit( ch, victim, gsn_spiket, 1);
    }
    else one_hit( ch, victim, gsn_spiket, 0);
  }
  /* Golems speciels */
  if (IS_NPC(ch))
  {
    if (ch->pIndexData->vnum == MOB_VNUM_FIRE)
    {
      if ((sn = skill_lookup( "curse" ) ) > 0)
        (*skill_table[sn].spell_fun) (sn,50,ch,victim);
      if ((sn = skill_lookup( "imp faerie fire" ) ) > 0)
        (*skill_table[sn].spell_fun) (sn,50,ch,victim);
    }
    if (ch->pIndexData->vnum == MOB_VNUM_STONE)
    {
      af.type      = skill_lookup("reserved");  
      af.duration  = 20;
      af.location  = APPLY_DAMROLL;
      af.modifier  = -50;
      af.bitvector = 0;
      affect_to_char(victim, &af );
    
      af.type      = skill_lookup("reserved");
      af.duration  = 20;
      af.location  = APPLY_HITROLL;
      af.modifier  = -50;
      af.bitvector = 0;
      affect_to_char(victim, &af );
      send_to_char("You feel weak in the presence of the stone golem.\n\r",victim);
    }
    if (ch->pIndexData->vnum == MOB_VNUM_CLAY)
    {
      if ((sn = skill_lookup( "clay" ) ) > 0)
        (*skill_table[sn].spell_fun) (sn,50,ch,victim);
    }
    if (ch->pIndexData->vnum == MOB_VNUM_IRON)
    {
      if ((sn = skill_lookup( "group heal" ) ) > 0)
        (*skill_table[sn].spell_fun) (sn,50,ch,victim);
    }
  }
  if ( IS_CLASS(ch, CLASS_DEMON) && IS_SET(ch->warp, WARP_SHARDS) && number_range(1,3) == 1)
    one_hit( ch, victim, gsn_shards, 0);
  if ( IS_CLASS(ch, CLASS_DEMON) && IS_SET(ch->warp, WARP_MAGMA) && number_range(1,3) == 1)
  {
    if (IS_NPC(victim)) {
      one_hit( ch, victim, gsn_magma, 0);
      one_hit( ch, victim, gsn_magma, 1);
    }
    else one_hit( ch, victim, gsn_magma, 0);
  }
  if ( IS_CLASS(ch, CLASS_DEMON) && IS_SET(ch->warp, WARP_VENOMTONG) && number_range(1,3) == 1)
    one_hit( ch, victim, gsn_venomt, 0);  
  if ( victim == NULL || victim->position != POS_FIGHTING ) return;
  if (!IS_NPC(ch) && !IS_NPC(victim))  // pkpower
  {
    if (get_ratio(ch) > 2499)
    {
      i = number_range(1,10);
      if (i < 5) one_hit(ch,victim, gsn_supreme,0);
      else if (i < 7) one_hit(ch,victim, gsn_supreme,1);
      else ;
    }
  }
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SHAPESHIFTER))
  {
    if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
    {
      one_hit(ch,victim, gsn_fangs, 1);
      if (ch->pcdata->powers[HYDRA_LEVEL]>0) one_hit(ch,victim, gsn_fangs, 1);
      if (ch->pcdata->powers[HYDRA_LEVEL]>1) one_hit(ch,victim, gsn_fangs, 1);
      if (ch->pcdata->powers[HYDRA_LEVEL]>3) one_hit(ch,victim, gsn_fangs, 1);
      if (ch->pcdata->powers[HYDRA_LEVEL]>4) one_hit(ch,victim, gsn_fangs, 1);
    }
    if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
    {
      if (ch->pcdata->powers[TIGER_LEVEL]>0) multi_hit(ch, victim, gsn_claws);
      if (ch->pcdata->powers[TIGER_LEVEL]>1) multi_hit(ch, victim, gsn_fangs);
    }
    if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM) 
    {
      if (ch->pcdata->powers[BULL_LEVEL]>0) multi_hit(ch, victim, gsn_headbutt);
      if (ch->pcdata->powers[BULL_LEVEL]>1) multi_hit(ch, victim, gsn_hooves);
    }
    if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
    {
      if (IS_NPC(victim)) {
      one_hit(ch, victim, gsn_fireball,1);
      one_hit(ch, victim, gsn_fireball,1);
      }
      else {
      one_hit(ch, victim, gsn_fireball,0);
      one_hit(ch, victim, gsn_fireball,0);
      }
      if (ch->pcdata->powers[FAERIE_LEVEL] > 1) one_hit(ch, victim, gsn_buffet,1);
    }
  }
  if (!IS_NPC(ch) && ch->level > 2)
  {
    if (IS_VAMPAFF(ch,VAM_HORNS) && number_range(1,3) == 1)
      multi_hit( ch, victim, gsn_headbutt );
  }
  if (IS_CLASS(ch, CLASS_TANARRI) && !IS_NPC(ch))
  {
    if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_FANGS))
    {
      one_hit(ch, victim, gsn_fangs, 1);
      if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) one_hit(ch, victim, gsn_fangs, 1);
    }
    if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_FIERY))
    {
      one_hit(ch, victim, gsn_fiery, 1);
      if (IS_CLASS(victim, CLASS_ANGEL)) one_hit(ch, victim, gsn_fiery, 1);
    }
  }
  if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT) && ch->pcdata->powers[WEAPONSKILL] > 9)
  {
    one_hit(ch,victim,gsn_lightningslash,1);
    one_hit(ch,victim,gsn_lightningslash,1);
    one_hit(ch,victim,gsn_lightningslash,1);
  }
  if (IS_CLASS(ch, CLASS_LICH))
  {
    if (ch->pcdata->powers[CON_LORE] > 4) one_hit(ch,victim, gsn_fireball,1);
    if (ch->pcdata->powers[NECROMANTIC] > 4) one_hit(ch,victim, gsn_chillhand,1);
    if (ch->pcdata->powers[DEATH_LORE] > 4) one_hit(ch,victim, gsn_deathaura,1);
  }
  if (IS_CLASS(ch, CLASS_DEMON)  && IS_DEMAFF(ch,DEM_HORNS) && number_range(1,3) == 1)
    one_hit( ch, victim, gsn_headbutt, 0 );
  if (IS_CLASS(ch, CLASS_DEMON) && number_range(1,3) == 1 && ch->power[DISC_DAEM_HELL] > 3)
    one_hit(ch, victim, gsn_hellfire, 0);
  if (IS_CLASS(ch, CLASS_DEMON)  && IS_DEMAFF(ch, DEM_TAIL) && number_range(1,2) == 2)
    one_hit( ch, victim, gsn_sweep, 0 );
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_NINGENNO] >=5 && number_range(1,2) != 1)
    multi_hit(ch, victim, gsn_shiroken);

  if (!IS_NPC(ch))
  {
    if (IS_VAMPAFF(ch,VAM_TAIL) && number_range(1,2) == 1)
      multi_hit( ch, victim, gsn_sweep );
  }
  
  if (!IS_NPC(ch) && IS_VAMPAFF(ch,VAM_HEAD) && number_range(1,2) == 1)
  {
    multi_hit( ch, victim, gsn_fangs );
  }
  if (!IS_NPC(ch) && IS_SET(ch->newbits,NEW_QUILLS) && IS_CLASS(ch, CLASS_WEREWOLF))
    multi_hit(ch,victim,gsn_quills);
  if (!IS_NPC(ch) && IS_SET(ch->newbits,NEW_BLADESPIN) && IS_CLASS(ch, CLASS_SAMURAI))
    multi_hit(ch,victim,gsn_bladespin);
  if (!IS_NPC(ch) && IS_SET(ch->newbits, NEW_DARKTENDRILS) && IS_CLASS(ch, CLASS_DROW))
    multi_hit(ch,victim,gsn_darktendrils);
  if (!IS_NPC(ch) && IS_SET(ch->newbits,NEW_MULTIARMS) && IS_CLASS(ch, CLASS_MAGE))
    multi_hit(ch,victim,gsn_mageshield);
  if (!IS_NPC(ch) && ch->monkab[BODY] > 3 && IS_CLASS(ch, CLASS_MONK))
    multi_hit(ch,victim,gsn_heavenlyaura);
  if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_ANGEL))
  {
    if (IS_SET(ch->pcdata->powers[ANGEL_POWERS], ANGEL_AURA)) multi_hit(ch,victim,gsn_heavenlyaura);
    if (IS_SET(ch->pcdata->powers[ANGEL_POWERS], ANGEL_HALO))
    {
      switch(number_range(1,5))
      {
        default : break;
        case 1 :
          if ((sn = skill_lookup( "curse" ) ) > 0)
            (*skill_table[sn].spell_fun) (sn,50,ch,victim);
          break;
        case 2 :
          if ((sn = skill_lookup( "web" ) ) > 0)
            (*skill_table[sn].spell_fun) (sn,50,ch,victim);
          break;
        case 3 :
          if ((sn = skill_lookup( "imp heal" ) ) > 0)
            (*skill_table[sn].spell_fun) (sn,50,ch,victim);
          break;
        case 4 :
          if ((sn = skill_lookup( "imp fireball" ) ) > 0)
            (*skill_table[sn].spell_fun) (sn,50,ch,victim);
          break;
        case 5 :
          if ((sn = skill_lookup( "godbless" ) ) > 0)
            (*skill_table[sn].spell_fun) (sn,50,ch,victim);
          break;
      }
    }
  }
  if (!IS_NPC(ch) && IS_SET(ch->newbits, NEW_CUBEFORM) && IS_CLASS(ch, CLASS_DROID))
  {
    one_hit(ch,victim,gsn_stuntubes,1);
    multi_hit(ch,victim,gsn_stinger);
  }    
  if (!IS_NPC(ch)&& IS_CLASS(ch,CLASS_WEREWOLF))
  {
    if (IS_SET(ch->newbits,NEW_SLAM) && number_range(1,5-ch->power[DISC_WERE_BEAR]/3)==1)
      multi_hit(ch,victim,gsn_cheapshot);
  }
  if (!IS_NPC(ch) && IS_VAMPAFF(ch,VAM_WINGS) && number_range(1,2) == 1 && ch->level > 2)
    multi_hit( ch, victim, gsn_buffet );
  if (!IS_NPC(ch) && IS_VAMPAFF(ch,VAM_WINGS) && number_range(1,2) == 1 && ch->level > 2)
    multi_hit( ch, victim, gsn_buffet );
  if (IS_CLASS(ch, CLASS_DEMON) && IS_SET(ch->warp, WARP_WINGS) && number_range(1,3) == 1)
    one_hit(ch, victim, gsn_buffet, 0 );
  if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_WEREWOLF) && ch->power[DISC_WERE_RAPT] > 0)
    multi_hit(ch,victim,gsn_rfangs);
  if ( victim == NULL || victim->position != POS_FIGHTING ) return;
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE) && IS_VAMPAFF(ch, VAM_SERPENTIS))
    spell_poison( gsn_poison, (ch->level*number_range(5,10)),ch,victim );
  else if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF) && ch->power[DISC_WERE_SPID] > 0)
    spell_poison( gsn_poison, (ch->level*number_range(5,10)),ch,victim );
  else if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DROW) && IS_SET(ch->pcdata->powers[1], DPOWER_DROWPOISON))
    spell_poison(gsn_poison,(ch->level*number_range(10,20)),ch,victim);
  else if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_NINGENNO] >=5)
    spell_poison(gsn_poison,(ch->level*number_range(5,10)),ch,victim);
  if (victim->itemaffect < 1) return;
  if (IS_NPC(victim) || victim->spl[1] < 4) level = victim->level;
  else level = (victim->spl[1] * 0.25);
  if (IS_SET(victim->newbits, NEW_MONKFLAME) && !IS_AFFECTED(ch, AFF_FLAMING))
  {
    if (number_percent() > 95)
    {
      SET_BIT(ch->affected_by, AFF_FLAMING);
      if (!IS_CLASS(ch, CLASS_WEREWOLF))
      {
        act("Your flaming hands catch $n on fire!", ch, NULL, victim, TO_VICT);
        act("$N's flaming hands catch $n on fire!", ch, NULL, victim, TO_NOTVICT);
        act("$N's flaming hands catch you on fire!.", ch, NULL, victim, TO_CHAR);
      }
      else
      {
      act("Your flaming claws catch $n on fire!", ch, NULL, victim, TO_VICT);
      act("$N's flaming claws catch $n on fire!", ch, NULL, victim, TO_NOTVICT);
      act("$N's flaming claws catch you on fire!.", ch, NULL, victim, TO_CHAR);
      }
    }
    else return;
  }
  if (!IS_NPC(victim))
  {
  if (IS_SET(victim->pcdata->powers[AURAS], DEATH_AURA) && IS_CLASS(victim, CLASS_UNDEAD_KNIGHT))
  {
    while (number_range(1,2) != 1)
      one_hit(victim,ch,gsn_deathaura,0);
  }
  if (IS_SET(victim->pcdata->powers[AURAS], FEAR_AURA) && IS_CLASS(victim, CLASS_UNDEAD_KNIGHT))
  {
    af.type      = skill_lookup("reserved");
    af.duration  = 20;
    af.location  = APPLY_DAMROLL;
    af.modifier  = -20;
    af.bitvector = 0;
    affect_to_char(ch, &af );

    af.type      = skill_lookup("reserved");
    af.duration  = 20;                      
    af.location  = APPLY_HITROLL;
    af.modifier  = -20;                             
    af.bitvector = 0;
    affect_to_char(ch, &af );
    send_to_char("You piss your pants out of fear.\n\r",ch);
  }
  }
  if (IS_ITEMAFF(victim, ITEMA_SHOCKSHIELD) && ch->position == POS_FIGHTING)
    if ( (sn = skill_lookup( "lightning bolt" ) ) > 0)
      (*skill_table[sn].spell_fun) (sn,level,victim,ch);
  if (IS_ITEMAFF(victim, ITEMA_FIRESHIELD) && ch->position == POS_FIGHTING)
    if ( (sn = skill_lookup( "fireball" ) ) > 0)
      (*skill_table[sn].spell_fun) (sn,level,victim,ch);
  if (IS_ITEMAFF(victim, ITEMA_ICESHIELD) && ch->position == POS_FIGHTING)
    if ( (sn = skill_lookup( "chill touch" ) ) > 0)
      (*skill_table[sn].spell_fun) (sn,level,victim,ch);
  if (IS_ITEMAFF(victim, ITEMA_ACIDSHIELD) && ch->position == POS_FIGHTING)
    if ( (sn = skill_lookup( "acid blast" ) ) > 0)
      (*skill_table[sn].spell_fun) (sn,level,victim,ch);
  if (IS_ITEMAFF(victim, ITEMA_CHAOSSHIELD) && ch->position == POS_FIGHTING)
    if ( (sn = skill_lookup( "chaos blast" ) ) > 0)
      (*skill_table[sn].spell_fun) (sn,level,victim,ch);
  return;
}

int number_attacks( CHAR_DATA *ch, CHAR_DATA *victim )
{
  int count = 1;

  if (IS_NPC(ch))
  {
    if (ch->level >= 50) count += 1;
    if (ch->level >= 100) count += 1;
    if (ch->level >= 500) count += 1;
    if (ch->level >= 1000) count +=1;
    if (ch->level >= 1500) count +=1;
    if (ch->level >= 2000) count +=2;
    if( ch->pIndexData->hitsizedice > 20) count += 20;
    else count += ch->pIndexData->hitsizedice;
    return count;
  }
  if (IS_NPC(victim))
  {
    if ( IS_STANCE(ch, STANCE_VIPER) && number_percent() < ch->stance[STANCE_VIPER] * 0.5) count += 1;
    else if ( IS_STANCE(ch, STANCE_MANTIS) && number_percent() < ch->stance[STANCE_MANTIS] * 0.5) count += 1;
    else if ( IS_STANCE(ch, STANCE_TIGER) && number_percent() < ch->stance[STANCE_TIGER] * 0.5) count += 1;
    else if ( IS_STANCE(ch, STANCE_WOLF) && number_percent() < ch->stance[STANCE_WOLF] * 0.5) count += 2;
    else if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_SPEED) 
    && number_percent() < ch->stance[(ch->stance[0])] * 0.5 ) count += 2;
    if (!IS_NPC(ch)) 
    {   
      if (ch->pcdata->upgrade_level > 0) count += ch->pcdata->upgrade_level;
      if ( IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_CELE] > 0)
      {
        count += .5;
        if (ch->power[DISC_VAMP_CELE] > 5)
        {
          count += .5;
          if ( ch->power[DISC_VAMP_CELE] > 7)
          {
            count += .5;
            if ( ch->power[DISC_VAMP_CELE] > 9)
              count += .5;
          }
        }
      }
      if (IS_CLASS(ch, CLASS_TANARRI)) count += ch->pcdata->rank;
      if (IS_CLASS(ch, CLASS_DROID)) count += ch->pcdata->powers[CYBORG_LIMBS];
      if (IS_CLASS(ch, CLASS_ANGEL)) count += ch->pcdata->powers[ANGEL_JUSTICE];
      if (IS_CLASS(ch, CLASS_WEREWOLF))
      {
        if (ch->power[DISC_WERE_LYNX] > 2 )count += 2;
        if (ch->power[DISC_WERE_BOAR] > 6)
        {
          if( ch->move > 40000 ) count += 5;
          else count += ch->move/8000;
        }
      }
      if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
        count += ch->pcdata->powers[WEAPONSKILL]/2;
      if (IS_CLASS(ch, CLASS_LICH)) count += 5;
      if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
      {
        count +=2;
        if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM) count += ch->pcdata->powers[BULL_LEVEL]/2;
        if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM) count += ch->pcdata->powers[TIGER_LEVEL]*2;
        if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM) count += ch->pcdata->powers[FAERIE_LEVEL]*1.5;
        if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM) count += ch->pcdata->powers[HYDRA_LEVEL];
      }
      if ( IS_CLASS(ch, CLASS_DEMON))
      {
        if (IS_DEMPOWER( ch, DEM_SPEED)) count += 2;     
        if (IS_SET(ch->warp, WARP_QUICKNESS)) count += 3;
      }
      if (IS_CLASS(ch, CLASS_DROW))
      {
        if (IS_SET(ch->special, SPC_DROW_WAR)) count += 2;
        if (IS_SET(ch->pcdata->powers[1],DPOWER_SPEED)) count += 3;
      }
    }
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_MAGE) && IS_ITEMAFF(ch, ITEMA_BEAST)) count += 4;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_CHIKYU] >= 3)
      count += 3;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_ONE)
      count += 1;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_TWO)
      count += 1;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_THREE)
      count += 2;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_FOUR)
      count += 2;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_FIVE)
      count += 2;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_SIX)
      count += 3;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_SEVEN)
      count += 3;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_EIGHT)
      count += 4;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_NINE)
      count += 4;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_TEN)
      count += 5;
    if (IS_CLASS(ch, CLASS_SAMURAI)) count += 5;
    if ( IS_ITEMAFF(ch, ITEMA_SPEED) ) count += 2;
  }
  else
  {
    if (!IS_NPC(ch)) if (ch->pcdata->upgrade_level > 0) count += ch->pcdata->upgrade_level;
    if ( IS_STANCE(ch, STANCE_VIPER) && number_percent() <  ch->stance[STANCE_VIPER] * 0.5) count += 1;
    else if ( IS_STANCE(ch, STANCE_MANTIS) && number_percent() < ch->stance[STANCE_MANTIS] * 0.5) count += 1;
    else if ( IS_STANCE(ch, STANCE_TIGER) && number_percent() < ch->stance[STANCE_TIGER] * 0.5) count += 1;
    else if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_SPEED) 
    && number_percent() < ch->stance[(ch->stance[0])] * 0.5 ) count += 2;
    if (!IS_NPC(ch))
    {
      if (IS_CLASS(ch, CLASS_DROID)) count += ch->pcdata->powers[CYBORG_LIMBS];
      if ( IS_CLASS(ch, CLASS_VAMPIRE))
      {
        if (ch->power[DISC_VAMP_CELE] > 0) count += 1;
        if (ch ->power[DISC_VAMP_CELE] > 5) count += 1;
        if (ch->power[DISC_VAMP_CELE] > 7) count += 1;
        if ( ch->power[DISC_VAMP_CELE] > 9) count += 1;
      }
    if ( IS_CLASS(ch, CLASS_MONK) && ch->monkab[COMBAT] > 0 ) count += ch->monkab[COMBAT];
    if ( IS_CLASS(ch, CLASS_MAGE) && IS_ITEMAFF(ch, ITEMA_BEAST)) count += 4;
    if (IS_CLASS(ch, CLASS_ANGEL)) count += 2;
    if (IS_CLASS(ch, CLASS_WEREWOLF))
    {
      if ( ch->power[DISC_WERE_LYNX] > 2) count += 1;
      if ( ch->power[DISC_WERE_BOAR] > 6)
      {
        if (ch->move > 15000)
          count +=10;
        else count += (ch->move/1500);
      }
    }
    if (IS_CLASS(ch, CLASS_LICH)) count += 5;
    if (IS_CLASS(ch, CLASS_TANARRI))
    {
      count += ch->pcdata->rank;
      if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_SPEED)) count += 3;
    }
    if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
      count += ch->pcdata->powers[WEAPONSKILL]/3;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_ONE)
      count += 1;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_TWO)
      count += 1;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_THREE)
      count += 2;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_FOUR)
      count += 2;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_FIVE)
      count += 2;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_SIX)
      count += 3;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_SEVEN)
      count += 3;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_EIGHT)
      count += 4;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_NINE)
      count += 4;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_TEN)
      count += 5;
    if ( IS_ITEMAFF(ch, ITEMA_SPEED) ) count += 1;
    if (IS_CLASS(ch, CLASS_SAMURAI)) count +=5;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_MAGE) && IS_ITEMAFF(ch, ITEMA_BEAST)) count += 4;
    if (IS_CLASS(ch, CLASS_DEMON)&& ch->power[DISC_DAEM_ATTA] > 0) count += ch->power[DISC_DAEM_ATTA] / 2;
    if (IS_CLASS(ch, CLASS_DROW))
    {
      if (IS_SET(ch->special, SPC_DROW_WAR)) count += 3;
      if (IS_SET(ch->pcdata->powers[1],DPOWER_SPEED)) count += 5;
    }
  }
  if (victim->power[DISC_VAMP_CELE] >= 3)
    count -= victim->power[DISC_VAMP_CELE] / 3;
  if (victim->power[DISC_WERE_MANT] >= 3)
    count -= victim->power[DISC_WERE_MANT] / 3;
  else if (IS_ITEMAFF(victim, ITEMA_AFFMANTIS))
    count -= 1;
  if (victim->power[DISC_DAEM_ATTA] > 0)
    count -= victim->power[DISC_DAEM_ATTA] /2;
  if (number_range(1,4) == 2)
    count -= 1;
  }
  if ( count < 1)
    count = 1;
  return count;
}


/*
 * Hit one guy once.
 */
void one_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt, int handtype)
{
  OBJ_DATA *wield;
  int victim_ac;
  int thac0;
  int thac0_00;
  int thac0_32;
  int dam, wdam;
  int diceroll;
  int level;
  int attack_modify;
  int right_hand;

  /*
   * Can't beat a dead char!
   * Guard against weird room-leavings.
   */
  if (victim->position == POS_DEAD || ch->in_room != victim->in_room)
    return;
  if ( !IS_NPC(ch) )
  {
    if (IS_AFFECTED( ch, AFF_PEACE) && ch->fighting != NULL)
    REMOVE_BIT(ch->affected_by, AFF_PEACE);
  }
  if ( !IS_NPC(victim) )
  {
    if (IS_AFFECTED(victim, AFF_PEACE) && victim->fighting != NULL)
    REMOVE_BIT(victim->affected_by, AFF_PEACE);
  }
  /*
   * Figure out the type of damage message.
   */
  if (handtype == 8)
  {
    wield = get_eq_char( ch, WEAR_FOURTH );
    right_hand = 8;
  }
  else if (handtype == 4)
  {
    wield = get_eq_char( ch, WEAR_THIRD );
    right_hand = 4;
  }
  else if (handtype == 2)
  {
    wield = get_eq_char( ch, WEAR_HOLD );
    right_hand = 2;
  }
  else if (handtype == 0) 
  {
    wield = NULL;
    right_hand = 0;
  }
  else
  {
    if (IS_SET(ch->act, PLR_RIGHTHAND))
      wield = get_eq_char( ch, WEAR_WIELD );
    else if (IS_SET(ch->act, PLR_LEFTHAND))
      wield = get_eq_char( ch, WEAR_HOLD );
    else
    {
      if (number_range(1,10) > 4)
        wield = get_eq_char( ch, WEAR_HOLD );
      else
        wield = get_eq_char( ch, WEAR_WIELD );
    }
    right_hand = 1; // improve the right hand anyway...
  }
  if ( dt == TYPE_UNDEFINED )
  {
    dt = TYPE_HIT;
    if ( wield != NULL && wield->item_type == ITEM_WEAPON )
      dt += wield->value[3];
  }
  if (ch->wpn[dt-1000] > 5) level = (ch->wpn[dt-1000] / 5);
  else level = 1;
  if (level > 40) level = 40;

  /*
   * Calculate to-hit-armor-class-0 versus armor.
   */
  if ( IS_NPC(ch) )
  {
    thac0_00 = 20;
    thac0_32 =  0;
  }
  else
  {
    thac0_00 = SKILL_THAC0_00;
    thac0_32 = SKILL_THAC0_32;
  }
  thac0     = interpolate( level, thac0_00, thac0_32 ) - char_hitroll(ch);
  victim_ac = UMAX( -100, char_ac(victim) / 10 );
  if (!can_see( ch, victim ))
    victim_ac -= 4;
  if (ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_3))
    victim_ac += 300;
  if (ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_2))
    victim_ac += 200;
  if (ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_1))
    victim_ac += 100;

  
  /*
   * The moment of excitement!
   */
  while ((diceroll = number_bits( 5 ) ) >= 20)
  ;

  if ( diceroll == 0 || ( diceroll != 19 && diceroll < thac0 - victim_ac ) )
  { /* Miss. */
    damage( ch, victim, 0, dt );
    tail_chain( );
    if (!is_safe(ch,victim))
    {
      improve_wpn(ch,dt,right_hand);
      improve_stance(ch);
    }
    return;
  }

  /*
   * Hit.
   * Calc damage.
   */
  if ( IS_NPC(ch) )
  {
    dam = number_range( ch->level / 2, ch->level * 3 / 2 );
    if ( wield != NULL )
      dam += dam * 0.5;
  }
  else
  {
    if (dt == gsn_mageshield)
    {
      dam = (ch->spl[RED_MAGIC] + ch->spl[YELLOW_MAGIC] + ch->spl[GREEN_MAGIC] 
             + ch->spl[PURPLE_MAGIC] + ch->spl[BLUE_MAGIC])/5;
      dam = number_range(2*dam, 3*dam);
    }
    else if (wield != NULL && wield->item_type == ITEM_WEAPON)
      dam = dice( wield->value[1], wield->value[2] );
    else if ( IS_VAMPAFF(ch, VAM_CLAWS) && IS_GAR1(ch, WOLF_RAZORCLAWS))
      dam = dice( 25, 35 ); 
    else if ( IS_VAMPAFF(ch,VAM_CLAWS))
      dam = dice( 10, 20 );
    else if ( IS_SET(ch->newbits, NEW_MONKADAM))
      dam = dice( 10, 25 );
    else
      dam = dice( 4, 10 );
    if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_SEVEN)
      dam += dice( 1, 5 );
    if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_EIGHT)
      dam += dice( 6, 10 );
    if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_NINE)
      dam += dice( 11, 15 );
    if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_TEN)
      dam += dice( 16, 20 );
  }
  /*
   * Bonuses.
   */
  dam += char_damroll(ch);
  if (!IS_AWAKE(victim)) dam *= 2;
  if (!IS_NPC(ch) && dt >= TYPE_HIT) dam = dam + (dam * (UMIN(350,(ch->wpn[dt-1000]+1)) / 60));

  /* Other Resistances */
   
  if (IS_ITEMAFF(victim, ITEMA_RESISTANCE))
  {
    dam *= 3;
    dam /= 4;
  }

  /* damage reduction */
  if (!IS_NPC(ch) && !IS_NPC(victim)) dam /= 2;  /* slow down pk */
  if (IS_NPC(ch) && dam > 2000) dam = 2000 + (dam-2000)/2;    /* mob damage >2000 halved */
  if ( dam <= 0 ) dam = 5;

  /* up that damage */
  if (!IS_NPC(ch)) {
    if (ch->pcdata->upgrade_level == 1)
      dam *= 1.05;
    else if (ch->pcdata->upgrade_level == 2)
      dam *= 1.10;
    else if (ch->pcdata->upgrade_level == 3)
      dam *= 1.15;
    else if (ch->pcdata->upgrade_level == 4)
      dam *= 1.20;
    else if (ch->pcdata->upgrade_level == 5)
      dam *= 1.25;
    /* 5% extra damage per level */
  }

  /*
   * I doubt this has much effect - Jobo
   */
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE) && IS_NPC(victim))
    dam *= (1 + ch->power[DISC_VAMP_POTE]/15);

  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_TANARRI))
  {
    if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_MIGHT)) dam *= 1.5;
  }

  /*
   * This one for angels have no effect, take a look and rebalance. Jobo
   */
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ANGEL)) dam *= (1 + ch->pcdata->powers[ANGEL_JUSTICE] / 10);

  if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_DEMON) &&  IS_DEMPOWER(ch, DEM_MIGHT) )
    dam *= 1.1;
  if (!IS_NPC(ch) && !IS_NPC(victim) && IS_CLASS(ch, CLASS_UNDEAD_KNIGHT) && IS_CLASS(victim, CLASS_SHAPESHIFTER)) dam *= 1.2;
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_MAGE))
  {
    if (dt==gsn_mageshield && ch->pcdata->powers[PINVOKE]>6) dam *=1.4;
    if (dt==gsn_mageshield && ch->pcdata->powers[PINVOKE]>9) dam *=1.4;
  }
  if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA))
  {
    if (ch->pcdata->rank == BELT_ONE) dam *= 1.1;
    else if (ch->pcdata->rank == BELT_TWO) dam *= 1.2;
    else if (ch->pcdata->rank == BELT_THREE) dam *= 1.3;
    else if (ch->pcdata->rank == BELT_FOUR) dam *= 1.4;
    else if (ch->pcdata->rank == BELT_FIVE) dam *= 1.5;
    else if (ch->pcdata->rank == BELT_SIX) dam *= 1.6;
    else if (ch->pcdata->rank == BELT_SEVEN) dam *= 1.7;
    else if (ch->pcdata->rank == BELT_EIGHT) dam *= 1.8;
    else if (ch->pcdata->rank == BELT_NINE) dam *= 1.9;
    else if (ch->pcdata->rank == BELT_TEN) dam *= 2.0;
  }
  if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_DEMON) &&  IS_SET(ch->warp, WARP_STRONGARMS))
    dam   *= 1.2;

  if(IS_NPC(victim)) {

  if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_3)
    && ch->stance[(ch->stance[0])] >100)
      dam += dam * 1.5 * (ch->stance[(ch->stance[0])] / 200);
  else if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_2)
    && ch->stance[(ch->stance[0])] >100)
      dam += dam * 1.0 * (ch->stance[(ch->stance[0])] / 200);
  else if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_1)
    && ch->stance[(ch->stance[0])] >100)
      dam += dam * 0.5 * (ch->stance[(ch->stance[0])] / 200);
  }

  if(!IS_NPC(victim)) {
    
  if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_3)
    && ch->stance[(ch->stance[0])] >100)
      dam += dam * 0.5 * (ch->stance[(ch->stance[0])] / 200);
  else if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_2)
    && ch->stance[(ch->stance[0])] >100)
      dam += dam * 0.3 * (ch->stance[(ch->stance[0])] / 200);
  else if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_1)
    && ch->stance[(ch->stance[0])] >100)
      dam += dam * 0.1 * (ch->stance[(ch->stance[0])] / 200);
  }

  if ( victim->stance[0] > 12 && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_RESIST_3)
    && victim->stance[(victim->stance[0])] >100)
      dam *= 0.7;
  else if ( victim->stance[0] > 12 && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_RESIST_2)
    && victim->stance[(victim->stance[0])] >100)
      dam *= 0.8;
  else if ( victim->stance[0] > 12 && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_RESIST_1)
    && victim->stance[(victim->stance[0])] >100)
      dam *= 0.9;

  /* The test ends here */

  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SHAPESHIFTER))
  {
    dam *= 1.4;
    if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM && ch->pcdata->powers[TIGER_LEVEL] > 1) dam *= 1.5;
    else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM && ch->pcdata->powers[FAERIE_LEVEL] > 1) dam *= 1.2;
    else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM && ch->pcdata->powers[HYDRA_LEVEL] > 1) dam *= 1.6;
    else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM && ch->pcdata->powers[BULL_LEVEL] > 1) dam *= 1.7;
    else dam = dam;
  }
  if (IS_CLASS(ch,CLASS_DROID) && !IS_NPC(ch))
  {
    if (ch->pcdata->powers[CYBORG_LIMBS] > 0) dam *= 1.3;
    if (ch->pcdata->powers[CYBORG_LIMBS] > 2) dam *= 1.5;
  }
  if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
  {
    if (ch->pcdata->powers[WEAPONSKILL] > 4) dam *=1.2;
    if (ch->pcdata->powers[WEAPONSKILL] > 8) dam *=1.3;
  }
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF) && ch->power[DISC_WERE_BEAR] > 5)
    dam *= 1.2;
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_SHAPESHIFTER))
  {
    if (victim->pcdata->powers[SHAPE_FORM] == FAERIE_FORM && victim->pcdata->powers[FAERIE_LEVEL]>0)
    {
      int growl = number_range(1,50);
      if (growl == 10 || growl == 20 || growl == 30 || growl == 40 || growl == 50)
      {
        stc("#3You dodge your opponents attack#n.\n\r",victim);
        stc("#3Your opponent dodges your attack#n.\n\r",ch);
        dam = 0;
      }
    }
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->newbits, NEW_DARKTENDRILS))
  {
    int ddodge = number_range(1,50);
    if (ddodge == 10 || ddodge == 20 || ddodge == 30 || ddodge == 40 || ddodge == 50)
    {
      stc("#0You dark aura absorbs the damage#n.\n\r",victim);
      stc("#0Your opponent's dark aura absorbs your damage#n.\n\r",ch);
      dam = 0;
    }
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_MAGE) && IS_ITEMAFF(victim, ITEMA_ILLUSIONS))
  {
    int ddodge = number_range(1,50);
    if (ddodge == 10 || ddodge == 20 || ddodge == 30 || ddodge == 40 || ddodge == 50)
    {
      stc("#CYour illusion takes the blow, saving you from harm#n.\n\r",victim);
      stc("#CYour weapon passes through your opponent without doing any harm#n.\n\r",ch);
      dam = 0;
    }
  }
  if (!IS_NPC(ch) && wield != NULL)
  {
    if (number_range(1,6) == 2)
    {
      if (ch->fighting == victim && IS_WEAP(wield,WEAPON_VAMPIRIC))
      {
        wdam = number_range(1,wield->level / 5 + 1);
        act("$p draws life from $n.",victim,wield,NULL,TO_ROOM);
        act("You feel $p drawing your life away.",
        victim,wield,NULL,TO_CHAR);
        hurt_person(ch,victim, wdam); 
        if (victim->hit <= 0) return;
      }
      if (ch->fighting == victim && (IS_WEAP(wield,WEAPON_FLAMING) || IS_WEAP(wield, WEAPON_ELE_FLAME)))
      {
        wdam = number_range(1,wield->level / 4 + 1);
        if (!IS_SET(ch->act, PLR_BRIEF2)) act("$N is burned by your $p.", ch, wield, victim, TO_CHAR);
        if (!IS_SET(victim->act, PLR_BRIEF2)) act("You are burned by $n's $p.", ch, wield, victim, TO_VICT);
        fire_effect( (void *) victim,wield->level/2,wdam,TARGET_CHAR);
        hurt_person(ch, victim, wdam);
        if (victim->hit <= 0) return;
      }
      if (ch->fighting == victim && (IS_WEAP(wield,WEAPON_FROST) || IS_WEAP(wield, WEAPON_ELE_WATER)))
      {
        int sn = skill_lookup("chill touch");
        if (!is_affected(victim,sn)) {
        wdam = number_range(1,wield->level / 6 + 2);
        if (!IS_SET(ch->act, PLR_BRIEF2)) act("Your $p freezes $N.", ch, wield, victim, TO_CHAR);
        if (!IS_SET(victim->act, PLR_BRIEF2)) act("You are frozen by $n's $p.", ch, wield, victim, TO_VICT);
        cold_effect(victim,wield->level/2,wdam,TARGET_CHAR);
        hurt_person(ch,victim, wdam); }
        if (victim->hit <= 0) return;
      }
      if (ch->fighting == victim && IS_WEAP(wield, WEAPON_ELE_EARTH) )
      {
        wdam = number_range(1,wield->level / 5 + 2);
        act("$p covers $n in acid.",victim,wield,NULL,TO_ROOM);
        act("The corrosive touch of $p surrounds you with acid.",
        victim,wield,NULL,TO_CHAR);
        acid_effect(victim,wield->level/2,wdam,TARGET_CHAR);
        hurt_person(ch,victim, wdam);
        if (victim->hit <= 0) return;
      }
      if (ch->fighting == victim && (IS_WEAP(wield,WEAPON_SHOCKING)  || IS_WEAP(wield, WEAPON_ELE_AIR)))
      {
        wdam = number_range(1,wield->level/5 + 2);
        act("$n is struck by lightning from $p.",victim,wield,NULL,TO_ROOM);
        act("You are shocked by $p.",victim,wield,NULL,TO_CHAR);
        shock_effect(victim,wield->level/2,wdam,TARGET_CHAR);
        hurt_person(ch,victim, wdam);
        if (victim->hit <= 0) return;
      }
    }
  }
  if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_MONK) && dt != gsn_heavenlyaura )
  {
    if (ch->chi[CURRENT] > 0 && ch->chi[CURRENT] < 3 ) dam *= 1.3;
    if (ch->chi[CURRENT] > 2 && ch->chi[CURRENT] < 7 ) dam *= ((ch->chi[CURRENT]) / 2);
  }
  if ( dt == gsn_garotte )
    dam *= (number_range(3,5));
   
  if ( dt == gsn_backstab ) {
    if (IS_CLASS(ch, CLASS_NINJA) && !IS_NPC(ch) && ch->pcdata->powers[NPOWER_NINGENNO] >=2)
      dam *= number_range(50,60);
    else if( IS_CLASS(ch, CLASS_DROW))
    {
      dam += number_range(100,1000);
      dam *= number_range(7,10);
    }
    else dam *= number_range(2,4);
  }

  /*
   * Damage type modifiers.
   */
  if (dt == gsn_magma && IS_CLASS(ch, CLASS_TANARRI))        dam *= 1.5;
  if (dt == gsn_booming && !IS_NPC(ch))                      dam *= ch->pcdata->rank/2;
  if (dt == gsn_circle && ch->pcdata->rank == BELT_EIGHT)    dam *= 2;
  if (dt == gsn_circle && ch->pcdata->rank == BELT_NINE)     dam *= 2.5;
  if (dt == gsn_circle && ch->pcdata->rank == BELT_TEN)      dam *= 3.0;
  if (dt == gsn_heavenlyaura && IS_CLASS(ch, CLASS_MONK))    dam /= 20;
  if (dt == gsn_deathaura)                                   dam /= 1.7;
  if (dt == gsn_potato)                                      dam /= 2;
  if (dt == gsn_shred)                                       dam *= 2;

  dam = cap_dam(ch,victim,dam);
  if( IS_NPC(ch) && ch->pIndexData->hitplus > 3) dam *=3;
  else if( IS_NPC(ch) && ch->pIndexData->hitplus > 0 ) dam *= ch->pIndexData->hitplus;

  if( IS_NPC(victim) && !IS_NPC(ch))
  {
    if( victim->pIndexData->hitnodice > 100 ) victim->pIndexData->hitnodice = 0;
    dam = dam * (100 - victim->pIndexData->hitnodice) / 100;
  }

  attack_modify = dice(1, 100);
  randomize_damage(ch,dam,attack_modify);
  if (dt == gsn_deathaura && dam >= victim->hit && IS_NPC(victim)) dam = victim->hit-1; /* trust me, Jobo */
  damage( ch, victim, dam, dt );

  tail_chain( );
  if (!is_safe(ch,victim))
  {
    improve_wpn(ch,dt,right_hand);
    improve_stance(ch);
  }
}

int randomize_damage( CHAR_DATA *ch, int dam, int am )
{
  dam = dam * (am + 50) / 100;
  return dam;
}

int cap_dam(CHAR_DATA *ch, CHAR_DATA *victim, int dam)
{
  if (is_safe(ch, victim)) return 0;

  if (!IS_NPC(victim)) {
    if (victim->pcdata->upgrade_level == 1)
      dam *= 0.95;
    else if (victim->pcdata->upgrade_level == 2)
      dam *= 0.90;
    else if (victim->pcdata->upgrade_level == 3)
      dam *= 0.85;
    else if (victim->pcdata->upgrade_level == 4)
      dam *= 0.80;
    else if (victim->pcdata->upgrade_level == 5)
      dam *= 0.75;
    /* 5% less damage per level */
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DROW)) 
  {
    if (IS_SET(victim->pcdata->powers[1], DPOWER_TOUGHSKIN))
      dam /= 3;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_LICH))
  {
    dam /=5;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_ANGEL))
  {
    dam *= (100 - (victim->pcdata->powers[ANGEL_HARMONY] * 12));
    dam /= 100;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_TANARRI))
  {
    if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_EXOSKELETON)) dam *= 0.20;
    if (IS_CLASS(ch, CLASS_ANGEL)) dam *= 0.75; /* trained to kill angels */
    if (IS_CLASS(ch, CLASS_DROID)) dam *= 0.8;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DROID))
  {
    if (IS_CLASS(ch, CLASS_SHAPESHIFTER)) dam *= 0.85;
    if (IS_CLASS(ch, CLASS_SAMURAI)) dam *= 0.9;
    if (victim->pcdata->powers[CYBORG_BODY] > 0) dam /= 1.75;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE))
  {
    if (victim->power[DISC_VAMP_PROT] >= 10) dam /= 1.5;
    if (victim->power[DISC_VAMP_FORT] > 0)
    {
      dam *= (100 - (victim->power[DISC_VAMP_FORT] * 5.5));
      dam /= 100;
    }
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_SAMURAI))
  {
    if (IS_CLASS(ch, CLASS_SHAPESHIFTER)) dam /= 1.6; /* damn shapies */
    dam /= 1.75;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_MAGE))
  {
    if (IS_ITEMAFF(victim, ITEMA_STEELSHIELD)) dam /= 1.8;
    if (IS_ITEMAFF(victim, ITEMA_BEAST))       dam /= 1.8;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_SHAPESHIFTER))
  {
    dam /= 2.5;
    if (victim->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
    {
      dam *= (100 - victim->pcdata->powers[FAERIE_LEVEL]*9);
      dam /= 100;
    }
    else if (victim->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
    {
      dam *= (100 - victim->pcdata->powers[TIGER_LEVEL]*8);
      dam /= 100;
      if (victim->pcdata->powers[PHASE_COUNTER] > 0)
      {
        dam *= (100 - victim->pcdata->powers[PHASE_COUNTER] * 2.5);
        dam /= 100;
      }
    }
    else if (victim->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
    {
      dam *= (100 - victim->pcdata->powers[HYDRA_LEVEL]*10);
      dam /= 100;
    }
    else if (victim->pcdata->powers[SHAPE_FORM] == BULL_FORM)
    {
      dam *= (100 - victim->pcdata->powers[BULL_LEVEL]*12);
      dam /= 100;
    }
    else dam = dam;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_MONK))
  {
    if (victim->monkab[BODY] >= 2) dam /= 1.5;
    if (victim->chi[CURRENT] > 0 )
    {
      dam *= (100 - (victim->chi[CURRENT] * 7) );
      dam /= 100;
    }
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DEMON))
  {
    if (victim->power[DISC_DAEM_IMMU] > 0)
    {
      dam *= (100 - (victim->power[DISC_DAEM_IMMU] * 4));
      dam /= 100;
    }
    if (IS_DEMPOWER( victim, DEM_TOUGH))    dam *= 0.8;
    if (IS_SET(victim->warp, WARP_STAMINA)) dam *= 0.8;
    if (IS_SET(victim->warp, WARP_SBODY))   dam *= 0.8;
    if (IS_SET(victim->warp, WARP_CBODY))   dam *= 0.8;    
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_UNDEAD_KNIGHT))
  {
    if (IS_CLASS(ch, CLASS_SHAPESHIFTER)) dam *= 0.75;
    if (IS_CLASS(ch, CLASS_SAMURAI)) dam *= 0.7;
    dam *= (100 - (victim->pcdata->powers[UNDEAD_SPIRIT] * 6));
    dam /= 100;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_NINJA))
  {
    if (victim->pcdata->powers[NPOWER_CHIKYU] >= 1) dam /= 2.2;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_WEREWOLF))
  {
    if(victim->power[DISC_WERE_BOAR] > 2) dam /= 2;
    if(IS_GAR1(victim, WOLF_COCOON))      dam /= 2;
  }
  if (dam >= 2000) dam = dam - (dam / number_range(3,10) + number_range(10,30));
  if (!IS_NPC(victim))
  {
    dam = dam + (dam / number_range(2,5) + number_range(10,50));
    dam *= (number_range(2,4) * number_range(2,3) / number_range(4,6));
  }
  if (dam <= 0) dam = 1;
  if (dam > 1000000) dam = 1000000;                                               
  if (dam > ch->damcap[DAM_CAP]) dam = number_range( (ch->damcap[DAM_CAP] - 200), (ch->damcap[DAM_CAP] + 100) );
  if ( IS_AFFECTED(victim, AFF_SANCTUARY ) ) dam *= 0.5;
  return dam;
}

bool can_counter( CHAR_DATA *ch)
{
  if (!IS_NPC(ch) && IS_STANCE(ch, STANCE_MONKEY)) return TRUE;
  return FALSE;
}

bool can_bypass( CHAR_DATA *ch, CHAR_DATA *victim )
{
  if (IS_NPC(ch) || IS_NPC(victim)) return FALSE;
  else if (IS_STANCE(ch, STANCE_VIPER)) return TRUE;
  else if (IS_STANCE(ch, STANCE_MANTIS)) return TRUE;
  else if (IS_STANCE(ch, STANCE_TIGER)) return TRUE;
  else if (IS_STANCE(ch, STANCE_WOLF)) return TRUE;
  else if (ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_BYPASS)) return TRUE;
  return FALSE;
}

void update_damcap( CHAR_DATA *ch, CHAR_DATA *victim )
{
  int max_dam = 1000;
  int i = 0;

  if (!IS_NPC(ch))
  {
    if (ch->pcdata->upgrade_level > 0) max_dam += ch->pcdata->upgrade_level * 500;
    if (ch->generation == 1) max_dam += 250;
    if (ch->generation == 2) max_dam += 200;
    if (ch->generation == 3) max_dam += 150;
    if (ch->generation == 4) max_dam += 100;
    if (ch->generation == 5) max_dam += 50;
    if (IS_CLASS(ch, CLASS_MAGE))
    {
       if (IS_ITEMAFF(ch, ITEMA_BEAST)) max_dam += 750;
       max_dam += 500;
    }
    if (IS_CLASS(ch, CLASS_TANARRI))
    {
      if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_MIGHT)) max_dam += 500;
      max_dam += ch->pcdata->rank*375; /* max rank is 6 btw */
    }
    if (IS_CLASS(ch, CLASS_LICH))
    {
      max_dam += 450;
      if (ch->pcdata->powers[CON_LORE] > 4) max_dam += 350;
      if (ch->pcdata->powers[DEATH_LORE] > 4) max_dam += 350;
      if (ch->pcdata->powers[LIFE_LORE] > 4) max_dam += 350;
      if (ch->pcdata->powers[NECROMANTIC] > 4) max_dam += 350;
      if (ch->pcdata->powers[CHAOS_MAGIC] > 4) max_dam += 350;
    }
    if (IS_CLASS(ch, CLASS_ANGEL))
    {
      max_dam += ch->pcdata->powers[ANGEL_JUSTICE]*125;
      max_dam += ch->pcdata->powers[ANGEL_PEACE]*125;
      max_dam += ch->pcdata->powers[ANGEL_HARMONY]*125;
      max_dam += ch->pcdata->powers[ANGEL_LOVE]*125;
    }
    if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
    {
      max_dam += 800;
      if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM) max_dam += ch->pcdata->powers[TIGER_LEVEL] * 325;
      else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM) max_dam += ch->pcdata->powers[HYDRA_LEVEL] * 350;
      else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM) max_dam += ch->pcdata->powers[BULL_LEVEL] * 300;
      else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM) max_dam += ch->pcdata->powers[FAERIE_LEVEL] * 275;
    }
    if (IS_CLASS(ch, CLASS_DROW))
    {
      max_dam += 500;
      if (IS_SET(ch->newbits, NEW_DROWHATE)) max_dam += 650;
      if (IS_SET(ch->newbits,NEW_DFORM)) max_dam += 650;
    }
    if (IS_CLASS(ch, CLASS_DEMON) && ch->in_room != NULL)
    {
      max_dam += ch->power[DISC_DAEM_ATTA] * 50;
      max_dam += 500;
      if (ch->pcdata->souls > 0) max_dam += UMIN(350, 70 * ch->pcdata->souls);
      if (ch->in_room->vnum >= 93420 && ch->in_room->vnum <= 93426) max_dam += 6000;
      max_dam += (ch->pcdata->stats[DEMON_POWER] * 2);
    }
    if (IS_CLASS(ch, CLASS_DROID))
    {
      if (ch->pcdata->powers[CYBORG_LIMBS] > 0) max_dam += 450;
      if (ch->pcdata->powers[CYBORG_LIMBS] > 1) max_dam += 450;
      if (ch->pcdata->powers[CYBORG_LIMBS] > 2) max_dam += 450;
      if (ch->pcdata->powers[CYBORG_LIMBS] > 3) max_dam += 450;
      if (ch->pcdata->powers[CYBORG_LIMBS] > 4) max_dam += 450;
    }
    if (IS_CLASS(ch, CLASS_MONK)) {
      max_dam +=  (ch->monkab[COMBAT] * 100);
      if (ch->chi[CURRENT] > 0) max_dam += ch->chi[CURRENT] * 200;
    }
    if (IS_CLASS(ch, CLASS_VAMPIRE))
    {
      max_dam += (ch->rage * 5);
      if (ch->power[DISC_VAMP_POTE] > 0) 
        max_dam += (ch->power[DISC_VAMP_POTE] * 50);
      if( ch->pcdata->rank == AGE_TRUEBLOOD)  max_dam += 600;
      else if( ch->pcdata->rank == AGE_LA_MAGRA) max_dam += 500;
      else if (ch->pcdata->rank == AGE_METHUSELAH) max_dam += 400;
      else if (ch->pcdata->rank == AGE_ELDER) max_dam += 250;
      else if (ch->pcdata->rank == AGE_ANCILLA) max_dam += 100;
    }
    if (IS_CLASS(ch, CLASS_NINJA))
    {
      max_dam += (ch->rage * 4);
      if(ch->pcdata->powers[NPOWER_CHIKYU] >=6 && ch->pcdata->powers[HARA_KIRI] > 0) 
        max_dam += 500;
      if(ch->pcdata->powers[NPOWER_CHIKYU] >=2)
        max_dam += 500;
    }
    if (IS_CLASS(ch, CLASS_WEREWOLF))
    {
      if (ch->rage > 99)
      {
        max_dam += ch->rage;
        max_dam += 400;
      }
      if (ch->power[DISC_WERE_PAIN]>9) max_dam += 750;
    }
    if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
      max_dam += ch->pcdata->powers[WEAPONSKILL] * 275;
    if (IS_CLASS(ch, CLASS_SAMURAI) && (get_eq_char(ch, WEAR_WIELD) != NULL))
    {
      for (i=0; i < 13 ; i++)
        if (ch->wpn[i] >= 1000) max_dam += 175;
      max_dam += 375;
    }
  }
  if (IS_ITEMAFF(ch, ITEMA_ARTIFACT)) max_dam += 500;

  if (IS_NPC(victim) || victim->stance[0] != STANCE_MONKEY )
  {
    if      (ch->stance[0] == STANCE_BULL)   max_dam += 200;
    else if (ch->stance[0] == STANCE_DRAGON) max_dam += 250;
    else if (ch->stance[0] == STANCE_WOLF)   max_dam += 250;
    else if (ch->stance[0] == STANCE_TIGER)  max_dam += 200;
    else if (ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMCAP_3)) max_dam += 550;
    else if (ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMCAP_2)) max_dam += 400;
    else if (ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMCAP_1)) max_dam += 250;
  }

  if (!IS_NPC(victim) && ch->stance[0] != STANCE_MONKEY )
  {  
    if (victim->stance[0] > 12 && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_REV_DAMCAP_3)) max_dam -= 550;
    else if (victim->stance[0] > 12 && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_REV_DAMCAP_2)) max_dam -= 400;
    else if (victim->stance[0] > 12 && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_REV_DAMCAP_1)) max_dam -= 250;
  }


  /* Max Dam Reductions */
  /* Werewolves */

  if (!IS_NPC(victim))
  {
    int silver_tol = (victim->siltol * 2.5);
    if (IS_CLASS(victim, CLASS_WEREWOLF) && IS_ITEMAFF(ch, ITEMA_RIGHT_SILVER))
      max_dam += (250 - silver_tol);
    if (IS_CLASS(victim, CLASS_WEREWOLF) && IS_ITEMAFF(ch, ITEMA_LEFT_SILVER))
      max_dam += (250 - silver_tol);
  /* Vampies */
    if (!IS_NPC(ch) && (IS_CLASS(victim, CLASS_VAMPIRE)))
      max_dam -= (victim->power[DISC_VAMP_FORT] * 50); 
    if (IS_NPC(ch) || ch->stance[0] != STANCE_MONKEY)
    {
      if      (victim->stance[0] == STANCE_CRAB)    max_dam -= 250;
      else if (victim->stance[0] == STANCE_DRAGON)  max_dam -= 250;
      else if (victim->stance[0] == STANCE_SWALLOW) max_dam -= 250;
    }
    if (IS_CLASS(victim,CLASS_DEMON) && (victim->in_room->vnum >= 93420 && victim->in_room->vnum <= 93426))
      max_dam *= 0.5;
  }

  if (ch->level >= LEVEL_BUILDER) max_dam = 30000;
//  if (ch->level >= LEVEL_BUILDER && !IS_NPC(ch)) max_dam = 100000; // Det er pisse irreterende Thomas.. jeg kan intet teste mod mobs, de doer automatisk. DIE!
  ch->damcap[DAM_CAP] = max_dam;
  ch->damcap[DAM_CHANGE] = 0;
  return;
}


/*
 * Inflict damage from a hit.
 */
void damage( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt )
{
  CHAR_DATA *emb;

  if ( victim->position == POS_DEAD ) return;
  if ( ch->embracing != NULL )
  {
    if ( ( emb = ch->embracing ) != NULL ) stop_embrace(ch,emb);
    else stop_embrace(ch,NULL);
  }
  /*
   * Stop up any residual loopholes.
   */
  if (ch->damcap[DAM_CHANGE] == 1) update_damcap(ch,victim);
  if ( dam > ch->damcap[0] ) dam = ch->damcap[0];
  
  /*
   * Certain attacks are forbidden.
   * Most other attacks are returned.
   */
  if ( victim != ch )
  {
    if (is_safe( ch, victim )) return;
    if ( victim->position > POS_STUNNED )
    {
      if ( victim->fighting == NULL ) set_fighting( victim, ch );
      victim->position = POS_FIGHTING;
    }
    if ( victim->position > POS_STUNNED )
    {
      if ( ch->fighting == NULL )
      set_fighting( ch, victim );
      /*
       * If victim is charmed, ch might attack victim's master.
       */
      if ( IS_NPC(ch)
      &&   IS_NPC(victim)
      &&   IS_AFFECTED(victim, AFF_CHARM)
      &&   victim->master != NULL
      &&   victim->master->in_room == ch->in_room
      &&   number_bits( 3 ) == 0 )
      {
        stop_fighting( ch, FALSE );
        multi_hit( ch, victim->master, TYPE_UNDEFINED );
        return;
      }
    }
    if ( victim->master == ch ) stop_follower( victim );
  /*
   * Damage modifiers.
   */
    if ( IS_AFFECTED(ch, AFF_HIDE) )
    {
      if (!can_see(victim,ch)) 
      {
        dam *= 1.5;
        send_to_char("You use your concealment to get a surprise attack!\n\r",ch);
      }
      REMOVE_BIT( ch->affected_by, AFF_HIDE );
      act( "$n leaps from $s concealment.", ch, NULL, NULL, TO_ROOM );
    }
    if ( IS_AFFECTED(victim, AFF_PROTECT) && IS_EVIL(ch) && dam > 1  )
      dam -= dam * 0.25;
    if ( IS_AFFECTED(victim, AFF_PROTECT_GOOD) && IS_GOOD(ch) && dam > 1  )
      dam -= dam * 0.25;
    if ( dam < 0 ) dam = 0;
    /*
     * Check for disarm, trip, parry, and dodge.
     */
    if ( dt >= TYPE_HIT )
    {
      if (IS_NPC(ch) && number_percent( ) < ch->level * 0.5)
        disarm( ch, victim );
      if (IS_NPC(ch) && number_percent( ) < ch->level * 0.5)
        trip( ch, victim );
      if (check_dodge( ch, victim, dt))
        return;
      if ( !IS_NPC(victim) && IS_STANCE(victim, STANCE_MONGOOSE) && 
      victim->stance[STANCE_MONGOOSE] > 100 && !can_counter(ch)
      && !can_bypass(ch,victim) && check_dodge( ch, victim, dt ))
        return;
      else if ( !IS_NPC(victim) && IS_STANCE(victim, STANCE_SWALLOW) && 
      victim->stance[STANCE_SWALLOW] > 100 && !can_counter(ch)
      && !can_bypass(ch,victim) && check_dodge( ch, victim, dt ))
        return;

/* ------------ This is the part for superstances, Jobo ------------------- */
      else if (!IS_NPC(victim) && victim->stance[0] > 12
      && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_DODGE)
      && victim->stance[(victim->stance[0])] > 100 && !can_counter(ch)
      && !can_bypass(ch,victim) && check_dodge( ch, victim, dt ))
        return;
/* ------------ This is the end for superstances, Jobo ------------------- */

      if (check_parry( ch, victim, dt)) return;
      if ( !IS_NPC(victim) && IS_STANCE(victim, STANCE_CRANE) && 
      victim->stance[STANCE_CRANE] > 100 && !can_counter(ch)
      && !can_bypass(ch,victim) && check_parry( ch, victim, dt ))
        return;
      else if (!IS_NPC(victim) && IS_STANCE(victim, STANCE_MANTIS) && 
      victim->stance[STANCE_MANTIS] > 100 && !can_counter(ch)
      && !can_bypass(ch,victim) && check_parry( ch, victim, dt ))
        return;

/* ------------ This is the part for superstances, Jobo ------------------- */
      else if (!IS_NPC(victim) && victim->stance[0] > 12
      && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_PARRY)
      && victim->stance[(victim->stance[0])] > 100 && !can_counter(ch)
      && !can_bypass(ch,victim) && check_parry( ch, victim, dt ))
        return;
/* ------------ This is the end for superstances, Jobo ------------------- */

    }
    dam_message( ch, victim, dam, dt );
  }
  if ( IS_CLASS(victim, CLASS_MONK ) && victim->monkab[SPIRIT] >= 2 )
  {
    if (IS_SET(victim->newbits, NEW_NATURAL))
    {
      stc("You absorb the natural imbalances of your opponent.\n\r",victim);
      act("$n absorbs the natural imbalances of you attack.",victim,NULL,ch,TO_VICT);
      if ( dam > 2000 ) dam /= 2;
      victim->hit += dam;
      REMOVE_BIT(victim->newbits, NEW_NATURAL);
      return;
    }
  }
  if (IS_SET(victim->newbits, NEW_IRONMIND))
  {
    REMOVE_BIT(victim->newbits, NEW_IRONMIND);
    send_to_char("Your focus your full concentration on the attack,\n\r", victim);
    send_to_char("and absorb the full impact into your system, channeling it into a healing force.\n\r",victim);
    victim->hit += dam;
    dam = 0;
  }
  hurt_person(ch,victim,dam);
  dropinvis(ch);
  dropinvis(victim);
  return;
}

void angel_eye( CHAR_DATA *ch, CHAR_DATA *victim, int dam )
{
  char buf[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  int new_dam;

  if (IS_SET(victim->pcdata->powers[ANGEL_POWERS], ANGEL_EYE) && dam > 100)
  {
    new_dam = number_range(dam/4,dam/5);
    if (new_dam > 300) new_dam = number_range(275,325);
    if (new_dam >= victim->hit) new_dam = victim->hit -1;  
    if (new_dam < 10) new_dam = 10;
    hurt_person(victim, ch, new_dam);
    sprintf(buf, "God's justice strikes you HARD [#C%d#n]",new_dam);
    sprintf(buf2, "The sinner dares to lay hands on you, God's wrath on the sinner [#C%d#n]", new_dam);
    act(buf, victim, NULL, ch, TO_VICT);
    act(buf2, victim, NULL, ch, TO_CHAR);
  }
  return;
}

void hurt_person( CHAR_DATA *ch, CHAR_DATA *victim, int dam )
{
  char buf[MAX_STRING_LENGTH];

  victim->hit -= dam;
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_ANGEL) && !IS_CLASS(ch, CLASS_ANGEL))
    angel_eye(ch,victim,dam); // check for that eye for an eye.
  if (!IS_NPC(victim) && victim->level >= LEVEL_IMMORTAL && victim->hit < 1) 
    victim->hit = 1;
  if (!IS_NPC(victim) && IS_SET(victim->newbits, NEW_CLOAK) &&victim->hit < 1 )
  {
    victim->hit += UMIN((victim->max_hit * .1), 4000);
    if (IS_CLASS(victim, CLASS_MONK)) send_to_char("your cloak of life saves your from certain death.\n\r",victim);
    else send_to_char("your cloak of death prevents the destruction of your body.\n\r",victim);
    send_to_char("#0They escape your final blow.#n\n\r",ch);
    REMOVE_BIT(victim->newbits, NEW_CLOAK);
  }
  update_pos( victim );

  switch( victim->position )
  {
    case POS_MORTAL:
    act( "$n is mortally wounded, and spraying blood everywhere.", victim, NULL, NULL, TO_ROOM );
    send_to_char("You are mortally wounded, and spraying blood everywhere.\n\r", victim );
    break;
    case POS_INCAP:
    act( "$n is incapacitated, and bleeding badly.",victim, NULL, NULL, TO_ROOM );
    send_to_char("You are incapacitated, and bleeding badly.\n\r",victim );
    break;
    case POS_STUNNED:
    act( "$n is stunned, but will soon recover.",victim, NULL, NULL, TO_ROOM );
    send_to_char("You are stunned, but will soon recover.\n\r", victim );
    break;
    case POS_DEAD:
    act( "$n is DEAD!!", victim, 0, 0, TO_ROOM );
    send_to_char( "You have been KILLED!!\n\r\n\r", victim );
    break;
    default:
    if ( dam > victim->max_hit / 4 )
      send_to_char( "That really did HURT!\n\r", victim );
    if ( victim->hit < victim->max_hit / 4 && dam > 0 )
    {
      if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE) && number_percent() < victim->beast)
        vamp_rage(victim);
      else
        send_to_char( "You sure are BLEEDING!\n\r", victim );
    }
    break;
  }

  /*
   * Sleep spells and extremely wounded folks.
   */
  if (!IS_AWAKE(victim)) stop_fighting( victim, FALSE );

  /*
   * Payoff for killing things.
   */
  if (victim->hit <= 0 && IS_NPC(victim))
  {
    group_gain( ch, victim );
    victim->position = POS_DEAD;
  }
  
  if ( victim->position == POS_DEAD )
  {
    if (IS_NPC(victim) && !IS_NPC(ch))
    {
      ch->mkill += 1;

      if (IS_CLASS(ch, CLASS_DEMON)
      || IS_CLASS(ch, CLASS_DROW) || IS_CLASS(ch,CLASS_DROID) || IS_CLASS(ch, CLASS_TANARRI))
      {
        if (IS_NPC(victim) && !IS_SET(victim->act, ACT_NOEXP))
        {
          if (IS_CLASS(ch, CLASS_DEMON)) 
          {
            ch->pcdata->stats[DEMON_CURRENT] += victim->level*2;
            ch->pcdata->stats[DEMON_TOTAL] += victim->level*2;
          }
          else
            ch->pcdata->stats[DROW_POWER] += victim->level*2;
          sprintf(buf,"You gain #y(#C%d#y)#n class points.\n\r", victim->level*2);
          if (!IS_SET(ch->act, PLR_BRIEF4)) send_to_char(buf,ch);
        }
      }
      if (ch->level == 1 && ch->mkill > 4)
      {
        ch->level = 2;
        do_save(ch,"");
      }
    }
    if (!IS_NPC(victim) && IS_NPC(ch)) 
    {
      victim->mdeath = victim->mdeath + 1;
    }
    raw_kill( victim );
    if (!IS_NPC(ch) && !IS_NPC(victim) && victim->pcdata->bounty > 0)
    {
      sprintf(buf,"You recive a %d QP bounty, for killing %s.\n\r",
      victim->pcdata->bounty, victim->name);
      send_to_char(buf, ch);
      ch->pcdata->quest += victim->pcdata->bounty;
      victim->pcdata->bounty =0;  
    }
    if (IS_SET(ch->act, PLR_AUTOLOOT) )
      do_get( ch, "all corpse" );
    else
      do_look( ch, "in corpse" );
    if ( !IS_NPC(ch) && IS_NPC(victim) )
    {
      if ( IS_SET(ch->act, PLR_AUTOSAC) )
        do_sacrifice( ch, "corpse" );
    }
    return;
  }
  if ( victim == ch ) return;
  tail_chain( );
  return;
}

bool is_safe( CHAR_DATA *ch, CHAR_DATA *victim )
{
  if (!ch->in_room) return TRUE; // when someone calls a one_hit or similar on something/someone who COULD be dead/gone.
  if (!victim->in_room) return TRUE;
  if (!IS_NPC(victim) && !IS_NPC(ch))
  {
    if ( !CAN_PK(ch) || !CAN_PK(victim) )
    {
      send_to_char( "Both players must be avatars to fight.\n\r", ch );
      return TRUE;
    }  
  }
  if (!IS_NPC(ch) && !IS_NPC(victim))
  {
    if(ch->pcdata->safe_counter > 0)
    {
      send_to_char("Your are safe for 10 ticks after training avatar.\n\r",ch);
      return TRUE;
    }
    if(victim->pcdata->safe_counter > 0)
    {
      send_to_char("They are safe for 10 ticks after training avatar.\n\r",ch);
      return TRUE;
    }
    if (victim->desc)
    {
      if (victim->desc->connected >= CON_NOTE_TO && victim->desc->connected <= CON_NOTE_FINISH)
      {
        send_to_char("That player is writing a note, be nice.\n\r",ch);
        return TRUE;
      }
    }
  }
  if (IS_AFFECTED(ch, AFF_ETHEREAL))
  {
    send_to_char( "You cannot while ethereal.\n\r", ch );
    return TRUE;
  }
  if (IS_AFFECTED(victim, AFF_ETHEREAL))
  {
    send_to_char( "You cannot fight an ethereal person.\n\r", ch );
    return TRUE;
  }
  if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
  {
    act( "You are too insubstantial!", ch, NULL, victim, TO_CHAR );
    return TRUE;
  }
  if (IS_AFFECTED(victim, AFF_SHADOWPLANE))
  {
    act( "$E is too insubstantial!", ch, NULL, victim, TO_CHAR );
    return TRUE;
  }
  if ( IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH) )
  {
    send_to_char( "Objects cannot fight!\n\r", ch );
    return TRUE;
  }
  if ( IS_HEAD(victim,LOST_HEAD) || IS_EXTRA(victim,EXTRA_OSWITCH))
  {
    send_to_char( "You cannot attack an object.\n\r", ch );
    return TRUE;
  }
  if (IS_SET(victim->extra, EXTRA_AFK))
  {
    send_to_char("They are AFK!\n\r",ch);
    return TRUE;
  }
  if (!IS_NPC(victim) && victim->desc == NULL && victim->timer > 1 && victim->fight_timer == 0)
  {
// Timer check to avoid people going ld in the first round.
    send_to_char("Nooo, they are linkdead.\n\r",ch);
    return TRUE;
  }
  if (ragnarok && is_upgrade(ch) && !is_upgrade(victim) && !IS_NPC(ch) && !IS_NPC(victim))
  {
    send_to_char("You cannot attack non-upgrades doing ragnarok.\n\r",ch);
    return TRUE;
  }
  if (ragnarok && !is_upgrade(ch) && is_upgrade(victim) && !IS_NPC(ch) && !IS_NPC(victim))
  {
    send_to_char("You cannot attack upgrades doing ragnarok.\n\r",ch);
    return TRUE;
  }
  if (victim->fight_timer > 0) return FALSE;
  if ( !IS_NPC(ch ) )
    if (IS_AFFECTED(ch, AFF_PEACE) && ch->fighting != NULL) REMOVE_BIT(ch->affected_by, AFF_PEACE);
  if ( !IS_NPC(victim) )
    if (IS_AFFECTED(victim, AFF_PEACE) && victim->fighting != NULL) REMOVE_BIT(victim->affected_by, AFF_PEACE);
  if ( IS_SET(ch->in_room->room_flags,ROOM_SAFE) && !ragnarok)
  {
    send_to_char( "You cannot fight in a safe room.\n\r", ch );
    return TRUE;
  }
  if (IS_NPC(ch) || IS_NPC(victim)) return FALSE;

  if (victim->fighting == ch) return FALSE;
  if (IS_AFFECTED(ch, AFF_PEACE))
  {
    send_to_char( "You are unable to attack them.\n\r", ch );
    return TRUE;
  }
  if (IS_AFFECTED(victim, AFF_PEACE))
  {
    send_to_char( "You can't seem to attack them.\n\r", ch );
    return TRUE;
  }
  if (IS_ITEMAFF(ch, ITEMA_PEACE))
  {
    send_to_char( "You are unable to attack them.\n\r", ch );
    return TRUE;
  }
  if (IS_ITEMAFF(victim, ITEMA_PEACE))
  {
    send_to_char( "You can't seem to attack them.\n\r", ch );
    return TRUE;
  }
  return FALSE;
}

/*
 * Check for parry.
 */
bool check_parry( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
  OBJ_DATA *obj = NULL;
  int chance = 0;
  bool claws = FALSE;

  if (!IS_AWAKE(victim)) return FALSE;
  if (IS_NPC(victim))  obj = NULL;
  else if (IS_CLASS(victim, CLASS_WEREWOLF) && victim->power[DISC_WERE_BEAR] > 2 
  && IS_VAMPAFF(victim, VAM_CLAWS) && get_eq_char(victim, WEAR_WIELD) == NULL 
  && get_eq_char(victim, WEAR_HOLD) == NULL)
  {
    obj = NULL;
    claws = TRUE;
  }
  else if ( IS_CLASS(victim, CLASS_MONK) && IS_SET(victim->newbits, NEW_MONKADAM) 
  && get_eq_char(victim, WEAR_WIELD) == NULL && get_eq_char(victim, WEAR_HOLD) == NULL)
  {
    obj = NULL;
    claws = TRUE;
  }
  else if( IS_CLASS(victim, CLASS_SHAPESHIFTER) && get_eq_char(victim, WEAR_WIELD) == NULL 
  && get_eq_char(victim, WEAR_HOLD) == NULL)
  {
    obj = NULL;
    claws = TRUE;
  }
  else
  {
    if ((obj = get_eq_char(victim, WEAR_WIELD)) == NULL || obj->item_type != ITEM_WEAPON)
    {
      if ((obj = get_eq_char(victim, WEAR_HOLD)) == NULL || obj->item_type != ITEM_WEAPON) 
        return FALSE;
    }
  }
  if ( ( dt < 1000 || dt > 1012) && !IS_CLASS(ch, CLASS_MONK) ) return FALSE;
  if (  dt >= 1000 && dt <= 1012 )
  {
    if (!IS_NPC(ch)) chance -= (ch->wpn[dt-1000] * 0.1);
    else chance -= (ch->level * 0.2);
    if (!IS_NPC(victim)) chance += (victim->wpn[dt-1000] * 0.5);
    else chance += victim->level;
  }
  else
  {
    if (!IS_NPC(ch)) chance -= (ch->wpn[0] * 0.1);
    else chance -= (ch->level * 0.2);
    if (!IS_NPC(victim)) chance += (victim->wpn[0] * 0.5);
    else chance += victim->level;
  }
  if (!IS_NPC(victim) && IS_STANCE(victim, STANCE_CRANE) && victim->stance[STANCE_CRANE] > 0 
  && !can_counter(ch) && !can_bypass(ch,victim))
    chance += (victim->stance[STANCE_CRANE] * 0.25);
  else if (!IS_NPC(victim) && IS_STANCE(victim, STANCE_MANTIS) && victim->stance[STANCE_MANTIS] > 0 
  && !can_counter(ch) && !can_bypass(ch,victim))  
    chance += (victim->stance[STANCE_MANTIS] * 0.25);

/* ------------ This is the part for superstances, Jobo ------------------- */
  if (!IS_NPC(victim) && victim->stance[0] > 12 
  && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_PARRY)
  && victim->stance[(victim->stance[0])] > 100 && !can_counter(ch) && !can_bypass(ch,victim))
    chance += victim->stance[(victim->stance[0])] * 0.25;
/* ------------ This is the end for superstances, Jobo ------------------- */
  
  chance -= (char_hitroll(ch) * 0.1);
  if (claws) 
  {
     if (victim->power[DISC_WERE_LYNX] > 3) chance += (char_hitroll(victim) * 0.1);
     else chance += (char_hitroll(victim) * 0.075);
  }
  if (!IS_NPC(ch))
  {
    if (IS_CLASS(ch,CLASS_DROID)) chance -= ch->pcdata->powers[CYBORG_LIMBS]*10;
    if (ch->pcdata->upgrade_level > 0) chance -= ch->pcdata->upgrade_level*10;
    if (IS_CLASS(ch, CLASS_LICH)) chance -= 40;
    if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
    {
      if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM) chance -= ch->pcdata->powers[TIGER_LEVEL]*11;
      else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM) chance -= ch->pcdata->powers[BULL_LEVEL]*7.5;
      else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM) chance -= ch->pcdata->powers[HYDRA_LEVEL]*7;
      else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM) chance -= ch->pcdata->powers[FAERIE_LEVEL]*6;
      else chance -= 10;
    }
    if (IS_CLASS(ch, CLASS_TANARRI))
    {
      if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) chance -= 15;
      if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_SPEED)) chance -= 17;
      if (ch->pcdata->powers[TANARRI_FURY_ON] == 1) chance -= 17;
    }
    if (IS_CLASS(ch, CLASS_ANGEL)) chance -= ch->pcdata->powers[ANGEL_JUSTICE]*9;
    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_CELE] > 0) chance -= (ch->power[DISC_VAMP_CELE]*3);
    if (IS_CLASS(ch, CLASS_NINJA)  && ch->pcdata->powers[NPOWER_CHIKYU] >= 6 && ch->pcdata->powers[HARA_KIRI] > 0) chance -= 25;
    if (ch->pcdata->powers[NPOWER_CHIKYU] >= 5 && IS_CLASS(ch, CLASS_NINJA)) chance -= 20;
    if (IS_CLASS(ch, CLASS_DEMON) && IS_DEMPOWER(ch, DEM_SPEED)) chance -= 25;
    if (IS_CLASS(ch, CLASS_MONK) && ch->chi[CURRENT] > 0) chance -= (ch->chi[CURRENT] * 7);
    if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT)) chance -= ch->pcdata->powers[WEAPONSKILL]*3.5;
    if (IS_CLASS(ch, CLASS_MAGE) && IS_ITEMAFF(ch, ITEMA_BEAST)) chance -= 30;
    if (IS_CLASS(ch, CLASS_DROW) && IS_SET(ch->pcdata->powers[1], DPOWER_ARMS)) chance -= 30;
    if (IS_SET(ch->newbits, NEW_MONKFLAME)) chance -= 3;
    if (IS_CLASS(ch, CLASS_WEREWOLF)) chance -= (ch->power[DISC_WERE_MANT] * 3);
    if (IS_CLASS(ch, CLASS_SAMURAI)) chance -= 25;
    if (IS_ITEMAFF(ch, ITEMA_AFFMANTIS)) chance -= 12;
  }
  if (!IS_NPC(victim))
  {
    if (IS_CLASS(victim,CLASS_DROID)) chance += victim->pcdata->powers[CYBORG_LIMBS]*10;
    if (victim->pcdata->upgrade_level > 0) chance += victim->pcdata->upgrade_level*10;
    if (IS_CLASS(victim, CLASS_LICH)) chance += 40;
    if (IS_CLASS(victim, CLASS_SHAPESHIFTER))
    {
      if (victim->pcdata->powers[SHAPE_FORM] == TIGER_FORM) chance += victim->pcdata->powers[TIGER_LEVEL]*6;
      else if (victim->pcdata->powers[SHAPE_FORM] == BULL_FORM) chance += victim->pcdata->powers[BULL_LEVEL]*7;
      else if (victim->pcdata->powers[SHAPE_FORM] == HYDRA_FORM) chance += victim->pcdata->powers[HYDRA_LEVEL]*6;
      else if (victim->pcdata->powers[SHAPE_FORM] == FAERIE_FORM) chance += victim->pcdata->powers[FAERIE_LEVEL]*11;
      else chance += 10;
    }
    if (IS_CLASS(victim, CLASS_TANARRI))
    {
      if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) chance += 15;
      if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_SPEED)) chance += 17;
      if (victim->pcdata->powers[TANARRI_FURY_ON] == 1) chance += 12;
    }
    if (IS_CLASS(victim, CLASS_ANGEL)) chance += victim->pcdata->powers[ANGEL_PEACE]*9;
    if (victim->pcdata->powers[NPOWER_CHIKYU] >= 5 && IS_CLASS(victim, CLASS_NINJA)) chance += 20;
    if (IS_CLASS(victim, CLASS_NINJA) && victim->pcdata->powers[NPOWER_CHIKYU] >= 6 && victim->pcdata->powers[HARA_KIRI] > 0) chance += 25;
    if (victim->power[DISC_VAMP_CELE] > 0 &&  IS_CLASS(victim, CLASS_VAMPIRE)) chance += (victim->power[DISC_VAMP_CELE]*3);
    if (IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1], DPOWER_ARMS)) chance += 30;
    if (IS_CLASS(victim, CLASS_MONK) && ch->chi[CURRENT] > 0 ) chance += (ch->chi[CURRENT] * 8);
    if (IS_SET(victim->newbits, NEW_MONKFLAME)) chance += 4;
    if (IS_CLASS(victim, CLASS_SAMURAI)) chance +=25;
    if (IS_CLASS(victim, CLASS_UNDEAD_KNIGHT)) chance += victim->pcdata->powers[WEAPONSKILL]*4;
    if (IS_CLASS(victim, CLASS_MAGE) && IS_ITEMAFF(victim, ITEMA_DEFLECTOR)) chance += 40;
    if (IS_CLASS(victim, CLASS_DEMON) && IS_DEMPOWER(victim, DEM_SPEED)) chance += 25;
  }
  if (chance > 80) chance = 80;
  else if (chance < 20) chance = 20;
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_LICH)) chance -= 15;
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_LICH)) chance += 15;
  if (!IS_NPC(ch) && ch->power[DISC_VAMP_CELE] > 0 && IS_CLASS(ch, CLASS_VAMPIRE))
  {
    if( ch->pcdata->rank == AGE_TRUEBLOOD)  chance -= 20;
    else if( ch->pcdata->rank == AGE_LA_MAGRA) chance -= 17;
    else if (ch->pcdata->rank == AGE_METHUSELAH) chance -= 15;
    else if (ch->pcdata->rank == AGE_ELDER) chance -= 10;
    else if (ch->pcdata->rank == AGE_ANCILLA) chance -= 5;    
  }
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SHAPESHIFTER))
  {
    if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM) chance -= ch->pcdata->powers[TIGER_LEVEL]*4;
    else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM) chance -= ch->pcdata->powers[BULL_LEVEL]*3;
    else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM) chance -= ch->pcdata->powers[HYDRA_LEVEL]*3;
    else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM) chance -= ch->pcdata->powers[FAERIE_LEVEL]*2;
    else chance -= 2;
  }
  if (IS_CLASS(ch, CLASS_TANARRI) && !IS_NPC(ch))
  {
    if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) chance -= 5;
    if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_SPEED)) chance -= 6;
    if (ch->pcdata->powers[TANARRI_FURY_ON] == 1) chance -= 5;
  }
  if (IS_CLASS(victim, CLASS_TANARRI) && !IS_NPC(victim))
  {
    if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) chance += 5;
    if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_SPEED)) chance += 6;
    if (victim->pcdata->powers[TANARRI_FURY_ON] == 1) chance += 5;
  }
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ANGEL)) chance -= ch->pcdata->powers[ANGEL_JUSTICE]*3;
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_ANGEL)) chance += victim->pcdata->powers[ANGEL_PEACE]*3;
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_SHAPESHIFTER))
  {
    if (victim->pcdata->powers[SHAPE_FORM] == TIGER_FORM) chance += victim->pcdata->powers[TIGER_LEVEL]*2;
    else if (victim->pcdata->powers[SHAPE_FORM] == BULL_FORM) chance += victim->pcdata->powers[BULL_LEVEL]*3;
    else if (victim->pcdata->powers[SHAPE_FORM] == HYDRA_FORM) chance += victim->pcdata->powers[HYDRA_LEVEL]*3;
    else if (victim->pcdata->powers[SHAPE_FORM] == FAERIE_FORM) chance += victim->pcdata->powers[FAERIE_LEVEL]*6;
    else chance += 2;
  }
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_MAGE) && IS_ITEMAFF(ch, ITEMA_BEAST)) chance -= 10;
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_UNDEAD_KNIGHT)) chance -= ch->pcdata->powers[WEAPONSKILL];
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_UNDEAD_KNIGHT)) chance += victim->pcdata->powers[WEAPONSKILL]*1.5;
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA))                                   
  {
    if( ch->pcdata->rank == BELT_TEN)  chance -= 20;
    else if( ch->pcdata->rank == BELT_NINE) chance -= 18;   
    else if (ch->pcdata->rank == BELT_EIGHT) chance -= 16;    
    else if (ch->pcdata->rank == BELT_SEVEN) chance -= 13;
    else if (ch->pcdata->rank == BELT_SIX) chance -= 10;  
    else if (ch->pcdata->rank == BELT_FIVE) chance -= 7;
    else if (ch->pcdata->rank == BELT_FOUR) chance -= 4;
    else if (ch->pcdata->rank == BELT_THREE) chance -= 2;
  }
  if(!IS_NPC(ch) && IS_CLASS(ch, CLASS_DEMON)) chance -= (ch->power[DISC_DAEM_ATTA]);
  if(!IS_NPC(victim) && IS_CLASS(victim, CLASS_DEMON)) chance += (victim->power[DISC_DAEM_ATTA]);
  if (!IS_NPC(victim) && victim->power[DISC_VAMP_CELE] > 0 && IS_CLASS(victim, CLASS_VAMPIRE))
  {
    if( victim->pcdata->rank == AGE_TRUEBLOOD)  chance += 20;
    else if( victim->pcdata->rank == AGE_LA_MAGRA) chance += 17;
    else if (victim->pcdata->rank == AGE_METHUSELAH) chance += 15;
    else if (victim->pcdata->rank == AGE_ELDER) chance += 10;
    else if (victim->pcdata->rank == AGE_ANCILLA) chance += 5;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_NINJA))
  {
    if( victim->pcdata->rank == BELT_TEN)  chance += 20;
    else if( victim->pcdata->rank == BELT_NINE) chance += 18;
    else if (victim->pcdata->rank == BELT_EIGHT) chance += 16;
    else if (victim->pcdata->rank == BELT_SEVEN) chance += 13;
    else if (victim->pcdata->rank == BELT_SIX) chance += 12;
    else if (victim->pcdata->rank == BELT_FIVE) chance += 10;
    else if (victim->pcdata->rank == BELT_FOUR) chance += 9;
    else if (victim->pcdata->rank == BELT_THREE) chance += 6;
    else if (victim->pcdata->rank == BELT_TWO) chance += 4;
    else if (victim->pcdata->rank == BELT_ONE) chance += 2;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_SAMURAI)) chance += 15;
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SAMURAI)) chance -=15;
  if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_DROID)) chance -= ch->pcdata->powers[CYBORG_LIMBS]*4;
  if (!IS_NPC(victim) && IS_CLASS(victim,CLASS_DROID)) chance += victim->pcdata->powers[CYBORG_LIMBS]*4;
  if (victim->monkblock / 4 > 0 ) chance += victim->monkblock / 4;
  if (number_percent( ) < 100 && number_percent( ) >= chance)
    return FALSE;
  if (!IS_NPC(victim))                                          
  {
    if (IS_SET(victim->pcdata->jflags, JFLAG_BULLY) && number_range(1,3) != 2) return FALSE;
  }
  if (!IS_NPC(ch) && IS_NPC(victim)
  && IS_SET(ch->newbits, NEW_FIGHTDANCE)
  && IS_CLASS(ch, CLASS_DROW) 
  && number_percent() >=50
  && ((( obj = get_eq_char(ch, WEAR_WIELD ))  != NULL && ( obj->value[3] == 4 || obj->value[3] == 11))
      || ((obj = get_eq_char (ch, WEAR_HOLD))!= NULL &&( obj->value[3] == 4 || obj->value[3] == 11))))
  {
    act( "$n's dark weapon dances around your defences, making a strange sound.",  ch, NULL, victim, TO_VICT );
    act( "Your sacrificial weapon hums in the air as it dances $N's defences.", ch, obj, victim, TO_CHAR );
    return FALSE;
  }
  if (claws)
  {
    if (IS_NPC(victim) || !IS_SET(victim->act, PLR_BRIEF3))
    {
      if (IS_CLASS(victim, CLASS_WEREWOLF))
        act( "You parry $n's blow with your claws.",  ch, NULL, victim, TO_VICT );
      else
        act( "You parry $n's blow with your Adamantium hands.", ch,NULL,victim, TO_VICT);
    }
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_MONK) && victim->monkblock < 150 && number_range(1,2)==1)
      victim->monkblock++;
    if (IS_NPC(ch) || !IS_SET(ch->act, PLR_BRIEF3))
    {
    if (IS_CLASS(victim, CLASS_WEREWOLF))
      act( "$N parries your blow with $S claws.", ch, NULL, victim, TO_CHAR );
    else
      act( "$N parries your blow with $S Adamantium claws.", ch,NULL,victim, TO_CHAR);
    }
    return TRUE;
  }
  if (!IS_NPC(victim) && obj != NULL && obj->item_type == ITEM_WEAPON && obj->value[3] >= 0 && obj->value[3] <= 12)
  {
    if (IS_NPC(victim) || !IS_SET(victim->act, PLR_BRIEF3))
      act( "You parry $n's blow with $p.",  ch, obj, victim, TO_VICT );
    if (IS_NPC(ch) || !IS_SET(ch->act, PLR_BRIEF3))
      act( "$N parries your blow with $p.", ch, obj, victim, TO_CHAR );
    return TRUE;
  }
  if (IS_NPC(victim) || !IS_SET(victim->act, PLR_BRIEF3))
    act( "You parry $n's attack.",  ch, NULL, victim, TO_VICT    );
  if (IS_NPC(ch) || !IS_SET(ch->act, PLR_BRIEF3))
    act( "$N parries your attack.", ch, NULL, victim, TO_CHAR    );
  return TRUE;
}

/*
 * Check for dodge.
 */
bool check_dodge( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
  int chance = 0;

  if (!IS_AWAKE(victim)) return FALSE;
  if (!IS_NPC(ch)) chance -= (ch->wpn[dt-1000] * 0.1);
  else chance -= (ch->level * 0.2);
  if (!IS_NPC(victim)) chance += (victim->wpn[0] * 0.5);
  else chance += victim->level;
  if (!IS_NPC(victim) && IS_STANCE(victim, STANCE_MONGOOSE) && victim->stance[STANCE_MONGOOSE] > 0 
  && !can_counter(ch) &&!can_bypass(ch,victim))
    chance += victim->stance[STANCE_MONGOOSE] * 0.25;
  if (!IS_NPC(victim) && IS_STANCE(victim, STANCE_SWALLOW) && victim->stance[STANCE_SWALLOW] > 0 
  && !can_counter(ch) && !can_bypass(ch,victim))
    chance += victim->stance[STANCE_SWALLOW] * 0.25;

/* ------------ This is the part for superstances, Jobo ------------------- */
  if (!IS_NPC(victim) && victim->stance[0] > 12 
  && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_DODGE)
  && victim->stance[(victim->stance[0])] > 100 && !can_counter(ch) && !can_bypass(ch,victim))
    chance += victim->stance[(victim->stance[0])] * 0.25;
/* ------------ This is the end for superstances, Jobo ------------------- */

  if (!IS_NPC(ch))
  {
    if (ch->pcdata->upgrade_level > 0) chance -= ch->pcdata->upgrade_level*10;
    if (IS_CLASS(ch,CLASS_DROID)) chance -= ch->pcdata->powers[CYBORG_LIMBS] * 10;
    if (IS_CLASS(ch, CLASS_LICH)) chance -= 40;
    if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
    {
      if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM) chance -= ch->pcdata->powers[TIGER_LEVEL]*9;
      else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM) chance -= ch->pcdata->powers[BULL_LEVEL]*7.5;
      else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM) chance -= ch->pcdata->powers[HYDRA_LEVEL]*7;
      else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM) chance -= ch->pcdata->powers[FAERIE_LEVEL]*6;
      else chance -= 10;
    }
    if (IS_CLASS(ch, CLASS_TANARRI))
    {
      if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) chance -= 15;
      if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_SPEED)) chance -= 17;
      if (ch->pcdata->powers[TANARRI_FURY_ON] == 1) chance -= 17;
    }
    if (IS_CLASS(ch, CLASS_ANGEL)) chance -= ch->pcdata->powers[ANGEL_JUSTICE]*9;
    if (IS_CLASS(ch, CLASS_NINJA)  && ch->pcdata->powers[NPOWER_CHIKYU] >= 6 && ch->pcdata->powers[HARA_KIRI] > 0) chance -= 25;
    if (IS_CLASS(ch, CLASS_MONK)) chance -= ch->chi[CURRENT]*3;
    if (ch->pcdata->powers[NPOWER_CHIKYU] >= 5 && IS_CLASS(ch, CLASS_NINJA)) chance -= 20;
    if (ch->power[DISC_VAMP_CELE] > 0 && IS_CLASS(ch, CLASS_VAMPIRE)) chance -= (ch->power[DISC_VAMP_CELE]*4);
    if (IS_CLASS(ch, CLASS_DEMON) && IS_DEMPOWER( ch, DEM_SPEED)) chance -= 25;
    if (IS_CLASS(ch, CLASS_MAGE) && IS_ITEMAFF(ch, ITEMA_BEAST)) chance -= 30;
    if (IS_CLASS(ch, CLASS_WEREWOLF) && ch->power[DISC_WERE_MANT] > 6) chance -= (ch->power[DISC_WERE_MANT] * 3);
    if (!IS_CLASS(ch, CLASS_WEREWOLF) && (IS_ITEMAFF(ch,ITEMA_AFFMANTIS))) chance -= 12;
    if (IS_CLASS(ch, CLASS_DROW) && IS_SET(ch->pcdata->powers[1], DPOWER_SPEED)) chance -= 50;
    if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT)) chance -= ch->pcdata->powers[WEAPONSKILL]*4;
    if (IS_CLASS(ch, CLASS_SAMURAI)) chance -= 25;
  }
  if (!IS_NPC(victim))
  {
    if (IS_CLASS(victim,CLASS_DROID)) chance += victim->pcdata->powers[CYBORG_LIMBS]*10;
    if (victim->pcdata->upgrade_level > 0) chance += victim->pcdata->upgrade_level*10;
    if (IS_CLASS(victim, CLASS_LICH)) chance += 40;
    if (victim->pcdata->powers[NPOWER_CHIKYU] >= 5 && IS_CLASS(victim, CLASS_NINJA)) chance += 30;
    if (IS_CLASS(victim, CLASS_NINJA) && victim->pcdata->powers[NPOWER_CHIKYU] >= 6 && victim->pcdata->powers[HARA_KIRI] > 0) chance += 20;
    if (IS_CLASS(victim, CLASS_SHAPESHIFTER))
    {
      if (victim->pcdata->powers[SHAPE_FORM] == TIGER_FORM) chance += victim->pcdata->powers[TIGER_LEVEL]*6;
      else if (victim->pcdata->powers[SHAPE_FORM] == BULL_FORM) chance += victim->pcdata->powers[BULL_LEVEL]*7;
      else if (victim->pcdata->powers[SHAPE_FORM] == HYDRA_FORM) chance += victim->pcdata->powers[HYDRA_LEVEL]*6;
      else if (victim->pcdata->powers[SHAPE_FORM] == FAERIE_FORM) chance += victim->pcdata->powers[FAERIE_LEVEL]*11;
      else chance += 10;
    }
    if (IS_CLASS(victim, CLASS_MONK)) chance += victim->chi[CURRENT]*3;
    if (IS_CLASS(victim, CLASS_TANARRI))
    {
      if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) chance += 15;
      if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_SPEED)) chance += 17;
      if (victim->pcdata->powers[TANARRI_FURY_ON] == 1) chance += 12;
    }
    if (IS_CLASS(victim, CLASS_ANGEL)) chance += victim->pcdata->powers[ANGEL_PEACE]*9;
    if (victim->power[DISC_VAMP_CELE] > 0 && IS_CLASS(victim, CLASS_VAMPIRE)) chance += (victim->power[DISC_VAMP_CELE]*3);
    if (IS_CLASS(victim, CLASS_DEMON) && IS_DEMPOWER( victim, DEM_SPEED)) chance += 25;
    if (IS_CLASS(victim, CLASS_WEREWOLF) && victim->power[DISC_WERE_MANT] > 6) chance +=(victim->power[DISC_WERE_MANT] * 3.5);
    if (IS_CLASS(victim, CLASS_DROW) &&  IS_SET(victim->pcdata->powers[1], DPOWER_SPEED)) chance += 50;
    if (IS_CLASS(victim, CLASS_SAMURAI)) chance +=25;
    if (IS_CLASS(victim, CLASS_UNDEAD_KNIGHT)) chance += victim->pcdata->powers[WEAPONSKILL]*4.5;
    if (!IS_CLASS(victim, CLASS_WEREWOLF) && IS_ITEMAFF(victim,ITEMA_AFFMANTIS)) chance += 15;
    if (IS_CLASS(victim, CLASS_MAGE) && IS_ITEMAFF(victim, ITEMA_DEFLECTOR)) chance += 40;
  }
  if (chance > 80) chance = 80;
  else if (chance < 20) chance = 20;
  if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_DROID)) chance -= ch->pcdata->powers[CYBORG_LIMBS]*4;
  if (!IS_NPC(victim) && IS_CLASS(victim,CLASS_DROID)) chance += victim->pcdata->powers[CYBORG_LIMBS]*4;
  if (IS_CLASS(ch, CLASS_LICH) && !IS_NPC(ch)) chance -= 15;
  if (IS_CLASS(victim, CLASS_LICH) && !IS_NPC(victim)) chance += 15;
  if (IS_CLASS(ch, CLASS_SHAPESHIFTER) && !IS_NPC(ch))
  {
    if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM) chance -= ch->pcdata->powers[TIGER_LEVEL]*4;
    else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM) chance -= ch->pcdata->powers[BULL_LEVEL]*3;
    else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM) chance -= ch->pcdata->powers[HYDRA_LEVEL]*3;
    else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM) chance -= ch->pcdata->powers[FAERIE_LEVEL]*2;
    else chance -= 2;
  }
  if (IS_CLASS(victim, CLASS_SHAPESHIFTER) && !IS_NPC(victim))
  {
    if (victim->pcdata->powers[SHAPE_FORM] == TIGER_FORM) chance += victim->pcdata->powers[TIGER_LEVEL]*2;
    else if (victim->pcdata->powers[SHAPE_FORM] == BULL_FORM) chance += victim->pcdata->powers[BULL_LEVEL]*3;  
    else if (victim->pcdata->powers[SHAPE_FORM] == HYDRA_FORM) chance += victim->pcdata->powers[HYDRA_LEVEL]*3;
    else if (victim->pcdata->powers[SHAPE_FORM] == FAERIE_FORM) chance += victim->pcdata->powers[FAERIE_LEVEL]*6;
    else chance += 2;
  }
  if (IS_CLASS(ch, CLASS_TANARRI) && !IS_NPC(ch))
  {  
    if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) chance -= 5;
    if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_SPEED)) chance -= 6;
    if (ch->pcdata->powers[TANARRI_FURY_ON] == 1) chance -= 5;
  }
  if (IS_CLASS(victim, CLASS_TANARRI) && !IS_NPC(victim))
  {
    if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) chance += 5;
    if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_SPEED)) chance += 6;
    if (victim->pcdata->powers[TANARRI_FURY_ON] == 1) chance += 5;
  }
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ANGEL)) chance -= ch->pcdata->powers[ANGEL_JUSTICE]*3;
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_ANGEL)) chance += victim->pcdata->powers[ANGEL_PEACE]*3;
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1], DPOWER_SPEED)) chance += 20;
  if (IS_CLASS(victim, CLASS_DEMON) && IS_DEMPOWER( victim, DEM_SPEED)) chance += 25;
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_UNDEAD_KNIGHT)) chance -= ch->pcdata->powers[WEAPONSKILL];
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_UNDEAD_KNIGHT)) chance += victim->pcdata->powers[WEAPONSKILL]*2;
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SAMURAI)) chance -= 15;
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_SAMURAI)) chance +=15;
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_MAGE) && IS_ITEMAFF(ch, ITEMA_BEAST)) chance -= 10;
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_MAGE) && IS_ITEMAFF(victim, ITEMA_BEAST)) chance += 10;
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF) && ch->power[DISC_WERE_MANT] > 6) chance -= (ch->power[DISC_WERE_MANT]);
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_WEREWOLF) && victim->power[DISC_WERE_MANT] > 6) chance += (victim->power[DISC_WERE_MANT]);
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE))
  {
    if( ch->pcdata->rank == AGE_TRUEBLOOD)  chance -= 20;
    else if( ch->pcdata->rank == AGE_LA_MAGRA) chance -= 17;
    else if (ch->pcdata->rank == AGE_METHUSELAH) chance -= 10;
    else if (ch->pcdata->rank == AGE_ELDER) chance -= 7;
    else if (ch->pcdata->rank == AGE_ANCILLA) chance -= 3;
  }
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA))
  {
    if( ch->pcdata->rank == BELT_TEN)  chance -= 20;
    else if( ch->pcdata->rank == BELT_NINE) chance -= 18;
    else if (ch->pcdata->rank == BELT_EIGHT) chance -= 17;
    else if (ch->pcdata->rank == BELT_SEVEN) chance -= 14;
    else if (ch->pcdata->rank == BELT_SIX) chance -= 12;
    else if (ch->pcdata->rank == BELT_FIVE) chance -= 10;
    else if (ch->pcdata->rank == BELT_FOUR) chance -= 9;
    else if (ch->pcdata->rank == BELT_THREE) chance -= 7;
    else if (ch->pcdata->rank == BELT_TWO) chance -= 4;
    else if (ch->pcdata->rank == BELT_ONE) chance -= 2;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE))
  {
    if(victim->pcdata->rank == AGE_TRUEBLOOD)  chance += 20;
    else if(victim->pcdata->rank == AGE_LA_MAGRA) chance += 17;
    else if (victim->pcdata->rank == AGE_METHUSELAH) chance += 10;
    else if (victim->pcdata->rank == AGE_ELDER) chance += 7;
    else if (victim->pcdata->rank == AGE_ANCILLA) chance += 3;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_NINJA))
  {
    if( victim->pcdata->rank == BELT_TEN)  chance += 20;
    else if( victim->pcdata->rank == BELT_NINE) chance += 18;
    else if (victim->pcdata->rank == BELT_EIGHT) chance += 17;
    else if (victim->pcdata->rank == BELT_SEVEN) chance += 14;
    else if (victim->pcdata->rank == BELT_SIX) chance += 11;
    else if (victim->pcdata->rank == BELT_FIVE) chance += 9;
    else if (victim->pcdata->rank == BELT_FOUR) chance += 7;
    else if (victim->pcdata->rank == BELT_THREE) chance += 5;
    else if (victim->pcdata->rank == BELT_TWO) chance += 4;
    else if (victim->pcdata->rank == BELT_ONE) chance += 2;
  }    
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_MONK) && ( victim->monkblock / 2 > 0 )) chance += (victim->monkblock / 2);
  if ( chance > 90) chance = 90;
  if (!IS_NPC(victim))
  {
    if (IS_SET(victim->pcdata->jflags, JFLAG_BULLY) && number_range(1,3) != 2) return FALSE;
  }
  if ( number_percent( ) >= chance ) 
    return FALSE;
  if (IS_NPC(victim) || !IS_SET(victim->act, PLR_BRIEF3))
    act( "You dodge $n's attack.", ch, NULL, victim, TO_VICT );
  if (IS_NPC(ch) || !IS_SET(ch->act, PLR_BRIEF3))
    act( "$N dodges your attack.", ch, NULL, victim, TO_CHAR );
  return TRUE;
}

/*
 * Set position of a victim.
 */
void update_pos( CHAR_DATA *victim )
{
  CHAR_DATA *mount;
  if (victim->hit > 0)
  {
    if (victim->position <= POS_STUNNED)
    {
      bool gm_stance = FALSE;
      victim->position = POS_STANDING;
      if (!IS_NPC(victim) && victim->stance[0] > 0)
      {
        int stance = victim->stance[0];
        if (victim->stance[stance] >= 200) gm_stance = TRUE;
      }
      if (IS_NPC(victim) || victim->max_hit * 0.25 > victim->hit)
      {
        act( "$n clambers back to $s feet.", victim, NULL, NULL, TO_ROOM);
        act( "You clamber back to your feet.", victim, NULL, NULL, TO_CHAR);
      }
      else
      {
        act( "$n flips back up to $s feet.", victim, NULL, NULL, TO_ROOM);
        act( "You flip back up to your feet.", victim, NULL, NULL, TO_CHAR);
      }
    }
    return;
  }
  else if ((mount = victim->mount) != NULL)
  {
    if (victim->mounted == IS_MOUNT)
    {
      act("$n rolls off $N.",mount,NULL,victim,TO_ROOM);
      act("You roll off $N.",mount,NULL,victim,TO_CHAR);
    }
    else if (victim->mounted == IS_RIDING)
    {
      act("$n falls off $N.",victim,NULL,mount,TO_ROOM);
      act("You fall off $N.",victim,NULL,mount,TO_CHAR);
    }
    mount->mount    = NULL;
    victim->mount   = NULL;
    mount->mounted  = IS_ON_FOOT;
    victim->mounted = IS_ON_FOOT;
  }
  if (!IS_NPC(victim) && victim->hit <=-11 && IS_HERO(victim))
  {
    victim->hit = -10;
    if (victim->position == POS_FIGHTING) stop_fighting(victim,TRUE);
    return;
  }
  if (!IS_NPC(victim) && victim->hit <-9 && !IS_HERO(victim))
  {
    victim->hit = 1;
    do_killperson(victim,victim->name);
    victim->mdeath=victim->mdeath+1;
  }
  if(IS_NPC(victim) && victim->hit < -6 )
  {
    victim->position = POS_DEAD;
    return;
  }
  if (victim->hit <= -6 ) victim->position = POS_MORTAL;
  else if ( victim->hit <= -3 ) victim->position = POS_INCAP;
  else victim->position = POS_STUNNED;
  return;
}    

/*
 * Start fights.
 */
void set_fighting( CHAR_DATA *ch, CHAR_DATA *victim )
{
  if (ch->fighting != NULL) return;
  if ( IS_AFFECTED(ch, AFF_SLEEP) )
    affect_strip( ch, gsn_sleep );
  ch->fighting = victim;
  ch->position = POS_FIGHTING;
  ch->damcap[DAM_CHANGE] = 1;
  autodrop(ch);
  autodrop(victim);
  dropinvis(ch);
  dropinvis(victim);
  return;
}

/*
 * Stop fights.
 */
void stop_embrace( CHAR_DATA *ch, CHAR_DATA *victim )
{
  if ( victim == NULL)
  {
    ch->embracing=NULL;
    return;
  }
  victim->embraced=NULL;
  ch->embracing=NULL;
  return;
}

void stop_fighting( CHAR_DATA *ch, bool fBoth )
{
  CHAR_DATA *fch;
  for ( fch = char_list; fch != NULL; fch = fch->next )
  {
    if ( fch == ch || ( fBoth && fch->fighting == ch ) )
    {
      fch->fighting = NULL;
      fch->position = POS_STANDING;
      update_pos( fch );
    }
  }
  return;
}

/*
 * Make a corpse out of a character.
 */
void make_corpse( CHAR_DATA *ch )
{
  char buf[MAX_STRING_LENGTH];
  OBJ_DATA *corpse;
  OBJ_DATA *obj;
  OBJ_DATA *obj_next;
  char *name;

  if (!ch) return;
  if (ch->in_room == NULL)
  {
    return;
  }
  if ( IS_NPC(ch) )
  {
    name = ch->short_descr;
    corpse = create_object(get_obj_index(OBJ_VNUM_CORPSE_NPC), 0);
    corpse->timer = number_range( 4, 8 );
    corpse->value[2]= ch->pIndexData->vnum;
  }
  else
  {
    name  = ch->name;
    corpse  = create_object(get_obj_index(OBJ_VNUM_CORPSE_PC), 0);
    corpse->timer = number_range( 25, 40 );
  }
  if (IS_SET(ch->extra, EXTRA_ZOMBIE))
    SET_BIT(corpse->quest, QUEST_ZOMBIE);
  sprintf( buf, corpse->short_descr, name );
  free_string( corpse->short_descr );
  corpse->short_descr = str_dup( buf );
  sprintf( buf, corpse->description, name );
  free_string( corpse->description );
  corpse->description = str_dup( buf );
  for ( obj = ch->carrying; obj != NULL; obj = obj_next )
  {
    obj_next = obj->next_content;
    obj_from_char( obj );
    if (IS_SET( obj->extra_flags, ITEM_VANISH )) extract_obj( obj );
    else
    {
      if (IS_AFFECTED(ch,AFF_SHADOWPLANE) && (!IS_SET(obj->extra_flags, ITEM_SHADOWPLANE)))
        SET_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
      obj_to_obj( obj, corpse );
    }
  }
  if (IS_AFFECTED(ch, AFF_SHADOWPLANE)) SET_BIT(corpse->extra_flags, ITEM_SHADOWPLANE);
  obj_to_room( corpse, ch->in_room );
  return;
}

void make_part( CHAR_DATA *ch, char *argument )
{
  char  arg [MAX_INPUT_LENGTH];
  int vnum;

  argument = one_argument(argument,arg);
  vnum = 0;

  if (arg[0] == '\0') return;
  if (ch->in_room == NULL) return;

  if (!str_cmp(arg,"head")) vnum = OBJ_VNUM_SEVERED_HEAD;
  else if (!str_cmp(arg,"arm")) vnum = OBJ_VNUM_SLICED_ARM;
  else if (!str_cmp(arg,"leg")) vnum = OBJ_VNUM_SLICED_LEG;
  else if (!str_cmp(arg,"heart")) vnum = OBJ_VNUM_TORN_HEART;
  else if (!str_cmp(arg,"turd")) vnum = OBJ_VNUM_TORN_HEART;
  else if (!str_cmp(arg,"entrails")) vnum = OBJ_VNUM_SPILLED_ENTRAILS;
  else if (!str_cmp(arg,"brain")) vnum = OBJ_VNUM_QUIVERING_BRAIN;
  else if (!str_cmp(arg,"eyeball")) vnum = OBJ_VNUM_SQUIDGY_EYEBALL;
  else if (!str_cmp(arg,"blood")) vnum = OBJ_VNUM_SPILT_BLOOD;
  else if (!str_cmp(arg,"face")) vnum = OBJ_VNUM_RIPPED_FACE;
  else if (!str_cmp(arg,"windpipe")) vnum = OBJ_VNUM_TORN_WINDPIPE;
  else if (!str_cmp(arg,"cracked_head")) vnum = OBJ_VNUM_CRACKED_HEAD;
  else if (!str_cmp(arg,"ear")) vnum = OBJ_VNUM_SLICED_EAR;
  else if (!str_cmp(arg,"nose")) vnum = OBJ_VNUM_SLICED_NOSE;
  else if (!str_cmp(arg,"tooth")) vnum = OBJ_VNUM_KNOCKED_TOOTH;
  else if (!str_cmp(arg,"tongue")) vnum = OBJ_VNUM_TORN_TONGUE;
  else if (!str_cmp(arg,"hand")) vnum = OBJ_VNUM_SEVERED_HAND;
  else if (!str_cmp(arg,"foot")) vnum = OBJ_VNUM_SEVERED_FOOT;
  else if (!str_cmp(arg,"thumb")) vnum = OBJ_VNUM_SEVERED_THUMB;
  else if (!str_cmp(arg,"index")) vnum = OBJ_VNUM_SEVERED_INDEX;
  else if (!str_cmp(arg,"middle")) vnum = OBJ_VNUM_SEVERED_MIDDLE;
  else if (!str_cmp(arg,"ring")) vnum = OBJ_VNUM_SEVERED_RING;
  else if (!str_cmp(arg,"little")) vnum = OBJ_VNUM_SEVERED_LITTLE;
  else if (!str_cmp(arg,"toe")) vnum = OBJ_VNUM_SEVERED_TOE;

  if ( vnum != 0 )
  {
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    char *name;

    name = IS_NPC(ch) ? ch->short_descr : ch->name;
    obj = create_object( get_obj_index( vnum ), 0 );
    obj->timer = number_range( 4, 7 );  // try this.. might work. Jobo

/*
    if (IS_NPC(ch)) obj->timer = number_range(2,5);
    else obj->timer = -1;
*/

    if (!str_cmp(arg,"head") && IS_NPC(ch))
      obj->value[1] = ch->pIndexData->vnum;
    else if (!str_cmp(arg,"head") && !IS_NPC(ch))
      {ch->pcdata->chobj = obj; obj->chobj = ch;obj->timer = number_range(1,2); obj->item_type = ITEM_HEAD;}
    else if (!str_cmp(arg,"arm"))
      SET_BIT(obj->extra_flags2, ITEM_ARM);
    else if (!str_cmp(arg,"brain") && !IS_NPC(ch) &&  IS_AFFECTED(ch,AFF_POLYMORPH) && IS_HEAD(ch, LOST_HEAD))
    {
      if (ch->pcdata->chobj != NULL) ch->pcdata->chobj->chobj = NULL;
      ch->pcdata->chobj = obj;
      obj->chobj = ch;
      obj->timer = number_range(1,2);
      obj->item_type = ITEM_HEAD;
    }
    if (vnum == OBJ_VNUM_SPILT_BLOOD) obj->timer = 2;
    if (!IS_NPC(ch))
    {
      sprintf( buf, obj->name, name );
      free_string( obj->name );
      obj->name = str_dup( buf );
    }
    else
    {
      sprintf( buf, obj->name, "mob" );
      free_string( obj->name );
      obj->name = str_dup( buf );
    }
    sprintf( buf, obj->short_descr, name );
    free_string( obj->short_descr );
    obj->short_descr = str_dup( buf );
    sprintf( buf, obj->description, name );
    free_string( obj->description );
    obj->description = str_dup( buf );
    if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
      SET_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
    obj_to_room( obj, ch->in_room );
  }
  return;
}

void raw_kill( CHAR_DATA *victim )
{
  CHAR_DATA *mount;
  stop_fighting( victim, TRUE );
  make_corpse( victim );
  if ( (mount = victim->mount) != NULL)
  {
    if (victim->mounted == IS_MOUNT)
    {
      act("$n rolls off the corpse of $N.",mount,NULL,victim,TO_ROOM);
      act("You roll off the corpse of $N.",mount,NULL,victim,TO_CHAR);
    }
    else if (victim->mounted == IS_RIDING)
    {
      act("$n falls off $N.",victim,NULL,mount,TO_ROOM);
      act("You fall off $N.",victim,NULL,mount,TO_CHAR);
    }
    mount->mount    = NULL;
    victim->mount   = NULL;
    mount->mounted  = IS_ON_FOOT;
    victim->mounted = IS_ON_FOOT;
  }
  if ( IS_NPC(victim) )
  {
    victim->pIndexData->killed++;
    kill_table[URANGE(0, victim->level, MAX_LEVEL-1)].killed++;
    extract_char( victim, TRUE );
    return;
  }
  extract_char( victim, FALSE );
  while ( victim->affected )
    affect_remove( victim, victim->affected );
  if (IS_AFFECTED(victim,AFF_POLYMORPH) && IS_AFFECTED(victim,AFF_ETHEREAL))
  {
    victim->affected_by = AFF_POLYMORPH + AFF_ETHEREAL;
  }
  else if (IS_AFFECTED(victim,AFF_POLYMORPH)) victim->affected_by = AFF_POLYMORPH;
  else if (IS_AFFECTED(victim,AFF_ETHEREAL)) victim->affected_by = AFF_ETHEREAL;
  else victim->affected_by = 0;
  REMOVE_BIT(victim->immune, IMM_STAKE);
  REMOVE_BIT(victim->extra, TIED_UP);
  REMOVE_BIT(victim->extra, GAGGED);
  REMOVE_BIT(victim->extra, BLINDFOLDED);
  victim->pcdata->stats[DEMON_POWER] = 0;
  victim->itemaffect   = 0;
  victim->loc_hp[0]    = 0;
  victim->loc_hp[1]    = 0;
  victim->loc_hp[2]    = 0;
  victim->loc_hp[3]    = 0;
  victim->loc_hp[4]    = 0;
  victim->loc_hp[5]    = 0;
  victim->loc_hp[6]    = 0;
  victim->armor  = 100;
  victim->position   = POS_RESTING;
  victim->hit    = UMAX( 1, victim->hit  );
  victim->mana   = UMAX( 1, victim->mana );
  victim->move   = UMAX( 1, victim->move );
  victim->hitroll  = 0;
  victim->damroll  = 0;
  victim->saving_throw = 0;
  victim->carry_weight = 0;
  victim->carry_number = 0;
  do_call(victim,"all");
  save_char_obj( victim );
  return;
}

void behead( CHAR_DATA *victim )
{
  char buf [MAX_STRING_LENGTH];
  ROOM_INDEX_DATA *location;

  if (IS_NPC(victim)) return;
  powerdown(victim); /* remove all class shit - Jobo */
  location = victim->in_room;
  stop_fighting( victim, TRUE );
  make_corpse( victim );
  extract_char( victim, FALSE );
  char_from_room(victim);
  char_to_room(victim,location);
  if (victim == NULL)
  {bug( "Behead: Victim no longer exists.", 0 );return;}
  make_part( victim, "head" );
  while ( victim->affected )
    affect_remove( victim, victim->affected );
  victim->affected_by = 0;
  REMOVE_BIT(victim->immune, IMM_STAKE);
  REMOVE_BIT(victim->extra, TIED_UP);
  REMOVE_BIT(victim->extra, GAGGED);
  REMOVE_BIT(victim->extra, BLINDFOLDED);
  REMOVE_BIT(victim->extra, EXTRA_PREGNANT);
  REMOVE_BIT(victim->extra, EXTRA_LABOUR);
  victim->pcdata->stats[DEMON_POWER] = 0;
  victim->itemaffect   = 0;
  victim->loc_hp[0]    = 0;
  victim->loc_hp[1]    = 0;
  victim->loc_hp[2]    = 0;
  victim->loc_hp[3]    = 0;
  victim->loc_hp[4]    = 0;
  victim->loc_hp[5]    = 0;
  victim->loc_hp[6]    = 0;
  victim->affected_by  = 0;
  victim->armor  = 100;
  victim->position   = POS_STANDING;
  victim->hit    = 1;
  victim->mana   = UMAX( 1, victim->mana );
  victim->move   = UMAX( 1, victim->move );
  victim->hitroll  = 0;
  victim->damroll  = 0;
  victim->saving_throw = 0;
  victim->carry_weight = 0;
  victim->carry_number = 0;
  victim->pcdata->sit_safe  = 0;
  victim->fight_timer  = 0;
  SET_BIT(victim->loc_hp[0],LOST_HEAD);
  SET_BIT(victim->affected_by,AFF_POLYMORPH);
  sprintf(buf,"the severed head of %s",victim->name);
  free_string(victim->morph);
  victim->morph = str_dup(buf);
  do_call(victim,"all");
  save_char_obj( victim );
  return;
}

void group_gain( CHAR_DATA *ch, CHAR_DATA *victim )
{
  char buf[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  CHAR_DATA *gch;
  CHAR_DATA *lch;
  CHAR_DATA *mount;
  int xp;
  int temp;
  int members;

  /*
   * Monsters don't get kill xp's or alignment changes.
   * P-killing doesn't help either.
   * Dying of mortal wounds or poison doesn't give xp to anyone!
   */
  if ((IS_NPC(ch) && (mount = ch->mount) == NULL) || victim == ch) return;
  members = 0;
  for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
  {
    if ( is_same_group( gch, ch ) )
      members++;
  }
  if ( members == 0 )
  {
    bug( "Group_gain: members.", members );
    members = 1;
  }
  lch = (ch->leader != NULL) ? ch->leader : ch;
  for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
  {
    int xp_modifier = 100;

    if (!is_same_group( gch, ch )) continue;
    xp = xp_compute( gch, victim ) * 2;

    sprintf(buf2,"#RExp modifiers  #G:");
    if ((IS_EVIL(gch) && IS_GOOD(victim)) || (IS_GOOD(gch) && IS_EVIL(victim)))
    {
      xp_modifier += 25;
      strcat(buf2," #Calignment#n");
    }
    if ((IS_EVIL(gch) && IS_EVIL(victim)) || (IS_GOOD(gch) && IS_GOOD(victim)))
    {
      xp_modifier -= 25;
      strcat(buf2," #palignment#n");
    }
    if (gch != NULL && gch->desc != NULL)
    {
      if (gch->desc->out_compress)
      {
        xp_modifier += 25;
        strcat(buf2," #Cmccp#n");
      }
    }
    if((get_age(gch)-17)<2)  /* 4 hours worth of newbie exp. */
    {
      xp_modifier += 200;
      strcat(buf2," #Cnewbie#n");
    }
    if (global_exp)
    {
      xp_modifier += 100;
      strcat(buf2," #Chappy hour#n");
    }
    if ((temp = get_ratio(gch)) > 100)
    {
      xp_modifier += UMIN(100, temp / 50);
      strcat(buf2," #Cpkscore#n");
    }
    if (!IS_NPC(gch) && gch->pcdata->kingdom != 0)
    {
      xp_modifier -= 5;
      strcat(buf2," #ptithe#n");
    }
    if (members > 1)
    {
      strcat(buf2," #pgrouped#n");
      xp_modifier *= 100;
      xp_modifier /= 75 * members;
    }
    strcat(buf2,"\n\r");
    xp = xp * xp_modifier / 100;
    if (!IS_SET(gch->act, PLR_BRIEF4)) send_to_char(buf2,gch);
    sprintf(buf2,"#RTotal modifier #G:#n %d percent bonus\n\r",xp_modifier - 100);
    if (!IS_SET(gch->act, PLR_BRIEF4)) send_to_char(buf2,gch);
    if (gch->exp > 2000000000)
    {
      send_to_char("YOU CANNOT GAIN ANY MORE EXP!\n\r",gch);
      xp = 0;
    }
    if (xp > 1000000 && get_age(gch) - 17 < 2 ) xp = 1000000 + number_range(-100,100) * number_range(-100,100);
    else if (xp > 3000000) xp = 3000000 + number_range(-100,100) * number_range(-100,100);
    sprintf( buf, "You receive %d experience points.\n\r", xp );
    send_to_char( buf, gch );
    if ((mount = gch->mount) != NULL) send_to_char( buf, mount );
    gain_exp( gch, xp );
  }
  return;
}

int xp_compute( CHAR_DATA *gch, CHAR_DATA *victim )
{
  double xp;
  int vnum;
  
  if (victim->level < 100)
    xp = 300 - URANGE(-5, 3 - (victim->level*5/3), 6 ) * 50;
  else
    xp = 300 - URANGE(-10, 3 - (victim->level*5/3), 6 ) * 50;   

    
  /* 
   * Put in mob vnum that you don't want players to gain exp for
   * Rotain
   */
  if (IS_NPC(victim) && (vnum = victim->pIndexData->vnum) > 29000)
  {
    switch (vnum)
    {
      default:
      break;
      case 29600:
      case 30001:
      case 30006:
      case 30007:
      case 30008:
      case 30009:
      case 30000:
      case 90001:
      case 90101:
      case 90201:
      case 90301:
      case 90401:
      case 90501:
      case 90601:
      case 90701:
      case 90801:
      case 90901:
      case 90100:  /* Kingdom healers 90x00 */
      case 90200:
      case 90300:
      case 90400:
      case 90500:
      case 90600:
      case 90700:
      case 90800:
      case 90900:
      return 0;
    }
  }

  /*
   * Adjust for popularity of target:
   *   -1/8 for each target over  'par' (down to -100%)
   *   +1/8 for each target under 'par' (  up to + 25%)
   */
  xp -= xp * number_range( -2, 2 ) / 8;
  xp  = number_range( xp * 3 / 4, xp * 5 / 4 );
  xp  = UMAX( 0, xp );
  xp  = (xp * (victim->level) * 0.60);
  xp  = xp / 2; /* Put in cause players compaling to much exp :P */
  if (!IS_NPC(gch))
  {
    gch->pcdata->score[SCORE_TOTAL_LEVEL] += victim->level;
    if (victim->level > gch->pcdata->score[SCORE_HIGH_LEVEL])
    gch->pcdata->score[SCORE_HIGH_LEVEL] += victim->level;
    gch->pcdata->score[SCORE_TOTAL_XP] += xp;
    if (xp > gch->pcdata->score[SCORE_HIGH_XP])
    gch->pcdata->score[SCORE_HIGH_XP] += xp;
  }
  if (!IS_NPC(gch) && xp > 499 && gch->pcdata->disc_points != 999 && gch->pcdata->disc_research != -1)
  {
    if (!IS_SET(gch->act, PLR_BRIEF4)) stc("#RYou gained a discipline point.#n\n\r", gch);
    gain_disc_points(gch, 1);
    if (!IS_SET(gch->act, PLR_BRIEF4)) if (victim->level > 200 && gch->pcdata->disc_points != 999)
    {
    stc("#RYou gained a discipline point.#n\n\r", gch);
    gain_disc_points(gch, 1);
    }
    if (!IS_SET(gch->act, PLR_BRIEF4)) if (victim->level > 400 && gch->pcdata->disc_points != 999)
    {
    stc("#RYou gained a discipline point.#n\n\r", gch);
    gain_disc_points(gch, 1);
    }
    xp -= 500;
  }
  if( xp < 0 ) xp = 0;
  if (xp > 0 && xp < 4000) xp = number_range(3000,5000);
  if (IS_NPC(victim) && (IS_SET(victim->act, ACT_NOEXP))) return 0;
  return (int) xp;
}

void dam_message( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt )
{
  static char * const attack_table[] =
  {
  "hit", "slice",  "stab",  "slash", "whip", "claw",
  "blast",  "pound", "crush", "grep", "bite", "pierce", "suck"
  };
  static char * const attack_table2[] =
  {
  "hits", "slices",  "stabs",  "slashes", "whips", "claws",
  "blasts",  "pounds", "crushes", "greps", "bites", "pierces", "sucks"
  };
  char buf1[512], buf2[512], buf3[512];
  const char *vs;
  const char *vp;
  const char *attack;
  const char *attack2;
  int damp;
  int bodyloc;
  bool critical = FALSE;
  char punct;

  if (!IS_NPC(ch) && IS_SET(ch->newbits,NEW_REND)&& dam>0 && number_range(1,7) == 5) critical=TRUE;
  if ( dam ==   0 ) { vs = " miss";  vp = " misses";   }
  else if ( dam <=  50 ) { vs = " #Clightly#n"; vp = " #Clightly#n";  }
  else if ( dam <= 250 ) { vs = " #yhard#n";  vp = " #yhard#n";   }
  else if ( dam <= 500 ) { vs = " #Lvery hard#n"; vp = " #Lvery hard#n";  }
  else if ( dam <= 750 ) { vs = " #Gextremely hard!#n"; vp = " #Gextremely hard!#n";}
  else if ( dam <= 1250 ) { vs = " #7incredibly hard!#n";vp = "#7 incredibly hard!#n";} 
  else if ( dam <= 2000 ) { vs = ", #rshredding flesh!#n";vp= ", #rshredding flesh!#n";}
  else if ( dam <= 3000 ) { vs = ", #osplintering bone!#n";vp= ", #osplintering bone!#n";}
  else if ( dam <= 4500 ) { vs = ", #Rspraying blood like a fine red mist!#n";vp= ", #Rspraying blood like a fine red mist!#n";}
  else if ( dam <= 6000 ) { vs = " #yso hard it hurts just to see!#n";vp = " #yso hard it hurts just to see!#n";}
  else if ( dam <= 8000 ) { vs = " #G<#y*#L{#R*#L}#y*#G> #sextracting organs #G<#y*#L{#R*#L}#y*#G>#n";vp = " #G<#y*#L{#R*#L}#y*#G> #sextracting organs #G<#y*#L{#R*#L}#y*#G>#n";}
  else { vs = " #R()#G()#R() #CHumiliatingly Hard #R()#G()#R()#n";vp = " #R()#G()#R() #CHumiliatingly Hard #R()#G()#R()#n";}

  /* If victim's hp are less/equal to 'damp', attacker gets a death blow */
  if (IS_NPC(victim)) damp = 0;
  else damp = -10;
  if ((victim->hit - dam > damp ) || (dt >= 0 && dt < MAX_SKILL) ||
     (IS_NPC(victim) && IS_SET(victim->act, ACT_NOPARTS)))
  {
    punct   = (dam <= 250) ? ' ' : ' ';
    if ( dt == TYPE_HIT && !IS_NPC(ch) && !IS_VAMPAFF(ch,VAM_CLAWS) )
    {
      if (dam == 0)
      {
        sprintf( buf1, "$n%s $N%c[%d]",  vp, punct, dam );
        sprintf( buf2, "You%s $N%c[%d]", vs, punct, dam );
        sprintf( buf3, "$n%s you%c", vp, punct );
      }
      else
      {
        sprintf( buf1, "$n hits $N%s%c[%d]",  vp, punct, dam );
        sprintf( buf2, "You hit $N%s%c[%d]", vs, punct, dam );
        sprintf( buf3, "$n hits you%s%c[%d]", vp, punct, dam );
        critical = TRUE;
      }
    }
    else if ( dt == TYPE_HIT && !IS_NPC(ch) && IS_SET(ch->newbits, NEW_MONKFLAME))
    {
      if (dam >= 1)
      {
        if (!IS_CLASS(ch, CLASS_WEREWOLF))
        {
          sprintf( buf1, "$n's flaming hands hit $N%s%c[%d]", vp, punct, dam );
          sprintf( buf2, "Your flaming hands hit $N%s%c[%d]", vs, punct, dam );
          sprintf( buf3, "$n's flaming hands hit you%s%c[%d]", vp, punct, dam );
        }
        else
        {
          sprintf( buf1, "$n's flaming claws hit $N%s%c[%d]", vp, punct, dam );
          sprintf( buf2, "Your flaming claws hit $N%s%c[%d]", vs, punct, dam );
          sprintf( buf3, "$n's flaming claws hit you%s%c[%d]", vp, punct, dam );
        }
      }
      else 
      {
        sprintf( buf1, "$n's hit%s $N%c[%d]",  vp, punct, dam );
        sprintf( buf2, "Your hit%s $N%c[%d]", vs, punct, dam );
        sprintf( buf3, "$n hit%s you%c[%d]", vp, punct, dam );
        critical = TRUE;
      }
    }
    else
    {
      if ( dt == TYPE_HIT && !IS_NPC(ch) && (IS_VAMPAFF(ch,VAM_CLAWS)))
      {
        attack  = attack_table[dt - TYPE_HIT + 5];
        attack2 = attack_table2[dt - TYPE_HIT + 5];
      }
      else if ( dt >= 0 && dt < MAX_SKILL )
      {
        attack = skill_table[dt].noun_damage;
        attack2 = skill_table[dt].noun_damage;
      }
      else if (dt >= TYPE_HIT && dt < TYPE_HIT + sizeof(attack_table)/sizeof(attack_table[0]))
      {
        attack  = attack_table[dt - TYPE_HIT];
        attack2 = attack_table2[dt - TYPE_HIT];
      }
      else
      {
        dt  = TYPE_HIT;
        attack  = attack_table[0];
        attack2 = attack_table2[0];
      }
      if (dam == 0)
      {
        sprintf( buf1, "$n's %s%s $N%c[%d]",  attack, vp, punct, dam );
        sprintf( buf2, "Your %s%s $N%c[%d]",  attack, vp, punct, dam );
        sprintf( buf3, "$n's %s%s you%c[%d]", attack, vp, punct, dam );
      }
      else if (IS_SET(ch->newbits, NEW_MONKFLAME) && (dam == 0))
      {
        sprintf( buf1, "$n's flaming %s%s $N%c[%d]", attack2, vp, punct, dam );
        sprintf( buf2, "Your flaming %s%s $N%c[%d]",  attack, vp, punct, dam );
        sprintf( buf3, "$n's flaming %s%s you%c[%d]", attack2, vp, punct, dam );
      }
      else
      {
        if ( dt >= 0 && dt < MAX_SKILL )
        {
          sprintf( buf1, "$n's %s strikes $N%s%c[%d]",  attack2, vp, punct, dam );
          sprintf( buf2, "Your %s strikes $N%s%c[%d]",  attack, vp, punct, dam );
          sprintf( buf3, "$n's %s strikes you%s%c[%d]", attack2, vp, punct, dam );
        }
        else if (IS_SET(ch->newbits, NEW_MONKFLAME))
        {
          sprintf( buf1, "$n's flaming %s hits $N%s%c[%d]", attack2, vp, punct, dam);
          sprintf( buf2, "Your flaming %s hits $N%s%c[%d]", attack, vp, punct, dam);
          sprintf( buf3, "$n's flaming %s hits you%s%c[%d]", attack2, vp, punct, dam);
        }
        else
        {
          sprintf( buf1, "$n %s $N%s%c[%d]",  attack2, vp, punct, dam );
          sprintf( buf2, "You %s $N%s%c[%d]",  attack, vp, punct, dam );
          sprintf( buf3, "$n %s you%s%c[%d]", attack2, vp, punct, dam );
          critical = TRUE;
        }
      }
    }
    act( buf1, ch, NULL, victim, TO_NOTVICT );

    if (!(IS_SET(ch->act, PLR_BRIEF2) && (dam == 0 || dt == skill_lookup("lightning bolt") ||
        dt == skill_lookup("acid blast") || dt == skill_lookup("chill touch") || dt == skill_lookup("fireball"))))
      act( buf2, ch, NULL, victim, TO_CHAR );
    if (!(IS_SET(victim->act, PLR_BRIEF2) && (dam == 0 || dt == skill_lookup("lightning bolt") ||
        dt == skill_lookup("acid blast") || dt == skill_lookup("chill touch") || dt == skill_lookup("fireball"))))
      act( buf3, ch, NULL, victim, TO_VICT );
    if (critical) critical_hit(ch,victim,dt,dam);
    return;
  }
  if ( dt == TYPE_HIT && !IS_NPC( ch ) && !IS_VAMPAFF(ch,VAM_CLAWS) &&!IS_VAMPAFF(ch,VAM_FANGS))
  {
    damp=number_range(1,5);
    if ( damp == 1 )
    {
      act("You ram your fingers into $N's eye sockets and rip $S face off.", ch, NULL, victim, TO_CHAR);
      act("$n rams $s fingers into $N's eye sockets and rips $S face off.", ch, NULL, victim, TO_NOTVICT);
      act("$n rams $s fingers into your eye sockets and rips your face off.", ch, NULL, victim, TO_VICT);
      make_part(victim,"face");
    }
    else if ( damp == 2 )
    {
      act("You grab $N by the throat and tear $S windpipe out.", ch, NULL, victim, TO_CHAR);
      act("$n grabs $N by the throat and tears $S windpipe out.", ch, NULL, victim, TO_NOTVICT);
      act("$n grabs you by the throat and tears your windpipe out.", ch, NULL, victim, TO_VICT);
      make_part(victim,"windpipe");
    }
    else if ( damp == 3 )
    {
      act("You punch your fist through $N's stomach and rip out $S entrails.", ch, NULL, victim, TO_CHAR);
      act("$n punches $s fist through $N's stomach and rips out $S entrails.", ch, NULL, victim, TO_NOTVICT);
      act("$n punches $s fist through your stomach and rips out your entrails.", ch, NULL, victim, TO_VICT);
      make_part(victim,"entrails");
    }
    else if ( damp == 4 )
    {
      if (!IS_BODY(victim,BROKEN_SPINE))
      SET_BIT(victim->loc_hp[1],BROKEN_SPINE);
      act("You hoist $N above your head and slam $M down upon your knee.\n\rThere is a loud cracking sound as $N's spine snaps.", ch, NULL, victim, TO_CHAR);
      act("$n hoists $N above $s head and slams $M down upon $s knee.\n\rThere is a loud cracking sound as $N's spine snaps.", ch, NULL, victim, TO_NOTVICT);
      act("$n hoists you above $s head and slams you down upon $s knee.\n\rThere is a loud cracking sound as your spine snaps.", ch, NULL, victim, TO_VICT);
    }
    else if ( damp == 5 )
    {
      act("You lock your arm around $N's head, and give it a vicious twist.", ch, NULL, victim, TO_CHAR);
      act("$n locks $s arm around $N's head, and gives it a vicious twist.", ch, NULL, victim, TO_NOTVICT);
      act("$n locks $s arm around your head, and gives it a vicious twist.", ch, NULL, victim, TO_VICT);
      if (!IS_BODY(victim,BROKEN_NECK))
      {
        act("There is a loud snapping noise as your neck breaks.", victim, NULL, NULL, TO_CHAR);
        act("There is a loud snapping noise as $n's neck breaks.", victim, NULL, NULL, TO_ROOM);
        SET_BIT(victim->loc_hp[1],BROKEN_NECK);
      }
    }
    return;
  }
  if (dt >= 0 && dt < MAX_SKILL) attack = skill_table[dt].noun_damage;
  else if ( dt >= TYPE_HIT && dt < TYPE_HIT + sizeof(attack_table)/sizeof(attack_table[0]))
    attack = attack_table[dt - TYPE_HIT];
  else
  {
    dt  = TYPE_HIT;
    attack  = attack_table[0];
  }
  if ( attack == "slash" || attack == "slice" )
  {
    damp=number_range(1,8);
    if ( damp == 1 )
    {
      act("You swing your blade in a low arc, rupturing $N's abdominal cavity.\n\r$S entrails spray out over a wide area.", ch, NULL, victim, TO_CHAR);
      act("$n swings $s blade in a low arc, rupturing $N's abdominal cavity.\n\r$S entrails spray out over a wide area.", ch, NULL, victim, TO_NOTVICT);
      act("$n swings $s blade in a low arc, rupturing your abdominal cavity.\n\rYour entrails spray out over a wide area.", ch, NULL, victim, TO_VICT);
      make_part(victim,"entrails");
    }
    else if ( damp == 2 )
    {
      act("You thrust your blade into $N's mouth and twist it viciously.\n\rThe end of your blade bursts through the back of $S head.", ch, NULL, victim, TO_CHAR);
      act("$n thrusts $s blade into $N's mouth and twists it viciously.\n\rThe end of the blade bursts through the back of $N's head.", ch, NULL, victim, TO_NOTVICT);
      act("$n thrusts $s blade into your mouth and twists it viciously.\n\rYou feel the end of the blade burst through the back of your head.", ch, NULL, victim, TO_VICT);
    }
    else if ( damp == 3 )
    {
      if (!IS_BODY(victim,CUT_THROAT)) SET_BIT(victim->loc_hp[1],CUT_THROAT);
      if (!IS_BLEEDING(victim,BLEEDING_THROAT)) SET_BIT(victim->loc_hp[6],BLEEDING_THROAT);
      act("Your blow slices open $N's carotid artery, spraying blood everywhere.", ch, NULL, victim, TO_CHAR);
      act("$n's blow slices open $N's carotid artery, spraying blood everywhere.", ch, NULL, victim, TO_NOTVICT);
      act("$n's blow slices open your carotid artery, spraying blood everywhere.", ch, NULL, victim, TO_VICT);
      make_part(victim,"blood");
    }
    else if ( damp == 4 )
    {
      if (!IS_BODY(victim,CUT_THROAT)) SET_BIT(victim->loc_hp[1],CUT_THROAT);
      if (!IS_BLEEDING(victim,BLEEDING_THROAT)) SET_BIT(victim->loc_hp[6],BLEEDING_THROAT);
      act("You swing your blade across $N's throat, showering the area with blood.", ch, NULL, victim, TO_CHAR);
      act("$n swings $s blade across $N's throat, showering the area with blood.", ch, NULL, victim, TO_NOTVICT);
      act("$n swings $s blade across your throat, showering the area with blood.", ch, NULL, victim, TO_VICT);
      make_part(victim,"blood");
    }
    else if ( damp == 5 )
    {
      if (!IS_HEAD(victim,BROKEN_SKULL))
      {
        act("You swing your blade down upon $N's head, splitting it open.\n\r$N's brains pour out of $S forehead.", ch, NULL, victim, TO_CHAR);
        act("$n swings $s blade down upon $N's head, splitting it open.\n\r$N's brains pour out of $S forehead.", ch, NULL, victim, TO_NOTVICT);
        act("$n swings $s blade down upon your head, splitting it open.\n\rYour brains pour out of your forehead.", ch, NULL, victim, TO_VICT);
        make_part(victim,"brain");
        SET_BIT(victim->loc_hp[0],BROKEN_SKULL);
      }
      else
      {
        act("You plunge your blade deep into $N's chest.", ch, NULL, victim, TO_CHAR);
        act("$n plunges $s blade deep into $N's chest.", ch, NULL, victim, TO_NOTVICT);
        act("$n plunges $s blade deep into your chest.", ch, NULL, victim, TO_VICT);
      }
    }
    else if ( damp == 6 )
    {
      act("You swing your blade between $N's legs, nearly splitting $M in half.", ch, NULL, victim, TO_CHAR);
      act("$n swings $s blade between $N's legs, nearly splitting $M in half.", ch, NULL, victim, TO_NOTVICT);
      act("$n swings $s blade between your legs, nearly splitting you in half.", ch, NULL, victim, TO_VICT);
    }
    else if ( damp == 7 )
    {
      if (!IS_ARM_L(victim,LOST_ARM))
      {
        act("You swing your blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_CHAR);
        act("$n swings $s blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_NOTVICT);
        act("$n swings $s blade in a wide arc, slicing off your arm.", ch, NULL, victim, TO_VICT);
        make_part(victim,"arm");
        SET_BIT(victim->loc_hp[2],LOST_ARM);
        if (!IS_BLEEDING(victim,BLEEDING_ARM_L)) SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
        if (IS_BLEEDING(victim,BLEEDING_HAND_L)) REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_L);
      }
      else if (!IS_ARM_R(victim,LOST_ARM))
      {
        act("You swing your blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_CHAR);
        act("$n swings $s blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_NOTVICT);
        act("$n swings $s blade in a wide arc, slicing off your arm.", ch, NULL, victim, TO_VICT);
        make_part(victim,"arm");
        SET_BIT(victim->loc_hp[3],LOST_ARM);
        if (!IS_BLEEDING(victim,BLEEDING_ARM_R)) SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
        if (IS_BLEEDING(victim,BLEEDING_HAND_R)) REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_R);
      }
      else
      {
        act("You plunge your blade deep into $N's chest.", ch, NULL, victim, TO_CHAR);
        act("$n plunges $s blade deep into $N's chest.", ch, NULL, victim, TO_NOTVICT);
        act("$n plunges $s blade deep into your chest.", ch, NULL, victim, TO_VICT);
      }
    }
    else if ( damp == 8 )
    {
      if (!IS_LEG_L(victim,LOST_LEG))
      {
        act("You swing your blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_CHAR);
        act("$n swings $s blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_NOTVICT);
        act("$n swings $s blade in a wide arc, slicing off your leg at the hip.", ch, NULL, victim, TO_VICT);
        make_part(victim,"leg");
        SET_BIT(victim->loc_hp[4],LOST_LEG);
        if (!IS_BLEEDING(victim,BLEEDING_LEG_L)) SET_BIT(victim->loc_hp[6],BLEEDING_LEG_L);
        if (IS_BLEEDING(victim,BLEEDING_FOOT_L)) REMOVE_BIT(victim->loc_hp[6],BLEEDING_FOOT_L);
      }
      else if (!IS_LEG_R(victim,LOST_LEG))
      {
        act("You swing your blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_CHAR);
        act("$n swings $s blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_NOTVICT);
        act("$n swings $s blade in a wide arc, slicing off your leg at the hip.", ch, NULL, victim, TO_VICT);
        make_part(victim,"leg");
        SET_BIT(victim->loc_hp[5],LOST_LEG);
        if (!IS_BLEEDING(victim,BLEEDING_LEG_R)) SET_BIT(victim->loc_hp[6],BLEEDING_LEG_R);
        if (IS_BLEEDING(victim,BLEEDING_FOOT_R)) REMOVE_BIT(victim->loc_hp[6],BLEEDING_FOOT_R);
      }
      else
      {
        act("You plunge your blade deep into $N's chest.", ch, NULL, victim, TO_CHAR);
        act("$n plunges $s blade deep into $N's chest.", ch, NULL, victim, TO_NOTVICT);
        act("$n plunges $s blade deep into your chest.", ch, NULL, victim, TO_VICT);
      }
    }
  }
  else if ( attack == "stab" || attack == "pierce" )
  {
    damp=number_range(1,5);
    if ( damp == 1 )
    {
      act("You defty invert your weapon and plunge it point first into $N's chest.\n\rA shower of blood sprays from the wound, showering the area.", ch, NULL, victim, TO_CHAR);
      act("$n defty inverts $s weapon and plunge it point first into $N's chest.\n\rA shower of blood sprays from the wound, showering the area.", ch, NULL, victim, TO_NOTVICT);
      act("$n defty inverts $s weapon and plunge it point first into your chest.\n\rA shower of blood sprays from the wound, showering the area.", ch, NULL, victim, TO_VICT);
      make_part(victim,"blood");
    }
    else if ( damp == 2 )
    {
      act("You thrust your blade into $N's mouth and twist it viciously.\n\rThe end of your blade bursts through the back of $S head.", ch, NULL, victim, TO_CHAR);
      act("$n thrusts $s blade into $N's mouth and twists it viciously.\n\rThe end of the blade bursts through the back of $N's head.", ch, NULL, victim, TO_NOTVICT);
      act("$n thrusts $s blade into your mouth and twists it viciously.\n\rYou feel the end of the blade burst through the back of your head.", ch, NULL, victim, TO_VICT);
    }
    else if ( damp == 3 )
    {
      act("You thrust your weapon up under $N's jaw and through $S head.", ch, NULL, victim, TO_CHAR);
      act("$n thrusts $s weapon up under $N's jaw and through $S head.", ch, NULL, victim, TO_NOTVICT);
      act("$n thrusts $s weapon up under your jaw and through your head.", ch, NULL, victim, TO_VICT);
    }
    else if ( damp == 4 )
    {
      act("You ram your weapon through $N's body, pinning $M to the ground.", ch, NULL, victim, TO_CHAR);
      act("$n rams $s weapon through $N's body, pinning $M to the ground.", ch, NULL, victim, TO_NOTVICT);
      act("$n rams $s weapon through your body, pinning you to the ground.", ch, NULL, victim, TO_VICT);
    }
    else if ( damp == 5 )
    {
      act("You stab your weapon into $N's eye and out the back of $S head.", ch, NULL, victim, TO_CHAR);
      act("$n stabs $s weapon into $N's eye and out the back of $S head.", ch, NULL, victim, TO_NOTVICT);
      act("$n stabs $s weapon into your eye and out the back of your head.", ch, NULL, victim, TO_VICT);
      if (!IS_HEAD(victim,LOST_EYE_L) && number_percent() < 50) SET_BIT(victim->loc_hp[0],LOST_EYE_L);
      else if (!IS_HEAD(victim,LOST_EYE_R)) SET_BIT(victim->loc_hp[0],LOST_EYE_R);
      else if (!IS_HEAD(victim,LOST_EYE_L)) SET_BIT(victim->loc_hp[0],LOST_EYE_L);
    }
  }
  else if ( attack == "blast" || attack == "pound" || attack == "crush" )
  {
    damp=number_range(1,3);
    bodyloc = 0;
    if ( damp == 1)
    {
      act("Your blow smashes through $N's chest, caving in half $S ribcage.", ch, NULL, victim, TO_CHAR);
      act("$n's blow smashes through $N's chest, caving in half $S ribcage.", ch, NULL, victim, TO_NOTVICT);
      act("$n's blow smashes through your chest, caving in half your ribcage.", ch, NULL, victim, TO_VICT);
      if (IS_BODY(victim,BROKEN_RIBS_1 )) {bodyloc += 1; REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_1);}
      if (IS_BODY(victim,BROKEN_RIBS_2 )) {bodyloc += 2; REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_2);}
      if (IS_BODY(victim,BROKEN_RIBS_4 )) {bodyloc += 4; REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_4);}
      if (IS_BODY(victim,BROKEN_RIBS_8 )) {bodyloc += 8; REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_8);}
      if (IS_BODY(victim,BROKEN_RIBS_16)) {bodyloc += 16; REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_16);}
      bodyloc += number_range(1,3);
      if (bodyloc > 24) bodyloc = 24;
      if (bodyloc >= 16) {bodyloc -= 16; SET_BIT(victim->loc_hp[1],BROKEN_RIBS_16);}
      if (bodyloc >= 8) {bodyloc -= 8; SET_BIT(victim->loc_hp[1],BROKEN_RIBS_8);}
      if (bodyloc >= 4) {bodyloc -= 4; SET_BIT(victim->loc_hp[1],BROKEN_RIBS_4);}
      if (bodyloc >= 2) {bodyloc -= 2; SET_BIT(victim->loc_hp[1],BROKEN_RIBS_2);}
      if (bodyloc >= 1) {bodyloc -= 1; SET_BIT(victim->loc_hp[1],BROKEN_RIBS_1);}
    }
    else if ( damp == 2)
    {
      act("Your blow smashes $N's spine, shattering it in several places.", ch, NULL, victim, TO_CHAR);
      act("$n's blow smashes $N's spine, shattering it in several places.", ch, NULL, victim, TO_NOTVICT);
      act("$n's blow smashes your spine, shattering it in several places.", ch, NULL, victim, TO_VICT);
      if (!IS_BODY(victim,BROKEN_SPINE)) SET_BIT(victim->loc_hp[1],BROKEN_SPINE);
    }
    else if ( damp == 3)
    {
      if (!IS_HEAD(victim,BROKEN_SKULL))
      {
        act("You swing your weapon down upon $N's head.\n\r$N's head cracks open like an overripe melon, leaking out brains.", ch, NULL, victim, TO_CHAR);
        act("$n swings $s weapon down upon $N's head.\n\r$N's head cracks open like an overripe melon, covering you with brains.", ch, NULL, victim, TO_NOTVICT);
        act("$n swings $s weapon down upon your head.\n\rYour head cracks open like an overripe melon, spilling your brains everywhere.", ch, NULL, victim, TO_VICT);
        make_part(victim,"brain");
        SET_BIT(victim->loc_hp[0],BROKEN_SKULL);
      }
      else
      {
        act("You hammer your weapon into $N's side, crushing bone.", ch, NULL, victim, TO_CHAR);
        act("$n hammers $s weapon into $N's side, crushing bone.", ch, NULL, victim, TO_NOTVICT);
        act("$n hammers $s weapon into your side, crushing bone.", ch, NULL, victim, TO_VICT);
      }
    }
  }
  else if ( !IS_NPC( ch ) && (attack == "bite" ||IS_VAMPAFF(ch,VAM_FANGS)))
  {
    act("You sink your teeth into $N's throat and tear out $S jugular vein.\n\rYou wipe the blood from your chin with one hand.", ch, NULL, victim, TO_CHAR);
    act("$n sink $s teeth into $N's throat and tears out $S jugular vein.\n\r$n wipes the blood from $s chin with one hand.", ch, NULL, victim, TO_NOTVICT);
    act("$n sink $s teeth into your throat and tears out your jugular vein.\n\r$n wipes the blood from $s chin with one hand.", ch, NULL, victim, TO_VICT);
    make_part(victim,"blood");
    if (!IS_BODY(victim,CUT_THROAT)) SET_BIT(victim->loc_hp[1],CUT_THROAT);
    if (!IS_BLEEDING(victim,BLEEDING_THROAT)) SET_BIT(victim->loc_hp[6],BLEEDING_THROAT);
  }
  else if ( !IS_NPC(ch) && (attack == "claw" || IS_VAMPAFF(ch,VAM_CLAWS)))
  {
    damp=number_range(1,2);
    if ( damp == 1 )
    {
      act("You tear out $N's throat, showering the area with blood.", ch, NULL, victim, TO_CHAR);
      act("$n tears out $N's throat, showering the area with blood.", ch, NULL, victim, TO_NOTVICT);
      act("$n tears out your throat, showering the area with blood.", ch, NULL, victim, TO_VICT);
      make_part(victim,"blood");
      if (!IS_BODY(victim,CUT_THROAT)) SET_BIT(victim->loc_hp[1],CUT_THROAT);
      if (!IS_BLEEDING(victim,BLEEDING_THROAT)) SET_BIT(victim->loc_hp[6],BLEEDING_THROAT);
    }
    if ( damp == 2 )
    {
      if (!IS_HEAD(victim,LOST_EYE_L) && number_percent() < 50)
      {
        act("You rip an eyeball from $N's face.", ch, NULL, victim, TO_CHAR);
        act("$n rips an eyeball from $N's face.", ch, NULL, victim, TO_NOTVICT);
        act("$n rips an eyeball from your face.", ch, NULL, victim, TO_VICT);
        make_part(victim,"eyeball");
        SET_BIT(victim->loc_hp[0],LOST_EYE_L);
      }
      else if (!IS_HEAD(victim,LOST_EYE_R))
      {
        act("You rip an eyeball from $N's face.", ch, NULL, victim, TO_CHAR);
        act("$n rips an eyeball from $N's face.", ch, NULL, victim, TO_NOTVICT);
        act("$n rips an eyeball from your face.", ch, NULL, victim, TO_VICT);
        make_part(victim,"eyeball");
        SET_BIT(victim->loc_hp[0],LOST_EYE_R);
      }
      else if (!IS_HEAD(victim,LOST_EYE_L))
      {
        act("You rip an eyeball from $N's face.", ch, NULL, victim, TO_CHAR);
        act("$n rips an eyeball from $N's face.", ch, NULL, victim, TO_NOTVICT);
        act("$n rips an eyeball from your face.", ch, NULL, victim, TO_VICT);
        make_part(victim,"eyeball");
        SET_BIT(victim->loc_hp[0],LOST_EYE_L);
      }
      else
      {
        act("You claw open $N's chest.", ch, NULL, victim, TO_CHAR);
        act("$n claws open $N's chest.", ch, NULL, victim, TO_NOTVICT);
        act("$n claws open $N's chest.", ch, NULL, victim, TO_VICT);
      }
    }
  }
  else if ( attack == "whip" )
  {
    act("You entangle $N around the neck, and squeeze out $S life.", ch, NULL, victim, TO_CHAR);
    act("$n entangles $N around the neck, and squeezes out $S life.", ch, NULL, victim, TO_NOTVICT);
    act("$n entangles you around the neck, and squeezes the life out of you.", ch, NULL, victim, TO_VICT);
    if (!IS_BODY(victim,BROKEN_NECK)) SET_BIT(victim->loc_hp[1],BROKEN_NECK);
  }
  else if ( attack == "suck" || attack == "grep" )
  {
    act("You place your weapon on $N's head and suck out $S brains.", ch, NULL, victim, TO_CHAR);
    act("$n places $s weapon on $N's head and suck out $S brains.", ch, NULL, victim, TO_NOTVICT);
    act("$n places $s weapon on your head and suck out your brains.", ch, NULL, victim, TO_VICT);
  }
  else
  {
  /*bug( "Dam_message: bad dt %d.", dt );*/
  }
  return;
}

/*
 * Disarm a creature.
 * Caller must check for successful attack.
 */
void disarm( CHAR_DATA *ch, CHAR_DATA *victim )
{
  OBJ_DATA *obj;
  char buf [MAX_STRING_LENGTH];

  if (IS_NPC(ch) && victim->level > 2 && number_percent() > 10)
    return;
  if (!IS_IMMUNE(ch, IMM_DISARM))
    return;
  if(((obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL) || obj->item_type != ITEM_WEAPON)
  {
    if(((obj = get_eq_char( victim, WEAR_HOLD ) ) == NULL) || obj->item_type != ITEM_WEAPON)
      return;
  }
  sprintf(buf,"$n disarms you!");
  ADD_COLOUR(ch,buf,WHITE);
  act( buf, ch, NULL, victim, TO_VICT    );
  sprintf(buf,"You disarm $N!");
  ADD_COLOUR(ch,buf,WHITE);
  act( buf,  ch, NULL, victim, TO_CHAR    );
  sprintf(buf,"$n disarms $N!");
  ADD_COLOUR(ch,buf,WHITE);
  act( buf,  ch, NULL, victim, TO_NOTVICT );
  obj_from_char( obj );
  if (IS_SET(obj->extra_flags, ITEM_LOYAL) && ( !IS_NPC(victim) ) )
  {
    act( "$p leaps back into your hand!", victim, obj, NULL, TO_CHAR );
    act( "$p leaps back into $n's hand!",  victim, obj, NULL, TO_ROOM );
    obj_to_char( obj, victim );
    do_wear( victim, obj->name);
  }
  else if (IS_NPC(victim))
    obj_to_char(obj, victim);
  else
    obj_to_room(obj, victim->in_room);
  return;
}

/*
 * Trip a creature.
 * Caller must check for successful attack.
 */
void trip( CHAR_DATA *ch, CHAR_DATA *victim )
{
  char buf [MAX_STRING_LENGTH];

  if (IS_AFFECTED(victim,AFF_FLYING)) return;
  if (IS_NPC(ch) && victim->level > 2 && number_percent() > 5) return;
  if (!IS_NPC(victim))
  {
    if (IS_CLASS(victim, CLASS_VAMPIRE) && IS_VAMPAFF(victim,VAM_FLYING))
      return;
    if (IS_CLASS(victim, CLASS_DEMON) && IS_DEMAFF(victim, DEM_UNFOLDED))
      return;
    if (IS_CLASS(victim, CLASS_ANGEL) && IS_SET(victim->pcdata->powers[ANGEL_POWERS], ANGEL_WINGS))
      return;
    if (IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1], DPOWER_LEVITATION)) 
      return;
  }
  if ( victim->wait == 0)
  {
    sprintf(buf,"$n trips you and you go down!");
    ADD_COLOUR(ch,buf,WHITE);
    act( buf, ch, NULL, victim, TO_VICT    );
    sprintf(buf,"You trip $N and $E goes down!");
    ADD_COLOUR(ch,buf,WHITE);
    act( buf,  ch, NULL, victim, TO_CHAR    );
    sprintf(buf,"$n trips $N and $E goes down!");
    ADD_COLOUR(ch,buf,WHITE);
    act( buf,  ch, NULL, victim, TO_NOTVICT );
    WAIT_STATE( victim, 2 * PULSE_VIOLENCE );
    victim->position = POS_RESTING;
  }
  return;
}

void do_kill( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  
  one_argument( argument, arg );
  if ( IS_NPC(ch) && ch->desc == NULL) return;
  if ( arg[0] == '\0' )
  {
    send_to_char( "Kill whom?\n\r", ch );
    return;
  }
  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    send_to_char( "They aren't here.\n\r", ch );
    return;
  }
  if ( victim == ch )
  {
    send_to_char( "You cannot kill yourself!\n\r", ch );
    return;
  }
  if (is_safe( ch, victim )) return;
  if (IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim)
  {
    act( "$N is your beloved master.", ch, NULL, victim, TO_CHAR );
    return;
  }
  if ( ch->position == POS_FIGHTING )
  {
    send_to_char( "You do the best you can!\n\r", ch );
    return;
  }
  WAIT_STATE( ch, 1 * PULSE_VIOLENCE );
  if(!IS_NPC(ch) && !IS_NPC(victim))
  {
    ch->fight_timer +=3;
    victim->fight_timer +=3;
  }
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF) && number_range(1,3) == 1
  && ch->power[DISC_WERE_BOAR] > 1 && victim->position == POS_STANDING)
  {
    act("You charge into $N, knocking $M from $S feet.",ch,NULL,victim,TO_CHAR);
    act("$n charge into $N, knocking $M from $S feet.",ch,NULL,victim,TO_NOTVICT);
    act("$n charge into you, knocking you from your feet.",ch,NULL,victim,TO_VICT);
    victim->position = POS_STUNNED;
    multi_hit( ch, victim, TYPE_UNDEFINED );
    multi_hit( ch, victim, TYPE_UNDEFINED );
    return;
  }
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ANGEL))
  {
    act("You swoop down from the sky and charge into $N.",ch,NULL,victim,TO_CHAR);
    act("$n swoops down from the sky and charges into $N.",ch,NULL,victim,TO_NOTVICT);
    act("$n swoops down from the sky and charges into you.",ch,NULL,victim,TO_VICT);
    if (ch->pcdata->powers[ANGEL_JUSTICE] >= 5) multi_hit( ch, victim, TYPE_UNDEFINED);
    else if (number_range(1,3)==1) multi_hit( ch, victim, TYPE_UNDEFINED);
  }
  multi_hit( ch, victim, TYPE_UNDEFINED );
  return;
}

void do_backstab( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  OBJ_DATA *obj;

  one_argument( argument, arg );
  if ( arg[0] == '\0' )
  {
    send_to_char( "Backstab whom?\n\r", ch );
    return;
  }
  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    send_to_char( "They aren't here.\n\r", ch );
    return;
  }
  if ( victim == ch )
  {
    send_to_char( "How can you sneak up on yourself?\n\r", ch );
    return;
  }
  if (is_safe( ch, victim )) return;
  if ((( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || obj->value[3] != 11)
      && ((obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || obj->value[3] != 11))
  {
    send_to_char( "You need to wield a piercing weapon.\n\r", ch );
    return;
  }
  if ( victim->fighting != NULL )
  {
    send_to_char( "You can't backstab a fighting person.\n\r", ch );
    return;
  }
  if ( victim->hit < victim->max_hit )
  {
    act( "$N is hurt and suspicious ... you can't sneak up.",
    ch, NULL, victim, TO_CHAR );
    return;
  }
  WAIT_STATE( ch, skill_table[gsn_backstab].beats );
  if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_BACKSTAB) && ch->power[DISC_VAMP_QUIE] < 10 &&
    !IS_CLASS(ch, CLASS_NINJA) && !IS_CLASS(ch, CLASS_DROW))
    damage( ch, victim, 0, gsn_backstab );
  else if (!IS_AWAKE(victim) || IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_backstab])
    multi_hit( ch, victim, gsn_backstab );
  if((ch->pcdata->rank == BELT_SEVEN || ch->pcdata->rank == BELT_EIGHT || ch->pcdata->rank == BELT_NINE 
    || ch->pcdata->rank == BELT_TEN) && number_range(1,3) == 1)
    multi_hit( ch, victim, gsn_backstab );
  if((ch->pcdata->rank == BELT_NINE || ch->pcdata->rank == BELT_TEN) && number_range(1,6) == 1)
    multi_hit( ch, victim, gsn_backstab ); 
  else
    damage( ch, victim, 0, gsn_backstab );
  return;
}

void do_flee( CHAR_DATA *ch, char *argument )
{
  ROOM_INDEX_DATA *was_in;
  ROOM_INDEX_DATA *now_in;
  CHAR_DATA *victim;
  char buf[MAX_STRING_LENGTH];    
  int attempt;

  WAIT_STATE(ch,4); /* to prevent spam fleeing */

  if (IS_EXTRA(ch, TIED_UP))
  {
    send_to_char("Not while tied up.\n\r",ch);
    return;
  }
  if ((victim = ch->fighting ) == NULL)
  {
    if (ch->position == POS_FIGHTING) ch->position = POS_STANDING;
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  if (ch->move <= 0)
  {
    send_to_char("You can't find the energy to leave.\n\r",ch);
    return;
  }
  if (IS_AFFECTED(ch, AFF_WEBBED))
  {
    send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
    return;
  }
  if (IS_SET(ch->flag2, AFF_TOTALBLIND) )
  {
    stc("You are unable to see anything, let alone flee!\n\r",victim);
    return;
  }
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_TANARRI))
  {
    if (ch->pcdata->powers[TANARRI_FURY_ON] == 1)
    {
      stc("Only cowards retreat from combat.\n\r",ch);
      return;
    }
  }
  if (IS_CLASS(victim, CLASS_VAMPIRE) && IS_SET(victim->newbits,NEW_COIL) && number_percent( ) > 30)
  {
    sprintf(buf,"Not with %s coiled around you!",victim->name);
    send_to_char(buf,ch);
    act("$n cant escape with $N coiled around them.",ch,NULL,victim,TO_ROOM);
    return;
  }
  if (!IS_NPC(victim))
  {
    if (IS_CLASS(victim, CLASS_TANARRI) && IS_SET(victim->pcdata->powers[TANARRI_POWER],TANARRI_TENDRILS) && number_percent( ) > 30)
    {
      sprintf(buf,"%s spews a shower of tendrils at you, preventing your escape!",victim->name);
      send_to_char(buf,ch);
      act("$n tries to run, but $N spews forth a shower of tendrils, preventing $n from running.",ch,NULL,victim,TO_ROOM);
      return;
    }
  }
  if (IS_CLASS(victim, CLASS_MAGE) && IS_ITEMAFF(victim,ITEMA_ILLUSIONS) && number_percent( ) > 30)
  {
    sprintf(buf,"Theres to many of %s to escape!",victim->name);
    send_to_char(buf,ch);
    act("$n cant escape with $N blocking his way.",ch,NULL,victim,TO_ROOM);
    return;
  }
  if (IS_CLASS(victim, CLASS_MONK) && IS_SET(victim->newbits,NEW_JAWLOCK) && number_percent( ) > 30)
  {
    sprintf(buf,"Not with god holding you!\n\r");
    send_to_char(buf,ch);
    act("$n cant escape god's mighty hold!",ch,NULL,victim,TO_ROOM);
    return;
  }
  if (IS_CLASS(victim, CLASS_WEREWOLF) && IS_SET(victim->newbits,NEW_JAWLOCK) && number_percent( ) > 30)
  {
    sprintf(buf,"Not with %s's jaws clamped on your neck!",victim->name);
    send_to_char(buf,ch);
    act("$n cant escape $N's clamped jaws!",ch,NULL,victim,TO_ROOM);
    return;
  }
  if (IS_CLASS(victim, CLASS_UNDEAD_KNIGHT) && IS_SET(victim->pcdata->powers[AURAS], BOG_AURA) && number_percent() > 30)
  {
    sprintf(buf,"Your stuck in the swamp surrounding %s!",victim->name);
    send_to_char(buf,ch);
    act("$n is stuck in the swamp!",ch,NULL,victim,TO_ROOM);
    return;
  }
  was_in = ch->in_room;
  for ( attempt = 0; attempt < 6; attempt++ )
  {
    EXIT_DATA *pexit;
    int door;

    door = number_door( );
    if ((pexit = was_in->exit[door] ) == 0 || pexit->to_room == NULL
      || IS_SET(pexit->exit_info, EX_CLOSED) || ( IS_NPC(ch)
      &&   IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB)))
      continue;
    move_char( ch, door );
    if (( now_in = ch->in_room ) == was_in)
      continue;
    ch->in_room = was_in;
    act( "$n has fled!", ch, NULL, NULL, TO_ROOM );
    ch->in_room = now_in;
    if ( !IS_NPC(ch) )
      send_to_char( "You flee from combat!  Coward!\n\r", ch );
    stop_fighting( ch, TRUE );
    return;
  }
  send_to_char( "You were unable to escape!\n\r", ch );
  return;
}

void do_rescue( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  CHAR_DATA *fch;
  
  one_argument( argument, arg );
  if ( arg[0] == '\0' )
  {
    send_to_char( "Rescue whom?\n\r", ch );
    return;
  }
  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    send_to_char( "They aren't here.\n\r", ch );
    return;
  }
  if ( victim == ch )
  {
    send_to_char( "What about fleeing instead?\n\r", ch );
    return;
  }
  if (!IS_NPC(ch) && IS_NPC(victim))
  {
    send_to_char( "Doesn't need your help!\n\r", ch );
    return;
  }
  if ( ch->fighting == victim )
  {
    send_to_char( "Too late.\n\r", ch );
    return;
  }
  if ( ( fch = victim->fighting ) == NULL )
  {
    send_to_char( "That person is not fighting right now.\n\r", ch );
    return;
  }
  if ( is_safe(ch, fch) || is_safe(ch, victim) ) return;
  WAIT_STATE( ch, skill_table[gsn_rescue].beats );
  if ( !IS_NPC(ch) && number_percent( ) > ch->pcdata->learned[gsn_rescue] )
  {
    send_to_char( "You fail the rescue.\n\r", ch );
    return;
  }
  act( "You rescue $N!",  ch, NULL, victim, TO_CHAR    );
  act( "$n rescues you!", ch, NULL, victim, TO_VICT    );
  act( "$n rescues $N!",  ch, NULL, victim, TO_NOTVICT );
  stop_fighting( fch, FALSE );
  stop_fighting( victim, FALSE );
  set_fighting( ch, fch );
  set_fighting( fch, ch );
  do_humanity(ch,"");
  return;
}

void do_kick( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  OBJ_DATA *boots;
  int dam;
  int stance;
  int hitpoints;

  if (!IS_NPC(ch) && ch->level < skill_table[gsn_kick].skill_level)
  {
    send_to_char("First you should learn to kick.\n\r", ch );
    return;
  }
  if ( ( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  WAIT_STATE( ch, skill_table[gsn_kick].beats );
  if ((IS_NPC(ch) && ch->level<1800))
  {
    dam=500;
    damage(ch,victim,dam,gsn_kick);
    return;
  }
  if ((IS_NPC(ch)) ||  number_percent( ) < ch->pcdata->learned[gsn_kick]) dam = number_range(1,4);
  else
  {
    dam = 0;
    damage( ch, victim, dam, gsn_kick );
    return;
  }
  dam += char_damroll(ch);
  if (dam == 0) dam = 1;
  if (!IS_AWAKE(victim)) dam *= 2;
  if(IS_NPC(ch)) dam *= 100;
  if ( !IS_NPC(victim) && IS_CLASS(victim, CLASS_WEREWOLF) )
  {
    if (IS_SET(victim->special, SPC_WOLFMAN)) dam *= 0.8;
    if (victim->power[DISC_WERE_BOAR] > 2 ) dam *= 0.5;
    if ((boots = get_eq_char( ch, WEAR_FEET ) ) != NULL 
    && IS_SET(boots->spectype, SITEM_SILVER)) dam *= 2;
  }
  if (!IS_NPC(ch)) dam = dam + (dam * ((ch->wpn[0]+1) / 100));
  if (!IS_NPC(ch))
  {
    stance = ch->stance[0];
    if ( IS_STANCE(ch, STANCE_NORMAL) ) dam *= 1.25;
    else dam = dambonus(ch,victim,dam,stance);
  }
  if (!IS_NPC(ch) && !IS_NPC(victim) && dam > 750) dam = 750;
  if (dam <= 0) dam = 2;
  dam = cap_dam(ch, victim, dam);
  hitpoints = victim->hit;
  if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_KICK) && !IS_NPC(ch) 
    && !(IS_CLASS(ch, CLASS_DEMON) && IS_DEMAFF(ch, DEM_HOOVES)) && !(IS_CLASS(ch, CLASS_SHAPESHIFTER)
    && ch->pcdata->powers[SHAPE_FORM] == BULL_FORM))
  {
    if (IS_CLASS(ch, CLASS_DEMON) && IS_DEMAFF(ch, DEM_HOOVES))
      dam = dam /2;
    if (IS_NPC(ch))
      dam = dam * 10;
    victim->hit += dam;
    damage( ch, victim, dam, gsn_kick );
    victim->hit = hitpoints;
  }
  else if IS_IMMUNE(victim, IMM_KICK)
  {
    dam=dam/2;
    damage( ch, victim, dam, gsn_kick );
  }
  else damage( ch, victim, dam, gsn_kick );
  return;
}


int dambonus( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int stance)
{
  if (dam < 1) return 0;
  if (stance < 1) return dam;
  if (!IS_NPC(ch) && !can_counter(victim))
  {
    if ( IS_STANCE(ch, STANCE_MONKEY) )
    {
      int mindam = dam * 0.25;
      dam *= (ch->stance[STANCE_MONKEY]+1) / 200;
      if (dam < mindam) dam = mindam;
    }
    else if ( IS_STANCE(ch, STANCE_BULL) && ch->stance[STANCE_BULL] > 100 )
      dam += dam * (ch->stance[STANCE_BULL] / 100);
    else if ( IS_STANCE(ch, STANCE_DRAGON) && ch->stance[STANCE_DRAGON] > 100 )
      dam += dam * (ch->stance[STANCE_DRAGON] / 100);
    else if ( IS_STANCE(ch, STANCE_WOLF) && ch->stance[STANCE_WOLF] > 100)
      dam += dam*(ch->stance[STANCE_WOLF] /100);
    else if ( IS_STANCE(ch, STANCE_TIGER) && ch->stance[STANCE_TIGER] > 100 )
      dam += dam * (ch->stance[STANCE_TIGER] / 100);
/* ------------ This is the part for superstances, Jobo ------------------- */
    else if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_3) 
    && ch->stance[(ch->stance[0])] >100)
      dam += dam * 3 * (ch->stance[(ch->stance[0])] / 100);
    else if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_2)
    && ch->stance[(ch->stance[0])] >100)
      dam += dam * 2 * (ch->stance[(ch->stance[0])] / 100);  
    else if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_1)
    && ch->stance[(ch->stance[0])] >100)
      dam += dam * (ch->stance[(ch->stance[0])] / 100);
/* ------------ This is the end for superstances, Jobo ------------------- */

    else if ( ch->stance[0] > 0 && ch->stance[stance] < 100 )
      dam *= 0.5;
  }
  if (!IS_NPC(victim) && !can_counter(ch))
  {
    if ( IS_STANCE(victim, STANCE_CRAB) && victim->stance[STANCE_CRAB] > 100 )
      dam /= victim->stance[STANCE_CRAB]/100;
    else if ( IS_STANCE(victim, STANCE_DRAGON) && victim->stance[STANCE_DRAGON] > 100)
      dam /= victim->stance[STANCE_DRAGON]/100;
    else if ( IS_STANCE(victim, STANCE_DRAGON) && victim->stance[STANCE_DRAGON] > 100 )
      dam /= victim->stance[STANCE_DRAGON]/100;
    else if ( IS_STANCE(victim, STANCE_SWALLOW) && victim->stance[STANCE_SWALLOW] > 100 )
      dam /= victim->stance[STANCE_SWALLOW]/100;
/* ------------ This is the part for superstances, Jobo ------------------- */
    else if ( victim->stance[0] > 12 && 
    IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_RESIST_3) 
    && victim->stance[(victim->stance[0])] >100)
      dam /= 3 * (victim->stance[(victim->stance[0])] / 100);
    else if ( victim->stance[0] > 12 &&
    IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_RESIST_2)
    && victim->stance[(victim->stance[0])] >100)
      dam /= 2 * (victim->stance[(victim->stance[0])] / 100);
    else if ( victim->stance[0] > 12 &&
    IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_RESIST_1)
    && victim->stance[(victim->stance[0])] >100)
      dam /= (victim->stance[(victim->stance[0])] / 100);
/* ------------ This is the end for superstances, Jobo ------------------- */
  }
  return dam;
}

void do_punch( CHAR_DATA *ch, char *argument )
{
  char arg [MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  int dam;
  int store;
  bool broke = FALSE;

  one_argument(argument,arg);
  if ( IS_NPC(ch) ) return;
  if ( ch->level < skill_table[gsn_punch].skill_level)
  {
    send_to_char( "First you should learn to punch.\n\r", ch );
    return;
  }
  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    send_to_char( "They are not here.\n\r", ch );
    return;
  }
  if ( ch == victim )
  {
    send_to_char( "You cannot punch yourself!\n\r", ch );
    return;
  }
  if (is_safe(ch,victim)) return;
  if ( victim->hit < victim->max_hit )
  {
    send_to_char( "They are hurt and suspicious.\n\r", ch );
    return;
  }
  if ( victim->position < POS_FIGHTING )
  {
    send_to_char( "You can only punch someone who is standing.\n\r", ch );
    return;
  }
  act("You draw your fist back and aim a punch at $N.",ch,NULL,victim,TO_CHAR);
  act("$n draws $s fist back and aims a punch at you.",ch,NULL,victim,TO_VICT);
  act("$n draws $s fist back and aims a punch at $N.",ch,NULL,victim,TO_NOTVICT);
  WAIT_STATE( ch, skill_table[gsn_punch].beats );
  if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_punch] )
    dam = number_range(1,4);
  else
  {
    dam = 0;
    damage( ch, victim, dam, gsn_punch );
    return;
  }
  dam += char_damroll(ch);
  if (dam == 0) dam = 1;
  if (!IS_AWAKE(victim)) dam *= 2;
  if ( !IS_NPC(ch) ) dam = dam + (dam * (ch->wpn[0] / 100));
  if ( dam <= 0 ) dam = 1;
  if (dam > 1000) dam = 1000;
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_WEREWOLF) && victim->power[DISC_WERE_BOAR] > 3)
  {
    store = victim->hit;
    victim->hit += dam;
    damage( ch, victim, dam, gsn_punch );
    victim->hit = store;
    if (number_percent() <= 25 && !IS_ARM_L(ch,LOST_ARM) && !IS_ARM_L(ch,LOST_HAND))
    {
      if (!IS_ARM_L(ch, LOST_FINGER_I) && !IS_ARM_L(ch, BROKEN_FINGER_I))
        {SET_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_I);broke = TRUE;}
      if (!IS_ARM_L(ch, LOST_FINGER_M) && !IS_ARM_L(ch, BROKEN_FINGER_M))
        {SET_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_M);broke = TRUE;}
      if (!IS_ARM_L(ch, LOST_FINGER_R) && !IS_ARM_L(ch, BROKEN_FINGER_R))
        {SET_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_R);broke = TRUE;}
      if (!IS_ARM_L(ch, LOST_FINGER_L) && !IS_ARM_L(ch, BROKEN_FINGER_L))
        {SET_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_L);broke = TRUE;}
      if (broke)
      {
        act("The fingers on your left hand shatter under the impact of the blow!",ch,NULL,NULL,TO_CHAR);
        act("The fingers on $n's left hand shatter under the impact of the blow! ",ch,NULL,NULL,TO_ROOM);
      }
    }
    else if (number_percent() <= 25 && !IS_ARM_R(ch,LOST_ARM) && !IS_ARM_R(ch,LOST_HAND))
    {
      if (!IS_ARM_R(ch, LOST_FINGER_I) && !IS_ARM_R(ch, BROKEN_FINGER_I))
        {SET_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_I);broke = TRUE;}
      if (!IS_ARM_R(ch, LOST_FINGER_M) && !IS_ARM_R(ch, BROKEN_FINGER_M))
        {SET_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_M);broke = TRUE;}
      if (!IS_ARM_R(ch, LOST_FINGER_R) && !IS_ARM_R(ch, BROKEN_FINGER_R))
        {SET_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_R);broke = TRUE;}
      if (!IS_ARM_R(ch, LOST_FINGER_L) && !IS_ARM_R(ch, BROKEN_FINGER_L))
        {SET_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_L);broke = TRUE;}
      if (broke)
      {
        act("The fingers on your right hand shatter under the impact of the blow!",ch,NULL,NULL,TO_CHAR);
        act("The fingers on $n's right hand shatter under the impact of the blow! ",ch,NULL,NULL,TO_ROOM);
      }
    }
    stop_fighting(victim,TRUE);
    return;
  }
  damage( ch, victim, dam, gsn_punch );
  if (victim == NULL || victim->position == POS_DEAD || dam < 1) return;
  if (victim->position == POS_FIGHTING) stop_fighting(victim,TRUE);
  if (number_percent() <= 25 && !IS_HEAD(victim,BROKEN_NOSE) && !IS_HEAD(victim,LOST_NOSE))
  {
    act("Your nose shatters under the impact of the blow!",victim,NULL,NULL,TO_CHAR);
    act("$n's nose shatters under the impact of the blow!",victim,NULL,NULL,TO_ROOM);
    SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_NOSE);
  }
  else if (number_percent() <= 25 && !IS_HEAD(victim,BROKEN_JAW))
  {
    act("Your jaw shatters under the impact of the blow!",victim,NULL,NULL,TO_CHAR);
    act("$n's jaw shatters under the impact of the blow!",victim,NULL,NULL,TO_ROOM);
    SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_JAW);
  }
  act("You fall to the ground stunned!",victim,NULL,NULL,TO_CHAR);
  act("$n falls to the ground stunned!",victim,NULL,NULL,TO_ROOM);
  victim->position = POS_STUNNED;
  if (dam > 1000) dam = 1000;
  return;
}

void do_berserk(CHAR_DATA *ch, char *argument)
{
  char arg [MAX_INPUT_LENGTH];
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  CHAR_DATA *mount;
  int number_hit = 0;

  argument = one_argument( argument, arg );
  if (IS_NPC(ch)) return;
  if ( ch->level < skill_table[gsn_berserk].skill_level)
  {
    send_to_char( "You are not wild enough to go berserk.\n\r", ch );
    return;
  }
  WAIT_STATE( ch, 24);
  if ( number_percent( ) > ch->pcdata->learned[gsn_berserk] )
  {
    act("You rant and rave, but nothing much happens.",ch,NULL,NULL,TO_CHAR);
    act("$n gets a wild look in $s eyes, but nothing much happens.",ch,NULL,NULL,TO_ROOM);
    return;
  }
  act("You go BERSERK!",ch,NULL,NULL,TO_CHAR);
  act("$n goes BERSERK!",ch,NULL,NULL,TO_ROOM);
  for ( vch = char_list; vch != NULL; vch = vch_next )
  {
    vch_next  = vch->next;
    if (number_hit > 4) continue;
    if (vch->in_room == NULL) continue;
    if (!IS_NPC(vch)) continue;
    if ( ch == vch ) continue;
    if ( vch->in_room == ch->in_room )
    {
      if ( ( mount = ch->mount ) != NULL )
        if ( mount == vch ) continue;
      if (can_see(ch,vch))
      {
        multi_hit( ch, vch, TYPE_UNDEFINED );
        number_hit++;
      }
    }  
  }    
  return;
}

void do_berserk2( CHAR_DATA *ch, char *argument )
{
  char arg [MAX_INPUT_LENGTH];
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  CHAR_DATA *mount;
  int number_hit = 0;

  argument = one_argument( argument, arg );
  if (IS_NPC(ch)) return;
  if ( ch->level < skill_table[gsn_berserk].skill_level)
  {
    send_to_char( "You are not wild enough to go berserk.\n\r", ch );
    return;
  }
  WAIT_STATE( ch, skill_table[gsn_berserk].beats );
  if ( number_percent( ) > ch->pcdata->learned[gsn_berserk] )
  {
    act("You rant and rave, but nothing much happens.",ch,NULL,NULL,TO_CHAR);
    act("$n gets a wild look in $s eyes, but nothing much happens.",ch,NULL,NULL,TO_ROOM);
    return;
  }
  act("You go BERSERK!",ch,NULL,NULL,TO_CHAR);
  act("$n goes BERSERK!",ch,NULL,NULL,TO_ROOM);
  for ( vch = char_list; vch != NULL; vch = vch_next )
  {
    vch_next  = vch->next;
    if (number_hit > 4) continue;
    if (vch->in_room == NULL) continue;
    if (!IS_NPC(vch) && vch->pcdata->chobj != NULL) continue;
    if ( ch == vch ) continue;
    if ( vch->in_room == ch->in_room ) 
    {
      if ( ( mount = ch->mount ) != NULL ) 
        if ( mount == vch ) continue;
      if (can_see(ch,vch))
      {
        multi_hit( ch, vch, TYPE_UNDEFINED );
        number_hit++;
      }
    }
  }
  do_beastlike(ch,"");
  return;
}
       
/* Hurl skill by KaVir */
void do_hurl( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  CHAR_DATA *mount;
  ROOM_INDEX_DATA *to_room;
  EXIT_DATA *pexit;
  EXIT_DATA *pexit_rev;
  char buf [MAX_INPUT_LENGTH];
  char direction [MAX_INPUT_LENGTH];
  char arg1 [MAX_INPUT_LENGTH];
  char arg2 [MAX_INPUT_LENGTH];
  int door;
  int rev_dir;
  int dam;

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );
  if ( !IS_NPC(ch) && ch->pcdata->learned[gsn_hurl] < 1 )
  {
    send_to_char("Maybe you should learn the skill first?\n\r",ch);
    return;
  }
  if ( arg1[0] == '\0' )
  {
    send_to_char("Who do you wish to hurl?\n\r", ch);
    return;
  }
  if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if ( victim == ch )
  {
    send_to_char( "How can you hurl yourself?\n\r", ch );
    return;
  }
  if (is_safe( ch, victim )) return;
  if ( (mount = victim->mount) != NULL && victim->mounted == IS_MOUNT)
  {
    send_to_char("But they have someone on their back!\n\r",ch);
    return;
  }
  else if ( (mount = victim->mount) != NULL && victim->mounted == IS_RIDING)
  {
    send_to_char("But they are riding!\n\r",ch);
    return;
  }
  if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_HURL) )
  {
    send_to_char("You are unable to get their feet of the ground.\n\r",ch);
    return;
  }
  if ( IS_NPC(victim) && victim->level > 900 )
  {
    send_to_char("You are unable to get their feet of the ground.\n\r",ch);
    return;
  }
  if ( (victim->hit < victim->max_hit)
     || ( victim->position == POS_FIGHTING && victim->fighting != ch) )
  {
    act( "$N is hurt and suspicious, and you are unable to approach $M.", ch, NULL, victim, TO_CHAR );
    return;
  }
  WAIT_STATE( ch, skill_table[gsn_hurl].beats );
  if ( !IS_NPC(ch) && number_percent( ) > ch->pcdata->learned[gsn_hurl] )
  {
    send_to_char("You are unable to get their feet of the ground.\n\r",ch);
    multi_hit( victim, ch, TYPE_UNDEFINED );
    return;
  }
  rev_dir = 0;
  if ( arg2[0] == '\0' ) door = number_range(0,3);
  else
  {
    if      (!str_cmp( arg2, "n" ) || !str_cmp( arg2, "north")) door = 0;
    else if (!str_cmp( arg2, "e" ) || !str_cmp( arg2, "east")) door = 1;
    else if (!str_cmp( arg2, "s" ) || !str_cmp( arg2, "south")) door = 2;
    else if (!str_cmp( arg2, "w" ) || !str_cmp( arg2, "west")) door = 3;
    else
    {
      send_to_char("You can only hurl people north, south, east or west.\n\r", ch);
      return;
    }
  }
  if (door == 0) {sprintf(direction,"north");rev_dir = 2;}
  if (door == 1) {sprintf(direction,"east");rev_dir = 3;}
  if (door == 2) {sprintf(direction,"south");rev_dir = 0;}
  if (door == 3) {sprintf(direction,"west");rev_dir = 1;}
  if (( pexit = ch->in_room->exit[door]) == NULL || (to_room = pexit->to_room) == NULL)
  {
    sprintf(buf,"$n hurls $N into the %s wall.", direction);
    act(buf,ch,NULL,victim,TO_NOTVICT);
    sprintf(buf,"You hurl $N into the %s wall.", direction);
    act(buf,ch,NULL,victim,TO_CHAR);
    sprintf(buf,"$n hurls you into the %s wall.", direction);
    act(buf,ch,NULL,victim,TO_VICT);
    dam = number_range(ch->level, (ch->level * 4));
    victim->hit = victim->hit - dam;
    update_pos(victim);
    if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill = ch->mkill + 1;
    if (!IS_NPC(victim) && IS_NPC(ch)) victim->mdeath = victim->mdeath + 1;
    if (victim->position == POS_DEAD) {raw_kill(victim);return;}
    return;
  }
  pexit = victim->in_room->exit[door];
  if( IS_SET(pexit->exit_info, EX_PRISMATIC_WALL))
  {
    sprintf(buf,"$n hurls $N into the %s wall.", direction);
    act(buf,ch,NULL,victim,TO_NOTVICT);
    sprintf(buf,"You hurl $N into the %s wall.", direction);
    act(buf,ch,NULL,victim,TO_CHAR);
    sprintf(buf,"$n hurls you into the %s wall.", direction);
    act(buf,ch,NULL,victim,TO_VICT);
    dam = number_range(ch->level, (ch->level * 4));
    victim->hit = victim->hit - dam;
    update_pos(victim);
    if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill = ch->mkill + 1;
    if (!IS_NPC(victim) && IS_NPC(ch)) victim->mdeath = victim->mdeath + 1;
    if (victim->position == POS_DEAD) {raw_kill(victim);return;}
    return;
  }
  if (IS_SET(pexit->exit_info, EX_CLOSED) && !IS_AFFECTED(victim,AFF_PASS_DOOR) && !IS_AFFECTED(victim,AFF_ETHEREAL))
  {
    if (IS_SET(pexit->exit_info, EX_LOCKED))
      REMOVE_BIT(pexit->exit_info, EX_LOCKED);
    if (IS_SET(pexit->exit_info, EX_CLOSED))
      REMOVE_BIT(pexit->exit_info, EX_CLOSED);
    sprintf(buf,"$n hoists $N in the air and hurls $M %s.", direction);
    act(buf,ch,NULL,victim,TO_NOTVICT);
    sprintf(buf,"You hoist $N in the air and hurl $M %s.", direction);
    act(buf,ch,NULL,victim,TO_CHAR);
    sprintf(buf,"$n hurls you %s, smashing you through the %s.", direction,pexit->keyword);
    act(buf,ch,NULL,victim,TO_VICT);
    sprintf(buf,"There is a loud crash as $n smashes through the $d.");
    act(buf,victim,NULL,pexit->keyword,TO_ROOM);
    if ((to_room = pexit->to_room ) != NULL && (pexit_rev = to_room->exit[rev_dir]) != NULL
      && pexit_rev->to_room == ch->in_room && pexit_rev->keyword != NULL)
    {
      if (IS_SET(pexit_rev->exit_info, EX_LOCKED))
        REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
      if (IS_SET(pexit_rev->exit_info, EX_CLOSED))
        REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
      if (door == 0) sprintf(direction,"south");
      if (door == 1) sprintf(direction,"west");
      if (door == 2) sprintf(direction,"north");
      if (door == 3) sprintf(direction,"east");
      char_from_room(victim);
      char_to_room(victim,to_room);
      sprintf(buf,"$n comes smashing in through the %s $d.", direction);
      act(buf,victim,NULL,pexit->keyword,TO_ROOM);
      dam = number_range(ch->level, (ch->level * 6));
      victim->hit = victim->hit - dam;
      update_pos(victim);
      if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill = ch->mkill + 1;
      if (!IS_NPC(victim) && IS_NPC(ch)) victim->mdeath = victim->mdeath + 1;
      if (victim->position == POS_DEAD) {raw_kill(victim);return;}
    }
  }
  else
  {
    sprintf(buf,"$n hurls $N %s.", direction);
    act(buf,ch,NULL,victim,TO_NOTVICT);
    sprintf(buf,"You hurl $N %s.", direction);
    act(buf,ch,NULL,victim,TO_CHAR);
    sprintf(buf,"$n hurls you %s.", direction);
    act(buf,ch,NULL,victim,TO_VICT);
    if (door == 0) sprintf(direction,"south");
    if (door == 1) sprintf(direction,"west");
    if (door == 2) sprintf(direction,"north");
    if (door == 3) sprintf(direction,"east");
    char_from_room(victim);
    char_to_room(victim,to_room);
    sprintf(buf,"$n comes flying in from the %s.", direction);
    act(buf,victim,NULL,NULL,TO_ROOM);
    dam = number_range(ch->level, (ch->level * 2));
    victim->hit = victim->hit - dam;
    update_pos(victim);
    if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill = ch->mkill + 1;
    if (!IS_NPC(victim) && IS_NPC(ch)) victim->mdeath = victim->mdeath + 1;
    if (victim->position == POS_DEAD) {raw_kill(victim);return;}
  }
  return;
}

void do_disarm( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int percent;

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_disarm].skill_level)
    {
  send_to_char( "You don't know how to disarm opponents.\n\r", ch );
  return;
    }

    if ( ( get_eq_char( ch, WEAR_WIELD ) == NULL )
    &&   ( get_eq_char( ch, WEAR_HOLD  ) == NULL ) )
    {
  send_to_char( "You must wield a weapon to disarm.\n\r", ch );
  return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
  send_to_char( "You aren't fighting anyone.\n\r", ch );
  return;
    }

    if ( ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL )
    &&   ( ( obj = get_eq_char( victim, WEAR_HOLD  ) ) == NULL ) )
    {
  send_to_char( "Your opponent is not wielding a weapon.\n\r", ch );
  return;
    }

    WAIT_STATE( ch, skill_table[gsn_disarm].beats );
    percent = number_percent( ) + victim->level - ch->level;
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_DISARM))
  send_to_char( "You failed.\n\r", ch );
    else if ( IS_NPC(ch) || percent < ch->pcdata->learned[gsn_disarm] * 2 / 3 )
  disarm( ch, victim );
    else
  send_to_char( "You failed.\n\r", ch );
    return;
}



void do_sla( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to SLAY, spell it out.\n\r", ch );
    return;
}

/* Had problems with people not dying when POS_DEAD...KaVir */
void do_killperson( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  one_argument( argument, arg );
  if ( arg[0] == '\0' ) return;
  if ( ( victim = get_char_room( ch, arg ) ) == NULL ) return;
  send_to_char( "You have been KILLED!!\n\r\n\r", victim );
  if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill = ch->mkill + 1;
  if (!IS_NPC(victim) && IS_NPC(ch)) victim->mdeath = victim->mdeath + 1;
  raw_kill( victim );
  return;
}

/* For decapitating players - KaVir */
void do_decapitate( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  bool can_decap = TRUE;
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;
  one_argument( argument, arg );

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
    send_to_char( "Decapitate whom?\n\r", ch );
    return;
  }
  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    send_to_char( "They aren't here.\n\r", ch );
    return;
  }
  if ( ch == victim )
  {
    send_to_char( "That might be a bit tricky...\n\r", ch );
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char( "You can only decapitate other players.\n\r", ch );
    return;
  }
  if ( victim->position > 1 )
  {
    send_to_char( "You can only do this to mortally wounded players.\n\r", ch );
    return;
  }
  if (is_safe(ch,victim)) return;
  if (ragnarok)
  {
    ragnarokdecap(ch,victim);
    return;
  }
  if (reachedDecapLimit(ch)) 
  {
    send_to_char("You have reached the decap limit for your current size, you need to gain more hps.\n\r",ch);
    return;
  }  
  /*
   * can't decide, one or two... sigh - Jobo
   *  if (!str_cmp(ch->pcdata->last_decap[0], victim->name) || !str_cmp(ch->pcdata->last_decap[1], victim->name))
   */
  if (!str_cmp(ch->pcdata->last_decap[0], victim->name))
  {
    send_to_char("Don't spamcap, it's not nice, find someone else to pick on.\n\r",ch);
    return;
  }
  if (!fair_fight(ch, victim)) can_decap = FALSE;
  if (ch->level > 6 && can_decap == FALSE) can_decap = TRUE;
  if (!can_decap)
  {
      if (IS_CLASS(victim, CLASS_VAMPIRE))           do_mortalvamp(victim,"");
      else if (IS_SET(victim->special, SPC_WOLFMAN)) do_unwerewolf(victim,"");
      behead( victim );
      victim->level = 2;
      dropinvis(ch);
      players_decap++;
      ch->pcdata->mean_paradox_counter++;
      ch->pcdata->bounty += number_range(60,120);
      sprintf(buf,"#P%s #owas torn to pieces by #R%s #c(#0Paradox Counter#c)#n",victim->name, ch->name);
      death_info(buf);
      if (ch->pcdata->mean_paradox_counter > 2)
      {
        ch->pcdata->mean_paradox_counter = 0;
        do_paradox(ch,"self");
      }
      return;
  }

  if (victim->pcdata->bounty > 0)
  {
    sprintf(buf,"You recive a %d QP bounty, for killing %s.\n\r", victim->pcdata->bounty, victim->name);
    send_to_char(buf, ch);
    ch->pcdata->quest += victim->pcdata->bounty;
    victim->pcdata->bounty =0;
  }

  ch->exp += victim->exp / 2;
  victim->exp -= victim->exp / 2;

  /*
   * Update the last decaps to prevent spamcapping.
   */
  free_string( ch->pcdata->last_decap[1] );
  ch->pcdata->last_decap[1] = str_dup(ch->pcdata->last_decap[0]);
  free_string( ch->pcdata->last_decap[0] );
  ch->pcdata->last_decap[0] = str_dup(victim->name);

  act( "A misty white vapour pours from $N's corpse into your body.",  ch, NULL, victim, TO_CHAR    );
  act( "A misty white vapour pours from $N's corpse into $n's body.",  ch, NULL, victim, TO_NOTVICT );
  act( "You double over in agony as raw energy pulses through your veins.",  ch, NULL, NULL, TO_CHAR    );
  act( "$n doubles over in agony as sparks of energy crackle around $m.",  ch, NULL, NULL, TO_NOTVICT );
  if (IS_CLASS(victim, CLASS_VAMPIRE))      do_mortalvamp(victim,"");
  else if (IS_SET(victim->special, SPC_WOLFMAN)) do_unwerewolf(victim,"");
  ch->fight_timer += 10;
  if (IS_CLASS(ch, CLASS_DEMON))
  {
    send_to_char("You rip away a part of their soul.\n\r", ch);
    send_to_char("You feel a part of your soul has been torn away.\n\r", victim);
    ch->pcdata->souls += 1;
  }
  behead( victim );
  dropinvis(ch);
  /*  Removes mage powers */
  if (IS_ITEMAFF(victim, ITEMA_STEELSHIELD)) REMOVE_BIT(ch->itemaffect, ITEMA_STEELSHIELD);
  if (IS_ITEMAFF(victim, ITEMA_BEAST)) REMOVE_BIT(victim->itemaffect, ITEMA_BEAST);
  if (IS_ITEMAFF(victim, ITEMA_MAGESHIELD)) REMOVE_BIT(victim->itemaffect, ITEMA_MAGESHIELD);
  if (IS_ITEMAFF(victim, ITEMA_ILLUSIONS)) REMOVE_BIT(victim->itemaffect, ITEMA_ILLUSIONS);
  if (IS_ITEMAFF(victim, ITEMA_DEFLECTOR)) REMOVE_BIT(victim->itemaffect, ITEMA_DEFLECTOR);
  do_beastlike(ch,"");
  if (ch->pcdata->mean_paradox_counter > 0) ch->pcdata->mean_paradox_counter--;
  ch->pkill = ch->pkill + 1;
  victim->pdeath = victim->pdeath + 1;
  if (!IS_CLASS(victim, CLASS_NINJA)) victim->rage = 0;
  victim->level = 2;
  decap_message(ch,victim);
  sprintf( log_buf, "%s decapitated by %s at %d.",
  victim->pcdata->switchname, ch->pcdata->switchname, victim->in_room->vnum );
  log_string( log_buf );
  players_decap++;
  ch->pcdata->bounty += number_range(30,80);

  /*
   * update kingdoms
   */
  if (ch->pcdata->kingdom != 0)
    kingdom_table[ch->pcdata->kingdom].kills++;
  if (victim->pcdata->kingdom != 0)
    kingdom_table[victim->pcdata->kingdom].deaths++;

  return;
}

void decap_message(CHAR_DATA *ch, CHAR_DATA *victim)
{
  OBJ_DATA  *obj;
  char buf[MAX_STRING_LENGTH];
  bool unarmed = FALSE;

  if (IS_NPC(ch) || IS_NPC(victim)) return;
  if ( (obj = get_eq_char( ch, WEAR_WIELD )) == NULL )
  {
    if (( obj = get_eq_char( ch, WEAR_HOLD )) == NULL )
    {
      unarmed = TRUE;
    }
  }
  if ((obj != NULL && obj->item_type != ITEM_WEAPON)) unarmed = TRUE;

  /* The players own decap message */
  if (IS_SET(ch->pcdata->jflags, JFLAG_SETDECAP))
  {
    special_decap_message(ch,victim);
    return;
  }

  if (unarmed)
  {
    if (victim->sex == SEX_MALE)        sprintf(buf,"#P%s #ygot his head torn off by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
    else if (victim->sex == SEX_FEMALE) sprintf(buf,"#P%s #ygot her head torn off by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
    else                                sprintf(buf,"#P%s #ygot its head torn off by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
  }
  else if (obj->value[3] == 1)
  {
    if (victim->sex == SEX_MALE)        sprintf(buf,"#P%s #ygot his head sliced off by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
    else if (victim->sex == SEX_FEMALE) sprintf(buf,"#P%s #ygot her head sliced off by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
    else                                sprintf(buf,"#P%s #ygot its head sliced off by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
  }
  else if (obj->value[3] == 2)
  {
    if (victim->sex == SEX_MALE)        sprintf(buf,"#P%s #ygot his heart stabbed through by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
    else if (victim->sex == SEX_FEMALE) sprintf(buf,"#P%s #ygot her heart stabbed through by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
    else                                sprintf(buf,"#P%s #ygot its heart stabbed through by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
  }
  else if (obj->value[3] == 3)
  {
    if (victim->sex == SEX_MALE)        sprintf(buf,"#P%s #ygot his head slashed off by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
    else if (victim->sex == SEX_FEMALE) sprintf(buf,"#P%s #ygot her head slashed off by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
    else                                sprintf(buf,"#P%s #ygot its head slashed off by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
  }
  else if (obj->value[3] == 4) sprintf(buf,"#P%s #ygot strangled by #R%s",victim->pcdata->switchname,ch->pcdata->switchname);
  else if (obj->value[3] == 5) sprintf(buf,"#R%s #yruns a clawed hand through #P%s #yand pulls out the heart#n",ch->pcdata->switchname,victim->pcdata->switchname);
  else if (obj->value[3] == 6) sprintf(buf,"#R%s #yshoots #P%s #yseveral times and spits on the corpse#n",ch->pcdata->switchname,victim->pcdata->switchname);
  else if (obj->value[3] == 7) sprintf(buf,"#R%s #ypounds #P%s #yon the head and the skull caves in#n",ch->pcdata->switchname,victim->pcdata->switchname);
  else if (obj->value[3] == 8) sprintf(buf,"#R%s #ycrushes #P%s #yto a bloody pulp#n",ch->pcdata->switchname,victim->pcdata->switchname);
  else if (obj->value[3] == 9) sprintf(buf,"#P%s #yhas been grepped by #R%s#y, that's just mean!#n",victim->pcdata->switchname,ch->pcdata->switchname);
  else if (obj->value[3] == 10) sprintf(buf,"#P%s #ywas bitten to death by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
  else if (obj->value[3] == 11) sprintf(buf,"#R%s #yhas punctured the lungs of #P%s#y, what a meanie!#n",ch->pcdata->switchname,victim->pcdata->switchname);
  else if (obj->value[3] == 12) sprintf(buf,"#R%s #ygrabs #P%s #yby the head and sucks the brain out#n",ch->pcdata->switchname,victim->pcdata->switchname);
  else
  {
    if (victim->sex == SEX_MALE)        sprintf(buf,"#P%s #ygot his head sliced off by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
    else if (victim->sex == SEX_FEMALE) sprintf(buf,"#P%s #ygot her head sliced off by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
    else                                sprintf(buf,"#P%s #ygot its head sliced off by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
  }
  death_info(buf);
  return;
}

void do_crack( CHAR_DATA *ch, char *argument )
{
  OBJ_DATA  *obj;
  OBJ_DATA  *right;
  OBJ_DATA  *left;

  right = get_eq_char( ch, WEAR_WIELD );
  left = get_eq_char( ch, WEAR_HOLD );
  if (right != NULL && right->pIndexData->vnum == 12) obj = right;
  else if (left != NULL && left->pIndexData->vnum == 12) obj = left;
  else
  {
    send_to_char("You are not holding any heads.\n\r",ch);
    return;
  }
  act("You hurl $p at the floor.", ch, obj, NULL, TO_CHAR);
  act("$n hurls $p at the floor.", ch, obj, NULL, TO_ROOM);
  act("$p cracks open, leaking brains out across the floor.", ch, obj, NULL, TO_CHAR);
  if (obj->chobj != NULL)
  {
    act("$p cracks open, leaking brains out across the floor.", ch, obj, obj->chobj, TO_NOTVICT);
    act("$p crack open, leaking brains out across the floor.", ch, obj, obj->chobj, TO_VICT);
  }
  else
  {
    act("$p cracks open, leaking brains out across the floor.", ch, obj, NULL, TO_ROOM);
  }
  crack_head(ch,obj,obj->name);
  obj_from_char(obj);
  extract_obj(obj);
}

void crack_head( CHAR_DATA *ch, OBJ_DATA *obj, char *argument )
{
  CHAR_DATA *victim;
  MOB_INDEX_DATA *pMobIndex; 

  char arg1 [MAX_INPUT_LENGTH];
  char arg2 [MAX_INPUT_LENGTH];
  char buf [MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );
  if (str_cmp(arg2,"mob") && obj->chobj != NULL && !IS_NPC(obj->chobj) && IS_AFFECTED(obj->chobj,AFF_POLYMORPH))
  {
    victim = obj->chobj;
    make_part(victim,"cracked_head");
    make_part(victim,"brain");
    sprintf(buf,"the quivering brain of %s",victim->name);
    free_string(victim->morph);
    victim->morph = str_dup(buf);
    return;
  }
  else if (!str_cmp(arg2,"mob"))
  {
    if ( ( pMobIndex = get_mob_index( obj->value[1] ) ) == NULL ) return;
    victim = create_mobile( pMobIndex );
    char_to_room(victim,ch->in_room);
    make_part(victim,"cracked_head");
    make_part(victim,"brain");
    extract_char(victim,TRUE);
    return;
  }
  else
  {
    if ( ( pMobIndex = get_mob_index( 30002 ) ) == NULL ) return;
    victim = create_mobile( pMobIndex );
    sprintf( buf, capitalize(arg2) );
    free_string( victim->short_descr );
    victim->short_descr = str_dup( buf );
    char_to_room(victim,ch->in_room);
    make_part(victim,"cracked_head");
    make_part(victim,"brain");
    extract_char(victim,TRUE);
    return;
  }
  return;
}

/* Voodoo skill by KaVir */

void do_voodoo( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  OBJ_DATA  *obj;
  char arg1 [MAX_INPUT_LENGTH];
  char arg2 [MAX_INPUT_LENGTH];
  char buf [MAX_INPUT_LENGTH];
  char part1 [MAX_INPUT_LENGTH];
  char part2 [MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );
  if ( arg1[0] == '\0')
  {
    send_to_char( "Who do you wish to use voodoo magic on?\n\r", ch );
    return;
  }
  if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL )
  {
    send_to_char( "You are not holding a voodoo doll.\n\r", ch );
    return;
  }
  if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
  {
    send_to_char( "They are not here.\n\r", ch );
    return;
  }
  if ( IS_NPC(victim) )
  {
    send_to_char( "Not on NPC's.\n\r", ch );
    return;
  }
  sprintf(part2,obj->name);
  sprintf(part1,"%s voodoo doll",victim->name);
  if ( str_cmp(part1,part2) )
  {
    sprintf(buf,"But you are holding %s, not %s!\n\r",obj->short_descr,victim->name);
    send_to_char( buf, ch );
    return;
  }
  if ( arg2[0] == '\0')
    send_to_char( "You can 'stab', 'burn' or 'throw' the doll.\n\r", ch );
  else if ( !str_cmp(arg2, "stab") )
  {
    WAIT_STATE(ch,12);
    act("You stab a pin through $p.", ch, obj, NULL, TO_CHAR); 
    act("$n stabs a pin through $p.", ch, obj, NULL, TO_ROOM);
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_VOODOO)) return;
    act("You feel an agonising pain in your chest!", victim, NULL, NULL, TO_CHAR);
    act("$n clutches $s chest in agony!", victim, NULL, NULL, TO_ROOM);
  }
  else if ( !str_cmp(arg2, "burn") )
  {
    WAIT_STATE(ch,12);
    act("You set fire to $p.", ch, obj, NULL, TO_CHAR);
    act("$n sets fire to $p.", ch, obj, NULL, TO_ROOM);
    act("$p burns to ashes.", ch, obj, NULL, TO_CHAR);
    act("$p burns to ashes.", ch, obj, NULL, TO_ROOM);
    obj_from_char(obj);
    extract_obj(obj);
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_VOODOO)) return;
    if (IS_AFFECTED(victim,AFF_FLAMING) )
      return;
    SET_BIT(victim->affected_by, AFF_FLAMING);
    act("You suddenly burst into flames!", victim, NULL, NULL, TO_CHAR);
    act("$n suddenly bursts into flames!", victim, NULL, NULL, TO_ROOM);
  }
  else if ( !str_cmp(arg2, "throw") )
  {
    WAIT_STATE(ch,12);
    act("You throw $p to the ground.", ch, obj, NULL, TO_CHAR);
    act("$n throws $p to the ground.", ch, obj, NULL, TO_ROOM);
    obj_from_char(obj);
    obj_to_room(obj, ch->in_room);
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_VOODOO)) return;
    if (victim->position < POS_STANDING) return;
    if (victim->position == POS_FIGHTING)
      stop_fighting(victim, TRUE);
    act("A strange force picks you up and hurls you to the ground!", victim, NULL, NULL, TO_CHAR);
    act("$n is hurled to the ground by a strange force.", victim, NULL, NULL, TO_ROOM);
    victim->position = POS_RESTING;
    victim->hit = victim->hit - number_range(ch->level,(5*ch->level));
    update_pos(victim);
    if (victim->position == POS_DEAD && !IS_NPC(victim))
    {
      do_killperson(ch,victim->name);
      return;
    }
  }
  else
    send_to_char( "You can 'stab', 'burn' or 'throw' the doll.\n\r", ch );
  return;
}

void do_garotte(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  OBJ_DATA *obj;
  int chance;

  one_argument(argument, arg);
  chance = number_percent();

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_GAROTTE)) 
  {
    send_to_char("Huh?\n\r", ch );
    return;
  }
  if ((victim = get_char_room(ch, arg)) == NULL) 
  {
    send_to_char("Garotte whom?\n\r", ch );
    return;
  }
  if (victim == ch) 
  {
    send_to_char("That would be a bad idea.\n\r", ch );
    return;
  }
  if (is_safe(ch, victim)) return;
  if (((obj = get_eq_char(ch, WEAR_WIELD)) == NULL || obj->value[3] != 4) 
    && ((obj = get_eq_char(ch, WEAR_HOLD)) == NULL|| obj->value[3] != 4))
  {
    send_to_char( "You need to wield a whip.\n\r", ch );
    return;
  }
  WAIT_STATE(ch, 12);
  if (chance <= 5)
  {
    send_to_char("You cannot get close enough.\n\r",ch);
    return;
  }
  if ( chance == 10 || chance == 15 || chance == 20 || chance == 25) 
  {
    act("$n wraps a whip around $N's neck! $N is choking to death!",ch,NULL,victim,TO_NOTVICT);
    act("You wrap your whip around $N's neck and refuse to let go!",ch,NULL,victim,TO_CHAR);
    act("$n wraps his whip around your neck, and you can't break free!",ch,NULL,victim,TO_VICT);
    if (!IS_NPC(victim)) 
    {
      one_hit(ch, victim, gsn_garotte,1);
      one_hit(ch, victim, gsn_garotte,1);
      one_hit(ch, victim, gsn_garotte,1);
      one_hit(ch, victim, gsn_garotte,1);
    } 
    else
    {
      victim->hit = 1;
      damage(ch, victim, 1000, gsn_garotte);
    }
    return;
  }
  else
  {
    one_hit(ch, victim, gsn_garotte,1);
    return;
  }
  return;
}

void do_dark_garotte(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  OBJ_DATA *obj;
  int chance;

  one_argument(argument, arg);
  chance = number_percent();
  if (IS_NPC(ch)) return;
  if (ch->in_room != NULL)
  {
    if (IS_SET (ch->in_room->room_flags, ROOM_ARENA))
    {
      send_to_char("Your in the arena.\n\r",ch);
      return;
    }
  }
  if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_GAROTTE)
    || !IS_SET(ch->pcdata->powers[1], DPOWER_DGAROTTE)) 
  {
    send_to_char("Huh?\n\r", ch );
    return;
  }
  if (!IS_SET(ch->newbits, NEW_DARKNESS)) 
  {
    send_to_char("You have to be surrounded in darkness.\n\r", ch );
    return;
  }
  if ((victim = get_char_room(ch, arg)) == NULL) 
  {
    send_to_char("Dark Garotte whom?\n\r", ch );
    return;
  }
  if (victim == ch) 
  {
    send_to_char("That would be a bad idea.\n\r", ch );
    return;
  }
  if (IS_NPC(victim) && victim->level>2000)
  {
    send_to_char("That would be a bad idea, too big mob.\n\r", ch );  
    return;
  }
  if (is_safe(ch, victim)) return;
  if (((obj = get_eq_char(ch, WEAR_WIELD)) == NULL || obj->value[3] != 4)
    && ((obj = get_eq_char(ch, WEAR_HOLD)) == NULL || obj->value[3] != 4))
  {
    send_to_char( "You need to wield a whip.\n\r", ch );
    return;
  }
  if ( victim->fighting != NULL )
  {
    send_to_char( "You can't garotte a fighting person.\n\r", ch );
    return;
  }
  if (victim->hit < victim->max_hit) 
  {
    act("$N is too hurt for you to sneak up.", ch, NULL,victim,TO_CHAR);
    return;
  }
  WAIT_STATE(ch, skill_table[gsn_garotte].beats);
  REMOVE_BIT(ch->newbits, NEW_DARKNESS);  
  REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
  if ( chance > 95 )
  {
    act("$n wraps a whip around $N's neck! $N is choking to death!",ch,NULL,victim,TO_NOTVICT);
    act("You wrap your whip around $N's neck and refuse to let go!",ch,NULL,victim,TO_CHAR);
    act("$n wraps his whip around your neck, and you can't break free!",ch,NULL,victim,TO_VICT);
    one_hit(ch, victim, gsn_garotte,1);
    one_hit(ch, victim, gsn_garotte,1);
    one_hit(ch, victim, gsn_garotte,1);
    one_hit(ch, victim, gsn_garotte,1);
    one_hit(ch, victim, gsn_garotte,1);
    one_hit(ch, victim, gsn_garotte,1);
  } 
  else
  {
    one_hit(ch, victim, gsn_garotte,1);
    one_hit(ch, victim, gsn_garotte,1);
    one_hit(ch, victim, gsn_garotte,1);
  }
  return;
}

bool has_timer(CHAR_DATA *ch) 
{
  if (ch->fight_timer > 0 && !IS_NPC(ch)) 
  {
    send_to_char("Not until your fight timer runs out!\n\r", ch );
    return TRUE;
  }
  return FALSE;
}

void do_circle( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  OBJ_DATA *obj;

  one_argument( argument, arg );
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_NINJA) || ch->pcdata->powers[NPOWER_NINGENNO] < 6) 
  {
    send_to_char("Huh?\n\r", ch );
    return;
  }
  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    if (ch->fighting == NULL) 
    {
      send_to_char( "They aren't here.\n\r", ch );
      return;
    }
    else victim = ch->fighting;
  }
  if ( victim == ch )
  {
    send_to_char( "How can you circle yourself?\n\r", ch );
    return;
  }
  if (is_safe( ch, victim )) return;
  if ((( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || obj->value[3] != 11)
    && ((obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || obj->value[3] != 11))
  {
    send_to_char( "You need to wield a piercing weapon.\n\r", ch );
    return;
  }
  one_hit(ch, victim, gsn_circle, 1);
  if (number_range(1,4) == 2)
  {
    send_to_char("You spin around once more, scoring a second hit.\n\r",ch);
    one_hit(ch, victim, gsn_circle, 1);
  }
  WAIT_STATE( ch, 8 );
  return;
}

void do_autostance(CHAR_DATA *ch, char *argument )
{
  char arg [MAX_INPUT_LENGTH];
  
  argument = one_argument(argument,arg);
  if (IS_NPC(ch)) return;
  if (!str_cmp(arg,"none"))
  {
    send_to_char("You no longer autostance.\n\r", ch );
    ch->stance[MONK_AUTODROP] = STANCE_NONE;
  }
  else if (!str_cmp(arg, "crane"))
  {
    send_to_char("You now autostance into the crane stance.\n\r",ch );
    ch->stance[MONK_AUTODROP] = STANCE_CRANE;
  }
  else if (!str_cmp(arg, "crab")) 
  {
    send_to_char("You now autostance into the crab stance.\n\r", ch );
    ch->stance[MONK_AUTODROP] = STANCE_CRAB;
  }
  else if (!str_cmp(arg, "bull"))
  {
    send_to_char("You now autostance into the bull stance.\n\r", ch );
    ch->stance[MONK_AUTODROP] = STANCE_BULL;
  }
  else if (!str_cmp(arg, "viper"))
  {
    send_to_char("You now autostance into the viper stance.\n\r", ch );
    ch->stance[MONK_AUTODROP] = STANCE_VIPER;
  }
  else if (!str_cmp(arg, "mongoose"))
  {
    send_to_char("You now autostance into the mongoose stance.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_MONGOOSE;
  }
  else if (!str_cmp(arg, "mantis") && ch->stance[STANCE_CRANE] >= 200 && ch->stance[STANCE_VIPER] >= 200)
  {
    send_to_char("You now autostance into the mantis stance.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_MANTIS;
  }
  else if (!str_cmp(arg, "monkey") && ch->stance[STANCE_CRANE] >= 200 && ch->stance[STANCE_MONGOOSE] >= 200)
  {
    send_to_char("You now autostance into the monkey stance.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_MONKEY;
  }
  else if (!str_cmp(arg, "swallow") && ch->stance[STANCE_CRAB] >= 200 && ch->stance[STANCE_MONGOOSE] >= 200)
  {
    send_to_char("You now autostance into the swallow stance.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_SWALLOW;
  }
  else if (!str_cmp(arg, "tiger") && ch->stance[STANCE_BULL] >= 200 && ch->stance[STANCE_VIPER] >= 200)
  {
    send_to_char("You now autostance into the tiger stance.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_TIGER;
  }
  else if (!str_cmp(arg, "dragon") && ch->stance[STANCE_CRAB] >= 200 && ch->stance[STANCE_BULL] >= 200)
  {
    send_to_char("You now autostance into the dragon stance.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_DRAGON;
  }
  else if (!str_cmp(arg, "wolf") && (IS_CLASS(ch, CLASS_WEREWOLF)) && ch->stance[STANCE_TIGER] >= 200 && ch->stance[STANCE_SWALLOW] >= 200 && ch->stance[STANCE_MONKEY] >= 200 && ch->stance[STANCE_MANTIS] >= 200 && ch->stance[STANCE_DRAGON] >= 200)
  {
    send_to_char("You now autostance into the wolf stance.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_WOLF;
  }
  else if (!str_cmp(arg, "ss1") && ch->stance[19] != -1)
  {
    send_to_char("You now autostance into superstance one.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_SS1;
  }
  else if (!str_cmp(arg, "ss2") && ch->stance[20] != -1)
  {
    send_to_char("You now autostance into superstance two.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_SS2;
  }
  else if (!str_cmp(arg, "ss3") && ch->stance[21] != -1)
  {
    send_to_char("You now autostance into superstance three.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_SS3;
  }
  else if (!str_cmp(arg, "ss4") && ch->stance[22] != -1)
  {
    send_to_char("You now autostance into superstance four.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_SS4;
  }
  else if (!str_cmp(arg, "ss5") && ch->stance[23] != -1)
  {
    send_to_char("You now autostance into superstance five.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_SS5;
  }
  else send_to_char("You can't set your autostance to that!\n\r",ch);
}

void autodrop(CHAR_DATA *ch) 
{
  char buf [MAX_INPUT_LENGTH];
  char buf2 [MAX_INPUT_LENGTH];
  char stancename [10];
  
  if (IS_NPC(ch)) return;
  if (ch->stance[MONK_AUTODROP]==STANCE_NONE) return;
  if (ch->stance[MONK_AUTODROP]==STANCE_VIPER) sprintf(stancename,"viper");
  else if (ch->stance[MONK_AUTODROP]==STANCE_CRANE) sprintf(stancename,"crane");
  else if (ch->stance[MONK_AUTODROP]==STANCE_CRAB) sprintf(stancename,"crab");
  else if (ch->stance[MONK_AUTODROP]==STANCE_MONGOOSE) sprintf(stancename,"mongoose");
  else if (ch->stance[MONK_AUTODROP]==STANCE_BULL) sprintf(stancename,"bull");
  else if (ch->stance[MONK_AUTODROP]==STANCE_MANTIS) sprintf(stancename,"mantis");
  else if (ch->stance[MONK_AUTODROP]==STANCE_DRAGON) sprintf(stancename,"dragon");
  else if (ch->stance[MONK_AUTODROP]==STANCE_TIGER) sprintf(stancename,"tiger");
  else if (ch->stance[MONK_AUTODROP]==STANCE_MONKEY) sprintf(stancename,"monkey");
  else if (ch->stance[MONK_AUTODROP]==STANCE_SWALLOW) sprintf(stancename,"swallow");
  else if (ch->stance[MONK_AUTODROP]==STANCE_SS1) sprintf(stancename,"ss1");
  else if (ch->stance[MONK_AUTODROP]==STANCE_SS2) sprintf(stancename,"ss2");
  else if (ch->stance[MONK_AUTODROP]==STANCE_SS3) sprintf(stancename,"ss3");
  else if (ch->stance[MONK_AUTODROP]==STANCE_SS4) sprintf(stancename,"ss4");
  else if (ch->stance[MONK_AUTODROP]==STANCE_SS5) sprintf(stancename,"ss5");
  else if (ch->stance[MONK_AUTODROP]) sprintf(stancename,"wolf");
  else return;
  if (ch->stance[0] < 1) 
  {
    ch->stance[0] = ch->stance[MONK_AUTODROP];;
    sprintf(buf, "#7You autodrop into the #3%s#7 stance.", stancename);
    act(buf, ch, NULL, NULL, TO_CHAR);
    sprintf(buf2, "#7$n autodrops into the #3%s#7 stance.",stancename);
    act(buf2, ch, NULL, NULL, TO_ROOM);
  }       
}

void dropinvis(CHAR_DATA *ch)
{
  if (ch->level < 7 && IS_SET(ch->act, AFF_HIDE)) REMOVE_BIT(ch->act, AFF_HIDE);
  if (ch->level < 7 && IS_SET(ch->act, PLR_WIZINVIS)) REMOVE_BIT(ch->act, PLR_WIZINVIS);
  if ((IS_CLASS(ch, CLASS_DROW) || IS_CLASS(ch, CLASS_DROID)) && IS_SET(ch->newbits, NEW_DARKNESS))
  {
    REMOVE_BIT(ch->newbits, NEW_DARKNESS);
    REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
  }
  return;
}


void ragnarokdecap(CHAR_DATA *ch, CHAR_DATA *victim)
{
  char buf[MAX_STRING_LENGTH];

  if (IS_CLASS(victim, CLASS_VAMPIRE))           do_mortalvamp(victim,"");
  else if (IS_SET(victim->special, SPC_WOLFMAN)) do_unwerewolf(victim,"");
  char_from_room(victim);
  char_to_room(victim,get_room_index(ROOM_VNUM_ALTAR));
  do_clearstats(victim,"");
  victim->level = 2;
  dropinvis(ch);
  players_decap++;
  sprintf(buf,"%s was beheaded by %s, the ragnarok continues",victim->name, ch->name);
  do_info(ch,buf);
  send_to_char("YOU HAVE BEEN KILLED!!!!\n\r",victim);
  do_call(victim, "all");
  do_train(victim,"avatar");
  do_restore(victim,"self");
  return;
}

/*
 * New system to replace status, called fair fight, it measures the
 * difference between two players, giving them points for their
 * stances, powers, and stats. If they are within each others range,
 * the call will return TRUE, if not FALSE. Call for fair_fight when
 * you need to see if a fight is fair (ie. decapping).
 */
bool fair_fight(CHAR_DATA *ch, CHAR_DATA *victim)
{
  int iAggr, iDef;

  if (IS_NPC(ch) || IS_NPC(victim)) return TRUE;

  /*
   * All the people that shouldn't be fighting anyway
   */
  if ((get_age(victim) - 17) < 2) return FALSE;
  if ((get_age(ch) - 17) < 2) return FALSE;
  if (ch == victim) return FALSE;
  if (ch->level != 3 || victim->level != 3) return FALSE;
  if (reachedDecapLimit(ch)) return FALSE;

  iAggr = getMight(ch);
  iDef  = getMight(victim);

  /*
   * To weak to fight, should be protected.
   */
  if (iDef < 150 || iAggr < 150) return FALSE;

  /*
   * Trying to scale the range depending on the size of the player,
   * allowing larger players a larger range, since theres fewer large players.
   */
  if (iAggr < 1000)
  {
    if (iAggr * 0.80 > iDef) return FALSE;
  }
  else
  {
    if (iAggr * 0.75 > iDef) return FALSE;
  }

  /*
   * They passed the test, FIGHT children.
   */
  return TRUE;
}
