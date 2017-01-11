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
using System.Linq;
using System.Text;

namespace gef
{
    public class MsgContext
    {
        public ScriptManager.MsgDispType DispType { get; set; }
        public uint Group { get; set; }
        public uint Type { get; set; }
        public List<object> Params { get; set; }

        public MsgContext(ScriptManager.MsgDispType dt, uint g, uint t, List<object> p)
        {
            DispType = dt; Group = g; Type = t; Params = p;
        }

        public override string ToString()
        {
            string l = string.Empty;
            for (int i = 0; i < Params.Count; i++)
            {
                object o = Params[i];

                l += o.ToString();

                if (o.GetType() == typeof(uint))
                    l += "(HEX: 0x" + ((uint)o).ToString("X8") + ")";
                else if (o.GetType() == typeof(int))
                    l += "(HEX: 0x" + ((int)o).ToString("X8") + ")";
                else if (o.GetType() == typeof(UInt64))
                    l += "(HEX: 0x" + ((UInt64)o).ToString("X16") + ")";
                else if (o.GetType() == typeof(Int64))
                    l += "(HEX: 0x" + ((Int64)o).ToString("X16") + ")";

                if (i != Params.Count - 1) l += ", ";
            }
            string prefix = string.Empty;
            if (DispType == ScriptManager.MsgDispType.Sink) prefix = "SINK: ";
            else if (DispType == ScriptManager.MsgDispType.Bubble) prefix = "BUBBLE: ";
            else if (DispType == ScriptManager.MsgDispType.PluginBroadcast) prefix = "PLUGIN BROADCAST: ";

            return prefix + Group.ToString() + ", " + Type.ToString() + ", " + "[" + l + "]";
        }
    }
}
