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

/*
 * Local functions.
 */

void do_lloth( CHAR_DATA *ch, char *argument )
{
  char buf[MAX_STRING_LENGTH];
  CHAR_DATA *gch;
  
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_DROW) && !IS_CLASS(ch, CLASS_DROID) )
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  send_to_char("#p          -*-  The Church of Lloth  -*-#n\n\r",ch);
  send_to_char("#7[#pName                #7] [#p  Hp   #7] [#p Mana  #7] [#p Move  #7] [#p Gen  #7 ]#n\n\r",ch);
  for ( gch = char_list; gch != NULL; gch = gch->next)
  {
    if (IS_NPC(gch)) continue;
    if (!IS_CLASS(gch, CLASS_DROW) && !IS_CLASS(gch, CLASS_DROID)) continue;
    sprintf( buf,"#7[%-20s] [%-7d] [%-7d] [%-7d] [%-7d]#n\n\r",
    capitalize( gch->name ), gch->hit, gch->mana, gch->move, gch->generation);
    send_to_char( buf, ch );
  }
  return;
}

void do_grant( CHAR_DATA *ch, char *argument )
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

    if (!IS_CLASS(ch, CLASS_DROW))
    {
	    send_to_char("Huh?\n\r",ch);
	    return;
    }

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
	{
	
		send_to_char( "Syntax: Grant <person> <power>\n\r", ch );
		send_to_char("#pDrowfire #0(#r2500#0)#n,  #pDarkness #0(#r7500#0)#n,  #pDrowpoison #0(#r2500#0)#n.\n\r",ch);
		send_to_char("#pDrowsight #0(#r5000#0)#n,  #pDrowshield #0(#r5000#0)#n,  #pLevitation #0(#r1000#0)#n.\n\r", ch );
		send_to_char("#pShadowwalk #0(#r10000#0)#n,  #pGarotte #0(#r5000#0)#n,  #pSpiderarms #0(#r25000#0)#n.\n\r", ch );
		send_to_char("#pDrowhate #0(#r20000#0)#n,  #pSpiderform #0(#r25000#0)#n,  #pWeb #0(#r5000#0)#n.\n\r",ch );
		send_to_char("#pDgarotte #0(#r2500#0)#n,  #pConfuse #0(#r2500#0)#n,  #pGlamour #0(#r5000#0)#n.\n\r", ch );
		send_to_char("#pEarthshatter #0(#r7500#0)#n,  #pSpeed #0(#r7500#0)#n,  #pToughskin #0(#r7500#0)#n.\n\r", ch );
		send_to_char("#pDarktendrils #0(#r25000#0)#n., #pFightdance#0 (#r10000#0)#n\n\r", ch );
		send_to_char("Warrior (0), Mage (0), Cleric (0).\n\r", ch );
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

        if (!IS_CLASS(victim, CLASS_DROW)) {
                send_to_char("Only on drows.\n\r", ch );
                return;}


    if (!str_cmp(arg2,"mage") || !str_cmp(arg2,"warrior") || !str_cmp(arg2, "cleric" ))
    {
	if (victim->generation < 3 )
	{	send_to_char("Not on them!\n\r", ch );
		return;}

	if (( IS_SET(victim->special, SPC_DROW_WAR) ||
	IS_SET(victim->special, SPC_DROW_MAG) ||
	IS_SET(victim->special, SPC_DROW_CLE)) && ch->level < 6 && ch->generation > 2)
  {		send_to_char( "They already have a profession.\n\r", ch );
		return;}
	if (!str_cmp(arg2, "mage"))
		SET_BIT(victim->special, SPC_DROW_MAG);
	else if (!str_cmp(arg2, "cleric"))
		SET_BIT(victim->special, SPC_DROW_CLE);
	else if (!str_cmp(arg2, "warrior"))
		SET_BIT(victim->special, SPC_DROW_WAR);
	send_to_char("You give them a profession.\n\r", ch );
	send_to_char("You have been given a profession.\n\r", victim);
	save_char_obj(victim);
	return;
    }


    if (!str_cmp(arg2,"drowfire")) 
	{inpart = DPOWER_DROWFIRE; cost = 2500;}
    else if (!str_cmp(arg2,"darkness"))
	{inpart = DPOWER_DARKNESS; cost= 7500;}
    else if (!str_cmp(arg2,"drowsight"))
	{inpart = DPOWER_DROWSIGHT; cost= 5000;}
    else if (!str_cmp(arg2,"spiderarms"))
	{inpart = DPOWER_ARMS; cost = 25000;}
    else if (!str_cmp(arg2,"web"))
	{inpart = DPOWER_WEB; cost = 5000;}
    else if (!str_cmp(arg2,"spiderform"))
	{inpart = DPOWER_SPIDERFORM; cost = 25000;}
    else if (!str_cmp(arg2,"drowhate"))
	{inpart = DPOWER_DROWHATE; cost = 20000;}
    else if (!str_cmp(arg2,"drowshield"))
    {inpart = DPOWER_DROWSHIELD; cost = 5000;}
    else if (!str_cmp(arg2,"levitation"))
	{inpart = DPOWER_LEVITATION; cost = 1000;}
    else if (!str_cmp(arg2,"shadowwalk"))
	{inpart = DPOWER_SHADOWWALK; cost = 10000;}
    else if (!str_cmp(arg2,"garotte")) 
	{inpart = DPOWER_GAROTTE; cost = 5000;}
    else if (!str_cmp(arg2,"dgarotte"))
	{inpart = DPOWER_DGAROTTE; cost = 2500;}
    else if (!str_cmp(arg2,"drowpoison"))
	{inpart = DPOWER_DROWPOISON; cost = 2500;}
    else if (!str_cmp(arg2,"glamour"))
	{inpart = DPOWER_GLAMOUR; cost = 5000;}
    else if (!str_cmp(arg2,"confuse"))
	{inpart = DPOWER_CONFUSE; cost = 2500;}
    else if (!str_cmp(arg2,"earthshatter"))
	{inpart = DPOWER_EARTHSHATTER; cost = 7500;}
    else if (!str_cmp(arg2,"speed"))
	{inpart = DPOWER_SPEED; cost = 7500;}
    else if (!str_cmp(arg2,"toughskin"))
	{inpart = DPOWER_TOUGHSKIN; cost =7500;}
	else if (!str_cmp(arg2,"darktendrils"))
	{inpart = DPOWER_DARKTENDRILS; cost =25000;}
	else if (!str_cmp(arg2,"fightdance"))
	{inpart = DPOWER_FIGHTDANCE; cost =10000;}
    else
	{
               send_to_char( "Syntax: Grant <person> <power>\n\r", ch );
                send_to_char("#pDrowfire #0(#r2500#0)#n,  #pDarkness #0(#r7500#0)#n,  #pDrowpoison #0(#r2500#0)#n.\n\r",ch);
                send_to_char("#pDrowsight #0(#r5000#0)#n,  #pDrowshield #0(#r5000#0)#n,  #pLevitation #0(#r1000#0)#n.\n\r", ch );
                send_to_char("#pShadowwalk #0(#r10000#0)#n,  #pGarotte #0(#r5000#0)#n,  #pSpiderarms #0(#r25000#0)#n.\n\r", ch );
                send_to_char("#pDrowhate #0(#r20000#0)#n,  #pSpiderform #0(#r25000#0)#n,  #pWeb #0(#r5000#0)#n.\n\r",ch );
                send_to_char("#pDgarotte #0(#r2500#0)#n,  #pConfuse #0(#r2500#0)#n,  #pGlamour #0(#r5000#0)#n.\n\r", ch );
                send_to_char("#pEarthshatter #0(#r7500#0)#n,  #pSpeed #0(#r7500#0)#n,  #pToughskin #0(#r7500#0)#n.\n\r", ch );
                send_to_char("#pDarktendrils #0(#r25000#0)#n., #pFightdance#0 (#r10000#0)#n\n\r", ch );
                send_to_char("Warrior (0), Mage (0), Cleric (0).\n\r", ch );
                return;

	}
    if (!(ch==victim)) cost *=5;

    if (IS_SET(victim->pcdata->powers[1], inpart))
    {
	send_to_char("They have already got that power.\n\r",ch);
	return;
    }

    if( !IS_CLASS(victim, CLASS_DROW) )
    {
	stc( "You can not grant powers to Non Drows.\n\r",ch);
	return;
    }

    if (ch->pcdata->stats[DROW_POWER] < cost) 
    {
	send_to_char("You have insufficient power to grant that gift.\n\r",ch);
	return;
    }
    SET_BIT(victim->pcdata->powers[1], inpart);
    ch->pcdata->stats[DROW_POWER]   -= cost;
    if (victim != ch) send_to_char("You have been granted a gift from your matron!\n\r",victim);
    send_to_char("Ok.\n\r",ch);
    if (victim != ch) save_char_obj(ch);
    save_char_obj(victim);
    return; 
}

