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

void do_tribe(CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];
  CHAR_DATA *gch;
  
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_WEREWOLF) )
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  send_to_char("#C          -*-  The Tribe of Gaia  -*-#n\n\r",ch);
  send_to_char("#7[#CName                #7] [#C  Hp   #7] [#C Mana  #7] [#C Move  #7]#n\n\r",ch);
  for ( gch = char_list; gch != NULL; gch = gch->next)
  {
    if (IS_NPC(gch)) continue;
    if (!IS_CLASS(gch, CLASS_WEREWOLF)) continue;
    sprintf( buf,"#7[%-20s] [%-7d] [%-7d] [%-7d]#n\n\r",
    capitalize( gch->name ), gch->hit, gch->mana, gch->move);
    send_to_char( buf, ch );
  }
  return;
}

void do_klaive( CHAR_DATA *ch, char *argument )
{
  OBJ_DATA *obj;
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_WEREWOLF))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( 60 > ch->practice)
  {
    send_to_char("It costs 60 points of primal to create a Lesser Klaive.\n\r",ch);
    return;
  }
  ch->practice   -= 60;
  obj = create_object(get_obj_index(OBJ_VNUM_LKLAIVE)  ,0 );
  SET_BIT(obj->quest, QUEST_RELIC);
  obj->value[1] = 30;
  obj->value[2] = 60;
  obj->questowner = str_dup(ch->pcdata->switchname);
  obj_to_char(obj, ch);
  act("A Lesser Klaive appears in your hands in a flash of light.",ch,NULL,NULL,TO_CHAR);
  act("A Lesser Klaive appears in $n's hands in a flash of light.",ch,NULL,NULL,TO_ROOM);
  return;
}


void do_sclaws( CHAR_DATA *ch, char *argument )
{

   if (IS_NPC(ch))
	return;

   if (!IS_CLASS(ch, CLASS_WEREWOLF))
   {
        send_to_char("What?\n\r",ch);
        return;
   }

   if (ch->power[DISC_WERE_LUNA] < 5)
   {
	send_to_char("You need level 5 in Luna to use Silver Claws.\n\r",ch);
	return;
   }

   if (!IS_SET(ch->newbits, NEW_SCLAWS))
   {
	send_to_char("Your claws turn into pure silver!\n\r",ch);
        act("$n's claws turn to pure silver!",ch,NULL,NULL,TO_ROOM);
        SET_BIT(ch->newbits, NEW_SCLAWS);
	return;
   }

   if (IS_SET(ch->newbits, NEW_SCLAWS))
   {
	send_to_char("Your claws return to their normal state.\n\r",ch);
	act("$n's claws are no longer composed of pure silver.\n\r",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->newbits, NEW_SCLAWS);
	return;
   }

}

void do_moonbeam( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg1 [MAX_INPUT_LENGTH];
 
    argument = one_argument( argument, arg1 );
  
    if ( IS_NPC(ch) )
    	return;
 
    if (!IS_CLASS(ch, CLASS_WEREWOLF))
    {
	send_to_char("Huh?\n\r",ch);
      return;
    }
 
    if (ch->power[DISC_WERE_LUNA] < 8 )
    {
	send_to_char("You need to obtain level 8 in Luna to use Moonbeam.\n\r",ch);
    	return;
    }
 
    if (ch->mana < 500)
    {
	send_to_char("You do not have enough energy to summon a Moonbeam.\n\r",ch);
	return;
    }
 
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Who do you wish to strike?\n\r", ch );
      return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
   	 send_to_char( "They aren't here.\n\r", ch );
	 return;
    }
    if (ch == victim) return;
    if (is_safe(ch,victim) == TRUE) return;
    if (ch->fighting == NULL)
        set_fighting(ch,victim);
        update_pos(victim);
  
    	act("$N is struck by a huge moonbeam.", ch, NULL, victim,TO_CHAR);
	act("$N is struck by a deadly beam of moonlight from $n.", ch, NULL, victim,TO_NOTVICT);
	act("You are struck by a deadly beam of moonlight!", ch, NULL, victim,TO_VICT);
    if (IS_GOOD(victim))
	hurt_person(ch,victim, 500);
    if (IS_EVIL(victim))
        hurt_person(ch,victim, 1000);  
    if (IS_NEUTRAL(victim))
        hurt_person(ch,victim, 750);  
	ch->mana -= 500;
    WAIT_STATE(ch,12);
	return;
    
}


