package it.unibo.iot03.model.impl;

import it.unibo.iot03.com.CommChannel;
import it.unibo.iot03.model.api.Wcs;

public class WcsImpl implements Wcs {

    private CommChannel serialCom;
    private int valveLevel;
    private boolean toggleModeRequested;

    @Override
    public void setSerialConnection(final CommChannel channel) {
        this.serialCom = channel;
        this.toggleModeRequested = false;
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
    public void sendSetValve(final int i) {
        if (i >= 0 && i <= 100) {
            this.serialCom.sendMsg("3-" + i + "\n");
            this.valveLevel = i;
        }
    }

    @Override
    public int getValveLevel() {
        return this.valveLevel;
    }

    @Override
    public void loopSerial() {
        if (this.serialCom.isMsgAvailable()) {
            try {
                String buf = this.serialCom.receiveMsg();
                System.out.println("[    WCS    ] New serial message: " + buf);
                switch (buf.charAt(0)) {
                    case '0':
                        this.toggleModeRequested = true;
                        break;

                    case '3':
                        String val = buf.substring(2);
                        this.valveLevel = Integer.parseInt(val);
                        break;
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public boolean isToggleModeRequested() {
        boolean buf = this.toggleModeRequested;
        this.toggleModeRequested = false;
        return buf;
    }
}
