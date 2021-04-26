#pragma once

struct CL_Args
{
    bool debug;
    bool fullscreen;
};

CL_Args ParseCommandLineArguments(int argc, const char* argv[]);