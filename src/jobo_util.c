/***********************************
 * Different needed function calls *
 ***********************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* close */
#include <time.h>
#include "merc.h"

/*
 * Is astr contained within bstr ?
 *
 */
bool is_contained( const char *astr, const char *bstr )
{
  int i, alen, blen, count;
   
  alen = strlen(astr);
  blen = strlen(bstr);
  if (alen > blen) return FALSE;
  for (i = 0; i <= (blen - alen); i++)
  {
    count = 0;
    while (count < alen && UPPER(astr[count]) == UPPER(bstr[i+count])) count++;
    if (count == alen) return TRUE;
  }
  return FALSE;
}

bool is_contained2( const char *astr, const char *bstr )
{
  int i, alen, blen, count;
   
  alen = strlen(astr);
  blen = strlen(bstr);
  if (alen > blen) return FALSE;
  for (i = 0; i <= (blen - alen); i++)
  {
    count = 0;
    while (count < alen && astr[count] == bstr[i+count]) count++;
    if (count == alen) return TRUE;
  }
  return FALSE;
}

int strlen2(const char *s)
{
  int i, b, count=0;

  if (s[0] == '\0') return 0;
  b = strlen(s);
  for (i = 0; i < b; i++)
  {
    if (s[i] == '#') count++;
  }
  return (b + 7 * count);
}

void win_prize( CHAR_DATA *ch )
{
  int i,vnum;
  OBJ_DATA *obj;
  OBJ_INDEX_DATA *pIndex;
  
  if (IS_NPC(ch)) return;
  i = number_range(1,100);
  if (i < 50) vnum = OBJ_VNUM_PROTOPLASM;
  else if (i < 65) vnum = 33851;
  else if (i < 75) vnum = 33852;
  else if (i < 80) vnum = 33853;
  else if (i < 85) vnum = 33854;
  else if (i < 90) vnum = 33855;
  else if (i < 95) vnum = 33856;
  else vnum = 33857;
  if ((pIndex = get_obj_index(vnum)) == NULL)
  {
    bug("BAD PRIZE!!",0);
    return;
  }
  obj = create_object(pIndex, 50);
  if (vnum == OBJ_VNUM_PROTOPLASM)
  {
    obj->level = 1;
    free_string(obj->short_descr);
    free_string(obj->name);
    free_string(obj->description);
    obj->short_descr = str_dup("A prize token");
    obj->description = str_dup("A token lies on the floor");
    obj->name = str_dup("prize token");
    obj->value[0] = number_range(100,300);
    obj->item_type = ITEM_QUEST;
  }
  obj_to_char(obj,ch);
  return;
}

void do_clearstats2( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    if (IS_NPC(ch)) return;

    powerdown(ch); /* remove class shit */

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
        obj_next = obj->next_content;
        if ( obj->wear_loc != WEAR_NONE )
        {obj_from_char(obj); obj_to_char(obj,ch);}
    }
     
    while ( ch->affected )
      affect_remove( ch, ch->affected );
      
    if (IS_SET(ch->affected_by, AFF_POLYMORPH)) REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
    if (IS_SET(ch->affected_by, AFF_ETHEREAL)) REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);  
    if (IS_EXTRA(ch,EXTRA_DRAGON)) REMOVE_BIT(ch->extra, EXTRA_DRAGON);
    
    ch->affected_by      = 0;
    ch->armor            = 100;
    ch->hit              = UMAX( 1, ch->hit  );
    ch->mana             = UMAX( 1, ch->mana );
    ch->move             = UMAX( 1, ch->move );
    ch->hitroll          = 0;
    ch->damroll          = 0;
    ch->saving_throw     = 0;
    ch->pcdata->mod_str  = 0;
    ch->pcdata->mod_int  = 0;
    ch->pcdata->mod_wis  = 0;
    ch->pcdata->mod_dex  = 0;
    ch->pcdata->mod_con  = 0;
    ch->pcdata->followers = 0;
    if (IS_SET(ch->newbits, NEW_DFORM)) REMOVE_BIT(ch->newbits, NEW_DFORM);
    if (IS_POLYAFF(ch, POLY_ZULOFORM)) REMOVE_BIT(ch->polyaff, POLY_ZULOFORM);
    if (IS_SET(ch->newbits, NEW_CUBEFORM)) REMOVE_BIT(ch->newbits, NEW_CUBEFORM);
    save_char_obj( ch );
    send_to_char("Your stats have been cleared.  Please rewear your equipment.\n\r",ch);
    return;
}

