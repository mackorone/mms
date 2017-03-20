import java.util.ArrayList;
import java.util.HashMap;
import java.util.Random;

public class Main {

    public static void main(String[] args) {

        // Print the usage
        if (args.length < 2 || 3 < args.length) {
            System.out.println("Usage: <BINARY> <WIDTH> <HEIGHT> [<SEED>]");
            return;
        }

        // Read the width and height args
        int width = Integer.parseInt(args[0]);
        int height = Integer.parseInt(args[1]);
        if (width <= 0 || height <= 0) {
            System.out.println(
                "Error: <WIDTH> and <HEIGHT> must be positive integers"
            );
            System.exit(1);
        }

        // Read the seed arg
        Random random = new Random();
        if (args.length == 3) {
            int seed = Integer.parseInt(args[2]);
            if (seed <= 0) {
                System.out.println("Error: <SEED> must be a positive integer");
                System.exit(1);
            }
            random = new Random(seed);
        }

        // Generate an empty maze
        ArrayList<ArrayList<HashMap<Character, Boolean>>> maze =
            new ArrayList<ArrayList<HashMap<Character, Boolean>>>();
        for (int i = 0; i < width; i += 1) {
            ArrayList<HashMap<Character, Boolean>> column =
                new ArrayList<HashMap<Character, Boolean>>();
            for (int j = 0; j < height; j += 1) {
                HashMap<Character, Boolean> cell =
                    new HashMap<Character, Boolean>();
                cell.put('n', false);
                cell.put('e', false);
                cell.put('s', false);
                cell.put('w', false);
                column.add(cell);
            }
            maze.add(column);
        }

        BooleanHolder success = new BooleanHolder();
        success.set(true);
        Interface interface_ = new Interface(maze, random, success);
        Algo algo = new Algo();
        algo.generate(interface_);

        if (!success.get()) {
            System.exit(1);
        }

        Printer.print(maze);
    }

}
