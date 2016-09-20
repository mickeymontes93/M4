namespace IDE_M4 {
    partial class frmParametrizacionScanner {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.nupMaxLinea = new System.Windows.Forms.NumericUpDown();
            this.nupMaxDigit = new System.Windows.Forms.NumericUpDown();
            this.nupMaxID = new System.Windows.Forms.NumericUpDown();
            this.btnGuardar = new System.Windows.Forms.Button();
            this.btnCancelar = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nupMaxLinea)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nupMaxDigit)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nupMaxID)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.nupMaxID);
            this.groupBox1.Controls.Add(this.nupMaxDigit);
            this.groupBox1.Controls.Add(this.nupMaxLinea);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(12, 8);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(419, 147);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(20, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(250, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Tamaño máximo de una línea del programa fuente :";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(20, 59);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(206, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "Máximo número de dígitos en los enteros :";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(20, 104);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(188, 13);
            this.label3.TabIndex = 2;
            this.label3.Text = "Máxima longitud de los identificadores:";
            // 
            // nupMaxLinea
            // 
            this.nupMaxLinea.Location = new System.Drawing.Point(277, 19);
            this.nupMaxLinea.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.nupMaxLinea.Name = "nupMaxLinea";
            this.nupMaxLinea.Size = new System.Drawing.Size(120, 20);
            this.nupMaxLinea.TabIndex = 3;
            this.nupMaxLinea.Value = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            // 
            // nupMaxDigit
            // 
            this.nupMaxDigit.Location = new System.Drawing.Point(277, 57);
            this.nupMaxDigit.Maximum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.nupMaxDigit.Name = "nupMaxDigit";
            this.nupMaxDigit.Size = new System.Drawing.Size(120, 20);
            this.nupMaxDigit.TabIndex = 4;
            this.nupMaxDigit.Value = new decimal(new int[] {
            5,
            0,
            0,
            0});
            // 
            // nupMaxID
            // 
            this.nupMaxID.Location = new System.Drawing.Point(277, 102);
            this.nupMaxID.Maximum = new decimal(new int[] {
            500,
            0,
            0,
            0});
            this.nupMaxID.Name = "nupMaxID";
            this.nupMaxID.Size = new System.Drawing.Size(120, 20);
            this.nupMaxID.TabIndex = 5;
            this.nupMaxID.Value = new decimal(new int[] {
            200,
            0,
            0,
            0});
            // 
            // btnGuardar
            // 
            this.btnGuardar.Location = new System.Drawing.Point(321, 165);
            this.btnGuardar.Name = "btnGuardar";
            this.btnGuardar.Size = new System.Drawing.Size(110, 28);
            this.btnGuardar.TabIndex = 1;
            this.btnGuardar.Text = "Guardar";
            this.btnGuardar.UseVisualStyleBackColor = true;
            this.btnGuardar.Click += new System.EventHandler(this.btnGuardar_Click);
            // 
            // btnCancelar
            // 
            this.btnCancelar.Location = new System.Drawing.Point(205, 165);
            this.btnCancelar.Name = "btnCancelar";
            this.btnCancelar.Size = new System.Drawing.Size(110, 28);
            this.btnCancelar.TabIndex = 2;
            this.btnCancelar.Text = "Cancelar";
            this.btnCancelar.UseVisualStyleBackColor = true;
            this.btnCancelar.Click += new System.EventHandler(this.btnCancelar_Click);
            // 
            // frmParametrizacionScanner
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(443, 207);
            this.ControlBox = false;
            this.Controls.Add(this.btnCancelar);
            this.Controls.Add(this.btnGuardar);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "frmParametrizacionScanner";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Parametrización del scanner";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nupMaxLinea)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nupMaxDigit)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nupMaxID)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown nupMaxID;
        private System.Windows.Forms.NumericUpDown nupMaxDigit;
        private System.Windows.Forms.NumericUpDown nupMaxLinea;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button btnGuardar;
        private System.Windows.Forms.Button btnCancelar;
    }
}