/*
 *  If an item have a bad char in it's short/long/name desc, the mud
 *  will freeze when the char with that item tries to log on, so use
 *  this check on any renaming of items (glamour/reshape/etc).
 */
bool has_bad_chars(CHAR_DATA *ch, char *argument)
{
  int i;
  
  if ( argument[0] == '\0' ) return FALSE;
  for (i = 0; argument[i] != '\0' ; i++)  
  {
    if (argument[i] == '~') return TRUE;
  }
  return FALSE;
}

void ragnarok_stop()
{
  DESCRIPTOR_DATA *d;
  
  ragnarok = FALSE;
  do_info(NULL,"#CPeace has been restored in the realms, the time of ragnarok is no more#n");
  for (d = descriptor_list; d != NULL; d = d->next)
  {
    if (d->character && d->connected == CON_PLAYING)
    {
      d->character->fight_timer = 0;
      d->character->pcdata->safe_counter = 5;
      do_call(d->character,"all");
      do_restore(d->character,"self");
    }
  }
  return;
}

/* agrr_test by blade of E, version 1.31. */
void aggr_test(CHAR_DATA * ch)
{
  char buf[60];
  CHAR_DATA *wch;
  CHAR_DATA *wch_next;
  CHAR_DATA *victim;
    
  if (!IS_NPC(ch) && ch->level < 7 && ch->in_room != NULL && !IS_SET(ch->in_room->room_flags,ROOM_SAFE))
  {  
    for ( wch = ch->in_room->people; wch != NULL; wch = wch_next )
    {
      wch_next = wch->next_in_room;
      if ( !IS_NPC(wch) || !IS_SET(wch->act, ACT_AGGRESSIVE)
         || wch->fighting != NULL || IS_AFFECTED(wch, AFF_CHARM)
         || !IS_AWAKE(wch) || ( IS_SET(wch->act, ACT_WIMPY) && IS_AWAKE(ch))
         || !can_see( wch, ch ) || number_bits(2) == 0)
      {
        continue;
      }
      victim = wch;
      if ( victim == NULL ) continue;
      sprintf(buf,"%s screams and attacks!\n\r", victim->name);
      send_to_char(buf, ch);
      multi_hit( victim, ch, TYPE_UNDEFINED );
    }
  }
  return;
}  

