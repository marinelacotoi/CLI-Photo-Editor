#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 300
#define UC unsigned char

typedef struct {
	char type[3];
	int width;
	int height;
	int max_value;
} image;

UC **alloc_matrix(int n, int m)
{	//functie de alocare dinamica a unei matrice
	UC **array = malloc(n * sizeof(unsigned char *));
	if (!array) {
		return NULL;
		printf("Eroare\n");
	}

	for (int i = 0; i < n; i++) {
		array[i] = malloc(m * sizeof(unsigned char));
		if (!array[i]) {
			printf("Eroare\n");
			for (int j = i - 1; j >= 0; j--)
				free(array[j]);

		free(array);
		return NULL;
		}
	}
	return array;
}

void dealoc_memory(int n, UC **array)
{	//functie de eliberare memorie
	for (int i = 0; i < n; i++)
		free(array[i]);

	free(array);
}

UC **matrix_ascii(image P[N], FILE *fin, int n, UC **array)
{
	array = alloc_matrix(P[0].height, n * P[0].width);

	for (int i = 0; i < P[0].height; i++)
		for (int j = 0; j < n * P[0].width; j++)
			fscanf(fin, "%hhu ", &array[i][j]);

	return array;
}

UC **matrix_binary(image P[N], FILE *fin, int n, UC **array)
{
	array = alloc_matrix(P[0].height, n * P[0].width);
	int a = fread(&a, sizeof(UC), 1, fin);

	for (int i = 0; i < P[0].height; i++)
		for (int j = 0; j < n * P[0].width; j++)
			fread(&array[i][j], sizeof(UC), 1, fin);

	return array;
}

UC **load_file(char comenzi[N], image P[N], UC **array)
{
	UC **m;
	char *filename;
	filename = strtok(comenzi, " ");
	filename = strtok(NULL, " ");

	FILE *fin;
	fin = fopen(filename, "rb");

	if (!fin) {
		printf("Failed to load %s\n", filename);
		return NULL;
	}

	printf("Loaded %s\n", filename);

	fscanf(fin, "%s", P[0].type);
	fscanf(fin, "%d", &P[0].width);
	fscanf(fin, "%d", &P[0].height);
	fscanf(fin, "%d", &P[0].max_value);

	if (strcmp("P2", P[0].type) == 0)
		m = matrix_ascii(P, fin, 1, array);
	if (strcmp("P3", P[0].type) == 0)
		m = matrix_ascii(P, fin, 3, array);
	if (strcmp("P5", P[0].type) == 0)
		m = matrix_binary(P, fin, 1, array);
	if (strcmp("P6", P[0].type) == 0)
		m = matrix_binary(P, fin, 3, array);

	fclose(fin);
	return m;
}

void print_ascii(image P[N], int n, UC **array, char *filename)
{
	FILE *fout;
	fout = fopen(filename, "w");
	if (!fout) {
		printf("Can't open file");
		return;
	}

	if (n == 1)
		fprintf(fout, "P2\n");
	if (n == 3)
		fprintf(fout, "P3\n");

	fprintf(fout, "%d %d\n", P[0].width, P[0].height);
	fprintf(fout, "%d\n", P[0].max_value);

	for (int i = 0; i < P[0].height; i++) {
		for (int j = 0; j < n * P[0].width; j++)

			fprintf(fout, "%hhu ", array[i][j]);

		fprintf(fout, "\n");
	}

	fclose(fout);
}

void print_binary(image P[N], int n, UC **array, char *filename)
{
	FILE *fout;
	fout = fopen(filename, "wb");
	if (!fout) {
		printf("Can't open file");
		return;
		}

	if (n == 1)
		fprintf(fout, "P5\n");
	if (n == 3)
		fprintf(fout, "P6\n");

	fprintf(fout, "%d %d\n", P[0].width, P[0].height);
	fprintf(fout, "%d\n", P[0].max_value);

	for (int i = 0; i < P[0].height; i++)
		for (int j = 0; j < n * P[0].width; j++)
			fwrite(&array[i][j], sizeof(UC), 1, fout);

	fclose(fout);
}

