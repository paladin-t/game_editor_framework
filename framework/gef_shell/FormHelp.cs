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
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace gef
{
    internal partial class FormHelp : Form
    {
        private static FormHelp self = null;

        private FormHelp()
        {
            InitializeComponent();

            FormClosed += (_sender, _e) =>
            {
                self = null;
            };
        }
        
        private void FormHelp_Load(object sender, EventArgs e)
        {
            webHelp.Url = new Uri(WrapperUtil.ApplicationDirectory + "\\gef_help.htm");
        }

        public static FormHelp GetInstance()
        {
            if (self == null)
            {
                self = new FormHelp();
            }

            return self;
        }

        public void Show(FormMain f)
        {
            if (!Visible)
                base.Show(f);
        }
    }
}