void powerdown(CHAR_DATA *ch)
{
  if (IS_NPC(ch)) return;

  if (IS_CLASS(ch, CLASS_DEMON))
  {
    if (IS_POLYAFF(ch, POLY_ZULOFORM))  /* demonform */
    {
      REMOVE_BIT(ch->polyaff, POLY_ZULOFORM);
      REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
      free_string( ch->morph );
      ch->morph = str_dup( "" );
    }
  }
  else if (IS_CLASS(ch, CLASS_DROW))
  {
    if (IS_SET(ch->newbits,NEW_DFORM)) /* spiderform */
    {
      free_string(ch->morph);
      ch->morph = str_dup("");
      REMOVE_BIT(ch->newbits,NEW_DFORM);
      REMOVE_BIT(ch->newbits,THIRD_HAND);
      REMOVE_BIT(ch->newbits,FOURTH_HAND);
    }
  }
  else if (IS_CLASS(ch, CLASS_VAMPIRE))
  {
    if (IS_EXTRA(ch, EXTRA_DRAGON)) /* dragonform */
    {
      REMOVE_BIT(ch->extra,EXTRA_DRAGON);
      free_string( ch->morph );
      ch->morph = str_dup( "" );
    }
    if (IS_POLYAFF(ch, POLY_ZULOFORM)) /* zuloform */
    {
      REMOVE_BIT(ch->polyaff, POLY_ZULOFORM);
      REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
      free_string( ch->morph );
      ch->morph = str_dup( "" );
    }
  }
  else if (IS_CLASS(ch, CLASS_TANARRI))
  {
    if (ch->pcdata->powers[TANARRI_FURY_ON] == 1)  /* fury */
      ch->pcdata->powers[TANARRI_FURY_ON] = 0;
  }
  else if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
  {
    if (IS_SET(ch->affected_by, AFF_POLYMORPH)) /* reset form */
    {
      ch->pcdata->powers[SHAPE_FORM]=0;
      REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
      free_string(ch->morph);
      ch->morph=str_dup("");
    }
  }
  else if (IS_CLASS(ch, CLASS_DROID))
  {
    if (IS_SET(ch->newbits, NEW_CUBEFORM))  /* avatar of Lloth */
    {
      REMOVE_BIT(ch->newbits, NEW_CUBEFORM);
      REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);  
      free_string(ch->morph);
      ch->morph=str_dup("");
    }
  }
  else if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
  {
    if (IS_SET(ch->pcdata->powers[AURAS], MIGHT_AURA))  /* aura of might */
      REMOVE_BIT(ch->pcdata->powers[AURAS], MIGHT_AURA);
  }
  else if (IS_CLASS(ch, CLASS_ANGEL))
  {
    if (IS_SET(ch->newbits, NEW_CUBEFORM)) /* godly favor */
    {
      REMOVE_BIT(ch->newbits, NEW_CUBEFORM);
      REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
      free_string(ch->morph);
      ch->morph=str_dup("");
    }
  }
  return;
}

int get_ratio(CHAR_DATA *ch)
{
  int ratio;

  if (IS_NPC(ch)) return 0;
  if ((ch->pkill + ch->pdeath) == 0) ratio = 0; // to avoid divide by zero.
  else if (ch->pkill > 0)
    ratio = ch->pkill * 100 * ((ch->pkill * ch->pkill) - (ch->pdeath * ch->pdeath))/((ch->pkill + ch->pdeath) * (ch->pkill + ch->pdeath));
  else
    ratio = 100 * ((ch->pkill * ch->pkill) - (ch->pdeath * ch->pdeath))/((ch->pkill + ch->pdeath) * (ch->pkill + ch->pdeath));
  return ratio;
}

bool multicheck(CHAR_DATA *ch)
{
  CHAR_DATA *gch;
  char buf[MAX_STRING_LENGTH];

  for (gch = char_list; gch; gch = gch->next)
  {
    if (IS_NPC(gch)) continue;
    if (gch == ch) continue;
    if (strlen(gch->lasthost) > 2)
    {
      if (gch->desc)
      {
        if (!str_cmp(gch->desc->host, ch->desc->host))
        {
          sprintf(buf,"%s has connected from the same IP as %s", ch->name, gch->name);
          log_string(buf);
          return TRUE;
        }
      }
      else if (!str_cmp(gch->lasthost, ch->desc->host))
      {
        sprintf(buf,"%s has connected from the same IP as %s", ch->name, gch->name);
        log_string(buf);
        return TRUE;
      }
    }
  }
  return FALSE;
}

