using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace vv_ui
{
    class VvUI
    {
        private double frequency;
        private double power;
        private uint gain;

        private BeepMode_t beep_mode;
        private RefMode_t ref_mode;

        private FreqUnit_t freq_unit;
        private PowerUnit_t pwr_uint;

        public enum BeepMode_t
        {
            BeepOnLock,
            BeepOff
        };
        public enum RefMode_t
        {
            Internal,
            InternalAndOut,
            External
        }
        public enum FreqUnit_t
        {
            GHz,
            MHz,
            KHz,
            Hz
        }
        public enum PowerUnit_t
        {
            dbm,
            mv,
            gain
        }

        public enum OutputMode_t
        {
            output,
            mute
        }

        public enum PowerParaMode_t
        {
            power,
            gain
        }

        public double Frequency
        {
            get
            {
                return frequency;
            }
            set
            {
                frequency = value;
            }
        }
        
        public double Power
        {
            get
            {
                return power;
            }

            set
            {
                power = value;
            }
        }
        public RefMode_t RefMode
        {
            get
            {
                return ref_mode;
            }

            set
            {
                ref_mode = value;
            }
        }
        public BeepMode_t Beep_mode
        {
            get
            {
                return beep_mode;
            }

            set
            {
                beep_mode = value;
            }
        }

        
        public FreqUnit_t FreqUnit
        {
            get
            {
                return freq_unit;
            }

            set
            {
                freq_unit = value;
            }
            
        }

        public VvUI()
        {
            beep_mode = BeepMode_t.BeepOnLock;
            ref_mode = RefMode_t.Internal;
            freq_unit = FreqUnit_t.MHz;
            pwr_uint = PowerUnit_t.dbm;
        }



        public string next (Type value)
        {
            if (value == typeof(FreqUnit_t))
            {
                //Console.WriteLine("FreqUnit_t");
                if (freq_unit == FreqUnit_t.Hz)
                {
                    freq_unit = FreqUnit_t.KHz;
                }
                else if (freq_unit == FreqUnit_t.KHz)
                {
                    freq_unit = FreqUnit_t.MHz;
                }
                else if (freq_unit == FreqUnit_t.MHz)
                {
                    freq_unit = FreqUnit_t.GHz;
                }
                else if (freq_unit == FreqUnit_t.GHz)
                {
                    freq_unit = FreqUnit_t.Hz;
                }
                return freq_unit.ToString();

            }
            else if (value == typeof(BeepMode_t))
            {
                //Console.WriteLine("BeepMode_t");
                if (beep_mode == BeepMode_t.BeepOff)
                {
                    beep_mode = BeepMode_t.BeepOnLock;
                    return "Beep on";
                }
                else if (beep_mode == BeepMode_t.BeepOnLock)
                {
                    beep_mode = BeepMode_t.BeepOff;
                    return "Beep off";
                }
                
            }
            else if (value == typeof(RefMode_t))
            {
                //Console.WriteLine("RefMode_t");
                if (ref_mode == RefMode_t.External)
                {
                    ref_mode = RefMode_t.Internal;
                    return "ref Internal";
                }
                else if (ref_mode == RefMode_t.Internal)
                {
                    ref_mode = RefMode_t.InternalAndOut;
                    return "ref Out";
                }
                else if (ref_mode == RefMode_t.InternalAndOut)
                {
                    ref_mode = RefMode_t.External;
                    return "ref External";
                }
            }
            else if (value == typeof(PowerUnit_t))
            {
                //Console.WriteLine("PowerUnit_t");
                if (pwr_uint == PowerUnit_t.dbm)
                {
                    pwr_uint = PowerUnit_t.mv;
                    return "mV(rms)";
                }
                else if (pwr_uint == PowerUnit_t.mv)
                {
                    pwr_uint = PowerUnit_t.gain;
                    return "Gain";
                }
                else if (pwr_uint == PowerUnit_t.gain)
                {
                    pwr_uint = PowerUnit_t.dbm;
                    return "dbm";
                }
            }

            return "err";
        }
        
        


        public PowerUnit_t PwrUint
        {
            get
            {
                return pwr_uint;
            }

            set
            {
                pwr_uint = value;
            }
        }

       
    }
}
