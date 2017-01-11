/**
 * @version: 1.1
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

            Plugin.Bubble += new Plugin.MsgProcDelegate(Plugin_Bubble);
        }

        private void Plugin_Bubble(uint group, uint type, List<object> param)
        {
        }

        private void btnAbout_Click(object sender, EventArgs e)
        {
            Plugin.DoSink((uint)MsgGroupTypes.MGT_HELP, (uint)MsgHelpTypes.MHT_HELP_SHOW_ABOUT, null);
        }

        private void btnHelp_Click(object sender, EventArgs e)
        {
            Plugin.DoSink((uint)MsgGroupTypes.MGT_HELP, (uint)MsgHelpTypes.MHT_HELP_SHOW_HELP, null);
        }
    }
}