bool reachedDecapLimit( CHAR_DATA *ch )
{
  AFFECT_DATA *paf;
  OBJ_DATA *obj;
  int limit = 0, objhps = 0, spellhps = 0;
  int hps, i;
     
  if (IS_NPC(ch)) return TRUE;
  if (ch->level > 6) return FALSE;
    
  if (!IS_CLASS(ch, CLASS_SAMURAI))
  {
    for (paf = ch->affected; paf != NULL; paf = paf->next)
    {
      if (paf->location == APPLY_HIT)
        spellhps += paf->modifier;
    } 
  }
  for (i = 0; i < MAX_WEAR; i++)
  {
    if ((obj = get_eq_char(ch, i)) == NULL) continue;
    if (IS_CLASS(ch, CLASS_SAMURAI) && obj->pIndexData->vnum != 33177) continue;
    for (paf = obj->pIndexData->affected; paf; paf = paf->next)
    {
      if (paf->location == APPLY_HIT)
        objhps += paf->modifier;
    }
    for (paf = obj->affected; paf; paf = paf->next)
    {
      if (paf->location == APPLY_HIT)
        objhps += paf->modifier;
    }
  }

  hps = (ch->max_hit - (spellhps + objhps));

  if (hps > 10000) limit += 10;
  else limit += hps/1000;
  if ((hps -= 10000) > 10000) limit += 20;
  else if (hps > 0) limit += 2 * hps/1000;
  if ((hps -= 10000) > 10000) limit += 30;
  else if (hps > 0) limit += 3 * hps/1000;
  if ((hps -= 10000) > 10000) limit += 40;
  else if (hps > 0) limit += 4 * hps/1000;
  if ((hps -= 10000) > 10000) limit += 50;  
  else if (hps > 0) limit += 5 * hps/1000;
  if ((hps -= 10000) > 10000) limit += 60;
  else if (hps > 0) limit += 6 * hps/1000;
     
  if (is_upgrade(ch)) limit *= (2 + ch->pcdata->upgrade_level);
     
  /*
   * For those with no skill
   */
  limit += ch->pdeath;   

  if (limit > ch->pkill) return FALSE;
  else return TRUE;
}

void death_info(char *str)
{
  DESCRIPTOR_DATA *d;

  if (str[0] == '\0') return;

  for (d = descriptor_list; d != NULL; d = d->next)
  {
    if (d->connected == CON_PLAYING && d->character != NULL)
    {
      send_to_char("#C<- #RDeath #C->#n ", d->character);
      send_to_char(str, d->character);
      send_to_char("\n\r", d->character);
    }
  }
  return;
}

void avatar_info(char *str)
{
  DESCRIPTOR_DATA *d;

  if (str[0] == '\0') return;

  for (d = descriptor_list; d != NULL; d = d->next) 
  {  
    if (d->connected == CON_PLAYING && d->character != NULL)                                              
    {    
      send_to_char("#C<- #RAvatar #C->#n ", d->character); 
      send_to_char(str, d->character); 
      send_to_char("\n\r", d->character); 
    }    
  }  
  return;
}

void leave_info(char *str)
{
  DESCRIPTOR_DATA *d;

  if (str[0] == '\0') return;

  for (d = descriptor_list; d != NULL; d = d->next) 
  {  
    if (d->connected == CON_PLAYING && d->character != NULL)                                              
    {    
      send_to_char("#C<- #RLeaves #C->#n ", d->character); 
      send_to_char(str, d->character); 
      send_to_char("\n\r", d->character); 
    }    
  }  
  return;
}

void enter_info(char *str)
{
  DESCRIPTOR_DATA *d;

  if (str[0] == '\0') return;

  for (d = descriptor_list; d != NULL; d = d->next) 
  {  
    if (d->connected == CON_PLAYING && d->character != NULL)                                              
    {    
      send_to_char("#C<- #REnters #C->#n ", d->character); 
      send_to_char(str, d->character); 
      send_to_char("\n\r", d->character); 
    }    
  }  
  return;
}

