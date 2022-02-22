package brainfuck.exception;

import lombok.Getter;

@Getter
public class BrainfuckException extends Exception {
    private final int lastMemoryPointerPosition;

    public BrainfuckException(int lastMemoryPointerPosition) {
        this(String.format("Could not interpret program at position %d", lastMemoryPointerPosition),
                lastMemoryPointerPosition);
    }

    public BrainfuckException(String message, int lastMemoryPointerPosition) {
        super(message);
        this.lastMemoryPointerPosition = lastMemoryPointerPosition;
    }
}