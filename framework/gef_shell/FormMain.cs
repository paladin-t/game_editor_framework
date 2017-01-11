/**
 * @version: 1.12
 * @author: Wang Renxin,
 *     https://github.com/paladin-t/game_editor_framework
 *     mailto:hellotony521@qq.com
 * @file: This file is a part of GEF, for copyright detail
 *     information, see the LICENSE file.
 */

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Reflection;
using Crom.Controls.Docking;
using System.Configuration;
using System.Xml.Linq;

namespace gef
{
    internal partial class FormMain : Form
    {
        private bool constructing = false;

        private Service service = null;

        private static FormMain self = null;

        private DataFile configuration = null;

        private Dictionary<string, FormDockable> name2formDict = null;
        public Dictionary<string, FormDockable> Name2FormDict
        {
            get { return name2formDict; }
        }

        private Dictionary<FormDockable, PluginManager.PluginInfo> form2PluginDict = null;

        private PluginManager.PluginInfo currentPlugin = null;
        public PluginManager.PluginInfo CurrentPlugin
        {
            get { return currentPlugin; }
            set
            {
                if (currentPlugin != null && currentPlugin.LostFocus != null)
                    currentPlugin.LostFocus.Invoke(null, null);
                currentPlugin = value;
                if (currentPlugin != null && currentPlugin.GotFocus != null)
                    currentPlugin.GotFocus.Invoke(null, null);
            }
        }

        public static FormMain GetInstance()
        {
            return self;
        }

        public FormMain()
        {
            Construct();
        }

        public FormMain(string path)
        {
            Construct();
            List<object> p = new List<object>();
            p.Add(path);
            DoSink(null, (uint)MsgGroupTypes.MGT_FILE, (uint)MsgFileTypes.MFT_FILE_RES_OPEN, p);
        }