int getMight(CHAR_DATA *ch)
{
  AFFECT_DATA *paf;
  OBJ_DATA *obj;
  int spellhps = 0;
  int objhps = 0;
  int might, temp, i;

  if (!IS_CLASS(ch, CLASS_SAMURAI))
  {
    for (paf = ch->affected; paf != NULL; paf = paf->next)
    {
      if (paf->location == APPLY_HIT)
        spellhps += paf->modifier;
    }
  }

  for (i = 0; i < MAX_WEAR; i++)
  {
    if ((obj = get_eq_char(ch, i)) == NULL) continue;
    if (IS_CLASS(ch, CLASS_SAMURAI) && obj->pIndexData->vnum != 33177) continue;
    for (paf = obj->pIndexData->affected; paf != NULL; paf = paf->next)
    {
      if (paf->location == APPLY_HIT)
        objhps += paf->modifier;
    }
    for (paf = obj->affected; paf; paf = paf->next)
    {
      if (paf->location == APPLY_HIT)
        objhps += paf->modifier;
    }
  }

  might = (ch->max_hit - (spellhps + objhps))/100;
  
  for (i = 0; i < 5; i++)  might += UMIN(2, ch->spl[i]/100);
  for (i = 0; i < 13; i++) might += UMIN(4, ch->wpn[i]/50);
  for (i = 1; i < 11; i++) might += UMIN(4, ch->stance[i]/50);
  if (IS_SET(ch->newbits, NEW_MASTERY)) might += 2;
  if (ch->stance[23] != -1) might += 250;
  else if (ch->stance[22] != -1) might += 200;
  else if (ch->stance[21] != -1) might += 150;
  else if (ch->stance[20] != -1) might += 100;
  else if (ch->stance[19] != -1) might += 50;
  if (might >= 150)
  {
    if (is_upgrade(ch)) might *= 2;
    if (ch->pcdata->rune_count > 0) might += UMIN(150, ch->pcdata->rune_count * 10);
    if ((temp = get_ratio(ch)) >= 500) might += UMIN(100, temp / 50);  // 10 points for each pkpower.
    if (ch->pcdata->upgrade_level > 0) might += ch->pcdata->upgrade_level * 120;
  }
  return might;
}

void forge_affect(OBJ_DATA *obj, int value)
{
  AFFECT_DATA paf;

  paf.type           = 0;
  paf.duration       = -1;
  paf.location       = APPLY_HITROLL;
  paf.modifier       = value;   
  paf.bitvector      = 0;
  affect_to_obj(obj, &paf);
    
  paf.type           = 0;
  paf.duration       = -1;
  paf.location       = APPLY_DAMROLL;
  paf.modifier       = value;
  paf.bitvector      = 0;
  affect_to_obj(obj, &paf);
}

void dump_last_command()
{
  FILE *fp;
  char buf[MAX_STRING_LENGTH];

  fp = fopen("../src/crash.txt","a");
  fprintf (fp,"Last command typed : %s\n",last_command);
  fflush(fp);
  fclose(fp);

  /*
   * creates a note to the immortals
   */
  sprintf(buf, "It seems we have crashed, the last command typed was\n\r\n\r");
  strcat(buf, last_command);
  strcat(buf, "\n\r\n\rPlease remember that this doesn't mean that this caused the crash.\n\r\n\rRegards,\n\r\n\rThe Crash Code");
  make_note("Immortal", "Crash Code", "imm", "We Crashed", 7, buf);

} 

void update_revision(CHAR_DATA *ch)
{
  OBJ_DATA *obj;
  int i;

  if (IS_NPC(ch)) return;
  if (ch->pcdata->revision == CURRENT_REVISION) return;

  /*
   * We don't end cases with break, since we want the player to be fully updated.
   */
  switch (ch->pcdata->revision)
  {
    case 0:
      for (i = 0; i < MAX_WEAR; i++)
      {
        if ((obj = get_eq_char(ch, i)) == NULL) continue;
        if (obj->pIndexData->vnum >= 814 && obj->pIndexData->vnum <= 825) ch->pcdata->rune_count++;
      }
      ch->pcdata->revision++;
    default: break;
  }
  return;
}

void logout_message(CHAR_DATA *ch)
{
  static char * const he_she  [] = { "XX", "he",  "she" };
  static char * const him_her [] = { "XX", "him", "her" };
  static char * const his_her [] = { "XX", "his", "her" };
 
  DESCRIPTOR_DATA *d;
  char buf[400]; // that should be plenty.
  const char *dmess;
  const char *i;
  char *ptr2;
  char *ptr;
  int size;
  
  size = strlen2(ch->pcdata->logoutmessage);
  if (size > 380)
  {
    bug("Bad logoutmessage.",0);
    return;
  }
    
  ptr2  = "#C<- #RLeaves #C->#n ";
  ptr   = buf;
  dmess = ch->pcdata->logoutmessage;
  
  while ((*ptr = *ptr2) != '\0')
    ++ptr, ++ptr2;
   
  while (*dmess != '\0')
  {
    if ( *dmess != '$' )
    {
      *ptr++ = *dmess++;
      continue;
    }
    ++dmess;
    switch (*dmess)
    {
      default:  i = ""; break;
      case 'n': i = ch->name; break;
      case 'e': i = he_she  [URANGE(1, ch->sex, 2)]; break;
      case 'm': i = him_her [URANGE(1, ch->sex, 2)]; break;
      case 's': i = his_her [URANGE(1, ch->sex, 2)]; break;
    }
    ++dmess;
    /* copying the data into the pointer */
    while ((*ptr = *i) != '\0')
      ++ptr, ++i;
  }
  *ptr++ = '\n';
  *ptr++ = '\r';
  
  for (d = descriptor_list; d; d = d->next)   
  {
    if (d->lookup_status != STATUS_DONE) continue;
    if (d->connected != CON_PLAYING ) continue;
    write_to_buffer( d, buf, ptr - buf );
  }
  return;
}
  
