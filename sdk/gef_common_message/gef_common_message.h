/**
 * @version: 1.4
 * @author: Wang Renxin,
 *     https://github.com/paladin-t/game_editor_framework
 *     mailto:hellotony521@qq.com
 * @file: This file is a part of GEF, for copyright detail
 *     information, see the LICENSE file.
 */

#ifndef __GEF_COMMON_MESSAGE_H__
#define __GEF_COMMON_MESSAGE_H__

#ifndef GEF_ENUM
#	ifdef GEF_MSG_DOTNET_COMPILE
#		define GEF_ENUM public enum class
#	else // GEF_MSG_DOTNET_COMPILE
#		define GEF_ENUM enum
#	endif // GEF_MSG_DOTNET_COMPILE
#endif // GEF_ENUM

namespace gef {

#ifdef GEF_MSG_DOTNET_COMPILE

public enum class PluginTabPageOrder {
	PO_HEAD = 0,
	PO_FILE = 100,
	PO_EDIT = 150,
	PO_SCRIPT = 200,
	PO_HELP = 250,
	PO_DEFAULT = 500,
	PO_USER = 600,
};

#endif // GEF_MSG_DOTNET_COMPILE

GEF_ENUM MsgGroupTypes {
	MGT_SYSTEM,
	MGT_INPUT_KEYBOARD,
	MGT_INPUT_MOUSE,
	MGT_FILE,
	MGT_EDIT,
	MGT_SCRIPT,
	MGT_HELP,
	MGT_USER,
};

GEF_ENUM MsgSystemTypes {
	MST_SYS_EXIT,             // sink
	MST_SYS_REQ_HWND,         // bubble
	MST_SYS_RES_HWND,         // sink
	MST_SYS_RESIZE,           // sink
	MST_SYS_LOST_FOCUS,       // sink
	MST_SYS_GAIN_FOCUS,       // sink
	MST_SYS_TIMER,            // sink
	MST_SYS_PAUSE_RENDER,     // bubble
	MST_SYS_RESUME_RENDER,    // bubble
	MST_SYS_REFRESH_CTRL,     // bubble
	MST_SYS_ACTIVE_TAB,       // sink
	MST_SYS_PANEL_GAIN_FOCUS, // bubble
	MST_SYS_PANEL_LOST_FOCUS, // bubble
	MST_SYS_ECHO,             // bubble
	MST_SYS_DRAG_ENTER,       // bubble
	MST_SYS_DRAG_OVER,        // bubble
	MST_SYS_DRAG_DROP,        // bubble
	MST_SYS_DRAG_LEAVE,       // bubble
	MST_SYS_USER,
};

GEF_ENUM MsgInputKeyboardTypes {
	MIK_KEY_DOWN,    // sink
	MIK_KEY_UP,      // sink
	MIK_KEY_PRESSED, // sink
	MIK_KEY_USER,
};

GEF_ENUM MsgInputMouseTypes {
	MIM_MOUSE_DOWN,           // sink
	MIM_MOUSE_UP,             // sink
	MIM_MOUSE_CLICKED,        // sink
	MIM_MOUSE_DOUBLE_CLICKED, // sink
	MIM_MOUSE_MOVE,           // sink
	MIM_MOUSE_WHEEL,          // sink
	MIM_MOUSE_USER,
};

GEF_ENUM MsgFileTypes {
	MFT_FILE_NEW,        // sink
	MFT_FILE_OPEN,       // sink
	MFT_FILE_SAVE,       // sink; ask the service to save current workspace
	MFT_FILE_SAVE_AS,    // sink; ask the service to save current workspace to another space
	MFT_FILE_IMPORT,     // sink
	MFT_FILE_EXPORT,     // sink
	MFT_FILE_REQ_OPEN,   // bubble
	MFT_FILE_RES_OPEN,   // sink
	MFT_FILE_REQ_SAVE,   // bubble; request the shell to indicate a saving path
	MFT_FILE_RES_SAVE,   // sink; response a indicated saving path to the service
	MFT_FILE_MARK_SAVED, // bubble; tell the system that current workspace is saved
	MFT_FILE_USER,
};

GEF_ENUM MsgEditTypes {
	MET_EDIT_DELETE,   // sink
	MET_EDIT_COPY,     // sink
	MET_EDIT_CUT,      // sink
	MET_EDIT_PASTE,    // sink
	MET_EDIT_UNDO,     // sink
	MET_EDIT_REDO,     // sink
	MET_EDIT_HAS_UNDO, // bubble
	MET_EDIT_HAS_REDO, // bubble
	MET_EDIT_ADD_CMD,  // bubble
	MET_EDIT_USER,
};

GEF_ENUM MsgScriptTypes {
	MST_SCRIPT_REQ_LIST, // sink
	MST_SCRIPT_RES_LIST, // bubble
	MST_SCRIPT_RUN,      // sink
	MST_SCRIPT_USER,
};

GEF_ENUM MsgHelpTypes {
	MHT_HELP_SHOW_ABOUT, // sink
	MHT_HELP_SHOW_HELP,  // sink
	MHT_HELP_USER,
};

GEF_ENUM GefMouseButtons {
    GMB_NONE = 0,
    GMB_LEFT = 1048576,
    GMB_RIGHT = 2097152,
    GMB_MIDDLE = 4194304,
    GMB_XBUTTON1 = 8388608,
    GMB_XBUTTON2 = 16777216,
};

}

#endif // __GEF_COMMON_MESSAGE_H__
