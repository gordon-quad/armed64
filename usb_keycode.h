/*
 * This file is part of the armed64 distribution (https://github.com/gordon-quad/armed64)
 * Copyright (c) 2015-2016 by Willem Dijkstra <wpd@xs4all.nl>.
 * Copyright (c) 2017 by Gordon Quad <gordon@niflheim.info>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _KEYCODE_H
#define _KEYCODE_H

/*
 * Keycode definitions as present in the usb hid usage tables, and as available
 * as valid key events for linux.
 *
 * Refer to
 * linux: source/drivers/hid/hid-input.c
 * usb hid usage tables v 1.12
 */
enum hid_keyboard_keypad_usage {
    EVENT_NONE = 0,
    EVENT_ERRORROLLOVER,
    EVENT_POSTFAIL,
    EVENT_ERRORUNDEFINED,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_0,
    KEY_ENTER,
    KEY_ESC,
    KEY_BACKSPACE,
    KEY_TAB,
    KEY_SPACE,
    KEY_MINUS,
    KEY_EQUAL,
    KEY_LEFTBRACE,
    KEY_RIGHTBRACE,
    KEY_BACKSLASH,
    KEY_NUMBER,
    KEY_SEMICOLON,
    KEY_QUOTE,
    KEY_BACKTICK,
    KEY_COMMA,
    KEY_PERIOD,
    KEY_SLASH,
    KEY_CAPS_LOCK,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEY_PRINTSCREEN,
    KEY_SCROLL_LOCK,
    KEY_PAUSE,
    KEY_INSERT,
    KEY_HOME,
    KEY_PAGE_UP,
    KEY_DELETE,
    KEY_END,
    KEY_PAGE_DOWN,
    KEY_RIGHT,
    KEY_LEFT,
    KEY_DOWN,
    KEY_UP,
    KEY_NUM_LOCK,
    KEY_PAD_SLASH,
    KEY_PAD_ASTERISK,
    KEY_PAD_MINUS,
    KEY_PAD_PLUS,
    KEY_PAD_ENTER,
    KEY_PAD_1,
    KEY_PAD_2,
    KEY_PAD_3,
    KEY_PAD_4,
    KEY_PAD_5,
    KEY_PAD_6,
    KEY_PAD_7,
    KEY_PAD_8,
    KEY_PAD_9,
    KEY_PAD_0,
    KEY_PAD_PERIOD,
    KEY_ISO_SLASH,
    KEY_APP,
    EVENT_STATUS,
    KEY_PAD_EQUAL,
    KEY_F13,
    KEY_F14,
    KEY_F15,
    KEY_F16,
    KEY_F17,
    KEY_F18,
    KEY_F19,
    KEY_F20,
    KEY_F21,
    KEY_F22,
    KEY_F23,
    KEY_F24,
    KEY_EXEC,
    KEY_HELP,
    KEY_MENU,
    KEY_SELECT,
    KEY_STOP,
    KEY_AGAIN,
    KEY_UNDO,
    KEY_CUT,
    KEY_COPY,
    KEY_PASTE,
    KEY_FIND,
    KEY_MUTE,
    KEY_VOL_UP,
    KEY_VOL_DOWN,
    KEY_CAPS_TLOCK,
    KEY_NUM_TLOCK,
    KEY_SCROLL_TLOCK,
    KEY_PAD_COMMA, /* Brazilian keypad period */
    KEY_PAD_EQUAL_AS, /* AS/400 Keyboard */
    KEY_INTER1, /* KANJI1 - Brazillian and Japanese "Ru" and "-" */
    KEY_INTER2, /* KANJI2 - Japanese Katakana/Hiragana */
    KEY_INTER3, /* KANJI3 - Japanese Yen */
    KEY_INTER4, /* KANJI4 - Japanese Henkan */
    KEY_INTER5, /* KANJI5 - Japanese Muhenkan */
    KEY_INTER6, /* KANJI6 - PC('USB', 0x62) Comma (Ka-m-ma) */
    KEY_INTER7, /* KANJI7 - Double-Byte/Single-Byte Toggle */
    KEY_INTER8, /* KANJI8 - Undefined */
    KEY_INTER9, /* KANJI9 - Undefined */
    KEY_LANG1, /* Korean Hangul/English Toggle */
    KEY_LANG2, /* Korean Hanja Conversion - Japanese Eisu */
    KEY_LANG3, /* Japanese Katakana Key (USB) */
    KEY_LANG4, /* Japanese Hiragana Key (USB) */
    KEY_LANG5, /* Japanese Zenkaku/Hankaku Key (USB) */
    KEY_LANG6, /* Reserved (Application Specific) */
    KEY_LANG7, /* Reserved (Application Specific) */
    KEY_LANG8, /* Reserved (Application Specific) */
    KEY_LANG9, /* Reserved (Application Specific) */
    KEY_ALT_ERASE, /* Special Erase (See Spec) */
    KEY_SYSREQ_ATT, /* Modifier Type */
    KEY_CANCEL,
    KEY_CLEAR,
    KEY_PRIOR,
    KEY_RETURN,
    KEY_SEPARATOR,
    KEY_OUT,
    KEY_OPER,
    KEY_CLEAR_AGAIN,
    KEY_CRSEL_PROPS,
    KEY_EXSEL,

