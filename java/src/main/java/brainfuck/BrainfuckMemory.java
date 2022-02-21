package brainfuck;


public class BrainfuckMemory {
    /**
     * Byte only ranges from -128 to 127
     * A short is used. Its range is restricted to 0 to 255
     */
    private short[] memoryContents;
    public int pointerPosition;

    public BrainfuckMemory(int size, int initialPointerPosition) {
        this.memoryContents = new short[size];
        this.pointerPosition = initialPointerPosition;
    }

    public void inc() {
        if (memoryContents[pointerPosition] == 255) {
            memoryContents[pointerPosition] = 0;
            return;
        }
        memoryContents[pointerPosition]++;
    }

    public void dec() {
        if (memoryContents[pointerPosition] == 0) {
            memoryContents[pointerPosition] = 255;
            return;
        }
        memoryContents[pointerPosition]--;
    }

    public void left() {
        if (pointerPosition == 0) {
            expandMemory(0);
        }
        pointerPosition--;
    }

    public void right() {
        if (pointerPosition == memoryContents.length - 1) {
            expandMemory(1);
        }
        pointerPosition++;
    }

    public short get() {
        return memoryContents[pointerPosition];
    }

    public void set(short value) {
        memoryContents[pointerPosition] = value;
    }

    /**
     * Expands the memory into a given direction
     *
     * @param direction 0 for left, 1 for right
     */
    public void expandMemory(int direction) {
        short[] newMemory = new short[memoryContents.length * 2];
        if (direction == 0) {
            System.arraycopy(memoryContents, 0, newMemory, memoryContents.length, memoryContents.length);
            pointerPosition += memoryContents.length;

        } else {
            System.arraycopy(memoryContents, 0, newMemory, 0, memoryContents.length);
        }
        memoryContents = newMemory;
    }
}