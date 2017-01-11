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
using System.IO;
using System.Reflection;
using Microsoft.CSharp;
using System.CodeDom.Compiler;
using System.Windows.Forms;

namespace gef
{
    internal class PluginManager
    {
        private static uint sysPluginId = 0;
        public static uint SysPluginId
        {
            get
            {
                if (sysPluginId == 0)
                {
                    char[] _id = { 'S', 'Y', 'S', 'T' };
                    for (int i = 0; i < _id.Length; i++) sysPluginId |= (uint)(((byte)_id[i]) << ((3 - i) * 8));
                }
                return sysPluginId;
            }
        }

        public class PluginInfo
        {
            public uint Id = 0;
            public MethodInfo Setup = null;
            public MethodInfo Shutdown = null;
            public List<Control> Controls = null;
            public List<Guid> Guids = null;
            public FieldInfo Host = null;
            public FieldInfo Sink = null;
            public MethodInfo OnBubble = null;
            public FieldInfo PluginBroadcast = null;
            public FieldInfo PluginSend = null;
            public MethodInfo Preproc = null;
            public MethodInfo GotFocus = null;
            public MethodInfo LostFocus = null;
            public Control TabPage = null;
            public uint TabOrder = 0;
            public FieldInfo MsgBox = null;
            public FieldInfo Echo = null;
            public FieldInfo GetGefCtrl = null;
            public FieldInfo GetEdited = null;
            public FieldInfo HasCfg = null;
            public FieldInfo GetCfg = null;
            public FieldInfo SetCfg = null;
            public FieldInfo RemoveCfg = null;
            public FieldInfo HasScriptVar = null;
            public FieldInfo GetScriptVar = null;
            public FieldInfo SetScriptVar = null;
            public FieldInfo RemoveScriptVar = null;
        }

        private Dictionary<string, PluginInfo> plugins = null;
        public Dictionary<string, PluginInfo> Plugins
        {
            get { return plugins; }
        }

        private Dictionary<Control, PluginInfo> controlHolders = null;
        public Dictionary<Control, PluginInfo> ControlHolders
        {
            get { return controlHolders; }
        }

        private Dictionary<string, string> scriptPlugins = null;
        public Dictionary<string, string> ScriptPlugins
        {
            get { return scriptPlugins; }
        }

        public delegate void PluginLoadHandler(string fileName);
        public event PluginLoadHandler PluginLoading;
        public event PluginLoadHandler PluginLoaded;

        private static PluginManager self = null;

        private PluginManager()
        {
            plugins = new Dictionary<string, PluginInfo>();
            controlHolders = new Dictionary<Control, PluginInfo>();
            scriptPlugins = new Dictionary<string, string>();
        }

        public static PluginManager GetInstance()
        {
            if (self == null)
                self = new PluginManager();

            return self;
        }

        public void Open()
        {
        }

        public void Close()
        {
            self = null;
        }

        public void LoadPlugins(string directory)
        {
            Dictionary<string, string> excludes = new Dictionary<string, string>();
            string exFile = directory + "\\__exclude__.inf";
            if (File.Exists(exFile))
            {
                using (FileStream fs = new FileStream(exFile, FileMode.Open, FileAccess.Read))
                {
                    using (StreamReader sr = new StreamReader(fs))
                    {
                        while (!sr.EndOfStream)
                        {
                            string f = sr.ReadLine();
                            if (!string.IsNullOrEmpty(f))
                            {
                                excludes.Add(f, f);
                            }
                        }
                    }
                }
            }

            DirectoryInfo dirInfo = new DirectoryInfo(directory);
            if (!dirInfo.Exists)
                throw new Exception("Plugin folder " + directory + " does not exists");
            FileInfo[] dlls = dirInfo.GetFiles("*.dll");
            FileInfo[] css = dirInfo.GetFiles("*.cs");
            foreach (FileInfo fi in dlls)
            {
                if (!excludes.ContainsKey(fi.Name))
                    LoadPlugin(fi.FullName, "dll");
            }
            foreach (FileInfo fi in css)
            {
                if (!excludes.ContainsKey(fi.Name))
                    LoadPlugin(fi.FullName, "cs");
            }
        }

        public void LoadPlugins(List<string> files)
        {
            foreach (string file in files)
            {
                FileInfo fi = new FileInfo(WrapperUtil.ApplicationDirectory + "\\" + file);
                if(!fi.Exists)
                    throw new Exception("Plugin file " + file + " does not exists");
                switch (fi.Extension)
                {
                    case ".dll":
                        LoadPlugin(fi.FullName, "dll");
                        break;
                    case ".cs":
                        LoadPlugin(fi.FullName, "cs");
                        break;
                    default:
                        throw new Exception("Unknown plugin type: " + fi.Extension);
                }
            }
        }

