#include <e.h>

void on_run(Editor_State *s)
{
    const char *t_name = "fu2ture";
    char *src_path = strf("src/%s.c", t_name);
    char *dylib_path = strf("bin/%s.dylib", t_name);
    
    const char *cc = "clang";
    const char *cflags = "-I/opt/homebrew/include -Ithird_party -DGL_SILENCE_DEPRECATION";
    const char *lflags = "-L/opt/homebrew/lib -lglfw -framework OpenGL";
    char *compile_command = strf("%s -g -dynamiclib %s %s %s -o %s", cc, cflags, lflags, src_path, dylib_path);
    
    printf("\nCompilation:\n%s\n\n", compile_command);
    int result = system(compile_command);
    printf("\nCompilation finished. Status: %d\n\n", result);
    
    free(compile_command);
    free(src_path);
    free(dylib_path);
}

