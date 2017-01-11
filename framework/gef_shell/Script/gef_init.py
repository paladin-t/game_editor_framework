"""GEF configuration script file
This script will be executed at the right begining before
other things initialized, even the service.
@version: $1.2$
@author: U{Wang Renxin<mailto:hellotony521@qq.com>}
@file: This file is a part of GEF, for copyright detail
    information, see the LICENSE file.
"""

import clr

from System import *
from System.Collections.Generic import *

editor_title = 'GEF Test'
customer_copyright = 'Author: Wang Renxin'
service_path = 'gef_service_test_case.dll'
plugin_folder = 'plugin'
plugin_files = List[str]([])