void do_chaosblast(CHAR_DATA *ch, char *argument) {

    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    int sn;
    int level;
    int spelltype;

    argument = one_argument( argument, arg );
    if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_DROW) || (!IS_SET(ch->special,
	SPC_DROW_MAG) && ch->generation > 2)) {
		return;}

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
        if ((victim = ch->fighting) == NULL)
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
        if (ch->mana < 750) {
                send_to_char("You don't have enough mana.\n\r", ch);
                return;}

    if ( ( sn = skill_lookup( "chaos blast" ) ) < 0 ) return;
    spelltype = skill_table[sn].target;
    level = ch->spl[spelltype]/3;
        act("You concentrate your power on $N.",ch,NULL,victim,TO_CHAR);
        act("$n concentrates $s power on you.",ch,NULL,victim,TO_VICT);
    (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
    WAIT_STATE( ch, 12 );
        ch->mana = ch->mana - 750;
    return;
}


void do_drowcreate( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DROW))
    {
	    send_to_char("Huh?\n\r",ch);
	    return;
    }

    if (arg[0] == '\0')
    {
	send_to_char("Please specify what kind of equipment you want to create.\n\r", ch );
	send_to_char("Whip, Dagger, Ring, Amulet, Armor, Helmet,\n\r", ch);
	send_to_char("Leggings, Boots, Gauntlets, Sleeves,\n\r", ch );
	send_to_char("Belt, Bracer, Mask, Cloak.\n\r", ch);
	return;
    }
    if      (!str_cmp(arg,"whip"     )) vnum = 33061;
    else if (!str_cmp(arg,"cloak"   )) vnum = 33070;
    else if (!str_cmp(arg,"armor"    )) vnum = 33073;
    else if (!str_cmp(arg,"boots"   )) vnum = 33067;
    else if (!str_cmp(arg,"gauntlets")) vnum = 33068;
    else if (!str_cmp(arg,"sleeves")) vnum = 33069;
    else if (!str_cmp(arg,"belt"))   vnum = 33062;
    else if (!str_cmp(arg,"helmet"))	vnum = 33065;
    else if (!str_cmp(arg,"leggings"))	vnum = 33066;
    else if (!str_cmp(arg,"mask" )) vnum = 33072;
    else if (!str_cmp(arg,"bracer"    )) vnum = 33071;
    else if (!str_cmp(arg,"dagger"	)) vnum = 33060;
    else if (!str_cmp(arg, "ring"))	vnum = 33063;
    else if (!str_cmp(arg, "amulet"))	vnum = 33064;
    else
    {
	send_to_char("That is an invalid type.\n\r", ch );
	return;
    }

    
    if ( ch->practice < 60)
    {
	send_to_char("It costs 60 points of primal to create a piece of drow armour.\n\r",ch);
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
    act("$p appears in your hands.",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
    return;
}

void do_drowfire( CHAR_DATA *ch, char *argument ) {

    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    int sn;
    int level;
    int spelltype;

	argument = one_argument(argument, arg);

	if (IS_NPC(ch)) return;

	if(!IS_CLASS(ch, CLASS_DROW) ||
!IS_SET(ch->pcdata->powers[1],DPOWER_DROWFIRE)) {
		send_to_char("Huh?\n\r", ch );
		return;}

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( ch == victim )
    {
        send_to_char( "Not on yourself.\n\r", ch );
        return;
    }

	if (ch->mana < 100) {
	send_to_char("You don't have enough mana.\n\r", ch );
		return;}

    if ( ( sn = skill_lookup( "drowfire" ) ) < 0 ) return;
    spelltype = skill_table[sn].target;
    level = ch->spl[spelltype] * 1.5;
    (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
    WAIT_STATE( ch, 12 );
	ch->mana = ch->mana - 100;
    return;
}
void do_heal ( CHAR_DATA *ch, char *argument ) 
{
        if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_DROW) || (!IS_SET(ch->special,SPC_DROW_CLE) && ch->generation > 2)) 
            {
		send_to_char("Huh?\n\r", ch );
		return;
            }

	if (ch->mana < 750) {
		send_to_char("You don't have enough mana.\n\r", ch );
		return;}

	ch->mana = ch->mana - 750;
	ch->hit = ch->hit + ch->spl[BLUE_MAGIC]*3;
	if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
	send_to_char("Lloth heals you.\n\r",ch);
	act("$n is healed by Lloth.",ch,NULL,NULL,TO_ROOM);
	WAIT_STATE(ch, 12);
	return;
}

