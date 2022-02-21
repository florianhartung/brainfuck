package brainfuck.exception;

public class UnclosedLoopException extends BrainfuckException {
    public UnclosedLoopException(int lastMemoryPointerPosition) {
        super(lastMemoryPointerPosition);
    }
}