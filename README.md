# A_Compiler

## Current State of the Project:
    - Scanner       (COMPLETED)
    - Symbol Table  (COMPLETED)
    - Parser        (ACTIVE)
    - Separate Debug ON/OFF: Scanner, Parser (ON DECK)

## Functionality:
    - 03/03/2021: 
        * Read source file
        * Recognize lexemes and generate a Token Stream
        * Prints stream to terminal

## Linux Build:
```bash
mkdir build
cd build
cmake ..
make
./ac <input_file>
```

## Windows Build:
```bash
md build
cd build
cmake .. -G"Unix Makefiles"
mingw3-make
./ac <input_file>
```

![](other/build_ac.gif)