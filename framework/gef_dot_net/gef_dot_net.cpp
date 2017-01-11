/**
 * @version: 1.5
 * @author: Wang Renxin,
 *     https://github.com/paladin-t/game_editor_framework
 *     mailto:hellotony521@qq.com
 * @file: This file is a part of GEF, for copyright detail
 *     information, see the LICENSE file.
 */

#include "stdafx.h"

#include <assert.h>
#include <exception>
#include <eh.h>
#include <vcclr.h>

#include "gef_dot_net.h"
#include "gef_wrapper_util.h"

#define _DEFAULT_UNDO_QUEUE_SIZE 2048

using namespace System;
using namespace System::IO;
using namespace System::Windows::Forms;

static gcroot<gef::Service^> gService = nullptr;

static gcroot<gef::Service^> &getSvr(void) {
	return gService;
}

#ifndef _GEF_TRY
#	define _GEF_TRY try {
#endif // _GEF_TRY
#ifndef _GEF_CATCH
#	define _GEF_CATCH \
	} catch(const GefServiceBaseException &ex) { \
		String^ msg = WrapperUtil::ConvertString(ex.what()); \
		throw gcnew Exception(msg); \
	} catch(const std::exception &ex) { \
		String^ msg = WrapperUtil::ConvertString(ex.what()); \
		throw gcnew Exception(msg); \
	}
#endif // _GEF_CATCH

namespace gef {

CommandManager::CommandManager() {
	mSavedPoint = 0;
	mUndoQueueSize = _DEFAULT_UNDO_QUEUE_SIZE;
}

CommandManager::~CommandManager() {
	clearDone();
	clearUndone();
}

u32 CommandManager::_getCurrId(void) const {
	if(mDone.empty()) return 0;
	else return mDone.back()->_getId();
}

void CommandManager::_setSavedPoint(void) {
	mSavedPoint = _getCurrId();
}

u32 CommandManager::_getSavedPoint(void) const {
	return mSavedPoint;
}

void CommandManager::add(ICommand* cmd) {
	mDone.push_back(cmd);
	clearUndone();

	gService->ManagedEdited = true;
}

void CommandManager::undo(void) {
	if(canUndo()) {
		ICommand* cmd = mDone.back();
		mDone.pop_back();
		mUndone.push_back(cmd);
		cmd->undo();

		if(mUndone.size() > mUndoQueueSize) mUndone.pop_front();

		gService->ManagedEdited = _getSavedPoint() == _getCurrId() ? false : true;
	}
	{
		ParamList p;
		p.push_back(canUndo());
		gService->Bubble((MsgGroup)MsgGroupTypes::MGT_EDIT, (MsgType)MsgEditTypes::MET_EDIT_HAS_UNDO, p);
	}
	{
		ParamList p;
		p.push_back(canRedo());
		gService->Bubble((MsgGroup)MsgGroupTypes::MGT_EDIT, (MsgType)MsgEditTypes::MET_EDIT_HAS_REDO, p);
	}
}

void CommandManager::redo(void) {
	if(canRedo()) {
		ICommand* cmd = mUndone.back();
		mUndone.pop_back();
		mDone.push_back(cmd);
		cmd->redo();

		gService->ManagedEdited = _getSavedPoint() == _getCurrId() ? false : true;
	}
	{
		ParamList p;
		p.push_back(canUndo());
		gService->Bubble((MsgGroup)MsgGroupTypes::MGT_EDIT, (MsgType)MsgEditTypes::MET_EDIT_HAS_UNDO, p);
	}
	{
		ParamList p;
		p.push_back(canRedo());
		gService->Bubble((MsgGroup)MsgGroupTypes::MGT_EDIT, (MsgType)MsgEditTypes::MET_EDIT_HAS_REDO, p);
	}
}

bool CommandManager::canUndo(void) const {
	return !mDone.empty();
}

bool CommandManager::canRedo(void) const {
	return !mUndone.empty();
}

u32 CommandManager::getUndoQueueSize(void) const {
	return mUndoQueueSize;
}

void CommandManager::setUndoQueueSize(u32 s) {
	assert(s > 0);
	mUndoQueueSize = s;
}

void CommandManager::setDestroyGefCommandFunc(DestroyGefCommandFunc f) {
	mDestroyCmd = f;
}

void CommandManager::clearDone(void) {
	for(CmdStack::iterator it = mDone.begin(); it != mDone.end(); ++it) {
		ICommand* cmd = *it;
		(*mDestroyCmd)(cmd);
	}
	std::swap(mDone, CmdStack());
}

void CommandManager::clearUndone(void) {
	for(CmdStack::iterator it = mUndone.begin(); it != mUndone.end(); ++it) {
		ICommand* cmd = *it;
		(*mDestroyCmd)(cmd);
	}
	std::swap(mUndone, CmdStack());
}

}

