/**
 * @version: 1.3
 * @author: Wang Renxin,
 *     https://github.com/paladin-t/game_editor_framework
 *     mailto:hellotony521@qq.com
 * @file: This file is a part of GEF, for copyright detail
 *     information, see the LICENSE file.
 */

namespace gef
{
    partial class FormSplash
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormSplash));
            this.pictureBoxBg = new System.Windows.Forms.PictureBox();
            this.panelBack = new System.Windows.Forms.Panel();
            this.labelVersion = new System.Windows.Forms.Label();
            this.labelCustomer = new System.Windows.Forms.Label();
            this.labelStatus = new System.Windows.Forms.Label();
            this.labelCopyright = new System.Windows.Forms.Label();
            this.labelTitle = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxBg)).BeginInit();
            this.panelBack.SuspendLayout();
            this.SuspendLayout();
            // 
            // pictureBoxBg
            // 
            this.pictureBoxBg.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pictureBoxBg.Location = new System.Drawing.Point(0, 0);
            this.pictureBoxBg.Name = "pictureBoxBg";
            this.pictureBoxBg.Size = new System.Drawing.Size(576, 366);
            this.pictureBoxBg.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBoxBg.TabIndex = 5;
            this.pictureBoxBg.TabStop = false;
            // 
            // panelBack
            // 
            this.panelBack.Controls.Add(this.labelVersion);
            this.panelBack.Controls.Add(this.labelCustomer);
            this.panelBack.Controls.Add(this.labelStatus);
            this.panelBack.Controls.Add(this.labelCopyright);
            this.panelBack.Controls.Add(this.labelTitle);
            this.panelBack.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelBack.Location = new System.Drawing.Point(0, 0);
            this.panelBack.Name = "panelBack";
            this.panelBack.Size = new System.Drawing.Size(576, 366);
            this.panelBack.TabIndex = 6;
            // 
            // labelVersion
            // 
            this.labelVersion.Location = new System.Drawing.Point(12, 300);
            this.labelVersion.Name = "labelVersion";
            this.labelVersion.Size = new System.Drawing.Size(552, 21);
            this.labelVersion.TabIndex = 6;
            this.labelVersion.Text = "version";
            this.labelVersion.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // labelCustomer
            // 
            this.labelCustomer.AutoSize = true;
            this.labelCustomer.Location = new System.Drawing.Point(12, 321);
            this.labelCustomer.Name = "labelCustomer";
            this.labelCustomer.Size = new System.Drawing.Size(23, 12);
            this.labelCustomer.TabIndex = 9;
            this.labelCustomer.Text = "   ";
            // 
            // labelStatus
            // 
            this.labelStatus.Location = new System.Drawing.Point(12, 345);
            this.labelStatus.Name = "labelStatus";
            this.labelStatus.Size = new System.Drawing.Size(552, 12);
            this.labelStatus.TabIndex = 8;
            this.labelStatus.Text = "status";
            this.labelStatus.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // labelCopyright
            // 
            this.labelCopyright.AutoSize = true;
            this.labelCopyright.Location = new System.Drawing.Point(12, 333);
            this.labelCopyright.Name = "labelCopyright";
            this.labelCopyright.Size = new System.Drawing.Size(59, 12);
            this.labelCopyright.TabIndex = 7;
            this.labelCopyright.Text = "copyright";
            // 
            // labelTitle
            // 
            this.labelTitle.Font = new System.Drawing.Font("宋体", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.labelTitle.Location = new System.Drawing.Point(12, 99);
            this.labelTitle.Name = "labelTitle";
            this.labelTitle.Size = new System.Drawing.Size(552, 68);
            this.labelTitle.TabIndex = 5;
            this.labelTitle.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.labelTitle.Visible = false;
            // 
            // FormSplash
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(576, 366);
            this.Controls.Add(this.panelBack);
            this.Controls.Add(this.pictureBoxBg);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FormSplash";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "FormSplash";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.FormSplash_FormClosed);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxBg)).EndInit();
            this.panelBack.ResumeLayout(false);
            this.panelBack.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBoxBg;
        private System.Windows.Forms.Panel panelBack;
        private System.Windows.Forms.Label labelVersion;
        private System.Windows.Forms.Label labelCustomer;
        private System.Windows.Forms.Label labelStatus;
        private System.Windows.Forms.Label labelCopyright;
        private System.Windows.Forms.Label labelTitle;
    }
}