void do_moongate(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    char       arg [MAX_INPUT_LENGTH];
 
    one_argument( argument, arg );
 
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WEREWOLF))
    {
      send_to_char("Huh?\n\r",ch);
      return;
    }

    if (ch->power[DISC_WERE_LUNA] < 6)
    {
      send_to_char("You need to obtain level 6 Luna to use moongate.\n\r",ch);
      return;
    }

    if ( arg[0] == '\0')
    {
        send_to_char( "Who do you wish to gate to?\n\r", ch );
	return;
    }
 
    if ( (victim = get_char_world( ch, arg ) ) == NULL )
    {
    send_to_char("They arent here.\n\r",ch);
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
 
    if ( (victim == ch) 
    ||   victim->in_room == NULL
    ||   (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON) )
    ||   IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(ch->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
    ||   victim->in_room->vnum == ch->in_room->vnum)
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }
 
 
    obj = create_object( get_obj_index( OBJ_VNUM_GATE2 ), 0 );
    obj->value[0] = victim->in_room->vnum;
    obj->value[3] = ch->in_room->vnum;
    obj->timer = 5;
    if (IS_AFFECTED(ch,AFF_SHADOWPLANE))
	obj->extra_flags = ITEM_SHADOWPLANE;
    obj_to_room( obj, ch->in_room );
 
    obj = create_object( get_obj_index( OBJ_VNUM_GATE2 ), 0 );
    obj->value[0] = ch->in_room->vnum;
    obj->value[3] = victim->in_room->vnum;
    obj->timer = 5;
    if (IS_AFFECTED(victim,AFF_SHADOWPLANE))
	obj->extra_flags = ITEM_SHADOWPLANE;
    obj_to_room( obj, victim->in_room );
    act( "A look of concentration passes over $n's face.", ch, NULL,NULL, TO_ROOM );
    send_to_char("A look of concentration passes over your face.\n\r",ch);
    act( "$p appears in front of $n in a beam of moonlight.", ch, obj, NULL, TO_ROOM );
    act( "$p appears in front of you in a beam of moonlight.", ch, obj, NULL, TO_CHAR );
    act( "$p appears in front of $n in a beam of moonlight.", victim, obj, NULL, TO_ROOM );
    act( "$p appears in front of you in a beam of moonlight.", ch, obj, victim, TO_VICT );
    return;
}

void do_gmotherstouch( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg1 [MAX_INPUT_LENGTH];
 
    argument = one_argument( argument, arg1 );
  
 
    if ( IS_NPC(ch) )
    	return;
 
 
    if (!IS_CLASS(ch, CLASS_WEREWOLF))
    {
	send_to_char("Huh?\n\r",ch);
      	return;
    }
 
    if (ch->power[DISC_WERE_LUNA] < 4 )
    {
	send_to_char("You need to obtain level 4 in Luna to use Grandmother's Touch.\n\r",ch);
    	return;
    }
 
    if ( arg1[0] == '\0' )
    {
      send_to_char( "Who is in need of your touch?\n\r", ch );
      return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
   	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (ch->mana < 100 && ch->fighting != NULL)
    {
	send_to_char("You do not have enough energy to summon the Grandmother's Touch.\n\r",ch);
	return;
    }
 
        
    if (ch->fighting != NULL)
    {
    	act("$N is filled with a feeling of warmth.", ch, NULL, victim,TO_CHAR);
	act("$N is surrounded by a beam of moonlight from $n's touch.", ch, NULL, victim,TO_NOTVICT);
	act("You are surrounded by a beam of moonlight, and feel restored.", ch, NULL, victim,TO_VICT);
	victim->hit += 200;
	ch->mana -= 100;
	update_pos(victim);
        WAIT_STATE(ch,16);
	return;
    }

    if (ch->mana < 300 && ch->fighting == NULL)
    {
	send_to_char("You do not have enough energy to summon the Grandmother's Touch.\n\r",ch);
	return;
    }

    if (ch->fighting == NULL)
    {
        act("$N is filled with a feeling of warmth.", ch, NULL, victim,TO_CHAR);
	act("$N is surrounded by a beam of moonlight from $n's touch.", ch, NULL, victim,TO_NOTVICT);
	act("You are surrounded by a beam of moonlight, and feel restored.", ch, NULL, victim,TO_VICT);
	victim->hit += 600;
	ch->mana -= 300;
        update_pos(victim);
	WAIT_STATE(ch,6);
	return;
    }
}


void do_motherstouch( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg1 [MAX_INPUT_LENGTH];
 
    argument = one_argument( argument, arg1 );

    if (IS_NPC(ch))
	return;

    if (ch->power[DISC_WERE_LUNA] < 3 )
    {
	send_to_char("You need to obtain level 3 in Luna to use Mother's Touch.\n\r",ch);
	return;
    }

    if ( arg1[0] == '\0' )
    {
      send_to_char( "Who is in need of your touch?\n\r", ch );
      return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
   	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
  
    if (ch->mana < 50 && ch->fighting != NULL)
    {
	send_to_char("You don't have enough energy to use Mother's Touch.\n\r",ch);
	return;
    }

    if (ch->fighting != NULL)
    {
    	act("$N is filled with a feeling of warmth.", ch, NULL, victim,TO_CHAR);
	act("$N is surrounded by a beam of moonlight from $n's touch.", ch, NULL, victim,TO_NOTVICT);
	act("You are surrounded by a beam of moonlight, and feel restored.", ch, NULL, victim,TO_VICT);
        victim->hit += 100;
        ch->mana -= 50;
        update_pos(victim);
	WAIT_STATE(ch,16);
	return;
    }

    if (ch->mana < 250 && ch->fighting == NULL)
    {
	send_to_char("You don't have enough energy to use Mother's Touch.\n\r",ch);
	return;
    }


    if (ch->fighting == NULL)
    {
    	act("$N is filled with a feeling of warmth.", ch, NULL, victim,TO_CHAR);
	act("$N is surrounded by a beam of moonlight from $n's touch.", ch, NULL, victim,TO_NOTVICT);
	act("You are surrounded by a beam of moonlight, and feel restored.", ch, NULL, victim,TO_VICT);
	victim->hit += 500;
	ch->mana -= 250;
        update_pos(victim);
	WAIT_STATE(ch,8);
	return;
    }

}

void do_flameclaws( CHAR_DATA *ch, char *argument )
{

    if (IS_NPC(ch))
	return;
    
   if (!IS_CLASS(ch, CLASS_WEREWOLF))
   {
        send_to_char("What?\n\r",ch);
        return;
   }
    if (ch->power[DISC_WERE_LUNA] < 1)
    {
	send_to_char("You need to obtain level 1 in luna to use flame claws.\n\r",ch);
	return;
    }

    if (IS_SET(ch->newbits, NEW_MONKFLAME))
    {
	send_to_char("The flames surrounding your claws fade.\n\r",ch);
	act("The flames surrounding $n's claws fade.\n\r",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->newbits, NEW_MONKFLAME);
	return;
    }

    if (!IS_SET(ch->newbits, NEW_MONKFLAME))
    {
	send_to_char("Your claws are surrounded by blazing flames.\n\r",ch);
        act("$n's claws are surrounded by blazing flames.\n\r",ch,NULL,NULL,TO_ROOM);
	SET_BIT(ch->newbits, NEW_MONKFLAME);
	return;
    }

}

void do_moonarmour( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WEREWOLF))
    {
	    send_to_char("Huh?\n\r",ch);
	    return;
    }
    
    if (ch->power[DISC_WERE_LUNA] < 2)
    {
	send_to_char("You must attain level 2 in Luna before you can create moon armour.\n\r",ch);
	return;
    }

    if (arg[0] == '\0')
    {
    send_to_char("What piece of moon armour do you wish to create? Ring Collar Plate Helmet Leggings Boots Gloves Sleeves Cape Belt Bracer Mask.\n\r", ch);
    return;
    }

    if      (!str_cmp(arg,"ring"     )) vnum = 33100;
    else if (!str_cmp(arg,"belt"   )) vnum = 33104;
    else if (!str_cmp(arg,"plate"    )) vnum = 33103;
    else if (!str_cmp(arg,"bracer"    )) vnum = 33101;
    else if (!str_cmp(arg,"collar"    )) vnum = 33102;
    else if (!str_cmp(arg,"helmet"    )) vnum = 33105;
    else if (!str_cmp(arg,"leggings"    )) vnum = 33106;
    else if (!str_cmp(arg,"boots"    )) vnum = 33107;
    else if (!str_cmp(arg,"gloves"    )) vnum = 33108;
    else if (!str_cmp(arg,"sleeves"    )) vnum = 33109;
    else if (!str_cmp(arg,"cape"    )) vnum = 33110;
    else if (!str_cmp(arg,"mask"    )) vnum = 33111;
    else
    {
    do_moonarmour(ch,"");
    return;
    }
    if (ch->practice < 60)
    {
	send_to_char("It costs 60 primal to create a piece of moon armour\n\r",ch);
	return;
    }
   
    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
	send_to_char("Missing object, please inform Jobo.\n\r",ch);
	return;
    }
    ch->practice -= 60;
    obj = create_object(pObjIndex, 50);
    obj->questowner = str_dup(ch->pcdata->switchname);
    obj_to_char(obj, ch);
    act("$p appears in a beam of moonlight.",ch,obj,NULL,TO_CHAR);
    act("$p appears in a beam of moonlight.",ch,obj,NULL,TO_ROOM);
    return;
}



