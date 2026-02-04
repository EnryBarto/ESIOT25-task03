package it.unibo.iot03.model;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

public record Data(LocalDateTime timestamp, int value) {

    public Data(int value) {
        this(LocalDateTime.now(), value);
    }

    public String formattedTime() {
		return this.timestamp.format(DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss"));
    }

}