    /* 0xa5 - 0xaf Reserved */

    KEY_PAD_00 = 0xB0,
    KEY_PAD_000,
    KEY_1000_SEP,
    KEY_DECIMAL_SEP,
    KEY_CURRENCY_MAIN,
    KEY_CURRENCY_SUB,
    KEY_PAD_LPAREN,
    KEY_PAD_RPAREN,
    KEY_PAD_LBRACE,
    KEY_PAD_RBRACE,
    KEY_PAD_TAB,
    KEY_PAD_BACKSPACE,
    KEY_PAD_A,
    KEY_PAD_B,
    KEY_PAD_C,
    KEY_PAD_D,
    KEY_PAD_E,
    KEY_PAD_F,
    KEY_PAD_XOR,
    KEY_PAD_CHEVRON,
    KEY_PAD_PERCENT,
    KEY_PAD_LTHAN,
    KEY_PAD_GTHAN,
    KEY_PAD_BITAND,
    KEY_PAD_AND,
    KEY_PAD_BITOR,
    KEY_PAD_OR,
    KEY_PAD_COLON,
    KEY_PAD_POUND,
    KEY_PAD_SPACE,
    KEY_PAD_AT,
    KEY_PAD_EXCLAIM,
    KEY_PAD_MEM_STORE,
    KEY_PAD_MEM_RECALL,
    KEY_PAD_MEM_CLEAR,
    KEY_PAD_MEM_ADD,
    KEY_PAD_MEM_SUB,
    KEY_PAD_MEM_MULT,
    KEY_PAD_MEM_DIV,
    KEY_PAD_PLUS_MINUS,
    KEY_PAD_CLEAR,
    KEY_PAD_CLEAR_ENTRY,
    KEY_PAD_BINARY,
    KEY_PAD_OCTAL,
    KEY_PAD_DECIMAL,
    KEY_PAD_HEX,

    /* 0xde - 0xdf = Reserved */

    KEY_LCTRL = 0xE0,
    KEY_LSHIFT,
    KEY_LALT,
    KEY_LGUI,
    KEY_RCTRL,
    KEY_RSHIFT,
    KEY_RALT,
    KEY_RGUI,

    /* 0xe8 - 0xff = Reserved */
};

#define MODIFIER_BIT(mod) (1<<(mod & 0x07))

enum hid_system_usage {

    /* 0x00 - 0x3c = Non momentary controls */

    SYSTEM_START = 0x003D,
    SYSTEM_SELECT,

    /* 0x3f - 0x80 = Reserved / Non momentary controls */

