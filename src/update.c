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
#include <unistd.h> /* unlink */
#include "merc.h"



/*
 * Local functions.
 */
void    char_update     args( ( void ) );
void	mobile_update	args( ( void ) );
void	weather_update	args( ( void ) );
void    room_update     args( ( void ) );
void	obj_update	args( ( void ) );
void    ww_update       args( ( void ) );
void    embrace_update  args((void));
void	werewolf_regen	args ( ( CHAR_DATA *ch, int multiplier) );

void update_morted_timer			args ((CHAR_DATA *ch));
void update_sit_safe_counter			args ((CHAR_DATA *ch));
void update_drunks				args ((CHAR_DATA *ch));
void sex_update					args ((CHAR_DATA *ch));
void update_vampire				args ((CHAR_DATA *ch));
void update_vampire_regen			args ((CHAR_DATA *ch));
void update_cyborg 				args ((CHAR_DATA *ch));
void update_monk				args ((CHAR_DATA *ch));
void update_drider				args ((CHAR_DATA *ch));
void update_lich				args ((CHAR_DATA *ch));
void update_lich_regen                          args ((CHAR_DATA *ch));
void update_angel                               args ((CHAR_DATA *ch));
void update_ninja				args ((CHAR_DATA *ch));
void update_werewolf				args ((CHAR_DATA *ch));
void update_demon_regen				args ((CHAR_DATA *ch));
void update_demon				args ((CHAR_DATA *ch));
void update_tanarri				args ((CHAR_DATA *ch));
void update_drow				args ((CHAR_DATA *ch));
void update_highlander				args ((CHAR_DATA *ch));
void update_mage				args ((CHAR_DATA *ch));
void update_knight				args ((CHAR_DATA *ch));
void update_shapeshifter			args ((CHAR_DATA *ch));
void update_arti_regen				args ((CHAR_DATA *ch));
void regen_limb					args ((CHAR_DATA *ch));
void update_safe_powers				args ((CHAR_DATA *ch));

void gain_exp( CHAR_DATA *ch, int gain )
{
    CHAR_DATA *mount = NULL;
    CHAR_DATA *master = NULL;
    if ( IS_NPC(ch) && (mount = ch->mount) != NULL && !IS_NPC(mount))
    {
	if ( (master = ch->master) == NULL || master != mount )
	    mount->exp += gain;
    }
    if ( !IS_NPC(ch) )
	ch->exp += gain;
    return;
}

void gain_condition( CHAR_DATA *ch, int iCond, int value )
{
  int condition;

  if ( value == 0 || IS_NPC(ch) ) return;

    if (!IS_NPC(ch) && IS_HERO(ch) && !IS_CLASS(ch, CLASS_VAMPIRE) 
	&& iCond != COND_DRUNK )
	return;

    condition				= ch->pcdata->condition[iCond];
    if (!IS_NPC(ch) && !IS_CLASS(ch, CLASS_VAMPIRE) )
	{
			ch->pcdata->condition[iCond]	= URANGE( 0, condition + value, 48 );
	}
	else
	ch->pcdata->condition[iCond]	= URANGE( 0, condition + value, 20000/ch->generation );

    if ( ch->pcdata->condition[iCond] == 0 )
    {
	switch ( iCond )
	{
	case COND_FULL:
	    if (!IS_CLASS(ch, CLASS_VAMPIRE))
	    {
		send_to_char( "You are REALLY hungry.\n\r",  ch );
		act( "You hear $n's stomach rumbling.", ch, NULL, NULL, TO_ROOM );
	    }
	    break;

	case COND_THIRST:
	    if (!IS_CLASS(ch, CLASS_VAMPIRE)) 
		send_to_char( "You are REALLY thirsty.\n\r", ch );
	    else if (ch->hit > 0)
	    {
		send_to_char( "You are DYING from lack of blood!\n\r", ch );
		act( "$n gets a hungry look in $s eyes.", ch, NULL, NULL, TO_ROOM );
		ch->hit = ch->hit - number_range(2,5);
		if (number_percent() <= ch->beast && ch->beast > 0) vamp_rage(ch);
		if (!IS_VAMPAFF(ch, VAM_FANGS)) do_fangs(ch,"");
	    }
	    break;

	case COND_DRUNK:
	    if ( condition != 0 )
		send_to_char( "You are sober.\n\r", ch );
	    break;
	}
    }
    else if ( ch->pcdata->condition[iCond] < 10 )
    {
	switch ( iCond )
	{
	case COND_FULL:
	    if (!IS_CLASS(ch, CLASS_VAMPIRE)) 
		send_to_char( "You feel hungry.\n\r",  ch );
	    break;

	case COND_THIRST:
	    if (!IS_CLASS(ch, CLASS_VAMPIRE)) 
		send_to_char( "You feel thirsty.\n\r", ch );
	    else
	    {
		send_to_char( "You crave blood.\n\r", ch );
		if (number_range(1,1000) <= ch->beast && ch->beast > 0) vamp_rage(ch);
		if (number_percent() > (ch->pcdata->condition[COND_THIRST]+75)
		    && !IS_VAMPAFF(ch, VAM_FANGS)) do_fangs(ch,"");
	    }
	    break;
	}
    }

    return;
}


/*
 * Update all chars, including mobs.
 * This function is performance sensitive.
 */