void do_shadowwalk(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	ROOM_INDEX_DATA *location;
	CHAR_DATA *victim;

	one_argument (argument, arg);

	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_DROW) && !IS_CLASS(ch,CLASS_DROID))
        {
	send_to_char("Huh?\n\r", ch );
	return;}

    if (!IS_SET(ch->pcdata->powers[1],DPOWER_SHADOWWALK) && IS_CLASS(ch,CLASS_DROW))
        {
        send_to_char("Huh?\n\r", ch );
        return;}

	if ((victim = get_char_world(ch, arg)) == NULL) {
	send_to_char("Shadowwalk to whom?\n\r", ch );
	return;}

 if( IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
  { 
    stc( "You can't find it's room.\n\r",ch);
    return; 
  } 

//  if( IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
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

	if (ch->move < 250) {
	send_to_char("You are too tired to go shadowwalking.\n\r", ch );
	return;}

/*	if (!IS_NPC(victim)) {
	send_to_char("You failed.\n\r", ch );
	return;}

	if (!IS_SET(victim->in_room->room_flags, ROOM_DARK)) {
	send_to_char("You failed.\n\r", ch );
	return;}*/

	act("You walk into the shadows.", ch, NULL, NULL, TO_CHAR);
	act("$n walks into the shadows.", ch, NULL, NULL, TO_ROOM);
	ch->move -= 250;
	char_from_room(ch);
	char_to_room(ch, location);
	do_look(ch, "auto");	
	act("You walk out of the shadows.", ch, NULL, NULL, TO_CHAR);
	act("$n walks out of the shadows.", ch, NULL, NULL, TO_ROOM);

	return;
}

