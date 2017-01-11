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
    partial class UserControlTest
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
            this.tableLayoutPanelTest = new System.Windows.Forms.TableLayoutPanel();
            this.btnAdd = new System.Windows.Forms.Button();
            this.btnRemove = new System.Windows.Forms.Button();
            this.btnMove = new System.Windows.Forms.Button();
            this.tableLayoutPanelTest.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanelTest
            // 
            this.tableLayoutPanelTest.ColumnCount = 1;
            this.tableLayoutPanelTest.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanelTest.Controls.Add(this.btnMove, 0, 2);
            this.tableLayoutPanelTest.Controls.Add(this.btnRemove, 0, 1);
            this.tableLayoutPanelTest.Controls.Add(this.btnAdd, 0, 0);
            this.tableLayoutPanelTest.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanelTest.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanelTest.Name = "tableLayoutPanelTest";
            this.tableLayoutPanelTest.RowCount = 4;
            this.tableLayoutPanelTest.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanelTest.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanelTest.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanelTest.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanelTest.Size = new System.Drawing.Size(278, 300);
            this.tableLayoutPanelTest.TabIndex = 0;
            // 
            // btnAdd
            // 
            this.btnAdd.Dock = System.Windows.Forms.DockStyle.Top;
            this.btnAdd.Location = new System.Drawing.Point(3, 3);
            this.btnAdd.Name = "btnAdd";
            this.btnAdd.Size = new System.Drawing.Size(272, 23);
            this.btnAdd.TabIndex = 0;
            this.btnAdd.Text = "Add";
            this.btnAdd.UseVisualStyleBackColor = true;
            this.btnAdd.Click += new System.EventHandler(this.btnAdd_Click);
            // 
            // btnRemove
            // 
            this.btnRemove.Dock = System.Windows.Forms.DockStyle.Top;
            this.btnRemove.Location = new System.Drawing.Point(3, 32);
            this.btnRemove.Name = "btnRemove";
            this.btnRemove.Size = new System.Drawing.Size(272, 23);
            this.btnRemove.TabIndex = 1;
            this.btnRemove.Text = "Remove";
            this.btnRemove.UseVisualStyleBackColor = true;
            this.btnRemove.Click += new System.EventHandler(this.btnRemove_Click);
            // 
            // btnMove
            // 
            this.btnMove.Dock = System.Windows.Forms.DockStyle.Top;
            this.btnMove.Location = new System.Drawing.Point(3, 61);
            this.btnMove.Name = "btnMove";
            this.btnMove.Size = new System.Drawing.Size(272, 23);
            this.btnMove.TabIndex = 2;
            this.btnMove.Text = "Move";
            this.btnMove.UseVisualStyleBackColor = true;
            this.btnMove.Click += new System.EventHandler(this.btnMove_Click);
            // 
            // UserControlTest
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tableLayoutPanelTest);
            this.Name = "UserControlTest";
            this.Size = new System.Drawing.Size(278, 300);
            this.tableLayoutPanelTest.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanelTest;
        private System.Windows.Forms.Button btnRemove;
        private System.Windows.Forms.Button btnAdd;
        private System.Windows.Forms.Button btnMove;



    }
}
