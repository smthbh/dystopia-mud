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

int dambonus args((CHAR_DATA *ch, CHAR_DATA *victim, int dam, int stance ));

int find_dam( CHAR_DATA *ch, CHAR_DATA *victim, int dam, bool foot )
{
  OBJ_DATA *boots; 
  int stance;

  dam += char_damroll(ch);

  if (dam == 0) dam = 1;
  if (IS_AFFECTED(victim, AFF_SANCTUARY))
    dam = dam/2;
  if (!IS_AWAKE(victim))
    dam *= 2;
  if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_POTE] > 0) dam *= (ch->power[DISC_VAMP_POTE] * .4);
  else if (ch->power[DISC_DAEM_ATTA] > 0) dam *= ch->power[DISC_DAEM_ATTA] * .5;
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_MONK) )
  {
    if (ch->chi[CURRENT] > 0 && ch->chi[CURRENT] < 3) dam *= 1.2;
    if (ch->chi[CURRENT] > 2) dam *= ( ch->chi[CURRENT] / 2 );
  }
  if ( !IS_NPC(victim) && IS_CLASS(victim, CLASS_WEREWOLF) )
  {
    if (IS_SET(victim->special, SPC_WOLFMAN)) dam *= 0.3;
    if (victim->power[DISC_WERE_BOAR] > 2 ) dam *= 0.3;
    if (foot == TRUE)
    {
      if ((boots = get_eq_char( ch, WEAR_FEET )) != NULL && IS_SET(boots->spectype, SITEM_SILVER)) 
        dam *= 2;
    }
    if (foot == FALSE)
    {
      if ((boots = get_eq_char(ch, WEAR_HANDS)) != NULL && IS_SET(boots->spectype, SITEM_SILVER)) 
        dam *= 2;
    }
  }
  if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF))
  {
    if (ch->power[DISC_WERE_LUNA] > 6)
    {
      if (weather_info.sunlight == SUN_DARK)
        dam *= 1.6;
    }
  }    
  if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE) )
  {
    if (weather_info.sunlight == SUN_LIGHT && dam > 1) dam /= 1.5;
    else if (weather_info.sunlight == SUN_DARK) dam *= 1.5;
  }
  if ( !IS_NPC(ch) ) dam = dam + (dam * ((ch->wpn[0]+1) / 100));
  if ( !IS_NPC(ch) )
  {
    stance = ch->stance[0];
    if ( IS_STANCE(ch, STANCE_NORMAL) ) dam *= 1.25;
    else dam = dambonus(ch,victim,dam,stance);
  }
  if (!IS_NPC(victim) && dam > 1000) dam = number_range( 940, 1020 );
  if (dam <= 0) dam = 2;
  return dam;
}