void do_rend(CHAR_DATA *ch,char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch,CLASS_WEREWOLF))
  {
  send_to_char("Huh?\n\r",ch);
  return;
  }
  if (ch->power[DISC_WERE_BOAR] < 7)
  {
  send_to_char("You need Boar 7 to Rend equipment.\n\r",ch);
  return;
  }
  if (!IS_SET(ch->newbits, NEW_REND))
  {
  send_to_char("You will now ruthlessly rend your opponents eq.\n\r",ch);
  SET_BIT(ch->newbits, NEW_REND);
  return;
  }
  else if (IS_SET(ch->newbits, NEW_REND))
  { 
  send_to_char("Your no longer mean to your enemies eq.\n\r",ch);
  REMOVE_BIT(ch->newbits,NEW_REND);
  return;
  }
return;
}

void do_skin(CHAR_DATA *ch,char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch,CLASS_WEREWOLF))
  {
  send_to_char("Huh?\n\r",ch);
  return;
  }
  if (ch->power[DISC_WERE_BEAR] < 7)
  {
  send_to_char("You need Bear 7 to toughen your skin.\n\r",ch);
  return;
  }
  if (!IS_SET(ch->newbits, NEW_SKIN))
  {
  send_to_char("Your skin hardens.\n\r",ch);
  ch->armor -= 100 ;
  SET_BIT(ch->newbits, NEW_SKIN);
  return;
  }
  else if (IS_SET(ch->newbits, NEW_SKIN))
  { 
  send_to_char("Your skin softens.\n\r",ch);
  ch->armor += 100;
  REMOVE_BIT(ch->newbits,NEW_SKIN);
  return;
  }
return;
}

void do_jawlock(CHAR_DATA *ch,char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch,CLASS_WEREWOLF))
  {
  send_to_char("Huh?\n\r",ch);
  return;
  }
  if (ch->power[DISC_WERE_RAPT] < 8)
  {
  send_to_char("You need Raptor 8 to use locked jaw.\n\r",ch);
  return;
  }
  if (!IS_SET(ch->newbits, NEW_JAWLOCK))
  {
  send_to_char("Your jaw grows stronger and more ready to clamp on your enemy.\n\r",ch);
  SET_BIT(ch->newbits, NEW_JAWLOCK);
  return;
  }
  else if (IS_SET(ch->newbits, NEW_JAWLOCK))
  { 
  send_to_char("Your jaw weakens.\n\r",ch);
  REMOVE_BIT(ch->newbits,NEW_JAWLOCK);
  return;
  }
return;
}

