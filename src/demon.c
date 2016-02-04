/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'licensedoc' as well the Merc       *
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

/*
 * Local functions.
 */

void do_notravel( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (IS_IMMUNE(ch, IMM_TRAVEL))
    {
	REMOVE_BIT(ch->immune, IMM_TRAVEL);
	send_to_char("You can now be the target of travel or burrow.\n\r",
ch);
	return;
    }
    else if (!IS_IMMUNE(ch, IMM_TRAVEL)) 
    {
	SET_BIT(ch->immune, IMM_TRAVEL);
	send_to_char("You can no longer be the target of travel or burrow.\n\r", ch);
	return;
    }
    else
	send_to_char( "Excuse you?\n\r", ch );
    return;
}



void do_warps( CHAR_DATA *ch, char *argument )
{
    char lin   [MAX_STRING_LENGTH];
 
 
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DEMON))
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }
    sprintf( lin,
"-=(**)=-=(**)=-=(**)=-=(**)=-=(**)=-=(**)=-=(**)=-=(**)=-=(**)=-=(**)=-=(**)=-\n\r");
    send_to_char( lin,ch);
    send_to_char(
"                      -=(**)=- Attained Warps -=(**)=-\n\r",ch);
send_to_char(lin,ch);
if (IS_SET(ch->warp, WARP_CBODY      )) send_to_char("Your body is protected by a indestructable crystal shell.\n\r",ch);
if (IS_SET(ch->warp, WARP_SBODY      )) send_to_char("Your skin is as hard as steel.\n\r",ch);
if (IS_SET(ch->warp, WARP_STRONGARMS )) send_to_char("Your arms are incredibly strong.\n\r",ch);
if (IS_SET(ch->warp, WARP_STRONGLEGS )) send_to_char("Your legs are incredibly strong.\n\r",ch);
if (IS_SET(ch->warp, WARP_VENOMTONG  )) send_to_char("Your tongue is long and venomous.\n\r",ch);
if (IS_SET(ch->warp, WARP_SPIKETAIL  )) send_to_char("Your tail fires deadly spikes during combat.\n\r",ch);
if (IS_SET(ch->warp, WARP_BADBREATH  )) send_to_char("Your breath is putrid and deadly.\n\r",ch);
if (IS_SET(ch->warp, WARP_QUICKNESS  )) send_to_char("You have incredible speed.\n\r",ch);
if (IS_SET(ch->warp, WARP_STAMINA    )) send_to_char("You have increased stamina, reducing the damage you take.\n\r",ch);
if (IS_SET(ch->warp, WARP_HUNT       )) send_to_char("Your heightened senses enable you to hunt people.\n\r",ch);
if (IS_SET(ch->warp, WARP_DEVOUR     )) send_to_char("You have the ability to devour your opponents, sending them into the pits of Hell.\n\r",ch);
if (IS_SET(ch->warp, WARP_TERROR     )) send_to_char("Your features are so horrid that they may stun those who look at you.\n\r",ch);
if (IS_SET(ch->warp, WARP_REGENERATE )) send_to_char("Your body has the ability to regenerate incredibly fast.\n\r",ch);
if (IS_SET(ch->warp, WARP_STEED      )) send_to_char("Your mounts transform into hideous Demons.\n\r",ch);
if (IS_SET(ch->warp, WARP_WEAPON     )) send_to_char("You have the power to transform into a deadly battle axe.\n\r",ch);
if (IS_SET(ch->warp, WARP_INFIRMITY  )) send_to_char("Your body has been afflicted by a terrible infirmity.\n\r",ch);
if (IS_SET(ch->warp, WARP_GBODY      )) send_to_char("Your skin is made of a fragile glass.\n\r",ch);
if (IS_SET(ch->warp, WARP_SCARED     )) send_to_char("You are incredibly scared of combat.\n\r",ch);
if (IS_SET(ch->warp, WARP_MAGMA      )) send_to_char("Your body is composed of deadly magma.\n\r",ch);
if (IS_SET(ch->warp, WARP_WEAK       )) send_to_char("Your muscles are severely weakened.\n\r",ch);
if (IS_SET(ch->warp, WARP_SLOW       )) send_to_char("Your body moves very slowly.\n\r",ch);
if (IS_SET(ch->warp, WARP_VULNER     )) send_to_char("Your skin is very vulnerable to magic.\n\r",ch);
if (IS_SET(ch->warp, WARP_SHARDS     )) send_to_char("Your skin is covered with shards of ice.\n\r",ch);
if (IS_SET(ch->warp, WARP_WINGS      )) send_to_char("A pair of leathery wings protrude from your back.\n\r",ch);
if (IS_SET(ch->warp, WARP_CLUMSY     )) send_to_char("You are incredibly clumsy, enabling you to be disarmed.\n\r",ch);
if (IS_SET(ch->warp, WARP_STUPID     )) send_to_char("Your intelligence is extremely low, preventing you from casting spells.\n\r",ch);
if (IS_SET(ch->warp, WARP_SPOON      )) send_to_char("There is a spoon stuck on your ear.\n\r",ch);
if (IS_SET(ch->warp, WARP_FORK       )) send_to_char("You have a fork stuck in your nose.\n\r",ch);
if (IS_SET(ch->warp, WARP_KNIFE      )) send_to_char("You have a knife hanging out of your ear.\n\r",ch);
if (IS_SET(ch->warp, WARP_SALADBOWL  )) send_to_char("Your head is made out of a salad bowl.\n\r",ch);
if (ch->warp < 1) send_to_char("You haven't obtained any warp powers.\n\r",ch);
send_to_char(lin,ch);
return;
}


