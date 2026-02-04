package it.unibo.iot03.model;

import java.time.LocalDateTime;
import java.util.List;

public interface Logic {

    public enum State {
        AUTOMATIC,
        MANUAL,
        UNCONNECTED
    }

    State getCurrentState();
    void addValue(int value);
    LocalDateTime getLastReceivedValueTime();
    List<Data> getHistory();
}
