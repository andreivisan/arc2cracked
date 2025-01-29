public class DynamicArray {
    private int[] data;
    private int capacity;
    private int size;

    public DynamicArray(int capacity) {
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

    public static void main(String args[]) {
        DynamicArray myArr = new DynamicArray(2);
        myArr.append(1);
        myArr.append(2);
        myArr.append(3);
        myArr.print();
    }
}
