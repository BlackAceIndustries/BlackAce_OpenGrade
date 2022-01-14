using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Windows.Forms;


namespace OpenGrade
{
    public partial class FormGPS
    {
        // Send and Recv socket
        private Socket sendSocket;
        private Socket recvSocket;

        private bool isSendConnected;

        //endpoint of the auto steer module
        IPEndPoint epAutoSteer;
        IPEndPoint epGradeControl;

        // Data stream
        private byte[] buffer = new byte[1024];

        // Status delegate
        private delegate void UpdateRecvMessageDelegate(string recvMessage);
        private UpdateRecvMessageDelegate updateRecvMessageDelegate = null;

        private void SendUDPMessage(string message)
        {
            if (isSendConnected)
            {
                try
                {
                    // Get packet as byte array
                    byte[] byteData = Encoding.ASCII.GetBytes(message);

                    if (byteData.Length != 0)

                        // Send packet to the zero
                        sendSocket.BeginSendTo(byteData, 0, byteData.Length, SocketFlags.None, epGradeControl, new AsyncCallback(SendData), null);
                }
                catch (Exception e)
                {
                    WriteErrorLog("Sending UDP Message" + e.ToString());

                    MessageBox.Show("Send Error: " + e.Message, "UDP Client", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }
 
        public void SendData(IAsyncResult asyncResult)
        {
            try
            {
                sendSocket.EndSend(asyncResult);
            }
            catch (Exception e)
            {
                WriteErrorLog(" UDP Send Data" + e.ToString());

                MessageBox.Show("SendData Error: " + e.Message, "UDP Server", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void ReceiveData(IAsyncResult asyncResult)
        {
            try
            {
                // Initialise the IPEndPoint for the client
                EndPoint epSender = new IPEndPoint(IPAddress.Any, 0);
            
                // Receive all data
                int msgLen = recvSocket.EndReceiveFrom(asyncResult, ref epSender);

                byte[] localMsg = new byte[msgLen];
                Array.Copy(buffer, localMsg, msgLen);

                // Listen for more connections again...
                recvSocket.BeginReceiveFrom(buffer, 0, buffer.Length, SocketFlags.None, ref epSender, new AsyncCallback(ReceiveData), epSender);

                string text = Encoding.ASCII.GetString(localMsg);

                // Update status through a delegate
                Invoke(updateRecvMessageDelegate, new object[] { text });
            }
            catch (Exception e)
            {
                WriteErrorLog("UDP Recv data " + e.ToString());

                MessageBox.Show("ReceiveData Error: " + e.Message, "UDP Server", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        
        private void UpdateRecvMessage(string recvd)
        {     
            string[] words = recvd.Split(',');
            if (words.Length == 6)
            {
                //first 2 used for display mainly in autosteer window chart as strings
                //parse the values
                mc.prevHeadingIMU = mc.headingIMU;

                if (words[0] == "DATA")
                {
                    int.TryParse(words[1], out mc.autoState);
                    double.TryParse(words[2], out mc.voltage);
                    float.TryParse(words[3], out mc.headingIMU);
                    float.TryParse(words[4], out mc.pitchIMU);
                    float.TryParse(words[5], out mc.rollIMU);
                }


            }
            else if (words.Length < 20)
            {
                if (words[0] == "GPS")
                {
                    pn.rawBuffer += recvd;
                    pn.ParseNMEA();

                }

            }


        }        

    }
}
