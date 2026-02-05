package it.unibo.iot03.model.impl;

import java.time.LocalDateTime;
import java.time.temporal.ChronoUnit;
import java.util.List;
import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;

import it.unibo.iot03.model.Data;
import it.unibo.iot03.model.api.Tms;

public class TmsImpl implements Tms {

    public final static int MAX_VALUES = 100;   // Max number of elements to keep in the queue
    private final static int INVALID_VAL = -1;  // Value sent by the TMS when the read is invalid
    private LocalDateTime lastValueTime;        // Last time when a valid value was received from the TMS
    private final Queue<Data> history = new ConcurrentLinkedQueue<>();
    private int lastValue; // Last value, stored so we don't have to iterate all over the queue from the begin
    private final static int TIMEOUT_TIME = 7000; // T2 period
    private boolean isThereNewVal;

    public TmsImpl() {
        // We force the TMS in UNCONNECTED, setting the last valid value received as the timeout
        this.lastValueTime = LocalDateTime.now().minus(TIMEOUT_TIME, ChronoUnit.MILLIS);
        this.isThereNewVal = false;
    }

    @Override
    public void addValue(final int value) {
        if (value == INVALID_VAL) return; // We ignore invalid values
        if (this.history.size() >= MAX_VALUES) this.history.poll();
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
