# WORDER

Worder aims to be a framework for easily expanding wordlists, by using a
pipeline infrastructure.

Currently only the basic elements are defined and not the pipeline itself.

## Basic Elements

### Word
A wordlist element.

### Bucket

A bucket of words, with an usage interface that resemble a list. A bucket can
be for instance a file or an array. The file interface, however is still
uncomplete.

### Generator

A generator takes an input bucket and generates multiple buckets.

### Transformer

A transformer takes a word and returns a new word.

## Usage

Documentation is still in progress, see the demo for an example.

## Compilation

The project is managed by cmake. To compile library, test and demo just:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

Of course on windows you can use cmake-gui and generate your fancy Visual
Studio project. I haven't tested the code portability, though.
