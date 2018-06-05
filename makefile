compile:
	gcc -o saveBMP saveBMP.c utils.c -Wall
	gcc -o analisysProperty analisysProperty.c utils.c -Wall
	gcc -o binary binary.c utils.c -Wall
	gcc -o greyScale greyScale.c utils.c -Wall
	gcc -o loadBMP loadBMP.c utils.c -Wall
	gcc -o Lab2 main.c utils.c -Wall
