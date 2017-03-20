import java.util.ArrayList;
import java.util.HashMap;
import java.util.Random;

public class Interface {

    ArrayList<ArrayList<HashMap<Character, Boolean>>> _maze;
    Random _random;
    BooleanHolder _success;

    public Interface(
        ArrayList<ArrayList<HashMap<Character, Boolean>>> maze,
        Random random,
        BooleanHolder success
    ) {
        _maze = maze;
        _random = random;
        _success = success;
    }

    int getWidth() {
        return _maze.size();
    }

    int getHeight() {
        return _maze.get(0).size();
    }

    double getRandomFloat() {
        return _random.nextDouble();
    }

    void setWall(int x, int y, char direction, boolean wallExists) {

        // Sanity checks
        if (x < 0 || getWidth() <= x) {
            System.out.println("Error: x value " + x + " is invalid");
            _success.set(false);
            return;
        }
        if (y < 0 || getHeight() <= y) {
            System.out.println("Error: x value " + y + " is invalid");
            _success.set(false);
            return;
        }
        if (direction != 'n' && direction != 'e' && direction != 's' && direction != 'w') {
            System.out.println(
                "Error: direction value '" + direction + "' is invalid"
            );
            _success.set(false);
            return;
        }

        // Set the wall value
        _maze.get(x).get(y).put(direction, wallExists);

        // Set the opposing wall value
        switch (direction) {
            case 'n': {
                if (y < getHeight() - 1) {
                    _maze.get(x).get(y + 1).put('s', wallExists);
                }
                break;
            }
            case 'e': {
                if (x < getWidth() - 1) {
                    _maze.get(x + 1).get(y).put('w', wallExists);
                }
                break;
            }
            case 's': {
                if (0 < y) {
                    _maze.get(x).get(y - 1).put('n', wallExists);
                }
                break;
            }
            case 'w': {
                if (0 < x) {
                    _maze.get(x - 1).get(y).put('e', wallExists);
                }
                break;
            }
        }
    }
}
