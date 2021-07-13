main.exe: main.cpp
	g++ -Wall -Wextra main.cpp Image.cpp -o main.exe

clean: 
	rm *.exe *.bmp
