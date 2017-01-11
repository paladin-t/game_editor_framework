/**
 * @version: 1.0
 * @author: Wang Renxin,
 *     https://github.com/paladin-t/game_editor_framework
 *     mailto:hellotony521@qq.com
 * @file: This file is a part of GEF, for copyright detail
 *     information, see the LICENSE file.
 */

#ifndef __GEF_COMMON_MESSAGE_PROCESSOR_H__
#define __GEF_COMMON_MESSAGE_PROCESSOR_H__

#ifdef GEF_MSG_DOTNET_COMPILE

namespace gef {

using namespace System;
using namespace System::Collections::Generic;

public ref class MessageProcessor {

public:
    delegate Void MsgEventHandler(UInt32 g, UInt32 t, List<Object^>^ p);

protected:
	typedef KeyValuePair<UInt32, UInt32> MsgKey;
	typedef Dictionary<MsgKey, MsgEventHandler^> MsgDict;

public:
	MessageProcessor();
	virtual ~MessageProcessor();

	Boolean RegMsgProc(UInt32 g, UInt32 t, MsgEventHandler^ proc);
	Boolean UnregMsgProc(UInt32 g, UInt32 t, MsgEventHandler^ proc);
	Void Raise(UInt32 g, UInt32 t, List<Object^>^ p);

protected:
	MsgDict^ msgDict;

};

}

#endif // GEF_MSG_DOTNET_COMPILE

#endif // __GEF_COMMON_MESSAGE_PROCESSOR_H__
