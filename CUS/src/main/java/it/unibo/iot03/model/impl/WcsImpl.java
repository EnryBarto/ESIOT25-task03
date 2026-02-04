package it.unibo.iot03.model.impl;

import it.unibo.iot03.com.CommChannel;
import it.unibo.iot03.model.api.Wcs;

public class WcsImpl implements Wcs {

    private CommChannel serialCom;

    @Override
    public void setSerialConnection(final CommChannel channel) {
        this.serialCom = channel;
    }

    @Override
    public void sendStateToggle() {
        this.serialCom.sendMsg("0\n");
    }

    @Override
    public void sendUnconnected() {
        this.serialCom.sendMsg("1\n");
    }

    @Override
    public void sendReconnected() {
        this.serialCom.sendMsg("2\n");
    }

    @Override
    public void sendSetValve(int i) {
        if (i >= 0 && i <= 100) {
            this.serialCom.sendMsg("3-" + i + "\n");
        }
    }
}
