#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <openssl/sha.h>
#include <filesystem>
#include <unistd.h>
#include <string>
#include <fstream>
#include <stdbool.h>
#include <string.h>
#include <vector>
#include <ios>
#include <utility>
#include <map>
#include <unordered_map>
#include <chrono>
#include <ctime>
#include <queue>
#include <algorithm>
namespace fs = std::filesystem;
std::string setPath;
void addToCache(std::string addPath, char fileType)
{
    struct stat p;
    if (stat((setPath + "/.imperium/.add").c_str(), &p) != 0)
        mkdir((setPath + "/.imperium/.add").c_str(), 0777);
    addPath = addPath.substr(1, addPath.length() - 1);
    if (fileType == 'f')
    {
        struct stat r;
        std::string src = setPath + "/" + addPath;
        std::string fileAdd, dest = setPath + "/.imperium/.add/" + addPath;
        if (addPath.find_last_of('/') != std::string::npos)
        {
            fileAdd = setPath + "/.imperium/.add/" + addPath.substr(0, addPath.find_last_of('/'));
            std::cout << fileAdd << "\n";
            if (stat(fileAdd.c_str(), &r) != 0)
            {
                fs::create_directories(fileAdd);
            }
        }
        fs::copy_file(src, dest, fs::copy_options::overwrite_existing);
    }
    else if (fileType == 'd')
    {
        struct stat r;
        std::string src = setPath + "/" + addPath;
        std::string fileAdd = setPath + "/.imperium/.add/" + addPath.substr(0, addPath.find_last_of('/'));
        std::string dest = setPath + "/.imperium/.add/" + addPath;
        if (stat(fileAdd.c_str(), &r) != 0)
        {
            fs::create_directories(fileAdd);
        }
    }
}

void addCommit(std::string addPath, char fileType, std::string commitHash)
{
    struct stat filecheck;
    std::string commitPath = setPath + "/.imperium/.commit";
    if (stat((commitPath + "/" + commitHash).c_str(), &filecheck) != 0)
        mkdir((commitPath + "/" + commitHash).c_str(), 0777);
    commitPath = commitPath + "/" + commitHash + "/";
    if (fileType == 'f')
    {
        struct stat resource;
        std::string src = setPath + "/.imperium/.add/" + addPath;
        std::string fileAdd, dest = commitPath + addPath;
        if (addPath.find_last_of('/') != std::string::npos)
        {
            fileAdd = commitPath + addPath.substr(0, addPath.find_last_of('/'));
            std::cout << fileAdd << "\n";
            if (stat(fileAdd.c_str(), &resource) != 0)
            {
                fs::create_directories(fileAdd);
            }
        }
        fs::copy_file(src, dest, fs::copy_options::overwrite_existing);
    }
    else
    {
        struct stat r;
        std::string src = setPath + "/.imperium/.add/" + addPath;
        std::string fileAdd = commitPath + addPath.substr(0, addPath.find_last_of('/')), dest = commitPath + addPath;
        if (stat(fileAdd.c_str(), &r) != 0)
        {
            fs::create_directories(fileAdd);
        }
    }
}

bool checkExistence(std::string filepath)
{
    std::string resrc = setPath + "/" + filepath;
    struct stat buffer;
    return (stat(resrc.c_str(), &buffer) == 0);
}

void addPrevCommit(std::string addPath, char fileType, std::string commitHash, std::string prevHash)
{
    if (checkExistence(addPath) && checkExistence(".imperium/.commit/" + prevHash + "/" + addPath))
    {
        struct stat checkExist;
        std::string commitPath = setPath + "/.imperium/.commit";
        if (stat((commitPath + "/" + commitHash).c_str(), &checkExist) != 0)
            mkdir((commitPath + "/" + commitHash).c_str(), 0777);
        if (fileType == 'f')
        {
            struct stat resource;
            std::string commitPath = setPath + "/.imperium/.commit/";
            std::string src = setPath + "/" + addPath;
            std::string fileAdd, dest = commitPath + commitHash + "/" + addPath;
            if (addPath.find_last_of('/') != std::string::npos)
            {
                fileAdd = commitPath + commitHash + "/" + addPath.substr(0, addPath.find_last_of('/'));
                // std::cout << fileAdd << "\n";
                if (stat(fileAdd.c_str(), &resource) != 0)
                {
                    fs::create_directories(fileAdd);
                }
            }
            fs::copy_file(src, dest, fs::copy_options::overwrite_existing);
        }
        else
        {
            struct stat r;
            std::string commitPath = setPath + "/.imperium/.commit/";
            std::string src = commitPath + prevHash + "/" + addPath;
            std::string fileAdd, dest = commitPath + commitHash + "/" + addPath;
            fileAdd = commitPath + commitHash + "/" + addPath.substr(0, addPath.find_last_of('/'));
            if (stat(fileAdd.c_str(), &r) != 0)
            {
                fs::create_directories(fileAdd);
            }
        }
    }
    else
    {
        std::cout << "The previous values have been modified\n";
    }
}

