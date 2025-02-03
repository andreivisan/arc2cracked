package io.pl.jparallelchain.array;

import static org.junit.jupiter.api.Assertions.assertThrows;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.ValueSource;

public class DynamicArrayTest {
    
    @Test
    public void append_CapacityDoubles_whenLimitReached() {
        DynamicArray<Integer> myArr = new DynamicArray<>(2);
        myArr.append(1);
        myArr.append(2);
        myArr.append(3);

        Assertions.assertEquals(myArr.size(), 3);
        Assertions.assertEquals(myArr.capacity(), 4);
    }

    @Test
    public void append_String() {
        DynamicArray<String> myArr = new DynamicArray<>(2);
        myArr.append("Test");

        Assertions.assertEquals(myArr.size(), 1);
        Assertions.assertEquals(myArr.get(0), "Test");
    }

    //TODO Test get and test error throwing for get

    @ParameterizedTest
    @ValueSource(ints = {-100, -2, -999})
    public void constructor_ThrowsException_ForNegativeCapacity(int invalidCapacity) {
        assertThrows(
                IllegalArgumentException.class,
                () -> new DynamicArray<>(invalidCapacity)
        );
    }
}
