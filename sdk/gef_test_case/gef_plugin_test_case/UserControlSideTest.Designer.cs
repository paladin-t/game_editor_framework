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
    partial class UserControlSideTest
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
            this.textBoxTest = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // textBoxTest
            // 
            this.textBoxTest.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxTest.Location = new System.Drawing.Point(0, 0);
            this.textBoxTest.Multiline = true;
            this.textBoxTest.Name = "textBoxTest";
            this.textBoxTest.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxTest.Size = new System.Drawing.Size(150, 150);
            this.textBoxTest.TabIndex = 0;
            this.textBoxTest.Text = "Why not try drag a file and drop here.";
            this.textBoxTest.WordWrap = false;
            // 
            // UserControlSideTest
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.textBoxTest);
            this.Name = "UserControlSideTest";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBoxTest;
    }
}