void save_file(char comenzi[N], UC **array, image P[N])
{
	char *filename = "z";
	char *ascii = "z";

	int nr = strlen(comenzi);

	filename = strtok(comenzi, " ");
	nr -= strlen(filename);
	filename = strtok(NULL, " ");
	nr -= strlen(filename);

	if (nr != 1)
		ascii = strtok(NULL, " ");
	//extrag parametrul ascii din comanda save, daca acesta exista

	if (ascii) {
		if (strcmp("P2", P[0].type) == 0 && strcmp("ascii", ascii) == 0)
			print_ascii(P, 1, array, filename);
		if (strcmp("P2", P[0].type) == 0 && strcmp("ascii", ascii) != 0)
			print_binary(P, 1, array, filename);

		if (strcmp("P3", P[0].type) == 0 && strcmp("ascii", ascii) == 0)
			print_ascii(P, 3, array, filename);
		if (strcmp("P3", P[0].type) == 0 && strcmp("ascii", ascii) != 0)
			print_binary(P, 3, array, filename);

		if (strcmp("P5", P[0].type) == 0 && strcmp("ascii", ascii) == 0)
			print_ascii(P, 1, array, filename);
		if (strcmp("P5", P[0].type) == 0 && strcmp("ascii", ascii) != 0)
			print_binary(P, 1, array, filename);

		if (strcmp("P6", P[0].type) == 0 && strcmp("ascii", ascii) == 0)
			print_ascii(P, 3, array, filename);
		if (strcmp("P6", P[0].type) == 0 && strcmp("ascii", ascii) != 0)
			print_binary(P, 3, array, filename);
	} else {
		if (strcmp("P2", P[0].type) == 0)
			print_binary(P, 1, array, filename);
		if (strcmp("P3", P[0].type) == 0)
			print_binary(P, 3, array, filename);
		if (strcmp("P5", P[0].type) == 0)
			print_binary(P, 1, array, filename);
		if (strcmp("P6", P[0].type) == 0)
			print_binary(P, 3, array, filename);
	}

	printf("Saved %s\n", filename);
}

void grayscale_filter(UC **array, image P[N], int x1, int y1, int x2, int y2)
{
	if (strcmp("P2", P[0].type) == 0) {
		printf("Grayscale filter not available\n");
		return;
	}

	if (strcmp("P5", P[0].type) == 0) {
		printf("Grayscale filter not available\n");
		return;
	}

	for (int i = y1; i < y2; i++) {
		for (int j = 3 * x1; j < 3 * x2; j = j + 3) {
			//∗ newRed = newGreen = newBlue = (Red + Greed + Blue) / 3
			int r = array[i][j];
			int g = array[i][j + 1];
			int b = array[i][j + 2];
			array[i][j] = (r + g + b) / 3;
			array[i][j + 1] = (r + g + b) / 3;
			array[i][j + 2] = (r + g + b) / 3;
			}
		}

	printf("Grayscale filter applied\n");
}

void sepia_filter(UC **array, image P[N], int x1, int y1, int x2, int y2)
{
	double x;

	if (strcmp("P2", P[0].type) == 0) {
		printf("Sepia filter not available\n");
		return;
	}

	if (strcmp("P5", P[0].type) == 0) {
		printf("Sepia filter not available\n");
		return;
	}

	for (int i = y1; i < y2; i++) {
		for (int j = 3 * x1; j < 3 * x2; j = j + 3) {
			int r = array[i][j];
			int g = array[i][j + 1];
			int b = array[i][j + 2];

			x = 0.393 * r + 0.769 * g + 0.189 * b;
			x = round(x);

			if (x <= P[0].max_value)
				array[i][j] = x;
			else
				array[i][j] = P[0].max_value;

			x = 0.349 * r + 0.686 * g + 0.168 * b;
			x = round(x);

			if (x <= P[0].max_value)
				array[i][j + 1] = x;
			else
				array[i][j + 1] = P[0].max_value;

			x = 0.272 * r + 0.534 * g + 0.131 * b;
			x = round(x);

			if (x <= P[0].max_value)
				array[i][j + 2] = x;
			else
				array[i][j + 2] = P[0].max_value;
		}
	}

	printf("Sepia filter applied\n");
}

void swap_pointers(int *p1, int *p2)
{//functie de interschimbare valori a doua variabile
	int aux;

	aux = *p1;
	*p1 = *p2;
	*p2 = aux;
}