void char_update( void )
{
  AFFECT_DATA *paf;
  AFFECT_DATA *paf_next;
  CHAR_DATA *ch_quit = NULL;
  CHAR_DATA *ch_save = NULL;
  CHAR_DATA *ch;
  CHAR_DATA *ch_next;
  bool is_obj;
  time_t save_time;
  int count = 0, i;

  save_time = current_time;

  for ( ch = char_list; ch != NULL; ch = ch_next )
  {
    ch_next = ch->next;
    count++;

    /*
     * Is the player an object ?
     */
    if (!IS_NPC(ch) && (IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH))) is_obj = TRUE;
    else if (!IS_NPC(ch) && ch->pcdata->obj_vnum != 0)
    {
      is_obj = TRUE;
      SET_BIT(ch->extra, EXTRA_OSWITCH);
    }
    else is_obj = FALSE;
      
    /*
     * Tick Timers and other PC only stuff
     */
    if (!IS_NPC(ch))
    {
      if (ch->tick_timer[TIMER_ENTOMB] == 1)
        send_to_char("You can use entomb again.\n\r", ch);

      for (i = 0; i < MAX_TIMER; i++)
        if (ch->tick_timer[i] > 0) ch->tick_timer[i] --;
  
      /*
       * void, autosave, time bonus, etc
       */
      if ((ch->level < LEVEL_IMMORTAL || !ch->desc) && !is_obj && !IS_SET(ch->extra,EXTRA_AFK))
      {
        if ((ch->desc == NULL || ch->desc->connected == CON_PLAYING) && ch->level >= 2 && ch->save_time < save_time )
        {
          ch_save = ch;
          save_time = ch->save_time;
        }
        if (++ch->timer >= 12)
        {
          if (ch->was_in_room == NULL && ch->in_room != NULL)
          {
            ch->was_in_room = ch->in_room;
            if ( ch->fighting != NULL ) stop_fighting( ch, TRUE );
            act( "$n disappears into the void.", ch, NULL, NULL, TO_ROOM );
            send_to_char( "You disappear into the void.\n\r", ch );
            save_char_obj( ch );
            char_from_room( ch );
            char_to_room( ch, get_room_index( ROOM_VNUM_LIMBO ) );
          }
        }
        if (ch->timer > 20) ch_quit = ch;
      }
    }
        
    /*
     * updating spells on all mobs and players
     */
    for ( paf = ch->affected; paf != NULL; paf = paf_next )
    {
      paf_next  = paf->next;
      if ( paf->duration > 0 ) paf->duration--;
      else
      {
        if ( paf_next == NULL  || paf_next->type != paf->type || paf_next->duration > 0 )
        {
          if ( paf->type > 0 && skill_table[paf->type].msg_off && !is_obj)
          {
            send_to_char( skill_table[paf->type].msg_off, ch );
            send_to_char( "\n\r", ch );
          }
        }
        affect_remove( ch, paf );
      } 
    }
         
    /*
     * Update class stuff
     */
    if (ch->fighting == NULL && !IS_NPC(ch))
    {
      if (IS_CLASS(ch, CLASS_WEREWOLF) && ch->gnosis[GMAXIMUM] > ch->gnosis[GCURRENT])
      {  
        if (ch->position <= POS_SLEEPING ) ch->gnosis[GCURRENT] += number_range(2,3);
        else if (ch->position <= POS_RESTING ) ch->gnosis[GCURRENT] += number_range(1,3);
        if (ch->gnosis[GCURRENT] > ch->gnosis[GMAXIMUM]) ch->gnosis[GCURRENT] = ch->gnosis[GMAXIMUM];
      }
      if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->beast > 0 && ch->pcdata->condition[COND_THIRST] <= 15)
      {
        act("You bare your fangs and scream in rage from lack of blood.",ch,NULL,NULL,TO_CHAR);
        act("$n bares $s fangs and screams in rage.",ch,NULL,NULL,TO_ROOM);
        do_beastlike(ch,"");
      }
      if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_CHIKYU] >= 6 && ch->pcdata->powers[HARA_KIRI] > 0)
      {
        if (ch->pcdata->powers[HARA_KIRI] == 1)
          send_to_char("Your HaraKiri wears off.\n\r", ch );
        ch->pcdata->powers[HARA_KIRI]--; 
      }
      if (!IS_SET(ch->newbits, NEW_CLOAK) && !is_obj && ((IS_CLASS(ch, CLASS_MONK)
        && ch->pcdata->powers[PMONK] > 10) || (IS_CLASS(ch,CLASS_UNDEAD_KNIGHT) && ch->pcdata->powers[NECROMANCY] > 9)))
      {
        SET_BIT(ch->newbits, NEW_CLOAK);
        if (IS_CLASS(ch, CLASS_MONK)) send_to_char("Your Cloak of Life is restored.\n\r",ch);
        else send_to_char("Your cloak of death is restored.\n\r",ch);
      }
    }

    /*
     * Updating current position
     */
    if ( ch->position == POS_MORTAL || ch->position == POS_STUNNED || ch->position == POS_INCAP )
    {
      update_pos(ch);
    } 
    if ( ch->position > POS_STUNNED && !is_obj)
    {  
      if ( ch->hit  < ch->max_hit ) ch->hit = UMIN(ch->hit + number_range(5,10), ch->max_hit);
      if ( ch->mana < ch->max_mana ) ch->mana = UMIN(ch->mana + number_range(5,10), ch->max_mana);
      if ( ch->move < ch->max_move ) ch->move = UMIN(ch->move + number_range(5,10), ch->max_move);
    }    
    else if ( ch->position <= POS_STUNNED && !is_obj)
    {
      ch->hit = ch->hit + number_range(2,4);
      update_pos( ch );
      if (ch->position > POS_STUNNED)
      {
        act( "$n clambers back to $s feet.", ch, NULL, NULL, TO_ROOM );
        act( "You clamber back to your feet.", ch, NULL, NULL, TO_CHAR );  
      }
    }  
      
    /* 
     * Dealing damage due to missling limbs, etc.
     */
    if ( ch->loc_hp[6] > 0 && !is_obj && ch->in_room != NULL )
    {  
      int dam = 0;
        
      if (IS_BLEEDING(ch,BLEEDING_HEAD))
      {
        act( "A spray of blood shoots from the stump of $n's neck.", ch, NULL, NULL, TO_ROOM );
        send_to_char( "A spray of blood shoots from the stump of your neck.\n\r", ch );
        dam += number_range(20,50);
      }
      if (IS_BLEEDING(ch,BLEEDING_THROAT))
      {
        act( "Blood pours from the slash in $n's throat.", ch, NULL, NULL, TO_ROOM );
        send_to_char( "Blood pours from the slash in your throat.\n\r", ch );
        dam += number_range(10,20);
      }  
      if (IS_BLEEDING(ch,BLEEDING_ARM_L))
      {
        act( "A spray of blood shoots from the stump of $n's left arm.", ch, NULL, NULL, TO_ROOM );
        send_to_char( "A spray of blood shoots from the stump of your left arm.\n\r", ch );
        dam += number_range(10,20);
      }
      else if (IS_BLEEDING(ch,BLEEDING_HAND_L))
      {
        act( "A spray of blood shoots from the stump of $n's left wrist.", ch, NULL, NULL, TO_ROOM );
        send_to_char( "A spray of blood shoots from the stump of your left wrist.\n\r", ch );
        dam += number_range(5,10);
      }
      if (IS_BLEEDING(ch,BLEEDING_ARM_R))
      {
        act( "A spray of blood shoots from the stump of $n's right arm.", ch, NULL, NULL, TO_ROOM );
        send_to_char( "A spray of blood shoots from the stump of your right arm.\n\r", ch );
        dam += number_range(10,20);
      }
      else if (IS_BLEEDING(ch,BLEEDING_HAND_R))
      {
        act( "A spray of blood shoots from the stump of $n's right wrist.", ch, NULL, NULL, TO_ROOM );
        send_to_char( "A spray of blood shoots from the stump of your right wrist.\n\r", ch );
        dam += number_range(5,10);
      }
      if (IS_BLEEDING(ch,BLEEDING_LEG_L))
      {
        act( "A spray of blood shoots from the stump of $n's left leg.", ch, NULL, NULL, TO_ROOM );
        send_to_char( "A spray of blood shoots from the stump of your left leg.\n\r", ch );
        dam += number_range(10,20);
      }
      else if (IS_BLEEDING(ch,BLEEDING_FOOT_L))
      {
        act( "A spray of blood shoots from the stump of $n's left ankle.", ch, NULL, NULL, TO_ROOM );
        send_to_char( "A spray of blood shoots from the stump of your left ankle.\n\r", ch );
        dam += number_range(5,10);
      }
      if (IS_BLEEDING(ch,BLEEDING_LEG_R))
      {
        act( "A spray of blood shoots from the stump of $n's right leg.", ch, NULL, NULL, TO_ROOM );
        send_to_char( "A spray of blood shoots from the stump of your right leg.\n\r", ch );
        dam += number_range(10,20);
      }
      else if (IS_BLEEDING(ch,BLEEDING_FOOT_R))
      {
        act( "A spray of blood shoots from the stump of $n's right ankle.", ch, NULL, NULL, TO_ROOM );
        send_to_char( "A spray of blood shoots from the stump of your right ankle.\n\r", ch );
        dam += number_range(5,10);
      }
      ch->hit = ch->hit - dam;
      if (ch->hit < 1) ch->hit = 1;
      update_pos(ch);
      ch->in_room->blood += dam;
      if (ch->in_room->blood > 1000) ch->in_room->blood = 1000;
    }  
    if (IS_EXTRA(ch,EXTRA_ROT) && !is_obj)
    {
      int dam;
       
      act( "$n's flesh shrivels and tears.", ch, NULL, NULL, TO_ROOM);
      send_to_char( "Your flesh shrivels and tears.\n\r", ch);
      dam = number_range(250,500);
      ch->hit = ch->hit - dam;
      if (ch->hit < 1) ch->hit = 1;
      update_pos(ch);
    }
    if (IS_AFFECTED(ch, AFF_FLAMING) && !is_obj)
    {
      int dam;
        
      act( "$n's flesh burns and crisps.", ch, NULL, NULL, TO_ROOM );
      send_to_char( "Your flesh burns and crisps.\n\r", ch );
      dam = number_range(250,300);
      ch->hit = ch->hit - dam;
      if (ch->hit < 1) ch->hit = 1;
      update_pos(ch);
    }  
      
    /* 
     * Class special damage
     */
    if (IS_CLASS(ch, CLASS_VAMPIRE) && (!IS_AFFECTED(ch,AFF_SHADOWPLANE)) &&
       (!IS_NPC(ch) && !IS_IMMUNE(ch,IMM_SUNLIGHT)) && ch->in_room != NULL &&
       (!ch->in_room->sector_type == SECT_INSIDE) && !is_obj &&
       (!room_is_dark(ch->in_room)) && (weather_info.sunlight != SUN_DARK))
    {
      act( "$n's flesh smolders in the sunlight!", ch, NULL, NULL, TO_ROOM );
      send_to_char( "Your flesh smolders in the sunlight!\n\r", ch );
       
      /* This one's to keep Zarkas quiet ;) */ 
      if (IS_POLYAFF(ch, POLY_SERPENT))
        ch->hit = ch->hit - number_range(2,4);
      else
        ch->hit = ch->hit - number_range(5,10);
      update_pos(ch);
    }
      
    /*
     * More damage stuff
     */
    if (IS_AFFECTED(ch, AFF_POISON) && !is_obj)
    {
      act( "$n shivers and suffers.", ch, NULL, NULL, TO_ROOM );
      send_to_char( "You shiver and suffer.\n\r", ch );
      ch->hit = ch->hit - number_range(100, 200);
      if (ch->hit < 1) ch->hit = 1;
      if (number_range(1,4)==1)
      {
        REMOVE_BIT(ch->affected_by, AFF_POISON);
        send_to_char("You feel the poison leave your system.\n\r",ch);
      }
    }
    
    /*
     * and then we do some healing - messy ?
     */ 
    if ( ch->position == POS_INCAP && !is_obj)
    {
      ch->hit = ch->hit + number_range(2,4);
      update_pos( ch );
      if (ch->position > POS_INCAP)
      {
        act( "$n's wounds stop bleeding and seal up.", ch, NULL, NULL, TO_ROOM );
        send_to_char( "Your wounds stop bleeding and seal up.\n\r", ch );
      }
      if (ch->position > POS_STUNNED)
      {
        act( "$n clambers back to $s feet.", ch, NULL, NULL, TO_ROOM );
        send_to_char( "You clamber back to your feet.\n\r", ch );
      }
    }
    else if (ch->position == POS_MORTAL && !is_obj)
    {
      ch->hit = ch->hit + number_range(2,4);
      update_pos( ch );
      if (ch->position == POS_INCAP)
      {
        act( "$n's wounds begin to close, and $s bones pop back into place.", ch, NULL, NULL, TO_ROOM );
        send_to_char( "Your wounds begin to close, and your bones pop back into place.\n\r", ch );
      }
    }
  }  
      
  /*  
   * Autosave, Autoquit checks
   */  
  if ( ch_save != NULL || ch_quit != NULL )
  {  
    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
      ch_next = ch->next;
      if ( ch == ch_save ) save_char_obj( ch );
      if ( ch == ch_quit ) do_quit( ch, "" );
    }  
  }
        
  return;
}



