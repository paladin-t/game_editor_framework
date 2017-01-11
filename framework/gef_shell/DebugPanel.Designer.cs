/**
 * @version: 1.1
 * @author: Wang Renxin,
 *     https://github.com/paladin-t/game_editor_framework
 *     mailto:hellotony521@qq.com
 * @file: This file is a part of GEF, for copyright detail
 *     information, see the LICENSE file.
 */

namespace gef
{
    partial class DebugPanel
    {
        /// <summary> 
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        #region 组件设计器生成的代码

        /// <summary> 
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(DebugPanel));
            this.txtOutput = new System.Windows.Forms.RichTextBox();
            this.panelCommand = new System.Windows.Forms.Panel();
            this.toolStripCommand = new System.Windows.Forms.ToolStrip();
            this.labelCommand = new System.Windows.Forms.ToolStripLabel();
            this.txtCommand = new System.Windows.Forms.ToolStripComboBox();
            this.btnCommand = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator = new System.Windows.Forms.ToolStripSeparator();
            this.btnAutoWrap = new System.Windows.Forms.ToolStripButton();
            this.btnClear = new System.Windows.Forms.ToolStripButton();
            this.panelOutput = new System.Windows.Forms.Panel();
            this.panelCommand.SuspendLayout();
            this.toolStripCommand.SuspendLayout();
            this.panelOutput.SuspendLayout();
            this.SuspendLayout();
            // 
            // txtOutput
            // 
            this.txtOutput.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtOutput.Location = new System.Drawing.Point(0, 0);
            this.txtOutput.Name = "txtOutput";
            this.txtOutput.ReadOnly = true;
            this.txtOutput.Size = new System.Drawing.Size(732, 179);
            this.txtOutput.TabIndex = 0;
            this.txtOutput.Text = "";
            this.txtOutput.WordWrap = false;
            // 
            // panelCommand
            // 
            this.panelCommand.Controls.Add(this.toolStripCommand);
            this.panelCommand.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panelCommand.Location = new System.Drawing.Point(0, 179);
            this.panelCommand.Name = "panelCommand";
            this.panelCommand.Size = new System.Drawing.Size(732, 30);
            this.panelCommand.TabIndex = 0;
            // 
            // toolStripCommand
            // 
            this.toolStripCommand.Dock = System.Windows.Forms.DockStyle.Fill;
            this.toolStripCommand.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.toolStripCommand.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.labelCommand,
            this.txtCommand,
            this.btnCommand,
            this.toolStripSeparator,
            this.btnAutoWrap,
            this.btnClear});
            this.toolStripCommand.Location = new System.Drawing.Point(0, 0);
            this.toolStripCommand.Name = "toolStripCommand";
            this.toolStripCommand.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.toolStripCommand.Size = new System.Drawing.Size(732, 30);
            this.toolStripCommand.TabIndex = 1;
            this.toolStripCommand.Text = "Command";
            // 
            // labelCommand
            // 
            this.labelCommand.Name = "labelCommand";
            this.labelCommand.Size = new System.Drawing.Size(68, 27);
            this.labelCommand.Text = "Command";
            // 
            // txtCommand
            // 
            this.txtCommand.FlatStyle = System.Windows.Forms.FlatStyle.Standard;
            this.txtCommand.Name = "txtCommand";
            this.txtCommand.Size = new System.Drawing.Size(320, 30);
            this.txtCommand.KeyDown += new System.Windows.Forms.KeyEventHandler(this.txtCommand_KeyDown);
            // 
            // btnCommand
            // 
            this.btnCommand.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnCommand.Image = ((System.Drawing.Image)(resources.GetObject("btnCommand.Image")));
            this.btnCommand.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnCommand.Name = "btnCommand";
            this.btnCommand.Size = new System.Drawing.Size(29, 27);
            this.btnCommand.Text = "&Do";
            this.btnCommand.ToolTipText = "Do it!";
            this.btnCommand.Click += new System.EventHandler(this.btnCommand_Click);
            // 
            // toolStripSeparator
            // 
            this.toolStripSeparator.Name = "toolStripSeparator";
            this.toolStripSeparator.Size = new System.Drawing.Size(6, 30);
            // 
            // btnAutoWrap
            // 
            this.btnAutoWrap.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnAutoWrap.Image = ((System.Drawing.Image)(resources.GetObject("btnAutoWrap.Image")));
            this.btnAutoWrap.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnAutoWrap.Name = "btnAutoWrap";
            this.btnAutoWrap.Size = new System.Drawing.Size(23, 27);
            this.btnAutoWrap.Text = "Auto Wrap";
            this.btnAutoWrap.Click += new System.EventHandler(this.btnAutoWrap_Click);
            // 
            // btnClear
            // 
            this.btnClear.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnClear.Image = ((System.Drawing.Image)(resources.GetObject("btnClear.Image")));
            this.btnClear.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnClear.Name = "btnClear";
            this.btnClear.Size = new System.Drawing.Size(23, 27);
            this.btnClear.Text = "Clear";
            this.btnClear.Click += new System.EventHandler(this.btnClear_Click);
            // 
            // panelOutput
            // 
            this.panelOutput.Controls.Add(this.txtOutput);
            this.panelOutput.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelOutput.Location = new System.Drawing.Point(0, 0);
            this.panelOutput.Name = "panelOutput";
            this.panelOutput.Size = new System.Drawing.Size(732, 179);
            this.panelOutput.TabIndex = 1;
            // 
            // DebugPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panelOutput);
            this.Controls.Add(this.panelCommand);
            this.Name = "DebugPanel";
            this.Size = new System.Drawing.Size(732, 209);
            this.SizeChanged += new System.EventHandler(this.DebugPanel_SizeChanged);
            this.panelCommand.ResumeLayout(false);
            this.panelCommand.PerformLayout();
            this.toolStripCommand.ResumeLayout(false);
            this.toolStripCommand.PerformLayout();
            this.panelOutput.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.RichTextBox txtOutput;
        private System.Windows.Forms.Panel panelCommand;
        private System.Windows.Forms.Panel panelOutput;
        private System.Windows.Forms.ToolStrip toolStripCommand;
        private System.Windows.Forms.ToolStripLabel labelCommand;
        private System.Windows.Forms.ToolStripComboBox txtCommand;
        private System.Windows.Forms.ToolStripButton btnCommand;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator;
        private System.Windows.Forms.ToolStripButton btnAutoWrap;
        private System.Windows.Forms.ToolStripButton btnClear;
    }
}