        private void Construct()
        {
            constructing = true;

            self = this;

            // step 1
            FormSplash.GetInstance().Status = "Configurating";
            ScriptManager.GetInstance().Open();
            string configFile = WrapperUtil.ApplicationDirectory + "\\gef_init.py";
            ScriptManager.GetInstance().Scope.SetVariable("editor_title", string.Empty);
            ScriptManager.GetInstance().Scope.SetVariable("service_path", string.Empty);
            ScriptManager.GetInstance().Scope.SetVariable("plugin_folder", string.Empty);
            ScriptManager.GetInstance().Scope.SetVariable("plugin_files", new List<string>());
            ScriptManager.GetInstance().Scope.SetVariable("customer_copyright", string.Empty);
            ScriptManager.GetInstance().DoFile(configFile);
            string titleText = (string)ScriptManager.GetInstance().Scope.GetVariable("editor_title");
            string serviceFile = WrapperUtil.ApplicationDirectory + "\\" + (string)ScriptManager.GetInstance().Scope.GetVariable("service_path");
            string pluginFolder = (string)ScriptManager.GetInstance().Scope.GetVariable("plugin_folder");
            List<string> pluginFiles = (List<string>)ScriptManager.GetInstance().Scope.GetVariable("plugin_files");
            string customerCopyright = (string)ScriptManager.GetInstance().Scope.GetVariable("customer_copyright");
            ScriptManager.GetInstance().Scope.RemoveVariable("editor_title");
            ScriptManager.GetInstance().Scope.RemoveVariable("service_path");
            ScriptManager.GetInstance().Scope.RemoveVariable("customer_copyright");
            FormSplash.GetInstance().Title = titleText;
            FormSplash.GetInstance().CustomerCopyright = customerCopyright;

            // step 2
            FormSplash.GetInstance().Status = "Opening service: " + (new FileInfo(serviceFile)).Name;
            service = new Service();
            try
            {
                service.Open(serviceFile);
            }
            catch (Exception ex)
            {
                string msg = "Service loading failed.\n" + "Path: " + serviceFile + "\n" + ShellUtil.GetExceptionMsg(ex);
                throw new Exception(msg);
            }
            service.BubbledMsgEvent += new Service.BubbledMsgEventHandler(service_BubbledMsgEvent);
            service.BroadcastedMsgEvent += new Service.BubbledMsgEventHandler(service_BroadcastedMsgEvent);
            ScriptManager.GetInstance().Scope.SetVariable("shell", ScriptManager.GetInstance());
            ScriptManager.GetInstance().Scope.SetVariable("service", service);

            // step 3
            List<string> plnNames = new List<string>();
            FormSplash.GetInstance().Status = "Loading plugins";
            PluginManager.GetInstance().Open();
            PluginManager.GetInstance().PluginLoading += (_f) => { FormSplash.GetInstance().Status = "Loading plugin: " + _f; };
            PluginManager.GetInstance().PluginLoaded += (_f) => { plnNames.Add(_f); };
            string pluginDir = WrapperUtil.ApplicationDirectory + "\\" + pluginFolder;
            if (!string.IsNullOrEmpty(pluginFolder))
                PluginManager.GetInstance().LoadPlugins(pluginDir);
            PluginManager.GetInstance().LoadPlugins(pluginFiles);
            ScriptManager.GetInstance().Scope.SetVariable("EXE_DIR", WrapperUtil.ApplicationDirectory);
            ScriptManager.GetInstance().Scope.SetVariable("PLUGIN_DIR", pluginDir);

            // step 4
            FormSplash.GetInstance().Status = "Initializing editor shell";
            InitializeComponent();
            Text = titleText;

            // step 5
            FormSplash.GetInstance().Status = "Initializing editor layout";
            name2formDict = new Dictionary<string, FormDockable>();
            form2PluginDict = new Dictionary<FormDockable, PluginManager.PluginInfo>();
            Dictionary<Guid, Form> guid2form = new Dictionary<Guid, Form>();
            List<TabPage> tabPages = new List<TabPage>();
            Dictionary<string, PluginManager.PluginInfo>.Enumerator pluginInfoIter = PluginManager.GetInstance().Plugins.GetEnumerator();
            DragEventHandler dragEnter = new DragEventHandler(dockableForm_DragEnter);
            DragEventHandler dragOver = new DragEventHandler(dockableForm_DragOver);
            DragEventHandler dragDrop = new DragEventHandler(dockableForm_DragDrop);
            EventHandler dragLeave = new EventHandler(dockableForm_DragLeave);
            while (pluginInfoIter.MoveNext())
            {
                PluginManager.PluginInfo pluginInfo = pluginInfoIter.Current.Value;
                if (pluginInfo.TabPage != null)
                {
                    TabPage tabPage = new TabPage(pluginInfoIter.Current.Key);
                    tabPage.Name = pluginInfoIter.Current.Key;
                    tabPage.Controls.Add(pluginInfo.TabPage);
                    tabPage.Tag = pluginInfo.TabOrder;
                    tabPages.Add(tabPage);
                }
                if (pluginInfo.Host != null)
                    pluginInfo.Host.SetValue(null, this);
                if (pluginInfo.Sink != null)
                    pluginInfo.Sink.SetValue(null, GetType().GetMethod("Sink"));
                if (pluginInfo.PluginBroadcast != null)
                    pluginInfo.PluginBroadcast.SetValue(null, GetType().GetMethod("PluginBroadcast"));
                if (pluginInfo.PluginSend != null)
                    pluginInfo.PluginSend.SetValue(null, GetType().GetMethod("PluginSend"));
                if (pluginInfo.MsgBox != null)
                    pluginInfo.MsgBox.SetValue(null, typeof(ShellUtil).GetMethod("MsgBox"));
                if (pluginInfo.Echo != null)
                    pluginInfo.Echo.SetValue(null, GetType().GetMethod("DoEcho"));
                if (pluginInfo.GetGefCtrl != null)
                    pluginInfo.GetGefCtrl.SetValue(null, GetType().GetMethod("GetGefControl"));
                if (pluginInfo.GetEdited != null)
                    pluginInfo.GetEdited.SetValue(null, GetType().GetMethod("GetEdited"));
                if (pluginInfo.HasCfg != null)
                    pluginInfo.HasCfg.SetValue(null, GetType().GetMethod("DoHasCfg"));
                if (pluginInfo.GetCfg != null)
                    pluginInfo.GetCfg.SetValue(null, GetType().GetMethod("DoGetCfg"));
                if (pluginInfo.SetCfg != null)
                    pluginInfo.SetCfg.SetValue(null, GetType().GetMethod("DoSetCfg"));
                if (pluginInfo.RemoveCfg != null)
                    pluginInfo.RemoveCfg.SetValue(null, GetType().GetMethod("DoRemoveCfg"));
                if (pluginInfo.HasScriptVar != null)
                    pluginInfo.HasScriptVar.SetValue(null, GetType().GetMethod("DoHasScriptVar"));
                if (pluginInfo.GetScriptVar != null)
                    pluginInfo.GetScriptVar.SetValue(null, GetType().GetMethod("DoGetScriptVar"));
                if (pluginInfo.SetScriptVar != null)
                    pluginInfo.SetScriptVar.SetValue(null, GetType().GetMethod("DoSetScriptVar"));
                if (pluginInfo.RemoveScriptVar != null)
                    pluginInfo.RemoveScriptVar.SetValue(null, GetType().GetMethod("DoRemoveScriptVar"));
                for (int i = 0; i < pluginInfo.Controls.Count; i++)
                {
                    Control c = pluginInfo.Controls[i];
                    c.Dock = DockStyle.Fill;
                    FormDockable fd = new FormDockable();
                    fd.Text = fd.Name = c.Name;
                    guid2form[pluginInfo.Guids[i]] = fd;
                    fd.Controls.Add(c);
                    DockStyle ds = ShellUtil.ConvertDockStyle((string)c.Tag);
                    DockableFormInfo info = dockContainerMain.Add(fd, zAllowedDock.All, pluginInfo.Guids[i]);
                    info.ShowCloseButton = false;
                    dockContainerMain.DockForm(info, ds, zDockMode.Outer);
                    fd.ForEachCtrl
                    (
                        (_ctrl) =>
                        {
                            _ctrl.DragEnter += dragEnter;
                            _ctrl.DragOver += dragOver;
                            _ctrl.DragDrop += dragDrop;
                            _ctrl.DragLeave += dragLeave;
                        }
                    );

                    name2formDict[fd.Name] = fd;

                    form2PluginDict[fd] = pluginInfo;
                    fd.GotFocus += (_sender, _e) =>
                    {
                        PluginManager.PluginInfo _info = form2PluginDict[(FormDockable)_sender];
                        if (_info.Id != PluginManager.SysPluginId)
                            CurrentPlugin = _info;
                        List<object> _par = new List<object>();
                        _par.Add(((FormDockable)_sender).Name);
                        PluginBroadcast(null, null, (uint)MsgGroupTypes.MGT_SYSTEM, (uint)MsgSystemTypes.MST_SYS_PANEL_GAIN_FOCUS, _par);
                    };
                    fd.LostFocus += (_sender, _e) =>
                    {
                        List<object> _par = new List<object>();
                        _par.Add(((FormDockable)_sender).Name);
                        PluginBroadcast(null, null, (uint)MsgGroupTypes.MGT_SYSTEM, (uint)MsgSystemTypes.MST_SYS_PANEL_LOST_FOCUS, _par);
                    };
                }
            }
            tabPages = tabPages.OrderBy((_tabPage) =>
            {
                return (uint)_tabPage.Tag;
            }).ToList();
            tabControlMain.TabPages.AddRange(tabPages.ToArray());
            {
                DebugPanel dbgp = new DebugPanel();
                dbgp.Dock = DockStyle.Fill;
                FormDockable fdbg = new FormDockable();
                fdbg.Text = "Debug Console";
                fdbg.Name = dbgp.Name;
                guid2form[dbgp.Guid] = fdbg;
                fdbg.Controls.Add(dbgp);
                DockStyle ds = DockStyle.Bottom;
                DockableFormInfo info = dockContainerMain.Add(fdbg, zAllowedDock.All, dbgp.Guid);
                info.ShowCloseButton = false;
                dockContainerMain.DockForm(info, ds, zDockMode.Outer);

                name2formDict[fdbg.Name] = fdbg;

                object[] attributes = Assembly.GetExecutingAssembly().GetCustomAttributes(typeof(AssemblyCopyrightAttribute), false);
                dbgp.WriteLine(dbgp.ForeColor, "GEF version " + Assembly.GetExecutingAssembly().GetName().Version.ToString());
                dbgp.WriteLine(dbgp.ForeColor, ((AssemblyCopyrightAttribute)attributes[0]).Copyright);
                dbgp.WriteLine(string.Empty);
                dbgp.WriteLine(dbgp.ForeColor, "Service:\n    " + (new FileInfo(serviceFile)).Name);
                dbgp.WriteLine(dbgp.ForeColor, "Plugins:");
                foreach (string plnn in plnNames)
                {
                    dbgp.WriteLine(dbgp.ForeColor, "    " + plnn);
                }
                dbgp.WriteLine(string.Empty);
                dbgp.WriteLine(dbgp.ForeColor, "Enter \"shell.help()\" below and press enter to see more help information.");
            }
            DockStateSerializer ser = new DockStateSerializer(dockContainerMain);
            ser.SavePath = WrapperUtil.ApplicationDirectory + "\\gef_layout.xml";
            if (File.Exists(ser.SavePath))
            {
                try { ser.Load(true, (_id) => { return guid2form[_id]; }); }
                catch { }
            }
            string gcxf = WrapperUtil.ApplicationDirectory + "\\gef_config.xml";
            if (File.Exists(gcxf))
            {
                try
                {
                    configuration = new DataFile();
                    configuration.Load(gcxf);
                    Location = ShellUtil.Parse<Point>(configuration.Item["win_position"].Value);
                    Size = ShellUtil.Parse<Size>(configuration.Item["win_size"].Value);
                    WindowState = configuration.Item["win_state"].Value == "max" ? FormWindowState.Maximized : FormWindowState.Normal;
                }
                catch { }
            }

            // step 6
            FormSplash.GetInstance().Status = "Doing plugin setup";
            foreach (PluginManager.PluginInfo pi in PluginManager.GetInstance().Plugins.Values)
            {
                if (pi.Setup != null) pi.Setup.Invoke(null, null);
            }

            // setp 7
            FormSplash.GetInstance().Close();

            configFile = WrapperUtil.ApplicationDirectory + "\\gef_opened.py";
            ScriptManager.GetInstance().DoFile(configFile);

            service.PostInitDone();

            constructing = false;
        }

