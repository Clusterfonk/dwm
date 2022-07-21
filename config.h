/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 10;
static const unsigned int bargappx  = 10;
static const unsigned int snap      = 8;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */

static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int usealtbar          = 1;
static const char *altbarclass      = "Polybar";
static const char *altbarcmd        = "$HOME/.config/polybar/launch.sh";

static const char *fonts[]          = { "-misc-fira mono-*-*-normal-*-*-*-*-*-*-*-ascii-*" };
static const char dmenufont[]       = { "fira mono" };

static const char col_bg0[]       = "#282828";
static const char col_bg1[]       = "#3c3836";
static const char col_fg0[]       = "#fbf1c7";
static const char col_gray4[]     = "#eeeeee";
static const char col_cyan[]      = "#8ec07c";
static const char col_yellow[]    = "#fabd2f";
static const char col_red[]       = "#fb4934";

static const char *colors[][3]    = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_fg0, col_bg0, col_bg1 },
	[SchemeSel]  = { col_fg0, col_bg0, col_red },
};

/* tagging */
static const char *tags[] = { "I", "II", "III", "IV", "V" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   isterminal   noswallow   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           0,            0,         -1 },
	{ "mpv",      NULL,       NULL,       0,            1,           0,            0,         -1 },
	{ "Alacritty",NULL,       NULL,       0,            0,           1,            0,         -1 },
	{ NULL,       NULL,     "Event Tester", 0,          0,           0,            1,         -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol    arrange function */
	{ "[T]",       tile }, /* first entry is default */
	{ "[F]",     monocle },
	{ NULL,      NULL },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont,
                                               "-x", "610", "-y", "540", "-z", "700", "-i", NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *snipregion[] = {"snipregion", NULL};
static const char *toggle_headphones[] = {"toggle-headphone-speakers", NULL};
static const char *nextaudio[] = {"playerctl", "next", NULL};
static const char *prevaudio[] = {"playerctl", "previous", NULL};
static const char *playaudio[] = {"playerctl", "play-pause", NULL};
static const char *stopaudio[] = {"playerctl", "stop", NULL};
static const char *muteaudio[] = {"amixer", "set", "Master", "toggle", NULL};
static const char *raiseaudio[] = {"amixer", "set", "Master", "5%+", NULL};
static const char *loweraudio[] = {"amixer", "set", "Master", "5%-", NULL};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_u,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_space,  cyclelayout,    {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_r,      rotatestack,    {.i = -1 } },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
    { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

	{ MODKEY|ShiftMask,             XK_s,      spawn,          {.v = snipregion} },

	{ ControlMask,    XK_dead_circumflex,      spawn,          {.v = toggle_headphones} },
	{ 0,                XF86XK_AudioNext,      spawn,          {.v = nextaudio} },
	{ 0,                XF86XK_AudioPrev,      spawn,          {.v = prevaudio} },
	{ 0,                XF86XK_AudioPlay,      spawn,          {.v = playaudio} },
	{ 0,                XF86XK_AudioStop,      spawn,          {.v = stopaudio} },
	{ 0,                XF86XK_AudioMute,      spawn,          {.v = muteaudio} },
	{ 0,         XF86XK_AudioRaiseVolume,      spawn,          {.v = raiseaudio} },
	{ 0,         XF86XK_AudioLowerVolume,      spawn,          {.v = loweraudio} },

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
	{ MODKEY,                       XK_q,      quit,           {1} },
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
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
};

static const char *ipcsockpath = "/tmp/dwm.sock";
static IPCCommand ipccommands[] = {
  IPCCOMMAND(  view,                1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggleview,          1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tag,                 1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggletag,           1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tagmon,              1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  focusmon,            1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  focusstack,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  zoom,                1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  incnmaster,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  killclient,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  togglefloating,      1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  setmfact,            1,      {ARG_TYPE_FLOAT}  ),
  IPCCOMMAND(  setlayoutsafe,       1,      {ARG_TYPE_PTR}    ),
  IPCCOMMAND(  quit,                1,      {ARG_TYPE_NONE}   )
};

