/* speedie's surf configuration
 * https://github.com/speedie-de/surf
 *
 * You'll probably wanna change a few settings below,*/

/* modifier 0 means no modifier */
static int surfuseragent    = 1;  /* Append Surf version to default WebKit user agent */
static int extendedtitle    = 0;
static int scrollspeed      = 7; /* Scrolling speed when using the mouse */
static char *fulluseragent  = "leet"; /* Or override the whole user agent string */
static char *scriptfile     = "~/.local/share/.surf/script.js";
static char *styledir       = "~/.local/share/.surf/styles/";
static char *certdir        = "~/.local/share/.surf/certificates/";
static char *cachedir       = "~/.local/share/.surf/cache/";
static char *cookiefile     = "~/.local/share/.surf/cookies.txt";
static char *historyfile    = "~/.local/share/.surf/history/txt";
static char *dlstatus       = "~/.local/share/.surf/dlstatus/";
static char *dldir          = "~/Downloads/";
static char *searchengine   = "https://searx.org/?q=";
#define HOMEPAGE              "https://searx.org"

/* Webkit default features */
/* Highest priority value will be used.
 * Default parameters are priority 0
 * Per-uri parameters are priority 1
 * Command parameters are priority 2
 */
static Parameter defconfig[ParameterLast] = {
	/* parameter                    Arg value       priority */
	[AccessMicrophone]    =       { { .i = 0 },     },
	[AccessWebcam]        =       { { .i = 0 },     },
	[Certificate]         =       { { .i = 0 },     },
	[CaretBrowsing]       =       { { .i = 0 },     },
	[CookiePolicies]      =       { { .v = "@Aa" }, },
	[DefaultCharset]      =       { { .v = "UTF-8" }, },
	[DiskCache]           =       { { .i = 1 },     },
	[DNSPrefetch]         =       { { .i = 0 },     },
	[Ephemeral]           =       { { .i = 0 },     },
	[FileURLsCrossAccess] =       { { .i = 0 },     },
	[FontSize]            =       { { .i = 12 },    },
	[FrameFlattening]     =       { { .i = 0 },     },
	[Geolocation]         =       { { .i = 0 },     },
	[HideBackground]      =       { { .i = 0 },     },
	[Inspector]           =       { { .i = 0 },     },
	[Java]                =       { { .i = 1 },     },
	[JavaScript]          =       { { .i = 1 },     },
	[KioskMode]           =       { { .i = 0 },     },
	[LoadImages]          =       { { .i = 1 },     },
	[MediaManualPlay]     =       { { .i = 0 },     },
	[PreferredLanguages]  =       { { .v = (char *[]){ NULL } }, },
	[RunInFullscreen]     =       { { .i = 0 },     },
	[ScrollBars]          =       { { .i = 1 },     },
	[ShowIndicators]      =       { { .i = 1 },     },
	[SiteQuirks]          =       { { .i = 1 },     },
	[SmoothScrolling]     =       { { .i = 1 },     },
	[SpellChecking]       =       { { .i = 0 },     },
	[SpellLanguages]      =       { { .v = ((char *[]){ "en_US", NULL }) }, },
	[StrictTLS]           =       { { .i = 1 },     },
	[Style]               =       { { .i = 1 },     },
	[WebGL]               =       { { .i = 0 },     },
	[ZoomLevel]           =       { { .f = 1.0 },   },
	[ClipboardNotPrimary] =	      { { .i = 1 },     },
};

static UriParameters uriparams[] = {
	{ "(://|\\.)suckless\\.org(/|$)", {
	  [JavaScript] = { { .i = 0 }, 1 },
	}, },
};

/* default window size: width, height */
static int winsize[] = { 800, 600 };

static WebKitFindOptions findopts = WEBKIT_FIND_OPTIONS_CASE_INSENSITIVE |
                                    WEBKIT_FIND_OPTIONS_WRAP_AROUND;

#define PROMPT_GO   "Go:"
#define PROMPT_FIND "Find:"

/* SETPROP(readprop, setprop, prompt)*/
#define SETPROP(r, s, p) { \
        .v = (const char *[]){ "/bin/sh", "-c", \
             "prop=\"$(printf '%b' \"$(xprop -id $1 "r" " \
             "| sed -e 's/^"r"(UTF8_STRING) = \"\\(.*\\)\"/\\1/' " \
             "      -e 's/\\\\\\(.\\)/\\1/g')\" " \
             "| dmenu -b -p '"p"' -w $1)\" " \
             "&& xprop -id $1 -f "s" 8u -set "s" \"$prop\"", \
             "surf-setprop", winid, NULL \
        } \
}

#define DLSTATUS { \
        .v = (const char *[]){ "st", "-e", "/bin/sh", "-c",\
            "while true; do cat $1/* 2>/dev/null || echo \"no hay descargas\";"\
            "A=; read A; "\
            "if [ $A = \"clean\" ]; then rm $1/*; fi; clear; done",\
            "surf-dlstatus", dlstatus, NULL } \
}

/* PLUMB(URI) */
/* This called when some URI which does not begin with "about:",
 * "http://" or "https://" should be opened.
 */
