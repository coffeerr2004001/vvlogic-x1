namespace vv_ui
{
    partial class Form1
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
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.lb_status = new System.Windows.Forms.Label();
            this.button7 = new System.Windows.Forms.Button();
            this.button6 = new System.Windows.Forms.Button();
            this.button5 = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.btn_output = new System.Windows.Forms.Button();
            this.btn_freq_unit = new System.Windows.Forms.Button();
            this.btn_pwr_unit = new System.Windows.Forms.Button();
            this.tb_freq = new System.Windows.Forms.TextBox();
            this.tb_pwr = new System.Windows.Forms.TextBox();
            this.rtb_log = new System.Windows.Forms.RichTextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.btn_pwr_gain = new System.Windows.Forms.Button();
            this.btn_beep = new System.Windows.Forms.Button();
            this.btn_ref_src = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // lb_status
            // 
            this.lb_status.AutoSize = true;
            this.lb_status.BackColor = System.Drawing.Color.MediumAquamarine;
            this.lb_status.ForeColor = System.Drawing.SystemColors.Control;
            this.lb_status.Location = new System.Drawing.Point(346, 568);
            this.lb_status.Name = "lb_status";
            this.lb_status.Size = new System.Drawing.Size(108, 24);
            this.lb_status.TabIndex = 11;
            this.lb_status.Text = "Disconnected";
            // 
            // button7
            // 
            this.button7.Enabled = false;
            this.button7.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button7.Font = new System.Drawing.Font("Arial Narrow", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button7.ForeColor = System.Drawing.SystemColors.Control;
            this.button7.Location = new System.Drawing.Point(197, 12);
            this.button7.Name = "button7";
            this.button7.Size = new System.Drawing.Size(80, 28);
            this.button7.TabIndex = 10;
            this.button7.UseVisualStyleBackColor = true;
            // 
            // button6
            // 
            this.button6.Enabled = false;
            this.button6.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button6.Font = new System.Drawing.Font("Arial Narrow", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button6.ForeColor = System.Drawing.SystemColors.Control;
            this.button6.Location = new System.Drawing.Point(111, 12);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(80, 28);
            this.button6.TabIndex = 10;
            this.button6.UseVisualStyleBackColor = true;
            // 
            // button5
            // 
            this.button5.Enabled = false;
            this.button5.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button5.Font = new System.Drawing.Font("Arial Narrow", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button5.ForeColor = System.Drawing.SystemColors.Control;
            this.button5.Location = new System.Drawing.Point(25, 12);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(80, 28);
            this.button5.TabIndex = 9;
            this.button5.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.ForeColor = System.Drawing.SystemColors.Control;
            this.label2.Location = new System.Drawing.Point(12, 80);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(77, 17);
            this.label2.TabIndex = 2;
            this.label2.Text = "Frequency";
            // 
            // btn_output
            // 
            this.btn_output.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btn_output.Font = new System.Drawing.Font("Arial Narrow", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_output.ForeColor = System.Drawing.SystemColors.Control;
            this.btn_output.Location = new System.Drawing.Point(11, 267);
            this.btn_output.Name = "btn_output";
            this.btn_output.Size = new System.Drawing.Size(443, 46);
            this.btn_output.TabIndex = 1;
            this.btn_output.Text = "Output";
            this.btn_output.UseVisualStyleBackColor = true;
            this.btn_output.Click += new System.EventHandler(this.btn_output_Click);
            // 
            // btn_freq_unit
            // 
            this.btn_freq_unit.BackColor = System.Drawing.Color.MediumAquamarine;
            this.btn_freq_unit.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btn_freq_unit.ForeColor = System.Drawing.SystemColors.Control;
            this.btn_freq_unit.Location = new System.Drawing.Point(350, 107);
            this.btn_freq_unit.Name = "btn_freq_unit";
            this.btn_freq_unit.Size = new System.Drawing.Size(100, 51);
            this.btn_freq_unit.TabIndex = 7;
            this.btn_freq_unit.Text = "MHz";
            this.btn_freq_unit.UseVisualStyleBackColor = false;
            this.btn_freq_unit.Click += new System.EventHandler(this.btn_freq_unit_Click);
            // 
            // btn_pwr_unit
            // 
            this.btn_pwr_unit.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btn_pwr_unit.ForeColor = System.Drawing.SystemColors.Control;
            this.btn_pwr_unit.Location = new System.Drawing.Point(350, 202);
            this.btn_pwr_unit.Name = "btn_pwr_unit";
            this.btn_pwr_unit.Size = new System.Drawing.Size(100, 51);
            this.btn_pwr_unit.TabIndex = 8;
            this.btn_pwr_unit.Text = "dbm";
            this.btn_pwr_unit.UseVisualStyleBackColor = true;
            this.btn_pwr_unit.Click += new System.EventHandler(this.btn_pwr_unit_Click);
            // 
            // tb_freq
            // 
            this.tb_freq.BackColor = System.Drawing.SystemColors.Control;
            this.tb_freq.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tb_freq.Font = new System.Drawing.Font("Agency FB", 25.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tb_freq.ForeColor = System.Drawing.Color.MediumSeaGreen;
            this.tb_freq.Location = new System.Drawing.Point(11, 107);
            this.tb_freq.Name = "tb_freq";
            this.tb_freq.Size = new System.Drawing.Size(333, 52);
            this.tb_freq.TabIndex = 2;
            this.tb_freq.Text = "1301.2";
            this.tb_freq.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.tb_freq.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tb_freq_KeyPress);
            // 
            // tb_pwr
            // 
            this.tb_pwr.BackColor = System.Drawing.SystemColors.Control;
            this.tb_pwr.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tb_pwr.Font = new System.Drawing.Font("Agency FB", 25.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tb_pwr.ForeColor = System.Drawing.Color.MediumSeaGreen;
            this.tb_pwr.Location = new System.Drawing.Point(11, 202);
            this.tb_pwr.Name = "tb_pwr";
            this.tb_pwr.Size = new System.Drawing.Size(333, 52);
            this.tb_pwr.TabIndex = 3;
            this.tb_pwr.Text = "-24.5";
            this.tb_pwr.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.tb_pwr.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tb_pwr_KeyPress);
            // 
            // rtb_log
            // 
            this.rtb_log.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.rtb_log.Font = new System.Drawing.Font("Consolas", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rtb_log.ForeColor = System.Drawing.Color.MediumAquamarine;
            this.rtb_log.Location = new System.Drawing.Point(11, 335);
            this.rtb_log.Name = "rtb_log";
            this.rtb_log.Size = new System.Drawing.Size(443, 230);
            this.rtb_log.TabIndex = 12;
            this.rtb_log.Text = "";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.BackColor = System.Drawing.Color.MediumAquamarine;
            this.label4.ForeColor = System.Drawing.SystemColors.Control;
            this.label4.Location = new System.Drawing.Point(11, 568);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(51, 24);
            this.label4.TabIndex = 13;
            this.label4.Text = " 2016";
            // 
            // btn_pwr_gain
            // 
            this.btn_pwr_gain.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btn_pwr_gain.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_pwr_gain.ForeColor = System.Drawing.SystemColors.Control;
            this.btn_pwr_gain.Location = new System.Drawing.Point(11, 168);
            this.btn_pwr_gain.Name = "btn_pwr_gain";
            this.btn_pwr_gain.Size = new System.Drawing.Size(78, 28);
            this.btn_pwr_gain.TabIndex = 14;
            this.btn_pwr_gain.Text = "Power";
            this.btn_pwr_gain.UseVisualStyleBackColor = true;
            // 
            // btn_beep
            // 
            this.btn_beep.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btn_beep.Font = new System.Drawing.Font("Arial Narrow", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_beep.ForeColor = System.Drawing.SystemColors.Control;
            this.btn_beep.Location = new System.Drawing.Point(283, 12);
            this.btn_beep.Name = "btn_beep";
            this.btn_beep.Size = new System.Drawing.Size(80, 28);
            this.btn_beep.TabIndex = 15;
            this.btn_beep.Text = "Beep on";
            this.btn_beep.UseVisualStyleBackColor = true;
            this.btn_beep.Click += new System.EventHandler(this.btn_beep_Click);
            // 
            // btn_ref_src
            // 
            this.btn_ref_src.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btn_ref_src.Font = new System.Drawing.Font("Arial Narrow", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_ref_src.ForeColor = System.Drawing.SystemColors.Control;
            this.btn_ref_src.Location = new System.Drawing.Point(369, 12);
            this.btn_ref_src.Name = "btn_ref_src";
            this.btn_ref_src.Size = new System.Drawing.Size(80, 28);
            this.btn_ref_src.TabIndex = 16;
            this.btn_ref_src.Text = "ref Internal";
            this.btn_ref_src.UseVisualStyleBackColor = true;
            this.btn_ref_src.Click += new System.EventHandler(this.btn_ref_src_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(10F, 24F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.MediumAquamarine;
            this.ClientSize = new System.Drawing.Size(462, 593);
            this.Controls.Add(this.btn_ref_src);
            this.Controls.Add(this.btn_beep);
            this.Controls.Add(this.btn_pwr_gain);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.rtb_log);
            this.Controls.Add(this.lb_status);
            this.Controls.Add(this.button7);
            this.Controls.Add(this.tb_pwr);
            this.Controls.Add(this.button6);
            this.Controls.Add(this.btn_output);
            this.Controls.Add(this.button5);
            this.Controls.Add(this.btn_pwr_unit);
            this.Controls.Add(this.tb_freq);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.btn_freq_unit);
            this.Font = new System.Drawing.Font("Arial Narrow", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "vvlogic X1";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Label lb_status;
        private System.Windows.Forms.Button button7;
        private System.Windows.Forms.Button button6;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btn_output;
        private System.Windows.Forms.Button btn_freq_unit;
        private System.Windows.Forms.Button btn_pwr_unit;
        private System.Windows.Forms.TextBox tb_freq;
        private System.Windows.Forms.TextBox tb_pwr;
        private System.Windows.Forms.RichTextBox rtb_log;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button btn_pwr_gain;
        private System.Windows.Forms.Button btn_beep;
        private System.Windows.Forms.Button btn_ref_src;
    }
}

