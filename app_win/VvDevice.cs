using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Management;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;

namespace vv_ui
{
    class VvDevice
    {
        public enum VvErr
        {
            NoErr,
            OpenComErr,
            DeviceNotResponseErr,
            ValueOutRangeErr
        }

        private string serial;
        private SerialPort dev_port = new SerialPort();
        private string name;

        private System.Windows.Forms.RichTextBox logbox;

        const string vid = "0403";
        const string pid = "6010";

        public VvDevice(ref System.Windows.Forms.RichTextBox value)
        {
            logbox = value;

            //initial serial port with default parameter.
            dev_port.WriteTimeout = 1000;
            dev_port.ReadTimeout = 1000;
            dev_port.NewLine = "\r\n";
            //dev_port.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.sport_datareceive);
            //dev_port.PortName = port_name;
            dev_port.BaudRate = 115200;
            dev_port.DataBits = 8;
            dev_port.Parity = System.IO.Ports.Parity.None;
            dev_port.StopBits = System.IO.Ports.StopBits.One;
        }

        public bool SetFreq(double freq, VvUI.FreqUnit_t freq_uint, double power, VvUI.PowerUnit_t power_unit)
        {
            if (name == null)
            {
                Logbox.AppendText("Device not open.\r\n");
                return false;
            }
            else
            {
                if (freq_uint == VvUI.FreqUnit_t.Hz)
                {
                    
                }
                else if (freq_uint == VvUI.FreqUnit_t.KHz)
                {
                    freq *= 1e3;
                }
                else if (freq_uint == VvUI.FreqUnit_t.MHz)
                {
                    freq *= 1e6;
                }
                else if (freq_uint == VvUI.FreqUnit_t.GHz)
                {
                    freq *= 1e9;
                }

                if (power_unit == VvUI.PowerUnit_t.mv)
                {
                    power = 10 * Math.Log(power);
                    return SetFreq(freq, power);
                }
                else if (power_unit == VvUI.PowerUnit_t.dbm)
                {
                    return SetFreq(freq, power);
                }
                else //(power_unit == VvUI.PowerUnit_t.gain)
                {
                    return SetFreq(freq, (int)power);
                }
                
            }
            
        }

        private bool SetFreq (double freq, double pwr)
        {

            return false;
        }

        private bool SetFreq(double freq, int gain)
        {
            String return_string;
            String cmd;
            Logbox.AppendText("Freq = [" + freq + "],gain = [" + gain + "].\r\n");

            if (true != open_port(name))
            {
                return false;
            }

            cmd = "AT+SETFREQ=" + freq + "HZ,GAIN=" + gain + ".\r\n";
            return_string = send_data(cmd, 1000);
            
            if (-1 == return_string.IndexOf("APT,Set freq done."))
            {
                Logbox.AppendText("Err:" + return_string);

                dev_port.Close();
                return false;
            }
            else
            {
                Logbox.AppendText("Set freq done.\r\n");
            }

            dev_port.Close();
            return true;            
        }

        public bool SetBeep()
        {
            return false;
        }

        public bool SetReference()
        {
            return false;
        }
        

        public string Serial
        {
            get
            {
                return serial;
            }

            set
            {
                serial = value;
            }
        }
        

        private bool verify_device(String port_name)
        {
            String return_string;

            if (true != open_port(port_name))
            {
                return false;
            }
            return_string = send_data("AT+VV?\r\n", 1000);
            
            if (-1 == return_string.IndexOf("APT,OK."))
            {
                dev_port.Close();
                return false;
            }

            dev_port.Close();
            return true;
        }

        public bool open_port(string port_name)
        {
            if (dev_port.IsOpen == true)
            {
                dev_port.Close();
            }

            try
            {                
                dev_port.PortName = port_name;
                dev_port.Open();
                if (dev_port.IsOpen == true)
                {

                }
            }
            catch (System.Exception ex)
            {
                
                dev_port.Close();
                return false;

            }
            return true;
        }

        private string send_data (string sdata, int wait_time)
        {

            Byte[] EncodeByte = new byte[128];

            if (sdata.Length != 0)
            {
                EncodeByte = Encoding.GetEncoding("GB2312").GetBytes(sdata);
                dev_port.Write(EncodeByte, 0, EncodeByte.Length);
            }

            Thread.Sleep(wait_time);
            return dev_port.ReadExisting();
        }

        public string send_data(Byte[] sdata, int len, int wait_time)
        {
            if (len != 0)
            {
                dev_port.Write(sdata, 0, len);
            }
            Thread.Sleep(wait_time);
            return dev_port.ReadExisting();
        }

        //detect a vv device by pid and vid.
        public bool ScanDevice ()
        {
            //device_scanned_flg = true;           

            try
            {
                var searcher = new ManagementObjectSearcher("root\\WMI", "SELECT * FROM MSSerial_PortName");

                foreach (ManagementObject queryObj in searcher.Get())
                {
                    if ((queryObj["InstanceName"].ToString().Contains(vid)) && (queryObj["InstanceName"].ToString().Contains(pid)))
                    {
                        String port_name = queryObj["PortName"].ToString();                        
                        
                        //Logbox.AppendText("Found : " + port_name + "\r\n");

                        if (true == verify_device(port_name))
                        {
                            Logbox.AppendText("Found vv device at " + "[" + port_name + "]"+ ".\r\n");
                            name = port_name;
                            return true;
                        }
                        else
                        {
                            Logbox.AppendText("Port [" + port_name + "]" + " open error or not response.\r\n");
                        }
                    }
                    else //not a vv device.
                    {                       
                     
                        Logbox.AppendText("Not found a device.");
                    }
                }

            }
            catch (ManagementException ex)
            {                
                Logbox.AppendText("Scan device error, disable UAC or run as administrator.\r\n");
            }

            return false;
        }
        

        public RichTextBox Logbox
        {
            get
            {
                return logbox;
            }

            set
            {
                logbox = value;
            }
        }

        public bool OpenDevice()
        {
            return false;
        }

        public bool CloseDevice()
        {
            return false;
        }


    }
}
