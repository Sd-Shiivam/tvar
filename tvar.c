#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATA 102400
#define VAR_NAME_LEN 8
#define MAX_VARS 100
#define TVAR_DB_FILE "/tmp/tvar.db"

typedef struct {
    char name[VAR_NAME_LEN];
    char data[MAX_DATA];
} Var;

static Var vars[MAX_VARS];
static int var_count = 0;

void list_vars() {
    if (var_count == 0) {
        return;
    }
    for (int i = 0; i < var_count; ++i) {
        printf("%s\n", vars[i].name);
    }
}


void load_vars() {
    FILE *f = fopen(TVAR_DB_FILE, "rb");
    if (f) {
        fread(&var_count, sizeof(var_count), 1, f);
        fread(vars, sizeof(Var), var_count, f);
        fclose(f);
    }
}

void save_vars() {
    FILE *f = fopen(TVAR_DB_FILE, "wb");
    if (f) {
        fwrite(&var_count, sizeof(var_count), 1, f);
        fwrite(vars, sizeof(Var), var_count, f);
        fclose(f);
    }
}

int save_data(const char *varname, const char *data) {
    if (var_count >= MAX_VARS) return -1;
    // Check for duplicate name
    for (int i = 0; i < var_count; ++i) {
        if (strncmp(vars[i].name, varname, VAR_NAME_LEN) == 0) {
            return -2; // duplicate
        }
    }
    strncpy(vars[var_count].name, varname, VAR_NAME_LEN);
    vars[var_count].name[VAR_NAME_LEN-1] = '\0';
    strncpy(vars[var_count].data, data, MAX_DATA);
    vars[var_count].data[MAX_DATA-1] = '\0';
    var_count++;
    save_vars();
    return 0;
}

int read_data(const char *varname) {
    for (int i = 0; i < var_count; ++i) {
        if (strncmp(vars[i].name, varname, VAR_NAME_LEN) == 0) {
            printf("%s\n", vars[i].data);
            return 0;
        }
    }
    return -1;
}


void gen_varname(char *varname, size_t len) {
    int num = 101 + var_count;
    snprintf(varname, len, "%d", num);
    varname[len-1] = '\0';
}

int main(int argc, char *argv[]) {
    load_vars();
    if (argc == 2 && strcmp(argv[1], "-l") == 0) {
        list_vars();
        return 0;
    }
    if (argc == 3 && strcmp(argv[1], "-r") == 0) {
        char varname[VAR_NAME_LEN];
        strncpy(varname, argv[2], VAR_NAME_LEN);
        varname[VAR_NAME_LEN-1] = '\0';
        if (read_data(varname) != 0) {
            return 1;
        }
        return 0;
    }
    if (argc == 3 && strcmp(argv[1], "-n") == 0) {
        char data[MAX_DATA];
        size_t len = fread(data, 1, sizeof(data)-1, stdin);
        data[len] = '\0';
        char varname[VAR_NAME_LEN];
        strncpy(varname, argv[2], VAR_NAME_LEN);
        varname[VAR_NAME_LEN-1] = '\0';
        int res = save_data(varname, data);
        if (res == -2) return 2; // duplicate name
        if (res != 0) return 1;
        printf("%s\n", varname);
        return 0;
    }

    char data[MAX_DATA];
    size_t len = fread(data, 1, sizeof(data)-1, stdin);
    data[len] = '\0';

    char varname[VAR_NAME_LEN];
    gen_varname(varname, VAR_NAME_LEN);

    if (save_data(varname, data) != 0) {
        return 1;
    }

    printf("%s\n", varname);
    return 0;
}
