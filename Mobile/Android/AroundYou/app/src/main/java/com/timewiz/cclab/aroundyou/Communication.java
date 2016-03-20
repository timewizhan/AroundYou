package com.timewiz.cclab.aroundyou;

import android.util.Log;

import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.net.SocketTimeoutException;

/**
 * Created by jmhan on 2016-03-14.
 */
public class Communication {
    private Connection m_connection;
    private JSONFactory m_JSONFactory;

    public Communication() {
        m_connection = Connection.getConnection();
        m_JSONFactory = JSONFactory.getJSONInstance();
    }

    public Errno communicateWithServer(E_PROTOCOL_TYPE eType, PROTOCOL_ROOT protoRoot, String strOutData) {
        Errno errno = Errno.E_RET_FAIL;
        try {
            errno = tryToConnect();
            if (errno != Errno.E_RET_SUCCESS) {
                throw new Exception();
            }

            switch (eType) {
                case E_PROTO_LOGIN_101:
                    errno = Communication_Proto_201(protoRoot, strOutData);
                    break;
                case E_PROTO_MAIN_201:
                    break;
                case E_PROTO_MAIN_202:
                    break;
                default:
                    // nothing
            }
        }
        catch (Exception e) {
            Log.e("Communication", "Fail to communicate with server");
        }
        return errno;
    }

    private  int m_nReTry = 1;
    private Errno tryToConnect() {
        Errno errno = Errno.E_RET_SUCCESS;

        int nCurrentReTry = 0;
        while (nCurrentReTry <= m_nReTry) {
            errno = m_connection.createConection();
            if (errno == Errno.E_RET_SUCCESS)
                break;

            nCurrentReTry++;
            Log.i("Communication", "ReTry to connect to server");
        }
        return errno;
    }

    private Errno Communication_Header_Common(E_PROTOCOL_REQUEST_TYPE eNextReqToSend, int nSizeOfNextData) {
        Errno errno = Errno.E_RET_SUCCESS;

        String strDefaultReq = m_JSONFactory.buildReqDefaultArg(eNextReqToSend, nSizeOfNextData);
        errno = m_connection.sendDataToServer(strDefaultReq);
        if (errno != Errno.E_RET_SUCCESS) {
            Log.e("Communication", "Fail to send default req");
            return errno;
        }

        String strRecvData = m_connection.recvDataFromServer(0);
        PROTOCOL_RES_DEFAULT protoResDefault = m_JSONFactory.buildResDefault(strRecvData);
        if (protoResDefault.nResponse != E_PROTOCOL_RESPONSE_TYPE.E_PROTO_RES_READY_FOR_DATA.ordinal()) {
            Log.e("Communication", "Not match with response protocol");
            errno = Errno.E_RET_FAIL;
            return errno;
        }
        return errno;
    }

    private String Communication_Common(String strDataToSend, int nRecvSize) {
        Errno errno = Errno.E_RET_SUCCESS;

        errno = m_connection.sendDataToServer(strDataToSend);
        if (errno != Errno.E_RET_SUCCESS) {
            Log.e("Communication", "Fail to send middle request");
            return "";
        }
       return m_connection.recvDataFromServer(nRecvSize);
    }

    /*
        Login Activity
        101 : recommended store
    */
    private Errno Communication_Proto_101() {
        Errno errno = Errno.E_RET_SUCCESS;

        return errno;
    }