        private void FormMain_FormClosed(object sender, FormClosedEventArgs e)
        {
            // step 1
            string configFile = WrapperUtil.ApplicationDirectory + "\\gef_closing.py";
            ScriptManager.GetInstance().DoFile(configFile);

            // step 2
            foreach (PluginManager.PluginInfo pi in PluginManager.GetInstance().Plugins.Values)
            {
                if (pi.Shutdown != null) pi.Shutdown.Invoke(null, null);
            }

            // step 3
            if (WindowState != FormWindowState.Minimized)
            {
                configuration.Item["win_position"] = new DataFile.DataItem(configuration.Item, DataFile.DataItem.DATA_ITEM_TYPE.DATA_ITEM_TYPE_NODE,
                    "win_position",
                    ShellUtil.ToString<Point>(Location));
                configuration.Item["win_size"] = new DataFile.DataItem(configuration.Item, DataFile.DataItem.DATA_ITEM_TYPE.DATA_ITEM_TYPE_NODE,
                    "win_size",
                    ShellUtil.ToString<Size>(Size));
            }
            configuration.Item["win_state"] = new DataFile.DataItem(configuration.Item, DataFile.DataItem.DATA_ITEM_TYPE.DATA_ITEM_TYPE_NODE,
                "win_state",
                WindowState == FormWindowState.Maximized ? "max" : "normal");
            configuration.Save(WrapperUtil.ApplicationDirectory + "\\gef_config.xml");

            // step 4
            DockStateSerializer ser = new DockStateSerializer(dockContainerMain);
            ser.SavePath = WrapperUtil.ApplicationDirectory + "\\gef_layout.xml";
            ser.Save();

            // step 5
            CurrentPlugin = null;

            // step 6
            PluginManager.GetInstance().Close();

            // step 7
            service.PrevDestroy();
            service.Close();

            // step 8
            ScriptManager.GetInstance().Close();

            self = null;
        }