/*
 * Mob autonomous action.
 * This function takes 25% to % of ALL Merc cpu time.
 * -- Furey
 */
void mobile_update( void )
{
  CHAR_DATA *ch;
  CHAR_DATA *ch_next;
  EXIT_DATA *pexit;
  int door;

  /* Examine all mobs. */
  for ( ch = char_list; ch != NULL; ch = ch_next )
  {
    ch_next = ch->next;

    if ( ch->in_room == NULL ) continue;

    if ( ch->hunting != NULL && ch->hunting != '\0' && strlen(ch->hunting) > 1 )
    {
      check_hunt( ch );
      continue;
    }
    if ( !IS_NPC(ch) )
    {
      update_morted_timer(ch);
      update_sit_safe_counter(ch);
      update_drunks(ch);
      sex_update(ch);
      if (ch->level < 7 && IS_HERO(ch)) update_safe_powers(ch);
      if (IS_HERO(ch) && ch->hit > 0 && !IS_SET(ch->extra, EXTRA_AFK))
      {
        if (IS_CLASS(ch, CLASS_VAMPIRE)) 
        {
          if (ch->rage >0) update_vampire_regen(ch);
          else if (IS_ITEMAFF(ch, ITEMA_REGENERATE)) update_arti_regen(ch);
          update_vampire(ch);
        }
        if (IS_CLASS(ch, CLASS_DROID))
        {
          update_drider(ch);
          if (ch->pcdata->powers[CYBORG_BODY] > 5) update_cyborg(ch);
          else if (IS_ITEMAFF(ch, ITEMA_REGENERATE)) update_arti_regen(ch);
        }
        if (IS_CLASS(ch, CLASS_ANGEL))
        {
          if (IS_ITEMAFF(ch, ITEMA_REGENERATE)) update_arti_regen(ch);
          update_angel(ch);
        }
        if (IS_CLASS(ch, CLASS_TANARRI))
        {
          if (IS_ITEMAFF(ch, ITEMA_REGENERATE)) update_arti_regen(ch);
          update_tanarri(ch);
        }
        if (IS_CLASS(ch, CLASS_LICH))
        {
          if (ch->pcdata->powers[LIFE_LORE] > 0) update_lich_regen(ch);
          else if (IS_ITEMAFF(ch, ITEMA_REGENERATE)) update_arti_regen(ch);
          update_lich(ch);
        }
        if (IS_CLASS(ch, CLASS_MONK)) update_monk(ch);
        if (IS_CLASS(ch, CLASS_NINJA)) update_ninja(ch);
        if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT)) update_knight(ch);
        if (IS_CLASS(ch, CLASS_WEREWOLF)) update_werewolf(ch);
        if (IS_CLASS( ch, CLASS_DEMON))
        {
          update_demon(ch);
          if (IS_SET(ch->warp, WARP_REGENERATE)) update_demon_regen(ch);
          else if (IS_ITEMAFF(ch, ITEMA_REGENERATE)) update_arti_regen(ch);
        }
        if (IS_CLASS(ch, CLASS_SHAPESHIFTER)) update_shapeshifter(ch);
        if (IS_CLASS(ch, CLASS_DROW))
        {
          update_drow(ch);
          if (IS_ITEMAFF(ch, ITEMA_REGENERATE)) update_arti_regen(ch);
        }
        if (IS_CLASS(ch, CLASS_SAMURAI)) update_highlander(ch);
        if (IS_CLASS(ch, CLASS_MAGE))
        {
          update_mage(ch);
          if (IS_ITEMAFF(ch, ITEMA_REGENERATE)) update_arti_regen(ch);
        }
        if (ch->class == 0 && IS_ITEMAFF(ch, ITEMA_REGENERATE)) update_arti_regen(ch);
      }
      else 
      {
        ch->hit += number_range(1, 5);
        if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
        update_pos(ch);
        if (IS_ITEMAFF(ch, ITEMA_REGENERATE) && ch->hit > 0)
        update_arti_regen(ch);
      }
    }
    else // This is for the mobs
    {
      if ( IS_AFFECTED(ch, AFF_CHARM) ) continue;
      if ( ch->spec_fun != 0 )
      {
        if ( (*ch->spec_fun) ( ch ) ) continue;
	if (ch == NULL) continue;
      }
      if ( ch->position != POS_STANDING ) {do_stand(ch, "");continue;}
      if ( IS_SET(ch->act, ACT_SCAVENGER) && ch->in_room->contents != NULL && number_bits( 2 ) == 0 )
      {
        OBJ_DATA *obj;
        OBJ_DATA *obj_best = 0;
        int max=1;
        for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
        {
          if ( CAN_WEAR(obj, ITEM_TAKE) && obj->cost > max )
          {
            obj_best    = obj;
            max         = obj->cost;
          }
        }
        if ( obj_best )
        {
          obj_from_room( obj_best );
          obj_to_char( obj_best, ch );
          act( "$n picks $p up.", ch, obj_best, NULL, TO_ROOM );
          act( "You pick $p up.", ch, obj_best, NULL, TO_CHAR );
        }
      }
      if ( !IS_SET(ch->act, ACT_SENTINEL) && ( door = number_bits( 5 ) ) <= 5 && ( pexit = ch->in_room->exit[door] ) != NULL
          && pexit->to_room != NULL && !IS_SET(pexit->exit_info, EX_CLOSED) && !IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB)
          && ( ch->hunting == NULL || strlen(ch->hunting) < 2 ) && ( (!IS_SET(ch->act, ACT_STAY_AREA) && ch->level < 900)
          || pexit->to_room->area == ch->in_room->area ) )
      {
        move_char( ch, door );
      }
      if ( ch->hit < ch->max_hit / 2 && ( door = number_bits( 3 ) ) <= 5 && ( pexit = ch->in_room->exit[door] ) != NULL
          && pexit->to_room != NULL && !IS_AFFECTED(ch, AFF_WEBBED) && ch->level < 900 && !IS_SET(pexit->exit_info, EX_CLOSED)
          && !IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB))
      {
        CHAR_DATA *rch;	    
        bool found;
        found = FALSE;
        for ( rch  = pexit->to_room->people;
        rch != NULL;
        rch  = rch->next_in_room )
        {
          if ( !IS_NPC(rch) )
          {
            found = TRUE;
            break;
          }
        }	    
        if ( !found )
        move_char( ch, door );
        }
      }
    }
  return;
}


/*
 * Update the weather.
 */
