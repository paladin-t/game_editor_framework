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
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace gef
{
    public partial class PluginTabPage : UserControl
    {
        private string workingFolder = string.Empty;

        public PluginTabPage()
        {
            InitializeComponent();

            Plugin.Bubble += (_g, _t, _p) => Plugin.MsgProc.Raise(_g, _t, _p);

            Plugin.MsgProc.RegMsgProc((uint)MsgGroupTypes.MGT_FILE, (uint)MsgFileTypes.MFT_FILE_REQ_OPEN, new MessageProcessor.MsgEventHandler(OnFileOperation));
            Plugin.MsgProc.RegMsgProc((uint)MsgGroupTypes.MGT_FILE, (uint)MsgFileTypes.MFT_FILE_REQ_SAVE, new MessageProcessor.MsgEventHandler(OnFileOperation));
        }

        private void OnFileOperation(uint group, uint type, List<object> param)
        {
            Plugin.DoPluginBroadcast(null, (uint)MsgGroupTypes.MGT_SYSTEM, (uint)MsgSystemTypes.MST_SYS_PAUSE_RENDER, null);

            bool useFileMode = Plugin.DoHasScriptVar("project_use_file_mode") && (bool)Plugin.DoGetScriptVar("project_use_file_mode");
            bool alsoNotifyIfCanceled = Plugin.DoHasScriptVar("also_notify_if_file_dlg_canceled") && (bool)Plugin.DoGetScriptVar("also_notify_if_file_dlg_canceled");
            if (type == (uint)MsgFileTypes.MFT_FILE_REQ_OPEN)
            {
                PushWorkingFolder();
                string latestPath = Plugin.DoGetCfg("plugin.std.file.path_cache.open");
                string path = string.Empty;
                if (useFileMode)
                {
                    OpenFileDialog ofd = new OpenFileDialog();
                    ofd.Filter = (string)param[0];
                    ofd.InitialDirectory = latestPath;
                    if (ofd.ShowDialog((IWin32Window)Plugin.Host) != DialogResult.Cancel)
                    {
                        path = ofd.FileName;
                        latestPath = (new FileInfo(path)).Directory.FullName;
                        Plugin.DoSetCfg("plugin.std.file.path_cache.open", latestPath);
                    }
                }
                else
                {
                    FolderBrowserDialog fbd = new FolderBrowserDialog();
                    fbd.SelectedPath = latestPath;
                    if (fbd.ShowDialog((IWin32Window)Plugin.Host) != DialogResult.Cancel)
                    {
                        path = fbd.SelectedPath;
                        latestPath = path;
                        Plugin.DoSetCfg("plugin.std.file.path_cache.open", latestPath);
                    }
                }
                PopWorkingFolder();
                if (!string.IsNullOrEmpty(path) || (string.IsNullOrEmpty(path) && alsoNotifyIfCanceled))
                {
                    List<object> p = new List<object>();
                    p.Add(path);
                    Plugin.DoSink((uint)MsgGroupTypes.MGT_FILE, (uint)MsgFileTypes.MFT_FILE_RES_OPEN, p);
                }
            }
            else if (type == (uint)MsgFileTypes.MFT_FILE_REQ_SAVE)
            {
                PushWorkingFolder();
                string latestPath = Plugin.DoGetCfg("plugin.std.file.path_cache.save");
                string path = string.Empty;
                if (useFileMode)
                {
                    SaveFileDialog sfd = new SaveFileDialog();
                    sfd.Filter = (string)param[0];
                    sfd.InitialDirectory = latestPath;
                    if (sfd.ShowDialog((IWin32Window)Plugin.Host) != DialogResult.Cancel)
                    {
                        path = sfd.FileName;
                        latestPath = (new FileInfo(path)).Directory.FullName;
                        Plugin.DoSetCfg("plugin.std.file.path_cache.save", latestPath);
                    }
                }
                else
                {
                    FolderBrowserDialog fbd = new FolderBrowserDialog();
                    fbd.SelectedPath = latestPath;
                    if (fbd.ShowDialog((IWin32Window)Plugin.Host) != DialogResult.Cancel)
                    {
                        path = fbd.SelectedPath;
                        latestPath = path;
                        Plugin.DoSetCfg("plugin.std.file.path_cache.save", latestPath);
                    }
                }
                PopWorkingFolder();
                if (!string.IsNullOrEmpty(path) || (string.IsNullOrEmpty(path) && alsoNotifyIfCanceled))
                {
                    List<object> p = new List<object>();
                    p.Add(path);
                    Plugin.DoSink((uint)MsgGroupTypes.MGT_FILE, (uint)MsgFileTypes.MFT_FILE_RES_SAVE, p);
                }
            }

            Plugin.DoPluginBroadcast(null, (uint)MsgGroupTypes.MGT_SYSTEM, (uint)MsgSystemTypes.MST_SYS_RESUME_RENDER, null);
        }

        private void btnNew_Click(object sender, EventArgs e)
        {
            if (!Plugin.EnsureSaved())
            {
                Plugin.DoSink((uint)MsgGroupTypes.MGT_FILE, (uint)MsgFileTypes.MFT_FILE_NEW, null);
            }
        }

        private void btnOpen_Click(object sender, EventArgs e)
        {
            if (!Plugin.EnsureSaved())
            {
                Plugin.DoSink((uint)MsgGroupTypes.MGT_FILE, (uint)MsgFileTypes.MFT_FILE_OPEN, null);
            }
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            Plugin.DoSink((uint)MsgGroupTypes.MGT_FILE, (uint)MsgFileTypes.MFT_FILE_SAVE, null);
        }

        private void btnSaveAs_Click(object sender, EventArgs e)
        {
            Plugin.DoSink((uint)MsgGroupTypes.MGT_FILE, (uint)MsgFileTypes.MFT_FILE_SAVE_AS, null);
        }

        private void btnImport_Click(object sender, EventArgs e)
        {
            Plugin.DoSink((uint)MsgGroupTypes.MGT_FILE, (uint)MsgFileTypes.MFT_FILE_IMPORT, null);
        }

        private void btnExport_Click(object sender, EventArgs e)
        {
            Plugin.DoSink((uint)MsgGroupTypes.MGT_FILE, (uint)MsgFileTypes.MFT_FILE_EXPORT, null);
        }

        private void PushWorkingFolder()
        {
            workingFolder = Environment.CurrentDirectory;
        }

        private void PopWorkingFolder()
        {
            Environment.CurrentDirectory = workingFolder;
        }
    }
}