void do_perception(CHAR_DATA *ch,char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch,CLASS_WEREWOLF))
  {
  send_to_char("Huh?\n\r",ch);
  return;
  }
  if (ch->power[DISC_WERE_RAPT] < 3)
  {
  send_to_char("You need Raptor 3 to heighten your Perception.\n\r",ch);
  return;
  }
  if (!IS_SET(ch->newbits, NEW_PERCEPTION))
  {
  send_to_char("You become more aware of stealthy enemies.\n\r",ch);
  SET_BIT(ch->newbits, NEW_PERCEPTION);
  return;
  }
  else if (IS_SET(ch->newbits, NEW_PERCEPTION))
  { 
  send_to_char("Your perception dulls.\n\r",ch);
  REMOVE_BIT(ch->newbits,NEW_PERCEPTION);
  return;
  }
return;
}

void do_roar(CHAR_DATA *ch,char *argument)
{
CHAR_DATA *victim;
char buf[MAX_STRING_LENGTH];
if (IS_NPC(ch)) return;
if (!IS_CLASS(ch,CLASS_WEREWOLF))
{send_to_char("You let out an insane BeeF-powered ROAR!!!!!!!!\n\r",ch);return;}
if (ch->power[DISC_WERE_BEAR] < 6)
{
send_to_char("You need bear 6 to roar.\n\r",ch);
return;
}
if ((victim=ch->fighting) == NULL)
{
send_to_char("You arent fighting anyone.\n\r",ch);
return;
}

  if (number_range(1,6)==2)
  {
sprintf(buf,"%s roars and screams, intimidating the hell out of %s.",
ch->name,victim->name);
act(buf,ch,NULL,NULL,TO_ROOM);
sprintf(buf,"You roar and scream, intimidating the hell out of %s.\n\r",
victim->name);
send_to_char(buf,ch);
do_flee(victim,"");
WAIT_STATE(ch,18);
return;
  }
  else
  {
   stc("You roar loudly.\n\r",ch);
   stc("Nothing happens.\n\r",ch);
   act("$n roars loudly.",ch,NULL,NULL,TO_ROOM);
   WAIT_STATE(ch, 12);
   return;
  }
}
 
/*
void do_quills(CHAR_DATA *ch,char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch,CLASS_WEREWOLF))
  {
  send_to_char("Huh?\n\r",ch);
  return;
  }
  if (ch->power[DISC_WERE_BEAR] < 5)
  {
  send_to_char("You need Bear 5 to extend your quills.\n\r",ch);
  return;
  }
  if (!IS_SET(ch->newbits, NEW_QUILLS))
  {
  send_to_char("Sharp quills extend from your back.\n\r",ch);
  SET_BIT(ch->newbits, NEW_QUILLS);
  return;
  }
  else if (IS_SET(ch->newbits, NEW_QUILLS))
  { 
  send_to_char("Your quills sink into your back.\n\r",ch);
  REMOVE_BIT(ch->newbits,NEW_QUILLS);
  return;
  }
return;
}
*/

void do_slam(CHAR_DATA *ch,char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch,CLASS_WEREWOLF))
  {
  send_to_char("Huh?\n\r",ch);
  return;
  }
  if (ch->power[DISC_WERE_BEAR] < 8)
  {
  send_to_char("You need Bear 8 to attempt to shoulder slam.\n\r",ch);
  return;
  }
  if (!IS_SET(ch->newbits, NEW_SLAM))
  {
  send_to_char("You will now shoulder slam when you get a chance.\n\r",ch);
  SET_BIT(ch->newbits, NEW_SLAM);
  return;
  }
  else if (IS_SET(ch->newbits, NEW_SLAM))
  { 
  send_to_char("Your now dont shoulderslam when given a chance.\n\r",ch);
  REMOVE_BIT(ch->newbits,NEW_SLAM);
  return;
  }
return;
}
 
void do_shred( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    argument = one_argument( argument, arg );
 
 
    if(!IS_CLASS(ch,CLASS_WEREWOLF))
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }
    if (ch->power[DISC_WERE_RAPT] < 7)
    {
    send_to_char("You need Raptor 7 to Shred.\n\r",ch);
    return;
    }
 
    if ( arg[0] == '\0' )
    {
        send_to_char( "Shred whom?\n\r", ch );
	return;
    }
 
 
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
 
    if ( victim == ch )
    {
        send_to_char( "How can you shred yourself?\n\r", ch );
	return;
    }
 
    if ( victim->fighting != NULL )
    {
        send_to_char( "You can't shred a fighting person.\n\r", ch );
	return;
    }
 
    if ( victim->hit < victim->max_hit )
    {
	act( "$N is hurt and suspicious ... you can't sneak up.",
	    ch, NULL, victim, TO_CHAR );
	return;
    }

    if (!IS_AFFECTED(ch, AFF_SHADOWPLANE))
    {
      send_to_char("You must be ShadowPlaned to Shred!\n\r",ch);
      return;
    }
    do_shadowplane(ch,"");
    if (is_safe(ch,victim)) return;
    WAIT_STATE(ch,24);

    one_hit(ch,victim,gsn_shred,1);
    one_hit(ch,victim,gsn_shred,1);

    if (ch->power[DISC_WERE_RAPT] > 6)
      one_hit(ch,victim,gsn_shred,1);
    if (ch->power[DISC_WERE_RAPT] > 7)
      one_hit(ch,victim,gsn_shred,1);
    if (ch->power[DISC_WERE_RAPT] > 9)
      one_hit(ch,victim,gsn_shred,1);
    return;
}


