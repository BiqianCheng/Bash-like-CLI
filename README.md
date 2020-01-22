# CS 100 Programming Project

## Introduction

This is the RShell project for CS 100 Winter 2020.

It is a simple shell program which will convert the command from user input to native linux command.

## Prototype and Research

The shell program is based on some linux system calls, such as fork(), waitpid() and execvp().

The fork() function creates a new process by duplicating the calling process. The new process is the child process, which is an almost duplicate of the calling process. The waitpid() function is used to wait for state changes in a child of the calling process, and obtain information about the child whose state has changed. The execvp() function provides an array of pointers to null-terminated string which contains the argument list for the new program.


## Class Descriptions

There's a fundamental class for all the other classes. And all the other classes is derived from the fundamental class. It's name is the Base class. Another important class is Connector class, which will be used as the parent class for different concrete classes. These concrete classes include the OrConnecter class, the AndConnector class and SeparatorConnetor class.


## UML Diagram

![alt text](https://github.com/cs100/assignment-swthursproj/blob/master/images/cs100UML.png?raw=true, "UML")


## Roadmap and Issues

The development of the project is from the bottom to the top. First step is try to parse the single command and then use the connector to operate the final results from individual command.

## Contributors
- Shiyou Wang
- Biqian Cheng
