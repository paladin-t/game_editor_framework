/**
 * @version: 1.1
 * @author: Wang Renxin,
 *     https://github.com/paladin-t/game_editor_framework
 *     mailto:hellotony521@qq.com
 * @file: This file is a part of GEF, for copyright detail
 *     information, see the LICENSE file.
 */

#ifndef __GEF_COMMON_MESSAGE_TEST_CASE_H__
#define __GEF_COMMON_MESSAGE_TEST_CASE_H__

#ifndef GEF_ENUM
#	ifdef GEF_MSG_DOTNET_COMPILE
#		define GEF_ENUM public enum class
#	else // GEF_MSG_DOTNET_COMPILE
#		define GEF_ENUM enum
#		include "../../gef_common_message/gef_common_message.h"
#	endif // GEF_MSG_DOTNET_COMPILE
#endif // GEF_ENUM

namespace gef {

#pragma warning(push)
#pragma warning(disable : 4482)

GEF_ENUM MsgGroupTypesTestCase {
	MGT_TEST = MsgGroupTypes::MGT_USER + 1,
};

GEF_ENUM MsgTestTypes {
	MTT_TEST_COMMON,
	MTT_TEST_SET_CLEAR_COLOR,
	MTT_TEST_PRE_ADD_QUAD,
	MTT_TEST_SET_PRE_ADDED_QUAD_SIZE,
	MTT_TEST_ADD_QUAD,
	MTT_TEST_REMOVE_QUAD,
	MTT_TEST_PRE_MOVE_QUAD,
	MTT_TEST_MOVING_QUAD,
	MTT_TEST_MOVE_QUAD,
};

#pragma warning(pop)

}

#endif // __GEF_COMMON_MESSAGE_TEST_CASE_H__