bool ignoreCheck(std::string pathFile)
{
    std::string getData;
    std::ifstream readData(".imperiumIgnore");
    while (getline(readData, getData))
    {
        if (pathFile.find(getData) != std::string::npos)
        {
            return true;
        }
    }
    return false;
}
bool toBeIgnored(std::string pathFile)
{
    std::string getData;
    std::ifstream readData(setPath + "/.imperium/add.log");
    while (getline(readData, getData))
    {
        if (pathFile.find(getData) != std::string::npos)
        {
            return true;
        }
    }
    return false;
}
bool fileDirException(std::string getPath)
{
    std::string getData;
    std::ifstream readData(setPath + "/.imperium/add.log");
    while (getline(readData, getData))
    {
        if ((getPath + "/").substr(0, (getData + '/').length()) == (getData + "/"))
        {
            return false;
        }
    }
    return true;
}
void add(std::vector<std::string> paths)
{
    std::string root = setPath;
    if (paths[0] == ".")
    {
        std::cout << "Relative Paths added include : "
                  << "\n";
        for (auto &it : fs::recursive_directory_iterator(root))
        {
            std::string fileCheck = it.path();
            std::string relativePath = "/" + fileCheck.substr(root.length() + 1, fileCheck.length() - (root.length() + 1));
            if (!ignoreCheck(fileCheck) && !toBeIgnored(fileCheck.substr(root.length() + 1, fileCheck.length() - (root.length() + 1))))
            {
                std::ofstream writeData;
                writeData.open(setPath + "/.imperium/add.log", std::ios_base::app);
                struct stat checkType;
                {
                    if (stat(fileCheck.c_str(), &checkType) == 0)
                    {
                        if (checkType.st_mode & S_IFDIR)
                        {
                            addToCache(relativePath, 'd');
                        }
                        else if (checkType.st_mode & S_IFREG)
                        {
                            addToCache(relativePath, 'f');
                        }
                    }
                    writeData << relativePath << "\n";
                }
                writeData.close();
            }
        }
    }
    else
    {
        std::cout << "Relative Paths added include : "
                  << "\n";
        for (auto it : paths)
        {
            struct stat checkType;
            {
                if (stat((setPath + "/" + it).c_str(), &checkType) == 0)
                {
                    if (checkType.st_mode & S_IFDIR)
                    {
                        for (auto &it2 : fs::recursive_directory_iterator(setPath + "/" + it))
                        {
                            std::string relpath = it2.path();
                            relpath = relpath.substr(setPath.length() + 1, relpath.length() - setPath.length() - 1);
                            struct stat checkType2;
                            {
                                if (stat((relpath).c_str(), &checkType2) == 0)
                                {
                                    if (checkType2.st_mode & S_IFDIR)
                                    {
                                        addToCache("/" + relpath, 'd');
                                    }
                                    else if (checkType2.st_mode & S_IFREG)
                                    {
                                        std::cout << "No error here"
                                                  << "\n";
                                        addToCache("/" + relpath, 'f');
                                    }
                                }
                            }
                        }
                    }
                    else if (checkType.st_mode & S_IFREG)
                    {
                        addToCache("/" + it, 'f');
                    }
                }
            }
            struct stat *buf;
            if (stat((setPath + "/" + it).c_str(), buf))
            {
                std::cout << setPath + "/" + it << "\n";

                if (ignoreCheck(setPath + "/" + it) == false)
                {
                    if (toBeIgnored(it) == false)
                    {
                        std::ofstream writeData;
                        writeData.open(setPath + "/.imperium/add.log", std::ios_base::app);
                        struct stat checkType;
                        {
                            if (stat((setPath + "/" + it).c_str(), &checkType) == 0)
                            {
                                if (checkType.st_mode & S_IFDIR)
                                {
                                    addToCache("/" + it, 'd');
                                }
                                else if (checkType.st_mode & S_IFREG)
                                {
                                    addToCache("/" + it, 'f');
                                }
                            }
                        }
                        writeData << "/" + it << "\n";
                        writeData.close();
                    }
                }
            }
            else
            {
                std::cout << "Wrong file chosen "
                          << "\n";
            }
        }
    }
}

