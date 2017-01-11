/**
 * @version: 1.0
 * @author: Wang Renxin,
 *     https://github.com/paladin-t/game_editor_framework
 *     mailto:hellotony521@qq.com
 * @file: This file is a part of GEF, for copyright detail
 *     information, see the LICENSE file.
 */

#ifndef __GEF_SERVICE_TEST_CASE_H__
#define __GEF_SERVICE_TEST_CASE_H__

#include "../../gef_service_base/gef_service_base.h"
#include "../gef_common_message_test_case/gef_common_message_test_case.h"

#ifdef GEF_SERVICE_TEST_CASE_EXPORTS
#	define GEF_SERVICE_TEST_CASE_API __declspec(dllexport)
#else
#	define GEF_SERVICE_TEST_CASE_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

GEF_SERVICE_TEST_CASE_API gef::IService* OpenGefService(void);
GEF_SERVICE_TEST_CASE_API void CloseGefService(gef::IService* svr);

GEF_SERVICE_TEST_CASE_API gef::ICommand* CreateGefCommand(void);
GEF_SERVICE_TEST_CASE_API gef::ICommand* CreateGefCommandByNum(gef::u32 n);
GEF_SERVICE_TEST_CASE_API gef::ICommand* CreateGefCommandByStr(gef::ConstStr s);
GEF_SERVICE_TEST_CASE_API void DestroyGefCommand(gef::ICommand* cmd);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __GEF_SERVICE_TEST_CASE_H__
