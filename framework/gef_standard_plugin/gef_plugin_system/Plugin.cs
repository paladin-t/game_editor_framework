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
                    char[] _id = { 'S', 'Y', 'S', 'T' };
                    for (int i = 0; i < _id.Length; i++) id |= (uint)(((byte)_id[i]) << ((3 - i) * 8));
                }
                return id;
            }
        }

        public static string Name
        {
            get { return "System"; }
        }

        private static Timer timer = null;

        public static void Setup()
        {
            timer = new Timer();
            timer.Interval = 15;
            timer.Start();
            timer.Tick += (sender, e) =>
            {
                List<object> p = new List<object>();
                p.Add(15);
                DoSink((uint)MsgGroupTypes.MGT_SYSTEM, (uint)MsgSystemTypes.MST_SYS_TIMER, p);
            };

            ((Quartered)Controls[1]).LoadConfig();

            Bubble += (_group, _type, _param) =>
            {
                if (_group == (uint)MsgGroupTypes.MGT_SYSTEM && _type == (uint)MsgSystemTypes.MST_SYS_PAUSE_RENDER)
                    timer.Enabled = false;
                else if (_group == (uint)MsgGroupTypes.MGT_SYSTEM && _type == (uint)MsgSystemTypes.MST_SYS_RESUME_RENDER)
                    timer.Enabled = true;
            };
        }

        public static void Shutdown()
        {
            ((Quartered)Controls[1]).SaveConfig();

            timer.Stop();
            timer = null;
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

                    ctrl = new Canvas();
                    ctrl.Name = "Canvas";
                    ctrl.Tag = "fill";
                    controls.Add(ctrl);

                    ctrl = new Quartered();
                    ctrl.Name = "Quartered";
                    ctrl.Tag = "fill";
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

                    guids.Add(new Guid("55384d6a-e111-4d18-b5d1-400cc73be104"));
                    guids.Add(new Guid("A80F5587-3597-43ec-AFED-2E27D2EA266B"));
                }

                return guids;
            }
        }

        public static object Host = null;

        public delegate void MsgProcDelegate(uint group, uint type, List<object> param);

        public static MethodInfo Sink = null;
        public static event MsgProcDelegate Bubble = null;
        public static MethodInfo PluginBroadcast = null;
        public static MethodInfo PluginSend = null;
        internal static void DoSink(uint group, uint type, List<object> param)
        {
            object[] objs = { Id, group, type, param };
            Sink.Invoke(Host, objs);
        }
        public static void OnBubble(uint group, uint type, List<object> param)
        {
            if (Bubble != null) Bubble(group, type, param);
        }
        internal static void DoPluginBroadcast(Control ctrl, uint group, uint type, List<object> param)
        {
            if (PluginBroadcast == null) return;
            object[] objs = { ctrl, Id, group, type, param };
            PluginBroadcast.Invoke(Host, objs);
        }

        public static MethodInfo HasCfg = null;
        public static MethodInfo GetCfg = null;
        public static MethodInfo SetCfg = null;
        public static MethodInfo RemoveCfg = null;
        internal static bool DoHasCfg(string path)
        {
            object[] objs = { path };
            return (bool)HasCfg.Invoke(Host, objs);
        }
        internal static string DoGetCfg(string path)
        {
            object[] objs = { path };
            return (string)GetCfg.Invoke(Host, objs);
        }
        internal static void DoSetCfg(string path, string val)
        {
            object[] objs = { path, val };
            SetCfg.Invoke(Host, objs);
        }
        internal static bool DoRemoveCfg(string path)
        {
            object[] objs = { path };
            return (bool)RemoveCfg.Invoke(Host, objs);
        }
    }
}