void do_obtain( CHAR_DATA *ch, char *argument )
{
    int       newwarp = 0;
    int       warpnum = number_range(1,18);

    if (IS_NPC(ch)) return;

	if ( !IS_CLASS(ch, CLASS_DEMON))
	{
	send_to_char("Huh?\n\r", ch);
	return;
	}

    if (ch->pcdata->stats[DEMON_TOTAL] < 15000 || 
	ch->pcdata->stats[DEMON_CURRENT] < 15000)
    {
	send_to_char("You need 15000 demon points to obtain a new warp!\n\r",ch);
	return;
    }

        if (ch->warpcount >= 18)
        {
	    send_to_char("You have already obtained as many warps as possible.\n\r",ch);
	    return;
	}

             if (warpnum == 1) newwarp = WARP_CBODY;
        else if (warpnum == 2) newwarp = WARP_SBODY;
	else if (warpnum == 3) newwarp = WARP_STRONGARMS;
	else if (warpnum == 4) newwarp = WARP_STRONGLEGS;
	else if (warpnum == 5) newwarp = WARP_VENOMTONG;
	else if (warpnum == 6) newwarp = WARP_SPIKETAIL;
	else if (warpnum == 7) newwarp = WARP_BADBREATH;
	else if (warpnum == 8) newwarp = WARP_QUICKNESS;
	else if (warpnum == 9) newwarp = WARP_STAMINA;
	else if (warpnum == 10) newwarp = WARP_HUNT;
	else if (warpnum == 11) newwarp = WARP_DEVOUR;
	else if (warpnum == 12) newwarp = WARP_TERROR;
	else if (warpnum == 13) newwarp = WARP_REGENERATE;
	else if (warpnum == 14) newwarp = WARP_STEED;
	else if (warpnum == 15) newwarp = WARP_WEAPON;
	else if (warpnum == 16) newwarp = WARP_MAGMA;
	else if (warpnum == 17) newwarp = WARP_SHARDS;
	else if (warpnum == 18) newwarp = WARP_WINGS;

    if (IS_SET(ch->warp, newwarp))
    {
      do_obtain(ch,"");
	return;
    }

    SET_BIT(ch->warp, newwarp);
    ch->pcdata->stats[DEMON_CURRENT] -= 15000;
    ch->warpcount += 1;
    send_to_char("You have obtained a new warp!\n\r",ch);
    save_char_obj(ch);
    return;
}

