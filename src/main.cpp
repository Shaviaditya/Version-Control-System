#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <filesystem>
#include <unistd.h>
#include <string>
#include <fstream>
#include <stdbool.h>
#include <string.h>
#include <vector>
#include <ios>
namespace fs = std::filesystem;
std::string setPath;
// void addToCache(std::string addPath)
// {

// }
bool ignoreCheck(std::string pathFile){
    std::string getData;
    std::ifstream readData(".imperiumIgnore");
    while(getline(readData,getData))
    {
        if(getData==pathFile || pathFile.find(getData)!=std::string::npos){
            return true;
        }
    }
    return false;
}
bool toBeIgnored(std::string pathFile){
    std::string getData;
    std::ifstream readData("/.imperium/add.log");
    while(getline(readData,getData))
    {
        if(getData==pathFile){
            return true;
        }
    }
    return ignoreCheck(pathFile);
}
void add(std::vector<std::string> paths)
{
    std::string root = setPath;
    std::ofstream writeData;
    writeData.open(setPath+"/.imperium/add.log",std::ios_base::app);
    if(paths[0]==".")
    {
        for(auto& it: fs::recursive_directory_iterator(root))
        {
            std::string fileCheck = it.path();
            if(toBeIgnored(fileCheck)==false){
               writeData<<fileCheck<<"\n"; 
            }
        }
        writeData.close();

    } else {
        struct stat *check;
        if(stat((setPath+"/.imperium/add.log").c_str(),check)){
            std::string prev="";
            std::ifstream readFile(setPath+"/.imperium/add.log");
            std::cout<<"Previous Files are => \n";
            while(getline(readFile,prev))
            {
                std::cout<<prev<<"\n";
            }
        }
        for(auto it: paths){
            struct stat *buf;
            if(stat((setPath+"/"+it).c_str(),buf)){
                if(toBeIgnored(setPath+"/"+it)==false){
                    writeData<<setPath+"/"+it+"\n";
                }
            } else {
                std::cout<<"Wrong file chosen\n";
            }
        }
        writeData.close();;
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
    setPath = getpath;
    if(!strcmp("init",argv[1])){
        std::cout<<"\nProcessing....................\n";
        std::string ins_var = (getenv("dir"));
        init(ins_var);
    }
    else if(!strcmp("add",argv[1])){
        std::vector<std::string> argVars;
        for(int i=2;i<argc;i++){
            argVars.push_back(argv[i]);
        }
        add(argVars);
    }
    else{
        std::cout<<"Command entered is wrong\n";
    }
    return 0;
}