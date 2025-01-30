package io.pl.jparallelchain.array.error;

public enum ErrorMessage {
    NEGATIVE_CAPACITY("Capacity of the array can't be negative. You initialised the array with capacity %d");

    private String errorMessage;

    private ErrorMessage(String errorMessage) {
        this.errorMessage = errorMessage;
    }

    public String errorMessage() {
        return this.errorMessage;
    }
}