void do_inpart( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    int       inpart = 0;
    int       cost = 0;

    smash_tilde(argument);
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

	if ( !IS_CLASS(ch, CLASS_DEMON))
	{
	send_to_char("Huh?\n\r", ch);
	return;
	}

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: Inpart <person> <power>\n\r", ch );
	send_to_char(" Fangs        [ 2500] Claws        [ 2500] Tail        [ 5000]\n\r",ch);
        send_to_char(" Horns        [ 2500] Hooves       [ 1500] Graft       [20000]\n\r",ch);
	send_to_char(" NightSight   [ 3000] Wings        [ 1000] Might       [ 7500]\n\r",ch);
        send_to_char(" Toughness    [ 7500] Speed        [ 7500] Travel      [ 1500]\n\r",ch);
	send_to_char(" Scry         [ 7500] Truesight    [ 7500] Move        [  500]\n\r",ch);
	send_to_char(" Leap         [  500] Magic        [ 1000] Lifespan    [  100]\n\r",ch);
	send_to_char(" Shield       [20000] Longsword    [    0] Shortsword  [    0]\n\r",ch);
	send_to_char(" Immolate     [ 2500] Inferno      [20000] Caust       [ 3000]\n\r",ch);
	send_to_char(" Unnerve      [ 5000] Freezeweapon [ 3000] Entomb      [20000]\n\r",ch);
	send_to_char(" Leech	      [15000] Demonform    [25000] Blink       [15000]\n\r",ch);
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "Nobody by that name.\n\r", ch );
	return;
    }


    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }


   if (!IS_CLASS(victim, CLASS_DEMON))
  {
    send_to_char("Only on demons.\n\r",ch);
    return;
  }

    if (!str_cmp(arg2,"longsword"))
    {
	send_to_char("You have been granted the power to transform into a demonic longsword!\n\r",victim);
	send_to_char("You grant them the power to transform into a demonic longsword.\n\r",ch);
	victim->pcdata->powers[DPOWER_OBJ_VNUM] = 33120;
	save_char_obj(victim);
	return;
    }

    if (!str_cmp(arg2,"fangs")) 
	{inpart = DEM_FANGS; cost = 2500;}
    else if (!str_cmp(arg2,"immolate"))
	{inpart = DEM_IMMOLATE; cost = 2500;}
    else if (!str_cmp(arg2,"inferno"))
	{inpart = DEM_INFERNO; cost = 20000;}
    else if (!str_cmp(arg2,"caust"))
	{inpart = DEM_CAUST; cost = 3000;}
    else if (!str_cmp(arg2,"freezeweapon"))
	{inpart = DEM_FREEZEWEAPON; cost = 3000;}
    else if (!str_cmp(arg2,"unnerve"))
	{inpart = DEM_UNNERVE; cost = 5000;}
    else if (!str_cmp(arg2,"entomb"))
	{inpart = DEM_ENTOMB; cost = 20000;}
    else if (!str_cmp(arg2,"claws")) 
	{inpart = DEM_CLAWS; cost = 2500;}
    else if (!str_cmp(arg2,"horns")) 
	{inpart = DEM_HORNS; cost = 2500;}
   else if (!str_cmp(arg2, "demonform"))
	{inpart = DEM_FORM; cost = 25000;} 
   else if (!str_cmp(arg2,"tail")) 
	{inpart = DEM_TAIL; cost = 5000;}
    else if (!str_cmp(arg2,"hooves")) 
	{inpart = DEM_HOOVES; cost = 1500;}
    else if (!str_cmp(arg2,"shield")) 
	{inpart = DEM_SHIELD; cost = 20000;}
    else if (!str_cmp(arg2,"nightsight")) 
	{inpart = DEM_EYES; cost = 3000;}
    else if (!str_cmp(arg2,"wings")) 
	{inpart = DEM_WINGS; cost = 1000;}
    else if (!str_cmp(arg2,"might")) 
	{inpart = DEM_MIGHT; cost = 7500;}
    else if (!str_cmp(arg2,"toughness")) 
	{inpart = DEM_TOUGH; cost = 7500;}
    else if (!str_cmp(arg2,"speed"))
	{inpart = DEM_SPEED; cost = 7500;}
    else if (!str_cmp(arg2,"travel")) 
	{inpart = DEM_TRAVEL; cost = 1500;}
    else if (!str_cmp(arg2,"scry")) 
	{inpart = DEM_SCRY; cost = 7500;}
    else if (!str_cmp(arg2,"move")) 
	{inpart = DEM_MOVE; cost = 500;}
    else if (!str_cmp(arg2,"leap")) 
	{inpart = DEM_LEAP; cost = 500;}
    else if (!str_cmp(arg2,"magic")) 
	{inpart = DEM_MAGIC; cost = 1000;}
    else if (!str_cmp(arg2,"truesight"))
        {inpart = DEM_TRUESIGHT; cost = 7500;}
    else if (!str_cmp(arg2,"graft"))
	{inpart = DEM_GRAFT; cost = 20000;}
    else if (!str_cmp(arg2,"leech"))
	{inpart = DEM_LEECH; cost = 15000;}
    else if (!str_cmp(arg2,"blink"))
        {inpart = DEM_BLINK; cost = 15000;}
    else if (!str_cmp(arg2,"lifespan")) 
	{inpart = DEM_LIFESPAN; cost = 100;}
    else
    {
	do_inpart(ch,"");
	return;
    }

    if (!(victim==ch)) cost *= 25;

    if (IS_DEMPOWER(victim, inpart))
    {
	send_to_char("They have already got that power.\n\r",ch);
	return;
    }

    if( !IS_CLASS(victim, CLASS_DEMON))
    {
	stc( "You can not inpart this onto nonDemons.\n\r",ch);
	return;
    }

    if (ch->pcdata->stats[DEMON_TOTAL] < cost || 
	ch->pcdata->stats[DEMON_CURRENT] < cost)
    {
	send_to_char("You have insufficient power to inpart that gift.\n\r",ch);
	return;
    }

    if( !IS_CLASS(victim, CLASS_DEMON) && ch->practice < 100 )
    {
	stc( "Victim does not have 100 primal.\n\r",ch);
	return;
    }

    SET_BIT(victim->pcdata->powers[DPOWER_FLAGS], inpart);
    ch->pcdata->stats[DEMON_CURRENT] -= cost;
    if( !IS_CLASS(victim, CLASS_DEMON)) ch->practice -= 100;
    if (victim != ch) send_to_char("You have been granted a demonic gift from your patron!\n\r",victim);
    send_to_char("Ok.\n\r",ch);
    if (victim != ch) save_char_obj(ch);
    save_char_obj(victim);
    return;
}

