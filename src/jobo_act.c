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
 * Player commands by Jobo *
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

ALIAS_DATA * alias_free;

void do_bountylist(CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];
  DESCRIPTOR_DATA *d;

  stc("#r--==#L**#r==--==#L**#r==--==#L**#r==      #RBOUNTY LIST    #r==#L**#r==--==#L**#r==--==#L**#r==--#n\n\r",ch);
  stc("  #CName               Bounty     Pkscore      Generation    Level\n\r#n",ch);
  for ( d = descriptor_list; d != NULL; d = d->next )
  {
    if ( d->character != NULL )
    {
       if (!d->connected == CON_PLAYING) continue;
       if (d->character->level > 6) continue;
       if (!can_see(ch, d->character)) continue;
       sprintf(buf, "  #G%-15s#n    %5d       %5d           %d           %d\n\r",d->character->name, d->character->pcdata->bounty,
         get_ratio(d->character),d->character->generation, d->character->pcdata->upgrade_level+1);
       stc(buf,ch);
    }
  }
  stc("#r--==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==#L**#r==--==#L**#r==--==#L**#r==-#n\n\r",ch);
  return;
}

void do_version( CHAR_DATA *ch, char *argument) 
{
  send_to_char("+++++++++++++++++++++++++++++++++++++++++++++++++++\n\r", ch);
  send_to_char("+ This mud runs the final release of dystopia 1.2 +\n\r", ch);
  send_to_char("+++++++++++++++++++++++++++++++++++++++++++++++++++\n\r", ch);
  return;
}

void do_runeeq( CHAR_DATA *ch, char *argument )
{
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];
  int vnum = 0;

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;

  if (arg[0] == '\0')
  {
    send_to_char("Please specify which piece of rune armor you wish to make: Ring Collar Plate Helmet Leggings Boots Gloves Sleeves Cape Belt Bracer Mask.\n\r",ch);
    return;
  }
  if ( ch->pcdata->quest < 10000 )
  {
    send_to_char("You don't have the 10K qps needed.\n\r",ch);
    return;
  }
  if      (!str_cmp(arg,"ring"        )) vnum = 814;
  else if (!str_cmp(arg,"collar"      )) vnum = 815;
  else if (!str_cmp(arg,"plate"       )) vnum = 817;
  else if (!str_cmp(arg,"helmet"      )) vnum = 818;
  else if (!str_cmp(arg,"leggings"    )) vnum = 819;
  else if (!str_cmp(arg,"boots"       )) vnum = 820;
  else if (!str_cmp(arg,"gloves"      )) vnum = 821;
  else if (!str_cmp(arg,"sleeves"     )) vnum = 822;
  else if (!str_cmp(arg,"cape"        )) vnum = 823;
  else if (!str_cmp(arg,"belt"        )) vnum = 824;
  else if (!str_cmp(arg,"bracer"      )) vnum = 816;
  else if (!str_cmp(arg,"mask"        )) vnum = 825;
  else
  {
    do_runeeq(ch,"");
    return;
  }
  if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
  {
    send_to_char("Missing object, please inform Jobo .\n\r",ch);
    return;
  }
  if(!IS_IMMORTAL(ch)) ch->pcdata->quest -= 10000;
  obj = create_object(pObjIndex, 50);
  obj->questowner = str_dup(ch->pcdata->switchname);
  obj_to_char(obj, ch);
  ch->pcdata->rune_count++;
  act("You transform the 10K qp into $p.",ch,obj,NULL,TO_CHAR);
  act("$n forms $p out of 10K qp.",ch,obj,NULL,TO_ROOM);
  return;
}

void do_upkeep(CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  send_to_char("#RUpkeep list#n\n\r\n\r",ch);
  if (IS_SET(ch->act, AFF_HIDE))          send_to_char("* You cannot be seen by other players.\n\r",ch);
  if (IS_SET(ch->act, PLR_WIZINVIS))      send_to_char("* You cannot be seen by other players.\n\r",ch);
  if (IS_SET(ch->newbits, NEW_DARKNESS))  send_to_char("* You are shrouded in a globe of darkness.\n\r",ch);
  if (IS_AFFECTED(ch, AFF_SHADOWPLANE))   send_to_char("* You are standing in the shadowplane.\n\r",ch);
  if (IS_AFFECTED(ch, AFF_ETHEREAL))      send_to_char("* You are in ethereal form.\n\r",ch);
  if (IS_SET(ch->act,PLR_HOLYLIGHT))      send_to_char("* You have superior vision.\n\r",ch);
  if (IS_IMMUNE(ch,IMM_SHIELDED))         send_to_char("* You are shielded from scrying eyes.\n\r",ch);

  if (IS_CLASS(ch, CLASS_LICH))
  {
    if (IS_IMMUNE(ch,IMM_SHIELD2)) send_to_char("* You are shielded by the powers of chaos.\n\r",ch);
  }

  if (IS_CLASS(ch, CLASS_NINJA))
  {
    if (IS_VAMPAFF(ch, VAM_CLAWS)) send_to_char("* You have iron wrists attached.\n\r",ch);
  }

  if (IS_CLASS(ch, CLASS_WEREWOLF))
  {
    if (IS_GAR1(ch, WOLF_COCOON)) send_to_char("* You are engulfed in a cocoon.\n\r",ch);
  }

  if (IS_CLASS(ch, CLASS_DEMON))
  {
    if (IS_POLYAFF(ch, POLY_ZULOFORM)) send_to_char("* You are in demonic form.\n\r", ch);
    if (IS_VAMPAFF(ch, VAM_CLAWS)) send_to_char("* You have wicked claws extending from your fingers.\n\r", ch);
    if (IS_VAMPAFF(ch, VAM_FANGS)) send_to_char("* You have sharp fangs extending from your gums.\n\r", ch);
    if (IS_DEMAFF(ch,DEM_TAIL)) send_to_char("* You have a long tail extending from your back.\n\r", ch);
    if (IS_DEMAFF(ch,DEM_HOOVES)) send_to_char("* You have hooves instead of feet.\n\r", ch);
  }
  return;
}

/* The costy healer */
void do_healme(CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if (ch->in_room != NULL && ch->in_room->vnum != ROOM_VNUM_ALTAR)
  {
    send_to_char("The healer is located at the altar in midgaard.\n\r",ch);
    return;
  }
  if (ch->fight_timer > 0)
  {
    send_to_char("Not with a fighttimer.\n\r",ch);
    return;
  }
  if (ch->pcdata->quest < 300)
  {
    send_to_char("The healer demands 300 qps as payment for his service.\n\r",ch);
    return;
  }
  ch->hit  = ch->max_hit;
  ch->mana = ch->max_mana;
  ch->move = ch->max_move;
  ch->loc_hp[0] = 0;
  ch->loc_hp[1] = 0;
  ch->loc_hp[2] = 0;
  ch->loc_hp[3] = 0;
  ch->loc_hp[4] = 0;
  ch->loc_hp[5] = 0;
  ch->loc_hp[6] = 0;
  send_to_char("The healer heals your wounds and takes 300 qps as payment.\n\r",ch);
  ch->pcdata->quest -= 300;
  WAIT_STATE(ch, 36);
  return;
}

