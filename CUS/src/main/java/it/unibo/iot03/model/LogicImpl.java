package it.unibo.iot03.model;

import java.time.LocalDateTime;
import java.util.List;
import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;

public class LogicImpl implements Logic {

    private State currentState = State.AUTOMATIC;
    private LocalDateTime lastValueTime;
    private final Queue<Data> history = new ConcurrentLinkedQueue<>();

    @Override
    public State getCurrentState() {
        return this.currentState;
    }

    @Override
    public void addValue(final int value) {
        this.history.add(new Data(value));
        this.lastValueTime = LocalDateTime.now();
    }

    @Override
    public LocalDateTime getLastReceivedValueTime() {
        return this.lastValueTime;
    }

    @Override
    public List<Data> getHistory() {
        return this.history.stream().toList();
    }
}
