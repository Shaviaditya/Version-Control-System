#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <filesystem>
#include <unistd.h>
#include <string>
#include <fstream>
#include <stdbool.h>
#include <string.h>
namespace fs = std::filesystem;
void add(std::string addPath,std::string reqPath)
{
    if(addPath==".")
    {
        std::cout<<fs::current_path()<<"\n";
        std::cout<<reqPath<<"\n";
    } 
}
void init(std::string path)
{
    struct stat *buf;
    const char *cstr = (path + "/.imperium").c_str();
    if (stat(cstr, buf))
    {
        int check = mkdir(".imperium", 0777);
        if (!check)
        {
            int checkdir = chdir("./.imperium");
            if (!checkdir)
            {
                std::ofstream add_file("add.log");
                std::ofstream commit_file(".commit.log");
                std::ofstream conflict("conflict");
                mkdir(".add",0777);
                conflict << "false\n";
                add_file.close();
                commit_file.close();
                conflict.close();
                chdir("..");
            }
        }
        if (access(".imperiumIgnore", F_OK))
        {
            std::ofstream add_ignore(".imperiumIgnore");
            add_ignore << "/.imperium"
                       << "\n";
            add_ignore << "/.imperiumIgnore"
                       << "\n";
            add_ignore << "/node_modules"
                       << "\n";
            add_ignore << "/.env"
                       << "\n";
            add_ignore.close();
        }
    }
    else
    {
        {
            printf("Folder exists\n");
            chdir("./imperium");
            if (access("add.log", F_OK) && access(".commit.log", F_OK))
            {
                std::ofstream add_file("add.log");
                std::ofstream commit_file(".commit.log");
                commit_file.close();
                add_file.close();
                chdir("..");
            }
            else
            {
                std::cout << "Logs exist\n";
            }
        }
    }
    printf("Repositiory Initialized\n");
}
int main(int argc, char const *argv[])
{
    chdir(getenv("dir"));
    std::string getpath = fs::current_path();
    if(!strcmp("init",argv[1])){
        std::cout<<"\nProcessing....................\n";
        std::string ins_var = (getenv("dir"));
        init(ins_var);
    }
    else if(!strcmp("add",argv[1])){
        std::string req_path = fs::current_path();
        add(argv[2],req_path);
    }
    else{
        std::cout<<"Command entered is wrong\n";
    }
    return 0;
}