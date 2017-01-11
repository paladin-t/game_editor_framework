/**
 * @version: 1.5
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
                    char[] _id = { 'F', 'I', 'L', 'E' };
                    for (int i = 0; i < _id.Length; i++) id |= (uint)(((byte)_id[i]) << ((3 - i) * 8));
                }
                return id;
            }
        }

        public static string Name
        {
            get { return "File"; }
        }

        private static List<Control> controls = null;
        public static List<Control> Controls
        {
            get
            {
                if (controls == null)
                {
                    controls = new List<Control>();
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
            get { return (uint)PluginTabPageOrder.PO_FILE; }
        }

        public static MethodInfo MsgBox = null;

        internal static DialogResult DoMsgBox(string text, MessageBoxButtons btn, MessageBoxIcon icon)
        {
            object[] objs = { text, btn, icon };
            return (DialogResult)MsgBox.Invoke(null, objs);
        }

        public static MethodInfo GetEdited = null;
        internal static bool DoGetEdited()
        {
            return (bool)GetEdited.Invoke(Host, null);
        }
        internal static bool EnsureSaved()
        {
            if (DoGetEdited())
            {
                DialogResult dr = DoMsgBox("Save changes?", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);
                if (dr == DialogResult.Yes)
                {
                    DoSink((uint)MsgGroupTypes.MGT_FILE, (uint)MsgFileTypes.MFT_FILE_SAVE, null);
                }
                else if (dr == DialogResult.No)
                {
                    // do nothing
                }
                else if (dr == DialogResult.Cancel)
                {
                    return true;
                }
            }

            return false;
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