void do_talons( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char buf[MAX_STRING_LENGTH];
  int dam;

  if (IS_NPC(ch)) return;
 
  if (!IS_CLASS(ch,CLASS_WEREWOLF) && !IS_SET(ch->itemaffect, ITEMA_TALON))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->power[DISC_WERE_RAPT] < 10 && !IS_SET(ch->itemaffect, ITEMA_TALON))
  {
    send_to_char("You need level 10 Raptor to use talons.\n\r",ch);
    return;
  }
  if ( ( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  WAIT_STATE( ch, 12 );

  if (!IS_NPC(victim)) dam = number_range(400,600);
  else dam = number_range(2000,4000);

  sprintf(buf,"Your talons strike $N incredibly hard! [%d]\n\r",dam);
  act(buf,ch,NULL,victim,TO_CHAR);
  sprintf(buf,"$n's talons strike you incredibly hard! [%d]\n\r",dam);
  act(buf,ch,NULL,victim,TO_VICT);
  sprintf(buf,"$n's talons strike $N incredibly hard! [%d]\n\r",dam);
  act(buf,ch,NULL,victim,TO_NOTVICT); 
  hurt_person(ch,victim, dam);
  return;
}
 
 
 
void do_devour( CHAR_DATA *ch, char *argument )
{
char arg[MAX_INPUT_LENGTH];
OBJ_DATA *obj;
char buf[MAX_INPUT_LENGTH];
int hpgain;
 
one_argument( argument, arg );
if (!IS_CLASS(ch,CLASS_WEREWOLF))
{
send_to_char("Huh?\n\r",ch);
return;
}
if (ch->power[DISC_WERE_RAPT] < 5)
{
send_to_char("You require Raptor 5 to devour.\n\r",ch);
return;
}
 
 
if ( arg[0] == '\0' )
{
    send_to_char("Devour what?\n\r",ch);
    return;
    }
obj = get_obj_list( ch, arg, ch->in_room->contents );
if ( obj == NULL )
    {
        send_to_char( "You can't find it.\n\r", ch );
    return;
}
if (obj->item_type != ITEM_CORPSE_NPC)
{
    act( "You are unable to devour $p.", ch, obj, 0, TO_CHAR);
return;
}
else if (obj->chobj != NULL && !IS_NPC(obj->chobj) &&
obj->chobj->pcdata->obj_vnum != 0)
{
    act( "You are unable to devour $p.", ch, obj, 0, TO_CHAR);
    return;
}
hpgain = number_range(100,250);
ch->hit += hpgain;        
if (ch->hit > ch->max_hit)
{
ch->hit = ch->max_hit;
} 
sprintf( buf,"You devour %d hp of life from $p.", hpgain );
act( buf, ch, obj, NULL, TO_CHAR );
act( "You devour $p.", ch, obj, NULL, TO_CHAR );
act( "$p is devoured by $n.", ch, obj, NULL, TO_ROOM );
extract_obj( obj );
return;
}


void do_staredown( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    
    argument = one_argument( argument, arg );
    if ( IS_NPC(ch) ) return;
    if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
    {
        stc("Huh?\n\r",ch);
        return;
    }
    if ( ch->power[DISC_WERE_OWL] < 5 )
    {
      stc("Your power in owl is not great enough.\n\r",ch);
      return;
    }
    if ( ( victim = get_char_room( ch, arg) ) == NULL )
    {
      stc("They aren't here.\n\r",ch);
      return;
    }
    arg[0] = UPPER(arg[0]);
    if ( ch->fighting == NULL )
    {
      stc("You must be fighting to use the gift of Staredown.\n\r",ch);
      return;
    }
    if ( victim->fighting == NULL )
    {
      sprintf(buf, "%s is not fighting anyone.", arg);
      stc(buf, ch);
      return;
    }
    WAIT_STATE( ch, 16 );
    if ( IS_NPC(victim) )
    {
      if ( ch->power[DISC_WERE_OWL]  < 6 )
      {
          if ( number_range( 1, 3 ) != 1 )
        {
          act("You stare deeply into $N's eyes, but nothing happens.",ch,NULL,victim,TO_CHAR);
          act("$n stares deeply into your eyes.",ch,NULL,victim,TO_VICT);
          act("$n stares deeply into $N's eyes, but nothing happens.",ch,NULL,victim,TO_NOTVICT);
            return;
        }
      }  
      act("You stare into $N's eyes, and $E flees in terror.",ch,NULL,victim,TO_CHAR);
      act("$n stares deeply into your eyes, and you are filled with terror.",ch,NULL,victim,TO_VICT);
      act("$n stares deeply into $N's eyes, and $E flees in terror.",ch,NULL,victim,TO_NOTVICT);
      do_flee( victim, "" );
      return;
    }
    else
    {   
      if ( ch->power[DISC_WERE_OWL] < 7 )
      {
        if ( number_range( 1, 4 ) != 2 )
        {
          act("You stare deeply into $N's eyes, but nothing happens.",ch,NULL,victim,TO_CHAR);
          act("$n stares deeply into your eyes.",ch,NULL,victim,TO_VICT);
          act("$n stares deeply into $N's eyes, but nothing happens.",ch,NULL,victim,TO_NOTVICT);
            return;
        }
      }
      act("You stare into $N's eyes, and $E flees in terror.",ch,NULL,victim,TO_CHAR);
      act("$n stares deeply into your eyes, and you are filled with terror.",ch,NULL,victim,TO_VICT);
      act("$n stares deeply into $N's eyes, and $E flees in terror.",ch,NULL,victim,TO_NOTVICT);
      do_flee( victim, "" );
      return;  
    }
  return;
}
      
void do_disquiet( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    AFFECT_DATA af;
    char arg [MAX_INPUT_LENGTH];
      
    argument = one_argument( argument, arg );
    
    if (IS_NPC(ch)) return;
     
    if (!IS_CLASS(ch, CLASS_WEREWOLF))
    {
      send_to_char("Huh?\n\r", ch);
      return;
    }
      
    if ( ch->power[DISC_WERE_OWL] < 6 )
    {
      stc("Your power in owl is not great enough.\n\r",ch);
      return;
    }
     
    if ( ( victim = get_char_room(ch, arg) ) == NULL )
    {  
      stc("They are not here.\n\r",ch);   
      return;
    }
          
    if ( is_safe( ch, victim) ) return;
            
    if ( ch->gnosis[GCURRENT] < 1 )
    {    
      stc("You need one point of gnosis power to use the gift of Disquiet.\n\r",ch);  
      return;
    }
      
    af.type      = skill_lookup("reserved");
    af.duration  = ch->power[DISC_WERE_OWL];
    af.location  = APPLY_DAMROLL;
    af.modifier  = -( ch->power[DISC_WERE_OWL] * 5 );
    af.bitvector = 0;
    affect_to_char( victim, &af );
        
    af.type      = skill_lookup("reserved");
    af.duration  = ch->power[DISC_WERE_OWL];
    af.location  = APPLY_HITROLL;
    af.modifier  = -( ch->power[DISC_WERE_OWL] * 5 );
    af.bitvector = 0;
    affect_to_char( victim, &af );
       
    ch->gnosis[CURRENT]--;
    act("Your eyes glow a sinister red as you glare at $N.",ch,NULL,victim,TO_CHAR);
    act("$n's eyes glow a sinister red as $e glares at you.\n\rYou feel odd.",ch,NULL,victim,TO_VICT);
    act("$n's eyes glow a sinister red as $e glares at $N.",ch,NULL,victim,TO_NOTVICT);
    WAIT_STATE(ch,4);
    return;
  
}
      
void do_reshape(CHAR_DATA *ch, char *argument)   
{
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char arg3[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    
    argument=one_argument(argument,arg1);
    argument=one_argument(argument,arg2);
    strcpy(arg3,argument);

    if (IS_NPC(ch)) return;
      
    if (!IS_CLASS(ch,CLASS_WEREWOLF))
    {
      send_to_char("Huh?\n\r",ch);
      return;
    }
      
    if (ch->power[DISC_WERE_OWL] < 7)
    {
      stc("Your power in owl is not great enough.\n\r",ch);
      return;
    }  
      
    if ( ( obj = get_obj_carry( ch,arg1 ) ) == NULL )
    {
      send_to_char("You dont have that item.\n\r",ch);
      return;
    }       
    
    if (!(!str_cmp(arg2,"short") || !str_cmp(arg2,"name")))
    {
      send_to_char("syntax : reshape <item> <short/name> <newname>.\n\r",ch);
      return;
    } 

    if (strlen(arg3) > 60 || strlen(arg3) < 3)
    {
      send_to_char("The name should be between 3 and 60 characters.\n\r",ch);
      return;
    }
    if (has_bad_chars(ch, arg3))
    {
      send_to_char("Illegal chars.\n\r",ch);
      return;
    }
  if (IS_SET(obj->quest, QUEST_ARTIFACT) || IS_SET(obj->quest, QUEST_PRIZE))
  {
    send_to_char("Not on artifacts and prizes.\n\r",ch);
    return;
  }
    
    if (!str_cmp(arg2,"name"))
    {
        free_string(obj->name);
        obj->name = str_dup(arg3);
        obj->questmaker = str_dup(ch->name);
    } 
    if (!str_cmp(arg2,"short"))
    {
        free_string(obj->short_descr);
        obj->short_descr=str_dup(arg3);
        obj->questmaker = str_dup(ch->name);
    }
    
    send_to_char("Ok.\n\r",ch);
    return;
}

void do_cocoon( CHAR_DATA *ch, char *argument )
{     
    if ( IS_NPC(ch) ) return;
     
    if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
    {
        stc("Huh?\n\r",ch);
        return;
    }
     
    if ( ch->power[DISC_WERE_OWL] < 8 )
    {
      stc("Your power in owl is not great enough.\n\r",ch);
      return;
    }
     
      
    if ( IS_GAR1(ch, WOLF_COCOON) )
    {       
      stc("Your cocoon breaks and falls to the ground.\n\r",ch);
      act("$n's cocoon breaks and falls to the ground.",ch,NULL,NULL,TO_ROOM);
      REMOVE_BIT(ch->garou1, WOLF_COCOON);
      return;
    }
      
    if ( ch->gnosis[GCURRENT] < 2 )
    {
      stc("You do not have enough gnosis to use Cocoon.\n\r",ch);
      return;
    }
     
    else
    {
      stc("Your body is surrounded by a thick, opaque epidermis.\n\r",ch);
      act("$n's body is surrounded by a thick, opaque epidermis.",ch,NULL,NULL,TO_ROOM);
      SET_BIT(ch->garou1, WOLF_COCOON);
      ch->gnosis[GCURRENT]--;
      return;
    }
  return;  
}  
    
/*
 * Metis Gifts
 * Level One  - Gift of the Porcupine - Quills
 * Level Two  - Burrow          - Like Demon Travel
 * Level Three- Eyes of the Cat - Nightsight
 * Level Four - Wither Limb     - Withers a limb, easier on NPCs
 * Level Five - Totem Gift      - Random, Special Power. Ooo.
 */  
        
void do_quills( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    
    if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
    {
        stc("Huh?\n\r",ch);
        return;
    }
      
    if ( ch->power[DISC_WERE_HAWK] < 5 )
    {
      stc("Your power in hawk is not great enough.\n\r",ch);
        return;
    }
      
    if ( IS_SET(ch->newbits, NEW_QUILLS) )
    {
        stc("Your quills soften and twist into fur.\n\r",ch);
        act("$n's quills soften and revert back to fur.",ch,NULL,NULL,TO_ROOM);
        REMOVE_BIT(ch->newbits, NEW_QUILLS);
        return;
    }
     
    if ( !IS_SET(ch->newbits, NEW_QUILLS) )
    {
        stc("Your fur becomes bristly and sharp.\n\r",ch);
        act("$n's fur becomes bristly and sharp.",ch,NULL,NULL,TO_ROOM);
        SET_BIT(ch->newbits, NEW_QUILLS);
        return;
    }
  return;
}     

void do_burrow( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
     
    argument = one_argument( argument, arg );
     
    if (IS_NPC(ch)) return;
     
    if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
   
    if ( ch->power[DISC_WERE_HAWK] < 6 )
    {
      stc("Your power in hawk is not great enough.\n\r",ch);
        return;
    }
 
    if ( arg[0] == '\0' )
    {
        send_to_char("Burrow to who?\n\r", ch);
        return;
    }
  if( IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "Your room is not connected to the astral plane.\n\r",ch);
    return;
  }
 
    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "They are not here.\n\r", ch );
        return;
    }

    if ( IS_NPC(victim) )
    { 
        send_to_char( "Not on NPC's.\n\r", ch );
        return;
    }
        
    if (IS_IMMUNE(victim, IMM_TRAVEL))
    { 
        send_to_char("I don't think they want you to do that.\n\r",ch);
        return;
    }
        
    if ( victim == ch )
    {
        send_to_char( "You cannot burrow to yourself.\n\r", ch);
        return;
    }
     
    if ( victim->level != LEVEL_AVATAR || !IS_CLASS(victim, CLASS_WEREWOLF))
    {
        send_to_char( "Nothing happens.\n\r", ch );
        return;
    }

    if (victim->in_room == NULL)
    {
        send_to_char( "Nothing happens.\n\r", ch );
        return;
    }
  if( IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "Their room is not connected to the astral plane.\n\r",ch);
    return;
  }
    
    send_to_char("You burrow into the ground.\n\r",ch);
    act("$n burrows into the ground.",ch,NULL,NULL,TO_ROOM);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    do_look(ch,"");
    send_to_char("You burrow out of the ground.\n\r",ch);
    act("$n burrows out of the ground.",ch,NULL,NULL,TO_ROOM);
    return;
}
     
