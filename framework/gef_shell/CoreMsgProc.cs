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

namespace gef
{
    partial class FormMain
    {
        #region Sink

        private void OnReqScriptList(object s, uint g, uint t, List<object> p)
        {
            try
            {
                List<object> par = new List<object>();
                DirectoryInfo di = new DirectoryInfo(WrapperUtil.ApplicationDirectory + "\\plugin");
                FileInfo[] files = di.GetFiles("*.py");
                ScriptManager.GetInstance().Scope.SetVariable("LOADING", true);
                foreach (FileInfo fi in files)
                {
                    if (fi.Name == "__init__.py") continue;
                    FormSplash.GetInstance().Status = "Loading IronPython plugin: " + fi.Name;
                    ScriptManager.GetInstance().Scope.SetVariable("plugin_name", string.Empty);
                    ScriptManager.GetInstance().Scope.SetVariable("plugin_order", (int)100);
                    ScriptManager.GetInstance().DoFile(fi.FullName);
                    string pluginName = (string)ScriptManager.GetInstance().Scope.GetVariable("plugin_name");
                    int pluginOrder = (int)ScriptManager.GetInstance().Scope.GetVariable("plugin_order");
                    par.Add(new KeyValuePair<string, int>(pluginName, pluginOrder));
                    PluginManager.GetInstance().ScriptPlugins[pluginName] = fi.FullName;
                }
                ScriptManager.GetInstance().Scope.SetVariable("LOADING", false);
                if (files.Length != 0)
                    ScriptManager.GetInstance().Scope.RemoveVariable("plugin_name");
                PluginManager.PluginInfo pi = PluginManager.GetInstance().Plugins["Script"];
                if (pi != null && pi.OnBubble != null)
                {
                    object[] objs = { (uint)MsgGroupTypes.MGT_SCRIPT, (uint)MsgScriptTypes.MST_SCRIPT_RES_LIST, par };
                    pi.OnBubble.Invoke(null, objs);
                }
            }
            catch (Exception ex)
            {
                string msg = "Plugin loading failed.\n" + ShellUtil.GetExceptionMsg(ex);
                ShellUtil.MsgBox(msg, MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        private void OnRunScript(object s, uint g, uint t, List<object> p)
        {
            try
            {
                string f = (string)p[0];
                f = PluginManager.GetInstance().ScriptPlugins[f];
                ScriptManager.GetInstance().DoFile(f);
            }
            catch (Exception ex)
            {
                string msg = "Plugin execution failed.\n" + ShellUtil.GetExceptionMsg(ex);
                ShellUtil.MsgBox(msg, MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        private void OnShowAbout(object s, uint g, uint t, List<object> p)
        {
            AboutBox ab = new AboutBox();
            ab.ShowDialog(this);
        }

        private void OnShowHelp(object s, uint g, uint t, List<object> p)
        {
            FormHelp.GetInstance().Show(this);
        }

        private void DoSink(object s, uint g, uint t, List<object> p)
        {
#if !DEBUG
            try
#endif
            {
                service.Sink(s, g, t, p);
            }
#if !DEBUG
            catch (Exception ex)
            {
                string msg = "Service exception.\n" + ShellUtil.GetExceptionMsg(ex);
                ShellUtil.MsgBox(msg, MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
#endif
        }

        #endregion

        #region Bubble
        
        public Control GetGefControl(List<object> p)
        {
            string first = (string)p[0];
            if (first == "FormMain")
            {
                Control c = this;
                for (int i = 1; i < p.Count; i++)
                {
                    object cn = p[i];
                    string scn = (string)cn;
                    c = c.Controls[scn];
                }

                return c;
            }
            else
            {
                string ctrl = first;
                if (name2formDict.ContainsKey(ctrl))
                {
                    Control c = name2formDict[ctrl];
                    foreach (object cn in p)
                    {
                        string scn = (string)cn;
                        c = c.Controls[scn];
                    }

                    return c;
                }
            }

            return null;
        }

        private void OnReqHwnd(uint g, uint t, List<object> p)
        {
            Control c = GetGefControl(p);
            if (c != null)
            {
                string full = string.Empty;
                string part = string.Empty;
                for (int i = 0; i < p.Count; i++)
                {
                    full += (string)p[i];
                    if (i != p.Count - 1)
                        full += ".";
                    else
                        part = (string)p[i];
                }
                List<object> par = new List<object>();
                par.Add(full);
                par.Add(part);
                par.Add(ShellUtil.GetPtr(c));
                par.Add(c.Width);
                par.Add(c.Height);
                Sink(this, (uint)MsgGroupTypes.MGT_SYSTEM, (uint)MsgSystemTypes.MST_SYS_RES_HWND, par);
            }
        }

        private void OnRefreshCtrl(uint g, uint t, List<object> p)
        {
            Control c = GetGefControl(p);
            if (c != null) c.Refresh();
        }

        private void OnActiveTab(uint g, uint t, List<object> p)
        {
            string n = (string)p[0];
            TabPage tp = tabControlMain.TabPages[n];
            tp.Select();
        }

        private void OnHasUndoOrRedo(uint g, uint t, List<object> p)
        {
            //PluginManager.PluginInfo pi = PluginManager.GetInstance().Plugins["Edit"];
            //if (pi != null && pi.OnBubble != null)
            //{
            //    object[] objs = { g, t, p };
            //    pi.OnBubble.Invoke(null, objs);
            //}
        }

        private void OnEcho(uint g, uint t, List<object> p)
        {
            ScriptManager.GetInstance().echo(p.ToArray());
        }

        #endregion

        #region Common
       
        public void OnBubble(uint g, uint t, List<object> p)
        {
            PluginManager.PluginInfo currPlugin = GetCurrentPlugin();
            if (currPlugin != null && currPlugin.OnBubble != null)
            {
                object[] objs = { g, t, p };
                currPlugin.OnBubble.Invoke(null, objs);
            }
        }

        #endregion
    }
}
