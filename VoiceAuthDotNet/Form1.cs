using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Threading;

namespace VoiceAuthDotNet
{
    public partial class Form1 : Form
    {
        [DllImport("Voice_auth_x86.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool init_model([MarshalAs(UnmanagedType.LPStr)]string model_folder);
        [DllImport("Voice_auth_x86.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool destroy_model();
        [DllImport("Voice_auth_x86.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool register_new_user([MarshalAs(UnmanagedType.LPStr)]string speaker_id, float rec_len);
        [DllImport("Voice_auth_x86.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool delete_user([MarshalAs(UnmanagedType.LPStr)]string speaker_id);
        [DllImport("Voice_auth_x86.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern float get_auth_result_mic_len([MarshalAs(UnmanagedType.LPStr)]string speaker_id, float rec_len);
        [DllImport("Voice_auth_x86.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern string get_ident_result_w(float rec_len, out float top_score);
        [DllImport("Voice_auth_x86.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        //[return: MarshalAs(UnmanagedType.LPStr)]
        public static extern string get_speakers_list_w();

        Thread t_start;
        float rec_len_auth = 5;  
        float rec_len_register = 15; //время записи аудио с микрофона для регистрации пользователя, в секундах.
        //int timer_tics = 0;
        public Form1()
        {
            InitializeComponent();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }
        void RegisterSpeaker()
        {
            string user_id = textBox1.Text;
            register_new_user(user_id, rec_len_register);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            progressBar1.Value = 0;
            progressBar1.Maximum = (int)rec_len_register*1000;
            timer1.Start();
            t_start = new Thread(new ThreadStart(RegisterSpeaker));
            t_start.IsBackground = true;
            t_start.Start();
            MessageBox.Show("Произнесите любые фразы вслух. Регистрация голоса пользователя уже началась. Длительность регистрации "+ rec_len_register.ToString()+" секунд");
            t_start.Join();
            timer1.Stop();
            //refresh Speakers List
            Add_List(get_speakers_list_w());
        }

        private void Add_List(string s)
        { 
            listBox1.Items.Clear();
            if (s.Length > 0)
            {
                while (s.IndexOf('\n') > 0)
                {
                    listBox1.Items.Add(s.Substring(0, s.IndexOf('\n')));
                    s = s.Substring(s.IndexOf('\n')+1);
                }
                listBox1.Items.Add(s);
            }
            
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            try
            {
                init_model("spk-model");
                string s = get_speakers_list_w();

                Add_List(s);
                textBox1.Text = listBox1.Items[0].ToString();
            }
            catch (Exception err)
            {
                MessageBox.Show("Initialization error " + err.Message);
                this.Close();
            }
            
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            destroy_model();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            string s = get_speakers_list_w();
            Add_List(s);
        }

        void AuthSpeaker()
        {
            string speaker_id = textBox1.Text;
            float score = get_auth_result_mic_len(speaker_id, rec_len_auth);
            string output = "Score: " + score.ToString() + "; ";
             
            if (score > -1.02)
            {
                output += "Аутентификация пройдена. ";
                if (score < 9)
                    output += "Низкая уверенность.";
            }
            else
            {
                output += "Аутентификация не пройдена. ";
                if (score > -6)
                    output += "Низкая уверенность.";
            }
            BeginInvoke(new Action(() => { textBox2.Text = output; }));
             
        }

        private void button1_Click(object sender, EventArgs e)
        {
            progressBar1.Value = 0;
            progressBar1.Maximum = (int)rec_len_auth * 1000;
            timer1.Start(); 
            t_start = new Thread(new ThreadStart(AuthSpeaker));
            t_start.IsBackground = true;
            t_start.Start();
            MessageBox.Show("Произнесите любую фразу вслух. Аутентификация уже началась. Длительность записи "+ rec_len_auth+ " секунд");
            t_start.Join();
            timer1.Stop();

        }
        void IdentSpeaker()
        {
            float top_score = 0;
            string output = "Идентифицирован диктор:" + get_ident_result_w(rec_len_auth, out top_score) + " Score: " + top_score.ToString();
            BeginInvoke(new Action(() => { textBox2.Text = output; }));

        }

        private void button2_Click(object sender, EventArgs e)
        {
            progressBar1.Value = 0;
            progressBar1.Maximum = (int)rec_len_auth * 1000;
            timer1.Start();
            t_start = new Thread(new ThreadStart(IdentSpeaker));
            t_start.IsBackground = true;
            t_start.Start();
            MessageBox.Show("Произнесите любую фразу вслух. Идентификация уже началась. Длительность записи " + rec_len_auth + " секунд");
            t_start.Join();
            timer1.Stop();
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listBox1.SelectedItem != null)
                textBox1.Text = listBox1.SelectedItem.ToString();
            else
            {
                listBox1.SelectedIndex = 0;
                textBox1.Text = listBox1.SelectedItem.ToString();
            }
        }

        private void button5_Click(object sender, EventArgs e)
        {
            string user_id = textBox1.Text;
            delete_user(user_id);
            //refresh Speakers List
            Add_List(get_speakers_list_w());
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            progressBar1.PerformStep();
        }
    }
}
