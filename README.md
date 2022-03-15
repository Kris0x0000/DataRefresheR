# Data RefresheR

## Description
This program is designed to alleviate the bit rot data corruption by periodic rewriting the data on the storage.

## Precautions
It is strongly recommended to run this program only on a computer with ECC (Error Correction Code) RAM. Running this program
on a non-ECC platform may be counterproductive. If you don't understand this read more about bit rot nature before proceed. Be warned.
I have made every effort to make this program useful, bug-free and safe, however I do not take any responsibility for any potential loss
or damage caused by it.


## Usage
DataRefresher /path/to/directory/ [Options]

## Options

	 -R - recursive
	 -o (n) - Process only files older than (n) days. Default: 730 days.
	 -b (n) - Buffer size in Megabytes. Default: 128 MB 
	 
## Example
DataRefresher /path/to/directory/ -R -o 365 

