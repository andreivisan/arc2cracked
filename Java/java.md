# Most important features

## Pattern Matching

Declarative type-driven logic, unifying type checks, casting, and data extraction in one step.

### Key Features

**1. instanceof + Auto-Casting**

    - Replace:

    ```java
    if (obj instanceof String) {
        String s = (String) obj; // Manual cast
    }
    ```

    With

    ```java
    if (obj instanceof String s) { // Auto-cast to 's'
        // Use 's' directly
    }
    ```

**2. Pattern Matching in switch**



