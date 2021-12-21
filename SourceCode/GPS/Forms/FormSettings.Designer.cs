namespace OpenGrade
{
    partial class FormSettings
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormSettings));
            this.tabOpenGrade = new System.Windows.Forms.TabControl();
            this.tabVehicle = new System.Windows.Forms.TabPage();
            this.label3 = new System.Windows.Forms.Label();
            this.nudMinSlope = new System.Windows.Forms.NumericUpDown();
            this.nudToolWidth = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.label28 = new System.Windows.Forms.Label();
            this.label26 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.label25 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.nudAntennaHeight = new System.Windows.Forms.NumericUpDown();
            this.tabDisplay = new System.Windows.Forms.TabPage();
            this.label24 = new System.Windows.Forms.Label();
            this.nudViewDistUnderGnd = new System.Windows.Forms.NumericUpDown();
            this.label20 = new System.Windows.Forms.Label();
            this.nudViewDistAboveGnd = new System.Windows.Forms.NumericUpDown();
            this.label16 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.nudGradeDistFromLine = new System.Windows.Forms.NumericUpDown();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.label7 = new System.Windows.Forms.Label();
            this.nudKd = new System.Windows.Forms.NumericUpDown();
            this.label6 = new System.Windows.Forms.Label();
            this.nudKi = new System.Windows.Forms.NumericUpDown();
            this.label5 = new System.Windows.Forms.Label();
            this.nudKp = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.nudPlowHeight = new System.Windows.Forms.NumericUpDown();
            this.lblMinTileCover = new System.Windows.Forms.Label();
            this.nudMinTileCover = new System.Windows.Forms.NumericUpDown();
            this.nudMaxTileCut = new System.Windows.Forms.NumericUpDown();
            this.lblMaxDitchCut = new System.Windows.Forms.Label();
            this.nudMaxDitchCut = new System.Windows.Forms.NumericUpDown();
            this.lblMaxTileCut = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.lblInchesCm = new System.Windows.Forms.Label();
            this.btnCancel = new System.Windows.Forms.Button();
            this.bntOK = new System.Windows.Forms.Button();
            this.tabOpenGrade.SuspendLayout();
            this.tabVehicle.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudMinSlope)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudToolWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudAntennaHeight)).BeginInit();
            this.tabDisplay.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudViewDistUnderGnd)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudViewDistAboveGnd)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudGradeDistFromLine)).BeginInit();
            this.tabPage1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudKd)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudKi)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudKp)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudPlowHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudMinTileCover)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudMaxTileCut)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudMaxDitchCut)).BeginInit();
            this.SuspendLayout();
            // 
            // tabOpenGrade
            // 
            resources.ApplyResources(this.tabOpenGrade, "tabOpenGrade");
            this.tabOpenGrade.Controls.Add(this.tabVehicle);
            this.tabOpenGrade.Controls.Add(this.tabDisplay);
            this.tabOpenGrade.Controls.Add(this.tabPage1);
            this.tabOpenGrade.Name = "tabOpenGrade";
            this.tabOpenGrade.SelectedIndex = 0;
            this.tabOpenGrade.SizeMode = System.Windows.Forms.TabSizeMode.Fixed;
            // 
            // tabVehicle
            // 
            this.tabVehicle.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(35)))), ((int)(((byte)(35)))));
            resources.ApplyResources(this.tabVehicle, "tabVehicle");
            this.tabVehicle.Controls.Add(this.label3);
            this.tabVehicle.Controls.Add(this.nudMinSlope);
            this.tabVehicle.Controls.Add(this.nudToolWidth);
            this.tabVehicle.Controls.Add(this.label2);
            this.tabVehicle.Controls.Add(this.label1);
            this.tabVehicle.Controls.Add(this.label15);
            this.tabVehicle.Controls.Add(this.label28);
            this.tabVehicle.Controls.Add(this.label26);
            this.tabVehicle.Controls.Add(this.label12);
            this.tabVehicle.Controls.Add(this.label13);
            this.tabVehicle.Controls.Add(this.label25);
            this.tabVehicle.Controls.Add(this.label18);
            this.tabVehicle.Controls.Add(this.nudAntennaHeight);
            this.tabVehicle.Name = "tabVehicle";
            this.tabVehicle.Click += new System.EventHandler(this.tabVehicle_Click);
            // 
            // label3
            // 
            resources.ApplyResources(this.label3, "label3");
            this.label3.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.label3.Name = "label3";
            // 
            // nudMinSlope
            // 
            this.nudMinSlope.BackColor = System.Drawing.SystemColors.MenuText;
            this.nudMinSlope.DecimalPlaces = 2;
            resources.ApplyResources(this.nudMinSlope, "nudMinSlope");
            this.nudMinSlope.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.nudMinSlope.Increment = new decimal(new int[] {
            1,
            0,
            0,
            131072});
            this.nudMinSlope.Maximum = new decimal(new int[] {
            20,
            0,
            0,
            0});
            this.nudMinSlope.Minimum = new decimal(new int[] {
            20,
            0,
            0,
            -2147483648});
            this.nudMinSlope.Name = "nudMinSlope";
            this.nudMinSlope.Value = new decimal(new int[] {
            2,
            0,
            0,
            -2147418112});
            this.nudMinSlope.ValueChanged += new System.EventHandler(this.nudMinSlope_ValueChanged);
            // 
            // nudToolWidth
            // 
            this.nudToolWidth.BackColor = System.Drawing.SystemColors.MenuText;
            resources.ApplyResources(this.nudToolWidth, "nudToolWidth");
            this.nudToolWidth.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.nudToolWidth.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
            this.nudToolWidth.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudToolWidth.Name = "nudToolWidth";
            this.nudToolWidth.Value = new decimal(new int[] {
            7,
            0,
            0,
            0});
            this.nudToolWidth.ValueChanged += new System.EventHandler(this.nudToolWidth_ValueChanged);
            // 
            // label2
            // 
            resources.ApplyResources(this.label2, "label2");
            this.label2.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.label2.Name = "label2";
            // 
            // label1
            // 
            resources.ApplyResources(this.label1, "label1");
            this.label1.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.label1.Name = "label1";
            // 
            // label15
            // 
            resources.ApplyResources(this.label15, "label15");
            this.label15.Name = "label15";
            // 
            // label28
            // 
            resources.ApplyResources(this.label28, "label28");
            this.label28.Name = "label28";
            // 
            // label26
            // 
            resources.ApplyResources(this.label26, "label26");
            this.label26.Name = "label26";
            // 
            // label12
            // 
            resources.ApplyResources(this.label12, "label12");
            this.label12.Name = "label12";
            // 
            // label13
            // 
            resources.ApplyResources(this.label13, "label13");
            this.label13.Name = "label13";
            // 
            // label25
            // 
            resources.ApplyResources(this.label25, "label25");
            this.label25.Name = "label25";
            // 
            // label18
            // 
            resources.ApplyResources(this.label18, "label18");
            this.label18.Name = "label18";
            // 
            // nudAntennaHeight
            // 
            this.nudAntennaHeight.BackColor = System.Drawing.SystemColors.MenuText;
            resources.ApplyResources(this.nudAntennaHeight, "nudAntennaHeight");
            this.nudAntennaHeight.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.nudAntennaHeight.Maximum = new decimal(new int[] {
            999,
            0,
            0,
            0});
            this.nudAntennaHeight.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudAntennaHeight.Name = "nudAntennaHeight";
            this.nudAntennaHeight.Value = new decimal(new int[] {
            7,
            0,
            0,
            0});
            this.nudAntennaHeight.ValueChanged += new System.EventHandler(this.nudAntennaHeight_ValueChanged);
            // 
            // tabDisplay
            // 
            this.tabDisplay.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(35)))), ((int)(((byte)(35)))));
            this.tabDisplay.Controls.Add(this.label24);
            this.tabDisplay.Controls.Add(this.nudViewDistUnderGnd);
            this.tabDisplay.Controls.Add(this.label20);
            this.tabDisplay.Controls.Add(this.nudViewDistAboveGnd);
            this.tabDisplay.Controls.Add(this.label16);
            this.tabDisplay.Controls.Add(this.label14);
            this.tabDisplay.Controls.Add(this.nudGradeDistFromLine);
            resources.ApplyResources(this.tabDisplay, "tabDisplay");
            this.tabDisplay.Name = "tabDisplay";
            // 
            // label24
            // 
            resources.ApplyResources(this.label24, "label24");
            this.label24.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.label24.Name = "label24";
            // 
            // nudViewDistUnderGnd
            // 
            this.nudViewDistUnderGnd.BackColor = System.Drawing.SystemColors.MenuText;
            resources.ApplyResources(this.nudViewDistUnderGnd, "nudViewDistUnderGnd");
            this.nudViewDistUnderGnd.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.nudViewDistUnderGnd.Maximum = new decimal(new int[] {
            999,
            0,
            0,
            0});
            this.nudViewDistUnderGnd.Name = "nudViewDistUnderGnd";
            this.nudViewDistUnderGnd.ValueChanged += new System.EventHandler(this.nudViewDistUnderGnd_ValueChanged);
            // 
            // label20
            // 
            resources.ApplyResources(this.label20, "label20");
            this.label20.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.label20.Name = "label20";
            this.label20.Click += new System.EventHandler(this.label20_Click);
            // 
            // nudViewDistAboveGnd
            // 
            this.nudViewDistAboveGnd.BackColor = System.Drawing.SystemColors.MenuText;
            resources.ApplyResources(this.nudViewDistAboveGnd, "nudViewDistAboveGnd");
            this.nudViewDistAboveGnd.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.nudViewDistAboveGnd.Maximum = new decimal(new int[] {
            999,
            0,
            0,
            0});
            this.nudViewDistAboveGnd.Name = "nudViewDistAboveGnd";
            this.nudViewDistAboveGnd.ValueChanged += new System.EventHandler(this.nudViewDistAboveGnd_ValueChanged);
            // 
            // label16
            // 
            resources.ApplyResources(this.label16, "label16");
            this.label16.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.label16.Name = "label16";
            // 
            // label14
            // 
            resources.ApplyResources(this.label14, "label14");
            this.label14.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.label14.Name = "label14";
            // 
            // nudGradeDistFromLine
            // 
            this.nudGradeDistFromLine.BackColor = System.Drawing.SystemColors.MenuText;
            resources.ApplyResources(this.nudGradeDistFromLine, "nudGradeDistFromLine");
            this.nudGradeDistFromLine.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.nudGradeDistFromLine.Maximum = new decimal(new int[] {
            999,
            0,
            0,
            0});
            this.nudGradeDistFromLine.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudGradeDistFromLine.Name = "nudGradeDistFromLine";
            this.nudGradeDistFromLine.Value = new decimal(new int[] {
            50,
            0,
            0,
            0});
            // 
            // tabPage1
            // 
            this.tabPage1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(35)))), ((int)(((byte)(35)))));
            this.tabPage1.Controls.Add(this.label7);
            this.tabPage1.Controls.Add(this.nudKd);
            this.tabPage1.Controls.Add(this.label6);
            this.tabPage1.Controls.Add(this.nudKi);
            this.tabPage1.Controls.Add(this.label5);
            this.tabPage1.Controls.Add(this.nudKp);
            this.tabPage1.Controls.Add(this.label4);
            this.tabPage1.Controls.Add(this.nudPlowHeight);
            this.tabPage1.Controls.Add(this.lblMinTileCover);
            this.tabPage1.Controls.Add(this.nudMinTileCover);
            this.tabPage1.Controls.Add(this.nudMaxTileCut);
            this.tabPage1.Controls.Add(this.lblMaxDitchCut);
            this.tabPage1.Controls.Add(this.nudMaxDitchCut);
            this.tabPage1.Controls.Add(this.lblMaxTileCut);
            resources.ApplyResources(this.tabPage1, "tabPage1");
            this.tabPage1.Name = "tabPage1";
            // 
            // label7
            // 
            resources.ApplyResources(this.label7, "label7");
            this.label7.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.label7.Name = "label7";
            // 
            // nudKd
            // 
            this.nudKd.BackColor = System.Drawing.SystemColors.MenuText;
            resources.ApplyResources(this.nudKd, "nudKd");
            this.nudKd.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.nudKd.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.nudKd.Name = "nudKd";
            this.nudKd.Value = new decimal(new int[] {
            255,
            0,
            0,
            0});
            // 
            // label6
            // 
            resources.ApplyResources(this.label6, "label6");
            this.label6.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.label6.Name = "label6";
            // 
            // nudKi
            // 
            this.nudKi.BackColor = System.Drawing.SystemColors.MenuText;
            resources.ApplyResources(this.nudKi, "nudKi");
            this.nudKi.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.nudKi.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.nudKi.Name = "nudKi";
            this.nudKi.Value = new decimal(new int[] {
            21,
            0,
            0,
            0});
            // 
            // label5
            // 
            resources.ApplyResources(this.label5, "label5");
            this.label5.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.label5.Name = "label5";
            this.label5.Click += new System.EventHandler(this.label5_Click_1);
            // 
            // nudKp
            // 
            this.nudKp.BackColor = System.Drawing.SystemColors.MenuText;
            resources.ApplyResources(this.nudKp, "nudKp");
            this.nudKp.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.nudKp.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.nudKp.Name = "nudKp";
            this.nudKp.Value = new decimal(new int[] {
            8,
            0,
            0,
            0});
            // 
            // label4
            // 
            resources.ApplyResources(this.label4, "label4");
            this.label4.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.label4.Name = "label4";
            this.label4.Click += new System.EventHandler(this.label4_Click);
            // 
            // nudPlowHeight
            // 
            this.nudPlowHeight.BackColor = System.Drawing.SystemColors.MenuText;
            resources.ApplyResources(this.nudPlowHeight, "nudPlowHeight");
            this.nudPlowHeight.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.nudPlowHeight.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
            this.nudPlowHeight.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudPlowHeight.Name = "nudPlowHeight";
            this.nudPlowHeight.Value = new decimal(new int[] {
            7,
            0,
            0,
            0});
            // 
            // lblMinTileCover
            // 
            resources.ApplyResources(this.lblMinTileCover, "lblMinTileCover");
            this.lblMinTileCover.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.lblMinTileCover.Name = "lblMinTileCover";
            // 
            // nudMinTileCover
            // 
            this.nudMinTileCover.BackColor = System.Drawing.SystemColors.MenuText;
            resources.ApplyResources(this.nudMinTileCover, "nudMinTileCover");
            this.nudMinTileCover.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.nudMinTileCover.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
            this.nudMinTileCover.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudMinTileCover.Name = "nudMinTileCover";
            this.nudMinTileCover.Value = new decimal(new int[] {
            7,
            0,
            0,
            0});
            // 
            // nudMaxTileCut
            // 
            this.nudMaxTileCut.BackColor = System.Drawing.SystemColors.MenuText;
            resources.ApplyResources(this.nudMaxTileCut, "nudMaxTileCut");
            this.nudMaxTileCut.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.nudMaxTileCut.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
            this.nudMaxTileCut.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudMaxTileCut.Name = "nudMaxTileCut";
            this.nudMaxTileCut.Value = new decimal(new int[] {
            7,
            0,
            0,
            0});
            // 
            // lblMaxDitchCut
            // 
            resources.ApplyResources(this.lblMaxDitchCut, "lblMaxDitchCut");
            this.lblMaxDitchCut.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.lblMaxDitchCut.Name = "lblMaxDitchCut";
            // 
            // nudMaxDitchCut
            // 
            this.nudMaxDitchCut.BackColor = System.Drawing.SystemColors.MenuText;
            resources.ApplyResources(this.nudMaxDitchCut, "nudMaxDitchCut");
            this.nudMaxDitchCut.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.nudMaxDitchCut.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
            this.nudMaxDitchCut.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudMaxDitchCut.Name = "nudMaxDitchCut";
            this.nudMaxDitchCut.Value = new decimal(new int[] {
            7,
            0,
            0,
            0});
            // 
            // lblMaxTileCut
            // 
            resources.ApplyResources(this.lblMaxTileCut, "lblMaxTileCut");
            this.lblMaxTileCut.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.lblMaxTileCut.Name = "lblMaxTileCut";
            this.lblMaxTileCut.Click += new System.EventHandler(this.label5_Click);
            // 
            // label17
            // 
            resources.ApplyResources(this.label17, "label17");
            this.label17.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.label17.Name = "label17";
            // 
            // lblInchesCm
            // 
            resources.ApplyResources(this.lblInchesCm, "lblInchesCm");
            this.lblInchesCm.ForeColor = System.Drawing.Color.Red;
            this.lblInchesCm.Name = "lblInchesCm";
            // 
            // btnCancel
            // 
            this.btnCancel.BackColor = System.Drawing.Color.Black;
            resources.ApplyResources(this.btnCancel, "btnCancel");
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Image = global::OpenGrade.Properties.Resources.Cancel64;
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.UseVisualStyleBackColor = false;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // bntOK
            // 
            this.bntOK.BackColor = System.Drawing.Color.Black;
            resources.ApplyResources(this.bntOK, "bntOK");
            this.bntOK.Image = global::OpenGrade.Properties.Resources.OK64;
            this.bntOK.Name = "bntOK";
            this.bntOK.UseVisualStyleBackColor = false;
            this.bntOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // FormSettings
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(35)))), ((int)(((byte)(35)))));
            this.ControlBox = false;
            this.Controls.Add(this.lblInchesCm);
            this.Controls.Add(this.label17);
            this.Controls.Add(this.tabOpenGrade);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.bntOK);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FormSettings";
            this.Load += new System.EventHandler(this.FormSettings_Load);
            this.tabOpenGrade.ResumeLayout(false);
            this.tabVehicle.ResumeLayout(false);
            this.tabVehicle.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudMinSlope)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudToolWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudAntennaHeight)).EndInit();
            this.tabDisplay.ResumeLayout(false);
            this.tabDisplay.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudViewDistUnderGnd)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudViewDistAboveGnd)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudGradeDistFromLine)).EndInit();
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudKd)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudKi)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudKp)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudPlowHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudMinTileCover)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudMaxTileCut)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudMaxDitchCut)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TabControl tabOpenGrade;
        private System.Windows.Forms.Button bntOK;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.TabPage tabDisplay;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label lblInchesCm;
        private System.Windows.Forms.TabPage tabVehicle;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label label28;
        private System.Windows.Forms.Label label26;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label25;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.NumericUpDown nudAntennaHeight;
        private System.Windows.Forms.NumericUpDown nudMinSlope;
        private System.Windows.Forms.NumericUpDown nudToolWidth;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.NumericUpDown nudMaxTileCut;
        private System.Windows.Forms.Label lblMaxDitchCut;
        private System.Windows.Forms.NumericUpDown nudMaxDitchCut;
        private System.Windows.Forms.Label lblMaxTileCut;
        private System.Windows.Forms.Label lblMinTileCover;
        private System.Windows.Forms.NumericUpDown nudMinTileCover;
        private System.Windows.Forms.Label label24;
        private System.Windows.Forms.NumericUpDown nudViewDistUnderGnd;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.NumericUpDown nudViewDistAboveGnd;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.NumericUpDown nudGradeDistFromLine;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown nudPlowHeight;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.NumericUpDown nudKd;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.NumericUpDown nudKi;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.NumericUpDown nudKp;
    }
}