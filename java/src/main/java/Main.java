import brainfuck.BrainfuckInterpreter;
import brainfuck.exception.BrainfuckException;
import brainfuck.exception.UnclosedLoopException;
import brainfuck.exception.UnopenedLoopException;

import java.io.*;
import java.nio.charset.StandardCharsets;

public class Main {
    public static void main(String[] args) {
        if (args.length == 0) {
            System.out.println("No brainfuck program given!");
            return;
        }
        File programFile = new File(args[0]);
        String program;
        try {
            program = readFile(programFile);
        } catch (IOException e) {
            System.out.println("Given brainfuck program file does not exist!");
            return;
        }

        InputStream programInputStream = System.in;
        if (args.length == 2) {
            programInputStream = new ByteArrayInputStream(args[1].getBytes(StandardCharsets.US_ASCII));
        }

        BrainfuckInterpreter interpreter = new BrainfuckInterpreter(64,
                32,
                programInputStream,
                System.out);

        try {
            interpreter.run(program);
        } catch (UnclosedLoopException e) {
            System.out.printf("""
                            Error whilst interpreting brainfuck program:
                            Unclosed loop starting at position %d found.
                            Consider closing this loop with ']'!""",
                    e.getLastMemoryPointerPosition());
        } catch (UnopenedLoopException e) {
            System.out.printf("""
                            Error whilst interpreting brainfuck program:
                            Unopened loop ending at position %d found.
                            Consider opening this loop with '['!""",
                    e.getLastMemoryPointerPosition());
        } catch (BrainfuckException e) {
            e.printStackTrace();
        }


    }

    private static String readFile(File file) throws IOException {
        StringBuilder fileStringBuilder = new StringBuilder();

        BufferedReader programReader = new BufferedReader(new FileReader(file));
        int buffer;
        while ((buffer = programReader.read()) != -1) {
            fileStringBuilder.append((char) buffer);
        }
        programReader.close();
        return fileStringBuilder.toString();
    }
}