    SYSTEM_POWER = 0x0081,
    SYSTEM_SLEEP,
    SYSTEM_WAKEUP,
    SYSTEM_CONTEXTMENU,
    SYSTEM_MAINMENU,
    SYSTEM_APPMENU,
    SYSTEM_MENUHELP,
    SYSTEM_MENUEXIT,
    SYSTEM_MENUSELECT,
    SYSTEM_MENURIGHT,
    SYSTEM_MENULEFT,
    SYSTEM_MENUUP,
    SYSTEM_MENUDOWN,
    SYSTEM_COLDRESTART,
    SYSTEM_WARMRESTART,
    SYSTEM_DPADUP,
    SYSTEM_DPADDOWN,
    SYSTEM_DPADRIGHT,
    SYSTEM_DPADLEFT,
};

enum hid_consumer_usage {

    CONSUMER_POWER = 0x30,
    CONSUMER_RESET,
    CONSUMER_SLEEP,
    CONSUMER_SLEEPAFTER,
    CONSUMER_SLEEPMODE,
    CONSUMER_ILLUMINATION,
    CONSUMER_FUNCBTNS,

    /* 0x37 - 0x3f = Reserved */

    CONSUMER_MENU = 0x40,
    CONSUMER_MENUPICK,
    CONSUMER_MENUUP,
    CONSUMER_MENUDOWN,
    CONSUMER_MENULEFT,
    CONSUMER_MENURIGHT,
    CONSUMER_MENUESCAPE,
    CONSUMER_MENUVALINC,
    CONSUMER_MENUVALDEC,

    /* 0x49 - 0x5f = Reserved */

    CONSUMER_DATAONSCREEN = 0x60,
    CONSUMER_CC,
    CONSUMER_CCSELECT,
    CONSUMER_VCRTV,
    CONSUMER_BROADCASTMODE,
    CONSUMER_SNAPSHOT,
    CONSUMER_STILL,

    /* 0x67 - 0x7f = Reserved */

    CONSUMER_SELECTION = 0x80,
    CONSUMER_ASSIGNSELECTION,
    CONSUMER_MODESTEP,
    CONSUMER_RECALLLAST,
    CONSUMER_ENTERCHANNEL,
    CONSUMER_ORDERMOVIE,
    CONSUMER_CHANNEL,
    CONSUMER_MEDIASELECTION,
    CONSUMER_MEDIASELECTCOMPUTER,
    CONSUMER_MEDIASELECTTV,
    CONSUMER_MEDIASELECTWWW,
    CONSUMER_MEDIASELECTDVD,
    CONSUMER_MEDIASELECTTELEPHONE,
    CONSUMER_MEDIASELECTPROGRAMGUIDE,
    CONSUMER_MEDIASELECTVIDEOPHONE,
    CONSUMER_MEDIASELECTGAMES,
    CONSUMER_MEDIASELECTMESSAGES = 0x90,
    CONSUMER_MEDIASELECTCD,
    CONSUMER_MEDIASELECTVCR,
    CONSUMER_MEDIASELECTTUNER,
    CONSUMER_QUIT,
    CONSUMER_HELP,
    CONSUMER_MEDIASELECTTAPE,
    CONSUMER_MEDIASELECTCABLE,
    CONSUMER_MEDIASELECTSATELLITE,
    CONSUMER_MEDIASELECTSECURITY,
    CONSUMER_MEDIASELECTHOME,
    CONSUMER_MEDIASELECTCALL,
    CONSUMER_CHANNELINC,
    CONSUMER_CHANNELDEC,
    CONSUMER_MEDIASELECTSAP,

    /* 0x9f = Reserved */

    CONSUMER_VCRPLUS = 0xA0,
    CONSUMER_ONCE,
    CONSUMER_DAILY,
    CONSUMER_WEEKLY,
    CONSUMER_MONTHLY,

    /* 0xa5 - 0xaf = Reserved */

