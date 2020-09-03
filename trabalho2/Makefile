
all:
	gcc -olinear src/linear.c src/imageprocessing.c -Isrc/ -lfreeimage
	gcc -omulti_thread src/multi_thread.c src/imageprocessing.c -Isrc/ -lfreeimage -lpthread
	gcc -omulti_process src/multi_process.c src/imageprocessing.c -Isrc/ -lfreeimage
test: all
	./generate.sh

zip: test zipall clean2

zipall:
	zip -R trabalho2.zip Makefile *.c *.h *.odt *.sh *.md *.py *.jpg *pdf

clean:
	$(RM) ./linear
	$(RM) ./multi_process
	$(RM) ./multi_thread
	$(RM) out/*
	$(RM) trabalho2.zip
	$(RM) doc/*
	rm -r src/__pycache__

clean2:
	$(RM) ./linear
	$(RM) ./multi_process
	$(RM) ./multi_thread
	$(RM) out/*
	$(RM) doc/*
	rm -r src/__pycache__