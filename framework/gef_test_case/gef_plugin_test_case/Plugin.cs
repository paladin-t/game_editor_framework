/**
 * @version: 1.4
 * @author: Wang Renxin,
 *     https://github.com/paladin-t/game_editor_framework
 *     mailto:hellotony521@qq.com
 * @file: This file is a part of GEF, for copyright detail
 *     information, see the LICENSE file.
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Reflection;

namespace gef
{
    public class Plugin
    {
        private static uint id = 0;
        public static uint Id
        {
            get
            {
                if (id == 0)
                {
                    char[] _id = { 'T', 'E', 'S', 'T' };
                    for (int i = 0; i < _id.Length; i++) id |= (uint)(((byte)_id[i]) << ((3 - i) * 8));
                }
                return id;
            }
        }

        public static string Name
        {
            get { return "Plugin Test"; }
        }

        public static void Setup()
        {
            Bubble += (_g, _t, _p) => Plugin.MsgProc.Raise(_g, _t, _p);

            Control c = (Control)Host;
            float r = c.BackColor.R / 255.0f;
            float g = c.BackColor.G / 255.0f;
            float b = c.BackColor.B / 255.0f;
            List<object> p = new List<object>();
            p.Add(r);
            p.Add(g);
            p.Add(b);
            DoSink((uint)MsgGroupTypesTestCase.MGT_TEST, (uint)MsgTestTypes.MTT_TEST_SET_CLEAR_COLOR, p);

            DoEcho("Plugin test case setup.");
        }
        public static void Shutdown()
        {
        }

        private static List<Control> controls = null;
        public static List<Control> Controls
        {
            get
            {
                if (controls == null)
                {
                    controls = new List<Control>();
                    Control ctrl = null;

                    ctrl = new UserControlTest();
                    ctrl.Name = "Canvas Tool Panel Test";
                    ctrl.Tag = "left";
                    controls.Add(ctrl);

                    ctrl = new UserControlSideTest();
                    ctrl.Name = "Plugin Side Panel Test";
                    ctrl.Tag = "left";
                    controls.Add(ctrl);
                }

                return controls;
            }
        }
        private static List<Guid> guids = null;
        public static List<Guid> Guids
        {
            get
            {
                if (guids == null)
                {
                    guids = new List<Guid>();

                    guids.Add(new Guid("aa2bc812-df2c-44da-9790-d3465735f061"));
                    guids.Add(new Guid("9D16C7A9-CCD7-4b35-8330-E49AF61C078C"));
                }

                return guids;
            }
        }

        public static object Host = null; 

        public delegate void MsgProcDelegate(uint group, uint type, List<object> param);
        public static MethodInfo Sink = null;
        public static event MsgProcDelegate Bubble = null;
        internal static void DoSink(uint group, uint type, List<object> param)
        {
            object[] objs = { Id, group, type, param };
            Sink.Invoke(Host, objs);
        }
        public static void OnBubble(uint group, uint type, List<object> param)
        {
            if (Bubble != null) Bubble(group, type, param);
        }
        public static bool Preproc(object senter, uint group, uint type, List<object> param)
        {
            return false;
        }

        public static void GotFocus()
        {
        }
        public static void LostFocus()
        {
        }

        public static uint TabOrder
        {
            get { return (uint)PluginTabPageOrder.PO_USER + 1; }
        }

        public static MethodInfo Echo = null;
        internal static void DoEcho(params object[] p)
        {
            object[] objs = { p };
            Echo.Invoke(Host, objs);
        }

        public static MethodInfo GetGefCtrl = null;
        internal static Control DoGetGefCtrl(List<object> p)
        {
            object[] objs = { p };
            return (Control)GetGefCtrl.Invoke(Host, objs);
        }

        public static MethodInfo HasScriptVar = null;
        public static MethodInfo GetScriptVar = null;
        public static MethodInfo SetScriptVar = null;
        public static MethodInfo RemoveScriptVar = null;
        internal static bool DoHasScriptVar(string name)
        {
            object[] objs = { name };
            return (bool)HasScriptVar.Invoke(Host, objs);
        }
        internal static object DoGetScriptVar(string name)
        {
            object[] objs = { name };
            return GetScriptVar.Invoke(Host, objs);
        }
        internal static void DoSetScriptVar(string name, object val)
        {
            object[] objs = { name, val };
            SetScriptVar.Invoke(Host, objs);
        }
        internal static bool DoRemoveScriptVar(string name)
        {
            object[] objs = { name };
            return (bool)RemoveScriptVar.Invoke(Host, objs);
        }

        internal static MessageProcessor MsgProc = new MessageProcessor();
    }
}