/* MUDSTAT command */
void do_mudstat( CHAR_DATA *ch, char *argument)
{
  DESCRIPTOR_DATA *d;
  char buf[MAX_STRING_LENGTH];
  CHAR_DATA *gch;  
  int mage_count=0, lich_count=0, drow_count=0, drider_count=0, ww_count=0, shape_count=0, monk_count=0, angel_count=0;
  int vampire_count=0, knight_count=0, ninja_count=0, sam_count=0, demon_count=0, tanarri_count=0, newbie_count=0, total_count=0;

  extern char str_boot_time[];

  if (IS_NPC(ch)) return;

  for ( d = descriptor_list; d != NULL; d = d->next )
  {
    if ( d->character != NULL ) gch = d->character;
    else continue;
    if (!d->connected == CON_PLAYING) continue;
    if (gch->level > 6) continue;
    switch (gch->class)
    {
      case 0     : newbie_count++;total_count++;break;
      case 1     : demon_count++;total_count++;break;
      case 2     : mage_count++;total_count++;break;
      case 4     : ww_count++;total_count++;break;
      case 8     : vampire_count++;total_count++;break;
      case 16    : sam_count++;total_count++;break;
      case 32    : drow_count++;total_count++;break;
      case 64    : monk_count++;total_count++;break;
      case 128   : ninja_count++;total_count++;break;
      case 256   : lich_count++;total_count++;break;
      case 512   : shape_count++;total_count++;break;
      case 1024  : tanarri_count++;total_count++;break;
      case 2048  : angel_count++;total_count++;break;
      case 4096  : knight_count++;total_count++;break;
      case 8192  : drider_count++;total_count++;break;
    }
  }

  send_to_char("#R--==#L**#R==--==#L**#R==--==#L**#R==--==#L**#R== [#y^^#R]  MUD STATS  [#y^^#R] ==#L**#R==--==#L**#R==--==#L**#R==--==#L**#R==--#n\n\r\n\r",ch);
  send_to_char("#LOnline Players by Class :#n\n\r\n\r",ch);
  sprintf(buf, "#GMages#n     : %-2d            #GLichs#n      : %-2d       #GDrows#n       : %-2d     #GDriders#n :       %-2d\n\r",
    mage_count, lich_count, drow_count, drider_count);
  send_to_char(buf,ch);
  sprintf(buf, "#GDemons#n    : %-2d            #GTanar'ris#n  : %-2d       #GWerewolfs#n   : %-2d     #GShapies#n :       %-2d\n\r",
    demon_count, tanarri_count, ww_count, shape_count);
  send_to_char(buf,ch);
  sprintf(buf, "#GNinjas#n    : %-2d            #GSamurais#n   : %-2d       #GVampires#n    : %-2d     #GKnights#n :       %-2d\n\r",
    ninja_count, sam_count, vampire_count, knight_count);
  send_to_char(buf,ch);
  sprintf(buf, "#GMonks#n     : %-2d            #GAngels#n     : %-2d       #GNewbies#n     : %-2d     #GTotal#n   :       %-2d\n\r\n\r",
    monk_count, angel_count, newbie_count, total_count);
  send_to_char(buf,ch);
  send_to_char("#R--==#L**#R==--==#L**#R==--==#L**#R==--==#L**#R==      OTHER STATS     ==#L**#R==--==#L**#R==--==#L**#R==--==#L**#R==--#n\n\r\n\r",ch);
  if (global_exp)
    send_to_char("#LHappy Hour is #yON!#n",ch);
  else
    send_to_char("#LHappy Hour is off",ch);
  if (global_qp)
    send_to_char("#L and Questors Delight is #yON!#n\n\r\n\r#n",ch);
  else
    send_to_char("#L and Questors Delight is off\n\r\n\r#n",ch);
  if (ragnarok)
    send_to_char("#CRAGNAROK MODE!!!!!!!#n  Everyone can kill everyone (HELP RAGNAROK)\n\r\n\r",ch);
  else
  {
    sprintf(buf,"#CRagnarok is still %d qps from happening.\n\r\n\r",ragnarok_cost);
    send_to_char(buf,ch);
  }
  sprintf(buf, "#RNumber of players connected since last copyover/restart :#C %d\n\r#n", players_logged);
  send_to_char(buf,ch);
  sprintf(buf, "#RNumber of players beheaded since last copyover/restart  :#C %d\n\r#n", players_decap);
  send_to_char(buf,ch);
  sprintf(buf, "#RAmount of generation stolen since last copyover/restart :#C %d\n\r\n\r#n", players_gstolen);
  send_to_char(buf,ch);
  sprintf( buf, "%s was last (re)started at : %s\rThe system time is currently     : %s\n\r",
    MUDNAME, str_boot_time, (char *) ctime( &current_time));
  send_to_char( buf, ch );
  send_to_char("#R--==#L**#R==--==#L**#R==--==#L**#R==--==#L**#R==--==#L**#R==--==#L**#R==--==#L**#R==--==#L**#R==--==#L**#R==--==#L**#R==--==--#n\n\r\n\r",ch);
  return;
}

void do_pkvision(CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
 
  if (get_ratio(ch) < 4500)
  {
    send_to_char("huh?\n\r",ch);
    return;
  }
  if (has_timer(ch)) return;
  if (!IS_SET(ch->itemaffect, ITEMA_VISION))
  {
    SET_BIT(ch->itemaffect, ITEMA_VISION);
    SET_BIT(ch->newbits, NEW_VISION);  // so we know to remove it again.
    send_to_char("Your eyes flicker, and everything is clear.\n\r",ch);
  }
  else
    send_to_char("You already have superior sight.\n\r",ch);
  return;
}

void do_pkscry(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_STRING_LENGTH];
  ROOM_INDEX_DATA *chroom;
  ROOM_INDEX_DATA *victimroom;

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;

  if (get_ratio(ch) < 1000)
  {
    send_to_char("huh?\n\r",ch);
    return;
  }
  if ( arg[0] == '\0' )
  {
    send_to_char( "Scry on whom?\n\r", ch );
    return;
  }
  if ( ( victim = get_char_world( ch, arg ) ) == NULL )
  {
    send_to_char( "They aren't here.\n\r", ch );
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobs.\n\r",ch);
    return;
  }
  if (victim->trust > 6)
  {
    send_to_char("SPYING ON THE GODS!?!?!?!\n\r",ch);
    send_to_char("Someone is scrying you.\n\r",victim);
    return;
  }
  chroom = ch->in_room;
  victimroom = victim->in_room;
  char_from_room(ch);
  char_to_room(ch,victimroom);
  if (IS_AFFECTED(ch, AFF_SHADOWPLANE) && (!IS_AFFECTED(victim, AFF_SHADOWPLANE)))
  { 
    REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    do_look(ch,"scry");
    SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
  }
  else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) && (IS_AFFECTED(victim, AFF_SHADOWPLANE)))
  {
    SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
    do_look(ch,"scry");
    REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
  } 
  else
    do_look(ch,"scry");
  char_from_room(ch);
  char_to_room(ch,chroom);
  if (ch->level < 7) ch->fight_timer += 3;
  if (get_ratio(victim) > 1499)
    send_to_char("You feel a slight tingle.\n\r",victim);
  return;
}

void do_pkobjscry(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *gch;
  CHAR_DATA *victim; 
  OBJ_DATA *obj;
  ROOM_INDEX_DATA *location;
  ROOM_INDEX_DATA *from;
  DESCRIPTOR_DATA *tmp;
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_INPUT_LENGTH];
  bool afk = FALSE;

  one_argument(argument, arg);
   
  if (IS_NPC(ch)) return;  
  if (get_ratio(ch) < 3500)
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ((obj = get_obj_world(ch, arg)) == NULL)
  {
    send_to_char("Nothing like that in hell, earth, or heaven.\n\r", ch);
    return;
  }
  if ((gch = obj->carried_by) != NULL)
  {
    if (gch->in_room == NULL)
    {
      send_to_char("All you see is a chaotic mass of colors, nothing seems right.\n\r",ch);
      return;
    }
    tmp = gch->desc;
    gch->desc = ch->desc;
    sprintf(buf,"A pair of eyes grows on %s's %s.\n\rThe eyes blink once, then disappear.\n\r", gch->name, obj->short_descr);
    for (victim = gch->in_room->people; victim != NULL; victim = victim->next_in_room)
    {
      if (victim == gch) continue;  // the victim cannot see this.
      send_to_char(buf,victim);
    }
    if (IS_SET(gch->extra, EXTRA_AFK))
    {
      afk = TRUE;
      REMOVE_BIT(gch->extra, EXTRA_AFK);
    }
    do_look(gch,"");
    gch->desc = tmp;
    if (afk) SET_BIT(gch->extra, EXTRA_AFK);
    return;
  }
  else if ((location = obj->in_room) != NULL)
  {
    sprintf(buf,"A pair of eyes grows on %s.\n\rThe eyes blink once, then disappear.\n\r", obj->short_descr);
    for (victim = location->people; victim != NULL; victim = victim->next_in_room)
    {
      if (victim == ch) continue; // the player is seeing through the item, and will not see the eyes even if he is in the room.
      send_to_char(buf,victim);
    }
    obj_from_room(obj);
    from = ch->in_room;  
    char_from_room(ch);
    char_to_room(ch, location);
    do_look(ch, "");
    char_from_room(ch);
    char_to_room(ch, from);
    obj_to_room(obj, location);
    return;
  }
  else  // inside something.
  {
    send_to_char("It's pitch black, wonder where this item is.\n\r",ch);
    return;
  }
}