UC **crop_file(image P[N], UC **array, int x1, int x2, int y1, int y2)
{
	int n;
	UC **array2;

	if (strcmp("P2", P[0].type) == 0)
		n = 1;
	if (strcmp("P3", P[0].type) == 0)
		n = 3;
	if (strcmp("P5", P[0].type) == 0)
		n = 1;
	if (strcmp("P6", P[0].type) == 0)
		n = 3;

	if (P[0].width == x2 - x1 && P[0].height == y2 - y1) {
		printf("Image cropped\n");
		return array;
	}

	array2 = alloc_matrix(y2 - y1, n * (x2 - x1));
	// alocam memorie pentru o matrice copie a matricei principale

	int b;
	int a = 0;
	for (int i = y1; i < y2; i++) {
		b = 0;
		for (int j = n * x1; j < n * x2; j++) {
			array2[a][b] = array[i][j];
			b++;
		}
		a++;
	}
	//copiem continutul selectiei din matricea principala in matricea copie

	dealoc_memory(P[0].height, array);

	P->width = x2 - x1;
	P->height = y2 - y1;

	array = alloc_matrix(P[0].height, n * P[0].width);
	if (!array) {
		return NULL;
		printf("Eroare\n");
	}
	//realocam memorie pentru matricea principala cu dimensiunile selectiei

	a = 0;
	for (int i = 0; i < P[0].height; i++) {
		b = 0;
		for (int j = 0; j < n * P[0].width; j++) {
			array[a][b] = array2[i][j];
			b++;
		}
		a++;
	}
	//copiem continutul selectiei in matricea principala

	dealoc_memory(P[0].height, array2);
	//eliberam memoria pentru matricea copie
	printf("Image cropped\n");
	return array;
}

int invalid_coordonates(image P[N], int x1, int x2, int y1, int y2)
{
	//functie ce valideaza coordonatele zona selectate
	int ok = 1;

	if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0)
		ok = 0;

	if (x1 > P[0].width || x2 > P[0].width)
		ok = 0;

	if (y1 > P[0].height || y2 > P[0].height)
		ok = 0;

	if (x1 == x2 || y1 == y2)
		ok = 0;

	return ok;
}

void rotate90(UC **array, int x1, int x2, int y1, int y2, int unghi)
{
	//functie de rotire cu un unghi 90/-90 pt o selectie patratica
	//se aplica imaginilor de tip P2/P5

	UC **array3 = alloc_matrix(y2 - y1, x2 - x1);

	int b;
	int a = 0;
	for (int i = y1; i < y2; i++) {
		b = 0;
		for (int j = x1; j < x2; j++) {
			array3[a][b] = array[i][j];
			b++;
		}
		a++;
	}

	if (unghi > 0) {
		for (int i = 0; i < y2 - y1; i++)
			for (int j = 0; j < x2 - x1; j++)
				array[y1 + i][x1 + j] = array3[y2 - y1 - 1 - j][i];
	}

	if (unghi < 0) {
		for (int i = 0; i < y2 - y1; i++)
			for (int j = 0; j < x2 - x1; j++)
				array[y1 + i][x1 + j] = array3[j][x2 - x1 - 1 - i];
	}

	dealoc_memory(y2 - y1, array3);
}

void rotate90color(UC **array, int x1, int x2, int y1, int y2, int unghi)
{
	//functie de rotire cu un unghi de 90/-90 pentru o imagine color
	UC **array3 = alloc_matrix(y2 - y1, 3 * (x2 - x1));

	int h = y2 - y1;
	int w = x2 - x1;

	int b;
	int a = 0;
	for (int i = y1; i < y2; i++) {
		b = 0;
		for (int j = 3 * x1; j < 3 * x2; j++) {
			array3[a][b] = array[i][j];
			b++;
		}
		a++;
	}

	if (unghi > 0) {
		b = 0;
		for (int i = 0; i < y2 - y1; i++) {
			a = 0;
			for (int j = 0; j < 3 * (x2 - x1); j = j + 3) {
				array[y1 + i][3 * x1 + j] = array3[h - 1 - a][b];
				array[y1 + i][3 * x1 + j + 1] = array3[h - 1 - a][b + 1];
				array[y1 + i][3 * x1 + j + 2] = array3[h - 1 - a][b + 2];
				a++;
			}
			b = b + 3;
		}
	}

	if (unghi < 0) {
		for (int i = 0; i < y2 - y1; i++) {
			a = 0;
			for (int j = 0; j < 3 * (x2 - x1); j = j + 3) {
				array[y1 + i][3 * x1 + j] = array3[a][3 * (w - i) - 3];
				array[y1 + i][3 * x1 + j + 1] = array3[a][3 * (w - i) - 2];
				array[y1 + i][3 * x1 + j + 2] = array3[a][3 * (w - i) - 1];
				a++;
			}
		}
	}

	dealoc_memory(y2 - y1, array3);
}

