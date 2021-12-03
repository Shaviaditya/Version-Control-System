# Kronos [VCS]

## Version Control System

This project is inspired from a similar project which was part of the popular 10 days of Code 2020, event organized by the GNU/LINUX USERS' GROUP, NIT Durgapur.

Link to repository : https://github.com/lugnitdgp/TDoC-Imperium

## Setup the project

Clone the project

```bash
https://github.com/Shaviaditya/Version-Control-System.git
```

Run the following command

```bash
cd scripts && ./build.sh

```

`build.sh` makes the project and creates an kronos folder in the `$HOME` folder. This folder will contain the compiled C++ file that is executed everytime an kronos command is called. The `kronos.sh` file is also copy pasted over to the folder for convenience. After the files are copied, the `kronos.sh` file is sourced to the `~/.bashrc` file so that its functions can be called from any directory.

## Test the commands

Create or move to an existing directory and type `kronos init`. This will initialize an kronos repository. Now you can play around with the commands listed below

```bash
kronos add [file/folder/.]
kronos commit "[message]"
kronos commit-log
kronos checkout [hash]
kronos status
```

Features currently under progress include :

```bash
kronos revert
```
