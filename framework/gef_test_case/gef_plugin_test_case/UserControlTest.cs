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

namespace gef
{
    public partial class UserControlTest : UserControl
    {
        private Action<List<object>> mouseDown = null;
        private Action<List<object>> mouseUp = null;
        private Action<List<object>> mouseClicked = null;
        private Action<List<object>> mouseDoubleClicked = null;
        private Action<List<object>> mouseMove = null;
        private Action<List<object>> keyDown = null;
        private Action<List<object>> keyUp = null;
        private Action<List<object>> keyPress = null;

        private void ClearActions()
        {
            mouseDown = null;
            mouseUp = null;
            mouseClicked = null;
            mouseDoubleClicked = null;
            mouseMove = null;
            keyDown = null;
            keyUp = null;
            keyPress = null;
        }

        public UserControlTest()
        {
            InitializeComponent();

            Plugin.MsgProc.RegMsgProc
            (
                (uint)MsgGroupTypes.MGT_INPUT_MOUSE, (uint)MsgInputMouseTypes.MIM_MOUSE_DOWN,
                (_g, _t, _p) => { if (mouseDown != null) mouseDown(_p); }
            );
            Plugin.MsgProc.RegMsgProc
            (
                (uint)MsgGroupTypes.MGT_INPUT_MOUSE, (uint)MsgInputMouseTypes.MIM_MOUSE_UP,
                (_g, _t, _p) => { if (mouseUp != null) mouseUp(_p); }
            );
            Plugin.MsgProc.RegMsgProc
            (
                (uint)MsgGroupTypes.MGT_INPUT_MOUSE, (uint)MsgInputMouseTypes.MIM_MOUSE_CLICKED,
                (_g, _t, _p) => { if (mouseClicked != null) mouseClicked(_p); }
            );
            Plugin.MsgProc.RegMsgProc
            (
                (uint)MsgGroupTypes.MGT_INPUT_MOUSE, (uint)MsgInputMouseTypes.MIM_MOUSE_DOUBLE_CLICKED,
                (_g, _t, _p) => { if (mouseDoubleClicked != null) mouseDoubleClicked(_p); }
            );
            Plugin.MsgProc.RegMsgProc
            (
                (uint)MsgGroupTypes.MGT_INPUT_MOUSE, (uint)MsgInputMouseTypes.MIM_MOUSE_MOVE,
                (_g, _t, _p) => { if (mouseMove != null) mouseMove(_p); }
            );

            Plugin.MsgProc.RegMsgProc
            (
                (uint)MsgGroupTypes.MGT_INPUT_KEYBOARD, (uint)MsgInputKeyboardTypes.MIK_KEY_DOWN,
                (_g, _t, _p) => { if (keyDown != null) keyDown(_p); }
            );
            Plugin.MsgProc.RegMsgProc
            (
                (uint)MsgGroupTypes.MGT_INPUT_KEYBOARD, (uint)MsgInputKeyboardTypes.MIK_KEY_UP,
                (_g, _t, _p) => { if (keyUp != null) keyUp(_p); }
            );
            Plugin.MsgProc.RegMsgProc
            (
                (uint)MsgGroupTypes.MGT_INPUT_KEYBOARD, (uint)MsgInputKeyboardTypes.MIK_KEY_PRESSED,
                (_g, _t, _p) => { if (keyPress != null) keyPress(_p); }
            );
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            ClearActions();
            int sx = 0;
            int sy = 0;
            bool down = false;
            mouseDown = new Action<List<object>>
            (
                _p =>
                {
                    down = true;
                    sx = (int)_p[0];
                    sy = (int)_p[1];
                    Random rnd = new Random();
                    List<object> _objs = new List<object>();
                    _objs.Add(sx);
                    _objs.Add(sy);
                    _objs.Add(rnd.Next(255));
                    _objs.Add(rnd.Next(255));
                    _objs.Add(rnd.Next(255));
                    Plugin.DoSink((uint)MsgGroupTypesTestCase.MGT_TEST, (uint)MsgTestTypes.MTT_TEST_PRE_ADD_QUAD, _objs);
                }
            );
            mouseMove = new Action<List<object>>
            (
                _p =>
                {
                    if (!down) return;
                    int _ex = (int)_p[0];
                    int _ey = (int)_p[1];
                    List<object> _objs = new List<object>();
                    _objs.Add(_ex);
                    _objs.Add(_ey);
                    Plugin.DoSink((uint)MsgGroupTypesTestCase.MGT_TEST, (uint)MsgTestTypes.MTT_TEST_SET_PRE_ADDED_QUAD_SIZE, _objs);
                }
            );
            mouseUp = new Action<List<object>>
            (
                _p =>
                {
                    Plugin.DoSink((uint)MsgGroupTypesTestCase.MGT_TEST, (uint)MsgTestTypes.MTT_TEST_ADD_QUAD, null);
                    down = false;
                }
            );
        }

        private void btnRemove_Click(object sender, EventArgs e)
        {
            ClearActions();
            int sx = 0;
            int sy = 0;
            mouseDown = new Action<List<object>>
            (
                _p =>
                {
                    sx = (int)_p[0];
                    sy = (int)_p[1];
                    List<object> _objs = new List<object>();
                    _objs.Add(sx);
                    _objs.Add(sy);
                    Plugin.DoSink((uint)MsgGroupTypesTestCase.MGT_TEST, (uint)MsgTestTypes.MTT_TEST_REMOVE_QUAD, _objs);
                }
            );
        }

        private void btnMove_Click(object sender, EventArgs e)
        {
            ClearActions();
            int sx = 0;
            int sy = 0;
            bool down = false;
            mouseDown = new Action<List<object>>
            (
                _p =>
                {
                    down = true;
                    sx = (int)_p[0];
                    sy = (int)_p[1];
                    List<object> _objs = new List<object>();
                    _objs.Add(sx);
                    _objs.Add(sy);
                    Plugin.DoSink((uint)MsgGroupTypesTestCase.MGT_TEST, (uint)MsgTestTypes.MTT_TEST_PRE_MOVE_QUAD, _objs);
                }
            );
            mouseMove = new Action<List<object>>
            (
                _p =>
                {
                    if (!down) return;
                    int _ex = (int)_p[0];
                    int _ey = (int)_p[1];
                    List<object> _objs = new List<object>();
                    _objs.Add(_ex);
                    _objs.Add(_ey);
                    Plugin.DoSink((uint)MsgGroupTypesTestCase.MGT_TEST, (uint)MsgTestTypes.MTT_TEST_MOVING_QUAD, _objs);
                }
            );
            mouseUp = new Action<List<object>>
            (
                _p =>
                {
                    Plugin.DoSink((uint)MsgGroupTypesTestCase.MGT_TEST, (uint)MsgTestTypes.MTT_TEST_MOVE_QUAD, null);
                    down = false;
                }
            );
        }
    }
}