void rotate_file(char comenzi[N], UC **array, image P[N], int x1, int x2,
				 int y1, int y2)
{
	//functie de rotire pentru o sectiune patratica
	int ok = 0;
	int unghi = 0;
	char s[N] = " ";
	int n;

	if (strcmp("P2", P[0].type) == 0)
		n = 1;
	if (strcmp("P3", P[0].type) == 0)
		n = 3;
	if (strcmp("P5", P[0].type) == 0)
		n = 1;
	if (strcmp("P6", P[0].type) == 0)
		n = 3;

	if (x2 - x1 != y2 - y1) {
		printf("The selection must be square\n");
		return;
	}

	sscanf(comenzi, "%s %d", s, &unghi);

	if (unghi == 90 || unghi == -90)
		ok = 1;
	if (unghi == 180 || unghi == -180)
		ok = 1;
	if (unghi == 270 || unghi == -270)
		ok = 1;
	if (unghi == 360 || unghi == -360)
		ok = 1;
	if (unghi == 0 || unghi == -0)
		ok = 1;

	if (ok == 0) {
		printf("Unsupported rotation angle\n");
		return;
	}

	if (unghi == 90 || unghi == -90) {
		if (n == 1)
			rotate90(array, x1, x2, y1, y2, unghi);
		if (n == 3)
			rotate90color(array, x1, x2, y1, y2, unghi);
	}

	if (unghi == 180 || unghi == -180) {
		int a = 2;
		while (a) {
			if (n == 1)
				rotate90(array, x1, x2, y1, y2, unghi);
			if (n == 3)
				rotate90color(array, x1, x2, y1, y2, unghi);
			a--;
		}
	}

	if (unghi == 270 || unghi == -270) {
		int a = 3;
		while (a) {
			if (n == 1)
				rotate90(array, x1, x2, y1, y2, unghi);
			if (n == 3)
				rotate90color(array, x1, x2, y1, y2, unghi);
			a--;
		}
	}

	printf("Rotated %d\n", unghi);
}

void rotate180_image(UC **array, image P[N], int n)
{
	//functie de rotire cu un unghi de 180/-180 pentru intreaga imagine
	UC **array3 = alloc_matrix(P[0].height, n * P[0].width);
	int a = P[0].height;
	int b = P[0].width;

	for (int i = 0; i < P[0].height; i++)
		for (int j = 0; j < n * P[0].width; j++)
			array3[i][j] = array[i][j];

	if (n == 1) {
		for (int i = 0; i < P[0].height; i++)
			for (int j = 0; j < P[0].width; j++)
				array[i][j] = array3[a - 1 - i][b - 1 - j];
	}

	if (n == 3) {
		for (int i = 0; i < P[0].height; i++) {
			for (int j = 0; j < 3 * P[0].width; j = j + 3) {
				array[i][j] = array3[a - 1 - i][3 * b - 3 - j];
				array[i][j + 1] = array3[a - 1 - i][3 * b - 2 - j];
				array[i][j + 2] = array3[a - 1 - i][3 * b - 1 - j];
			}
		}
	}

	dealoc_memory(P[0].height, array3);
}

UC **rotate90_image(UC **array, image P[N], int unghi)
{
	//functie de rotire cu un unghi de 90/-90 pentru intreaga imagine
	//se aplica imaginilor de tip P2/P5
	int a = P[0].height;
	int b = P[0].width;

	UC **array3 = alloc_matrix(P[0].height, P[0].width);

	for (int i = 0; i < P[0].height; i++)
		for (int j = 0; j < P[0].width; j++)
			array3[i][j] = array[i][j];

	dealoc_memory(P[0].height, array);

	P->height = b;
	P->width = a;

	array = alloc_matrix(P[0].height, P[0].width);

	if (unghi > 0) {
		for (int i = 0; i < P[0].height; i++)
			for (int j = 0; j < P[0].width; j++)
				array[i][j] = array3[a - 1 - j][i];
	}

	if (unghi < 0) {
		for (int i = 0; i < P[0].height; i++)
			for (int j = 0; j < P[0].width; j++)
				array[i][j] = array3[j][b - 1 - i];
	}

	dealoc_memory(a, array3);
	return array;
}

