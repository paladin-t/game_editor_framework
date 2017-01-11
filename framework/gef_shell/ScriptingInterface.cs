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
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Drawing;
using System.ComponentModel;
using System.Reflection;
using Microsoft.Scripting.Hosting;
using IronPython.Hosting;

namespace gef
{
    partial class ScriptManager
    {
        #region Scripting Interfaces for IronPython

        [Description("Print help information")]
        public void help()
        {
            DebugPanel.GetInstance().WriteLine(Pens.Black.Color, "Supported commands:");
            MethodInfo[] mis = GetType().GetMethods();
            foreach (MethodInfo mi in mis)
            {
                Attribute attrib = Attribute.GetCustomAttribute(mi, typeof(DescriptionAttribute));
                if (attrib != null)
                {
                    string s = ((DescriptionAttribute)attrib).Description;
                    DebugPanel.GetInstance().WriteLine(Pens.Blue.Color, "   -Description: " + s);
                    DebugPanel.GetInstance().WriteLine(Pens.Black.Color, "    Prototype: shell." + mi.Name);
                }
            }
        }

        [Description("Reset current scripting context")]
        public void reset()
        {
            IEnumerable<string> names = Scope.GetVariableNames();
            foreach (string name in names)
            {
                if (name != "shell" && name != "service" && name != "PLUGIN_DIR" && name != "EXE_DIR" && name != "LOADING")
                    Scope.RemoveVariable(name);
            }
        }

        [Description("Quit editor")]
        public void quit()
        {
            FormMain.GetInstance().Close();
        }

        [Description("Get a control on the editor window")]
        public Control getCtrl(List<object> p)
        {
            return FormMain.GetInstance().GetGefControl(p);
        }

        [Description("List controls' name and relations")]
        public void echoControls()
        {
            int level = -1;
            Action<Control> walkCtrl = null;
            walkCtrl = new Action<Control>
            (
                (_ctrl) =>
                {
                    level++;
                    string l = string.Empty;
                    for (int i = 0; i < level; i++) l += "__";
                    if (string.IsNullOrEmpty(_ctrl.Name))
                        echo(l + "<anonymous> : " + _ctrl.GetType().ToString());
                    else
                        echo(l + "[" + _ctrl.Name + "] : " + _ctrl.GetType().ToString());
                    foreach (Control _sub in _ctrl.Controls) walkCtrl(_sub);
                    level--;
                }
            );

            echo("");
            echo("== Main Window ==");
            walkCtrl(FormMain.GetInstance());

            echo("");
            echo("== Dockable Panels ==");
            Dictionary<string, FormDockable>.Enumerator it = FormMain.GetInstance().Name2FormDict.GetEnumerator();
            while (it.MoveNext())
            {
                string fn = it.Current.Key;
                FormDockable f = it.Current.Value;
                walkCtrl(f);
            }
        }

        [Description("Close a panel")]
        public void closePanel(string name)
        {
            FormDockable f = FormMain.GetInstance().Name2FormDict[name];
            f.Close();
        }

        [Description("Show a message box")]
        public DialogResult msgBox(params object[] p)
        {
            string t = string.Empty;
            foreach (object o in p)
            {
                t += o.ToString();
            }
            return ShellUtil.MsgBox(t, MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        [Description("Set the enabled property of debug echo")]
        public void setEcho(bool on)
        {
            DebugPanel.GetInstance().EchoEnabled = on;
        }

        [Description("Echo some text")]
        public void echo(params object[] p)
        {
            DebugPanel.GetInstance().Echo(p);
        }

        [Description("Clear debug console")]
        public void clearConsole()
        {
            DebugPanel.GetInstance().Clear();
        }

        [Description("Get current working directory")]
        public string getWorkDir()
        {
            return ShellUtil.GetWorkingDirectory();
        }

        [Description("Push current working directory")]
        public void pushWorkDir()
        {
            ShellUtil.PushWorkingDirectory();
        }

        [Description("Push current working directory and set a new one by argument")]
        public void pushWorkDir(string n)
        {
            ShellUtil.PushWorkingDirectory(n);
        }

        [Description("Pop current working directory")]
        public string popWorkDir()
        {
            return ShellUtil.PopWorkingDirectory();
        }

        [Description("Get max size of the message queue")]
        public int getMsgQueueMaxSize()
        {
            echo("Message queue max size: ", MsgQueueMaxSize);
            return MsgQueueMaxSize;
        }

        [Description("Set max size of the message queue")]
        public void setMsgQueueMaxSize(int size)
        {
            MsgQueueMaxSize = size;
            echo("Message queue max size set to: ", MsgQueueMaxSize);
        }

        [Description("Get size of the message queue")]
        public int getMsgQueueSize()
        {
            echo("Message queue size: ", msgQueue.Count);
            return msgQueue.Count;
        }

        [Description("Sink a message")]
        public void sink(uint g, uint t, List<object> p)
        {
            FormMain.GetInstance().Sink(this, g, t, p);
        }

        [Description("Bubble a message")]
        public void bubble(uint g, uint t, List<object> p)
        {
            FormMain.GetInstance().OnBubble(g, t, p);
        }

        [Description("Broadcast a message")]
        public void broadcast(uint g, uint t, List<object> p)
        {
            FormMain.GetInstance().PluginBroadcast(null, null, g, t, p);
        }

        [Description("Get a message")]
        public MsgContext getMsg(int index)
        {
            MsgContext msg = msgQueue[index];
            echo("Message at (", index, "): ", msg);
            return msg;
        }

        #endregion
    }
}
