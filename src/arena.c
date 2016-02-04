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

/***********************
 * Arena stuff by Jobo *
 ***********************/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

#define ARENA_LVNUM     50 // lower vnum for the arena
#define ARENA_HVNUM     67 // upper vnum for the arena
#define ARENA_PLAYERS    6 // max players in the arena

int next_arena_room;

void do_arenastats(CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];
  DESCRIPTOR_DATA *d;
  int hp_stats = 0;
  int mana_stats = 0;
  int move_stats = 0;

  if (IS_NPC(ch)) return;
  if (ch->in_room != NULL)
  {
    if (IS_SET (ch->in_room->room_flags, ROOM_ARENA))
    {
      send_to_char("Your in the arena.\n\r",ch);
      return;
    }
  }
  send_to_char("#G            People in the arena#n\n\r\n\r",ch);
  send_to_char("#RName                Health   Stamina     Mana#n\n\r",ch);
  send_to_char("#0----------------------------------------------#n\n\r",ch);
  for ( d = descriptor_list; d != NULL; d = d->next )
  {
    if (d->character != NULL)
    {
      if (d->character->in_room != NULL)
      {
        if (!IS_SET(d->character->in_room->room_flags, ROOM_ARENA)) continue;
        if (d->character->max_hit > 0)  hp_stats   = 100 * d->character->hit / d->character->max_hit;
        if (d->character->max_move > 0) move_stats = 100 * d->character->move / d->character->max_move;
        if (d->character->max_mana > 0) mana_stats = 100 * d->character->mana / d->character->max_mana;
        sprintf(buf,"%-15s    %3d/100   %3d/100   %3d/100\n\r", d->character->name, hp_stats, move_stats, mana_stats);
        send_to_char(buf,ch);
      }
    }
  }
  return;
}

void open_arena()
{
  arenaopen = TRUE;
  next_arena_room = ARENA_LVNUM; // first person to join will be put in this room.

  if (number_range(1,10) > 3)
  {
    arena_base = FALSE;
    do_info(NULL,"The arena is now open for EVERYONE '#Larenajoin#n'");
  }
  else
  {
    do_info(NULL,"The arena is now open for BASE classes only '#Larenajoin#n'");
    arena_base = TRUE;
  }
}

void close_arena()
{
  CHAR_DATA *gch;
  CHAR_DATA *vch;
  int arenaplayers = 0;

  arenaopen = FALSE;

  /* unfreeze all players */
  for (vch=char_list;vch!=NULL;vch=vch->next)
  {
    if (IS_NPC(vch)) continue;
    if (vch->in_room != NULL)
    {
      if (IS_SET (vch->in_room->room_flags, ROOM_ARENA))
      {
        REMOVE_BIT(vch->act, PLR_FREEZE);
        gch = vch;
        arenaplayers++;
      }
    }
  }

  /* if there was only one player, remove him */
  if (arenaplayers <= 1)
  {
    if (arenaplayers) do_transfer(gch,"self 3054");
    do_info(NULL, "The Arena fight was cancelled due to lack of players!");
    return;
  }
  else do_info(NULL, "The arena is now closed, let the games begin!");
  return;
}

void do_arenajoin(CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];
  int arenapeople=0;
  DESCRIPTOR_DATA *d;

  if (IS_NPC(ch)) return;
  if (ch->fight_timer > 0)
  {
    send_to_char("You have a fighttimer.\n\r",ch);
    return;
  }
  if ((get_age(ch) - 17) < 2)
  {
    send_to_char("Your still a newbie.\n\r",ch);
    return;
  }
  if (!arenaopen)
  {
    send_to_char("The arena is closed.\n\r",ch);
    return;
  }
  if (arena_base && is_upgrade(ch))
  {
    send_to_char("Your an upgrade, not for you this time.\n\r",ch);
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
  for ( d = descriptor_list; d != NULL; d = d->next )
  {
    if ( d->character != NULL )
    {
      if (!d->connected == CON_PLAYING) continue;
      if (d->character->in_room != NULL)
      {
        if (IS_SET (d->character->in_room->room_flags, ROOM_ARENA)) arenapeople++;
      }
    }
  }
  if (arenapeople > ARENA_PLAYERS)
  {
    send_to_char("The arena is crowded atm.\n\r",ch);
    return;
  }
  char_from_room(ch);
  char_to_room(ch, get_room_index(next_arena_room));
  next_arena_room += (ARENA_HVNUM - ARENA_LVNUM) / ARENA_PLAYERS;
  sprintf(buf,"%s has joined the arena!",ch->name);
  do_info(ch,buf);
  do_restore(ch,"self");
  SET_BIT(ch->act, PLR_FREEZE);
  return;
}

void do_resign(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  CHAR_DATA *gch;
  int found = 0;
  ROOM_INDEX_DATA *location;
  char buf[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;
  if (ch->in_room != NULL)
  {
    if (!IS_SET(ch->in_room->room_flags, ROOM_ARENA))
    {
      send_to_char("Your not in the arena.\n\r",ch);
      return;
    }
  }
  sprintf(buf,"%s resigns from the arena",ch->name);
  do_info(ch,buf);
  if ((location = get_room_index(ROOM_VNUM_ALTAR)) == NULL) return;
  char_from_room(ch);
  char_to_room(ch, location);
  ch->fight_timer = 0;
  do_restore(ch, "self");
  do_call(ch, "all");
  ch->pcdata->alosses++;
  for (victim = char_list; victim != NULL; victim = victim->next)
  {
    if (IS_NPC(victim)) continue;
    if (victim->in_room != NULL && IS_SET(victim->in_room->room_flags, ROOM_ARENA) && victim->pcdata->chobj == NULL)
    {
      gch = victim;
      found++;
    }
  }
  if (found == 1)
  {
    sprintf(buf,"#C%s #oemerges victorious from the #Rarena#n",gch->name);
    gch->pcdata->awins++;
    do_info(gch,buf);
    if ((location = get_room_index(ROOM_VNUM_ALTAR)) == NULL) return;
    char_from_room(gch);
    char_to_room(gch, location);
    gch->fight_timer = 0;
    do_restore(gch, "self");
    win_prize(gch);
    pulse_arena = PULSE_ARENA;
  }
  return;
}
