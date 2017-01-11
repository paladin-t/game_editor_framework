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
    partial class Canvas
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
            this.SuspendLayout();
            // 
            // Canvas
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Name = "Canvas";
            this.Size = new System.Drawing.Size(349, 323);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Canvas_MouseMove);
            this.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.Canvas_MouseDoubleClick);
            this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.Canvas_KeyUp);
            this.MouseClick += new System.Windows.Forms.MouseEventHandler(this.Canvas_MouseClick);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.Canvas_MouseDown);
            this.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Canvas_KeyPress);
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.Canvas_MouseUp);
            this.SizeChanged += new System.EventHandler(this.Canvas_SizeChanged);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Canvas_KeyDown);
            this.ResumeLayout(false);

        }

        #endregion
    }
}
