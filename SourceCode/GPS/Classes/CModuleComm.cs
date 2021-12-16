
namespace OpenGrade
{
    public class CModuleComm
    {
        //copy of the mainform address
        private readonly FormGPS mf = null;


        //GradeControl ------------------------------------------------------------------------------------------------
        public string serialRecvGradeControlStr;   ///Black Ace Industries

        //For parsing incoming int on serial port
        public int incomingInt;

        // PGN - 32762 - 127.250
        public static int numGradeControlDataItems = 6;
        public byte[] GradeControlData = new byte[numGradeControlDataItems];

        public int gcHeaderHi, gcHeaderLo = 1, gcDeltaDir = 2, gcCutDelta = 3, gcisAutoActive = 4;                                

        //info coming from Grade Control Mod
        public int autoModeVal = 0, bladeOffset = 0;
        public double outputPID = 0;

        // PGN - 32760 - 127.248
        public static int numGradeControlSettingsItems = 10;
        public byte[] gradeControlSettings = new byte[numGradeControlSettingsItems];
        public int rsHeaderHi, rsHeaderLo = 1, rsAccumulatedVolumeHi = 2, rsAccumulatedVolumeLo = 3,
            rsFlowCalFactorHi = 4, rsFlowCalFactorLo = 5;

        //AutoSteer ------------------------------------------------------------------------------------------------
        public string serialRecvAutoSteerStr;

        // PGN - 32766 - 127.254
        public static int numSteerDataItems = 8;
        public byte[] autoSteerData = new byte[numSteerDataItems];
        public int sdHeaderHi, sdHeaderLo=1, sdRelay=2, sdSpeed=3, sdDistanceHi=4, sdDistanceLo=5,
                    sdSteerAngleHi=6, sdSteerAngleLo=7;

        // PGN - 32764 - 127.252
        public static int numSteerSettingItems = 10;
        public byte[] autoSteerSettings = new byte[numSteerSettingItems];
        public int ssHeaderHi, ssHeaderLo = 1, ssKp = 2, ssKi = 3, ssKd = 4, ssKo = 5,
                    ssSteerOffset = 6, ssMinPWM = 7, ssMaxIntegral = 8, ssCountsPerDegree = 9;

        //for the workswitch
        public bool isWorkSwitchActiveLow, isWorkSwitchEnabled;
        public int workSwitchValue;

        //imu and roll inclinometer
        public int steerSwitchValue, gyroHeading=9999, prevGyroHeading = 9999;
        public int rollRaw=9999; //inclinometer ?

        

        //constructor
        public CModuleComm(FormGPS _f)
        {
            mf = _f;
            serialRecvAutoSteerStr = "Oops";
            serialRecvGradeControlStr = "Oops";

            //WorkSwitch logic
            isWorkSwitchEnabled = false;

            //does a low, grounded out, mean on
            isWorkSwitchActiveLow = true;
        }

        //Reset all the byte arrays from modules
        public void ResetAllModuleCommValues()
        {
            GradeControlData[gcHeaderHi] = 127; // PGN - 32762
            GradeControlData[gcHeaderLo] = 250;
            GradeControlData[gcDeltaDir] = 0;
            GradeControlData[gcCutDelta] = 0;
            GradeControlData[gcisAutoActive] = 0;                       
            mf.GradeControlDataOutToPort();

            autoSteerData[sdHeaderHi] = 127; // PGN - 32766
            autoSteerData[sdHeaderLo] = 254;
            autoSteerData[sdRelay] = 0;
            autoSteerData[sdSpeed] = (0);
            autoSteerData[sdDistanceHi] = (125); // PGN - 32020
            autoSteerData[sdDistanceLo] = 20;
            autoSteerData[sdSteerAngleHi] = (125); // PGN - 32020
            autoSteerData[sdSteerAngleLo] = 20;
            mf.AutoSteerDataOutToPort();

            autoSteerSettings[ssHeaderHi] = 127;// PGN - 32764 as header
            autoSteerSettings[ssHeaderLo] = 252;
            autoSteerSettings[ssKp] = Properties.Settings.Default.setAS_Kp;
            autoSteerSettings[ssKi] = Properties.Settings.Default.setAS_Ki;
            autoSteerSettings[ssKd] = Properties.Settings.Default.setAS_Kd;
            autoSteerSettings[ssKo] = Properties.Settings.Default.setAS_Ko;
            autoSteerSettings[ssSteerOffset] = Properties.Settings.Default.setAS_steerAngleOffset;
            autoSteerSettings[ssMinPWM] = Properties.Settings.Default.setAS_minSteerPWM;
            autoSteerSettings[ssMaxIntegral] = Properties.Settings.Default.setAS_maxIntegral;
            autoSteerSettings[ssCountsPerDegree] = Properties.Settings.Default.setAS_countsPerDegree;
            mf.AutoSteerSettingsOutToPort();
        }
    }
}
