"""IronPython script plugin test
@version: $1.2$
@author: U{Wang Renxin<mailto:hellotony521@qq.com>}
@file: This file is a part of GEF, for copyright detail
    information, see the LICENSE file.
"""

shell.pushWorkDir(EXE_DIR)
from plugin import *
shell.popWorkDir()

def go():
	shell.msgBox("This is a message from IronPython script plugin");

if LOADING:
	plugin_name = 'Python Plugin Test'
	plugin_order = 0
else:
	go()