void do_pkportal (CHAR_DATA *ch, char *argument)
{  
  CHAR_DATA *victim;
  char arg[MAX_STRING_LENGTH];
  ROOM_INDEX_DATA *location;

  one_argument (argument, arg);
    
  if (IS_NPC(ch)) return;
  if (get_ratio(ch) < 5000)
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ((victim = get_char_world(ch, arg)) == NULL)
  {
    send_to_char("Walk the silverpath to whom?\n\r", ch );
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobs.\n\r",ch);
    return;
  }
  if (victim->fight_timer == 0)
  {
    send_to_char("They are not engaged in pk.\n\r",ch);
    return;
  }
  if (IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "You can't find it's room.\n\r",ch);
    return;
  }
  if (IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "Your room is not connected to the astral plane.\n\r",ch);
    return;
  }
  location = victim->in_room;
  act("You step onto the silverpath.", ch, NULL, NULL, TO_CHAR);
  act("$n steps into thin air.", ch, NULL, NULL, TO_ROOM);
  char_from_room(ch);
  char_to_room(ch, location);
  do_look(ch, "auto");
  act("You leave the silverpath.", ch, NULL, NULL, TO_CHAR);
  act("$n steps out of thin air.", ch, NULL, NULL, TO_ROOM);
  WAIT_STATE(ch, 6);
  return;  
}

void do_pkaura (CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char      arg [MAX_INPUT_LENGTH];
  char      buf [MAX_INPUT_LENGTH];
   
  argument = one_argument( argument, arg );
   
  if (IS_NPC(ch)) return;
  if (get_ratio(ch) < 2000)
  {
    send_to_char("huh?\n\r",ch);
    return;
  }
  if ( arg[0] == '\0' )
  {
    send_to_char("Read the aura of who?\n\r",ch);
    return;
  }
  if ( ( victim = get_char_world( ch, arg ) ) == NULL )
  {
    send_to_char( "They aren't here.\n\r", ch );
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobs.\n\r",ch);
    return;
  }
  if (victim->trust > 6)
  {
    send_to_char("SPYING ON THE GODS!?!?!?!\n\r",ch);
    send_to_char("Someone is trying to read your aura.\n\r",victim);
    return;
  }
  sprintf(buf,"Hp:%d/%d, Mana:%d/%d,Move:%d/%d.\n\r",
    victim->hit,victim->max_hit,victim->mana,victim->max_mana,victim->move,victim->max_move);
  send_to_char(buf,ch);
  if (ch->trust < 7) send_to_char("You are being examined.\n\r",victim); 
  sprintf(buf,"Hitroll:%d, Damroll:%d, AC:%d.\n\r",char_hitroll(victim),char_damroll(victim),char_ac(victim));
  send_to_char(buf,ch);
  sprintf(buf,"Generation:%d\n\r",victim->generation);
  send_to_char(buf,ch);
  if (ch->level < 7) ch->fight_timer += 3;
  return;
}

void do_pkheal(CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if (get_ratio(ch) < 500)
  {
    send_to_char("huh?\n\r",ch);
    return;
  }
  if (ch->fight_timer > 0)
  {
    send_to_char("You cannot focus your mind.\n\r",ch);
    return;
  }
  ch->hit += UMIN(get_ratio(ch), 2500);
  if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
  send_to_char("You release your control and let harmony and peace flow over you.\n\r",ch);
  WAIT_STATE(ch,12);
  return;
}

void do_pkpowers(CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if (get_ratio(ch) < 500)
    send_to_char("You suck, get lost.\n\r",ch);
  if (get_ratio(ch) > 499)
    send_to_char("* Sanctum      - Concentrate on your inner peace.\n\r",ch);
  if (get_ratio(ch) > 999)
    send_to_char("* Eaglesight   - Scry on players even through shield.\n\r",ch);
  if (get_ratio(ch) > 1999)
    send_to_char("* Lifesense    - Global readaura.\n\r",ch);
  if (get_ratio(ch) > 2999)
    send_to_char("* Calltoarms   - Call for help during battle.\n\r",ch);
  if (get_ratio(ch) > 3499)
    send_to_char("* Objectscry   - See through the 'eyes' of an item.\n\r",ch);
  if (get_ratio(ch) > 3999)
    send_to_char("* Ironmind     - Prepare yourself mentally for battle.\n\r",ch);
  if (get_ratio(ch) > 4499)
    send_to_char("* Crystalsight - Gain superior sight for a short while.\n\r",ch);
  if (get_ratio(ch) > 4999)
    send_to_char("* Silverpath   - Low lag portal to players with a fighttimer.\n\r",ch);
  if (get_ratio(ch) > 2499)
    send_to_char("* You have enhanced fighting abilities.\n\r",ch);
  if (get_ratio(ch) > 1499)
    send_to_char("* You have enhanced awereness.\n\r",ch);
  return;
}

/* Mastery command to gain mastery items */

void do_mastery(CHAR_DATA *ch, char *argument)
{
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  int vnum;
  char buf[MAX_INPUT_LENGTH];
  
  if (IS_NPC(ch)) return;
  
  if (IS_SET(ch->newbits, NEW_MASTERY) && ch->level < 6)
  {
    send_to_char("You've already gotten your mastery. If you lost it, tough luck!\n\r",ch);
    return;
  }
  if (ch->wpn[0] < 200 || ch->wpn[1] < 200 || ch->wpn[2] < 200 || ch->wpn[3] < 200 || ch->wpn[4] < 200
   || ch->wpn[5] < 200 || ch->wpn[6] < 200 || ch->wpn[7] < 200 || ch->wpn[8] < 200
   || ch->wpn[9] < 200 || ch->wpn[10] < 200 || ch->wpn[11] < 200 || ch->wpn[12] < 200 )
  {
    send_to_char("Maybe you should grandmaster your weapons first.\n\r",ch);
    return;
  }
  if (ch->spl[0] < 200 || ch->spl[1] < 200 || ch->spl[2] < 200 || ch->spl[3] < 200 || ch->spl[4] < 200 )
  {
    send_to_char("Maybe you should be grand sorcerer in all spell colors first.\n\r",ch);
    return;
  }
  if (ch->stance[1] < 200 || ch->stance[2] < 200 || ch->stance[3] < 200 || ch->stance[4] < 200
   || ch->stance[5] < 200 || ch->stance[6] < 200 || ch->stance[7] < 200 || ch->stance[8] < 200
   || ch->stance[9] < 200 || ch->stance[10] < 200 )
  {
    send_to_char("Maybe you should grandmaster your stances first.\n\r",ch);
    return;
  }
  if (IS_CLASS(ch, CLASS_MAGE)) vnum = 33014;
  else if (IS_CLASS(ch, CLASS_WEREWOLF)) vnum = 33112;
  else if (IS_CLASS(ch, CLASS_NINJA)) vnum = 33094;
  else if (IS_CLASS(ch, CLASS_MONK)) vnum = 33032;
  else if (IS_CLASS(ch, CLASS_DROW)) vnum = 33074;
  else if (IS_CLASS(ch, CLASS_DEMON)) vnum = 33134;
  else if (IS_CLASS(ch, CLASS_VAMPIRE)) vnum = 33054;
  else if (IS_CLASS(ch, CLASS_DROID)) vnum = 33153;
  else if (IS_CLASS(ch, CLASS_SAMURAI)) vnum = 33177;
  else if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT)) vnum = 29989;
  else if (IS_CLASS(ch, CLASS_SHAPESHIFTER)) vnum = 33174;
  else if (IS_CLASS(ch, CLASS_ANGEL)) vnum = 33193;
  else if (IS_CLASS(ch, CLASS_TANARRI)) vnum = 33213;
  else if (IS_CLASS(ch, CLASS_LICH)) vnum = 33233;
  else
  {
    send_to_char("Your class mastery is not done yet, please notify Jobo that you want him to make it.\n\r",ch);
    return;
  }
  if ((pObjIndex = get_obj_index(vnum)) == NULL)
  {
    send_to_char("Missing object, inform Jobo.\n\r",ch);
    return;
  }
  obj = create_object(pObjIndex, 50);
  obj_to_char(obj,ch);
  obj->questowner = str_dup(ch->pcdata->switchname);
  sprintf(buf,"%s has achieved mastery.",ch->name);
  do_info(ch,buf);
  SET_BIT(ch->newbits, NEW_MASTERY);
  return;
}

