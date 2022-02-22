package brainfuck;

import brainfuck.exception.BrainfuckException;
import brainfuck.exception.UnclosedLoopException;
import brainfuck.exception.UnopenedLoopException;
import lombok.Getter;
import lombok.RequiredArgsConstructor;

import java.util.Arrays;
import java.util.Optional;

@RequiredArgsConstructor
public enum BrainfuckOperation {
    /**
     * All brainfuck operations
     * Every operation consists of a character code and an operation executor,
     * that is executed when this specific operation is interpreted.
     * The executor has access to the memory, program and inputs/outputs
     */
    PLUS('+', (m, p, io) -> m.inc()),
    MINUS('-', (m, p, io) -> m.dec()),
    LEFT('<', (m, p, io) -> m.left()),
    RIGHT('>', (m, p, io) -> m.right()),
    OUTPUT('.', (m, p, io) -> io.write((char) m.get())),
    INPUT(',', (m, p, io) -> m.set((short) io.read())),
    LOOPSTART('[', (m, p, io) -> {
        if (m.get() == 0) {
            p.setPointer(seekEndOfLoop(p));
        }
    }),
    LOOPEND(']', (m, p, io) -> {
        if (m.get() != 0) {
            p.setPointer(seekStartOfLoop(p));
        }
    });

    @Getter
    private final char code;
    private final OperationExecutor operationExecutor;

    public void execute(BrainfuckMemory memory, BrainfuckProgram program, BrainfuckIO io) throws BrainfuckException {
        operationExecutor.execute(memory, program, io);
    }

    public static Optional<BrainfuckOperation> valueByCode(char code) {
        return Arrays.stream(BrainfuckOperation.values())
                .filter(brainfuckOperation -> brainfuckOperation.getCode() == code)
                .findFirst();
    }

    private static int seekEndOfLoop(BrainfuckProgram program) throws UnclosedLoopException {
        int nestedLoops = 0;
        for (int i = program.pointer(); i < program.size(); i++) {
            switch (program.get(i)) {
                case LOOPSTART -> nestedLoops++;
                case LOOPEND -> {
                    if (--nestedLoops == 0) {
                        return i;
                    }
                }
            }
        }
        throw new UnclosedLoopException(program.pointer());
    }

    private static int seekStartOfLoop(BrainfuckProgram program) throws UnopenedLoopException {
        int nestedLoops = 0;
        for (int i = program.pointer(); i >= 0; i--) {
            switch (program.get(i)) {
                case LOOPEND -> nestedLoops++;
                case LOOPSTART -> {
                    if (--nestedLoops == 0) {
                        return i;
                    }
                }
            }
        }
        throw new UnopenedLoopException(program.pointer());
    }

    @FunctionalInterface
    public interface OperationExecutor {
        void execute(BrainfuckMemory memory, BrainfuckProgram program, BrainfuckIO io) throws BrainfuckException;
    }
}