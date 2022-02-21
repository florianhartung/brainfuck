package brainfuck.exception;

public class UnopenedLoopException extends BrainfuckException {
    public UnopenedLoopException(int lastMemoryPointerPosition) {
        super(lastMemoryPointerPosition);
    }
}