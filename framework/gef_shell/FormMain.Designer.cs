/**
 * @version: 1.0
 * @author: Wang Renxin,
 *     https://github.com/paladin-t/game_editor_framework
 *     mailto:hellotony521@qq.com
 * @file: This file is a part of GEF, for copyright detail
 *     information, see the LICENSE file.
 */

namespace gef
{
    partial class FormMain
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormMain));
            this.tabControlMain = new System.Windows.Forms.TabControl();
            this.dockContainerMain = new Crom.Controls.Docking.DockContainer();
            this.StatusStripMain = new System.Windows.Forms.StatusStrip();
            this.Label1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.Label2 = new System.Windows.Forms.ToolStripStatusLabel();
            this.Progress1 = new System.Windows.Forms.ToolStripProgressBar();
            this.Label3 = new System.Windows.Forms.ToolStripStatusLabel();
            this.Label4 = new System.Windows.Forms.ToolStripStatusLabel();
            this.Label5 = new System.Windows.Forms.ToolStripStatusLabel();
            this.StatusStripMain.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControlMain
            // 
            this.tabControlMain.Dock = System.Windows.Forms.DockStyle.Top;
            this.tabControlMain.Location = new System.Drawing.Point(0, 0);
            this.tabControlMain.Name = "tabControlMain";
            this.tabControlMain.SelectedIndex = 0;
            this.tabControlMain.Size = new System.Drawing.Size(624, 65);
            this.tabControlMain.TabIndex = 0;
            // 
            // dockContainerMain
            // 
            this.dockContainerMain.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(118)))), ((int)(((byte)(118)))), ((int)(((byte)(118)))));
            this.dockContainerMain.CanMoveByMouseFilledForms = true;
            this.dockContainerMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dockContainerMain.Location = new System.Drawing.Point(0, 65);
            this.dockContainerMain.Name = "dockContainerMain";
            this.dockContainerMain.Size = new System.Drawing.Size(624, 355);
            this.dockContainerMain.TabIndex = 1;
            this.dockContainerMain.TitleBarGradientColor1 = System.Drawing.SystemColors.Control;
            this.dockContainerMain.TitleBarGradientColor2 = System.Drawing.Color.White;
            this.dockContainerMain.TitleBarGradientSelectedColor1 = System.Drawing.Color.DarkGray;
            this.dockContainerMain.TitleBarGradientSelectedColor2 = System.Drawing.Color.White;
            this.dockContainerMain.TitleBarTextColor = System.Drawing.Color.Black;
            // 
            // StatusStripMain
            // 
            this.StatusStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.Label1,
            this.Label2,
            this.Progress1,
            this.Label3,
            this.Label4,
            this.Label5});
            this.StatusStripMain.Location = new System.Drawing.Point(0, 420);
            this.StatusStripMain.Name = "StatusStripMain";
            this.StatusStripMain.Size = new System.Drawing.Size(624, 22);
            this.StatusStripMain.TabIndex = 2;
            this.StatusStripMain.Text = "statusStrip1";
            // 
            // Label1
            // 
            this.Label1.Name = "Label1";
            this.Label1.Size = new System.Drawing.Size(44, 17);
            this.Label1.Text = "Ready";
            // 
            // Label2
            // 
            this.Label2.Name = "Label2";
            this.Label2.Size = new System.Drawing.Size(46, 17);
            this.Label2.Text = "Label2";
            this.Label2.Visible = false;
            // 
            // Progress1
            // 
            this.Progress1.Name = "Progress1";
            this.Progress1.Size = new System.Drawing.Size(100, 16);
            this.Progress1.Visible = false;
            // 
            // Label3
            // 
            this.Label3.Name = "Label3";
            this.Label3.Size = new System.Drawing.Size(46, 17);
            this.Label3.Text = "Label3";
            this.Label3.Visible = false;
            // 
            // Label4
            // 
            this.Label4.Name = "Label4";
            this.Label4.Size = new System.Drawing.Size(46, 17);
            this.Label4.Text = "Label4";
            this.Label4.Visible = false;
            // 
            // Label5
            // 
            this.Label5.Name = "Label5";
            this.Label5.Size = new System.Drawing.Size(46, 17);
            this.Label5.Text = "Label5";
            this.Label5.Visible = false;
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(624, 442);
            this.Controls.Add(this.dockContainerMain);
            this.Controls.Add(this.tabControlMain);
            this.Controls.Add(this.StatusStripMain);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(640, 480);
            this.Name = "FormMain";
            this.Text = "GEF Shell";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.FormMain_FormClosed);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormMain_FormClosing);
            this.StatusStripMain.ResumeLayout(false);
            this.StatusStripMain.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TabControl tabControlMain;
        private Crom.Controls.Docking.DockContainer dockContainerMain;
        private System.Windows.Forms.StatusStrip StatusStripMain;
        private System.Windows.Forms.ToolStripStatusLabel Label1;
        private System.Windows.Forms.ToolStripStatusLabel Label2;
        private System.Windows.Forms.ToolStripProgressBar Progress1;
        private System.Windows.Forms.ToolStripStatusLabel Label3;
        private System.Windows.Forms.ToolStripStatusLabel Label4;
        private System.Windows.Forms.ToolStripStatusLabel Label5;
    }
}

