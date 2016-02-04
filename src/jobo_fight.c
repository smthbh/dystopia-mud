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

/***************************
 * Combat commands by Jobo *
 ***************************/
  
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

void do_pkcall(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *gch;
  
  if (IS_NPC(ch)) return;  
  if (get_ratio(ch) < 3000)
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->fighting == NULL)
  {
    send_to_char("Your not engaged in combat.\n\r",ch);
    return;
  }
  do_say(ch,"TO ARMS!!! TO ARMS!!!!");
  for (gch=ch->in_room->people;gch!=NULL;gch = gch->next_in_room)
  {
    if (!IS_NPC(gch)) continue;
    if (gch == ch->fighting) continue;
    if (gch->fighting != NULL) continue;
    if (gch->level > get_ratio(ch)/2)
    {
      do_say(gch,"Fight your own fights you pansy.\n\r");
      continue;
    }
    /*
     * If the player was fighting a mob, it could actually be dead.
     */
    if (ch->fighting == NULL) return;
    if (number_range(1,100) > 33)
    {
      do_say(gch,"To arms brothers, tonight we will feast in the halls of midgaard");
      multi_hit(gch,ch->fighting,TYPE_UNDEFINED);
    }
  }
  WAIT_STATE(ch,12);
  return;
}

void do_pkmind(CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if (get_ratio(ch) < 4000)
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (IS_SET(ch->newbits, NEW_IRONMIND))
  {
    send_to_char("Your already focusing your mental powers.\n\r",ch);
    return;
  }
  SET_BIT(ch->newbits, NEW_IRONMIND);
  send_to_char("You focus your concentration on absorbing damage.\n\r",ch);
  WAIT_STATE(ch, 6);
  return;
}

/* Genstealing is in :-) */
  
void do_gensteal(CHAR_DATA *ch, char *argument)   
{
  CHAR_DATA *victim;
  ROOM_INDEX_DATA *location;
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_INPUT_LENGTH];
  
  one_argument( argument, arg);
  if (IS_NPC(ch)) return;
  if (ch->in_room != NULL)
  {
    if (IS_SET (ch->in_room->room_flags, ROOM_ARENA))
    {
      send_to_char("Your in the arena.\n\r",ch);
      return;
    }
  }
  if (ragnarok)
  {
    send_to_char("Not while ragnarok is running.\n\r",ch);
    return;
  }
  if (arg[0] == '\0')
  {
    send_to_char("Gensteal whom?\n\r",ch);
    return;
  }
  if ((victim=get_char_room(ch,arg))==NULL)
  {
    send_to_char("They aren't here.\n\r",ch);
    return;
  }
  if (ch == victim)
  {
    send_to_char("How stupid are you?!?\n\r",ch);
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on the mobs.\n\r",ch);
    return;
  }
  if (ch->class != victim->class)
  {
    send_to_char("Only on people of the same class as yourself.\n\r",ch);
    return;
  }
  if (victim->position > 1)
  {
    send_to_char("He resists your attempt to gensteal.\n\r",ch);
    return;
  }
  if (ch->generation < victim->generation)
  {
    send_to_char("Sorry, you are already a higher generation than they are.\n\r",ch);
    return;
  }
  if (victim->generation > 7)
  {
    send_to_char("They are worthless.\n\r",ch);
    return;
  }
  if (ch->generation == 1)
  {
    send_to_char("You are the highest possibly generation.\n\r",ch);
    return;
  }
  if (is_safe(ch,victim)) return;
  act("Lightning flows through your body as you steal the powers of $N.\n\r", ch, NULL, victim, TO_CHAR);
  act("$n puts his hands on $N's head and lightning cowers his body.\n\r", ch, NULL, victim, TO_NOTVICT);
  send_to_char("Your generation has been stolen.\n\r",victim);
  sprintf(buf, "#G%s #ohas beaten #L%s #oin combat, and has stolen their generation!#n",ch->pcdata->switchname,
    victim->pcdata->switchname);
  do_info(ch,buf);
  ch->generation -= 1;
  victim->generation += 1;
  ch->fight_timer +=5;
  victim->hit = 1;
  update_pos(victim);
  WAIT_STATE(ch, 12);
  if ((location = get_room_index(ROOM_VNUM_ALTAR)) == NULL)
    return;
  char_from_room(victim);  
  char_to_room(victim, location);
  victim->fight_timer = 0;
  players_gstolen++;
  return;
}

void do_combatswitch( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  
  argument = one_argument(argument,arg);
  if (IS_NPC(ch)) return;
  
  if (ch->fighting == NULL)
  {
    send_to_char("Ah, but your not really fighting anyone!\n\r",ch);
    return;
  }
  if ((victim=get_char_room(ch,arg))==NULL)
  {
    send_to_char("They aren't here.\n\r",ch);
    return;
  }
  if (ch == victim)
  {
    send_to_char("How stupid are you?!?\n\r",ch);
    return;
  }
  if (ch->fighting == victim)
  {    
    send_to_char("Maybe you should just continue fighting them.\n\r",ch);
    return;
  }
  if (victim->fighting == NULL || victim->fighting != ch)
  {
    send_to_char("You failed.\n\r",ch);
    return;
  }
  if (number_range(1,3) == 1)
  {      
    send_to_char("You failed.\n\r",ch);
    WAIT_STATE(ch, 12);
    return;
  }
  ch->fighting = victim;
  send_to_char("Hehe, bet they didn't expect that to happen...\n\r",ch);
  WAIT_STATE(ch,6);
  return;
}