    CONSUMER_PLAY = 0xB0,
    CONSUMER_PAUSE,
    CONSUMER_RECORD,
    CONSUMER_FASTFORWARD,
    CONSUMER_REWIND,
    CONSUMER_SCANNEXTTRACK,
    CONSUMER_SCANPREVIOUSTRACK,
    CONSUMER_STOP,
    CONSUMER_EJECT,
    CONSUMER_RANDOMPLAY,
    CONSUMER_SELECTDISC,
    CONSUMER_ENTERDISC,
    CONSUMER_REPEAT,
    CONSUMER_TRACKING,
    CONSUMER_TRACKNORMAL,
    CONSUMER_SLOWTRACKING,
    CONSUMER_FRAMEFORWARD,
    CONSUMER_FRAMEBACK,
    CONSUMER_MARK,
    CONSUMER_CLEARMARK,
    CONSUMER_REPEATFROMMARK,
    CONSUMER_RETURNTOMARK,
    CONSUMER_SEARCHMARKFORWARD,
    CONSUMER_SEARCHMARKBACKWARDS,
    CONSUMER_COUNTERRESET,
    CONSUMER_SHOWCOUNTER,
    CONSUMER_TRACKINGINC,
    CONSUMER_TRACKINGDEC,
    CONSUMER_STOPEJECT,
    CONSUMER_PLAYPAUSE,
    CONSUMER_PLAYSKIP,

    /* 0xcf - 0xdf = Reserved */

    CONSUMER_VOLUME = 0xE0,
    CONSUMER_BALANCE,
    CONSUMER_MUTE,
    CONSUMER_BASS,
    CONSUMER_TREBLE,
    CONSUMER_BASSBOOST,
    CONSUMER_SURROUNDMODE,
    CONSUMER_LOUDNESS,
    CONSUMER_MPX,
    CONSUMER_VOLUMEINC,
    CONSUMER_VOLUMEDEC,

    /* 0xeb - 0xef = Reserved */

    /* 0xf6 - 0xff = Reserved */

    /* 0x10e - 0x14f = Reserved */

    /* 0x156 - 0x15f = Reserved */

    /* 0x16b - 0x16f = Reserved */

    /* 0x175 - 0x17f = Reserved */

    CONSUMER_APPLICATIONLAUNCHBUTTONS = 0x180,
    CONSUMER_AL_CONFIGURATION,
    CONSUMER_AL_PROGRAMMABLEBUTTON,
    CONSUMER_AL_CONSUMERCONTROL,
    CONSUMER_AL_WORDPROCESSOR,
    CONSUMER_AL_TEXTEDITOR,
    CONSUMER_AL_SPREADSHEET,
    CONSUMER_AL_GRAPHICSEDITOR,
    CONSUMER_AL_PRESENTATIONAPP,
    CONSUMER_AL_DATABASEAPP,
    CONSUMER_AL_EMAILREADER,
    CONSUMER_AL_NEWSREADER,
    CONSUMER_AL_VOICEMAIL,
    CONSUMER_AL_ADDRESSBOOK,
    CONSUMER_AL_CALENDAR,
    CONSUMER_AL_TASKMANAGER,
    CONSUMER_AL_JOURNAL = 0x190,
    CONSUMER_AL_FINANCE,
    CONSUMER_AL_CALCULATOR,
    CONSUMER_AL_PLAYBACK,
    CONSUMER_AL_LOCALBROWSER,
    CONSUMER_AL_BROWSER,
    CONSUMER_AL_INTERNETBROWSER,
    CONSUMER_AL_REMOTENETWORKING,
    CONSUMER_AL_CONFERENCE,
    CONSUMER_AL_CHAT,
    CONSUMER_AL_DIALER,
    CONSUMER_AL_LOGON,
    CONSUMER_AL_LOGOFF,
    CONSUMER_AL_LOGONLOGOFF,
    CONSUMER_AL_TERMINALLOCK,
    CONSUMER_AL_CONTROLPANEL,
    CONSUMER_AL_COMMANDLINE,
    CONSUMER_AL_PROCESSMANAGER,
    CONSUMER_AL_SELECTTASK,
    CONSUMER_AL_NEXTTASK,
    CONSUMER_AL_PREVIOUSTASK,
    CONSUMER_AL_PREEMPTIVEHALT,
    CONSUMER_AL_HELPCENTER,
    CONSUMER_AL_DOCUMENTS,
    CONSUMER_AL_THESAURUS,
    CONSUMER_AL_DICTIONARY,
    CONSUMER_AL_DESKTOP,
    CONSUMER_AL_SPELLCHECK,
    CONSUMER_AL_GRAMMARCHECK,
    CONSUMER_AL_WIRELESSSTATUS,
    CONSUMER_AL_KEYBOARDLAYOUT,
    CONSUMER_AL_VIRUSPROTECTION,
    CONSUMER_AL_ENCRYPTION,
    CONSUMER_AL_SCREENSAVER,
    CONSUMER_AL_ALARMS,
    CONSUMER_AL_CLOCK,
    CONSUMER_AL_FILEBROWSER,
    CONSUMER_AL_POWERSTATUS,
    CONSUMER_AL_IMAGEBROWSER,
    CONSUMER_AL_AUDIOBROWSER,
    CONSUMER_AL_MOVIEBROWSER,
    CONSUMER_AL_DIGITALRIGHTSMANAGER,
    CONSUMER_AL_DIGITALWALLET,

