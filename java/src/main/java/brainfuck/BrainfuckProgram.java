package brainfuck;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class BrainfuckProgram implements Iterator<BrainfuckOperation>, Iterable<BrainfuckOperation> {
    private final List<BrainfuckOperation> programOperations;
    private int programPointer;

    public BrainfuckProgram(String programString) {
        programOperations = new ArrayList<>();
        for (char c : programString.toCharArray()) {
            BrainfuckOperation.valueByCode(c).ifPresent(programOperations::add);
        }
        programPointer = -1;
    }

    public int size() {
        return programOperations.size();
    }


    public BrainfuckOperation get(int index) {
        return programOperations.get(index);
    }

    public int pointer() {
        return programPointer;
    }

    public void setPointer(int newPointerPosition) {
        programPointer = newPointerPosition;
    }

    @Override
    public boolean hasNext() {
        return programPointer + 1 < programOperations.size();
    }

    @Override
    public BrainfuckOperation next() {
        if (hasNext()) {

            return programOperations.get(++programPointer);
        }
        return null;
    }

    @Override
    public Iterator<BrainfuckOperation> iterator() {
        return this;
    }
}