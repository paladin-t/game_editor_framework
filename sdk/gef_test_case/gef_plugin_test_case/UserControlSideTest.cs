/**
 * @version: 1.1
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
    public partial class UserControlSideTest : UserControl
    {
        public UserControlSideTest()
        {
            InitializeComponent();

            Plugin.MsgProc.RegMsgProc((uint)MsgGroupTypes.MGT_SYSTEM, (uint)MsgSystemTypes.MST_SYS_DRAG_ENTER, new MessageProcessor.MsgEventHandler(OnDragEnter));
            Plugin.MsgProc.RegMsgProc((uint)MsgGroupTypes.MGT_SYSTEM, (uint)MsgSystemTypes.MST_SYS_DRAG_DROP, new MessageProcessor.MsgEventHandler(OnDragDrop));
        }

        private void OnDragEnter(uint group, uint type, List<object> param)
        {
            DragEventArgs e = (DragEventArgs)param[0];
            string[] fileList = (string[])e.Data.GetData(DataFormats.FileDrop, false);
            if (e.Data.GetDataPresent(DataFormats.FileDrop) && fileList.Length == 1)
                e.Effect = DragDropEffects.Copy;
            else
                e.Effect = DragDropEffects.None;
        }

        private void OnDragDrop(uint group, uint type, List<object> param)
        {
            DragEventArgs e = (DragEventArgs)param[0];
            string[] fileList = (string[])e.Data.GetData(DataFormats.FileDrop, false);
            FileStream fs = new FileStream(fileList[0], FileMode.Open, FileAccess.Read);
            StreamReader sr = new StreamReader(fs);
            textBoxTest.Text = sr.ReadToEnd();
            sr.Close();
            fs.Close();
        }
    }
}
