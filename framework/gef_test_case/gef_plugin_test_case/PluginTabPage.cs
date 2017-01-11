/**
 * @version: 1.0
 * @author: Wang Renxin,
 *     https://github.com/paladin-t/game_editor_framework
 *     mailto:hellotony521@qq.com
 * @file: This file is a part of GEF, for copyright detail
 *     information, see the LICENSE file.
 */

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace gef
{
    public partial class PluginTabPage : UserControl
    {
        public PluginTabPage()
        {
            InitializeComponent();
        }

        private void btnMsgTest_Click(object sender, EventArgs e)
        {
            List<object> p = new List<object>();
            p.Add(((Form)Plugin.Host).Handle);
            p.Add("hello");
            Plugin.DoSink((uint)MsgGroupTypesTestCase.MGT_TEST, (uint)MsgTestTypes.MTT_TEST_COMMON, p);
        }
    }
}