    /* 0x1bb = Reserved_*/

    CONSUMER_AL_IM = 0x1bc,
    CONSUMER_AL_INFO,

    /* 0x1c8 - 0x1ff = Reserved */

    CONSUMER_GENERICGUIAPPLICATION = 0x200,
    CONSUMER_AC_NEW,
    CONSUMER_AC_OPEN,
    CONSUMER_AC_CLOSE,
    CONSUMER_AC_EXIT,
    CONSUMER_AC_MAXIMIZE,
    CONSUMER_AC_MINIMIZE,
    CONSUMER_AC_SAVE,
    CONSUMER_AC_PRINT,
    CONSUMER_AC_PROPERTIES,
    CONSUMER_AC_UNDO,
    CONSUMER_AC_COPY,
    CONSUMER_AC_CUT,
    CONSUMER_AC_PASTE,
    CONSUMER_AC_SELECTALL,
    CONSUMER_AC_FIND,
    CONSUMER_AC_FINDREPLACE,
    CONSUMER_AC_SEARCH,
    CONSUMER_AC_GOTO,
    CONSUMER_AC_HOME,
    CONSUMER_AC_BACK,
    CONSUMER_AC_FORWARD,
    CONSUMER_AC_STOP,
    CONSUMER_AC_REFRESH,
    CONSUMER_AC_PREVIOUSLINK,
    CONSUMER_AC_NEXTLINK,
    CONSUMER_AC_BOOKMARKS,
    CONSUMER_AC_HISTORY,
    CONSUMER_AC_SUBSCRIPTIONS,
    CONSUMER_AC_ZOOMIN,
    CONSUMER_AC_ZOOMOUT,
    CONSUMER_AC_ZOOM,

    CONSUMER_AC_SCROLLUP = 0x233,
    CONSUMER_AC_SCROLLDOWN,
    CONSUMER_AC_SCROLL,
    CONSUMER_AC_PANLEFT,
    CONSUMER_AC_PANRIGHT,
    CONSUMER_AC_PAN,
    CONSUMER_AC_NEWWINDOW,
    CONSUMER_AC_TILEHORIZONTALLY,
    CONSUMER_AC_TILEVERTICALLY,
    CONSUMER_AC_FORMAT,
    CONSUMER_AC_EDIT,

    CONSUMER_AC_CANCEL = 0x25f,

    CONSUMER_AC_INSERT = 0x269,
    CONSUMER_AC_DELETE,

    CONSUMER_AC_REDO = 0x279,

    CONSUMER_AC_REPLY = 0x289,
    CONSUMER_AC_FORWARDMSG = 0x28b,
    CONSUMER_AC_SEND,
};
#endif
