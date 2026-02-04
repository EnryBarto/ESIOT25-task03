package it.unibo.iot03.controller;

public interface Controller {

    void startSerialCommunication(String portName, int baudRate) throws Exception;
    void startMqttBroker(int port);
    void startHttpServer(int port);
    void run();
}
