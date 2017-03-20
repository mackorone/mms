public class Algo {
    public void generate(Interface interface_) {
        for (int i = 0; i < interface_.getWidth(); i += 1) {
            for (int j = 0; j < interface_.getHeight(); j += 1) {
                for (int k = 0; k < 4; k += 1) {
                    char direction = new String("nesw").charAt(k);
                    interface_.setWall(
                        i,
                        j,
                        direction,
                        interface_.getRandomFloat() < .4
                    );
                }
            }
        }
    }
}