void do_drowhate(CHAR_DATA *ch, char *argument)
{
	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_DROWHATE))
	{send_to_char("Huh?\n\r", ch );
	return;}

	if (IS_SET(ch->newbits, NEW_DROWHATE)) {
	send_to_char("You calm your hatred.\n\r", ch );
	REMOVE_BIT(ch->newbits, NEW_DROWHATE);
	}
	else {
	send_to_char("You invoke your hatred for others.\n\r", ch );
	SET_BIT(ch->newbits, NEW_DROWHATE);
	}

	return;
}

void do_darktendrils(CHAR_DATA *ch, char *argument)
{
	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_DARKTENDRILS))
	{send_to_char("Huh?\n\r", ch );
	return;}

	if (IS_SET(ch->newbits, NEW_DARKTENDRILS)) {
	send_to_char("Your tendrils of darkness disappear.\n\r", ch );
	REMOVE_BIT(ch->newbits, NEW_DARKTENDRILS);
	}
	else {
	send_to_char("You call forth dark tendrils to attack your foes.\n\r", ch );
	SET_BIT(ch->newbits, NEW_DARKTENDRILS);
	}

	return;
}

void do_fightdance(CHAR_DATA *ch, char *argument)
{
	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_FIGHTDANCE))
	{send_to_char("Huh?\n\r", ch );
	return;}

	if (IS_SET(ch->newbits, NEW_FIGHTDANCE)) {
	send_to_char("You will no longer dance The Dance of Lloth.\n\r", ch );
	REMOVE_BIT(ch->newbits, NEW_FIGHTDANCE);
	}
	else {
	send_to_char("You will start dancing The Dance of Lloth while fighting.\n\r", ch );
	SET_BIT(ch->newbits, NEW_FIGHTDANCE);
	}

	return;
}