/* superstances are in :-) */

void do_setstance(CHAR_DATA *ch, char *argument)
{
  char arg1 [MAX_INPUT_LENGTH];
  char arg2 [MAX_INPUT_LENGTH];
  char arg3 [MAX_INPUT_LENGTH];
  int currentstance;
  char buf[MAX_STRING_LENGTH];
  int cost = 0;
  int count = 0;
  int max_supreme = 0;
  int max_greater = 0;
  int max_lesser = 0; 
  int min_cost = 0;   
  int max_cost = 0;   
  int current_supreme = 0;
  int current_greater = 0;
  int current_lesser = 0; 
  
  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );
  strcpy(arg3, argument);
  
  if (ch->stance[STANCE_TIGER] < 200 || ch->stance[STANCE_SWALLOW] < 200 || ch->stance[STANCE_MONKEY] < 200
  || ch->stance[STANCE_MANTIS] < 200 || ch->stance[STANCE_DRAGON] < 200)
  {
    send_to_char("You need to max your normal stances first.\n\r",ch);
    return;
  }
  if (ch->stance[19] == -1) {max_lesser = 3; currentstance = 19; min_cost=40; max_cost=240;}
  else if (ch->stance[20] == -1) {max_lesser = 4; max_greater = 1; currentstance = 20; min_cost = 80; max_cost = 280;}
  else if (ch->stance[21] == -1) {max_lesser = 4; max_greater = 2; currentstance = 21; min_cost = 120; max_cost = 300;}
  else if (ch->stance[22] == -1) {currentstance = 22; max_lesser = 4; max_greater = 4; max_supreme = 1; min_cost = 160; max_cost = 360;}
  else if (ch->stance[23] == -1) {currentstance = 23; max_lesser = 4; max_greater = 4; max_supreme = 2; min_cost = 200; max_cost = 380;}
  else if (!str_cmp(arg1,"clear") || !str_cmp(arg1,"show")) {currentstance = 23; max_lesser = 4; max_greater = 4; max_supreme = 2; min_cost = 200; max_cost = 380;}
  else
  {
    send_to_char("You already have all your superstances, if you want to clear ALL your superstances, you can\n\r",ch);
    send_to_char("type 'setstance clear all stances'. WARNING, this will remove ALL your superstances!\n\r",ch);
    return;
  }
  if (currentstance > 19 && ch->stance[currentstance-7] < 200)
  {
    send_to_char("Maybe you should max your current superstance first.\n\r",ch);
    return;
  }

  if (IS_SET(ch->stance[18], STANCEPOWER_DODGE)) {count++;cost += count*20;}
  if (IS_SET(ch->stance[18], STANCEPOWER_PARRY)) {count++;cost += count*20;}
  if (IS_SET(ch->stance[18], STANCEPOWER_SPEED)) {count++;cost += count*20;}
  if (IS_SET(ch->stance[18], STANCEPOWER_BYPASS)) {count++;cost += count*20;}
  if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_1)) {current_lesser++;cost += 20;}
  if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_2)) {current_greater++;cost += 40;}
  if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_3)) {current_supreme++;cost += 60;}
  if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_1)) {current_lesser++;cost += 20;}
  if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_2)) {current_greater++;cost += 40;}
  if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_3)) {current_supreme++;cost += 60;}
  if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_1)) {current_lesser++;cost += 20;}
  if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_2)) {current_greater++;cost += 40;}
  if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_3)) {current_supreme++;cost += 60;}
  if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_1)) {current_lesser++;cost += 20;}
  if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_2)) {current_greater++;cost += 40;}
  if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_3)) {current_supreme++;cost += 60;}
   
  if (arg1[0] == '\0')
  {
    send_to_char(" #r*****************************************************************************************#n\n\r",ch);
    send_to_char(" #r*#C                          Superstance Editor v0.3 by Jobo                              #r*#n\n\r",ch);
    send_to_char(" #r*****************************************************************************************#n\n\r",ch);
    send_to_char(" #r*#n [DAMCAP] Increase your damcap.                                                        #r*#n\n\r",ch);
    send_to_char(" #r*#n [REV_DC] Lower your opponents damcap.                                                 #r*#n\n\r",ch);
    send_to_char(" #r*#n [DAMAGE] Increase your damage.                                                        #r*#n\n\r",ch);
    send_to_char(" #r*#n [RESIST] Resist more damage.                                                          #r*#n\n\r",ch);
    send_to_char(" #r*---------------------------------------------------------------------------------------*#n\n\r",ch);
    send_to_char(" #r*#n The above powers comes in lesser, greater and supreme versions.                       #r*#n\n\r",ch);
    send_to_char(" #r*#n The powers each cost 20/40/60 million exp, depending on level.                        #r*#n\n\r",ch);

    if (currentstance == 19)
      send_to_char(" #r*#C You may choose up to three lesser powers.                                             #r*#n\n\r",ch);
    else if (currentstance == 20)
      send_to_char(" #r*#C You may choose up to one greater power.                                               #r*#n\n\r",ch);
    else if (currentstance == 21)
      send_to_char(" #r*#C You may choose up to two greater powers.                                              #r*#n\n\r",ch);
    else if (currentstance == 22)
      send_to_char(" #r*#C You may choose up to one supreme power.                                               #r*#n\n\r",ch);
    else if (currentstance == 23)
      send_to_char(" #r*#C You may choose up to two supreme powers.                                              #r*#n\n\r",ch);
    if (currentstance > 19)
      send_to_char(" #r*#n There are no maximum on powers of lower level than that.                              #r*#n\n\r",ch);
    send_to_char(" #r*---------------------------------------------------------------------------------------*#n\n\r",ch);
    send_to_char(" #r*#n Currently you have chosen the following for your next superstance :                   #r*#n\n\r",ch);
    if (ch->stance[18] == 0)
      send_to_char(" #r*#n None.                                                                                 #r*#n\n\r",ch);
    else
    {
      if (IS_SET(ch->stance[18], STANCEPOWER_DODGE))
        send_to_char(" #r*#n Advanced dodge.                                                                       #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_PARRY))
        send_to_char(" #r*#n Advanced parry.                                                                       #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_SPEED))
        send_to_char(" #r*#n Superior Speed.                                                                       #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_BYPASS))
        send_to_char(" #r*#n Bypass Parry and Dodge.                                                               #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_1))
        send_to_char(" #r*#n Increased damage (lesser)                                                             #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_2))
        send_to_char(" #r*#n Increased damage (greater)                                                            #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_3))
        send_to_char(" #r*#n Increased damage (supreme)                                                            #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_1))
        send_to_char(" #r*#n Increased damage resistance (lesser)                                                  #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_2))
        send_to_char(" #r*#n Increased damage resistance (greater)                                                 #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_3))
        send_to_char(" #r*#n Increased damage resistance (supreme)                                                 #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_1))
        send_to_char(" #r*#n Increased damcap (lesser)                                                             #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_2))
        send_to_char(" #r*#n Increased damcap (greater)                                                            #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_3))
        send_to_char(" #r*#n Increased damcap (supreme)                                                            #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_1))
        send_to_char(" #r*#n Damcap reduction for opponent (lesser)                                                #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_2))
        send_to_char(" #r*#n Damcap reduction for opponent (greater)                                               #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_3))
        send_to_char(" #r*#n Damcap reduction for opponent (supreme)                                               #r*#n\n\r",ch);
    }
    send_to_char(" #r*---------------------------------------------------------------------------------------*#n\n\r",ch);
    send_to_char(" #r*#n You may also select any of these powers for the cost of 20 mill exp for the first,    #r*#n\n\r",ch);
    send_to_char(" #r*#n 40 for the next, 60 for the third, and finally 80 for the last.                       #r*#n\n\r",ch);
    send_to_char(" #r*#n [SPEED] Increase your fighting speed.                                                 #r*#n\n\r",ch);
    send_to_char(" #r*#n [PARRY] Increase your parrying ability.                                               #r*#n\n\r",ch);
    send_to_char(" #r*#n [DODGE] Increase your dodging ability.                                                #r*#n\n\r",ch);
    send_to_char(" #r*#n [BYPASS] Bypass the two above powers.                                                 #r*#n\n\r",ch);
    send_to_char(" #r*---------------------------------------------------------------------------------------*#n\n\r",ch);
    send_to_char(" #r*#n Type \"setstance <power> <lesser/greater/supreme>\"                                     #r*#n\n\r",ch);
    send_to_char(" #r*#n [the latter only for the four top powers] to set or remove a certain power            #r*#n\n\r",ch);
    send_to_char(" #r*#n to your stance. then type \"setstance done\" when your done.                            #r*#n\n\r",ch);  
    send_to_char(" #r*#n You can also type \"setstance clear\" to clear your current settings.                   #r*#n\n\r",ch);
    send_to_char(" #r*---------------------------------------------------------------------------------------*#n\n\r",ch);
    send_to_char(" #r*#n You can also type \"setstance show <ss1/ss2/etc>\"                                      #r*#n\n\r",ch);
    send_to_char(" #r*#n to show the settings on one of your stances.                                          #r*#n\n\r",ch);
    send_to_char(" #r*---------------------------------------------------------------------------------------*#n\n\r",ch);
    sprintf(buf, " #r*#n Cost of current stance : %-3d million exp.                                             #r*#n\n\r", cost);
    send_to_char(buf,ch);
    send_to_char(" #r*****************************************************************************************#n\n\r",ch);
    return;
  }
  else if (!str_cmp(arg1, "show"))
  {
    if (!str_cmp(arg2,"ss1")) currentstance = 19;
    else if (!str_cmp(arg2,"ss2")) currentstance = 20;
    else if (!str_cmp(arg2,"ss3")) currentstance = 21;
    else if (!str_cmp(arg2,"ss4")) currentstance = 22;
    else if (!str_cmp(arg2,"ss5")) currentstance = 23;
    else
    {
      send_to_char("No stance by that name.\n\r",ch);
      return;
    }
    if (ch->stance[currentstance] == -1)
    {
      send_to_char("No, you don't have that stance yet, SILLY!\n\r",ch);
      return;
    }
    send_to_char("This stance has the following powers :\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_DODGE)) send_to_char("Advanced Dodge.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_PARRY)) send_to_char("Advanced Parry.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_SPEED)) send_to_char("Superior Speed.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_BYPASS)) send_to_char("The Bypassing ability.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_DAMAGE_1)) send_to_char("Lesser Damage.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_DAMAGE_2)) send_to_char("Greater Damage.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_DAMAGE_3)) send_to_char("Superior Damage.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_RESIST_1)) send_to_char("Lesser Resistance.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_RESIST_2)) send_to_char("Greater Resistance.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_RESIST_3)) send_to_char("Supreme Resistance.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_DAMCAP_1)) send_to_char("Lesser Damcap.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_DAMCAP_2)) send_to_char("Greater Damcap.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_DAMCAP_3)) send_to_char("Supreme Damcap.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_REV_DAMCAP_1)) send_to_char("Lesser reverse Damcap.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_REV_DAMCAP_2)) send_to_char("Greater reverse Damcap.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_REV_DAMCAP_3)) send_to_char("Supreme reverse Damcap.\n\r",ch);
    return;
  }
  else if (!str_cmp(arg1, "done"))
  {
    if (ch->exp < cost * 1000000)
    {
      send_to_char("You don't have enough exp to buy this stance.\n\r",ch);
      return;
    }
    if (cost < min_cost)
    {
      sprintf(buf, "You need to spend at least %d million exp on this stance.\n\r", min_cost);
      send_to_char(buf, ch);
      return;
    }
    if (cost > max_cost)
    {
      sprintf(buf, "You can only spend %d million exp on this stance.\n\r", max_cost);
      send_to_char(buf, ch);
      return;
    }
    ch->stance[currentstance] = ch->stance[18];
    ch->stance[18] = 0;
    ch->exp -= cost * 1000000;
    send_to_char("ok.\n\r",ch);
    return;
  }
  else if (!str_cmp(arg1, "clear"))
  {
    if (arg2[0] == '\0')
    {
      ch->stance[18] = 0;
      send_to_char("Stance cleared.\n\r",ch);
      return;
    }
    if (!str_cmp(arg2,"all") && !str_cmp(arg3,"stances"))
    {
      if ((ch->in_room == NULL || ch->in_room->vnum != 3054))
      {
        send_to_char("To clear ALL your superstance, you must be at the Temple Altar of Midgaard.\n\r",ch);
        return;
      }
      ch->stance[0] = 0;
      ch->stance[13] = 0;
      ch->stance[14] = 0;
      ch->stance[15] = 0;
      ch->stance[16] = 0;
      ch->stance[17] = 0;
      ch->stance[18] = 0;
      ch->stance[19] = -1;
      ch->stance[20] = -1;
      ch->stance[21] = -1;
      ch->stance[22] = -1;
      ch->stance[23] = -1;
      send_to_char("Ok.\n\r",ch);
    }
  }
  else if (!str_cmp(arg1, "dodge"))
  {
    if (IS_SET(ch->stance[18], STANCEPOWER_DODGE ))  REMOVE_BIT(ch->stance[18], STANCEPOWER_DODGE);
    else SET_BIT(ch->stance[18], STANCEPOWER_DODGE);
    send_to_char("Ok.\n\r",ch);
    return;
  }  
  else if (!str_cmp(arg1, "parry"))
  {
    if (IS_SET(ch->stance[18], STANCEPOWER_PARRY ))  REMOVE_BIT(ch->stance[18], STANCEPOWER_PARRY);
    else SET_BIT(ch->stance[18], STANCEPOWER_PARRY);
    send_to_char("Ok.\n\r",ch);
    return;
  }
  else if (!str_cmp(arg1, "speed"))
  {
    if (IS_SET(ch->stance[18], STANCEPOWER_SPEED ))  REMOVE_BIT(ch->stance[18], STANCEPOWER_SPEED);
    else SET_BIT(ch->stance[18], STANCEPOWER_SPEED);
    send_to_char("Ok.\n\r",ch);
    return;
  }
  else if (!str_cmp(arg1, "bypass"))
  {
    if (IS_SET(ch->stance[18], STANCEPOWER_BYPASS ))  REMOVE_BIT(ch->stance[18], STANCEPOWER_BYPASS);
    else SET_BIT(ch->stance[18], STANCEPOWER_BYPASS);
    send_to_char("Ok.\n\r",ch);
    return;
  }
  else if (arg2[0] == '\0')
  {  
    send_to_char("What?!?\n\r",ch);
    return;
  }
  else if (!str_cmp(arg1, "damage"))
  {
    if (!str_cmp(arg2, "lesser"))
    {
      if (max_lesser > current_lesser || IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_1))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_2) || IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_3))
        {
          send_to_char("You allready have the damage power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_1)) {current_lesser--; REMOVE_BIT(ch->stance[18], STANCEPOWER_DAMAGE_1);}
        else {current_lesser++; SET_BIT(ch->stance[18], STANCEPOWER_DAMAGE_1);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more lesser powers.\n\r",ch);
        return;
      }
    }  
    else if (!str_cmp(arg2, "greater"))
    {
      if (max_greater > current_greater || IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_2))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_1) || IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_3))
        {
          send_to_char("You allready have the damage power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_2)) {current_greater--; REMOVE_BIT(ch->stance[18], STANCEPOWER_DAMAGE_2);}
        else {current_greater++; SET_BIT(ch->stance[18], STANCEPOWER_DAMAGE_2);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more greater powers.\n\r",ch);
        return;
      }
    }
    else if (!str_cmp(arg2, "supreme"))
    {  
      if (max_supreme > current_supreme || IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_3))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_1) || IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_2))
        {
          send_to_char("You allready have the damage power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_3)) {current_supreme--; REMOVE_BIT(ch->stance[18], STANCEPOWER_DAMAGE_3);}
        else {current_supreme++; SET_BIT(ch->stance[18], STANCEPOWER_DAMAGE_3);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more supreme powers.\n\r",ch);
        return;
      }
    }
    else
    {  
      send_to_char("What?!?\n\r",ch);
      return;
    }  
  }
  else if (!str_cmp(arg1, "damcap"))
  {
    if (!str_cmp(arg2, "lesser"))
    {
      if (max_lesser > current_lesser || IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_1))
      {  
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_2) || IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_3))
        {
          send_to_char("You allready have the damcap power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_1)) {current_lesser--; REMOVE_BIT(ch->stance[18], STANCEPOWER_DAMCAP_1);}
        else {current_lesser++; SET_BIT(ch->stance[18], STANCEPOWER_DAMCAP_1);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      { 
        send_to_char("You are not allowed to have any more lesser powers.\n\r",ch);
        return;
      }
    }  
    else if (!str_cmp(arg2, "greater"))
    {
      if (max_greater > current_greater || IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_2))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_1) || IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_3))
        {
          send_to_char("You allready have the damcap power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_2)) {current_greater--; REMOVE_BIT(ch->stance[18], STANCEPOWER_DAMCAP_2);}
        else {current_greater++; SET_BIT(ch->stance[18], STANCEPOWER_DAMCAP_2);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more greater powers.\n\r",ch);
        return;
      } 
    }
    else if (!str_cmp(arg2, "supreme"))
    {  
      if (max_supreme > current_supreme || IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_3))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_1) || IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_2))
        {
          send_to_char("You allready have the damcap power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_3)) {current_supreme--; REMOVE_BIT(ch->stance[18], STANCEPOWER_DAMCAP_3);}
        else {current_supreme++; SET_BIT(ch->stance[18], STANCEPOWER_DAMCAP_3);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more supreme powers.\n\r",ch);
        return;
      }
    }
    else
    {   
      send_to_char("What?!?\n\r",ch);
      return;
    }  
  }
  else if (!str_cmp(arg1, "resist"))
  {
    if (!str_cmp(arg2, "lesser"))
    {
      if (max_lesser > current_lesser || IS_SET(ch->stance[18], STANCEPOWER_RESIST_1))
      {  
        if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_2) || IS_SET(ch->stance[18], STANCEPOWER_RESIST_3))
        {
          send_to_char("You allready have the resist power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_1)) {current_lesser--; REMOVE_BIT(ch->stance[18], STANCEPOWER_RESIST_1);}
        else {current_lesser++; SET_BIT(ch->stance[18], STANCEPOWER_RESIST_1);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      { 
        send_to_char("You are not allowed to have any more lesser powers.\n\r",ch);
        return;
      }
    }  
    else if (!str_cmp(arg2, "greater"))
    {
      if (max_greater > current_greater || IS_SET(ch->stance[18], STANCEPOWER_RESIST_2))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_1) || IS_SET(ch->stance[18], STANCEPOWER_RESIST_3))
        {
          send_to_char("You allready have the resist power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_2)) {current_greater--; REMOVE_BIT(ch->stance[18], STANCEPOWER_RESIST_2);}
        else {current_greater++; SET_BIT(ch->stance[18], STANCEPOWER_RESIST_2);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more greater powers.\n\r",ch);
        return;
      } 
    }
    else if (!str_cmp(arg2, "supreme"))
    {  
      if (max_supreme > current_supreme || IS_SET(ch->stance[18], STANCEPOWER_RESIST_3))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_1) || IS_SET(ch->stance[18], STANCEPOWER_RESIST_2))
        {
          send_to_char("You allready have the resist power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_3)) {current_supreme--; REMOVE_BIT(ch->stance[18], STANCEPOWER_RESIST_3);}
        else {current_supreme++; SET_BIT(ch->stance[18], STANCEPOWER_RESIST_3);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more supreme powers.\n\r",ch);
        return;
      }
    }
    else
    {   
      send_to_char("What?!?\n\r",ch);
      return;
    }  
  }
  else if (!str_cmp(arg1, "rev_dc"))
  {
    if (!str_cmp(arg2, "lesser"))
    {
      if (max_lesser > current_lesser || IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_1))
      {  
        if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_2) || IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_3))
        {
          send_to_char("You allready have the reverse damcap power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_1)) {current_lesser--; REMOVE_BIT(ch->stance[18], STANCEPOWER_REV_DAMCAP_1);}
        else {current_lesser++; SET_BIT(ch->stance[18], STANCEPOWER_REV_DAMCAP_1);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      { 
        send_to_char("You are not allowed to have any more lesser powers.\n\r",ch);
        return;
      }
    }  
    else if (!str_cmp(arg2, "greater"))
    {
      if (max_greater > current_greater || IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_2))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_1) || IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_3))
        {
          send_to_char("You allready have the reverse damcap power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_2)) {current_greater--; REMOVE_BIT(ch->stance[18], STANCEPOWER_REV_DAMCAP_2);}
        else {current_greater++; SET_BIT(ch->stance[18], STANCEPOWER_REV_DAMCAP_2);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more greater powers.\n\r",ch);
        return;
      } 
    }
    else if (!str_cmp(arg2, "supreme"))
    {  
      if (max_supreme > current_supreme || IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_3))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_1) || IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_2))
        {
          send_to_char("You allready have the reverse damcap power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_3)) {current_supreme--; REMOVE_BIT(ch->stance[18], STANCEPOWER_REV_DAMCAP_3);}
        else {current_supreme++; SET_BIT(ch->stance[18], STANCEPOWER_REV_DAMCAP_3);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more supreme powers.\n\r",ch);
        return;
      }
    }
    else
    {   
      send_to_char("What?!?\n\r",ch);
      return;
    }  
  }
  else do_setstance(ch,"");
}