UC **rotate90color_image(UC **array, image P[N], int unghi)
{
	//functie de rotire cu un unghi de 90/-90 pentru intreaga imagine
	//se aplica imaginilor colore (P3/P6)
	int a = P[0].height;
	int b = P[0].width;

	UC **array3 = alloc_matrix(P[0].height, 3 * P[0].width);

	for (int i = 0; i < P[0].height; i++)
		for (int j = 0; j < 3 * P[0].width; j++)
			array3[i][j] = array[i][j];

	dealoc_memory(P[0].height, array);

	P->height = b;
	P->width = a;

	array = alloc_matrix(P[0].height, 3 * P[0].width);
	int d;
	int c;

	if (unghi > 0) {
		d = 0;
		for (int i = 0; i < P[0].height; i++) {
			c = 0;
			for (int j = 0; j < 3 * P[0].width; j = j + 3) {
				array[i][j] = array3[a - 1 - c][d];
				array[i][j + 1] = array3[a - 1 - c][d + 1];
				array[i][j + 2] = array3[a - 1 - c][d + 2];
				c++;
			}
		d = d + 3;
		}
	}

	if (unghi < 0) {
		for (int i = 0; i < P[0].height; i++) {
			c = 0;
			for (int j = 0; j < 3 * P[0].width; j = j + 3) {
				array[i][j] = array3[c][3 * (b - i) - 3];
				array[i][j + 1] = array3[c][3 * (b - i) - 2];
				array[i][j + 2] = array3[c][3 * (b - i) - 1];
				c++;
			}
		}
	}

	dealoc_memory(a, array3);
	return array;
}

UC **rotate_full_image(char comenzi[N], UC **array, image P[N])
{
	//functie de rotire a intregii imagini
	int ok = 0;
	int unghi = 0;
	char s[N] = " ";
	int n;

	if (strcmp("P2", P[0].type) == 0)
		n = 1;
	if (strcmp("P3", P[0].type) == 0)
		n = 3;
	if (strcmp("P5", P[0].type) == 0)
		n = 1;
	if (strcmp("P6", P[0].type) == 0)
		n = 3;

	int check = sscanf(comenzi, "%s %d", s, &unghi);
	if (check != 2)
		return array;

	if (unghi == 90 || unghi == -90)
		ok = 1;
	if (unghi == 180 || unghi == -180)
		ok = 1;
	if (unghi == 270 || unghi == -270)
		ok = 1;
	if (unghi == 360 || unghi == -360)
		ok = 1;
	if (unghi == 0 || unghi == -0)
		ok = 1;
	if (ok == 0) {
		printf("Unsupported rotation angle\n");
		return array;
	}

	if (unghi == 90 || unghi == -90) {
		if (n == 1)
			array = rotate90_image(array, P, unghi);
		if (n == 3)
			array = rotate90color_image(array, P, unghi);
	}

	if (unghi == 180 || unghi == -180)
		rotate180_image(array, P, n);

	if (unghi == 270) {
		if (n == 1)
			array = rotate90_image(array, P, -90);
		if (n == 3)
			array = rotate90color_image(array, P, -90);
	}

	if (unghi == -270) {
		if (n == 1)
			array = rotate90_image(array, P, 90);
		if (n == 3)
			array = rotate90color_image(array, P, 90);
	}

	printf("Rotated %d\n", unghi);
	return array;
}

void exit_file(UC **array, image P[N])
{
	dealoc_memory(P[0].height, array);
}

