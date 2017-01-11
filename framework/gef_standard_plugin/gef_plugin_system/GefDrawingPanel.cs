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
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace gef
{
    public class GefDrawingPanel : Panel
    {
        public new event KeyEventHandler KeyDown;

        public new event KeyEventHandler KeyUp;

        public event KeyPressEventHandler KeyPressed;

        protected override void OnKeyDown(KeyEventArgs e)
        {
            base.OnKeyDown(e);

            if (KeyDown != null)
                KeyDown(this, e);
        }

        protected override void OnKeyUp(KeyEventArgs e)
        {
            base.OnKeyUp(e);

            if (KeyUp != null)
                KeyUp(this, e);
        }

        protected override void OnKeyPress(KeyPressEventArgs e)
        {
            base.OnKeyPress(e);

            if (KeyPressed != null)
                KeyPressed(this, e);
        }
    }
}
