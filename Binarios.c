#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FAVORITOS 10

typedef struct {
    int id;
    char titulo[50];
    char categoria[30];
    int duracao;
} Video;

typedef struct {
    int id;
    char nome[50];
    int favoritos[MAX_FAVORITOS];
    int qtdFavoritos;
} Usuario;

void cadastrarVideo() {
    FILE *arq = fopen("videos.dat", "ab");

    if (arq == NULL) {
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    Video v;

    printf("ID do video: ");
    scanf("%d", &v.id);

    getchar();

    printf("Titulo: ");
    fgets(v.titulo, 50, stdin);
    v.titulo[strcspn(v.titulo, "\n")] = 0;

    printf("Categoria: ");
    fgets(v.categoria, 30, stdin);
    v.categoria[strcspn(v.categoria, "\n")] = 0;

    printf("Duracao (min): ");
    scanf("%d", &v.duracao);

    fwrite(&v, sizeof(Video), 1, arq);

    fclose(arq);

    printf("Video cadastrado!\n");
}

void listarVideos() {
    FILE *arq = fopen("videos.dat", "rb");

    if (arq == NULL) {
        printf("Nenhum video cadastrado.\n");
        return;
    }

    Video v;

    while (fread(&v, sizeof(Video), 1, arq)) {
        printf("\nID: %d\n", v.id);
        printf("Titulo: %s\n", v.titulo);
        printf("Categoria: %s\n", v.categoria);
        printf("Duracao: %d min\n", v.duracao);
    }

    fclose(arq);
}

void buscarVideoPorID(int id) {
    FILE *arq = fopen("videos.dat", "rb");

    if (arq == NULL) {
        printf("Arquivo inexistente.\n");
        return;
    }

    Video v;
    int encontrado = 0;

    while (fread(&v, sizeof(Video), 1, arq)) {
        if (v.id == id) {
            printf("\nVideo encontrado:\n");
            printf("Titulo: %s\n", v.titulo);
            printf("Categoria: %s\n", v.categoria);
            printf("Duracao: %d min\n", v.duracao);

            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Video nao encontrado.\n");
    }

    fclose(arq);
}

void cadastrarUsuario() {
    FILE *arq = fopen("usuarios.dat", "ab");

    if (arq == NULL) {
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    Usuario u;

    printf("ID do usuario: ");
    scanf("%d", &u.id);

    getchar();

    printf("Nome: ");
    fgets(u.nome, 50, stdin);
    u.nome[strcspn(u.nome, "\n")] = 0;

    printf("Quantidade de favoritos: ");
    scanf("%d", &u.qtdFavoritos);

    for (int i = 0; i < u.qtdFavoritos; i++) {
        printf("ID do video favorito %d: ", i + 1);
        scanf("%d", &u.favoritos[i]);
    }

    fwrite(&u, sizeof(Usuario), 1, arq);

    fclose(arq);

    printf("Usuario cadastrado!\n");
}

void listarUsuarios() {
    FILE *arq = fopen("usuarios.dat", "rb");

    if (arq == NULL) {
        printf("Nenhum usuario cadastrado.\n");
        return;
    }

    Usuario u;

    while (fread(&u, sizeof(Usuario), 1, arq)) {
        printf("\nID: %d\n", u.id);
        printf("Nome: %s\n", u.nome);

        printf("Favoritos: ");

        for (int i = 0; i < u.qtdFavoritos; i++) {
            printf("%d ", u.favoritos[i]);
        }

        printf("\n");
    }

    fclose(arq);
}

void relatorioUsuariosFavoritos() {
    FILE *arqUsuarios = fopen("usuarios.dat", "rb");
    FILE *arqVideos = fopen("videos.dat", "rb");

    if (arqUsuarios == NULL || arqVideos == NULL) {
        printf("Arquivos inexistentes.\n");
        return;
    }

    Usuario u;
    Video v;

    while (fread(&u, sizeof(Usuario), 1, arqUsuarios)) {

        printf("\nUsuario: %s\n", u.nome);
        printf("Videos favoritados:\n");

        for (int i = 0; i < u.qtdFavoritos; i++) {

            rewind(arqVideos);

            while (fread(&v, sizeof(Video), 1, arqVideos)) {

                if (v.id == u.favoritos[i]) {
                    printf("- %s\n", v.titulo);
                }
            }
        }
    }

    fclose(arqUsuarios);
    fclose(arqVideos);
}

void atualizarVideo() {
    FILE *arq = fopen("videos.dat", "rb+");

    if (arq == NULL) {
        printf("Arquivo inexistente.\n");
        return;
    }

    int id;
    printf("Digite o ID do video: ");
    scanf("%d", &id);

    Video v;

    while (fread(&v, sizeof(Video), 1, arq)) {

        if (v.id == id) {

            getchar();

            printf("Novo titulo: ");
            fgets(v.titulo, 50, stdin);
            v.titulo[strcspn(v.titulo, "\n")] = 0;

            printf("Nova categoria: ");
            fgets(v.categoria, 30, stdin);
            v.categoria[strcspn(v.categoria, "\n")] = 0;

            printf("Nova duracao: ");
            scanf("%d", &v.duracao);

            fseek(arq, -sizeof(Video), SEEK_CUR);

            fwrite(&v, sizeof(Video), 1, arq);

            printf("Video atualizado!\n");

            fclose(arq);
            return;
        }
    }

    printf("Video nao encontrado.\n");

    fclose(arq);
}

void removerVideo() {

    FILE *original = fopen("videos.dat", "rb");
    FILE *novo = fopen("temp.dat", "wb");

    if (original == NULL || novo == NULL) {
        printf("Erro nos arquivos.\n");
        return;
    }

    int id;
    printf("ID do video a remover: ");
    scanf("%d", &id);

    Video v;

    while (fread(&v, sizeof(Video), 1, original)) {

        if (v.id != id) {
            fwrite(&v, sizeof(Video), 1, novo);
        }
    }

    fclose(original);
    fclose(novo);

    remove("videos.dat");
    rename("temp.dat", "videos.dat");

    printf("Video removido!\n");
}

int main(void) {

    int op;

    do {

        printf("\n===== MENU =====\n");
        printf("1 - Cadastrar video\n");
        printf("2 - Listar videos\n");
        printf("3 - Buscar video por ID\n");
        printf("4 - Cadastrar usuario\n");
        printf("5 - Listar usuarios\n");
        printf("6 - Atualizar video\n");
        printf("7 - Remover video\n");
        printf("8 - Relatorio usuarios e favoritos\n");
        printf("0 - Sair\n");

        printf("Opcao: ");
        scanf("%d", &op);

        switch(op) {

            case 1:
                cadastrarVideo();
                break;

            case 2:
                listarVideos();
                break;

            case 3: {
                int id;
                printf("ID: ");
                scanf("%d", &id);
                buscarVideoPorID(id);
                break;
            }

            case 4:
                cadastrarUsuario();
                break;

            case 5:
                listarUsuarios();
                break;

            case 6:
                atualizarVideo();
                break;

            case 7:
                removerVideo();
                break;

            case 8:
                relatorioUsuariosFavoritos();
                break;

            case 0:
                printf("Encerrando...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while(op != 0);

    return 0;
}
