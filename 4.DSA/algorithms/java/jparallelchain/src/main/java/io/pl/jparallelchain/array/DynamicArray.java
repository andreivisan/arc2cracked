package io.pl.jparallelchain.array;

import io.pl.jparallelchain.array.error.ErrorMessage;

public class DynamicArray<T> {
    private T[] data;
    private int capacity;
    private int size;

    private static enum SortingDirection {
        ASC,
        DESC
    }

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
        int bulkLen = values.length;
        if (bulkLen == 0) {
            return;
        }
        for (int i = 0; i < bulkLen; i++) {
            append(values[i]);
        }
    }

    /*
     * Retrieves a value at a given index.
     */
    public T get(int index) {
        if (index < 0)
            throw new IllegalArgumentException(String.format(ErrorMessage.NEGATIVE_INDEX.errorMessage(), index));
        if (index >= this.size)
            throw new IllegalArgumentException(String.format(ErrorMessage.INDEX_OVERFLOW.errorMessage(), this.size, index));
        return this.data[index];
    }

    /*
     * Removes a value at a given index
     * TODO: maybe a fast version would be to copy each element minus the deleted element
     * to a new array using the GPU
     */
    public void remove(int index) {
        if (index < 0)
            throw new IllegalArgumentException(String.format(ErrorMessage.NEGATIVE_INDEX.errorMessage(), index));
        if (index >= this.size)
            throw new IllegalArgumentException(String.format(ErrorMessage.INDEX_OVERFLOW.errorMessage(), this.size, index));
        // Added this for optimisation, otherwise the for loop
        // would loop through all elements until the last
        if (index == this.size - 1) {
            this.data[this.size - 1] = null;
            this.size--;
            return;
        }
        for (int i = index; i < this.size - 1; i++) {
            this.data[i] = this.data[i+1];
        }
        this.data[this.size - 1] = null;
        this.size--;
    }

    /*
     * Removes a list of values at given indices
     * Uses GPU to copy data and for parallel processing
     */
    public void bulkRemove(int[] indices) {
        //temp implementation before GPU
        if (indices.length == 0) {
            return;
        }
        for (int idx : indices) {
            remove(idx);
        }
    }

    /*
     * Sorts the array
     * The chosen algorithm is Bitonic Sort as it is opimised
     * Uses GPU to copy data and for parallel processing
     */ 
    public T[] sort(SortingDirection direction) {
        return null;
    }

    private boolean isPowOfTwoLong() {
        return this.data.length / 2 == 0;
    }

    private void pad(SortingDirection direction) {
        if (direction == SortingDirection.DESC) {
            // this.data.in
        }
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