void do_demonarmour( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("Huh?\n\r", ch );
	return;
    }
    if (arg[0] == '\0')
    {
	send_to_char("Please specify which piece of demon armor you wish to make: Ring Collar Plate Helmet Leggings Boots Gauntlets Sleeves Cape Belt Bracer Visor longsword shortsword.\n\r",ch);
	return;
    }
    else if (!str_cmp(arg,"ring"     ))   vnum = 33122;
    else if (!str_cmp(arg,"collar"   ))   vnum = 33123;
    else if (!str_cmp(arg,"plate"    ))   vnum = 33125;
    else if (!str_cmp(arg,"helmet"   ))   vnum = 33126;
    else if (!str_cmp(arg,"leggings" ))   vnum = 33127;
    else if (!str_cmp(arg,"boots"    ))   vnum = 33128;
    else if (!str_cmp(arg,"gauntlets"))   vnum = 33129;
    else if (!str_cmp(arg,"sleeves"  ))   vnum = 33130;
    else if (!str_cmp(arg,"cape"     ))   vnum = 33131;
    else if (!str_cmp(arg,"belt"     ))   vnum = 33132;
    else if (!str_cmp(arg,"bracer"   ))   vnum = 33124;
    else if (!str_cmp(arg,"visor"    ))   vnum = 33133;
    else if (!str_cmp(arg,"longsword"))   vnum = 33120;
    else if (!str_cmp(arg,"shortsword"))  vnum = 33121;
    else
    {
        do_demonarmour(ch,"");
	return;
    }
    if (ch->practice < 60)
    {
     send_to_char("It costs 60 points of primal to create a piece of demon armour.\n\r",ch);
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
    act("$p appears in your hands in a blast of flames.",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands in a blast of flames.",ch,obj,NULL,TO_ROOM);
    return;
}

void do_travel( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (IS_CLASS(ch, CLASS_DEMON))
    {
	if (!IS_DEMPOWER( ch, DEM_TRAVEL))
	{
	    send_to_char("You haven't been granted the gift of travel.\n\r",ch);
	    return;
	}
    }
    else
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "Nobody by that name.\n\r", ch );
	return;
    }

/*    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }
*/
    if (IS_IMMUNE(victim, IMM_TRAVEL))
    {
	send_to_char("I don't think they want you to do that.\n\r",ch);
	return;
    }   
