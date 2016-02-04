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

/**********************************
 * All kinds of new stuff by Jobo *
 **********************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#if !defined(WIN32)
#include <unistd.h>
#endif
#include "merc.h"

void    sort_top_board  args( ( void ) );

void load_topboard()
{
  int i;   
  FILE *fp;

  if ((fp = fopen("../txt/topboard.txt", "r")) == NULL)
  {
    log_string("Error: topboard.txt not found!");
    exit(1);
  }
  top_board[0].name = " ";
  top_board[0].pkscore = 0;
  for (i = 1; i <= MAX_TOP_PLAYERS; i++)
  {
    top_board[i].name = fread_string(fp);
    top_board[i].pkscore = fread_number(fp);
  }
  fclose(fp);
}

void update_top_board( CHAR_DATA *ch )
{
  int i,j;
  bool changed = FALSE;

  if (IS_NPC(ch)) return;
  if (ch->level > 6) return;

  for (i = 1; i <= MAX_TOP_PLAYERS && !changed; i++)
  {
    if (!str_cmp(ch->name,top_board[i].name))
    {
      if (get_ratio(ch) != top_board[i].pkscore) top_board[i].pkscore = get_ratio(ch);
      changed = TRUE;
      sort_top_board();
      i = MAX_TOP_PLAYERS+1;
    }
  }
  for (i = 1; i <= MAX_TOP_PLAYERS && !changed; i++)
  {
    if (get_ratio(ch) > top_board[i].pkscore)
    {
      for (j = MAX_TOP_PLAYERS; j > i; j--)
      {
        top_board[j].pkscore = top_board[j-1].pkscore;
        free_string(top_board[j].name);
        top_board[j].name = str_dup(top_board[j-1].name);
      }
      top_board[i].pkscore = get_ratio(ch);
      free_string(top_board[i].name);
      top_board[i].name = str_dup(ch->name);
      changed = TRUE;
      i = MAX_TOP_PLAYERS+1;
    }
  }
  if (changed) save_topboard();
  return;
}

void sort_top_board()
{
  int i, tempscore;
  bool sorting = FALSE;
  char buf[MAX_STRING_LENGTH];

  for (i = 1; i < MAX_TOP_PLAYERS; i++)
  {
    if (top_board[i].pkscore < top_board[i+1].pkscore)
    {
      tempscore = top_board[i].pkscore;
      sprintf (buf, "%s", top_board[i].name);
      free_string(top_board[i].name);
      top_board[i].name = str_dup(top_board[i+1].name);
      top_board[i].pkscore = top_board[i+1].pkscore;
      free_string(top_board[i+1].name);
      top_board[i+1].name = str_dup(buf);;
      top_board[i+1].pkscore = tempscore;
      sorting = TRUE;
    }
  }
  if (sorting) sort_top_board();
  else return;
}

void save_topboard()
{
  FILE *fp;
  int i;

  if ((fp = fopen("../txt/topboard.txt","w")) == NULL)
  {
    log_string("Error writing to topboard.txt");
    return;
  }
  for (i=1;i<=MAX_TOP_PLAYERS;i++)
  {
    fprintf(fp, "%s~\n", top_board[i].name); 
    fprintf(fp, "%d\n", top_board[i].pkscore);
  }
  fclose (fp);
}

void load_leaderboard()
{
  FILE *fp;

  if ((fp = fopen("../txt/leader.txt", "r")) == NULL)
  {   
    log_string("Error: leader.txt not found!");
    exit(1);
  }
  leader_board.bestpk_name = fread_string(fp);  
  leader_board.bestpk_number = fread_number(fp);
  leader_board.pk_name = fread_string(fp);
  leader_board.pk_number = fread_number(fp);
  leader_board.pd_name = fread_string(fp);
  leader_board.pd_number = fread_number(fp);
  leader_board.mk_name = fread_string(fp);
  leader_board.mk_number = fread_number(fp);
  leader_board.md_name = fread_string(fp);
  leader_board.md_number = fread_number(fp);
  leader_board.tt_name = fread_string(fp);
  leader_board.tt_number = fread_number(fp);
  leader_board.qc_name = fread_string(fp);
  leader_board.qc_number = fread_number(fp);
  fclose(fp);
}

void save_leaderboard()
{
  FILE *fp;

  if ((fp = fopen("../txt/leader.txt","w")) == NULL)
  {
    log_string("Error writing to leader.txt");
    return;
  }
  fprintf(fp, "%s~\n", leader_board.bestpk_name); 
  fprintf(fp, "%d\n", leader_board.bestpk_number);
  fprintf(fp, "%s~\n", leader_board.pk_name);
  fprintf(fp, "%d\n", leader_board.pk_number);
  fprintf(fp, "%s~\n", leader_board.pd_name);     
  fprintf(fp, "%d\n", leader_board.pd_number);
  fprintf(fp, "%s~\n", leader_board.mk_name);
  fprintf(fp, "%d\n", leader_board.mk_number);
  fprintf(fp, "%s~\n", leader_board.md_name);
  fprintf(fp, "%d\n", leader_board.md_number);
  fprintf(fp, "%s~\n", leader_board.tt_name);
  fprintf(fp, "%d\n", leader_board.tt_number);
  fprintf(fp, "%s~\n", leader_board.qc_name);
  fprintf(fp, "%d\n", leader_board.qc_number);
  fclose (fp);
}

void do_leader( CHAR_DATA *ch, char *argument )
{    
  char buf[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;

  stc("#r-==--==#L**#r==--==#L**#r==--==#L**#r== #GLEADER BOARD #r==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==#n\n\r\n\r",ch);

  send_to_char("   #oMr. Best PK'er   #C--->    ",ch);
  sprintf(buf, "#G%-13s",leader_board.bestpk_name);
  send_to_char(buf,ch);
  sprintf(buf, " #owith a #G%d #oPK score#n\n\r",leader_board.bestpk_number);
  send_to_char(buf,ch);

  send_to_char("   #oMr. Deadly       #C--->    ",ch);
  sprintf(buf, "#G%-13s",leader_board.pk_name);
  send_to_char(buf,ch);
  sprintf(buf, " #owith #G%d #opkills#n\n\r",leader_board.pk_number);
  send_to_char(buf,ch);

  send_to_char("   #oMr. Pathetic     #C--->    ",ch);
  sprintf(buf, "#G%-13s",leader_board.pd_name);
  send_to_char(buf,ch);
  sprintf(buf, " #owith #G%d #opdeaths#n\n\r",leader_board.pd_number);
  send_to_char(buf,ch);
  
  send_to_char("   #oMr. Slayer       #C--->    ",ch);
  sprintf(buf, "#G%-13s",leader_board.mk_name);
  send_to_char(buf,ch);
  sprintf(buf, " #owith #G%d #omkills#n\n\r",leader_board.mk_number);
  send_to_char(buf,ch);
  
  send_to_char("   #oMr. Wimpy        #C--->    ",ch);
  sprintf(buf, "#G%-13s",leader_board.md_name);
  send_to_char(buf,ch);
  sprintf(buf, " #owith #G%d #omdeaths#n\n\r",leader_board.md_number);
  send_to_char(buf,ch);
  
  send_to_char("   #oMr. No life      #C--->    ",ch);
  sprintf(buf, "#G%-13s",leader_board.tt_name);
  send_to_char(buf,ch);
  sprintf(buf, " #owith #G%d #oHours played#n\n\r",leader_board.tt_number);
  send_to_char(buf,ch);
  
  send_to_char("   #oMr. Questor      #C--->    ",ch);
  sprintf(buf, "#G%-13s",leader_board.qc_name);
  send_to_char(buf,ch);
  sprintf(buf, " #owith #G%d #oQuests completed#n\n\r",leader_board.qc_number);
  send_to_char(buf,ch);

  stc("\n\r#r-==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==#n\n\r",ch);
  return;
}

void check_leaderboard( CHAR_DATA *ch )
{
  bool changed = FALSE;

  if (IS_NPC(ch)) return;
  if (ch->level > 6) return;
  if (get_ratio(ch) > leader_board.bestpk_number || !str_cmp(leader_board.bestpk_name , ch->name))
  {
    leader_board.bestpk_number = get_ratio(ch);
    free_string(leader_board.bestpk_name);
    leader_board.bestpk_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (ch->mdeath > leader_board.md_number)
  {
    leader_board.md_number = ch->mdeath;
    free_string(leader_board.md_name);
    leader_board.md_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (ch->mkill > leader_board.mk_number)
  {
    leader_board.mk_number = ch->mkill;
    free_string(leader_board.mk_name);
    leader_board.mk_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (ch->pkill > leader_board.pk_number)
  {
    leader_board.pk_number = ch->pkill;
    free_string(leader_board.pk_name);
    leader_board.pk_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (ch->pdeath > leader_board.pd_number)
  {
    leader_board.pd_number = ch->pdeath;
    free_string(leader_board.pd_name);
    leader_board.pd_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (ch->pcdata->questsrun > leader_board.qc_number)
  {
    leader_board.qc_number = ch->pcdata->questsrun;
    free_string(leader_board.qc_name);
    leader_board.qc_name = str_dup(ch->name);
    changed = TRUE;
  }
  if ((get_age(ch)-17)*2 > leader_board.tt_number)
  {
    leader_board.tt_number = (get_age(ch)-17)*2;
    free_string(leader_board.tt_name);
    leader_board.tt_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (changed) save_leaderboard();
  return;
}

void do_leaderclear(CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (ch->level < 7) return;
  free_string(leader_board.bestpk_name);
  leader_board.bestpk_name = str_dup("Noone");
  leader_board.bestpk_number = 0;
  free_string(leader_board.pk_name);
  leader_board.pk_name = str_dup("Noone");
  leader_board.pk_number = 0;
  free_string(leader_board.pd_name);
  leader_board.pd_name = str_dup("Noone");
  leader_board.pd_number = 0;
  free_string(leader_board.md_name);
  leader_board.md_name = str_dup("Noone");
  leader_board.md_number = 0;
  free_string(leader_board.mk_name);
  leader_board.mk_name = str_dup("Noone");
  leader_board.mk_number = 0;
  free_string(leader_board.qc_name);
  leader_board.qc_name = str_dup("Noone");
  leader_board.qc_number = 0;
  free_string(leader_board.tt_name);
  leader_board.tt_name = str_dup("Noone");
  leader_board.tt_number = 0;
  save_leaderboard();
  send_to_char("Leader board cleared.\n\r",ch);
  return;
}

void do_top( CHAR_DATA *ch, char *argument )
{
  char buf[MAX_STRING_LENGTH];
  int i;

  if (IS_NPC(ch)) return;   
 
  stc("#r==--==#L**#r== #GTOP PKSCORE #r==#L**#r==--==#n\n\r\n\r",ch);
  for (i = 1; i <= MAX_TOP_PLAYERS; i++)
  {
    sprintf(buf," %2d.  %-15s      %-4d\n\r", i, top_board[i].name, top_board[i].pkscore);
    send_to_char(buf,ch);
  }
  sprintf(buf,"\n\r   #GYou #0:#G                %-4d\n\r",get_ratio(ch));
  send_to_char(buf,ch);
  stc("\n\r#r==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==--==#n\n\r",ch);
  return;
}

void do_topclear( CHAR_DATA *ch, char *argument )
{
  int i;

  if (IS_NPC(ch)) return;
  if (ch->level < MAX_LEVEL) return;
  for (i = 1; i <= MAX_TOP_PLAYERS; i++)  
  {
    top_board[i].pkscore = 0;
    free_string(top_board[i].name);
    top_board[i].name = str_dup("Noone");
  }
  save_topboard();
  send_to_char("Top cleared.\n\r",ch);
  return;
}
