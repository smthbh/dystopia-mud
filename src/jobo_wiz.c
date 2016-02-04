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

/*********************************
 * New immortal commands by Jobo *
 *********************************/

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


void do_multicheck(CHAR_DATA *ch, char *argument)
{
  DESCRIPTOR_DATA *d;
  DESCRIPTOR_DATA *d2;
  char buf[MAX_STRING_LENGTH];
  bool found = FALSE;
  
  for ( d = descriptor_list; d != NULL; d = d->next )
  {
    if (d->lookup_status != STATUS_DONE) continue;
    if (d->connected != CON_PLAYING) continue;
    for ( d2 = d->next; d2 != NULL; d2 = d2->next )
    {
      if (d2->lookup_status != STATUS_DONE) continue;
      if (!str_cmp(d->host, d2->host))
      {
        if (d2->connected != CON_PLAYING) continue;
        if (d2->character == NULL || d->character == NULL) continue;
        found = TRUE;
        sprintf(buf, "%s and %s are multiplaying bitches\n\r",d2->character->name, d->character->name);
        send_to_char(buf,ch);
      }
    }  
  }    
  if (!found) send_to_char("Noone is multiplaying atm.\n\r",ch);
  return;
}

void do_slay2(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  
  one_argument(argument, arg);
  
  if (IS_NPC(ch)) return;
  if (ch->level < 12) return;
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
  one_hit(ch,victim, gsn_supreme,1);
  one_hit(ch,victim, gsn_supreme,1);
  one_hit(ch,victim, gsn_supreme,1);
  one_hit(ch,victim, gsn_supreme,1);
  one_hit(ch,victim, gsn_supreme,1);
  one_hit(ch,victim, gsn_supreme,1);
  one_hit(ch,victim, gsn_supreme,1);
  one_hit(ch,victim, gsn_supreme,1);
  one_hit(ch,victim, gsn_supreme,1);
  one_hit(ch,victim, gsn_supreme,1);
  return;
}

void do_openthearena( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (arena_open) return;
  arena_open = TRUE;
  return;
}

void do_showsilence( CHAR_DATA *ch, char *argument )
{
  DESCRIPTOR_DATA *d;
  CHAR_DATA *gch;
  char buf[MAX_STRING_LENGTH];
  bool found = FALSE;
  
  if (IS_NPC(ch)) return; 
  
  for ( d = descriptor_list; d != NULL; d = d->next )
  {
    if ( d->connected != CON_PLAYING) continue;
    if ( d->character != NULL ) gch = d->character;
    else continue;
    if (gch->level > 6) continue;
    if (IS_SET(gch->act, PLR_SILENCE))
    {
      found = TRUE;
      sprintf(buf,"%-15s is silenced\n\r",gch->name);
      send_to_char(buf,ch);
    }
  }
  if (!found) send_to_char("Noone is silenced.\n\r",ch);
  return;
}

void do_asperson(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  DESCRIPTOR_DATA *tmp;
  char arg[MAX_INPUT_LENGTH];
  bool afk = FALSE;

  argument = one_argument(argument, arg);

  if (IS_NPC(ch)) return;
  if (ch->level < 12) return;
  if ((victim = get_char_world(ch, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch );
    return;
  }
  if (victim == ch)
  {
    send_to_char("That would be a bad idea.\n\r", ch );
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobiles.\n\r",ch);
    return;
  }
  tmp = victim->desc;
  victim->desc = ch->desc;
  if (IS_SET(victim->extra, EXTRA_AFK))
  {
    afk = TRUE;
    REMOVE_BIT(victim->extra, EXTRA_AFK);
  }
  interpret(victim, argument);
  victim->desc = tmp;
  if (afk) SET_BIT(victim->extra, EXTRA_AFK);
  return;
}

void do_bully(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
 
  argument = one_argument(argument, arg);
  
  if (IS_NPC(ch)) return;
  if (ch->level < MAX_LEVEL) return;
  if ((victim = get_char_world(ch, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch );
    return;
  }
  if (victim == ch)
  {
    send_to_char("That would be a bad idea.\n\r", ch );
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobiles.\n\r",ch);
    return;
  }
  if (IS_SET(victim->pcdata->jflags, JFLAG_BULLY))
  {
    REMOVE_BIT(victim->pcdata->jflags, JFLAG_BULLY);
    send_to_char("You are to nice.\n\r",ch);
  }
  else
  {
    SET_BIT(victim->pcdata->jflags, JFLAG_BULLY);
    send_to_char("Take that bully.\n\r",ch);
  }
  return;
}

void do_offline(CHAR_DATA *ch, char *argument)
{
  DESCRIPTOR_DATA *dummy;
  DESCRIPTOR_DATA *temp;
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument(argument, arg);

  if (IS_NPC(ch)) return;
  if (ch->level < MAX_LEVEL)
  {
    send_to_char("This action is restricted.\n\r", ch);
    return;
  }
  if (strlen(arg) < 3)
  {
    send_to_char("What player do you wish to load?\n\r", ch);
    return;
  }
  if (argument[0] == '\0')
  {
    send_to_char("What do you want the player to do?\n\r", ch);
    return;
  }
  arg[0] = UPPER(arg[0]);
  if (descriptor_free == NULL )
  {
    dummy = alloc_perm(sizeof(*dummy));
  }
  else
  {
    dummy = descriptor_free;
    descriptor_free = descriptor_free->next;
  }
  if (load_char_obj(dummy, arg))
  {
    victim = dummy->character;
    temp = victim->desc;
    victim->desc = ch->desc;
    interpret(victim, argument);
    victim->desc = temp;
  }
  else
  {
    send_to_char("Player doesn't exist.\n\r", ch);
    dummy->next     = descriptor_free;
    descriptor_free = dummy;
    return;
  }

  /*
   * Clean up.
   */
  save_char_obj(victim);
  free_char(victim);
  dummy->next     = descriptor_free;
  descriptor_free = dummy;
  return;
}
