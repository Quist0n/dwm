/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 16;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int swallowfloating    = 1;        /* 1 means swallow floating windows by default */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = {"Hack Nerd Font:size=12:style=Regular",
                                       "JoyPixels:size=12:antialias=true:autohint=true",
                                       "monospace:size=12"}; /*Setting the fonts*/

static const char dmenufont[]       = "Hack Nerd Font:size=14:style=Bold"; /* Setting the font for Dmenu */
//background color
static const char col_color1[]       = "#111111";
//inactive window border color
static const char col_color2[]       = "#111111";
//font color
static const char col_color3[]       = "#cccccc";
//current tag and current window font color
static const char col_color4[]       = "#000000";
// top bar second color (col_custom) and active window border color
static const char col_custom[]       = "#e5e1ff";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_color3, col_color1, col_color2 },
	[SchemeSel]  = { col_color4, col_custom,  col_custom  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
		/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
//Make spawning rules for browsers
	{ "firefox", NULL,     NULL,           1 << 1,    0,          0,          -1,        -1 },
	{ "Brave-browser", NULL,     NULL,           1 << 2,    0,          0,          -1,        -1 },
	{ "librewolf", NULL,     NULL,           1 << 2,    0,          0,          -1,        -1 },
//Make spawning rules for other programs
	{ "Element", NULL,     NULL,           1 << 0,    0,          0,          -1,        -1 },
	{ "nheko", NULL,     NULL,           1 << 0,    0,          0,          -1,        -1 },
	{ "discord", NULL,     NULL,           1 << 0,    0,          0,          -1,        -1 },
	{ "Ripcord", NULL,     NULL,           1 << 0,    0,          0,          -1,        -1 },
	{ "TelegramDesktop", NULL,     NULL,           1 << 0,    0,          0,          -1,        -1 },
	{ "Signal", NULL,     NULL,           1 << 0,    0,          0,          -1,        -1 },
	{ "FreeTube", NULL,     NULL,           1 << 3,    0,          0,          -1,        -1 },
	{ "qBittorrent", NULL,     NULL,           1 << 5,    0,          0,          -1,        -1 },
	{ "Steam", NULL,     NULL,           1 << 6,    0,          0,          0,        -1 },
	{ "zoom", NULL,     NULL,           1 << 7,    0,          0,          1,        -1 },
//Make spawning rules for terminals, allow window swallowing
	{ "Alacritty",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ "Xterm",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ "St",      NULL,     NULL,           0,         0,          1,           0,        -1 },
//Make xev not get window swallowed
	{ NULL,      NULL,     "Event Tester", 0,         1,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int attachdirection = 5;    /* 0 default, 1 above, 2 aside, 3 below, 4 bottom, 5 top */

#include "layouts.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[][]",      grid },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_color1, "-nf", col_color3, "-sb", col_custom, "-sf", col_color4, "-b", NULL };
//static const char *termcmd[]  = { "alacritty", NULL };


// Importing the shiftview file to make tag switching easier
#include "shiftview.c"

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_r,      spawn,          {.v = dmenucmd } },
//	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },

	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_k,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.01} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.01} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
//	{ MODKEY,                       XK_s,      view,           {0} },
	{ MODKEY,                       XK_Tab,    shiftview,      { .i = +1} },
	{ MODKEY|ShiftMask,             XK_Tab,    shiftview,      { .i = -1} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
    { MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
    { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
//	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button3,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY|ShiftMask,         Button1,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

