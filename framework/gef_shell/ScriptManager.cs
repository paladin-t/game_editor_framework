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
using System.IO;
using Microsoft.Scripting.Hosting;
using IronPython.Hosting;
using System.Reflection;
using System.ComponentModel;

namespace gef
{
    public partial class ScriptManager
    {
        public enum MsgDispType
        {
            Sink,
            Bubble,
            PluginBroadcast
        }

        private List<MsgContext> msgQueue = new List<MsgContext>();

        internal int MsgQueueMaxSize { get; set; }

        private ScriptEngine engine = null;
        public ScriptEngine Engine
        {
            get { return engine; }
        }

        private ScriptScope scope = null;
        public ScriptScope Scope
        {
            get { return scope; }
        }

        private static ScriptManager self = null;

        internal ScriptManager()
        {
            MsgQueueMaxSize = 64;
        }

        internal static ScriptManager GetInstance()
        {
            if (self == null)
                self = new ScriptManager();

            return self;
        }

        internal void Open()
        {
            engine = Python.CreateEngine();
            scope = engine.CreateScope();
        }

        internal void Close()
        {
            self = null;
        }

        internal void DoFile(string path)
        {
            try
            {
                ScriptSource source = engine.CreateScriptSourceFromFile(path);
                source.Execute(scope);
            }
            catch (Exception ex)
            {
                string msg = "Script execution failed.\nFile: " + path + "\n" + ShellUtil.GetExceptionMsg(ex);
                ShellUtil.MsgBox(msg, MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        internal void DoString(string code)
        {
            try
            {
                ScriptSource source = engine.CreateScriptSourceFromString(code);
                source.Execute(scope);
            }
            catch (Exception ex)
            {
                string msg = "Script execution failed.\nSource: " + code + "\n" + ShellUtil.GetExceptionMsg(ex);
                ShellUtil.MsgBox(msg, MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        internal void AppendMsg(MsgDispType dt, uint g, uint t, List<object> p)
        {
            if (g == (uint)MsgGroupTypes.MGT_SYSTEM && t == (uint)MsgSystemTypes.MST_SYS_TIMER) return;
            msgQueue.Add(new MsgContext(dt, g, t, p));
            if (msgQueue.Count > MsgQueueMaxSize) msgQueue.RemoveAt(0);
        }

        public List<string> GetInterfaces()
        {
            List<string> inte = new List<string>();
            MethodInfo[] mis = GetType().GetMethods();
            foreach (MethodInfo mi in mis)
            {
                Attribute attrib = Attribute.GetCustomAttribute(mi, typeof(DescriptionAttribute));
                if (attrib != null)
                {
                    inte.Add("shell." + mi.Name);
                }
            }

            return inte;
        }
    }
}
