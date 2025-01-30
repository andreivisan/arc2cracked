package io.pl.jparallelchain.array;

import io.pl.jparallelchain.array.error.ErrorMessage;

public class DynamicArray {
    private int[] data;
    private int capacity;
    private int size;

    public DynamicArray(int capacity) {
        if (capacity < 0) {
            throw new IllegalArgumentException(String.format(ErrorMessage.NEGATIVE_CAPACITY.errorMessage(), capacity));
        }
        this.capacity = capacity;
        data = new int[capacity];
        this.size = 0;
    }

    /*
     * Returns the capacity of the array
     */
    public int capacity() {
        return this.capacity;
    }

    /*
     * Returns the size of the array
     */
    public int size() {
        return this.size;
    }

    /*
     * When max capacity is achieved the size of
     * the array will double.
     */
    private int[] expandCapacity() {
        this.capacity *= 2;
        int[] temp = new int[capacity];
        // copy data into temp
        for (int i = 0; i < this.size; i++) {
            temp[i] = this.data[i];
        }
        return temp;
    }

    /*
     * Adds a value at the end of the array
     * If max capacity is achieved the array is expanded
     * See docs for expand method
     */
    public void append(int value) {
        if (this.size == this.capacity) {
            this.data = expandCapacity();
        }
        this.data[this.size] = value;
        this.size++;
    }

    public void print() {
        for (int i = 0; i < this.size; i++) {
            System.out.print(String.format("%d ", this.data[i]));
        }
    }
}
