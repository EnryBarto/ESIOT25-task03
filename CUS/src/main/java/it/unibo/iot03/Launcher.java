package it.unibo.iot03;

import javax.swing.JFrame;
import javax.swing.JOptionPane;

import it.unibo.iot03.controller.ControllerImpl;
import it.unibo.iot03.controller.Controller;

public class Launcher {

    private static final int DEFAULT_BAUD_RATE = 9600;
    private static final String DEFAULT_SERIAL_PORT = "COM10";
    private static final int DEFAULT_HTTP_PORT = 8888;
    private static final int DEFAULT_MQTT_PORT = 1883;

    public static void main(String[] args) {

        // Request connection parameters
        String serialPort = DEFAULT_SERIAL_PORT;
        int serialBaudRate = DEFAULT_BAUD_RATE;
        int httpPort = DEFAULT_HTTP_PORT;
        int mqttPort = DEFAULT_MQTT_PORT;

        final JFrame tempFrame = new JFrame("Parametri connessione");
        tempFrame.setLocationByPlatform(true);
        tempFrame.setAlwaysOnTop(true);
        tempFrame.setVisible(true);

        try {
            serialPort = JOptionPane.showInputDialog(tempFrame, "Porta seriale:", DEFAULT_SERIAL_PORT);
            serialBaudRate = Integer.parseInt(JOptionPane.showInputDialog(tempFrame, "Baud rate seriale:", DEFAULT_BAUD_RATE));
            httpPort = Integer.parseInt(JOptionPane.showInputDialog(tempFrame, "Porta server HTTP:", DEFAULT_HTTP_PORT));
            mqttPort = Integer.parseInt(JOptionPane.showInputDialog(tempFrame, "Porta broker MQTT:", DEFAULT_MQTT_PORT));
        } catch (Exception e) {
            JOptionPane.showMessageDialog(tempFrame, e.getMessage(), "ERRORE", JOptionPane.ERROR_MESSAGE);
            return;
        }

        tempFrame.dispose();

        Controller controller = new ControllerImpl();

        // Connect
        try {
            controller.startHttpServer(httpPort);
            controller.startMqttBroker(mqttPort);
            controller.startSerialCommunication(serialPort, serialBaudRate);
        } catch (Exception e) {
            JOptionPane.showMessageDialog(null, e.getMessage(), "ERRORE", JOptionPane.ERROR_MESSAGE);
        }

        try {
            controller.run();
        } catch (Exception e) {
            JOptionPane.showMessageDialog(tempFrame, e.getMessage(), "ERRORE", JOptionPane.ERROR_MESSAGE);
        }
    }
}
