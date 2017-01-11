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
    partial class PluginTabPage
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

        #region 组件设计器生成的代码

        /// <summary> 
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PluginTabPage));
            this.PluginToolStrip = new System.Windows.Forms.ToolStrip();
            this.btnHelp = new System.Windows.Forms.ToolStripButton();
            this.btnAbout = new System.Windows.Forms.ToolStripButton();
            this.PluginToolStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // PluginToolStrip
            // 
            this.PluginToolStrip.Dock = System.Windows.Forms.DockStyle.Fill;
            this.PluginToolStrip.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.PluginToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnHelp,
            this.btnAbout});
            this.PluginToolStrip.Location = new System.Drawing.Point(0, 0);
            this.PluginToolStrip.Name = "PluginToolStrip";
            this.PluginToolStrip.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.PluginToolStrip.Size = new System.Drawing.Size(686, 150);
            this.PluginToolStrip.TabIndex = 2;
            // 
            // btnHelp
            // 
            this.btnHelp.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnHelp.Image = ((System.Drawing.Image)(resources.GetObject("btnHelp.Image")));
            this.btnHelp.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnHelp.Name = "btnHelp";
            this.btnHelp.Size = new System.Drawing.Size(23, 147);
            this.btnHelp.Text = "Help";
            this.btnHelp.Click += new System.EventHandler(this.btnHelp_Click);
            // 
            // btnAbout
            // 
            this.btnAbout.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnAbout.Image = ((System.Drawing.Image)(resources.GetObject("btnAbout.Image")));
            this.btnAbout.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnAbout.Name = "btnAbout";
            this.btnAbout.Size = new System.Drawing.Size(23, 147);
            this.btnAbout.Text = "About";
            this.btnAbout.Click += new System.EventHandler(this.btnAbout_Click);
            // 
            // PluginTabPage
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.PluginToolStrip);
            this.Name = "PluginTabPage";
            this.Size = new System.Drawing.Size(686, 150);
            this.PluginToolStrip.ResumeLayout(false);
            this.PluginToolStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        public System.Windows.Forms.ToolStrip PluginToolStrip;
        private System.Windows.Forms.ToolStripButton btnAbout;
        private System.Windows.Forms.ToolStripButton btnHelp;

    }
}
