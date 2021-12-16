//Please, if you use this, share the improvements

using System;
using System.IO;
using System.Windows.Forms;

namespace OpenGrade
{
    public partial class FormSettings : Form
    {
       //class variables
        private readonly FormGPS mf = null;

        private double antennaHeight, plowHeight, minSlope, toolWidth, maxTileCut, maxDitchCut, minTileCover, viewDistAboveGnd, viewDistUnderGnd;
        private double KpGain, KiGain, KdGain;
        private readonly double metImp2m, m2MetImp;

        //constructor
        public FormSettings(Form callingForm, int page)
        {
            //get copy of the calling main form
            mf = callingForm as FormGPS;
            InitializeComponent();

            if (mf.isMetric)
            {
                metImp2m = 0.01;
                m2MetImp = 100.0;
                lblInchesCm.Text = "Centimeters";
            }
            else
            {
                metImp2m = glm.in2m;
                m2MetImp = glm.m2in;
                lblInchesCm.Text = "Inches";
            }
            //select the page as per calling menu or button from mainGPS form
            tabOpenGrade.SelectedIndex = page;
        }

        //do any field initializing for form here
        private void FormSettings_Load(object sender, EventArgs e)
        {
            //Vehicle settings to what it is in the settings page------------------------------------------------
            antennaHeight = Properties.Vehicle.Default.setVehicle_antennaHeight;
            plowHeight = Properties.Vehicle.Default.setVehicle_plowHeight;

            maxTileCut = Properties.Vehicle.Default.setVehicle_maxTileCut;  
            maxDitchCut = Properties.Vehicle.Default.setVehicle_maxDitchCut;
            minTileCover = Properties.Vehicle.Default.setVehicle_minTileCover;            
            

            KpGain = Properties.Vehicle.Default.setVehicle_KpGain;
            KiGain = Properties.Vehicle.Default.setVehicle_KiGain;
            KdGain = Properties.Vehicle.Default.setVehicle_KdGain;

            viewDistAboveGnd = Properties.Vehicle.Default.setVehicle_viewDistAboveGnd;
            viewDistUnderGnd = Properties.Vehicle.Default.setVehicle_viewDistUnderGnd;         
            toolWidth = Properties.Vehicle.Default.setVehicle_toolWidth;
            minSlope = Properties.Vehicle.Default.setVehicle_minSlope * 100;
            

            nudAntennaHeight.ValueChanged -= nudAntennaHeight_ValueChanged;
            nudAntennaHeight.Value = (decimal)(antennaHeight * m2MetImp);
            nudAntennaHeight.ValueChanged += nudAntennaHeight_ValueChanged;

            nudPlowHeight.ValueChanged -= nudPlowHeight_ValueChanged;
            nudPlowHeight.Value = (decimal)(plowHeight * m2MetImp);
            nudPlowHeight.ValueChanged += nudPlowHeight_ValueChanged;

            //Black Ace Industries

            nudMaxTileCut.ValueChanged -= nudMaxTileCut_ValueChanged;
            nudMaxTileCut.Value = (decimal)(maxTileCut * m2MetImp);
            nudMaxTileCut.ValueChanged += nudMaxTileCut_ValueChanged;

            nudMinTileCover.ValueChanged -= nudMinTileCover_ValueChanged;
            nudMinTileCover.Value = (decimal)(minTileCover * m2MetImp);
            nudMinTileCover.ValueChanged += nudMinTileCover_ValueChanged;

            nudMaxDitchCut.ValueChanged -= nudMaxDitchCut_ValueChanged;
            nudMaxDitchCut.Value = (decimal)(maxDitchCut * m2MetImp);
            nudMaxDitchCut.ValueChanged += nudMaxDitchCut_ValueChanged;

            nudKp.ValueChanged -= nudKp_ValueChanged;
            nudKp.Value = (decimal)(KpGain * m2MetImp);
            nudKp.ValueChanged += nudKp_ValueChanged;

            nudKi.ValueChanged -= nudKi_ValueChanged;
            nudKi.Value = (decimal)(KiGain * m2MetImp);
            nudKi.ValueChanged += nudKi_ValueChanged;

            nudKd.ValueChanged -= nudKd_ValueChanged;
            nudKd.Value = (decimal)(KdGain * m2MetImp);
            nudKd.ValueChanged += nudKd_ValueChanged;




            // Pat Code
            nudViewDistAboveGnd.ValueChanged -= nudViewDistAboveGnd_ValueChanged;
            nudViewDistAboveGnd.Value = (decimal)(viewDistAboveGnd * m2MetImp);
            nudViewDistAboveGnd.ValueChanged += nudViewDistAboveGnd_ValueChanged;

            nudViewDistUnderGnd.ValueChanged -= nudViewDistUnderGnd_ValueChanged;
            nudViewDistUnderGnd.Value = (decimal)(viewDistUnderGnd * m2MetImp);
            nudViewDistUnderGnd.ValueChanged += nudViewDistUnderGnd_ValueChanged;        

            nudToolWidth.ValueChanged -= nudToolWidth_ValueChanged;
            nudToolWidth.Value = (decimal)(toolWidth * m2MetImp);
            nudToolWidth.ValueChanged += nudToolWidth_ValueChanged;

            nudMinSlope.ValueChanged -= nudMinSlope_ValueChanged;
            nudMinSlope.Value = (decimal)(minSlope);
            nudMinSlope.ValueChanged += nudMinSlope_ValueChanged;
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            //Vehicle settings -------------------------------------------------------------------------------
            mf.vehicle.minSlope = minSlope/100;
            Properties.Vehicle.Default.setVehicle_minSlope = mf.vehicle.minSlope;

            mf.vehicle.plowHeight = plowHeight;
            Properties.Vehicle.Default.setVehicle_plowHeight = mf.vehicle.plowHeight;
            
            // Black Ace Industries
            mf.vehicle.maxTileCut = maxTileCut;
            Properties.Vehicle.Default.setVehicle_maxTileCut = mf.vehicle.maxTileCut;
            mf.vehicle.maxDitchCut = maxDitchCut;
            Properties.Vehicle.Default.setVehicle_maxDitchCut = mf.vehicle.maxDitchCut;
            mf.vehicle.minTileCover = minTileCover;
            Properties.Vehicle.Default.setVehicle_minTileCover = mf.vehicle.minTileCover;

            mf.vehicle.V_KpGain = KpGain;
            Properties.Vehicle.Default.setVehicle_KpGain = mf.vehicle.V_KpGain;
            mf.vehicle.V_KiGain = KiGain;
            Properties.Vehicle.Default.setVehicle_KiGain = mf.vehicle.V_KiGain;
            mf.vehicle.V_KdGain = KdGain;
            Properties.Vehicle.Default.setVehicle_KdGain = mf.vehicle.V_KdGain;
            


            mf.vehicle.viewDistAboveGnd = viewDistAboveGnd;
            Properties.Vehicle.Default.setVehicle_viewDistAboveGnd = mf.vehicle.viewDistAboveGnd;
            mf.vehicle.viewDistUnderGnd = viewDistUnderGnd;
            Properties.Vehicle.Default.setVehicle_viewDistUnderGnd = mf.vehicle.viewDistUnderGnd;



            mf.vehicle.antennaHeight = antennaHeight;
            Properties.Vehicle.Default.setVehicle_antennaHeight = mf.vehicle.antennaHeight;

            mf.vehicle.toolWidth = toolWidth;
            Properties.Vehicle.Default.setVehicle_toolWidth = toolWidth;

            //Sections ------------------------------------------------------------------------------------------

            Properties.Settings.Default.Save();
            Properties.Vehicle.Default.Save();
            
            

            //back to FormGPS
            DialogResult = DialogResult.OK;
            Close();
        }