namespace gef {

static void _bubble(MsgGroup g, MsgType t, const ParamList &p) {
	Service::_Bubble(g, t, p);
}

static void _broadcast(MsgGroup g, MsgType t, const ParamList &p) {
	Service::_Broadcast(g, t, p);
}

void Service::BubbledMsgEvent::add(BubbledMsgEventHandler^ _d) {
	OnBubbledMsgEvent += _d;
}

void Service::BubbledMsgEvent::remove(BubbledMsgEventHandler^ _d) {
	OnBubbledMsgEvent -= _d;
}

void Service::BubbledMsgEvent::raise(MsgGroup g, MsgType t, List<Object^>^ p) {
	if(OnBubbledMsgEvent)
		OnBubbledMsgEvent->Invoke(g, t, p);
}

void Service::BroadcastedMsgEvent::add(BubbledMsgEventHandler^ _d) {
	OnBroadcastedMsgEvent += _d;
}

void Service::BroadcastedMsgEvent::remove(BubbledMsgEventHandler^ _d) {
	OnBroadcastedMsgEvent -= _d;
}

void Service::BroadcastedMsgEvent::raise(MsgGroup g, MsgType t, List<Object^>^ p) {
	if(OnBroadcastedMsgEvent)
		OnBroadcastedMsgEvent->Invoke(g, t, p);
}

Boolean Service::FullyManageEditedStatus::get() {
	return fullyManageEditedStatus;
}

Void Service::FullyManageEditedStatus::set(Boolean value) {
	fullyManageEditedStatus = value;
}

Boolean Service::ManagedEdited::get() {
	return managedEdited;
}

Void Service::ManagedEdited::set(Boolean value) {
	managedEdited = value;
}

Service::Service() {
	fullyManageEditedStatus = false;
	managedEdited = false;
	mService = NULL;
	gService = this;
}

Service::~Service() {
}

Void Service::Open(String^ path) {
	_GEF_TRY
		if(mService) Close();

		StdStr p = WrapperUtil::ConvertString(path);
		mSvrModule = LoadLibraryA(p.c_str());
		mOpen = (OpenGefServiceFunc)GetProcAddress(mSvrModule, OPEN_GEF_SERVICE_FUNC_NAME);
		mClose = (CloseGefServiceFunc)GetProcAddress(mSvrModule, CLOSE_GEF_SERVICE_FUNC_NAME);
		mCreateCmd = (CreateGefCommandFunc)GetProcAddress(mSvrModule, CREATE_GEF_COMMAND_FUNC_NAME);
		mCreateCmdByNum = (CreateGefCommandByNumFunc)GetProcAddress(mSvrModule, CREATE_GEF_COMMAND_BY_NUM_FUNC_NAME);
		mCreateCmdByStr = (CreateGefCommandByStrFunc)GetProcAddress(mSvrModule, CREATE_GEF_COMMAND_BY_STR_FUNC_NAME);
		mDestroyCmd = (DestroyGefCommandFunc)GetProcAddress(mSvrModule, DESTROY_GEF_COMMAND_FUNC_NAME);
		GEF_ENSURE(!!mOpen, "Cannot get address of %s", OPEN_GEF_SERVICE_FUNC_NAME);
		GEF_ENSURE(!!mClose, "Cannot get address of %s", CLOSE_GEF_SERVICE_FUNC_NAME);
		GEF_ENSURE(!!mCreateCmd, "Cannot get address of %s", CREATE_GEF_COMMAND_FUNC_NAME);
		GEF_ENSURE(!!mCreateCmdByNum, "Cannot get address of %s", CREATE_GEF_COMMAND_BY_NUM_FUNC_NAME);
		GEF_ENSURE(!!mCreateCmdByStr, "Cannot get address of %s", CREATE_GEF_COMMAND_BY_STR_FUNC_NAME);
		GEF_ENSURE(!!mDestroyCmd, "Cannot get address of %s", DESTROY_GEF_COMMAND_FUNC_NAME);
		mService = (*mOpen)();
		mService->setMsgBubbler(_bubble);
		mService->setMsgBroadcaster(_broadcast);

		mCmdMgr = new CommandManager;
		mCmdMgr->setDestroyGefCommandFunc(mDestroyCmd);
	_GEF_CATCH
}

Void Service::Close() {
	_GEF_TRY
		if(!mService) return;

		delete mCmdMgr;

		(*mClose)(mService);
		FreeLibrary(mSvrModule);
		mService = NULL;
		gService = nullptr;
	_GEF_CATCH
}

Void Service::PostInitDone() {
	_GEF_TRY
		if(!mService) return;
		mService->postInitDone();
	_GEF_CATCH
}

Void Service::PrevDestroy() {
	_GEF_TRY
		if(!mService) return;
		mService->prevDestroy();
	_GEF_CATCH
}

Boolean Service::Sink(Object^ s, MsgGroup g, MsgType t, List<Object^>^ p) {
	_GEF_TRY
		if(!mService) return false;
		if(g == (MsgGroup)MsgGroupTypes::MGT_EDIT && t == (MsgType)MsgEditTypes::MET_EDIT_UNDO) {
			gService->mCmdMgr->undo();
		} else if(g == (MsgGroup)MsgGroupTypes::MGT_EDIT && t == (MsgType)MsgEditTypes::MET_EDIT_REDO) {
			gService->mCmdMgr->redo();
		} else {
			ParamList l;
			if(p != nullptr)
				WrapperUtil::ConvertParamList(p, l);
			mService->sink(g, t, l);
		}
	_GEF_CATCH

	return true;
}

Boolean Service::Bubble(MsgGroup g, MsgType t, const ParamList &p) {
	_GEF_TRY
		if(!p.empty()) {
			List<Object^>^ l = WrapperUtil::ConvertParamList(p);
			BubbledMsgEvent(g, t, l);
		} else {
			BubbledMsgEvent(g, t, nullptr);
		}
	_GEF_CATCH

	return true;
}

Boolean Service::Broadcast(MsgGroup g, MsgType t, const ParamList &p) {
	_GEF_TRY
		if(!p.empty()) {
			List<Object^>^ l = WrapperUtil::ConvertParamList(p);
			BroadcastedMsgEvent(g, t, l);
		} else {
			BroadcastedMsgEvent(g, t, nullptr);
		}
	_GEF_CATCH

	return true;
}

Boolean Service::GetEdited() {
	_GEF_TRY
		if(!mService) return false;
		if(FullyManageEditedStatus) return ManagedEdited;
		else return ManagedEdited ? true : mService->getEdited();
	_GEF_CATCH
}

UInt32 Service::GetUndoQueueSize() {
	assert(mCmdMgr);
	if(!mCmdMgr) return 0;
	return mCmdMgr->getUndoQueueSize();
}

Void Service::SetUndoQueueSize(UInt32 s) {
	assert(mCmdMgr);
	if(!mCmdMgr) return;
	mCmdMgr->setUndoQueueSize(s);
}

void Service::_Bubble(MsgGroup g, MsgType t, const ParamList &p) {
	_GEF_TRY
		if(!gService) return;

		if(g == (MsgGroup)MsgGroupTypes::MGT_EDIT && t == (MsgType)MsgEditTypes::MET_EDIT_ADD_CMD) {
			Ptr ptr = p.at(0).getPtr();
			ICommand* cmd = static_cast<ICommand*>(ptr);
			gService->mCmdMgr->add(cmd);
		} else if(g == (MsgGroup)MsgGroupTypes::MGT_FILE && t == (MsgType)MsgFileTypes::MFT_FILE_MARK_SAVED) {
			gService->ManagedEdited = false;
			gService->mCmdMgr->_setSavedPoint();
		} else {
			gService->Bubble(g, t, p);
		}
	_GEF_CATCH
}

void Service::_Broadcast(MsgGroup g, MsgType t, const ParamList &p) {
	_GEF_TRY
		if(!gService) return;
		gService->Broadcast(g, t, p);
	_GEF_CATCH
}

}
