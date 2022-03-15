# Data RefresheR

## Description
This program is designed to alleviate the bit rot data corruption by periodic rewriting the data on the storage.

## Precautions
It is strongly recommended to run this program only on a computer with ECC (Error Correction Code) RAM. Running this program
<<<<<<< HEAD
on a non-ECC platform may be counterproductive. If you don't understand this read more about bit rot nature before proceed. Be warned.
I have made every effort to make this program useful, bug-free and safe, however I do not take any responsibility for any potential loss
or damage caused by it.


## Usage
DataRefresher /path/to/directory/ [Options]
=======
on a non-ECC platform may be counterproductive. If you don't understand this please read more about bit rot nature before proceed. Be warned.

I have made every effort to make this program useful, bug-free and safe, however I do not take any responsibility for any potential loss
or damage caused by it.

## How it works?

1. Copy file to the temp file with .temp extension (if yes go to the next step).
2. Check if the checksum of the copied file is the same as the original one (if yes go to the next step).
3. Remove the original file (if ok go to the next step).
4. Rename the temp file to the original filename (if ok go to the next step).
5. Exit program (if ok the exit code: 0 else 1).

## Usage
	DataRefresher /path/to/directory/ [Options]
>>>>>>> 43cc51281b8715c4bcf17376722193521ce0be15

## Options

	 -R - recursive
	 -o (n) - Process only files older than (n) days. Default: 730 days.
	 -b (n) - Buffer size in Megabytes. Default: 128 MB 
	 
## Example
<<<<<<< HEAD
DataRefresher /path/to/directory/ -R -o 365 

=======
	DataRefresher /path/to/directory/ -R -o 365 
>>>>>>> 43cc51281b8715c4bcf17376722193521ce0be15