#define PLUMB(u) {\
        .v = (const char *[]){ "/bin/sh", "-c", \
             "xdg-open \"$0\"", u, NULL \
        } \
}

/* VIDEOPLAY(URI) */
#define VIDEOPLAY(u) {\
        .v = (const char *[]){ "/bin/sh", "-c", \
             "mpv --really-quiet \"$0\"", u, NULL \
        } \
}

/* styles */
/*
 * The iteration will stop at the first match, beginning at the beginning of
 * the list.
 */
static SiteSpecific styles[] = {
	/* regexp               file in $styledir */
	{ ".*",                 "default.css" },
};

/* certificates */
/*
 * Provide custom certificate for urls
 */
static SiteSpecific certs[] = {
	/* regexp               file in $certdir */
	{ "://suckless\\.org/", "suckless.org.crt" },
};

#define MODKEY GDK_CONTROL_MASK

/* hotkeys */
/*
 * If you use anything else but MODKEY and GDK_SHIFT_MASK, don't forget to
 * edit the CLEANMASK() macro.
 */
static Key keys[] = {
	/* modifier              keyval          function    arg */
	{ MODKEY,                GDK_KEY_comma,  spawn,      SETPROP("_SURF_URI", "_SURF_GO", PROMPT_GO) },
	{ MODKEY,                GDK_KEY_f,          spawn,      SETPROP("_SURF_FIND", "_SURF_FIND", PROMPT_FIND) },
	{ MODKEY,                GDK_KEY_slash,  spawn,      SETPROP("_SURF_FIND", "_SURF_FIND", PROMPT_FIND) },

	{ 0,                     GDK_KEY_Escape, stop,       { 0 } },
	{ MODKEY,                GDK_KEY_c,      stop,       { 0 } },

	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_r,      reload,     { .i = 1 } },
	{ MODKEY,                GDK_KEY_r,      reload,     { .i = 0 } },

	{ MODKEY,                GDK_KEY_Q,      navigate,   { .i = +1 } },
	{ MODKEY,                GDK_KEY_E,      navigate,   { .i = -1 } },

	/* vertical and horizontal scrolling, in viewport percentage */
	{ MODKEY,                GDK_KEY_j,      scrollv,    { .i = +10 } },
	{ MODKEY,                GDK_KEY_k,      scrollv,    { .i = -10 } },
	{ MODKEY,                GDK_KEY_u,      scrollv,    { .i = +50 } },
	{ MODKEY,                GDK_KEY_d,      scrollv,    { .i = -50 } },
	{ MODKEY,                GDK_KEY_h,      scrollh,    { .i = +10 } },
	{ MODKEY,                GDK_KEY_l,      scrollh,    { .i = -10 } },


	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_j,      zoom,       { .i = -1 } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_k,      zoom,       { .i = +1 } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_0,      zoom,       { .i =  0  } },
	{ MODKEY,                GDK_KEY_minus,  zoom,       { .i = -1 } },
	{ MODKEY,                GDK_KEY_equal,  zoom,       { .i = +1 } },

	{ MODKEY,                GDK_KEY_v,      clipboard,  { .i = 1 } },
	{ MODKEY,                GDK_KEY_c,      clipboard,  { .i = 0 } },

	{ MODKEY,                GDK_KEY_n,      find,       { .i = +1 } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_n,      find,       { .i = -1 } },

	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_p,      print,      { 0 } },
	{ MODKEY,                GDK_KEY_t,      showcert,   { 0 } },

	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_a,      togglecookiepolicy, { 0 } },
	{ 0,                     GDK_KEY_F11,    togglefullscreen, { 0 } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_o,      toggleinspector, { 0 } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_e,      toggletitle,        { 0 } },

	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_c,      toggle,     { .i = CaretBrowsing } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_f,      toggle,     { .i = FrameFlattening } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_g,      toggle,     { .i = Geolocation } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_s,      toggle,     { .i = JavaScript } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_i,      toggle,     { .i = LoadImages } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_b,      toggle,     { .i = ScrollBars } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_t,      toggle,     { .i = StrictTLS } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_m,      toggle,     { .i = Style } },

	/* download-console */
	{ MODKEY,                GDK_KEY_d,      spawndls,   { 0 } },
};

/* button definitions */
/* target can be OnDoc, OnLink, OnImg, OnMedia, OnEdit, OnBar, OnSel, OnAny */
static Button buttons[] = {
	/* target       event mask      button  function        argument        stop event */
	{ OnLink,       0,              2,      clicknewwindow, { .i = 0 },     1 },
	{ OnLink,       MODKEY,         2,      clicknewwindow, { .i = 1 },     1 },
	{ OnLink,       MODKEY,         1,      clicknewwindow, { .i = 1 },     1 },
	{ OnAny,        0,              8,      clicknavigate,  { .i = -1 },    1 },
	{ OnAny,        0,              9,      clicknavigate,  { .i = +1 },    1 },
	{ OnMedia,      MODKEY,         1,      clickexternplayer, { 0 },       1 },
};
