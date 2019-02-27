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
using Rapper;

namespace WFEditor
{
    public partial class Form1 : Form
    {
        public CoreRapper coreRapper = new CoreRapper();
        private void Run(object sender, EventArgs e)
        {
            while(this.Created == true)
            {
                coreRapper.Logic();
                Application.DoEvents();
            }
        
        }

        public Form1()
        {
            InitializeComponent();

            IntPtr window_handle = pictureBox1.Handle;
            coreRapper.Init(window_handle);

            //Run함수 연결
            Application.Idle += Run;
        }

        private void 닫어좀(object sender, FormClosingEventArgs e)
        {
            coreRapper.Delete();
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
        }

        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {

        }

        private void richTextBox1_TextChanged(object sender, EventArgs e)
        {
            richTextBox1.Focus();
            string getString = richTextBox1.Text;

            long Value = Convert.ToInt64(getString);

        }

        private void TextBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            textBox1.Focus();

            if (!(char.IsDigit(e.KeyChar) || e.KeyChar == Convert.ToChar(Keys.Back)))
            {
                e.Handled = true;
            }
        }
    }
}
