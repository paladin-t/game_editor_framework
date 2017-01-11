/**
 * @version: 1.3
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

            Plugin.Bubble += (_g, _t, _p) => Plugin.MsgProc.Raise(_g, _t, _p);

            Plugin.MsgProc.RegMsgProc((uint)MsgGroupTypes.MGT_SCRIPT, (uint)MsgScriptTypes.MST_SCRIPT_RES_LIST, new MessageProcessor.MsgEventHandler(OnScriptResList));
        }

        private void OnScriptResList(uint group, uint type, List<object> param)
        {
            param.Sort
            (
                (_x, _y) =>
                {
                    KeyValuePair<string, int> _px = (KeyValuePair<string, int>)_x;
                    KeyValuePair<string, int> _py = (KeyValuePair<string, int>)_y;
                    return _px.Value - _py.Value;
                }
            );
            foreach (object obj in param)
            {
                KeyValuePair<string, int> pair = (KeyValuePair<string, int>)obj;
                string pn = pair.Key;
                int order = pair.Value;
                ToolStripButton btn = new ToolStripButton(pn);
                btn.DisplayStyle = ToolStripItemDisplayStyle.Text;
                btn.Tag = pair;
                PluginToolStrip.Items.Add(btn);
                btn.Click += (_sender, _e) =>
                {
                    ToolStripButton _b = (ToolStripButton)_sender;
                    KeyValuePair<string, int> _pair = (KeyValuePair<string, int>)_b.Tag;
                    string _pn = _pair.Key;
                    List<object> _p = new List<object>();
                    _p.Add(_pn);
                    Plugin.DoSink((uint)MsgGroupTypes.MGT_SCRIPT, (uint)MsgScriptTypes.MST_SCRIPT_RUN, _p);
                };
            }
        }
    }
}