void weather_update( void )
{
  char buf[MAX_STRING_LENGTH];
  DESCRIPTOR_DATA *d;
  CHAR_DATA *ch = NULL;
  int diff;
  bool char_up;

  buf[0] = '\0';

  switch ( ++time_info.hour )
  {
    case  5:
	weather_info.sunlight = SUN_LIGHT;
	strcat( buf, "The day has begun.\n\r" );
	break;

    case  6:
	weather_info.sunlight = SUN_RISE;
	strcat( buf, "The sun rises in the east.\n\r" );
	break;

    case 19:
	weather_info.sunlight = SUN_SET;
	strcat( buf, "The sun slowly disappears in the west.\n\r" );
	break;

    case 20:
        weather_info.sunlight = SUN_DARK;
	strcat( buf, "The night has begun.\n\r" );
	break;

    case 24:
	time_info.hour = 0;
	time_info.day++;
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    char_up = FALSE;
	    if (( d->connected == CON_PLAYING
	    ||    d->connected == CON_EDITING )
	    &&   (ch = d->character) != NULL
	    &&  !IS_NPC(ch))
	    {
		if (ch->fighting == NULL && !IS_SET(ch->newbits, NEW_NATURAL)
		 && ch->monkab[SPIRIT] >= 2 )
		  SET_BIT(ch->newbits, NEW_NATURAL);

		send_to_char( "You hear a clock in the distance strike midnight.\n\r",ch);
		if (IS_EXTRA(ch, EXTRA_ROT))
                {
                        send_to_char("Your flesh feels better.\n\r", ch);
                        REMOVE_BIT(ch->extra, EXTRA_ROT);
                        
                }
                if (ch->pcdata->followers > 0) ch->pcdata->followers=0;
                if (IS_SET(ch->in_room->room_flags, ROOM_SILENCE))
                {
                send_to_char("The silence leaves the room.\n\r",ch);
                act("The silence leaves the room.",ch,NULL,NULL,TO_ROOM);
                REMOVE_BIT(ch->in_room->room_flags,ROOM_SILENCE);
                
                }

                if (IS_SET(ch->in_room->room_flags,ROOM_FLAMING))
                {
                send_to_char("The flames in the room die down.\n\r",ch);
                act("The flames in the room die down.",ch,NULL,NULL,TO_ROOM);
                REMOVE_BIT(ch->in_room->room_flags,ROOM_FLAMING);
                }
                if ( IS_CLASS(ch, CLASS_VAMPIRE) )
		{
		    if (ch->hit < ch->max_hit)
			{ ch->hit = ch->max_hit; char_up = TRUE; }
		    if (ch->mana < ch->max_mana)
			{ ch->mana = ch->max_mana; char_up = TRUE; }
		    if (ch->move < ch->max_move)
			{ ch->move = ch->max_move; char_up = TRUE; }
		    if (char_up) send_to_char( "You feel the strength of the kindred flow through your veins!\n\r", ch );
		    ch->position = POS_STANDING;

                    if (IS_SET(ch->newbits, NEW_TIDE))
 	            {
 		     REMOVE_BIT(ch->newbits, NEW_TIDE);
                     send_to_char("The tide of vitae leaves you.\n\r", ch);
		    }

		    if (IS_SET(ch->extra, EXTRA_BAAL))
                    {
                       send_to_char("The spirit of Baal escapes you.\n\r",ch);  
                       ch->power[DISC_VAMP_POTE] -= 2;
                       ch->power[DISC_VAMP_CELE] -= 2;
                       ch->power[DISC_VAMP_FORT] -= 2;
                       REMOVE_BIT(ch->extra, EXTRA_BAAL);
                    }   

                    if (IS_EXTRA(ch, EXTRA_FLASH))
                    {
                       send_to_char("Your speed slows.\n\r",ch);  
                       ch->power[DISC_VAMP_CELE] -= 2;
                       REMOVE_BIT(ch->extra, EXTRA_FLASH);
                    }   

                   }			
		  } 
                 
	    
	}
	break;
    }

    if ( time_info.day   >= 35 )
    {
	time_info.day = 0;
	time_info.month++;
    }

    if ( time_info.month >= 17 )
    {
	time_info.month = 0;
	time_info.year++;
    }

    /*
     * Weather change.
     */
    if ( time_info.month >= 9 && time_info.month <= 16 )
	diff = weather_info.mmhg >  985 ? -2 : 2;
    else
	diff = weather_info.mmhg > 1015 ? -2 : 2;

    weather_info.change   += diff * dice(1, 4) + dice(2, 6) - dice(2, 6);
    weather_info.change    = UMAX(weather_info.change, -12);
    weather_info.change    = UMIN(weather_info.change,  12);

    weather_info.mmhg += weather_info.change;
    weather_info.mmhg  = UMAX(weather_info.mmhg,  960);
    weather_info.mmhg  = UMIN(weather_info.mmhg, 1040);

    switch ( weather_info.sky )
    {
    default: 
	bug( "Weather_update: bad sky %d.", weather_info.sky );
	weather_info.sky = SKY_CLOUDLESS;
	break;

    case SKY_CLOUDLESS:
	if ( weather_info.mmhg <  990
	|| ( weather_info.mmhg < 1010 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "The sky is getting cloudy.\n\r" );
	    weather_info.sky = SKY_CLOUDY;
	}
	break;

    case SKY_CLOUDY:
	if ( weather_info.mmhg <  970
	|| ( weather_info.mmhg <  990 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "It starts to rain.\n\r" );
	    weather_info.sky = SKY_RAINING;
	}

	if ( weather_info.mmhg > 1030 && number_bits( 2 ) == 0 )
	{
	    strcat( buf, "The clouds disappear.\n\r" );
	    weather_info.sky = SKY_CLOUDLESS;
	}
	break;

    case SKY_RAINING:
	if ( weather_info.mmhg <  970 && number_bits( 2 ) == 0 )
	{
	    strcat( buf, "Lightning flashes in the sky.\n\r" );
	    weather_info.sky = SKY_LIGHTNING;
	}

	if ( weather_info.mmhg > 1030
	|| ( weather_info.mmhg > 1010 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "The rain stopped.\n\r" );
	    weather_info.sky = SKY_CLOUDY;
	}
	break;

    case SKY_LIGHTNING:
	if ( weather_info.mmhg > 1010
	|| ( weather_info.mmhg >  990 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "The lightning has stopped.\n\r" );
	    weather_info.sky = SKY_RAINING;
	    break;
	}
	break;
    }

    if ( buf[0] != '\0' )
    {
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if (( d->connected == CON_PLAYING
	    ||    d->connected == CON_EDITING )
	    &&   IS_OUTSIDE(d->character)
	    &&   IS_AWAKE(d->character) )
		send_to_char( buf, d->character );
	}
    }

    return;
}


int is_wall(EXIT_DATA *ex)
{

    if (IS_SET(ex->exit_info, EX_ICE_WALL)) return 1;
    if (IS_SET(ex->exit_info, EX_FIRE_WALL)) return 2;
    if (IS_SET(ex->exit_info, EX_SWORD_WALL)) return 3;
    if (IS_SET(ex->exit_info, EX_PRISMATIC_WALL)) return 4;
    if (IS_SET(ex->exit_info, EX_IRON_WALL)) return 5;
    if (IS_SET(ex->exit_info, EX_MUSHROOM_WALL)) return 6;
    if (IS_SET(ex->exit_info, EX_CALTROP_WALL)) return 7;
    if (IS_SET(ex->exit_info, EX_ASH_WALL)) return 8;
    return 0;
}

const char * wall[MAX_WALL+1] = 
{
    "","wall of ice", "wall of fire", "wall of swords",
	"prismatic wall", "wall or iron", "wall of mushrooms",
	"wall of caltrops", "wall of ash"
};

/*
 * Update all rooooooms, like gore, poison clouds etc....
 */
void room_update( void )
{
    int i;
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *room;

    
    for ( room = room_list; room != NULL; room = room->next_room)
    {
        
        if (RTIMER(room, RTIMER_WALL_NORTH) == 1
	    && room->exit[DIR_NORTH] != NULL
	    && is_wall(room->exit[DIR_NORTH])!= 0)
        {
	    sprintf(buf, "The %s slowly fades away.", wall[is_wall(room->exit[DIR_NORTH])]);
	    room_message(room, buf);
	    make_wall(room, DIR_NORTH, 0);
        }

        if (RTIMER(room, RTIMER_WALL_SOUTH) == 1
	    && room->exit[DIR_SOUTH] != NULL
	    && is_wall(room->exit[DIR_SOUTH])!= 0)
        {
	    sprintf(buf, "The %s slowly fades away.", wall[is_wall(room->exit[DIR_SOUTH])]);
	    room_message(room, buf);
	    make_wall(room, DIR_SOUTH, 0);
        }

        if (RTIMER(room, RTIMER_WALL_EAST) == 1
	    && room->exit[DIR_EAST] != NULL
	    && is_wall(room->exit[DIR_EAST])!= 0)
        {
	    sprintf(buf, "The %s slowly fades away.", wall[is_wall(room->exit[DIR_EAST])]);
	    room_message(room, buf);
	    make_wall(room, DIR_EAST, 0);
        }

        if (RTIMER(room, RTIMER_WALL_WEST) == 1
	    && room->exit[DIR_WEST] != NULL
	    && is_wall(room->exit[DIR_WEST])!= 0)
        {
	    sprintf(buf, "The %s slowly fades away.", wall[is_wall(room->exit[DIR_WEST])]);
	    room_message(room, buf);
	    make_wall(room, DIR_WEST, 0);
        }

        if (RTIMER(room, RTIMER_WALL_UP) == 1
	    && room->exit[DIR_UP] != NULL
	    && is_wall(room->exit[DIR_UP])!= 0)
        {
	    sprintf(buf, "The %s slowly fades away.", wall[is_wall(room->exit[DIR_UP])]);
	    room_message(room, buf);
	    make_wall(room, DIR_UP, 0);
        }

        if (RTIMER(room, RTIMER_WALL_DOWN) == 1
	    && room->exit[DIR_DOWN] != NULL
	    && is_wall(room->exit[DIR_DOWN])!= 0)
        {
	    sprintf(buf, "The %s slowly fades away.", wall[is_wall(room->exit[DIR_DOWN])]);
	    room_message(room, buf);
	    make_wall(room, DIR_DOWN, 0);
        }


	if (RTIMER(room, RTIMER_STINKING_CLOUD) == 1)
	    room_message(room, "The poisonous vapours dissipate and clear.");

	if (RTIMER(room, RTIMER_HIDE_ROOM) == 1)
	    room_message(room, "The shroud leaves the room.");

	if (RTIMER(room, RTIMER_GHOST_LIGHT) == 2)
	    room_message(room, "The vapourous ghosts start howling insanely.");

	if (RTIMER(room, RTIMER_GHOST_LIGHT) == 3)
	    room_message(room, "The vapourous ghosts start moaning.");

	if (RTIMER(room, RTIMER_GHOST_LIGHT) == 1)
	    room_message(room, "The vapourous ghosts dissipate and vanish.");

	if (RTIMER(room, RTIMER_GLYPH_PROTECTION) == 1)
	    room_message(room, "The glyph of protection flares and vanishes.");

	if (RTIMER(room, RTIMER_SWARM_BEES) == 1)
	    room_message(room, "The bees fly away into the sky.");

	if (RTIMER(room, RTIMER_DISCORD) == 1)
	    room_message(room, "The banging and crashing stops.");

	if (RTIMER(room, RTIMER_SWARM_BATS) == 1)
	    room_message(room, "The bats flap away into the night.");

	if (RTIMER(room, RTIMER_SWARM_RATS) == 1)
	    room_message(room, "The rats scurry away into the floorboards.");
  
      if( RTIMER(room, RTIMER_SILENCE) < 0) room->tick_timer[i] = UMAX(room->tick_timer[i], 0);
      if (RTIMER(room, RTIMER_SILENCE) == 1)
	    room_message(room, "The silence disappates.");

	for (i = 0 ; i < MAX_RTIMER ; i++)
	    room->tick_timer[i] = UMAX(room->tick_timer[i] - 1, 0);

    }

    return;

}


