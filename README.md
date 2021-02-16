<h1 align="center">FlitDB</h1>

<p align="center"><i><b>Verb</b> ~ Move swiftly and lightly.</i></p>
</center>

FlitDB is teeny tiny database. Databases are part of every our everyday lives; but one issue tends to arise with databases. *What is that?* Simply put, it is: **Redundancy**. With this in mind, FlitDB sets out to eliminate this by removing all unnecessary values from the data that you store.

You might think that this would lead to your data being: lost, mangled, or a pain to read; but it doesn't have to be. FlitDB nixes the need for redundant nullified data, by introducing a variable skip amount between each value grouping; this allows FlitDB to have a broad scope of influence over thousands of different data points, whilst yielding a smaller file size, compared to other conventional database technologies.

Instructions are good, but are even better, when they can be utilized in the construction of a database; FlitDB uses instructions embedded into the actual database file, to aid with the reading, and writing of data on itself. The database does not include any `null` (or empty) values; this done to lower the total size of the database file. With a smaller file size the data can be read in a fast and timely manor.

## System support 🖥️
This is currently only tested on a Linux system; but support for other platforms is planned.

## Syntax 🗂️
Take a look at the API syntax used to handler and manage a database connection through FlitDB. A thorough explanation of each function, and their return values, are all documented and can be viewed in the [FlitDB syntax manual](SYNTAX.md).

## Demo 🛠️
For a demo of FlitDB in action, take a look at this [example program](demo) for more...

## Awards 🏆
 - Royal Hackaway v4 - Grand Prize 🥇

## Acknowledgements 🤝
This project would not have been developed without the additional support, and contributions of:

 - [Matt Dear](https://github.com/mattdear)
 - John Hawkins