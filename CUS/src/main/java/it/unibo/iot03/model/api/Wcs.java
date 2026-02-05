package it.unibo.iot03.model.api;

import it.unibo.iot03.com.CommChannel;

public interface Wcs {

    void setSerialConnection(CommChannel channel);
    void sendStateToggle();
    void sendUnconnected();
    void sendReconnected();
    void sendSetValve(int i);
    int getValveLevel();
    void loopSerial();
    boolean isToggleModeRequested();
}