        private void FormMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (GetEdited())
            {
                DialogResult dr = ShellUtil.MsgBox("Save changes?", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);
                if (dr == DialogResult.Yes)
                {
                    Sink(this, (uint)MsgGroupTypes.MGT_FILE, (uint)MsgFileTypes.MFT_FILE_SAVE, null);
                }
                else if (dr == DialogResult.No)
                {
                    // do nothing
                }
                else if (dr == DialogResult.Cancel)
                {
                    e.Cancel = true;
                }
            }
        }

        public DialogResult MsgBox(string text, MessageBoxButtons btn, MessageBoxIcon icon)
        {
            if (constructing)
                return MessageBox.Show(this, text, "GEF", btn, icon);
            else
                return (DialogResult)Invoke
                (
                    new Func<string, MessageBoxButtons, MessageBoxIcon, DialogResult>
                    (
                        (_text, _btn, _icon) =>
                        {
                            return MessageBox.Show(this, _text, "GEF", _btn, _icon);
                        }
                    ),
                    text, btn, icon
                );
        }

        public void DoEcho(params object[] p)
        {
            DebugPanel.GetInstance().Echo(p);
        }

        public bool GetEdited()
        {
            return service.GetEdited();
        }

        private void dockableForm_DragEnter(object sender, DragEventArgs e)
        {
            PluginManager.PluginInfo plugin = PluginManager.GetInstance().ControlHolders[(Control)sender];
            if (plugin != null)
            {
                uint g = (uint)MsgGroupTypes.MGT_SYSTEM;
                uint t = (uint)MsgSystemTypes.MST_SYS_DRAG_ENTER;
                List<object> p = new List<object>();
                p.Add(e);
                SendToPlugin(plugin, null, g, t, p);
            }
        }

