Bernes Hut Algorithm with TBB
input is a textfile and the first line of that text file has the N of particles  and the second line is the length R of universe 

Compile with: make 
!!!!!!Warning!!!!
    with  make , it is only for macOS M1.
    For windows or Linux its another command

Running with: 

./tbb  inputfile.txt   how_many_times_to_run   outputfile.txt  threads 




./tbb  input5.txt 1 output.txt 1 
TIME ELAPSED 0.001892s 

./tbb  input5.txt 2  output.txt 2 
TIME ELAPSED 0.002030s 

 ./tbb  input5.txt 4   output.txt 4
TIME ELAPSED 0.002862s 
