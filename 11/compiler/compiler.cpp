#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include "compilation_engine.h"

std::string extract_file_name(std::string path) {
    int start_index = 0;
    int end_index = path.size() - 1;
    for (int i = path.size() - 1; i >= 0; --i) {
        if (path[i] == '.')
            end_index = i - 1;
        if (path[i] == '/') {
            start_index = i + 1;
            break;
        }
    }
    return path.substr(start_index, end_index - start_index + 1);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "./tokenizer_test <path>\n");
        return 1;
    }

    std::vector<std::string> paths;
    std::string output_name;
    struct stat st;
    if (stat(argv[1], &st) != 0)
        throw std::runtime_error("failed to open");
    if ((st.st_mode & S_IFMT) == S_IFDIR) {
        struct dirent *ent;
        DIR *pDir = opendir(argv[1]);
        std::string base_path = argv[1];
        while ((ent = readdir(pDir)) != NULL) {
            std::string name = ent->d_name;
            if (name.find(".jack") != std::string::npos)
                paths.push_back(base_path + "/" + name);
        }
        closedir(pDir);
    } else {
        paths.push_back(argv[1]);
    }

    for (int i = 0; i < paths.size(); ++i) {
        std::string name = extract_file_name(paths[i]);
        JackTokenizer tokenizer(paths[i].c_str());
        CompilationEngine compilation_engine(&tokenizer, (name + ".vm").c_str());
        compilation_engine.compileClass();
    }

    return 0;
}
