/* See LICENSE file for copyright and license details. */ 
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 5;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int swallowfloating    = 1;        /* 1 means swallow floating windows by default */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = {"Hack Nerd Font:size=12:style=Regular", "JoyPixels:size=12:antialias=true:autohint=true", "monospace:size=12"}; /*Setting the fonts*/
static const char dmenufont[]       = "Hack Nerd Font:size=14:style=Bold"; /* Setting the font for Dmenu */
//background color
static const char col_color1[]       = "#333333";
//inactive window border color
static const char col_color2[]       = "#444444";
//font color
static const char col_color3[]       = "#cccccc";
//current tag and current window font color
static const char col_color4[]       = "#eeeeee";
// top bar second color (col_custom) and active window border color
static const char col_custom[]        = "#1b0ff8";
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
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "Terminator",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ "Alacritty",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ "Xterm",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ "Termite",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ "St",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ "st",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

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
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_color1, "-nf", col_color3, "-sb", col_custom, "-sf", col_color4, NULL };
static const char *termcmd[]  = { "st", NULL };

// Custom commands to run to do some volume stuff.
static const char *volumeup_cmd[] = {"amixer", "-q","sset", "Master","2%+", NULL};

static const char *volumedown_cmd[] = {"amixer", "-q","sset", "Master","2%-", NULL};

static const char *volumemute_cmd[] = {"amixer", "-q","sset","Master", "toggle", NULL};

static const char *mic_toggle_cmd[] = {"amixer", "-q", "sset", "Capture", "toggle", NULL};

static const char *mic_volume_up_cmd[] = {"amixer", "-q", "sset", "Capture", "2%+", NULL};

static const char *mic_volume_down_cmd[] = {"amixer", "-q", "sset", "Capture", "2%-", NULL};
// Custom volume end.
// Custom brightness settings

static const char *brightness_up_cmd[] = {"brightnessctl", "set", "+2%"};

static const char *brightness_down_cmd[] = {"brightnessctl", "set", "2%-"};

//Custom command to take a screenshot

static const char *screenshot[] = {"mate-screenshot"};

//Custom emoji menu via dmenu provided by Luke Smith
static const char *emoji_menu[] = {"dmenuunicode.sh"};

// Importing the shiftview file to make tag switching easier
#include "shiftview.c"

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },

	{0,								XF86XK_AudioRaiseVolume, spawn, {.v = volumeup_cmd} },
	{0,								XF86XK_AudioLowerVolume, spawn, {.v = volumedown_cmd} },
	{0,								XF86XK_AudioMute, spawn, {.v = volumemute_cmd} },

	{ MODKEY|ShiftMask,				XK_Up,				spawn, {.v = mic_volume_up_cmd} },
	{ MODKEY|ShiftMask,				XK_Down,			spawn, {.v = mic_volume_down_cmd} },
	{ MODKEY|ShiftMask, 			XK_m,		spawn, {.v = mic_toggle_cmd} },

	{ MODKEY,						XK_Right,	shiftview, {.i = +1} },
	{ MODKEY,						XK_Left,	shiftview, {.i = -1} },

	{0,								XF86XK_MonBrightnessUp, spawn, {.v = brightness_up_cmd} },
	{0,								XF86XK_MonBrightnessDown, spawn, {.v = brightness_down_cmd} },

	{0,								XK_Print,				  spawn, {.v = screenshot} },

	{MODKEY,						XK_e,						spawn, {.v = emoji_menu} },

	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_k,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
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
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button3,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button2,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