/*
 * Update all objs.
 * This function is performance sensitive.
 */
void obj_update( void )
{   
  OBJ_DATA *obj;
  OBJ_DATA *obj_next;

  for ( obj = object_list; obj != NULL; obj = obj_next )
  {
    CHAR_DATA *rch;
    char *message;

    obj_next = obj->next;
    if ( obj->timer <= 0 || --obj->timer > 0 ) continue;

    switch ( obj->item_type )
    {
	default:              message = "$p vanishes.";         	break;
	case ITEM_FOUNTAIN:   message = "$p dries up.";         	break;
	case ITEM_CORPSE_NPC: message = "$p decays into dust."; 	break;
	case ITEM_CORPSE_PC:  message = "$p decays into dust."; 	break;
	case ITEM_FOOD:       message = "$p decomposes.";		break;
	case ITEM_TRASH:      message = "$p crumbles into dust.";	break;
	case ITEM_EGG:        message = "$p cracks open.";		break;
	case ITEM_WEAPON:     message = "$p turns to fine dust and blows away."; break;
        case ITEM_WALL:       message = "$p flows back into the ground.";break;
    }

	if ( obj->carried_by != NULL && !IS_OBJ_STAT2(obj, ITEM_DAEMONSEED) )
	{
	    act( message, obj->carried_by, obj, NULL, TO_CHAR );
	}
	else if ( obj->in_room != NULL
	&&      ( rch = obj->in_room->people ) != NULL && !IS_OBJ_STAT2(obj, ITEM_DAEMONSEED) )
	{
	    act( message, rch, obj, NULL, TO_ROOM );
	    act( message, rch, obj, NULL, TO_CHAR );
	}

        if (IS_OBJ_STAT2(obj, ITEM_DAEMONSEED) && obj != NULL
	    && obj->in_obj == NULL && (locate_obj(obj))->people )
	{
	    char buf[MAX_STRING_LENGTH];
	    CHAR_DATA *vch;
	    int wdam;

	    sprintf(buf, "%s suddenly explodes in a ball of flame, incinerating you!\n\r",obj->short_descr);
	    buf[0] = UPPER(buf[0]);
	    if ((locate_obj(obj))->people == NULL) break;
	    for ( vch = (locate_obj(obj))->people; vch != NULL;vch=vch->next_in_room )
    	    {
		if (vch->class == 0 || (!IS_NPC(vch) && vch->level < 3)) continue;
	    if ( IS_SET(vch->in_room->room_flags,ROOM_SAFE) )
	    {
		stc("You are unaffected by the blast.\n\r",vch);
		continue;
	    }
	    wdam = obj->level + dice(12,50);
            damage(vch,vch,obj->level+ dice(12, 50),gsn_inferno); 
		send_to_char(buf, vch);
	    sprintf(buf,"The flames strike you incredibly hard![%d]\n\r",wdam);
		stc(buf,vch);
 	    }
	}

        /*
         * BUGFIX : The famous extract_obj bug....
         *          basicly we just make sure that we don't skip into the obj_free list
         *          by accident. (That's what caused it).
         */

        if (obj == object_list)
        {
          extract_obj(obj);
          obj_next = object_list;
        }
        else
        {
          OBJ_DATA *bugObj;

          for (bugObj = object_list; bugObj; bugObj = bugObj->next)
          {
            if (bugObj->next == obj) break;
          }
          /*
           * This shouldn't happen, but if it does, I want to make sure we never see that object again,
           * so we let extract_obj() try and handle it, terminating to avoid further corruption.
           */
          if (!bugObj)
          {
            bug("obj_update: obj %d not in object_list. Terminating obj_update.", obj->pIndexData->vnum);
            return;
          }
          extract_obj(obj);
          obj_next = bugObj->next;
        }
    }

    return;
}

 
void embrace_update( void )
{
    DESCRIPTOR_DATA *d;
    CHAR_DATA       *ch;
    CHAR_DATA       *victim;
    int blpr;  /* variable to check for amout of blood sucked. Shakti */
    char buf[MAX_STRING_LENGTH];

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
      
      if ( ( ch = d->character) == NULL )
	continue;

      if ( IS_CLASS(ch, CLASS_MONK) && ch->fighting == NULL )
	ch->monkblock = 0;

      if ( IS_NPC(ch) || ch->embracing == NULL )
        continue;

	if (!IS_CLASS(ch,CLASS_VAMPIRE))
	  stop_embrace(ch,NULL);

	victim=ch->embracing;

/* Fix for embracing mobs by Shakti.					*/

	if (IS_NPC(victim) )
	{
		if (victim->level > 75 * ch->spl[2] )
		{
			stop_embrace(ch,victim);
			send_to_char("You cannot embrace someone so powerful!\n\r",ch);
			return;
		}
		
	/* To keep how much blood was sucked, and how much gained the same. Shakti */
	
		(blpr =number_range(30,40));
		victim->practice -=blpr;
		ch->pcdata->condition[COND_THIRST] +=blpr;
		
		sprintf(buf, "%s shudders in ecstacy as he drinks blood from %s's neck.\n\r", ch->name,victim->short_descr);
	    act(buf,ch,NULL,NULL,TO_ROOM);
	    sprintf(buf, "You shudder in ecstacy as you drink blood from %s's neck.\n\r",victim->short_descr);
	    send_to_char(buf,ch);
	    
		if (victim->practice < 0)
		{
			victim->practice = 0;
		}

		if (victim->practice == 0)
		
		{
	     raw_kill(victim);
	     sprintf(buf,"%s's body falls lifless to the ground!.\n\r",victim->short_descr);
	     act(buf,ch,NULL,NULL,TO_ROOM);
	     send_to_char(buf,ch);
	     stop_embrace(ch,victim);
		 return;
		}

		if ( ch->pcdata->condition[COND_THIRST] > (3000 / ch->generation))
		{
	 	  ch->pcdata->condition[COND_THIRST] = (3000 / ch->generation);
		  stc("Your bloodlust is sated.\n\r",ch);
		}

		continue;
	}



	if (get_char_world(ch,victim->name) == NULL)
	  {stop_embrace(ch,NULL);continue;}
	if (ch->in_room != victim->in_room)
	  stop_embrace(ch,victim);
	if( ch->in_room->vnum == 3054 )
	  stop_embrace(ch,victim);
	if (victim->pcdata->condition[COND_THIRST] < 0)
	  victim->pcdata->condition[COND_THIRST] = 0;
       
	ch->pcdata->condition[COND_THIRST] += number_range(30,40);
    sprintf(buf, "%s shudders in ecstacy as he drinks blood from %s's neck.\n\r", ch->name,victim->name);
	act(buf,ch,NULL,NULL,TO_ROOM);
	sprintf(buf, "You shudder in ecstacy as you drink blood from %s's neck.\n\r",victim->name);
	send_to_char(buf,ch);
	sprintf(buf, "You feel some of your life slip away as %s drinks from your neck.\n\r", ch->name);
	send_to_char(buf,victim);
	ch->pcdata->condition[COND_THIRST] += number_range(35,40);
	victim->pcdata->condition[COND_THIRST] -= number_range(40,42);
	if( victim->pcdata->condition[COND_THIRST] > 0 ) 
	  victim->pcdata->condition[COND_THIRST]=0;

	if (ch->pcdata->condition[COND_THIRST] > 3000/ch->generation)

	{ch->pcdata->condition[COND_THIRST]=3000/ch->generation;
	send_to_char("Your bloodlust is sated.\n\r",ch);}
	if (victim->pcdata->condition[COND_THIRST] < 0)
	  victim->pcdata->condition[COND_THIRST]=0;
	if( !IS_NPC(victim) && victim->pcdata->condition[COND_THIRST] != 0 )
	  victim->pcdata->condition[COND_THIRST]=0;

    continue; 
  }
  return;
}

