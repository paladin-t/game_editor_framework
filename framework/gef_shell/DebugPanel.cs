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
    public partial class DebugPanel : UserControl
    {
        private static DebugPanel self = null;

        public Guid Guid
        {
            get { return new Guid("81C7A67A-B0E8-473d-9DC2-227B144404EA"); }
        }

        private bool echoEnabled = true;
        public bool EchoEnabled
        {
            get { return echoEnabled; }
            set { echoEnabled = value; }
        }

        public DebugPanel()
        {
            InitializeComponent();

            txtOutput.VisibleChanged += (_sender, _e) =>
            {
                txtOutput.ScrollToCaret();
            };

            List<string> intes = ScriptManager.GetInstance().GetInterfaces();
            foreach (string inte in intes)
            {
                txtCommand.Items.Add(inte);
            }
            txtCommand.AutoCompleteMode = AutoCompleteMode.SuggestAppend;
            txtCommand.AutoCompleteSource = AutoCompleteSource.ListItems;

            self = this;
        }

        protected override void Dispose(bool disposing)
        {
            self = null;

            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        public static DebugPanel GetInstance()
        {
            return self;
        }

        private void DebugPanel_SizeChanged(object sender, EventArgs e)
        {
            txtOutput.Refresh();
        }

        private void btnAutoWrap_Click(object sender, EventArgs e)
        {
            btnAutoWrap.Checked = !btnAutoWrap.Checked;
            txtOutput.WordWrap = btnAutoWrap.Checked;
            txtOutput.Select(txtOutput.Text.Length, 0);
            txtOutput.ScrollToCaret();
        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            txtOutput.Clear();
        }

        private void txtCommand_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyValue == 13)
                btnCommand_Click(sender, null);
        }

        private void btnCommand_Click(object sender, EventArgs e)
        {
            txtCommand.Text.Trim();
            if (string.IsNullOrEmpty(txtCommand.Text))
                return;
            try
            {
                ScriptManager.GetInstance().DoString(txtCommand.Text);
            }
            catch (Exception ex)
            {
                string msg = ShellUtil.GetExceptionMsg(ex);
                WriteLine(Pens.Red.Color, "Command error:");
                WriteLine(Pens.Red.Color, "    Command: " + txtCommand.Text);
                WriteLine(Pens.Red.Color, "    Details: " + msg);
            }
            try
            {
                if (!txtCommand.Items.Contains(txtCommand.Text))
                    txtCommand.Items.Add(txtCommand.Text);
                txtCommand.Text = string.Empty;
            }
            catch { }
        }

        public void Clear()
        {
            txtOutput.Clear();
        }

        public void Write(Color col, string text)
        {
            txtOutput.Select(txtOutput.Text.Length, 0);
            txtOutput.SelectionColor = col;
            txtOutput.AppendText(text);
            txtOutput.Select(txtOutput.Text.Length, 0);
            txtOutput.ScrollToCaret();
        }

        public void Write(string text)
        {
            Write(txtOutput.ForeColor, text);
        }

        public void WriteLine(Color col, string text)
        {
            Write(col, text + "\n");
        }

        public void WriteLine(string text)
        {
            WriteLine(txtOutput.ForeColor, text);
        }

        public void Echo(params object[] p)
        {
            if (EchoEnabled)
            {
                string t = string.Empty;
                foreach (object o in p)
                {
                    t += o.ToString();
                }
                WriteLine(Pens.Black.Color, "> " + t);
            }
        }
    }
}
