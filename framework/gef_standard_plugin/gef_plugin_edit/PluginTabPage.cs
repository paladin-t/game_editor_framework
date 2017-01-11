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
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace gef
{
    public partial class PluginTabPage : UserControl
    {
        private Action<List<object>> keyDown = null;
        private Action<List<object>> keyUp = null;
        private Action<List<object>> keyPress = null;

        private void ClearActions()
        {
            keyDown = null;
            keyUp = null;
            keyPress = null;
        }

        public PluginTabPage()
        {
            InitializeComponent();

            SetKeyEventHandler();

            Plugin.Bubble += (_g, _t, _p) => Plugin.MsgProc.Raise(_g, _t, _p);

            Plugin.MsgProc.RegMsgProc((uint)MsgGroupTypes.MGT_EDIT, (uint)MsgEditTypes.MET_EDIT_HAS_UNDO, new MessageProcessor.MsgEventHandler(OnHasUndo));
            Plugin.MsgProc.RegMsgProc((uint)MsgGroupTypes.MGT_EDIT, (uint)MsgEditTypes.MET_EDIT_HAS_REDO, new MessageProcessor.MsgEventHandler(OnHasUndo));
            Plugin.MsgProc.RegMsgProc((uint)MsgGroupTypes.MGT_INPUT_KEYBOARD, (uint)MsgInputKeyboardTypes.MIK_KEY_DOWN, new MessageProcessor.MsgEventHandler(OnKeyDown));
            Plugin.MsgProc.RegMsgProc((uint)MsgGroupTypes.MGT_INPUT_KEYBOARD, (uint)MsgInputKeyboardTypes.MIK_KEY_UP, new MessageProcessor.MsgEventHandler(OnKeyUp));
            Plugin.MsgProc.RegMsgProc((uint)MsgGroupTypes.MGT_INPUT_KEYBOARD, (uint)MsgInputKeyboardTypes.MIK_KEY_PRESSED, new MessageProcessor.MsgEventHandler(OnKeyPressed));
        }

        private void OnHasUndo(uint group, uint type, List<object> param)
        {
            bool h = (bool)param[0];
            btnUndo.Enabled = h;
        }

        private void OnHasRedo(uint group, uint type, List<object> param)
        {
            bool h = (bool)param[0];
            btnRedo.Enabled = h;
        }

        private void OnKeyDown(uint group, uint type, List<object> param)
        {
            if (keyDown != null)
                keyDown(param);
        }

        private void OnKeyUp(uint group, uint type, List<object> param)
        {
            if (keyUp != null)
                keyUp(param);
        }

        private void OnKeyPressed(uint group, uint type, List<object> param)
        {
            if (keyPress != null)
                keyPress(param);
        }

        private void SetKeyEventHandler()
        {
            ClearActions();

            bool altDown = false;
            bool ctrlDown = false;
            bool shiftDown = false;
            keyDown = new Action<List<object>>
            (
                _p =>
                {
                    altDown = (bool)_p[0];
                    ctrlDown = (bool)_p[1];
                    shiftDown = (bool)_p[2];
                }
            );
            keyUp = new Action<List<object>>
            (
                _p =>
                {
                    altDown = (bool)_p[0];
                    ctrlDown = (bool)_p[1];
                    shiftDown = (bool)_p[2];

                    int kv = (int)_p[3];
                    if (ctrlDown)
                    {
                        if (kv == (int)'Z')
                        {
                            btnUndo_Click(null, null);
                        }
                        else if (kv == (int)'Y')
                        {
                            btnRedo_Click(null, null);
                        }
                    }
                }
            );
            keyPress = new Action<List<object>>
            (
                _p =>
                {
                }
            );
        }

        private void btnDelete_Click(object sender, EventArgs e)
        {
            Plugin.DoSink((uint)MsgGroupTypes.MGT_EDIT, (uint)MsgEditTypes.MET_EDIT_DELETE, null);
        }

        private void btnCopy_Click(object sender, EventArgs e)
        {
            Plugin.DoSink((uint)MsgGroupTypes.MGT_EDIT, (uint)MsgEditTypes.MET_EDIT_COPY, null);
        }

        private void btnCut_Click(object sender, EventArgs e)
        {
            Plugin.DoSink((uint)MsgGroupTypes.MGT_EDIT, (uint)MsgEditTypes.MET_EDIT_CUT, null);
        }

        private void btnPaste_Click(object sender, EventArgs e)
        {
            Plugin.DoSink((uint)MsgGroupTypes.MGT_EDIT, (uint)MsgEditTypes.MET_EDIT_PASTE, null);
        }

        private void btnUndo_Click(object sender, EventArgs e)
        {
            Plugin.DoSink((uint)MsgGroupTypes.MGT_EDIT, (uint)MsgEditTypes.MET_EDIT_UNDO, null);
        }

        private void btnRedo_Click(object sender, EventArgs e)
        {
            Plugin.DoSink((uint)MsgGroupTypes.MGT_EDIT, (uint)MsgEditTypes.MET_EDIT_REDO, null);
        }
    }
}