void tie_message(CHAR_DATA *ch, CHAR_DATA *victim)
{
  static char * const he_she  [] = { "XX", "he",  "she" };
  static char * const him_her [] = { "XX", "him", "her" };
  static char * const his_her [] = { "XX", "his", "her" };
  DESCRIPTOR_DATA *d;
  char buf[400]; // that should be plenty.
  const char *dmess;
  const char *i;
  char *ptr2;
  char *ptr;
  int size;
   
  size = strlen2(ch->pcdata->tiemessage);
  if (size > 380)
  {
    bug("Bad tiemessage.",0);
    return;
  }
  
  ptr2  = "#C<- #RTie #C->#n ";
  ptr   = buf;
  dmess = ch->pcdata->tiemessage;
   
  while ((*ptr = *ptr2) != '\0')
    ++ptr, ++ptr2;
   
  while (*dmess != '\0')
  {
    if ( *dmess != '$' )
    {
      *ptr++ = *dmess++;
      continue;
    }
    ++dmess;
    switch (*dmess)
    {
      default:  i = ""; break;
      case 'n': i = ch->name; break;
      case 'e': i = he_she  [URANGE(1, ch->sex, 2)]; break;
      case 'm': i = him_her [URANGE(1, ch->sex, 2)]; break;
      case 's': i = his_her [URANGE(1, ch->sex, 2)]; break;
      case 'N': i = victim->name; break;
      case 'S': i = his_her [URANGE(1, victim->sex, 2)]; break;
      case 'M': i = him_her [URANGE(1, victim->sex, 2)]; break;
      case 'E': i = he_she  [URANGE(1, victim->sex, 2)]; break;
    }
    ++dmess;
    /* copying the data into the pointer */   
    while ((*ptr = *i) != '\0')
      ++ptr, ++i;
  }
  *ptr++ = '\n';
  *ptr++ = '\r';
  
  for (d = descriptor_list; d; d = d->next)
  {
    if (d->lookup_status != STATUS_DONE) continue;
    if (d->connected != CON_PLAYING ) continue;
    write_to_buffer( d, buf, ptr - buf );
  }
  return;
}

void login_message(CHAR_DATA *ch)
{
  static char * const he_she  [] = { "XX", "he",  "she" };
  static char * const him_her [] = { "XX", "him", "her" };
  static char * const his_her [] = { "XX", "his", "her" };
   
  DESCRIPTOR_DATA *d;
  char buf[400]; // that should be plenty.
  const char *dmess;
  const char *i;
  char *ptr2;
  char *ptr;
  int size;
  
  size = strlen2(ch->pcdata->loginmessage);
  if (size > 380)
  {
    bug("Bad loginmessage.",0); 
    return;
  }

  ptr2  = "#C<- #REnters #C->#n ";
  ptr   = buf;
  dmess = ch->pcdata->loginmessage;
      
  while ((*ptr = *ptr2) != '\0')
    ++ptr, ++ptr2;

  while (*dmess != '\0')
  {  
    if ( *dmess != '$' )
    {
      *ptr++ = *dmess++;
      continue;
    }
    ++dmess;
    switch (*dmess)
    {
      default:  i = ""; break;
      case 'n': i = ch->name; break;
      case 'e': i = he_she  [URANGE(1, ch->sex, 2)]; break;
      case 'm': i = him_her [URANGE(1, ch->sex, 2)]; break;
      case 's': i = his_her [URANGE(1, ch->sex, 2)]; break;
    }
    ++dmess;
    /* copying the data into the pointer */
    while ((*ptr = *i) != '\0')
      ++ptr, ++i;
  }
  *ptr++ = '\n';
  *ptr++ = '\r';
    
  for (d = descriptor_list; d; d = d->next)
  {
    if (d->lookup_status != STATUS_DONE) continue;
    if (d->connected != CON_PLAYING ) continue;
    write_to_buffer( d, buf, ptr - buf );
  }
  return;
}

