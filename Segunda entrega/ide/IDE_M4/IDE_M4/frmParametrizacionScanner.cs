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
    public partial class frmParametrizacionScanner : Form {
        private int MAXLINEA = 1000, MAXDIGIT = 5, MAXID = 200;
        public frmParametrizacionScanner() {
            InitializeComponent();

                leerParametros();

            this.nupMaxLinea.Value = MAXLINEA;
            this.nupMaxDigit.Value = MAXDIGIT;
            this.nupMaxID.Value = MAXID;
        }

        private void btnGuardar_Click(object sender, EventArgs e) {
            try {
                this.guardarParametros();
                this.DialogResult = System.Windows.Forms.DialogResult.OK;
                this.Close();            
            }
            catch (Exception ex) {
                MessageBox.Show("Ocurrio un error al intentar guardar los parametros",
                    "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btnCancelar_Click(object sender, EventArgs e) {
            this.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.Close();
        }

        private void guardarParametros() {
            System.IO.StreamWriter file = new System.IO.StreamWriter("param.txt",false);
            file.WriteLine("MAXLINEA;" + nupMaxLinea.Value);
            file.WriteLine("MAXDIGIT;" + nupMaxDigit.Value);
            file.WriteLine("MAXID;" + nupMaxID.Value);

            file.Close();
        }
        
        private void leerParametros() {
            string[] lines;
            try {
                lines = System.IO.File.ReadAllLines("param.txt");
            } catch (Exception ex) {
                return;
            }

            foreach (string line in lines) {
                try {
                    string[] param = line.Split(';');
                    if (param.Length == 2) {
                        if (param[0].Equals("MAXLINEA")) {
                            MAXLINEA = Int32.Parse(param[1]);
                        } else if (param[0].Equals("MAXDIGIT")) {
                            MAXDIGIT = Int32.Parse(param[1]);
                        } else if (param[0].Equals("MAXID")) {
                            MAXID = Int32.Parse(param[1]);
                        }
                    }
                } catch (Exception e) { }
            }
        }
    }
}
