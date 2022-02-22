package brainfuck;

import brainfuck.exception.BrainfuckException;
import lombok.RequiredArgsConstructor;

import java.io.InputStream;
import java.io.OutputStream;


@RequiredArgsConstructor
public class BrainfuckInterpreter {

    private final int memorySize;
    private final int initialPointerPosition;
    private final InputStream input;
    private final OutputStream output;

    public void run(String programString) throws BrainfuckException {
        BrainfuckProgram program = new BrainfuckProgram(programString);
        BrainfuckMemory memory = new BrainfuckMemory(memorySize, initialPointerPosition);
        BrainfuckIO io = new BrainfuckIO(input, output);

        for (BrainfuckOperation op : program) {
            op.execute(memory, program, io);
        }

        io.close();
    }
}