UC **select_file(char comenzi[N], UC **array, image P[N], int v[1])
{	//functie ce selecteaza o portiune din imaginea principala
	int ok = 1;
	char *s = " ";
	char buffer[N] = " ";
	v[0] = 1;
	int x1 = -9;
	int x2 = -9;
	int y1 = -9;
	int y2 = -9;

	strcpy(buffer, comenzi);
	s = strtok(buffer, " ");
	s = strtok(NULL, " ");

	if (strcmp("ALL", s) != 0) {
		int check = sscanf(comenzi, "%s %d %d %d %d", s, &x1, &y1, &x2, &y2);
		if (check == 0)
			return array;

		if (x1 == -9 || y1 == -9 || y2 == -9 || x2 == -9) {
			printf("Invalid command\n");
			v[0] = 0;
			return array;
		}

		ok = invalid_coordonates(P, x1, x2, y1, y2);
		if (x1 > x2)
			swap_pointers(&x1, &x2);
		if (y1 > y2)
			swap_pointers(&y1, &y2);
		if (ok == 1)
			printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
		if (ok == 0) {
			printf("Invalid set of coordinates\n");
			v[0] = 0;
			return array;
		}

		while (ok == 1) {
			memset(comenzi, 0, N * sizeof(char));
			fgets(comenzi, N, stdin);
			if (strcmp("EXIT", comenzi) != 0)
				comenzi[strlen(comenzi) - 1] = '\0';
			char buffer2[N] = "S";
			strcpy(buffer2, comenzi);
			//se citesc comenzi si se efectueaza pe zona selectata
			if (strcmp("SELECT", strtok(buffer2, " ")) == 0) {
				array = select_file(comenzi, array, P, v);
				if (v[0] == 1)
					return array;
			}

			if (strcmp("ROTATE", strtok(buffer2, " ")) == 0) {
				if (x1 == 0 && y1 == 0 && x2 == P[0].width && y2 == P[0].height)
					return array;
			else
				rotate_file(comenzi, array, P, x1, x2, y1, y2);
			}

			if (strcmp("CROP", buffer2) == 0) {
				array = crop_file(P, array, x1, x2, y1, y2);
				return array;
			}
			if (strcmp("GRAYSCALE", buffer2) == 0)
				grayscale_filter(array, P, x1, y1, x2, y2);
			if (strcmp("SEPIA", buffer2) == 0)
				sepia_filter(array, P, x1, y1, x2, y2);
			if (strcmp("EXIT", buffer2) == 0)
				return array;
			if (strcmp("SAVE", strtok(buffer2, " ")) == 0)
				save_file(comenzi, array, P);
			if (strcmp("LOAD", strtok(buffer2, " ")) == 0)
				return array;
		}
	}
	if (strcmp("ALL", s) == 0) {
		printf("Selected ALL\n");
		return array;
	}
	return array;
}

int main(void)
{	int ok = 0;
	int v[1] = {0};
	image P[2] = {0};
	UC **array;
	char comenzi[N] = "S";

	while (strcmp("EXIT", strtok(comenzi, " ")) != 0) {
		memset(comenzi, 0, sizeof(comenzi));
		fgets(comenzi, N, stdin);
		if (strcmp("EXIT", comenzi) != 0)
			comenzi[strlen(comenzi) - 1] = '\0'; //sterg '\n'
		char buffer[N] = "S";
		strcpy(buffer, comenzi);

		if (strcmp("SELECT", strtok(buffer, " ")) == 0) {
			if (ok == 1)
				array = select_file(comenzi, array, P, v);
			if (ok != 1)
				printf("No image loaded\n");
			if (strcmp("CROP", comenzi) != 0)
				strcpy(buffer, comenzi);
		}

		if (strcmp("LOAD", strtok(buffer, " ")) == 0) {
			if (ok == 1)
				dealoc_memory(P[0].height, array);
			array = load_file(comenzi, P, array);
			if (array)
				ok = 1;
			else
				ok = 0;
		}

		else if (strcmp("SAVE", strtok(buffer, " ")) == 0) {
			if (ok == 1)
				save_file(comenzi, array, P);
			if (ok != 1)
				printf("No image loaded\n");
		}

		else if (strcmp("GRAYSCALE", strtok(buffer, " ")) == 0) {
			if (ok == 1)
				grayscale_filter(array, P, 0, 0, P[0].width, P[0].height);
			if (ok != 1)
				printf("No image loaded\n");
		} else if (strcmp("SEPIA", strtok(buffer, " ")) == 0) {
			if (ok == 1)
				sepia_filter(array, P, 0, 0, P[0].width, P[0].height);
			if (ok != 1)
				printf("No image loaded\n");
		}

		else if (strcmp("CROP", strtok(buffer, " ")) == 0) {
			if (ok == 1)
				printf("Image cropped\n");
			if (ok != 1)
				printf("No image loaded\n");
		} else if (strcmp("ROTATE", strtok(buffer, " ")) == 0) {
			if (ok == 1)
				array = rotate_full_image(comenzi, array, P);
			if (ok != 1)
				printf("No image loaded\n");
		}

		else if (strcmp("EXIT", strtok(buffer, " ")) == 0) {
			if (ok == 1)
				exit_file(array, P);
			if (ok != 1) {
				printf("No image loaded\n");
				break;
			}
		} else if (strcmp("SELECT", strtok(buffer, " ")) != 0) {
			printf("Invalid command\n");
		}
	}
	return 0;
}

