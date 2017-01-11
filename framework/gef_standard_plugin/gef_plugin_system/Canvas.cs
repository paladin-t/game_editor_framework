/**
 * @version: 1.2
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
    public partial class Canvas : UserControl
    {
        public Canvas()
        {
            InitializeComponent();

            MouseWheel += new MouseEventHandler(Canvas_MouseWheel);
        }

        private void Canvas_SizeChanged(object sender, EventArgs e)
        {
            if (Plugin.Sink == null) return;

            string full = Name;
            string part = Name;
            Control c = Parent;
            while (c != null && c.Parent != null)
            {
                full = c.Name + "." + full;
                c = c.Parent;
            }

            List<object> p = new List<object>();
            p.Add(full);
            p.Add(part);
            p.Add(Width);
            p.Add(Height);
            Plugin.DoSink((uint)MsgGroupTypes.MGT_SYSTEM, (uint)MsgSystemTypes.MST_SYS_RESIZE, p);
        }

        private void Canvas_MouseDown(object sender, MouseEventArgs e)
        {
            List<object> objs = new List<object>();
            objs.Add(e.X);
            objs.Add(e.Y);
            objs.Add((uint)e.Button);
            Plugin.DoPluginBroadcast(this, (uint)MsgGroupTypes.MGT_INPUT_MOUSE, (uint)MsgInputMouseTypes.MIM_MOUSE_DOWN, objs);
        }

        private void Canvas_MouseUp(object sender, MouseEventArgs e)
        {
            List<object> objs = new List<object>();
            objs.Add(e.X);
            objs.Add(e.Y);
            objs.Add((uint)e.Button);
            Plugin.DoPluginBroadcast(this, (uint)MsgGroupTypes.MGT_INPUT_MOUSE, (uint)MsgInputMouseTypes.MIM_MOUSE_UP, objs);
        }

        private void Canvas_MouseClick(object sender, MouseEventArgs e)
        {
            List<object> objs = new List<object>();
            objs.Add(e.X);
            objs.Add(e.Y);
            objs.Add((uint)e.Button);
            Plugin.DoPluginBroadcast(this, (uint)MsgGroupTypes.MGT_INPUT_MOUSE, (uint)MsgInputMouseTypes.MIM_MOUSE_CLICKED, objs);
        }

        private void Canvas_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            List<object> objs = new List<object>();
            objs.Add(e.X);
            objs.Add(e.Y);
            objs.Add((uint)e.Button);
            Plugin.DoPluginBroadcast(this, (uint)MsgGroupTypes.MGT_INPUT_MOUSE, (uint)MsgInputMouseTypes.MIM_MOUSE_DOUBLE_CLICKED, objs);
        }

        private void Canvas_MouseMove(object sender, MouseEventArgs e)
        {
            Focus();

            List<object> objs = new List<object>();
            objs.Add(e.X);
            objs.Add(e.Y);
            objs.Add((uint)e.Button);
            Plugin.DoPluginBroadcast(this, (uint)MsgGroupTypes.MGT_INPUT_MOUSE, (uint)MsgInputMouseTypes.MIM_MOUSE_MOVE, objs);
        }

        private void Canvas_MouseWheel(object sender, MouseEventArgs e)
        {
            List<object> objs = new List<object>();
            objs.Add(e.X);
            objs.Add(e.Y);
            objs.Add(e.Delta);
            Plugin.DoPluginBroadcast(this, (uint)MsgGroupTypes.MGT_INPUT_MOUSE, (uint)MsgInputMouseTypes.MIM_MOUSE_WHEEL, objs);
        }

        private void Canvas_KeyDown(object sender, KeyEventArgs e)
        {
            List<object> objs = new List<object>();
            objs.Add(e.Alt);
            objs.Add(e.Control);
            objs.Add(e.Shift);
            objs.Add(e.KeyValue);
            Plugin.DoPluginBroadcast(this, (uint)MsgGroupTypes.MGT_INPUT_KEYBOARD, (uint)MsgInputKeyboardTypes.MIK_KEY_DOWN, objs);
        }

        private void Canvas_KeyUp(object sender, KeyEventArgs e)
        {
            List<object> objs = new List<object>();
            objs.Add(e.Alt);
            objs.Add(e.Control);
            objs.Add(e.Shift);
            objs.Add(e.KeyValue);
            Plugin.DoPluginBroadcast(this, (uint)MsgGroupTypes.MGT_INPUT_KEYBOARD, (uint)MsgInputKeyboardTypes.MIK_KEY_UP, objs);
        }

        private void Canvas_KeyPress(object sender, KeyPressEventArgs e)
        {
            List<object> objs = new List<object>();
            objs.Add(e.KeyChar);
            Plugin.DoPluginBroadcast(this, (uint)MsgGroupTypes.MGT_INPUT_KEYBOARD, (uint)MsgInputKeyboardTypes.MIK_KEY_PRESSED, objs);
        }
    }
}