        private void dockableForm_DragOver(object sender, DragEventArgs e)
        {
            PluginManager.PluginInfo plugin = PluginManager.GetInstance().ControlHolders[(Control)sender];
            if (plugin != null)
            {
                uint g = (uint)MsgGroupTypes.MGT_SYSTEM;
                uint t = (uint)MsgSystemTypes.MST_SYS_DRAG_OVER;
                List<object> p = new List<object>();
                p.Add(e);
                SendToPlugin(plugin, null, g, t, p);
            }
        }

        private void dockableForm_DragDrop(object sender, DragEventArgs e)
        {
            PluginManager.PluginInfo plugin = PluginManager.GetInstance().ControlHolders[(Control)sender];
            if (plugin != null)
            {
                uint g = (uint)MsgGroupTypes.MGT_SYSTEM;
                uint t = (uint)MsgSystemTypes.MST_SYS_DRAG_DROP;
                List<object> p = new List<object>();
                p.Add(e);
                SendToPlugin(plugin, null, g, t, p);
            }
        }

        private void dockableForm_DragLeave(object sender, EventArgs e)
        {
            PluginManager.PluginInfo plugin = PluginManager.GetInstance().ControlHolders[(Control)sender];
            if (plugin != null)
            {
                uint g = (uint)MsgGroupTypes.MGT_SYSTEM;
                uint t = (uint)MsgSystemTypes.MST_SYS_DRAG_LEAVE;
                List<object> p = new List<object>();
                p.Add(e);
                SendToPlugin(plugin, null, g, t, p);
            }
        }

        public bool DoHasCfg(string path)
        {
            return configuration.PeekItem(path, false) == null;
        }

        public string DoGetCfg(string path)
        {
            DataFile.DataItem item = configuration.PeekItem(path, false);
            return item != null ? item.Value : null;
        }

        public void DoSetCfg(string path, string val)
        {
            DataFile.DataItem item = configuration.PeekItem(path, true);
            item.Value = val;
        }

        public bool DoRemoveCfg(string path)
        {
            DataFile.DataItem item = configuration.PeekItem(path, false);
            if (item == null)
                return false;
            item.Parent.Children.Remove(item.Name);
            return true;
        }

        public bool DoHasScriptVar(string name)
        {
            return ScriptManager.GetInstance().Scope.ContainsVariable(name);
        }

        public object DoGetScriptVar(string name)
        {
            return ScriptManager.GetInstance().Scope.GetVariable(name);
        }

        public void DoSetScriptVar(string name, object val)
        {
            ScriptManager.GetInstance().Scope.SetVariable(name, val);
        }

        public bool DoRemoveScriptVar(string name)
        {
            return ScriptManager.GetInstance().Scope.RemoveVariable(name);
        }

        private PluginManager.PluginInfo GetCurrentPlugin()
        {
            if (CurrentPlugin != null)
                return CurrentPlugin;

            if (tabControlMain.TabPages.Count == 0 || tabControlMain.SelectedTab == null) return null;
            string tab = tabControlMain.SelectedTab.Text;
            if (!PluginManager.GetInstance().Plugins.Keys.Contains(tab)) return null;
            else if (PluginManager.GetInstance().Plugins == null) return null;
            PluginManager.PluginInfo pluginInfo = PluginManager.GetInstance().Plugins[tab];

            return pluginInfo;
        }

