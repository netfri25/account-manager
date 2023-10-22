#define NOB_IMPLEMENTATION
#include "nob.h"
#include <stdlib.h>

#define CC "gcc"
#define CFLAGS "-Wall", "-Wextra", "-pedantic"
#define BUILD_DIR "build"

// using X as the prefix
#define CLIENT_FILES X "main.c"
#define SERVER_FILES X "main.c"

int main(int argc, char** argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    Nob_Procs procs = {0};
    if (!nob_mkdir_if_not_exists(BUILD_DIR)) return EXIT_FAILURE;

    {
#define X "client/src/"
        Nob_Cmd cmd = {0};
        nob_cmd_append(&cmd, CC, CFLAGS);
        nob_cmd_append(&cmd, CLIENT_FILES, "-o", "build/client");
        nob_da_append(&procs, nob_cmd_run_async(cmd));
#undef X
    }

    {
#define X "server/src/"
        Nob_Cmd cmd = {0};
        nob_cmd_append(&cmd, CC, CFLAGS);
        nob_cmd_append(&cmd, SERVER_FILES, "-o", "build/server");
        nob_da_append(&procs, nob_cmd_run_async(cmd));
#undef X
    }


    if (!nob_procs_wait(procs)) return EXIT_FAILURE;
}