        private void LoadPlugin(string file, string type)
        {
            if (PluginLoading != null)
            {
                string fileName = (new FileInfo(file)).Name;
                PluginLoading(fileName);
            }
#if !DEBUG
            try
#endif
            {
                Assembly assembly = null;
                switch (type)
                {
                    case "dll":
                        assembly = Assembly.LoadFile(file);
                        break;
                    case "cs":
                        FileStream fs = new FileStream(file, FileMode.Open, FileAccess.Read);
                        StreamReader sr = new StreamReader(fs);
                        string source = sr.ReadToEnd();
                        sr.Close();
                        fs.Close();
                        CSharpCodeProvider csc = new CSharpCodeProvider();
                        CompilerParameters param = new CompilerParameters();
                        param.ReferencedAssemblies.Add(System.Runtime.InteropServices.RuntimeEnvironment.GetRuntimeDirectory() + "mscorlib.dll");
                        param.ReferencedAssemblies.Add("System.dll");
                        param.ReferencedAssemblies.Add("System.Data.dll");
                        param.ReferencedAssemblies.Add("System.Deployment.dll");
                        param.ReferencedAssemblies.Add("System.Drawing.dll");
                        param.ReferencedAssemblies.Add("System.Xml.dll");
                        param.ReferencedAssemblies.Add("System.Xml.Linq.dll");
                        param.ReferencedAssemblies.Add("System.Windows.Forms.dll");
                        param.GenerateExecutable = false;
                        param.GenerateInMemory = true;
                        CompilerResults results = csc.CompileAssemblyFromSource(param, source);
                        StringBuilder sb = new StringBuilder("Output:\n");
                        foreach (string output in results.Output)
                        {
                            sb.Append(output + "\n");
                        }
                        sb.Append("\nErrors:\n");
                        bool withError = false;
                        foreach (CompilerError error in results.Errors)
                        {
                            withError = true;
                            sb.Append(error.ErrorText + "\n");
                        }
                        if (withError)
                            throw new Exception(sb.ToString());
                        else
                            assembly = results.CompiledAssembly;
                        break;
                    default:
                        throw new Exception("Unknown plugin type " + type);
                }
                if (LoadPlugin(assembly))
                {
                    if (PluginLoaded != null)
                    {
                        string fileName = (new FileInfo(file)).Name;
                        PluginLoaded(fileName);
                    }
                }
            }
#if !DEBUG
            catch (Exception ex)
            {
                string msg = ShellUtil.GetExceptionMsg(ex);
                ShellUtil.MsgBox(msg, MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
#endif
        }

        private bool LoadPlugin(Assembly assembly)
        {
            PluginInfo info = new PluginInfo();
            string name = string.Empty;
            Type[] types = assembly.GetExportedTypes();
            bool valid = false;
            foreach (Type t in types)
            {
                if (t.Name == "Plugin")
                {
                    valid = true;

                    PropertyInfo propN = t.GetProperty("Name");
                    name = (string)propN.GetValue(null, null);

                    PropertyInfo propId = t.GetProperty("Id");
                    info.Id = (uint)propId.GetValue(null, null);

                    info.Setup = t.GetMethod("Setup");
                    info.Shutdown = t.GetMethod("Shutdown");

                    PropertyInfo propCtrls = t.GetProperty("Controls");
                    info.Controls = (List<Control>)propCtrls.GetValue(null, null);
                    Action<Dictionary<Control, PluginInfo>, Control, PluginInfo> holderInserter = null;
                    holderInserter = (_coll, _ctrl, _info) =>
                    {
                        _coll[_ctrl] = _info;
                        foreach (Control _subCtrl in _ctrl.Controls)
                            holderInserter(_coll, _subCtrl, _info);
                    };
                    foreach (Control c in info.Controls)
                    {
                        holderInserter(controlHolders, c, info);
                    }
                    PropertyInfo propGuids = t.GetProperty("Guids");
                    info.Guids = (List<Guid>)propGuids.GetValue(null, null);

                    info.Host = t.GetField("Host");

                    info.Sink = t.GetField("Sink");
                    info.OnBubble = t.GetMethod("OnBubble");
                    info.PluginBroadcast = t.GetField("PluginBroadcast");
                    info.PluginSend = t.GetField("PluginSend");

                    PropertyInfo to = t.GetProperty("TabOrder");
                    if (to != null)
                        info.TabOrder = (uint)to.GetValue(null, null);
                    else
                        info.TabOrder = (uint)PluginTabPageOrder.PO_DEFAULT;

                    info.Preproc = t.GetMethod("Preproc");

                    info.GotFocus = t.GetMethod("GotFocus");
                    info.LostFocus = t.GetMethod("LostFocus");

                    info.MsgBox = t.GetField("MsgBox");
                    info.Echo = t.GetField("Echo");

                    info.GetGefCtrl = t.GetField("GetGefCtrl");

                    info.GetEdited = t.GetField("GetEdited");

                    info.HasCfg = t.GetField("HasCfg");
                    info.GetCfg = t.GetField("GetCfg");
                    info.SetCfg = t.GetField("SetCfg");
                    info.RemoveCfg = t.GetField("RemoveCfg");

                    info.HasScriptVar = t.GetField("HasScriptVar");
                    info.GetScriptVar = t.GetField("GetScriptVar");
                    info.SetScriptVar = t.GetField("SetScriptVar");
                    info.RemoveScriptVar = t.GetField("RemoveScriptVar");
                }
                else if (t.Name == "PluginTabPage")
                {
                    valid = true;

                    info.TabPage = ((Control)Activator.CreateInstance(t)).Controls["PluginToolStrip"];
                }
            }
            if (valid)
                plugins[name] = info;

            return valid;
        }
    }
}
