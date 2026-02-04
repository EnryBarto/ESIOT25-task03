package it.unibo.iot03;

import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import io.netty.handler.codec.mqtt.MqttProperties;
import io.netty.handler.codec.mqtt.MqttQoS;
import io.vertx.core.Handler;
import io.vertx.mqtt.MqttEndpoint;
import io.vertx.mqtt.MqttTopicSubscription;
import io.vertx.mqtt.messages.codes.MqttSubAckReasonCode;


public class MyMqttHandler implements Handler<MqttEndpoint> {

    // Map to store the list of the endpoints subscribed to every topic
    private static final Map<String, List<MqttEndpoint>> subscriptions = new ConcurrentHashMap<>();

    @Override
    public void handle(MqttEndpoint endpoint) {

        // Show connections info
        System.out.println(
            "MQTT client [" + endpoint.clientIdentifier() +
            "] request to connect, clean session = " + endpoint.isCleanSession()
        );

        // Show auth details if autenticated
        if (endpoint.auth() != null) {
            System.out.println(
                "[username = " + endpoint.auth().getUsername() + ", password = " + endpoint.auth().getPassword() + "]"
            );
        }

        // Show connection propreties
        System.out.println("[properties = " + endpoint.connectProperties() + "]");
        if (endpoint.will() != null) {
            byte[] willMessageBytes = endpoint.will().getWillMessageBytes();
            String willMessage = (willMessageBytes != null) ? new String(willMessageBytes) : "";
            System.out.println(
                "[will topic = " + endpoint.will().getWillTopic() + " msg = " + willMessage +
                " QoS = " + endpoint.will().getWillQos() + " isRetain = " + endpoint.will().isWillRetain() + "]"
            );
        }
        System.out.println("[keep alive timeout = " + endpoint.keepAliveTimeSeconds() + "]");

        // Accept connection from the remote client
        endpoint.accept(false);

        // The handler executed when a endpoint subscribes to a topic
        endpoint.subscribeHandler(subscribe -> {

            List<MqttSubAckReasonCode> reasonCodes = new ArrayList<>();
            for (MqttTopicSubscription s : subscribe.topicSubscriptions()) {
                System.out.println("Subscription for " + s.topicName() + " with QoS " + s.qualityOfService());
                subscriptions.computeIfAbsent(s.topicName(), k -> new ArrayList<>()).add(endpoint);
                reasonCodes.add(MqttSubAckReasonCode.qosGranted(s.qualityOfService()));
            }
            // ack the subscriptions request
            endpoint.subscribeAcknowledge(subscribe.messageId(), reasonCodes, MqttProperties.NO_PROPERTIES);
        });

        // The handler executed when a endpoit publish a message
        endpoint.publishHandler(message -> {

            System.out.println(
                "Just received message [" + message.payload().toString(Charset.defaultCharset())
                + "] with QoS [" + message.qosLevel() + "]"
            );
            // Alerts all the endpoint subscribed to the topic, except the sender
            List<MqttEndpoint> subscribers = subscriptions.get(message.topicName());
            if (subscribers != null) {
                for (MqttEndpoint subscriber : subscribers) {
                    if (subscriber != endpoint) {
                        subscriber.publish(
                            message.topicName(),
                            message.payload(),
                            message.qosLevel(),
                            message.isDup(),
                            message.isRetain()
                        );
                    }
                }
            }

            // Handle the ACK based on the QoS of the message
            switch (message.qosLevel()) {
                case MqttQoS.AT_LEAST_ONCE:
                    endpoint.publishAcknowledge(message.messageId());
                    break;

                case MqttQoS.EXACTLY_ONCE:
                    endpoint.publishReceived(message.messageId());
                    break;

                default:
            }
        })
        .publishReleaseHandler(endpoint::publishComplete);

        endpoint.disconnectHandler(v -> {
            System.out.println("Client [" + endpoint.clientIdentifier() + "] disconnected");
            subscriptions.values().forEach(list -> list.remove(endpoint));
        });
    }
}
