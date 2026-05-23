#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FAV 10
#define VIDEO_FILE "videos.dat"
#define USER_FILE "usuarios.dat"

typedef struct {
    int id;
    char titulo[50];
    char genero[30];
} Video;

typedef struct {
    int id;
    char nome[50];
    int favoritos[MAX_FAV]; // Vetor de IDs de vídeos
    int qtd_fav;
} Usuario;

void menu();
void cadastrarVideo();
void cadastrarUsuario();
void listarUsuariosComVideos();
void atualizarVideo();
void removerVideo();
Video* buscarVideoPorId(int id);


void cadastrarVideo() {
    FILE *f = fopen(VIDEO_FILE, "ab");
    Video v;
    printf("ID do Video: "); scanf("%d", &v.id);
    printf("Titulo: "); scanf(" %[^\n]", v.titulo);
    printf("Genero: "); scanf(" %[^\n]", v.genero);
    fwrite(&v, sizeof(Video), 1, f);
    fclose(f);
}

void cadastrarUsuario() {
    FILE *f = fopen(USER_FILE, "ab");
    Usuario u;
    u.qtd_fav = 0;
    printf("ID do Usuario: "); scanf("%d", &u.id);
    printf("Nome: "); scanf(" %[^\n]", u.nome);
    
    int op, id_v;
    do {
        printf("Adicionar ID de video favorito? (1-Sim, 0-Nao): ");
        scanf("%d", &op);
        if(op == 1 && u.qtd_fav < MAX_FAV) {
            printf("ID do Video: "); scanf("%d", &id_v);
            u.favoritos[u.qtd_fav++] = id_v;
        }
    } while(op == 1 && u.qtd_fav < MAX_FAV);

    fwrite(&u, sizeof(Usuario), 1, f);
    fclose(f);
}

Video* buscarVideoPorId(int id) {
    FILE *f = fopen(VIDEO_FILE, "rb");
    if (!f) return NULL;
    Video *v = malloc(sizeof(Video));
    while(fread(v, sizeof(Video), 1, f)) {
        if(v->id == id) {
            fclose(f);
            return v;
        }
    }
    fclose(f);
    free(v);
    return NULL;
}

void listarUsuariosComVideos() {
    FILE *f = fopen(USER_FILE, "rb");
    if (!f) { printf("Nenhum usuario cadastrado.\n"); return; }
    Usuario u;
    printf("\n--- RELATORIO DE USUARIOS E FAVORITOS ---\n");
    while(fread(&u, sizeof(Usuario), 1, f)) {
        printf("Usuario: %s (ID: %d)\n", u.nome, u.id);
        printf("Favoritos: ");
        for(int i = 0; i < u.qtd_fav; i++) {
            Video *v = buscarVideoPorId(u.favoritos[i]);
            if(v) {
                printf("[%s] ", v->titulo);
                free(v);
            } else {
                printf("[ID %d nao encontrado] ", u.favoritos[i]);
            }
        }
        printf("\n------------------------------------------\n");
    }
    fclose(f);
}

void atualizarVideo() {
    FILE *f = fopen(VIDEO_FILE, "r+b");
    if (!f) return;
    int id;
    Video v;
    int achou = 0;
    printf("ID do video para atualizar: "); scanf("%d", &id);
    while(fread(&v, sizeof(Video), 1, f)) {
        if(v.id == id) {
            printf("Novo Titulo: "); scanf(" %[^\n]", v.titulo);
            fseek(f, -sizeof(Video), SEEK_CUR);
            fwrite(&v, sizeof(Video), 1, f);
            achou = 1;
            break;
        }
    }
    if(!achou) printf("Video nao encontrado.\n");
    fclose(f);
}

void removerVideo() {
    int id;
    printf("ID do video para remover: "); scanf("%d", &id);
    
    FILE *antigo = fopen(VIDEO_FILE, "rb");
    FILE *novo = fopen("temp.dat", "wb");
    if(!antigo) return;

    Video v;
    while(fread(&v, sizeof(Video), 1, antigo)) {
        if(v.id != id) {
            fwrite(&v, sizeof(Video), 1, novo);
        }
    }
    fclose(antigo);
    fclose(novo);
    remove(VIDEO_FILE);
    rename("temp.dat", VIDEO_FILE);
    printf("Operacao concluida.\n");
}

void menu() {
    int op;
    do {
        printf("\n1. Cadastrar Video\n2. Cadastrar Usuario\n3. Relatorio (Usuarios/Videos)\n");
        printf("4. Atualizar Video\n5. Remover Video\n0. Sair\nEscolha: ");
        scanf("%d", &op);
        switch(op) {
            case 1: cadastrarVideo(); break;
            case 2: cadastrarUsuario(); break;
            case 3: listarUsuariosComVideos(); break;
            case 4: atualizarVideo(); break;
            case 5: removerVideo(); break;
        }
    } while(op != 0);
}

int main(void) {
    menu();
    return 0;
}