/**
 * @version: 1.3
 * @author: Wang Renxin,
 *     https://github.com/paladin-t/game_editor_framework
 *     mailto:hellotony521@qq.com
 * @file: This file is a part of GEF, for copyright detail
 *     information, see the LICENSE file.
 */

#ifndef __GEF_DOTNET_H__
#define __GEF_DOTNET_H__

#include <windows.h>
#include "../gef_service_base/gef_service_base.h"

using namespace System;
using namespace System::Collections::Generic;

namespace gef {

class CommandManager : Final<CommandManager> {

private:
	typedef std::deque<ICommand*> CmdStack;

public:
	CommandManager();
	~CommandManager();

	u32 _getCurrId(void) const;
	void _setSavedPoint(void);
	u32 _getSavedPoint(void) const;

	void add(ICommand* cmd);
	void undo(void);
	void redo(void);

	bool canUndo(void) const;
	bool canRedo(void) const;

	u32 getUndoQueueSize(void) const;
	void setUndoQueueSize(u32 s);

	void setDestroyGefCommandFunc(DestroyGefCommandFunc f);

private:
	void clearDone(void);
	void clearUndone(void);

private:
	u32 mSavedPoint;
	DestroyGefCommandFunc mDestroyCmd;
	CmdStack mDone;
	CmdStack mUndone;
	u32 mUndoQueueSize;

};

}

namespace gef {

public ref class Service sealed {

public:
    delegate Void BubbledMsgEventHandler(MsgGroup g, MsgType t, List<Object^>^ p);

protected:
    BubbledMsgEventHandler^ OnBubbledMsgEvent;
	BubbledMsgEventHandler^ OnBroadcastedMsgEvent;

public:
    event BubbledMsgEventHandler^ BubbledMsgEvent {

	public:
		void add(BubbledMsgEventHandler^ _d);
		void remove(BubbledMsgEventHandler^ _d);

	private:
		void raise(MsgGroup g, MsgType t, List<Object^>^ p);

	}

	event BubbledMsgEventHandler^ BroadcastedMsgEvent {

	public:
		void add(BubbledMsgEventHandler^ _d);
		void remove(BubbledMsgEventHandler^ _d);

	private:
		void raise(MsgGroup g, MsgType t, List<Object^>^ p);

	}

private:
	Boolean fullyManageEditedStatus;

public:
	property Boolean FullyManageEditedStatus {
		Boolean get();
		Void set(Boolean value);
	}

private:
	Boolean managedEdited;

public:
	property Boolean ManagedEdited {
		Boolean get();
		Void set(Boolean value);
	}

public:
	Service();
	~Service();

	Void Open(String^ path);
	Void Close();

	Void PostInitDone();
	Void PrevDestroy();

	Boolean Sink(Object^ s, MsgGroup g, MsgType t, List<Object^>^ p);
    Boolean Bubble(MsgGroup g, MsgType t, const ParamList &p);
	Boolean Broadcast(MsgGroup g, MsgType t, const ParamList &p);

	Boolean GetEdited();

	UInt32 GetUndoQueueSize();
	Void SetUndoQueueSize(UInt32 s);

    static void _Bubble(MsgGroup g, MsgType t, const ParamList &p);
	static void _Broadcast(MsgGroup g, MsgType t, const ParamList &p);

private:
	IService* mService;
	CommandManager* mCmdMgr;
	HMODULE mSvrModule;
	OpenGefServiceFunc mOpen;
	CloseGefServiceFunc mClose;
	CreateGefCommandFunc mCreateCmd;
	CreateGefCommandByNumFunc mCreateCmdByNum;
	CreateGefCommandByStrFunc mCreateCmdByStr;
	DestroyGefCommandFunc mDestroyCmd;

};

}

#endif // __GEF_DOTNET_H__
