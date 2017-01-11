"""IronPython script plugin test
@version: $1.0$
@author: U{Wang Renxin<mailto:hellotony521@qq.com>}
@file: This file is a part of GEF, for copyright detail
    information, see the LICENSE file.
"""

shell.pushWorkDir(EXE_DIR)
from plugin import *
shell.popWorkDir()

def go():
	p = List[object](['*.*|*.*'])
	shell.broadcast(UInt32(MsgGroupTypes.MGT_FILE), UInt32(MsgFileTypes.MFT_FILE_REQ_OPEN), p)

if LOADING:
	plugin_name = 'Open Dlg Test'
	plugin_order = 10
else:
	go()