        //don't save anything, leave the settings as before
        private void btnCancel_Click(object sender, EventArgs e)
        { DialogResult = DialogResult.Cancel; Close(); }

        private void lblTilePlow_Click(object sender, EventArgs e)
        {

        }

        private void tabVehicle_Click(object sender, EventArgs e)
        {

        }

        private void label5_Click(object sender, EventArgs e)
        {

        }

        private void label20_Click(object sender, EventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void label5_Click_1(object sender, EventArgs e)
        {

        }

        #region Vehicle //----------------------------------------------------------------

        private void nudAntennaHeight_ValueChanged(object sender, EventArgs e)
        {
            antennaHeight = (double)nudAntennaHeight.Value * metImp2m;
        }


        // Black Ace Industries
        private void nudPlowHeight_ValueChanged(object sender, EventArgs e)
        {
            plowHeight = (double)nudPlowHeight.Value * metImp2m;
        }
        private void nudMaxTileCut_ValueChanged(object sender, EventArgs e)
        {
            maxTileCut = (double)nudMaxTileCut.Value * metImp2m;
        }
        private void nudMaxDitchCut_ValueChanged(object sender, EventArgs e)
        {
            maxDitchCut = (double)nudMaxDitchCut.Value * metImp2m;
        }
        private void nudMinTileCover_ValueChanged(object sender, EventArgs e)
        {
            minTileCover = (double)nudMinTileCover.Value * metImp2m;
        }

        private void nudKp_ValueChanged(object sender, EventArgs e)
        {
            KpGain = (double)nudKp.Value * metImp2m;
        }
        private void nudKi_ValueChanged(object sender, EventArgs e)
        {
            KiGain = (double)nudKi.Value * metImp2m;
        }
        private void nudKd_ValueChanged(object sender, EventArgs e)
        {
            KdGain = (double)nudKd.Value * metImp2m;
        }




        private void nudViewDistAboveGnd_ValueChanged(object sender, EventArgs e)
        {
            viewDistAboveGnd = (double)nudViewDistAboveGnd.Value * metImp2m;
        }
        
        private void nudViewDistUnderGnd_ValueChanged(object sender, EventArgs e)
        {
            viewDistUnderGnd = (double)nudViewDistUnderGnd.Value * metImp2m;     
        }


        private void nudMinSlope_ValueChanged(object sender, EventArgs e)
        {
            minSlope = (double)nudMinSlope.Value;
        }

        private void nudToolWidth_ValueChanged(object sender, EventArgs e)
        {
            toolWidth = (double)nudToolWidth.Value * metImp2m;
        }








      #endregion Vehicle
    }
}