/**
 * @version: 1.1
 * @author: Wang Renxin,
 *     https://github.com/paladin-t/game_editor_framework
 *     mailto:hellotony521@qq.com
 * @file: This file is a part of GEF, for copyright detail
 *     information, see the LICENSE file.
 */

#ifndef __GEF_DOTNET_UTIL_H__
#define __GEF_DOTNET_UTIL_H__

#include <string>

#include "../gef_service_base/gef_service_base.h"

using namespace System;
using namespace System::Collections::Generic;

namespace gef {

public ref class WrapperUtil sealed {

public:
	static property String^ ApplicationDirectory {
		String^ get();
	}

	static String^ ConvertString(ConstStr n);
	static StdStr ConvertString(String^ m);

	static Object^ ConvertObject(const Obj &n);
	static Obj ConvertObject(Object^ m);

	static List<Object^>^ ConvertParamList(const ParamList &n);
	static ParamList ConvertParamList(List<Object^>^ m);
	static void ConvertParamList(List<Object^>^ m, ParamList &o);

	static Dictionary<String^, Object^>^ ConvertParamDict(const StrParamDict &n);
	static StrParamDict ConvertParamDict(Dictionary<String^, Object^>^ m);
	static void ConvertParamDict(Dictionary<String^, Object^>^ m, StrParamDict &o);

	static Dictionary<Object^, Object^>^ ConvertParamDict(const ObjParamDict &n);
	static ObjParamDict ConvertParamDict(Dictionary<Object^, Object^>^ m);
	static void ConvertParamDict(Dictionary<Object^, Object^>^ m, ObjParamDict &o);

};

}

#endif // __GEF_DOTNET_UTIL_H__
