package it.unibo.iot03;

import io.vertx.core.Vertx;
import io.vertx.mqtt.MqttServer;

public class Launcher {

    public static void main(String[] args) {
        Vertx vertx = Vertx.vertx();
        MqttServer mqttServer = MqttServer.create(vertx);

        mqttServer
            .endpointHandler(new MyMqttHandler())
            .listen()
            .onComplete(asyncResult -> {
                if (asyncResult.succeeded()) {
                    System.out.println("MQTT server is listening on port " + asyncResult.result().actualPort());
                } else {
                    System.out.println("Error on starting the server");
                    asyncResult.cause().printStackTrace();
                }
            }
        );

    }
}