void do_ragnarok( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  int amount;

  argument = one_argument(argument, arg);
  if (IS_NPC(ch)) return;
  if (ragnarok_safe_timer > 0)
  {
    send_to_char("You may not call for ragnarok yet.\n\r",ch);
    return;
  }
  if (has_timer(ch)) return;
  if (arg[0] == '\0')
  {
    send_to_char("How many qps do you want to bid ?\n\r",ch);
    return;
  }
  if (!is_number(arg))
  {
    send_to_char("A number please, how many qps do you want to bid towards ragnarok.\n\r",ch);
    return;
  }
  amount = atoi(arg);
  if (amount < 100)
  {
    send_to_char("You cannot tempt the gods with this puny amount.\n\r",ch);
    return;
  }
  if (amount > 1000)
  {
    send_to_char("You don't want to spend this many qps to end the world.\n\r",ch);
    return;
  }
  if (ch->pcdata->quest < amount)
  {
    send_to_char("Hah!\n\r",ch);
    return;
  }
  ch->pcdata->quest -= amount;
  ragnarok_cost -= amount;
  if (ragnarok_cost <= 0)
  {
    ragnarok_cost = 3000;
    ragnarok = TRUE;
    ragnarok_on_timer = PULSE_RAGNAROK;
    ragnarok_safe_timer = 0;
    do_info(ch,"#0The world comes to an end, #yRAGNAROK#0 is over us all!!!!#n");
  }
  else do_info(ch,"The ragnarok moves closer, the gods shiver with fear");
  return;
}

