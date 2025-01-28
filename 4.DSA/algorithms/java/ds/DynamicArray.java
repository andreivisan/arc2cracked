public class DynamicArray {
    private int[] data;
    private int capacity;

    public DynamicArray(int capacity) {
        this.capacity = capacity;
        data = new int[capacity];
    }

    public int getCapacity() {
        return this.capacity;
    }

    /*
     * When max capacity is achieved the size of
     * the array will double.
     */
    private int[] expandCapacity() {
        this.capacity *= 2;
        int[] temp = new int[capacity];
        // copy data into temp
        for (int i = 0; i < this.data.length; i++) {
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
        System.out.println(this.data.length);
        if (this.capacity < this.data.length) {
            this.data = expandCapacity();
        }
        this.data[this.data.length - 1] = value;
    }

    public void print() {
        for (int i = 0; i < this.data.length; i++) {
            System.out.print(String.format("%d ", this.data[i]));
        }
    }

    public static void main(String args[]) {
        DynamicArray myArr = new DynamicArray(2);
        myArr.append(1);
        myArr.print();
    }
}


