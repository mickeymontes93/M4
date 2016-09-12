using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace IDE_M4 {
    public partial class frmMain : Form {
        public frmMain() {
            InitializeComponent();
        }

        private void createNewFile(object sender, EventArgs e) {
            frmNuevoArchivo form = new frmNuevoArchivo();
            form.ShowDialog(this);
            if (form.DialogResult == System.Windows.Forms.DialogResult.OK)
                createNewTabFromFile(form.FileName);
        }

        private void createNewTabFromFile(string filePath) {

            String[] substrings = filePath.Split('\\');
            string fileName = substrings[substrings.Length - 1];


            TabPage tab = new TabPage(fileName);
            CustomM4Editor editor = new CustomM4Editor(filePath);
            editor.Dock = DockStyle.Fill;

            tabControl1.TabPages.Add(tab);
            tab.Controls.Add(editor);
            changeFormText();
        }

        private void undoToolStripMenuItem_Click(object sender, EventArgs e) {

        }

        private void openFile(object sender, EventArgs e) {
            openFileDialog1.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Personal);
            openFileDialog1.Filter = "Codigo fuente (*.m4)|*.m4";
            if (openFileDialog1.ShowDialog(this) == DialogResult.OK) {
                createNewTabFromFile(openFileDialog1.FileName);
            }
            changeFormText();
        }

        private void saveToolStripButton_Click(object sender, EventArgs e) {

        }

        private void toolBarToolStripMenuItem_Click(object sender, EventArgs e) {
            if (toolBarToolStripMenuItem.Checked)
                toolStrip.Visible = true;
            else
                toolStrip.Visible = false;
        }
        private void statusBarToolStripMenuItem_Click(object sender, EventArgs e) {
            if (statusBarToolStripMenuItem.Checked)
                statusStrip.Visible = true;
            else
                statusStrip.Visible = false;
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e) {
            this.Close();
        }

        private void redoToolStripMenuItem_Click(object sender, EventArgs e) {
            CustomM4Editor editor = (CustomM4Editor)tabControl1.SelectedTab.Controls[0];
            editor.rehacer();
        }

        private void cutToolStripMenuItem_Click(object sender, EventArgs e) {
            CustomM4Editor editor = (CustomM4Editor)tabControl1.SelectedTab.Controls[0];
            editor.cortar();
        }

        private void copyToolStripMenuItem_Click(object sender, EventArgs e) {
            CustomM4Editor editor = (CustomM4Editor)tabControl1.SelectedTab.Controls[0];
            editor.copiar();
        }

        private void pasteToolStripMenuItem_Click(object sender, EventArgs e) {
            CustomM4Editor editor = (CustomM4Editor)tabControl1.SelectedTab.Controls[0];
            editor.pegar();
        }

        private void selectAllToolStripMenuItem_Click(object sender, EventArgs e) {
            CustomM4Editor editor = (CustomM4Editor)tabControl1.SelectedTab.Controls[0];
            editor.seleccionarTodo();
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e) {
            CustomM4Editor editor = (CustomM4Editor)tabControl1.SelectedTab.Controls[0];
            editor.guardar();
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e) {

        }

        private void tabControl1_SelectedIndexChanged(object sender, EventArgs e) {
            changeFormText();
        }

        private void changeFormText() {
            CustomM4Editor editor = (CustomM4Editor)tabControl1.SelectedTab.Controls[0];
            this.Text = "IDE M4 - " + editor.FileName;
        }

        private void btnRun_Click(object sender, EventArgs e) {
            this.txtConsole.Clear();
            CustomM4Editor editor = (CustomM4Editor)tabControl1.SelectedTab.Controls[0];
           
            Process proc = new Process {
                StartInfo = new ProcessStartInfo {
                    FileName = "scanner.exe",
                    Arguments = "\"" +  editor.FileName + "\"" ,
                    UseShellExecute = false,
                    RedirectStandardOutput = true,
                    CreateNoWindow = true
                }
            };

            proc.Start();
            while (!proc.StandardOutput.EndOfStream) {
                string line = proc.StandardOutput.ReadLine();
                this.txtConsole.Text += line + Environment.NewLine;
                // do something with line
            }
        }
    }
}
