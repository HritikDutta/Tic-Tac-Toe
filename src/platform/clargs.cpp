#include "clargs.h"

#include <cstring>
#include <iostream>

CL_Args ParseCommandLineArguments(int argc, const char* argv[])
{
    CL_Args args = { 0 };

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            if (strcmp(argv[i], "-debug") == 0)
            {
                args.debug = true;
                continue;
            }

            if (strcmp(argv[i], "-fullscreen") == 0)
            {
                args.fullscreen = true;
                continue;
            }

            std::cout << "Flag '" << argv[i] << "' not recognised" << std::endl;
        }
        else
        {
            std::cout << "Argument '" << argv[i] << "' not recognised" << std::endl;
        }
    }

    return args;
}