if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_SUMMON)) 
{
        send_to_char("I don't think they want you to do that.\n\r",ch);
        return;
    }   
    if ( victim == ch ) {
		send_to_char( "Nothing happens.\n\r", ch);
		return; }

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
    if (victim->in_room == NULL)
    {
	send_to_char( "Nothing happens.\n\r", ch );
	return;
    }
    send_to_char("You sink into the ground.\n\r",ch);
    act("$n sinks into the ground.",ch,NULL,NULL,TO_ROOM);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    do_look(ch,"");
    send_to_char("You rise up out of the ground.\n\r",ch);
    act("$n rises up out of the ground.",ch,NULL,NULL,TO_ROOM);
    return;
}

void do_horns( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch,CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if( IS_CLASS(ch, CLASS_DEMON) && ch->power[DISC_DAEM_ATTA] < 4 )
    {
        if (!IS_DEMPOWER( ch, DEM_HORNS) && IS_CLASS(ch,CLASS_DEMON))
        {
	    send_to_char("You haven't been granted the gift of horns or attack is below level 4.\n\r",ch);
	    return;
        }
    }
    else if (IS_CLASS(ch,CLASS_VAMPIRE) && !IS_VAMPAFF(ch,VAM_VICISSITUDE) )
    {
	send_to_char("Use bonemod.\n\r", ch);
	return;
    }

    if (IS_DEMAFF(ch,DEM_HORNS) )
    {
	send_to_char("Your horns slide back into your head.\n\r",ch);
	act("$n's horns slide back into $s head.", ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_HORNS);
	return;
    }
    send_to_char("Your horns extend out of your head.\n\r",ch);
    act("A pair of pointed horns extend from $n's head.", ch, NULL, NULL, TO_ROOM);
    SET_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_HORNS);
    return;
}
void do_tail( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_DEMPOWER( ch, DEM_TAIL))
    {
	send_to_char("You haven't been granted the gift of a tail.\n\r",ch);
	return;
    }

    if (IS_DEMAFF(ch,DEM_TAIL) )
    {
	send_to_char("Your tail slides back into your back.\n\r",ch);
	act("$n's tail slides back into $s back.", ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_TAIL);
	return;
    }

    send_to_char("Your tail slides out of your back side.\n\r",ch);
    act("A huge tail extends from $n's back.", ch, NULL, NULL, TO_ROOM);
    SET_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_TAIL);
    return;
}

/*moved*/
void do_hooves( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_DEMPOWER( ch, DEM_HOOVES))
    {
	send_to_char("You haven't been granted the gift of horns.\n\r",ch);
	return;
    }

    if (IS_DEMAFF(ch,DEM_HOOVES) )
    {
	send_to_char("Your hooves transform into feet.\n\r",ch);
	act("$n's hooves transform back into $s feet.", ch, NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_HOOVES);
	return;
    }
    send_to_char("Your feet transform into hooves.\n\r",ch);
    act("$n's feet transform into hooves.", ch, NULL, NULL, TO_ROOM);
    SET_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_HOOVES);
    return;
}

void do_wings( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

   if (!IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }


    if (!IS_DEMPOWER( ch, DEM_WINGS) && ch->power[DISC_DAEM_ATTA] < 5 && IS_CLASS(ch, CLASS_DEMON) )
    {
	send_to_char("You haven't been granted the gift of wings.\n\r",ch);
	return;
    }

    if (arg[0] != '\0')
    {
	if (!IS_DEMAFF(ch,DEM_WINGS) )
	{
	    send_to_char("First you better get your wings out!\n\r",ch);
	    return;
	}
	if (!str_cmp(arg,"unfold") || !str_cmp(arg,"u"))
	{
	    if (IS_DEMAFF(ch,DEM_UNFOLDED) )
	    {
		send_to_char("But your wings are already unfolded!\n\r",ch);
		return;
	    }
	    send_to_char("Your wings unfold from behind your back.\n\r",ch);
	    act("$n's wings unfold from behind $s back.", ch, NULL, NULL, TO_ROOM);
	    SET_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_UNFOLDED);
	    return;
	}
	else if (!str_cmp(arg,"fold") || !str_cmp(arg,"f"))
	{
	    if (!IS_DEMAFF(ch,DEM_UNFOLDED) )
	    {
		send_to_char("But your wings are already folded!\n\r",ch);
		return;
	    }
	    send_to_char("Your wings fold up behind your back.\n\r",ch);
	    act("$n's wings fold up behind $s back.", ch, NULL, NULL, TO_ROOM);
	    REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_UNFOLDED);
	    return;
	}
	else
	{
	    send_to_char("Do you want to FOLD or UNFOLD your wings?\n\r",ch);
	    return;
	}
    }

    if (IS_DEMAFF(ch,DEM_WINGS) )
    {
	if (IS_DEMAFF(ch,DEM_UNFOLDED) )
	{
	    send_to_char("Your wings fold up behind your back.\n\r",ch);
	    act("$n's wings fold up behind $s back.", ch, NULL, NULL, TO_ROOM);
	    REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_UNFOLDED);
	}
	send_to_char("Your wings slide into your back.\n\r",ch);
	act("$n's wings slide into $s back.", ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_WINGS);
	return;
    }
    send_to_char("Your wings extend from your back.\n\r",ch);
    act("A pair of wings extend from $n's back.", ch, NULL, NULL, TO_ROOM);
    SET_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_WINGS);
    return;
}

