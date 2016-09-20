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
            editor.ContextMenuStrip = cmMain;

            tabControl1.TabPages.Add(tab);
            tab.Controls.Add(editor);
            changeFormText();
        }

        private void undoToolStripMenuItem_Click(object sender, EventArgs e) {
            try {
                CustomM4Editor editor = (CustomM4Editor)tabControl1.SelectedTab.Controls[0];
                editor.deshacer();
            } catch (Exception ex) { }
        }

        private void openFile(object sender, EventArgs e) {
            openFileDialog1.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Personal);
            openFileDialog1.Filter = "Codigo fuente (*.m4)|*.m4";
            if (openFileDialog1.ShowDialog(this) == DialogResult.OK) {
                createNewTabFromFile(openFileDialog1.FileName);
                changeFormText();
            }
        }

        private void saveToolStripButton_Click(object sender, EventArgs e) {
            try {
                CustomM4Editor editor = (CustomM4Editor)tabControl1.SelectedTab.Controls[0];
                editor.guardar();
                MessageBox.Show("Archivo guardado exitosamente", "Guardar", MessageBoxButtons.OK, MessageBoxIcon.Information);
            } catch (Exception ex) { }
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
            try {
                CustomM4Editor editor = (CustomM4Editor)tabControl1.SelectedTab.Controls[0];
                editor.rehacer();
            } catch (Exception ex) { }

        }

        private void cutToolStripMenuItem_Click(object sender, EventArgs e) {
            try {
                CustomM4Editor editor = (CustomM4Editor)tabControl1.SelectedTab.Controls[0];
                editor.cortar();
            } catch (Exception ex) { }

        }

        private void copyToolStripMenuItem_Click(object sender, EventArgs e) {
            try {
                CustomM4Editor editor = (CustomM4Editor)tabControl1.SelectedTab.Controls[0];
                editor.copiar();
            } catch (Exception ex) { }

        }

        private void pasteToolStripMenuItem_Click(object sender, EventArgs e) {
            try {
                CustomM4Editor editor = (CustomM4Editor)tabControl1.SelectedTab.Controls[0];
                editor.pegar();
            } catch (Exception ex) { }

        }

        private void selectAllToolStripMenuItem_Click(object sender, EventArgs e) {
            try {
                CustomM4Editor editor = (CustomM4Editor)tabControl1.SelectedTab.Controls[0];
                editor.seleccionarTodo();
            } catch (Exception ex) { }

        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e) {
            try {
                CustomM4Editor editor = (CustomM4Editor)tabControl1.SelectedTab.Controls[0];

                SaveFileDialog saveFileDialog1 = new SaveFileDialog();
                saveFileDialog1.Filter = "M4 file|*.m4";
                saveFileDialog1.Title = "Save an M4 File";
                saveFileDialog1.ShowDialog();

                // If the file name is not an empty string open it for saving.
                if (saveFileDialog1.FileName != "") {
                    // Saves the Image via a FileStream created by the OpenFile method.
                    editor.FileName = saveFileDialog1.FileName;
                    editor.guardar();
                    MessageBox.Show("Archivo guardado exitosamente", "Guardar", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            } catch (Exception ex) { }

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
                    FileName = "scanner\\objeto\\scanner.exe",
                    Arguments = "\"" + editor.FileName + "\"",
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

        private void contentsToolStripMenuItem_Click(object sender, EventArgs e) {
            System.Diagnostics.Process.Start("ayuda.pdf");
        }

        private void parametrizarScannerToolStripMenuItem_Click(object sender, EventArgs e) {
            frmParametrizacionScanner form = new frmParametrizacionScanner();
            form.ShowDialog(this);
        }
    }
}
