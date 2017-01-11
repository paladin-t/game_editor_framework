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
    public partial class Quartered : UserControl
    {
        public Quartered()
        {
            InitializeComponent();

            Panel1.Tag = 1;
            Panel2.Tag = 2;
            Panel3.Tag = 3;
            Panel4.Tag = 4;

            Panel1.MouseWheel += new MouseEventHandler(Panel_MouseWheel);
            Panel2.MouseWheel += new MouseEventHandler(Panel_MouseWheel);
            Panel3.MouseWheel += new MouseEventHandler(Panel_MouseWheel);
            Panel4.MouseWheel += new MouseEventHandler(Panel_MouseWheel);

            Panel1.KeyDown += new KeyEventHandler(Panel_KeyDown);
            Panel2.KeyDown += new KeyEventHandler(Panel_KeyDown);
            Panel3.KeyDown += new KeyEventHandler(Panel_KeyDown);
            Panel4.KeyDown += new KeyEventHandler(Panel_KeyDown);
            Panel1.KeyUp += new KeyEventHandler(Panel_KeyUp);
            Panel2.KeyUp += new KeyEventHandler(Panel_KeyUp);
            Panel3.KeyUp += new KeyEventHandler(Panel_KeyUp);
            Panel4.KeyUp += new KeyEventHandler(Panel_KeyUp);
            Panel1.KeyPress += new KeyPressEventHandler(Panel_KeyPress);
            Panel2.KeyPress += new KeyPressEventHandler(Panel_KeyPress);
            Panel3.KeyPress += new KeyPressEventHandler(Panel_KeyPress);
            Panel4.KeyPress += new KeyPressEventHandler(Panel_KeyPress);

            Quaters.ColumnStyles[0].SizeType = SizeType.Percent;
            Quaters.RowStyles[0].SizeType = SizeType.Percent;
        }

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        private void Quartered_Resize(object sender, EventArgs e)
        {
            trackBarBottom.Maximum = Width;
            trackBarRight.Maximum = Height;
            trackBarBottom.Value = (int)(Quaters.ColumnStyles[0].Width / 100.0f * Width);
            trackBarRight.Value = (int)(Quaters.RowStyles[1].Height / 100.0f * Height);
        }

        private void trackBarBottom_Scroll(object sender, EventArgs e)
        {
            Quaters.ColumnStyles[0].Width = (trackBarBottom.Value / (float)trackBarBottom.Maximum) * 100.0f;
            Quaters.ColumnStyles[1].Width = 100.0f - Quaters.ColumnStyles[0].Width;
        }

        private void trackBarRight_Scroll(object sender, EventArgs e)
        {
            Quaters.RowStyles[1].Height = (trackBarRight.Value / (float)trackBarRight.Maximum) * 100.0f;
            Quaters.RowStyles[0].Height = 100.0f - Quaters.RowStyles[1].Height;
        }

        private void Panel_SizeChanged(object sender, EventArgs e)
        {
            if (Plugin.Sink == null) return;

            GefDrawingPanel pnl = ((GefDrawingPanel)sender);
            string full = "Quartered.Quaters." + pnl.Name;
            string part = pnl.Name;
            
            List<object> p = new List<object>();
            p.Add(full);
            p.Add(part);
            p.Add(pnl.Width);
            p.Add(pnl.Height);
            p.Add((int)((Control)sender).Tag);
            Plugin.DoSink((uint)MsgGroupTypes.MGT_SYSTEM, (uint)MsgSystemTypes.MST_SYS_RESIZE, p);
        }

        private void Panel_MouseDown(object sender, MouseEventArgs e)
        {
            List<object> objs = new List<object>();
            objs.Add(e.X);
            objs.Add(e.Y);
            objs.Add((uint)e.Button);
            objs.Add((int)((Control)sender).Tag);
            Plugin.DoPluginBroadcast(this, (uint)MsgGroupTypes.MGT_INPUT_MOUSE, (uint)MsgInputMouseTypes.MIM_MOUSE_DOWN, objs);
        }

        private void Panel_MouseUp(object sender, MouseEventArgs e)
        {
            List<object> objs = new List<object>();
            objs.Add(e.X);
            objs.Add(e.Y);
            objs.Add((uint)e.Button);
            objs.Add((int)((Control)sender).Tag);
            Plugin.DoPluginBroadcast(this, (uint)MsgGroupTypes.MGT_INPUT_MOUSE, (uint)MsgInputMouseTypes.MIM_MOUSE_UP, objs);
        }

        private void Panel_MouseClick(object sender, MouseEventArgs e)
        {
            List<object> objs = new List<object>();
            objs.Add(e.X);
            objs.Add(e.Y);
            objs.Add((uint)e.Button);
            objs.Add((int)((Control)sender).Tag);
            Plugin.DoPluginBroadcast(this, (uint)MsgGroupTypes.MGT_INPUT_MOUSE, (uint)MsgInputMouseTypes.MIM_MOUSE_CLICKED, objs);
        }

        private void Panel_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            List<object> objs = new List<object>();
            objs.Add(e.X);
            objs.Add(e.Y);
            objs.Add((uint)e.Button);
            objs.Add((int)((Control)sender).Tag);
            Plugin.DoPluginBroadcast(this, (uint)MsgGroupTypes.MGT_INPUT_MOUSE, (uint)MsgInputMouseTypes.MIM_MOUSE_DOUBLE_CLICKED, objs);
        }

        private void Panel_MouseMove(object sender, MouseEventArgs e)
        {
            ((Control)sender).Focus();

            List<object> objs = new List<object>();
            objs.Add(e.X);
            objs.Add(e.Y);
            objs.Add((uint)e.Button);
            objs.Add((int)((Control)sender).Tag);
            Plugin.DoPluginBroadcast(this, (uint)MsgGroupTypes.MGT_INPUT_MOUSE, (uint)MsgInputMouseTypes.MIM_MOUSE_MOVE, objs);
        }

        private void Panel_MouseWheel(object sender, MouseEventArgs e)
        {
            List<object> objs = new List<object>();
            objs.Add(e.X);
            objs.Add(e.Y);
            objs.Add(e.Delta);
            objs.Add((int)((Control)sender).Tag);
            Plugin.DoPluginBroadcast(this, (uint)MsgGroupTypes.MGT_INPUT_MOUSE, (uint)MsgInputMouseTypes.MIM_MOUSE_WHEEL, objs);
        }

        private void Panel_KeyDown(object sender, KeyEventArgs e)
        {
            List<object> objs = new List<object>();
            objs.Add(e.Alt);
            objs.Add(e.Control);
            objs.Add(e.Shift);
            objs.Add(e.KeyValue);
            objs.Add((int)((Control)sender).Tag);
            Plugin.DoPluginBroadcast(this, (uint)MsgGroupTypes.MGT_INPUT_KEYBOARD, (uint)MsgInputKeyboardTypes.MIK_KEY_DOWN, objs);
        }

        private void Panel_KeyUp(object sender, KeyEventArgs e)
        {
            List<object> objs = new List<object>();
            objs.Add(e.Alt);
            objs.Add(e.Control);
            objs.Add(e.Shift);
            objs.Add(e.KeyValue);
            objs.Add((int)((Control)sender).Tag);
            Plugin.DoPluginBroadcast(this, (uint)MsgGroupTypes.MGT_INPUT_KEYBOARD, (uint)MsgInputKeyboardTypes.MIK_KEY_UP, objs);
        }

        private void Panel_KeyPress(object sender, KeyPressEventArgs e)
        {
            List<object> objs = new List<object>();
            objs.Add(e.KeyChar);
            objs.Add((int)((Control)sender).Tag);
            Plugin.DoPluginBroadcast(this, (uint)MsgGroupTypes.MGT_INPUT_KEYBOARD, (uint)MsgInputKeyboardTypes.MIK_KEY_PRESSED, objs);
        }

        public void LoadConfig()
        {
            string v = Plugin.DoGetCfg("plugin.std.system.quartered.v");
            string h = Plugin.DoGetCfg("plugin.std.system.quartered.h");
            if (v != null && h != null)
            {
                float _v = float.Parse(v);
                float _h = float.Parse(h);
                trackBarBottom.Value = (int)(trackBarBottom.Maximum * _v);
                trackBarRight.Value = (int)(trackBarRight.Maximum * _h);
                trackBarBottom_Scroll(null, null);
                trackBarRight_Scroll(null, null);
            }
        }

        public void SaveConfig()
        {
            float v = trackBarBottom.Value / (float)trackBarBottom.Maximum;
            float h = trackBarRight.Value / (float)trackBarRight.Maximum;
            Plugin.DoSetCfg("plugin.std.system.quartered.v", v.ToString());
            Plugin.DoSetCfg("plugin.std.system.quartered.h", h.ToString());
        }
    }
}
