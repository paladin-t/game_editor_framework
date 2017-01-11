/**
 * @version: 1.6
 * @author: Wang Renxin,
 *     https://github.com/paladin-t/game_editor_framework
 *     mailto:hellotony521@qq.com
 * @file: This file is a part of GEF, for copyright detail
 *     information, see the LICENSE file.
 */

#include <assert.h>
#include <sstream>

#include "../gef_common_message/gef_common_message.h"
#include "gef_service_base.h"

namespace gef {

void GEF_EXCEPT(ConstStr fmt, ...) {
	char _text[_MAX_PATH];
	va_list argptr;
	va_start(argptr, fmt);
	vsprintf_s(_text, fmt, argptr);
	va_end(argptr);

#ifdef _DEBUG
	OutputDebugStringA(_text);
	OutputDebugStringA("\n");
#endif

	throw GefServiceBaseException(_text);
}

void GEF_ENSURE(bool exp, ConstStr fmt, ...) {
	char _text[_MAX_PATH];
	va_list argptr;
	va_start(argptr, fmt);
	vsprintf_s(_text, fmt, argptr);
	va_end(argptr);

	if(!exp) GEF_EXCEPT("%s", _text);
}

Obj Obj::NIL = Obj();

StdStr Obj::toStr(void) const {
	StdStr result;
	std::stringstream ss;
	switch(type) {
		case OT_NIL:
			ss << "(nil)";
			break;
		case OT_S32:
			ss << data._s32;
			break;
		case OT_U32:
			ss << data._u32;
			break;
		case OT_S64:
			ss << data._s64;
			break;
		case OT_U64:
			ss << data._u64;
			break;
		case OT_F32:
			ss << data._f32;
			break;
		case OT_F64:
			ss << data._f64;
			break;
		case OT_STR:
			ss << data._str;
			break;
		case OT_PTR:
			union { Ptr ptr; u32 u; } d;
			d.ptr = data._ptr;
			ss << d.u;
			break;
		case OT_BLN:
			ss << (data._bln ? "true" : "false");
			break;
		default:
			assert(!"Invalid type");
			break;
	}
	ss >> result;

	return result;
}

IService::IService()
	: mBubbler(NULL), mBroadcaster(NULL) {
}

IService::~IService() {
}

bool IService::regMsgProc(MsgGroup g, MsgType t, MsgProcFunc f) {
	return impl_regMsgProc(g, t, f);
}

bool IService::unregMsgProc(MsgGroup g, MsgType t) {
	return impl_unregMsgProc(g, t);
}

void IService::setMsgBubbler(MsgProcFunc f) {
	mBubbler = f;
}

void IService::setMsgBroadcaster(MsgProcFunc f) {
	mBroadcaster = f;
}

void IService::bubble(MsgGroup g, MsgType t, const ParamList &p) {
	if(mBubbler) mBubbler(g, t, p);
	else GEF_EXCEPT("Bubbler not set yet");
}

void IService::broadcast(MsgGroup g, MsgType t, const ParamList &p) {
	if(mBroadcaster) mBroadcaster(g, t, p);
	else GEF_EXCEPT("Broadcaster not set yet");
}

void IService::sink(MsgGroup g, MsgType t, const ParamList &p) {
	impl_sink(g, t, p);
}

bool IService::getEdited(void) {
	return impl_getEdited();
}

void IService::postInitDone(void) {
	impl_postInitDone();
}

void IService::prevDestroy(void) {
	impl_prevDestroy();
}

void IService::echo(const ParamList &p) {
	bubble((MsgGroup)MGT_SYSTEM, (MsgType)MST_SYS_ECHO, p);
}

void IService::echo(ConstStr fmt, ...) {
	char _text[_MAX_PATH];
	va_list argptr;
	va_start(argptr, fmt);
	vsprintf_s(_text, fmt, argptr);
	va_end(argptr);
	ParamList m;
	m.push_back(Obj(_text));
	echo(m);
}

void IService::impl_postInitDone(void) {
	// do nothing
}

void IService::impl_prevDestroy(void) {
	// do nothing
}

static ConstStr const _NIL_OBJECT_NAME = "(nil)";

ICommand::ICommand() {
	static u32 id = 0;
	mId = ++id;
	mParamDict = new StrParamDict;
	(*mParamDict)[_NIL_OBJECT_NAME] = Obj();
}

ICommand::~ICommand() {
	delete mParamDict;
}

void ICommand::_setId(u32 id) {
	mId = id;
}

u32 ICommand::_getId(void) const {
	return mId;
}

void ICommand::setParam(const StdStr &n, const Obj &v) {
	impl_setParam(n, v);
}

const Obj &ICommand::getParam(const StdStr &n) const {
	return impl_getParam(n);
}

void ICommand::exec(void) {
	impl_exec();
}

void ICommand::undo(void) {
	impl_undo();
}

void ICommand::redo(void) {
	impl_redo();
}

void ICommand::impl_setParam(const StdStr &n, const Obj &v) {
	(*mParamDict)[n] = v;
}

const Obj &ICommand::impl_getParam(const StdStr &n) const {
	if(mParamDict->find(n) != mParamDict->end()) return (*mParamDict)[n];
	else return (*mParamDict)[_NIL_OBJECT_NAME];
}

}
