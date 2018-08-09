all:minisearch

minisearch: minisearch.o parser.o trie.o heap.o
	gcc minisearch.o parser.o trie.o heap.o -o minisearch -lm

minisearch.o : minisearch.c
		gcc -c -g minisearch.c

parser.o: parser.c
		gcc -c -g parser.c

trie.o: trie.c
		gcc -c -g trie.c

heap.o: heap.c
		gcc -c -g heap.c

clean:
		rm -f *.o minisearch
