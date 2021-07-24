![FlitDB](./assets/banner.png?raw=true)
[![Version](https://img.shields.io/github/v/release/bradley499/flitdb.svg)](https://github.com/bradley499/flitdb/releases) [![C build](https://github.com/bradley499/flitdb/actions/workflows/c.yml/badge.svg)](https://github.com/bradley499/flitdb/actions/workflows/c.yml) [![GitHub license](https://img.shields.io/github/license/bradley499/flitdb.svg)](https://github.com/bradley499/flitdb/blob/master/LICENSE)


<p align="center"><i><b>Verb</b> ~ Move swiftly and lightly.</i></p>

***

FlitDB is a teeny tiny database - written in C. Databases are part of our everyday lives; but one issue tends to arise with databases. *What is that?* Simply put, it is: **Redundancy**. With this in mind, FlitDB sets out to eliminate this by removing all unnecessary values from the data that you store.

You might think that this would lead to your data being: lost, mangled, or a pain to read; but it doesn't have to be. FlitDB nixes the need for redundant nullified data, by introducing a variable skip amount between each value grouping; this allows FlitDB to have a broad scope of influence over thousands of different data points, whilst yielding a smaller file size, compared to other conventional database technologies.

Instructions are good, but are even better, when they can be utilized in the construction of a database; FlitDB uses instructions embedded into the actual database file, to aid with the reading, and writing of data on itself. The database does not include any `null` (or empty) values; this done to lower the total size of the database file. With a smaller file size the data can be read in a fast and timely manner.

## Comparison 📊
Here is the comparison in file sizes of different structured data storage formats against FlitDB. Each database contains an identical dataset (a list of numbers between `100000` and `1000000000`), each database format was tasked in storing `1000` different values in a single column; below are the results.

||FlitDB|SQLite3|JSON|
|-|-|-|-|
|**Database file Size**|***7004 bytes***|20480 bytes|9881 bytes|
|**Percentage increase in file size**||`192.404%` larger|`41.0765%` larger|

It is easily seen that a storing data within FlitDB results in a much smaller file size, compared to other structured data storage formats.

## How to use 🖇️
### Install as library
To install FlitDB as a shared library, simply navigate to the [`flitdb`](flitdb) directory and run the command `make`; this command will compile FlitDB into a shared library, and install it into the `/usr/lib` directory, along with required header files into the `/usr/include` directory. Then to access the library, all you need to do is `#include <`[`flit.h`](flitdb/flit.h)`>` into your project. To compile, all you need to do, is make sure that you compile with the parameter `-lflitdb` to include the shared FlitDB library.

### Compile into your project
Instead of [installing the FlitDB library](#Install-as-library) on your system, you can also compile the FlitDB source directly into your project. This is very easy to accomplish, as all you need to do, is copy the contents of the [`flitdb`](flitdb) directory into your project, and make sure that you include the file `#include "`[`flit.h`](flitdb/flit.h)`"` in your project.

## Syntax 🗂️
Take a look at the API syntax used to handle and manage a database connection through FlitDB. A thorough explanation of each function, and their return values, are all documented and can be viewed in the [FlitDB syntax manual](SYNTAX.md).

## Demo 🛠️
For a demo of FlitDB in action, take a look at this [example program](demo) for more...

## System support 💻
FlitDB is designed for UNIX based systems. However, certain provisions have been made to ensure that FlitDB is compatible with:
 - Linux
 - MacOS

## Awards 🏆
 - Royal Hackaway v4 - Grand Prize 🥇

## Acknowledgements 🤝
This project would not have been developed without the additional support, and contributions of:

 - [Matt Dear](https://github.com/mattdear)
 - Sky Hawkins
