#include "Driver.h"
#include "DriverCli.h"
int main(int argc, char *argv[])
{
#ifdef CLI
    return mms::DriverCli::drive(argc, argv);
#else
    return mms::Driver::drive(argc, argv);
#endif
}
