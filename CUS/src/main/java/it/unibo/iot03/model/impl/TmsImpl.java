package it.unibo.iot03.model.impl;

import java.time.LocalDateTime;
import java.time.temporal.ChronoUnit;
import java.util.List;
import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;

import it.unibo.iot03.model.Data;
import it.unibo.iot03.model.api.Tms;

public class TmsImpl implements Tms {

    private LocalDateTime lastValueTime;
    private final Queue<Data> history = new ConcurrentLinkedQueue<>();
    private int lastValue;
    private final static int TIMEOUT_TIME = 10000;
    private boolean isThereNewVal;

    public TmsImpl() {
        this.lastValueTime = LocalDateTime.now().minus(TIMEOUT_TIME, ChronoUnit.MILLIS);
        this.isThereNewVal = false;
    }

    @Override
    public void addValue(final int value) {
        this.history.add(new Data(value));
        this.lastValueTime = LocalDateTime.now();
        this.isThereNewVal = true;
        this.lastValue = value;
    }

    @Override
    public LocalDateTime getLastReceivedValueTime() {
        return this.lastValueTime;
    }

    @Override
    public List<Data> getHistory() {
        return this.history.stream().toList();
    }

    @Override
    public boolean isUnconnected() {
        return ChronoUnit.MILLIS.between(this.lastValueTime, LocalDateTime.now()) >= TIMEOUT_TIME;
    }

    @Override
    public boolean isNewValueReceived() {
        boolean tmp = this.isThereNewVal;
        this.isThereNewVal = false;
        return tmp;
    }

    @Override
    public int getLastValue() {
        return this.lastValue;
    }
}
