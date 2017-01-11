/**
 * @version: 1.0
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
using System.Text;
using System.Windows.Forms;

namespace gef
{
    public partial class FormDockable : Form
    {
        public FormDockable()
        {
            InitializeComponent();
            FormBorderStyle = FormBorderStyle.SizableToolWindow;
            TopLevel = false;
        }

        private void ForEachCtrl(Control c, Action<Control> action)
        {
            c.AllowDrop = true;
            action(c);
            foreach (Control sub in c.Controls)
                ForEachCtrl(sub, action);
        }

        public void ForEachCtrl(Action<Control> action)
        {
            ForEachCtrl(this, action);
        }
    }
}
