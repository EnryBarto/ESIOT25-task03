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
    private final static int VALVE_CLOSED = 0;
    private final static int VALVE_HALF   = 50;
    private final static int VALVE_OPEN   = 100;
    private final static int HYSTERESIS_TIME = 5000; // T1 period

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
		HttpServer httpServer = new HttpServer(port, this.tms, this.wcs, this);
		vertx.deployVerticle(httpServer);
    }

    @Override
    public boolean toggleMode() {
        if (this.currentState == State.AUTO || this.currentState == State.MANUAL) {
            this.wcs.sendStateToggle();
            return true;
        } else {
            return false;
        }
    }

    @Override
    public boolean setValveOpening(final int value) {
        if (this.currentState == State.MANUAL) {
            if (value >= 0 && value <= 100) {
                this.wcs.sendSetValve(value);
                return true;
            }
        }
        return false;
    }

    @Override
    public State getState() {
        return this.currentState;
    }

    @Override
    public void run() {

        // Sync serial
        try {
            Thread.sleep(5000);
        } catch (InterruptedException e) { }

        while (true) {

            this.wcs.loopSerial();

            switch (this.currentState) {

                case AUTO:

                    if (this.wcs.isToggleModeRequested()) {
                        this.currentState = State.MANUAL;
                    } else if (this.tms.isNewValueReceived()) {
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
                                this.wcs.sendSetValve(VALVE_CLOSED);
                                break;
                            case CLOSED_WAITING:
                                this.timer = LocalDateTime.now();
                                break;
                            case HALF:
                                this.wcs.sendSetValve(VALVE_HALF);
                                break;
                            case OPEN:
                                this.wcs.sendSetValve(VALVE_OPEN);
                                break;
                            default:
                                break;
                        }
                        this.precAutoState = this.currentAutoState;
                        System.out.println("[    CUS    ] AUTO STATE CHANGED - New State: " + this.currentAutoState);
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
                    this.tms.isNewValueReceived(); // Consume the received value
                    if (this.wcs.isToggleModeRequested()) {
                        this.currentState = State.AUTO;
                    }
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
                        if (this.tms.getLastValue() >= L2) {
                            this.currentAutoState = AutoState.OPEN;
                            this.wcs.sendSetValve(VALVE_OPEN);
                        } else if (this.tms.getLastValue() < L1) {
                            this.currentAutoState = AutoState.CLOSED;
                            this.wcs.sendSetValve(VALVE_CLOSED);
                        } else {
                            this.currentAutoState = AutoState.CLOSED_WAITING;
                            this.timer = LocalDateTime.now();
                            this.wcs.sendSetValve(VALVE_CLOSED);
                        }
                        this.precAutoState = this.currentAutoState;
                        break;

                    case MANUAL:
                        break;

                    case UNCONNECTED_AUTO:
                    case UNCONNECTED_MANUAL:
                        this.wcs.sendUnconnected();
                        break;

                    default:
                        break;

                }
                this.precState = this.currentState;
                System.out.println("[    CUS    ] STATE CHANGED - New State: " + this.currentState);
            }

            try {
                Thread.sleep(200);
            } catch (InterruptedException e) { }
        }
    }
}