void do_timer( CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;

  if (pulse_arena > 1)
  {
    sprintf(buf,"Next arena will happen in     :  %d hour and %d minutes.\n\r",
      pulse_arena/60, pulse_arena - (pulse_arena/60) * 60);
    send_to_char(buf,ch);
  }

  if (!ragnarok)
  {
    sprintf(buf,"Bidding on ragnarok starts in :  %d hour and %d minutes.\n\r",
      ragnarok_safe_timer/60, ragnarok_safe_timer - (ragnarok_safe_timer/60) * 60);
    send_to_char(buf,ch);
  }

  if (global_exp)
  {
    sprintf(buf,"Happy Hour will end in        :  %d minutes.\n\r", pulse_doubleexp);
    send_to_char(buf,ch);
  }

  if (global_qp)
  {
    sprintf(buf,"Questors Delight will end in  :  %d minutes.\n\r", pulse_doubleqp );
    send_to_char(buf,ch);
  }

  return;
}

void do_exp( CHAR_DATA *ch, char *argument )
{  
  char buf[MAX_STRING_LENGTH];
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  unsigned int cost, to, from;

  argument = one_argument(argument,arg1);
  argument = one_argument(argument,arg2);

  if (IS_NPC(ch)) return;
  if (arg1[0] == '\0' || arg2[0] == '\0')
  {
    send_to_char("Usage is :\n\r",ch);
    send_to_char("exp <from> <to>\n\r",ch);
    send_to_char("ie. exp 4000 6000 will tell you how many exp it will cost to train from 4K to 6K\n\r",ch);
    return;
  }
  if (!is_number(arg1) || !is_number(arg2))
  {
    send_to_char("Please use numbers.\n\r",ch);
    return;
  }
  from = atoi(arg1);
  to = atoi(arg2);
  if (from >= to)
  {
    send_to_char("Begin with a smaller number.\n\r",ch);
    return;
  }
  if (to > 120000)
  {
    send_to_char("Thats beyond the statcap.\n\r",ch);
    return;
  }
  if ((to > 50000 && to - from > 20000) || (to > 100000 && to - from > 10000) || (to - from > 50000))
  {
    send_to_char("Please use a smaller interval to avoid integer overflows.\n\r",ch);
    return;
  }
  cost = (from + to) * (to - from + 1) / 2 - from;
  if (cost > 500000000)
    sprintf(buf,"The total cost in exp will be %d million\n\r", cost/1000000);
  else sprintf(buf,"The total cost in exp will be %d\n\r", cost);
  send_to_char(buf,ch);
  return;
}

