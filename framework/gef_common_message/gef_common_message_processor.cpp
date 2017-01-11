/**
 * @version: 1.0
 * @author: Wang Renxin,
 *     https://github.com/paladin-t/game_editor_framework
 *     mailto:hellotony521@qq.com
 * @file: This file is a part of GEF, for copyright detail
 *     information, see the LICENSE file.
 */

#define GEF_MSG_DOTNET_COMPILE

#include "gef_common_message_processor.h"

namespace gef {

MessageProcessor::MessageProcessor() {
	msgDict = gcnew MsgDict();
}

MessageProcessor::~MessageProcessor() {
	msgDict = nullptr;
}

Boolean MessageProcessor::RegMsgProc(UInt32 g, UInt32 t, MsgEventHandler^ proc) {
	if(proc == nullptr) return false;
	if(!msgDict->ContainsKey(MsgKey(g, t)))
		msgDict->Add(MsgKey(g, t), nullptr);
	msgDict[MsgKey(g, t)] += proc;

	return true;
}

Boolean MessageProcessor::UnregMsgProc(UInt32 g, UInt32 t, MsgEventHandler^ proc) {
	if(proc == nullptr) return false;
	if(msgDict->ContainsKey(MsgKey(g, t)))
		msgDict[MsgKey(g, t)] -= proc;
	else
		return false;
	return true;
}

Void MessageProcessor::Raise(UInt32 g, UInt32 t, List<Object^>^ p) {
	if(!msgDict->ContainsKey(MsgKey(g, t))) return;
	if(msgDict[MsgKey(g, t)])
		msgDict[MsgKey(g, t)]->Invoke(g, t, p);
}

}

#undef GEF_MSG_DOTNET_COMPILE
