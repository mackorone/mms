// TODO: MACK
// #include "Printer.h"
// 
// #include <iostream>
// 
// void Printer::print(
//         const std::string& fname) {
//     print(fname, "", "", "", "");
// }
// 
// void Printer::print(
//         const std::string& fname,
//         const std::string& argOne) {
//     print(fname, argOne, "", "", "");
// }
// 
// void Printer::print(
//         const std::string& fname,
//         const std::string& argOne,
//         const std::string& argTwo) {
//     print(fname, argOne, argTwo, "", "");
// }
// 
// void Printer::print(
//         const std::string& fname,
//         const std::string& argOne,
//         const std::string& argTwo,
//         const std::string& argThree) {
//     print(fname, argOne, argTwo, argThree, "");
// }
// 
// void Printer::print(
//         const std::string& fname,
//         const std::string& argOne,
//         const std::string& argTwo,
//         const std::string& argThree,
//         const std::string& argFour) {
// 
//     static const std::string delimiter = " ";
// 
//     #define PRINT_IF_NONEMPTY(arg)\
//     if (arg != "") {\
//         std::cerr << delimiter;\
//         std::cerr << arg;\
//     }
// 
//     std::cerr << "\"";
//     std::cerr << fname;
// 
//     PRINT_IF_NONEMPTY(argOne);
//     PRINT_IF_NONEMPTY(argTwo);
//     PRINT_IF_NONEMPTY(argThree);
//     PRINT_IF_NONEMPTY(argFour);
// 
//     std::cerr << "\"";
//     std::cerr << std::endl;
// }
