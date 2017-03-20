import java.util.Arrays;
import java.util.ArrayList;
import java.util.HashMap;

public class Printer {

    private static String replace(String string, int index, char c) {
        return string.substring(0, index) + c + string.substring(index + 1);
    }

    public static void print(
        ArrayList<ArrayList<HashMap<Character, Boolean>>> maze
    ) {

        // The characters to use in the file
        char post = '+';
        char space = ' ';
        char vertical = '|';
        char horizontal = '-';

        // A blank line, and a list of all lines to be written
        char[] chars = new char[4 * maze.size() + 1];
        Arrays.fill(chars, space);
        String blankLine = new String(chars);
        ArrayList<String> upsideDownLines = new ArrayList<String>();
        upsideDownLines.add(blankLine);

        // For all tiles in the maze
        for (int i = 0; i < maze.size(); i += 1) {
            for (int j = 0; j < maze.get(i).size(); j += 1) {

                // Insert more lines if necessary
                if (upsideDownLines.size() <= 2 * j + 1) {
                    upsideDownLines.add(blankLine);
                    upsideDownLines.add(blankLine);
                }

                // Insert posts at the boundaries
                int left  = 4 * (i + 0);
                int right = 4 * (i + 1);
                int up    = 2 * (j + 1);
                int down  = 2 * (j + 0);
                upsideDownLines.set(down, replace(upsideDownLines.get(down), left, post));
                upsideDownLines.set(down, replace(upsideDownLines.get(down), right, post));
                upsideDownLines.set(up, replace(upsideDownLines.get(up), left, post));
                upsideDownLines.set(up, replace(upsideDownLines.get(up), right, post));

                // Insert walls if they exist
                if (maze.get(i).get(j).get('n')) {
                    for (int k = 0; k < 3; k += 1) {
                        upsideDownLines.set(
                            up, 
                            replace(upsideDownLines.get(up), left + 1 + k, horizontal)
                        );
                    }
                }
                if (maze.get(i).get(j).get('s')) {
                    for (int k = 0; k < 3; k += 1) {
                        upsideDownLines.set(
                            down, 
                            replace(upsideDownLines.get(down), left + 1 + k, horizontal)
                        );
                    }
                }
                if (maze.get(i).get(j).get('e')) {
                    upsideDownLines.set(
                        down + 1,
                        replace(upsideDownLines.get(down + 1), right, vertical)
                    );
                }
                if (maze.get(i).get(j).get('w')) {
                    upsideDownLines.set(
                        down + 1,
                        replace(upsideDownLines.get(down + 1), left, vertical)
                    );
                }
            }
        }

        // Flip the lines so that they're right side up
        ArrayList<String> rightSideUpLines = new ArrayList<String>();
        for (int i = upsideDownLines.size() - 1; i >= 0; i -= 1) {
            rightSideUpLines.add(upsideDownLines.get(i));
        }

        // Print the lines to stderr
        for (int i = 0; i < rightSideUpLines.size(); i += 1) {
            System.err.println(rightSideUpLines.get(i));
        }
    }

}