void do_lifespan( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch,CLASS_DEMON))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (!IS_DEMPOWER( ch, DEM_LIFESPAN))
    {
	send_to_char("You haven't been granted the gift of lifespan.\n\r",ch);
	return;
    }

    if ( ( obj = ch->pcdata->chobj ) == NULL )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( obj->chobj == NULL || obj->chobj != ch )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (!IS_HEAD(ch,LOST_HEAD))
    {
	send_to_char("You cannot change your lifespan in this form.\n\r",ch);
	return;
    }

         if (!str_cmp(arg,"l") || !str_cmp(arg,"long" )) obj->timer = 0;
    else if (!str_cmp(arg,"s") || !str_cmp(arg,"short")) obj->timer = 1;
    else
    {
	send_to_char("Do you wish to have a long or short lifespan?\n\r",ch);
	return;
    }
    send_to_char("Ok.\n\r",ch);

    return;
}

void do_weaponform( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char( "Huh?\n\r", ch);
	return;
    }
    else if (IS_AFFECTED(ch,AFF_POLYMORPH))
    {
	send_to_char( "You cannot do this while polymorphed.\n\r", ch);
	return;
    }
   

    if (ch->pcdata->powers[DPOWER_OBJ_VNUM] < 1)
    {
	send_to_char( "You don't have the ability to change into a weapon.\n\r", ch);
	return;
    }
    if ((obj = create_object(get_obj_index(33120),60)) == NULL)
    {
	send_to_char( "You don't have the ability to change into a weapon.\n\r", ch);
	return;
    }
    if (IS_AFFECTED(ch,AFF_WEBBED))
    {
	send_to_char( "Not with all this sticky webbing on.\n\r", ch);
	return;
    }

    if (ch->fight_timer>0)
    {
       send_to_char("Not with a fighttimer.\n\r",ch);
       return;
    }

    obj_to_room(obj,ch->in_room);
    act("$n transforms into $p and falls to the ground.",ch,obj,NULL,TO_ROOM);
    act("You transform into $p and fall to the ground.",ch,obj,NULL,TO_CHAR);
    ch->pcdata->obj_vnum = 33120;
    obj->chobj = ch;
    ch->pcdata->chobj = obj;
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->extra, EXTRA_OSWITCH);
    free_string(ch->morph);
    ch->morph = str_dup(obj->short_descr);
    return;
}

