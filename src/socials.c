/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license._doc' as well the Merc       *
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
 * The social table.
 * Add new socials here.
 * Alphabetical order is not required.
 */

const	struct	social_type	social_table [] =
{
    {
	"accuse",
	"Accuse whom?",
	"$n is in an accusing mood.",
	"You look accusingly at $M.",
	"$n looks accusingly at $N.",
	"$n looks accusingly at you.",
	"You accuse yourself.",
	"$n seems to have a bad conscience."
    },

    {
	"ack",
	"You gasp and say 'ACK!' at your mistake.",
	"$n ACKS at $s big mistake.",
	"You ACK $M.",
	"$n ACKS $N.",
	"$n ACKS you.",
	"You ACK yourself.",
	"$n ACKS $mself.  Must be a bad day."
    },
	
    {
	"apologize",
	"You apologize for your behavior.",
	"$n apologizes for $s rude behavior.",
	"You apologize to $M.",
	"$n apologizes to $N.",
	"$n apologizes to you.",
	"You apologize to yourself.",
	"$n apologizes to $mself.  Hmmmm."
    },

    {
	"applaud",
	"Clap, clap, clap.",
	"$n gives a round of applause.",
	"You clap at $S actions.",
	"$n claps at $N's actions.",
	"$n gives you a round of applause.  You MUST'VE done something good!",
	"You applaud at yourself.  Boy, are we conceited!",
	"$n applauds at $mself.  Boy, are we conceited!"
    },

    {
	"bark",
	"Woof!  Woof!",
	"$n barks like a dog.",
	"You bark at $M.",
	"$n barks at $N.",
	"$n barks at you.",
	"You bark at yourself.  Woof!  Woof!",
	"$n barks at $mself.  Woof!  Woof!"
    },

    {
	"bearhug",
	"You hug a grizzly bear.",
	"$n hugs a flea-infested grizzly bear.",
	"You bearhug $M.",
	"$n bearhugs $N.  Some ribs break.",
	"$n bearhugs you.  Wonder what's coming next?",
	"You bearhug yourself.",
	"$n bearhugs $mself."
    },

    {
	"beg",
	"You beg the for mercy.",
	"$n falls to the ground and begs for mercy.",
	"You desperately beg $M for mercy.",
	"$n begs you for mercy.",
	"$n begs $N for mercy!",
	"Begging yourself for mercy doesn't help.",
	"$n begs $mself for mercy."
    },

    {
	"whimper",
	"You whimper softly.",
	"$n whimpers pathetically.",
	"You look at $N and whimper sadly.",
	"$n looks at $N and whimpers sadly.",
	"$n looks at you and whimpers sadly.",
	"You whimper to yourself.",
	"$n whimpers to $mself."
    },

    {
	"behead",
	"You look around for some heads to cut off.",
	"$n looks around for some heads to cut off.",
	"You grin evilly at $N and brandish your weapon.",
	"$n grins evilly at $N, while branding $s weapon!",
	"$n grins evilly at you, brandishing $s weapon.",
	"I really don't think you want to do that...",
	"$n is so desperate for exp that $e tries to decapitate $mself!"
    },

    {
	"blush",
	"Your cheeks are burning.",
	"$n blushes.",
	"You get all flustered up seeing $M.",
	"$n blushes as $e sees $N here.",
	"$n blushes as $e sees you here.  Such an effect on people!",
	"You blush at your own folly.",
	"$n blushes as $e notices $s boo-boo."
    },

    {
	"boggle",
	"You boggle at all the loonies around you.",
	"$n boggles at all the loonies around $m.",
	"You boggle at $S ludicrous idea.",
	"$n boggles at $N's ludicrous idea.",
	"$n boggles at your ludicrous idea.",
	"BOGGLE.",
	"$n wonders what BOGGLE means."
    },

    {
	"bonk",
	"BONK.",
	"$n spells 'potato' like Dan Quayle: 'B-O-N-K'.",
	"You bonk $M for being a numbskull.",
	"$n bonks $N.  What a numbskull.",
	"$n bonks you.  BONK BONK BONK!",
	"You bonk yourself.",
	"$n bonks $mself."
    },

    {
      "bounce",
      "You curl into a ball and bounce madly around the room.",
      "$n leaps into the air and madly bounces up and down like a crazed hobbit after too much pipeweed.",
      "You jump on $N and bounce off $S head!",
      "$n screams a warcry as $e leaps at $N's head and bounces off.",
      "You try in vain to avoid $n as $e vaults towards you and bounces off your head, nearly knocking you over.",
      "You pull yourself into the air by the seat of your pants, then bounce yourself off the floor.",
      "$n heaves $mself into the sky and bounces $mself off the floor!"
    },

    {
	"bow",
	"You bow deeply.",
	"$n bows deeply.",
	"You bow before $M.",
	"$n bows before $N.",
	"$n bows before you.",
	"You kiss your toes.",
	"$n folds up like a jack knife and kisses $s own toes."
    },

    {
	"burp",
	"You burp loudly.",
	"$n burps loudly.",
	"You burp loudly to $M in response.",
	"$n burps loudly in response to $N's remark.",
	"$n burps loudly in response to your remark.",
	"You burp at yourself.",
	"$n burps at $mself.  What a sick sight."
    },

    {
	"cackle",
	"You throw back your head and cackle with insane glee!",
	"$n throws back $s head and cackles with insane glee!",
	"You cackle gleefully at $N",
	"$n cackles gleefully at $N.",
	"$n cackles gleefully at you.  Better keep your distance from $m.",
	"You cackle at yourself.  Now, THAT'S strange!",
	"$n is really crazy now!  $e cackles at $mself."
    },

    {
	"chuckle",
	"You chuckle politely.",
	"$n chuckles politely.",
	"You chuckle at $S joke.",
	"$n chuckles at $N's joke.",
	"$n chuckles at your joke.",
	"You chuckle at your own joke, since no one else would.",
	"$n chuckles at $s own joke, since none of you would."
    },

    {
	"comfort",
	"Do you feel uncomfortable?",
	NULL,
	"You comfort $M.",
	"$n comforts $N.",
	"$n comforts you.",
	"You make a vain attempt to comfort yourself.",
	"$n has no one to comfort $m but $mself."
    },

    {
	"cough",
	"You cough to clear your throat and eyes and nose and....",
	"$n coughs loudly.",
	"You cough loudly.  It must be $S fault, $E gave you this cold.",
	"$n coughs loudly, and glares at $N, like it is $S fault.",
	"$n coughs loudly, and glares at you.  Did you give $m that cold?",
	"You cough loudly.  Why don't you take better care of yourself?",
	"$n coughs loudly.  $n should take better care of $mself." 
    },

    {
	"cry",
	"Waaaaah ...",
	"$n bursts into tears.",
	"You cry on $S shoulder.",
	"$n cries on $N's shoulder.",
	"$n cries on your shoulder.",
	"You cry to yourself.",
	"$n sobs quietly to $mself."
    },

    {
	"cuddle",
	"Whom do you feel like cuddling today?",
	NULL,
	"You cuddle $M.",
	"$n cuddles $N.",
	"$n cuddles you.",
	"You must feel very cuddly indeed ... :)",
	"$n cuddles up to $s shadow.  What a sorry sight."
    },

    {
	"curse",
	"You swear loudly for a long time.",
	"$n swears: @*&^%@*&!",
	"You swear at $M.",
	"$n swears at $N.",
	"$n swears at you!  Where are $s manners?",
	"You swear at your own mistakes.",
	"$n starts swearing at $mself.  Why don't you help?"
    },

    {
	"curtsey",
	"You curtsey to your audience.",
	"$n curtseys gracefully.",
	"You curtsey to $M.",
	"$n curtseys gracefully to $N.",
	"$n curtseys gracefully for you.",
	"You curtsey to your audience (yourself).",
	"$n curtseys to $mself, since no one is paying attention to $m."
    },

    {
	"dance",
	"Feels silly, doesn't it?",
	"$n tries to break dance, but nearly breaks $s neck!",
	"You sweep $M into a romantic waltz.",
	"$n sweeps $N into a romantic waltz.",
	"$n sweeps you into a romantic waltz.",
	"You skip and dance around by yourself.",
	"$n dances a pas-de-une."
    },

    {
	"doh",
	"You say, 'Doh!!' and hit your forehead.  What an idiot you are!",
	"$n hits $mself in the forehead and says, 'Doh!!!'",
	"You say, 'Doh!!' and hit your forehead.  What an idiot you are!",
	"$n hits $mself in the forehead and says, 'Doh!!!'",
	"$n hits $mself in the forehead and says, 'Doh!!!'",
	"You hit yourself in the forehead and say, 'Doh!!!'",
	"$n hits $mself in the forehead and says, 'Doh!!!'"    
    },
    
    {
	"eyebrow",
	"You raise an eyebrow.",
	"$n raises an eyebrow.",
	"You raise an eyebrow at $M.",
	"$n raises an eyebrow at $N.",
	"$n raises an eyebrow at you.",
	"You raise an eyebrow at yourself.  That hurt!",
	"$n raises an eyebrow at $mself.  That must have hurt!"
    },

    {
	"flirt",
	"Wink wink!",
	"$n flirts -- probably needs a date, huh?",
	"You flirt with $M.",
	"$n flirts with $N.",
	"$n wants you to show some interest and is flirting with you.",
	"You flirt with yourself.",
	"$n flirts with $mself.  Hoo boy."
    },
    
    {
	"fondle",
	"Who needs to be fondled?",
	NULL,
	"You fondly fondle $M.",
	"$n fondly fondles $N.",
	"$n fondly fondles you.",
	"You fondly fondle yourself, feels funny doesn't it?",
	"$n fondly fondles $mself - this is going too far!!"
    },

    {
	"french",
	"Kiss whom?",
	NULL,
	"You give $N a long and passionate kiss.",
	"$n kisses $N passionately.",
	"$n gives you a long and passionate kiss.",
	"You gather yourself in your arms and try to kiss yourself.",
	"$n makes an attempt at kissing $mself."
    },

    {
	"frown",
	"What's bothering you ?",
	"$n frowns.",
	"You frown at what $E did.",
	"$n frowns at what $E did.",
	"$n frowns at what you did.",
	"You frown at yourself.  Poor baby.",
	"$n frowns at $mself.  Poor baby."
    },

    {
	"gasp",
	"You gasp in astonishment.",
	"$n gasps in astonishment.",
	"You gasp as you realize what $E did.",
	"$n gasps as $e realizes what $N did.",
	"$n gasps as $e realizes what you did.",
	"You look at yourself and gasp!",
	"$n takes one look at $mself and gasps in astonisment!"
    },

    {
	"giggle",
	"You giggle.",
	"$n giggles.",
	"You giggle in $S's presence.",
	"$n giggles at $N's actions.",
	"$n giggles at you.  Hope it's not contagious!",
	"You giggle at yourself.  You must be nervous or something.",
	"$n giggles at $mself.  $e must be nervous or something."
    },

    {
	"grumble",
	"You grumble.",
	"$n grumbles.",
	"You grumble to $M.",
	"$n grumbles to $N.",
	"$n grumbles to you.",
	"You grumble under your breath.",
	"$n grumbles under $s breath."
    },

    {
	"highfive",
	"You jump in the air...oops, better get someone else to join you.",
	"$n jumps in the air by $mself.  Is $e a cheerleader, or just daft?",
	"You jump in the air and give $M a big highfive!",
	"$n jumps in the air and gives $N a big highfive!",
	"$n jumps in the air and gives you a big highfive!",
	"You jump in the air and congratulate yourself!",
	"$n jumps in the air and gives $mself a big highfive!  Wonder what $e did?"
    },

    {
	"hug",
	"Hug whom?",
	NULL,
	"You hug $M.",
	"$n hugs $N.",
	"$n hugs you.",
	"You hug yourself.",
	"$n hugs $mself in a vain attempt to get friendship."
    },

    {
	"kiss",
	"Isn't there someone you want to kiss?",
	NULL,
	"You kiss $M.",
	"$n kisses $N.",
	"$n kisses you.",
	"All the lonely people :(",
	NULL
    },

    {
	"laugh",
	"You laugh.",
	"$n laughs.",
	"You laugh at $N mercilessly.",
	"$n laughs at $N mercilessly.",
	"$n laughs at you mercilessly.  Hmmmmph.",
	"You laugh at yourself.  I would, too.",
	"$n laughs at $mself.  Let's all join in!!!"
    },

    {
	"lick",
	"You lick your lips and smile.",
	"$n licks $s lips and smiles.",
	"You lick $M.",
	"$n licks $N.",
	"$n licks you.",
	"You lick yourself.",
	"$n licks $mself - YUCK."
    },

    {
	"nod",
	"You nod affirmative.",
	"$n nods affirmative.",
	"You nod in recognition to $M.",
	"$n nods in recognition to $N.",
	"$n nods in recognition to you.  You DO know $m, right?",
	"You nod at yourself.  Are you getting senile?",
	"$n nods at $mself.  $e must be getting senile."
    },

    {
	"nuzzle",
	"Nuzzle whom?",
	NULL,
	"You nuzzle $S neck softly.",
	"$n softly nuzzles $N's neck.",
	"$n softly nuzzles your neck.",
	"I'm sorry, friend, but that's impossible.",
	NULL
    },

    {
        "ooo",
        "You go ooOOooOOooOOoo.",
        "$n says, 'ooOOooOOooOOoo.'",
        "You go ooOOooOOooOOoo.",
        "$n says, 'ooOOooOOooOOoo.'",
        "$n thinks of you and says, 'ooOOooOOooOOoo.'",
        "You go ooOOooOOooOOoo.",
        "$n says, 'ooOOooOOooOOoo.'"
    },

    {
	"rofl",
	"You roll on the floor laughing hysterically.",
	"$n rolls on the floor laughing hysterically.",
	"You laugh your head off at $S remark.",
	"$n rolls on the floor laughing at $N's remark.",
	"$n can't stop laughing at your remark.",
	"You roll on the floor and laugh at yourself.",
	"$n laughs at $mself.  Join in the fun."
    },
    
    {
	"sad",
	"You put on a glum expression.",
	"$n looks particularly glum today.  *sniff*",
	"You give $M your best glum expression.",
	"$n looks at $N glumly.  *sniff*  Poor $n.",
	"$n looks at you glumly.  *sniff*   Poor $n.",
	"You bow your head and twist your toe in the dirt glumly.",
	"$n bows $s head and twists $s toe in the dirt glumly."
    },

    {
	"shake",
	"You shake your head.",
	"$n shakes $s head.",
	"You shake $S hand.",
	"$n shakes $N's hand.",
	"$n shakes your hand.",
	"You are shaken by yourself.",
	"$n shakes and quivers like a bowl full of jelly."
    },

    {
	"shrug",
	"You shrug.",
	"$n shrugs helplessly.",
	"You shrug in response to $s question.",
	"$n shrugs in response to $N's question.",
	"$n shrugs in respopnse to your question.",
	"You shrug to yourself.",
	"$n shrugs to $mself.  What a strange person."
    },

    {
	"sigh",
	"You sigh.",
	"$n sighs loudly.",
	"You sigh as you think of $M.",
	"$n sighs at the sight of $N.",
	"$n sighs as $e thinks of you.  Touching, huh?",
	"You sigh at yourself.  You MUST be lonely.",
	"$n sighs at $mself.  What a sorry sight."
    },

    {
	"slap",
	"Slap whom?",
	NULL,
	"You slap $M across the face.",
	"$n slaps $N across the face for $s stupidity.",
	"$n slaps you across the face. Don't take it from $m.",
	"You slap yourself.  You deserve it.",
	"$n slaps $mself.  Why don't you join in?"
    },

    {
	"smile",
	"You smile happily.",
	"$n smiles happily.",
	"You smile at $M.",
	"$n beams a smile at $N.",
	"$n smiles at you.",
	"You smile at yourself.",
	"$n smiles at $mself."
    },

    {
	"smirk",
	"You smirk.",
	"$n smirks.",
	"You smirk at $S saying.",
	"$n smirks at $N's saying.",
	"$n smirks at your saying.",
	"You smirk at yourself.  Okay ...",
	"$n smirks at $s own 'wisdom'."
    },

    {
	"tap",
	"You tap your foot impatiently.",
	"$n taps $s foot impatiently.",
	"You tap your foot impatiently.  Will $E ever be ready?",
	"$n taps $s foot impatiently as $e waits for $N.",
	"$n taps $s foot impatiently as $e waits for you.",
	"You tap yourself on the head.  Ouch!",
	"$n taps $mself on the head."
    },

    {
	"thank",
	"Thank you too.",
	NULL,
	"You thank $N heartily.",
	"$n thanks $N heartily.",
	"$n thanks you heartily.",
	"You thank yourself since nobody else wants to !",
	"$n thanks $mself since you won't."
    },

    {
	"tickle",
	"Whom do you want to tickle?",
	NULL,
	"You tickle $N.",
	"$n tickles $N.",
	"$n tickles you - hee hee hee.",
	"You tickle yourself, how funny!",
	"$n tickles $mself."
    },

    {
	"wave",
	"You wave.",
	"$n waves happily.",
	"You wave goodbye to $N.",
	"$n waves goodbye to $N.",
	"$n waves goodbye to you.  Have a good journey.",
	"Are you going on adventures as well?",
	"$n waves goodbye to $mself."
    },

    {
	"wiggle",
	"Your wiggle your bottom.",
	"$n wiggles $s bottom.",
	"You wiggle your bottom toward $M.",
	"$n wiggles $s bottom toward $N.",
	"$n wiggles $s bottom toward you.",
	"You wiggle about like a fish.",
	"$n wiggles about like a fish."
    },

    {
        "",
	NULL, NULL, NULL, NULL, NULL, NULL, NULL
    }
};