void do_favor( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];

  one_argument(argument,arg);

  if (IS_NPC(ch)) return;
  if (IS_SET(ch->act, PLR_RIGHTHAND) || IS_SET(ch->act, PLR_LEFTHAND) || IS_SET(ch->act, PLR_AMBI))
  {
    send_to_char("Already set.\n\r",ch);
    return;
  }
  if (arg[0] == '\0')
  {
    send_to_char("Favor left, right or both hands ?\n\r",ch);
    return;
  }
  if (!str_cmp(arg,"left"))
  {
    SET_BIT(ch->act, PLR_LEFTHAND);
    send_to_char("You favor your left arm in combat.\n\r",ch);
  }
  else if (!str_cmp(arg,"right"))
  {
    SET_BIT(ch->act, PLR_RIGHTHAND);
    send_to_char("You favor your right arm in combat.\n\r",ch);
  }
  else if (!str_cmp(arg,"both"))
  {
    SET_BIT(ch->act, PLR_AMBI);
    send_to_char("You fight well with both arms.\n\r",ch);
  }
  else do_favor(ch,"");
  return;
}

void do_alias(CHAR_DATA *ch, char *argument)
{
  ALIAS_DATA *ali;
  char arg1[MAX_INPUT_LENGTH];
  char testarg[MAX_INPUT_LENGTH];

  if (IS_NPC(ch)) return;

  argument = one_argument(argument, arg1);
  one_argument(argument, testarg);

  if ((ch->in_room == NULL || ch->in_room->vnum != 3054))
  {
    send_to_char("You must be at the Temple Altar of Midgaard to create a new alias.\n\r",ch);
    return;
  }
  if (ch->pcdata->alias_count >= MAX_ALIAS)
  {
    send_to_char("Sorry, you have reached the limit for aliases, you need to remove some before you can make more.\n\r",ch);
    return;
  }
  if (arg1[0] == '\0' || argument[0] == '\0')
  {
    send_to_char("Syntax : alias 'short' 'long'\n\r",ch);
    send_to_char("\n\rThe 'short' is the alias, and the 'long' is the string that the alias replaces.\n\r",ch);
    send_to_char("ie. alias tt teleport orc.\n\r",ch);
    return;
  }
  if (strlen2(argument) > 400)
  {
    send_to_char("Behave... that's way to big.\n\r",ch);
    return;
  }
  smash_tilde(arg1);
  smash_tilde(testarg);
  smash_tilde(argument);
  if (!str_cmp(arg1, testarg))
  {
    send_to_char("You cannot alias the alias.\n\r",ch);
    return;
  }
  if (!str_cmp(arg1, "showalias") || !str_cmp(arg1, "alias") || !str_cmp(arg1, "removealias"))
  {
    send_to_char("You really shouldn't alias the alias commands.\n\r", ch);
    return;
  }
  for (ali = ch->pcdata->alias; ali; ali = ali->next)
  {
    if (!str_cmp(arg1, ali->short_n))
    {
      send_to_char("You already have such an alias.\n\r",ch);
      return;
    }
    if (!str_cmp(arg1, ali->long_n) || !str_cmp(argument, ali->short_n) || !str_cmp(argument, ali->long_n))
    {
      send_to_char("No looping aliases please.\n\r", ch);
      return;
    }
  }
  if (alias_free == NULL )
  {
    ali = alloc_perm(sizeof(*ali));
  }
  else
  {
    ali = alias_free;
    alias_free = alias_free->next;
  }
  ali->short_n = str_dup(arg1);
  ali->long_n = str_dup(argument);
  ali->next = ch->pcdata->alias;
  ch->pcdata->alias = ali;
  ch->pcdata->alias_count++;
  send_to_char("Ok.\n\r",ch);
  return;
}

