#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructura para almacenar una línea
typedef struct LineNode {
    char *line;
    struct LineNode *next;
} LineNode;

// Función para agregar una línea a la lista enlazada
LineNode* add_line(LineNode *head, char *line) {
    LineNode *new_node = (LineNode *)malloc(sizeof(LineNode));
    if (new_node == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    new_node->line = strdup(line);
    if (new_node->line == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    new_node->next = head;
    return new_node;
}

// Función para liberar la memoria de la lista enlazada
void free_lines(LineNode *head) {
    LineNode *current = head;
    while (current != NULL) {
        LineNode *temp = current;
        current = current->next;
        free(temp->line);
        free(temp);
    }
}

// Función para leer líneas de longitud indefinida
ssize_t read_line(char **lineptr, size_t *n, FILE *stream) {
    if (*lineptr == NULL || *n == 0) {
        *n = 128; // Tamaño inicial del buffer
        *lineptr = malloc(*n);
        if (*lineptr == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
    }

    ssize_t num_chars = getline(lineptr, n, stream);
    if (num_chars == -1) {
        return -1; // Error o fin de archivo
    }

    return num_chars;
}

// Función principal
int main(int argc, char *argv[]) {
    FILE *input_file = stdin;
    FILE *output_file = stdout;
    LineNode *lines = NULL;
    char *buffer = NULL;
    size_t buffer_size = 0;

    // Manejo de argumentos
    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    if (argc >= 2) {
        input_file = fopen(argv[1], "r");
        if (input_file == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
    }

    if (argc == 3) {
        output_file = fopen(argv[2], "w");
        if (output_file == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
            exit(1);
        }

        if (strcmp(argv[1], argv[2]) == 0) {
            fprintf(stderr, "El archivo de entrada y salida deben diferir\n");
            exit(1);
        }
    }

    // Leer el archivo de entrada línea por línea y agregar a la lista
    while (read_line(&buffer, &buffer_size, input_file) != -1) {
        lines = add_line(lines, buffer);
    }

    // Imprimir las líneas en orden inverso
    LineNode *current = lines;
    while (current != NULL) {
        fprintf(output_file, "%s", current->line);
        current = current->next;
    }

    // Limpiar
    free_lines(lines);
    free(buffer); // Liberar el buffer
    if (input_file != stdin) fclose(input_file);
    if (output_file != stdout) fclose(output_file);

    return 0;
}