// Eyes of the Cat - Nightsight.  clan.c
    
void do_wither( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    bool blah = FALSE;   
    int chance = 0;
        
    argument = one_argument( argument, arg );
     
    if ( IS_NPC(ch) ) return;
    
    if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
    {
        stc("Huh?\n\r",ch);
        return;
    }
    
    if ( ch->power[DISC_WERE_HAWK] < 7 )
    {
      stc("Your power in hawk is not great enough.\n\r",ch);
        return;
    }   
    
    if ( ch->gnosis[GCURRENT] < 3 )
    {
        stc("You do not have enough gnosis power stored to use the gift of Wither Limb.\n\r",ch);
        return;
    }   
    
    if ( arg[0] == '\0' )
    {
        stc("Who's limb do you wish to wither?\n\r",ch);
        return;
    }
    
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        stc("They aren't here.\n\r",ch);
        return;
    }
    
    if ( IS_ARM_R(victim, LOST_ARM) && IS_ARM_L(victim, LOST_ARM) )
    {
        stc("They have already lost both their arms.\n\r",ch);
        return;
    }
    
    WAIT_STATE(ch, 18);
    ch->gnosis[CURRENT] -= 3;
    if ( IS_NPC( victim ) )
    {
      chance = number_range( 45, 55 );
    
      if ( number_percent( ) > chance )
      {
        stc("Nothing happened.\n\r",ch);
        return;
      }

       if ( !IS_ARM_R(victim, LOST_ARM) )
         blah = FALSE;
       else blah = TRUE;
 
         if ( !blah )    
         {
          if (!IS_ARM_R(victim,LOST_ARM))
              SET_BIT(victim->loc_hp[3],LOST_ARM);
          if (!IS_BLEEDING(victim,BLEEDING_ARM_R))
              SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
          if (IS_BLEEDING(victim,BLEEDING_HAND_R))
              REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_R);
          act("You concentrate deeply and $N's arm starts to wither!",ch,NULL,victim,TO_CHAR);
          act("$n's eyes glow dark red as $e gazes at you, and your arm begins withering!",ch,NULL,victim,TO_NOTVICT);
          act("$n gazes evilly at $N, and $S arm begins withering!",ch,NULL,victim,TO_VICT);
          make_part(victim,"arm");
          if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
          {
              if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)
                  take_item(victim,obj);
          }
          if ((obj = get_eq_char( victim, WEAR_WIELD )) != NULL)
              take_item(victim,obj);
          if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
              take_item(victim,obj);
          if ((obj = get_eq_char( victim, WEAR_WRIST_R )) != NULL)
              take_item(victim,obj);
          if ((obj = get_eq_char( victim, WEAR_FINGER_R )) != NULL)
              take_item(victim,obj);
          return;
         }
     
         else if ( blah )
         {
          if (!IS_ARM_L(victim,LOST_ARM))
              SET_BIT(victim->loc_hp[2],LOST_ARM);
          else blah = TRUE;
          if (!IS_BLEEDING(victim,BLEEDING_ARM_L))
              SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
          if (IS_BLEEDING(victim,BLEEDING_HAND_L))
              REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_L);
          act("You concentrate deeply and $N's arm starts to wither!",ch,NULL,victim,TO_CHAR);
          act("$n's eyes glow dark red as $e gazes at you, and your arm begins withering!",ch,NULL,victim,TO_NOTVICT);
          act("$n gazes evilly at $N, and $S arm begins withering!",ch,NULL,victim,TO_VICT);
          make_part(victim,"arm");
          if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
          {
              if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)
                  take_item(victim,obj);
          }
         if ((obj = get_eq_char( victim, WEAR_WIELD )) != NULL)
              take_item(victim,obj);  
          if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
              take_item(victim,obj);   
          if ((obj = get_eq_char( victim, WEAR_WRIST_L )) != NULL)
              take_item(victim,obj);
          if ((obj = get_eq_char( victim, WEAR_FINGER_L )) != NULL)
              take_item(victim,obj);
          return;
        }
    }
       
    else if ( !IS_NPC(victim) )
    {
        chance = 15;
      if ( number_percent( ) > chance )  
      {
        act("You gaze intently at $N.\n\rNothing happens.",ch,NULL,victim,TO_CHAR);
        act("$n gazes eerily at you.\n\rNothing happens.",ch,NULL,victim,TO_VICT);
        act("$n gazes eerily at $N.",ch,NULL,victim,TO_NOTVICT);
        return;
      }
          
          
       if ( !IS_ARM_R(victim, LOST_ARM) )
         blah = FALSE;
       else blah = TRUE;
              
         if ( !blah )
         { 
          if (!IS_ARM_R(victim,LOST_ARM))
              SET_BIT(victim->loc_hp[3],LOST_ARM);
          if (!IS_BLEEDING(victim,BLEEDING_ARM_R))
              SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
          if (IS_BLEEDING(victim,BLEEDING_HAND_R))
              REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_R);
          act("You concentrate deeply and $N's arm starts to wither!",ch,NULL,victim,TO_CHAR);
          act("$n's eyes glow dark red as $e gazes at you, and your arm begins withering!",ch,NULL,victim,TO_NOTVICT);
          act("$n gazes evilly at $N, and $S arm begins withering!",ch,NULL,victim,TO_VICT);
          make_part(victim,"arm");
          if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
          {
              if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)
                  take_item(victim,obj); 
          }
          if ((obj = get_eq_char( victim, WEAR_WIELD )) != NULL)
              take_item(victim,obj);
          if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
              take_item(victim,obj);
          if ((obj = get_eq_char( victim, WEAR_WRIST_R )) != NULL)
              take_item(victim,obj);
          if ((obj = get_eq_char( victim, WEAR_FINGER_R )) != NULL)
              take_item(victim,obj);
          return;
         }
           
         else if ( blah )
         {
          if (!IS_ARM_L(victim,LOST_ARM))
              SET_BIT(victim->loc_hp[2],LOST_ARM);
          else blah = TRUE;
          if (!IS_BLEEDING(victim,BLEEDING_ARM_L))
              SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
          if (IS_BLEEDING(victim,BLEEDING_HAND_L))
              REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_L);
          act("You concentrate deeply and $N's arm starts to wither!",ch,NULL,victim,TO_CHAR);
          act("$n's eyes glow dark red as $e gazes at you, and your arm begins withering!",ch,NULL,victim,TO_NOTVICT);
          act("$n gazes evilly at $N, and $S arm begins withering!",ch,NULL,victim,TO_VICT);
          make_part(victim,"arm");
          if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
          {
              if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)
                  take_item(victim,obj);
          }
          if ((obj = get_eq_char( victim, WEAR_WIELD )) != NULL)
              take_item(victim,obj);
          if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
              take_item(victim,obj);
          if ((obj = get_eq_char( victim, WEAR_WRIST_L )) != NULL)
              take_item(victim,obj);
          if ((obj = get_eq_char( victim, WEAR_FINGER_L )) != NULL)
              take_item(victim,obj);
          return;
        }
      }
  return;
}             
         
/*
 * Ahroun Gifts
 */
          
void do_razorclaws(CHAR_DATA *ch, char *argument)
{
              
        if ( ch->power[DISC_WERE_WOLF] < 4 )
        {
            stc("Huh?\n\r",ch);
            return;
        }
           
   if ( !IS_VAMPAFF(ch, VAM_CLAWS) )
   {
        stc("Your talons aren't even extended!\n\r",ch);
        return;
   }
          
   if ( IS_GAR1(ch, WOLF_RAZORCLAWS ) )
   {
      stc("Your claws lose their razor-sharp edge.\n\r",ch);
      act("$n's claws lose their razor-sharp edge.",ch,NULL,NULL,TO_ROOM);
      REMOVE_BIT(ch->garou1, WOLF_RAZORCLAWS);
   }
   else   
   {       
        send_to_char("You hone your claws to a razor sharpness.\n\r",ch);
        act("$n hones $s claws to a razor sharpness.",ch, NULL, NULL, TO_ROOM);
        SET_BIT( ch->garou1, WOLF_RAZORCLAWS );
   }
   return;
}