void do_spiderform( CHAR_DATA *ch, char *argument ) 
{
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
 
	argument = one_argument(argument,arg);

	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_DROW) ||!IS_SET(ch->pcdata->powers[1], DPOWER_SPIDERFORM))
	{send_to_char("Huh?\n\r", ch );
	return;}

	if (IS_AFFECTED(ch, AFF_POLYMORPH)) 
      {
	send_to_char("You can't spiderform while changed.\n\r", ch );
	return;
      }

      if (IS_SET(ch->newbits,NEW_DFORM))
      {
	sprintf(buf, "$n morphs back into %s.", GET_PROPER_NAME(ch));
        act(buf, ch, NULL, NULL, TO_ROOM);
        stc("You return to your normal form.\n\r", ch);
        free_string(ch->morph);
        ch->morph = str_dup("");
        ch->hitroll -= 400;
        ch->damroll -= 400;
        ch->armor   += 1000;
        REMOVE_BIT(ch->newbits,NEW_DFORM);
	REMOVE_BIT(ch->newbits,THIRD_HAND);
	REMOVE_BIT(ch->newbits,FOURTH_HAND);
        WAIT_STATE(ch, 7);
        return;   
      }

      else if (!IS_SET(ch->newbits,NEW_DFORM))
      {
	
	act("You mutate into a giant spider.",ch,NULL,NULL,TO_CHAR);
        act("$n mutates into a giant spider.",ch,NULL,NULL,TO_ROOM);
        sprintf(buf,"%s the giant mylochar",ch->name);
        free_string(ch->morph);
        ch->morph = str_dup(buf);
        ch->hitroll += 400;
        ch->damroll += 400;
        ch->armor   -= 1000;
        SET_BIT(ch->newbits,NEW_DFORM);
	SET_BIT(ch->newbits,THIRD_HAND);
	SET_BIT(ch->newbits,FOURTH_HAND);
	return;
    }

	return;
}


void do_drowsight(CHAR_DATA *ch, char *argument)
{
	if (IS_NPC(ch)) return;
	
	if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1],DPOWER_DROWSIGHT)) {
	send_to_char("Huh?\n\r", ch );
	return;}

    if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
    {   
        REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
        send_to_char( "Your senses return to normal.\n\r", ch );
    }
    else
    {
        SET_BIT(ch->act, PLR_HOLYLIGHT);
        send_to_char( "Your senses increase to incredible proportions.\n\r", ch);
    }

	return;
}

void do_drowshield(CHAR_DATA *ch, char *argument)
{

	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_DROWSHIELD)) {
	send_to_char("Huh?\n\r", ch );
	return;}

    if (!IS_IMMUNE(ch,IMM_SHIELDED) )
    {
        send_to_char("You shield your aura from those around you.\n\r",ch);
        SET_BIT(ch->immune, IMM_SHIELDED);
        return;
    }
    send_to_char("You stop shielding your aura.\n\r",ch);
    REMOVE_BIT(ch->immune, IMM_SHIELDED);


	return;
}


