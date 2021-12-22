src=../src
dist=../dist
outname=pc

testing:
	gcc -Wall -pedantic -g -O0 -o $(dist)/$(outname) $(src)/*.c

dist:
	gcc -O3 -o $(dist)/$(outname) $(src)/*.c

clean:
	rm -f $(dist)/$(outname)