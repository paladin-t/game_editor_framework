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
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using System.IO;

namespace gef
{
    internal partial class FormSplash : Form
    {
        public string Title
        {
            get { return labelTitle.Text; }
            set
            {
                labelTitle.Text = value;
                Application.DoEvents();
            }
        }

        public string Status
        {
            get { return labelStatus.Text; }
            set
            {
                labelStatus.Text = value;
                Application.DoEvents();
            }
        }

        public string CustomerCopyright
        {
            get { return labelCustomer.Text; }
            set
            {
                labelCustomer.Text = value;
                Application.DoEvents();
            }
        }

        private static FormSplash self = null;

        private FormSplash()
        {
            InitializeComponent();

            labelVersion.Text = "GEF Version: " + Assembly.GetExecutingAssembly().GetName().Version.ToString();
            object[] attributes = Assembly.GetExecutingAssembly().GetCustomAttributes(typeof(AssemblyCopyrightAttribute), false);
            labelCopyright.Text = ((AssemblyCopyrightAttribute)attributes[0]).Copyright;

            labelTitle.BackColor = Color.Transparent;
            labelVersion.BackColor = Color.Transparent;
            labelCopyright.BackColor = Color.Transparent;
            labelStatus.BackColor = Color.Transparent;
            labelCustomer.BackColor = Color.Transparent;
            panelBack.BackColor = Color.Transparent;
            panelBack.Parent = pictureBoxBg;

            try
            {
                string imgPath = Directory.GetParent(Application.ExecutablePath).FullName + "\\gef_splash.png";
                pictureBoxBg.Image = Image.FromFile(imgPath);
            }
            catch
            {
            }
        }

        public static FormSplash GetInstance()
        {
            if (self == null)
                self = new FormSplash();

            return self;
        }

        private void FormSplash_FormClosed(object sender, FormClosedEventArgs e)
        {
            self = null;
        }
    }
}