    /*
        Main Activity
        201 : recommended store
        202 : recommended menu
     */
    private Errno Communication_Proto_201(PROTOCOL_ROOT protoRoot, String strOutData) {
        Errno errno = Errno.E_RET_FAIL;

        PROTOCOL_REQ_201_RECOMMENDED_STORE protoReqRecommendedStore = (PROTOCOL_REQ_201_RECOMMENDED_STORE) protoRoot;
        String strProtoReqRecommendedStore = m_JSONFactory.buildReqForProto201(protoReqRecommendedStore);

        errno = Communication_Header_Common(E_PROTOCOL_REQUEST_TYPE.E_PROTO_REQ_RECOMMENDED_STORE, strProtoReqRecommendedStore.length());
        if (errno != Errno.E_RET_SUCCESS) {
            return errno;
        }

        String strRecvData;
        strRecvData = Communication_Common(strProtoReqRecommendedStore, 0);
        PROTOCOL_RESQ_DEFAULT_ARG protoResDefaultArg = m_JSONFactory.buildResDefaultArg(strRecvData);
        if (protoResDefaultArg.nResponse != E_PROTOCOL_RESPONSE_TYPE.E_PROTO_RES_READY_FOR_DATA.ordinal()) {
            Log.e("Communication", "Not match with response protocol");
            return errno;
        }

        String strProtoResRecommendedStore = m_JSONFactory.buildReqDefault(E_PROTOCOL_REQUEST_TYPE.E_PROTO_REQ_READY_FOR_DATA);
        strRecvData = Communication_Common(strProtoResRecommendedStore, protoResDefaultArg.nNextSize);
        if (protoResDefaultArg.nResponse != E_PROTOCOL_RESPONSE_TYPE.E_PROTO_RES_SUCCESS.ordinal()) {
            Log.e("Communication", "Not match with response protocol");
            return errno;
        }

        strOutData = strRecvData;
        return errno;
    }
}

class Connection {
    private final String m_strIPAddress = "";
    private final int m_nPort = 1234;
    private Socket m_socket;

    private Connection() {
        // For singleton
    }

    private static Connection m_connection;
    public static Connection getConnection() {
        if (m_connection != null) {
            m_connection = new Connection();
        }
        return m_connection;
    }

    private final int m_nTimeout = 5000;
    public Errno createConection() {
        SocketAddress socketAddress = new InetSocketAddress(m_strIPAddress, m_nPort);
        m_socket = new Socket();
        try {
            // timeout unit is milliseconds
            m_socket.connect(socketAddress, m_nTimeout);
        }
        catch (SocketTimeoutException e) {
            Log.i("Comunication", "Timeout was occured");
            return Errno.E_RET_FAIL;
        }
        catch (Exception e) {
            Log.i("Comunication", "Unknown error was occured");
            return Errno.E_RET_FAIL;
        }
        return Errno.E_RET_SUCCESS;
    }

    public Errno sendDataToServer(String strInputData) {
        Errno errno = Errno.E_RET_SUCCESS;

        try {
            BufferedWriter bufferedWriter = new BufferedWriter(new OutputStreamWriter(m_socket.getOutputStream()));
            bufferedWriter.write(strInputData, 0, strInputData.length());
            bufferedWriter.close();
        }
        catch (IOException e) {
            Log.e("Communication", "Fail to send data to server");
            errno = Errno.E_RET_FAIL;
        }
        return errno;
    }

    public String recvDataFromServer(int nRecvSize) {
        Errno errno = Errno.E_RET_SUCCESS;

        int nRecvSizeFromServer = nRecvSize;
        if (nRecvSizeFromServer == 0) {
            // 512 byte is default size for receiving from server
            nRecvSizeFromServer = 512;
        }

        String strRecvData = null;
        try {
            BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(m_socket.getInputStream()));

            char[] szRecvData = new char[nRecvSizeFromServer];
            bufferedReader.read(szRecvData, 0, nRecvSizeFromServer);
            strRecvData = new String(szRecvData, 0, szRecvData.length);
        }
        catch (IOException e) {
            Log.e("Communication", "Fail to receive data from server");
        }
        return strRecvData;
    }

    public Errno closeConnection() {
        Errno errno = Errno.E_RET_SUCCESS;

        try {
            m_socket.close();
        }
        catch (IOException e) {
            Log.e("Communication", "Fail to close connection");
            errno = Errno.E_RET_FAIL;
        }
        return errno;
    }
}
