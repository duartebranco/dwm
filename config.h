/* See LICENSE file for copyright and license details. */

/* appearance */
#include <X11/Xutil.h>
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 20;       /* vert inner gap between windows */
static const unsigned int gappoh    = 20;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static int smartgaps                = 0;        /* 1 means no outer gap when there is only one window */

static unsigned int borderpx  = 3;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static char *fonts[]          = { "Liberation Mono:size=13", "NotoColorEmoji:pixelsize=13:antialias=true:autohint=true", "FiraCode Nerd Font:size=13"  };
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";

static char *colors[][3] = {
    /*               fg           bg           border   */
    [SchemeNorm] = { selfgcolor, normbgcolor, normbordercolor }, // unfocused wins
    [SchemeSel]  = { selfgcolor, selbgcolor,  selbordercolor },  // the focused win

	[SchemeStatus]  = { normfgcolor, normbgcolor,  "#000000"  }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeTagsSel]  = { normfgcolor, selbordercolor,  "#000000"  }, // Tagbar left selected {text,background,not used but cannot be empty}
	[SchemeTagsNorm]  = { normfgcolor, normbgcolor,  "#000000"  }, // Tagbar left unselected {text,background,not used but cannot be empty}
	[SchemeInfoSel]  = { normfgcolor, normbgcolor,  "#000000"  }, // infobar middle  selected {text,background,not used but cannot be empty}
	[SchemeInfoNorm]  = { normfgcolor, normbgcolor,  "#000000"  }, // infobar middle  unselected {text,background,not used but cannot be empty}
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
    /* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
    { "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
    { "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
    { "St",      NULL,     NULL,           0,         0,          1,           0,        -1 },
    { NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile },    /* first entry is default */
    { "TTT",      bstack },
    { "===",      bstackhoriz },

    { "[@]",      spiral },

    { "[M]",      monocle },

    { "|M|",      centeredmaster },

    { "HHH",      grid },
    { "###",      nrowgrid },

    { "><>",      NULL },    /* no layout function means floating behavior */
    { NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
/* stacker */
#define STACKKEYS(MOD,ACTION) \
	{ MOD,	XK_j,	ACTION##stack,	{.i = INC(+1) } }, \
	{ MOD,	XK_k,	ACTION##stack,	{.i = INC(-1) } }, \
	{ MOD,  XK_Right,   ACTION##stack,  {.i = INC(+1) } }, \
	{ MOD,  XK_Left,    ACTION##stack,  {.i = INC(-1) } }, \

	/* { MOD,  XK_v,   ACTION##stack,  {.i = 0 } }, \ */
	/* { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
	/* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
	/* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
	/* { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
/* static char dmenumon[2] = "0"; */ /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]  = { "/home/duarte/scripts/dmenu/dmenu_drun.sh", NULL };
static const char *dmenushut[]  = { "/home/duarte/scripts/dmenu/dmenu_shut.sh", NULL };
static const char *dmenurealshut[]  = { "/home/duarte/scripts/dmenu/dmenu_realshut.sh", NULL };
static const char *dmenubookmarks[]  = { "/home/duarte/scripts/dmenu/dmenu_bookmarks.sh", NULL };

static const char *vms[]  = { "/home/duarte/scripts/dmenu_vm.sh", NULL };
static const char *wallpaper[]  = { "/home/duarte/scripts/wallpaper.sh", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *browsercmd[]  = { "/home/duarte/scripts/firefox.sh", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
	{ "normbgcolor",        STRING,  &normbgcolor },
    { "normbordercolor",    STRING,  &normbordercolor },
    { "normfgcolor",        STRING,  &normfgcolor },
    { "selbgcolor",         STRING,  &selbgcolor },
    { "selbordercolor",     STRING,  &selbordercolor },
    { "selfgcolor",         STRING,  &selfgcolor },
    { "borderpx",           INTEGER, &borderpx },
    { "snap",               INTEGER, &snap },
    { "showbar",            INTEGER, &showbar },
    { "topbar",             INTEGER, &topbar },
    { "nmaster",            INTEGER, &nmaster },
    { "resizehints",        INTEGER, &resizehints },
    { "mfact",              FLOAT,   &mfact },
};

#include <X11/XF86keysym.h>

static const Key keys[] = {
	/* modifier                     key            function        argument */
	STACKKEYS(MODKEY,                              focus)
	STACKKEYS(MODKEY|ShiftMask,                    push)

	TAGKEYS(                        XK_1,          0)
	TAGKEYS(                        XK_2,          1)
	TAGKEYS(                        XK_3,          2)
	TAGKEYS(                        XK_4,          3)
	TAGKEYS(                        XK_5,          4)

    { MODKEY,                       XK_F2, spawn, SHCMD("pamixer --decrease 5; pkill -RTMIN+10 dwmblocks") },
    { MODKEY,                       XK_F3, spawn, SHCMD("pamixer --increase 5; pkill -RTMIN+10 dwmblocks") },
    { MODKEY,                       XK_F1, spawn, SHCMD("pamixer --toggle-mute; pkill -RTMIN+10 dwmblocks") },

    { MODKEY, XK_F6, spawn, SHCMD("brightnessctl set +10%; pkill -RTMIN+11 dwmblocks") },
    { MODKEY, XK_F5, spawn, SHCMD("brightnessctl set 10%-; pkill -RTMIN+11 dwmblocks") },

    { 0, XF86XK_AudioLowerVolume, spawn, SHCMD("pamixer --decrease 5; pkill -RTMIN+10 dwmblocks") },
    { 0, XF86XK_AudioRaiseVolume, spawn, SHCMD("pamixer --increase 5; pkill -RTMIN+10 dwmblocks") },
    { 0, XF86XK_AudioMute,        spawn, SHCMD("pamixer --toggle-mute; pkill -RTMIN+10 dwmblocks") },

    { 0, XF86XK_MonBrightnessUp,   spawn, SHCMD("brightnessctl set +10%; pkill -RTMIN+11 dwmblocks") },
    { 0, XF86XK_MonBrightnessDown, spawn, SHCMD("brightnessctl set 10%-; pkill -RTMIN+11 dwmblocks") },

    { 0, XF86XK_PowerOff, spawn, {.v = dmenurealshut } },

    { MODKEY,                       XK_i,          incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,          incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,          setmfact,       {.f = -0.02} },
	{ MODKEY,                       XK_l,          setmfact,       {.f = +0.02} },
	{ MODKEY,                       XK_Return,     zoom,           {0} },
	{ MODKEY,                       XK_Tab,        view,           {0} },
	{ MODKEY,                       XK_q,          killclient,     {0} },

	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },

	{ MODKEY|Mod1Mask,              XK_KP_Insert,  setlayout,      {.v = &layouts[0]} },
	{ MODKEY|Mod1Mask,              XK_KP_End,     setlayout,      {.v = &layouts[1]} },
	{ MODKEY|Mod1Mask,              XK_KP_Down,    setlayout,      {.v = &layouts[2]} },
	{ MODKEY|Mod1Mask,              XK_KP_Next,    setlayout,      {.v = &layouts[3]} },
	{ MODKEY|Mod1Mask,              XK_KP_Left,    setlayout,      {.v = &layouts[4]} },
	{ MODKEY|Mod1Mask,              XK_KP_Begin,   setlayout,      {.v = &layouts[5]} },
	{ MODKEY|Mod1Mask,              XK_KP_Right,   setlayout,      {.v = &layouts[6]} },
	{ MODKEY|Mod1Mask,              XK_KP_Home,    setlayout,      {.v = &layouts[7]} },

	{ MODKEY,                       XK_F11,        togglefullscr,  {0} },
	{ MODKEY,                       XK_p,          spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_q,          spawn,          {.v = dmenushut } },
	{ MODKEY|ControlMask|ShiftMask, XK_q,          quit,           {1} },
	{ MODKEY,                       XK_w,          spawn,          {.v = wallpaper } },
	{ MODKEY|Mod1Mask,              XK_m,          spawn,          {.v = vms } },
	{ MODKEY,                       XK_a,          spawn,          {.v = dmenubookmarks } },
	{ MODKEY,                       XK_t,          spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,          spawn,          {.v = browsercmd } },
	{ MODKEY,                       XK_r,          spawn,          SHCMD("st -e ranger") },
	{ MODKEY,                       XK_m,          spawn,          SHCMD("st -e ncmpcpp") },

	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY,                       XK_v,      togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },

	{ 0,                            XK_Print,  spawn,          SHCMD("scrot -s ~/img/Screenshots/screenshot_%Y%m%d_%H%M%S.png") },
    { Mod1Mask,                     XK_Print,  spawn,          SHCMD("scrot ~/img/Screenshots/screenshot_%Y%m%d_%H%M%S.png") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask           button          function        argument */
#ifndef __OpenBSD__
	{ ClkWinTitle,          0,                   Button2,        zoom,           {0} },
	{ ClkStatusText,        0,                   Button1,        sigdwmblocks,   {.i = 1} }, /* left click on statusbar */
	{ ClkStatusText,        0,                   Button2,        sigdwmblocks,   {.i = 2} }, /* middle click on statusbar */
	{ ClkStatusText,        0,                   Button3,        sigdwmblocks,   {.i = 3} }, /* right click on statusbar */
	{ ClkStatusText,        ShiftMask,           Button1,        sigdwmblocks,   {.i = 6} }, /* shift + left click on statusbar */
#endif
	{ ClkClientWin,         MODKEY,              Button1,        movemouse,      {0} }, /* Super + left click -> move window */
	{ ClkClientWin,         MODKEY,              Button3,        resizemouse,    {0} }, /* Super + right click -> resize window */

	{ ClkClientWin,         MODKEY,              Button2,        defaultgaps,    {0} }, /* Super + middle click -> default gaps */
	{ ClkClientWin,		    MODKEY, 		     Button4,	     incrgaps,       {.i = +1} }, /* Super + scroll up -> increase gaps */
	{ ClkClientWin,		    MODKEY, 		     Button5,	     incrgaps,       {.i = -1} }, /* Super + scroll down -> decrease gaps */

	{ ClkTagBar,            0,                   Button1,        view,           {0} }, /* left click -> go to tab */
	{ ClkTagBar,            0,                   Button3,        toggleview,     {0} }, /* right click -> see that tab also */
	{ ClkTagBar,            MODKEY,              Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,              Button3,        toggletag,      {0} },
};
