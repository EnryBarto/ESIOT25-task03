package it.unibo.iot03.controller;

public interface Controller {

    public enum State {
        AUTO,
        MANUAL,
        UNCONNECTED_AUTO,
        UNCONNECTED_MANUAL,
        NONE
    }

    public enum AutoState {
        CLOSED,
        CLOSED_WAITING,
        HALF,
        OPEN,
        NONE
    }

    void startSerialCommunication(String portName, int baudRate) throws Exception;
    void startMqttBroker(int port);
    void startHttpServer(int port);
    void run();
    boolean toggleMode();
    boolean setValveOpening(int value);
    State getState();
}