void do_showalias(CHAR_DATA *ch, char *argument)
{
  ALIAS_DATA *ali;
  bool found = FALSE;
  char buf[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;

  for (ali = ch->pcdata->alias; ali; ali = ali->next)
  {
    found = TRUE;
    sprintf(buf, "%s '%s'\n\r", ali->short_n, ali->long_n);
    send_to_char(buf,ch);
  }
  if (!found) send_to_char("You have no aliases.\n\r",ch);
  return;
}

void do_removealias(CHAR_DATA *ch, char *argument)
{
  ALIAS_DATA *ali;
  char arg[MAX_INPUT_LENGTH];

  if (IS_NPC(ch)) return;

  one_argument(argument, arg);

  if (arg[0] == '\0')
  {
    send_to_char("Remove which alias?\n\r",ch);
    return;
  }
  for (ali = ch->pcdata->alias; ali; ali = ali->next)
  {
    if (!str_cmp(arg, ali->short_n))
    {
      alias_remove(ch, ali);
      send_to_char("Alias removed.\n\r",ch);
      ch->pcdata->alias_count--;
      return;
    }
  }
  send_to_char("No such alias.\n\r",ch);
  return;
}

void do_birth(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  CHAR_DATA *victim;
  DESCRIPTOR_DATA *dummy;

  if (IS_NPC(ch)) return;

  argument = one_argument(argument, arg);

  if (ch->sex != SEX_FEMALE)
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if (!IS_EXTRA(ch, EXTRA_PREGNANT))
  {
    send_to_char("But you are not even pregnant!\n\r",ch);
    return;
  }
  if (!IS_EXTRA(ch, EXTRA_LABOUR))
  {
    send_to_char("You're not ready to give birth yet.\n\r",ch);
    return;
  }
  if ( arg[0] == '\0' )
  {
    if (ch->pcdata->genes[4] == SEX_MALE)
      send_to_char( "What do you wish to name your little boy?\n\r", ch );
    else if (ch->pcdata->genes[4] == SEX_FEMALE)
      send_to_char( "What do you wish to name your little girl?\n\r", ch );
    else
      send_to_char( "What do you wish to name your child?\n\r", ch );
    return;
  }
  if (!check_parse_name(arg))
  {
    send_to_char("Thats an illegal name.\n\r", ch);
    return;
  }
  if (char_exists(FALSE, arg))
  {
    send_to_char("That player already exists.\n\r", ch);
    return;
  }
  if (descriptor_free == NULL )
  {
    dummy = alloc_perm(sizeof(*dummy));
  }
  else
  {
    dummy = descriptor_free;
    descriptor_free = descriptor_free->next;
  }
  arg[0] = UPPER(arg[0]);
  load_char_short(dummy, arg);
  victim = dummy->character;

  victim->pcdata->perm_str = 18;
  victim->pcdata->perm_int = 18;
  victim->pcdata->perm_wis = 18;
  victim->pcdata->perm_dex = 18;
  victim->pcdata->perm_con = 18;
  victim->class = 0;
  victim->max_hit = 5000;
  victim->hit = 5000;
  victim->max_mana = 5000;
  victim->mana = 5000;
  victim->max_move = 5000;
  victim->move = 5000;
  victim->sex = ch->pcdata->genes[4];
  victim->level = 2;
  victim->in_room = ch->in_room;
  set_learnable_disciplines(ch);
  save_char_obj(victim);
  free_char(victim);

  /*
   * Being nice and putting the descriptor back into the free list.
   */
  dummy->next     = descriptor_free;
  descriptor_free = dummy;

  ch->pcdata->genes[9] += 1;
  REMOVE_BIT(ch->extra, EXTRA_PREGNANT);
  REMOVE_BIT(ch->extra, EXTRA_LABOUR);  
  save_char_obj(ch);
  if (ch->pcdata->genes[4] == SEX_MALE)
    sprintf(buf, "%s gives birth to %s's son, named %s!", ch->name, ch->pcdata->cparents, arg);
  else
    sprintf(buf, "%s gives birth to %s's daughter, named %s!", ch->name, ch->pcdata->cparents, arg);
  do_info(ch, buf);
  send_to_char("Pop!\n\r", ch);
  return;
}

void do_setdecap(CHAR_DATA *ch, char *argument)
{  
  char buf[MAX_STRING_LENGTH];
  int cost = 1000;

  if (IS_NPC(ch)) return;
  if (IS_SET(ch->pcdata->jflags, JFLAG_NOSET))
  {
    send_to_char("Your not allowed to use custom settings.\n\r", ch);
    return;
  }
  if (ch->pcdata->quest < cost)
  {
    sprintf(buf,"You need %d more qps to use this command.\n\r", cost - ch->pcdata->quest);
    send_to_char(buf,ch);
    return;
  }
  smash_tilde(argument);
  if (strlen(argument) < 10 || strlen(argument) > 90)
  {
    send_to_char("Between 10 and 90 chars... thanks.\n\r",ch);
    return;
  }
  if (!is_contained2("$n", argument) || !is_contained2("$N", argument))
  {
    send_to_char("You really need to include both $n and $N in the message.\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->jflags, JFLAG_SETDECAP)) SET_BIT(ch->pcdata->jflags, JFLAG_SETDECAP);
  free_string(ch->pcdata->decapmessage);
  ch->pcdata->decapmessage = str_dup(argument);
  ch->pcdata->quest -= cost;
  send_to_char("done.\n\r",ch);
  return;
}

void do_settie(CHAR_DATA *ch, char *argument) 
{  
  char buf[MAX_STRING_LENGTH];
  int cost = 1000;
   
  if (IS_NPC(ch)) return;
  if (IS_SET(ch->pcdata->jflags, JFLAG_NOSET))
  {
    send_to_char("Your not allowed to use custom settings.\n\r", ch);
    return;
  }
  if (ch->pcdata->quest < cost)
  {
    sprintf(buf,"You need %d more qps to use this command.\n\r", cost - ch->pcdata->quest);
    send_to_char(buf,ch);
    return;
  }
  smash_tilde(argument);
  if (strlen(argument) < 10 || strlen(argument) > 90)
  {
    send_to_char("Between 10 and 90 chars... thanks.\n\r",ch);
    return;
  }
  if (!is_contained2("$n", argument) || !is_contained2("$N", argument))
  {
    send_to_char("You really need to include both $n and $N in the message.\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->jflags, JFLAG_SETTIE)) SET_BIT(ch->pcdata->jflags, JFLAG_SETTIE);
  free_string(ch->pcdata->tiemessage);
  ch->pcdata->tiemessage = str_dup(argument);
  ch->pcdata->quest -= cost;
  send_to_char("done.\n\r",ch);
  return;
}    

void do_setlogout(CHAR_DATA *ch, char *argument)
{  
  char buf[MAX_STRING_LENGTH];
  int cost = 1000;
   
  if (IS_NPC(ch)) return;
  if (IS_SET(ch->pcdata->jflags, JFLAG_NOSET))
  {
    send_to_char("Your not allowed to use custom settings.\n\r", ch);
    return;
  }
  if (ch->pcdata->quest < cost)
  {
    sprintf(buf,"You need %d more qps to use this command.\n\r", cost - ch->pcdata->quest);
    send_to_char(buf,ch);
    return;
  }
  smash_tilde(argument);
  if (strlen(argument) < 10 || strlen(argument) > 90)
  {
    send_to_char("Between 10 and 90 chars... thanks.\n\r",ch);
    return;
  }
  if (!is_contained2("$n", argument)) 
  {
    send_to_char("You really need to include $n in the message.\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->jflags, JFLAG_SETLOGOUT)) SET_BIT(ch->pcdata->jflags, JFLAG_SETLOGOUT);
  free_string(ch->pcdata->logoutmessage);
  ch->pcdata->logoutmessage = str_dup(argument);
  ch->pcdata->quest -= cost;
  send_to_char("done.\n\r",ch);
  return;
}  

void do_setlogin(CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH]; 
  int cost = 1000;
   
  if (IS_NPC(ch)) return;
  if (IS_SET(ch->pcdata->jflags, JFLAG_NOSET))
  {
    send_to_char("Your not allowed to use custom settings.\n\r", ch);
    return;
  }
  if (ch->pcdata->quest < cost)
  {
    sprintf(buf,"You need %d more qps to use this command.\n\r", cost - ch->pcdata->quest);
    send_to_char(buf,ch);
    return;
  }
  smash_tilde(argument);
  if (strlen(argument) < 10 || strlen(argument) > 90)
  {
    send_to_char("Between 10 and 90 chars... thanks.\n\r",ch);
    return;
  }
  if (!is_contained2("$n", argument))
  {
    send_to_char("You really need to include $n in the message.\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->jflags, JFLAG_SETLOGIN)) SET_BIT(ch->pcdata->jflags, JFLAG_SETLOGIN);
  free_string(ch->pcdata->loginmessage);
  ch->pcdata->loginmessage = str_dup(argument);
  ch->pcdata->quest -= cost;
  send_to_char("done.\n\r",ch);
  return;
}

void do_setavatar(CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];
  int cost = 1000;

  if (IS_NPC(ch)) return;
  if (IS_SET(ch->pcdata->jflags, JFLAG_NOSET))
  {
    send_to_char("Your not allowed to use custom settings.\n\r", ch);
    return;
  }
  if (ch->pcdata->quest < cost)
  {
    sprintf(buf,"You need %d more qps to use this command.\n\r", cost - ch->pcdata->quest);
    send_to_char(buf,ch);
    return;
  }
  smash_tilde(argument);
  if (strlen(argument) < 10 || strlen(argument) > 90)
  {
    send_to_char("Between 10 and 90 chars... thanks.\n\r",ch);
    return;
  }
  if (!is_contained2("$n", argument))   
  {
    send_to_char("You really need to include $n in the message.\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->jflags, JFLAG_SETAVATAR)) SET_BIT(ch->pcdata->jflags, JFLAG_SETAVATAR);
  free_string(ch->pcdata->avatarmessage);
  ch->pcdata->avatarmessage = str_dup(argument);
  ch->pcdata->quest -= cost;
  send_to_char("done.\n\r",ch);
  return;
}