void special_decap_message(CHAR_DATA *ch, CHAR_DATA *victim)
{  
  static char * const he_she  [] = { "XX", "he",  "she" };
  static char * const him_her [] = { "XX", "him", "her" };
  static char * const his_her [] = { "XX", "his", "her" };
  DESCRIPTOR_DATA *d;
  char buf[400]; // that should be plenty.
  const char *dmess;
  const char *i;
  char *ptr2;
  char *ptr;
  int size;
   
  size = strlen2(ch->pcdata->decapmessage);
  if (size > 380)
  {
    bug("Bad decapmessage.",0);
    return;
  }

  ptr2  = "#C<- #RDeath #C->#n ";
  ptr   = buf;
  dmess = ch->pcdata->decapmessage;

  while ((*ptr = *ptr2) != '\0')
    ++ptr, ++ptr2;

  while (*dmess != '\0')
  {
    if ( *dmess != '$' )
    {
      *ptr++ = *dmess++;
      continue;
    }
    ++dmess;
    switch (*dmess)
    {
      default:  i = ""; break;
      case 'n': i = ch->name; break;
      case 'e': i = he_she  [URANGE(1, ch->sex, 2)]; break;
      case 'm': i = him_her [URANGE(1, ch->sex, 2)]; break;
      case 's': i = his_her [URANGE(1, ch->sex, 2)]; break;
      case 'N': i = victim->name; break;
      case 'S': i = his_her [URANGE(1, victim->sex, 2)]; break;
      case 'M': i = him_her [URANGE(1, victim->sex, 2)]; break;
      case 'E': i = he_she  [URANGE(1, victim->sex, 2)]; break;
    }
    ++dmess;
    /* copying the data into the pointer */
    while ((*ptr = *i) != '\0')
      ++ptr, ++i;
  }
  *ptr++ = '\n';
  *ptr++ = '\r';
  
  for (d = descriptor_list; d; d = d->next)
  {
    if (d->lookup_status != STATUS_DONE) continue;
    if (d->connected != CON_PLAYING ) continue;
    write_to_buffer( d, buf, ptr - buf );
  }
  return;
}

void avatar_message(CHAR_DATA *ch)
{  
  static char * const he_she  [] = { "XX", "he",  "she" };
  static char * const him_her [] = { "XX", "him", "her" };
  static char * const his_her [] = { "XX", "his", "her" };

  DESCRIPTOR_DATA *d; 
  char buf[400]; // that should be plenty.
  const char *dmess;
  const char *i;
  char *ptr2;
  char *ptr;  
  int size;

  size = strlen2(ch->pcdata->avatarmessage);
  if (size > 380)
  {
    bug("Bad avatarmessage.",0);
    return;
  }

  ptr2  = "#C<- #RAvatar #C->#n ";
  ptr   = buf;
  dmess = ch->pcdata->avatarmessage;
  
  while ((*ptr = *ptr2) != '\0')
    ++ptr, ++ptr2;
   
  while (*dmess != '\0')
  {
    if ( *dmess != '$' )
    {
      *ptr++ = *dmess++;
      continue;
    }
    ++dmess;
    switch (*dmess)
    {
      default:  i = ""; break;
      case 'n': i = ch->name; break;
      case 'e': i = he_she  [URANGE(1, ch->sex, 2)]; break;
      case 'm': i = him_her [URANGE(1, ch->sex, 2)]; break;
      case 's': i = his_her [URANGE(1, ch->sex, 2)]; break;
    }
    ++dmess;
    /* copying the data into the pointer */
    while ((*ptr = *i) != '\0')
      ++ptr, ++i;
  }
  *ptr++ = '\n';
  *ptr++ = '\r'; 
   
  for (d = descriptor_list; d; d = d->next)
  {
    if (d->lookup_status != STATUS_DONE) continue;
    if (d->connected != CON_PLAYING ) continue;
    write_to_buffer( d, buf, ptr - buf );
  }
  return;
} 