        private void SendToPlugin(PluginManager.PluginInfo plugin, object s, uint g, uint t, List<object> p)
        {
            ScriptManager.GetInstance().AppendMsg(ScriptManager.MsgDispType.PluginBroadcast, g, t, p);
            if (plugin.OnBubble != null)
            {
                object[] _objs = { g, t, p };
                plugin.OnBubble.Invoke(null, _objs);
            }
        }

        public void PluginBroadcast(Control c, object s, uint g, uint t, List<object> p)
        {
            PluginManager.PluginInfo info = c == null ? null : PluginManager.GetInstance().ControlHolders[c];
            if (PluginManager.GetInstance().Plugins != null)
            {
                foreach (PluginManager.PluginInfo currPlugin in PluginManager.GetInstance().Plugins.Values)
                {
                    if (info != currPlugin)
                    {
                        object[] objs = { s, g, t, p };
                        if (currPlugin != null && currPlugin.Preproc != null)
                        {
                            bool filtered = (bool)currPlugin.Preproc.Invoke(null, objs);
                            if (filtered) return;
                        }
                        SendToPlugin(currPlugin, s, g, t, p);
                    }
                }
            }
        }

        public void PluginSend(object s, uint g, uint t, List<object> p)
        {
            PluginManager.PluginInfo currPlugin = GetCurrentPlugin();
            SendToPlugin(currentPlugin, s, g, t, p);
        }

        public void Sink(object s, uint g, uint t, List<object> p)
        {
            PluginManager.PluginInfo currPlugin = GetCurrentPlugin();
            object[] objs = { s, g, t, p };
            if (currPlugin != null && currPlugin.Preproc != null)
            {
                bool filtered = (bool)currPlugin.Preproc.Invoke(null, objs);
                if (filtered) return;
            }

            ScriptManager.GetInstance().AppendMsg(ScriptManager.MsgDispType.Sink, g, t, p);

            if (g == (uint)MsgGroupTypes.MGT_SCRIPT && t == (uint)MsgScriptTypes.MST_SCRIPT_REQ_LIST)
                OnReqScriptList(s, g, t, p);
            else if (g == (uint)MsgGroupTypes.MGT_SCRIPT && t == (uint)MsgScriptTypes.MST_SCRIPT_RUN)
                OnRunScript(s, g, t, p);
            else if (g == (uint)MsgGroupTypes.MGT_HELP && t == (uint)MsgHelpTypes.MHT_HELP_SHOW_ABOUT)
                OnShowAbout(s, g, t, p);
            else if (g == (uint)MsgGroupTypes.MGT_HELP && t == (uint)MsgHelpTypes.MHT_HELP_SHOW_HELP)
                OnShowHelp(s, g, t, p);
            else
                DoSink(s, g, t, p);
        }

        private void service_BubbledMsgEvent(uint g, uint t, List<object> p)
        {
            ScriptManager.GetInstance().AppendMsg(ScriptManager.MsgDispType.Bubble, g, t, p);

            if (g == (uint)MsgGroupTypes.MGT_SYSTEM && t == (uint)MsgSystemTypes.MST_SYS_REQ_HWND)
                OnReqHwnd(g, t, p);
            else if (g == (uint)MsgGroupTypes.MGT_SYSTEM && t == (uint)MsgSystemTypes.MST_SYS_REFRESH_CTRL)
                OnRefreshCtrl(g, t, p);
            else if (g == (uint)MsgGroupTypes.MGT_SYSTEM && t == (uint)MsgSystemTypes.MST_SYS_ACTIVE_TAB)
                OnActiveTab(g, t, p);
            else if (g == (uint)MsgGroupTypes.MGT_EDIT && t == (uint)MsgEditTypes.MET_EDIT_HAS_UNDO)
                OnHasUndoOrRedo(g, t, p);
            else if (g == (uint)MsgGroupTypes.MGT_EDIT && t == (uint)MsgEditTypes.MET_EDIT_HAS_REDO)
                OnHasUndoOrRedo(g, t, p);
            else if (g == (uint)MsgGroupTypes.MGT_SYSTEM && t == (uint)MsgSystemTypes.MST_SYS_ECHO)
                OnEcho(g, t, p);
            else
                OnBubble(g, t, p);
        }

        private void service_BroadcastedMsgEvent(uint g, uint t, List<object> p)
        {
            ScriptManager.GetInstance().AppendMsg(ScriptManager.MsgDispType.Bubble, g, t, p);
            PluginBroadcast(null, null, g, t, p);
        }
    }
}
