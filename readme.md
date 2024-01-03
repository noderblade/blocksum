# blocksum - Simple file or block device - per block SHA512 sum calculator

## Overview
bsum is a command-line utility for Linux that reads files in specified block sizes and calculates the SHA512 sum for each block. 
It offers options such as progress tracking and customizable block sizes.

## Installation

### Prerequisites
- OpenSSL
- GCC compiler or equivalent
- `make` utility

### Compiling the Utility
Clone the repository or download the source code. 
To compile the utility, navigate to the code directory and run:

make

### Arch linux install
Use any aur package manager - for example `yay`:

`yay -S bsum`

## Usage
Execute the utility from the command line, specifying the block size and the file or device to read.

./bsum [--progress] [--blocksize <size>] --device <file>

Options:
- `--progress`: Optional. Displays the progress percentage.
- `--blocksize <size>`: Sets the block size for reading. Specify the size in bytes, or use suffixes 'K', 'M', 'G' or 'T' for kilobytes, megabytes, or gigabytes, respectively. The default is 1G.
- `--device <file>`: Specifies the file or device to read.

### Examples
To read a file in 1MB blocks with progress displayed and safe output to output.sha512:

`./bsum --progress --blocksize 1M --device /path/to/file > output.sha512`

To use the default block size (1GB):

`./bsum --device /path/to/file`

## Contributing
Contributions to blocksum are welcome. Feel free to submit pull requests, open issues, or suggest improvements.

## License
This project is licensed under the MIT License - see the LICENSE file for details.

## Contact
For any inquiries or issues regarding this utility, please contact me at marcin.stragowski@gmail.com

