package it.unibo.iot03.model.api;

import java.time.LocalDateTime;
import java.util.List;

import it.unibo.iot03.model.Data;

public interface Tms {

    void addValue(int value);
    LocalDateTime getLastReceivedValueTime();
    List<Data> getHistory();
    boolean isUnconnected();
    boolean isNewValueReceived();
    int getLastValue();
}
