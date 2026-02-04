package it.unibo.iot03.controller;

import io.vertx.core.Vertx;
import io.vertx.mqtt.MqttServer;
import io.vertx.mqtt.MqttServerOptions;
import it.unibo.iot03.com.CommChannel;
import it.unibo.iot03.com.HttpServer;
import it.unibo.iot03.com.MyMqttHandler;
import it.unibo.iot03.com.SerialCommChannel;
import it.unibo.iot03.model.Logic;
import it.unibo.iot03.model.Logic.State;
import it.unibo.iot03.model.LogicImpl;

public class ControllerImpl implements Controller {

    private CommChannel serialCom;
    private final Vertx vertx;
    private final Logic logic;

    public ControllerImpl() {
        this.vertx = Vertx.vertx();
        this.logic = new LogicImpl();
    }

    @Override
    public void startSerialCommunication(final String portName, final int baudRate) throws Exception {
        if (this.serialCom != null) {
            this.serialCom.close();
        }
        this.serialCom = new SerialCommChannel(portName, baudRate);
    }

    @Override
    public void startMqttBroker(final int port) {
        MqttServerOptions options = new MqttServerOptions().setPort(port);
        MqttServer mqttServer = MqttServer.create(vertx, options);
        mqttServer
            .endpointHandler(new MyMqttHandler(this.logic))
            .listen()
            .onComplete(asyncResult -> {
                if (asyncResult.succeeded()) {
                    System.out.println("[MQTT SERVER] MQTT server is listening on port " + asyncResult.result().actualPort());
                } else {
                    System.out.println("[MQTT SERVER] Error on starting the server");
                    asyncResult.cause().printStackTrace();
                }
            }
        );
    }

    @Override
    public void startHttpServer(final int port) {
		HttpServer httpServer = new HttpServer(port, this.logic);
		vertx.deployVerticle(httpServer);
    }

    @Override
    public void run() {
        while (true) {
            switch (this.logic.getCurrentState()) {
                default:
                    break;
            }
        }
    }

}
