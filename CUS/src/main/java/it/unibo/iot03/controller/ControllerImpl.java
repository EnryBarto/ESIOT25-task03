package it.unibo.iot03.controller;

import java.time.LocalDateTime;
import java.time.temporal.ChronoUnit;

import io.vertx.core.Vertx;
import io.vertx.mqtt.MqttServer;
import io.vertx.mqtt.MqttServerOptions;
import it.unibo.iot03.com.CommChannel;
import it.unibo.iot03.com.HttpServer;
import it.unibo.iot03.com.MyMqttHandler;
import it.unibo.iot03.com.SerialCommChannel;
import it.unibo.iot03.model.api.Tms;
import it.unibo.iot03.model.api.Wcs;
import it.unibo.iot03.model.impl.TmsImpl;
import it.unibo.iot03.model.impl.WcsImpl;

public class ControllerImpl implements Controller {

    private final static int L1 = 20;
    private final static int L2 = 60;
    private final static int HYSTERESIS_TIME = 5000;

    private State currentState;
    private State precState;
    private AutoState currentAutoState;
    private AutoState precAutoState;

    private CommChannel serialCom;
    private final Vertx vertx;
    private final Tms tms;
    private final Wcs wcs;
    private LocalDateTime timer;

    public ControllerImpl() {
        this.vertx = Vertx.vertx();
        this.tms = new TmsImpl();
        this.wcs = new WcsImpl();
        this.currentState = State.AUTO;
        this.precState = State.NONE;
        this.currentAutoState = AutoState.CLOSED;
        this.precAutoState = AutoState.NONE;
    }

    @Override
    public void startSerialCommunication(final String portName, final int baudRate) throws Exception {
        if (this.serialCom != null) {
            this.serialCom.close();
        }
        this.serialCom = new SerialCommChannel(portName, baudRate);
        this.wcs.setSerialConnection(this.serialCom);
    }

    @Override
    public void startMqttBroker(final int port) {
        MqttServerOptions options = new MqttServerOptions().setPort(port);
        MqttServer mqttServer = MqttServer.create(vertx, options);
        mqttServer
            .endpointHandler(new MyMqttHandler(this.tms))
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
		HttpServer httpServer = new HttpServer(port, this.tms);
		vertx.deployVerticle(httpServer);
    }

    @Override
    public void run() {

        // Sync serial
        try {
            Thread.sleep(5000);
        } catch (InterruptedException e) { }

        while (true) {

            if (this.serialCom.isMsgAvailable()) {
                try {
                    System.out.println(this.serialCom.receiveMsg());
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            switch (this.currentState) {

                case AUTO:

                    if (this.tms.isNewValueReceived()) {
                        switch (this.currentAutoState) {
                            case CLOSED:
                                if (this.tms.getLastValue() >= L2) {
                                    this.currentAutoState = AutoState.OPEN;
                                } else if (this.tms.getLastValue() >= L1) {
                                    this.currentAutoState = AutoState.CLOSED_WAITING;
                                }
                                break;

                            case CLOSED_WAITING:
                                if (this.tms.getLastValue() < L1) {
                                    this.currentAutoState = AutoState.CLOSED;
                                } else if (this.tms.getLastValue() >= L2) {
                                    this.currentAutoState = AutoState.OPEN;
                                } else if (ChronoUnit.MILLIS.between(this.timer, LocalDateTime.now()) >= HYSTERESIS_TIME) {
                                    this.currentAutoState = AutoState.HALF;
                                }
                                break;

                            case HALF:
                                if (this.tms.getLastValue() >= L2) {
                                    this.currentAutoState = AutoState.OPEN;
                                } else if (this.tms.getLastValue() < L1) {
                                    this.currentAutoState = AutoState.CLOSED;
                                }
                                break;

                            case OPEN:
                                if (this.tms.getLastValue() < L1) {
                                    this.currentAutoState = AutoState.CLOSED;
                                } else if (this.tms.getLastValue() < L2) {
                                    this.currentAutoState = AutoState.HALF;
                                }
                                break;
                            default:
                                break;
                        }
                    }

                    if (this.precAutoState != this.currentAutoState) {
                        switch (this.currentAutoState) {
                            case CLOSED:
                                this.wcs.sendSetValve(0);
                                break;
                            case CLOSED_WAITING:
                                this.timer = LocalDateTime.now();
                                break;
                            case HALF:
                                this.wcs.sendSetValve(50);
                                break;
                            case OPEN:
                                this.wcs.sendSetValve(100);
                                break;
                            default:
                                break;
                        }
                        this.precAutoState = this.currentAutoState;
                        System.out.println("[CUS] CAMBIO STATO AUTO: ENTRO IN " + this.currentAutoState);
                    }
                    break;

                case UNCONNECTED_AUTO:
                    if (this.tms.isNewValueReceived()) {
                        this.currentState = State.AUTO;
                        this.wcs.sendReconnected();
                    }
                    break;

                case UNCONNECTED_MANUAL:
                    if (this.tms.isNewValueReceived()) {
                        this.currentState = State.MANUAL;
                        this.wcs.sendReconnected();
                    }
                    break;

                case MANUAL:
                    break;
                default:
                    break;
            }

            if (this.tms.isUnconnected()) {
                this.currentState = switch (this.currentState) {
                    case AUTO -> State.UNCONNECTED_AUTO;
                    case MANUAL -> State.UNCONNECTED_MANUAL;
                    default -> this.currentState;
                };
            }

            if (this.currentState != this.precState) {
                switch (this.currentState) {
                    case AUTO:
                        if (this.precState == State.MANUAL) this.wcs.sendStateToggle();
                        if (this.tms.getLastValue() >= L2) {
                            this.currentAutoState = AutoState.OPEN;
                        } else if (this.tms.getLastValue() < L1) {
                            this.currentAutoState = AutoState.CLOSED;
                        } else {
                            this.currentAutoState = AutoState.CLOSED_WAITING;
                            this.timer = LocalDateTime.now();
                        }
                        this.precAutoState = AutoState.NONE;
                        break;

                    case MANUAL:
                        if (this.precState == State.AUTO) this.wcs.sendStateToggle();
                        break;

                    case UNCONNECTED_AUTO:
                        this.wcs.sendUnconnected();
                        break;

                    case UNCONNECTED_MANUAL:
                        this.wcs.sendUnconnected();
                        break;

                    default:
                        break;

                }
                this.precState = this.currentState;
                System.out.println("[CUS] CAMBIO STATO: ENTRO IN " + this.currentState);
            }

            try {
                Thread.sleep(200);
            } catch (InterruptedException e) { }
        }
    }
}