/*
 * The X-social table.
 * Add new X-socials here.
 * Alphabetical order is not required.
 */
const	struct	xsocial_type	xsocial_table [] =
{

    {
	"x-earlobe",
	"On whom do you wish to do this?",
	NULL,
	"You gently tug on $M earlobe with your teeth.",
	"$n gently tugs on $N's earlobe with $s teeth.",
	"$n gently tugs on your earlobe with $s teeth.",
	"Not on yourself!",
	NULL,
	0, 0, 0, 1, 1, FALSE
    },

{
"x-kneel",
"Drop to your knees before who?",
NULL,
"You drop to your knees before $M, begging for mercy.",
"$n drops to $s knees before $N, begging for $M mercy.",
"$n drops to $s knees before you, begging for mercy.",
"Nope.",
NULL,
0, 0, 0, 7, 7, FALSE
},
    {
	"x-french",
	"On whom do you wish to do this?",
	NULL,
	"You give $M a deep throbbing kiss, rolling your tongue around $S.",
	"$n gives $N a deep throbbing kiss.",
	"$n gives you a deep throbbing kiss, rolling $s tongue around yours.",
	"Not on yourself!",
	NULL,
	0, 0, 0, 1, 1, FALSE
    },

    {
	"x-kissneck",
	"On whom do you wish to do this?",
	NULL,
	"You slowly and softly kiss and nuzzle $M neck.",
	"$n slowly and softly kisses and nuzzles $N's neck.",
	"$n slowly and softly kisses and nuzzles your neck.",
	"Not on yourself!",
	NULL,
	0, 0, 0, 1, 1, FALSE
    },

{
"x-wcream",
"On whom do you wish to do this?",
NULL,
"You spray whipped cream all over $M body, licking it up with your tongue.",
"$n coats $N's body in whipped cream and then licks it up with their tongue.",
"$n covers your body in whipped cream, and you moan as it gets licked up with their tongue.",
"No.",
NULL,
0, 0, 0, 5, 5, FALSE
},
{
"x-ice",
"On who?",
NULL,
"You run a cold piece of ice down $M back, following up with your tongue.",
"$n runs a piece of ice over $N's back, following up with their tongue.",
"$n runs a piece of ice over your burning hot flesh, following up with their tongue.",
"No.",
NULL,
0, 0, 0, 6, 8, FALSE
},

{
"x-whip",
"Who's been bad?",
NULL,
"You smirk slightly as you crack a whip over $N's tender skin, making $M whimper.",
"$n grins slightly as he whips $N's tender flesh, making $M whimper.",
"$n grins wickedly as they whip your tender flesh, making you whimper slightly in fear.",
"No.",
NULL,
0, 0, 0, 9, 2, FALSE
},
    {
	"x-moan",
	"On whom do you wish to do this?",
	NULL,
	"You start moaning 'Oh $N...oh yes...don't stop...mmMMmm...'",
	"$n starts moaning 'Oh $N...oh yes...don't stop...mmMMmm...'",
	"$n starts moaning 'Oh $N...oh yes...don't stop...mmMMmm...'",
	"Not on yourself!",
	NULL,
	0, 2, 0, 1, 1, FALSE
    },

    {
	"x-nuttella",
	"On whom do you wish to do this?",
	NULL,
	"You cover $N's naked flesh with a popular choc spread, before licking it off.",
	"$n smears a chocolate spread over $N's body, licking $M clean with relish",
	"$n smears a popular choc spread on your naked flesh, licking it off you",
	"Not on yourself!",
	NULL,
	0, 0, 0, 5, 20, FALSE
    },

    {
	"x-stroke",
	"On whom do you wish to do this?",
	NULL,
	"You lightly run your fingers along the insides of $S thighs.",
	"$n lightly runs $s fingers along the insides of $N's thighs.",
	"$n lightly runs $s fingers along the insides of your thighs.",
	"Not on yourself!",
	NULL,
	0, 0, 0, 5, 10, FALSE
    },

    {
	"x-tender",
	"On whom do you wish to do this?",
	NULL,
	"You run your fingers through $S hair and kiss $M tenderly on the lips.",
	"$n runs $s fingers through $N's hair and kisses $M tenderly on the lips.",
	"$n runs $s fingers through your hair and kisses you tenderly on the lips.",
	"Not on yourself!",
	NULL,
	0, 0, 0, 1, 1, FALSE
    },

    {
	"x-tie",
	"On whom do you wish to do this?",
	NULL,
	"You tie $N to the bed posts!",
	"$n ties $N's body, fixing $M helplessly on the bed.  Oh!! Kinky!",
	"$n ties you to the bed post, leaving you completely at $s mercy",
	"Not on yourself!",
	NULL,
	0, 0, 0, 15, 10, FALSE
    },

	{
	"x-gag",
	"Gag whom?",
	NULL,
	"You tear off a small strip of $N's clothing, and stuff it into their mouth, tying it back behind their head to secure it.",
	"$n reaches down and tears off a small strip of $N's clothing, stuffing it into $S mouth and tying it behind $S head, effectively gagging $S.",
	"$n tears off a strip of your clothing, and as you look up at $s, $s shoves it into your mouth and ties it behind your head, gagging you.",
	"No.",
	NULL,
	0, 0, 0, 5, 2, FALSE
	},

	{
	"x-blindfold",
	"Blindfold who?",
	NULL,
	"You take out a black silk blindfold, and place it over $N's eyes, tying it securely.",
	"$n takes out a black silk blindfold, and places it over $N's eyes, tying it securely behind $S.",
	"$n takes out a black silk blindfold, and grins as $s places it over your eyes, tying it behind your head.",
	"No.",
	NULL,
	0, 0, 0, 7, 2, FALSE
	},

	{
    "x-knife",
	"On whom do you wish to do this?",
    NULL,
    "You lightly run a cool knife over $N's skin, letting it slide across their body and make weak cuts in their flesh.",
    "$n lightly traces a knife over $N's skin, smiling as it makes a few shallow cuts, drawing a bit of blood.",
    "$n lightly runs a knife over your skin, and you shiver a bit as it makes a few shallow cuts, drawing a bit of your blood.",
    "No.",
    NULL,
    0, 0, 0, 15, 3, FALSE
	}, 

    {
	"x-withdraw",
	"On whom do you wish to do this?",
	NULL,
	"You gently pull yourself free of $M.",
	"$n gently pulls $mself free of $N.",
	"$n gently pulls $mself free of you.",
	"Not on yourself!",
	NULL,
	0, 1, 0, 0, 0, FALSE
    },

	{
	"xl-candle",
	"On who?",
	NULL,
	"You produce a thick white candle, and you step behind $N with it, grinning a bit as you run it up and down her pussy lips.  After a moment you push it in and twist it, starting to slowly pump it in and out.",
	"$n produces a thick white candle, and walks behind $N, holding it in her hands.  She grins a bit as she pushes it to the girl's wet pussy lips, and pushes it in, twisting it as she begins to pump it in and out.",
	"$n produces a thick white candle, and steps behind you as you whimper a bit.  After a moment, you feel it running up and down your cunt lips, and you let out a moan as it slips inside you, twisting as it slides in and out.",
	"No.",
	NULL,
	3, 0, 0, 17, 39, FALSE
	},
{
"xl-afinger",
"Use this on who?",
NULL,
"You take your finger to $N's mouth and have her lick it before taking it to her puckered ass and slowly slipping it in.",
"$N sucks lightly on $n's finger a moment before she takes it between $N's ass cheeks and pushes it in to the knuckle, twisting.",
"$n brings her finger to your lips, and you suck on it a bit before she pulls it away.  A moment later, she presses it to your tight little ass and pushes it in, twisting as you whimper softly.",
"No.",
NULL,
3, 0, 0, 20, 25, FALSE
},

	{
    "xl-finger",
	"On whom do you wish to do this?",
    NULL,
    "You spread $N's thighs and slip your finger between them, tracing it over her lips a few times before pushing it past and into her hot and wet pink folds.",
    "$n spreads $N's thighs and plunges her finger into her twat, pumping it out a few times as sweet, sticky juice spills over it.",
    "$n spreads your legs and begins to trace her finger over your moistened lips, and you let out a soft moan as she slips it past them and into your hot snatch, plunging it in and out as it collects your juices.",
    "No.",
    NULL,
    3, 0, 250, 19, 30, FALSE
	},

	{
	"xl-gag",
	"Gag who?",
	NULL,
	"You pick up your soaked panties, and wad them up into a ball, stuffing them into $N's mouth to gag her.",
	"$n picks up her drenched panties, wadding them into a ball and stuffing them into $N's mouth, gagging her.",
	"$n picks up her panties, soaked through with her cunt juice, and wads them into a ball, stuffing them into your mouth to gag you.",
	"No.",
	NULL,
	3, 0, 0, 7, 9, FALSE
	},

	{
    "xl-stroke",
	"On whom do you wish to do this?",
    NULL,
    "You slip your finger between $N's legs and gently stroke her throbbing clit, making her squirm and moan.",
    "$n slips her finger between $N's damp thighs and begins to softly stroke her clit, making the poor girl squirm and moan in pleasure.",
    "$n slips her finger between your thighs and begins to softly stroke your throbbing clit, making you shut your eyes and moan in pleasure.",
    "No.",
    NULL,
    3, 0, 250, 10, 17, FALSE
	},

	{
    "xl-undress",
	"On whom do you wish to do this?",
    NULL,
    "You reach back and unhook your bra, letting it slide to the floor with your panties.",
    "$n reaches back and unhooks her bra, letting it fall to the floor along with her panties.",
    "$n reaches back and unhooks her bra, letting it fall to the floor along with her panties.",
    "Pick a valid target.",
    NULL,
    3, 0, 250, 15, 15, FALSE
	}, 

	{
    "xl-lick",
	"On whom do you wish to do this?",
    NULL,
    "You bury your face between $N's thighs, letting your tongue slide over her swollen pussy lips and gently tease her clit.",
    "$n pushes $N to the floor and buries her face between her thighs, letting her tongue slide over $N's pussy lips and caress her clit.",
    "$n pushes you to the floor, burying her face between your damp thighs as she lets her tongue caress your swollen pussy lips, and you squirm as it lightly runs over your clit.",
    "No.",
    NULL,
    3, 0, 250, 25, 37, FALSE 
	},

	{ 
    "xl-69",
	"On whom do you wish to do this?",
    NULL, 
    "You lie down on your back and motion for $N to straddle your face, moaning as she licks at you while you hungrily devour her wet snatch.", 
    "$N lowers her pussy over $n's face and places her face between her thighs, both of them gasping as their tongues dart in and out of each other.",
    "$n pulls you on top of her, placing your hot, wet pussy right over her face, and you do the same, both of you panting and moaning as you devour each other.", 
    "No.", 
    NULL, 
    3, 1, 250, 40, 40, FALSE 
	},

	{ 
    "xl-breasts",
	"On whom do you wish to do this?",
    NULL, 
    "You lean forward and place your hands over $N's supple breasts, kneading them softly as she moans.",
    "$n places her hands over $N's soft breasts and begins to gently knead them, smiling as she begins to moan.", 
    "$n places her hands over your soft breasts and begins to knead them, a smile playing over her face as you start moaning.", 
    "No.",
    NULL,
    3, 0, 250, 10, 18, FALSE 
	}, 
	
	{
	"xf-oral",
	"On whom do you wish to do this?",
	NULL,
	"You take $N's hot member in your hands, licking his cock from base to tip.",
	"$n takes $N's penis in $s hands, licking $N's cock from base to tip.",
    "You let out a small moan as $n takes your cock into her hands, licking it from your balls to the tip.",
	"On yourself? I'm impressed!",
	NULL,
	2, 0, 0, 5, 15, FALSE
    },

	{
	"xf-spank",
	"Who's been naughty?",
	NULL,
	"You take $N over your knee and spank him hard, making him squirm.",
	"$n bends $N over her knees and spanks him hard on the ass, making him squirm.",
	"You whimper softly as $n takes you over her knee, spanking you hard on the ass, making it sting.",
	"Nah.",
	NULL,
	2, 0, 0, 25, 4, FALSE
	},

	{
	"xf-urine",
	"On whom do you wish to do this?",
	NULL,
	"You maliciously lean $N's head back and stand over him with his mouth open before letting a hard stream of piss shoot down his throat.",
	"$n shoots a golden stream of urine down $N's throat, making him choke and swallow.",
	"You eyes widen as your mistress shoots a stream of piss odown your throat, making you swallow it all.",
	"Nope.",
	NULL,
	2, 0, 0, 37, -10, FALSE
	},
    
	{
	"xf-beg",
	"Beg who, you stupid slut?",
	NULL,
	"You drop to your kneees before $N, begging for him to abuse you and fuck you hard.",
	"$n drops to her knees and begs for $N to take her and use her for his pleasure.",
	"$n drops to her knees before you, begging for you to abuse her worthless slut body.",
	"It doesn't work that way, you stupid whore.",
	NULL,
	2, 0, 0, -10, 12, FALSE
	},

	{
	"xf-blowjob",
	"On whom do you wish to do this?",
	NULL,
	"You take $N's hot member in your mouth, sucking $S shaft.",
	"$n takes $N's throbbing penis in $s mouth, sucking $N's cock.",
	"You gasp as $n takes your penis in $s mouth, licking your head.",
	"On yourself? I'm impressed!",
	NULL,
	2, 1, 250, 10, 25, FALSE
    },

	{
       "xf-tip",
       "On whom do you wish to do this?",
       NULL,
       "You gently run the tip of your tongue along the underside of $N's hot cock.",
       "$n runs the tip of $s tongue along the bottom of $N's pulsing cock.",
       "You let out a low moan as $n runs the tip of her tongue along the underside of your cock.",
       "I think not.",
       NULL,
         2, 0, 0, 5, 59, FALSE
	},

	{
	"xf-handjob",
	"On who?",
	NULL,
	"You take his hot cock in your hand and wrap your fingers around it, slowly pumping up and down.",
	"$n takes $N's hot cock in her hands, wrapping her fingers around it as she slowly pumps up and down.",
	"$n takes your hot cock into her hands, and you moan softly as she wraps her fingers around it, slowly pumping up and down.",
	"No.",
	NULL,
	2, 0, 0, 9, 14, FALSE
	},

	{
	"xf-sitface",
	"Who gets to taste your sweet juice?",
	NULL,
	"You push $N onto his back and crawl up his body, sitting down on his face as he licks at your pussy.",
	"$n pushes $N onto his back and straddles his face, moaning as he licks her hot pussy.",
	"$n pushes you onto your back and straddles your face, moaning and squirming as you eagerly lap at her hot, wet cunt.",
	"No.",
	NULL,
	2, 1, 250, 15, 0, FALSE
	},

	{
	"xf-69",
	"Who?",
	NULL,
	"You crawl on top of $N and takes his hot cock between your lips, moaning as he laps at your cunt.",
	"$n pushes $N onto his back and crawls on top of him, sucking his cock while he wriggles his tongue about in her cunt.",
	"$n pushes you onto your back, crawling on top of you as she sucks on your cock, moaning while you lick her pussy.",
	"No.",
	NULL,
	2, 1, 250, 20, 20, TRUE
	},

    {
	"xf-breasts",
	"On whom do you wish to do this?",
	NULL,
	"You take $S hands and press them to your breasts.",
	"$n takes $N's hands and presses them to $m breasts.",
	"$n takes your hands and presses them to $m breasts.",
	"Not on yourself!",
	NULL,
	2, 0, 0, 5, 10, FALSE
    },

{
"xf-cum",
"Wrong...",
NULL,
"You feel your muscles tighten for a moment, and you let out a loud moan and arch your back as you release, cumming hard as your juices run down your thighs.",
"$n arches her back and lets out a loud moan, gasping and writhing in pleasure as she cums, her juices running down her thighs.",
"$n arches her back and lets out a loud moan, gasping and writhing in pleasure as her juices gush down her thighs.",
"No.",
NULL,
2, 1, 0, 500, 25, FALSE
},
{ 
"xf-cface",
"Wrong...",
NULL,
"You bite down hard on your lip, letting out a loud moan as you climax, gushing your sticky, wet cum all over $N's face, glazing it over.",
"$n bites down on her lip and lets out a loud cry of pleasure, gushing her juices all over $N's face as he sucks and slurps at her dripping wet pussy.",
"$n bites down on her lip and lets out a loud moan as you slurp at her dripping wet cunt, and she shudders before gushing her sweet juices all over your face, glazing it completely over.",
"No.",
NULL,
2, 1, 0, 500, 30, FALSE
},
{
"xf-chands",
"No...",
NULL,
"You keep sliding your hands back and forth over $N's cock as it grows hotter and hotter, not stopping until he lets out a low groan and fills your hand with his hot, sticky cum.",
"$n slides her hands back and forth over $N's cock as he moans, eventually gritting his teeth and shutting his eyes as he fills her hand full of his hot sticky load of cum, most of it oozing out from between her fingertips.", 
"$n slides her hands over your cock as it grows hotter and hotter, and you let out a low moan as you grit your teeth and shoot a hot, sticky load of cum into her closed hands, filling it up as most of your gooey sperm drips out from between her fingertips.",
"No.",
NULL,
2, 1, 0, 30, 500, FALSE
},

	{
"xf-cbreasts",
"No..",
NULL,
"You take $N's twitching cock between your hands, and aim it straight at your heaving tits, jerking him off until you feel stream after stream of hot, white cum splatter against them and trickle over your stiff nipples.",
"$n takes $N's stiff cock in her hands and aims it at her soft tits, giving him a few quick jerks to finish him off as he shoots out a jet of hot, sticky cum all over her ample mounds and hard nipples.",
"$n takes your cock into her hands and aims it straight at her tits, giving you a few quick jerks to finish you off, and you moan as you shoot your sticky white seed all over her soft breasts, your cum oozing down and trickling over her nipples.",
"No.",
NULL,
1, 1, 0, 37, 500, FALSE
},

	{
      "xf-rub",
      "Who gets to feel the magic touch?",
	  NULL,
      "You gently run your fingers over $N's hardening cock.",
      "$n gently runs her fingers over $N's hardening cock.",
      "$n gently runs her fingers over your hardening cock.",
      "You don't have a penis.  At least, I hope not.",
      NULL,
      2, 0, 0, 15, 20, FALSE
	},
    {
	"xf-contract",
	"On whom do you wish to do this?",
	NULL,
	"You contract your vaginal muscles, driving $M wild.",
	"$n contracts $m vaginal muscles, driving $N wild.",
	"$n contracts $m vaginal muscles, driving you wild.",
	"Not on yourself!",
	NULL,
	2, 2, 0, 10, 15, TRUE
    },

{
"xf-afinger",
"Use this on who?",
NULL,
"You lick your finger and press it to $N's ass, pushing lightly until it pops in and he lets out a small moan.",
"$n licks her finger and takes it to $N's ass, popping it in and gently pumping it back and forth, making him moan.",
"$n licks her finger and takes it to your ass, pushing it in and pumping it in and out, making your dick rock-hard as you moan.",
"No.",
NULL,
2, 1, 0, 20, 23, FALSE
},

    {
	"xf-finger",
	"On whom do you wish to do this?",
	NULL,
	"You put your hands between your legs and begin to masterbate for $N.",
	"$n puts $m hands between $m legs and begins to masterbate for $N.",
	"$n puts $m hands between $m legs and begins to masterbate for your viewing pleasure.  What a turn on!.",
	"Not on yourself!",
	NULL,
	2, 0, 0, 20, 10, FALSE
    },
    {
	"xf-floor",
	"On whom do you wish to do this?",
	NULL,
	"You lie on your back, and pull $M between your parted legs.",
	"$n lies on $m back, and pulls $N between $m parted legs.",
	"$n lies on $m back, and pulls you between $m parted legs.",
	"Not on yourself!",
	NULL,
	2, 1, 250, 25, 20, TRUE
    },

    {
	"xf-grind",
	"On whom do you wish to do this?",
	NULL,
	"You grind your hips up to meet $S thrusts.",
	"$n grinds $m hips up to meet $N's thrusts.",
	"$n grinds $m hips up to meet your thrusts.",
	"Not on yourself!",
	NULL,
	2, 2, 0, 15, 10, TRUE
    },

    {
	"xf-mount",
	"On whom do you wish to do this?",
	NULL,
	"You push $M onto $S back, and slowly lower yourself onto $S erection.",
	"$n pushes $N onto $S back, and slowly lowers $mself onto $S erection.",
	"$n pushes you onto your back, and slowly lowers $mself onto your erection.",
	"Not on yourself!",
	NULL,
	2, 1, 250, 25, 20, TRUE
    },

    {
	"xf-nails",
	"On whom do you wish to do this?",
	NULL,
	"You scratch your nails down $S back.",
	"$n scratches $m nails down $N's back.",
	"$n scratches $m nails down your back.",
	"Not on yourself!",
	NULL,
	2, 2, 0, 5, 1, TRUE
    },

    {
	"xf-pull",
	"On whom do you wish to do this?",
	NULL,
	"You wrap your arms and legs around $M and pull $M into you.",
	"$n wraps $m arms and legs around $N and pulls $M into $m.",
	"$n wraps $m arms and legs around you and pulls you into $m.",
	"Not on yourself!",
	NULL,
	2, 1, 250, 15, 10, TRUE
    },

    {
	"xf-squeeze",
	"On whom do you wish to do this?",
	NULL,
	"Your wrap your legs around $M and squeeze tightly.",
	"$n wraps $m legs around $N and squeezes tightly.",
	"$n wraps $m legs around you and squeezes tightly.",
	"Not on yourself!",
	NULL,
	2, 2, 0, 15, 10, TRUE
    },

    {
	"xf-titfuck",
	"Whom do you wish to treat to this?",
	NULL,
	"You take $N's cock and place it between your breasts, as $N gently thrusts.",
	"$n takes $N's penis, places it between $s breasts, and wanks $M off.",
	"$n takes your cock, places it between her breasts, and wanks you gently with them.",
	"Not on yourself!",
	NULL,
	2, 1, 0, 10, 20, FALSE
    },

{
"xf-fondle",
"On whom do you wish to perform this?",
NULL,
"You reach down and gently fondle $N's warm penis.",
"$n takes $N's cock into her hands, fondling it gently.",
"$n takes your warm cock into her hands, fondling you gently.",
"No.",
NULL,
2, 0, 0, 5, 13, FALSE
},

{
"xf-bob",
"On whom do you wish to perform this?",
NULL,
"You wrap your lips tight around $N's cock and bob your head back and forth, forcing him into your mouth.",
"$n wraps her lips over $N's cock and bobs her head back and forth, making him fuck her mouth.",
"$n wraps her lips tight around your cock and bobs her head back and forth, pumping your length in and out of her mouth.",
"No.",
NULL,
2, 1, 0, 20, 25, TRUE
},

    {
	"xf-undress",
	"On whom do you wish to do this?",
	NULL,
	"You push $N onto the floor, a gleam in your eyes, and tear $S clothes off.",
	"$n pushes $N to the floor, tears $S clothes from $S body.",
	"$n pushes you to the floor, grins, and rips your clothes from your body.",
	"Not on yourself!",
	NULL,
	2, 0, 0, 1, 1, FALSE
    },

    {
	"xm-anal",
	"On whom do you wish to do this?",
	NULL,
	"$N screams in extacy as you come from behind and penetrate $M tight ass from behind.",
	"$n comes from behind $N and penetrates $M tight ass from behind.",
	"You scream as $n comes from behind and penetrates your tight ass from behind!",
	"Not on yourself!",
	NULL,
	1, 1, 250, 25, 25, FALSE
    },

    {
	"xm-spank",
	"Who's the naughty whore?",
	NULL,
	"You grab $N by the hair and throw her over your knees, slapping her hard on the ass.",
	"$n visciously throws $N over his knee, spanking her relentlessly on her tight ass.",
	"You cry out softly as $n visciously takes you over his knee, spanking you mercilessly.",
	"Nah.",
	NULL,
	1, 0, 0, 33, -14, FALSE
	},

	{
	"xm-69",
	"Whom do you wish to perform this on?",
	NULL,
	"You pull $N on top of you and spread her legs, burying your face in her pussy as she sucks on your hard cock.",
	"$n pulls $N on top of him and buries his face between her legs, eating her out as she goes down on his cock.",
	"$n pulls you on top of him and spreads your legs, burying his face between your legs and sucking on your clit as you takes his cock into your mouth.",
	"No.",
	NULL,
	1, 1, 0, 24, 24, TRUE
	},

	{
	"xm-urine",
	"Who gets to taste it?",
	NULL,
	"You force open $N's slutty mouth and shoot a hot stream of piss into it, making her swallow.",
	"$n forces open $Ns mouth and shoots a hot stream of golden piss into it, making her swallow.",
	"$n forces open your slutty mouth and shoots a hot, golden stream of his piss into it, making you swallow.",
	"No.",
	NULL,
	1, 0, 0, 40, -19, FALSE
	},

	{
	"xm-beg",
	"Beg who, you worthless bitchboy?",
	NULL,
	"You drop to your knees, tears in your eyes, and beg for $N to take you as hers.",
	"$n drops to his knees before $N, begging for her to take him as hers.",
	"$n drops to his knees before you with tears in his eyes, sobbing as he begs for you to take him as your own personal toy.",
	"You worthless slut, you can't even do this right.",
	NULL,
	1, 0, 0, -15, 30, FALSE
	},

	{
	"xm-facial",
	"Who gets to have their slutty face done over?",
	NULL,
	"You groan as you shoot your load across her delicate features, your hot cum coating her face and dripping from her chin.",
	"$n grits his teeth as he shoots his sticky cum over $N's face, moaning as she licks her lips and lets it drip down.",
	"$n's eyes widen as he shoots his hot, sticky cum over your face, letting it drip from your chin and trickle over your lips.",
	"Nope.",
	NULL,
	1, 1, 0, 300, 20, TRUE
	},
	
	{
	"xm-cum",
	"So close, yet, so far away?",
	NULL,
	"You let out a low moan as you release your load into $N, filling her up with your hot cum.",
	"$n cries out as he shoots his hot, sticky cum all inside of $N, his creamy load shooting home.",
	"$n grits his teeth and shoots his sticky cum inside of yor whorish body, filling you up until it drips out.",
	"Nopers.",
	NULL,
	1, 2, 0, 300, 20, TRUE
	},

{
"xm-chair",
"Cum in whose hair?",
NULL,
"You pull your hard cock out of $N's hole, and force her to her knees before you as you finish yourself off with your hand, shooting your gooey wad straight into her hair and using a handfull of it to wipe off the sticky head.",
"$n pulls his cock out of $N's hole, and forces her to her knees, jerking himself off for a moment before shooting his hot, sticky sperm into her hair, and he takes a handfull of it to wipe off the head of his still-stiff meat.",
"$n pulls himself out of your hole, and forces you to your knees before him, jerking off into your hair as you look up.  A hot, gooey splatter of cum smacks against your scalp, and he takes a handfull of your hair to wipe off the still-sticky head of his cock.",
"No.",
NULL,
1, 1, 0, 500, 47, FALSE
},

{
"xm-canal",
"Cum in whose ass?",
NULL,
"You pump your cock in and out of $N's ass a few more times before grunting and shooting your sticky load of cum right up it, feeling it fill and trickle over your still-hard cock and down her thighs.",
"$n pumps into $N's tight ass a few more times before he grunts, holding his cock there as he shoots her full of cum, and she lets out a tiny gasp as a little bit oozes out of her tight ass and trickles down her thighs.",
"$n shoves his cock into your tight little ass a few more times, and you hear a grunt from behind you as he starts to cum, shooting a sticky white jet of his cream straight up your ass, and you feel a bit of it trickle out and run down your thighs.",
"No.",
NULL,
1, 1, 0, 500, 47, FALSE
},


{
"xm-cbreasts",
"So close, yet, so far away?",
NULL,
"You let out a low moan as you release your load into $N, filling her up with your hot cum.",
"$n cries out as he shoots his hot, sticky cum all inside of $N, his creamy load shooting home.",
"$n grits his teeth and shoots his sticky cum inside of yor whorish body, filling you up until it drips out.",
"Nopers.",
NULL,
1, 1, 0, 300, 20, TRUE
},
	{
	"xm-chand",
	"So close, yet, so far away?",
	NULL,
	"You let out a low moan as you release your load into $N, filling her up with your hot cum.",
	"$n cries out as he shoots his hot, sticky cum all inside of $N, his creamy load shooting home.",
	"$n grits his teeth and shoots his sticky cum inside of yor whorish body, filling you up until it drips out.",
	"Nopers.",
	NULL,
	1, 1, 0, 300, 20, TRUE
	},

	{
	"xm-stomach",
	"Cum on whose stomach?",
	NULL,
	"You pull out of $N and stand up, pushing her to her back.  After a few quick jerks on your stiff cock, you let out a moan and start to cum, spraying your hot, gooey cum all over her smooth body.",
	"$n pulls out of $N and stands up, pushing her to her back.  After a few quick jerks on his stiff cock, he closes his eyes and lets out a moan, a stream of gooey whip cum spraying from the swollen head of his cock and all over $N's smooth skin.",
	"$n pulls out of you and pushes you onto your back.  As you look up, he gives his hard cock a few quick jerks, and he moans as he starts to cum, a spray of hot, gooey white cum splattering all over your body, coating you from head to toe.",
	"No.",
	NULL,
	1, 1, 0, 500, 50, TRUE
	},

	{
	"xm-breasts",
	"On whom do you wish to do this?",
	NULL,
	"You gently run the tip of your tongue across $M naked breasts.",
	"$n gently runs the tip of $s tongue across $N's naked breasts.",
	"$n gently runs the tip of $s tongue across your naked breasts.",
	"Not on yourself!",
	NULL,
	1, 0, 0, 5, 10, FALSE
    },

    {
	"xm-cup",
	"On whom do you wish to do this?",
	NULL,
	"You cup $N's breasts in your hands, and caress $S nipples.",
	"$n cups $N's breasts in $s hands and caress' $S nipples.",
	"$n cups your breasts in $s palms and caress' your nipples.",
	"ON YOURSELF??",
	NULL,
	1, 0, 0, 1, 5, FALSE
    },

    {
	"xm-doggy",
	"On whom do you wish to do this?",
	NULL,
	"You roll $M onto all fours and penetrate $M hot flesh from behind.",
	"$n rolls $N onto all fours and penetrates $M hot flesh from behind.",
	"$n rolls you onto all fours and penetrates your hot flesh from behind.",
	"Not on yourself!",
	NULL,
	1, 1, 250, 25, 15, FALSE
    },

    {
	"xm-finger",
	"On whom do you wish to do this?",
	NULL,
	"You slide your fingers between $M legs, gently stroking $M clitoris.",
	"$n slides $s fingers between $N's legs, gently stroking $M clitoris.",
	"$n slides $s fingers between your legs, gently stroking your clitoris.",
	"Not on yourself!",
	NULL,
	1, 0, 0, 0, 10, FALSE
    },

    {
	"xm-fist",
	"On whom do you wish to do this? I hope you asked!",
	NULL,
	"You make a fist and thrust it up $N's spread wide vagina as she screams with pleasure.",
	"$N gasps as $n pushes $s hand up between $S legs into $S vagina.",
	"$n spreads your legs wide, and thrusts $s hand up your vagina making you cry out in pleasure.",
	"C'est non possible.. I hope...",
	NULL,
        1, 1, 0, 5, 15, FALSE
    },

    {
	"xm-floor",
	"On whom do you wish to do this?",
	NULL,
	"You lower $M to the floor, and slide your body between $M parted legs.",
	"$n lowers $N to the floor, and slides $s body between $M parted legs.",
	"$n lowers you to the floor, and slides $s body between your parted legs.",
	"Not on yourself!",
	NULL,
	1, 1, 250, 15, 10, TRUE
    },

    {
	"xm-jerkoff",
	"On whom do you wish to do this?",
	NULL,
	"You grab your penis and begin to jerkoff for $N.",
	"$n grabs your penis and begins to jerkoff for $N.",
	"$n grab his penis and begin to jerkoff for your viewing pleasure.  What a turn on!.",
	"Not on yourself!",
	NULL,
	1, 1, 0, 25, 5, FALSE
    },

    {
	"xm-nipple",
	"On whom do you wish to do this?",
	NULL,
	"You gently twist $M nipple between your thumb and forefinger.",
	"$n gently twists $N's nipple between $s thumb and forefinger.",
	"$n gently twists your nipple between $s thumb and forefinger.",
	"Not on yourself!",
	NULL,
	1, 0, 0, 5, 10, FALSE
    },

    {
	"xm-oral",
	"On whom do you wish to do this?",
	NULL,
	"$N squirms in delight as you bend down and run your tongue along and into $S vagina.",
	"$N squirms ecstatically as $n licks and kisses $S loveslit.",
	"$n wriggles $s tongue about between your legs, making you squirm in ecstacy.",
	"Biologically impossible I think you'll find!",
	NULL,
	1, 0, 0, 10, 25, FALSE
    },

    {
	"xm-press",
	"On whom do you wish to do this?",
	NULL,
	"You press $M against the wall, pulling $M legs around your hips.",
	"$n presses $N against the wall, pulling $M legs around $s hips.",
	"$n presses you against the wall, pulling your legs around $s hips.",
	"Not on yourself!",
	NULL,
	1, 1, 250, 25, 20, TRUE
    },

    {
	"xm-pull",
	"On whom do you wish to do this?",
	NULL,
	"You grab $M around the hips and pull $M firmly onto your erection.",
	"$n grabs $N around the hips and pull $M firmly onto $s erection.",
	"$n grabs you around the hips and pulls you firmly onto $s erection.",
	"Not on yourself!",
	NULL,
	1, 1, 250, 10, 10, TRUE
    },

    {
	"xm-spoon",
	"On whom do you wish to do this?",
	NULL,
	"You roll $M onto $S side and penetrate $M hot flesh from behind.",
	"$n rolls $N onto $S side and penetrates $M hot flesh from behind.",
	"$n rolls you onto your side and penetrates your hot flesh from behind.",
	"Not on yourself!",
	NULL,
	1, 1, 250, 20, 20, TRUE
    },

    {
	"xm-suck",
	"On whom do you wish to do this?",
	NULL,
	"You suck slowly on $M nipple, feeling it harden between your lips.",
	"$n suck slowly on $N's nipple.",
	"$n sucks slowly on your nipple, and you feel it harden between $s lips.",
	"Not on yourself!",
	NULL,
	1, 0, 0, 5, 10, FALSE
    },

    {
	"xm-thrust",
	"On whom do you wish to do this?",
	NULL,
	"You thrust deeply between $M warm, slippery thighs.",
	"$n thrusts deeply between $N's warm, slippery thighs.",
	"$n thrusts deeply between your warm, slippery thighs.",
	"Not on yourself!",
	NULL,
	1, 2, 0, 15, 10, TRUE
    },
{
"xm-hump",
"On whom do you wish to do this?",
NULL,
"You push $M onto all fours and mount her, madly shoving your hard cock in and out of her tiny hole.",
"$n mounts $N like she was a bitch in heat, madly pumping in and out of her hole.",
"$n pushes you onto all fours like you were a bitch and heat and mounts you as he rapidly shoves his cock in and out of your slutty hole.",
"No!.",
NULL,
1, 2, 0, 35, 65, TRUE
},

{
"xm-mfuck",
"Fuck who?",
NULL,
"You have $N wrap her lips tight around your cock, and you thrust gently, pumping your cock in and out of her warm, wet mouth.",
"$N wraps her lips tight aroudn $n's cock, and he thrusts gently, pumping in and out from between her lips.",
"You wrap your lips tight around $n's cock and he thrusts gently, pumping in and out from between your ruby red lips.",
"No.",
NULL,
1, 1, 0, 12, 20, TRUE
},

{
"xm-feed",
"Feed who?",
NULL,
"You gently stick your fingers in $N's hot cunt and bring them to her lips, making her suck them clean.",
"$n sticks his fingers in $N's twat and then brings them to her lips, making her lick them clean.",
"$n sticks his fingers in your dripping wet pussy and then brings them to your lips, where you suck them clean.",
"No.",
NULL,
1, 1, 0, 5, 10, FALSE
},

{
"xm-gag",
"Who gets to taste it?",
NULL,
"You ram your cock deep into $M mouth, making her gag and try to swallow.",
"$n rams his cock deep into $N's throat, making her gag and try to swallow.",
"$n shoves his cock deep into your throat, making you gag and try to swallow.",
"No.",
NULL,
1, 1, 0, 5, 13, FALSE
},
    {
	"xm-tug",
	"On whom do you wish to do this?",
	NULL,
	"You gently tug $M nipple between your teeth.",
	"$n gently tugs $N's nipple between $s teeth.",
	"$n gently tugs your nipple between $s teeth.",
	"Not on yourself!",
	NULL,
	1, 0, 0, 5, 10, FALSE
    },

{
"xm-nibble",
"On whom do you wish to do this?",
NULL,
"You push $M onto her back and bury your face in her cunt, nibbling gently on her clit.",
"$n pushes $N onto her back and buries his face between her legs, nibbling gently on her clit.",
"$n pushes you onto your back and buries his face in your womanhood, nibbling gently on your clit.",
"Nope.",
NULL,
1, 1, 0, 10, 45, FALSE
},
    {
	"xm-undress",
	"Who do you wish to undress?",
	NULL,
	"You gently tug at $N's garments with your teeth, until $E stands naked before you.",
	"$n gently tugs at $N's clothes with his teeth, until $E stands naked infront of $m.",
	"$n gently tugs at your clothing with his teeth, you stand naked before $m.",
	"Not on yourself!",
	NULL,
	1, 0, 0, 1, 1, FALSE
    },

    {
	"",
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0, 0, FALSE
    }
};