void do_shinkick( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    bool foot = TRUE;
    int dam;
  
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch,CLASS_MONK))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( ( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  
  WAIT_STATE(ch, 12);
  dam = number_range(1,6);
  dam = find_dam( ch, victim, dam, foot );   

  if ( ch->monkcrap == 0 )
  {
    stc("Your aura glows light blue for a moment.\n\r",ch);
    act("$n's aura glows a light blue for a moment.",ch,NULL,victim,TO_VICT);
    SET_BIT(ch->monkcrap, COMB_SHIN);
  }
  else if ( ch->monkcrap > 0 )
  {
    stc("Your aura fades away.\n\r",ch);
    act("$n's aura fades away.",ch,NULL,victim,TO_VICT);
    ch->monkcrap = 0;
  }
  act("You deliver a powerful blow to $N's shin.",ch,NULL,victim,TO_CHAR);
  act("$n delivers a powerful blow to your shin.",ch,NULL,victim,TO_VICT);
  act("$n delivers a powerful blow to $N's shin.",ch,NULL,victim,TO_NOTVICT);
  damage( ch, victim, dam, gsn_shinkick );
  return;
}

void do_palmstrike( CHAR_DATA *ch, char *argument )
{
  char arg [MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  int dam;
  bool foot = FALSE;

  one_argument(argument,arg);

  if ( IS_NPC(ch) ) return;
  if (!IS_CLASS(ch,CLASS_MONK))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    send_to_char( "They aren't here.\n\r", ch );
    return;
  }

  if ( ch == victim )
  {
    send_to_char( "You cannot palm strike yourself!\n\r", ch );
    return;
  }

  if (is_safe(ch,victim)) return;

  if ( victim->position < POS_FIGHTING )
  {
    send_to_char( "You can only palm strike someone who is standing.\n\r", ch );
    return;
  }

  act("You slam your palm into $N's chest.",ch,NULL,victim,TO_CHAR);
  act("$n slams $s palm into your chest.",ch,NULL,victim,TO_VICT);
  act("$n slams $s palm into $N's chest.",ch,NULL,victim,TO_NOTVICT);

  WAIT_STATE(ch, 11);
  dam = number_range(1,4);

  find_dam( ch, victim, dam, foot );
  dam /= 2;

  damage(ch, victim, dam, gsn_palmstrike);
  if (victim == NULL || victim->position == POS_DEAD || dam < 1) return;
  if (victim->position == POS_FIGHTING) stop_fighting(victim,TRUE);
  act("You fall to the ground stunned!",victim,NULL,NULL,TO_CHAR);
  act("$n falls to the ground stunned!",victim,NULL,NULL,TO_ROOM);
  victim->position = POS_STUNNED;
  return;
}

void do_knee( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  bool foot = TRUE;
  int dam;
  
  if (!IS_CLASS(ch,CLASS_MONK))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( !IS_FS(ch, TECH_KNEE) )
  {
    stc("You haven't learned that technique.\n\r",ch);
    return;
  }

  if ( ( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }

  WAIT_STATE(ch, 11);
  dam = number_range(1,6);
  dam = find_dam( ch, victim, dam, foot );   

  if ( IS_SET(ch->monkcrap, COMB_SHIN) && !IS_SET(ch->monkcrap, COMB_KNEE ) )
  {
    stc("Your aura glows dark blue.\n\r",ch);
    act("$n's aura glows dark blue.",ch,NULL,victim,TO_VICT);
    SET_BIT(ch->monkcrap, COMB_KNEE);
  }
  else if ( IS_SET(ch->monkcrap, COMB_REV1) )
  {
    stc("Your aura glows a dark green for a moment.\n\r",ch);
    SET_BIT(ch->monkcrap, COMB_KNEE);
  }
  else if ( ch->monkcrap > 0 )
  {
    stc("Your aura slowly fades away.\n\r",ch);
    act("$n's aura slowly fades away.\n\r",ch,NULL,victim,TO_VICT);
    ch->monkcrap = 0;
  }
  act("You leap up and slam your knee into $N's chin.",ch,NULL,victim,TO_CHAR);
  act("$n leaps up and slams $s knee into your chin.",ch,NULL,victim,TO_VICT);
  act("$n leaps up and slams $s knee into $N's chin.",ch,NULL,victim,TO_NOTVICT);
  damage( ch, victim, dam, gsn_knee );
  return;
}

void do_reverse( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  bool foot = TRUE;
  int dam;
  int move = 0;

  if (!IS_CLASS(ch,CLASS_MONK))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( !IS_FS(ch, TECH_SWEEP) )
  {
    stc("You haven't learned that technique.\n\r",ch);
    return;
  }
  if ( ( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  WAIT_STATE( ch, skill_table[gsn_monksweep].beats );
  if ( ch->monkcrap == 0 )
  {
    stc("Your aura glows a light blue for a moment.\n\r",ch);
    SET_BIT(ch->monkcrap, COMB_REV1);
  }
  else if ( IS_SET(ch->monkcrap, COMB_REV1) && IS_SET(ch->monkcrap, COMB_SWEEP) )
  {
    act("You spin around and kick $N viciously in the head.",ch,NULL,victim,TO_CHAR);
    act("You place your hands on $N's head.",ch,NULL,victim,TO_CHAR);
    act("You feel revitalized.",ch,NULL,victim,TO_CHAR);
    act("$n's choyoken **- PULVERISES -** you!",ch,NULL,victim,TO_VICT);
    act("$n's choyoken **- PULVERISES -** $N!",ch,NULL,victim,TO_NOTVICT);
    move = (victim->move / 2);
    victim->move -= move;
    ch->move += UMIN(10000,move);
    ch->hit += UMIN(10000,move);
    ch->mana += UMIN(10000,move);
    ch->monkcrap = 0;
    return;
  }

  dam = number_range(1,4);
  dam = find_dam( ch, victim, dam, foot );

  act("You crouch down and sweep $N's legs out from under $M.",ch,NULL,victim,TO_CHAR);
  act("$n crouches down and sweeps your legs out from under you",ch,NULL,victim,TO_VICT);
  act("$n crouches down and sweeps $N's legs out from under $M",ch,NULL,victim,TO_NOTVICT);
  damage( ch, victim, dam, gsn_monksweep );
  return;
}

void do_sweep( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  bool foot = TRUE;
  int dam, mana;

  if (!IS_CLASS(ch,CLASS_MONK))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( !IS_FS(ch, TECH_SWEEP) )
  {
    stc("You haven't learned that technique.\n\r",ch);
    return;
  }  
  if ( ( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }

  WAIT_STATE(ch, 11);
  dam = number_range(1,4);
  dam = find_dam( ch, victim, dam, foot );
 
  if ( IS_SET(ch->monkcrap, COMB_REV1) )
  {
    stc("Your aura glows a light blue for a moment.\n\r",ch);
    SET_BIT(ch->monkcrap, COMB_SWEEP);
  }
  if ( IS_COMB(ch, COMB_THRUST1) && IS_COMB(ch, COMB_THRUST2) )  
  {
    act("You sweep around and kick $N viciously in the stomach.",ch,NULL,victim,TO_CHAR);
    act("You place your hands on $N's head.",ch,NULL,victim,TO_CHAR);
    act("You feel revitalized.",ch,NULL,victim,TO_CHAR);
    act("$n's raptor strike #R**- SIPHONS -**#n you!",ch,NULL,victim,TO_VICT);
    act("$n's raptor strike #R**- SIPHONS -**#n $N!",ch,NULL,victim,TO_NOTVICT);
    mana = (victim->mana / 2);
    victim->mana -= mana;
    ch->move += UMIN(10000,mana);
    ch->hit += UMIN(10000,mana);
    ch->mana += UMIN(10000,mana);
    ch->monkcrap = 0;
    return;
  }
  if (IS_COMB(ch, COMB_REV1) && IS_COMB(ch, COMB_KNEE))
  {
    act("You crouch down and sweep $N's legs out from under $M.",ch,NULL,victim,TO_CHAR);
    act("$n crouches down and sweeps your legs out from under you",ch,NULL,victim,TO_VICT);
    act("$n crouches down and sweeps $N's legs out from under $M",ch,NULL,victim,TO_NOTVICT);
    act("You place strike $N on the neck with two fingers #G**PARALYSING**#n $M.",ch,NULL,victim,TO_CHAR);
    act("$n's neck pinch #G**- PARALYSES -**#n you!",ch,NULL,victim,TO_VICT);
    act("$n's neck pinch #G**- PARALYSES -**#n $N!",ch,NULL,victim,TO_NOTVICT);
    WAIT_STATE(victim, 30);
    ch->monkcrap = 0;
    return;
  }
  act("You crouch down and sweep $N's legs out from under $M.",ch,NULL,victim,TO_CHAR);
  act("$n crouches down and sweeps your legs out from under you",ch,NULL,victim,TO_VICT);
  act("$n crouches down and sweeps $N's legs out from under $M",ch,NULL,victim,TO_NOTVICT);
  damage( ch, victim, dam, gsn_monksweep );
  return;
}

void do_elbow( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  bool foot = FALSE;
  int dam;

  if (!IS_CLASS(ch,CLASS_MONK))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( !IS_FS(ch, TECH_ELBOW) )
  {
    stc("You haven't learned that technique.\n\r",ch);
    return;
  }

  if ( ( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }

  WAIT_STATE(ch, 11);
  dam = number_range(1,6);
  dam = find_dam( ch, victim, dam, foot );   
  act("You ram your elbow into $N's face.",ch,NULL,victim,TO_CHAR);
  act("$n rams $s elbow into your face.",ch,NULL,victim,TO_VICT);
  act("$n rams $s elbow into $N's face.",ch,NULL,victim,TO_NOTVICT);
  damage( ch, victim, dam, gsn_elbow );
  return;
}


void do_thrustkick( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  bool foot = TRUE;
  int dam;
  
  if (!IS_CLASS(ch,CLASS_MONK))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( !IS_FS(ch, TECH_THRUST) )
  {
    stc("You haven't learned that technique.\n\r",ch);
    return;
  } 

  if ( ( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }

  WAIT_STATE(ch, 11);
  dam = number_range(1,5);
  dam = find_dam( ch, victim, dam, foot );   

  if ( ch->monkcrap == 0 && !IS_SET(ch->monkcrap, COMB_THRUST1) )
  {
    stc("Your aura glows dark blue for a moment.\n\r",ch);
    act("$n's aura glows dark blue for a moment.",ch,NULL,victim,TO_VICT);
    act("$n's aura glows dark blue for a moment.",ch,NULL,victim,TO_NOTVICT);
    SET_BIT(ch->monkcrap, COMB_THRUST1);
  }
  else if ( IS_SET(ch->monkcrap, COMB_THRUST1) && !IS_SET(ch->monkcrap,COMB_THRUST2) )
  {
    stc("Your aura flashes bright blue.\n\r",ch);
    act("$n's aura flashes bright blue.",ch,NULL,victim,TO_VICT);
    act("$n's aura flashes bright blue.",ch,NULL,victim,TO_NOTVICT);
    SET_BIT(ch->monkcrap, COMB_THRUST2);
  }
  else if ( ch->monkcrap != 0 )
  {
    stc("Your aura fades.\n\r",ch);
    act("$n's aura fades.",ch,NULL,victim,TO_VICT);
    act("$n's aura fades.",ch,NULL,victim,TO_NOTVICT);
    ch->monkcrap = 0;
  }

  act("You deliver a thrust kick to $N's chest.",ch,NULL,victim,TO_CHAR);
  act("$n delivers a powerful thrust kick to your chest.",ch,NULL,victim,TO_VICT);
  act("$n delivers a powerful thrust kick to $N's chest.",ch,NULL,victim,TO_NOTVICT);
  damage( ch, victim, dam, gsn_thrustkick );
  return;
}

void do_spinkick( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  bool foot = TRUE;
  int dam;

  if (!IS_CLASS(ch,CLASS_MONK))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( !IS_FS(ch, TECH_SPIN) )
  {
    stc("You haven't learned that technique.\n\r",ch);
    return;
  } 

  if ( ( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }

  WAIT_STATE(ch, 11);
  dam = number_range(1,4);

  dam = find_dam( ch, victim, dam, foot );   

  if ( IS_COMB(ch, COMB_THRUST1) && IS_COMB(ch, COMB_THRUST2) )
  {
    act("Your aura pulsates and channels energy into your attack.",ch,NULL,NULL,TO_CHAR);
    act("$n's aura pulsates and channels energy into $s attack.",ch,NULL,victim,TO_VICT);
    act("$n's aura pulsates and channels energy into $s attack.",ch,NULL,victim,TO_NOTVICT);
    ch->monkcrap = 0;
    dam /= 1.2;
    damage( ch, victim, dam, gsn_lightningkick );
    if ( ch->chi[CURRENT] > 1 )
    {
      dam /= 1.2;
      damage( ch, victim, dam, gsn_lightningkick );
    }
    if ( ch->chi[CURRENT] > 2 )
    {
      dam /= 1.2;
      damage( ch, victim, dam, gsn_lightningkick );
    }
    if ( ch->chi[CURRENT] > 3 )
    {
      dam /= 1.2;
      damage( ch, victim, dam, gsn_lightningkick );
    }
    if ( ch->chi[CURRENT] > 4 )
    {
      dam /= 1.2;
      damage( ch, victim, dam, gsn_lightningkick );
    }
    if ( ch->chi[CURRENT] > 5 )
    {
      dam /= 1.2;
      damage( ch, victim, dam, gsn_lightningkick );
    }
    return;
  }
  else if ( IS_SET(ch->monkcrap, COMB_SHIN) && IS_SET(ch->monkcrap, COMB_KNEE ) )
  {
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *mount;
    int number_hit = 0;

    ch->monkcrap = 0;
    stc("You spin around and around, going berserk.\n\r",ch);
    act("$n body speeds up, and $e spins around rapidly.",ch,NULL,victim,TO_ROOM);  
    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
      vch_next  = vch->next;
      if (number_hit > (ch->chi[CURRENT] + 1)) continue;
      if ( vch->in_room == NULL )
        continue;
      if ( !IS_NPC(vch) && vch->pcdata->chobj != NULL )
        continue;
      if ( ch == vch )
        continue;
      if ( vch->in_room == ch->in_room )
      {
        if ( ( mount = ch->mount ) != NULL ) {if ( mount == vch ) continue;}
        if (can_see(ch,vch))
        {
          one_hit( ch, vch, gsn_tornadokick, 0 );
          one_hit( ch, vch, gsn_tornadokick, 0 );
          number_hit++;
        }
      }
    }
    return;
  }
  act("You spin around and deliver a blow to $N's head.",ch,NULL,victim,TO_CHAR);
  act("$n spins around and delivers a blow to your head.",ch,NULL,victim,TO_VICT);
  act("$n spins around and delivers a devastating blow to $N's head.",ch,NULL,victim,TO_NOTVICT);
  damage( ch, victim, dam, gsn_spinkick );
  return;
}

void do_backfist( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  bool foot = FALSE;
  int dam;

  if (!IS_CLASS(ch,CLASS_MONK))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( !IS_FS(ch, TECH_BACK) )
  {
    stc("You haven't learned that technique.\n\r",ch);
    return;
  } 
  if ( ( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }

  WAIT_STATE(ch, 11);
  dam = number_range(1,5);
  dam = find_dam( ch, victim, dam, foot );   

  act("You spin around and slam $N's jaw with your fist.",ch,NULL,victim,TO_CHAR);
  act("$n spins around and slams you in the jaw with $s fist.",ch,NULL,victim,TO_VICT);
  act("$n spins around and slams $N in the jaw with $s fist.",ch,NULL,victim,TO_NOTVICT);
  damage( ch, victim, dam, gsn_backfist );
  return;
}

void do_learn( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  char arg1[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  char lin[MAX_STRING_LENGTH];
  int inpart = 0;
  int cost = 50000;

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg );
  
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MONK))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }


  sprintf(lin,"===========================================================================\n\r");
  if ( arg[0] == '\0' )
  {
    stc(" Syntax: Learn <fight|techniques|abilities|chi> <power|chi>\n\r",ch);
    stc("=============================|Fight Styles|==============================\n\r",ch);
    sprintf(buf,"       Trip     [%s] Kick      [%s] Bash     [%s] Elbow    [%s] Knee     [%s]   \n\r",
      IS_FS(ch, FS_TRIP) ? "*" : " ",
      IS_FS(ch, FS_KICK) ? "*" : " ",
      IS_FS(ch, FS_BASH) ? "*" : " ",
      IS_FS(ch, FS_ELBOW) ? "*" : " ", 
      IS_FS(ch, FS_KNEE) ? "*" : " ");
    stc(buf,ch);
    sprintf(buf,"       Disarm   [%s] Bite      [%s] Dirt     [%s] Grapple  [%s] Punch    [%s]   \n\r",
      IS_FS(ch, FS_DISARM) ? "*" : " ",
      IS_FS(ch, FS_BITE) ? "*" : " ",
      IS_FS(ch, FS_DIRT) ? "*" : " ",
      IS_FS(ch, FS_GRAPPLE) ? "*" : " ",
      IS_FS(ch, FS_PUNCH) ? "*" : " ");
    stc(buf,ch);
    sprintf(buf,"       Rip      [%s] Stamp     [%s] Backfist [%s] Jumpkick [%s] Spinkick [%s]   \n\r",
      IS_FS(ch, FS_RIP) ? "*" : " ",
      IS_FS(ch, FS_STAMP) ? "*" : " ",
      IS_FS(ch, FS_BACKFIST) ? "*" : " ",
      IS_FS(ch, FS_JUMPKICK) ? "*" : " ",
      IS_FS(ch, FS_SPINKICK) ? "*" : " ");
    stc(buf,ch);
    sprintf(buf,"       Sweep    [%s] Charge    [%s] Hurl     [%s] Gouge    [%s] Headbutt [%s]   \n\r",
      IS_FS(ch, FS_SWEEP) ? "*" : " ",
      IS_FS(ch, FS_CHARGE) ? "*" : " ",
      IS_FS(ch, FS_HURL) ? "*" : " ",
      IS_FS(ch, FS_GOUGE) ? "*" : " ",
      IS_FS(ch, FS_HEADBUTT) ? "*" : " ");
    stc(buf,ch);
    stc("===========================================================================\n\r",ch);
    stc("                                   Techniques                                  \n\r",ch);
    stc(lin,ch);
    sprintf(buf,"    Thrust Kick [%s]   Spin Kick   [%s]   Backfist    [%s]   Palm Strike [%s]   \n\r",
      IS_FS(ch, TECH_THRUST) ? "*" : " ",
      IS_FS(ch, TECH_SPIN)   ? "*" : " ",
      IS_FS(ch, TECH_BACK)   ? "*" : " ",
      IS_FS(ch, TECH_PALM)   ? "*" : " ");
    stc(buf,ch);
    sprintf(buf,"    Elbow       [%s]   Sweep       [%s]   Shin Kick   [%s]   Knee        [%s]   \n\r",
      IS_FS(ch, TECH_ELBOW)  ? "*" : " ",
      IS_FS(ch, TECH_SWEEP)  ? "*" : " ",
      IS_FS(ch, TECH_SHIN)   ? "*" : " ",
      IS_FS(ch, TECH_KNEE)   ? "*" : " ");
    stc(buf,ch);
    stc(lin,ch);
    stc("                                   Abilities                                   \n\r",ch);
    stc(lin,ch);
    sprintf(buf,"                 Awareness [%s%s%s%s]               Body     [%s%s%s%s]\n\r",
      ch->monkab[AWARE] > 0 ? "*" : " ",
      ch->monkab[AWARE] > 1 ? "*" : " ",
      ch->monkab[AWARE] > 2 ? "*" : " ",
      ch->monkab[AWARE] > 3 ? "*" : " ",
      ch->monkab[BODY]  > 0 ? "*" : " ",
      ch->monkab[BODY]  > 1 ? "*" : " ",
      ch->monkab[BODY]  > 2 ? "*" : " ",
      ch->monkab[BODY]  > 3 ? "*" : " ");
    stc(buf, ch);
    sprintf(buf,"     Combat   [%s%s%s%s]              Spirit    [%s%s%s%s]\n\r",
      ch->monkab[COMBAT] > 0 ? "*" : " ",
      ch->monkab[COMBAT] > 1 ? "*" : " ",
      ch->monkab[COMBAT] > 2 ? "*" : " ",
      ch->monkab[COMBAT] > 3 ? "*" : " ",
      ch->monkab[SPIRIT] > 0 ? "*" : " ",
      ch->monkab[SPIRIT] > 1 ? "*" : " ",
      ch->monkab[SPIRIT] > 2 ? "*" : " ",
      ch->monkab[SPIRIT] > 3 ? "*" : " ");
    stc(buf, ch);
    stc(lin,ch);
    stc("                                      Chi\n\r",ch);
    stc(lin,ch);
    sprintf(buf,"                 You have attained a level %d mastery of your Ch'i.\n\r",ch->chi[MAXIMUM]);
    stc(buf,ch);
    if ( ch->chi[CURRENT] > 0 )
    {
      sprintf(buf,"                     Your ch'i is currently active at level %d\n\r",ch->chi[CURRENT]);
      stc(buf,ch);
    }
    else stc("         You are currently not focusing your ch'i\n\r",ch);
    stc(lin,ch);
    return;
  }
  if ( !str_cmp(arg1, "fight" ) )
  {
    if (!str_cmp(arg,"trip")) 
    inpart = FS_TRIP;
    else if (!str_cmp(arg,"kick")) inpart = FS_KICK;
    else if (!str_cmp(arg,"bash")) inpart = FS_BASH;
    else if (!str_cmp(arg,"elbow")) inpart = FS_ELBOW;
    else if (!str_cmp(arg,"knee")) inpart = FS_KNEE;
    else if (!str_cmp(arg,"headbutt")) inpart = FS_HEADBUTT;
    else if (!str_cmp(arg,"disarm")) inpart = FS_DISARM;
    else if (!str_cmp(arg,"bite")) inpart = FS_BITE;
    else if (!str_cmp(arg,"dirt")) inpart = FS_DIRT;
    else if (!str_cmp(arg,"grapple")) inpart = FS_GRAPPLE;
    else if (!str_cmp(arg,"punch"))inpart = FS_PUNCH;
    else if (!str_cmp(arg,"gouge"))inpart = FS_GOUGE;
    else if (!str_cmp(arg,"rip")) inpart = FS_RIP;
    else if (!str_cmp(arg,"stamp")) inpart = FS_STAMP;
    else if (!str_cmp(arg,"backfist")) inpart = FS_BACKFIST;
    else if (!str_cmp(arg,"jumpkick")) inpart = FS_JUMPKICK;
    else if (!str_cmp(arg,"spinkick")) inpart = FS_SPINKICK;
    else if (!str_cmp(arg,"hurl")) inpart = FS_HURL;
    else if (!str_cmp(arg,"sweep")) inpart = FS_SWEEP;
    else if (!str_cmp(arg,"charge")) inpart = FS_CHARGE;
    else
    {
      do_learn(ch,"");
      return;
    }
  }
  else if ( !strcmp(arg1, "techniques") )
  {
    if (!str_cmp(arg,"knee")) {inpart = TECH_KNEE; cost = 200000;}
    else if (!str_cmp(arg,"spin")) {inpart = TECH_SPIN; cost = 200000;}
    else if (!str_cmp(arg,"backfist")) {inpart = TECH_BACK; cost = 200000;}
    else if (!str_cmp(arg,"elbow")) {inpart = TECH_ELBOW; cost = 200000;}
    else if (!str_cmp(arg,"palm")) {inpart = TECH_PALM; cost = 200000;}
    else if (!str_cmp(arg,"shin")) {inpart = TECH_SHIN; cost = 200000;}
    else if (!str_cmp(arg,"sweep")){inpart = TECH_SWEEP; cost = 200000;}
    else if (!str_cmp(arg, "thrust")) {inpart = TECH_THRUST; cost = 200000;}
    else
    {
      do_learn(ch,"");
      return;
    }
  }
  else if (!str_cmp(arg1, "abilities"))
  {
    if (!str_cmp( arg, "body" )) {inpart = BODY; cost = 500000;}
    else if (!str_cmp( arg, "combat")) {inpart = COMBAT; cost = 500000;}
    else if (!str_cmp( arg, "spirit")) {inpart = SPIRIT; cost = 500000;}
    else if (!str_cmp( arg, "awareness")) {inpart = AWARE; cost = 500000;}
    else
    {
      do_learn(ch,"");
      return;
    }
  }
  else if ( !str_cmp(arg1, "chi" ) )
  {
    inpart = MAXIMUM;
    cost = (ch->chi[inpart]+1) * 1000000;
  }
  else 
  {
    do_learn(ch,"");
    return;
  }
  if ( !str_cmp(arg1, "abilities" ) )
  {
    if (ch->monkab[inpart] >= 4)
    {
      stc("You have already learned all you can in this ability.\n\r",ch);
      return;
    }
    if ( ch->exp < cost )
    {
      send_to_char("You do not have enough experience to learn that power.\n\r",ch);
      return;
    }
    ch->monkab[inpart]++;
    ch->exp -= cost;
    send_to_char("Ok.\n\r",ch);
    save_char_obj(ch);
    return; 
  }
  else if ( !str_cmp( arg1, "chi" ) )
  {
    if ( ch->chi[MAXIMUM] >= 6 )
    {
      stc("You have already mastered your ch'i.\n\r",ch);
      return;
    }
    if ( ch->exp < cost)
    {
      sprintf(buf,"You need %d experience to gain the next level of mastery of your ch'i.",cost);
      return;
    }
    ch->exp -= cost;
    ch->chi[MAXIMUM]++;
    save_char_obj(ch);
    stc("You gain more control over your ch'i.\n\r",ch);
    return;
  }
  else
  {
    if (IS_FS(ch, inpart))
    {
      send_to_char("You have already got that power.\n\r",ch);
      return;
    }
    if ( ch->exp < cost )
    {
      send_to_char("You do not have enough experience to learn that power.\n\r",ch);
      return;
    }
    SET_BIT(ch->monkstuff, inpart);
    ch->exp -= cost;
    send_to_char("Ok.\n\r",ch);
    save_char_obj(ch);
    return;
  }
}
