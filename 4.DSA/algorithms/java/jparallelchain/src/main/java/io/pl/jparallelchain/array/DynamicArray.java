package io.pl.jparallelchain.array;

import io.pl.jparallelchain.array.error.ErrorMessage;

public class DynamicArray<T> {
    private T[] data;
    private int capacity;
    private int size;

    @SuppressWarnings("unchecked")
    public DynamicArray(int capacity) {
        if (capacity < 0) {
            throw new IllegalArgumentException(String.format(ErrorMessage.NEGATIVE_CAPACITY.errorMessage(), capacity));
        }
        this.capacity = capacity;
        data = (T[]) new Object[capacity];
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
     * TODO: Use GPU to copy data
     */
    @SuppressWarnings("unchecked")
    private T[] expandCapacity() {
        this.capacity *= 2;
        T[] temp = (T[]) new Object[capacity];
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
    public void append(T value) {
        if (this.size == this.capacity) {
            this.data = expandCapacity();
        }
        this.data[this.size] = value;
        this.size++;
    }

    /*
     * Adds a list of values to the array   
     * If max capacity is achieved the array is expanded
     * Uses GPU to copy data and for parallel processing
     * See docs for expand method
     */
    public void bulkAppend(T[] values) {
        // TODO: Implement bulk append
    }

    /*
     * Removes a value at a given index
     */
    public void remove(int index) {
        // TODO: Implement remove
    }

    /*
     * Removes a list of values at given indices
     * Uses GPU to copy data and for parallel processing
     */
    public void bulkRemove(int[] indices) {
        // TODO: Implement bulk remove
    }

    /*
     * Sorts the array
     * Uses GPU to copy data and for parallel processing
     */ 
    public T[] sort() {
        // TODO: Implement sort
        return null;
    }

    /*
     * Searches for a value in the array
     * Uses GPU to copy data and for parallel processing
     */
    public int search(T value) {
        // TODO: Implement search
        return -1;
    }

    public void print() {
        for (int i = 0; i < this.size; i++) {
            System.out.print(String.format("%s ", this.data[i]));
        }
    }
}