void updateCommitLog(char const *message, std::string commitHash)
{
    std::string line;
    std::ifstream readData;
    readData.open(setPath + "/.imperium/.commit.log");
    int c = 0;
    std::vector<std::string> log_cont;
    while (getline(readData, line))
    {
        if (line.find("-->") == std::string::npos)
        {
            log_cont.push_back(line);
        }
        else
        {
            log_cont.push_back(line.substr(0, line.find("-->") - 1));
        }
    }
    readData.close();
    system(("rm -r " + setPath + "/.imperium/.commit.log").c_str());
    if (log_cont.size() == 0)
    {
        std::ofstream writeIn(setPath + "/.imperium/.commit.log");
        std::string msg = commitHash + " -- " + message + " --> HEAD";
        writeIn << msg << "\n";
        writeIn.close();
    }
    else
    {
        std::queue<std::string> stk;
        std::string currentMsg = commitHash + " -- " + message + " --> HEAD";
        stk.push(currentMsg);
        for (auto it : log_cont)
        {
            stk.push(it);
        }
        std::ofstream writeIn(setPath + "/.imperium/.commit.log");
        while (!stk.empty())
        {
            writeIn << stk.front() << "\n";
            stk.pop();
        }
        writeIn.close();
    }
}

void commit(int argc, char const *argv[])
{
    struct stat pathCheck;
    if (argv[3] == "")
    {
        std::cout << "No Description Found!"
                  << "\n";
    }
    else if (stat((setPath + "/.imperium").c_str(), &pathCheck) != 0)
    {
        std::cout << "No Imperium folder found, initialise one using [ imperium init ] "
                  << "\n";
    }
    else
    {
        /*
            This section creates a SHA1 encrypted hash to store a unique hash for each folder/file to be commited
        */
        auto timehash = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string ins_string = std::to_string(timehash).c_str();
        unsigned char in_buffer[40];
        for (int i = 0; i < ins_string.length(); i++)
        {
            in_buffer[i] = ins_string[i];
        }
        unsigned char out_buffer[20];
        SHA1(in_buffer, ins_string.length(), out_buffer);
        std::string ss;
        ss.resize(41);
        int use = 0;
        for (int i = 0; i < 20; i++)
        {
            use += sprintf(&ss[use], "%02x", out_buffer[i]);
        }
        ss.resize(use);
        std::string hashVar = ss;
        struct stat p;
        if (stat((setPath + "/.imperium/.commit").c_str(), &p) != 0)
        {
            /*
                This section handles first commit and creates respective folder in commit folder as well as
                the commit log part.
            */
            mkdir((setPath + "/.imperium/.commit").c_str(), 0777);
            for (auto &it : fs::recursive_directory_iterator(setPath + "/.imperium/.add"))
            {
                std::string fileTypeDef = it.path();
                std::string relativePath = fileTypeDef.substr((setPath + "./imperium/.add").length() + 1, fileTypeDef.length() - ((setPath + "./imperium/.add").length() + 1));
                struct stat checkType;
                {
                    if (stat(fileTypeDef.c_str(), &checkType) == 0)
                    {
                        if (checkType.st_mode & S_IFDIR)
                        {
                            addCommit(relativePath, 'd', hashVar);
                        }
                        else if (checkType.st_mode & S_IFREG)
                        {
                            addCommit(relativePath, 'f', hashVar);
                        }
                    }
                }
            }
            std::string rmstr = setPath + "/.imperium/";
            system(("rm -r " + rmstr + ".add").c_str());
            system(("rm -r " + rmstr + "add.log").c_str());
            updateCommitLog(argv[3], hashVar);
        }
        else
        {
            /*
                This section handles commits after 1st commit while creating new commit cehcking for files of previous commit
                mamnaging new/old commits.
            */
            std::string getData, commitPath1 = setPath + "/.imperium/.commit", getHash = "";
            std::ifstream readData(setPath + "/.imperium/.commit.log");
            while (getline(readData, getData))
            {
                getHash = getData.substr(0, getData.find("--") - 1);
                break;
            }
            readData.close();
            std::string prevCommitDir = commitPath1 + "/" + getHash;
            for (auto &it : fs::recursive_directory_iterator(prevCommitDir))
            {
                std::string fileTypeDef = it.path();
                std::string relativePath = fileTypeDef.substr((prevCommitDir).length() + 1, fileTypeDef.length() - ((prevCommitDir).length() + 1));
                struct stat checkType;
                {
                    if (stat(fileTypeDef.c_str(), &checkType) == 0)
                    {
                        if (checkType.st_mode & S_IFDIR)
                        {
                            addPrevCommit(relativePath, 'd', hashVar, getHash);
                        }
                        else if (checkType.st_mode & S_IFREG)
                        {
                            addPrevCommit(relativePath, 'f', hashVar, getHash);
                        }
                    }
                }
            }
            if (checkExistence("/.imperium/.add"))
            {
                for (auto &it : fs::recursive_directory_iterator(setPath + "/.imperium/.add"))
                {
                    std::string fileTypeDef = it.path();
                    std::string relativePath = fileTypeDef.substr((setPath + "./imperium/.add").length() + 1, fileTypeDef.length() - ((setPath + "./imperium/.add").length() + 1));
                    struct stat checkType;
                    {
                        if (stat(fileTypeDef.c_str(), &checkType) == 0)
                        {
                            if (checkType.st_mode & S_IFDIR)
                            {
                                addCommit(relativePath, 'd', hashVar);
                            }
                            else if (checkType.st_mode & S_IFREG)
                            {
                                addCommit(relativePath, 'f', hashVar);
                            }
                        }
                    }
                }
                std::string rmstr = setPath + "/.imperium/";
                system(("rm -r " + rmstr + ".add").c_str());
                // system(("rm -r " + rmstr + "add.log").c_str());
            }
            updateCommitLog(argv[3], hashVar);
        }
    }
}