void ww_update( void )
{
    DESCRIPTOR_DATA *d;
    CHAR_DATA       *victim;
    float            dam = 0;

    for ( d = descriptor_list; d != NULL; d = d->next )
      {
      if (!IS_PLAYING(d) || (victim = d->character) == NULL
        || IS_NPC(victim) || IS_IMMORTAL(victim)
	|| victim->in_room == NULL || victim->pcdata->chobj != NULL
        || IS_CLASS(victim,CLASS_WEREWOLF))
	{
            continue;
        }
      if ( !IS_SET( d->character->in_room->room_flags, ROOM_BLADE_BARRIER ) )
        continue;

      act( "The scattered blades on the ground fly up into the air ripping into you.", d->character, NULL, NULL, TO_CHAR );
      act( "The scattered blades on the ground fly up into the air ripping into $n.", d->character, NULL, NULL, TO_ROOM );

      act( "The blades drop to the ground inert.", d->character, NULL, NULL, TO_CHAR );
      act( "The blades drop to the ground inert.", d->character, NULL, NULL, TO_ROOM );

      dam = number_range( 7, 14 );
      dam = dam / 100;
      dam = d->character->hit * dam;
      if ( dam < 100 ) dam = 100;
      d->character->hit = d->character->hit - dam;
      if ( d->character->hit < -10 ) d->character->hit = -10;
      update_pos( victim );
      }

    return;
}


/*
 * Handle all kinds of updates.
 * Called once per pulse from game loop.
 * Random times to defeat tick-timing clients and players.
 */
void update_handler( void )
{
  static  int     pulse_area;
  static  int     pulse_mobile;
  static  int     pulse_violence;
  static  int     pulse_point;
  static  int     pulse_ww;
  static  int     pulse_embrace;
  static  int     pulse_minute;

  /* need to do this each cycle - Jobo */
  recycle_descriptors();

  if (--pulse_minute   <= 0)
  {
    pulse_minute = PULSE_MINUTE;
    update_ragnarok();
    update_arena();
    update_doubleexp();
    update_doubleqps();
  }
  if ( --pulse_ww       <= 0 )
  {
    pulse_ww        = PULSE_WW;
    ww_update       ( );
  }
  if ( --pulse_area     <= 0 )
  {
    pulse_area = number_range( PULSE_AREA / 2, 3 * PULSE_AREA / 2 );
    area_update ( );
  }
  if ( --pulse_mobile   <= 0 )
  {
    pulse_mobile = PULSE_MOBILE;
    mobile_update ( );
  }
  if ( --pulse_violence <= 0 )
  {
    pulse_violence  = PULSE_VIOLENCE;
    violence_update ( );
  }
  if ( --pulse_embrace <= 0)
  {
    pulse_embrace = PULSE_EMBRACE;
    embrace_update ( ); 
  }
  if ( --pulse_point    <= 0 )
  {
    pulse_point    = number_range( PULSE_TICK / 2, 3 * PULSE_TICK / 2 );
    weather_update  ( );
    char_update ( );
    obj_update  ( );
    room_update ( );

    /*
     * The following is some excessive force.
     * If the mud crashes again within a pulse from last crash,
     * let it go down to avoid crashloops.
     */
    if(iDelete == 0) iDelete++;
    else if(iDelete == 1)
    {
      unlink(CRASH_TEMP_FILE);
      iDelete++;
    }
  }
  tail_chain( );
  return;
}


void update_morted_timer(CHAR_DATA *ch)
{
	if( ch->hit > 0 && ch->pcdata->mortal > 0 ) ch->pcdata->mortal = 0;
	else	 
	{
		ch->pcdata->mortal += 1;		
		if( ch->pcdata->mortal > 10 )
		{
			ch->hit = 100;
			update_pos(ch);
			char_from_room(ch);
			char_to_room(ch,get_room_index(3054));
			stc( "You recieve a heal from the gods.\n\r",ch);
			ch->pcdata->mortal = 0;
                        ch->fight_timer = 0;
		}
	}
}

void update_sit_safe_counter(CHAR_DATA *ch)
{
  if (IS_NPC(ch)) return;

  if (ch->pcdata->safe_counter > 0) ch->pcdata->safe_counter--;

	if(IS_SET(ch->in_room->room_flags, ROOM_SAFE) && ch->level > 2 && ch->level < MAX_LEVEL -5 && ch->max_hit > 15000)
	{
		if (!IS_SET(ch->extra,EXTRA_AFK)) ch->pcdata->sit_safe += number_range(5,10);

		if( ch->hit != ch->max_hit )
			ch->pcdata->sit_safe = 0;

		if((ch->pcdata->sit_safe > 200 || ch->fight_timer != 0) && !ragnarok)
		{
			stc( "\nThe gods are tired of granting you refuge.\n\r",ch);
			char_from_room(ch);
			char_to_room(ch,get_room_index(3005));
		}
	}
        else
	{
		if (ch->pcdata->sit_safe > 0) ch->pcdata->sit_safe -=10;
		else ch->pcdata->sit_safe = 0;
	}
}

void update_drunks(CHAR_DATA *ch)
{

  if (ch->pcdata->condition[COND_DRUNK] > 10 && number_range(1,10) == 1)
  {
    send_to_char("You hiccup loudly.\n\r",ch);
    act("$n hiccups.",ch,NULL,NULL,TO_ROOM);
  }
  if (ch->pcdata->condition[COND_DRUNK] > 0 && number_range(1,10) == 1)
  {
    ch->pcdata->condition[COND_DRUNK]--;
  }
}

void sex_update(CHAR_DATA *ch)
{
	if (ch->pcdata->stage[0] > 0 || ch->pcdata->stage[2] > 0)
	{
		CHAR_DATA *vch;
		if (ch->pcdata->stage[1] > 0 && ch->pcdata->stage[2] >= 225)
		{
			ch->pcdata->stage[2] += 1;
			if ( ( vch = ch->pcdata->partner ) != NULL &&
			!IS_NPC(vch) && vch->pcdata->partner == ch 
			&& ((vch->pcdata->stage[2] >= 200 && vch->sex == SEX_FEMALE) 
			|| (ch->pcdata->stage[2] >= 200 && ch->sex == SEX_FEMALE)))
			{
				if (ch->in_room != vch->in_room)return ;
				if (vch->pcdata->stage[2] >= 225 
				&& ch->pcdata->stage[2] >= 225 
				&& vch->pcdata->stage[2] < 240 
				&& ch->pcdata->stage[2] < 240)
				{
					ch->pcdata->stage[2] = 240;
					vch->pcdata->stage[2] = 240;
				}
				if (ch->sex == SEX_MALE && vch->pcdata->stage[2] >= 240)
				{
					act("You thrust deeply between $N's warm, damp thighs.",ch,NULL,vch,TO_CHAR);
					act("$n thrusts deeply between your warm, damp thighs.",ch,NULL,vch,TO_VICT);
					act("$n thrusts deeply between $N's warm, damp thighs.",ch,NULL,vch,TO_NOTVICT);
					if (vch->pcdata->stage[2] > ch->pcdata->stage[2])
						ch->pcdata->stage[2] = vch->pcdata->stage[2];
				}
				else if (ch->sex == SEX_FEMALE && vch->pcdata->stage[2] >= 240)
				{
					act("You squeeze your legs tightly around $N, moaning loudly.",ch,NULL,vch,TO_CHAR);
					act("$n squeezes $s legs tightly around you, moaning loudly.",ch,NULL,vch,TO_VICT);
					act("$n squeezes $s legs tightly around $N, moaning loudly.",ch,NULL,vch,TO_NOTVICT);
					if (vch->pcdata->stage[2] > ch->pcdata->stage[2])
						ch->pcdata->stage[2] = vch->pcdata->stage[2];
				}
			}
			if (ch->pcdata->stage[2] >= 250)
			{
				if ( ( vch = ch->pcdata->partner ) != NULL &&
					!IS_NPC(vch) && vch->pcdata->partner == ch &&
					ch->in_room == vch->in_room)
				{
					vch->pcdata->stage[2] = 250;
					if (ch->sex == SEX_MALE)
					{
						stage_update(ch,vch,2,"xm-thrust");
						stage_update(vch,ch,2,"xf-squeeze");
					}
					else
					{
						stage_update(vch,ch,2,"xm-thrust");
						stage_update(ch,vch,2,"xf-squeeze");
					}
											
					ch->pcdata->stage[0] = 0;
					vch->pcdata->stage[0] = 0;
					
					if (!IS_EXTRA(ch, EXTRA_EXP))
					{
						send_to_char("Congratulations on achieving a simultanious orgasm!  Recieve 100000 exp!\n\r",ch);
						SET_BIT(ch->extra, EXTRA_EXP);
						ch->exp += 100000;
					}
					if (!IS_EXTRA(vch, EXTRA_EXP))
					{
						send_to_char("Congratulations on achieving a simultanious orgasm!  Recieve 100000 exp!\n\r",vch);
						SET_BIT(vch->extra, EXTRA_EXP);
						vch->exp += 100000;
					}
				}
			}
		}
		else
		{
			if (ch->pcdata->stage[0] > 0 && ch->pcdata->stage[2] < 1 &&
				ch->position != POS_RESTING) 
			{
				if (ch->pcdata->stage[0] > 1)
					ch->pcdata->stage[0] -= 1;
				else
					ch->pcdata->stage[0] = 0;
			}
			else if (ch->pcdata->stage[2]>0 && ch->pcdata->stage[0] < 1)
			{
				if (ch->pcdata->stage[2] > 10)
					ch->pcdata->stage[2] -= 10;
				else
					ch->pcdata->stage[2] = 0;
				if (ch->sex == SEX_MALE && ch->pcdata->stage[2] == 0)
					send_to_char("You feel fully recovered.\n\r",ch);
			}
		}
	}
}

