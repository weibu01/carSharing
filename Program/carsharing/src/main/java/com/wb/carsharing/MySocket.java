package com.wb.carsharing;

import android.app.Application;

import java.net.Socket;

public class MySocket extends Application {
    Socket socket=null;
    public Socket getSocket()
    {
        return socket;
    }
    public void setSocket(Socket socket)
    {
        this.socket=socket;
    }
}



