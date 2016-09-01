#include "Printer.h"

#include <iostream>
#include <string>

void Printer::print(
        std::vector<std::vector<std::map<char, bool> > >* maze) {

    // The characters to use in the file
    char post = '+';
    char space = ' ';
    char vertical = '|';
    char horizontal = '-';

    // A blank line, and a list of all lines to be written
    std::string blankLine(4 * maze->size() + 1, space);
    std::vector<std::string> upsideDownLines;
    upsideDownLines.push_back(blankLine);

    // For all tiles in the maze
    for (int i = 0; i < maze->size(); i += 1) {
        for (int j = 0; j < maze->at(i).size(); j += 1) {

            // Insert more lines if necessary
            if (upsideDownLines.size() <= 2 * j + 1) {
                upsideDownLines.push_back(blankLine);
                upsideDownLines.push_back(blankLine);
            }

            // Insert posts at the boundaries
            int left  = 4 * (i + 0);
            int right = 4 * (i + 1);
            int up    = 2 * (j + 1);
            int down  = 2 * (j + 0);
            upsideDownLines.at(down).at(left)  = post;
            upsideDownLines.at(down).at(right) = post;
            upsideDownLines.at(up).at(left)    = post;
            upsideDownLines.at(up).at(right)   = post;

            // Insert walls if they exist
            if (maze->at(i).at(j).at('n')) {
                for (int k = 0; k < 3; k += 1) {
                    upsideDownLines.at(up).at(left + 1 + k) = horizontal;
                }
            }
            if (maze->at(i).at(j).at('s')) {
                for (int k = 0; k < 3; k += 1) {
                    upsideDownLines.at(down).at(left + 1 + k) = horizontal;
                }
            }
            if (maze->at(i).at(j).at('e')) {
                upsideDownLines.at(down + 1).at(right) = vertical;
            }
            if (maze->at(i).at(j).at('w')) {
                upsideDownLines.at(down + 1).at(left) = vertical;
            }
        }
    }

    // Flip the lines so that they're right side up
    std::vector<std::string> rightSideUpLines;
    for (int i = upsideDownLines.size() - 1; i >= 0; i -= 1) {
        rightSideUpLines.push_back(upsideDownLines.at(i));
    }

    // Print the lines to stderr
    for (int i = 0; i < rightSideUpLines.size(); i += 1) {
        std::cerr << rightSideUpLines.at(i) << std::endl;
    }
}