void recycle_descriptors()
{
  DESCRIPTOR_DATA *dclose;
  DESCRIPTOR_DATA *dclose_next;

  for (dclose = descriptor_list; dclose; dclose = dclose_next)
  {
    dclose_next = dclose->next;   
    if (dclose->lookup_status != STATUS_CLOSED) continue;
   
    /*
     * First let's get it out of the descriptor list.
     */
    if ( dclose == descriptor_list )
    {
      descriptor_list = descriptor_list->next;
    }
    else 
    {  
      DESCRIPTOR_DATA *d;
   
      for (d = descriptor_list; d && d->next != dclose; d = d->next)
        ;
      if (d != NULL)
        d->next = dclose->next;
      else
      {
        bug( "Recycle_descriptors: dclose not found.", 0 );
        continue;
      }
    }

    /*
     * Clear out that memory
     */
    free_string( dclose->host );
    free_mem( dclose->outbuf, dclose->outsize );
      
    /*
     * Mccp
     */
    if (dclose->out_compress)
    {
      deflateEnd(dclose->out_compress);
      free_mem(dclose->out_compress_buf, COMPRESS_BUF_SIZE);
      free_mem(dclose->out_compress, sizeof(z_stream));
    }
   
    /*
     * Bye bye mr. Descriptor.
     */
    close( dclose->descriptor );
      
    /* 
     * And then we recycle
     */
    dclose->next        = descriptor_free;
    descriptor_free     = dclose;
  }
}

void update_ragnarok()
{
  if (ragnarok_safe_timer > 0)
    ragnarok_safe_timer--;
  if (--ragnarok_on_timer <= 0 && ragnarok)
  {
    ragnarok_stop();
    ragnarok_safe_timer = 120;  /* 120 minutes of safe after a ragnarok */
  }
}

void update_doubleexp()
{
  if (pulse_doubleexp > 0)
  {
    if (--pulse_doubleexp == 0)
    {
      global_exp = FALSE;
      pulse_doubleexp = -1;
      do_info(NULL,"#GH#Ra#Gp#Rp#Gy #GH#Ro#Gu#Rr #0(#7ENDED#0)#n");
    }
  }
}

void update_doubleqps()
{
  if (pulse_doubleqp > 0)
  {
    if ( --pulse_doubleqp == 0)
    {
      global_qp = FALSE;
      pulse_doubleqp = -1;
      do_info(NULL,"#GQ#RU#GE#RS#GT#RO#GR#RS #GD#RE#GL#RI#GG#RH#GT #0(#7ENDED#0)#n");
    }
  }
}

void update_arena()
{
  if (--pulse_arena <= 1)
  {
    if (pulse_arena > 0) open_arena();
    else if (pulse_arena == 0)
    {
      pulse_arena = -1;
      close_arena();
    }
  }
}

void recycle_dummys()
{
  DUMMY_ARG *dummy;
  DUMMY_ARG *dummy_next;
        
  for (dummy = dummy_list; dummy; dummy = dummy_next)
  {
    dummy_next = dummy->next;
    if (dummy->status == 1) continue;  // being used

    if (dummy == dummy_list)
    {
      dummy_list = dummy_list->next;
    } 
    else
    {
      DUMMY_ARG *prev;
        
      /* we find the prev dummy arg */
      for (prev = dummy_list; prev && prev->next != dummy; prev = prev->next)
        ;
      if (prev != NULL)
        prev->next = dummy->next;
      else
      {
        bug( "Recycle_dymmys: dummy not found.", 0 );  
        continue;
      }
      
      /* recycle */  
      dummy->next = dummy_free;
      dummy_free = dummy;
    }
  }     
}