void do_humanform( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;

    if (IS_NPC(ch)) return;
    if ( ( obj = ch->pcdata->chobj ) == NULL )
    {
	send_to_char("You are already in human form.\n\r",ch);
	return;
    }

    ch->pcdata->obj_vnum = 0;
    obj->chobj = NULL;
    ch->pcdata->chobj = NULL;
    REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
    REMOVE_BIT(ch->extra, EXTRA_OSWITCH);
    free_string(ch->morph);
    ch->morph = str_dup("");
    act("$p transforms into $n.",ch,obj,NULL,TO_ROOM);
    act("Your reform your human body.",ch,obj,NULL,TO_CHAR);
    extract_obj(obj);
    if (ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
    {
	char_from_room(ch);
	char_to_room(ch,get_room_index(ROOM_VNUM_HELL));
    }
    return;
}

void do_champions( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    sprintf( buf, "The champions of Hell:\n\r");
    send_to_char( buf, ch );
    send_to_char("[      Name      ] [ Hits ] [ Mana ] [ Move ] [       Power ]\n\r", ch );
    for ( gch = char_list; gch != NULL; gch = gch->next )
    {
	if ( IS_NPC(gch) ) continue;
	if ( !IS_CLASS(gch, CLASS_DEMON))  continue;
	    sprintf( buf,
	    "[%-16s] [%-6d] [%-6d] [%-6d] [ %-9d%9d ]\n\r",
		capitalize( gch->name ),
		gch->hit,gch->mana,gch->move,
		gch->pcdata->stats[DEMON_CURRENT], gch->pcdata->stats[DEMON_TOTAL]);
		send_to_char( buf, ch );
    }
    return;
}


void do_eyespy( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *familiar;

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch,CLASS_DEMON))
    {
      send_to_char("huh.\n\r",ch);
      return;
    }
    if (IS_HEAD(ch,LOST_EYE_L) && IS_HEAD(ch,LOST_EYE_R))
    {
	send_to_char( "But you don't have any more eyes to pluck out!\n\r", ch );
	return;
    }
    if (!IS_HEAD(ch,LOST_EYE_L) && number_range(1,2) == 1)
    {
	act( "You pluck out your left eyeball and throw it to the ground.", ch, NULL, NULL, TO_CHAR );
	act( "$n plucks out $s left eyeball and throws it to the ground.", ch, NULL, NULL, TO_ROOM );
    }
    else if (!IS_HEAD(ch,LOST_EYE_R))
    {
	act( "You pluck out your right eyeball and throw it to the ground.", ch, NULL, NULL, TO_CHAR );
	act( "$n plucks out $s right eyeball and throws it to the ground.", ch, NULL, NULL, TO_ROOM );
    }
    else
    {
	act( "You pluck out your left eyeball and throw it to the ground.", ch, NULL, NULL, TO_CHAR );
	act( "$n plucks out $s left eyeball and throws it to the ground.", ch, NULL, NULL, TO_ROOM );
    }
    if ( ( familiar = ch->pcdata->familiar ) != NULL ) 
    {
	make_part(ch,"eyeball");
	return;
    }

    victim = create_mobile( get_mob_index( MOB_VNUM_EYE ) );
    SET_BIT(victim->act, ACT_NOEXP);
    if (victim == NULL)
    {send_to_char("Error - please inform KaVir.\n\r",ch); return;}

    char_to_room( victim, ch->in_room );

    ch->pcdata->familiar = victim;
    victim->wizard = ch;
    return;
}


void do_cone( CHAR_DATA *ch, char *argument ) {
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    int sn;
    int level;
    int spelltype;

    argument = one_argument( argument, arg );
    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DEMON))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if ( arg[0] == '\0' )
    {
    send_to_char("Cone who?\n\r", ch);
    return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	if ((victim = ch->fighting) == NULL)
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
	if (ch->mana < 100) {
		send_to_char("You don't have enough mana.\n\r", ch);
		return;}

    if ( ( sn = skill_lookup( "cone" ) ) < 0 ) return;
    spelltype = skill_table[sn].target;
    level = ch->spl[spelltype] * 1.0;
	level = level * 1.0;
	act("You Blast $N with a cone of fire.",ch,NULL,victim,TO_CHAR);
	act("$n Blasts you with a cone of fire.",ch,NULL,victim,TO_VICT);
    (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
    WAIT_STATE( ch, 10 );
	ch->mana = ch->mana - 100;
    return;
}

void do_dstake( CHAR_DATA *ch, char *argument )

{
OBJ_DATA *obj;

if (IS_NPC(ch)) return;

if (!IS_CLASS(ch, CLASS_DEMON))
    {

send_to_char("Huh?\n\r",ch);
return;
}

if ( 60 > ch->practice)
{
send_to_char("It costs 60 points of primal to create a stake.\n\r",ch);
return;
}
ch->practice   -= 60;
obj = create_object(get_obj_index(OBJ_VNUM_STAKE)  ,0 );
if (IS_SET(obj->quest, QUEST_ARTIFACT))
REMOVE_BIT(obj->quest, QUEST_ARTIFACT);
obj_to_char(obj, ch);
act("A Stake appears in your hands in a flash of light.",ch,NULL,NULL,TO_CHAR);
act("A Stake appears in $n's hands in a flash of light.",ch,NULL,NULL,TO_ROOM);
return;
}