void do_drowpowers(CHAR_DATA *ch,char *argument)
{
	char buf[MAX_STRING_LENGTH];

	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_DROW))  {
	send_to_char("Huh?\n\r", ch );
	return;}

	send_to_char("#6The powers Lloth has bestowed upon you...#n\n\r", ch );
	send_to_char("#4-----------------------------------------#n\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_SPEED))
	send_to_char("You move quite quickly.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_TOUGHSKIN))
	send_to_char("Your skin has been toughened.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_DROWFIRE))
	send_to_char("You can call Drowfire upon your enemies.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_DARKNESS))
	send_to_char("You can summon a globe of darkness.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_DROWSIGHT))
	send_to_char("You can use enhanced sight.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_LEVITATION))
	send_to_char("You can levitate in the air.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_DROWSHIELD))
	send_to_char("You can shield your aura.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_DROWPOISON))
	send_to_char("You have control over deadly poisons.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_SHADOWWALK))
	send_to_char("You can walk through the shadows.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_GAROTTE)) 
	send_to_char("You have mastered the art of garotte.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_DGAROTTE))
	send_to_char("You have dark-enhanced garotte.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_ARMS))
	send_to_char("You have extra spidery arms.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_DROWHATE))
	send_to_char("You can invoke the hatred of the Drow.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_SPIDERFORM))
	send_to_char("You can mutate into a large spider.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_WEB))
	send_to_char("You can shoot a spidery web at opponents.\n\r", ch);

	if (IS_SET(ch->pcdata->powers[1], DPOWER_GLAMOUR))
	send_to_char("You can glamourosly change the appearance of items.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_CONFUSE))
	send_to_char("You can confuse your enemies during battle.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_EARTHSHATTER))
	send_to_char("You can shatter the earth under your feet.\n\r", ch );

	if (IS_SET(ch->pcdata->powers[1], DPOWER_DARKTENDRILS))
	send_to_char("You call forth Tendrils of Darkness to aid battle.\n\r", ch );
	
	if (IS_SET(ch->pcdata->powers[1], DPOWER_FIGHTDANCE))
	send_to_char("You will fight The Dance of Lloth when engaged in battle.\n\r", ch );
	
	send_to_char("\n\r", ch);
	
	if (IS_SET(ch->special, SPC_DROW_WAR))
	send_to_char("You are a Drow Warrior.\n\r", ch);

	if (IS_SET(ch->special, SPC_DROW_MAG))
	send_to_char("You are a Drow Mage.\n\r", ch );

	if (IS_SET(ch->special, SPC_DROW_CLE))
	send_to_char("You are a Drow Cleric.\n\r", ch );

        sprintf( buf, "You have %d drow power points!.\n\r",
        ch->pcdata->stats[DROW_POWER] );
        send_to_char( buf, ch );

        sprintf( buf, "You have %d points of magic resistance.\n\r", 
        ch->pcdata->stats[DROW_MAGIC] );
        send_to_char( buf, ch );

        if (weather_info.sunlight == SUN_DARK)
        send_to_char("You feel strong in the night.\n\r", ch );

	send_to_char("\n\r", ch );
	
	return;
}


void do_darkness(CHAR_DATA *ch, char *argument) 
{
	bool blah = FALSE;

	if (IS_NPC(ch)) return;

	if ((!IS_CLASS(ch, CLASS_DROW) && !IS_CLASS(ch, CLASS_DROID))
        || (!IS_SET(ch->pcdata->powers[1], DPOWER_DARKNESS) && !IS_CLASS(ch, CLASS_DROID)))
        {
	do_vampdarkness(ch,argument);
	return;}

	if (ch->fight_timer>0) {
	  send_to_char("Not until your fighttimer expires.\n\r",ch);
	  return;
	}

	if (IS_SET(ch->newbits, NEW_DARKNESS)) {
	send_to_char("You banish your globe of darkness.\n\r", ch );
	act("The globe of darkness around $n disappears.",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->newbits, NEW_DARKNESS);
	if (ch->in_room != NULL)
	if (IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS))
	REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
	return;
	}

	if (ch->mana < 500) {
	send_to_char("You don't have enough mana to summon the darkness.\n\r",ch );
	return;}

	send_to_char("You summon a globe of darkness.\n\r", ch );
	act("$n summons a globe of darkness.",ch,NULL,NULL,TO_ROOM);
	ch->mana -= 500;
	if (IS_SET(ch->extra, TIED_UP)) {
		REMOVE_BIT(ch->extra, TIED_UP);
		blah = TRUE;}

	if (IS_AFFECTED(ch, AFF_WEBBED)) {
		REMOVE_BIT(ch->affected_by, AFF_WEBBED);
		blah = TRUE;}

	if (blah) send_to_char("The darkness sets you free.\n\r", ch );		

	SET_BIT(ch->newbits, NEW_DARKNESS);
	if (!IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS))
	SET_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
	return;
}

void do_glamour(CHAR_DATA *ch, char *argument)
{

	char arg1[MAX_STRING_LENGTH];
	char arg2[MAX_STRING_LENGTH];
	char arg3[MAX_STRING_LENGTH];
	OBJ_DATA *obj;

	argument=one_argument(argument,arg1);
	argument=one_argument(argument,arg2);
	strcpy(arg3,argument);

	if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_DROW))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
	if (IS_CLASS(ch,CLASS_DROW) && !IS_SET(ch->pcdata->powers[1],DPOWER_GLAMOUR)) {
	send_to_char("Huh?\n\r", ch );
	return;}


	if ((obj=get_obj_carry(ch,arg1)) == NULL)
	{send_to_char("You dont have that item.\n\r",ch);
	return;}

	if (!(!str_cmp(arg2,"short") || !str_cmp(arg2,"name")))
        {send_to_char("syntax : glamour <item> <short/name> <newname>.\n\r",ch);
        return;}

	if (strlen(arg3) > 60 || strlen(arg3) < 3)
	{send_to_char("From 3 to 60 characters please.\n\r",ch);
	return;}

  if (has_bad_chars(ch, arg3))
  {
    send_to_char("Illegal chars, please retry.\n\r",ch);
    return;
  }

  if (IS_SET(obj->quest, QUEST_ARTIFACT) || IS_SET(obj->quest, QUEST_PRIZE))
  {
    send_to_char("Not on artifacts and prizes.\n\r",ch);
    return;
  }

	if (!str_cmp(arg2,"name"))
        {free_string(obj->name);
	obj->name = str_dup(arg3);
	obj->questmaker = str_dup(ch->name);
	}
	if (!str_cmp(arg2,"short"))
	{free_string(obj->short_descr);
	obj->short_descr=str_dup(arg3);
	obj->questmaker = str_dup(ch->name);
	}
	send_to_char("Ok.\n\r",ch);

	return;
}


void do_confuse(CHAR_DATA *ch, char *argument) {

	CHAR_DATA *victim;

	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_CONFUSE)) {
	send_to_char("Huh?\n\r",  ch);
	return;}

	if ((victim = ch->fighting) == NULL) {
	send_to_char("You are not fighting anyone.\n\r",  ch);
	return;}

	if (ch->move < 75) {
	send_to_char("You need 75 move to confuse your opponent.\n\r",ch);
	return;}

	act("$n attempts to confuse you.",ch,NULL,victim,TO_VICT);
	act("You attempt to confuse $N.",ch,NULL,victim,TO_CHAR);
	act("$n attempts to confuse $N.",ch,NULL,victim,TO_NOTVICT);

	ch->move -=75;

	if ( number_percent() > 25 ) {
	send_to_char("You failed.\n\r", ch );
	return;}

	else {
		do_flee(victim,"");	
	WAIT_STATE(ch, 16);
	return;	}

	return;
}


void do_earthshatter( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int dam;
    int level;
    

	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_EARTHSHATTER)) {
	send_to_char("Huh?\n\r", ch );
	return;}

	if (ch->mana < 150) {
	send_to_char("You need more mana.\n\r", ch );
	return;}

	level = ch->spl[PURPLE_MAGIC];
	ch->mana -= 150;

	send_to_char("You summon the power of the underworld, shattering the earth.\n\r", ch );
	act("$n causes the earth to shatter",ch,NULL,NULL,TO_ROOM);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
        vch_next = vch->next_in_room;

	if (vch == ch) continue;
        if (vch->trust>6) continue;
            dam  = dice(level, 7 );
            if ( saves_spell( level, vch ) )
                dam /= 2;
            damage( ch, vch, dam, skill_lookup("earthquake"));
    }
	WAIT_STATE(ch, 12);
    return;
}