void update_safe_powers(CHAR_DATA *ch)
{
  int cost = 800;
  bool cheating = FALSE;
  char buf[MAX_STRING_LENGTH];

  /* Arene chaters getting it the hard way */
  if (ch->in_room != NULL && IS_SET(ch->in_room->room_flags, ROOM_ARENA) && ch->level < 7)
  {
    if (IS_SET(ch->act, PLR_WIZINVIS)) cheating = TRUE;
    if (IS_SET(ch->act, AFF_HIDE)) cheating = TRUE;
    if (IS_SET(ch->newbits, NEW_DARKNESS)) cheating = TRUE;
    if (IS_AFFECTED(ch, AFF_PEACE)) cheating = TRUE;
    if (IS_AFFECTED(ch,AFF_SHADOWPLANE)) cheating = TRUE;
    if (IS_AFFECTED(ch,AFF_ETHEREAL)) cheating = TRUE; 
    if (!IS_HEAD(ch,LOST_HEAD) && IS_EXTRA(ch,EXTRA_OSWITCH)) cheating = TRUE;
    if (cheating)
    {
      ch->hit = ch->hit/2; // Punish those bastards
      sprintf(buf, "%s is cheating in the arena!!!", ch->name);
      do_info(ch, buf);
    }
  }
  if (ch->position != POS_STANDING && ch->position != POS_STUNNED)
  {
    cost *= 4;
  }
  if (IS_CLASS(ch, CLASS_WEREWOLF)) cost *= 1.5;
  if (!IS_HEAD(ch,LOST_HEAD) && IS_EXTRA(ch,EXTRA_OSWITCH))
  {
    if (ch->move >= cost) ch->move -=cost;
    else do_humanform(ch,"");
  }
  if (IS_SET(ch->act, PLR_WIZINVIS))
  {
    if (ch->move >= cost) ch->move -=cost;
    else
    {
      REMOVE_BIT(ch->act, PLR_WIZINVIS);
      act( "$n slowly fades into existence.", ch, NULL, NULL, TO_ROOM );
      send_to_char( "You slowly fade back into existence.\n\r", ch );
    }
  }
  if (IS_SET(ch->act, AFF_HIDE))
  {
    if (ch->move >= cost) ch->move -= cost;
    else
    {
      REMOVE_BIT(ch->act, AFF_HIDE);
      act( "$n slowly fades into existence.", ch, NULL, NULL, TO_ROOM );
      send_to_char( "You slowly fade back into existence.\n\r", ch );
    }
  }
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ANGEL))
  {
    if (IS_SET(ch->pcdata->powers[ANGEL_POWERS], ANGEL_EYE))
    {
      if(ch->move < cost*2 || ch->mana < cost*2)
      {
        REMOVE_BIT(ch->pcdata->powers[ANGEL_POWERS], ANGEL_EYE);
        send_to_char("The sinners have payed for their sins, time to rest.\n\r",ch);
      }
      else
      {
        ch->move -= cost*2;
        ch->mana -= cost*2;
      }
    }
  }
  if (IS_SET(ch->newbits, NEW_DARKNESS))
  {
    if (ch->mana >= cost) ch->mana -= cost;
    else
    {
      REMOVE_BIT(ch->newbits, NEW_DARKNESS);
      REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
      send_to_char("You cannot pay the upkeep and banish your globe of darkness.\n\r", ch );
      act("The globe of darkness around $n disappears.",ch,NULL,NULL,TO_ROOM);
    }
  }
  if (IS_AFFECTED(ch, AFF_PEACE))
  {
    if (ch->mana >= cost) ch->mana -= cost;
    else if (!IS_CLASS(ch, CLASS_ANGEL))
    {
      REMOVE_BIT(ch->affected_by, AFF_PEACE);
      act( "You lower your majesty.",  ch, NULL, NULL, TO_CHAR    );
      act( "$n looks less imposing.",  ch, NULL, NULL, TO_NOTVICT );
    }
    else
    {   
      REMOVE_BIT(ch->affected_by, AFF_PEACE);
      act( "God protects you no longer.",  ch, NULL, NULL, TO_CHAR    );
      act( "$n looks wicked.",  ch, NULL, NULL, TO_NOTVICT );
    }
  }
  if (IS_AFFECTED(ch,AFF_SHADOWPLANE))
  {
    if (ch->mana >= cost) ch->mana -= cost;
    else
    {
      REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
      send_to_char("You fade back into the real world.\n\r",ch);
      act("The shadows flicker and $n fades into existance.",ch,NULL,NULL,TO_ROOM);
    }
  }
  if (IS_AFFECTED(ch,AFF_ETHEREAL))
  {
    if (ch->mana >= cost) ch->mana -= cost;
    else
    {
      REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);
      send_to_char("Your body becomes firm again.\n\r",ch);
      act("$n becomes sold again.",ch,NULL,NULL,TO_ROOM);
    }
  }

  /* Has nothing to do with safe powers, just needed it somewhere, so here it is */
  if (IS_SET(ch->newbits, NEW_VISION))
  {
    if (number_percent() < 15 && IS_SET(ch->itemaffect, ITEMA_VISION))
    {
      send_to_char("#pYour superior sight fades away.#n\n\r",ch);
      REMOVE_BIT(ch->itemaffect, ITEMA_VISION);
      REMOVE_BIT(ch->newbits, NEW_VISION);
    }
  }
  return;
}

void update_vampire_regen(CHAR_DATA *ch)
{
  if ( ch->position == POS_FIGHTING && ch->rage > 0 && ch->rage < 25 && !IS_ITEMAFF(ch, ITEMA_RAGER))
    ch->rage += 1;
  else if (ch->rage > 0 && !IS_ITEMAFF(ch, ITEMA_RAGER))
    ch->rage -= 1;
  if (ch->rage == 0) return;
  if (( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move) && ch->rage >0)
  {
    werewolf_regen(ch, 2);
    regen_limb(ch);
	}
  return;
}

void update_vampire(CHAR_DATA *ch)
{
  if ( ch->in_room != NULL && ( ch->in_room->vnum >= 93350 && ch->in_room->vnum <= 93359))
  {
    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch, 1);
  }
  return;
}

void update_cyborg (CHAR_DATA *ch)
{
  if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
    werewolf_regen(ch, 2);
  regen_limb(ch);
  return;
}

void update_knight (CHAR_DATA *ch)
{
  if ( ch->in_room != NULL && ( ch->in_room->vnum >= 93300 && ch->in_room->vnum <= 93309))
  {
    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch, 1);
  }
  if (ch->pcdata->powers[POWER_TICK] > 0) ch->pcdata->powers[POWER_TICK]--;
  if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
    werewolf_regen(ch, 2);
  regen_limb(ch);
  return;
}

void update_shapeshifter (CHAR_DATA *ch)
{
  if ( ch->in_room != NULL && ( ch->in_room->vnum >= 93310 && ch->in_room->vnum <= 93319))
  {  
    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch, 1);
  }
  if (ch->pcdata->powers[PHASE_COUNTER] > 0) ch->pcdata->powers[PHASE_COUNTER]--;
  if (ch->pcdata->powers[SHAPE_COUNTER] > 0) ch->pcdata->powers[SHAPE_COUNTER]--;
  if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
    werewolf_regen(ch, 2);
  regen_limb(ch);
  return;
}

void update_tanarri(CHAR_DATA *ch)
{
  if ( ch->in_room != NULL && ( ch->in_room->vnum >= 93330 && ch->in_room->vnum <= 93339))
  {
    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch, 1);
  }
  return;
}

void update_drider(CHAR_DATA *ch)
{
  if ( ch->in_room != NULL && ( ch->in_room->vnum >= 93350 && ch->in_room->vnum <= 93359))
  {
    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch, 1);
  }
  return;
}

void update_lich_regen(CHAR_DATA *ch)
{
  if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
    werewolf_regen(ch, 2);
  regen_limb(ch);
  return;
}