void getCommitLog()
{
    std::ifstream readFile(setPath + "/.imperium/.commit.log");
    std::string paths;
    while (getline(readFile, paths))
    {
        std::cout << paths << "\n";
    }
    readFile.close();
}

void checkout(char const *argv[])
{

    std::string route = setPath + "/.imperium/.commit.log", check = "";
    bool dirExist = false;
    std::ifstream readFile(route);
    while (getline(readFile, check))
    {
        if (check.find(argv[2]) != std::string::npos)
        {
            dirExist = true;
            break;
        }
    }
    if (dirExist)
    {
        std::string src = setPath + "/.imperium/.commit/" + argv[2];
        /*
        for(auto &it : fs::recursive_directory_iterator(setPath)){
            if(ignoreCheck(it.path())==false){
                std::cout<<it.path()<<"\n";
                struct stat path;
                if(stat(it.path().c_str(),&path)==0){
                    if(path.st_mode & S_IFDIR){
                        std::string stp = it.path();
                        system(("rm -r "+stp).c_str());
                    } else {
                        fs::remove(it.path());
                    }
                }
            }
        }
        */
        for (auto &it : fs::recursive_directory_iterator(src))
        {
            std::string relPath = it.path();
            relPath = relPath.substr(src.length(), relPath.length() - (src.length()));
            struct stat checkType;
            {
                if (stat((setPath + relPath).c_str(), &checkType) == 0)
                {
                    if (checkType.st_mode & S_IFDIR)
                    {
                        fs::copy(it.path(), setPath + relPath, fs::copy_options::overwrite_existing);
                    }
                    else if (checkType.st_mode & S_IFREG)
                    {
                        fs::copy_file(it.path(), setPath + relPath, fs::copy_options::overwrite_existing);
                    }
                }
                else
                {
                    struct stat check2;
                    if (stat(it.path().c_str(), &check2) == 0)
                    {
                        if (check2.st_mode & S_IFDIR)
                            fs::create_directories((setPath + relPath));
                        else
                            fs::copy_file(it.path(), setPath + relPath, fs::copy_options::overwrite_existing);
                    }
                    // fs::copy(it.path(), setPath + relPath, fs::copy_options::overwrite_existing);
                }
            }
        }
    }
    else
    {
        std::cout << "No such commit exists."
                  << "\n";
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
                mkdir(".add", 0777);
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

int BUFFER_SIZE = 8192;

int compareFiles(std::string path1, std::string path2)
{

    std::ifstream lFile(path1.c_str(), std::ifstream::in | std::ifstream::binary);
    std::ifstream rFile(path2.c_str(), std::ifstream::in | std::ifstream::binary);

    if (!lFile.is_open() || !rFile.is_open())
    {
        return 1;
    }

    char *lBuffer = new char[BUFFER_SIZE]();
    char *rBuffer = new char[BUFFER_SIZE]();

    do
    {
        lFile.read(lBuffer, BUFFER_SIZE);
        rFile.read(rBuffer, BUFFER_SIZE);
        int numberOfRead = lFile.gcount(); // I check the files with the same size
        if (numberOfRead != rFile.gcount())
            return 1;

        if (memcmp(lBuffer, rBuffer, numberOfRead) != 0)
        {
            memset(lBuffer, 0, numberOfRead);
            memset(rBuffer, 0, numberOfRead);
            return 1;
        }
    } while (lFile.good() || rFile.good());

    delete[] lBuffer;
    delete[] rBuffer;

    return 0;
}

std::string getheadHash()
{
    std::string getData, getHash = "";
    std::ifstream readData(setPath + "/.imperium/.commit.log");
    while (getline(readData, getData))
    {
        getHash = getData.substr(0, getData.find("--") - 1);
        break;
    }
    readData.close();
    return getHash;
}

void status()
{
    std::vector<std::string> staged, unstaged;
    std::string headHash = getheadHash();
    if (checkExistence(".imperium/.add") && checkExistence(".imperium/.commit"))
    {
        if (checkExistence(".imperium/add.log"))
        {
            std::ifstream readFile(setPath + "/.imperium/add.log");
            std::string line;
            while (getline(readFile, line))
            {
                if (!checkExistence(".imperium/.commit/getHash/" + line))
                {
                    staged.push_back("created : ");
                    staged.push_back(line);
                }
                else
                {
                    staged.push_back("modified : ");
                    staged.push_back(line);
                }
                if (compareFiles(setPath + "/" + line, setPath + "/.imperium/.add" + line))
                {
                    unstaged.push_back("modified : ");
                    unstaged.push_back(line);
                }
            }
            readFile.close();
        }
    }
    else if (checkExistence(".imperium/.commit"))
    {
        for (auto it : fs::recursive_directory_iterator(setPath))
        {
            if (ignoreCheck(it.path()) && !toBeIgnored(it.path()))
            {
                continue;
            }
            std::string ins_path = it.path();
            std::string relpath = ins_path.substr(setPath.length() + 1, ins_path.length() - setPath.length() - 1);
            if(relpath.find(".imperium")!=std::string::npos){
                continue;
            }
            // std::cout << relpath << "\n";
            std::string commitPath = setPath + "/.imperium/.commit/" + headHash + "/" + relpath;
            // auto it2 =  std::find(staged.begin(), staged.end(), relpath);
            // std::cout<<setPath+"/"+".imperium/.commit/" + headHash + "/" + relpath<<"\n";
            if (!checkExistence(".imperium/.commit/" + headHash + "/" + relpath))
            {
                unstaged.push_back("created :");
                unstaged.push_back(relpath);
            }
            else
            {
                if (checkExistence(".imperium/.commit/" + headHash + "/" + relpath))
                {
                    if (compareFiles(it.path(), commitPath))
                    {
                        unstaged.push_back("modified : ");
                        unstaged.push_back(relpath);
                    }
                }
            }
        }
    }
    std::cout<<"Staged Changes : "<<"\n";
    for(int i=0;i<staged.size();i+=2){
        std::cout<<staged[i]<<" "<<staged[i+1]<<"\n";
    }
    std::cout<<"UnStaged Changes : "<<"\n";
    for(int i=0;i<unstaged.size();i+=2){
        std::cout<<unstaged[i]<<" "<<unstaged[i+1]<<"\n";
    }
    staged.clear();
    unstaged.clear();
}
int main(int argc, char const *argv[])
{
    chdir(getenv("dir"));
    std::string getpath = fs::current_path();
    setPath = getpath;
    if (!strcmp("init", argv[1]))
    {
        std::cout << "\nProcessing....................\n";
        std::string ins_var = (getenv("dir"));
        init(ins_var);
    }
    else if (!strcmp("add", argv[1]))
    {
        std::vector<std::string> argVars;
        for (int i = 2; i < argc; i++)
        {
            argVars.push_back(argv[i]);
        }
        add(argVars);
    }
    else if (!strcmp("commit", argv[1]) && !strcmp("-m", argv[2]))
    {
        commit(argc, argv);
    }
    else if (!strcmp("commit-logs", argv[1]))
    {
        getCommitLog();
    }
    else if (!strcmp("checkout", argv[1]))
    {
        checkout(argv);
    }
    else if (!strcmp("status", argv[1]))
    {
        status();
    }
    else
    {
        std::cout << "Command entered is wrong\n";
    }
    return 0;
}