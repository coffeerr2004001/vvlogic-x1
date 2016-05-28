using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace vv_ui
{
    public partial class Form1 : Form
    {
        private VvUI ui0 = new VvUI();
        VvDevice vv0;

        public Form1()
        {
            InitializeComponent();
        }        

        private void Form1_Load(object sender, EventArgs e)
        {
            this.tb_freq.AutoSize = false;
            this.tb_pwr.AutoSize = false;
            vv0 = new VvDevice(ref rtb_log);
            //
            //vv0.Logbox = rtb_log;
            vv0.ScanDevice();
        }

        private delegate void AddlogEventHandler(String Message);
        public void AddLog(String Message)
        {
            if (this.rtb_log.InvokeRequired)
            {
                AddlogEventHandler evt = new AddlogEventHandler(AddLog);
                this.Invoke(evt, new object[] { Message });
            }
            else
            {
                if (Message == null)
                {
                    this.rtb_log.AppendText("Device not connected.");
                }
                else
                {
                    this.rtb_log.AppendText(":" + Message);
                }
            }
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            System.Environment.Exit(0);
        }

        private void btn_freq_unit_Click(object sender, EventArgs e)
        {
            btn_freq_unit.Text = ui0.next(typeof(VvUI.FreqUnit_t));
        }

        private void btn_pwr_unit_Click(object sender, EventArgs e)
        {
            btn_pwr_unit.Text =  ui0.next(typeof(VvUI.PowerUnit_t));
            if (ui0.PwrUint != VvUI.PowerUnit_t.dbm)
            {
                if (tb_pwr.Text.Substring(0, 1) == "-")
                {
                    tb_pwr.Text = tb_pwr.Text.Substring(1, tb_pwr.Text.Length - 1);
                }
            }
        }

        private void btn_beep_Click(object sender, EventArgs e)
        {
            btn_beep.Text = ui0.next(typeof(VvUI.BeepMode_t));
        }

        private void btn_ref_src_Click(object sender, EventArgs e)
        {
            btn_ref_src.Text = ui0.next(typeof(VvUI.RefMode_t));
        }

        private void btn_output_Click(object sender, EventArgs e)
        {
            Double freq = Convert.ToDouble(tb_freq.Text);
            Double power = Convert.ToDouble(tb_pwr.Text);

            rtb_log.AppendText("freq = [" +  freq + btn_freq_unit.Text +"]["+ power + btn_pwr_unit.Text+"].\r\n");
            vv0.SetFreq(freq,ui0.FreqUnit,power,ui0.PwrUint);
                
        }

        private string remove_string_last_char(string str_in)
        {
            return str_in.Remove(str_in.Length - 1, 1);
        }
        
        private void tb_freq_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == '.')
            {
                if (tb_freq.Text.Length == 0)
                {
                    tb_freq.Text = "0";
                    tb_freq.SelectionStart = int.MaxValue;
                }
                if(tb_freq.Text.IndexOf(".") != -1)
                {
                    e.KeyChar = (char)0;
                }
            }
            else if (e.KeyChar == (char)8) //backspace
            {
            }
            else if( (e.KeyChar < '0') || (e.KeyChar > '9'))
            {
                e.KeyChar = (char)0;

            }
            
        }

        private void tb_pwr_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == '.')
            {
                if (tb_pwr.Text.Length == 0)
                {
                    tb_pwr.Text = "0";
                    tb_pwr.SelectionStart = int.MaxValue;
                }
                if (tb_pwr.Text.IndexOf(".") != -1)
                {
                    e.KeyChar = (char)0;
                }
            }
            else if (e.KeyChar == (char)8) //backspace
            {

            }
            else if (e.KeyChar == '-')
            {
                if (tb_pwr.Text.Length == 0)
                {

                }
                else
                {
                    e.KeyChar = (char)0;
                }
            }
            else if ((e.KeyChar < '0') || (e.KeyChar > '9'))
            {
                e.KeyChar = (char)0;

            }
        }
    }
}
