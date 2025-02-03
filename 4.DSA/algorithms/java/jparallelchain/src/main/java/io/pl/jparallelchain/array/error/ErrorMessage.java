package io.pl.jparallelchain.array.error;

public enum ErrorMessage {
    NEGATIVE_CAPACITY("Capacity of the array can't be negative. You initialised the array with capacity %d"),
    NEGATIVE_INDEX("Index cannot be negative. You tried to retrieve a value at index %d"),
    INDEX_OVERFLOW("The given index exceeds the array's size. The size of the array is %d and the given index is %d");

    private String errorMessage;

    private ErrorMessage(String errorMessage) {
        this.errorMessage = errorMessage;
    }

    public String errorMessage() {
        return this.errorMessage;
    }
}