void update_lich(CHAR_DATA *ch)
{
  if ( ch->in_room != NULL && ( ch->in_room->vnum >= 93360 && ch->in_room->vnum <= 93369))
  {
    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch, 1);
  }
  if (ch->position == POS_MEDITATING && ch->mana < ch->max_mana)
  {
    ch->mana += number_range(1000,2000);
    if (ch->mana > ch->max_mana)
      ch->mana = ch->max_mana;
  }
  if (IS_SET(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_FIRE) && number_range(1,50) == 4)
  {
    send_to_char("You can summon another fire golem.\n\r",ch);
    REMOVE_BIT(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_FIRE);
  }
  if (IS_SET(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_IRON) && number_range(1,50) == 4)
  {
    send_to_char("You can summon another iron golem.\n\r",ch);
    REMOVE_BIT(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_IRON);
  }
  if (IS_SET(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_CLAY) && number_range(1,50) == 4)
  {
    send_to_char("You can summon another clay golem.\n\r",ch);
    REMOVE_BIT(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_CLAY);
  }
  if (IS_SET(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_STONE) && number_range(1,50) == 4)
  {
    send_to_char("You can summon another stone golem.\n\r",ch);
    REMOVE_BIT(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_STONE);
  }
  return;
}

void update_angel(CHAR_DATA *ch)
{
  if (ch->pcdata->powers[ANGEL_LOVE] > 3)
  {
    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
      werewolf_regen(ch, 2);
    regen_limb(ch);
  }
  if (ch->pcdata->powers[ANGEL_PEACE_COUNTER] > 0) ch->pcdata->powers[ANGEL_PEACE_COUNTER]--;
  if ( ch->in_room != NULL && ( ch->in_room->vnum >= 93340 && ch->in_room->vnum <= 93349))
  {
    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch, 1);
  }
  return;
}


void update_monk(CHAR_DATA *ch)
{
  if ( ch->in_room != NULL && ( ch->in_room->vnum >= 93410 && ch->in_room->vnum <= 93419))
  {
    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch, 1);
  } 
  if (!IS_ITEMAFF(ch, ITEMA_MONKCHI))
  {
    if (ch->position != POS_FIGHTING 	&& ch->chi[CURRENT] > 0	&& number_range( 1,4 ) == 2)
    {
      ch->chi[CURRENT]--;
      stc("You feel more relaxed.\n\r",ch);
      act("$n looks more relaxed.",ch,NULL,NULL,TO_ROOM);
    }
  }
  if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
    werewolf_regen(ch, 2);
  regen_limb(ch); 
  return;
}

void update_ninja(CHAR_DATA *ch)
{
  if ( ch->in_room != NULL && ( ch->in_room->vnum >= 93460 && ch->in_room->vnum <= 93469))  
  {
    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch, 1);
  }
  if (ch->position == POS_FIGHTING && ch->rage > 0 && ch->rage < 25 && !IS_ITEMAFF(ch, ITEMA_RAGER))
    ch->rage += 1;
  else if (ch->rage > 0 && !IS_ITEMAFF(ch, ITEMA_RAGER))
    ch->rage -= 1;
  if (ch->rage < 1) return;
  if (ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
  {
    if (ch->position == POS_MEDITATING) werewolf_regen(ch,3);
    else werewolf_regen(ch,2);
  }
  regen_limb(ch);
  return;
}

void update_werewolf(CHAR_DATA *ch)
{
  if ( ch->in_room != NULL && ( ch->in_room->vnum >= 93430 && ch->in_room->vnum <= 93439))
  {
    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch, 1);
  }
  if (!IS_ITEMAFF(ch, ITEMA_RAGER))
  {
    if (ch->position == POS_FIGHTING)
    {
      if (ch->rage < 200) ch->rage += number_range(5,10);
      if (ch->rage < 200 && ch->pcdata->powers[DISC_WERE_WOLF] > 3)	ch->rage += number_range(5,10);
      if (!IS_SET(ch->special, SPC_WOLFMAN) && ch->rage >= 100)	do_werewolf(ch,"");
    }
    else if (ch->rage > 0)
    {
      ch->rage -= 1;
      if (ch->rage < 100) do_unwerewolf(ch,"");
    }
  }
  if (ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
    werewolf_regen(ch,2);
	regen_limb(ch);
  return;
}

void update_demon(CHAR_DATA *ch)
{
  if ( ch->in_room != NULL && ( ch->in_room->vnum >= 93420 && ch->in_room->vnum <= 93426))
  {
    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch, 2);
  }
  regen_limb(ch);
}

void update_demon_regen(CHAR_DATA *ch)
{
  if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
    werewolf_regen(ch, 2);
  regen_limb(ch);
  return;
}

void update_drow(CHAR_DATA *ch)
{
  if ( ch->in_room != NULL && ( ch->in_room->vnum >= 93440 && ch->in_room->vnum <= 93446))
  {
    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch, 1);
  }
  return;
}

void update_highlander(CHAR_DATA *ch)
{
  if ( ch->in_room != NULL && ( ch->in_room->vnum >= 93320 && ch->in_room->vnum <= 93326))
  {
    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch, 1);
  }
  if (get_eq_char(ch, WEAR_WIELD) != NULL)
  {
    int wpn = (ch->wpn[1]/3);
    if ( ch->hit < ch->max_hit)
    {
      ch->hit += wpn;
      if (ch->hit > ch->max_hit)
        ch->hit = ch->max_hit;
    }
    if ( ch->mana < ch->max_mana)
    {
      ch->mana += wpn;
      if (ch->mana > ch->max_mana)
        ch->mana = ch->max_mana;
    }
    if ( ch->move < ch->max_move)
    {
      ch->move += wpn;
      if (ch->move > ch->max_move)
        ch->move = ch->max_move;
    }
  }
  if (ch->fighting == NULL && ch->pcdata->powers[SAMURAI_FOCUS] > 0)
    ch->pcdata->powers[SAMURAI_FOCUS]--;
  if (ch->fighting == NULL && ch->pcdata->powers[SAMURAI_LAST] > 0)
    ch->pcdata->powers[SAMURAI_LAST] = 0;
  if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
    werewolf_regen(ch, 2);
  regen_limb(ch);
  return;
}

void update_mage(CHAR_DATA *ch)
{
  if ( ch->in_room != NULL && ( ch->in_room->vnum >= 93400 && ch->in_room->vnum <= 93406))
  {
    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch, 1);
  }
  if (ch->position == POS_MEDITATING && ch->mana < ch->max_mana)
  {
    ch->mana += number_range(1000,2000);
    if (ch->mana > ch->max_mana)
      ch->mana = ch->max_mana;
  }
  return;
}

void update_arti_regen(CHAR_DATA *ch)
{
  if (ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
    werewolf_regen(ch, 2);
  regen_limb(ch);
  return;
}

void regen_limb(CHAR_DATA *ch)
{
  if (ch->loc_hp[6] > 0)
  {
    int sn = skill_lookup( "clot" );
    (*skill_table[sn].spell_fun) (sn,ch->level,ch,ch);
  }
  else if ((ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] + ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5]) != 0)
    reg_mend(ch);
  return;
}

void werewolf_regen( CHAR_DATA *ch, int multiplier )
{
	int min = 5;
	int max = 10;
	int hit_gain = 0;
	int mana_gain = 0;
	int move_gain = 0;
	
	if (IS_NPC(ch)) return;
  else if (IS_CLASS(ch, CLASS_WEREWOLF) && (ch->position == POS_SLEEPING || ch->position == POS_FIGHTING))
  {
    if (ch->position == POS_SLEEPING && ch->power[DISC_WERE_BEAR] > 3)
    {
      min += 1200;
      max += 1700;
      hit_gain += number_range(min,max);
      mana_gain += number_range(min/2,max/2);
      move_gain += number_range(min,max);
    }
    else if (ch->position == POS_FIGHTING && ch->power[DISC_WERE_BEAR] > 3)
    {
      min += 275;
      max += 475;
      hit_gain += number_range(min,max);
      mana_gain += number_range(min/2,max/2);
      move_gain += number_range(min,max);
    }
  }
  else if (ch->position == POS_SLEEPING)
  {
    min += 300;
    max += 500;
    hit_gain += number_range(min,max);
    mana_gain += number_range(min,max);
    move_gain += number_range(min,max);
  }
  else
  {
    min += 100;
    max += 200;
    hit_gain += number_range(min,max);
    mana_gain += number_range(min,max);
    move_gain += number_range(min,max);
  }
  ch->hit = UMIN (ch->hit+(hit_gain*multiplier), ch->max_hit);
  ch->mana = UMIN (ch->mana+(mana_gain*multiplier), ch->max_mana);
  ch->move = UMIN (ch->move+(move_gain*multiplier), ch->max_move);
  update_pos(ch);
  if ( ch->hit >= ch->max_hit && ch->mana >= ch->max_mana && ch->move >= ch->max_move )
    send_to_char("Your body has completely regenerated itself.\n\r",ch);
  return;
}
