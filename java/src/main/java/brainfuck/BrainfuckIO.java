package brainfuck;

import lombok.RequiredArgsConstructor;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;

@RequiredArgsConstructor
public class BrainfuckIO {
    private final InputStream input;
    private final OutputStream output;

    public void write(char c) {
        try {
            output.write(c);
        } catch (IOException e) {
            System.err.printf("Could not write character %c to output stream %s", c, output.getClass().toString());
        }
    }

    public char read() {
        try {
            if (input.equals(System.in)) {
                System.out.print("Input character: ");
                return (char) new InputStreamReader(input).read();
            } else {
                return (char) input.read();
            }
        } catch (Exception e) {
            System.err.printf("Could not read character from input stream %s", input.getClass().toString());
            return (char) 0;
        }
    }

    public void close() {
        try {
            output.flush();
            input.close();
            output.close();
        } catch (IOException e) {
            System.err.println("Could not flush/close io streams!");
        }
    }
}