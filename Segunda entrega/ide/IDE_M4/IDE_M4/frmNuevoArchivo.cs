using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace IDE_M4 {
    public partial class frmNuevoArchivo : Form {
        private string fileName;

        public string FileName {
            get { return fileName; }
            set { fileName = value; }
        }

        public frmNuevoArchivo() {
            InitializeComponent();

        }

        private void btnCancelar_Click(object sender, EventArgs e) {
            this.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.Close();
        }

        private void btnCrear_Click(object sender, EventArgs e) {
            this.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.Close();
        }

        private void btnFolderBrowser_Click(object sender, EventArgs e) {
            this.folderBrowserDialog.ShowDialog(this);
            this.txtFolder.Text = this.folderBrowserDialog.SelectedPath;
            changeFileName();
            enableCreate();
        }

        private void txtFileName_TextChanged(object sender, EventArgs e) {
            changeFileName();
            enableCreate();
        }

        private void changeFileName() {
            this.fileName = this.txtFolder.Text + "\\" + this.txtFileName.Text + ".m4";
            this.txtCreatedFile.Text = this.fileName;
        }

        private void enableCreate() {
            if (String.IsNullOrEmpty(this.txtFileName.Text) || String.IsNullOrEmpty(this.txtFolder.Text))
                this.btnCrear.Enabled = false;
            else
                this.btnCrear.Enabled = true;
        }
    }
}
