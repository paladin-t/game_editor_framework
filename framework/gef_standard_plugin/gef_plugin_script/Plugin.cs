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
                    char[] _id = { 'S', 'C', 'R', 'P' };
                    for (int i = 0; i < _id.Length; i++) id |= (uint)(((byte)_id[i]) << ((3 - i) * 8));
                }
                return id;
            }
        }

        public static string Name
        {
            get { return "Script"; }
        }

        public static void Setup()
        {
            DoSink((uint)MsgGroupTypes.MGT_SCRIPT, (uint)MsgScriptTypes.MST_SCRIPT_REQ_LIST, null);
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
                    controls = new List<Control>();

                return controls;
            }
        }

        private static List<Guid> guids = null;
        public static List<Guid> Guids
        {
            get
            {
                if (guids == null)
                    guids = new List<Guid>();

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

        public static uint TabOrder
        {
            get { return (uint)PluginTabPageOrder.PO_SCRIPT; }
        }

        internal static MessageProcessor MsgProc = new MessageProcessor();
    }
}
